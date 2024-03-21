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

const extern sSpriteSheet* mSpriteSheetTypes_InGame_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Font_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Recruit_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Hill_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Briefing_PC[];
const extern sSpriteSheet* mSpriteSheetTypes_Service_PC[];

const extern uint8 mUnkStringModifier_Recruit[];

class cGraphics_PC : public cGraphics {
	int32			mRecruitDestX;
	int32			dword_44A3E;
	int32			mRecruitDestY;

	uint8*				mTile_Gfx_Ptrs[480];

	sImage				mImageRecruit;
	tSharedBuffer		mBriefing_ParaHeli;
	tSharedBuffer		mMission_Intro_Gfx_Clouds1;
	tSharedBuffer		mMission_Intro_Gfx_Clouds2;
	tSharedBuffer		mMission_Intro_Gfx_Clouds3;
	tSharedBuffer		mMission_Intro_Gfx_TreesMain;

	uint16          mMission_Intro_DrawX;
	uint16          mMission_Intro_DrawY;

	int32 Heli_VeryBack;
	int32 Heli_Back;
	int32 Heli_middle;
	int32 Heli_Front;
	int32 Heli_TextPos;
	int32 Heli_TextPosBottom;

	public:
	virtual				~cGraphics_PC();

	virtual uint8*		GetSpriteData( uint16 pSegment );
	virtual void		Mouse_DrawCursor();

	virtual void		Tile_Prepare_Gfx();
	virtual void		Load_pStuff();
	virtual void		Load_Sprite_Font();
	virtual void		Load_Hill_Data();
	virtual void		Load_Service_Data();

	virtual void		Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors, size_t pBackColor);
	virtual sImage		Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex);

	virtual void		Map_Load_Resources();
	virtual void		Map_Tile_Draw(cSurface *pTarget, uint16 pTile, uint16 pX, uint16 pY, uint16 pOffset);
	virtual void		MapTiles_Draw();
	virtual void		MapOverview_Render_Tiles( uint16 pTile, uint16 pDestX, uint16 pDestY );
	
	virtual void		PaletteSetOverview();
	virtual void		PaletteSet(cSurface *pTarget);
	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 );
	virtual void		SetActiveSpriteSheet(eGFX_Types pSpriteType );

	uint8				Video_Get_Pixel(uint8* pSi, int16 pX, int16 pY);
	void				Video_Put_Pixel(uint8* pDi, uint8 pAl);

	virtual void		Video_Draw_16(const uint8* RowPallete = 0);
	virtual void		Video_Draw_8(cSurface *pTarget = 0, const uint8* RowPallete = 0);

	virtual void		Sidebar_Copy_To_Surface( int16 pStartY, cSurface* pSurface = 0);
	virtual void		Sidebar_Copy_Sprite_To_ScreenBufPtr( int16 pSpriteType, size_t pX, size_t pY );
	virtual void		Sidebar_Copy_ScreenBuffer( uint16 pRow, int16 pRows, int16 pCopyToScreen, uint32*& Data20 );

	virtual void		Recruit_Sprite_Draw( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics );
	
	virtual bool		Sprite_OnScreen_Check();

	virtual void		Mission_Intro_Load_Resources(const eTileTypes pTileset);
	virtual void		Mission_Intro_DrawHelicopter( uint16 );

	void				Mission_Intro_Play( const bool pShowHelicopter, const eTileTypes pTileset, const std::string pTop, const std::string pBottom);

	void				Mission_Intro_Render_2(tSharedBuffer pDs, int16 pCx);
	void				Mission_Intro_Render_1(tSharedBuffer pDs, int16 pCx);

	void				Briefing_Helicopter_Background_Unk_1();

	void				sub_15B98(tSharedBuffer pDs, int16 pCx);

	virtual void		Recruit_Draw_Hill();
	virtual void		Recruit_Draw_HomeAway();
};
