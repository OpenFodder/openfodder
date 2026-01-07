/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
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

void cFodder::Squad_Walk_Target_SetAll(int16 pValue) {

    for (unsigned int x = 0; x < 10; ++x) {

        for (unsigned int y = 0; y < 30; ++y) {

            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
        }
    }

}

void cFodder::Squad_Set_Squad_Leader() {

    mSquad_Leader = &mSprites[0];
}

void cFodder::Squad_Member_PhaseCount() {

    for( auto& Troop : mGame_Data.mSoldiers_Allocated ) {
        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        ++Troop.mPhaseCount;
    }
}

void cFodder::Squad_Set_CurrentVehicle() {

    if (mSquad_Selected >= 0) {
        sSprite** Data20 = mSquads[mSquad_Selected];

        mSquad_CurrentVehicles[mSquad_Selected] = 0;

        int16 Data0 = 0;

        for (;;) {
            if (*Data20 != INVALID_SPRITE_PTR && *Data20 != 0) {
                sSprite* Data24 = *Data20++;

                // Sprite not in vehicle?
                if (!Data24->mInVehicle)
                    break;

                mSquad_CurrentVehicle = Data24->mCurrentVehicle;
                Data0 = -1;
                continue;

            }
            else {
                //loc_12C2B
                if (Data0 == 0)
                    break;

                mSquad_CurrentVehicles[mSquad_Selected] = mSquad_CurrentVehicle;
                mSquad_Leader = mSquad_CurrentVehicle;
                return;
            }
        }

    }

    mSquad_CurrentVehicle = 0;
}

void cFodder::Squad_EnteredVehicle_TimerTick() {

    for (int16 Data0 = 0; Data0 < 3; ++Data0) {
        if (!mSquad_EnteredVehicleTimer[Data0])
            continue;

        mSquad_EnteredVehicleTimer[Data0]--;
        if (mSquad_EnteredVehicleTimer[Data0])
            continue;

        sSprite** Data2C = mSquads[Data0];

        for (;;) {
            if (*Data2C == INVALID_SPRITE_PTR)
                break;

            sSprite* Data30 = *Data2C++;
            Data30->mAnimState = eSprite_Anim_Vehicle_Enter;
        }
    }
}

void cFodder::Squad_Walk_Steps_Decrease() {

    for (int16 Data0 = 2; Data0 >= 0; --Data0) {

        if (mSquad_Walk_Target_Steps[Data0])
            --mSquad_Walk_Target_Steps[Data0];

    }
}

