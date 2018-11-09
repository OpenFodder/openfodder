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

void cGraphics_Amiga2::Load_pStuff() {

    mImagePStuff = GetImage("PSTUFF", 0x00);
    mImagePStuff.CopyPalette(mImageFonts.mPalette, 16);

    mImageFonts = mImagePStuff;
}

tSharedBuffer cGraphics_Amiga2::GetPalette(const std::string pFilename) {
    tSharedBuffer PaletteFinal = std::make_shared<std::vector<uint8>>();

    auto Palette = g_Resource->fileGet(pFilename + ".pal");

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


void cGraphics_Amiga2::Load_And_Draw_Image(const std::string &pFilename, unsigned int pColors, unsigned int pBackColor) {
    std::string	Filename = pFilename;

    // Try it as an IFF
    sImage Decoded;

    // No, treat as a raw file
    Decoded.mData = g_Resource->fileGet(Filename);
    if (!Decoded.mData->size())
        return;

    // Calculate planes based on file size
    if (Decoded.mData->size() == 51400)
        Decoded.mPlanes = 5;
    else
        Decoded.mPlanes = 4;

    if (Filename.find('.') != std::string::npos)
        Filename.erase(Filename.find('.'));

    auto Palette = GetPalette(Filename);
    Decoded.LoadPalette_Amiga((uint8*)Palette->data(), Palette->size() / 2, 0x00);

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
