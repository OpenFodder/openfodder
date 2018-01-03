;	Open Fodder
; 	-------------------------------
; 
;	Copyright (C) 2008-2018 Open Fodder
;  
;	
;
; $Id$
; 
;
; NSIS Installer script

SetCompressor lzma
;--------------------------------
;Include Modern UI

  !include MUI2.nsh
  !include LogicLib.nsh
  
;--------------------------------
;General
	;XPStyle on
	
	RequestExecutionLevel admin
	Name	"Open Fodder"
	Caption "Open Fodder Installer"
	OutFile  .\packages\OpenFodder-Installer.exe

	;Default installation folder
	InstallDir "$PROGRAMFILES64\OpenFodder"
  
	;Get installation folder from registry if available
	InstallDirRegKey HKLM "Software\OpenFodder" "Install_Dir"

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages
  ; Welcome page
  !insertmacro MUI_PAGE_WELCOME

  !insertmacro MUI_PAGE_LICENSE "..\..\gpl-3.0.txt"
  ;!insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\OpenFodder" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Open Fodder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH 

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Copy Files" drcreepInst

  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  
  File ".\packages\VC_redist.x64.exe"
  File "..\..\gpl-3.0.txt"
  File "..\..\Readme.md"
  File "..\..\Run\OpenFodder.exe"
  File "..\..\Run\SDL2.dll"
  File "..\..\Run\SDL2_mixer.dll"
  
  SetOutPath "$PROFILE\Documents\OpenFodder"
  File /r "..\..\Run\Campaigns"
  File /r "..\..\Run\Data"
  File /r "..\..\Run\Saves"

  SetOutPath "$INSTDIR"
  ;Store installation folder
  WriteRegStr HKLM "Software\OpenFodder" "Install_Dir" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
 	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    	;Create shortcuts
     CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
     CreateShortCut "$SMPROGRAMS\$StartMenuFolder\OpenFodder.lnk" "$INSTDIR\OpenFodder.exe"
     CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    	!insertmacro MUI_STARTMENU_WRITE_END

	CreateShortCut "$DESKTOP\OpenFodder.lnk" "$INSTDIR\OpenFodder.exe"

	MessageBox MB_YESNO|MB_ICONQUESTION "Install Microsoft Visual C++ 2017 64bit Redistributable Package?" IDNO NoRunVC
    Exec "$INSTDIR\VC_redist.x64.exe"
	
	
  NoRunVC:
	MessageBox MB_YESNO|MB_ICONQUESTION "Open the OpenFodder Data Folder" IDNO NoOpenData
	ExecShell "open" "$PROFILE\Documents\OpenFodder"
	
  NoOpenData:
SectionEnd 

;--------------------------------
;Descriptions

	;Language strings
	 LangString DESC_creepInst ${LANG_ENGLISH} "Main Program Installation"

	;Assign language strings to sections
	!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${creepInst} $(DESC_creepInst)
	!insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------

Function .onInit
	# the plugins dir is automatically deleted when the installer exits
	InitPluginsDir
	#File /oname=$PLUGINSDIR\splash.bmp "splash.bmp"
	#optional
	#File /oname=$PLUGINSDIR\splash.wav "C:\myprog\sound.wav"

	#splash::show 2000 $PLUGINSDIR\splash

	Pop $0 ; $0 has '1' if the user closed the splash screen early,

FunctionEnd

Function .onInstSuccess

ExecShell open 'notepad $INSTDIR\Readme.md'

MessageBox MB_YESNO|MB_ICONQUESTION "Launch Open Fodder?" IDNO NoRun
    Exec "$INSTDIR\OpenFodder.exe"
  NoRun:
FunctionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"
!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

    Delete "$INSTDIR\VC_redist.x64.exe"
    Delete "$INSTDIR\gpl-3.0.txt"
    Delete "$INSTDIR\OpenFodder.exe"
    Delete "$INSTDIR\Uninstall.exe"
    Delete "$INSTDIR\Readme.md"
    Delete "$INSTDIR\SDL2.dll"
	Delete "$INSTDIR\SDL2_mixer.dll"
    Delete "$INSTDIR\data\*"
    Delete "$INSTDIR\data\castles\*"

	RMDir "$INSTDIR\data\castles"
	RMDir "$INSTDIR\data"
	RMDir "$INSTDIR"
		
	IfFileExists $INSTDIR 0 no 
		MessageBox MB_YESNO|MB_ICONQUESTION "Remove all files in the Open Fodder directory? " IDNO no
			
			Delete $INSTDIR\*.*
			RMDir /r $INSTDIR
			
	  IfFileExists $INSTDIR 0 no 		
      MessageBox MB_OK|MB_ICONEXCLAMATION "Note: $INSTDIR could not be removed."
no:
   
  Delete "$DESKTOP\OpenFodder.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\OpenFodder.lnk" 
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"
 
  DeleteRegKey /ifempty HKCU "Software\OpenFodder"

SectionEnd
