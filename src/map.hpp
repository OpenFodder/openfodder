class cTiles;

class cMap {
private:
	cTiles					*mTiles;
	size_t					 mMapNumber;
	
	word					 mWidth, mHeight;
	string					 mFileBase, mFileSub;

public:

							 cMap( size_t pMapNumber );
							~cMap();

	bool					 load();

	cTiles					*tilesGet() { return mTiles; }
};
