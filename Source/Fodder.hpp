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

#include "Sprites.hpp"
#include "GameData.hpp"

#define INVALID_SPRITE_PTR (sSprite*) -1


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

struct sMapPosition {
    int16   mX;
    int16   mY;

    sMapPosition(const int16& pX = 0, const int16& pY = 0) { mX = pX; mY = pY; }

    bool operator==(const sMapPosition& pPos) {
        return mX == pPos.mX && mY == pPos.mY;
    }
};

union sMapTarget {
    int32 asInt;

    struct {
        int16   mX;
        int16   mY;
    };
};

struct sSavedGame {
    std::string mFileName;
    std::string	mName;
};

struct sService_Draw {
    int16 mSpriteType;
    int16 mFrame;
    int16 mX;
    int16 mY;

    sService_Draw(int16 pSpriteType, int16 pFrame, int16 pX, int16 pY) {
        mSpriteType = pSpriteType;
        mFrame = pFrame; 
        mX = pX; 
        mY = pY; 
    }
};

extern const sSpriteSheet_pstuff mSpriteSheet_PStuff[209];
extern const int16 mMap_Direction_Calculations[256];
extern const int8 mMap_Distance_Calculations[1025];
extern const int16 mMap_DirectionsBetweenPoints[];

class cFodder {
public:
    std::string             mInput;

    bool					mOpenFodder_Intro_Done;
    bool                    mSoundDisabled;

    bool                    mVersionPlatformSwitchDisabled;
    const sGameVersion*     mVersionDefault;		// Version to switch back to when looking for data
    const sGameVersion*     mVersionCurrent;		// Version currently being used
    bool                    mVersionReturnAfterPhase;   // Return to default data after phase

	std::shared_ptr<sFodderParameters> mStartParams;
	std::shared_ptr<sFodderParameters> mParams;

    std::shared_ptr<cGraphics>  mGraphics;
    std::shared_ptr<cSound>     mSound;
    std::shared_ptr<cResources> mResources;
    std::shared_ptr<cWindow>    mWindow;
    std::shared_ptr<cVersions>  mVersions;

    sGameData				mGame_Data;
    sGameData               mGame_Data_Backup;
    
	cPseudorand				mRandom;
    sSprite                 mSprite_Spare;
    std::vector<sSprite>    mSprites;

    const sSpriteSheet**    mSprite_SheetPtr;

    void                    (cFodder::*mSprite_Function[118])(sSprite* pSprite);
	std::string				mSprite_Names[118];

    std::stringstream       mWindowTitle;

    std::string     mFilenameCopt;
    std::string     mFilenameBaseSwp;
    std::string     mFilenameBaseHit;
    std::string     mFilenameBaseBht;
    std::string     mFilenameArmy;
    std::string     mFilenameSubSwp;
    std::string     mFilenameSubHit;
    std::string     mFilenameSubBht;
    std::string     mFilenameBasePal;

    cSurface*       mSurfaceMapOverview;
    cSurface*       mSurface;
    cSurface*       mSurface2;
    int32           mSurfaceMapTop, mSurfaceMapLeft;

    uint8           mKeyCode;

    tSharedBuffer   mMap;
	std::shared_ptr<cMap> mMapLoaded;

    tSharedBuffer   mTile_BaseBlk;
    tSharedBuffer   mTile_SubBlk;

    bool            mInput_Enabled;
    uint16          mGame_InputTicks;

    int16           mButtonPressLeft, mButtonPressRight;

    bool            mVehicle_Input_Disabled;





    int16           mMouse_Button_Left_Toggle;
    int16           mMouse_Button_Right_Toggle;
    bool            mMouse_Button_LeftRight_Toggle;
    bool            mMouse_Button_LeftRight_Toggle2;

    int16           mMouse_Exit_Loop;
    cPosition       mMouse_EventLastPosition;
    uint32          mMouse_EventLastButtonsPressed;
    cPosition       mMouse_EventLastWheel;
    bool            mMouse_Locked;

    bool            mSquad_Member_Fire_CoolDown_Override;


    int32           dword_39F36;

    int16           mMapTile_DrawX;
    int16           mMapTile_DrawY;
    int16           mMapTile_MoveDirectionX;
    int16           mMapTile_MoveDirectionY;
    int16           mMapTile_MoveDirectionX_Previous;
    int16           mMapTile_MoveDirectionY_Previous;
    int32           mMapTile_SpeedX;
    int32           mMapTile_SpeedY;
    int16           mMapTile_SpeedX_Previous;
    int16           mMapTile_SpeedY_Previous;

    int32           mMapTile_TargetX;
    int32           mMapTile_TargetY;

    int16           mCamera_Panning_ToTarget;
    int32           mCamera_AccelerationX;
    int32           mCamera_AccelerationY;
    int16           mCamera_MoveDirectionX;
    int16           mCamera_MoveDirectionY;
    int16           mCamera_MovePauseX;
    int16           mCamera_MovePauseY;
    int16           mCamera_PanTargetX;
    int16           mCamera_PanTargetY;
    int16           mCamera_Scroll_Speed;
    bool            mCamera_Speed_Reset_X;
    bool            mCamera_Speed_Reset_Y;
    int32           mCamera_Speed_X;
    int32           mCamera_Speed_Y;
    int16           mCamera_StartPosition_X;
    int16           mCamera_StartPosition_Y;
    int16           mCamera_SquadLeaderX;
    int16           mCamera_SquadLeaderY;
    int32           mCamera_TileSpeed_Overflow;
    int16           mCamera_TileX;
    int16           mCamera_TileY;
    int32           mCamera_TileSpeedX;
    int32           mCamera_TileSpeedY;

    int16           mKeyCodeAscii;

    sSprite*        mSquad_Leader;
    int16           mSquad_Selected;
    int16           mSquad_JoiningTo;

    int16           mSprite_Frame_1;
    int16           mSprite_Frame_Modifier_Update_Countdown;
    int16           mSprite_Frame_2;
    int16           mSprite_Frame3_ChangeCount;
    int16           mSprite_Frame_3;

    bool            mTroop_Weapon_Grenade_Disabled;
    bool            mTroop_Weapon_Bullet_Disabled;
    bool            mTroop_Weapon_Rocket_Disabled;

    uint16          mSidebar_SmallMode;
    int32           dword_3A030;

    int8            mSquads_TroopCount[4];
    uint16          word_3A05F;

    std::vector<sSprite*> mSprite_DrawList_First;
    std::vector<sSprite*> mSprite_DrawList_Second;
    std::vector<sSprite*> mSprite_DrawList_Third;
    std::vector<sSprite*> mSprite_DrawList_Final;

    std::vector<int16> mMap_PathToDest;

   
    int32           mStoredSpriteX;
    int32          mStoredSpriteY;
    int16           mTmp_FrameNumber;

    int16           mSidebar_Draw_Y;
    int16           word_3A3BF;

    int16           mDirectionMod;

    sMapPosition    mCheckPattern_Position;

    bool            mPhase_Aborted2;
    bool            mPhase_Aborted;
    bool            mPhase_EscapeKeyAbort;

    bool            mPhase_Complete;
    int16           mPhase_Completed_Timer;
    bool            mPhase_In_Progress;
    bool            mPhase_Paused;
    bool            mPhase_TryAgain;
    bool            mPhase_TryingAgain;

	bool            mPhase_Finished;
	int16           mPhase_ShowMapOverview;

