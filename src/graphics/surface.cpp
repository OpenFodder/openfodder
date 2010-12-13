#include "stdafx.h"
#include "surface.hpp"

cSurface::cSurface( size_t pWidth, size_t pHeight ) {
	mWidth = pWidth; 
	mHeight = pHeight;

	mColorKey = 0xFF;

	// Create the screen buffer
	mSDLSurface = SDL_CreateRGBSurface(	SDL_SWSURFACE,	pWidth,	pHeight,	 32, 0, 0, 0, 0);
	
	SDL_SetColorKey(mSDLSurface, SDL_SRCCOLORKEY, 0xFFFFFFFF	);

	mSurfaceBuffer = new byte[ mWidth * mHeight ];
	mSurfaceBufferSize = mWidth * mHeight;
	
	for( int i = 0; i < mSurfaceBufferSize; ++i )
		mSurfaceBuffer[i] = 0;

	wipe();
}

cSurface::~cSurface() {
	delete[] mSurfaceBuffer;

	SDL_FreeSurface( mSDLSurface );
}

void cSurface::wipe( dword pColor ) {

	SDL_FillRect( mSDLSurface, 0, pColor );
}

void cSurface::wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor) {
	SDL_Rect dest;

	dest.x = pX;		dest.y = pY; 
	dest.w = pSizeX; dest.h = pSizeY;

	SDL_FillRect( mSDLSurface, &dest, pColor );
}

void cSurface::blitFrom( cSurface *pSource, word destX, word destY, dword colorKey, dword colorKey2, word pMaxHeight ) {

	if(!pSource)
		return;

	SDL_Surface *sdlSurface = pSource->surfaceGet();
	dword		*srcSurface = (dword*) sdlSurface->pixels;
	dword		*srcEnd		= (dword*) sdlSurface->pixels + (sdlSurface->pitch * sdlSurface->h);
	dword		*dstSurface	= 0;

	word x, y, maxX, maxY;

	maxX = destX + pSource->widthGet();

	if( pMaxHeight )
		maxY = destY + pMaxHeight;
	else
		maxY = destY + pSource->heightGet();

	dword difference = mWidth - pSource->widthGet();

	word	maxWidth = mWidth, maxHeight = mHeight;

	for(y = destY; y < maxY; ++y) {
		for(x = destX; x < maxX; ++x) {

			if(srcSurface >= srcEnd)
				break;

			// Ensure we're drawing inside the screen 
			// (skip the bytes if its outside the screen area)
			if( x < 0 || y < 0 || x >= maxWidth || y >= maxHeight ) {
				dstSurface = 0;
				++srcSurface;
				continue;

			} else {

				// Make sure we got a surface to draw to
				if(!dstSurface)
					dstSurface = pixelGet( x, y );

				// If its nots a match of either key colors
				if( *srcSurface != colorKey && *srcSurface != colorKey2 ) {
					*dstSurface = *srcSurface;
				}
			}

			++srcSurface;
			++dstSurface;
		}

		if(dstSurface)
			dstSurface += difference;
	}
}

void cSurface::paletteLoad( const byte  *pBuffer, size_t pColors, size_t pColorID ) {
	size_t colorStartID = pColorID;

	if( pColors >= g_MaxColors )
		pColors = g_MaxColors-1;

	for(; pColorID < pColors + colorStartID; pColorID++) {
		
		// Get the next color values
		mPaletteNew[ pColorID ].mRed =		*pBuffer++;
		mPaletteNew[ pColorID ].mGreen =	*pBuffer++;
		mPaletteNew[ pColorID ].mBlue =		*pBuffer++;
	}
}

void cSurface::paletteFade() {
	
	for( int cx = 0x0; cx < 0x100; ++cx ) {

		for( int i = 0; i < 3; ++i ) {
			byte al = mPaletteNew[cx].getPos(i);
			byte bl = mPalette[cx].getPos(i);
			al -= bl;
			if(!al)
				continue;

			if( al != 1 ) {
				al >>= 1;
				if( al != 1 ) {
					al >>= 1;
					if( al != 1 )
						al >>= 1;
				}
			}

			// loc_13918
			mPalette[cx].setPos(i, bl + al );
		}
	}

	paletteLoadSDL();
}

void cSurface::paletteLoadSDL() {

	for( int cx = 0; cx < g_MaxColors; ++cx )
		paletteSDLColorSet( cx, &mPalette[cx] );

	draw();
}

void cSurface::paletteLoadNewSDL() {

	for( int cx = 0; cx < g_MaxColors; ++cx )
		paletteSDLColorSet( cx, &mPaletteNew[cx] );

	draw();
}

