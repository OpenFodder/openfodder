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

/* Amiga Format Menu Buttons */
const sGUI_Element mAfx_Buttons[] = {
	{ &cFodder::GUI_Button_NoAction, 0x6B, 0x6B, 0x4A, 0x6B, &cFodder::GUI_Button_NoAction2 },
	{ &cFodder::GUI_Button_NoAction, 0x01, 0x9E, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap0 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9C, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap1 },
	{ &cFodder::GUI_Button_NoAction, 0x02, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap0 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap1 },
	{ 0 }
};

/* Cannon Fodder Plus Menu Buttons */
const sGUI_Element mPlus_Buttons[] = {
	{ &cFodder::GUI_Button_NoAction, 0x6B, 0x6B, 0x4A, 0x6B, &cFodder::GUI_Button_SelectQuiz },
	{ &cFodder::GUI_Button_NoAction, 0x01, 0x9E, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap0 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9C, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap1 },
	{ &cFodder::GUI_Button_NoAction, 0x02, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap2 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap3 },
};

/* Amiga Action Menu Buttons */
const sGUI_Element mAmigaAction_Buttons[] = {
    { &cFodder::GUI_Button_NoAction, 0x6B, 0x6B, 0x4A, 0x6B, &cFodder::GUI_Button_NoAction2 },
    { &cFodder::GUI_Button_NoAction, 0x01, 0x9E, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap0 },
    { &cFodder::GUI_Button_NoAction, 0xA1, 0x9C, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap1 },
    { &cFodder::GUI_Button_NoAction, 0x02, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap2 },
    { &cFodder::GUI_Button_NoAction, 0xA1, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap3 },
};

/* Cannon Fodder Plus Quiz Buttons */
const sGUI_Element mPlusQuiz_Buttons[] = {
	{ &cFodder::GUI_Button_NoAction, 0x0BD, 0x19, 0x5B, 0x48,	&cFodder::GUI_Button_ExitMenu },
	{ &cFodder::GUI_Button_NoAction, 0x0B2, 0x1C, 0xA2, 0x33,	&cFodder::GUI_Button_ExitMenu },
	{ &cFodder::GUI_Button_NoAction, 0x000, 0x31, 0x03, 0x1E,	&cFodder::GUI_Button_Quiz_2 },
	{ &cFodder::GUI_Button_NoAction, 0x10C, 0x31, 0x03, 0x1E,	&cFodder::GUI_Button_Quiz_3 },
	{ &cFodder::GUI_Button_NoAction, 0x000, 0x31, 0xD6, 0x1E,	&cFodder::GUI_Button_Quiz_4 },
	{ &cFodder::GUI_Button_NoAction, 0x10C, 0x31, 0xD6, 0x1E,	&cFodder::GUI_Button_Quiz_5 },
	{ &cFodder::GUI_Button_NoAction, 0x02A, 0x32, 0x10, 0xC9,	&cFodder::GUI_Button_Quiz_6 },
	{ &cFodder::GUI_Button_NoAction, 0x05B, 0x27, 0x1F, 0xB4,	&cFodder::GUI_Button_Quiz_7 },
	{ &cFodder::GUI_Button_NoAction, 0x081, 0x1E, 0x25, 0xA7,	&cFodder::GUI_Button_Quiz_8 },
	{ &cFodder::GUI_Button_NoAction, 0x09E, 0x1F, 0x24, 0xB1,	&cFodder::GUI_Button_Quiz_9 },
	{ &cFodder::GUI_Button_NoAction, 0x0BC, 0x2B, 0x21, 0xAA,	&cFodder::GUI_Button_Quiz_10 },
	{ &cFodder::GUI_Button_NoAction, 0x0E6, 0x3B, 0x13, 0xCE,	&cFodder::GUI_Button_Quiz_11 },
};

const int8 mGUI_Sidebar_TroopList_Sprite_Modifier[] = {
	0x29,	// Squad Selected (Normal)
	0x4D,	// Selected		  (Gold/Yellow)
	0x71	// Squad Not Selected (Grayed Out)
};

const int16 mGUI_Squad_Icons[3] = { 
	0, 
	1, 
	2
};

const int16 mGUI_Squad_Split_Icons[] = {
	0, 3, 4, // Squad 0
	6, 1, 5, // Squad 1
	8, 7, 2  // Squad 2
};

const int16 mGUI_Squad_Active_Icons[] = {
	149, 150, 151, 
	152, 153, 154, 
	155, 156, 157, 
	158, 158
};

const int16 mGUI_Squad_Inactive_Icons[] = {
	159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
	168
};

// Left value is vehicle ID type
const sGUI_SquadVehicleIcon mGUI_Squad_Vehicle_Icons[] = {
	{ eVehicle_Turret_Cannon,		 2 },
	{ eVehicle_Turret_Missile,		 3 },
	{ eVehicle_Jeep,				 4 },
	{ eVehicle_JeepRocket,			 5 },
	{ eVehicle_Tank,				 6 },
	{ eVehicle_Helicopter,			 7 },
	{ eVehicle_Helicopter_Grenade,	 8 },
	{ eVehicle_Helicopter_Missile,	 9 },
	{ eVehicle_Helicopter_Homing,	10 },
    { -1 }
};


void cFodder::GUI_Sidebar_Grenades_Draw() {
    int16 Data0 = mGUI_Loop_Squad_Current;
    int16 Data8, DataC, Data4, Data10;

    if (!mSquad_Grenades[mGUI_Loop_Squad_Current])
        goto loc_2ED63;

    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] != eWeapon_Grenade)
        goto loc_2ED63;

    if (!mGUI_Loop_Is_CurrentSquad) {
    loc_2ED63:;
        Data0 = 0xC4;
    }
    else {
        Data0 = 0xC3;
    }

    // Draw Grenade Icon
    Data8 = 0;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, Data8, DataC);

    Data0 = mSquad_Grenades[mGUI_Loop_Squad_Current];
    if (!Data0)
        return;

    Data4 = 0;
    Data8 = 0x0D;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x19;
    Data10 = 0xAF;

    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] != eWeapon_Grenade)
        goto loc_2EDF7;

    if (mGUI_Loop_Is_CurrentSquad)
        goto loc_2EDFD;

