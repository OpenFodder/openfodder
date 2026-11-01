# Keep host executables, disk images, saves, and diagnostics out of the download.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
option(OPENFODDER_WEB_RETAIL_DATA "Include locally installed retail game data in the web bundle" OFF)
set(_web_assets "${CMAKE_CURRENT_BINARY_DIR}/web-assets")
set(_web_retail_arg "")
if(OPENFODDER_WEB_RETAIL_DATA)
    set(_web_retail_arg --retail)
endif()
add_custom_target(openfodder_web_assets
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/Projects/emscripten/package_assets.py"
        "${CMAKE_SOURCE_DIR}/Run" "${_web_assets}" ${_web_retail_arg}
    BYPRODUCTS "${_web_assets}/manifest.json"
    COMMENT "Stage browser runtime assets"
    VERBATIM)
add_dependencies(openfodder openfodder_web_assets)
set_property(TARGET openfodder APPEND PROPERTY LINK_DEPENDS
    "${_web_assets}/manifest.json"
    "${CMAKE_SOURCE_DIR}/Projects/emscripten/shell.html")
target_link_options(openfodder PRIVATE
    "SHELL:--preload-file \"${_web_assets}/runtime@/\""
    "-lidbfs.js")
