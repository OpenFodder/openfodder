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

void cSurface::paletteLoad( const byte  *pBuffer, size_t pColors ) {
	size_t	colorID;
	byte  colorRed, colorGreen, colorBlue;

	if( pColors > g_MaxColors )
		pColors = g_MaxColors;

	for( colorID = 0; colorID < pColors; colorID++) {
		
		// Get the next color values
		colorRed	=	*pBuffer++;
		colorGreen	=	*pBuffer++;
		colorBlue	=	*pBuffer++;

		// 
		colorRed <<= 1;
		colorGreen <<= 1;
		colorBlue <<= 1;

		// Set the color in the palette
		paletteColorSet( colorID, colorRed, colorGreen, colorBlue );
	}

	draw();
}

inline void cSurface::paletteColorSet( size_t id, byte red, byte green, byte blue ) {
	if(id >= g_MaxColors)
		return;

	// Get the palette color for the provided RGB values
	mPalette[id] = SDL_MapRGB (	mSDLSurface->format , red , green , blue ) ;
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
			*bufferTarget = mPalette[ *bufferCurrent ];
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

		draw();
	}
}

void cSurface::loadBuffer( byte *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY ) {
	byte	*destPtr = mSurfaceBuffer;
	byte	*destPtrRow = 0;

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
