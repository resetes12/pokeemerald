#!/usr/bin/env bash

set -euo pipefail

repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
toolchain="$repo_dir/.toolchains/devkitARM-r62"

if [[ ! -x "$toolchain/bin/arm-none-eabi-gcc" ]]; then
    echo "Missing devkitARM r62 at: $toolchain" >&2
    exit 1
fi

exec make -C "$repo_dir" -j"${JOBS:-4}" modern TOOLCHAIN="$toolchain" "$@"
