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

cTiles::cTiles( string pFileBase, string pFileSub0 ) {
	std::transform( pFileBase.begin(), pFileBase.end(), pFileBase.begin(), ::tolower );
	std::transform( pFileSub0.begin(), pFileSub0.end(), pFileSub0.begin(), ::tolower );

	mFileBase = pFileBase;
	mFileSub0 = pFileSub0;
	mPalette = 0;
	/*
	for( size_t count = 0; count < 6; ++count ) {

		if( pFileBase.compare( l_landscapes[count] ) != pFileBase.npos )
			break;
	}
	*/
	landscapeLoad( );
}

cTiles::~cTiles() {
	vector< byte* >::iterator		tileIT;

	for( tileIT = mTiles.begin(); tileIT != mTiles.end(); ++tileIT )
		delete (*tileIT);

	delete mPalette;
}

void cTiles::landscapeLoad( ) {
	byte *buffer = 0;
	size_t size = 0;

	buffer = g_Fodder->resourcesGet()->fileGet( mFileBase, size );
	for( int i = 0; i < 0xF0; ++i )
		mTiles.push_back( tileLoad( buffer, i ) );

	mPalette = new byte[0x300];
	memcpy( mPalette, buffer + (size - 0x300), 0x300 );

	buffer = g_Fodder->resourcesGet()->fileGet( mFileSub0, size );
	for( int i = 0; i < 0xF0; ++i )
		mTiles.push_back( tileLoad( buffer, i ) );
}

byte *cTiles::tileLoad( byte *pBuffer, size_t pIndex ) {
	byte *tile = new byte[16*16];
	byte *tilePtr = tile;

	size_t row = pIndex / 20;
	size_t bufStart = pIndex * 0x10;

	// Starting position in buffer
	pBuffer += (bufStart) + ((row * 15) * 0x140);

	// Loop each row of the tile
	for(int i = 0; i < 16; ++i ) {

		memcpy(tilePtr, pBuffer, 16 );
		pBuffer += (0x140);
		tilePtr += 16;
	}

	return tile;
}

byte *cTiles::tileGet( size_t pIndex ) {

	return mTiles[pIndex];
}