    int16           mEnemy_BuildingCount;
    int16           mSquad_SwitchWeapon;
    int16           word_3A9B8;
    int16           mSquad_Walk_Target_Indexes[3];  // values here, seem to be a byte count originally.. now its an index
    int16           mSquad_Walk_Target_Steps[3];
    int16           mSprite_Bumped_Into_SquadMember;
    int16           mSprite_Player_CheckWeapon;

    int8            mSprite_Projectile_Counters[4];
    int8            mSprite_Missile_Projectile_Counters[4];

    int32           mCamera_Speed_Max;

    int16           mSprites_Found_Count;

    int16           mSquad_Grenades[3];
    int16           mSquad_Rockets[3];

    int16           mGUI_Squad_Icon[3];

	size_t           mTroops_Enemy_Count;
    int16           mHostage_Count;
    sSprite*        mHostage_Rescue_Tent;

    bool            mCamera_Start_Adjust;
    int16           word_3AA1D;             // 2 = Use mSprite_Frame_3
    int16           mCamera_Reached_Target;
    int16           word_3AA21;
    int16           mSprite_FaceWeaponTarget;
    int16           word_3AA45;
    int16           mSquad_Select_Timer;
    int16           mSprite_Find_Distance;

    int16           mSprite_Reached_Target;
    int16           mStoredSpriteFrame;
    int16           word_3ABB1;
    int16           mSquad_Member_Fire_CoolDown;
    size_t          mTroop_Rotate_Next;
    int16           mPhase_MapIs17x12;
    sWeaponData     mSprite_Weapon_Data;
    int16           mSprite_Bullet_Time_Modifier;
    int16           mSprite_Bullet_Fire_Speed_Modifier;
    int16           mSquad_Member_Clicked_TroopInSameSquad;
    sMission_Troop* mSquad_Member_Clicked_TroopPtr;

    int16           mString_GapCharID;      // Replace spaces in strings with this id

    int16           mGUI_Loop_Squad_Current;
    int16           mGUI_Loop_Draw_Y;
    int16           word_3AC2D[3];
    int16           mGUI_Print_String_To_Sidebar;
    int16           mGUI_Squad_NextDraw_Y;
    int16           mGUI_Sidebar_Setup;
    int8            mGUI_RefreshSquadGrenades[3];
    int8            mGUI_RefreshSquadRockets[3];
    int16           mSquad_CurrentWeapon[3];

    int16           mGUI_Loop_Is_CurrentSquad;
    int16           word_3AC4B;
    int16           word_3AC4D;
    int16           word_3AC51;
    sGUI_Element    mGUI_Elements[42];
    sGUI_Element*   mGUI_NextFreeElement;
    int16           mGUI_TroopName_DrawOffset;

    int16           mMap_Destroy_Tile_LastTile;
    int16           mMap_Destroy_Tile_X;
    int16           mMap_Destroy_Tile_Y;
    int16           mMap_Destroy_Tiles_Countdown;
    std::vector<sMapPosition> mMap_Destroy_Tiles;

    int16           mSprite_SpareUsed;
    int16           mSprite_SpareUsed2;
    int16           mSquad_WalkTargetX;
    int16           mSquad_WalkTargetY;
    int16           mSprite_Bullet_Destroy;
    int16           mSprite_Helicopter_DestroyLight;
    int16           mSprite_DistanceTo_Squad0;
    int16           mSprite_Tank_SpriteX;
    int16           mSprite_Tank_SpriteY;
    int16           mSprite_Tank_Squad0_X;
    int16           mSprite_Tank_DistanceTo_Squad0;
    int16           mSprite_Missile_LaunchDistance_X;
    int16           mSprite_Missile_LaunchDistance_Y;
    int16           mSprite_Bullet_Deviate_Counter;
    int32           dword_3B1CB;
			
    int16           mMission_EngineTicks;
    bool            mRecruit_Mission_Restarting;
    sSprite*        mMission_Troops_SpritePtrs[9];

    int16           mMission_Final_TimeToDie_Ticker;
    int16           mMission_Final_TimeRemain;
    int16           mMission_Final_TimeToAbort;
    int16           mMission_Save_Blocked[0x18];

    sSprite*        mSquad_CurrentVehicles[3];
    sSprite*        mSquad_CurrentVehicle;

    bool           mSprite_HumanVehicles_Found;
    std::vector<sSprite*>        mSprites_HumanVehicles;
    int16           dword_3B24B;

    int16           word_3B25B;
    int16           word_3B25D;
    int16           mGUI_SaveLoadAction;
    int16           word_3B2CF;
    int16           word_3B2D1[6];
    int16           mSprite_Find_Types[5];
    int16           word_3B2ED;
    int16           mSpawnSpriteType;
    int16           word_3B2F1;
    int16           word_3B2F3;
    int16           mSprite_Field10_Saved;
    int16           word_3B2F7;

    int16           mGUI_Temp_X;
    int16           mGUI_Temp_Width;

    int16           mGUI_Temp_Y;
    int16           mGUI_Draw_LastHeight;

    void            (cFodder::*mGUI_Select_File_String_Input_Callback)(int16 pPosY);

    int16			mGUI_Select_File_ShownItems;
    int16           mGUI_Select_File_CurrentIndex;
    int16           mGUI_Select_File_Count;
    int16           mGUI_Select_File_SelectedFileIndex;

    int16           mIntro_PlayTextDuration;
    int16           mSoundEffectToPlay_Set;
    int16           mSoundEffectToPlay;

    int16           mSquad_EnteredVehicleTimer[3];
    sSprite*        mSprite_OpenCloseDoor_Ptr;
    int16           mSprite_Civilian_GotHome;
    bool            mSwitchesActivated;
    int16           mSprite_Civilian_Tmp_X;
    int16           mSprite_Civilian_Tmp_Y;
    int16           word_3B481;
    int16           word_3B483;
    int16           mHelicopterCallPadCount;
    int16           mHelicopterCallPadPressedCount;
    sSprite*        mSprite_TroopsAlive[16];
    int16           mHelicopterCall_X;
    int16           mHelicopterCall_Y;
    sSprite*        mTroop_InRange_Callpad;

    int16           mGUI_Sidebar_MapButton_Prepared;

    int16           mTurretFires_HomingMissile;
    int16           word_3B4ED[2];

    int16           mImage_Aborted;
    int16           mBriefing_Aborted;

    int32           mMapTile_Ptr;				// Tile in the top left corner of the screen (under the sidebar)
    int16           mMapTile_ColumnOffset;
    int16           mMapTile_RowOffset;
    int16           mMapTile_MovedHorizontal;
    int16           mMapTile_MovedVertical;

	size_t			mSidebar_Buffer_Size;
    uint16*         mSidebar_Screen_Buffer;
    uint16*         mSidebar_Back_Buffer;

    int16               mRecruit_Button_Load_Pressed;
    int16               mRecruit_Button_Save_Pressed;
    int16               mRecruit_Hill_Positions_Use[0x0F];

    std::vector<sRecruitRendered> mRecruit_Rendereds;
    std::vector<sRecruitRendered>::iterator mRecruit_RenderedNext;

    bool                mRecruit_Screen_Active;
    int16               mRecruit_Sidebar_Draw_Y_Start;
    int16               mRecruit_Truck_Animation_Play;
    int16               mRecruit_Truck_Troops_ToEnter_Count;
    int16               mRecruit_Truck_Frame;
    int16               mRecruit_Truck_FrameAnimBufferPtr;
    uint16              mRecruit_Truck_FramesPlay[100]; // This probably doesn't need to be this big
    int16               mRecruit_Truck_Reached;
    int16               mRecruit_Render_Name_SmallGap;

    int16           mMouseX;
    int16           mMouseY;
    int16           mMouseX_Offset;
    int16           mMouseY_Offset;

