/*
*  Cannon Fodder
*  ------------------------
*
*  Copyright (C) 2008-2017 Robert Crossfield
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
#include "Amiga/SpriteData_Amiga.hpp"

const sHillOverlay_Amiga mHillOverlay_Amiga[] = {
	{ 0x12, 0, 0x90, 0xA8 },			// Top Right
	{ 0x12, 1, 0xA0, 0xA8 },
	{ 0x12, 2, 0xC0, 0xA8 },			// Bottom Right

	{ 0x13, 0, 0x30, 0xC8 },			// Truck
	{ 0x13, 1, 0x40, 0xC8 },
	{ 0x13, 2, 0x50, 0xC8 },
	{ 0x13, 3, 0x60, 0xC8 },
	{ 0x13, 4, 0x70, 0xC8 },
	{ 0x13, 5, 0x80, 0xC8 },
	{ 0x13, 6, 0x90, 0xC8 },
	{ 0x13, 7, 0xA0, 0xC8 },
	{ 0x13, 8, 0xB0, 0xC8 },
	{ 0x13, 9, 0xC0, 0xC8 },
	{ 0x13, 0xA, 0xD0, 0xC8 },
	{ 0x13, 0xB, 0xE0, 0xC8 },
	{ -1 }
};

const sStruct0_Amiga stru_A918A[] = {
	{ 0, 0x00, 5, 0x90 },
	{ 0, 0x0A, 5, 0x90 },
	{ 0, 0x14, 5, 0x90 },
	{ 0, 0x1E, 5, 0x90 },
	{ 0, 0x1680, 5, 0x90 },
	{ 0, 0x168A, 5, 0x90 },
	{ 0, 0x1694, 5, 0x90 },
	{ 0, 0x1E, 5, 0x90 },
	{ 1, 0x2170, 0xE, 0x1C },
	{ 1, 0x1D10, 0xE, 0x1C },
	{ 1, 0x2B6, 8, 0x16 },
};

cGraphics_Amiga::cGraphics_Amiga() : cGraphics() {
	mBlkData = std::make_shared<std::vector<uint8>>();
}

cGraphics_Amiga::~cGraphics_Amiga() {

}

uint8* cGraphics_Amiga::GetSpriteData( uint16 pSegment ) {

	switch (pSegment) {
	case eSPRITE_IN_GAME:
		mFodder->mVideo_Draw_PaletteIndex = 0;
		mBMHD_Current = mSpriteSheet_InGame1.GetHeader();
		// We should be returning the sImage, then mBMHD can go
		return mSpriteSheet_InGame1.mData->data();

	case eSPRITE_IN_GAME2:
		mFodder->mVideo_Draw_PaletteIndex = 0;
		mBMHD_Current = mSpriteSheet_InGame2.GetHeader();
		return mSpriteSheet_InGame2.mData->data();
	
	case eSPRITE_RANKFONT:
		mFodder->mVideo_Draw_PaletteIndex = 0;
		mBMHD_Current = mSpriteSheet_RankFont.GetHeader();

		return mSpriteSheet_RankFont.mData->data();

	case eSPRITE_PSTUFF:
		if (mFodder->mVersion->mVersion == eVersion::AmigaFormat)
			mFodder->mVideo_Draw_PaletteIndex = (uint8)mCursorPalette;
		else
			mFodder->mVideo_Draw_PaletteIndex = 0xE0;

		mBMHD_Current = mImagePStuff.GetHeader();
		return mImagePStuff.mData->data();
	
	case eSPRITE_RECRUIT:
		mFodder->mVideo_Draw_PaletteIndex = 0xD0;
		mBMHD_Current = mImageHillBackground.GetHeader();
		mBMHD_Current->mHeight = 0x100;
		return mImageHillBackground.mData->data();

	case eSPRITE_HILL:
		mFodder->mVideo_Draw_PaletteIndex = 0xD0;
		mBMHD_Current = mImageHillSprites.GetHeader();
		mBMHD_Current->mHeight = 0x101;
		return mImageHillSprites.mData->data();

	case eSPRITE_FONT:
		mFodder->mVideo_Draw_PaletteIndex = 0xF0;
		mBMHD_Current = mImageFonts.GetHeader();
		return mImageFonts.mData->data();

	case eSPRITE_BRIEFING:
		mFodder->mVideo_Draw_PaletteIndex = 0xA0;
		mBMHD_Current = mImageBriefingIntro.GetHeader();
		mFodder->mVideo_Draw_Columns = 0x28;
		mFodder->mVideo_Draw_Rows = 64;
		return mImageBriefingIntro.mData->data();

	case eSPRITE_BRIEFING_AMIGA_1:
		mFodder->mVideo_Draw_PaletteIndex = 0xB0;
		mBMHD_Current = mImageBriefingIntro.GetHeader();
		mFodder->mVideo_Draw_Columns = 0x28;
		mFodder->mVideo_Draw_Rows = 74;
		return mImageBriefingIntro.mData->data() + (176 * 40);

	case eSPRITE_BRIEFING_AMIGA_2:
		mFodder->mVideo_Draw_PaletteIndex = 0xC0;
		mBMHD_Current = mImageBriefingIntro.GetHeader();
		mFodder->mVideo_Draw_Columns = 0x28;
		mFodder->mVideo_Draw_Rows = 111;
		return mImageBriefingIntro.mData->data() + (65 * 40);

	case eSPRITE_BRIEFING_AMIGA_3:
		mFodder->mVideo_Draw_PaletteIndex = 0xD0;
		mBMHD_Current = mImageBriefingIntro.GetHeader();
		mFodder->mVideo_Draw_Columns = 0x28;
		mFodder->mVideo_Draw_Rows = 48;
		return mImageBriefingIntro.mData->data() + (252 * 40);

	default:
		std::cout << "cGraphics_Amiga::GetSpriteData: Invalid ID " << pSegment << "\n";
		return 0;
	}

}

void cGraphics_Amiga::Mouse_DrawCursor() {
	const sSpriteSheet_pstuff* di = &mSpriteSheet_PStuff[mFodder->mMouseSpriteCurrent];

	mFodder->mVideo_Draw_Columns = di->mColumns >> 3;
	mFodder->mVideo_Draw_Rows = di->mRows;

	int16 ax = di->mY & 0xFF;
	int16 bx = (di->mX >> 3) & -2;

	ax <<= 3;
	int16 d1 = ax;

	ax <<= 2;
	ax += d1;

	mFodder->mVideo_Draw_FrameDataPtr = di->GetGraphicsPtr(ax + bx);
	Video_Draw_8();
}

bool cGraphics_Amiga::Sprite_OnScreen_Check() {

	return Sprite_OnScreen_Check( false );
}

bool cGraphics_Amiga::Sprite_OnScreen_Check( bool p16bit ) {
	int16 ax;

	if (g_Fodder.mVideo_Draw_PosY < 0) {
		ax = g_Fodder.mVideo_Draw_PosY + g_Fodder.mVideo_Draw_Rows;
		--ax;
		if (ax < 0)
			return false;

		ax -= 0;
		ax -= g_Fodder.mVideo_Draw_Rows;
		++ax;
		ax = -ax;
		g_Fodder.mVideo_Draw_PosY += ax;
		g_Fodder.mVideo_Draw_Rows -= ax;

		ax *= 40;

		g_Fodder.mVideo_Draw_FrameDataPtr += ax;
	}

	ax = g_Fodder.mVideo_Draw_PosY + g_Fodder.mVideo_Draw_Rows;
	--ax;

	if (ax > 256) {
		if (g_Fodder.mVideo_Draw_PosY > 256)
			return false;

		ax -= 256;
		g_Fodder.mVideo_Draw_Rows -= ax;

	}

	if (g_Fodder.mVideo_Draw_PosX < 0) {
		ax = g_Fodder.mVideo_Draw_PosX + g_Fodder.mVideo_Draw_Columns;
		--ax;
		if (ax < 0)
			return false;


		ax -= g_Fodder.mVideo_Draw_Columns;
		++ax;
		ax = -ax;
		--ax;

		// TODO: This all needs work, as it expects to move in groups of 8 pixels
		do {
			++ax;
		} while (ax & 8);

		if (p16bit)
			ax >>= 3;

		g_Fodder.mVideo_Draw_PosX += ax;
		g_Fodder.mVideo_Draw_Columns -= ax;

		if (!p16bit)
			ax >>= 1;
		//else
		//	ax >>= 2;
		g_Fodder.mVideo_Draw_FrameDataPtr += ax;
	}

	ax = g_Fodder.mVideo_Draw_PosX + g_Fodder.mVideo_Draw_Columns;
	--ax;

	if (ax > 351) {
		if (g_Fodder.mVideo_Draw_PosX > 351)
			return false;

		ax -= 351;
		--ax;

		do {
			++ax;
		} while (ax & 8);


		g_Fodder.mVideo_Draw_Columns -= ax;
	}

	if (g_Fodder.mVideo_Draw_Columns <= 0)
		return false;

	if (g_Fodder.mVideo_Draw_Rows <= 0)
		return false;

	return true;
}

void cGraphics_Amiga::Briefing_DrawHelicopter( uint16 pID ) {
	const sSpriteSheet_pstuff* di = &mSpriteSheet_PStuff[pID];

	mFodder->mVideo_Draw_Columns = di->mColumns;
	mFodder->mVideo_Draw_Rows = di->mRows;

	int16 ax = di->mY & 0xFF;
	int16 bx = (di->mX >> 3) & -2;

	ax <<= 3;
	int16 d1 = ax;

	ax <<= 2;
	ax += d1;

	mFodder->mVideo_Draw_FrameDataPtr = di->GetGraphicsPtr(ax + bx);

	if (pID >= 205 && pID <= 207) {
		mFodder->mVideo_Draw_Rows++;
	}

	if (Sprite_OnScreen_Check( true )) {
		mFodder->mVideo_Draw_Columns >>= 3;
		Video_Draw_16();
	}
}

void cGraphics_Amiga::Load_pStuff() {

	mImagePStuff = DecodeIFF("pstuff.lbm");
	mImagePStuff.CopyPalette(mImageFonts.mPalette, 16);
	mImageFonts = mImagePStuff;
}

void cGraphics_Amiga::Load_Hill_Data() {

	mImageHillBackground = Decode_Image("hills.lbm", 64);
	mImageHillBackground.mData->resize(mImageHillBackground.GetHeader()->ScreenSize() *(mImageHillBackground.GetHeader()->mPlanes + 30));

	// We increase these buffers because the engine still writes to it
	mImageHillSprites = Decode_Image("hills.lbm", 64);
	mImageHillSprites.mData->resize(mImageHillBackground.GetHeader()->ScreenSize() * (mImageHillBackground.GetHeader()->mPlanes + 30));

	// A5A7E
	uint8* a0 = mImageHillSprites.mData->data() + (29 * 40);
	uint8* a1 = mImageHillSprites.mData->data() + 0x390EE + 0x3E8;

	for (int16 d1 = 0xB7; d1 >= 0; --d1) {
		uint8* a2 = a0 + 6;
		uint8* a3 = a1 + 6;

		for (int16 d0 = 0x10; d0 >= 0; --d0) {
			writeBEWord(a3, readBEWord(a2));
			writeBEWord(a3 + 0x2828, readBEWord(a2 + 0x2800));
			writeBEWord(a3 + 0x5050, readBEWord(a2 + 0x5000));
			writeBEWord(a3 + 0x7878, readBEWord(a2 + 0x7800));

			// Clear memory which has hill sprites drawn into it
			writeBEWord(a2, 0);
			writeBEWord(a2 + 0x2800, 0);
			writeBEWord(a2 + 0x5000, 0);
			writeBEWord(a2 + 0x7800, 0);

			a2 += 2;
			a3 += 2;
		}

		a0 += 0x28;
		a1 += 0x28;
	}
}


void cGraphics_Amiga::Load_Sprite_Font() {

	mImageFonts.mData = g_Resource.fileGet( "FONT.RAW" );
	
	// Load the palette
	mImageFonts.LoadPalette_Amiga(mImageFonts.mData->data(), 16);

	// Remove the palette
	mImageFonts.mData->erase(mImageFonts.mData->begin(), mImageFonts.mData->begin() + 0x20);


	// 
	mImageFonts.mDimension.mWidth = 0x140;
	mImageFonts.mDimension.mHeight = 0x100;
	mImageFonts.mPlanes = 4;

	SetActiveSpriteSheet( eSPRITE_FONT );
}

sImage cGraphics_Amiga::Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex) {
	sImage Image;

	Image = DecodeIFF(pFilename);
	Image.CopyPalette(mPalette, pCount, pStartIndex);

	return Image;
}

void cGraphics_Amiga::Load_Service_Data() {

	mImageService = Decode_Image("morphbig.lbm", 64);

	mSpriteSheet_RankFont = DecodeIFF("rankfont.lbm");
}

void cGraphics_Amiga::Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors ) {
	std::string	Filename = pFilename;

	if (Filename.find( '.' ) == std::string::npos)
		Filename.append( ".raw" );

	// Try it as an IFF
	auto Decoded = DecodeIFF( Filename );

	// Did we decode an IFF?
	if (!Decoded.mData->size()) {

		// No, treat as a raw file
		Decoded.mData = g_Resource.fileGet( Filename );

		// Calculate planes based on file size
		if (Decoded.mData->size() == 51464)
			Decoded.mPlanes = 5;
		else
			Decoded.mPlanes = 4;

		// Not an iff, probably RAW which has the palette at the start
		Decoded.LoadPalette_Amiga(Decoded.mData->data(), (1 << Decoded.mPlanes));

		// Remove the palette
		Decoded.mData->erase(Decoded.mData->begin(), Decoded.mData->begin() + (2 << Decoded.mPlanes));

		// All raws are 320x257
		Decoded.mDimension.mWidth = 0x140;
		Decoded.mDimension.mHeight = 0x101;
	}

	// Load the palette
	Decoded.CopyPalette(mPalette, (1 << Decoded.mPlanes));

	mBMHD_Current = Decoded.GetHeader();

	g_Fodder.mVideo_Draw_FrameDataPtr = Decoded.mData->data();
	g_Fodder.mVideo_Draw_PosX = 16;
	g_Fodder.mVideo_Draw_PosY = 16;
	g_Fodder.mVideo_Draw_Columns = Decoded.mDimension.mWidth >> 3;
	g_Fodder.mVideo_Draw_Rows = Decoded.mDimension.mHeight;
	g_Fodder.mVideo_Draw_PaletteIndex = 0;

	mImage->clearBuffer();

	Video_Draw_16();
	mBMHD_Current = 0;
}

void cGraphics_Amiga::PaletteLoad( const uint8  *pBuffer, uint32 pColorID, uint32 pColors ) {

}

void cGraphics_Amiga::SetCursorPalette( uint16 pIndex ) {
	mCursorPalette = pIndex;
}

void cGraphics_Amiga::SetActiveSpriteSheet( eSpriteType pSpriteType ) {

	switch (pSpriteType) {
	case eSPRITE_IN_GAME:
		mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_InGame_Amiga );
		return;

	case eSPRITE_FONT:
		mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Font_Amiga );
		return;

	case eSPRITE_RECRUIT:
		mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Recruit_Amiga );
		return;

	case eSPRITE_HILL:
		mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Hill_Amiga );
		return;

	case eSPRITE_BRIEFING:
		mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Briefing_Amiga );
		return;

	case eSPRITE_SERVICE:
		mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Service_Amiga );
		return;
	}
}

void cGraphics_Amiga::Tile_Prepare_Gfx() {

	mBlkData->resize( mFodder->mTile_BaseBlk->size() + mFodder->mTile_SubBlk->size() );

	// Load the palette for the graphic tile set
	auto Palette = g_Resource.fileGet( mFodder->mFilenameBasePal );
	DecodePalette(Palette->data(), 0, Palette->size() / 2);

	memcpy( mBlkData->data(), mFodder->mTile_BaseBlk->data(), mFodder->mTile_BaseBlk->size() );
	memcpy( mBlkData->data() + mFodder->mTile_BaseBlk->size(), mFodder->mTile_SubBlk->data(), mFodder->mTile_SubBlk->size() );
}

void cGraphics_Amiga::PaletteSetOverview() {

	mFodder->mSurfaceMapOverview->paletteSet(mPalette, 0, 64);
	mFodder->mSurfaceMapOverview->paletteSet(mImagePStuff.mPalette, 0xE0, 16 );
	mFodder->mSurfaceMapOverview->paletteSet(mImageHillBackground.mPalette, 0xD0, 16);
	mFodder->mSurfaceMapOverview->paletteSet(mImageFonts.mPalette, 0xF0, 16 );

	mFodder->mSurfaceMapOverview->surfaceSetToPaletteNew();
}

void cGraphics_Amiga::PaletteBriefingSet() {

	mImage->paletteSet(mImageBriefingIntro.mPalette, 0, 0xFF );
	mImage->paletteSet(mImageFonts.mPalette, 0xF0, 16);
}

void cGraphics_Amiga::PaletteSet(cSurface *pTarget) {
	if (!pTarget)
		pTarget = mImage;

	mImage->paletteSet(mPalette, 0, 64);
	mImage->paletteSet(mImagePStuff.mPalette, 0xE0, 16);
	mImage->paletteSet(mImageHillBackground.mPalette, 0xB0, 16);		// 0xB0 is used by save/load screen
	mImage->paletteSet(mImageHillBackground.mPalette, 0xD0, 16);
	mImage->paletteSet(mImageFonts.mPalette, 0xF0, 16);
}

sImage cGraphics_Amiga::DecodeIFF( const std::string& pFilename ) {
	sImage			Result;

	auto			File = g_Resource.fileGet( pFilename );
	auto			DataPtr = File->data();

	if (!DataPtr || readBEDWord( DataPtr ) != 'FORM')
		return Result;

	DataPtr += 4;
	size_t FileSize = readBEDWord( DataPtr );
	DataPtr += 4;

	if (readBEDWord( DataPtr ) != 'ILBM')
		return Result;

	DataPtr += 4;
	FileSize -= 4;

	uint32 Header = 0;
	uint32 Size = 0;

	while (FileSize > 8) {
		Header = readBEDWord( DataPtr );
		DataPtr += 4; FileSize -= 4;
		Size = readBEDWord( DataPtr );
		DataPtr += 4; FileSize -= 4;

		switch (Header) {
		case 'BMHD':
			Result.mDimension.mWidth = readBEWord( DataPtr ); DataPtr += 2;
			Result.mDimension.mHeight = readBEWord( DataPtr ); DataPtr += 2;
			DataPtr += 2; DataPtr += 2;	// X, Y
			Result.mPlanes = *DataPtr++;
			DataPtr++;	// Mask
			DataPtr++;	// Compression
			DataPtr += 2; ++DataPtr; ++DataPtr; ++DataPtr;
			DataPtr += 2; DataPtr += 2;
			FileSize -= Size;
			break;

		case 'BODY': {
			// + 2 is because the amiga does writing after the end of the image
			Result.mData->resize( Result.mDimension.WidthByHeight() * (Result.mPlanes + 2) );

			int16 Width = Result.mDimension.mWidth + 0x0F;
			Width >>= 4;
			Width <<= 1;

			int16 Height = Result.mDimension.mHeight - 1;

			uint8* pDataDest = Result.mData->data();

			for (int16 Y = Height; Y >= 0; --Y) {
				uint8* DataDest = pDataDest;

				for (int8 Plane = 0; Plane < Result.mPlanes; ++Plane) {

					for (int16 X = Width; X > 0;) {
						--FileSize;
						int8 d0 = *DataPtr++;

						if (d0 < 0) {
							if (d0 == -128)
								continue;

							int8 d1 = -d0;

							--FileSize;
							d0 = *DataPtr++;

							do {
								*DataDest++ = d0;
								--X;
							} while (d1-- > 0);

							continue;

						}
						else {

							do {
								*DataDest++ = *DataPtr++;
								--X;
								--FileSize;
							} while (d0-- > 0);

						}
					}

					// Move the destination back to start of row
					DataDest -= Width;

					// Move it to the next plane
					DataDest += (Width * Result.mDimension.mHeight);
				}

				// Next Row
				pDataDest += Width;
			}

			break;
		}

		case 'CMAP':
			for (uint16 i = 0; i < Size / 3; ++i) {
				int16 d0 = (int16)*DataPtr++;
				int16 Final = 0;

				d0 >>= 4;
				d0 <<= 8;
				Final += d0;

				d0 = *DataPtr++;
				d0 >>= 4;
				d0 = (int16)d0;
				d0 <<= 4;
				Final += d0;

				d0 = *DataPtr++;
				d0 >>= 4;
				d0 = (int16)d0;
				Final += d0;

				Result.mPalette[i].mRed = ((Final >> 8) & 0xF) << 2;
				Result.mPalette[i].mGreen = ((Final >> 4) & 0xF) << 2;
				Result.mPalette[i].mBlue = ((Final >> 0) & 0xF) << 2;

				FileSize -= 3;

			}
			break;

		default:
			if (Size & 1)
				++Size;

			DataPtr += Size;
			FileSize -= Size;
			break;
		}
	}

	return Result;
}

void cGraphics_Amiga::DecodePalette(const uint8* pBuffer, size_t pColorID, const size_t pColors) {
	size_t ColorID = pColorID;

	for (; pColorID < ColorID + pColors; pColorID++) {

		// Get the next color codes
		int16 color = readBEWord(pBuffer);
		pBuffer += 2;

		// Extract each color from the word
		//  X X X X   R3 R2 R1 R0     G3 G2 G1 G0   B3 B2 B1 B0
		
		mPalette[pColorID].mRed = ((color >> 8) & 0xF) << 2;
		mPalette[pColorID].mGreen = ((color >> 4) & 0xF) << 2;
		mPalette[pColorID].mBlue = ((color >> 0) & 0xF) << 2;
	}
}

void cGraphics_Amiga::Map_Tile_Draw(cSurface *pTarget, uint16 pTile, uint16 pX, uint16 pY, uint16 pOffset) {
	uint8* Target = pTarget->GetSurfaceBuffer();

	pX *= (16 + pOffset);

	Target += (pY * (16 + pOffset)) * pTarget->GetWidth();
	Target += pX;

	uint8* TilePtr = mBlkData->data() + (pTile << 7);

	uint8* TargetRow = Target;

	// Each bitfield
	for (uint16 BitField = 0; BitField < 4; ++BitField) {

		// Each Tile Row
		for (uint16 i = 0; i < 16; ++i) {

			uint16 RowData = readBEWord(TilePtr);
			TilePtr += 2;

			// Each pixel of a Tile Row
			for (uint16 x = 0; x < 16; ++x) {
				uint8 Bit = (RowData & 0x8000) ? 1 : 0;
				RowData <<= 1;

				if (Bit)
					*(Target + x) |= (Bit << BitField);

			}

			Target += pTarget->GetWidth();
		}

		// Next Bitfield
		Target = TargetRow;
	}
}

void cGraphics_Amiga::MapTiles_Draw() {
	mImage->clearBuffer();
	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = mFodder->mMap->data() + mFodder->mMapTilePtr;

	// Y
	for (uint16 cx = 0; cx < 0x10; ++cx) {

		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = mFodder->mMapTile_RowOffset;
		else
			StartY = 0;

		if (CurrentMapPtr >= mFodder->mMap->data()) {

			// X
			for (uint16 cx2 = 0; cx2 < 0x16; ++cx2) {
				uint8* TargetTmp = TargetRow;

				uint16 Tile = readLEWord( MapPtr ) & 0x1FF;

				if (Tile > 0x1C0)
					Tile = 0;

				Tile <<= 7;
				uint8* TilePtr = mBlkData->data() + Tile;
				uint16 StartX = 0;

				TilePtr += StartY * 2;

				if (cx2 == 0)
					StartX = mFodder->mMapTile_ColumnOffset;
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
		}

		Target += (mImage->GetWidth() * (16 - StartY));
		CurrentMapPtr += (mFodder->mMapWidth << 1);
	}

	mImage->Save();
}

void cGraphics_Amiga::Hill_Prepare_Overlays() {
	const sHillOverlay_Amiga* a3 = mHillOverlay_Amiga;

	for (; a3->mSpriteType != -1; ++a3) {

		auto SpriteSheet = mFodder->Sprite_Get_Sheet(a3->mSpriteType, a3->mFrame);

		int d0 = SpriteSheet->mColCount - 1;
		int d1 = SpriteSheet->mRowCount - 1;

		uint8* DestBase = mImageHillSprites.mData->data();

		int16 d2 = a3->mX;
		d2 >>= 3;
		d2 &= 0xFFE;

		DestBase += d2;
		d2 = a3->mY;

		int16 d1_s = d1;
		d2 <<= 3;
		d1 = d2;
		d2 <<= 2;
		d2 += d1;
		d1 = d1_s;

		DestBase += d2;
		uint8* Source = SpriteSheet->GetGraphicsPtr();
		uint8* Dest = DestBase;

		do {

			for (d2 = d1; d2 >= 0; --d2) {
				
				// Draw the overlay
				writeBEWord(Dest, readBEWord(Source) );
				writeBEWord(Dest + 0x2828, readBEWord(Source + 0x2800 ) );
				writeBEWord(Dest + 0x5050, readBEWord(Source + 0x5000 ) );
				writeBEWord(Dest + 0x7878, readBEWord(Source + 0x7800 ) );
				
				Source += 0x28;
				Dest += 0x28;
			}

			Source = SpriteSheet->GetGraphicsPtr() + 2;
			Dest = DestBase + 2;

		} while (--d0 >= 0);
	}
}

void cGraphics_Amiga::MapOverview_Render_Tiles( uint16 pTile, uint16 pDestX, uint16 pDestY ) {
	uint8* Target = mFodder->mSurfaceMapOverview->GetSurfaceBuffer();

	pDestX *= 16;

	Target += (pDestY * 16) * mFodder->mSurfaceMapOverview->GetWidth();
	Target += pDestX;

	pTile <<= 7;
	uint8* TilePtr = mBlkData->data() + pTile;

	// Each bitfield
	for (uint16 BitField = 0; BitField < 4; ++BitField) {
		uint8* TargetTmp = Target;

		// Each Tile Row
		for (uint16 i = 0; i < 16; ++i) {

			uint16 RowData = readBEWord( TilePtr );
			TilePtr += 2;

			// Each pixel of a Tile Row
			for (uint16 x = 0; x < 16; ++x) {
				uint8 Bit = (RowData & 0x8000) ? 1 : 0;
				RowData <<= 1;

				if (Bit)
					*(TargetTmp + x) |= (Bit << BitField);

			}

			TargetTmp += mFodder->mSurfaceMapOverview->GetWidth();
		}

		// Next Bitfield
		TargetTmp = Target;
	}
}

void cGraphics_Amiga::Map_Load_Resources() {
	mFodder->mFilenameCopt = mFodder->Filename_CreateFromBase( mFodder->mFilenameCopt, "lbm" );
	mFodder->mFilenameArmy = mFodder->Filename_CreateFromBase( mFodder->mFilenameArmy, "lbm" );

	mSpriteSheet_InGame2 = DecodeIFF( mFodder->mFilenameCopt );
	mSpriteSheet_InGame1 = DecodeIFF( mFodder->mFilenameArmy );

	SetActiveSpriteSheet( eSPRITE_IN_GAME );
}

void cGraphics_Amiga::Video_Draw_8(cSurface *pTarget) {
	if (!pTarget)
		pTarget = mImage;

	uint8*	di = pTarget->GetSurfaceBuffer();
	uint8* 	si = mFodder->mVideo_Draw_FrameDataPtr;

	di += pTarget->GetWidth() * mFodder->mVideo_Draw_PosY;
	di += mFodder->mVideo_Draw_PosX;

	mFodder->mVideo_Draw_Columns -= 1;
	mFodder->mVideo_Draw_Columns <<= 1;

	mFodder->mDraw_Source_SkipPixelsPerRow = (mBMHD_Current->mWidth >> 3) - (mFodder->mVideo_Draw_Columns);
	mFodder->mDraw_Dest_SkipPixelsPerRow = pTarget->GetWidth() - (mFodder->mVideo_Draw_Columns * 8);

	// Height
	for (int16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx) {

		// Width
		for (int16 cx = 0; cx < mFodder->mVideo_Draw_Columns; ++cx) {

			DrawPixels_8(si, di);

			di += 8;
			si += 1;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_Amiga::Video_Draw_16() {

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->mVideo_Draw_FrameDataPtr;

	di += mImage->GetWidth() * mFodder->mVideo_Draw_PosY;
	di += mFodder->mVideo_Draw_PosX;

	mFodder->word_42066 = di;

	mFodder->mDraw_Source_SkipPixelsPerRow = (mBMHD_Current->mWidth >> 3) - mFodder->mVideo_Draw_Columns;
	mFodder->mVideo_Draw_Columns >>= 1;
	mFodder->mDraw_Dest_SkipPixelsPerRow = mImage->GetWidth() - (mFodder->mVideo_Draw_Columns * 16);

	// Height
	for (int16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx) {

		// Width
		for (int16 cx = 0; cx < mFodder->mVideo_Draw_Columns; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_Amiga::Sidebar_Copy_To_Surface( int16 pStartY ) {

	uint8*	Buffer = mImage->GetSurfaceBuffer();
	uint8* 	si = (uint8*)mFodder->mSidebar_Screen_Buffer;

	// Start 16 rows down
	Buffer += (16 * mImage->GetWidth()) + 16;

	// Start further in?
	if (pStartY) {
		Buffer += (mImage->GetWidth() * pStartY);
		si += (0x30 * pStartY);
	}

	// Entire Height of Sidebar
	for (unsigned int Y = 0; Y < 250; ++Y) {

		// Width of Sidebar
		for (unsigned int X = 0; X < 0x30; X++) {

			// Clear at bottom
			if (si >= ((uint8*)mFodder->mSidebar_Screen_Buffer) + (0x300 * 16))
				Buffer[X] = 0;
			else
				Buffer[X] = *si++;
		}

		// Next Row
		Buffer += mImage->GetWidth();
	}

}

void cGraphics_Amiga::Sidebar_Copy_Sprite_To_ScreenBufPtr( int16 pSpriteType, int16 pX, int16 pY ) {
	const sSpriteSheet_pstuff* str2 = &mSpriteSheet_PStuff[pSpriteType];

	mFodder->mVideo_Draw_Columns = str2->mColumns >> 3;
	mFodder->mVideo_Draw_Rows = str2->mRows;

	uint16 ax = str2->mY & 0xFF;
	uint16 bx = str2->mX >> 3 & -2;

	ax <<= 3;
	int16 d1 = ax;

	ax <<= 2;
	ax += d1;

	mFodder->mVideo_Draw_FrameDataPtr = str2->GetGraphicsPtr(ax + bx);

	mFodder->mDraw_Source_SkipPixelsPerRow = 40 - mFodder->mVideo_Draw_Columns;
	mFodder->mDraw_Dest_SkipPixelsPerRow = 0x30 - (mFodder->mVideo_Draw_Columns * 8);

	uint8* di = ((uint8*)mFodder->mSidebar_Screen_BufferPtr) + (0x30 * pY) + pX;
	uint8* si = mFodder->mVideo_Draw_FrameDataPtr;

	// Height
	for (int16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx) {

		// Width
		for (int16 cx = 0; cx < mFodder->mVideo_Draw_Columns / 2; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_Amiga::Sidebar_Copy_ScreenBuffer( uint16 pData0, int16 pData4, int16 pCopyToScreen, uint32*& pBuffer) {
	pData0 += 8;
	pData4 *= 4;
	uint8* SptPtr = (uint8*)mFodder->mSidebar_Screen_Buffer;
	uint32* BuffPtr = (uint32*)(SptPtr + (0x30 * pData0));

	if (pCopyToScreen == 0) {
		for (int16 cx = pData4; cx > 0; --cx) {
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;

			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;

			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
		}
	}
	else {
		for (int16 cx = pData4; cx > 0; --cx) {
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;

			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;

			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
		}
	}
}

void cGraphics_Amiga::Recruit_Draw_Hill() {
	auto Grave = g_Resource.fileGet( "grave32.pal");

	mImagePStuff.LoadPalette_Amiga(Grave->data(), Grave->size() / 2);
	
	{
		mFodder->mVideo_Draw_FrameDataPtr = mImageHillBackground.mData->data() + (29 * 40) + 6;

		mBMHD_Current = mImageHillBackground.GetHeader();
		mFodder->mVideo_Draw_PaletteIndex = 0xD0;

		mFodder->mVideo_Draw_PosX = 0x40;
		mFodder->mVideo_Draw_PosY = 0x28;
		mFodder->mVideo_Draw_Columns = 0x110 >> 3;		// W
		mFodder->mVideo_Draw_Rows = 0xB8;				// H
		mFodder->mVideo_Draw_ColumnsMax = 0x140;

		Video_Draw_16();
	}

	Recruit_Draw_HomeAway();
}

void cGraphics_Amiga::Recruit_Draw_HomeAway() {
	const char* strHomeAndAway = "HOME                AWAY";

	mFodder->GUI_Draw_Frame_8( 0x11, 0, 0xfe, 0 );
	int16 Data4 = mFodder->mMission_Save_Availability[(mFodder->mMissionNumber - 1)];

	// Draw Icon
	mFodder->GUI_Draw_Frame_8( 0x16, 0x0, 0x0, 0 );
	// Save Icon
	mFodder->GUI_Draw_Frame_8( 0x17, Data4, 0x130, 0 );

	mFodder->GUI_Draw_Frame_8( 0xE, 0, 0x9B, 0x0A );

	mFodder->String_CalculateWidth( 320, mFont_Recruit_Width, strHomeAndAway );
	mFodder->String_Print( mFont_Recruit_Width, 0x0D, mFodder->mGUI_Temp_X, 0x0A, strHomeAndAway );

	std::string Home = tool_StripLeadingZero( tool_NumToString( mFodder->mTroops_Home ) );
	mFodder->Recruit_Draw_String( 0x0D, 0x9A - (Home.length() * 0x0C), 0x0A, Home );

	std::string Away = tool_StripLeadingZero( tool_NumToString( mFodder->mTroops_Away ) );
	mFodder->Recruit_Draw_String( 0x0D, 0xAA, 0x0A, Away );
}

void cGraphics_Amiga::Service_Draw( int16 pSpriteID, int16 pX, int16 pY ) {

	uint8*	di = mImage->GetSurfaceBuffer() + 16;

	pY += 16;
	di += mImage->GetWidth() * pY;
	di += pX;

	uint8* si = 0;

	if (stru_A918A[pSpriteID].mSpriteSheet == 0) {
		si = mImageService.mData->data();
		mBMHD_Current = mImageService.GetHeader();
	}
	else {
		si = mSpriteSheet_RankFont.mData->data();
		mBMHD_Current = mSpriteSheet_RankFont.GetHeader();
	}

	int16 d0 = stru_A918A[pSpriteID].mWidth << 1;
	mFodder->mDraw_Source_SkipPixelsPerRow = 40 - d0;
	mFodder->mDraw_Dest_SkipPixelsPerRow = mImage->GetWidth() - (d0 * 8);
	mFodder->mVideo_Draw_PaletteIndex = 0;

	si += stru_A918A[pSpriteID].mOffset;

	// Height
	for (int16 dx = stru_A918A[pSpriteID].mHeight; dx > 0; --dx) {

		// Width
		for (int16 cx = 0; cx < stru_A918A[pSpriteID].mWidth; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_Amiga::Briefing_Load_Resources() {

	// Load the current map & Set TileType
	mFodder->mMap = g_Resource.fileGet(mFodder->map_Filename_MapGet());
	mFodder->Map_SetTileType();

	// Briefing images
	std::string JunData1 = mTileTypes[mFodder->mMap_TileSet].mName + "play.lbm";
	std::string JunData2 = mTileTypes[mFodder->mMap_TileSet].mName + "sky.pl8";
	std::string JunData3 = mTileTypes[mFodder->mMap_TileSet].mName + "mid.pl8";
	std::string JunData4 = mTileTypes[mFodder->mMap_TileSet].mName + "fgnd.pl8";
	std::string JunData5 = mTileTypes[mFodder->mMap_TileSet].mName + "fgn2.pl8";
	std::string JunData6 = mTileTypes[mFodder->mMap_TileSet].mName + "heli.pal";

	// Load the intro images
	mImageBriefingIntro = DecodeIFF(JunData1);

	// Load the palettes
	auto Data = g_Resource.fileGet(JunData2);
	mImageBriefingIntro.LoadPalette_Amiga(Data->data(), 16, 0xA0);

	Data = g_Resource.fileGet(JunData3);
	mImageBriefingIntro.LoadPalette_Amiga(Data->data(), 16, 0xB0);

	Data = g_Resource.fileGet(JunData4);
	mImageBriefingIntro.LoadPalette_Amiga(Data->data(), 16, 0xC0);

	Data = g_Resource.fileGet(JunData5);
	mImageBriefingIntro.LoadPalette_Amiga(Data->data(), 16, 0xD0);

	Data = g_Resource.fileGet(JunData6);
	mImageBriefingIntro.LoadPalette_Amiga(Data->data(), 16, 0xE0);
}

void cGraphics_Amiga::Recruit_Sprite_Draw( int16 pRows, int16 pColumns, int16 pD2, int16 pD4, int16 pD5, int16 pD3, uint8* pSource ) {
	int32 d0, d1, d2, d3, d4, d5, d6;

	d0 = pRows;
	d1 = pColumns;
	d2 = pD2;
	d3 = pD3;
	d4 = pD4;
	d5 = pD5;

	uint8* Dest = mImageHillSprites.mData->data();

	int32 word_8271A = d0;
	int32 word_8271C = d1;
	int32 word_8271E = d2;
	if (d2 <= 0)
		return;

	int32 word_82720 = d3;
	if (d3 <= 0)
		return;

	d6 = word_8271E;
	d6 >>= 1;
	d4 -= d6;

	d6 = word_82720;
	d6 >>= 1;
	d5 -= d6;


	int32 D1_Saved = d1;
	d5 <<= 3;
	d1 = d5;
	d5 <<= 2;
	d5 += d1;

	d1 = D1_Saved;
	Dest += d5;
	d6 = d4;
	d6 >>= 3;

	Dest += d6;
	d4 &= 7;
	d6 = 7;
	d6 -= d4;

	int32 word_82726 = d6;
	d4 = d2;
	d4 >>= 3;
	d0 &= 0xFFF;
	d1 &= 0xFFF;

	d0 /= d2;
	d1 /= d3;

	d0 = 0;

	d0 = d1 = d2 = d3 = 0;

	d0 = word_8271A;
	d1 = word_8271C;
	int32 d0_u = (d0 % word_8271E) << 16;
	d0 /= word_8271E;
	d0 |= d0_u;

	int32 word_8159E = d0 & 0xFFFF;
	d0 &= 0xFFFF0000;
	d0 /= word_8271E;
	d0 = (d0_u >> 16) | (d0 << 16);

	d0 = (d0 & 0xFFFF0000) | word_8159E;
	d0 = (d0 >> 16) | (d0 << 16);

	int32 d1_u = (d1 % word_82720) << 16;
	d1 /= word_82720;
	d1 |= d1_u;

	word_8159E = d1 & 0xFFFF;
	d1 &= 0xFFFF0000;
	d1 /= word_82720;
	d1 = (d1_u >> 16) | (d1 << 16);

	d1 = (d1 & 0xFFFF0000) | word_8159E;
	d1 = (d1 >> 16) | (d1 << 16);

	//loc_A0850
	do {
		uint8* a3 = pSource;
		uint8* a6 = Dest;

		d2 = 0;
		d4 = 7;
		d5 = word_8271E;
		d6 = word_82726;

	loc_A0864:;
		do {
			--d5;
			if (d5 < 0)
				goto loc_A08A8;

			if (*a3 & (1 << d4))
				*a6 |= (1 << d6);

			if (*(a3 + 0x2800) & (1 << d4))
				*(a6 + 0x2828) |= (1 << d6);

			if (*(a3 + 0x5000) & (1 << d4))
				*(a6 + 0x5050) |= (1 << d6);

			if (*(a3 + 0x7800) & (1 << d4))
				*(a6 + 0x7878) |= (1 << d6);

			d2 += d0;
			int32 d7 = d2;
			d7 = (d7 >> 16) | (d7 << 16);

			d4 = 7;
			d4 -= (d7 & 0xFFFF);
			d7 = (d7 & 0xFFFF0000) | ((d7 & 0xFFFF) >> 3);

			a3 = pSource + (d7 & 0xffff);
		} while (--d6 >= 0);

		++a6;
		d6 = 7;
		goto loc_A0864;

	loc_A08A8:;

		Dest += 0x28;
		d3 += d1;
		d3 = (d3 >> 16) | (d3 << 16);

		D1_Saved = d1;

		d3 = (d3 & 0xFFFF0000) | ((d3 & 0xFFFF) << 3);

		d1 = (d1 & 0xFFFF0000) | (d3 & 0xFFFF);
		d3 = (d3 & 0xFFFF0000) | ((d3 & 0xFFFF) << 2);
		d3 += (d1 & 0xFFFF);
		d1 = D1_Saved;
		pSource += (d3 & 0xFFFF);
		d3 = d3 >> 16;

	} while (--word_82720 > 0);

}

void cGraphics_Amiga::DrawPixels_8( uint8* pSource, uint8* pDestination ) {
	uint8	Planes[5];

	// Load bits for all planes
	for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane)
		Planes[Plane] = *(pSource + ((mBMHD_Current->mHeight * 40) * Plane));

	// Loop each pixel 
	for (uint8 X = 0; X < 8; ++X) {
		uint16 Bit = (0x80 >> X);
		uint8 Result = 0;

		// Value for each plane
		for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane) {
			Result |= Planes[Plane] & Bit ? (1 << Plane) : 0;
		}

		if (Result)
			pDestination[X] = mFodder->mVideo_Draw_PaletteIndex | Result;
	}
}

void cGraphics_Amiga::DrawPixels_16( uint8* pSource, uint8* pDestination ) {
	uint16	Planes[5];

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
			pDestination[X] = mFodder->mVideo_Draw_PaletteIndex | Result;
	}
}

void cGraphics_Amiga::Briefing_Intro() {

		mImage->clearBuffer();

		Briefing_Load_Resources();
		SetActiveSpriteSheet(eSPRITE_BRIEFING);

		g_Fodder.mSound->Music_Play(0x07);
		g_Fodder.Briefing_Helicopter_Start();

		if (g_Fodder.mVersion->mPlatform == ePlatform::Amiga) {
			//TODO
			g_Fodder.mVideo_Draw_PosX = 16;

			g_Fodder.mVideo_Draw_FrameDataPtr = GetSpriteData(eSPRITE_BRIEFING);
			g_Fodder.mVideo_Draw_PosY = 40;
			Video_Draw_16();

			g_Fodder.mVideo_Draw_FrameDataPtr = GetSpriteData(eSPRITE_BRIEFING_AMIGA_1);
			g_Fodder.mVideo_Draw_PosY = 60;
			Video_Draw_16();

			g_Fodder.mVideo_Draw_FrameDataPtr = GetSpriteData(eSPRITE_BRIEFING_AMIGA_2);
			g_Fodder.mVideo_Draw_PosY = 100;
			Video_Draw_16();

			g_Fodder.mVideo_Draw_FrameDataPtr = GetSpriteData(eSPRITE_BRIEFING_AMIGA_3);
			g_Fodder.mVideo_Draw_PosY = 163;
			Video_Draw_16();

			PaletteBriefingSet();
			g_Fodder.mImage->palette_FadeTowardNew();
			g_Fodder.mImageFaded = -1;

			g_Fodder.Briefing_Draw_Mission_Name();
			mImage->Save();

			int16 word_42875 = 0;

			do {
				if (g_Fodder.mBriefing_Helicopter_Moving == -1)
					g_Fodder.Briefing_Update_Helicopter();

				g_Fodder.Mouse_Inputs_Get();

				// Front
				g_Fodder.mVideo_Draw_PosX = g_Fodder.mHelicopterPosX >> 16;
				g_Fodder.mVideo_Draw_PosY = g_Fodder.mHelicopterPosY >> 16;
				Briefing_DrawHelicopter(203);

				// Tail
				g_Fodder.mVideo_Draw_PosX = (g_Fodder.mHelicopterPosX >> 16) + 48;
				g_Fodder.mVideo_Draw_PosY = (g_Fodder.mHelicopterPosY >> 16);
				Briefing_DrawHelicopter(204);

				int16 Blade = 205 + word_42875;

				++word_42875;
				if (word_42875 >= 3)
					word_42875 = 0;

				// Blade
				g_Fodder.mVideo_Draw_PosX = (g_Fodder.mHelicopterPosX >> 16);
				g_Fodder.mVideo_Draw_PosY = (g_Fodder.mHelicopterPosY >> 16);
				Briefing_DrawHelicopter(Blade);

				if (g_Fodder.mImageFaded)
					g_Fodder.mImageFaded = mImage->palette_FadeTowardNew();

				g_Fodder.eventProcess();
				g_Fodder.Video_Sleep();
				g_Window.RenderAt(mImage, cPosition());
				g_Window.FrameEnd();

				g_Fodder.Mouse_GetData();

				if (g_Fodder.mMouse_Exit_Loop) {
					g_Fodder.word_428D8 = 0;
					mImage->paletteNew_SetToBlack();
					g_Fodder.mImageFaded = -1;
					g_Fodder.mMouse_Exit_Loop = 0;
				}
				mImage->Restore();
			} while (g_Fodder.word_428D8 || g_Fodder.mImageFaded != 0);

			g_Fodder.mMouse_Exit_Loop = 0;
		}
}
