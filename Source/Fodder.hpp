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

#include "Sprites.hpp"

enum eWeaponSelected {
	eWeapon_None = 0,
	eWeapon_Grenade = 1,
	eWeapon_Rocket = 3,
};

enum eSquad_Weapon_SplitMode {
	eSquad_Weapon_Split_All = 0,
	eSquad_Weapon_Split_Half = 1,
	eSquad_Weapon_Split_None = 2
};

struct sSquad_Member {
	int16		mRecruitID;
	uint8		mRank;
	int8		mPhaseCount;		// Number of phases completed this Mission
	sSprite*	mSprite;
	uint16		field_6;
	int8		field_8;
	int8		mSelected;
	uint16		mNumberOfKills; 
};

struct sSpriteSheet {
	uint16 field_0;
	uint16 field_2;
	int16 field_4;
	int16 field_6;
	int16 mColCount;
	int16 mRowCount;
	int16 field_C;
	int8 field_E;
	int8 field_F;
};

struct sHero {
	int8	mRecruitID;
	int16	mRank;
	int16	mKills;	
};

struct struct_2 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
};

struct sMapPosition {
	int16	mX;
	int16	mY;
};

union sMapTarget {
	int32 asInt;

	struct {
		int16	field_0;
		int16	field_2;
	};
};

extern const struct_2 stru_44B50[209];

class cFodder : public cSingleton < cFodder > {
	public:
	char					mInputString[0x15];

	std::vector<const sVersion*> mVersions;

	bool					mSkipIntro;
	std::vector<cEvent>		mEvents;
	
	const sVersion*			mVersion;

	cGraphics*				mGraphics;
	cSound*					mSound;
	cResources*				mResources;
	cWindow*				mWindow;

	uint8*					word_397D8;

	sSprite					mSprite_Spare;
	sSprite					mSprites[45];

	const sSpriteSheet**	mSpriteDataPtr;
	
	void					(cFodder::*mSprite_Function[117])( sSprite* pSprite );

	std::stringstream		mTitle;

	std::string		mFilenameCopt;
	std::string		mFilenameBaseSwp;
	std::string		mFilenameBaseHit;
	std::string		mFilenameBaseBht;
	std::string		mFilenameArmy;
	std::string		mFilenameSubSwp;
	std::string		mFilenameSubHit;
	std::string		mFilenameSubBht;
	std::string		mFilenameBasePal;

	cSurface*		mSurfaceMapOverview;
	cSurface*		mImage;
	int32			mSurfaceMapTop, mSurfaceMapLeft;

	cPosition		mMousePosition;
	uint32			mMouseButtons;

	uint16			mMapWidth;
	uint16			mMapHeight;
	uint8			mKeyCode;

	uint8*			mDataPStuff;
	uint8* 			mDataHillBits;
	uint8*			mDataArmy;
	uint8*			mMap;
	size_t			mMapSize;
	size_t			mDataBlkSize;
	uint8*			mDataBaseBlk;
	uint8*			mDataSubBlk;
	size_t			mDataBaseBlkSize;
	size_t			mDataSubBlkSize;
	sRecruitRendered	mRecruit_Rendered[0x0D];

	uint32			dword_3901A;
	uint16			word_3901E;
	int16			word_39096;
	int16			word_3909A;
	uint16			word_390A4;
	int16			word_390A6;
	uint16			word_390AE;
	int16			word_390B0;
	uint16			word_390B8;
	
	/************** Save Game Region *********** */
	uint16			mMapNumber;
	int16			mSprite_Enemy_AggressionAverage;
	int16			mSprite_Enemy_AggressionMin;
	int16			mSprite_Enemy_AggressionMax;
	int16			mSprite_Enemy_AggressionNext;
	int16			mSprite_Enemy_AggressionIncrement;
	uint16			mMissionNumber;
	uint16			mMissionPhase;
	uint16			mSquad_AliveCount;
	uint16			mSaved_MissionNumber;					// unused
	uint16			mSaved_MapNumber;						// unused
	int16			word_390D4;
	sSprite*		mSquad_SpritePtrs[8];					// unused
	int16			mSprite_Enemy_AggressionCreated_Count;
	uint16			word_390E8;
	int16			mGame_Prepare;
	int16			word_390EC;
	uint16			mMissionPhaseRemain;
	uint16			mMissionPhases;
	uint16			mRecruit_NextID;
	int16			word_390F8;
	sSquad_Member	mSquad[9];

	int16			mGraveRanks[361];
	int16*			mGraveRankPtr;
	int16*			mGraveRankPtr2;
	int16			mGraveRecruitID[361];
	int16*			mGraveRecruitIDPtr;

	sHero			mHeroes[6];

	int16			mTroops_Away;
	int16			mTroops_Home;
	int16			mMapPlayerTroopCount;
	int16			mSquadMemberCount;
	int16			mTroopsAvailable;

	/************** End Save Game Region *********** */

	int16			mButtonPressLeft, mButtonPressRight;
	int16			mMouse_Button_Left_Toggle;
	int16			word_39F00;
	int16			mMouse_Exit_Loop;
	int16			word_39F06;
	int16			word_39FA0;
	int16			word_39FA2;
	int16			word_39FA4;
	int16			word_39FA6;
	int16			mCamera_Column;
	int16			mCamera_Row;
	int16			word_39FAC;

	int16			word_39FAE;
	int16			word_39FBA;

	int16			word_39FBC;
	int16			mMouse_Button_Right_Toggle;
	int16			mMouse_Button_LeftRight_Toggle;
	int16			word_39F04;
	int32			dword_39F18;
	int32			dword_39F1C;
	int32			dword_39F20;
	int32			dword_39F24;
	int32			dword_39F28;
	int32			mCamera_Adjust_Col;
	int32			mCamera_Adjust_Row;
	int32			dword_39F36;
	int16			word_39F3A;
	int16			word_39F3C;
	int16			word_39F3E;

	int32			dword_39F84;		// X : Unknown
	int32			dword_39F88;		// Y 
	int32			dword_39F8C;
	int32			dword_39F90;

	int16			word_39F34;
	int16			word_39F38;
	int16			word_39F40;
	int32			dword_39F4A;
	int32			dword_39F4E;
	int16			word_39F52;
	int16			word_39F54;
	int32			mCamera_Speed_X;
	int32			mCamera_Speed_Y;
	int32			dword_39F56;
	int32			dword_39F5A;
	int16			mCamera_Adjust_Col_High;
	int16			mCamera_Adjust_Row_High;
	int16			word_39F66;
	int32			dword_39F7C;

	int16			word_39F94;
	int16			word_39F96;
	int16			dword_39F98;
	int16			word_39F9C;
	int16			word_39F9E;
	int16			word_39FB2;
	int16			word_39FB4;
	int16			word_39FB6;
	int16			word_39FB8;
	sSprite*		mSquad_Leader;
	int16			mSquad_Selected;
	int16			word_39FD2;
	int16			word_39FD4;

	int16			word_39FF2;
	int16			word_39FF4;
	int16			word_39FF6;
	int16			word_39FF8;
	int16			word_39FFA;

