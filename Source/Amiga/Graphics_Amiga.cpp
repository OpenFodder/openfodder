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

	memset( &mPaletteArmy, 0, 0x10 );
	memset( &mPaletteCopt, 0, 0x10 );
	memset( &mPalletePStuff, 0, 0x10 );
	memset( &mPalleteAfx, 0, 0x20 );

}

uint8* cGraphics_Amiga::GetSpriteData( uint16 pSegment ) {

	switch (pSegment) {
	case 0:
		mFodder->byte_42070 = 0;
		mBMHD_Current = &mBMHDArmy;
		return mFodder->mDataArmy;

	case 1:
		mFodder->byte_42070 = 0;
		mBMHD_Current = &mBMHDCopt;
		return mFodder->mDataHillBits;

	case 2:
		mFodder->byte_42070 = mCursorPalette;
		mBMHD_Current = &mBMHDPStuff;
		return mFodder->mDataPStuff;

	default:
		std::cout << "cGraphics_Amiga::GetSpriteData: Invalid ID " << pSegment << "\n";
		return 0;
	}

}

void cGraphics_Amiga::Mouse_DrawCursor() {
	const struct_2* di = &stru_44B50[mFodder->mMouseSpriteCurrent];
	mFodder->mouseData1->anonymous_5 = di->field_4;
	mFodder->mouseData1->anonymous_6 = di->field_6;

	mFodder->word_4206C = di->field_4 >> 3;
	mFodder->word_4206E = di->field_6;

	int16 ax = di->field_2 & 0xFF;
	int16 bx = di->field_0 >> 3 & -2;

	ax <<= 3;
	int16 d1 = ax;

	ax <<= 2;
	ax += d1;

	mFodder->word_42062 = GetSpriteData( 2 ) + (ax + bx);
	video_Draw_Sprite();
}

void cGraphics_Amiga::LoadpStuff() {

	size_t Size = 0;
	uint8* pstuff = g_Resource.fileGet( "pstuff.lbm", Size );

	DecodeIFF( pstuff, mFodder->mDataPStuff, &mBMHDPStuff, mPalletePStuff );

	delete[] pstuff;
}

void cGraphics_Amiga::Load_Sprite_Font() {
	size_t Size = 0;

	g_Resource.fileLoadTo( "font.raw", mFodder->mDataPStuff );

	uint8* Font = g_Resource.fileGet( "font.pl8", Size );

	PaletteLoad( Font, 0xD0, 0x04 );

	SetSpritePtr( eSPRITE_FONT );

	delete[] Font;
}

void cGraphics_Amiga::LoadAFXMenu() {
	size_t Size = 0;
	uint8* apmenu = g_Resource.fileGet( "apmenu.lbm", Size );

	DecodeIFF( apmenu, mFodder->mDataBaseBlk, &mBMHDAfx, mPalleteAfx );

	mImage->paletteLoad_Amiga( (uint8*)mPalleteAfx, 0, 32 );

	mBMHD_Current = &mBMHDAfx;

	delete[] apmenu;
}

void cGraphics_Amiga::PaletteLoad( const uint8  *pBuffer, uint32 pColorID, uint32 pColors ) {
	int16  color;
	int16  ColorID = pColorID;

	for( ; pColorID < ColorID + pColors; pColorID++) {
		
		// Get the next color codes
		color = readBEWord( pBuffer );
		pBuffer+=2;

		// Extract each color from the word
		//  X X X X   R3 R2 R1 R0     G3 G2 G1 G0   B3 B2 B1 B0

		mFodder->mPalette[pColorID].mRed	= ((color >> 8) & 0xF)   << 2;	// Why 2? no idea, but it works.. 1 is too dark, and 3 causes incorrect colours
		mFodder->mPalette[pColorID].mGreen	= ((color >> 4) & 0xF) << 2;
		mFodder->mPalette[pColorID].mBlue	= ((color >> 0) & 0xF)  << 2;
	}
}

void cGraphics_Amiga::SetCursorPalette( uint16 pIndex ) {
	mCursorPalette = pIndex;
}

void cGraphics_Amiga::SetSpritePtr( eSpriteType pSpriteType ) {

	switch (pSpriteType) {
	case eSPRITE_IN_GAME:
		mFodder->Sprite_SetDataPtrToBase( off_8BFB8 );
		return;

	case eSPRITE_FONT:

		return;
	}
}

