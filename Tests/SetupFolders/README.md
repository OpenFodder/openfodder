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
Resource-manager filesystem operations are supplied by the fixture harness.
