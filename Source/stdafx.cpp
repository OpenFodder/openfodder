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
#include "Utils/cxxopts.hpp"
#include "Utils/ini.hpp"

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

std::string str_to_lower(std::string pStr) {
	std::transform(pStr.begin(), pStr.end(), pStr.begin(), ::tolower);
	return pStr;
}

sFodderParameters parseini() {
	INI<> ini("openfodder.ini", false);
	sFodderParameters params;

	if (!ini.parse())
		return params;

	// Section: Openfodder
	{
		if (ini.select("openfodder")) {
			if (ini.get("window", "false") == "true")
				params.mWindowMode = true;
			else
				params.mWindowMode = false;

			if (ini.get("cheats", "false") == "true")
				params.mCheatsEnabled = true;
			else
				params.mCheatsEnabled = false;

			if (ini.get("scale", "false") == "auto")
				params.mWindowScale = 0;
			else {
				params.mWindowScale = ini.get("scale", 0);
			}
		}
	}

	// Section: Engine
	{
		if (ini.select("engine")) {
			auto platform = str_to_lower(ini.get("platform", ""));
			if (platform == "amiga")
				params.mDefaultPlatform = ePlatform::Amiga;
			if (platform == "pc")
				params.mDefaultPlatform = ePlatform::PC;

			auto maxsprite = ini.get("maxsprite", 0);
			if(maxsprite)
				params.mSpritesMax = maxsprite;
			auto maxspawn = ini.get("maxspawn", 0);
			if(maxspawn)
				params.mSpawnEnemyMax = maxspawn;
		}
	}

	// Section: Skip
	{
		if (ini.select("skip")) {
			if (ini.get("intro", "false") == "true")
				params.mSkipIntro = true;

			if (ini.get("briefing", "false") == "true")
				params.mSkipBriefing = true;

			if (ini.get("service", "false") == "true")
				params.mSkipService = true;

			if (ini.get("hill", "false") == "true")
				params.mSkipRecruit = true;
		}
	}

	if (ini.select("paths")) {

		for (auto& path : ini["paths"]) {

			g_ResourceMan->addDir(path.second);
		}
	}

	return params;
}

#ifndef _OFED
#ifndef _OFBOT