    int32           mCameraX;
    int32           mCameraY;

    int16           mMouseCursor_Enabled;

    int16           mMouseSpriteNew;
    int16           mMouseSetToCursor;
    int16           mMouseDisabled;

    uint16          mSquad_Grenade_SplitMode;
    uint16          mSquad_Rocket_SplitMode;
    uint16          mGUI_Sidebar_TroopList_Name_BreakOnSpace;

    sSprite*        mSquad_0_Sprites[9];
    sSprite*        mSquad_1_Sprites[9];
    sSprite*        mSquad_2_Sprites[9];
    sSprite*        mSquad_3_Sprites[9];
    sSprite*        mSquad_4_Sprites[9];

    sSprite**       mSquads[5];
    int16           mSprite_Frame1_Modifier;
    int16           mSprite_Frame2_Modifier;
    int16           mSprite_Frame_3_Modifier;
    int16           mGUI_Mouse_Modifier_X;
    int16           mGUI_Mouse_Modifier_Y;

    uint16          word_3BED5[5];

    sMapTarget      mSquad_WalkTargets[10][30];
    int8            mSquad_Join_TargetSquad[3];
    sSprite*        mSquad_Join_TargetSprite[6];

    bool            mIntroDone;

    int16           mTile_Hit[512];
    int8	        mTile_BHit[512][8];
    int16           mTile_Destroy_Swap[400];

    int16           mMapTile_Column_CurrentScreen;
    int16           mMapTile_Row_CurrentScreen;
    uint16*         mSidebar_Screen_BufferPtr;

    sGUI_Element    mSidebar_OverlayButtons[2];

    int16           mBriefing_Render_1_Mode;

    int16           mDebug_PhaseSkip;
    int16           mKeyControlPressed;

    int16           mMouseSpriteCurrent;
    int16           mService_ExitLoop;

    uint8*          mVideo_Draw_FrameDataPtr;
    int16           mVideo_Draw_PosX;
    int16           mVideo_Draw_PosY;
    int16           mVideo_Draw_Columns;
    int16           mVideo_Draw_Rows;
    uint8           mVideo_Draw_PaletteIndex;
    size_t          mDraw_Source_SkipPixelsPerRow;
    size_t          mDraw_Dest_SkipPixelsPerRow;
    uint16          mVideo_Draw_ColumnsMax;


    int16           word_428B6;
    int16           word_428B8;
    uint16          word_428BA;

    uint32          mHelicopterPosX;
    uint32          mHelicopterPosY;
    int16           mBriefing_Helicopter_Off1;
    int16           mBriefing_Helicopter_Off2;
    int16           mBriefing_Helicopter_Off3;
    const int16*    mBriefing_Helicopter_Off4;
    uint16          mBriefing_ParaHeli_Frame;
    int16           mBriefing_Helicopter_Moving;
    int16           word_428D8;

    int16           mMouseButtonStatus;
    int16           mInputMouseX;
    int16           mInputMouseY;

    int16           mService_Promotion_Exit_Loop;
    std::vector<sService_Draw> mService_Draw_List;

    int16           byte_44AC0;
    int16           mInput_LastKey;

    int16           mDemo_ExitMenu;
    int16           mCustom_ExitMenu;

    eCustomMode     mCustom_Mode;

    int64           mTicks;
    int64           mTicksDiff;
    std::vector<std::string>	mCampaignList;

    void(cFodder::*mGUI_Handle_Button_SelectSquad_Array[3])(void);

public:

    bool            Demo_Amiga_ShowMenu();

    void            Custom_ShowMapSelection();
    void            Campaign_Selection();

    void			Campaign_Select_Sprite_Prepare();

	void			Campaign_Select_Setup();

	std::string 	Campaign_Select_File(const char* pTitle, const char* pSubTitle, const char* pPath, const char* pType, eDataType pData = eData);
	void            Campaign_Select_File_Cycle(const char* pTitle, const char* pSubTitle);
    void            Campaign_Select_File_Loop(const char* pTitle, const char* pSubTitle);
    void			Campaign_Select_DrawMenu(const char* pTitle, const char* pSubTitle);

    void            Image_FadeIn();
    void            Image_FadeOut();

	bool			Engine_Loop();

    virtual int16   Mission_Loop();
	virtual int16	Phase_Cycle();
    virtual int16   Phase_Loop();
	void			Phase_Prepare();

    void            Game_Handle();
    void            Camera_Handle();
    void            Camera_PanTarget_AdjustToward_SquadLeader();

    void            GameData_Reset();
    void            GameData_Backup();
    void            GameData_Restore();

    void            Phase_EngineReset();
    void            Phase_SquadPrepare();
    void            Squad_Set_Squad_Leader();

    void            Sprite_Clear_All();

    // Campaign
    bool            Campaign_Load(std::string pName);

    // Map Functions
    void            Map_Create(sMapParams pParams);
    void            Map_Load();
    void            Map_Load_Sprites();
    void			Map_Load_Sprites_Count();
    void            Map_Load_Resources();

    void			Map_Add_Structure(const sStructure& pStructure, int16 pTileX, int16 pTileY);

    bool			Tiles_Load_Data();
    int16			Tile_FindType(const eTerrainFeature pType);
    std::vector<int16> Tile_FindType(const eTerrainFeature pType, const eTerrainFeature pType2);

    void            Music_Play_Tileset();

    // Mission Functions
    void            Phase_Soldiers_Count();
    void            Phase_Soldiers_Prepare(const bool pPrebriefing);
    void            Mission_Troop_Prepare_Next_Recruits();
    void            Phase_Soldiers_AttachToSprites();
    void            Mission_Troops_Clear_Selected();

    void            Camera_Speed_Update_From_PanTarget();
    int16           sub_119E1(int16& pX1, int16& pY1, int16 pX2, int16 pY2);

    void            Camera_Speed_Calculate();
    void            Camera_Prepare();
    void            Camera_SetTargetToStartPosition();
    void            Camera_Pan_To_Target();
    void            Camera_Pan_Set_Speed();
    void            Camera_Update_Mouse_Position_For_Pan();

    void            MapTile_UpdateFromCamera();
    void            Camera_Reset();
    void            Camera_TileSpeedX_Set();
    void            Camera_TileSpeedY_Set();
    void            Camera_Speed_MaxSet();
    void            Camera_Speed_Reset();
    void            Camera_Acceleration_Set();

    void            Sprite_Sort_DrawList();
    void            Sprite_Bullet_SetData();

    void            Mission_Final_Timer();

    void            Phase_Goals_Check();
    void            Phase_Goals_Set();
    void            Phase_Progress_Check();

    void            Phase_Map_Overview_Show();

    void            Mission_Set_Final_TimeRemaining();
    void            Mission_Sprites_Handle();

    void            Phase_GameOver();
    void            Phase_Paused();

    void            Phase_Show_Complete();
    void            Phase_Show_TryAgain();
    void            Phase_TextSprite_Prepare(sSprite* pData2C);
    void            Phase_TextSprite_Create_Mission(sSprite* pData2C);
    void            Phase_TextSprite_Create_Phase(sSprite* pData2C);
    void            Phase_TextSprite_Create_Complete(sSprite* pData2C);
    void            Phase_TextSprite_Create_Try(sSprite* pData2C);
    void            Phase_TextSprite_Create_Again(sSprite* pData2C);
    void            Phase_TextSprite_Create_GameOver(sSprite* pData2C);

    void            Squad_Member_PhaseCount();
    void            Squad_Set_CurrentVehicle();
    void            Squad_EnteredVehicle_TimerTick();

