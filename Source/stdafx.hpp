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

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#ifndef EMSCRIPTEN
#include <SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif

#include "Types.hpp"

//#include <Windows.h>

std::string local_PathGenerate( std::string pFile, std::string pPath );
uint8*		local_FileRead( std::string pFile, std::string pPath, size_t& pFileSize );
void		tool_EndianSwap( uint8* pBuffer, size_t pSize );
std::string	tool_NumToString( uint32 pValue );
std::string tool_StripLeadingZero( const std::string& pValue );
uint16		tool_DecimalToBinaryCodedDecimal( uint16 pDecimal );

// Read a BE word from the buffer
inline uint16 readBEWord( const void *buffer ) {
	const uint8* bytes = (const uint8*) buffer;

	return (bytes[0] << 8) | bytes[1];
}

inline void writeBEWord( const void *buffer, uint16 pValue ) {
	uint8* bytes = (uint8*) buffer;

	bytes[0] = pValue >> 8;
	bytes[1] = pValue & 0xFF;
}


inline uint32 readBEDWord( const void *buffer ) {
	const uint8* bytes = (const uint8*) buffer;

	return (bytes[0] << 24) | bytes[1] << 16 | (bytes[2] << 8) | bytes[3];
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

#include "Versions.hpp"

#include "Window.hpp"
#include "Graphics.hpp"
#include "Fodder.hpp"

#include "PC/Graphics_PC.hpp"
#include "Amiga/Graphics_Amiga.hpp"


#define g_Fodder cFodder::GetSingleton()
#define g_Window cWindow::GetSingleton()
#define g_Resource cResources::GetSingleton()
#define g_Graphics cGraphics::GetSingleton()
