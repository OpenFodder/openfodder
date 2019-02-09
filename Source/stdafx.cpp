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
#include "md5.hpp"
#include "Utils/cxxopts.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

std::shared_ptr<cResources> g_Resource;
std::shared_ptr<cWindow>    g_Window;
std::shared_ptr<cFodder>    g_Fodder;
std::shared_ptr<cDebugger>  g_Debugger;

#ifdef _WIN32
    const char gPathSeperator = '/';
#else
    const char gPathSeperator = '/';
#endif

#ifndef _OFED
#ifndef _OFBOT

int start(int argc, char *argv[]) {
    sFodderParameters Params;
    g_Debugger = std::make_shared<cDebugger>();
    g_Window = std::make_shared<cWindow>();
    g_Fodder = std::make_shared<cFodder>(g_Window);

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
        Params.mSkipIntro = result["skipintro"].as<bool>();
        Params.mSkipService = result["skipservice"].as<bool>();
        Params.mSkipBriefing = result["skipbriefing"].as<bool>();

        Params.mUnitTesting = result["unit-test"].as<bool>();

        Params.mCampaignName = result["campaign"].as<std::string>();
        Params.mMissionNumber = result["mission"].as<std::uint32_t>();
        Params.mPhaseNumber = result["phase"].as<std::uint32_t>();
        Params.mWindowMode = result["window"].as<bool>();
        Params.mRandom = result["random"].as<bool>();
        Params.mDisableSound = result["nosound"].as<bool>();
        Params.mPlayground = result["playground"].as<bool>();

		Params.mSpritesMax = result["max-sprite"].as<uint32_t>();
		Params.mSpawnEnemyMax = result["max-spawn"].as<uint32_t>();
		Params.mSleepDelta = result["sleep-delta"].as<uint32_t>();

		Params.mCheatsEnabled = result["cheats"].as<bool>();

		// Cheats perm enabled in debug build
#ifdef _DEBUG
		Params.mCheatsEnabled = true;
#endif

        if (Params.mMissionNumber || Params.mPhaseNumber) {
            Params.mSkipRecruit = true;
            Params.mSkipIntro = true;
        }

        if (result["pc"].as<bool>())
            Params.mDefaultPlatform = ePlatform::PC;
        if (result["amiga"].as<bool>())
            Params.mDefaultPlatform = ePlatform::Amiga;

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

std::string gLocalBasePath;

void local_BasePathGenerate() {
    std::stringstream filePathFinal;

    // TODO: This needs improvements for LINUX/UNIX

#ifdef WIN32
    const char* path = std::getenv("USERPROFILE");
    if (path)
        filePathFinal << path;

    filePathFinal << "/Documents/OpenFodder/";
#else
    std::string FinalPath;

    // Lets find a base data folder
    const char* path = std::getenv("XDG_DATA_DIRS");
    if (path) {
        std::vector<std::string> Paths;

        std::stringstream ss;
        ss << path;
        while (ss.good()) {
            std::string substr;
            std::getline(ss, substr, ':');
            Paths.push_back(substr);
        }

        // Dodgy loop and test all paths
        for (auto& CheckPath : Paths) {
            FinalPath = CheckPath;

            filePathFinal << FinalPath << "/OpenFodder/";

            // If the path exists, abort the search
            if (local_FileExists(filePathFinal.str()))
                break;

            FinalPath = "";
            filePathFinal.str("");
        }

    }

    // No path found? check the home directory
    if (!FinalPath.size()) {
        // Test the home directory
        path = std::getenv("HOME");

        if (path) {
            FinalPath = path;
            FinalPath.append("/.local/share/");
        }
    }

    // Fall back just incase
    if (!FinalPath.size())
        FinalPath = "/usr/local/share/";

    filePathFinal << FinalPath << "OpenFodder/";

#endif

    // If this base path doesnt exist, then clear it and fall back to the exe working path
    if (!local_FileExists(filePathFinal.str()))
        filePathFinal.str("");

    gLocalBasePath = filePathFinal.str();
}

std::string local_PathGenerate( const std::string& pFile, const std::string& pPath, eDataType pDataType = eData) {
	std::stringstream	 filePathFinal;

    if (!gLocalBasePath.size())
        local_BasePathGenerate();

    if (pDataType != eNone)
        filePathFinal << gLocalBasePath;

	switch (pDataType) {
	case eData:
		filePathFinal << "Data" << gPathSeperator;
		break;

	case eSave:
		filePathFinal << "Saves" << gPathSeperator;
		break;

	case eCampaign:
		filePathFinal << "Campaigns" << gPathSeperator;
		break;

    case eTest:
        filePathFinal << "Tests" << gPathSeperator;
        break;

    case eRoot:
	case eNone:
	default:
		break;
	}
	if( pPath.size() )
		filePathFinal << pPath << gPathSeperator;

	filePathFinal << pFile;

	return filePathFinal.str();
}

std::string local_FileMD5( const std::string& pFile, const std::string& pPath ) {
	md5_context ctx;
	unsigned char MD5[16];

	auto File = local_FileRead( pFile, pPath );
	if (!File->size())
		return "";

	md5_starts( &ctx );
	md5_update( &ctx, File->data(), (uint32) File->size() );
	md5_finish( &ctx, MD5 );

	std::string FinalMD5;
	FinalMD5.reserve(32);

	for (size_t i = 0; i != 16; ++i) {
	  FinalMD5 += "0123456789ABCDEF"[MD5[i] / 16];
	  FinalMD5 += "0123456789ABCDEF"[MD5[i] % 16];
	}

	return FinalMD5;
}
	

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

tSharedBuffer local_FileRead( const std::string& pFile, const std::string& pPath, eDataType pDataType ) {
	std::ifstream*	fileStream;
	auto			fileBuffer = std::make_shared<std::vector<uint8_t>>();

	std::string finalPath;
	
	finalPath = local_PathGenerate(pFile, pPath, pDataType );

	// Attempt to open the file
 	fileStream = new std::ifstream ( finalPath.c_str(), std::ios::binary );
	if (fileStream->is_open() != false) {

		// Get file size
		fileStream->seekg( 0, std::ios::end );
		fileBuffer->resize( static_cast<const unsigned int>(fileStream->tellg()) );
		fileStream->seekg( std::ios::beg );

		// Allocate buffer, and read the file into it
		fileStream->read( (char*) fileBuffer->data(), fileBuffer->size() );
		if (!(*fileStream))
			fileBuffer->clear();
	}

	// Close the stream
	fileStream->close();
	delete fileStream;

	// All done ;)
	return fileBuffer;
}

/**
 *
 * @param pBuffer
 * @param pSize Number of words
 */
void tool_EndianSwap( uint8 *pBuffer, size_t pSize ) {
	uint8 *pDest = pBuffer;
	
	pSize /= 2;

	while( pSize-- ) {
		uint8 al = *pBuffer++;
		uint8 ah = *pBuffer++;

		*pDest++ = ah;
		*pDest++ = al;
	}
}

std::string tool_StripLeadingZero( const std::string& pValue ) {
	std::string Final = pValue;

	while (*Final.begin() == 0x30 && Final.length() > 1 ) {

		Final.erase( Final.begin() );
	}

	return Final;
}

uint16 tool_DecimalToBinaryCodedDecimal(uint16 pDecimal) {

	return ((pDecimal/10)<<4)+(pDecimal%10);
}

#ifdef WIN32
#include "Windows.h"
#include <direct.h>

std::vector<std::string> local_DirectoryList( const std::string& pPath, const std::string& pExtension) {
    WIN32_FIND_DATA fdata;
    HANDLE dhandle;
	std::vector<std::string> results;

	char path[2000];
	_getcwd(path, 2000);

	// Build the file path
	std::stringstream finalPath;

	if(pPath.size())
		finalPath << pPath;

	finalPath << "/*" << pExtension;

	size_t size = MultiByteToWideChar( 0,0, finalPath.str().c_str(), (int) finalPath.str().length(), 0, 0);
	WCHAR    *pathFin = new WCHAR[ size + 1];
	memset( pathFin, 0, size + 1);

	size = MultiByteToWideChar( 0,0, finalPath.str().c_str(), (int) size, pathFin, (int) size);
	pathFin[size] = 0;

	if((dhandle = FindFirstFile(pathFin, &fdata)) == INVALID_HANDLE_VALUE) {
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

    while(1) {
        if(FindNextFile(dhandle, &fdata)) {
			char *file = new char[ wcslen(fdata.cFileName) + 1];
			memset(file, 0, wcslen(fdata.cFileName) + 1 );
			
			wcstombs_s( &tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName) );
			results.push_back(std::string(file));
			delete file;
				
        } else {
                if(GetLastError() == ERROR_NO_MORE_FILES) {
                        break;
                } else {
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

int file_select(const struct dirent *entry) {
	std::string name = entry->d_name;

	transform( name.begin(), name.end(), name.begin(), ::toupper );
   	
	if( name.find( findType ) == std::string::npos )
		return false;
	
	return true;
}

std::vector<std::string> local_DirectoryList( const std::string& pPath, const std::string& pExtension) {
	struct dirent		**directFiles;
	std::vector<std::string>		  results;

	// Build the file path
	std::stringstream finalPath;

	finalPath << pPath << "/";

	findType = pExtension;
		
    transform( findType.begin(), findType.end(), findType.begin(), ::toupper);

	int count = scandir(finalPath.str().c_str(), (dirent***) &directFiles, file_select, 0);
	
	for( int i = 0; i < count; ++i ) {

		results.push_back( std::string( directFiles[i]->d_name ) );
	}

	transform( findType.begin(), findType.end(), findType.begin(), ::tolower );

	count = scandir( finalPath.str().c_str(), (dirent***)&directFiles, file_select, 0 );

	for (int i = 0; i < count; ++i) {

		results.push_back( std::string( directFiles[i]->d_name ) );
	}

	return results;
}

#endif