inline void cSurface::paletteSDLColorSet( size_t id, cPalette *pPalette ) {
	if(id >= g_MaxColors)
		return;

	// Get the palette color for the provided RGB values
	mPaletteSDL[id] = SDL_MapRGB (	mSDLSurface->format , pPalette->mRed, pPalette->mGreen, pPalette->mBlue ) ;
}

dword *cSurface::pixelGet( word x,	word y ) {
	register dword *position = (dword*) mSDLSurface->pixels ;
		
	// Offset by Y
	position +=  (y * (mSDLSurface->pitch / 4) );		// 4 Bytes per Pixel

	// Offset by X
	position +=  x;

	return position;
}


void cSurface::draw( size_t pX, size_t pY ) {

	wipe();

	byte *bufferCurrent = mSurfaceBuffer;
	byte *bufferCurrentMax = (mSurfaceBuffer + mSurfaceBufferSize);

	dword *bufferTarget = (dword*) pixelGet( pX, pY );
	dword *bufferTargetMax = (dword*) (((byte*) mSDLSurface->pixels) + (mSDLSurface->h * mSDLSurface->pitch));

	while( bufferTarget < bufferTargetMax ) {
			
		if( bufferCurrent >= bufferCurrentMax )
			break;

		if( *bufferCurrent < g_MaxColors )
			*bufferTarget = mPaletteSDL[ *bufferCurrent ];
		else
			*bufferTarget = 0;

		++bufferTarget; 
		++bufferCurrent;
	}
	
}

void cSurface::decode( byte *pBuffer, size_t pSize, size_t pStart, size_t pColors ) {
	size_t colorSize = (pColors * 3);
	size_t dataSize = pSize - colorSize;

	memcpy( mSurfaceBuffer + pStart, pBuffer, dataSize);

	if(pColors) {
		paletteLoad( pBuffer + dataSize, pColors );

		paletteLoadNewSDL();
	}
}

void cSurface::decodeSprite( byte *pBuffer, size_t pSize, byte *pSpriteData ) {
	byte *buffer			= pBuffer;
	byte *dstBufferStart	= mSurfaceBuffer;
	byte *dstBuffer			= dstBufferStart;

	size_t colCount = mWidth >> 1;
	
	size_t word_42074 = 0xA0 - colCount;

	colCount >>= 1;
	size_t word_42076 = mWidth - colCount;

	mHeight = 1;
	byte al = 0;
	for( int y = 0; y < mHeight; ++y ) {
		for( int x = 0; x < colCount; ++x ) {
			word ax = *buffer++;
			byte al = ax >> 4;

			if(al) 
				*dstBuffer = al | 0xF0;

			++dstBuffer;
			++buffer;
		}
		buffer += word_42074;
		dstBuffer += word_42076;
	}


	buffer = pBuffer;
	dstBuffer = ++dstBufferStart;

	for( int y = 0; y < mHeight; ++y ) {
		for( int x = 0; x < colCount; ++x ) {
			word ax = *buffer++;

			al = ax & 0x0F;
			if(al)
				*dstBuffer = al | 0xF0;

			++dstBuffer;
			++buffer;
		}
		buffer += word_42074;
		dstBuffer += word_42076;
	}
	
	buffer = ++pBuffer;
	dstBuffer = ++dstBufferStart;
	
	for( int y = 0; y < mHeight; ++y ) {
		for( int x = 0; x < colCount; ++x ) {
			
			byte al = *buffer++;
			al >>= 4;

			if(al)
				*dstBuffer = al | 0xF0;

			++buffer;
			++dstBuffer;
		}

		buffer += word_42074;
		dstBuffer += word_42076;
	}

	buffer = pBuffer;
	dstBuffer = ++dstBufferStart;

	for( int y = 0; y < mHeight; ++y ) {
		for( int x = 0; x < colCount; ++x ) {
			word ax = *buffer++;

			al = ax & 0x0F;
			if(al)
				*dstBuffer = al | 0xF0;

			++dstBuffer;
			++buffer;
		}
		buffer += word_42074;
		dstBuffer += word_42076;
	}
}

void cSurface::loadBuffer( byte *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY ) {
	byte	*destPtr = mSurfaceBuffer;
	byte	*destPtrRow = 0;

	if(!pBuffer)
		return;

	destPtr += (mWidth * pDestY);
	
	for( int y = 0; y < pMaxY; ++y ) {
		destPtrRow = destPtr;
		
		destPtr += pDestX;

		for( int x = 0; x < pMaxX; ++x ) {
			*destPtr++ = *pBuffer++;
		}

		destPtr = destPtrRow + mWidth;
	}
}