    void            Map_Overview_Prepare();

    void            Sprite_Find_HumanVehicles();
    void            Sprite_Count_HelicopterCallPads();

    void            Sprite_HelicopterCallPad_Check();

    int16           Sprite_Create_RandomExplosion();

    void            GUI_Draw_Frame_8(int32 pSpriteType, int32 pFrame, const size_t pPositionX, const size_t pPositionY);
    void            GUI_Draw_Frame_16(int16 pSpriteType, int16 pFrame, const size_t pPosX, const size_t pPosY);
    void            Sprite_Draw_Frame(sSprite* pDi, int16 pSpriteType, int16 pFrame, cSurface *pDestination = 0);

    bool            Sprite_OnScreen_Check();
    void            Sprites_Draw();
    void            Sprite_Map_Sound_Play(int16& pData0);

    void            Sound_Play(sSprite* pSprite, int16 pSoundEffect, int16 pData8);

    // 14EAC
    void			Mission_Intro_Draw_OpenFodder();
    void            Mission_Intro_Helicopter_Start();
	void            Mission_Intro_Draw_Mission_Name();

    void            sub_1594F();

    void            CopyProtection();
    void            CopyProtection_EncodeInput();

    /* Recruitment */
    int16           Recruit_Show();
    void            Recruit_Prepare();
    bool            Recruit_Loop();
    void            Recruit_Draw_String(int32 pParam0, size_t pParam8, size_t pParamC, const std::string& pString);
    void            Recruit_Truck_Anim_Prepare();
    void            Recruit_Truck_Anim_CopyFrames(uint16** pDi, const int16* pSource);
    void            Recruit_Render_Sidebar();
    void            Recruit_Render_Squad_Names();
    void            Recruit_Render_Squad_RankKills();
    void            Recruit_Render_Number(int16 pNumber, int16 pData10);
    void            Recruit_Render_HeroList();
    void            Recruit_Render_Names_UnusedSlots();
    void            Recruit_Sidebar_Render_SquadName();
    void            Recruit_Update_Actors();
    void            sub_175C0();
    void            Recruit_Update_Soldiers();
    void            Recruit_Prepare_Anims();
    void            Recruit_Frame_Check();
    void            Recruit_Position_Troops();
    void            Recruit_Update_Truck();
    void            Recruit_Copy_Sprites();
    void            Recruit_Cycle();
    void            Recruit_Draw_Graves();
    void            Recruit_Draw_Grave(int16 pSpriteType, const size_t pPosX, const size_t pPosY);
    bool            Recruit_Check_Buttons_SaveLoad();
    void            GUI_Render_Text_Centred(const char* pText, const size_t pPosY);
    /* End Recruitment */

    /* Promotion / Heroes */
    void            Service_Show();
    void            Service_KIA_Loop();
    void            Service_Promotion_Loop();
    int16           Service_KIA_Troop_Prepare();
    int16           Service_Promotion_Prepare_Draw();
    void            Service_Draw_Troop_And_Rank(int16 pRecruitID, int16 pRank);
    void            Service_Draw_String(const std::string& pText, const bool pLarge, const size_t pY);
    void            Service_Draw_List();
    void            Service_ScrollUp_DrawList();
    void            Service_Draw_String(const std::string& pText, const uint8* pData28, int16 pData0, int16 pData8, int16 pDataC);
    int16           Service_Sprite_Draw(int16& pSpriteType, int16& pFrame, int16& pX, int16& pY);
    int16           Service_Sprite_OnScreen_Check();
    void            Service_Mission_Text_Prepare();
    void            Service_Promotion_Check();

    /* End Promotion / Heroes */

    int16           Squad_Join(sSprite* pSprite);


	void            Briefing_Update_Helicopter();
	void            Briefing_Draw_Mission_Title(int16 pDrawAtY);

    void            Briefing_Draw_Phase();
    void            Briefing_Draw_With();
    void            Briefing_DrawBox(int16 pX, int16 pY, int16 pWidth, int16 pHeight, uint8 pColor);
    void            Briefing_Draw_Horizontal_Line(int16 pX, int16 pWidth, int16 pY, uint8 pColor);
    void            Briefing_Draw_Vertical_Line(int16 pX, int16 pHeight, int16 pY, uint8 pColor);

    void            Briefing_Draw_Pixel(int16 pX, int16 pY, uint8 pColor);

	int16			Briefing_Show();
	void            Briefing_Show_PreReady();
	void            Briefing_Show_Ready();

    void			Mission_Intro_Play(const bool pShowHelicopter = false, eTileTypes pTileset = eTileTypes_Jungle);
    void            Intro_Print_String(const sIntroString* pString);

    void            Sprite_Frame_Modifier_Update();

    void            Sprite_Table_Setup();

    void            Sprite_Handle_Loop();
    void            Sprite_Handle_Null(sSprite* pSprite);

    void            Sprite_Handle_Player(sSprite* pSprite);               // 0
    void            Sprite_Handle_Grenade(sSprite* pSprite);              // 2
    void            Sprite_Projectile_Counters_Decrease(sSprite* pSprite);
    void            Sprite_Handle_ShadowSmall(sSprite* pSprite);          // 3
    void            Sprite_Handle_Enemy(sSprite* pSprite);                // 5
    void            Sprite_Handle_Bullet(sSprite* pSprite);               // 6
    void            Sprite_Handle_Helicopter(sSprite* pSprite);
    void            Sprite_Handle_Explosion(sSprite* pSprite);            // 12
    void            Sprite_Handle_Shrub(sSprite* pSprite);                // 13
    void            Sprite_Handle_Tree(sSprite* pSprite);                 // 14
    void            Sprite_Handle_BuildingRoof(sSprite* pSprite);         // 15
    void            Sprite_Handle_Snowman(sSprite* pSprite);              // 16
    void            Sprite_Handle_Shrub2(sSprite* pSprite);               // 17
    void            Sprite_Handle_Waterfall(sSprite* pSprite);            // 18
    void            Sprite_Handle_Bird2_Left(sSprite* pSprite);           // 19

    void            Sprite_Handle_BuildingDoor(sSprite* pSprite);                         // 20
    void            Sprite_Handle_Player_Rank(sSprite* pSprite);                          // 21
    void            Sprite_Handle_Player_Shadow(sSprite* pSprite);                        // 22
    void            Sprite_Handle_BloodTrail(sSprite* pSprite);                           // 23
    void            Sprite_Handle_GroundHole(sSprite* pSprite);                           // 24
    void            Sprite_Handle_BuildingDoor2(sSprite* pSprite);                        // 25
    void            Sprite_Handle_Floating_Dead_Soldier(sSprite* pSprite);                // 27
    void            Sprite_Handle_Text_Complete(sSprite* pSprite);                        // 28
    void            Sprite_Handle_Text_Mission(sSprite* pSprite);                         // 29

    void            Sprite_Handle_Text_Phase(sSprite* pSprite);                           // 30
    void            Sprite_Handle_Vehicle(sSprite* pSprite);
    void            Sprite_Handle_Rocket(sSprite* pSprite);                               // 33
    void            Sprite_Handle_Text_GameOver(sSprite* pSprite);                        // 34
    void            Sprite_Handle_Shadow(sSprite* pSprite);                               // 35
    void            Sprite_Handle_Enemy_Rocket(sSprite* pSprite);                         // 36
    void            Sprite_Handle_GrenadeBox(sSprite* pSprite);                           // 37
    void            Sprite_Handle_RocketBox(sSprite* pSprite);                            // 38
    void            Sprite_Handle_Building_Explosion(sSprite* pSprite);                   // 39