void cGraphics_Amiga::graphicsBlkPtrsPrepare() {

	delete mBlkData;
	delete mPalette;

	mBlkData = new uint8[mFodder->mDataBaseBlkSize + mFodder->mDataSubBlkSize];
	mPalette = g_Resource.fileGet( mFodder->mFilenameBasePal, mPaletteSize );

	memcpy( mBlkData, mFodder->mDataBaseBlk, mFodder->mDataBaseBlkSize );
	memcpy( mBlkData + mFodder->mDataBaseBlkSize, mFodder->mDataSubBlk, mFodder->mDataSubBlkSize );
}

void cGraphics_Amiga::PaletteSet() {

	mImage->paletteLoad_Amiga( mPalette, 0 );
	//mImage->paletteLoad_Amiga( (uint8*) mPaletteArmy, 0x80 );
	mImage->paletteLoad_Amiga( (uint8*)mPalletePStuff, 0xF0 );
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

	while (FileSize > 8) {
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

						if (d0 < 0) {
							if (d0 == -128)
								continue;

							int8 d1 = -d0;

							--FileSize;
							d0 = *pData++;

							do {
								*DataDest++ = d0;
								--X;
							} while (d1-- > 0);

							continue;

						}
						else {

							do {
								*DataDest++ = *pData++;
								--X;
								--FileSize;
							} while (d0-- > 0);

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
			for (int16 i = 0; i < Size / 3; ++i) {
				int16 d0 = (int16)*pData++;
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
				FileSize -= 3;

			}
			break;

		default:
			if (Size & 1)
				++Size;

			pData += Size;
			FileSize -= Size;
			break;
		}
	}

	return true;
}

void cGraphics_Amiga::map_Tiles_Draw() {
	mImage->clearBuffer();
	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = &mFodder->mMap[mFodder->mMapTilePtr];

	// Y
	for (uint16 cx = 0; cx < 0x10; ++cx) {

		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = mFodder->word_3B610;
		else
			StartY = 0;

		// X
		for (uint16 cx2 = 0; cx2 < 0x17; ++cx2) {
			uint8* TargetTmp = TargetRow;

			uint16 Tile = readLEWord( MapPtr ) & 0x1FF;

			if (/*Tile > 0xE0 && Tile < 0xF0 ||*/ Tile > 0x1DF)
				Tile = 0;

			Tile <<= 7;
			uint8* TilePtr = &mBlkData[Tile];
			uint16 StartX = 0;

			TilePtr += StartY * 2;

			if (cx2 == 0)
				StartX = mFodder->word_3B60E;
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
			TargetRow += (16 - StartX);
		}

		Target += mImage->GetWidth() * (16 - StartY);
		CurrentMapPtr += mFodder->mMapWidth << 1;
	}

	mImage->Save();
}

void cGraphics_Amiga::sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY ) {

}

void cGraphics_Amiga::map_Load_Resources() {
	mFodder->mFilenameCopt = mFodder->sub_12AA1( mFodder->mFilenameCopt, "lbm" );
	mFodder->mFilenameArmy = mFodder->sub_12AA1( mFodder->mFilenameArmy, "lbm" );

	size_t Size = 0;
	uint8* Copt = g_Resource.fileGet( mFodder->mFilenameCopt, Size );
	uint8* Army = g_Resource.fileGet( mFodder->mFilenameArmy, Size );

	DecodeIFF( Copt, mFodder->mDataHillBits, &mBMHDCopt, mPaletteCopt );
	DecodeIFF( Army, mFodder->mDataArmy, &mBMHDArmy, mPaletteArmy );

	delete[] Copt;
	delete[] Army;

	SetSpritePtr( eSPRITE_IN_GAME );
}