	int16			mTroop_Cannot_Throw_Grenade;
	int16			mTroop_Cannot_Fire_Bullet;
	int16			word_3A010;
	int16			word_3A016;
	uint16			word_3A024;
	sGUI_Element*	mGUI_Loop_Element;
	int32			dword_3A030;
	uint16			word_3A054;
	int8			mSquads_TroopCount[4];
	uint8			byte_3A05E;
	uint16			word_3A05F;
	int16			mCamera_Position_Column;
	int16			mCamera_Position_Row;
	int16			mCamera_Position_X;
	int16			mCamera_Position_Y;
	int16			word_3A06B;
	sSprite*		dword_3A071[45];
	sSprite*		dword_3A125[45];
	sSprite*		dword_3A1D9[45];
	int16			word_3A28D;

	int16			word_3A8B1;
	int16			word_3A8D5;
	int16			word_3A8D7;
	int16			word_3A8D9;
	int32			dword_3A8DB;
	uint8			byte_3A8DE[200];

	sSprite*		dword_3A291[64];
	int32			dword_3A391;
	uint32			dword_3A395;
	int16			mTmp_FrameNumber;
	uint32			dword_3A39D;
	int16			word_3A3AB;
	int16			word_3A3AD;
	int16			word_3A3AF;
	int16			word_3A3A1;
	int16			word_3A3A7;
	int16			word_3A3A9;
	int16			word_3A3B9;
	int16			word_3A3BB;
	int16			word_3A3BD;
	int16			word_3A3BF;
	sMapTarget*		dword_3A3F9;
	sMapTarget		stru_3A3FD[30];
	sMapTarget		stru_3A475[30];
	sMapTarget		stru_3A4ED[30];
	sMapTarget		stru_3A565[30];
	sMapTarget		stru_3A5DD[30];
	sMapTarget		stru_3A655[30];
	sMapTarget		stru_3A6CD[30];
	sMapTarget		stru_3A745[30];
	sMapTarget		stru_3A7BD[30];
	sMapTarget		stru_3A835[30];
	int16			word_3A8CF;

	int16			word_3A9A6[2];
	int16			mMission_TryAgain;
	int16			mMission_Complete;
	int16			mMission_Completed_Timer;
	int16			mEnemy_BuildingCount;
	int16			mMission_Aborted;
	int16			mSquad_SwitchWeapon;
	int16			word_3A9B8;
	int16			word_3A9BA[3];	// values here, seem to be a byte count originally.. now its an index
	int16			word_3A9C0[3];
	int16			word_3A9C6;
	sSprite**		dword_3A9C8;
	int16			word_3A9CE;
	int16			mMission_Paused;
	int8			byte_3A9D2[4];
	int8			byte_3A9D6[4];
	int8			byte_3A9DA[10];
	int16			word_3A9E4;
	int16			word_3A9E6;
	int16			word_3A9F3;
	int16			word_3A9F5;
	int16			mMouseSpriteNew;
	int16			word_3A9FB;
	int32			dword_3A9FD;
	int16			word_3AA01;
	int16			mSprites_Found_Count;
	int16			mSquad_Grenades[3];
	int16			mSquad_Rockets[3];
	int16			word_3AA11[3];
	int16			mTroops_Enemy_Count;
	int16			mHostage_Count;
	int16			mCamera_Start_Adjust;
	int16			word_3AA1D;
	int16			mCamera_Reached_Target;
	int16			word_3AA21;
	int16			word_3AA41;
	int16			word_3AA45;
	int16			mSquad_Select_Timer;
	int16			word_3AA4B;
	int32			mMapWidth_Shifted;
	int32			mMapHeight_Shifted;
	int16			mMouseCursor_Enabled;
	int16			word_3AA55;
	int16			word_3AA67;
	int16			word_3AA71;
	int16			word_3AAC7;
	sRecruitRendered*			dword_3AAC9;
	int16			word_3AACD;
	int16			word_3AACF;
	int16			word_3AAD1;

	int16			word_3AA43;
	int16			word_3AB39;
	int16			word_3ABA7;
	int8			byte_3ABA9;
	int16			word_3ABAD;
	int16			word_3ABAF;
	int16			word_3ABB1;
	int16			mSquad_Member_Fire_CoolDown;
	int16			word_3ABB5;
	int16			word_3ABB7;
	sWeaponData		mSprite_Weapon_Data;
	int16			mSprite_Bullet_Time_Modifier;
	int16			mSprite_Bullet_Fire_Speed_Modifier;
	int16			word_3ABC7;
	sSquad_Member*	dword_3ABC9;
	int16			word_3ABE7;
	int16			word_3ABE9;
	int16			word_3ABEB;
	int16			word_3ABFB;
	int16			word_3ABFD;
	int16			word_3ABFF[9];
	int16*			dword_3AC11;
	int16			mString_GapCharID;		// Replace spaces in strings with this id

	int16			mGUI_Loop_Squad_Current;
	int16			word_3AC1D;
	int16			mGUI_Squad_Current;
	int16			word_3AC2D[3];
	int16			word_3AC21;
	int16			mGUI_Squad_NextDraw_Y;
	int16			mGUI_Sidebar_Setup;
	int8			mGUI_RefreshSquadGrenades[3];
	int8			mGUI_RefreshSquadRockets[3];
	int16			mSquad_CurrentWeapon[3];
	int16			mMouseDisabled;
	int16			word_3AC47;
	int16			word_3AC49;
	int16			word_3AC4B;
	int16			word_3AC4D;
	int16			word_3AC4F;
	int16			word_3AC51;
	sGUI_Element	mGUI_Elements[42];
	sGUI_Element*	mGUI_NextFreeElement;
	int16			word_3AEFB;
	int16			word_3AF01;
	int16			mMap_Destroy_Tile_X;
	int16			mMap_Destroy_Tile_Y;
	int16			word_3AF07;
	sMapPosition	mMap_Destroy_Tiles[128];

	sMapPosition*	mMap_Destroy_TilesPtr2;
	sMapPosition*	mMap_Destroy_TilesPtr;
	int16			word_3B15D;
	int16			word_3B15F;
	int16			word_3B161;
	int16			word_3B163;
	int16			word_3B173;
	int16			word_3B175;
	int16			word_3B177;
	int16			word_3B179;
	int16			word_3B17B;
	int16			word_3B17D;
	int16			word_3B17F;
	int16			word_3B189;
	int16			word_3B18B;
	int16			word_3B1A9;
	int16			word_3B1AB;
	int32			dword_3B1CB;
	int16			word_3B1CF[0x0F];
	int16			word_3B1ED;
	int16			mRecruit_Reached_Truck;
	int16			word_3B1F1;

	const sRecruit_Screen_Pos*		dword_3B1FB;
	
