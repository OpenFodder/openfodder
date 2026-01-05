@echo off
setlocal

set "BUILD_DIR=%~dp0VS"
set "GENERATOR=Visual Studio 17 2022"

if "%~1"=="" (
    set "PLATFORM=x64"
) else (
    set "PLATFORM=%~1"
)

if "%~2"=="" (
    set "CONFIG=Release"
) else (
    set "CONFIG=%~2"
)

set "VS_ARCH=%PLATFORM%"
if /I "%VS_ARCH%"=="Win32" set "VS_ARCH=x86"

if not "%VSINSTALL%"=="" (
    set "VSDEVCMD=%VSINSTALL%\Common7\Tools\VsDevCmd.bat"
)

if not defined VSDEVCMD (
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    if exist "%VSWHERE%" (
        for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%i"
        if defined VSINSTALL set "VSDEVCMD=%VSINSTALL%\Common7\Tools\VsDevCmd.bat"
    )
)

if not defined VSDEVCMD (
    if exist "%ProgramFiles%\Microsoft Visual Studio" (
        for /d %%v in ("%ProgramFiles%\Microsoft Visual Studio\*") do (
            for /d %%e in ("%%v\*") do (
                if exist "%%e\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%%e\Common7\Tools\VsDevCmd.bat"
            )
        )
    )
)

if not defined VSDEVCMD (
    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio" (
        for /d %%v in ("%ProgramFiles(x86)%\Microsoft Visual Studio\*") do (
            for /d %%e in ("%%v\*") do (
                if exist "%%e\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%%e\Common7\Tools\VsDevCmd.bat"
            )
        )
    )
)

if not defined VSDEVCMD (
    echo Visual Studio developer environment not found.
    exit /b 1
)

call "%VSDEVCMD%" -arch=%VS_ARCH% -host_arch=x64
if errorlevel 1 exit /b 1

cmake -S "%~dp0.." -B "%BUILD_DIR%" -G "%GENERATOR%" -A "%PLATFORM%"
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%" --config "%CONFIG%"
if errorlevel 1 exit /b 1

echo Built %CONFIG% for %PLATFORM% in %BUILD_DIR%
