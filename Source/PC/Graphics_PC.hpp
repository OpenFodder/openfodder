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

const extern sSpriteSheet* mSpriteSheetTypes_InGame_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Font_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Recruit_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Hill_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Briefing_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Service_PC[];

const extern uint8 mUnkStringModifier_Recruit[];

class cGraphics_PC : public cGraphics {
	uint8*				mTile_Gfx_Ptrs[480];
	sImage				mImageRecruit;

	public:
	virtual				~cGraphics_PC();

	virtual uint8*		GetSpriteData( uint16 pSegment );
	virtual void		Mouse_DrawCursor();

	virtual void		Tile_Prepare_Gfx();
	virtual void		Load_pStuff();
	virtual void		Load_Sprite_Font();
	virtual void		Load_Hill_Data();
	virtual void		Load_Service_Data();

	virtual void		Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors );
	virtual sImage		Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex);

	virtual void		Map_Load_Resources();
	virtual void		Map_Tiles_Draw();
	virtual void		MapOverview_Render_Tiles( uint16 pTile, uint16 pDestX, uint16 pDestY );
	
	virtual void		PaletteSetOverview();
	virtual void		PaletteSet();
	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 );
	virtual void		SetActiveSpriteSheet( eSpriteType pSpriteType );

	virtual void		Video_Draw_16();
	virtual void		Video_Draw_8();

	virtual void		Sidebar_Copy_To_Surface( int16 pStartY );
	virtual void		Sidebar_Copy_Sprite_To_ScreenBufPtr( int16 pSpriteType, int16 pX, int16 pY );
	virtual void		Sidebar_Copy_ScreenBuffer( uint16 Data0, int16 Data4, int16 pCopyToScreen, uint32*& Data20 );

	virtual void		Recruit_Sprite_Draw( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics );
	
	virtual bool		Sprite_OnScreen_Check();

	virtual void		Briefing_Load_Resources();
	virtual void		Briefing_DrawHelicopter( uint16 pID );

	virtual void		Recruit_Draw_Hill();
	virtual void		Recruit_Draw_HomeAway();
};
