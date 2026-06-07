# Open Fodder installation

## About

Open Fodder requires campaign/mission data plus files from a demo or retail release
of Cannon Fodder. The data must be placed in specific folders depending on your
operating system.

## Windows

Download the latest portable archive (`OpenFodder-<version>-win64.zip`) from
the [Releases page](https://github.com/OpenFodder/openfodder/releases) and
unzip it anywhere — the executable runs in place. There's no installer to
run and no admin rights required.

On first launch, if OpenFodder cannot find any game data, it will offer to
download the demo data and scripts from GitHub. You can also click the
**UPDATE** button on the About screen at any time to check for newer
compatible releases.

Default data folders:

* **Portable** (the working directory contains `Data/`, `Scripts/`, `about.png`,
  or `openfodder.ini.example`):

  ```
  <unzipped folder>\Data
  <unzipped folder>\Scripts
  ```

* **Per-user** (anywhere else, e.g. when the exe is installed under
  `Program Files` and launched from elsewhere):

  ```
  %USERPROFILE%\Documents\OpenFodder\Data
  %USERPROFILE%\Documents\OpenFodder\Scripts
  ```

If you have retail Cannon Fodder data, copy the files into the matching
subfolders below. The in-game setup wizard can also locate retail folders or
mount disk images at runtime.

## Linux / Unix

Open Fodder searches for an `OpenFodder` data folder in this order:

1. The current working directory
2. `$XDG_DATA_HOME/OpenFodder` (if set)
3. Each path in `$XDG_DATA_DIRS` (appends `/OpenFodder`)
4. `$HOME/.local/share/OpenFodder`
5. `/usr/local/share/OpenFodder`

### Flatpak

The [Flatpak](https://flathub.org/en/apps/org.openfodder.OpenFodder) package uses:

```
~/.var/app/org.openfodder.OpenFodder/data/OpenFodder
```

Create the `OpenFodder` folder there, then place the campaign data pack and any
retail data inside it.

The in-app "Download data" prompt and the About-screen UPDATE button are both
**disabled** under Flatpak. Updates flow through Flatpak itself:

```
flatpak update org.openfodder.OpenFodder
```

If your install is missing data files, prefer:

```
flatpak repair --user org.openfodder.OpenFodder
```

over a manual side-load — Flatpak's per-app data extension version is recorded
in the manifest, and a side-load can put the runtime out of sync with it.

## Auto-download (Windows / non-Flatpak Linux / macOS)

When OpenFodder cannot find any data files on first launch, it offers to
download the latest demo data and scripts from GitHub:

* `github.com/OpenFodder/data`    → Data files (campaigns + demo content)
* `github.com/OpenFodder/scripts` → JavaScript modules (random map, multiplayer)

The About screen also has an UPDATE button that checks for newer compatible
releases. Both download paths use the engine's hardcoded compatibility range
(`Source/Setup/EngineVersion.hpp`) and refuse to install a release whose
declared `dataVersion` / `scriptVersion` falls outside it.

Install destination depends on whether the working directory looks portable:

* **Portable** (cwd contains `Data/`, `Scripts/`, `about.png`, or
  `openfodder.ini.example`, AND is writable):
  installs into `<cwd>/Data` and `<cwd>/Scripts`. Used by dev checkouts and
  the Windows `.zip` distribution.

* **Per-user** (anywhere else, e.g. `/usr/bin/openfodder` with cwd `~`):
  installs into the per-user OpenFodder data root —
  `%USERPROFILE%/Documents/OpenFodder` on Windows,
  `$HOME/Library/Application Support/OpenFodder` on macOS,
  `$XDG_DATA_HOME/OpenFodder` (or `$HOME/.local/share/OpenFodder`) on Linux —
  the same paths the engine's read-side scan checks.

## Data

### Campaign Data

The campaign information was originally hardcoded in the executable. It is
required to play and is published — alongside six Amiga demos and one PC demo —
in a separate repository:

https://github.com/OpenFodder/data/releases/

You don't normally need to download this manually: OpenFodder will offer to
fetch it on first launch (see the auto-download note above), and the About
screen's UPDATE button keeps it current. The portable Windows release also
ships a copy bundled inside the .zip. Manual download is only necessary if
you're running a Flatpak (which uses Flatpak's own update flow), running
behind a network firewall that blocks GitHub, or building entirely from
source on a system without network access.

  
### Retail Data

The folder you place data into should look like:

```
OpenFodder/
  Data/
  Campaigns/
  Saves/
```

#### DOS CD Versions
  
For Cannon Fodder (DOS CD), copy `CF_ENG.DAT` from the CD (or the GOG install
destination) to:

```
OpenFodder/Data/Dos_CD
```

For Cannon Fodder 2 (DOS CD), copy all files (not just `CF_ENG.DAT`) from the CD
or the GOG install destination to:

```
OpenFodder/Data/Dos2_CD
```
  
#### Amiga

Please be aware: Cannon Fodder 2 is a typical AmigaDOS disk (SOS Unpacker will
not work). Use an ADF tool on your machine, such as Directory Opus on Windows
with the firy plugin: https://github.com/segrax/directory_opus_firy_plugin

##### Option 1. SOS Unpacker
  
Use the [SOS Unpacker](https://github.com/OpenFodder/SOS_Unpacker) to unpack
Kyroflux "CT Raw" images of your Cannon Fodder disks, then copy the contents of
`out` into:

```
OpenFodder/Data/Amiga
```
	
##### Option 2. WHDLoad
  
Use the [WHDLoad installer](http://www.whdload.de/games/CannonFodder.html) on an
Amiga or an emulator (WinUAE/UAE) to extract the game files from the original
floppy disks, then copy the contents of `cf_data` into:

```
OpenFodder/Data/Amiga
```


#### Amiga CD32
  
Copy the files from the `Fodder` folder on the CD-ROM to:

```
OpenFodder/Data/Amiga_CD
```

### Optional intro video (CD32 Track 2)

The CD32 version includes a full-motion intro video on Track 2. Open Fodder
will play it for any retail Cannon Fodder 1 install if you extract it as an
MPEG file and place it in the root of the `Data` folder. The preferred name is
`Intro.mpg`, but the game also accepts `intro.mpg`, `TRACK02.MPG`, `Track02.mpg`,
`track02.mpg`.

```
OpenFodder/Data/Intro.mpg
```

## Additional data search paths (openfodder.ini)

Open Fodder reads `openfodder.ini` from the working directory.
Use the `[paths]` section to add extra data search paths:

```
[paths]
path0=/path/to/OpenFodder
path1=f:\openfodder
```

## Building from source

See `COMPILING.md` for build instructions.
  
