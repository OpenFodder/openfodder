@echo off
rem Build with the default WSL distribution; works from any current directory.
wsl.exe --cd "%~dp0" bash ./build.sh %*
exit /b %errorlevel%
