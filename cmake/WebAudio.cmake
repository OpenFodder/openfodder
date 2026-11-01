# Build the SDL libraries together so the browser uses the same mixer API as
# desktop. All decoders are static: browsers cannot load native decoder DLLs.
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(web_sdl
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.2
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(web_sdl)

set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDLMIXER_DEPS_SHARED OFF CACHE BOOL "" FORCE)
set(SDLMIXER_STRICT ON CACHE BOOL "" FORCE)
set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDLMIXER_INSTALL OFF CACHE BOOL "" FORCE)
# VOC is used for CF1 effects, WAVE for external tracks, and libxmp for the
# PC Format demo's CANNON2.DAT music (a ProTracker module).
set(SDLMIXER_WAVE ON CACHE BOOL "" FORCE)
set(SDLMIXER_VOC ON CACHE BOOL "" FORCE)
set(SDLMIXER_MOD ON CACHE BOOL "" FORCE)
set(SDLMIXER_MOD_XMP ON CACHE BOOL "" FORCE)
set(SDLMIXER_MOD_XMP_SHARED OFF CACHE BOOL "" FORCE)
foreach(_web_decoder IN ITEMS AIFF AU FLAC GME MP3 MIDI OPUS VORBIS_STB VORBIS_VORBISFILE VORBIS_TREMOR WAVPACK)
    set(SDLMIXER_${_web_decoder} OFF CACHE BOOL "" FORCE)
endforeach()
FetchContent_Declare(web_sdl_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-3.2.4
    GIT_SHALLOW TRUE
    GIT_SUBMODULES external/libxmp
    GIT_SUBMODULES_RECURSE FALSE)
FetchContent_MakeAvailable(web_sdl_mixer)

# Keep the static dependencies' license notices beside the build for ZIP
# packaging, including when FetchContent source directories are overridden.
configure_file("${web_sdl_SOURCE_DIR}/LICENSE.txt"
    "${CMAKE_BINARY_DIR}/web-licenses/SDL3.txt" COPYONLY)
configure_file("${web_sdl_mixer_SOURCE_DIR}/LICENSE.txt"
    "${CMAKE_BINARY_DIR}/web-licenses/SDL3_mixer.txt" COPYONLY)
configure_file("${web_sdl_mixer_SOURCE_DIR}/external/libxmp/docs/COPYING"
    "${CMAKE_BINARY_DIR}/web-licenses/libxmp.txt" COPYONLY)
