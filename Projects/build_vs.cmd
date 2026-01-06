@echo off
setlocal EnableExtensions EnableDelayedExpansion

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

set "ENABLE_FFMPEG=ON"
if /I "%~3"=="noffmpeg" (
    set "ENABLE_FFMPEG=OFF"
)

set "VS_ARCH=%PLATFORM%"
if /I "%VS_ARCH%"=="Win32" set "VS_ARCH=x86"

set "CMAKE_ARGS="
if /I "%ENABLE_FFMPEG%"=="ON" (
    set "CMAKE_ARGS=%CMAKE_ARGS% -DOPENFODDER_ENABLE_FFMPEG=ON"
)
set "USE_TOOLCHAIN=0"

set "VCPKG_EXE="
if not "%VCPKG_ROOT%"=="" if exist "%VCPKG_ROOT%\\vcpkg.exe" set "VCPKG_EXE=%VCPKG_ROOT%\\vcpkg.exe"
if "%VCPKG_EXE%"=="" for /f "delims=" %%i in ('where vcpkg 2^>nul') do set "VCPKG_EXE=%%i"

if not "%VCPKG_EXE%"=="" (
    set "VCPKG_BIN_DIR=%~dp0"
    for %%i in ("%VCPKG_EXE%") do set "VCPKG_BIN_DIR=%%~dpi"
    set "VCPKG_ROOT_CAND=%VCPKG_ROOT%"
    if "!VCPKG_ROOT_CAND!"=="" set "VCPKG_ROOT_CAND=!VCPKG_BIN_DIR!"
    if not "!VCPKG_ROOT_CAND:~-1!"=="\" set "VCPKG_ROOT_CAND=!VCPKG_ROOT_CAND!\"

    pushd "%~dp0.."
    if /I "%PLATFORM%"=="x64" (
        "!VCPKG_EXE!" install --triplet x64-windows
        if errorlevel 1 exit /b 1
    )
    if /I "%PLATFORM%"=="Win32" (
        "!VCPKG_EXE!" install --triplet x86-windows
        if errorlevel 1 exit /b 1
    )
    popd

    set "CMAKE_ARGS=!CMAKE_ARGS! -DCMAKE_TOOLCHAIN_FILE=!VCPKG_ROOT_CAND!scripts\\buildsystems\\vcpkg.cmake"
    set "USE_TOOLCHAIN=1"
    if /I "%PLATFORM%"=="x64" set "CMAKE_ARGS=!CMAKE_ARGS! -DVCPKG_TARGET_TRIPLET=x64-windows"
    if /I "%PLATFORM%"=="Win32" set "CMAKE_ARGS=!CMAKE_ARGS! -DVCPKG_TARGET_TRIPLET=x86-windows"
) else (
    echo vcpkg not found in PATH and VCPKG_ROOT is not set. Skipping vcpkg install.
)

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

if "%USE_TOOLCHAIN%"=="1" if exist "%BUILD_DIR%\\CMakeCache.txt" (
    del /f /q "%BUILD_DIR%\\CMakeCache.txt"
)

cmake -S "%~dp0.." -B "%BUILD_DIR%" -G "%GENERATOR%" -A "%PLATFORM%" %CMAKE_ARGS%
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%" --config "%CONFIG%"
if errorlevel 1 exit /b 1

echo Built %CONFIG% for %PLATFORM% in %BUILD_DIR%
