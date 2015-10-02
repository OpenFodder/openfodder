/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield <robcrossfield@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

const size_t		 g_MaxColors = 0xFF;

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
	cPalette		mPalette[ g_MaxColors ];
	cPalette		mPaletteNew[ g_MaxColors ];

	uint32			mPaletteSDL[ g_MaxColors ];

	uint8*			mSurfaceBuffer;								// Loaded Image (uses palette indexs)
	uint8*			mSurfaceBufferSaved;
	size_t			mSurfaceBufferSize;

	SDL_Surface*	mSDLSurface;
	SDL_Texture*	mTexture;

	size_t			mWidth, mHeight;
	bool			mFaded;

	void			paletteSDLColorSet( size_t id, cPalette *pPalette  );	// Set a color in the palette
	
	void			paletteLoadSDL();
	void			paletteLoadNewSDL();
public:
	
					cSurface( size_t pWidth, size_t pHeight );
					~cSurface();

	void			clearBuffer();

	void			load( cSurface* pImage );
	void			loadBuffer( uint8 *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY );


	void			decode( uint8 *pBuffer, size_t pSize, size_t pStart, size_t pColors );

	void			draw( size_t pX = 0 , size_t pY = 0);					// Draw image to SDL Surface

	void			Save();
	void			Restore();

	void			paletteClear();
	void			paletteSet( cPalette* pPalette, uint32 pColorID = 0, bool pUseNow = false );
	void			paletteLoad( const uint8  *pBuffer, size_t pColors, size_t pColorID = 0 );			// Load a palette	void			paletteLoad_Amiga(  const uint8  *pBuffer );
	void			paletteLoad_Amiga(  const uint8  *pBuffer, uint32 pColorID = 0, uint32 pColors = 16 );

	int16			paletteFade();
	void			paletteFadeOut();

	void			wipe( uint32 pColor = 0 );						// Clear the surface
	void			wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor = 0);

	inline SDL_Texture* GetTexture() const { return mTexture; };
	inline uint8*		GetSurfaceBuffer() const { return mSurfaceBuffer; }
	inline size_t		GetSurfaceBufferSize() const { return mSurfaceBufferSize; }

	inline size_t		GetWidth() const { return mWidth; }
	inline size_t		GetHeight() const { return mHeight; }

	inline bool			GetFaded() { return mFaded; }
};