loc_2EDF7:;
    Data10 = 0xB9;

loc_2EDFD:;
    GUI_Sidebar_Number_Draw(Data0, Data4, Data8, DataC, Data10);

}

void cFodder::GUI_Prepare_Button_Grenade() {
    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    sGUI_Element* NewElement = mGUI_NextFreeElement;
    NewElement->field_0 = &cFodder::GUI_Button_NoAction;
    NewElement->mX = 0;
    NewElement->mWidth = 0x0C;
    NewElement->mY = mGUI_Loop_Draw_Y + 0x0E;
    NewElement->mHeight = 0x14;
    NewElement->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_Grenades;

    ++NewElement;
    GUI_ClearElement(NewElement);
}

void cFodder::GUI_Handle_Button_Grenades() {

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count()) {
        Squad_Select_Grenades();
        return;
    }

    ++mSquad_Grenade_SplitMode;
    if (mSquad_Grenade_SplitMode >= (eSquad_Weapon_Split_None + 1))
        mSquad_Grenade_SplitMode = eSquad_Weapon_Split_All;

    sub_303AE();

}


void cFodder::GUI_Sidebar_TroopList_Draw() {

    int16 Data0 = 0;
    int8 Data4 = mSquads_TroopCount[mGUI_Loop_Squad_Current];
    int16 DataC = mGUI_Loop_Draw_Y;

    int8 Data4_Saved = Data4;

    DataC += 0x22;
    Data4 -= 1;
    if (Data4 < 0)
        goto loc_2F1BC;

    for (; Data4 >= 0; --Data4) {

        if (!mGUI_Loop_Is_CurrentSquad)
            Data0 = 0xAB;
        else
            Data0 = 0xA9;

        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, 0, DataC);
        DataC += 0x0C;
    }
loc_2F1BC:;

    mGUI_Squad_NextDraw_Y = DataC;
    Data4 = Data4_Saved;
    if (Data4 < 0)
        return;

    mSidebar_Draw_Y = mGUI_Loop_Draw_Y + 0x22;

    // Loop the squad
    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        sSprite* Data34 = Troop.mSprite;

        if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
            continue;

        if (mGUI_Loop_Squad_Current != Data34->field_32)
            continue;

        // If sliding, or entering a vehicle, or invincible
        if (Data34->field_38 >= eSprite_Anim_Slide1 || (Data34->field_75 & eSprite_Flag_Invincibility))
            goto loc_2F25C;

        // If hit/dying, no longer render the troop name
        if (Data34->field_38 != eSprite_Anim_None)
            continue;

    loc_2F25C:;

        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Troop.mRank + 9, 0x00, mSidebar_Draw_Y);
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Troop.mRank + 9, 0x23, mSidebar_Draw_Y);

        const sRecruit* Data28 = &mRecruits[Troop.mRecruitID];
        if (!mGUI_Loop_Is_CurrentSquad)
            Data0 = 2;
        else
            Data0 = Troop.mSelected & 1;

        //loc_2F318
        GUI_Sidebar_TroopList_Name_Draw(Data0, 0x23, mSidebar_Draw_Y + 2, Data28->mName);
        mSidebar_Draw_Y += 0x0C;
    }
}

void cFodder::GUI_Sidebar_TroopList_Name_Draw(int16 pData0, int16 pData8, int16 pDataC, const char* pData28) {

    word_3AA21 = mGUI_Sidebar_TroopList_Sprite_Modifier[pData0];

    int16 Data14;

    for (Data14 = 0; Data14 <= mGUI_Sidebar_TroopList_Name_BreakOnSpace; ++Data14) {

        if (mGUI_Sidebar_TroopList_Name_BreakOnSpace == 5) {

            if (pData28[Data14] == 0x20)
                break;
        }

        if (pData28[Data14] == 0)
            break;

    }

    Data14 <<= 2;

    int16 Data18 = 0x30;
    Data18 -= Data14;
    Data18 >>= 1;
    word_3A05F = Data18;

    for (Data14 = 0; Data14 <= mGUI_Sidebar_TroopList_Name_BreakOnSpace; ++Data14) {

        if (pData28[Data14] != 0x20) {
            pData0 = pData28[Data14];
            pData0 -= 0x41;
            pData0 += word_3AA21;

            pData8 = Data14;
            pData8 <<= 2;
            pData8 += word_3A05F;

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(pData0, pData8, pDataC);
        }
    }
}

void cFodder::GUI_Sidebar_SquadIcon_Update() {

    GUI_Sidebar_SquadIcon_Set();
    GUI_Sidebar_SquadIcon_Current_Draw();
}

void cFodder::GUI_Sidebar_SquadIcon_Current_Draw() {

    int16 Data0 = mGUI_Squad_Icon[mGUI_Loop_Squad_Current];

    const int16* Data20 = mGUI_Squad_Active_Icons;

    if (!mGUI_Loop_Is_CurrentSquad)
        Data20 = mGUI_Squad_Inactive_Icons;

    Data0 = Data20[Data0];
    int16 Data8 = 0x0C;
    int16 DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, Data8, DataC);
}

int16 cFodder::GUI_Sidebar_SquadIcon_Set() {
    int16 Data1C = 0;
    sSprite* Vehicle = 0;
    const sGUI_SquadVehicleIcon* SquadIcon = mGUI_Squad_Vehicle_Icons;

    // No Troops in Squad?
    if (!mSquads_TroopCount[mGUI_Loop_Squad_Current])
        return 0;

    // More than 1 troop in squad?
    if (mSquads_TroopCount[mGUI_Loop_Squad_Current] != 1)
        Data1C = 1;

    // Squad not in vehicle?
    if (!mSquad_CurrentVehicles[mGUI_Loop_Squad_Current])
        goto loc_2F593;


    Vehicle = mSquad_CurrentVehicles[mGUI_Loop_Squad_Current];

    for (;; ++SquadIcon) {
        if (SquadIcon->mVehicleType < 0)
            goto loc_2F593;

        if (Vehicle->field_6F == SquadIcon->mVehicleType)
            goto loc_2F586;
    }
loc_2F586:;
    Data1C = SquadIcon->mIconID;

loc_2F593:;

    // has status changed?
    if (Data1C == mGUI_Squad_Icon[mGUI_Loop_Squad_Current])
        return 0;

    mGUI_Squad_Icon[mGUI_Loop_Squad_Current] = Data1C;
    return -1;
}

