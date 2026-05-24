/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
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

static void SurfacePng_WriteBE32(std::vector<uint8>& pOut, uint32 pValue) {
	pOut.push_back((uint8)((pValue >> 24) & 0xFF));
	pOut.push_back((uint8)((pValue >> 16) & 0xFF));
	pOut.push_back((uint8)((pValue >> 8) & 0xFF));
	pOut.push_back((uint8)(pValue & 0xFF));
}

static uint32 SurfacePng_Crc32(const uint8* pData, size_t pSize) {
	uint32 crc = 0xFFFFFFFF;

	for (size_t i = 0; i < pSize; ++i) {
		crc ^= pData[i];
		for (int bit = 0; bit < 8; ++bit)
			crc = (crc & 1) ? (0xEDB88320 ^ (crc >> 1)) : (crc >> 1);
	}

	return ~crc;
}

static uint32 SurfacePng_Adler32(const uint8* pData, size_t pSize) {
	uint32 a = 1;
	uint32 b = 0;

	for (size_t i = 0; i < pSize; ++i) {
		a = (a + pData[i]) % 65521;
		b = (b + a) % 65521;
	}

	return (b << 16) | a;
}

static void SurfacePng_AppendChunk(std::vector<uint8>& pOut, const char* pType, const std::vector<uint8>& pData) {
	SurfacePng_WriteBE32(pOut, (uint32)pData.size());

	const size_t crcStart = pOut.size();
	pOut.push_back((uint8)pType[0]);
	pOut.push_back((uint8)pType[1]);
	pOut.push_back((uint8)pType[2]);
	pOut.push_back((uint8)pType[3]);

	pOut.insert(pOut.end(), pData.begin(), pData.end());

	const uint32 crc = SurfacePng_Crc32(pOut.data() + crcStart, pOut.size() - crcStart);
	SurfacePng_WriteBE32(pOut, crc);
}

