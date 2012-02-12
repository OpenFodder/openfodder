#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "types.hpp"

using namespace std;
class cFodder;

extern cFodder	*g_Fodder;

byte			*local_FileRead( string pFile, size_t	&pFileSize, bool pDataSave );
void			 tool_EndianSwap( byte *pBuffer, size_t pSize );

	// Read a word from the buffer
inline word readBEWord( const void *buffer ) {
	const byte *bytes = (const byte *) buffer;

	return (bytes[0] << 8) | bytes[1];
}

	// Read a word from the buffer
inline dword readLEDWord( const void *buffer ) {
	const dword *wordBytes = (const dword *) buffer;
	return *wordBytes;
}

// Read a word from the buffer
inline word readLEWord( const void *buffer ) {
	const word *wordBytes = (const word *) buffer;
	return *wordBytes;
}

inline void writeLEWord( const void *buffer, word pValue ) {
	word *wordBytes = (word *) buffer;
	*wordBytes = pValue;
}