void cFodder::Squad_Walk_Target_Set(int16 pTargetX, int16 pTargetY, int16 pSquadNumber, int16 pData10) {
    int16 Data1C;
    sSprite** Saved_Data24 = 0;

    if (pSquadNumber < 0 || pSquadNumber >= 3)
        return;

    pData10 = mSquad_Walk_Target_Indexes[pSquadNumber];

    sSprite** Data24 = mSquads[pSquadNumber];

    // Currently walking to a target?
    if (mSquad_Walk_Target_Steps[pSquadNumber]) {

        // Add this target to the queue
        mSquad_WalkTargets[pSquadNumber][pData10].mX = pTargetX;
        mSquad_WalkTargets[pSquadNumber][pData10].mY = pTargetY;

        mSquad_WalkTargets[pSquadNumber][pData10 + 1].asInt = -1;

        if (pData10 < (sizeof(mSquad_WalkTargets[pSquadNumber]) / sizeof(mSquad_WalkTargets[pSquadNumber][0])))   // 0x74
            pData10++;

        mSquad_Walk_Target_Indexes[pSquadNumber] = pData10;
        goto loc_2ABF8;
    }

    //loc_2AA4C
    pData10 = 0;
    Data1C = -1;

    Saved_Data24 = Data24;

    for (;;) {
        if (*Data24++ == INVALID_SPRITE_PTR)
            break;

        ++Data1C;
        pData10 += 1;
    }

    if (mSquad_WalkTargetX) {
        mSquad_WalkTargets[pSquadNumber][pData10].mX = mSquad_WalkTargetX;
        mSquad_WalkTargets[pSquadNumber][pData10].mY = mSquad_WalkTargetY;
        pData10 += 1;
    }
    //loc_2AAAE
    mSquad_WalkTargets[pSquadNumber][pData10].mX = pTargetX;
    mSquad_WalkTargets[pSquadNumber][pData10].mY = pTargetY;

    mSquad_WalkTargetX = pTargetX;
    mSquad_WalkTargetY = pTargetY;

    mSquad_WalkTargets[pSquadNumber][pData10 + 1].asInt = -1;

    Data24 = Saved_Data24;
    if (Data1C < 0)
        return;

    mSquad_Walk_Target_Indexes[pSquadNumber] = pData10;
    mSquad_Walk_Target_Indexes[pSquadNumber]++;

    for (;;) {
        sSprite* Data28 = *Data24++;

        if (Data28 == INVALID_SPRITE_PTR)
            break;

        if (Data28->mIsSinking)
            continue;

        if (Data28->mInVehicle)
            continue;

        Data28->mRetargetCooldown = 0;
        Data28->field_4D = 0;
        Data28->mActionCooldown = 0;
        Data28->field_43 = 0;
        Data28->mNextWalkTargetIndex = pData10;
        Data28->mNextWalkTargetIndex++;
        pData10--;

        mSquad_WalkTargets[pSquadNumber][pData10].mX = Data28->mPosX;
        mSquad_WalkTargets[pSquadNumber][pData10].mY = Data28->mPosY;

        Data28->mTargetX = pTargetX;
        Data28->mTargetY = pTargetY;

        pTargetX = Data28->mPosX;
        pTargetY = Data28->mPosY;
    }

loc_2ABF8:;
    mSquad_Walk_Target_Steps[pSquadNumber] = 8;
}

int16 cFodder::Squad_Members_Find_In_Region(sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14) {
    int16 Data4;

    if (mPhase_Finished) {
        word_3AA45 = 0;
        return 0;
    }

    mSprites_Found_Count = 0;

    int16 Data0;

    for( auto& Troop : mGame_Data.mSoldiers_Allocated ) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        if (pData8 > (Troop.mSprite->mPosX + 9))
            continue;

        if (pDataC < Troop.mSprite->mPosX)
            continue;

        if (pData10 > (Troop.mSprite->mPosY - 6))
            continue;

        if (pData14 < (Troop.mSprite->mPosY - 13))
            continue;

        if (Troop.mSprite->mHeight >= 0x0B)
            continue;

        if (!word_3AA45)
            goto loc_2AD3D;

        if (Troop.mSprite->mInVehicle)
            continue;

        if (pSprite->field_3A <= 4)
            goto loc_2AD25;

        if (Troop.mSprite->mRowsToSkip >= 5)
            goto loc_2AD2D;

        if (Troop.mSprite->mInWater)
            goto loc_2AD2D;


        Data0 = Troop.mSprite->field_32;
        if (word_3BED5[Data0]) {
        loc_2AD25:;
            Data0 = 0;
            goto loc_2AD36;
        }

    loc_2AD2D:;
        Data0 = tool_RandomGet() & 7;

    loc_2AD36:;
        if (!Data0)
            goto loc_2ADC3;
        continue;

    loc_2AD3D:;
        Data0 = pDataC;
        Data0 -= pData8;
        Data0 >>= 1;
        Data0 += pData8;

        Data4 = pData14;
        Data4 -= pData10;
        Data4 >>= 1;
        Data4 += pData10;

        Sprite_Direction_Between_Points(Troop.mSprite, Data0, Data4);

        Troop.mSprite->mDirection += 0x100;
        Troop.mSprite->mDirection &= 0x1FE;

    loc_2ADC3:; // Hit
        if (Troop.mSprite->mSpriteType)
            goto loc_2ADFF;

        Troop.mSprite->mDirection = pSprite->mDirection;
        Troop.mSprite->field_12 = pSprite->mDirection;

    loc_2ADFF:;
        Troop.mSprite->mAnimState = eSprite_Anim_Hit;
        Troop.mSprite->field_64 = -1;
        mSprites_Found_Count += 1;

        if (word_3AA45)
            break;
    }

    word_3AA45 = 0;
    return mSprites_Found_Count;
}

