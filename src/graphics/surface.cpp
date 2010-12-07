#include "stdafx.h"
#include "surface.hpp"

cSurface::cSurface( size_t pWidth, size_t pHeight ) {
	mWidth = pWidth; 
	mHeight = pHeight;

	mColorKey = 0xFF;

	// Create the screen buffer
	mSDLSurface = SDL_CreateRGBSurface(	SDL_SWSURFACE,	pWidth,	pHeight,	 32, 0, 0, 0, 0);
	
	SDL_SetColorKey(mSDLSurface, SDL_SRCCOLORKEY, 0xFFFFFFFF	);

	mSurfaceBuffer = 0;
	mSurfaceBufferSize = 0;

	wipe();
}

cSurface::~cSurface() {
	delete mSurfaceBuffer;

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

void cSurface::blit( cSurface *pSource, word destX, word destY, dword colorKey, dword colorKey2, word pMaxHeight ) {

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

			if(srcSurface == srcEnd)
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

void cSurface::unblit( cSurface	*Source, word destX, word destY, dword colorKey ) {
		if(!Source)
			return;

		dword *bufferSrc = Source->pixelGet( 0, 0 );
		dword *bufferDst = pixelGet( destX, destY );
		word x, y, maxX, maxY;

		maxX = Source->widthGet();
		maxY = Source->heightGet(); // -1

		dword difference = mWidth - maxX;

		for(y = 0; y < maxY; y++) {
			for(x = 0; x < maxX; x++) {
	
				if(*bufferSrc == *bufferDst && *bufferSrc != colorKey ) {
					*bufferDst++ = 0x00;
					bufferSrc++;
				} else {
					bufferSrc++;
					bufferDst++;
				}

			}

			bufferDst += difference;
		}
	}
void cSurface::paletteLoad( const byte  *pBuffer ) {
	word  color;
	byte  colorID, colorRed, colorGreen, colorBlue;

	for( colorID = 0; colorID < 16; colorID++) {
		
		// Get the next color codes
		color = readLEWord( pBuffer );
		pBuffer+=2;

		// Extract each color from the word
		//  X X X X   R3 R2 R1 R0     G3 G2 G1 G0   B3 B2 B1 B0
		colorRed	=	(color >> 8) & 0xF;
		colorGreen	=	(color >> 4) & 0xF;
		colorBlue	=	(color >> 0) & 0xF;
		
		colorRed	<<= 5;
		colorGreen	<<= 5;
		colorBlue	<<= 5;

		// Set the color in the palette
		paletteColorSet( colorID, colorRed, colorGreen, colorBlue );
	}
}

byte cSurface::paletteIndexGet( dword pColor ) {
	byte  colorID;

	for( colorID = 0; colorID < 16; colorID++) {
		if( mPalette[colorID] == pColor )
			return colorID;
	}

	return 0;
}

inline void cSurface::paletteColorSet( byte id, byte red, byte green, byte blue ) {
	if(id >= 16)
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

void cSurface::decode(	size_t numBits, byte colorMask, const byte *buffer, size_t bufferSize ) {
	byte	 bitCount, paletteIndex, count, modifier;
	word	*bufferCurrent = (word*) buffer;
	byte	*bufferDest;
	word	 colorIndex[4];
	
	// Set our buffer size, 8 is bits per byte
	mSurfaceBufferSize = (bufferSize / numBits) * 8;

	// Create the buffer
	delete mSurfaceBuffer;
	mSurfaceBuffer = new byte[ mSurfaceBufferSize];
	bufferDest = (byte*) mSurfaceBuffer;
	memset( mSurfaceBuffer, 0, mSurfaceBufferSize );

	// Clear the surface
	wipe( mPalette[ colorMask ]);

	// Loop until end of source is reached
	while( bufferCurrent < (word*) (buffer + bufferSize) ) {
		
		// Read in 'numBits' WORDs from the source
		for( count = 0; count < numBits; count++)
			colorIndex[count] = readLEWord( bufferCurrent++ );

		// Loop for each bit of the WORDs
		for( bitCount = 0; bitCount < 16; bitCount++) {
			paletteIndex = 0x00;
				
			// Read the index bit for the pixel from each bitplane
			for( count = 0, modifier = 1; count < numBits; count++ ) {
				
				if( (colorIndex[count] & 0x8000))
					paletteIndex += modifier;

				// Move to the next pixels bit-value
				modifier <<= 1;

				// Shift the source pixels
				colorIndex[count] <<= 1;
			}

			// index 0 is the colormask
			if( paletteIndex == 0)
				paletteIndex = colorMask;

			// Write the palette index to the destination
			(*bufferDest++) = paletteIndex;
		}	// for bitCount < 16
			
	}	// while bufferCurrent
	
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

		if( *bufferCurrent < 16 )
			*bufferTarget = mPalette[ *bufferCurrent ];
		else
			*bufferTarget = 0;

		++bufferTarget; 
		++bufferCurrent;
	}
	
}

void cSurface::drawStrips( size_t pX, size_t pY ) {
	register size_t X = pX, Y = pY, count = 0;

	byte *bufferCurrent = mSurfaceBuffer;
	byte *bufferCurrentMax = (mSurfaceBuffer + mSurfaceBufferSize);

	dword *bufferTarget = pixelGet( pX, pY );
	dword *bufferTargetMax = (dword*) (((byte*) mSDLSurface->pixels) + (mSDLSurface->h * mSDLSurface->pitch));

	wipe(0xFFFFFFFF);

	// Loop until the end of the target buffer is reached
	while( bufferTarget < bufferTargetMax ) {
		
		// Exit if we pass the end of the source
		if( bufferCurrent >= bufferCurrentMax )
			break;

		// Write the pixel using the color from the palette
		if( *bufferCurrent < 16 )
			*bufferTarget = mPalette[ *bufferCurrent ];

		// Move forward
		++bufferTarget; 
		++bufferCurrent;
		++X;

		// Reached end of this strip?
		if(++count == 16) {
			count = 0;

			// Next row
			++Y;

			// Maximum height for strip?
			if( Y == mHeight ) {
				Y = pY;

				// Next column
				pX += 16;
			}

			X = pX;
		}

		// Grab target pixel
		bufferTarget = pixelGet( X, Y );
	}
}

void cSurface::fade( size_t pStepSize ) {
	dword *buffer = pixelGet( 0, 0 );
	byte colorRed, colorGreen, colorBlue;
	word x, y;
	word Color;

	for(y = 0; y < mHeight; y++) {
		for(x = 0; x < mWidth; x++) {
			*buffer /= 2;

			SDL_GetRGB( *buffer, mSDLSurface->format, &colorRed, &colorGreen, &colorBlue);
			Color = (colorRed << 4) + (colorGreen) + (colorBlue >> 4);
			
			Color &= 0x777;
			Color -= pStepSize;

			while(Color & 0x8)
				Color+=0x1;

			while(Color & 0x80) 
				Color+=0x10;

			while(Color & 0x800)
				Color+=0x100;

			Color &= 0x777;

			colorRed	= (Color >> 8) & 0xF;
			colorGreen	= (Color >> 4) & 0xF;
			colorBlue	= (Color >> 0) & 0xF;

			colorRed	<<= 5;
			colorGreen	<<= 5;
			colorBlue	<<= 5;

			*buffer = SDL_MapRGB( mSDLSurface->format, colorRed, colorGreen, colorBlue);

			buffer++;
		} // X
	}	// Y
}

void cSurface::imageLoadPaletteDraw( const byte *pBuffer, size_t &pSize ) {
	
	paletteLoad( pBuffer + 0x04 );
	decode( 4, 0xFF, pBuffer + 0x80, pSize - 0x80);
	draw();

}

void cSurface::imageLoad( const byte *pBuffer, size_t &pSize, size_t pNumBits, byte pColorMask, bool pStrips ) {
	
	decode( pNumBits, pColorMask, pBuffer, pSize );

	if(pStrips)
		drawStrips();
	else
		draw();
}
