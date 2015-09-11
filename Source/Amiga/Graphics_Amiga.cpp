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

cGraphics_Amiga::cGraphics_Amiga() : cGraphics() {
	mBlkData = 0;
	mPalette = 0;
	mHeight = 0;

	memset( &mPaletteArmy, 0, 0x20 );
	memset( &mPaletteCopt, 0, 0x20 );
	memset( &mPalletePStuff, 0, 0x20 );
}

uint8* cGraphics_Amiga::GetSpriteData( uint16 pSegment ) {
	
	switch (pSegment) {
	case 0:
		mHeight = mBMHDArmy.mHeight;
		return g_Fodder.mDataArmy;
		
	case 1:
		mHeight = mBMHDCopt.mHeight;
		return g_Fodder.mDataHillBits;

	case 2:
		mHeight = mBMHDPStuff.mHeight;
		return g_Fodder.mDataPStuff;
	}
}

void cGraphics_Amiga::LoadpStuff() {

	size_t Size = 0;
	uint8* pstuff = g_Resource.fileGet( "pstuff.lbm", Size );

	DecodeIFF( pstuff, g_Fodder.mDataPStuff, &mBMHDPStuff, mPalletePStuff );

	delete[] pstuff;
}

void cGraphics_Amiga::SetSpritePtr( eSpriteType pSpriteType ) {

	switch (pSpriteType) {
		case eSPRITE_IN_GAME:
			g_Fodder.Sprite_SetDataPtrToBase( off_8BFB8 );
			return;
	}
}

void cGraphics_Amiga::graphicsBlkPtrsPrepare() {

	delete mBlkData;
	delete mPalette;

	mBlkData = new uint8[g_Fodder.mDataBaseBlkSize + g_Fodder.mDataSubBlkSize];
	mPalette = g_Resource.fileGet( "afxbase.pal", mPaletteSize );

	memcpy( mBlkData, g_Fodder.mDataBaseBlk, g_Fodder.mDataBaseBlkSize );
	memcpy( mBlkData + g_Fodder.mDataBaseBlkSize, g_Fodder.mDataSubBlk, g_Fodder.mDataSubBlkSize );
}

void cGraphics_Amiga::PaletteSet() {

	mImage->paletteLoad_Amiga( mPalette, 0 );
	mImage->paletteLoad_Amiga( (uint8*) mPaletteArmy, 0x0 );
	//mImage->paletteLoad_Amiga( mPaletteCopt, 0x0 );
}


bool cGraphics_Amiga::DecodeIFF( uint8* pData, uint8* pDataDest, sILBM_BMHD* pBMHD, uint16* pPalette ) {

	if (readBEDWord( pData ) != 'FORM')
		return false;
	pData += 4;

	size_t FileSize = readBEDWord( pData );
	pData += 4;

	if (readBEDWord( pData ) != 'ILBM')
		return false;
	pData += 4;
	FileSize -= 4;

	uint32 Header = 0;
	uint32 Size = 0;

	while (FileSize > 0) {
		Header = readBEDWord( pData );
		pData += 4; FileSize -= 4;
		Size = readBEDWord( pData );
		pData += 4; FileSize -= 4;

		switch (Header) {
		case 'BMHD':
			pBMHD->mWidth = readBEWord( pData ); pData += 2;
			pBMHD->mHeight = readBEWord( pData ); pData += 2;
			pData += 2; pData += 2;	// X, Y
			pBMHD->mPlanes = *pData++;
			pBMHD->mMask = *pData++;
			pBMHD->mCompression = *pData++;
			pData += 2; ++pData; ++pData; ++pData;
			pData += 2; pData += 2;
			FileSize -= Size;
			break;
		
		case 'BODY': {
			int16 Width = pBMHD->mWidth + 0x0F;
			Width >>= 4;
			Width <<= 1;

			int16 Height = pBMHD->mHeight - 1;

			for (int16 Y = Height; Y >= 0; --Y) {
				uint8* DataDest = pDataDest;

				for (int8 Plane = 0; Plane < pBMHD->mPlanes; ++Plane) {

					for (int16 X = Width; X > 0;) {
						--FileSize;
						int8 d0 = *pData++;
						
						if(d0 < 0) {
							if(d0 == -128)
								continue;
							
							int8 d1 = -d0;
							
							--FileSize;
							d0 = *pData++;
							
							do {
								*DataDest++ = d0;
								--X;
							} while( d1-- > 0 );
							
							continue;
							
						} else {
							
							do {
								*DataDest++ = *pData++;
								--X;
								--FileSize;
							} while( d0-- > 0 );
							
						}
					}
					
					// Move the destination back to start of row
					DataDest -= Width;

					// Move it to the next plane
					DataDest += (Width * pBMHD->mHeight);
				}

				// Next Row
				pDataDest += Width;
			}

			break;
		}

		case 'CMAP':
			for (int16 i = 0; i < 0x10; ++i) {
				int16 d0 = (int16) *pData++;
				int16 Final = 0;

				d0 >>= 4;
				d0 <<= 8;
				Final += d0;

				d0 = *pData++;
				d0 >>= 4;
				d0 = (int16)d0;
				d0 <<= 4;
				Final += d0;

				d0 = *pData++;
				d0 >>= 4;
				d0 = (int16)d0;
				Final += d0;

				writeBEWord( pPalette, Final );
				pPalette++;

			}
			FileSize -= 0x30;
			break;

		default:
			if (Size & 1)
				++Size;

			pData += Size;
			FileSize -= Size;
			break;
		}
	}
}

