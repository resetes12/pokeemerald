#!/usr/bin/env python3.12

import argparse
import asyncio
import contextlib
import platform
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


MAX_FRAME_SIZE = 64 * 1024
HELLO_TIMEOUT_SECONDS = 600
MAX_CLIENTS = 3
ROLES = {"host", "client"}


def is_wsl() -> bool:
    return sys.platform == "linux" and "microsoft" in platform.release().lower()


def windows_path(path: str) -> str:
    if len(path) >= 3 and path[1] == ":" and path[2] in "\\/":
        return path
    return subprocess.run(
        ["wslpath", "-w", path], check=True, capture_output=True, text=True
    ).stdout.strip()


def wsl_path(path: str) -> str:
    return subprocess.run(
        ["wslpath", "-u", path], check=True, capture_output=True, text=True
    ).stdout.strip()


def run_on_windows(args: argparse.Namespace) -> int:
    launcher = shutil.which("py.exe")
    if launcher is None:
        raise RuntimeError("Windows Python launcher py.exe is unavailable through WSL")

    interpreter = subprocess.run(
        [launcher, "-3.12", "-c", "import sys; print(sys.executable)"],
        check=True, capture_output=True, text=True,
    ).stdout.strip()
    command = [
        wsl_path(interpreter),
        "-u",
        windows_path(str(Path(__file__).resolve())),
        "--bind",
        args.bind,
        "--port",
        str(args.port),
    ]
    if args.emuhawk:
        command.extend(("--emuhawk", windows_path(args.emuhawk)))

    print("[Bridge] WSL detected; starting the bridge with Windows Python")
    process = subprocess.Popen(command, stdin=subprocess.DEVNULL)
    try:
        return process.wait()
    except KeyboardInterrupt:
        print("\n[Bridge] stopping Windows bridge")
        if process.poll() is None:
            process.terminate()
        return process.wait()


async def read_frame(reader: asyncio.StreamReader) -> bytes:
    prefix = await reader.readuntil(b" ")
    length_text = prefix[:-1]
    if len(length_text) > 10 or not length_text.isdigit():
        raise ValueError("invalid frame length")

    length = int(length_text)
    if length < 1 or length > MAX_FRAME_SIZE:
        raise ValueError(f"frame length {length} is outside the allowed range")
    return await reader.readexactly(length)


async def write_frame(writer: asyncio.StreamWriter, payload: bytes) -> None:
    writer.write(str(len(payload)).encode("ascii") + b" " + payload)
    await writer.drain()


def parse_message(payload: bytes) -> tuple[str, str]:
    parts = payload.decode("utf-8").split("|", 4)
    if len(parts) != 5 or parts[0] != "SL1" or parts[2] not in ROLES:
        raise ValueError("invalid Soul Link message")
    return parts[2], parts[3]


@dataclass(eq=False, slots=True)
class Peer:
    reader: asyncio.StreamReader
    writer: asyncio.StreamWriter
    role: str | None = None
    client_id: int | None = None
    hello: bytes | None = None

    @property
    def address(self) -> object:
        return self.writer.get_extra_info("peername")

    @property
    def name(self) -> str:
        if self.role == "client":
            return f"client {self.client_id}"
        return self.role or "unregistered peer"


class SoulLinkRelay:
    def __init__(self) -> None:
        self.host: Peer | None = None
        self.clients: dict[int, Peer] = {}

    async def drop(self, peer: Peer) -> None:
        if peer.role == "host" and self.host is peer:
            self.host = None
            print(f"[Bridge] host disconnected: {peer.address}")
        elif (peer.role == "client" and peer.client_id is not None
              and self.clients.get(peer.client_id) is peer):
            del self.clients[peer.client_id]
            print(f"[Bridge] {peer.name} disconnected: {peer.address}")
        peer.writer.close()
        with contextlib.suppress(ConnectionError, OSError):
            await peer.writer.wait_closed()

    async def register(self, peer: Peer, payload: bytes) -> None:
        role, message_type = parse_message(payload)
        if message_type != "HELLO":
            raise ValueError("first message must be HELLO")

        peer.role = role
        peer.hello = payload
        if role == "host":
            if self.host is not None:
                raise ValueError("a host is already connected")
            self.host = peer
            print(f"[Bridge] host registered: {peer.address}")
            for client in list(self.clients.values()):
                await self.pair(peer, client)
        else:
            client_id = next(
                (candidate for candidate in range(1, MAX_CLIENTS + 1)
                 if candidate not in self.clients),
                None,
            )
            if client_id is None:
                raise ValueError("lobby already has three clients")
            peer.client_id = client_id
            self.clients[client_id] = peer
            print(f"[Bridge] {peer.name} registered: {peer.address}")
            if self.host is not None:
                await self.pair(self.host, peer)

    async def pair(self, host: Peer, client: Peer) -> None:
        await self.forward(host, client.hello or b"")
        await self.forward(client, host.hello or b"")
        print(f"[Bridge] host and {client.name} are paired")

    async def forward(self, target: Peer, payload: bytes) -> None:
        try:
            await write_frame(target.writer, payload)
        except (ConnectionError, OSError):
            await self.drop(target)

    async def handle(self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter) -> None:
        peer = Peer(reader, writer)
        print(f"[Bridge] connection opened: {peer.address}")
        try:
            first = await asyncio.wait_for(read_frame(reader), timeout=HELLO_TIMEOUT_SECONDS)
            await self.register(peer, first)

            while True:
                payload = await read_frame(reader)
                sender, _ = parse_message(payload)
                if sender != peer.role:
                    raise ValueError("message sender does not match registered role")

                if peer.role == "host":
                    for client in list(self.clients.values()):
                        await self.forward(client, payload)
                elif self.host is not None:
                    await self.forward(self.host, payload)
        except asyncio.TimeoutError:
            print(f"[Bridge] HELLO timeout: {peer.address}")
        except (asyncio.IncompleteReadError, ConnectionError):
            pass
        except (OSError, ValueError) as error:
            print(f"[Bridge] closing {peer.address}: {error}")
        finally:
            await self.drop(peer)


async def run(bind: str, port: int, emuhawk: str | None) -> None:
    relay = SoulLinkRelay()
    server = await asyncio.start_server(relay.handle, bind, port)
    addresses = ", ".join(str(sock.getsockname()) for sock in server.sockets or ())
    print(f"[Bridge] listening on {addresses}")
    if emuhawk:
        process = subprocess.Popen(
            [emuhawk, "--socket-ip=127.0.0.1", f"--socket-port={port}"]
        )
        print(f"[Bridge] launched native EmuHawk (pid={process.pid})")
    async with server:
        await server.serve_forever()


def main() -> None:
    parser = argparse.ArgumentParser(description="Modern Emerald Soul Link host relay")
    parser.add_argument("--bind", default="0.0.0.0")
    parser.add_argument("--port", type=int, default=7777)
    parser.add_argument("--emuhawk", help="path to EmuHawk.exe to launch after listening")
    args = parser.parse_args()
    if is_wsl():
        raise SystemExit(run_on_windows(args))
    if args.emuhawk and sys.platform != "win32":
        parser.error("--emuhawk requires Windows or WSL")
    try:
        asyncio.run(run(args.bind, args.port, args.emuhawk))
    except KeyboardInterrupt:
        print("\n[Bridge] stopped")


if __name__ == "__main__":
    main()
