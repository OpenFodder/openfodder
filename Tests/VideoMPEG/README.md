# MPEG video and audio check

Configure with `-DOPENFODDER_ENABLE_FFMPEG=ON -DOPENFODDER_BUILD_MEDIA_TESTS=ON`,
build, then run `ctest --test-dir <build-directory> -C Release --output-on-failure`.
The check runs on Windows and Linux without a window, sound device, or game data.

It demuxes a synthetic MPEG program stream, decodes ten MPEG-1 video frames and
MP2 audio, converts video to BGRA and audio to stereo signed 16-bit PCM, and
requires visible pixels and non-silent samples. This exercises all five FFmpeg
libraries used by the engine. Windows runs against the DLLs in the build output.
It does not exercise the game's rendering, timing, or physical audio output.
Windows vcpkg builds additionally require exactly the two intended decoders,
the MPEG program-stream and video-probing demuxers, no muxers/encoders, and only
local file access.

To check the complete retail intro locally, run
`openfodder_video_check Run/Data/Intro.mpg --intro`. This verifies the full
352x288 stream and its audio through EOF. The retail file is not included in
the test fixtures or uploaded by CI.

`fixtures/test.mpg` contains generated colour bars and a 440 Hz tone, with no game
assets. It was generated using:

```
ffmpeg -f lavfi -i testsrc=size=32x32:rate=25 -f lavfi -i sine=frequency=440:sample_rate=44100 -t 0.4 -c:v mpeg1video -b:v 200k -c:a mp2 -b:a 64k -f mpeg test.mpg
```