void cFodder::GUI_Sidebar_Draw() {
    Mission_Final_TimeToDie();
    GUI_Sidebar_SquadIcons_Draw();

    int16 Data0 = 2;
    int16 Data4 = 2;

    int8* Data20 = &mGUI_RefreshSquadGrenades[2]; //byte_3AC35; 
    int8* Data24 = &mGUI_RefreshSquadRockets[2]; //byte_3AC3B;
    int16* Data28 = mSquad_Grenades;
    int16* Data2C = mSquad_Rockets;
    int16* Data30 = mSquad_CurrentWeapon;

    for (Data0 = 2; Data0 >= 0; --Data0) {

        if (!*Data20)
            goto loc_2F67A;

        if (*(Data28 + Data4))
            goto loc_2F67A;

        if (*(Data30 + Data4) != 1)
            goto loc_2F67A;

        *(Data30 + Data4) = 3;
        *Data24 = -1;

    loc_2F67A:;
        if (!*Data24)
            goto loc_2F6B7;

        if (*(Data2C + Data4))
            goto loc_2F6B7;

        if (*(Data30 + Data4) != 3)
            goto loc_2F6B7;

        *(Data30 + Data4) = 1;
        *(Data20) = -1;

    loc_2F6B7:;
        --Data20;
        --Data24;
        Data4--;
    }

    Data20 = &mGUI_RefreshSquadGrenades[2];
    Data24 = &mGUI_RefreshSquadRockets[2];

    for (int16 Data0 = 2; Data0 >= 0; --Data0, --Data20, --Data24) {

        if (!*Data20)
            goto loc_2F707;

        *Data20 = 0;

        GUI_Sidebar_Grenades_Draw(Data0);

    loc_2F707:;

        if (!*Data24)
            continue;

        *Data24 = 0;

        GUI_Sidebar_Rockets_Draw(Data0);
    }
}

void cFodder::GUI_Sidebar_SquadIcons_Draw() {

    mGUI_Loop_Squad_Current = 0;

    for (mGUI_Loop_Squad_Current = 0; mGUI_Loop_Squad_Current < 3; ++mGUI_Loop_Squad_Current) {

        mGUI_Loop_Draw_Y = word_3AC2D[mGUI_Loop_Squad_Current];

        if (!mSquads_TroopCount[mGUI_Loop_Squad_Current])
            continue;

        mGUI_Loop_Is_CurrentSquad = 0;

        if (mGUI_Loop_Squad_Current == mSquad_Selected)
            mGUI_Loop_Is_CurrentSquad = -1;

        if (GUI_Sidebar_SquadIcon_Set())
            GUI_Sidebar_SquadIcon_Current_Draw();
    }
}

void cFodder::GUI_Sidebar_Grenades_CurrentSquad_Draw() {
    GUI_Sidebar_Grenades_Draw(mSquad_Selected);
}

void cFodder::GUI_Sidebar_Grenades_Draw(int16 pData0) {

    int16 Data4 = word_3AC2D[pData0];

    GUI_Loop_Draw_Prepare(pData0, Data4);
    GUI_Sidebar_Grenades_Draw();

    if (!mSquad_Grenades[mGUI_Loop_Squad_Current])
        return;

    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count())
        return;

    // Grenades not selected
    if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_None)
        return;

    int16 Data0 = 0xC7;
    Data0 += mSquad_Grenade_SplitMode;

    int16 Data8 = 0;
    int16 DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, Data8, DataC);
}

void cFodder::GUI_Sidebar_Rockets_CurrentSquad_Draw() {

    GUI_Sidebar_Rockets_Draw(mSquad_Selected);
}

void cFodder::GUI_Sidebar_Rockets_Draw(int16 pData0) {

    int16 Data4 = word_3AC2D[pData0];
    GUI_Loop_Draw_Prepare(pData0, Data4);
    GUI_Sidebar_Rockets_Draw();

    if (!mSquad_Rockets[mGUI_Loop_Squad_Current])
        return;

    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count())
        return;

    if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_None)
        return;

    pData0 = 0xC7;
    pData0 += mSquad_Rocket_SplitMode;

    int16 Data8 = 0x24;
    int16 DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(pData0, Data8, DataC);
}

int16 cFodder::GUI_Sidebar_SelectedTroops_Count() {

    int16 Data10 = 0;
    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        if (mSquad_Selected != Troop.mSprite->field_32)
            continue;

        if (!(Troop.mSelected & 1))
            continue;

        Data10++;
    }

    int16 Data0 = mSquads_TroopCount[mSquad_Selected];
    if (Data10 == Data0)
        return 0;

    if (Data10)
        return Data10;

    Data0 = 0;
    return 0;
}

void cFodder::GUI_Sidebar_Setup() {

    int16 Data0 = mGUI_Sidebar_Setup++;

    switch (Data0) {
    case 0: //loc_2FA05
        sub_2FA05();
        break;

    case 1: //loc_2FA1D
        GUI_Sidebar_MapButton_Prepare();
        break;

    case 2: //loc_2FA44
        GUI_Sidebar_Squad0_Prepare();
        break;

    case 3: //loc_2FA5B
        GUI_Sidebar_Squad1_Prepare();
        break;

    case 4: //loc_2FA75
        GUI_Sidebar_Squad2_Prepare();
        break;

    case 5: //loc_2FA8F
        GUI_Sidebar_Ready();
        break;

    case 6:
        mGUI_Sidebar_Setup = -1;
        return;

    default:
        std::cout << "GUI_Sidebar_Setup(): Missing Function " << Data0;
        break;
    }
}


void cFodder::sub_2FA05() {
    mMouseDisabled = -1;

    if (word_3AC51)
        return;

    Sidebar_Render_To_BackBuffer();
    word_3AC51 = -1;
}

void cFodder::GUI_Sidebar_MapButton_Prepare() {

    mGUI_NextFreeElement = mGUI_Elements;

    Sidebar_Clear_ScreenBufferPtr();

    GUI_Sidebar_MapButton_RenderWrapper();
}

