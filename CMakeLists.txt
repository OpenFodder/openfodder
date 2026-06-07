cmake_minimum_required(VERSION 3.18.0)

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif ()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(openfodder VERSION 1.5.4 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(OPENFODDER_ENABLE_FFMPEG "Enable FFmpeg intro video playback" OFF)
option(OPENFODDER_AUTO_FFMPEG "Auto-enable FFmpeg when found (non-Windows)" ON)
option(OPENFODDER_ENABLE_NETWORK "Enable GGPO rollback networking for cooperative multiplayer" ON)

if(MSVC)
    # Enable Hot Reload (Edit and Continue) for Debug builds.
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<CONFIG:Debug>,EditAndContinue,ProgramDatabase>")
    add_compile_options($<$<CONFIG:Debug>:/ZI>)
    add_link_options($<$<CONFIG:Debug>:/INCREMENTAL>)
endif()

if(EMSCRIPTEN)
    set(OPENFODDER_EMSCRIPTEN ON)
else()
    find_package(Threads REQUIRED)
endif()

if(EMSCRIPTEN)
elseif(WIN32)
    include(FetchContent)

    set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)

    set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLMIXER_MOD_XMP ON CACHE BOOL "" FORCE)
    set(SDLMIXER_MOD_XMP_LITE ON CACHE BOOL "" FORCE)
    set(SDLMIXER_MOD_XMP_SHARED OFF CACHE BOOL "" FORCE)
    set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)
    set(SDLMIXER_INSTALL OFF CACHE BOOL "" FORCE)

    set(SDL3_GIT_TAG "release-3.4.0")
    set(SDL3_MIXER_GIT_TAG "3045b35335113e5e5ba258858cb403d8803bfbeb")

    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG ${SDL3_GIT_TAG}
        GIT_SHALLOW FALSE
    )

    FetchContent_Declare(
        SDL3_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG ${SDL3_MIXER_GIT_TAG}
        GIT_SHALLOW FALSE
    )

    FetchContent_MakeAvailable(SDL3)
    FetchContent_MakeAvailable(SDL3_mixer)

    set(_sdl_mixer_compat_header "${CMAKE_CURRENT_LIST_DIR}/cmake/sdl3_mixer_compat.h")
    foreach(_sdl_mixer_target IN ITEMS SDL3_mixer SDL3_mixer-shared SDL3_mixer-static)
        if(TARGET ${_sdl_mixer_target})
            if(MSVC)
                target_compile_options(${_sdl_mixer_target} PRIVATE "/FI${_sdl_mixer_compat_header}")
            else()
                target_compile_options(${_sdl_mixer_target} PRIVATE "-include" "${_sdl_mixer_compat_header}")
            endif()
        endif()
    endforeach()

    # libcurl — used by the setup wizard / updater for HTTPS downloads of
    # retail-data manifests, scripts, and engine update archives. On Windows
    # we vendor it via FetchContent and pin TLS to Schannel so we don't
    # have to ship an OpenSSL CA bundle. HTTP_ONLY plus the per-protocol
    # disables strip out everything we don't need (LDAP, SMTP, etc.).
    set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
    set(CURL_USE_OPENSSL OFF CACHE BOOL "" FORCE)
    set(CURL_DISABLE_LDAP ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_LDAPS ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_TELNET ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_DICT ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_FILE ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_TFTP ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_RTSP ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_POP3 ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_IMAP ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_SMTP ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_GOPHER ON CACHE BOOL "" FORCE)
    set(CURL_DISABLE_MQTT ON CACHE BOOL "" FORCE)
    set(HTTP_ONLY ON CACHE BOOL "" FORCE)

    set(LIBCURL_GIT_TAG "curl-8_5_0")
    FetchContent_Declare(libcurl
        GIT_REPOSITORY https://github.com/curl/curl.git
        GIT_TAG ${LIBCURL_GIT_TAG}
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(libcurl)
else()
    find_package(SDL3 CONFIG REQUIRED)
    find_package(SDL3_mixer CONFIG REQUIRED)
    find_package(CURL REQUIRED)
endif()

# Locate git binary to provide information to the build environment
find_package(Git)

if(GIT_FOUND)
# Define short commit hash.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE OPENFODDER_COMMIT_SHA1_SHORT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
# Fallback in case the git exe can't be found.
    set(OPENFODDER_COMMIT_SHA1_SHORT "deadbeef")
endif()

configure_file(cmake/gitver.hpp.in Source/gitver.hpp @ONLY)

file(GLOB_RECURSE GAMEENGINE_HEADERS CONFIGURE_DEPENDS "Source/*.hpp" "Source/*.h")
file(GLOB_RECURSE GAMEENGINE_SOURCE CONFIGURE_DEPENDS "Source/*.cpp")

# Vendored miniz is C, not C++. miniz.c contains both prototypes and
# definitions and explodes when the compiler treats it as C++ (the static
# table arrays end up emitted twice). Compile it as C explicitly.
list(APPEND GAMEENGINE_SOURCE "${CMAKE_SOURCE_DIR}/Source/Utils/miniz.c")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source" FILES ${GAMEENGINE_HEADERS} ${GAMEENGINE_SOURCE})

if(EMSCRIPTEN)
    list(FILTER GAMEENGINE_SOURCE EXCLUDE REGEX "Source/PC/Sound_PC2.cpp$")
    list(FILTER GAMEENGINE_SOURCE EXCLUDE REGEX "Source/PC/Sound_PC.cpp$")
    list(FILTER GAMEENGINE_SOURCE EXCLUDE REGEX "Source/Start.cpp$")
endif()

# segrax/firy — disk-image filesystem library (ADF/D64/FAT, soon ISO9660).
# Pulled in as a git submodule under library/firy/. The setup wizard mounts
# user-supplied disk images via firy at runtime instead of extracting them.
# Skip on Emscripten — the setup wizard isn't part of the web build.
if(NOT EMSCRIPTEN AND EXISTS "${CMAKE_SOURCE_DIR}/library/firy/CMakeLists.txt")
    add_subdirectory(library/firy)
    set(OPENFODDER_HAS_FIRY TRUE)
else()
    set(OPENFODDER_HAS_FIRY FALSE)
endif()

add_executable(openfodder ${GAMEENGINE_SOURCE} ${GAMEENGINE_HEADERS})

if(OPENFODDER_HAS_FIRY)
    target_link_libraries(openfodder PRIVATE firy)
    target_compile_definitions(openfodder PRIVATE OPENFODDER_HAS_FIRY=1)
endif()

if(WIN32)
    target_sources(openfodder PRIVATE
        "${CMAKE_SOURCE_DIR}/Projects/OpenFodder.rc"
        "${CMAKE_SOURCE_DIR}/Projects/resource.h")
endif()

if(MSVC)
    set_target_properties(openfodder PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Run/$<CONFIG>")
    set_target_properties(openfodder PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Run/$<CONFIG>")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT openfodder)
endif()

target_include_directories(openfodder PRIVATE
    Source
    ${CMAKE_CURRENT_BINARY_DIR}/Source)

target_compile_definitions(openfodder PRIVATE
    UNICODE
    _UNICODE
    _USE_MATH_DEFINES)

if(OPENFODDER_ENABLE_NETWORK AND NOT EMSCRIPTEN)
    # Clone GGPO directly via git during configure so the source is
    # immediately available for add_library (avoids FetchContent subbuild
    # issues with CMake 4.x and GGPO's outdated CMakeLists.txt).
    set(GGPO_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ggpo-src")
    find_package(Git REQUIRED)

    # Use .git presence as the reliable "fully cloned" marker.
    # If it's missing (never cloned or partial), wipe and re-clone.
    if(NOT EXISTS "${GGPO_SOURCE_DIR}/.git")
        file(REMOVE_RECURSE "${GGPO_SOURCE_DIR}")
        message(STATUS "[GGPO] Cloning pond3r/ggpo...")
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" clone --depth 1
                    "https://github.com/pond3r/ggpo.git"
                    "${GGPO_SOURCE_DIR}"
            RESULT_VARIABLE _ggpo_result
            ERROR_VARIABLE  _ggpo_error
        )
        if(_ggpo_result)
            message(FATAL_ERROR "Failed to clone GGPO (${_ggpo_result}): ${_ggpo_error}")
        endif()
    endif()

    if(NOT WIN32)
        set(GGPO_LINUX_PATCH "${CMAKE_CURRENT_LIST_DIR}/cmake/ggpo-linux-compat.patch")
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" -C "${GGPO_SOURCE_DIR}" apply --check "${GGPO_LINUX_PATCH}"
            RESULT_VARIABLE _ggpo_patch_check
            ERROR_VARIABLE _ggpo_patch_error
        )
        if(_ggpo_patch_check EQUAL 0)
            message(STATUS "[GGPO] Applying Linux compatibility patch...")
            execute_process(
                COMMAND "${GIT_EXECUTABLE}" -C "${GGPO_SOURCE_DIR}" apply "${GGPO_LINUX_PATCH}"
                RESULT_VARIABLE _ggpo_patch_result
                ERROR_VARIABLE _ggpo_patch_apply_error
            )
            if(_ggpo_patch_result)
                message(FATAL_ERROR "Failed to apply GGPO Linux compatibility patch: ${_ggpo_patch_apply_error}")
            endif()
        else()
            execute_process(
                COMMAND "${GIT_EXECUTABLE}" -C "${GGPO_SOURCE_DIR}" apply --reverse --check "${GGPO_LINUX_PATCH}"
                RESULT_VARIABLE _ggpo_patch_reverse_check
                ERROR_QUIET
            )
            if(NOT _ggpo_patch_reverse_check EQUAL 0)
                file(READ "${GGPO_SOURCE_DIR}/src/include/ggponet.h" _ggpo_net_header)
                file(READ "${GGPO_SOURCE_DIR}/src/lib/ggpo/platform_linux.h" _ggpo_linux_header)
                if(_ggpo_net_header MATCHES "__cdecl" AND
                   _ggpo_linux_header MATCHES "WSAGetLastError" AND
                   _ggpo_linux_header MATCHES "CreateDirectoryA")
                    message(STATUS "[GGPO] Linux compatibility patch appears to be already applied.")
                else()
                    message(FATAL_ERROR "GGPO Linux compatibility patch no longer applies: ${_ggpo_patch_error}")
                endif()
            endif()
        endif()
    endif()

    # Auto-discover all GGPO .cpp files (robust to repo layout changes).
    set(GGPO_SRC_DIR "${GGPO_SOURCE_DIR}/src/lib/ggpo")
    file(GLOB_RECURSE GGPO_SOURCES "${GGPO_SRC_DIR}/*.cpp")
    if(WIN32)
        list(FILTER GGPO_SOURCES EXCLUDE REGEX "platform_linux\\.cpp$")
    else()
        list(FILTER GGPO_SOURCES EXCLUDE REGEX "platform_windows\\.cpp$")
    endif()
    if(NOT GGPO_SOURCES)
        message(FATAL_ERROR
            "No GGPO source files found under ${GGPO_SRC_DIR}.\n"
            "Delete ${GGPO_SOURCE_DIR} and reconfigure to force a fresh clone.")
    endif()

    add_library(ggpo_static STATIC ${GGPO_SOURCES})
    target_include_directories(ggpo_static
        PUBLIC  "${GGPO_SOURCE_DIR}/src/include"
        PRIVATE "${GGPO_SRC_DIR}"
    )
    if(WIN32)
        target_link_libraries(ggpo_static PUBLIC ws2_32 winmm)
    endif()
    if(MSVC)
        # Suppress warnings in GGPO that we don't own
        target_compile_options(ggpo_static PRIVATE /W0)
    endif()

    target_compile_definitions(openfodder PRIVATE OPENFODDER_ENABLE_NETWORK)
    target_link_libraries(openfodder PRIVATE ggpo_static)
    target_include_directories(openfodder PRIVATE "${GGPO_SOURCE_DIR}/src/include")
endif()

if (OPENFODDER_ENABLE_FFMPEG OR (OPENFODDER_AUTO_FFMPEG AND NOT WIN32))
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
    find_package(FFmpeg)
    if (FFmpeg_FOUND)
        target_compile_definitions(openfodder PRIVATE OPENFODDER_ENABLE_FFMPEG)
        target_include_directories(openfodder PRIVATE ${FFMPEG_INCLUDE_DIRS})
        target_link_libraries(openfodder PRIVATE ${FFMPEG_LIBRARIES})
    elseif(OPENFODDER_ENABLE_FFMPEG)
        message(WARNING "FFmpeg not found; intro video playback disabled.")
    endif()
endif()

if(EMSCRIPTEN)
    target_compile_definitions(openfodder PRIVATE EMSCRIPTEN OPENFODDER_NO_MIXER)
    target_compile_options(openfodder PRIVATE "-sUSE_SDL=3")
    set_target_properties(openfodder PROPERTIES
        OUTPUT_NAME "OpenFodder"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/RunE"
        SUFFIX ".html")
    target_link_options(openfodder PRIVATE
        "-sUSE_SDL=3"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sASSERTIONS=1"
        "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/Run@/")
else()
    target_link_libraries(openfodder PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer Threads::Threads)
    if(WIN32)
        target_link_libraries(openfodder PRIVATE libcurl)
    else()
        target_link_libraries(openfodder PRIVATE CURL::libcurl)
    endif()
endif()

if(WIN32)
    set(_runtime_dlls "")
    if(TARGET SDL3::SDL3)
        list(APPEND _runtime_dlls $<TARGET_FILE:SDL3::SDL3>)
    elseif(TARGET SDL3-shared)
        list(APPEND _runtime_dlls $<TARGET_FILE:SDL3-shared>)
    endif()

    if(TARGET SDL3_mixer::SDL3_mixer)
        list(APPEND _runtime_dlls $<TARGET_FILE:SDL3_mixer::SDL3_mixer>)
    elseif(TARGET SDL3_mixer-shared)
        list(APPEND _runtime_dlls $<TARGET_FILE:SDL3_mixer-shared>)
    endif()

    if(_runtime_dlls)
        add_custom_command(TARGET openfodder POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${_runtime_dlls}
                "$<TARGET_FILE_DIR:openfodder>"
            COMMENT "Copy SDL runtime DLLs to output directory")
    endif()
endif()
