class cTiles;
class cSurface;

class cMapPiece {
private:
	byte					*mTile;

public:

							 cMapPiece() { mTile = 0; }
	
	void					 tileSet( byte *pTile ) { mTile = pTile; }
	byte					*tileGet() { return mTile; }
};

class cMap {
private:
	cTiles					*mTiles;

	cMapPiece				*mMapPieces;

	size_t					 mMapNumber;
	
	word					 mTilesX,	mTilesY;		// Total Tiles
	word					 mTileX,	mTileY;			// Top Left Tile

	string					 mFileBase, mFileSub;


	string					 generateFilename();
	string					 generateFilenameMap();
	string					 generateFilenameSpt();

public:

							 cMap( size_t pMapNumber );
							~cMap();

	bool					 load();
	bool					 loadMap();
	bool					 loadSpt();

	cSurface				*surfaceLandscapeGet( size_t pTilesX, size_t pTilesY );

	inline	word			 mTilesXGet() { return mTilesX; }
	inline word				 mTilesYGet() { return mTilesY; }

	cTiles					*tilesGet() { return mTiles; }
};