void cFodder::Squad_Rebuild() {
    sSprite** Data34 = mSprite_TroopsAlive;

    mSquads_TroopCount[0] = 0;
    mSquads_TroopCount[1] = 0;
    mSquads_TroopCount[2] = 0;
    mSquads_TroopCount[3] = 0;

    int16 TotalTroops = 0;

    mSquad_0_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_1_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_2_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_3_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_4_Sprites[0] = INVALID_SPRITE_PTR;

    // Loop through all mission troops
    for( auto& Troop : mGame_Data.mSoldiers_Allocated ) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        sSprite* Sprite = Troop.mSprite;

        // Sprite is not invincible
        if (!(Sprite->field_75 & eSprite_Flag_Invincibility)) {

            // Is dying
            if (Sprite->mAnimState < eSprite_Anim_Slide1) {
                if (Sprite->mAnimState) {

                    // Not in vehicle
                    if (!Sprite->mInVehicle)
                        continue;

                    Sprite->mAnimState = eSprite_Anim_None;
                }
            }
        }
        //loc_2D165
        if (Sprite->field_32 < 0)
            continue;

        uint8 Data10 = mSquads_TroopCount[Sprite->field_32] & 0xFF;
        mSquads_TroopCount[Sprite->field_32] += 1;
        ++TotalTroops;

        sSprite** Data30 = mSquads[Sprite->field_32];
        //seg009:0151
        Data30[Data10] = Sprite;
        Data30[Data10 + 1] = INVALID_SPRITE_PTR;

        *Data34 = Sprite;
        ++Data34;
    }
    //seg009:019E

    if (!TotalTroops)
        mPhase_TryAgain = true;

    for (int16 Data0 = 2; Data0 >= 0; --Data0) {
        if (mSquads_TroopCount[Data0])
            continue;

        mSquad_Join_TargetSquad[Data0] = -1;
    }

    *Data34 = INVALID_SPRITE_PTR;
}

void cFodder::Squad_Member_CanFire() {
    if (mSquad_Member_Fire_CoolDown_Override)
        mSquad_Member_Fire_CoolDown_Override = false;
    else {
        mSquad_Member_Fire_CoolDown -= 1;
        if (mSquad_Member_Fire_CoolDown >= 0)
            return;
    }

    //loc_2D2EB
    sSprite* Data20 = mSquad_Leader;
    if (Data20 == INVALID_SPRITE_PTR || Data20 == 0)
        return;

    sMission_Troop* Dataa20 = Data20->mMissionTroop;
    if (Dataa20 == 0)
        return;

    mSquad_Member_Fire_CoolDown = mSprite_Weapon_Data.mCooldown;
    mSprite_Bullet_Time_Modifier = 0;
    mSprite_Bullet_Fire_Speed_Modifier = 0;

    // TODO: 0x0F is the max rank, so what was this code for?
    {
        if (Dataa20->mRank > 0x14) {
            if (mSquad_Selected >= 0) {
                if (mSquads_TroopCount[mSquad_Selected] == 1) {
                    mSquad_Member_Fire_CoolDown = 1;
                    mSprite_Bullet_Time_Modifier = -3;
                    mSprite_Bullet_Fire_Speed_Modifier = 0x0A;
                }
            }
        }
    }

    //loc_2D381
    if (mSquad_Selected < 0)
        return;

    int16 TroopsInSquad = (mSquads_TroopCount[mSquad_Selected] & 0xFF) - 1;
    int16 LastTroopNumber = TroopsInSquad;

    if (LastTroopNumber < 0)
        return;

    // Order based on number of troops in squad
    const std::vector<int16>& Rotate_Fire_Order = mSoldier_Squad_Fire_RotationOrder[TroopsInSquad];
    if (mTroop_Rotate_Next >= Rotate_Fire_Order.size())
        mTroop_Rotate_Next = 0;

    size_t Data0 = mTroop_Rotate_Next;

    // Squad Member Number
    int16 Data4 = Rotate_Fire_Order[Data0];

    // Past the last troop in squad?
    if (Data4 < 0 || Data4 > LastTroopNumber) {
        Data4 = 0;
        Data0 = 0;
    }

    // Next Troop in Squad
    Data0++;
    if (Rotate_Fire_Order[Data0] < 0)
        Data0 = 0;

    mTroop_Rotate_Next = Data0;

    // Can the squad member use their weapon
    Data20 = mSquads[mSquad_Selected][Data4];
    if (Data20->mActionCooldown)
        return;

    // Soldier fires weapon
    Data20->mWeaponFireTimer = -1;
}

