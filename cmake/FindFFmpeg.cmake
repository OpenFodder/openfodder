include(FindPackageHandleStandardArgs)

set(FFMPEG_ROOT_DIR "" CACHE PATH "Root directory of FFmpeg installation")

set(_ffmpeg_hint_paths "")
if (FFMPEG_ROOT_DIR)
    list(APPEND _ffmpeg_hint_paths "${FFMPEG_ROOT_DIR}")
endif()

find_path(FFMPEG_INCLUDE_DIR
    NAMES libavcodec/avcodec.h
    HINTS ${_ffmpeg_hint_paths}
    PATH_SUFFIXES include)

find_library(FFMPEG_AVCODEC_LIBRARY
    NAMES avcodec
    HINTS ${_ffmpeg_hint_paths}
    PATH_SUFFIXES lib)

find_library(FFMPEG_AVFORMAT_LIBRARY
    NAMES avformat
    HINTS ${_ffmpeg_hint_paths}
    PATH_SUFFIXES lib)

find_library(FFMPEG_AVUTIL_LIBRARY
    NAMES avutil
    HINTS ${_ffmpeg_hint_paths}
    PATH_SUFFIXES lib)

find_library(FFMPEG_SWSCALE_LIBRARY
    NAMES swscale
    HINTS ${_ffmpeg_hint_paths}
    PATH_SUFFIXES lib)

find_library(FFMPEG_SWRESAMPLE_LIBRARY
    NAMES swresample
    HINTS ${_ffmpeg_hint_paths}
    PATH_SUFFIXES lib)

set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIR})
set(FFMPEG_LIBRARIES
    ${FFMPEG_AVCODEC_LIBRARY}
    ${FFMPEG_AVFORMAT_LIBRARY}
    ${FFMPEG_AVUTIL_LIBRARY}
    ${FFMPEG_SWSCALE_LIBRARY}
    ${FFMPEG_SWRESAMPLE_LIBRARY})

find_package_handle_standard_args(
    FFmpeg
    REQUIRED_VARS
        FFMPEG_INCLUDE_DIR
        FFMPEG_AVCODEC_LIBRARY
        FFMPEG_AVFORMAT_LIBRARY
        FFMPEG_AVUTIL_LIBRARY
        FFMPEG_SWSCALE_LIBRARY
        FFMPEG_SWRESAMPLE_LIBRARY)
