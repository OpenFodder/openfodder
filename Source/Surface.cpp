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
	mFaded = false;

	mColorKey = 0xFF;

	// Create the screen buffer
	mSDLSurface = SDL_CreateRGBSurface( 0, pWidth, pHeight, 32, 0xFF, 0xFF << 8, 0xFF << 16, 0 );
	mTexture = SDL_CreateTexture(g_Window.GetRenderer(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, pWidth, pHeight);
	
	//SDL_SetColorKey(mSDLSurface, 1, 0xFFFFFFFF	);

	mSurfaceBuffer = new uint8[ mWidth * mHeight ];
	mSurfaceBufferSaved = new uint8[ mWidth * mHeight ];
	mSurfaceBufferSize = mWidth * mHeight;
	
	clearBuffer();
	wipe();
}

cSurface::~cSurface() {
	delete[] mSurfaceBuffer;
	delete[] mSurfaceBufferSaved;

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

void cSurface::paletteClear() {
	for(int16 ColorID = 0; ColorID < g_MaxColors; ColorID++) {
		
		// Get the next color values
		mPalette[ColorID].mRed = 0;
		mPalette[ColorID].mGreen = 0;
		mPalette[ColorID].mBlue = 0;
	}
	paletteLoadSDL();
}

void cSurface::paletteSet( cPalette* pPalette, uint32 pColorID, bool pUseNow ) {

	if (pUseNow) {
		for (uint32 ColorID = pColorID; ColorID < g_MaxColors; ++ColorID) {
			mPalette[ColorID] = pPalette[ColorID];
		}
	}
	else {
		for (uint32 ColorID = pColorID; ColorID < g_MaxColors; ++ColorID) {
			mPaletteNew[ColorID] = pPalette[ColorID];
		}
	}
	paletteLoadSDL();
	if (!pUseNow)
		mFaded = false;
}

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

	paletteLoadNewSDL();
}

void cSurface::paletteLoad_Amiga( const uint8  *pBuffer, uint32 pColorID, uint32 pColors ) {
	int16  color;
	int16  ColorID = pColorID;

	for( ; pColorID < ColorID + pColors; pColorID++) {
		
		// Get the next color codes
		color = readBEWord( pBuffer );
		pBuffer+=2;

		// Extract each color from the word
		//  X X X X   R3 R2 R1 R0     G3 G2 G1 G0   B3 B2 B1 B0

		mPaletteNew[pColorID].mRed		= ((color >> 8) & 0xF)   << 2;	// Why 2? no idea, but it works.. 1 is too dark, and 3 causes incorrect colours
		mPaletteNew[pColorID].mGreen	= ((color >> 4) & 0xF) << 2;
		mPaletteNew[pColorID].mBlue		= ((color >> 0) & 0xF)  << 2;
	}

	//paletteLoadNewSDL();
}

void cSurface::paletteFadeOut() {
	mFaded = false;

	for (int cx = 0; cx < g_MaxColors; ++cx) {
		mPaletteNew[cx].mBlue = 0;
		mPaletteNew[cx].mRed = 0;
		mPaletteNew[cx].mGreen = 0;
	}
}

int16 cSurface::paletteFade() {
	int bx = 0;
	mFaded = true;

	for( int cx = 0x0; cx < g_MaxColors; ++cx ) {

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
			mFaded = false;
		}
	}

	paletteLoadSDL();
	return bx;
}

void cSurface::paletteLoadSDL() {

	for( int cx = 0; cx < g_MaxColors; ++cx )
		paletteSDLColorSet( cx, &mPalette[cx] );
}

void cSurface::paletteLoadNewSDL() {

	for( int cx = 0; cx < g_MaxColors; ++cx )
		paletteSDLColorSet( cx, &mPaletteNew[cx] );
}

inline void cSurface::paletteSDLColorSet( size_t id, cPalette *pPalette ) {
	if(id >= g_MaxColors)
		return;

	// Get the palette color for the provided RGB values
	mPaletteSDL[id] = SDL_MapRGB (	mSDLSurface->format , pPalette->mRed << 2, pPalette->mGreen << 2, pPalette->mBlue << 2) ;
}

void cSurface::draw( size_t pX, size_t pY ) {

	wipe();

	uint8 *bufferCurrent = mSurfaceBuffer;
	uint8 *bufferCurrentMax = (mSurfaceBuffer + mSurfaceBufferSize);

	uint32 *bufferTarget = (uint32*)mSDLSurface->pixels;
	uint32 *bufferTargetMax = (uint32*) (((uint8*) mSDLSurface->pixels) + (mSDLSurface->h * mSDLSurface->pitch));

	while( bufferTarget < bufferTargetMax ) {
			
		if( bufferCurrent >= bufferCurrentMax )
			break;

		if (*bufferCurrent) {
			if (*bufferCurrent < g_MaxColors)
				*bufferTarget = mPaletteSDL[*bufferCurrent];
			else
				*bufferTarget = 0;
		}

		++bufferTarget; 
		++bufferCurrent;
	}

	SDL_UpdateTexture(mTexture, NULL, mSDLSurface->pixels, mSDLSurface->pitch);
}

void cSurface::clearBuffer() {
	for (int i = 0; i < mSurfaceBufferSize; ++i) {
		mSurfaceBuffer[i] = 0;
		mSurfaceBufferSaved[i] = 0;
	}

	wipe();
}

void cSurface::load( cSurface* pImage ) {
	size_t size = pImage->GetSurfaceBufferSize();
	if (size > mSurfaceBufferSize)
		size = mSurfaceBufferSize;

	memcpy( mSurfaceBuffer, pImage->GetSurfaceBuffer(), size );
}

void cSurface::Save() {

	memcpy( mSurfaceBufferSaved, mSurfaceBuffer, mSurfaceBufferSize );
}

void cSurface::Restore() {
	
	memcpy( mSurfaceBuffer, mSurfaceBufferSaved, mSurfaceBufferSize );
}

void cSurface::decode( uint8 *pBuffer, size_t pSize, size_t pStart, size_t pColors ) {
	size_t colorSize = (pColors * 3);
	size_t dataSize = pSize - colorSize;

	memcpy( mSurfaceBuffer + pStart, pBuffer, dataSize);
	
	if(pColors) {
		paletteLoad( pBuffer + dataSize, pColors );
	}
}

void cSurface::loadBuffer( uint8 *pBuffer, size_t pDestX, size_t pDestY, size_t pMaxX, size_t pMaxY ) {
	uint8	*destPtr = mSurfaceBuffer;
	uint8	*destPtrRow = 0;

	if(!pBuffer)
		return;

	destPtr += (mWidth * pDestY) + 16;
	
	for( int y = 0; y < pMaxY; ++y ) {
		destPtrRow = destPtr;
		
		destPtr += pDestX;

		for( int x = 0; x < pMaxX; ++x ) {
			*destPtr++ = *pBuffer++;
		}

		destPtr = destPtrRow + mWidth;
	}
}
