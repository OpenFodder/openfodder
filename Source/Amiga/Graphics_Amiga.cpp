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

	mImage->paletteLoad_Amiga( mPalette );
}

bool cGraphics_Amiga::DecodeIFF( uint8* pData ) {

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
	sILBM_BMHD BMHD;

	while (FileSize > 0) {
		Header = readBEDWord( pData );
		pData += 4; FileSize -= 4;
		Size = readBEDWord( pData );
		pData += 4; FileSize -= 4;

		switch (Header) {
		case 'BMHD':
			BMHD.mWidth = readBEWord( pData ); pData += 2;
			BMHD.mHeight = readBEWord( pData ); pData += 2;
			pData += 2; pData += 2;	// X, Y
			BMHD.mPlanes = *pData++;
			BMHD.mMask = *pData++;
			BMHD.mCompression = *pData++;
			pData += 2; ++pData; ++pData; ++pData;
			pData += 2; pData += 2;
			FileSize -= Size;
			break;
		
		case 'BODY': {
			int16 Width = BMHD.mWidth + 0x0F;
			Width >>= 4;
			Width <<= 1;

			int16 Height = BMHD.mHeight - 1;

			for (int16 Y = Height; Y >= 0; --Y) {
				for (int8 Plane = 0; Plane < BMHD.mPlanes; ++Plane) {

					for (int16 X = Width; X > 0; --X) {

						int8 d0 = *pData++;
					}
				}
			}
		}
			


		case 'CMAP':
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
