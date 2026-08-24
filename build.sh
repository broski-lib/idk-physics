#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

BUILD_DIR="${BUILD_DIR:-build}"

cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release "$@"
ln -sf "$BUILD_DIR/compile_commands.json" compile_commands.json
cmake --build "$BUILD_DIR" -j"$(nproc)"