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

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <functional>
#include <memory>
#include <cstring>

#include <SDL.h>
#ifndef EMSCRIPTEN
#include <SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif

#include "Types.hpp"

typedef std::shared_ptr<std::vector<uint8>> tSharedBuffer;

enum eDataType {
	eData		= 0,
	eSave		= 1,
	eCampaign	= 2,
	eNone		= 3,
};

std::vector<std::string> local_DirectoryList( const std::string& pPath, const std::string& pExtension );

bool				local_FileExists(const std::string& pPath);
std::string			local_FileMD5( const std::string& pFile, const std::string& pPath );
tSharedBuffer 		local_FileRead( const std::string& pFile, const std::string& pPath, eDataType pDataType = eData);
std::string			local_PathGenerate(  const std::string& pFile, const std::string& pPath, eDataType pDataType );

void		tool_EndianSwap( uint8* pBuffer, size_t pSize );
std::string tool_StripLeadingZero( const std::string& pValue );
uint16		tool_DecimalToBinaryCodedDecimal( uint16 pDecimal );


// Read a BE word from the buffer
inline uint16 readBEWord( const void *buffer ) {
	const uint8* bytes = (const uint8*) buffer;

	return uint16((bytes[0] << 8) + bytes[1]);
}

inline void writeBEWord( const void *buffer, uint16 pValue ) {
	uint8* bytes = (uint8*) buffer;

	bytes[0] = (uint8) (pValue >> 8);
	bytes[1] = (uint8) (pValue & 0xFF);
}


inline uint32 readBEDWord( const void *buffer ) {
	const uint8* bytes = (const uint8*) buffer;

	return uint32((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + (bytes[3]));
}

// Read a word from the buffer
inline uint32 readLEDWord( const void *buffer ) {
	const uint32* wordBytes = (const uint32*) buffer;
	return *wordBytes;
}

// Read a word from the buffer
inline uint16 readLEWord( const void *buffer ) {
	const uint16* wordBytes = (const uint16*) buffer;
	return *wordBytes;
}

inline void writeLEWord( const void *buffer, uint16 pValue ) {
	uint16* wordBytes = (uint16 *) buffer;
	*wordBytes = pValue;
}

#ifndef _WIN32
	#include <unistd.h>
    #define	 Sleep( a ) usleep( a * 1000 );

	#define _stricmp( a, b ) strcmp(a, b)
#endif

#include "Singleton.hpp"

#include "Dimension.hpp"
#include "Position.hpp"
#include "Event.hpp"

#include "Surface.hpp"
#include "Resources.hpp"
#include "PC/Resource_PC_CD.hpp"
#include "Amiga/Resource_Amiga_File.hpp"

#include "CopyProtection.hpp"
#include "IntroData.hpp"
#include "Campaign.hpp"
#include "Map.hpp"
#include "Tiles.hpp"
#include "FontData.hpp"
#include "Graphics.hpp"
#include "Versions.hpp"

#include "Window.hpp"
#include "Sound.hpp"
#include "Recruits.hpp"
#include "GUI_Element.hpp"
#include "SpriteSheet.hpp"
#include "Fodder.hpp"

#include "Structures/Barracks.hpp"

#include "PC/Graphics_PC.hpp"
#include "PC/Sound_PC.hpp"
#include "PC/Sound_PC2.hpp"

#include "Amiga/paula.hpp"
#include "Amiga/rjp1.hpp"
#include "Amiga/Sound_Amiga.hpp"
#include "Amiga/Graphics_Amiga.hpp"


#define g_Fodder	cFodder::GetSingleton()
#define g_Window	cWindow::GetSingleton()
#define g_Resource	cResources::GetSingleton()
#define g_Graphics	cGraphics::GetSingleton()
#define g_Sound		cSound::GetSingleton()

#define VERSION_BASED( pPC, pAmiga ) (g_Fodder.mVersionCurrent->mPlatform == ePlatform::Amiga ? pAmiga : pPC)
