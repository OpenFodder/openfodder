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

cGraphics_Amiga2::cGraphics_Amiga2() : cGraphics_Amiga() {

}

cGraphics_Amiga2::~cGraphics_Amiga2() {

}

void cGraphics_Amiga2::Map_Load_Resources() {

    mSpriteSheet_InGame2 = GetImage(mFodder->mFilenameCopt, 0 );
    mSpriteSheet_InGame1 = GetImage(mFodder->mFilenameArmy, 0 );

    mSpriteSheet_InGame2.mDimension.mHeight = 0x151;
    mSpriteSheet_InGame1.mDimension.mHeight = 0x150;

    SetActiveSpriteSheet(eGFX_IN_GAME);
}

void cGraphics_Amiga2::Mission_Intro_Load_Resources(const eTileTypes pTileset) {

    // Briefing images
    std::string JunData1 = mTileTypes[pTileset].mName + "PLAY";
    std::string JunData2 = mTileTypes[pTileset].mName + "sky.pl8";
    std::string JunData3 = mTileTypes[pTileset].mName + "mid.pl8";
    std::string JunData4 = mTileTypes[pTileset].mName + "fgnd.pl8";
    std::string JunData5 = mTileTypes[pTileset].mName + "fgn2.pl8";
    std::string JunData6 = mTileTypes[pTileset].mName + "heli.pal";

    // Load the intro images
    mImageMissionIntro = GetImage(JunData1, 0);
    if (!mImageMissionIntro.mData->size())
        return;

    mImageMissionIntro.mPlanes = 3;
    mImageMissionIntro.mDimension.mHeight = 0x12C;

    // Load the palettes
    auto Data = g_Resource->fileGet(JunData2);
    mImageMissionIntro.LoadPalette_Amiga(Data->data(), 16, 0xA0);

    Data = g_Resource->fileGet(JunData3);
    mImageMissionIntro.LoadPalette_Amiga(Data->data(), 16, 0xB0);

    Data = g_Resource->fileGet(JunData4);
    mImageMissionIntro.LoadPalette_Amiga(Data->data(), 16, 0xC0);

    Data = g_Resource->fileGet(JunData5);
    mImageMissionIntro.LoadPalette_Amiga(Data->data(), 16, 0xD0);

    Data = g_Resource->fileGet(JunData6);
    mImageMissionIntro.LoadPalette_Amiga(Data->data(), 16, 0xE0);
}

void cGraphics_Amiga2::SetActiveSpriteSheet(eGFX_Types pSpriteType) {
    switch (pSpriteType) {
    case eGFX_IN_GAME:
        mFodder->SetActiveSpriteSheetPtr(mSpriteSheetTypes_InGame_Amiga_CF2);
        return;

    default:
        cGraphics_Amiga::SetActiveSpriteSheet(pSpriteType);
        return;
    }
}

tSharedBuffer cGraphics_Amiga2::GetPalette(const std::string pFilename) {
    tSharedBuffer PaletteFinal = std::make_shared<std::vector<uint8>>();
    std::string Filename = pFilename;

    if (Filename.find('.') == std::string::npos)
        Filename.append(".PAL");

    auto Palette = g_Resource->fileGet(Filename);

    auto a0 = Palette->data();
    for (; a0 < Palette->data() + Palette->size(); ++a0) {

        uint16 d0 = *a0++;
        uint16 d1 = *a0++;

        d0 &= 0xF0;
        d0 <<= 4;

        d1 &= 0xF0;
        d0 |= d1;

        d1 = *a0;
        d1 &= 0xF0;
        d1 >>= 4;
        d0 |= d1;

        // push as little endian
        PaletteFinal->push_back(d0 >> 8);
        PaletteFinal->push_back(d0 & 0xFFFF);
    }

    return PaletteFinal;
}

sImage cGraphics_Amiga2::GetImage(const std::string& pFilename, const size_t pPaletteIndex) {
    auto Palette = GetPalette(pFilename);

    sImage Decoded;

    // No, treat as a raw file
    Decoded.mData = g_Resource->fileGet(pFilename + ".RAW");
    if (!Decoded.mData->size())
        return Decoded;

    // Calculate planes based on file size
    if (Decoded.mData->size() == 51400)
        Decoded.mPlanes = 5;
    else {
        Decoded.mPlanes = 4;
    }

    Decoded.mDimension.mWidth = 0x140;
    Decoded.mDimension.mHeight = 0x100;

    Decoded.LoadPalette_Amiga((uint8*)Palette->data(), Palette->size() / 2, pPaletteIndex);

    return Decoded;
}

sImage cGraphics_Amiga2::Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex) {
    sImage Image;

    Image = GetImage(pFilename, 0);
    Image.CopyPalette(mPalette, pCount, pStartIndex);

    return Image;
}
void cGraphics_Amiga2::Load_pStuff() {

    mImagePStuff = GetImage("PSTUFF", 0x00);
    mImagePStuff.CopyPalette(mImageFonts.mPalette, 16);

    mImageFonts = mImagePStuff;
}

void cGraphics_Amiga2::Load_Service_Data() {

    mImageService = Decode_Image("morphbig", 64, 0, 0);
    mImageService.mDimension.mHeight = 0x150;
    mImageService.mPlanes = 5;

    mSpriteSheet_RankFont = GetImage("rankfont", 0);
}

void cGraphics_Amiga2::Load_And_Draw_Image(const std::string &pFilename, unsigned int pColors, size_t pBackColor) {
    std::string	Filename = pFilename;

    // Try it as an IFF
    sImage Decoded = GetImage(pFilename, 0);

    // All raws are 320x257
    Decoded.mDimension.mWidth = 0x140;
    Decoded.mDimension.mHeight = 0x101;
    
    // Load the palette
    Decoded.CopyPalette(mPalette, (1LL << 5));

    mBMHD_Current = Decoded.GetHeader();

    mFodder->mVideo_Draw_FrameDataPtr = Decoded.mData->data();
    mFodder->mVideo_Draw_PosX = 16;
    mFodder->mVideo_Draw_PosY = 16;
    mFodder->mVideo_Draw_Columns = Decoded.mDimension.mWidth >> 3;
    mFodder->mVideo_Draw_Rows = Decoded.mDimension.mHeight;
    mFodder->mVideo_Draw_PaletteIndex = 0x00;

    mSurface->clearBuffer(pBackColor);

    Video_Draw_16();
    mBMHD_Current = 0;
}


void cGraphics_Amiga2::Recruit_Draw_Hill() {
    //auto Grave = GetPalette("grave32");
    // mImagePStuff.LoadPalette_Amiga(Grave->data(), Grave->size() / 2);

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