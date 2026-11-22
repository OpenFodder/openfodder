Folder discovery regression checks
==================================

Enable with `-DOPENFODDER_BUILD_SETUP_TESTS=ON`, build `setup_folder_check`,
then run `ctest -C Release -R setup_folder_scope --output-on-failure`.
The directory also supports standalone CMake configuration with SDL3 installed.

Uses temporary files and two small overlapping release manifests, without game
assets. Exercises the production validator and automatic scanner: selected
release directories must not expose sibling games; direct, Data-wrapped and
engine layouts must work; arbitrary images must not become automatic game
suggestions; checksum mismatches must not identify a different release.
Collections appear as one automatic suggestion and expand into individual
choices. Explicit folder picks inspect immediate child game folders, deduplicate
canonical layouts, and stop before arbitrary nested directory trees.
Resource-manager filesystem operations are supplied by the fixture harness.

Installed releases are excluded by version identity, including external copies.
Floppy/CD32 overlaps prefer the edition with stronger checksum evidence.
Synthetic RNC fixtures cover compressed-file identification, output limits and
truncated input; directory cancellation must not publish partial matches.
The optional engine setup diagnostic also checks responsive frames during a
blocked worker, cancellation, already-added data and preserving a chosen release.
