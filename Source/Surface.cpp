/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

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
        mTexture = SDL_CreateTexture((SDL_Renderer*)g_Window->GetRenderer(), SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, (int)pWidth, (int)pHeight);

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
void cSurface::draw(const int16 pSkipX, const int16 pSkipY) {
	if (mIsLoadedImage)
		return;

	const uint8_t* bufferCurrent = mSurfaceBuffer;
	const uint8_t* const bufferEnd = mSurfaceBuffer + mSurfaceBufferSize;
	uint32_t* bufferTarget = reinterpret_cast<uint32_t*>(mSDLSurface->pixels);
	const int width = mSDLSurface->w, height = mSDLSurface->h;

	clearSDLSurface(0);

	// Skip 'skipY' rows of pixels
	bufferCurrent += width * pSkipY;
	bufferTarget += width * pSkipY;

	for (int y = pSkipY; y < height; ++y) {
		// Skip first 'skipX' pixels
		bufferCurrent += pSkipX;
		bufferTarget += pSkipX;

		// Process remaining pixels
		for (int x = pSkipX; x < width; ++x) {
			if (bufferCurrent >= bufferEnd) break;

			uint8_t currentPixel = *bufferCurrent++;
			if (currentPixel) {
				*bufferTarget = (currentPixel < g_MaxColors) ? mPaletteSDL[currentPixel] : 0;
			}

			++bufferTarget;
		}
	}

	if (mTexture) {
		SDL_UpdateTexture(mTexture, NULL, mSDLSurface->pixels, mSDLSurface->pitch);
	}
}

void cSurface::copyFrom(const cSurface* pFrom) {
	
	memcpy(mSurfaceBuffer, pFrom->mSurfaceBuffer, mSurfaceBufferSize);

	for (size_t cx = 0; cx < g_MaxColors; ++cx) {
		mPalette[cx] = pFrom->mPalette[cx];
		mPaletteNew[cx] = pFrom->mPaletteNew[cx];
		mPaletteSDL[cx] = pFrom->mPaletteSDL[cx];
	}
}

void cSurface::mergeFrom(const cSurface* pFrom) {
	auto SourceSurface = pFrom->GetSurfaceBuffer();
	auto SourceSize = pFrom->GetSurfaceBufferSize();

	uint32* bufferTarget = (uint32*)mSurfaceBuffer;
	uint32* bufferTargetMax = (uint32*)((uint8*)mSurfaceBuffer + mSurfaceBufferSize);

	const uint32* bufferCurrent = ((uint32*)SourceSurface);
	const uint32* bufferCurrentMax = (uint32*)(((uint8*)SourceSurface) + SourceSize);

	while (bufferTarget < bufferTargetMax) {

		if (*bufferCurrent) {

			// Value in palette range?
			*bufferTarget = *bufferCurrent;
		}

		// Next Source/Destination
		++bufferCurrent;
		++bufferTarget;
	}
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

bool cSurface::LoadPng(const std::string& pFile) {
    int32 width, height, bytesPerPixel;
    void* data = stbi_load(pFile.c_str(), &width, &height, &bytesPerPixel, 0);

    // Calculate pitch
    int pitch;
    pitch = width * bytesPerPixel;
    pitch = (pitch + 3) & ~3;

    // Setup relevance bitmask
    uint32 Rmask, Gmask, Bmask, Amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    Rmask = 0x000000FF;
    Gmask = 0x0000FF00;
    Bmask = 0x00FF0000;
    Amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
#else
    int s = (bytesPerPixel == 4) ? 0 : 8;
    Rmask = 0xFF000000 >> s;
    Gmask = 0x00FF0000 >> s;
    Bmask = 0x0000FF00 >> s;
    Amask = 0x000000FF >> s;
#endif
    auto Surface = SDL_CreateRGBSurfaceFrom(data, width, height, bytesPerPixel*8, pitch, Rmask, Gmask, Bmask, Amask);
    if (!Surface)
        return false;
    SDL_FreeSurface(mSDLSurface);
    mSDLSurface = Surface;

    mIsLoadedImage = true;
    mWidth = mSDLSurface->w;
    mHeight = mSDLSurface->h;

    if (g_Window->GetRenderer()) {
        SDL_DestroyTexture(mTexture);
        mTexture = SDL_CreateTextureFromSurface((SDL_Renderer*)g_Window->GetRenderer(), mSDLSurface );
        SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_ADD);
        SDL_SetTextureAlphaMod(mTexture, 0xa0);
        SDL_SetTextureColorMod(mTexture, 0xFF, 0xFF, 0xFF);
    }

    return mSDLSurface != nullptr;
}

void cSurface::Save() {

	memcpy( mSurfaceBufferSaved, mSurfaceBuffer, mSurfaceBufferSize );
}

void cSurface::Restore() {
	
	memcpy( mSurfaceBuffer, mSurfaceBufferSaved, mSurfaceBufferSize );
}