cSurface::cSurface( size_t pWidth, size_t pHeight ) {
    mIsLoadedImage = false;
	mWidth = pWidth; 
	mHeight = pHeight;
	mPaletteAdjusting = false;

	// Create the screen buffer
	mSDLSurface = SDL_CreateSurface((int)pWidth, (int)pHeight, SDL_PIXELFORMAT_XRGB8888);
	mTexture = 0;

	if (!mSDLSurface) {
		g_Debugger->Error("SDLSurface not initialised");
		exit(1);
	}
    if (g_Window->GetRenderer()) {
        mTexture = SDL_CreateTexture((SDL_Renderer*)g_Window->GetRenderer(), SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, (int)pWidth, (int)pHeight);

		
		if (g_Fodder->mParams->mIntegerScaling || !g_Fodder->mWindow->isFullscreen()) {
			SDL_SetTextureScaleMode(mTexture, SDL_SCALEMODE_NEAREST);
		} else {
			SDL_SetTextureScaleMode(mTexture, SDL_SCALEMODE_LINEAR);
		}
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

	SDL_DestroySurface( mSDLSurface );

	if(mTexture)
		SDL_DestroyTexture( mTexture );
}

void cSurface::clearSDLSurface( uint32 pColor ) {

	SDL_FillSurfaceRect(mSDLSurface, nullptr, pColor);
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

void cSurface::paletteNew_SetDimmed(uint8 pScale) {
	for (size_t cx = 0; cx < g_MaxColors; ++cx) {
		mPaletteNew[cx].mRed = (uint8)((mPalette[cx].mRed * pScale) >> 8);
		mPaletteNew[cx].mGreen = (uint8)((mPalette[cx].mGreen * pScale) >> 8);
		mPaletteNew[cx].mBlue = (uint8)((mPalette[cx].mBlue * pScale) >> 8);
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
	const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(mSDLSurface->format);
	mPaletteSDL[id] = SDL_MapRGB(format, nullptr, pPalette->mRed << 2, pPalette->mGreen << 2, pPalette->mBlue << 2);
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
	const uint8* source = pFrom->GetSurfaceBuffer();
	const size_t sourceSize = pFrom->GetSurfaceBufferSize();
	const size_t copySize = std::min(mSurfaceBufferSize, sourceSize);

	for (size_t i = 0; i < copySize; ++i) {
		const uint8 value = source[i];
		if (value) {
			mSurfaceBuffer[i] = value;
		}
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
    SDL_PixelFormat format = SDL_GetPixelFormatForMasks(bytesPerPixel * 8, Rmask, Gmask, Bmask, Amask);
    if (format == SDL_PIXELFORMAT_UNKNOWN) {
        return false;
    }
    auto Surface = SDL_CreateSurfaceFrom(width, height, format, data, pitch);
    if (!Surface)
        return false;
    SDL_DestroySurface(mSDLSurface);
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

bool cSurface::SavePng(const std::string& pFile) const {
	if (!mSDLSurface)
		return false;

	SDL_Surface* Surface = mSDLSurface;
	SDL_Surface* Converted = 0;

	if (Surface->format != SDL_PIXELFORMAT_XRGB8888) {
		Converted = SDL_ConvertSurface(Surface, SDL_PIXELFORMAT_XRGB8888);
		if (!Converted)
			return false;

		Surface = Converted;
	}

	if (!SDL_LockSurface(Surface)) {
		if (Converted)
			SDL_DestroySurface(Converted);
		return false;
	}

	const int SurfaceWidth = Surface->w;
	const int SurfaceHeight = Surface->h;
	std::vector<uint8> Raw;
	Raw.reserve(((size_t)SurfaceWidth * 3 + 1) * (size_t)SurfaceHeight);

	const SDL_PixelFormatDetails* Format = SDL_GetPixelFormatDetails(Surface->format);
	for (int y = 0; y < SurfaceHeight; ++y) {
		Raw.push_back(0); // PNG filter type: none.

		const uint32* Row = (const uint32*)((const uint8*)Surface->pixels + (y * Surface->pitch));
		for (int x = 0; x < SurfaceWidth; ++x) {
			uint8 r = 0;
			uint8 g = 0;
			uint8 b = 0;

			SDL_GetRGB(Row[x], Format, 0, &r, &g, &b);
			Raw.push_back(r);
			Raw.push_back(g);
			Raw.push_back(b);
		}
	}

	SDL_UnlockSurface(Surface);

	if (Converted)
		SDL_DestroySurface(Converted);

	std::vector<uint8> Png;
	const uint8 Signature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	Png.insert(Png.end(), Signature, Signature + sizeof(Signature));

	std::vector<uint8> Ihdr;
	SurfacePng_WriteBE32(Ihdr, (uint32)SurfaceWidth);
	SurfacePng_WriteBE32(Ihdr, (uint32)SurfaceHeight);
	Ihdr.push_back(8); // bit depth
	Ihdr.push_back(2); // RGB
	Ihdr.push_back(0); // compression
	Ihdr.push_back(0); // filter
	Ihdr.push_back(0); // interlace
	SurfacePng_AppendChunk(Png, "IHDR", Ihdr);

	std::vector<uint8> Idat;
	Idat.reserve(Raw.size() + (Raw.size() / 65535 + 1) * 5 + 6);
	Idat.push_back(0x78);
	Idat.push_back(0x01);

	size_t Offset = 0;
	while (Offset < Raw.size()) {
		const size_t Remaining = Raw.size() - Offset;
		const uint16 BlockSize = (uint16)std::min<size_t>(Remaining, 65535);
		const bool FinalBlock = Remaining == BlockSize;
		const uint16 NLen = (uint16)~BlockSize;

		Idat.push_back(FinalBlock ? 1 : 0);
		Idat.push_back((uint8)(BlockSize & 0xFF));
		Idat.push_back((uint8)((BlockSize >> 8) & 0xFF));
		Idat.push_back((uint8)(NLen & 0xFF));
		Idat.push_back((uint8)((NLen >> 8) & 0xFF));
		Idat.insert(Idat.end(), Raw.begin() + Offset, Raw.begin() + Offset + BlockSize);

		Offset += BlockSize;
	}

	SurfacePng_WriteBE32(Idat, SurfacePng_Adler32(Raw.data(), Raw.size()));
	SurfacePng_AppendChunk(Png, "IDAT", Idat);

	std::vector<uint8> Iend;
	SurfacePng_AppendChunk(Png, "IEND", Iend);

	std::ofstream OutFile(pFile, std::ofstream::binary);
	if (!OutFile.is_open())
		return false;

	OutFile.write((const char*)Png.data(), Png.size());
	OutFile.close();
	return OutFile.good();
}

void cSurface::Save() {

	memcpy( mSurfaceBufferSaved, mSurfaceBuffer, mSurfaceBufferSize );
}

void cSurface::Restore() {
	
	memcpy( mSurfaceBuffer, mSurfaceBufferSaved, mSurfaceBufferSize );
}