	int16			mMap_TileSet;
	sSprite*		mSquad_CurrentVehicle;
	int16			mMission_In_Progress;
	int16			word_3B211;
	sSprite*		dword_3B213[15];
	sSprite*		dword_3B24F[3];
	int16			word_3B25B;
	int16			word_3B25D;
	int16			word_3B2CB;
	int16			mGUI_SaveLoadAction;
	int16			word_3B2CF;
	int16			word_3B2D1[6];
	int16			word_3B2DD[5];
	int16			word_3B2ED;
	int16			mSpawnSpriteType;
	int16			word_3B2F1;
	int16			word_3B2F3;
	int16			word_3B2F5;
	int16			word_3B2F7;
	int16			mGame_Load;
	int16			mGame_Save;
	int16			mGUI_Temp_X;
	int16			mGUI_Temp_Width;

	int16			mGUI_Temp_Y;
	int16			mGUI_Temp_Height;

	void			(cFodder::*dword_3B30D)(int16 pPosY);

	int16			mInputString_Position;
	int16			word_3B335;
	int16			word_3B33D;
	int16			word_3B33F;

	int16			mPhase_Goals[8];
	int16			word_3B447;
	int16			word_3B449;
	int16			mSoundEffectToPlay;

	int16			word_3B461[3];
	sSprite*		mSprite_OpenCloseDoor_Ptr;
	int16			mSprite_Civilian_GotHome;
	int16			mSprite_Indigenous_Tmp_X;
	int16			mSprite_Indigenous_Tmp_Y;
	int16			word_3B481;
	int16			word_3B483;
	int16			mHelicopterCallPadCount;
	int16			mHelicopterCallPadPressedCount;
	sSprite*		mSprite_TroopsAlive[16];
	int16			mHelicopterCall_X;
	int16			mHelicopterCall_Y;
	sSprite*		dword_3B4CF;
	int16			word_3B4D3;
	int16			word_3B4D5;
	int16			word_3B4D7;
	int16			word_3B4D9;
	int16			mMission_ShowMapOverview;
	int16			word_3B4DD;
	int16			word_3B4DF;
	int16			word_3B4E9;
	int16			word_3B4EB;
	int16			word_3B4ED[2];
	int16			mMission_Finished;
	int16			word_3B4F3;
	int16			word_3B4F5;

	sSprite*		dword_3B5F5;
	int32			mMapTilePtr;
	int16			word_3B60E;
	int16			word_3B610;
	int16			word_3B612;
	int16			word_3B614;
	int16			mCamera_Pan_RowCount;	// Number of rows to move the camera

	uint16*			mMapSptPtr;
	size_t			mMapSptSize;

	uint16*			word_3BDAD;
	int16			mMouseX;
	int16			mMouseY;
	uint16			word_3BDB3;
	uint16			word_3BDB5;
	

	uint16			mSquad_Grenade_SplitMode;
	uint16			mSquad_Rocket_SplitMode;
	uint16			word_3BDEB;
	
	sSprite*		mSquad_0_Sprites[9];
	sSprite*		mSquad_1_Sprites[9];
	sSprite*		mSquad_2_Sprites[9];
	sSprite*		mSquad_3_Sprites[9];
	sSprite*		mSquad_4_Sprites[9];
	
	sSprite**		mSquads[5];
	int16			word_3BEB9;
	int16			word_3BEBB;
	int16			word_3BEBD;
	int16			word_3BEC1;
	int16			word_3BEC3;
	uint16			word_3BEC9;
	uint16			word_3BED5[5];
	uint16			word_3BEDF[10];
	
	sMapTarget*		mSprite_WalkTargets[10];
	int8			byte_3BF1B[3];
	sSprite*		word_3BF1E[6];

	uint16			mIntroDone;

	int16			graphicsBaseHit[240];
	int16			graphicsSub0[160];
	int16			graphicsBaseBht[960];
	int16			graphicsSub0Bht[640];
	int16			mMap_Base_Swp[240];
	int16			mMap_Sub_Swp[161];

	int16			word_3D465;
	int16			word_3D467;
	int16			word_3D469;
	int16			mCamera_Column_Previous;
	int16			mCamera_Row_Previous;
	uint16*			word_3D5B7;

	sGUI_Element	stru_3DEDC[2];
	int16			mMission_Save_Availability[0x18];

	uint8*			word_3E1B7;
	size_t			word_3E1B7_size;
	
	int16			word_3E75B;

	/* Configuration Settings (SETUP.DAT) */
	uint16			word_3E7ED;
	uint16			mEffectDriver;
	uint16			mMusicDriver;
	/* End Configuration Settings */

	int16			word_3EABD;

	int32			dword_3F946;
	int32			dword_3F94A;
	int16			word_3F94E;
	int16			word_3F950;

	int32			dword_3E9A3[70];
	int16			word_3FA1F;
	int16			word_3FA21;
	int16			mPaused;
	int16			mKeyControlPressed;

	int16			mImageFaded;
	int16			mMouseSpriteCurrent;
	int16			word_40048;
	uint16			word_40054;
	uint16			word_40056;
	uint16			word_40058;
	uint16			word_4005A;
	uint16			word_4005C;
	uint8*			mDrawSpriteFrameDataPtr;
	uint8*			word_42066;
	int16			mDrawSpritePositionX;
	int16			mDrawSpritePositionY;
	int16			mDrawSpriteColumns;
	int16			mDrawSpriteRows;
	uint8			byte_42070;
	uint8			byte_42071;
	uint16			word_42074;
	uint16			word_42076;
	uint16			word_42078;


	int16			word_42851;
	uint16			word_42859;
	uint16			word_4285B;
	uint8*			word_4285D;
	int16			word_4285F;

	uint8*			word_42861;
	uint8*			word_42863;
	uint8*			word_42865;
	int16			word_428B6;
	uint8*			word_42867;
	int16			word_428B8;
	uint8*			word_42869;
	uint16			word_428BA;
	uint16*			word_4286B;

	size_t			word_4286D;
	uint32			mHelicopterPosX;
	uint32			mHelicopterPosY;
	int16			word_428C6;
	int16			word_428C4;
	int16			word_428C8;
	const int16*	word_428CA;
	uint16			word_428CC;
	int16			word_428D6;
	int16			word_428D8;

	uint8			mErrorCode;
	int16			mKeyNumberPressed;

	int16			mouse_Button_Status;
	int16			mouse_Pos_Column;
	int16			mouse_Pos_Row;

	int16			word_42072;
	uint16			word_427D4;
	int8			byte_427E6;
	int8			byte_427EE;
	int16			word_44475;
	int16			word_44A20;
	int16			word_44A2E;
	int16			word_44A30;
	int16			word_44A32;
	int16			word_44A34;

	int32			dword_44A36;
	int32			dword_44A3E;
	int32			dword_44A3A;
	int32			dword_44A42;
	int32			dword_44A46;
	int16			byte_44AC0;
	int16			mInput_LastKey;

	int16			mDemo_ExitMenu;
	int16			mCustom_ExitMenu;

	enum eCustomMode {
		eCustomMode_None = 0,
		eCustomMode_Map = 1,
		eCustomMode_Set = 2
	};

	eCustomMode		mCustom_Mode;

	int64			mTicks;
	int64			mTicksDiff;

	cPalette		mPalette[ g_MaxColors ];

	void(cFodder::*mGUI_Handle_Button_SelectSquad_Array[3])(void);

public:
	bool			Demo_Amiga_ShowMenu();
    bool            Custom_ShowMenu();
	void            Custom_ShowMapSelection();
	void            Custom_ShowMissionSetSelection();
	
