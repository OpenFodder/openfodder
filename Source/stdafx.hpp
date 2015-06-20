#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Types.hpp"

#include <Windows.h>

std::string local_PathGenerate( std::string pFile, std::string pPath );
uint8*		local_FileRead( std::string pFile, std::string pPath, size_t& pFileSize );
void		tool_EndianSwap( uint8* pBuffer, size_t pSize );

// Read a word from the buffer
inline uint16 readBEWord( const void *buffer ) {
	const uint8* bytes = (const uint8*) buffer;

	return (bytes[0] << 8) | bytes[1];
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

#include "Singleton.hpp"

#include "Dimension.hpp"
#include "Position.hpp"
#include "Event.hpp"

#include "Surface.hpp"
#include "Resources.hpp"

#include "Window.hpp"
#include "Fodder.hpp"


#define g_Fodder cFodder::GetSingleton()
#define g_Window cWindow::GetSingleton()
#define g_Resource cResources::GetSingleton()
