
const size_t		 g_MaxColors = 0x100;

class cPalette {
public:
	uint8 mRed;
	uint8 mGreen;
	uint8 mBlue;

	cPalette() { mRed = 0; mGreen = 0; mBlue = 0; }

	uint8 getPos( size_t i ) { if (i == 0) return mRed; if (i == 1) return mGreen; if (i == 2) return mBlue; return 0; }
	void setPos( size_t i, uint8 pval ) {
		if( i == 0 )
			mRed = pval;
		if( i == 1 )
			mGreen = pval;
		if( i == 2 )
			mBlue = pval;
	}
};


class cSurface {
protected:
	cPalette		mPaletteOriginal[g_MaxColors];
	cPalette		 mPalette[ g_MaxColors ];
	cPalette		 mPaletteNew[ g_MaxColors ];

	uint32			 mPaletteSDL[ g_MaxColors ];
	uint32			 mColorKey;

	uint8			*mSurfaceBuffer;								// Loaded Image (uses palette indexs)
	size_t			 mSurfaceBufferSize;

	SDL_Surface*	mSDLSurface;
	SDL_Texture*	mTexture;

	size_t			 mWidth, mHeight;

	void			paletteSDLColorSet( size_t id, cPalette *pPalette  );	// Set a color in the palette
	void			paletteLoadSDL();
	void			paletteLoadNewSDL();
public:
	
					 cSurface( size_t pWidth, size_t pHeight );
					~cSurface();

	void			 blitFrom( cSurface *pSource, uint16 destX, uint16 destY, uint32 colorKey, uint32 colorKey2, uint16 pMaxHeight = 0 );
	void			 loadBuffer( uint8 *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY );


	void			 decode( uint8 *pBuffer, size_t pSize, size_t pStart, size_t pColors );
	void			decode4Plane( uint8* pBuffer, size_t pSize );

	void			 decodeSprite( uint8 *pBuffer, size_t pSize, uint8 *pSpriteData );

	void			 draw( size_t pX = 0 , size_t pY = 0);					// Draw image to SDL Surface

	void			 paletteLoad( const uint8  *pBuffer, size_t pColors, size_t pColorID = 0 );	// Load a palette
	int16			 paletteFade();
	void			 paletteFadeOut();

	uint32			*pixelGet( uint16 pX = 0,	uint16 pY = 0 );

	void			 wipe( uint32 pColor = 0 );						// Clear the surface
	void			 wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor = 0);

	inline SDL_Texture* GetTexture() const { return mTexture; };
};
