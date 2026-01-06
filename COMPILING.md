# Open Fodder build instructions

This document describes how to build Open Fodder from source.

## Windows (Visual Studio 2022 + CMake)

Open Fodder uses CMake's FetchContent to download SDL3 and SDL3_mixer on Windows,
so a network connection and `git` in `PATH` are required for the first configure.

### Option A: Use the helper script (recommended)

The script locates a Visual Studio installation and builds the default `Release`
configuration.

```
Projects\build_vs.cmd
```

Optional arguments:

```
Projects\build_vs.cmd x64 Release
Projects\build_vs.cmd Win32 Release
Projects\build_vs.cmd x64 Debug
Projects\build_vs.cmd x64 Release noffmpeg
```

If `VCPKG_ROOT` (or `vcpkg` in `PATH`) points to a full vcpkg checkout with
`scripts/buildsystems/vcpkg.cmake`, the script runs `vcpkg install` in the
repo root and enables intro video playback by default. Use the `noffmpeg`
argument to skip FFmpeg. The manifest pins a minimal FFmpeg feature set
(avcodec/avformat/swresample/swscale).

The executable and runtime DLLs are copied to `Run\`.

### Option B: Run CMake manually

From the repo root:

```
cmake -S . -B Projects/VS -G "Visual Studio 17 2022" -A x64
cmake --build Projects/VS --config Release
```

For 32-bit builds, use:

```
cmake -S . -B Projects/VS -G "Visual Studio 17 2022" -A Win32 -DWITH_ASM=OFF
cmake --build Projects/VS --config Release
```

The executable and runtime DLLs are copied to `Run\`.

### FFmpeg intro video (optional)

Linux/macOS: CMake auto-detects FFmpeg by default. If FFmpeg is installed, the
intro video support is enabled; if not, it is skipped.

Windows: the helper script enables FFmpeg automatically when vcpkg is
available (use `noffmpeg` to skip it).

Manual enable (any platform):

```
-DOPENFODDER_ENABLE_FFMPEG=ON
```

Manual disable of auto-detect (non-Windows):

```
-DOPENFODDER_AUTO_FFMPEG=OFF
```

## Linux (Ubuntu example)

On Linux the build links against system-installed SDL3 and SDL3_mixer, so you
must install them in a way that provides CMake package config files.
The CI workflow builds and installs them from source:

```
sudo apt-get update
sudo apt-get install -y cmake ninja-build build-essential git pkg-config \
  libasound2-dev libpulse-dev libudev-dev libx11-dev libxext-dev \
  libxrandr-dev libxcursor-dev libxi-dev libxss-dev libxtst-dev \
  libwayland-dev libxkbcommon-dev libdrm-dev libgbm-dev \
  libegl1-mesa-dev libgl1-mesa-dev \
  libflac-dev libogg-dev libvorbis-dev libmpg123-dev libopusfile-dev \
  libxmp-dev

git clone --depth 1 --branch release-3.4.0 https://github.com/libsdl-org/SDL.git /tmp/SDL
cmake -S /tmp/SDL -B /tmp/SDL/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build /tmp/SDL/build
sudo cmake --install /tmp/SDL/build

git clone --depth 1 https://github.com/libsdl-org/SDL_mixer.git /tmp/SDL_mixer
cmake -S /tmp/SDL_mixer -B /tmp/SDL_mixer/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DSDLMIXER_VENDORED=OFF -DSDLMIXER_MOD_XMP=ON -DSDLMIXER_MOD_XMP_LITE=OFF -DSDLMIXER_MOD_XMP_SHARED=OFF
cmake --build /tmp/SDL_mixer/build
sudo cmake --install /tmp/SDL_mixer/build
```

Then configure and build:

```
cmake -S . -B build -G Ninja
cmake --build build
```

## Tests (optional)

The Windows CI test step uses the data and tests repositories and runs:

```
openfodder.exe --appveyor --unit-test-headless
```

You can reproduce this by placing `openfodder.exe` (and its SDL3 DLLs) alongside
the checked-out data and tests folders.
