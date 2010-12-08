#include "stdafx.h"
#include "fodder.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "tiles.hpp"

#include <iomanip>

const char *l_MapName = "mapm";

cMap::cMap( size_t pMapNumber ) {
	mMapNumber = pMapNumber;

	mWidth = mHeight = 0;
}

cMap::~cMap() {

}

bool cMap::load() {
	size_t		  mapSize = 0;
	stringstream  filename;

	filename << l_MapName;
	filename << mMapNumber;
	filename << ".map";

	byte *mapBuffer = g_Fodder->resourcesGet()->fileGet( filename.str(), mapSize );

	if(!mapBuffer)
		return false;

	mFileBase = string( (char*) mapBuffer );
	mFileSub = string( (char*) mapBuffer + 0x10 );

	mWidth = readBEWord( &mapBuffer[0x54] );
	mHeight = readBEWord( &mapBuffer[0x56] );

	tool_EndianSwap( mapBuffer + 0x60, mapSize - 0x60 );


	mTiles = new cTiles(mFileBase, mFileSub);

	delete[] mapBuffer;
	return true;
}