	void			Image_FadeIn();
	void			Image_FadeOut();

	virtual int16	Mission_Loop( );

	void			Game_Handle( );
	void			Camera_Handle();
	void			sub_1096B();
	void			Game_ClearVariables();
	void			Squad_Clear();
	void			Heroes_Clear();
	void			Mission_Memory_Backup();
	void			Mission_Memory_Restore();
	void			Mission_Memory_Clear();
	void			Mission_Prepare_Squads();
	void			sub_10DEC();
	void			Squad_Set_Squad_Leader();
	void			Sprite_Clear_All_Wrapper();
	void			Sprite_Clear_All();

	void			Map_Load_Sprites();
	void			Squad_Member_Count();
	void			Squad_Member_Sort();
	void			Squad_Prepare();
	void			Squad_Prepare_NextRecruit();
	void			Squad_Prepare_Sprites();
	void			Camera_Position_Update();
	int16			sub_119E1( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );

	void			Camera_Calculate_Scroll();
	void			sub_11CAD();
	void			Camera_Refresh( );
	void			map_Load_Resources();
	void			Music_Play_Tileset();
	void			Camera_Pan_To_Target( );
	void			Camera_Pan_Set_Speed();
	void			sub_11FCD();

	void			sub_12018();
	void			Camera_Reset();
	void			sub_120F6();
	void			Camera_Adjust_Row( int32* pData20 );
	void			sub_12245();
	void			Camera_Speed_Reset();
	void			Camera_Pan_Toward_SquadLeader();
	void			Mission_Sprites_Handle( );
	void			Sprite_Sort_DrawList();
	void			Sprite_Bullet_SetData();
	void			Mission_Phase_Goals_Check();
	void			Mission_Clear_Destroy_Tiles();
	void			Mission_Phase_Goals_Set();
	void			Mission_Progress_Check( );
	void			Mission_Text_Completed();
	void			Mission_Text_Sprite_Mission( sSprite* pData2C );
	void			Mission_Text_Sprite_Phase( sSprite* pData2C );
	void			Mission_Text_Sprite_Complete( sSprite* pData2C );
	void			Mission_Text_Prepare( sSprite* pData2C );
	void			Mission_Text_TryAgain();
	void			Mission_Text_Sprite_Try( sSprite* pData2C );
	void			Mission_Text_Sprite_Again( sSprite* pData2C );

	std::string		Filename_CreateFromBase( const std::string& pBase, const char* pFinish );
	void			Squad_Member_PhaseCount();
	void			sub_12AEE();
	void			sub_12B6E();
	void			sub_12C69();
	void			Mission_Map_Overview_Show();

	void			Map_Overview_Prepare();
	void			map_SetTileType();

	void			Sprite_Count_HelicopterCallPads();
	void			sub_13148();
	void			Sprite_HelicopterCallPad_Check();
	void			Mission_Final_Timer();
	int16			Sprite_Create_RandomExplosion();
	void			Mission_GameOver();
	void			Mission_Text_GameOver( sSprite* pData2C );
	void			Mission_Paused();
	void			sub_136D0();
	void			Video_Sleep_Wrapper();
	void			Mouse_DrawCursor( );
	void			Sprite_Draw_Frame( int32 pSpriteType, int32 pPositionY, int32 pFrame, int32 pPositionX );
	void			sub_13C8A(  int16 pData0, int16 pData4, int16 pPosX, int16 pPosY );
	void			sub_13CF0(  sSprite* pDi, int16 pData0, int16 pData4 );

	bool			Sprite_OnScreen_Check();
	void			Sprite_Draw( );
	void			Sprite_Map_Sound_Play( int16& pData0 );

	void			Sound_Play( sSprite* pSprite, int16 pSoundEffect, int16 pData8 );

	// 14EAC
	void			Briefing_Intro();

	void			Briefing_Intro_Jungle();
	void			Briefing_Intro_Desert();
	void			Briefing_Intro_Ice();
	void			Briefing_Intro_Mor();
	void			Briefing_Intro_Int();

	void			sub_1590B( );
	void			sub_1594F( );
	void			sub_159A6( );
	void			sub_15A36(  uint8* pDs, int16 pCx );
	void			sub_15B86(  uint8* pDs, int16 pCx );
	void			sub_15B98(  uint8* pDs, int16 pCx );

	void			sub_15CE8(  uint8* pDs, int16 pCx );

	void			Briefing_Draw_Mission_Name( );
	void			Briefing_Draw_Mission_Title( int16 pDrawAtY );

	void			CopyProtection();
	void			CopyProtection_EncodeInput();

	/* Recruitment */
	bool			Recruit_Show();
	void			Recruit_Draw_String(  int16 pParam0, int16 pParam8, int16 pParamC, const std::string& pString );
	void			sub_16BC3();
	void			sub_16C45( uint16** pDi, int16* pSource );
	void			sub_16C6C();
	void			Recruit_Render_LeftMenu();
	void			Recruit_Render_Squad_Names();
	void			Recruit_Render_Squad_RankKills();
	void			Recruit_Render_Number( int16 pNumber, int16 pData10 );
	void			Recruit_Render_HeroList();
	void			Recruit_Render_Names_UnusedSlots();
	void			sub_17429();
	void			Recruit_Draw_Actors( );
	void			sub_175C0();
	void			Recruit_Draw_Troops();
	void			sub_1787C();
	void			sub_178DD();
	void			sub_17911();
	void			Recruit_Draw_Truck( );
	void			sub_17B64();
	void			Recruit_Draw();
	void			Recruit_Draw_Graves( );
	void			Recruit_Draw_Grave( int16 pSpriteType, int16 pPosX, int16 pPosY );
	bool			Recruit_Check_Buttons_SaveLoad();
	void			Recruit_Render_Text( const char* pText, int16 pPosY );
	/* End Recruitment */

	/* Promotion / Heroes */
	void			Service_Show();
	void			Service_KIA_Loop();
	void			Service_Promotion_Loop();
	int16			Service_KIA_Troop_Prepare();
	int16			Service_Promotion_Prepare_Draw();
	void			Service_Draw_Troop_And_Rank( uint16*& pDi,int16 pRecruitID, int16 pRank );
	void			sub_18149();
	void			sub_181BD();
	void			sub_181E6( uint16*& pDi, const std::string& pText, const uint8* pData28, int16 pData0, int16 pData8, int16 pDataC );
	int16			sub_1828A( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );
	void			sub_182EA();
	int16			sub_184C7();
	void			Service_Mission_Text_Prepare( uint16*& pTarget );
	void			Service_Promotion_Prepare();
	void			Service_Promotion_Check();
	void			Service_Promotion_SetNewRanks();

	/* End Promotion / Heroes */

	int16			Squad_Join( sSprite* pSprite );