void cFodder::GUI_Sidebar_Squad0_Prepare() {
    mGUI_Squad_NextDraw_Y = 0;

    GUI_Sidebar_Prepare(0, 0);
}

void cFodder::GUI_Sidebar_Squad1_Prepare() {
    int16 Data4 = mGUI_Squad_NextDraw_Y;
    if (Data4)
        Data4 += 5;

    GUI_Sidebar_Prepare(1, Data4);
}

void cFodder::GUI_Sidebar_Squad2_Prepare() {
    int16 Data4 = mGUI_Squad_NextDraw_Y;

    if (Data4)
        Data4 += 5;

    GUI_Sidebar_Prepare(2, Data4);
}

void cFodder::GUI_Sidebar_Ready() {
    GUI_Sidebar_Add_Sidebar_Overlay();
    Sidebar_Render_To_ScreenBuffer();

    word_3AC51 = 0;
    mMouseDisabled = 0;
    mMouse_Button_LeftRight_Toggle = false;
}

void cFodder::GUI_Sidebar_Rockets_Draw() {
    int16 Data0, Data8, DataC, Data4, Data14, Data10;

    if (!mSquad_Rockets[mGUI_Loop_Squad_Current])
        goto loc_2FAF6;

    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] != eWeapon_Rocket)
        goto loc_2FAF6;


    if (!mGUI_Loop_Is_CurrentSquad) {
    loc_2FAF6:;
        Data0 = 0xC6;
    }
    else {
        Data0 = 0xC5;
    }

    //loc_2FB04
    Data8 = 0x20;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;
    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, Data8, DataC);

    Data0 = mSquad_Rockets[mGUI_Loop_Squad_Current];
    if (!Data0)
        return;

    Data4 = 0x24;
    Data8 = 0x0D;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x19;
    Data14 = mGUI_Loop_Squad_Current;

    if (mSquad_CurrentWeapon[Data14] != eWeapon_Rocket)
        goto loc_2FB8A;

    Data10 = 0xAF;
    if (!mGUI_Loop_Is_CurrentSquad) {
    loc_2FB8A:;
        Data10 = 0xB9;
    }

    GUI_Sidebar_Number_Draw(Data0, Data4, Data8, DataC, Data10);
}

void cFodder::GUI_Prepare_Button_Rockets() {

    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    sGUI_Element* NewElement = mGUI_NextFreeElement;
    NewElement->field_0 = &cFodder::GUI_Button_NoAction;
    NewElement->mX = 0x24;
    NewElement->mWidth = 0x0C;
    NewElement->mY = mGUI_Loop_Draw_Y + 0x0E;
    NewElement->mHeight = 0x14;
    NewElement->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_Rockets;
    ++NewElement;

    GUI_ClearElement(NewElement);
}

void cFodder::GUI_Handle_Button_Rockets() {

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count()) {
        Squad_Select_Rockets();
        return;
    }

    ++mSquad_Rocket_SplitMode;

    if (mSquad_Rocket_SplitMode >= (eSquad_Weapon_Split_None + 1))
        mSquad_Rocket_SplitMode = eSquad_Weapon_Split_All;

    GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper();
}


void cFodder::Squad_Select_Grenades() {

    if (mSquad_CurrentWeapon[mSquad_Selected] == eWeapon_Grenade)
        return;

    mSquad_CurrentWeapon[mSquad_Selected] = eWeapon_Grenade;
    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
}



void cFodder::Squad_Select_Rockets() {

    if (mSquad_CurrentWeapon[mSquad_Selected] == eWeapon_Rocket)
        return;

    mSquad_CurrentWeapon[mSquad_Selected] = eWeapon_Rocket;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
}

void cFodder::GUI_Prepare_Button_TroopName() {

    if (mSquad_Selected != mGUI_Loop_Squad_Current)
        return;

    for (int16 Data4 = 2; Data4 >= 0; --Data4) {
        if (!mSquads_TroopCount[Data4])
            goto loc_2FCF4;
    }

    return;

loc_2FCF4:;
    mGUI_TroopName_DrawOffset = mGUI_Loop_Draw_Y;
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;
    Element->mX = 0;
    Element->mWidth = 0x2F;
    Element->mY = mGUI_Loop_Draw_Y + 0x22;

    Element->mHeight = mSquads_TroopCount[mSquad_Selected] * 0x0C;
    Element->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_TroopName;

    ++Element;
    GUI_ClearElement(Element);
}

void cFodder::GUI_Handle_Button_TroopName() {

    if (Mouse_Button_Left_Toggled() < 0) {
        if (Mouse_Button_Right_Toggled() < 0)
            return;
    }

    if (mSquad_Selected < 0)
        return;

    int16 Data0 = mMouseY;
    Data0 -= mGUI_Mouse_Modifier_Y;
    Data0 -= mGUI_TroopName_DrawOffset;
    Data0 -= 0x22;
    Data0 /= 0x0C;

    if (Data0 >= mSquads_TroopCount[mSquad_Selected])
        return;

    word_3A3BF = Data0;

    sMission_Troop* Troop = mGame_Data.mSoldiers_Allocated;
    int16 Data8 = word_3A3BF;

    for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Troop) {

        sSprite* Data34 = Troop->mSprite;

        if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
            continue;

        if (mSquad_Selected != Data34->field_32)
            continue;

        --Data8;
        if (Data8 < 0)
            break;
    }

    Troop->mSelected ^= 1;

    const sRecruit* Data28 = &mRecruits[Troop->mRecruitID];
    Data0 = Troop->mSelected & 1;
    Data8 = word_3A3BF;
    Data8 *= 0x0C;

    Data8 += mGUI_TroopName_DrawOffset;
    Data8 += 0x24;

    int16 DataC = Data8;

    GUI_Sidebar_TroopList_Name_Draw(Data0, Data8, DataC, Data28->mName);
    sub_3049B();
    sub_3037A();
}


