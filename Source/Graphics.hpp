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

enum eSpriteType {
	eSPRITE_IN_GAME = 0,
	eSPRITE_FONT	= 1,
	eSPRITE_HILL	= 2,
	eSPRITE_HILL_UNK = 3,
	eSPRITE_BRIEFING = 4,
};

class cFodder;

class cGraphics : public cSingleton<cGraphics> {
	
protected:
	cSurface*			mImage;
	cSurface*			mImageOriginal;
	cFodder*			mFodder;

public:
						cGraphics();

	virtual uint8*		GetSpriteData( uint16 pSegment ) = 0;
	virtual void		Mouse_DrawCursor() = 0;
	virtual void		LoadpStuff() = 0;
	virtual void		Load_Sprite_Font() = 0;
	virtual void		Load_Hill_Data() = 0;

	virtual void		graphicsBlkPtrsPrepare() = 0;
	
	virtual void		imageLoad( const std::string &pFilename, unsigned int pColors ) = 0;

	virtual void		map_Tiles_Draw() = 0;
	virtual void		map_Load_Resources() = 0;

	virtual void		sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY ) = 0;

	virtual void		PaletteSet() = 0;
	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 ) = 0;

	virtual void		video_Draw_Linear() = 0;
	virtual void		video_Draw_Sprite() = 0;

	virtual void		SetSpritePtr( eSpriteType pSpriteType ) = 0;
	virtual void		SetImage( cSurface* pImage );
	virtual void		SetImageOriginal();

	virtual void		sub_144A2( int16 pStartY = 0 ) = 0;
	virtual void		sub_145AF( int16 pData0, int16 pData8, int16 pDataC ) = 0;
	virtual void		sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 ) = 0;

	virtual void		Briefing_Load_Resources() = 0;
	virtual void		Recruit_Draw_Hill() = 0;
	virtual void		Recruit_Draw_HomeAway() = 0;
};
