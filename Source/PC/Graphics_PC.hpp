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

const extern sSpriteSheet* off_32C0C[];
const extern sSpriteSheet* mFontSpriteSheetPtr[];

class cGraphics_PC : public cGraphics {
	uint8*				mGraphicBlkPtrs[480];

	public:

	virtual uint8*		GetSpriteData( uint16 pSegment );
	virtual void		Mouse_DrawCursor();

	virtual void		graphicsBlkPtrsPrepare();
	virtual void		LoadpStuff();
	virtual void		Load_Sprite_Font();
	
	virtual void		map_Load_Resources();
	virtual void		map_Tiles_Draw();
	virtual void		sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY );

	virtual void		PaletteSet();
	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 );
	virtual void		SetSpritePtr( eSpriteType pSpriteType );

	virtual void		video_Draw_Linear();
	virtual void		video_Draw_Sprite();

	virtual void		sub_144A2();
	virtual void		sub_145AF( int16 pData0, int16 pData8, int16 pDataC );
};
