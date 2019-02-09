# Open Fodder Installation instructions

## About

Open Fodder requires campaign and mission information, as well as the data files from a retail release of Cannon Fodder. These files must be installed to certain locations depending on your operating system, please see the details below for instructions.  


### OS Specific

#### Windows

Download the [installer package](https://github.com/OpenFodder/openfodder/releases).  

The Windows version of Open Fodder defaults to using and installing data to the location '%USERPROFILE%/Documents/OpenFodder'.  
While the demos will be installed as part of the Windows installer package, you will be required to copy a full retail release into this folder (following the instructions below, or when prompted to select it during the installer)
  
  
#### Linux / Unix

Open Fodder will check the environment variable $XDG_DATA_DIRS and iterate each path checking for the data folder "/OpenFodder/".  
Failure to locate the folder in these paths, will result in a check of $HOME/.local/share/OpenFodder.  
Failure on this path, will fall back to the current working directory.  
  
  
## Data

### Campaign Data

The campaign information was originally hardcoded in the executable, this is required to play and is downloadable alonside six demos from (https://github.com/OpenFodder/data/releases/download/1.6.0/Data.pack.1.6.0.zip).  

This data is packaged with the Windows Installer, but will be a required download if you're compiling / running on Linux/Unix.  
  
### Retail Data

#### Dos CD Version
  
Copy 'CF_ENG.DAT' from the CD (or the GOG install destination) to the OpenFodder/Data/Dos_CD folder
  
#### Amiga

##### Option1. SOS Unpacker  
  
Use the [SOS Unpacker](https://github.com/OpenFodder/SOS_Unpacker) to unpack Kyroflux 'CT Raw' images of your Cannon Fodder disks, then copy the contents of 'out'  into the OpenFodder/Data/Amiga directory.  
    
##### Option2. WHDLoad
  
Use the [WHDLoad installer](http://www.whdload.de/games/CannonFodder.html) on an Amiga or an emulator (WinUAE/UAE) to extract the game files from the original floppy disks, then copy the contents of 'cf_data'  into the OpenFodder/Data/Amiga directory.  


#### Amiga CD32
  
Copy the files from the 'Fodder' folder on the CD-Rom, to the OpenFodder/Data/Amiga_CD folder. For the final video, Extract Track2 from the CD and name it 'Track2.mpg', placed in the Amiga_CD folder.
  
* Please note: The video playback is not yet supported
  
