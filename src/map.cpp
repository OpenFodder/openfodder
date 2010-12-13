#include "stdafx.h"
#include "fodder.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "tiles.hpp"
#include "graphics/surface.hpp"

#include <iomanip>

const char *l_MapName = "mapm";

cMap::cMap( size_t pMapNumber ) {
	mMapNumber = pMapNumber;

	mTilesX = mTilesY = 0;
	mTileX = mTileY = 0;

	mMapPieces = 0;
}

cMap::~cMap() {

	delete mTiles;
	delete mMapPieces;
}

string cMap::generateFilename() {
	stringstream	filename;
	
	// Calculate map 
	filename << l_MapName;
	filename << mMapNumber;

	return filename.str();
}

string cMap::generateFilenameMap() {
	string	filename = generateFilename();

	filename.append(".map");

	return filename;
}

string cMap::generateFilenameSpt() {
	string	filename = generateFilename();

	filename.append(".spt");

	return filename;
}

bool cMap::load() {
	bool result = false;

	if(! loadMap() )
		return false;

	result = loadSpt();


	return result;
}

bool cMap::loadMap() {
	size_t		  mapSize = 0;

	byte *mapBuffer = g_Fodder->resourcesGet()->fileGet( generateFilenameMap(), mapSize );
	if(!mapBuffer)
		return false;

	// Read tileset filenames
	mFileBase = string( (char*) mapBuffer );
	mFileSub = string( (char*) mapBuffer + 0x10 );

	// Map Width and Height
	mTilesX = readBEWord( &mapBuffer[0x54] );
	mTilesY = readBEWord( &mapBuffer[0x56] );

	// Create the map
	delete mMapPieces;
	mMapPieces = new cMapPiece[ mTilesX * mTilesY];

	//
	mTiles = new cTiles(mFileBase, mFileSub);
	
	size_t	mapTiles = (mTilesX * mTilesY);

	byte *mapPieces = (mapBuffer + 0x60);

	for( int index = 0; index < mapTiles; ++index ) {
		// Read tile type, and get pointer to buffer
		byte *tile = mTiles->tileGet( readBEWord( mapPieces ) & 0x1FF );

		// Next Piece
		mapPieces += 2;

		// Save current tile
		mMapPieces[index].tileSet( tile );
	}

	delete[] mapBuffer;
	return true;
}

bool cMap::loadSpt() {
	size_t		  sptSize = 0;

	byte *sptBuffer = g_Fodder->resourcesGet()->fileGet( generateFilenameSpt(), sptSize );
	if(!sptBuffer)
		return false;

	delete[] sptBuffer;
	return true;
}

cSurface *cMap::surfaceLandscapeGet( size_t pTilesX, size_t pTilesY ) {
	cSurface *surface = new cSurface(pTilesX * 16, pTilesY * 16);
	surface->wipe(0);

	// Calculate tile area to draw
	size_t tileEndX = mTileX + pTilesX;
	size_t tileEndY = mTileY + pTilesY;

	// Passed edge of map?
	if( tileEndX > mTilesX )
		tileEndX = mTilesX;
	if( tileEndY > mTilesY )
		tileEndY = mTilesY;

	// Draw each tile to the surface
	for( int y = mTileY; y < tileEndY; ++y ) {

		size_t index = (y * mTilesX) + mTileX;

		for( int x = mTileX; x < tileEndX; ++x ) {
			
			surface->loadBuffer( mMapPieces[ index++ ].tileGet(), x * 16, y * 16, 16, 16 );

		}
	}

	return surface;
}
