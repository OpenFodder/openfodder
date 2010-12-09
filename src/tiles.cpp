#include "stdafx.h"
#include "types.hpp"
#include "tiles.hpp"
#include "fodder.hpp"
#include "resources.hpp"
#include "graphics\surface.hpp"

#include<algorithm>

char *l_landscapes[6] = {
	"jun",
	"des",
	"ice",
	"mor",
	"int",
	"hid"
};

cTiles::cTiles( string pFileBase, string pFileSub ) {
	std::transform( pFileBase.begin(), pFileBase.end(), pFileBase.begin(), ::tolower );
	std::transform( pFileSub.begin(), pFileSub.end(), pFileSub.begin(), ::tolower );

	mFileBase = pFileBase;
	mFileSub = pFileSub;
	/*
	for( size_t count = 0; count < 6; ++count ) {

		if( pFileBase.compare( l_landscapes[count] ) != pFileBase.npos )
			break;
	}
	*/
	landscapeLoad( );
}

cTiles::~cTiles() {
	vector< byte* >::iterator		byteIT;

	for( byteIT = mTiles.begin(); byteIT != mTiles.end(); ++byteIT )
		delete (*byteIT);

	for( byteIT = mPalette.begin(); byteIT != mPalette.end(); ++byteIT )
		delete (*byteIT);

}

void cTiles::landscapeLoad( ) {

	tileSetLoad( mFileBase );
	tileSetLoad( mFileSub );
}

byte *cTiles::mPaletteGet( size_t pIndex ) { 
	if(pIndex >= mPalette.size())
		pIndex = 0;

	return mPalette[pIndex]; 
}	

void cTiles::tileSetLoad( string pFilename ) {
	byte *buffer = 0, *palette = 0;
	size_t size = 0;

	buffer = g_Fodder->resourcesGet()->fileGet( pFilename, size );
	for( int i = 0; i < 0xF0; ++i )
		mTiles.push_back( tileLoad( buffer, i ) );

	// Add palette
	palette = new byte[0x300];
	memcpy( palette, buffer + (size - 0x300), 0x300 );
	mPalette.push_back( palette );

	delete[] buffer;
}

byte *cTiles::tileLoad( byte *pBuffer, size_t pIndex ) {
	byte *tile = new byte[16*16];
	byte *tilePtr = tile;

	size_t row = pIndex / 20;

	// Starting position in buffer
	pBuffer += (pIndex * 0x10) + ((row * 15) * 0x140);

	// Loop each row of the tile
	for(int i = 0; i < 16; ++i ) {

		memcpy(tilePtr, pBuffer, 16 );
		pBuffer += (0x140);
		tilePtr += 16;
	}

	return tile;
}

byte *cTiles::tileGet( size_t pIndex ) {
	if(pIndex >= mTiles.size())
		return 0;

	return mTiles[pIndex];
}

