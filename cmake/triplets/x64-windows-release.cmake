set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
# Release builds do not need a second, Debug build of FFmpeg and other ports.
set(VCPKG_BUILD_TYPE release)