int16 cFodder::sub_2FF41() {

    if (!GUI_Sidebar_SelectedTroops_Count())
        return 0;

    int16 Data0;
    for (Data0 = 0; Data0 < 3; ++Data0) {
        if (!mSquads_TroopCount[Data0])
            goto loc_2FF79;
    }
    return 0;

loc_2FF79:;
    int16 Data14 = Data0;

    mSquad_JoiningTo = mSquad_Selected;
    int16 Data10 = Data14;

    sMapTarget* Dataa34 = mSquad_WalkTargets[Data10];
    Dataa34->asInt = -1;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        if (mSquad_Selected != Troop.mSprite->field_32)
            continue;

        if (!(Troop.mSelected & 1))
            continue;

        Troop.mSprite->field_32 = Data14;
        Troop.mSprite->field_40 = 0;
        Troop.mSprite->field_42 = 0;
        Troop.mSprite->field_44 = 0;
        Troop.mSprite->field_28 += 4;
    }

    mSquad_Selected = Data14;
    Mission_Troops_Clear_Selected();

    return -1;
}


void cFodder::GUI_Sidebar_MapButton_RenderWrapper() {

    // Don't display the map button on the final map
    if (mVersionCurrent->isRetail() && mVersionCurrent->isCannonFodder1()) {
        if ((mGame_Data.mMission_Number == 24 && mGame_Data.mMission_Phase == 6))
            return;
    }

    // No the map overview button in the Demo versions
    if (mVersionCurrent->isDemo() && !mVersionCurrent->isCustom())
        return;

    mGUI_Sidebar_MapButton_Prepared = 0;
    GUI_Sidebar_MapButton_Render();
}

void cFodder::GUI_Sidebar_Number_Draw(int16 pNumber, int16 pX, int16 pData8, int16 pY, int16 pData10) {
    pData10 -= 0x1A;

    std::stringstream Tmp;
    Tmp << tool_StripLeadingZero(std::to_string(pNumber));

    String_CalculateWidth(pData8, mFont_Sidebar_Width, Tmp.str());
    mGUI_Print_String_To_Sidebar = -1;
    String_Print(mFont_Sidebar_Width, pData10, mGUI_Temp_X + pX, pY, Tmp.str());

    mGUI_Print_String_To_Sidebar = 0;
}

int16 cFodder::Mouse_Button_Left_Toggled() {
    if (mMouse_Button_Left_Toggle >= 0)
        return -1;

    mMouse_Button_Left_Toggle = 1;
    return 1;
}


void cFodder::GUI_Sidebar_Squad_Split_Icon_Draw() {
    int16 EmptySquadID;

    // Loop each squad
    for (EmptySquadID = 0; EmptySquadID < 3; ++EmptySquadID) {

        // Find an empty squad
        if (!mSquads_TroopCount[EmptySquadID]) {

            if (mSquad_Selected < 0)
                return;

            // 3 Icons per Squad Slot
            int16 SquadIcon = (mSquad_Selected * 3);

            // Split into squad
            SquadIcon += EmptySquadID;

            int16 SplitIconID = mGUI_Squad_Split_Icons[SquadIcon];

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(SplitIconID, 0, mGUI_Loop_Draw_Y);
            return;
        }
    }
}


void cFodder::GUI_Draw_Frame_8(int32 pSpriteType, int32 pFrame, const size_t pPositionX, const size_t pPositionY) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_PosX = (int16)(pPositionX + 0x10);
    mVideo_Draw_PosY = (int16)(pPositionY + 0x10);
    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount;

    if (SheetData->mRowCount > mGUI_Draw_LastHeight)
        mGUI_Draw_LastHeight = SheetData->mRowCount;

    if (Sprite_OnScreen_Check())
        mGraphics->Video_Draw_8();

}

void cFodder::GUI_Draw_Frame_16(int16 pSpriteType, int16 pFrame, const size_t pPosX, const size_t pPosY) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_PosX = (int16)(pPosX + 0x10);
    mVideo_Draw_PosY = (int16)(pPosY + 0x10);
    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount;
    mVideo_Draw_ColumnsMax = 0x140;

    if (Sprite_OnScreen_Check())
        mGraphics->Video_Draw_16();
}

void cFodder::GUI_Render_Text_Centred(const char* pText, const size_t pPosY) {

    String_CalculateWidth(320, mFont_Recruit_Width, pText);
    String_Print(mFont_Recruit_Width, 0x0D, mGUI_Temp_X, pPosY, pText);
}


void cFodder::GUI_Handle_Button_ShowOverview() {

    if (mPhase_Finished)
        return;

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    mPhase_ShowMapOverview = -1;
}

void cFodder::sub_3037A() {

    if (GUI_Sidebar_SelectedTroops_Count()) {

        if (word_3AC4B < 0)
            return;

        word_3AC4B = -1;
        sub_303AE();
        return;
    }

    if (!word_3AC4B)
        return;

    word_3AC4B = 0;
    mSquad_Grenade_SplitMode = 1;

    GUI_Sidebar_Grenades_CurrentSquad_Draw();
    Sidebar_Render_SquadIcon();
}

void cFodder::sub_3049B() {

    if (GUI_Sidebar_SelectedTroops_Count()) {

        if (word_3AC4D < 0)
            return;

        word_3AC4D = -1;
        GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper();
        return;
    }

    if (word_3AC4D) {
        word_3AC4D = 0;
        mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;

        GUI_Sidebar_Rockets_CurrentSquad_Draw();
    }
}

void cFodder::GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper() {
    GUI_Sidebar_Rockets_CurrentSquad_Draw();
}

void cFodder::GUI_Sidebar_MapButton_Render() {

    if (mGUI_Sidebar_MapButton_Prepared)
        return;

    sGUI_Element* Element = mGUI_NextFreeElement;
    Element->field_0 = &cFodder::GUI_Button_NoAction;
    Element->mX = 0;

    Element->mY = (getCameraHeight() - 0x0B); // PLATFORM_BASED(189, 214);
    Element->mWidth = 0x2F;
    Element->mHeight = 0x0B;
    Element->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_ShowOverview;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(0xD0, Element->mX, Element->mY);

    ++Element;

    GUI_ClearElement(Element);
    mGUI_Sidebar_MapButton_Prepared = -1;

}