    void            Sprite_Handle_Helicopter_Grenade_Enemy(sSprite* pSprite);             // 40
    void            Sprite_Handle_Flashing_Light(sSprite* pSprite);                       // 41
    void            Sprite_Handle_Helicopter_Grenade2_Enemy(sSprite* pSprite);            // 42
    void            Sprite_Handle_Helicopter_Missile_Enemy(sSprite* pSprite);             // 43
    void            Sprite_Handle_Helicopter_Homing_Enemy(sSprite* pSprite);              // 44
    void            Sprite_Handle_Missile(sSprite* pSprite);                              // 45
    void            Sprite_Handle_MissileHoming(sSprite* pSprite);                        // 46
    void            Sprite_Handle_Sparks(sSprite* pSprite);                                           // 47
    void            Sprite_Handle_FireTrail(sSprite* pSprite);                                            // 48
    void            Sprite_Handle_Helicopter_Grenade2_Human(sSprite* pSprite);            // 49

    void            Sprite_Handle_Helicopter_Grenade_Human(sSprite* pSprite);             // 50
    void            Sprite_Handle_Helicopter_Missile_Human(sSprite* pSprite);             // 51
    void            Sprite_Handle_Helicopter_Homing_Human(sSprite* pSprite);              // 52
    void            Sprite_Handle_Helicopter_PropCrash(sSprite* pSprite);                 // 53
    void            Sprite_Handle_Mine(sSprite* pSprite);                                 // 54
    void            Sprite_Handle_Mine2(sSprite* pSprite);                                // 55
    void            Sprite_Handle_Spike(sSprite* pSprite);                                // 56
    void            Sprite_Handle_Smoke(sSprite* pSprite);                                            // 57
    void            Sprite_Handle_Text_Try(sSprite* pSprite);                             // 58
    void            Sprite_Handle_Text_Again(sSprite* pSprite);                           // 59

    void            Sprite_Handle_BoilingPot(sSprite* pSprite);           // 60
    void            Sprite_Handle_Civilian(sSprite* pSprite);           // 61
    void            Sprite_Handle_Civilian2(sSprite* pSprite);          // 62
    void            Sprite_Handle_VehicleNoGun_Human(sSprite* pSprite);   // 63
    void            Sprite_Handle_VehicleGun_Human(sSprite* pSprite);     // 64
    void            Sprite_Handle_Tank_Human(sSprite* pSprite);           // 65
    void            Sprite_Handle_Bird_Left(sSprite* pSprite);            // 66
    void            Sprite_Handle_Bird_Right(sSprite* pSprite);           // 67
    void            Sprite_Handle_Seal(sSprite* pSprite);                 // 68
    void            Sprite_Handle_Tank_Enemy(sSprite* pSprite);           // 69

    void            Sprite_Handle_Civilian_Spear(sSprite* pSprite);     // 70
    void            Sprite_Handle_Civilian_Spear2(sSprite* pSprite);    // 71
    void            Sprite_Handle_Hostage(sSprite* pSprite);              // 72
    void            Sprite_Handle_Hostage_Rescue_Tent(sSprite* pSprite);  // 73
    void            Sprite_Handle_Door_Civilian(sSprite* pSprite);      // 74
    void            Sprite_Handle_Door2_Civilian(sSprite* pSprite);     // 75
    void            Sprite_Handle_Door_Civilian_Spear(sSprite* pSprite);// 76
    void            Sprite_Handle_Cannon(sSprite* pSprite);               // 77
    void            Sprite_Handle_Turret_Missile_Human(sSprite* pSprite); // 78
    void            Sprite_Handle_Turret_Missile2_Human(sSprite* pSprite);// 79

    void            Sprite_Handle_VehicleNoGun_Enemy(sSprite* pSprite);   // 80
    void            Sprite_Handle_VehicleGun_Enemy(sSprite* pSprite);     // 81
    void            Sprite_Handle_Vehicle_Unk_Enemy(sSprite* pSprite);    // 82
    void            Sprite_Handle_Civilian_Invisible(sSprite* pSprite); // 83
    void            Sprite_Handle_Turret_Missile_Enemy(sSprite* pSprite); // 84
    void            Sprite_Handle_Turret_Missile2_Enemy(sSprite* pSprite);// 85
    void            Sprite_Handle_Vehicle_Sinking_1(sSprite* pSprite);    // 86
    void            Sprite_Handle_Vehicle_Sinking_2(sSprite* pSprite);    // 87
    void            Sprite_Handle_BuildingDoor3(sSprite* pSprite);        // 88
    void            Sprite_Handle_Explosion2(sSprite* pSprite);           // 89

    void            Sprite_Handle_Door_Civilian_Rescue(sSprite* pSprite);         // 90
    void            Sprite_Handle_Seal_Mine(sSprite* pSprite);                    // 91
    void            Sprite_Handle_Spider_Mine(sSprite* pSprite);                      // 92
    void            Sprite_Handle_Bonus_RankToGeneral(sSprite* pSprite);              // 93
    void            Sprite_Handle_Bonus_Rockets(sSprite* pSprite);                    // 94
    void            Sprite_Handle_Bonus_Armour(sSprite* pSprite);                    // 95
    void            Sprite_Handle_Bonus_RankHomingInvin_SquadLeader(sSprite* pSprite);            // 96
    void            Sprite_Handle_MissileHoming2(sSprite* pSprite);                   // 97
    void            Sprite_Handle_Bonus_RankHomingInvin_Squad(sSprite* pSprite);      // 98
    void            Sprite_Handle_Helicopter_CallPad(sSprite* pSprite);               // 99

    void            Sprite_Handle_BuildingDoor_Reinforced(sSprite* pSprite);                                      // 100
    void            Sprite_Handle_Helicopter_Grenade2_Human_Called(sSprite* pSprite); // 101
    void            Sprite_Handle_Helicopter_Grenade_Human_Called(sSprite* pSprite);  // 102
    void            Sprite_Handle_Helicopter_Missile_Human_Called(sSprite* pSprite);  // 103
    void            Sprite_Handle_Helicopter_Homing_Human_Called(sSprite* pSprite);   // 104
    void            Sprite_Handle_Turret_HomingMissile_Enemy(sSprite* pSprite);       // 105
    void            Sprite_Handle_Enemy_Leader(sSprite* pSprite);                        // 106
    void            Sprite_Handle_Helicopter_Homing_Enemy2(sSprite* pSprite);         // 107
    void            Sprite_Handle_Computer_1(sSprite* pSprite);                       // 108
    void            Sprite_Handle_Computer_2(sSprite* pSprite);                       // 109
    void            Sprite_Handle_Computer_3(sSprite* pSprite);                       // 110

    // Cannon Fodder 2 Sprites
    void            Sprite_Handle_UFO_Callpad(sSprite* pSprite);                                      // 111
    void            Sprite_Handle_Turret_Cannon_Invulnerable(sSprite* pSprite);                                      // 112
    void            Sprite_Handle_Turret_Missile_Invulnerable(sSprite* pSprite);                                      // 113
    void            Sprite_Handle_Looping_Vehicle_Left(sSprite* pSprite);                                      // 114
    void            Sprite_Handle_Looping_Vehicle_Right(sSprite* pSprite);                                      // 115
    void            Sprite_Handle_Looping_Vehicle_Up(sSprite* pSprite);                                      // 116
    void            Sprite_Handle_Looping_Vehicle_Down(sSprite* pSprite);                                      // 117

    void            Sprite_PositionNext_AdjustByHeight(sSprite* pSprite);
    int16           sub_1D92E(sSprite* pSprite);

