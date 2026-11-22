"""Decode and mix audio fixtures using the DLLs in a staged Windows build.

Uses an offline mixer, so no sound device is required. Run with a Python
interpreter matching the build architecture (64-bit Python for x64).
"""
import argparse
import ctypes as ct
import math
import os
from pathlib import Path
import struct
import tempfile
import wave


class AudioSpec(ct.Structure):
    _fields_ = [("format", ct.c_uint32), ("channels", ct.c_int), ("freq", ct.c_int)]


def fixtures(directory):
    wav_path = directory / "tone.wav"
    with wave.open(str(wav_path), "wb") as stream:
        stream.setparams((1, 2, 22050, 0, "NONE", "not compressed"))
        stream.writeframes(b"".join(struct.pack("<h", int(12000 * math.sin(i * 2 * math.pi * 440 / 22050)))
                                    for i in range(11025)))
    # Minimal ProTracker module: one looping sample and one note in a 4-channel
    # pattern. Generated here so the check needs no retail game assets.
    mod = bytearray(1084 + 1024)
    mod[:20] = b"OpenFodder MOD check".ljust(20, b"\0")
    struct.pack_into(">H", mod, 42, 16)  # sample length, in 16-bit words
    mod[45] = 64                       # sample volume
    struct.pack_into(">H", mod, 48, 16)  # loop length
    mod[950] = 1                       # song length
    mod[1080:1084] = b"M.K."
    mod[1084:1088] = bytes((1, 0xAC, 0x10, 0))  # sample 1, period 428
    mod.extend(struct.pack("32b", *(int(100 * math.sin(i * 2 * math.pi / 32)) for i in range(32))))
    mod_path = directory / "tone.mod"
    mod_path.write_bytes(mod)
    return wav_path, mod_path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--expect-mod-failure", action="store_true",
                        help="Negative control for an isolated copy without libxmp.dll")
    args = parser.parse_args()
    runtime = args.runtime.resolve()
    fixture_dir = Path(__file__).resolve().parent / "fixtures"
    # SDL_LoadObject uses the normal Windows DLL search. Python's executable
    # lives elsewhere; use the staged folder as cwd to mirror the game's
    # adjacent-DLL lookup without preloading libxmp and hiding packaging bugs.
    os.chdir(runtime)
    with os.add_dll_directory(str(runtime)):
        sdl = ct.CDLL(str(runtime / "SDL3.dll"))
        mix = ct.CDLL(str(runtime / "SDL3_mixer.dll"))

        def bind(library, name, result, *arguments):
            fn = getattr(library, name)
            fn.restype, fn.argtypes = result, arguments
            return fn

        error = bind(sdl, "SDL_GetError", ct.c_char_p)
        init_sdl = bind(sdl, "SDL_Init", ct.c_bool, ct.c_uint32)
        quit_sdl = bind(sdl, "SDL_Quit", None)
        init_mix = bind(mix, "MIX_Init", ct.c_bool)
        quit_mix = bind(mix, "MIX_Quit", None)
        create_mixer = bind(mix, "MIX_CreateMixer", ct.c_void_p, ct.POINTER(AudioSpec))
        destroy_mixer = bind(mix, "MIX_DestroyMixer", None, ct.c_void_p)
        load = bind(mix, "MIX_LoadAudio", ct.c_void_p, ct.c_void_p, ct.c_char_p, ct.c_bool)
        destroy_audio = bind(mix, "MIX_DestroyAudio", None, ct.c_void_p)
        create_track = bind(mix, "MIX_CreateTrack", ct.c_void_p, ct.c_void_p)
        destroy_track = bind(mix, "MIX_DestroyTrack", None, ct.c_void_p)
        set_audio = bind(mix, "MIX_SetTrackAudio", ct.c_bool, ct.c_void_p, ct.c_void_p)
        play = bind(mix, "MIX_PlayTrack", ct.c_bool, ct.c_void_p, ct.c_uint32)
        generate = bind(mix, "MIX_Generate", ct.c_int, ct.c_void_p, ct.c_void_p, ct.c_int)

        def require(condition, message):
            if not condition:
                raise RuntimeError(f"{message}: {(error() or b'').decode(errors='replace')}")

        require(init_sdl(0), "SDL_Init failed")
        require(init_mix(), "MIX_Init failed")
        mixer = create_mixer(ct.byref(AudioSpec(0x8010, 2, 48000)))
        require(mixer, "Offline mixer creation failed")
        try:
            with tempfile.TemporaryDirectory(prefix="openfodder-audio-") as work:
                wav_path, mod_path = fixtures(Path(work))
                for path in (wav_path, mod_path, fixture_dir / "tone.flac",
                             fixture_dir / "tone.mp3", fixture_dir / "tone.ogg"):
                    for predecode in (False, True):
                        audio = load(mixer, os.fsencode(path), predecode)
                        if path == mod_path and args.expect_mod_failure:
                            if audio:
                                destroy_audio(audio)
                                raise RuntimeError("Negative control unexpectedly decoded MOD without libxmp")
                            print(f"PASS: MOD needs libxmp (predecode={predecode})")
                            continue
                        require(audio, f"Could not load {path.suffix} (predecode={predecode})")
                        track = create_track(mixer)
                        try:
                            require(track, "Could not create track")
                            require(set_audio(track, audio), "Could not assign audio")
                            require(play(track, 0), "Could not start track")
                            samples = (ct.c_int16 * 9600)()
                            generated = generate(mixer, samples, ct.sizeof(samples))
                            require(generated > 0, "No PCM generated")
                            peak = max(abs(value) for value in samples[:generated // 2])
                            require(peak > 0, "Generated audio is silent")
                            print(f"PASS: {path.suffix} predecode={predecode}, bytes={generated}, peak={peak}")
                        finally:
                            if track:
                                destroy_track(track)
                            destroy_audio(audio)
        finally:
            destroy_mixer(mixer)
            quit_mix()
            quit_sdl()


if __name__ == "__main__":
    main()
