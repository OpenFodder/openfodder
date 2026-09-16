# Open Fodder build instructions

This document describes how to build Open Fodder from source.

## Windows (Visual Studio 2022 or 2026 + CMake)

SDL3, SDL3_mixer and FFmpeg are installed by vcpkg during CMake configuration. The
registry and vcpkg tool are pinned to the revision in `vcpkg-configuration.json`.
The mixer retains MOD, FLAC and MP3 support through libxmp, libFLAC and mpg123;
WAV and Ogg Vorbis support are built in. Git and network access are
required on the first build; subsequent builds reuse the vcpkg binary cache.

### Option A: Use the helper script

From the repository root:

```powershell
Projects\build_vs.cmd
Projects\build_vs.cmd Win32 Release
Projects\build_vs.cmd x64 Debug
Projects\build_vs.cmd x64 Release noffmpeg # Explicitly omit MPEG intro playback
```

The helper bootstraps a project-local vcpkg checkout in `build/vcpkg-tool`,
selects the installed Visual Studio generator, and keeps separate build trees
for each architecture, configuration and video option. FFmpeg is enabled by
default. The explicit `noffmpeg` option produces a build without MPEG intro playback.

Output is in `Projects/VS-vcpkg-<platform>-<configuration>-<video>/bin/<configuration>/`.
For example, the default build produces
`Projects/VS-vcpkg-x64-Release-ffmpeg/bin/Release/openfodder.exe`.
Copy the executable and **every DLL in that directory** alongside the runtime
data in `Run/` to play. Include `libxmp.dll`: the mixer loads it dynamically,
so a missing copy can disable MOD music even when the executable starts.

### Option B: Run CMake manually

Use a new build directory when migrating from the old FetchContent build.
From PowerShell in the repository root (choose the generator matching your
installed Visual Studio version):

```powershell
./cmake/BootstrapVcpkg.ps1 -Root "$PWD/build/vcpkg-tool"
cmake -S . -B build/windows-x64 -G "Visual Studio 17 2022" -A x64 `
  "-DCMAKE_TOOLCHAIN_FILE=$PWD/build/vcpkg-tool/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows-release `
  -DOPENFODDER_ENABLE_FFMPEG=ON `
  "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$PWD/build/windows-x64/bin"
cmake --build build/windows-x64 --config Release
```

For 32-bit builds, use a separate build directory, `-A Win32`,
`-DVCPKG_TARGET_TRIPLET=x86-windows-release`, and `-DWITH_ASM=OFF`.
Do not run a separate `vcpkg install`: CMake installs the selected manifest
features automatically. The other source-built dependencies (libcurl,
libsodium and GGPO) retain their existing build paths.

### Dependency build times

The helper and Release CI use `x64-windows-release` or `x86-windows-release`.
These triplets set `VCPKG_BUILD_TYPE=release`, avoiding a second Debug build of
FFmpeg and every other dependency. For Debug builds, the helper uses the
standard `x64-windows` / `x86-windows` triplets in separate build directories.

Keep the vcpkg binary cache: repeated builds can restore compiled libraries.
CI saves that cache immediately after dependency configuration succeeds, even
if a later engine build or test fails. Cache keys include the manifest, pinned
registry, architecture, Visual Studio version, runner image, triplets and port overlays.
Older caches can supply unchanged packages and downloads; vcpkg checks package
compatibility before reuse.
The Windows CI and release jobs share cache keys, so a release can reuse the
same dependency packages built by CI on the same branch or default branch.

### FFmpeg intro video

FFmpeg is the only MPEG video decoder. The Windows helper and both release jobs
enable it. Manual CMake builds should pass `-DOPENFODDER_ENABLE_FFMPEG=ON`;
this also enables the manifest's `ffmpeg` feature. On Windows, the FFmpeg port
overlay builds only MPEG-1 video and MP2 audio decoding, MPEG program-stream
reading, local file access, and the video/audio conversion libraries. Encoders,
other codecs, network support, devices and hardware backends are disabled.
This matches `Run/Data/Intro.mpg`; music and sound formats handled by SDL_mixer
are unaffected. The Windows package must include all five FFmpeg runtime DLLs
from the build output directory. See `cmake/ports/ffmpeg/README.openfodder.md`.

Linux/macOS can auto-detect system FFmpeg, but explicit enabling makes missing
libraries a configuration error. An intentional build without MPEG playback
requires `-DOPENFODDER_ENABLE_FFMPEG=OFF` (and, on non-Windows platforms,
`-DOPENFODDER_AUTO_FFMPEG=OFF`). Use separate output directories for the two
variants to avoid retaining old DLLs.

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
  libxmp-dev libcurl4-openssl-dev \
  libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libswresample-dev

git clone --depth 1 --branch release-3.4.10 https://github.com/libsdl-org/SDL.git /tmp/SDL
cmake -S /tmp/SDL -B /tmp/SDL/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build /tmp/SDL/build
sudo cmake --install /tmp/SDL/build

git clone --depth 1 --branch release-3.2.4 https://github.com/libsdl-org/SDL_mixer.git /tmp/SDL_mixer
cmake -S /tmp/SDL_mixer -B /tmp/SDL_mixer/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DSDLMIXER_VENDORED=OFF -DSDLMIXER_MOD_XMP=ON -DSDLMIXER_MOD_XMP_LITE=OFF -DSDLMIXER_MOD_XMP_SHARED=OFF
cmake --build /tmp/SDL_mixer/build
sudo cmake --install /tmp/SDL_mixer/build
```

Then configure and build:

```
cmake -S . -B build -G Ninja -DOPENFODDER_ENABLE_FFMPEG=ON
cmake --build build
```

## Build support files

Commit `cmake/` together with `CMakeLists.txt`, the vcpkg manifests, build helpers
and workflow changes. These files are build inputs:

- `BootstrapVcpkg.ps1` installs the pinned vcpkg tool used locally and in CI.
- `triplets/` selects Release-only Windows dependency builds.
- `ports/ffmpeg/` overrides the standard vcpkg recipe to build only the intro's
  codecs. It includes the upstream recipe's license and applies no source patches.
- The remaining CMake modules, version-header template and GGPO patches support
  dependency discovery, version information and networking.

Downloaded sources, installed libraries, caches and compiled output are generated
under the ignored build directories.

## Tests (optional)

Both Windows architectures and Linux CI check FFmpeg decoding before packaging.
To run the same check locally, configure with `-DOPENFODDER_BUILD_MEDIA_TESTS=ON`
and `-DOPENFODDER_ENABLE_FFMPEG=ON`, build, then run:

```
ctest --test-dir <build-directory> -C Release --output-on-failure --no-tests=error
```

The synthetic MPEG fixture checks video decoding/conversion and audio
decoding/resampling without a display or audio device. See `Tests/VideoMPEG/README.md`.

The staged x64 Windows build can validate WAV, MOD, FLAC, MP3 and Ogg decoding without an
audio device (use a Python interpreter matching the build architecture):

```powershell
python Tests/AudioMixer/check.py --runtime Projects/VS-vcpkg-x64-Release-ffmpeg/bin/Release
```

This checks both streamed and predecoded audio and verifies non-silent PCM
output. It does not replace listening tests on a real audio device.

The Windows CI test step uses the data and tests repositories and runs:

```
openfodder.exe --appveyor --unit-test-headless
```

You can reproduce this by placing `openfodder.exe` (and all its runtime DLLs) alongside
the checked-out data and tests folders.
