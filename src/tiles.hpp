enum eLandscape {
	eLandscape_Desert = 0,
	eLandscape_Jungle,
	eLandscape_Ice,
	eLandscape_Jungle2,
	eLandscape_Mor,
	eLandscape_Int
};

class cSurface;

class cTiles {
private:
	vector< byte* >		 mTiles;
	vector< byte* >		 mPalette;

	string				 mFileBase, mFileSub;

private:
	void				 landscapeLoad( );

	byte				*tileLoad( byte *pBuffer, size_t pIndex );
	void				 tileSetLoad( string pFilename );

public:
						 cTiles( string pFileBase, string pFileSub0);
						~cTiles();

	byte				*tileGet( size_t pIndex );

	byte				*mPaletteGet( size_t pIndex );
};