void cFodder::Squad_Prepare_Explosives() {
    mSquad_Grenades[0] = mGame_Data.mGamePhase_Data.mSoldiers_Available << 1;
    mSquad_Rockets[0] = mGame_Data.mGamePhase_Data.mSoldiers_Available;

    if (mVersionCurrent->isCannonFodder1()) {
        if (mVersionCurrent->isAmigaTheOne()) {
            mSquad_Grenades[0] += mGame_Data.mGamePhase_Data.mSoldiers_Available;

            if (mGame_Data.mMission_Number == 1 && mGame_Data.mMission_Phase < 3)
                mSquad_Grenades[0] = 0;

            mSquad_Rockets[0] = 0;
        }
        else {

            if (mGame_Data.mMission_Number < 4 || (mGame_Data.mMission_Number == 4 && mGame_Data.mMission_Phase < 2))       // Original CF: Map 5
                mSquad_Grenades[0] = 0;

            if (mGame_Data.mMission_Number < 5 || (mGame_Data.mMission_Number == 5 && mGame_Data.mMission_Phase < 2))       // Original CF: Map 10
                mSquad_Rockets[0] = 0;
        }
    }
    else {
        if (mGame_Data.mMission_Number < 3)       // Original CF2: Map 5
            mSquad_Grenades[0] = 0;

        if (mGame_Data.mMission_Number < 4 || (mGame_Data.mMission_Number == 4 && mGame_Data.mMission_Phase < 2))       // Original CF2: Map 10
            mSquad_Rockets[0] = 0;
    }

    // If grenades/rockets are provided in the campaign data, override with those
    if (mGame_Data.mPhase_Current->mGrenades != -1)
        mSquad_Grenades[0] = mGame_Data.mPhase_Current->mGrenades;

    if (mGame_Data.mPhase_Current->mRockets != -1)
        mSquad_Rockets[0] = mGame_Data.mPhase_Current->mRockets;
}

int16 cFodder::Squad_Join(sSprite* pSprite) {
    int16 Data14 = pSprite->field_32;
    int16 Data0;

    int16 Data18 = mSquad_Join_TargetSquad[Data14];
    if (Data18 < 0)
        return -1;

    Data18 &= 0xFF;

    if (mSquads_TroopCount[Data14] > 8)
        return -1;

    sMission_Troop* Dataa24 = pSprite->mMissionTroop;
    Dataa24->mSelected &= 0xFE;

    sMapTarget* Data24 = mSquad_WalkTargets[Data18];
    Data0 = 0;

    for (;;) {

        sMapTarget eax = *Data24++;
        if (eax.asInt < 0)
            break;

        Data0++;
    }

    pSprite->mNextWalkTargetIndex = Data0;
    pSprite->field_32 = Data18;
    Squad_Rebuild();

    mSquad_Selected = Data18;

    mSquad_Grenades[Data18] += mSquad_Grenades[Data14];
    mSquad_Rockets[Data18] += mSquad_Rockets[Data14];

    //seg010:0232
    mSquad_Grenades[Data14] = 0;
    mSquad_Rockets[Data14] = 0;

    Squad_UpdateLeader(pSprite);
    mGUI_Sidebar_Setup = 0;

    return -1;
}

