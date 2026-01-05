cmake_minimum_required(VERSION 3.10.0)

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(openfodder VERSION 1.5.4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

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
else()
    find_package(SDL3 CONFIG REQUIRED)
    find_package(SDL3_mixer CONFIG REQUIRED)
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

file(GLOB_RECURSE GAMEENGINE_HEADERS "Source/*.hpp")
file(GLOB_RECURSE GAMEENGINE_SOURCE "Source/*.cpp")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source" FILES ${GAMEENGINE_HEADERS} ${GAMEENGINE_SOURCE})

if(EMSCRIPTEN)
    list(FILTER GAMEENGINE_SOURCE EXCLUDE REGEX "Source/PC/Sound_PC2.cpp$")
    list(FILTER GAMEENGINE_SOURCE EXCLUDE REGEX "Source/PC/Sound_PC.cpp$")
    list(FILTER GAMEENGINE_SOURCE EXCLUDE REGEX "Source/Start.cpp$")
endif()

add_executable(openfodder ${GAMEENGINE_SOURCE} ${GAMEENGINE_HEADERS})

if(WIN32)
    target_sources(openfodder PRIVATE
        "${CMAKE_SOURCE_DIR}/Projects/OpenFodder.rc"
        "${CMAKE_SOURCE_DIR}/Projects/resource.h")
endif()

if(MSVC)
    set_target_properties(openfodder PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Run"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Run"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Run"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Run"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/Run")
    set_target_properties(openfodder PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Run")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT openfodder)
endif()

target_include_directories(openfodder PRIVATE
    Source
    ${CMAKE_CURRENT_BINARY_DIR}/Source)

target_compile_definitions(openfodder PRIVATE
    SDL_ENABLE_OLD_NAMES
    UNICODE
    _UNICODE
    _USE_MATH_DEFINES)

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
    target_link_libraries(openfodder SDL3::SDL3 SDL3_mixer::SDL3_mixer Threads::Threads)
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
                "${CMAKE_SOURCE_DIR}/Run"
            COMMENT "Copy SDL runtime DLLs to output directory")
    endif()
endif()
