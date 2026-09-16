@echo off
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build_vs.ps1" %*
exit /b %ERRORLEVEL%