void cGraphics_Amiga::video_Draw_Linear() {

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->word_42062;
	int16	ax, cx;

	di += mImage->GetWidth() * mFodder->mDrawSpritePositionY;

	ax = mFodder->mDrawSpritePositionX;
	ax += mFodder->word_40054;
	//ax >>= 2;

	di += ax;
	mFodder->word_42066 = di;

	mFodder->word_4206C >>= 3;
	mFodder->word_42074 = 40 - mFodder->word_4206C;
	mFodder->word_4206C >>= 1;
	mFodder->word_42076 = 352 - (mFodder->word_4206C * 16);

	// Height
	for (int16 dx = mFodder->word_4206E; dx > 0; --dx) {

		// Width
		for (cx = 0; cx < mFodder->word_4206C; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
}
void cGraphics_Amiga::video_Draw_Sprite() {

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->word_42062;
	int16	ax, cx;

	di += mImage->GetWidth() * mFodder->mDrawSpritePositionY;

	ax = mFodder->mDrawSpritePositionX;
	ax += mFodder->word_40054;
	di += ax;

	mFodder->word_4206C -= 1;
	mFodder->word_4206C <<= 1;

	mFodder->word_42074 = 40 - (mFodder->word_4206C);
	mFodder->word_42076 = 352 - (mFodder->word_4206C * 8);

	// Height
	for (int16 dx = mFodder->word_4206E; dx > 0; --dx) {

		// Width
		for (cx = 0; cx < mFodder->word_4206C; ++cx) {

			DrawPixels_8( si, di );

			di += 8;
			si += 1;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
}

void cGraphics_Amiga::sub_144A2() {

	uint8*	Buffer = mImage->GetSurfaceBuffer();
	uint8* 	si = (uint8*)mFodder->mMapSptPtr;

	Buffer += (16 * 352) + 16; // 0x584
	mFodder->byte_42071 = 1 << mFodder->word_40054;
	mFodder->word_42066 = Buffer;

	Buffer = mFodder->word_42066;

	for (unsigned int Y = 0; Y < 230; ++Y) {

		for (unsigned int X = 0; X < 0x30; X++) {

			if (si >= ((uint8*)mFodder->mMapSptPtr) + (0x258 * 16))
				Buffer[X] = 0;
			else
				Buffer[X] = *si++;
		}

		Buffer += 352;
	}

}

void cGraphics_Amiga::sub_145AF( int16 pData0, int16 pData8, int16 pDataC ) {

	const struct_2* str2 = &stru_44B50[pData0];

	int16 cx = str2->field_4;
	int16 dx = str2->field_6;
	mFodder->word_4206C = cx >> 3;
	mFodder->word_4206E = dx;

	uint16 ax = str2->field_2 & 0xFF;
	uint16 bx = str2->field_0 >> 3 & -2;

	ax <<= 3;
	int16 d1 = ax;

	ax <<= 2;
	ax += d1;

	mFodder->word_42062 = GetSpriteData( 2 ) + (ax + bx);

	mFodder->word_42074 = 40 - mFodder->word_4206C;
	mFodder->word_4206C >>= 1;
	mFodder->word_42076 = 0x30 - (mFodder->word_4206C * 16);

	uint16 w42066 = (0x30 * pDataC) + pData8;

	uint8* di = ((uint8*)mFodder->word_3D5B7) + w42066;
	uint8* si = mFodder->word_42062;

	// Height
	for (int16 dx = mFodder->word_4206E; dx > 0; --dx) {

		// Width
		for (cx = 0; cx < mFodder->word_4206C; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
}

void cGraphics_Amiga::DrawPixels_8( uint8* pSource, uint8* pDestination ) {
	uint8	Result = 0;
	uint16	Planes[5];
	uint8	bl = mFodder->byte_42070;

	// Load bits for all planes
	for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane)
		Planes[Plane] = *(pSource + ((mBMHD_Current->mHeight * 40) * Plane) );

	// Loop each pixel 
	for (uint8 X = 0; X < 8; ++X) {
		uint16 Bit = (0x80 >> X);
		uint8 Result = 0;

		// Value for each plane
		for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane) {
			Result |= Planes[Plane] & Bit ? (1 << Plane) : 0;
		}

		if (Result)
			pDestination[X] = bl | Result;
	}
}

void cGraphics_Amiga::DrawPixels_16( uint8* pSource, uint8* pDestination ) {
	uint8	Result = 0;
	uint16	Planes[5];
	uint8	bl = mFodder->byte_42070;

	// Load bits for all planes
	for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane)
		Planes[Plane] = readBEWord( pSource + ((mBMHD_Current->mHeight * 40) * Plane) );

	// Loop each pixel 
	for (uint8 X = 0; X < 16; ++X) {
		uint16 Bit = (0x8000 >> X);
		uint8 Result = 0;

		// Value for each plane
		for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane) {
			Result |= Planes[Plane] & Bit ? (1 << Plane) : 0;
		}

		if (Result)
			pDestination[X] = bl | Result;
	}
}