void cGraphics_Amiga::map_Tiles_Draw() {
	mImage->clearBuffer();
	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = &g_Fodder.mMap[g_Fodder.mMapTilePtr];

	// Y
	for (uint16 cx = 0; cx < 0x0F; ++cx) {

		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = g_Fodder.word_3B610;
		else
			StartY = 0;

		// X
		for (uint16 cx2 = 0; cx2 < 0x16; ++cx2) {
			uint8* TargetTmp = TargetRow;

			uint16 Tile = readLEWord( MapPtr ) & 0x1FF;

			if (/*Tile > 0xE0 && Tile < 0xF0 ||*/ Tile > 0x1DF)
				Tile = 0;

			Tile <<= 7;
			uint8* TilePtr = &mBlkData[Tile];
			uint16 StartX = 0;

			TilePtr += StartY * 2;
			
			if (cx2 == 0)
				StartX = g_Fodder.word_3B60E;
			else
				StartX = 0;


			// Each bitfield
			for (uint16 BitField = 0; BitField < 4; ++BitField) {

				// Each Tile Row
				for (uint16 i = StartY; i < 16; ++i) {

					uint16 RowData = readBEWord( TilePtr );
					TilePtr += 2;
					RowData <<= StartX;

					// Each pixel of a Tile Row
					for (uint16 x = StartX; x < 16; ++x) {
						uint8 Bit = (RowData & 0x8000) ? 1 : 0;
						RowData <<= 1;

						if (Bit)
							*(TargetTmp + x) |= (Bit << BitField);

					}

					TargetTmp += mImage->GetWidth();
				}

				// Next Bitfield
				TargetTmp = TargetRow;
			}

			MapPtr += 2;
			TargetRow += (16-StartX);
		}

		Target += mImage->GetWidth() * (16-StartY);
		CurrentMapPtr += g_Fodder.mMapWidth << 1;
	}
	
	mImage->Save();
}

void cGraphics_Amiga::map_Load_Resources() {
	g_Fodder.mFilenameCopt = g_Fodder.sub_12AA1( g_Fodder.mFilenameCopt, "lbm" );
	g_Fodder.mFilenameArmy = g_Fodder.sub_12AA1( g_Fodder.mFilenameArmy, "lbm" );

	size_t Size = 0;
	uint8* Copt = g_Resource.fileGet( g_Fodder.mFilenameCopt, Size );
	uint8* Army = g_Resource.fileGet( g_Fodder.mFilenameArmy, Size );

	DecodeIFF( Copt, g_Fodder.mDataHillBits, &mBMHDCopt, mPaletteCopt );
	DecodeIFF( Army, g_Fodder.mDataArmy, &mBMHDArmy, mPaletteArmy );

	delete[] Copt;
	delete[] Army;

	SetSpritePtr( eSPRITE_IN_GAME );
}

void cGraphics_Amiga::video_Draw_Sprite() {
	uint16 height = mHeight;

	cFodder* Fodder = cFodder::GetSingletonPtr();

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = Fodder->word_42062;
	int16	ax, cx;
	
	di += mImage->GetWidth() * Fodder->mDrawSpritePositionY;

	ax = Fodder->mDrawSpritePositionX;
	ax += Fodder->word_40054;
	//ax >>= 2;
	
	di += ax;
	Fodder->word_42066 = di;
	cx = Fodder->mDrawSpritePositionX;
	cx += Fodder->word_40054;
	cx &= 3;

	Fodder->byte_42071 = 1 << cx;
	int8 bl = Fodder->byte_42070;
	
	
	Fodder->word_42074 = 40 - Fodder->word_4206C;
	Fodder->word_4206C >>= 1;
	Fodder->word_42076 = 352 - (Fodder->word_4206C*16);

	// Height
	for (int16 dx = Fodder->word_4206E; dx > 0; --dx) {

		// Width
		for (cx = 0; cx < Fodder->word_4206C; ++cx) {

			// Each Pixel
			for (uint16 x = 0; x < 16; ++x) {

				int8 Pixel = GetPixel( x, height, si );

				if (Pixel)
					*(di + x) = Pixel;
			}

			di += 16;
			si += 2;
		}

		si += Fodder->word_42074;
		di += Fodder->word_42076;
	}
}

uint8 cGraphics_Amiga::GetPixel( uint8 pixel, uint16 height, uint8* pSource ) {
	uint8 Result = 0;
	uint16 Bits = readBEWord( pSource );

	Result = (Bits << pixel) & 0x8000 ? 1 : 0;

	Bits = readBEWord( pSource + (height * 40) );
	Result |= (Bits << pixel) & 0x8000 ? 2 : 0;

	Bits = readBEWord( pSource + ((height * 40) * 2));
	Result |= (Bits << pixel) & 0x8000 ? 4 : 0;

	Bits = readBEWord( pSource + ((height * 40) * 3));
	Result |= (Bits << pixel) & 0x8000 ? 8 : 0;

	return Result;
}