int16 cFodder::Squad_Join_Check(sSprite* pSprite) {

    if (mSquad_Join_TargetSquad[pSprite->field_32] < 0)
        return -1;

    sSprite* Dataa2C = mSquad_Join_TargetSprite[pSprite->field_32];
    if (Dataa2C->mSpriteType != eSprite_Player)
        return -1;

    sSprite** Data30 = mSquads[mSquad_Selected];
    int16 MaxDistance = 0;

    for (MaxDistance = 0x0C;;) {
        if (*Data30 == INVALID_SPRITE_PTR)
            break;

        sSprite* eax = *Data30++;

        if (pSprite == eax)
            break;
        MaxDistance += 8;
    }

    int16 Data0 = Dataa2C->mPosX;
    int16 Data4 = Dataa2C->mPosY;
    int16 Data8 = pSprite->mPosX;
    int16 DataC = pSprite->mPosY;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= MaxDistance)
        return Squad_Join(pSprite);

    return 0;
}

void cFodder::Squad_Walk_Target_Reset(int16 pData0) {
    mSquad_WalkTargets[pData0]->asInt = -1;

    sSprite** Data24 = mSquads[pData0];

    for (;;) {

        if (*Data24 == INVALID_SPRITE_PTR)
            break;

        sSprite* Data28 = *Data24++;
        Data28->mNextWalkTargetIndex = 0;
        Data28->mTargetX = Data28->mPosX;
        Data28->mTargetY = Data28->mPosY;
    }

}

void cFodder::Squad_Select(int16 pData4, bool pCheckMouse) {

    if (pCheckMouse && Mouse_Button_Left_Toggled() < 0)
        return;

    mSquad_Selected = pData4;
    Mission_Troops_Clear_Selected();
    Squad_Rebuild();

    sSprite* Data20 = 0;
    Squad_UpdateLeader(Data20);

    if (Data20 == INVALID_SPRITE_PTR)
        return;

    mCamera_Panning_ToTarget = false;
    mCamera_StartPosition_X = Data20->mPosX;
    mCamera_StartPosition_Y = Data20->mPosY;
    mCamera_Start_Adjust = true;
    mSquad_Select_Timer = 1;
    mGUI_Sidebar_Setup = 0;
    mSquad_Grenade_SplitMode = eSquad_Weapon_Split_Half;
    mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;
    word_3AC4B = 0;
    word_3AC4D = 0;
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

void cFodder::Squad_Split_Assets() {
    int16 Data8, DataC;

    if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_All) {
        mSquad_Grenades[mSquad_Selected] = mSquad_Grenades[mSquad_JoiningTo];
        mSquad_Grenades[mSquad_JoiningTo] = 0;
    }
    else {

        if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_Half) {
            Data8 = mSquad_Grenades[mSquad_JoiningTo];
            DataC = Data8;
            Data8 >>= 1;

            mSquad_Grenades[mSquad_JoiningTo] = Data8;
            DataC -= Data8;
            mSquad_Grenades[mSquad_Selected] = DataC;
        }
    }

    if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_All) {
        mSquad_Rockets[mSquad_Selected] = mSquad_Rockets[mSquad_JoiningTo];
        mSquad_Rockets[mSquad_JoiningTo] = 0;
    }
    else {

        if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_Half) {

            Data8 = mSquad_Rockets[mSquad_JoiningTo];
            DataC = Data8;
            Data8 >>= 1;

            mSquad_Rockets[mSquad_JoiningTo] = Data8;
            DataC -= Data8;
            mSquad_Rockets[mSquad_Selected] = DataC;
        }
    }

    mSquad_CurrentWeapon[mSquad_Selected] = mSquad_CurrentWeapon[mSquad_JoiningTo];
}