	void			Briefing_Show( );
	void			Briefing_Prepare();
	void			Briefing_Wait();
	void			Briefing_Draw_With( );
	void			Briefing_DrawBox( int16 pX, int16 pY, int16 pWidth, int16 pHeight, uint8 pColor );
	void			Briefing_Draw_Horizontal_Line( int16 pX, int16 pWidth, int16 pY, uint8 pColor );
	void			Briefing_Draw_Vertical_Line(  int16 pX, int16 pHeight, int16 pY, uint8 pColor );

	void			Briefing_Draw_Pixel( int16 pX, int16 pY, uint8 pColor );

	void			Intro_Print_String( int32 pPosX, const sIntroString* pString );

	void			sub_18D5E();
	
	void			Sprite_Table_Setup();

	void			Sprite_Handle_Loop();
	void			Sprite_Handle_Null( sSprite* pSprite );

	void			Sprite_Handle_Player( sSprite* pSprite );				// 0
	void			Sprite_Handle_Grenade( sSprite* pSprite );				// 2
	void			sub_1998C( sSprite* pSprite );
	void			Sprite_Handle_ShadowSmall( sSprite* pSprite );			// 3
	void			Sprite_Handle_Enemy( sSprite* pSprite );				// 5
	void			Sprite_Handle_Bullet( sSprite* pSprite );				// 6
	void			Sprite_Handle_Helicopter( sSprite* pSprite );
	void			Sprite_Handle_Explosion( sSprite* pSprite );			// 12
	void			Sprite_Handle_Shrub( sSprite* pSprite );				// 13
	void			Sprite_Handle_Tree( sSprite* pSprite );					// 14
	void			Sprite_Handle_BuildingRoof( sSprite* pSprite );			// 15
	void			Sprite_Handle_Snowman( sSprite* pSprite );							// 16
	void			Sprite_Handle_Shrub2( sSprite* pSprite );				// 17
	void			Sprite_Handle_Waterfall( sSprite* pSprite );			// 18
	void			Sprite_Handle_Bird2_Left( sSprite* pSprite );			// 19

	void			Sprite_Handle_BuildingDoor( sSprite* pSprite );							// 20
	void			Sprite_Handle_Player_Rank( sSprite* pSprite );							// 21
	void			sub_1AF78( sSprite* pSprite );											// 22
	void			sub_1B07C( sSprite* pSprite );											// 23
	void			Sprite_Handle_GroundHole( sSprite* pSprite );											// 24
	void			Sprite_Handle_BuildingDoor2( sSprite* pSprite );						// 25
	void			Sprite_Handle_Floating_Dead_Soldier( sSprite* pSprite );				// 27
	void			Sprite_Handle_Text_Complete( sSprite* pSprite );						// 28
	void			Sprite_Handle_Text_Mission( sSprite* pSprite );							// 29

	void			Sprite_Handle_Text_Phase( sSprite* pSprite );							// 30
	void			sub_1B4BB( sSprite* pSprite );
	void			Sprite_Handle_Rocket( sSprite* pSprite );								// 33
	void			Sprite_Handle_Text_GameOver( sSprite* pSprite );						// 34
	void			Sprite_Handle_Shadow( sSprite* pSprite );								// 35
	void			Sprite_Handle_Enemy_Rocket( sSprite* pSprite );							// 36
	void			Sprite_Handle_GrenadeBox( sSprite* pSprite );							// 37
	void			Sprite_Handle_RocketBox( sSprite* pSprite );							// 38
	void			Sprite_Handle_Building_Explosion( sSprite* pSprite );					// 39

	void			Sprite_Handle_Helicopter_Grenade_Enemy( sSprite* pSprite );				// 40
	void			Sprite_Handle_Flashing_Light( sSprite* pSprite );						// 41
	void			Sprite_Handle_Helicopter_Grenade2_Enemy( sSprite* pSprite );			// 42
	void			Sprite_Handle_Helicopter_Missile_Enemy( sSprite* pSprite );				// 43
	void			Sprite_Handle_Helicopter_Homing_Enemy( sSprite* pSprite );				// 44
	void			Sprite_Handle_Missile( sSprite* pSprite );								// 45
	void			Sprite_Handle_MissileHoming( sSprite* pSprite );						// 46
	void			sub_1C1C0( sSprite* pSprite );											// 47
	void			sub_1C268( sSprite* pSprite );											// 48
	void			Sprite_Handle_Helicopter_Grenade2_Human( sSprite* pSprite );			// 49

	void			Sprite_Handle_Helicopter_Grenade_Human( sSprite* pSprite );				// 50
	void			Sprite_Handle_Helicopter_Missile_Human( sSprite* pSprite );				// 51
	void			Sprite_Handle_Helicopter_Homing_Human( sSprite* pSprite );		// 52
	void			sub_1C2D2( sSprite* pSprite );											// 53
	void			Sprite_Handle_Mine( sSprite* pSprite );									// 54
	void			Sprite_Handle_Mine2( sSprite* pSprite );								// 55
	void			Sprite_Handle_Spike( sSprite* pSprite );								// 56
	void			sub_1C4C2( sSprite* pSprite );											// 57
	void			Sprite_Handle_Text_Try( sSprite* pSprite );								// 58
	void			Sprite_Handle_Text_Again( sSprite* pSprite );							// 59

	void			Sprite_Handle_BoilingPot( sSprite* pSprite );			// 60
	void			Sprite_Handle_Indigenous( sSprite* pSprite );			// 61
	void			Sprite_Handle_Indigenous2( sSprite* pSprite );			// 62
	void			Sprite_Handle_VehicleNoGun_Human( sSprite* pSprite );	// 63
	void			Sprite_Handle_VehicleGun_Human( sSprite* pSprite );		// 64
	void			Sprite_Handle_Tank_Human( sSprite* pSprite );			// 65
	void			Sprite_Handle_Bird_Left( sSprite* pSprite );			// 66
	void			Sprite_Handle_Bird_Right( sSprite* pSprite );			// 67
	void			Sprite_Handle_Seal( sSprite* pSprite );					// 68
	void			Sprite_Handle_Tank_Enemy( sSprite* pSprite );			// 69

	void			Sprite_Handle_Indigenous_Spear( sSprite* pSprite );		// 70
	void			sub_1CE80( sSprite* pSprite );							// 71
	void			Sprite_Handle_Hostage( sSprite* pSprite );				// 72
	void			Sprite_Handle_Hostage_Rescue_Tent( sSprite* pSprite );	// 73
	void			Sprite_Handle_Door_Indigenous( sSprite* pSprite );		// 74
	void			Sprite_Handle_Door2_Indigenous( sSprite* pSprite );		// 75
	void			Sprite_Handle_Door_Indigenous_Spear( sSprite* pSprite );// 76
	void			sub_1D4D2( sSprite* pSprite );							// 77
	void			Sprite_Handle_Turret_Missile_Human( sSprite* pSprite );	// 78
	void			Sprite_Handle_Turret_Missile2_Human( sSprite* pSprite );// 79

