/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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
#include <cassert>
#include <map>
#include <mutex>

#include <SDL.h>
#ifndef EMSCRIPTEN
#include <SDL_mixer.h>
#else
#include <emscripten.h>
#include <SDL/SDL_mixer.h>
#endif

#include "Types.hpp"

typedef std::shared_ptr<std::vector<uint8>> tSharedBuffer;

enum eDataType {
	eData = 0,
	eSave,
	eCampaign,
    eTest,
    eRoot,
	eScript,
	eNone,
};

int			start(int argc, char *argv[]);
void		tool_EndianSwap( uint8* pBuffer, size_t pSize );
std::string tool_StripLeadingZero( const std::string& pValue );
uint16		tool_DecimalToBinaryCodedDecimal( uint16 pDecimal );

// Read a BE word from the buffer
inline uint16 readBEWord( const void *buffer ) {
	const uint8* bytes = (const uint8*) buffer;

	return uint16((bytes[0] << 8) + bytes[1]);
}

// Write a LE word in BE
inline void writeBEWord( const void *buffer, uint16 pValue ) {
	uint8* bytes = (uint8*) buffer;

	bytes[0] = (uint8) (pValue >> 8);
	bytes[1] = (uint8) (pValue & 0xFF);
}

// Read a LE DWord in BE
inline uint32 readBEDWord( const void *buffer ) {
	const uint8* bytes = (const uint8*) buffer;

	return uint32((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + (bytes[3]));
}

// Read little endian from the buffer
template <class tRet> inline tRet readLE(const void *pBuffer) {
    const tRet* data = (const tRet*)pBuffer;
    return *data;
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

#include "Debugger.hpp"
#include "Utils/pseudorand.hpp"

#include "Position.hpp"
#include "Dimension.hpp"
#include "Event.hpp"

#include "Surface.hpp"
#include "Version.hpp"
#include "Resources.hpp"

#include "Parameters.hpp"

#include "PC/Resource_PC_CD.hpp"
//#include "Amiga/Resource_Amiga_File.hpp"

#include "CopyProtection.hpp"
#include "IntroData.hpp"
#include "Tiles.hpp"

#include "Map/Map.hpp"
#include "Map/Original.hpp"
//#include "Map/Random.hpp"

#include "Campaign.hpp"
#include "FontData.hpp"
#include "Graphics.hpp"
#include "Recruits.hpp"
#include "Versions.hpp"
#include "ResourceMan.hpp"

#include "Window.hpp"
#include "Sound.hpp"
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
#include "Amiga/Graphics_Amiga2.hpp"

#include "About.hpp"
#include "UnitTesting.hpp"

#include "ScriptingEngine.hpp"

extern std::shared_ptr<cResources> g_Resource;
extern std::shared_ptr<cWindow>    g_Window;
extern std::shared_ptr<cFodder>    g_Fodder;
extern std::shared_ptr<cDebugger>  g_Debugger;
extern std::shared_ptr<cResourceMan> g_ResourceMan;
extern std::shared_ptr<cScriptingEngine> g_ScriptingEngine;

extern const char gPathSeperator;

#define CANNON_BASED( pCF1, pCF2 )    (g_Fodder->mVersionCurrent->isCannonFodder1() ? pCF1 : pCF2 )
#define PLATFORM_BASED( pPC, pAmiga ) (g_Fodder->mVersionCurrent->isAmiga() ? pAmiga : pPC )
