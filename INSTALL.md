# Open Fodder Installation instructions

## About

Open Fodder requires campaign and mission information, as well as the data files from a demo or retail release of Cannon Fodder. These files must be installed to certain locations depending on your operating system, please see the details below for instructions.  

## Windows

Download the [installer package](https://github.com/OpenFodder/openfodder/releases).  

The Windows version of Open Fodder defaults to using and installing data to the location '%USERPROFILE%/Documents/OpenFodder'.  
While the demos will be installed as part of the Windows installer package, you will be required to copy a full retail release into this folder (following the instructions below, or when prompted to select it during the installer)

## Building (Windows, CMake + vcpkg)

1. Install vcpkg and set `VCPKG_ROOT` to the vcpkg folder.
2. From the repo root, run `vcpkg install` (manifest mode uses `vcpkg.json`).
3. Configure and build:
   - `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake`
   - `cmake --build build --config Release`
  
  
## Linux / Unix

Open Fodder will check the environment variable $XDG_DATA_DIRS and iterate each path checking for the data folder `/OpenFodder/`.  
Failure to locate the folder in these paths, will result in a check of `$HOME/.local/share/OpenFodder`.  
Failure on this path, will fall back to the current working directory.  

### Flatpak

The [Flatpak](https://flathub.org/en/apps/org.openfodder.OpenFodder) package for Linux creates a user-writable folder in  `~/.var/app/org.openfodder.OpenFodder/data/`
You will need to manually create an `OpenFodder` folder in this location, before proceeding with the steps below to unzip the Campaign Data pack into it, and add any Retail Data.

## Data

### Campaign Data

This data is packaged with the Windows Installer, but will be a required download if you're compiling / running on Linux/Unix.  

The campaign information was originally hardcoded in the executable. This is required to play and is downloadable alonside six demos from (https://github.com/OpenFodder/data/releases/).  

  
### Retail Data

#### DOS CD Versions
  
Copy 'CF_ENG.DAT' from the CD (or the GOG install destination) to the `OpenFodder/Data/Dos_CD` folder.

Copy all files (not just CF_ENG.DAT) from the CD or the GOG install destination to the `OpenFodder/Data/Dos2_CD` folder.
  
#### Amiga

Please be aware, Cannon Fodder 2 is a typical AmigaDOS disk (SOS Unpacker will not work). Use an ADF tool on your machine, such as Directory Opus on Windows with the firy plugin (https://github.com/segrax/directory_opus_firy_plugin)

##### Option1. SOS Unpacker  
  
Use the [SOS Unpacker](https://github.com/OpenFodder/SOS_Unpacker) to unpack Kyroflux 'CT Raw' images of your Cannon Fodder disks, then copy the contents of 'out' into the `OpenFodder/Data/Amiga` directory.  
	
##### Option2. WHDLoad
  
Use the [WHDLoad installer](http://www.whdload.de/games/CannonFodder.html) on an Amiga or an emulator (WinUAE/UAE) to extract the game files from the original floppy disks, then copy the contents of 'cf_data'  into the `OpenFodder/Data/Amiga` directory.  


#### Amiga CD32
  
Copy the files from the 'Fodder' folder on the CD-Rom, to the `OpenFodder/Data/Amiga_CD` folder. For the final video, Extract Track2 from the CD and name it 'Track2.mpg', placed in the Amiga_CD folder.
  
* Please note: The video playback is not yet supported
  