	void			Sprite_Handle_VehicleNoGun_Enemy( sSprite* pSprite );	// 80
	void			Sprite_Handle_VehicleGun_Enemy( sSprite* pSprite );		// 81
	void			Sprite_Handle_Vehicle_Unk_Enemy( sSprite* pSprite );	// 82
	void			Sprite_Handle_Indigenous_Invisible( sSprite* pSprite );	// 83
	void			Sprite_Handle_Turret_Missile_Enemy( sSprite* pSprite );	// 84
	void			Sprite_Handle_Turret_Missile2_Enemy( sSprite* pSprite );// 85
	void			sub_1D7DD( sSprite* pSprite );							// 86
	void			sub_1D802( sSprite* pSprite );							// 87
	void			Sprite_Handle_BuildingDoor3( sSprite* pSprite );		// 88
	void			Sprite_Handle_Explosion2( sSprite* pSprite );			// 89

	void			Sprite_Handle_OpenCloseDoor( sSprite* pSprite );										// 90
	void			Sprite_Handle_Seal_Mine( sSprite* pSprite );					// 91
	void			Sprite_Handle_Spider_Mine( sSprite* pSprite );						// 92
	void			Sprite_Handle_Bonus_RankToGeneral( sSprite* pSprite );				// 93
	void			Sprite_Handle_Bonus_Rockets( sSprite* pSprite );					// 94
	void			Sprite_Handle_Player_Rocket( sSprite* pSprite );					// 95
	void			Sprite_Handle_Bonus_RocketsAndGeneral( sSprite* pSprite );			// 96
	void			Sprite_Handle_MissileHoming2( sSprite* pSprite );					// 97
	void			Sprite_Handle_Bonus_SquadGeneralRockets( sSprite* pSprite );		// 98
	void			Sprite_Handle_Helicopter_CallPad( sSprite* pSprite );				// 99

	void			Sprite_Handle_BuildingDoor_Reinforced( sSprite* pSprite );										// 100
	void			Sprite_Handle_Helicopter_Grenade2_Human_Called( sSprite* pSprite );	// 101
	void			Sprite_Handle_Helicopter_Grenade_Human_Called( sSprite* pSprite );	// 102
	void			Sprite_Handle_Helicopter_Missile_Human_Called( sSprite* pSprite );	// 103
	void			Sprite_Handle_Helicopter_Homing_Human_Called( sSprite* pSprite );	// 104
	void			Sprite_Handle_Turret_HomingMissile_Enemy( sSprite* pSprite );		// 105
	void			Sprite_Handle_Hostage_2( sSprite* pSprite );						// 106
	void			Sprite_Handle_Helicopter_Homing_Enemy2( sSprite* pSprite );			// 107
	void			Sprite_Handle_Computer_1( sSprite* pSprite );						// 108
	void			Sprite_Handle_Computer_2( sSprite* pSprite );						// 109
	void			Sprite_Handle_Computer_3( sSprite* pSprite );						// 110

	int16			sub_1D92E( sSprite* pSprite );
	void			sub_14D6D( sSprite* pSprite, int16 pData4 );
	int16			sub_1E05A( sSprite* pSprite );
	int16			Sprite_Troop_Dies( sSprite* pSprite );
	int16			loc_1F043( sSprite* pSprite );
	void			loc_1F092( sSprite* pSprite, sSprite* pData24 );

	int16			Sprite_Handle_Player_MissionOver( sSprite* pSprite );
	void			Sprite_Handle_Troop( sSprite* pSprite );
	void			sub_1F5A0( sSprite* pSprite );
	void			sub_1F5CA( sSprite* pSprite );
	void			Sprite_XY_Store( sSprite* pSprite );
	void			Sprite_XY_Restore( sSprite* pSprite );

	void			sub_1F66F( sSprite* pSprite );
	void			sub_1F6F4( sSprite* pSprite );
	void			sub_1F762( sSprite* pSprite );
	void			sub_1FCF2( sSprite* pSprite );
	void			sub_1FDE7( sSprite* pSprite );
	void			sub_1FE35( sSprite* pSprite );
	int16			sub_1FF1A( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 );
	void			Sprite_Handle_Soldier_Terrain_Check( sSprite* pSprite, int16& pData4 );

	void			sub_20456( sSprite* pSprite, int16& pData8 );
	void			sub_20478( sSprite* pSprite );
	void			Hero_Add( sSquad_Member* pSquadMember );

	int16			Sprite_Destroy_Wrapper( sSprite* pSprite );
	int16			Sprite_Destroy( sSprite* pSprite );
	int16			Sprite_Create_Bullet( sSprite* pSprite );
	int16			Sprite_Create_Grenade( sSprite* pSprite );
	void			sub_20E5C( sSprite* pSprite );
	int16			sub_20E91( sSprite* pSprite );
	void			sub_20F19( sSprite* pSprite );

	void			sub_21041( sSprite* pSprite );
	int16			Sprite_Get_Free( int16& pData0, sSprite*& pData2C, sSprite*& pData30 );
	void			Sprite_Clear( sSprite* pSprite );
	void			Sprite_Handle_Exploidable( sSprite* pSprite );
	void			Sprite_Create_Shadow( sSprite* pSprite );
	void			Sprite_Handle_Grenade_Terrain_Check( sSprite* pSprite );
	int16			sub_21618( sSprite* pSprite );
	void			sub_21702( sSprite* pSprite, int16 pData18 );
	void			sub_2183B( sSprite* pSprite );
	void			sub_218E2( sSprite* pSprite );
	int16			Sprite_Create_Building_Explosion_Wrapper( int16& pX, int16& pY );
	int16			Sprite_Create_Building_Explosion( sSprite* pData2C, int16& pX, int16& pY );
	int16			Sprite_Create_Enemy( sSprite* pSprite, sSprite*& pData2C );
	int16			Sprite_Get_Free2( int16& pData0, sSprite*& pData2C, sSprite*& pData30 );
	void			sub_21C00( sSprite* pData2C );
	void			Sprite_Create_Rank( );
	void			sub_21CD1( sSprite* pSprite );