void cFodder::GUI_Element_Reset() {

    mGUI_NextFreeElement = mGUI_Elements;

    for (uint16 x = 0; x < 42; ++x) {
        mGUI_Elements[x].field_0 = 0;
        mGUI_Elements[x].mX = 0;
        mGUI_Elements[x].mWidth = 0;
        mGUI_Elements[x].mY = 0;
        mGUI_Elements[x].mHeight = 0;
        mGUI_Elements[x].mMouseInsideFuncPtr = 0;
    }
}

void cFodder::GUI_Button_Draw_Small(const std::string pText, const size_t pY, const size_t pColorShadow, const size_t pColorPrimary) {
    String_Print_Small(pText, pY);

    GUI_Box_Draw(pColorShadow, pColorPrimary);
}

void cFodder::GUI_Button_Draw_SmallAt(const std::string pText, const size_t pX, const size_t pY, const size_t pColorShadow, const size_t pColorPrimary) {
    String_Print_Small(pText, pX, pY);

    mGUI_Temp_X = (int16) pX;
    mGUI_Temp_Y = (int16) pY;

    GUI_Box_Draw(pColorShadow, pColorPrimary);
}

void cFodder::GUI_Button_Draw(std::string pText, const size_t pY, const size_t pColorShadow, const size_t pColorPrimary) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    GUI_Render_Text_Centred(pText.c_str(), pY);

    GUI_Box_Draw(pColorShadow, pColorPrimary);
}

void cFodder::GUI_Box_Draw(const size_t pColorShadow, const size_t pColorPrimary) {

    int16 X = mGUI_Temp_X;
    int16 Y = mGUI_Temp_Y;
    int16 Width = mGUI_Temp_Width;
    int16 Height = mGUI_Draw_LastHeight;

    Briefing_DrawBox(X - 2, Y - 2, Width + 4, Height + 5, (uint8)pColorPrimary);
    Briefing_DrawBox(X - 3, Y - 3, Width + 4, Height + 5, (uint8)pColorShadow);
}

void cFodder::GUI_Button_Setup(void(cFodder::*pFunction)(void)) {
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;

    Element->mX = mGUI_Temp_X - 3;
    Element->mWidth = mGUI_Temp_Width + 4;
    Element->mY = mGUI_Temp_Y;
    Element->mHeight = mGUI_Draw_LastHeight + 5;
    Element->mMouseInsideFuncPtr = pFunction;

    ++Element;
    Element->field_0 = 0;
    mGUI_NextFreeElement = Element;
}

void cFodder::GUI_Button_Setup_Small(void(cFodder::*pFunction)(void)) {
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;

    Element->mX = mGUI_Temp_X - 3;
    Element->mWidth = mGUI_Temp_Width + 4;
    Element->mY = mGUI_Temp_Y;
    Element->mHeight = mGUI_Draw_LastHeight + 5;
    Element->mMouseInsideFuncPtr = pFunction;

    ++Element;
    Element->field_0 = 0;
    mGUI_NextFreeElement = Element;
}

std::string cFodder::GUI_Select_File(const char* pTitle, const std::vector<sSavedGame>& pSave, const std::vector<std::string> &pMaps) {
    mPhase_Aborted = false;
    mGUI_SaveLoadAction = 0;

    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);

    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_Count = (pSave.size() == 0 ? (int16)pMaps.size() : (int16)pSave.size());

    do {
        size_t YOffset = PLATFORM_BASED(0, 25);

        mSurface->clearBuffer();
        GUI_Element_Reset();
        GUI_Render_Text_Centred(pTitle, 0x0C);

        GUI_Button_Draw("UP", 0x24);
        GUI_Button_Setup(&cFodder::GUI_Button_Load_Up);

        GUI_Button_Draw("DOWN", 0x99 + YOffset);
        GUI_Button_Setup(&cFodder::GUI_Button_Load_Down);

        GUI_Button_Draw("CONTINUE GAME", 0xB3 + YOffset);
        GUI_Button_Setup(&cFodder::GUI_Button_Load_Exit);

        mSurface->Save();

        int16 DataC = 0;

        // Draw the raw files list?
        if (!pSave.size()) {
            auto FileIT = pMaps.begin() + mGUI_Select_File_CurrentIndex;

            for (; DataC < mGUI_Select_File_ShownItems && FileIT != pMaps.end(); ++DataC) {
                size_t Pos = FileIT->find_first_of(".");

                GUI_Button_Draw(FileIT->substr(0, Pos), 0x3E + (DataC * 0x15), 0xB2, 0xB3);
                GUI_Button_Setup(&cFodder::GUI_Button_Filename);
                ++FileIT;
            }
        }
        else {
            // Or the save game list
            auto FileIT = pSave.begin() + mGUI_Select_File_CurrentIndex;

            for (; DataC < mGUI_Select_File_ShownItems && FileIT != pSave.end(); ++DataC) {

                GUI_Button_Draw(FileIT->mName, 0x3E + (DataC * 0x15), 0xB2, 0xB3);
                GUI_Button_Setup(&cFodder::GUI_Button_Filename);
                ++FileIT;
            }
        }

        GUI_Select_File_Loop(false);
        mSurface->Restore();

    } while (mGUI_SaveLoadAction == 3);

    if (mGUI_SaveLoadAction == 1)
        return "";

    if (pSave.size())
        return pSave[mGUI_Select_File_CurrentIndex + mGUI_Select_File_SelectedFileIndex].mFileName;

    return pMaps[mGUI_Select_File_CurrentIndex + mGUI_Select_File_SelectedFileIndex];
}