void cFodder::Squad_Member_Click_Check() {
    word_3A3BF = -1;

    if (mSquad_Selected < 0)
        return;

    int16 ClickedX = mMouseX;
    int16 ClickedY = mMouseY;

    ClickedX += mCameraX >> 16;
    ClickedY += mCameraY >> 16;
    ClickedX -= 0x0F;
    ClickedY -= 3;

    for (auto& SquadMember : mGame_Data.mSoldiers_Allocated) {

        if (SquadMember.mSprite == INVALID_SPRITE_PTR || SquadMember.mSprite == 0)
            continue;

        sSprite* SquadMemberSprite = SquadMember.mSprite;

        if (SquadMemberSprite->field_32 == mSquad_Selected)
            ++word_3A3BF;

        int16 SquadMemberX = SquadMemberSprite->mPosX + 4;
        int16 SquadMemberY = SquadMemberSprite->mPosY - SquadMemberSprite->mHeight;

        // Clicked before/after troop?
        if (ClickedX < SquadMemberX)
            continue;
        if (ClickedX > SquadMemberX + 6)
            continue;

        // Clicked above/below troop?
        if (ClickedY < SquadMemberY - 13)
            continue;
        if (ClickedY > SquadMemberY)
            continue;

        // 
        if (mSquad_Selected == SquadMemberSprite->field_32) {
            mSquad_Member_Clicked_TroopInSameSquad = -1;
            mSquad_Member_Clicked_TroopPtr = SquadMemberSprite->mMissionTroop;
            return;
        }

        // Two squads joined will have more than 8 members?
        if (mSquads_TroopCount[SquadMemberSprite->field_32] + mSquads_TroopCount[mSquad_Selected] > 8)
            return;

        mSquad_Join_TargetSprite[mSquad_Selected] = SquadMemberSprite;
        mSquad_Join_TargetSquad[mSquad_Selected] = (int8)SquadMemberSprite->field_32;

        Squad_Walk_Target_Reset(SquadMemberSprite->field_32);

        break;
    }
}

void cFodder::Squad_Switch_Weapon() {

    if (!mInput_Enabled || mPhase_Finished)
        return;

    if (mSquad_CurrentWeapon[mSquad_Selected] != eWeapon_Rocket) {
        if (mSquad_Rockets[mSquad_Selected])
            Squad_Select_Rockets();
    }
    else {
        if (mSquad_Grenades[mSquad_Selected])
            Squad_Select_Grenades();
    }
}

void cFodder::Squad_Member_Target_Set() {
    sSprite** Data20 = 0;

    if (mSquad_CurrentVehicle) {
        Vehicle_Target_Set();
        return;
    }

    mSprite_Player_CheckWeapon = -1;
    if (word_3A9B8 < 0) {
        word_3A9B8 = 0x30;

        if (mSquad_Leader)
            mSquad_Leader->mWeaponFireTimer = 0;
    }

    int16 TargetX = mMouseX + (mCameraX >> 16);
    int16 TargetY = mMouseY + (mCameraY >> 16);

    TargetX -= 0x10;
    if (!TargetX)
        TargetX = 1;

    TargetY -= 8;
    if (mSquad_Selected < 0)
        goto loc_30E05;

    if ((mSquads_TroopCount[mSquad_Selected] - 1) < 0)
        goto loc_30E05;

    Data20 = mSquads[mSquad_Selected];

    for (;;) {
        if (*Data20 == INVALID_SPRITE_PTR || *Data20 == 0)
            break;

        sSprite* Data24 = *Data20++;

        Data24->mWeaponTargetX = TargetX;
        Data24->mWeaponTargetY = TargetY;
    }

loc_30E05:;
    Squad_Member_CanFire();
}

void cFodder::Squad_Switch_Timer() {

    if (!mSquad_Select_Timer)
        return;

    --mSquad_Select_Timer;
    if (mSquad_Select_Timer)
        return;

    if (mSquad_Selected >= 0)
        return;

    for (int16 Data0 = 2; Data0 >= 0; --Data0) {

        if (mSquads_TroopCount[Data0]) {
            Squad_Switch_To(Data0);
            return;
        }

    }
}