    void            Sprite_Native_Sound_Play(sSprite* pSprite, int16 pSoundID);
    int16           Sprite_Handle_Soldier_Animation(sSprite* pSprite);
    virtual int16   Sprite_Troop_Dies(sSprite* pSprite);
    int16           loc_1F043(sSprite* pSprite);
    void            loc_1F092(sSprite* pSprite, sSprite* pData24);

    int16           Sprite_Handle_Player_MissionOver(sSprite* pSprite);
    void            Sprite_Handle_Troop_Weapon(sSprite* pSprite);
    void            Sprite_Handle_Troop_Direct_TowardWeaponTarget(sSprite* pSprite);
    void            Sprite_Handle_Troop_Direct_TowardMouse(sSprite* pSprite);

    void            Sprite_XY_Store(sSprite* pSprite);
    void            Sprite_XY_Restore(sSprite* pSprite);

    void            Sprite_Handle_Player_Adjust_Movement_Speed(sSprite* pSprite);
    void            Sprite_Draw_Row_Update(sSprite* pSprite);
    void            Sprite_Handle_Troop_FrameUnk(sSprite* pSprite);
    void            sub_1FCF2(sSprite* pSprite);
    void            sub_1FDE7(sSprite* pSprite);
    void            Sprite_Create_Player_Shadow(sSprite* pSprite);
    int16           Sprite_Create_BloodTrail(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30);
    void            Sprite_Terrain_Check(sSprite* pSprite, int16& pData4);

    int16           Sprite_Handle_Troop_Get_Frame_For_Direction(sSprite* pSprite);
    void            Sprite_Reached_MapEdge(sSprite* pSprite);

    int16           Sprite_Destroy_Wrapper(sSprite* pSprite);
    int16           Sprite_Destroy(sSprite* pSprite);
    int16           Sprite_Create_Bullet(sSprite* pSprite);
    int16           Sprite_Create_Grenade(sSprite* pSprite);
    void            Sprite_Set_Direction_To_Next(sSprite* pSprite);
    int16           Sprite_Reached_Target(sSprite* pSprite);
    void            Sprite_Movement_Speed_Update(sSprite* pSprite);

    void            Sprite_Handle_Player_Close_To_SquadMember(sSprite* pSprite);
    int16           Sprite_Get_Free_Max42(int16& pData0, sSprite*& pData2C, sSprite*& pData30);
    int16           Sprite_Get_Free_Max29(int16& pData0, sSprite*& pData2C, sSprite*& pData30);

    void            Sprite_Handle_Exploidable(sSprite* pSprite);
    void            Sprite_Create_Shadow(sSprite* pSprite);
    void            Sprite_Handle_Grenade_Terrain_Check(sSprite* pSprite);
    int16           Sprite_Projectile_Collision_Check(sSprite* pSprite);
    void            Sprite_Create_Sparks(sSprite* pSprite, int16 pData18);
    void            Sprite_Create_FireTrail(sSprite* pSprite);
    void            Sprite_Turn_Into_Building_Explosion(sSprite* pSprite);
    int16           Sprite_Create_Building_Explosion_Wrapper(int16& pX, int16& pY);
    int16           Sprite_Create_Building_Explosion(sSprite* pData2C, int16& pX, int16& pY);
    int16           Sprite_Create_Enemy(sSprite* pSprite, sSprite*& pData2C);
    void            Sprite_Enemy_Aggression_Update(sSprite* pData2C);
    void            Sprite_Create_Rank();
    void            sub_21CD1(sSprite* pSprite);

    void            sub_2212A(sSprite* pSprite);
    int16           Sprite_Handle_BuildingDoor_Explode(sSprite* pSprite);
    int16           sub_222A3(sSprite* pSprite);
    void            sub_223B2(sSprite* pSprite);
    int16           Map_Get_Distance_Between_Sprite_And_Squadleader(sSprite* pSprite, int16& pData0);
    int16           Sprite_Create_Rocket(sSprite* pSprite);
    int16           Sprite_Homing_LockInRange(sSprite* pSprite, sSprite*& pFoundSprite);
    void            Sprite_Projectile_HitTarget(sSprite* pSprite);
    void            Sprite_Destroy_Wrapper_At_TopLeft(sSprite* pSprite);
    int16           Sprite_Destroy_Wrapper_2(sSprite* pSprite);
    void            Sprite_Handle_Player_InVehicle(sSprite* pSprite);
    void            Sprite_Handle_Player_Enter_Vehicle(sSprite* pSprite);
    void            sub_22C87(sSprite* pSprite);
    void            sub_22CD7(sSprite* pSprite, int16& Data0, int16& Data4);
    void            Sprite_Handle_Vehicle_Terrain_Check(sSprite* pSprite);
    void            Sprite_Under_Vehicle(sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C);
    int16           Sprite_Animation_SlideOrDie(sSprite* pSprite);
    int16           Map_Sprite_Check_Around_Position(sSprite* pSprite);
    void            Sprite_Handle_Vehicle_Human(sSprite* pSprite);
    int16           Sprite_Create_Smoke(sSprite* pSprite, sSprite*& pData2C);
    void            sub_23525(sSprite* pSprite);
    void            sub_236F7(sSprite* pSprite);
    void            Sprite_Handle_Tank_FireMissile(sSprite* pSprite);
    int16           Sprite_Create_Missile(sSprite* pSprite, sSprite*& pData2C);
    void            Sprite_Enemy_Set_Target(sSprite* pData2C);
    void            Sprite_Handle_Helicopter_Human(sSprite* pSprite);
    int16           Sprite_Handle_Helicopter_Terrain_Check(sSprite* pSprite);
    void            sub_23E01(sSprite* pSprite, int16& pData0, int16& pData4);
    void            Sprites_HumanVehicles_Remove(sSprite* pSprite);
    void            Sprite_Handle_Turret(sSprite* pSprite);
    void            Sprite_Handle_Turret_Fire(sSprite* pSprite, sSprite* pData34);
    int16           Sprite_Find_By_Types(sSprite* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite*& pData28);
    void            sub_243E9(sSprite* pSprite);
    int16           Sprite_Handle_Vehicle_Sinking(sSprite* pSprite);
    void            sub_245BF(sSprite* pSprite);
    int16           Sprite_Create_Cannon(sSprite* pSprite);
    void            Sprite_Handle_Helicopter_Human_Deploy_Weapon(sSprite* pSprite);
    int16           Sprite_Create_Grenade2(sSprite* pSprite);
    int16           Sprite_Create_MissileHoming(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34);
    int16           Sprite_Handle_Helicopter_Enemy_Weapon(sSprite* pSprite, sSprite*& pData30);
    void            Sprite_Handle_Vehicle_Enemy(sSprite* pSprite);
    void            Sprite_Handle_Helicopter_Enemy(sSprite* pSprite);
    void            Sprite_Handle_Civilian_Unk(sSprite* pSprite);
    void            Sprite_Handle_Civilian_Movement(sSprite* pSprite);
    int16           Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(sSprite* pSprite);
    void            Sprite_Handle_Civilian_Death(sSprite* pSprite);
    void            sub_2593D(sSprite* pSprite);
    void            sub_25A31(sSprite* pSprite);
    void            sub_25A66(sSprite* pSprite);
    int16           Sprite_Handle_Civilian_RandomMovement(sSprite* pSprite);
    int16           Sprite_Create_Civilian_Spear2(sSprite* pSprite);
    int16           sub_25DCF(sSprite* pSprite);
    void            sub_25F2B(sSprite* pSprite);
    void            Sprite_Handle_Hostage_Movement(sSprite* pSprite);
    void            Sprite_Handle_Hostage_FrameUpdate2(sSprite* pSprite);
    void            Sprite_Handle_Hostage_FrameUpdate(sSprite* pSprite);
    void            sub_26490(sSprite* pSprite);
    void            sub_264B0(sSprite* pSprite);
    int16           Sprite_Create_Native(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30);
    int16           Sprite_Handle_Helicopter_Callpad_InRange(sSprite* pSprite, sSprite*& pData2C);
    void            Sprite_Handle_Helicopter_Human_CallCheck(sSprite* pSprite);
    void            Sprite_Handle_Computer(sSprite* pSprite, int16 pData1C);

