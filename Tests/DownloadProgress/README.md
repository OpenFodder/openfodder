# Download progress checks

Configure this directory with `SDL3_DIR` pointing to the SDL3 CMake package,
build, then run CTest. The window checks use SDL's dummy video driver, need no
game data or network connection, and cover completion, failure, cancellation,
worker exceptions, and preserving a pending application quit.

The main project includes these checks with `-DOPENFODDER_BUILD_SETUP_TESTS=ON`.

Run `download_progress_check --preview download`, `--preview unknown`, or
`--preview install` without the dummy driver to inspect the real dialog.
These previews use synthetic progress and never install files.
An optional final argument limits a preview's duration in seconds.
