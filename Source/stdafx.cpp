/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield <robcrossfield@gmail.com>
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

	cFodder* Fodder = new cFodder(SkipIntro);

	Fodder->Prepare();
	Fodder->Start( MapNumber );

	delete Fodder;

    return 0;

}

std::string local_PathGenerate( const std::string& pFile, const std::string& pPath, bool pData = true ) {
	std::stringstream	 filePathFinal;

	if (pData)
		filePathFinal << "Data/";
	else
		filePathFinal << "Saves/";

	if( pPath.size() )
		filePathFinal << pPath << "/";

	filePathFinal << pFile;

	return filePathFinal.str();
}

std::string local_FileMD5( const std::string& pFile, const std::string& pPath ) {
	md5_context ctx;
	size_t Size = 0;
	unsigned char MD5[16];

	uint8* File = local_FileRead( pFile, pPath, Size );
	if (!File)
		return "";

	md5_starts( &ctx );
	md5_update( &ctx, File, Size );
	md5_finish( &ctx, MD5 );
	delete[] File;

	std::string FinalMD5;
	FinalMD5.reserve(32);

	for (std::size_t i = 0; i != 16; ++i) {
	  FinalMD5 += "0123456789ABCDEF"[MD5[i] / 16];
	  FinalMD5 += "0123456789ABCDEF"[MD5[i] % 16];
	}

	return FinalMD5;
}
	

uint8 *local_FileRead( const std::string& pFile, const std::string& pPath, size_t& pFileSize ) {
	std::ifstream*		fileStream;
	uint8*				fileBuffer = 0;

	std::string finalPath = local_PathGenerate( pFile, pPath );

	// Attempt to open the file
	fileStream = new std::ifstream ( finalPath.c_str(), std::ios::binary );
	if (fileStream->is_open() != false) {

		// Get file size
		fileStream->seekg( 0, std::ios::end );
		pFileSize = (size_t)fileStream->tellg();
		fileStream->seekg( std::ios::beg );

		// Allocate buffer, and read the file into it
		fileBuffer = new uint8[pFileSize];
		fileStream->read( (char*)fileBuffer, pFileSize );
		if (!(*fileStream)) {
			delete fileBuffer;
			fileBuffer = 0;
		}

	}

	// Close the stream
	fileStream->close();
	delete fileStream;

	// All done ;)
	return fileBuffer;
}

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

std::string	tool_NumToString( uint32 pValue ) {
   std::stringstream Result;

   Result << pValue;
   return Result.str();
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
            strcpy(&path[0],"/Applications/DrCreep");
        #else
            getcwd(path, 2000);
        #endif
    #else
	strcpy(&path[0],"/usr/local/share/drcreep");
	#endif

	// Build the file path
	std::stringstream finalPath;

	if(pPath.size())
		finalPath << pPath;

	finalPath << pPath << "/";

	findType = pExtension;
		
    transform( findType.begin(), findType.end(), findType.begin(), ::toupper);

	int count = scandir(finalPath.str().c_str(), (dirent***) &directFiles, file_select, 0);
	
	for( int i = 0; i < count; ++i ) {

		results.push_back( std::string( directFiles[i]->d_name ) );
	}
	
	return results;
}

#endif