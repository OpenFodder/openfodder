Folder discovery regression checks
==================================

Enable with `-DOPENFODDER_BUILD_SETUP_TESTS=ON`, build `setup_folder_check`,
then run `ctest -C Release -R setup_folder_scope --output-on-failure`.
The directory also supports standalone CMake configuration with SDL3 installed.

Uses temporary files and small overlapping release manifests, without game
assets. Exercises the production validator and automatic scanner: selected
release directories must not expose sibling games; direct, Data-wrapped and
engine layouts must work; arbitrary images must not become automatic game
suggestions; checksum mismatches must not identify a different release.
Collections appear as one automatic suggestion and expand into individual
choices. Explicit folder picks inspect two levels of child game folders from
any selection, deduplicate canonical layouts, and stop at complete release folders.
Search paths and installed-release snapshots are supplied by the fixture harness.

Installed releases are excluded by version identity, including external copies.
Their discovered locations are retained for the already-added result screen.
Floppy/CD32 overlaps prefer the edition with stronger checksum evidence.
Synthetic RNC fixtures cover compressed-file identification, output limits and
truncated input; directory cancellation must not publish partial matches.
The optional engine setup diagnostic also checks responsive frames during a
blocked worker, cancellation, already-added data and preserving a chosen release.
Native engine diagnostics (`OPENFODDER_SETUP_DIAGNOSTICS=ON`) also support
`OPENFODDER_CURSOR_TRANSITION_CHECK=1` for cursor transitions and
`OPENFODDER_SQUAD_JOIN_CHECK=1` for enemy squad-zero joins, invalid source/target
state and a normal player merge including ammunition transfer.
On Windows, `OPENFODDER_BROWSER_CHECK=1` checks drive-list navigation and captures
drive roots and folder names containing dollar signs.
`OPENFODDER_INSTALLED_FOLDER_CHECK=1` checks and captures the result for a
selected folder whose discovered child games are already available. Run with
`--setup`, `OPENFODDER_SETUP_PREVIEW_DIR` set to an output directory and
`OPENFODDER_SETUP_PREVIEW_DISKS` set to a game-data directory.
