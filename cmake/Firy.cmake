# Keep the pinned firy checkout untouched. Its Windows-specific declarations
# need a portability patch on Linux; build that version from a private copy.
get_filename_component(_firy_source "${CMAKE_CURRENT_LIST_DIR}/../library/firy" ABSOLUTE)
if(NOT WIN32)
    if(NOT GIT_EXECUTABLE)
        message(FATAL_ERROR "Git is required to apply the firy Linux compatibility patch.")
    endif()
    set(_firy_patch "${CMAKE_CURRENT_LIST_DIR}/firy-linux-compat.patch")
    file(GLOB_RECURSE _firy_inputs CONFIGURE_DEPENDS
        "${_firy_source}/source/*.cpp" "${_firy_source}/source/*.hpp")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
        ${_firy_inputs} "${_firy_source}/CMakeLists.txt" "${_firy_patch}")
    set(_firy_patched "${CMAKE_BINARY_DIR}/firy-src")
    # COPYONLY also refreshes files when the patch or submodule changes.
    foreach(_firy_input IN LISTS _firy_inputs)
        file(RELATIVE_PATH _firy_relative "${_firy_source}" "${_firy_input}")
        configure_file("${_firy_input}" "${_firy_patched}/${_firy_relative}" COPYONLY)
    endforeach()
    configure_file("${_firy_source}/CMakeLists.txt" "${_firy_patched}/CMakeLists.txt" COPYONLY)
    execute_process(
        # Prevent Git from finding the parent checkout and silently skipping
        # paths when this build directory is inside that checkout.
        COMMAND "${CMAKE_COMMAND}" -E env "GIT_CEILING_DIRECTORIES=${CMAKE_BINARY_DIR}"
            "${GIT_EXECUTABLE}" apply --ignore-space-change "${_firy_patch}"
        WORKING_DIRECTORY "${_firy_patched}"
        RESULT_VARIABLE _firy_result ERROR_VARIABLE _firy_error)
    if(NOT _firy_result EQUAL 0)
        message(FATAL_ERROR "Failed to apply firy Linux compatibility patch: ${_firy_error}")
    endif()
    set(_firy_source "${_firy_patched}")
endif()
add_subdirectory("${_firy_source}" "${CMAKE_BINARY_DIR}/library/firy")