int start(int argc, char *argv[]) {
    g_Debugger = std::make_shared<cDebugger>();
    g_Window = std::make_shared<cWindow>();
	g_ResourceMan = std::make_shared<cResourceMan>();
    g_Fodder = std::make_shared<cFodder>(g_Window);

	sFodderParameters Params = parseini();

    cxxopts::Options options("OpenFodder", "War has never been so much fun");
    options.allow_unrecognised_options();
    options.add_options()
        ("about",         "About",                cxxopts::value<bool>()->default_value("false"))
        ("h,help",        "Help",                 cxxopts::value<bool>()->default_value("false")  )
        ("pc",            "Default to PC platform data", cxxopts::value<bool>()->default_value("false"))
        ("amiga",         "Default to Amiga platform data", cxxopts::value<bool>()->default_value("false"))
        ("w,window",	  "Start in window mode", cxxopts::value<bool>()->default_value("false"))

		("cheats",		  "Enable cheat keys", cxxopts::value<bool>()->default_value("false"))
		("max-sprite",	  "Set the maximum sprites", cxxopts::value<std::uint32_t>()->default_value("45"), "45")
		("max-spawn",	  "Set the maximum spawn", cxxopts::value<std::uint32_t>()->default_value("10"), "10")
		("sleep-delta",	  "Set the engine speed", cxxopts::value<std::uint32_t>()->default_value("2"), "2")

        ("demo-record",   "Record Demo",    cxxopts::value<std::string>()->default_value(""), "\"Demo File\"")
        ("demo-record-all", "Record Demo")
        ("demo-play",     "Play Demo",      cxxopts::value<std::string>()->default_value(""), "\"Demo File\"")
        
        ("unit-test",     "Run Tests",      cxxopts::value<bool>()->default_value("false"))
        ("unit-test-headless", "Run Tests, with no output", cxxopts::value<bool>()->default_value("false"))

        ("appveyor",      "Output for appveyor", cxxopts::value<bool>()->default_value("false"))
        ("nosound",       "Disable sound output", cxxopts::value<bool>()->default_value("false"))
        ("playground",    "Sprite playground", cxxopts::value<bool>()->default_value("false"))

        ("skipintro",      "Skip all game intros", cxxopts::value<bool>()->default_value("false"))
        ("skipbriefing",   "Skip mission briefing", cxxopts::value<bool>()->default_value("false"))
        ("skipservice",    "Skip mission debriefing", cxxopts::value<bool>()->default_value("false"))
		("skiphill",		"Skip the hill", cxxopts::value<bool>()->default_value("false"))

        ("list-campaigns", "List available campaigns", cxxopts::value<bool>()->default_value("false"))
        ("c,campaign",  "Starting campaign",        cxxopts::value<std::string>()->default_value(""), "\"name\"" )
        ("m,mission",   "Starting mission",         cxxopts::value<std::uint32_t>()->default_value("0"), "1" )
        ("p,phase",     "Starting phase",           cxxopts::value<std::uint32_t>()->default_value("0"), "2" )
        ("r,random",    "Generate and play a random map",    cxxopts::value<bool>()->default_value("false") )
        ;


    try {
        auto result = options.parse(argc, argv);

        if (result["appveyor"].as<bool>()) {
            Params.mAppVeyor = true;
        }

        if (result["help"].as<bool>() == true) {
            g_Debugger->Notice( options.help() );
            return -1;
        }

		if(result.count("pc"))
			Params.mDefaultPlatform = ePlatform::PC;
		if (result.count("amiga"))
			Params.mDefaultPlatform = ePlatform::Amiga;

        if (result["list-campaigns"].as<bool>() == true) {
            g_Debugger->Notice("\nAvailable Campaigns\n\n");

            for (auto& Name : g_Fodder->mVersions->GetCampaignNames())
                g_Debugger->Notice(Name);

            return -1;
        }

        if (result.count("demo-record-all")) {
            Params.mDemoRecord = true;
            Params.mDemoFile = "-";
        }
        else {
            Params.mDemoFile = result["demo-record"].as<std::string>();
            if (Params.mDemoFile.size())
                Params.mDemoRecord = true;
            else {
                Params.mDemoFile = result["demo-play"].as<std::string>();
                if (Params.mDemoFile.size())
                    Params.mDemoPlayback = true;
            }
        }
        
        Params.mShowAbout = result["about"].as<bool>();

		if (result.count("skipintro"))
			Params.mSkipIntro = result["skipintro"].as<bool>();
		if (result.count("skipservice"))
			Params.mSkipService = result["skipservice"].as<bool>();
		if (result.count("skipbriefing"))
			Params.mSkipBriefing = result["skipbriefing"].as<bool>();
		if (result.count("skiphill"))
			Params.mSkipRecruit = result["skiphill"].as<bool>();

        Params.mUnitTesting = result["unit-test"].as<bool>();

        Params.mCampaignName = result["campaign"].as<std::string>();
        Params.mMissionNumber = result["mission"].as<std::uint32_t>();
        Params.mPhaseNumber = result["phase"].as<std::uint32_t>();

		if(result.count("window"))
	        Params.mWindowMode = result["window"].as<bool>();

        Params.mRandom = result["random"].as<bool>();
        Params.mDisableSound = result["nosound"].as<bool>();
        Params.mPlayground = result["playground"].as<bool>();

		if(result.count("max-sprite"))
			Params.mSpritesMax = result["max-sprite"].as<uint32_t>();
		if (result.count("max-spawn"))
			Params.mSpawnEnemyMax = result["max-spawn"].as<uint32_t>();

		Params.mSleepDelta = result["sleep-delta"].as<uint32_t>();

		Params.mCheatsEnabled = result["cheats"].as<bool>();

		if (Params.mSpritesMax < 16)
			Params.mSpritesMax = 16;

		// Cheats perm enabled in debug build
#ifdef _DEBUG
		Params.mCheatsEnabled = true;
#endif

        if (Params.mMissionNumber || Params.mPhaseNumber) {
            Params.mSkipRecruit = true;
            Params.mSkipIntro = true;
        }

        if (result["unit-test-headless"].as<bool>()) {
            Params.mUnitTesting = true;
            Params.mDisableVideo = true;
            Params.mDisableSound = true;

            g_Window = std::make_shared<cWindowNull>();
            g_Fodder->mWindow = g_Window;
        }

        if (Params.mUnitTesting) {
            Params.mWindowMode = true;
        }
#ifdef _DEBUG
        Params.mWindowMode = true;
#endif
    } catch (...) {
        g_Debugger->Notice(options.help());
        return -1;
    }
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