    int16           Map_Get_Distance_BetweenSprites_Within_Window(const sSprite* pSprite, const sSprite* pSprite2);
    int16           Map_Get_Distance_BetweenPoints_Within_Window(int16& pX, int16 pY, int16& pX2, int16& pY2);
    int16           Direction_Between_Points(int16& pData0, int16& pData4, int16& pData8, int16& pDataC);

	int16			map_GetRandomX();
	int16			map_GetRandomY();

    int16           tool_RandomGet();
    uint16          tool_RandomGet(size_t pMin, size_t pMax);

    void            Sprite_Movement_Calculate(sSprite* pSprite);
    int16           Sprite_Direction_Between_Points(sSprite* pSprite, int16& pData0, int16& pData4);
    void            sub_2A3D4(sSprite* pSprite);
    void            Squad_Walk_Steps_Decrease();
    int16           Map_PathCheck_CalculateTo(int16& pX1, int16& pY1, int16& pX2, int16& pY2);
    void            Map_PathCheck_Generate(int16& pX1, int16&  pY1, int16& pX2, int16& pY2, int16& pData18, int16& pData1C);
    int16           Map_PathCheck_CanPass(int16& pData0);
    int16           Map_Get_Distance_BetweenPoints(int16& pPosX, int16& pPosY, int16& pPosX2, int16& pDistanceMax, int16& pPosY2);
	int32			Map_Get_Distance_BetweenPositions(cPosition pPos1, cPosition pPos2, int32 pDistanceMax);

    int16           Map_Terrain_Get_Type_And_Walkable(int16& pY, int16& pX);
    int16           Map_Terrain_Get_Type_And_Walkable(sSprite* pSprite, int16& pY, int16& pX);
    int16           Map_Terrain_Get(int16& pY, int16& pX, int16& pData10, int16& pData14);
    int16           Map_Terrain_Get(int16 pX, int16 pY);
    int16           Tile_Terrain_Get(const int16 pTileID, int16 pX, int16 pY);

    void            Squad_Walk_Target_SetAll(int16 pValue);
    void            Squad_Walk_Target_Set(int16 pTargetX, int16 pTargetY, int16 pSquadNumber, int16 pData10);
    int16           Squad_Member_Sprite_Hit_In_Region(sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14);

    const sSpriteSheet* Sprite_Get_Sheet(int16 pSpriteType, int16 pFrame);

    void            sub_2AEB6(int16 pColumns, int16 pRows, int16 *pData8, int16* pDataC);

    void            Sprite_SetDirectionMod(sSprite* pSprite);
    void            Sprite_Vehicle_Direction_Update(sSprite* pSprite, int16& pData1C);

    int16           Map_Terrain_Get_Moveable_Wrapper(const int8* pMovementData, int16& pX, int16& pY, int16& pData10, int16& pData14);
    int16           Map_Terrain_Get_Moveable(const int8* pMovementData, int16& pX, int16& pY);

    void            Map_Get_Distance_BetweenPoints_Within_640(int16& pX, int16& pY, int16& pX2, int16& pY2);

    bool            MapTile_Update_Position();

    void            MapTile_Move_Right(int16 pPanTiles);
    void            MapTile_Move_Left(int16 pPanTiles);
    void            MapTile_Move_Down(int16 pPanTiles);
    void            MapTile_Move_Up(int16 pPanTiles);

    void            MapTile_Update_Y();
    void            MapTile_Update_X();

    int32           MapTile_Get(const size_t pTileX, const size_t pTileY);
    void            MapTile_Set(const size_t pTileX, const size_t pTileY, const size_t pTileID);
    sSprite*		Sprite_Add(const size_t pSpriteID, const int16 pTileX, const int16 pTileY);

    void            Squad_Troops_Count();
    int16           Mission_Troop_GetDeviatePotential(sMission_Troop* pSquadMember);
    void            Squad_Member_Rotate_Can_Fire();
    int16           Sprite_Find_In_Region(sSprite* pSprite, sSprite*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14);
    void            Sprite_Handle_Player_DestroyAll();
    void            Sprite_Handle_Player_Destroy_Unk();
    void            Squad_Prepare_GrenadesAndRockets();
    void            Sprite_Aggression_Set();
    void            Sprite_Handle_Enemy_Aggression_Set(sSprite* pSprite);
    int16           Sprite_Next_WalkTarget_Set(sSprite* pSprite);
    int16           Squad_Join_Check(sSprite* pSprite);
    void            Squad_Walk_Target_Update(int16 pData0);
    void            Sprite_Handle_Explosion_MapTiles(sSprite* pSprite);
    void            Map_Destroy_Tiles();
    void            Map_Destroy_Tiles_Next();

    void					Game_Load();
    std::vector<sSavedGame>	Game_Load_Filter(const std::vector<std::string>& pFiles);

    void					Game_Save();
	bool			GameOverCheck();

    void            GUI_Handle_Element_Mouse_Check(sGUI_Element* pData20);

    void            Game_Save_Wrapper();

    void            GUI_Element_Reset();

    void            GUI_Button_Setup(void(cFodder::*pFunction)(void));
    void            GUI_Button_Setup_Small(void(cFodder::*pFunction)(void));

    void            GUI_Button_Draw(std::string pText, const size_t pY, const size_t pColorShadow = 0xBF, const size_t pColorPrimary = 0xBC);
    void            GUI_Button_Draw_Small(const std::string pText, const size_t pY, const size_t pColorShadow = 0xBF, const size_t pColorPrimary = 0xBC);
    void            GUI_Button_Draw_SmallAt(const std::string pText, const size_t pX, const size_t pY, const size_t pColorShadow = 0xBF, const size_t pColorPrimary = 0xBC);

    void            GUI_Box_Draw(const size_t pColorShadow, const size_t pColorPrimary);
    void            GUI_Select_File_Loop(bool pShowCursor);
    std::string     GUI_Select_File(const char* pTitle, const std::vector<sSavedGame>& pSave, const std::vector<std::string> &pMaps);


    void            GUI_Input_CheckKey();
    void            GUI_Button_Load_MouseWheel();
    void            GUI_Button_Load_Up();
    void            GUI_Button_Load_Down();
    void            GUI_Button_Load_Exit();
    void            GUI_Button_Show_About();
    void            GUI_Button_Filename();

    int16           GUI_Button_NoAction();
    void            GUI_Button_NoAction2();
    void            GUI_Sidebar_Prepare_Squads();

    void            GUI_Button_SelectQuiz();
    void            GUI_Button_SelectMap0();
    void            GUI_Button_SelectMap1();
    void            GUI_Button_SelectMap2();
    void            GUI_Button_SelectMap3();
    void            GUI_Element_Mouse_Over(const sGUI_Element *pA0);
    void            Demo_Quiz();
    void            GUI_Button_ExitMenu();
    void            GUI_Button_Quiz_2();
    void            GUI_Button_Quiz_3();
    void            GUI_Button_Quiz_4();
    void            GUI_Button_Quiz_5();
    void            GUI_Button_Quiz_6();
    void            GUI_Button_Quiz_7();
    void            GUI_Button_Quiz_8();
    void            GUI_Button_Quiz_9();
    void            GUI_Button_Quiz_10();
    void            GUI_Button_Quiz_11();
    void            Demo_Quiz_ShowScreen(const char* pFilename);

