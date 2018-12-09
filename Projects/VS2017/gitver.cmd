@echo off
git log -n 1 --pretty="const char* gitversion=\"%%h\";" > ..\..\Source\gitver.hpp
