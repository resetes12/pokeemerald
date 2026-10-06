#!/usr/bin/env bash

set -euo pipefail

repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
toolchain="$repo_dir/.toolchains/devkitARM-r62"
standalone=0
make_args=()

for arg in "$@"; do
    case "$arg" in
        --standalone)
            standalone=1
            ;;
        --help|-h)
            echo "Usage: $0 [--standalone] [make arguments...]"
            echo "  --standalone  Build without Lua/Soul Link startup gates"
            exit 0
            ;;
        *)
            make_args+=("$arg")
            ;;
    esac
done

if [[ ! -x "$toolchain/bin/arm-none-eabi-gcc" ]]; then
    echo "Missing devkitARM r62 at: $toolchain" >&2
    exit 1
fi

exec make -C "$repo_dir" -j"${JOBS:-4}" modern MODERN=1 \
    SOUL_LINK_STANDALONE="$standalone" TOOLCHAIN="$toolchain" "${make_args[@]}"
