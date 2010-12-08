#include "stdafx.h"
#include <fstream>

#include "fodder.hpp"

cFodder	*g_Fodder = 0;

int main() {
	g_Fodder = new cFodder();

	g_Fodder->Start();

	delete g_Fodder;
}

byte *local_FileRead( string pFile, size_t	&pFileSize, bool pDataSave ) {
	ifstream		*fileStream;
	byte			*fileBuffer = 0;

	// Attempt to open the file
	fileStream = new ifstream ( pFile.c_str(), ios::binary );
	if(fileStream->is_open() != false) {
	
		// Get file size
		fileStream->seekg(0, ios::end );
		pFileSize = fileStream->tellg();
		fileStream->seekg( ios::beg );

		// Allocate buffer, and read the file into it
		fileBuffer = new byte[ pFileSize ];
		if(fileStream->read( (char*) fileBuffer, pFileSize ) == false) {
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

void tool_EndianSwap( byte *pBuffer, size_t pSize ) {
	byte *pDest = pBuffer;
	
	pSize /= 2;

	while( pSize-- ) {
		byte al = *pBuffer++;
		byte ah = *pBuffer++;

		*pDest++ = ah;
		*pDest++ = al;
	};
}
