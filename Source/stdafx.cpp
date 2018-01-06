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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef _OFED
int main(int argc, char *args[]) {
	bool SkipIntro = false;
	int16 MapNumber = 0;


	if (argc > 1) {

		if (strcmp( args[1], "skipintro" ) == 0)
			SkipIntro = true;

		if (strcmp( args[2], "map" ) == 0) {
			MapNumber = atoi( args[3] );
		}
	}

	cFodder* Fodder = new cFodder(new cWindow(), SkipIntro);

	Fodder->Prepare();
	Fodder->Start( MapNumber );

	delete Fodder;

    return 0;

}
#endif

std::string local_PathGenerate( const std::string& pFile, const std::string& pPath, eDataType pDataType = eData) {
	std::stringstream	 filePathFinal;

    // TODO: This needs improvements for LINUX/UNIX
    if (pDataType != eNone) {
#ifdef WIN32
        filePathFinal << getenv("USERPROFILE") << "\\Documents\\OpenFodder\\";

#else

        // Lets find a base data folder
        std::string path = std::getenv("XDG_DATA_HOME");
        if (!path.size()) {
            path = std::getenv("HOME");

            if (path.size())
                path.append("/.local/share/");
        }

        // Fall back just incase
        if(!path.size())
            path = "/usr/local/share/";

        filePathFinal << path << "OpenFodder/";

    #endif

        // If this base path doesnt exist, then clear it and fall back to the exe working path
        if (!local_FileExists(filePathFinal.str()))
            filePathFinal.str("");
    }

	switch (pDataType) {
	case eData:
		filePathFinal << "Data/";
		break;

	case eSave:
		filePathFinal << "Saves/";
		break;

	case eCampaign:
		filePathFinal << "Campaigns/";
		break;

	case eNone:
	default:
		break;
	}
	if( pPath.size() )
		filePathFinal << pPath << "/";

	filePathFinal << pFile;

	return filePathFinal.str();
}

std::string local_FileMD5( const std::string& pFile, const std::string& pPath ) {
	md5_context ctx;
	size_t Size = 0;
	unsigned char MD5[16];

	auto File = local_FileRead( pFile, pPath );
	if (!File->size())
		return "";

	md5_starts( &ctx );
	md5_update( &ctx, File->data(), (uint32) File->size() );
	md5_finish( &ctx, MD5 );

	std::string FinalMD5;
	FinalMD5.reserve(32);

	for (std::size_t i = 0; i != 16; ++i) {
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
		fileBuffer->resize( fileStream->tellg() );
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
	char *file = new char[ wcslen(fdata.cFileName) + 1];
	memset(file, 0, wcslen(fdata.cFileName) + 1 );
	size_t tmp = 0;

	wcstombs_s( &tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName) );
    results.push_back(std::string(file));
	delete file;

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

	char path[2000];
	#ifndef FREEBSD
        #ifdef _MACOSX
            strcpy(&path[0],"/Applications/OpenFodder");
        #else
            getcwd(path, 2000);
        #endif
    #else
	strcpy(&path[0],"/usr/local/share/openfodder");
	#endif

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
