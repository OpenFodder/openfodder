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

#include "stdafx.hpp"

cSurface::cSurface( size_t pWidth, size_t pHeight ) {
	mWidth = pWidth; 
	mHeight = pHeight;

	mColorKey = 0xFF;

	// Create the screen buffer
	mSDLSurface = SDL_CreateRGBSurface( 0, pWidth, pHeight, 32, 0xFF, 0xFF << 8, 0xFF << 16, 0 );
	mTexture = SDL_CreateTexture(g_Window.GetRenderer(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, pWidth, pHeight);
	
	//SDL_SetColorKey(mSDLSurface, 1, 0xFFFFFFFF	);

	mSurfaceBuffer = new uint8[ mWidth * mHeight ];
	mSurfaceBufferSize = mWidth * mHeight;
	
	for( int i = 0; i < mSurfaceBufferSize; ++i )
		mSurfaceBuffer[i] = 0;

	wipe();
}

cSurface::~cSurface() {
	delete[] mSurfaceBuffer;

	SDL_FreeSurface( mSDLSurface );
	SDL_DestroyTexture( mTexture );
}

void cSurface::wipe( uint32 pColor ) {

	SDL_FillRect( mSDLSurface, 0, pColor );
}

void cSurface::wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor) {
	SDL_Rect dest;

	dest.x = pX; dest.y = pY; 
	dest.w = pSizeX; dest.h = pSizeY;

	SDL_FillRect( mSDLSurface, &dest, pColor );
}
/*
void cSurface::blitFrom( cSurface *pSource, uint16 destX, uint16 destY, uint32 colorKey, uint32 colorKey2, uint16 pMaxHeight ) {

	if(!pSource)
		return;

	SDL_Surface *sdlSurface = pSource->surfaceGet();
	uint32		*srcSurface = (uint32*) sdlSurface->pixels;
	uint32		*srcEnd		= (uint32*) sdlSurface->pixels + (sdlSurface->pitch * sdlSurface->h);
	uint32		*dstSurface	= 0;

	uint16 x, y, maxX, maxY;

	maxX = destX + pSource->widthGet();

	if( pMaxHeight )
		maxY = destY + pMaxHeight;
	else
		maxY = destY + pSource->heightGet();

	uint32 difference = mWidth - pSource->widthGet();

	uint16	maxWidth = mWidth, maxHeight = mHeight;

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
*/
void cSurface::paletteLoad( const uint8  *pBuffer, size_t pColors, size_t pColorID ) {
	size_t colorStartID = pColorID;

	if( pColors >= g_MaxColors )
		pColors = g_MaxColors-1;

	for(; pColorID < pColors + colorStartID; pColorID++) {
		
		// Get the next color values
		mPaletteNew[ pColorID ].mRed =		*pBuffer++;
		mPaletteNew[ pColorID ].mGreen =	*pBuffer++;
		mPaletteNew[ pColorID ].mBlue =		*pBuffer++;
	}

	for (unsigned int x = 0; x < g_MaxColors; ++x) {
		mPaletteOriginal[x] = mPaletteNew[x];
	}

	paletteLoadNewSDL();
}

void cSurface::paletteFadeOut() {
	
	for (int cx = 0; cx < g_MaxColors; ++cx) {
		mPaletteNew[cx].mBlue = 0;
		mPaletteNew[cx].mRed = 0;
		mPaletteNew[cx].mGreen = 0;
	}

	draw();
}

int16 cSurface::paletteFade() {
	int bx = 0;

	for( int cx = 0x0; cx < 0x100; ++cx ) {

		for( int i = 0; i < 3; ++i ) {
			int8 al = mPaletteNew[cx].getPos(i);
			int8 bl = mPalette[cx].getPos(i);
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
			bx = -1;
		}
	}

	paletteLoadSDL();

	return bx;
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
	mPaletteSDL[id] = SDL_MapRGB (	mSDLSurface->format , pPalette->mRed << 2, pPalette->mGreen << 2, pPalette->mBlue << 2) ;
}

uint32 *cSurface::pixelGet( uint16 x,	uint16 y ) {
	register uint32 *position = (uint32*) mSDLSurface->pixels ;
		
	// Offset by Y
	position +=  (y * (mSDLSurface->pitch / 4) );		// 4 Bytes per Pixel

	// Offset by X
	position +=  x;

	return position;
}


void cSurface::draw( size_t pX, size_t pY ) {

	wipe();

	uint8 *bufferCurrent = mSurfaceBuffer;
	uint8 *bufferCurrentMax = (mSurfaceBuffer + mSurfaceBufferSize);

	uint32 *bufferTarget = (uint32*) pixelGet( pX, pY );
	uint32 *bufferTargetMax = (uint32*) (((uint8*) mSDLSurface->pixels) + (mSDLSurface->h * mSDLSurface->pitch));

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

	SDL_UpdateTexture(mTexture, NULL, mSDLSurface->pixels, mSDLSurface->pitch);
}

void cSurface::decode( uint8 *pBuffer, size_t pSize, size_t pStart, size_t pColors ) {
	size_t colorSize = (pColors * 3);
	size_t dataSize = pSize - colorSize;

	memcpy( mSurfaceBuffer + pStart, pBuffer, dataSize);
	
	if(pColors) {
		paletteLoad( pBuffer + dataSize, pColors );
	}
}

void cSurface::decode4Plane( uint8* pBuffer, size_t pSize ) {
	uint8* SrcBuffer = pBuffer;
	uint8 *Buffer = mSurfaceBuffer;
	wipe();

	for (unsigned int Plane = 0; Plane < 4; Plane++) {
		Buffer = mSurfaceBuffer;
		
		for (unsigned int Y = 0; Y < mHeight; ++Y) {

			for (unsigned int X = Plane; X < mWidth; X += 4 ) {

				Buffer[X] = *SrcBuffer++;
			}

			Buffer += mWidth;
		}
	}
}

void cSurface::decodeSprite( uint8 *pBuffer, size_t pSize, uint8 *pSpriteData ) {
	uint8 *buffer			= pBuffer;
	uint8 *dstBufferStart	= mSurfaceBuffer;
	uint8 *dstBuffer			= dstBufferStart;

	size_t colCount = mWidth >> 1;
	
	size_t word_42074 = 0xA0 - colCount;

	colCount >>= 1;
	size_t word_42076 = mWidth - colCount;

	mHeight = 1;
	uint8 al = 0;
	for( int y = 0; y < mHeight; ++y ) {
		for( int x = 0; x < colCount; ++x ) {
			uint16 ax = *buffer++;
			uint8 al = ax >> 4;

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
			uint16 ax = *buffer++;

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
			
			uint8 al = *buffer++;
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
			uint16 ax = *buffer++;

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

void cSurface::loadBuffer( uint8 *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY ) {
	uint8	*destPtr = mSurfaceBuffer;
	uint8	*destPtrRow = 0;

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
