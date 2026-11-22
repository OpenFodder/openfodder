cmake_minimum_required(VERSION 3.18)
project(SetupFolderChecks LANGUAGES CXX)
find_package(SDL3 3.4 CONFIG REQUIRED)
enable_testing()
add_executable(setup_folder_check check.cpp
    ../../Source/Setup/Validation.cpp ../../Source/Setup/AutoScan.cpp ../../Source/Utils/md5.cpp ../../Source/Amiga/dernc.cpp
    ../../Source/Setup/MountedImage.cpp ../../Source/Setup/CapsImageLoader.cpp)
target_compile_features(setup_folder_check PRIVATE cxx_std_17)
target_include_directories(setup_folder_check PRIVATE ../../Source)
target_compile_definitions(setup_folder_check PRIVATE OPENFODDER_NO_MIXER=1)
target_link_libraries(setup_folder_check PRIVATE SDL3::SDL3)
if(WIN32)
    add_custom_command(TARGET setup_folder_check POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:setup_folder_check>)
endif()
add_test(NAME setup_folder_scope COMMAND setup_folder_check)
set_tests_properties(setup_folder_scope PROPERTIES TIMEOUT 30)
