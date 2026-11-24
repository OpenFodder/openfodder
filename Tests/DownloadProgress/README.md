# Download progress checks

Configure this directory with `SDL3_DIR` pointing to the SDL3 CMake package,
build, then run CTest. The window checks use SDL's dummy video driver, need no
game data or network connection, and cover completion, failure, cancellation,
worker exceptions, and preserving a pending application quit.
Startup checks cover keyboard and mouse rejection without running the worker,
confirmation into progress, and failure details in the same window.

The main project includes these checks with `-DOPENFODDER_BUILD_SETUP_TESTS=ON`.

Run `download_progress_check --preview download`, `--preview unknown`, or
`--preview install` without the dummy driver to inspect the real dialog.
These previews use synthetic progress and never install files.
An optional final argument limits a preview's duration in seconds.

`--preview-prompt` and `--preview-prompt failure` render and automatically close
the startup and failure screens without any network requests. Set
`OPENFODDER_DOWNLOAD_PREVIEW` to a BMP path to capture the rendered screen;
this capture hook is only compiled into the test executable.
