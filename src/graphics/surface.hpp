#include <sdl.h>

class cSurface {
protected:
	dword			 mPalette[ 16 ];
	dword			 mColorKey;

	byte			*mSurfaceBuffer;								// Loaded Image (uses palette indexs)
	size_t			 mSurfaceBufferSize;

	SDL_Surface		*mSDLSurface;

	size_t			 mWidth, mHeight;

	void			 paletteColorSet(	byte id, byte red, byte green, byte blue );	// Set a color in the palette
	
public:
	
	
					 cSurface( size_t pWidth, size_t pHeight );
					~cSurface();

	void			 blit( cSurface *pSource, word destX, word destY, dword colorKey, dword colorKey2, word pMaxHeight = 0 );
	void			 unblit( cSurface	*Source, word destX, word destY, dword colorKey );

	virtual void	 decode( size_t numBits, byte colorMask, const byte *buffer, size_t bufferSize);

	void			 draw( size_t pX = 0 , size_t pY = 0);					// Draw image to SDL Surface
	void			 drawStrips( size_t pX = 0, size_t pY = 0 );

	void			 fade( size_t pStepSize );

	void			 imageLoadPaletteDraw( const byte *pBuffer, size_t &pSize );
	void			 imageLoad( const byte *pBuffer, size_t &pSize, size_t pNumBits = 4, byte pColorMask = 0, bool pStrips = false );

	void			 paletteLoad( const byte  *pBuffer );			// Load a palette
	byte			 paletteIndexGet( dword pColor );

	dword			*pixelGet( word pX = 0,	word pY = 0 );

	void			 wipe( dword pColor = 0 );						// Clear the surface
	void			 wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor = 0);

	inline SDL_Surface  *surfaceGet() { return mSDLSurface; }		// Get the surface
	inline byte			*surfaceBufferGet() { return mSurfaceBuffer; }

	inline size_t		 widthGet() { return mWidth; }
	inline size_t		 heightGet() { return mHeight; }
};