void cFodder::GUI_Select_File_Loop(bool pShowCursor) {
    int8 byte_44B49 = 0;
    if (mGUI_SaveLoadAction != 3) {

        mSurface->palette_FadeTowardNew();
        Mouse_Setup();
    }
    mGUI_SaveLoadAction = 0;

    mGraphics->PaletteSet();

    mSurface->Save();
    bool mShow = false;

    do {
        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        ++byte_44B49;
        byte_44B49 &= 0x0F;
        if (!byte_44B49) {
            if (pShowCursor)
                mShow = !mShow;
            mMouse_Button_Left_Toggle = 0;
        }

        // Show the cursor?
        if (mShow)
            GUI_Draw_Frame_8(PLATFORM_BASED(0x0F, 0x18), 0x00, mGUI_Temp_X + mGUI_Temp_Width, 0x50);

        Mouse_Inputs_Get();
        Mouse_DrawCursor();

        if (mPhase_Aborted)
            GUI_Button_Load_Exit();

        ++byte_44AC0;
        if (Mouse_Button_Left_Toggled() >= 0)
            GUI_Handle_Element_Mouse_Check(mGUI_Elements);

        GUI_Button_Load_MouseWheel();

        if (mGUI_Select_File_String_Input_Callback)
            (this->*mGUI_Select_File_String_Input_Callback)(0x50);

        Video_SurfaceRender();
        Cycle_End();

    } while (mGUI_SaveLoadAction <= 0);

    mPhase_Aborted = false;

    if (mGUI_SaveLoadAction == 3)
        return;

    Image_FadeOut();
}

void cFodder::GUI_Handle_Element_Mouse_Check(sGUI_Element* pLoop_Element) {

    for (; pLoop_Element->field_0; ++pLoop_Element) {

        int16 Data0 = pLoop_Element->mX;
        int16 Data4 = mMouseX;
        Data4 += 0x20;

        if (Data0 > Data4)
            continue;

        Data0 += pLoop_Element->mWidth;
        if (Data0 < Data4)
            continue;

        Data0 = pLoop_Element->mY;
        if (Data0 > mMouseY)
            continue;

        Data0 += pLoop_Element->mHeight;
        if (Data0 < mMouseY)
            continue;

        (*this.*pLoop_Element->mMouseInsideFuncPtr)();
        return;
    }
}

void cFodder::GUI_Button_Load_Exit() {
    mGUI_Select_File_String_Input_Callback = 0;
    mGUI_SaveLoadAction = 1;
}

void cFodder::GUI_Button_Show_About() {
    mGUI_Select_File_String_Input_Callback = 0;
    mGUI_SaveLoadAction = 4;
}

void cFodder::GUI_Input_CheckKey() {

    if (mKeyCode != mInput_LastKey) {
        mInput_LastKey = mKeyCode;

        if (!(mInput_LastKey & 0x80)) {

            if (mKeyCode >= SDL_SCANCODE_A && mKeyCode <= SDL_SCANCODE_Z)
                mKeyCodeAscii = 'A' + (mKeyCode - 4);

            if (mKeyCode >= SDL_SCANCODE_1 && mKeyCode <= SDL_SCANCODE_9)
                mKeyCodeAscii = '1' + (mKeyCode - 30);

            if (mKeyCode == SDL_SCANCODE_0)
                mKeyCodeAscii = '0';

            if (mKeyCode == SDL_SCANCODE_RETURN)
                mKeyCodeAscii = 0x0D;

            if (mKeyCode == SDL_SCANCODE_BACKSPACE)
                mKeyCodeAscii = 8;

            return;
        }
    }

    mKeyCodeAscii = 0;
}


void cFodder::GUI_Button_Load_MouseWheel() {
    if (mMouse_EventLastWheel.mY > 0) {
        GUI_Button_Load_Up();
    }
    else if (mMouse_EventLastWheel.mY < 0) {
        GUI_Button_Load_Down();
    }
}

void cFodder::GUI_Button_Load_Up() {
    mGUI_Select_File_CurrentIndex -= (mGUI_Select_File_ShownItems - 1);
    if (mGUI_Select_File_CurrentIndex < 0)
        mGUI_Select_File_CurrentIndex = 0;

    mGUI_SaveLoadAction = 3;
}

void cFodder::GUI_Button_Load_Down() {
    mGUI_Select_File_CurrentIndex += (mGUI_Select_File_ShownItems - 1);

    int16 Data0 = mGUI_Select_File_CurrentIndex;
    Data0 += mGUI_Select_File_ShownItems;

    if (Data0 >= mGUI_Select_File_Count) {
        Data0 = mGUI_Select_File_Count;
        Data0 -= mGUI_Select_File_ShownItems;
        if (Data0 < 0)
            Data0 = 0;

        mGUI_Select_File_CurrentIndex = Data0;
    }

    mGUI_SaveLoadAction = 3;
}

void cFodder::GUI_Button_Filename() {
    int16 Data0 = mMouseY;

    Data0 -= 0x3E;
    Data0 /= 0x15;

    mGUI_Select_File_SelectedFileIndex = Data0;
    mGUI_SaveLoadAction = 2;
}

int16 cFodder::GUI_Button_NoAction() {
    return 0;
}

void cFodder::GUI_Button_NoAction2() {
    return;
}

void cFodder::GUI_Sidebar_Prepare_Squads() {

    mMouseDisabled = -1;
    Sidebar_Render_To_BackBuffer();

    mGUI_NextFreeElement = mGUI_Elements;

    Sidebar_Clear_ScreenBufferPtr();
    int16 Data4 = 0;
    {
        // Squad 1
        mGUI_Squad_NextDraw_Y = 0;
        GUI_Sidebar_Prepare(0, Data4);
    }

    {
        // Squad 2
        Data4 = mGUI_Squad_NextDraw_Y;
        if (Data4)
            Data4 += 5;
        GUI_Sidebar_Prepare(1, Data4);
    }

    {
        // Squad 3
        Data4 = mGUI_Squad_NextDraw_Y;
        if (Data4)
            Data4 += 5;
        GUI_Sidebar_Prepare(2, Data4);
    }

    GUI_Sidebar_Add_Sidebar_Overlay();

    mMouseDisabled = 0;

    Sidebar_Render_To_ScreenBuffer();
    mMouse_Button_LeftRight_Toggle = false;
}

void cFodder::GUI_Element_Mouse_Over(const sGUI_Element *pElement) {
    for (;; ++pElement) {
        const sGUI_Element* Data20 = pElement;

        if (Data20->field_0 == 0)
            break;

        if ((*this.*Data20->field_0)() < 0)
            return;

        int16 Data0 = Data20->mX;

        int16 Data4 = mMouseX + 0x20;

        if (Data0 > Data4)
            continue;

        Data0 += Data20->mWidth;
        if (Data0 < Data4)
            continue;

        Data0 = mGUI_Mouse_Modifier_Y;
        Data0 += Data20->mY;
        if (Data0 > mMouseY)
            continue;

        Data0 += Data20->mHeight;
        if (Data0 < mMouseY)
            continue;

        (*this.*Data20->mMouseInsideFuncPtr)();
        return;
    }
}