    void            GUI_Loop_Draw_Prepare(int16& pData0, int16& pData4);
    void            GUI_Sidebar_Prepare(int16 pData0, int16 pData4);
    void            GUI_Sidebar_Add_Sidebar_Overlay();
    void            GUI_SetElementsFrom(sGUI_Element* pTo, sGUI_Element* pFrom);
    void            GUI_ClearElement(sGUI_Element *pData20);
    void            GUI_Sidebar_SplitButton_Draw();
    void            GUI_Prepare_Button_Squad();
    void            GUI_Handle_Button_SelectSquad_0();
    void            GUI_Handle_Button_SelectSquad_1();
    void            GUI_Handle_Button_SelectSquad_2();
    void            Squad_Select(int16 pData4, bool pCheckMouse = true);
    void            GUI_Handle_Button_SplitSquad();
    void            Mission_Set_Initial_Weapon();

    void            MapTiles_Draw();

    void            Recruit_CheckLoadSaveButtons();
    void            GUI_Sidebar_Grenades_Draw();

    void            GUI_Prepare_Button_Grenade();
    void            GUI_Handle_Button_Grenades();
    virtual void    GUI_Sidebar_TroopList_Draw();
    void            GUI_Sidebar_TroopList_Name_Draw(int16 pData0, int16 pData8, int16 pDataC, const char* pData28);
    void            GUI_Sidebar_SquadIcon_Update();
    void            GUI_Sidebar_SquadIcon_Current_Draw();
    int16           GUI_Sidebar_SquadIcon_Set();
    void            GUI_Sidebar_Draw();
    void            GUI_Sidebar_SquadIcons_Draw();
    void            GUI_Sidebar_Grenades_CurrentSquad_Draw();
    void            GUI_Sidebar_Grenades_Draw(int16 pData0);
    void            GUI_Sidebar_Rockets_CurrentSquad_Draw();
    void            GUI_Sidebar_Rockets_Draw(int16 pData0);
    int16           GUI_Sidebar_SelectedTroops_Count();
    void            GUI_Sidebar_Setup();
    void            sub_2FA05();
    void            GUI_Sidebar_MapButton_Prepare();
    void            GUI_Sidebar_Squad0_Prepare();
    void            GUI_Sidebar_Squad1_Prepare();
    void            GUI_Sidebar_Squad2_Prepare();

    void            GUI_Sidebar_Ready();
    void            GUI_Sidebar_Rockets_Draw();

    void            GUI_Prepare_Button_Rockets();
    void            GUI_Handle_Button_Rockets();

    void            GUI_Prepare_Button_TroopName();
    void            GUI_Handle_Button_TroopName();

    void            Squad_Select_Grenades();
    void            Squad_Select_Rockets();

    int16           sub_2FF41();

    void            Squad_Split_Assets();

    void            GUI_Sidebar_MapButton_RenderWrapper();
    void            GUI_Sidebar_Number_Draw(int16 pNumber, int16 pX, int16 pData8, int16 pY, int16 pData10);
    void            GUI_Sidebar_MapButton_Render();
    void            GUI_Handle_Button_ShowOverview();
    void            sub_3037A();
    void            sub_3049B();
    void            GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper();
    void            Squad_Member_Click_Check();

    void            sub_303AE();
    void            Sidebar_Render_SquadIcon();
    void            GUI_Sidebar_Squad_Split_Icon_Draw();
    void            Sidebar_Render_To_BackBuffer();
    void            Sidebar_Render_To_ScreenBuffer();
    void            Squad_Switch_Weapon();
    void            Mission_Final_TimeToDie();
    int16           sub_305D5(sSprite*& pData20);

    void            Squad_Member_Target_Set();

    void            Squad_Switch_Timer();
    void            Squad_Switch_To(int16 pData0);
    void            Vehicle_Input_Handle();
    void            sub_311A7();
    int16           sub_313CD();
    void            Vehicle_Target_Set();

    void            String_CalculateWidth(int32 pPosX, const uint8* pWidths, const std::string& pString);
    void            String_CalculateWidth(int32 pPosX, const uint8* pWidths, const char* pString);

    void            String_Print(const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const char* pText);
    void            String_Print(const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const std::string& pText);

    void            intro_LegionMessage();
    int16           intro_Play();

	void			intro_main();

    void            intro_Retail();
    void            intro_AmigaTheOne();
	void			Intro_OpenFodder();

    void            Sidebar_Clear_ScreenBufferPtr();

    void            WonGame();

    void            Video_SurfaceRender( const bool pRestoreSurface = true );
    void            Cycle_End();

    void            sleepLoop(int64 pMilliseconds);
    int16           ShowImage_ForDuration(const std::string& pFilename, uint16 pDuration, size_t pBackColor = 0, bool pCanAbort = true);

    void            Mouse_ButtonCheck();
    int16           Mouse_Button_Left_Toggled();
    int16           Mouse_Button_Right_Toggled();
    void            Mouse_Cursor_Handle();
    void            Mouse_Cursor_Update();
    void            Mouse_DrawCursor();
    virtual void    Mouse_Inputs_Get();
    void            Mouse_Inputs_Check();
    void            Mouse_Setup();

    void            eventProcess(const cEvent& pEvent);
    void            eventsProcess();
    void            keyProcess(uint8 pKeyCode, bool pPressed);

    void            Game_Setup();

    template <typename tType> std::shared_ptr<tType> GetGraphics() {
        return std::dynamic_pointer_cast<tType>(mGraphics);
    }

    template <typename tType> std::shared_ptr<tType> GetSound() {
        return std::dynamic_pointer_cast<tType>(mSound);
    }

public:

    cFodder(std::shared_ptr<cWindow> pWindow);
    virtual         ~cFodder();

    void            SetActiveSpriteSheetPtr(const sSpriteSheet** pSpriteSheet);

    void            String_Input_Print(int16 pPosY);
    void            String_Input_Check();

    void            String_Print_Small(std::string pText, const size_t pY);
    void            String_Print_Small(std::string pText, const size_t pX, const size_t pY);
    void            String_Print_Large(std::string pText, const bool pOverAndUnderLine, const size_t pY);
	void            String_Print_Large(std::string pText, const bool pOverAndUnderLine, const size_t pX, const size_t pY);

	void			DataNotFound();
    virtual void    Prepare(std::shared_ptr<sFodderParameters> pParams);

    void            Playground();

    void            About();
	void			CreateRandom();
	virtual void    Start();
    void            Exit(unsigned int pExitCode);

    void            WindowTitleSet(bool pInMission);
    void            WindowTitleBaseSetup();

    void			Menu_Button_Reset();
    void            Menu_Loop(const std::function<void()> pButtonHandler);
    bool            Menu_Draw(const std::function<void()> pButtonHandler);

    bool            Demo_Load();
    void            Window_UpdateScreenSize();

    void            VersionSwitch(const sGameVersion* pVersion);

	cDimension		getSurfaceSize() const;
	cDimension		getWindowSize() const;
	int16			getWindowWidth() const;

	int16			getWindowRows() const;
	int16			getWindowColumns() const;

	int16			getCameraWidth() const;
	int16			getCameraHeight() const;
};
