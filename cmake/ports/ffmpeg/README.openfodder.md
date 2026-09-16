# OpenFodder FFmpeg overlay

This is based on the upstream `ports/ffmpeg` directory from microsoft/vcpkg commit
`a0500e27083be7e7aa344e537f2d4684fab5b86e` (FFmpeg 8.1.1, port revision 2).
Upstream port files are covered by `LICENSE.vcpkg.txt`.

Local changes add the Windows `mpeg-intro` feature in `vcpkg.json`, require that
feature in `portfile.cmake`, restrict the enabled components before `Building
Options`, and explicitly select MSVC as the host compiler on MSVC builds.
The rest of the upstream build recipe is retained to preserve its toolchain,
installation and dependency handling.

The actual `Run/Data/Intro.mpg` uses MPEG-1 video (352x288) and MP2 audio
(44.1 kHz stereo) in an MPEG program stream. The feature keeps only:

- `mpeg1video` and `mp2` decoders;
- `mpegvideo` and `mpegaudio` parsers;
- `mpegps` demuxer, `mpegvideo` demuxer for stream probing, and local `file` protocol;
- libavcodec, libavformat, libavutil, libswscale and libswresample.

Encoders, muxers, filters, devices, network protocols, hardware backends and
other codecs are disabled. SIMD optimizations remain enabled. SDL_mixer's
music and sound decoders are separate and unaffected.

## Build configuration

This overlay applies no FFmpeg source patches. `--host-cc=cl.exe` supplies the
host C11 compiler instead of bypassing its configure check when GCC is absent.
The selected components do not use the `bin2c` generator, so its separate host
dependency is also removed. The broader upstream port's patches are unnecessary
for this configuration. The overlay rejects other feature combinations because
those broader configurations are no longer maintained.

When updating vcpkg/FFmpeg, compare this directory with the new pinned upstream
port, review changes to its build requirements, and run the media checks. CI cache keys include
the overlay files. Linux CI uses prebuilt distribution FFmpeg libraries, so it
does not compile FFmpeg or use this Windows feature.