	void			sub_2212A( sSprite* pSprite );
	int16			Sprite_Handle_BuildingDoor_Explode( sSprite* pSprite );
	int16			sub_222A3( sSprite* pSprite );
	void			sub_223B2( sSprite* pSprite );
	int16			Map_Get_Distance_Between_Sprite_And_Squadleader( sSprite* pSprite, int16& pData0 );
	int16			sub_224ED( sSprite* pSprite );
	int16			Sprite_Homing_LockInRange( sSprite* pSprite, sSprite*& pFoundSprite );
	void			sub_229C9( sSprite* pSprite );
	void			sub_22A3D( sSprite* pSprite );
	int16			Sprite_Destroy_Wrapper_2( sSprite* pSprite );
	void			sub_22AA9( sSprite* pSprite );
	void			Sprite_Handle_Player_Unk( sSprite* pSprite );
	void			sub_22C87( sSprite* pSprite );
	void			sub_22CD7( sSprite* pSprite, int16& Data0, int16& Data4 );
	void			Sprite_Handle_Vehicle_Terrain_Check( sSprite* pSprite );
	void			Sprite_Under_Vehicle( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C );
	int16			sub_2329D( sSprite* pSprite );
	int16			Map_Sprite_Check_Around_Position( sSprite* pSprite );
	void			Sprite_Handle_Vehicle_Human( sSprite* pSprite );
	int16			sub_23444( sSprite* pSprite, sSprite*& pData2C  );
	void			sub_23525( sSprite* pSprite );
	void			sub_236F7( sSprite* pSprite );
	void			sub_23879( sSprite* pSprite );
	int16			Sprite_Create_Missile( sSprite* pSprite, sSprite*& pData2C );
	void			sub_23C70( sSprite* pData2C );
	void			Sprite_Handle_Helicopter_Human( sSprite* pSprite );
	int16			Sprite_Handle_Helicopter_Terrain_Check( sSprite* pSprite );
	void			sub_23E01( sSprite* pSprite, int16& pData0, int16& pData4  );
	void			sub_23EA6( sSprite* pSprite );
	void			Sprite_Handle_Turret( sSprite* pSprite );
	void			Sprite_Handle_Turret_Fire( sSprite* pSprite, sSprite* pData34 );
	int16			Sprite_Find_By_Types( sSprite* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite*& pData28 );
	void			sub_243E9( sSprite* pSprite );
	int16			sub_2449E( sSprite* pSprite );
	void			sub_245BF( sSprite* pSprite );
	int16			sub_246CC( sSprite* pSprite );
	void			Sprite_Handle_Helicopter_Human_Deploy_Weapon( sSprite* pSprite );
	int16			Sprite_Create_Grenade2( sSprite* pSprite );
	int16			Sprite_Create_MissileHoming( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34 );
	int16			sub_2531F( sSprite* pSprite, sSprite*& pData30 );
	void			Sprite_Handle_Vehicle_Enemy( sSprite* pSprite );
	void			Sprite_Handle_Helicopter_Enemy( sSprite* pSprite );
	void			sub_25863( sSprite* pSprite );
	void			Sprite_Handle_Indigenous_Movement( sSprite* pSprite );
	int16			Sprite_Handle_Indigenous_Within_Range_OpenCloseDoor( sSprite* pSprite );
	void			Sprite_Handle_Indigenous_Death( sSprite* pSprite );
	void			sub_2593D( sSprite* pSprite );
	void			sub_25A31( sSprite* pSprite );
	void			sub_25A66( sSprite* pSprite );
	int16			Sprite_Handle_Indigenous_RandomMovement( sSprite* pSprite );
	int16			sub_25B6B( sSprite* pSprite );
	int16			sub_25DCF( sSprite* pSprite );
	void			sub_25F2B( sSprite* pSprite );
	void			sub_25FDA( sSprite* pSprite );
	void			Sprite_Handle_Hostage_FrameUpdate2( sSprite* pSprite );
	void			Sprite_Handle_Hostage_FrameUpdate( sSprite* pSprite );
	void			sub_26490( sSprite* pSprite );
	void			sub_264B0( sSprite* pSprite );
	int16			sub_265D6( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 );
	int16			Sprite_Handle_Helicopter_Callpad_InRange( sSprite* pSprite, sSprite*& pData2C );
	void			Sprite_Handle_Helicopter_Human_CallCheck( sSprite* pSprite );
	void			Sprite_Handle_Computer( sSprite* pSprite, int16 pData1C );

	void			Map_Get_Distance_BetweenPoints_Within_320( int16& pX, int16& pY, int16& pX2, int16& pY2 );
	int16			Direction_Between_Points( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );

