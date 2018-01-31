# Open Fodder
[![Build Status](https://api.travis-ci.org/OpenFodder/openfodder.svg?branch=master)](https://travis-ci.org/OpenFodder/openfodder)

![image](https://user-images.githubusercontent.com/1327406/35489590-9f823528-04ec-11e8-9f03-6d18b8d9b581.png)

#### About

Cannon Fodder is an action-strategy shoot 'em up game developed by Sensible Software and published by Virgin Interactive.

The game is military-themed and based on shooting action but with a strategy game-style 
control system. The player directs troops through numerous missions, battling enemy infantry, vehicles and installations.

Open Fodder is an open source version of the Cannon Fodder engine, for modern operating systems.

#### Map Editor

See [OpenFodder Editor](https://github.com/OpenFodder/editor) for a map editor for Windows.


#### Media

[Open Fodder 1.3 Video - Custom: Segra's First](https://www.youtube.com/watch?v=D0Ap38IYVUU)  
[Open Fodder 1.3 Video - Amiga Format Special](https://www.youtube.com/watch?v=aSGOTSw-LlI)  
[Open Fodder 0.8 Video](https://www.youtube.com/watch?v=7AjELdOzoaw)  
[Open Fodder 0.7 Video](https://www.youtube.com/watch?v=c9iu7Jwm3Ak)  
[Open Fodder 0.6 Video](https://youtu.be/lHSw2vQDbfI)  
[Open Fodder 0.5 Video](https://www.youtube.com/watch?v=qUH0BCSMDsY)  
[Open Fodder 0.3 Video](https://www.youtube.com/watch?v=xVSWP2x0JBo)  
[Open Fodder 0.2 Video](https://www.youtube.com/watch?v=MLovwBEWr6k)  
[Open Fodder 0.1 Video](https://www.youtube.com/watch?v=9QLg0hYsFFY)  


#### How to play

Open Fodder supports two demos from the Amiga platform, and 3 retail releases (See below for supported versions).
  
  
##### Hot Keys

* +:   Increase Window Size
* -:   Decrease Window Size 
* F11: Full screen  
* ESC: Back Menu/Abort Phase
  
  
##### Windows

The Windows version of Open Fodder defaults to using and installing to the location '%USERPROFILE%/Documents/OpenFodder'.  
While the demos will be installed as part of the package, you will be required to copy a full retail release into this folder (following the instructions below)
  
  
##### Linux / Unix

Open Fodder will check the environment variable $XDG_DATA_DIRS and iterate each path checking for the Data Folder "/OpenFodder/".  
Failure to locate the folder in these paths, will result in a check of $HOME/.local/share/OpenFodder.  
Failure on this path, will fall back to the current working directory.  
  
##### Mission Campaign Data

Campaign and Demos are available from (https://github.com/OpenFodder/data/releases/download/1.4/OpenFodder.zip).  
The campaign data contains the mission and map names, as well as the mission phase layouts.  
  
This Data is packed with the Windows Installer, but will be a required download if you're compiling / running on Linux/Unix.  
  
  
#### Purchasing the retail release

Cannon Fodder can be purchased from [Good Old Games](http://www.gog.com/game/cannon_fodder), this version is fully supported.  
  
  
#### Supported Versions
  
##### Retail
  
###### Dos CD Version
  
Copy 'CF_ENG.DAT' from the CD (or the GOG install destination) to the OpenFodder/Data/Dos_CD folder
  
###### Amiga
  
Use the [WHDLoad installer](http://www.whdload.de/games/CannonFodder.html) on an Amiga or an emulator (WinUAE/UAE) to extract the game files from the original floppy disks, then copy the contents of 'cf_data'  into the OpenFodder/Data/Amiga directory.
  
###### Amiga CD32
  
Copy the files from the 'Fodder' folder on the CD-Rom, to the OpenFodder/Data/Amiga_CD folder. For the final video, Extract Track2 from the CD and name it 'Track2.mpg', placed in the Amiga_CD folder.
  
* Please note: The video playback is not yet supported
  
##### Demos
  
Two demos are included in the data pack.  
  
* Amiga Format Issue #53: Christmas Special (Cannon Soccer)
* Amiga Power Issue #31: Cannon Fodder Plus
  
  
##### Unsupported Versions
  
###### Cannon Fodder 2
  
Not officially supported and is not currently on the road map.
While the game can start it has no music or sound support, and a number of sprites are not implemented.
  
  
#### Original Engine Bugs

Issues in the original engine which have been fixed  
  
* Squad 2 and 3 grenade and rocket count did not refresh after use.
  
#### Thanks To

Sensible Software, for the original game  
Alessandro Petralia, for his valuable testing  
ScummVM, for the Amiga sound routines  
