#include <sdl.h>

const size_t		 g_MaxColors = 0x100;

class cSurface {
protected:
	dword			 mPalette[ g_MaxColors ];
	dword			 mColorKey;

	byte			*mSurfaceBuffer;								// Loaded Image (uses palette indexs)
	size_t			 mSurfaceBufferSize;

	SDL_Surface		*mSDLSurface;

	size_t			 mWidth, mHeight;

	void			 paletteColorSet(	size_t id, byte red, byte green, byte blue );	// Set a color in the palette
	
public:
	
					 cSurface( size_t pWidth, size_t pHeight );
					~cSurface();

	void			 blitFrom( cSurface *pSource, word destX, word destY, dword colorKey, dword colorKey2, word pMaxHeight = 0 );
	void			 loadBuffer( byte *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY );


	virtual void	 decode( byte *pBuffer, size_t pSize, size_t pStart, size_t pColors );
	void			 draw( size_t pX = 0 , size_t pY = 0);					// Draw image to SDL Surface

	void			 paletteLoad( const byte  *pBuffer, size_t pColors );	// Load a palette

	dword			*pixelGet( word pX = 0,	word pY = 0 );

	void			 wipe( dword pColor = 0 );						// Clear the surface
	void			 wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor = 0);

	inline SDL_Surface  *surfaceGet() { return mSDLSurface; }		// Get the surface
	inline byte			*surfaceBufferGet() { return mSurfaceBuffer; }

	inline size_t		 widthGet() { return mWidth; }
	inline size_t		 heightGet() { return mHeight; }
};