void cFodder::Squad_Switch_To(int16 pData0) {

    if (!mSquads_TroopCount[pData0])
        return;

    mSquad_Selected = pData0;
    sSprite* Data20 = 0;

    if (Squad_UpdateLeader(Data20) >= 0) {
        mCamera_StartPosition_X = Data20->mPosX;
        mCamera_StartPosition_Y = Data20->mPosY;
        mCamera_Start_Adjust = true;
        mMouse_Locked = false;
    }

    mGUI_Sidebar_Setup = 0;
}

int16 cFodder::Squad_UpdateLeader(sSprite*& pData20) {
    sSprite** Data30 = 0;
    int16 Data0 = mSquad_Selected;

    if (Data0 < 0)
        goto loc_306AD;

    Data30 = mSquads[Data0];
    if (*Data30 == INVALID_SPRITE_PTR || *Data30 == 0)
        goto loc_306AD;

    do {
        if (*Data30 == INVALID_SPRITE_PTR || *Data30 == 0)
            goto loc_306AD;

        pData20 = *Data30;
        ++Data30;
        //seg011:1B61
        if (pData20->mSpriteType != eSprite_Player)
            continue;

        if (pData20->field_75 & eSprite_Flag_Invincibility)
            goto loc_3066E;

        if (pData20->mAnimState) {
            if (pData20->mAnimState < eSprite_Anim_Slide1)
                goto loc_30662;
        }
        //loc_30656
        if (!pData20->mIsSinking)
            goto loc_30681;

    loc_30662:;
        if (!pData20->mInVehicle)
            continue;

    loc_3066E:;
        pData20->mAnimState = eSprite_Anim_None;
        pData20->mIsSinking = 0;
    loc_30681:;

        if (!pData20->mInVehicle)
            break;

    } while (*Data30 != INVALID_SPRITE_PTR);
    //loc_3069A
    goto loc_306BE;

loc_306AD:;
    pData20 = INVALID_SPRITE_PTR;
    mSquad_Selected = -1;

loc_306BE:;
    if (mSquad_CurrentVehicle)
        return 1;

    mSquad_Leader = pData20;
    if (pData20 == INVALID_SPRITE_PTR || pData20 == 0)
        return -1;

    return 1;
}

void cFodder::Squad_Assign_Target_From_Mouse() {
    if (mSquad_Selected < 0)
        return;

    sSprite** Data24 = mSquads[mSquad_Selected];

    for (;;) {
        if (*Data24 == INVALID_SPRITE_PTR)
            break;
        sSprite* Data28 = *Data24++;
        Data28->mVehicleWalkTarget = 0;
    }

    int16 Data0 = mMouseX;
    int16 Data4 = mMouseY;

    Data0 += mCameraX >> 16;
    Data4 += mCameraY >> 16;

    Data0 -= 0x0F;
    Data4 -= 3;

    for(auto& Sprite : mSprites) {

        if (Sprite.mPosX == -32768)
            continue;

        if (!Sprite.mVehicleEnabled)
            continue;

        if (Sprite.mPersonType != eSprite_PersonType_Human)
            continue;

        if (Sprite.mHeight)
            continue;

        int16 Data18 = Sprite.mSpriteType;
        int16 Data8 = Sprite.mPosX;

        if (Sprite.mVehicleType == eVehicle_Turret_Cannon ||
            Sprite.mVehicleType == eVehicle_Turret_Missile)
            Data8 -= 8;

        if (Data0 < Data8)
            continue;

        Data8 += mSprite_Width[Data18];
        if (Data0 > Data8)
            continue;

        Data8 = Sprite.mPosY - mSprite_Height_Top[Data18];
        Data8 -= 0x14;
        if (Data4 < Data8)
            continue;

        Data8 = Sprite.mPosY + mSprite_Height_Bottom[Data18];
        if (Data4 > Data8)
            continue;

        if (mSquad_Selected < 0)
            break;

        Data24 = mSquads[mSquad_Selected];
        for (;;) {
            if (*Data24 == INVALID_SPRITE_PTR)
                goto loc_313C6;

            sSprite* Data28 = *Data24++;
            Data28->mVehicleWalkTarget = &Sprite;
        }
    }

loc_313C6:;
}