void cFodder::GUI_Button_SelectMap0() {
    mGame_Data.mMission_Number = 0;
    mGame_Data.Phase_Next();
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_SelectMap1() {
    mGame_Data.mMission_Number = 1;
    mGame_Data.Phase_Next();
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_SelectQuiz() {
    Demo_Quiz();
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_SelectMap2() {
    mGame_Data.mMission_Number = 2;
    mGame_Data.Phase_Next();
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_SelectMap3() {
    mGame_Data.mMission_Number = 3;
    mGame_Data.Phase_Next();
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_ExitMenu() {
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_Quiz_2() {

    Demo_Quiz_ShowScreen("2.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_3() {

    Demo_Quiz_ShowScreen("3.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_4() {

    Demo_Quiz_ShowScreen("4.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_5() {

    Demo_Quiz_ShowScreen("5.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_6() {

    Demo_Quiz_ShowScreen("6.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_7() {

    Demo_Quiz_ShowScreen("7.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_8() {

    Demo_Quiz_ShowScreen("8.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_9() {
    Demo_Quiz_ShowScreen("9.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_10() {
    Demo_Quiz_ShowScreen("10.lbm");
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_11() {
    Demo_Quiz_ShowScreen("11.lbm");
    mDemo_ExitMenu = -1;
}


void cFodder::GUI_Loop_Draw_Prepare(int16& pData0, int16& pData4) {
    mGUI_Loop_Squad_Current = pData0;
    mGUI_Loop_Draw_Y = pData4;
    mGUI_Loop_Is_CurrentSquad = 0;

    if (pData0 == mSquad_Selected)
        mGUI_Loop_Is_CurrentSquad = -1;
}

void cFodder::GUI_Sidebar_Prepare(int16 pSquadNumber, int16 pData4) {

    GUI_Loop_Draw_Prepare(pSquadNumber, pData4);

    word_3AC2D[pSquadNumber] = pData4;
    Squad_Troops_Count();

    if (!mSquads_TroopCount[mGUI_Loop_Squad_Current])
        return;

    GUI_Sidebar_SplitButton_Draw();
    GUI_Prepare_Button_Squad();

    Mission_Set_Initial_Weapon();

    GUI_Sidebar_Grenades_Draw();
    GUI_Prepare_Button_Grenade();

    GUI_Sidebar_Rockets_Draw();
    GUI_Prepare_Button_Rockets();

    GUI_Sidebar_SquadIcon_Update();

    GUI_Sidebar_TroopList_Draw();
    GUI_Prepare_Button_TroopName();
}

void cFodder::GUI_Sidebar_Add_Sidebar_Overlay() {

    GUI_SetElementsFrom(mGUI_NextFreeElement, mSidebar_OverlayButtons);
}

void cFodder::GUI_SetElementsFrom(sGUI_Element* pTo, sGUI_Element* pFrom) {

    while (pFrom->field_0 != 0) {
        *pTo = *pFrom;

        ++pTo;
        ++pFrom;
    }

    GUI_ClearElement(pTo);

}

void cFodder::GUI_ClearElement(sGUI_Element *pData20) {

    pData20->field_0 = 0;
    mGUI_NextFreeElement = pData20;
}

void cFodder::GUI_Sidebar_SplitButton_Draw() {

    int16 Data0 = mGUI_Squad_Icons[mGUI_Loop_Squad_Current];
    int16 Data8 = 0;
    int16 DataC = mGUI_Loop_Draw_Y;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, Data8, DataC);
}

void cFodder::GUI_Prepare_Button_Squad() {
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;
    Element->mX = 0;
    Element->mY = mGUI_Loop_Draw_Y;

    Element->mWidth = 0x2F;
    Element->mHeight = 0x0E;

    if (mGUI_Loop_Squad_Current != mSquad_Selected) {
        Element->mMouseInsideFuncPtr = mGUI_Handle_Button_SelectSquad_Array[mGUI_Loop_Squad_Current];
    }
    else {
        Element->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_SplitSquad;
    }

    ++Element;
    GUI_ClearElement(Element);
}

void cFodder::GUI_Handle_Button_SelectSquad_0() {
    Squad_Select(0);
}

void cFodder::GUI_Handle_Button_SelectSquad_1() {
    Squad_Select(1);
}

void cFodder::GUI_Handle_Button_SelectSquad_2() {
    Squad_Select(2);
}

void cFodder::GUI_Handle_Button_SplitSquad() {

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (!sub_2FF41())
        return;

    Squad_Split_Assets();

    mSquad_Grenade_SplitMode = eSquad_Weapon_Split_Half;
    mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;
    word_3AC4B = 0;
    word_3AC4D = 0;

    Squad_Walk_Target_Update(mSquad_Selected);
    Squad_Troops_Count();

    sSprite* Data20 = 0;
    sub_305D5(Data20);

    mSquad_Select_Timer = 1;
    mGUI_Sidebar_Setup = 0;
}

void cFodder::Sidebar_Render_SquadIcon() {
    if (mSquad_Selected < 0)
        return;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(mSquad_Selected, 0, mGUI_Loop_Draw_Y);
}

void cFodder::Sidebar_Clear_ScreenBufferPtr() {

	memset(mSidebar_Screen_BufferPtr, 0, mSidebar_Buffer_Size);
}

void cFodder::Sidebar_Render_To_BackBuffer() {

	memcpy(mSidebar_Back_Buffer, mSidebar_Screen_Buffer, mSidebar_Buffer_Size);

    mSidebar_Screen_BufferPtr = mSidebar_Back_Buffer;
}

/**
 * Copy 'mSidebar_Back_Buffer' to the Screen Buffer, and set the Ptr to the Screen Buffer
 */
void cFodder::Sidebar_Render_To_ScreenBuffer() {

	memcpy(mSidebar_Screen_Buffer, mSidebar_Back_Buffer, mSidebar_Buffer_Size);

    mSidebar_Screen_BufferPtr = mSidebar_Screen_Buffer;
}
