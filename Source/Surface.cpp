/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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
    mIsLoadedImage = false;
	mWidth = pWidth; 
	mHeight = pHeight;
	mPaletteAdjusting = false;

	// Create the screen buffer
	mSDLSurface = SDL_CreateRGBSurface( 0, (int) pWidth, (int) pHeight, 32, 0xFF << 16, 0xFF << 8, 0xFF, 0 );
	mTexture = 0;

	if (!mSDLSurface) {
		g_Debugger->Error("SDLSurface not initialised");
		exit(1);
	}
    if (g_Window->GetRenderer()) {
        mTexture = SDL_CreateTexture(g_Window->GetRenderer(), SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, (int)pWidth, (int)pHeight);

        SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_ADD);
        SDL_SetTextureAlphaMod(mTexture, 0xFF);
        SDL_SetTextureColorMod(mTexture, 0xFF, 0xFF, 0xFF);
    }
	mSurfaceBuffer = new uint8[ mWidth * mHeight ];
	mSurfaceBufferSaved = new uint8[ mWidth * mHeight ];
	mSurfaceBufferSize = mWidth * mHeight;
	
	clearBuffer();
	clearSDLSurface();
}

cSurface::cSurface(const cDimension& pDimension) : cSurface(pDimension.mWidth, pDimension.mHeight) {

}

cSurface::~cSurface() {
	delete[] mSurfaceBuffer;
	delete[] mSurfaceBufferSaved;

	SDL_FreeSurface( mSDLSurface );

	if(mTexture)
		SDL_DestroyTexture( mTexture );
}

void cSurface::clearSDLSurface( uint32 pColor ) {

	SDL_FillRect( mSDLSurface, 0, pColor );
}

void cSurface::palette_SetToBlack() {

	for(size_t ColorID = 0; ColorID < g_MaxColors; ColorID++) {
		
		// Get the next color values
		mPalette[ColorID].mRed = 0;
		mPalette[ColorID].mGreen = 0;
		mPalette[ColorID].mBlue = 0;
	}

	surfaceSetToPalette();
}

void cSurface::palette_SetFromNew() {

	for (size_t cx = 0; cx < g_MaxColors; ++cx)
		mPalette[cx] = mPaletteNew[cx];
}

void cSurface::paletteNew_SetToBlack() {

	for (size_t cx = 0; cx < g_MaxColors; ++cx) {
		mPaletteNew[cx].mBlue = 0;
		mPaletteNew[cx].mRed = 0;
		mPaletteNew[cx].mGreen = 0;
	}

    mPaletteAdjusting = true;
}

void cSurface::paletteSet( cPalette* pPalette, uint32 pColorID, uint32 pColors, bool pUseNow ) {

	// immediately switch to this new palette?
	if (pUseNow) {
		for (uint32 ColorID = pColorID; ColorID < pColorID + pColors; ++ColorID) {
			mPalette[ColorID] = pPalette[ColorID - pColorID];
		}

	} else {
		// No, we fade to it later
		for (uint32 ColorID = pColorID; ColorID < pColorID + pColors; ++ColorID) {
			mPaletteNew[ColorID] = pPalette[ColorID - pColorID];
		}
	}

	// Set the actual surface palette to mPalette
	surfaceSetToPalette();

	if (!pUseNow)
		mPaletteAdjusting = true;
}

/**
 * Fade each color of mPalette by a factor of 2, until mPalette reaches mPaletteNew
 *
 * @return True if still fading
 */
bool cSurface::palette_FadeTowardNew() {
	mPaletteAdjusting = false;

	// Loop each color 
	for(size_t cx = 0; cx < g_MaxColors; ++cx ) {

		// Each component of the current color
		for( int i = 0; i < 3; ++i ) {

			int8 al = mPaletteNew[cx].getPos(i);
			int8 bl = mPalette[cx].getPos(i);
			
			// Difference between current and target
			al -= bl;

			// No Difference?
			if(!al)
				continue;

			// We divide by two, 3 times, unless we reach 1 (the last possible enabled bit)
			if( al != 1 ) {
				al >>= 1;

				if( al != 1 ) {
					al >>= 1;

					if( al != 1 )
						al >>= 1;
				}
			}

			// loc_13918
			// Set the new color
			mPalette[cx].setPos(i, bl + al );

			// Still fading
			mPaletteAdjusting = true;
		}
	}

	surfaceSetToPalette();
	return !mPaletteAdjusting;
}

/**
 * Immediately apply mPalette to the surface palette
 */
void cSurface::surfaceSetToPalette() {

	for(size_t cx = 0; cx < g_MaxColors; ++cx )
		paletteSDLColorSet( cx, &mPalette[cx] );
}

