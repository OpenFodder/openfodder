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

const extern sSpriteSheet* mSpriteSheetTypes_InGame_Amiga_CF2[];

class cGraphics_Amiga2 : public cGraphics_Amiga {

public:

    virtual void		Load_pStuff();
    virtual void        Load_And_Draw_Image(const std::string &pFilename, unsigned int pColors, size_t pBackColor);
    virtual void		Load_Service_Data();

    sImage              Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex);

    tSharedBuffer       GetPalette(const std::string pFilename);
    virtual sImage		GetImage(const std::string& pFilename, const size_t pPaletteIndex);

    virtual void        Map_Load_Resources();
    virtual void        Mission_Intro_Load_Resources(const eTileTypes pTileset);

    virtual void		SetActiveSpriteSheet(eGFX_Types pSpriteType);

    void                Recruit_Draw_Hill();

    cGraphics_Amiga2();
    ~cGraphics_Amiga2();
};
