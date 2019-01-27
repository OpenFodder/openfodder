@echo off
if exist ..\..\.git\ (
    git --version >nul 2>&1 && (
	    git log -n 1 --pretty="const char* gitversion=\"%%h\";" > ..\..\Source\gitver.hpp
    ) || (echo const char* gitversion="unknown install git";> ..\..\Source\gitver.hpp)
) else (echo const char* gitversion="unknown no git folder";> ..\..\Source\gitver.hpp)
