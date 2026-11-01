# Build the native SPS SDK carried by SOS_Unpacker directly into OpenFodder.
# SOS_Unpacker carries the portable SPS sources and the matching headers.
include(FetchContent)
FetchContent_Declare(openfodder_capsimage
    GIT_REPOSITORY https://github.com/OpenFodder/SOS_Unpacker.git
    GIT_TAG 45b2fc4e318cc0530bd1d22ead86cb73b5786483)
FetchContent_MakeAvailable(openfodder_capsimage)
set(caps_root "${openfodder_capsimage_SOURCE_DIR}")
file(GLOB caps_sources CONFIGURE_DEPENDS
    "${caps_root}/capsimg/CAPSImg/*.cpp"
    "${caps_root}/capsimg/Core/*.cpp"
    "${caps_root}/capsimg/Codec/*.cpp")
add_library(openfodder_caps_sdk STATIC ${caps_sources})
target_include_directories(openfodder_caps_sdk PRIVATE "${caps_root}")
target_compile_features(openfodder_caps_sdk PRIVATE cxx_std_14)
if(WIN32)
    target_compile_definitions(openfodder_caps_sdk PRIVATE WIN32 NOMINMAX _CRT_SECURE_NO_WARNINGS)
    set_target_properties(openfodder_caps_sdk PROPERTIES VS_GLOBAL_VcpkgAutoLink false)
else()
    target_sources(openfodder_caps_sdk PRIVATE "${caps_root}/stdafx.cpp")
endif()
target_link_libraries(openfodder PRIVATE openfodder_caps_sdk)
get_filename_component(caps_adapter "${CMAKE_CURRENT_LIST_DIR}/../Source/Setup/CapsImageLoader.cpp" ABSOLUTE)
set_property(SOURCE "${caps_adapter}" APPEND PROPERTY
    COMPILE_DEFINITIONS OPENFODDER_HAS_CAPSIMAGE=1)
set_property(SOURCE "${caps_adapter}" APPEND PROPERTY
    INCLUDE_DIRECTORIES "${caps_root}")
# Keep the SDK license and complete SDK sources with the build.
# The archive includes a standalone project for rebuilding the static SDK.
set(caps_source_stage "${CMAKE_BINARY_DIR}/capsimage-source")
file(MAKE_DIRECTORY "${caps_source_stage}")
add_custom_command(TARGET openfodder_caps_sdk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${caps_root}/capsimg" "${caps_source_stage}/capsimg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${caps_root}/stdafx.h" "${caps_source_stage}/stdafx.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${caps_root}/stdafx.cpp" "${caps_source_stage}/stdafx.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/CapsImageStandalone.cmake" "${caps_source_stage}/CMakeLists.txt"
    COMMAND ${CMAKE_COMMAND} -E tar cf "$<TARGET_FILE_DIR:openfodder_caps_sdk>/SOS-SDK-Source.zip" --format=zip -- capsimg stdafx.h stdafx.cpp CMakeLists.txt
    WORKING_DIRECTORY "${caps_source_stage}")
add_custom_command(TARGET openfodder POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${caps_root}/capsimg/LICENCE.txt" "$<TARGET_FILE_DIR:openfodder>/CAPSImage-LICENCE.txt"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:openfodder_caps_sdk>/SOS-SDK-Source.zip" "$<TARGET_FILE_DIR:openfodder>")
