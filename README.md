# Open Fodder
[![Build Status](https://api.travis-ci.com/OpenFodder/openfodder.svg?branch=master)](https://travis-ci.com/OpenFodder/openfodder)
[![Build status](https://ci.appveyor.com/api/projects/status/7pc2r2qe4niv4848?svg=true)](https://ci.appveyor.com/project/segrax/openfodder)

  
Come for a chat over at our [Discord](https://discord.gg/4mX2wFM)  

![image](https://user-images.githubusercontent.com/1327406/49718991-f1bd6e00-fcaf-11e8-88e3-68136828c2d1.png)

#### About

Cannon Fodder is an action-strategy shoot 'em up game developed by Sensible Software and published by Virgin Interactive.

The game is military-themed and based on shooting action but with a strategy game-style 
control system. The player directs troops through numerous missions, battling enemy infantry, vehicles and installations.

Open Fodder is an open source version of the Cannon Fodder engine, for modern operating systems.
  
* Website: [http://openfodder.com](http://openfodder.com/)
* Discord: [https://discord.gg/4mX2wFM](https://discord.gg/4mX2wFM)
* Facebook:[https://www.facebook.com/openfodder/](https://www.facebook.com/openfodder/)
* Twitter: [https://twitter.com/segrax](https://twitter.com/segrax)


#### Play now

You can play the demos in your browser [here](https://s3.amazonaws.com/openfodder/OpenFodder.html).  

 
#### Map Editor

See [OpenFodder Editor](https://github.com/OpenFodder/editor) for the campaign and map editor.


#### Scripting

Open Fodder includes a Javascript interpreter for the purposes of creating randomized maps, 
a number of scripts are provided which implement various functionality, such as placing of random hostages, rescue tents,
and a helicopter if required to rescue to hostages.

Customisations and additions can be made to any of the scripts

#### Media

[Open Fodder 1.5 Video](https://youtu.be/f16FarouchY)  
[Open Fodder 1.4 Video](https://youtu.be/jb4TmM9zcr4)  
[Open Fodder 1.3 Video - Amiga Format Special](https://www.youtube.com/watch?v=aSGOTSw-LlI)  
[Open Fodder 0.8 Video](https://www.youtube.com/watch?v=7AjELdOzoaw)  

[Open Fodder Smoke Testing](https://youtu.be/z-Lct_oxu0k)

#### Installation

Please see [INSTALL.md](https://github.com/OpenFodder/openfodder/blob/master/INSTALL.md)
  
For the most recent Windows build, you may obtain the executable built during testing from [Here](https://s3.amazonaws.com/openfodder-builds/OpenFodder-latest.zip) 
  
#### How to play

Open Fodder supports six demos from the Amiga platform, and three retail releases (See below for supported versions).
  
##### Command Line Parameters
```
  OpenFodder [OPTION...]

      --about                   About
  -h, --help                    Help
      --pc                      Default to PC platform data
      --amiga                   Default to Amiga platform data
      --engine cf1              Default to engine (single map/random)
                                (default: cf1)
      --columns 22              Number of camera columns (default: 22)
      --rows 16                 Number of camera rows (default: 16)
  -w, --window                  Start in window mode
      --window-scale arg        Set the window scale (default: 0)
      --cheats                  Enable cheat keys

      --max-sprite 45           Set the maximum sprites (default: 45)
      --max-spawn 10            Set the maximum spawn (default: 10)
      --sleep-delta 2           Set the engine speed (default: 2)
      --demo-record "Demo File"
                                Record Demo (default: )
      --demo-record-all         Record Demo
      --demo-play "Demo File"   Play Demo (default: )
      --unit-test               Run Tests
      --unit-test-headless      Run Tests, with no output

      --nosound                 Disable sound output
      --playground              Sprite playground
	  
      --skipintro               Skip all game intros
      --skipbriefing            Skip mission briefing
      --skipservice             Skip mission debriefing
      --skiphill                Skip the hill

      --list-campaigns          List available campaigns
  -c, --campaign "name"         Starting campaign (default: )
  -m, --mission 1               Starting mission (default: 0)
  -p, --phase 2                 Starting phase (default: 0)

      --single-map "MyMap"      Play a single map (default: )
  -r, --random                  Generate and play a random map
      --random-save "MyMap"     Generate and save a random map (default: )
      --script "script.js"      Name of script to execute (default: )				  
```
  
##### Hot Keys
  
```
* F1:    Switch to Amiga version (if available)
* F2:    Switch to PC version (if available)
* F9:    Make current squad invincible (with --cheats enabled)
* F10:   Complete current map (with --cheats enabled)
* F11:   Toggle full screen
* +:     Increase window size
* -:     Decrease window size  

* ESC:   Back menu/abort phase
* M:     Show map (Retail releases only)
* P:     Pause game
* 1:     Select squad 1
* 2:     Select squad 2
* 3:     Select squad 3
* Space: Switch weapon
```
  
#### Purchasing the retail release

Cannon Fodder can be purchased from [Good Old Games](http://www.gog.com), directly at [Cannon Fodder](http://www.gog.com/game/cannon_fodder) and [Cannon Fodder 2](http://www.gog.com/game/cannon_fodder_2)  
  
  
#### Supported Versions
  
##### Cannon Fodder
  
* Amiga
* Amiga CD32
* Dos CD
  
##### Cannon Fodder 2
  
* Amiga
* Dos CD
  
##### Demos
  
Six Amiga magazine coverdisk demos and one PC demo are included in the data pack.  
  
* Amiga The One Issue #68: (Jun 1993)
* Amiga Power   Issue #31: (Nov 1993) Cannon Fodder Plus
* Amiga Action  Issue #51: (Dec 1993)
* Amiga Format  Issue #54: (Dec 1993) Christmas Special (Cannon Soccer)
* Amiga Power   Issue #45: (Jan 1995) Alien Levels
* Amiga Format  Issue #68: (Feb 1995) Not Very Festive Fodder
* PC    Format  CD-ROM #1: (May 1994)
  
#### Thanks To

Sensible Software, for the original game  
Alessandro Petralia, for his valuable testing  
ScummVM, for the Amiga sound routines  


#### License
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FOpenFodder%2Fopenfodder.svg?type=large)](https://app.fossa.io/projects/git%2Bgithub.com%2FOpenFodder%2Fopenfodder?ref=badge_large)
