# Open Fodder installation

## About

Open Fodder requires campaign/mission data plus files from a demo or retail release
of Cannon Fodder. The data must be placed in specific folders depending on your
operating system.

## Windows

Download the [installer package](https://github.com/OpenFodder/openfodder/releases).

Default data folder:

```
%USERPROFILE%\Documents\OpenFodder
```

The Windows installer includes the demo data. If you have a retail release, copy
it into the folders below (or use the installer prompt).

Open Fodder also checks the current working directory for an `OpenFodder` folder.

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

## Data

### Campaign Data

This data is packaged with the Windows installer, but is a required download if
you're running on Linux/Unix or building from source.

The campaign information was originally hardcoded in the executable. This is
required to play and is downloadable alongside six demos from:

https://github.com/OpenFodder/data/releases/

  
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
  
