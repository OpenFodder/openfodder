#include <sdl.h>

const size_t		 g_MaxColors = 0x100;

class cPalette {
public:
	byte mRed;
	byte mGreen;
	byte mBlue;

	cPalette() { mRed = 0; mGreen = 0; mBlue = 0; }

	byte getPos( size_t i ) { if( i == 0 ) return mRed; if( i == 1 ) return mGreen; if( i == 2 ) return mBlue; }
	void setPos( size_t i, byte pval ) {
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
	cPalette		 mPalette[ g_MaxColors ];
	cPalette		 mPaletteNew[ g_MaxColors ];

	dword			 mPaletteSDL[ g_MaxColors ];
	dword			 mColorKey;

	byte			*mSurfaceBuffer;								// Loaded Image (uses palette indexs)
	size_t			 mSurfaceBufferSize;

	SDL_Surface		*mSDLSurface;

	size_t			 mWidth, mHeight;

	void			 paletteSDLColorSet( size_t id, cPalette *pPalette  );	// Set a color in the palette
	void			 paletteLoadSDL();
	void			 paletteLoadNewSDL();
public:
	
					 cSurface( size_t pWidth, size_t pHeight );
					~cSurface();

	void			 blitFrom( cSurface *pSource, word destX, word destY, dword colorKey, dword colorKey2, word pMaxHeight = 0 );
	void			 loadBuffer( byte *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY );


	void			 decode( byte *pBuffer, size_t pSize, size_t pStart, size_t pColors );
	void			 decodeSprite( byte *pBuffer, size_t pSize, byte *pSpriteData );

	void			 draw( size_t pX = 0 , size_t pY = 0);					// Draw image to SDL Surface

	void			 paletteLoad( const byte  *pBuffer, size_t pColors, size_t pColorID = 0 );	// Load a palette
	void			 paletteFade();

	dword			*pixelGet( word pX = 0,	word pY = 0 );

	void			 wipe( dword pColor = 0 );						// Clear the surface
	void			 wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor = 0);

	inline SDL_Surface  *surfaceGet() { return mSDLSurface; }		// Get the surface
	inline byte			*surfaceBufferGet() { return mSurfaceBuffer; }

	inline size_t		 widthGet() { return mWidth; }
	inline size_t		 heightGet() { return mHeight; }
};