	void			tool_RandomSeed();
	int16			tool_RandomGet();
	void			Sprite_Movement_Calculate( sSprite* pSprite );
	int16			Direction_Between_SpriteAndPoints( sSprite* pSprite, int16& pData0, int16& pData4 );
	void			sub_2A3D4( sSprite* pSprite );
	void			sub_2A470();
	int16			sub_2A4A2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );
	void			sub_2A4FD( int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData18, int16& pData1C );
	int16			sub_2A622( int16& pData0 );
	int16			Map_Get_Distance_BetweenPoints( int16& pPosX, int16& pPosY, int16& pPosX2, int16& pDistance, int16& pPosY2 );

	int16			Map_Terrain_Get_Type_And_Walkable( int16& pY, int16& pX );
	int16			Map_Terrain_Get_Type_And_Walkable( sSprite* pSprite, int16& pY, int16& pX );
	int16			Map_Terrain_Get( int16& pY, int16& pX, int16& pData10, int16& pData14 );

	void			Squad_Walk_Target_Set( int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );
	int16			Squad_Member_Sprite_Hit_In_Region( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 );

	uint8*			Sprite_Get_Gfx_Ptr( int16& pSpriteType, int16& pFrame );
	void			sub_2AEB6( int16 pColumns, int16 pRows, int16 *pData8, int16* pDataC );
	uint8			sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx );

	void			sub_2B016( uint8* pDi, uint8 pAl );
	void			sub_2B12E( sSprite* pSprite );
	void			Sprite_Vehicle_Direction_Update( sSprite* pSprite, int16& pData1C );

	int16			Map_Terrain_Get_Moveable_Wrapper( const int8* pMovementData, int16& pX, int16& pY, int16& pData10, int16& pData14 );
	int16			Map_Terrain_Get_Moveable( const int8* pMovementData, int16& pX, int16& pY, int16& pData10, int16& pData14 );

	void			Map_Get_Distance_BetweenPoints_Within_640( int16& pX, int16& pY, int16& pX2, int16& pY2 );

	void			Camera_Pan();

	void			Camera_Pan_Right();
	void			Camera_Pan_Left();
	void			Camera_Pan_Down();
	void			Camera_Pan_Up();
	void			Camera_Update_Row();
	void			Camera_Update_Column();

	void			Squad_Troops_Count();
	int16			Squad_Member_GetDeviatePotential( sSquad_Member* pSquadMember );
	void			Squad_Member_Rotate_Can_Fire();
	int16			Sprite_Find_In_Region( sSprite* pSprite, sSprite*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 );
	void			Sprite_Handle_Player_DestroyAll();
	void			Sprite_Handle_Player_Destroy_Unk();
	void			Squad_Prepare_GrenadesAndRockets();
	void			Sprite_Aggression_Set();
	void			Sprite_Handle_Enemy_Aggression_Set( sSprite* pSprite );
	int16			Sprite_Next_WalkTarget_Set( sSprite* pSprite );
	int16			Squad_Join_Check( sSprite* pSprite );
	void			sub_2DCB0( int16 pData0 );
	void			Sprite_Handle_Explosion_MapTiles( sSprite* pSprite );
	void			Map_Destroy_Tiles( );
	void			Map_Destroy_Tiles_Next();
	
	void			Game_Save_Wrapper2();
	void			Game_Save_Wrapper();

	void			GUI_Element_Reset();

	void			GUI_Button_Setup( void(cFodder::*pFunction)(void) );
	void			GUI_Button_Draw( const char* pText, int16 pY, int16 pColorShadow = 0xBF, int16 pColorPrimary = 0xBC );
	
	void			GUI_Box_Draw( int16 pColorShadow, int16 pColorPrimary );
	void			GUI_SaveLoad( bool pShowCursor );
	std::string		GUI_Select_File( const char* pTitle, const char* pPath, const char* pType );

	void			Game_Save();
	void			GUI_SaveLoad_MouseHandle( sGUI_Element* pData20 );
	void			GUI_Button_Load_Exit();
	void			String_Print_Input( int16 pPosY );
	void			sub_2E6A9();
	void			GUI_Input_CheckKey();
	void			Game_Load();
	void			GUI_Button_Load_Up();
	void			GUI_Button_Load_Down();
	void			GUI_Button_Filename();

	int16			GUI_Button_NoAction();
	void			GUI_Button_NoAction2();
	void			GUI_Sidebar_Prepare_Squads();
	
	void			GUI_Button_SelectQuiz();
	void			GUI_Button_SelectMap0();
	void			GUI_Button_SelectMap1();
	void			GUI_Element_Mouse_Over( const sGUI_Element *pA0 );
	void			GUI_Button_SelectMap2();
	void			GUI_Button_SelectMap3();
	void			Demo_Quiz();
	void			GUI_Button_ExitMenu();
	void			GUI_Button_Quiz_2();
	void			GUI_Button_Quiz_3();
	void			GUI_Button_Quiz_4();
	void			GUI_Button_Quiz_5();
	void			GUI_Button_Quiz_6();
	void			GUI_Button_Quiz_7();
	void			GUI_Button_Quiz_8();
	void			GUI_Button_Quiz_9();
	void			GUI_Button_Quiz_10();
	void			GUI_Button_Quiz_11();
	void			Demo_Quiz_ShowScreen( const char* pFilename );

	void			sub_2EBE0( int16& pData0, int16& pData4 );
	void			GUI_Sidebar_Prepare( int16 pData0, int16 pData4 );
	void			sub_2EBC4();
	void			GUI_SetElementsFrom( sGUI_Element* pData20, sGUI_Element* pData24 );
	void			GUI_ClearElement( sGUI_Element *pData20 );
	void			GUI_Sidebar_SplitButton_Draw();
	void			GUI_Prepare_Button_Squad();
	void			GUI_Handle_Button_SelectSquad_0();
	void			GUI_Handle_Button_SelectSquad_1();
	void			GUI_Handle_Button_SelectSquad_2();
	void			Squad_Select( int16 pData4 );
	void			GUI_Handle_Button_SplitSquad();
	void			Mission_Set_Initial_Weapon();

	void			map_Tiles_Draw();

	void			Game_CheckLoadSave();
	void			GUI_Sidebar_Grenades_Draw( );

	void			GUI_Prepare_Button_Grenade();
	void			GUI_Handle_Button_Grenades();
	void			GUI_Sidebar_TroopList_Draw();
	void			GUI_Sidebar_TroopList_Name_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, const char* pData28 );
	void			sub_2F452();
	void			GUI_Sidebar_SquadIcon_Draw();
	int16			sub_2F4CB();
	void			GUI_Sidebar_Refresh();
	void			GUI_Sidebar_SquadIcon_Refresh();
	void			GUI_Sidebar_Grenades_Refresh_CurrentSquad( );
	void			GUI_Sidebar_Grenades_Refresh( int16 pData0 );
	void			GUI_Sidebar_Rockets_Refresh_CurrentSquad( );
	void			GUI_Sidebar_Rockets_Refresh( int16 pData0 );
	int16			GUI_Sidebar_SelectedTroops_Count();
	void			GUI_Sidebar_Setup( );
	void			sub_2FA05();
	void			GUI_Sidebar_MapButton_Prepare();
	void			GUI_Sidebar_Squad0_Prepare( );
	void			GUI_Sidebar_Squad1_Prepare( );
	void			GUI_Sidebar_Squad2_Prepare( );

	void			sub_2FA8F();
	void			GUI_Sidebar_Rockets_Draw( );

	void			GUI_Prepare_Button_Rockets();
	void			GUI_Handle_Button_Rockets();

	void			GUI_Prepare_Button_TroopName();
	void			GUI_Handle_Button_TroopName();
	
	void			Squad_Select_Grenades();
	void			Squad_Select_Rockets();

	int16			sub_2FF41();

	void			Squad_Split_Assets();

	void			GUI_Sidebar_MapButton_RenderWrapper();
	void			GUI_Sidebar_Number_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );
	void			GUI_Sidebar_MapButton_Render();
	void			GUI_Handle_Button_ShowOverview();
	void			sub_3037A( );
	void			sub_3049B( );
	void			GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper( );
	void			sub_30AB0();
	int16			Mouse_Button_Left_Toggled();
	void			sub_30E49();
	void			Squad_Clear_Selected();
	void			sub_303AE();
	void			sub_303B7();
	void			sub_303DA();
	void			sub_30465();
	void			sub_30480();
	void			Squad_Switch_Weapon();
	void			Mission_Final_TimeToDie();
	int16			sub_305D5( sSprite*& pData20 );
	void			Mouse_Inputs_Check();
	void			Squad_Member_Target_Set();
	int16			Mouse_Button_Right_Toggled();
	void			Squad_Switch_Timer();
	void			Squad_Switch_To( int16 pData0 );
	void			Vehicle_Input_Handle();
	void			sub_311A7();
	int16			sub_313CD();
	void			Vehicle_Target_Set();

	void			String_CalculateWidth( int32 pPosX, const uint8* pWidths, const std::string& pString );
	void			String_CalculateWidth( int32 pPosX, const uint8* pWidths, const char* pString );

	void			String_Print(  const uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const char* pText );
	void			String_Print( const uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const std::string& pText );

	std::string		map_Filename_MapGet();
	std::string		map_Filename_SptGet();

	void			intro_LegionMessage();
	int16			introPlayText();
	void			intro();
	void			intro_Music_Play();

	void			map_ClearSpt();
	void			Mission_Phase_Next();

	void			sub_12AB1();

	void			WonGame();

	void			Load_SetupData( const std::string& pFilename );
	
	void			Video_Sleep();

	void			sleepLoop( int64 pMilliseconds );
	int16			ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration );

	void			memory_XMS_Detect();
	void			Mouse_Setup();
	void			Mouse_Inputs_Get();
	void			Mouse_ButtonCheck();
	void			Mouse_GetData();

	void			eventProcess();
	void			keyProcess( uint8 pKeyCode, bool pPressed );

	void			Game_Setup( int16 pStartMap );

public:

					cFodder( bool pSkipIntro = false );
	virtual			~cFodder();

	void			Sprite_SetDataPtrToBase( const sSpriteSheet** pSpriteSheet );

	void			String_Print_Small( std::string pText, const uint16 pY );
	void			String_Print_Large( std::string pText, const bool pOverAndUnderLine, const uint16 pY );

	bool			EventAdd( cEvent pEvent );
	void			Prepare();

	virtual void	Start( int16 pStartMap );
	void			Exit( unsigned int pExitCode );

	void			WindowTitleSet( bool pInMission );
	void			WindowTitleBaseSetup();

	void			VersionCleanup();
	void			VersionLoad( const sVersion* pVersion );
	void			VersionSelect();
	void			VersionSelect_0();
	void			VersionSelect_1();
	void			VersionSelect_2();
	void			VersionSelect_3();
	void			VersionSelect_4();

};
