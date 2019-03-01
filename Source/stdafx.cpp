/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stdafx.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <string>

std::shared_ptr<cResources> g_Resource;
std::shared_ptr<cWindow>    g_Window;
std::shared_ptr<cFodder>    g_Fodder;
std::shared_ptr<cDebugger>  g_Debugger;
std::shared_ptr<cResourceMan> g_ResourceMan;

const char gPathSeperator = '/';

#ifndef _OFED
#ifndef _OFBOT

int start(int argc, char *argv[]) {
    g_Debugger = std::make_shared<cDebugger>();
    g_Window = std::make_shared<cWindow>();
	g_ResourceMan = std::make_shared<cResourceMan>();
    g_Fodder = std::make_shared<cFodder>(g_Window);

	sFodderParameters Params;

	Params.ProcessINI();
	Params.ProcessCLI(argc, argv);

    g_Fodder->Prepare(Params);

    if (Params.mShowAbout) {
        g_Fodder->About();
        return 0;
    }

    if (Params.mUnitTesting) {
        cUnitTesting Testing;
        return Testing.Start() ? 0 : -1;
    }
    else {
        g_Fodder->Start();
        g_Fodder->mGame_Data.mDemoRecorded.save();
    }
    return 0;
}

int main(int argc, char *argv[]) {

    auto result = start(argc, argv);
    if (result == -1) {
        std::cout << "Press enter to continue\n";
        std::cin.get();
    }

    return result;
}
#endif
#endif

bool local_FileExists(const std::string& pPath) {
	struct stat info;

	if (stat(pPath.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else if (info.st_mode & S_IFMT)
		return true;

	return false;
}


/**
 *
 * @param pBuffer
 * @param pSize Number of words
 */
void tool_EndianSwap(uint8 *pBuffer, size_t pSize) {
	uint8 *pDest = pBuffer;

	pSize /= 2;

	while (pSize--) {
		uint8 al = *pBuffer++;
		uint8 ah = *pBuffer++;

		*pDest++ = ah;
		*pDest++ = al;
	}
}

std::string tool_StripLeadingZero(const std::string& pValue) {
	std::string Final = pValue;

	while (*Final.begin() == 0x30 && Final.length() > 1) {

		Final.erase(Final.begin());
	}

	return Final;
}

uint16 tool_DecimalToBinaryCodedDecimal(uint16 pDecimal) {

	return ((pDecimal / 10) << 4) + (pDecimal % 10);
}

#ifdef WIN32
#include "Windows.h"
#include <direct.h>

std::string local_getcwd() {
	char buff[1024];
	_getcwd(buff, 1024);	
	std::string cwd(buff);
	return cwd;
}

std::vector<std::string> local_DirectoryList(const std::string& pPath, const std::string& pExtension) {
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;
	std::vector<std::string> results;

	// Build the file path
	std::stringstream finalPath;

	if (pPath.size())
		finalPath << pPath;

	finalPath << "/*" << pExtension;

	size_t size = MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)finalPath.str().length(), 0, 0);
	WCHAR    *pathFin = new WCHAR[size + 1];
	memset(pathFin, 0, size + 1);

	size = MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)size, pathFin, (int)size);
	pathFin[size] = 0;

	if ((dhandle = FindFirstFile(pathFin, &fdata)) == INVALID_HANDLE_VALUE) {
		delete pathFin;
		return results;
	}

	delete pathFin;
	size_t tmp = 0;

	{
		char *file = new char[wcslen(fdata.cFileName) + 1];
		memset(file, 0, wcslen(fdata.cFileName) + 1);

		wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
		results.push_back(std::string(file));
		delete file;
	}

	while (1) {
		if (FindNextFile(dhandle, &fdata)) {
			char *file = new char[wcslen(fdata.cFileName) + 1];
			memset(file, 0, wcslen(fdata.cFileName) + 1);

			wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
			results.push_back(std::string(file));
			delete file;

		}
		else {
			if (GetLastError() == ERROR_NO_MORE_FILES) {
				break;
			}
			else {
				FindClose(dhandle);
				return results;
			}
		}
	}

	FindClose(dhandle);

	return results;
}

#else
#include <dirent.h>
std::string findType;

std::string local_getcwd() {
	char buff[1024];
	getcwd(buff, 1024);
	std::string cwd(buff);
	return cwd;
}

int file_select(const struct dirent *entry) {
	std::string name = entry->d_name;

	transform(name.begin(), name.end(), name.begin(), ::toupper);

	if (name.find(findType) == std::string::npos)
		return false;

	return true;
}

std::vector<std::string> local_DirectoryList(const std::string& pPath, const std::string& pExtension) {
	struct dirent		**directFiles;
	std::vector<std::string>		  results;

	// Build the file path
	std::stringstream finalPath;

	finalPath << pPath << "/";

	findType = pExtension;

	transform(findType.begin(), findType.end(), findType.begin(), ::toupper);

	int count = scandir(finalPath.str().c_str(), (dirent***)&directFiles, file_select, 0);

	for (int i = 0; i < count; ++i) {

		results.push_back(std::string(directFiles[i]->d_name));
	}

	transform(findType.begin(), findType.end(), findType.begin(), ::tolower);

	count = scandir(finalPath.str().c_str(), (dirent***)&directFiles, file_select, 0);

	for (int i = 0; i < count; ++i) {

		results.push_back(std::string(directFiles[i]->d_name));
	}

	return results;
}

#endif