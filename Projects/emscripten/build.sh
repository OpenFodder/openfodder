#!/usr/bin/env bash
set -euo pipefail
root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)"
# Keep compiler intermediates on WSL's Linux filesystem for faster builds.
build_dir="${OPENFODDER_WEB_BUILD_DIR:-${XDG_CACHE_HOME:-$HOME/.cache}/openfodder/web-build}"
sdk_dir="${EMSDK:-${XDG_CACHE_HOME:-$HOME/.cache}/openfodder/emsdk}"
if ! command -v emcmake >/dev/null 2>&1; then
    if [[ ! -f "$sdk_dir/emsdk_env.sh" ]]; then
        echo "Install Emscripten 6.0.12 in $sdk_dir, or activate an SDK before running this script." >&2
        exit 1
    fi
    EMSDK_QUIET=1 source "$sdk_dir/emsdk_env.sh"
fi
emcmake cmake -S "$root" -B "$build_dir" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release -DOPENFODDER_ENABLE_NETWORK=OFF \
    -DOPENFODDER_WEB_RETAIL_DATA=OFF \
    -DOPENFODDER_ENABLE_FFMPEG=OFF -DOPENFODDER_AUTO_FFMPEG=OFF "$@"
cmake --build "$build_dir" --parallel "${CMAKE_BUILD_PARALLEL_LEVEL:-8}"
echo "Built $root/RunE/OpenFodder.html"