/**
 * Immediately apply mPaletteNew to the surface palette
 */
void cSurface::surfaceSetToPaletteNew() {

	for(size_t cx = 0; cx < g_MaxColors; ++cx )
		paletteSDLColorSet( cx, &mPaletteNew[cx] );
}

/**
 * Calculate the surface palette color from the provided palette entry
 */
inline void cSurface::paletteSDLColorSet( size_t id, cPalette *pPalette ) {

	// Get the palette color for the provided RGB values
	mPaletteSDL[id] = SDL_MapRGB (	mSDLSurface->format , pPalette->mRed << 2, pPalette->mGreen << 2, pPalette->mBlue << 2) ;
}

/**
 * Draw the Surface Buffer to SDLSurface, using the surface palette
 */
void cSurface::draw() {

    if (mIsLoadedImage)
        return;

	const uint8 *bufferCurrent = mSurfaceBuffer;
	const uint8 *bufferCurrentMax = (mSurfaceBuffer + mSurfaceBufferSize);

	uint32 *bufferTarget = (uint32*)mSDLSurface->pixels;
	uint32 *bufferTargetMax = (uint32*) (((uint8*) mSDLSurface->pixels) + (mSDLSurface->h * mSDLSurface->pitch));
	
	clearSDLSurface(0);

	// Loop until we reach the destination end
	while( bufferTarget < bufferTargetMax ) {
			
		// Break out if we pass the source end
		if( bufferCurrent >= bufferCurrentMax )
			break;

		// Non zero value to draw
		if (*bufferCurrent) {

			// Value in palette range?
			if (*bufferCurrent < g_MaxColors)
				*bufferTarget = mPaletteSDL[*bufferCurrent];
			else
				*bufferTarget = 0;
		}

		// Next Source/Destination
		++bufferCurrent;
		++bufferTarget;
	}

	if(mTexture)
		SDL_UpdateTexture(mTexture, NULL, mSDLSurface->pixels, mSDLSurface->pitch);
}

/**
 * Merge another SDLsurface onto our rendered surface
 */
void cSurface::mergeSurfaceBuffer( const cSurface* pFrom ) {
    auto SourceSurface = pFrom->GetSurface();

    const uint32 *bufferCurrent = ((uint32*)SourceSurface->pixels);
    const uint32 *bufferCurrentMax = (uint32*)(((uint8*)SourceSurface->pixels) + (SourceSurface->h * SourceSurface->pitch));

    uint32 *bufferTarget = (uint32*)mSDLSurface->pixels;
    uint32 *bufferTargetMax = (uint32*)(((uint8*)mSDLSurface->pixels) + (mSDLSurface->h * mSDLSurface->pitch));

    // Loop until we reach the destination end
    while (bufferTarget < bufferTargetMax) {

        // Break out if we pass the source end
        if (bufferCurrent >= bufferCurrentMax)
            break;

        // Non zero value to draw
        if (*bufferCurrent) {

            // Value in palette range?
            *bufferTarget = *bufferCurrent;
        }

        // Next Source/Destination
        ++bufferCurrent;
        ++bufferTarget;
    }

    if (mTexture)
        SDL_UpdateTexture(mTexture, NULL, mSDLSurface->pixels, mSDLSurface->pitch);
}

void cSurface::clearBuffer(size_t pColor) {

	memset(mSurfaceBuffer, (int) pColor, mSurfaceBufferSize);
	memset(mSurfaceBufferSaved, (int) pColor, mSurfaceBufferSize);

	clearSDLSurface();
}

bool cSurface::LoadBitmap(const std::string& pFile) {
    auto Surface = SDL_LoadBMP(pFile.c_str());
    if (!Surface)
        return false;

    SDL_FreeSurface(mSDLSurface);
    mSDLSurface = Surface;

    mIsLoadedImage = true;
    mWidth = mSDLSurface->w;
    mHeight = mSDLSurface->h;

    if (g_Window->GetRenderer()) {
        SDL_DestroyTexture(mTexture);
        mTexture = SDL_CreateTextureFromSurface(g_Window->GetRenderer(), mSDLSurface );
        SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_ADD);
        SDL_SetTextureAlphaMod(mTexture, 0xa0);
        SDL_SetTextureColorMod(mTexture, 0xFF, 0xFF, 0xFF);
    }

    return mSDLSurface != 0;
}

void cSurface::Save() {

	memcpy( mSurfaceBufferSaved, mSurfaceBuffer, mSurfaceBufferSize );
}

void cSurface::Restore() {
	
	memcpy( mSurfaceBuffer, mSurfaceBufferSaved, mSurfaceBufferSize );
}
