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

void cFodder::Sprite_Handle_Civilian_Unk(sSprite* pSprite) {
    if (!pSprite->mWasDrawn)
        return;

    if ((tool_RandomGet() & 0x3F) != 9)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    pSprite->mWeaponTargetX = mSquad_Leader->mPosX;
    pSprite->mWeaponTargetY = mSquad_Leader->mPosY;

    Sprite_Create_Civilian_Spear2(pSprite);
}

void cFodder::Sprite_Handle_Civilian_Movement(sSprite* pSprite) {

    // Destination change cool down
    if (pSprite->mRetargetCooldown) {
        pSprite->mRetargetCooldown -= 1;
        if (pSprite->mRetargetCooldown)
            return;
    }

    if (pSprite->mSpriteType == eSprite_Hostage || pSprite->mSpriteType == eSprite_Enemy_Leader) {
        pSprite->mTargetX = pSprite->mPosX;
        pSprite->mTargetY = pSprite->mPosY;
    }
    else {
        if (Sprite_Handle_Civilian_RandomMovement(pSprite) < 0)
            return;
    }

    // Random time until destination change
    int16 Data0 = tool_RandomGet() & 0x3F;
    Data0 += 0x14;
    pSprite->mRetargetCooldown = static_cast<int8>(Data0);
}

int16 cFodder::Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(sSprite* pSprite) {

    if (!mSprite_OpenCloseDoor_Ptr)
        return -1;

    if (pSprite->mRetargetCooldown) {
        --pSprite->mRetargetCooldown;
        return 0;
    }

    sSprite* Data28 = mSprite_OpenCloseDoor_Ptr;

    int16 Data0 = pSprite->mPosX;
    int16 Data4 = pSprite->mPosY;
    int16 Data8 = Data28->mPosX;
    int16 DataC = Data28->mPosY;

    pSprite->mTargetX = Data8;
    pSprite->mTargetY = DataC;

    mSprite_Civilian_Tmp_X = Data0;
    mSprite_Civilian_Tmp_Y = Data4;
    word_3B481 = Data8;
    word_3B483 = DataC;
    int16 Data10 = 0x20;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 3) {
        Sprite_Destroy_Wrapper(pSprite);
        mSprite_Civilian_GotHome = -1;
        return 1;
    }

    if (Data0 < 6) {
        Data28->field_2A = 0x0A;
        return 0;
    }

    Data0 = mSprite_Civilian_Tmp_X;
    Data4 = mSprite_Civilian_Tmp_Y;
    Data8 = word_3B481;
    DataC = word_3B483;

    if (!Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC))
        return 0;

	// Move toward top left
    Data0 = tool_RandomGet() & 0x3F;
    if (Data0)
        return 0;

    pSprite->mTargetX = pSprite->mPosX;
    pSprite->mTargetY = pSprite->mPosY;
    pSprite->mTargetX -= 0x10;
    pSprite->mTargetY -= 0x20;
    pSprite->mRetargetCooldown = 0x1E;

    return 0;
}

void cFodder::Sprite_Handle_Civilian_Death(sSprite* pSprite) {

    if (pSprite->mSheetIndex != 0xD6) {
        word_3B2D1[2] = -1;

        if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Protect_Civilians - 1])
            mPhase_Aborted = true;

        if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Get_Civilian_Home - 1])
            mPhase_Aborted = true;

        pSprite->mSheetIndex = 0xD6;
        pSprite->mFrameIndex = 0;

        int16 Data0 = tool_RandomGet() & 7;
        int16 Data4 = mSprite_Civilian_Sound_Death[Data0];

        Sound_Play(pSprite, Data4, 0x14);
    }
    else {
        //loc_2584A
        pSprite->mFrameIndex += 1;
        if (pSprite->mFrameIndex < 9)
            return;

        Sprite_Destroy_Wrapper(pSprite);
    }
}

int16 cFodder::Sprite_Handle_Civilian_RandomMovement(sSprite* pSprite) {
    int16 Data8 = map_GetRandomX();
	Data8 += 4;
    if (Data8 >= mMapLoaded->getWidth())
        return -1;

	int16 DataC = map_GetRandomY();
	DataC += 4;
    if (DataC >= mMapLoaded->getHeight())
        return -1;

    Data8 <<= 4;
    Data8 += tool_RandomGet() & 0x0F;

    DataC <<= 4;
    DataC += tool_RandomGet() & 0x0F;

    int16 X, Y;

    if (Map_Terrain_GetMoveable_WithCache(mTiles_NotFlyable, Data8, DataC, X, Y))
        return -1;

    pSprite->mTargetX = X;
    pSprite->mTargetY = Y;
    return 0;
}

void cFodder::Sprite_Civilian_Update_Movement(sSprite* pSprite) {
    Sprite_XY_Store(pSprite);

    if (pSprite->field_44)
        --pSprite->field_44;
    else {
        int16 Data0 = pSprite->mTargetX;
        int16 Data4 = pSprite->mTargetY;

        if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) < 0)
            mSprite_Reached_Target = -1;
    }

    if (pSprite->mSpriteType != eSprite_Seal) {
        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 0x0E;
        if (Data4 < 0)
            Data0 = -Data0;

        pSprite->mDirection += Data0;
        pSprite->mDirection &= 0x1FE;
    }
    //loc_259BE
    if (!Sprite_Reached_Target(pSprite)) {
        Sprite_Movement_Speed_Update(pSprite);

        if (!word_3B25B)
            Sprite_Movement_Calculate(pSprite);
    }
    //loc_259D4
    Sprite_Reached_MapEdge(pSprite);

    int16 Data4 = 0;
    Sprite_Terrain_Check(pSprite, Data4);

    Data4 = pSprite->mTerrainType;

    if (Data4 == 6) {
        Sprite_XY_Restore(pSprite);
        return;
    }

    if (Data4 != 4) {
        if (Data4 != 5)
            return;

        pSprite->mRowsToSkip = 5;
        return;
    }

    pSprite->mRowsToSkip = 3;
}

void cFodder::Sprite_WalkAnim_Tick(sSprite* pSprite) {

    if (mSprite_Reached_Target || word_3B25B)
        return;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 3;
    if (pSprite->field_2A)
        return;

    pSprite->mFrameIndex += 1;
    pSprite->mFrameIndex &= 1;
}

void cFodder::Sprite_WalkFrame_From_Direction(sSprite* pSprite) {
    int16 Data0 = pSprite->mDirection;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;

    int16 Data4 = pSprite->mFrameIndex;
    Data4 &= 1;
    Data0 |= Data4;
    pSprite->mFrameIndex = Data0;
}

void cFodder::Sprite_Hostage_Update_IdleFrame(sSprite* pSprite) {

    if (word_3B2ED) {
        pSprite->mSheetIndex = 0xD8;
        pSprite->mFrameIndex = 1;
        Sprite_XY_Restore(pSprite);
        return;
    }

    if (word_3B2D1[2] != pSprite->mPosX || word_3B2D1[3] != pSprite->mPosXFrac)
        return;

    if (word_3B2D1[4] != pSprite->mPosY || word_3B2D1[5] != pSprite->mPosYFrac)
        return;

    pSprite->mSheetIndex = 0xDA;

    int16 Data0 = mMission_EngineTicks & 0x0F;
    if (!Data0) {
        Data0 = tool_RandomGet() & 0x06;
        pSprite->field_32 = mSprite_Hostage_Frames[Data0 / 2];
    }

    pSprite->mFrameIndex = pSprite->field_32;
}

void cFodder::Sprite_Advance_SearchIndex(sSprite* pSprite) {
    ++pSprite->mSpriteScanIndex;
    if (pSprite->mSpriteScanIndex >= (mParams->mSpritesMax - 2))
		pSprite->mSpriteScanIndex = 0;
}

void cFodder::Sprite_CivilianDoor_SpawnUpdate(sSprite* pSprite) {
    int16 Data0, Data4;

    if (!Sprite_Update_Die2_Explosion_IceAware(pSprite))
        goto loc_264CF;

    if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Protect_Civilians - 1])
        return;

    mPhase_Aborted = true;
    return;

loc_264CF:;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->mSheetIndex = 0x9B;
        return;
    }

    pSprite->mDrawOrder = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_26580;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A)
        goto loc_265B1;

    if (pSprite->field_43 >= 0) {
        pSprite->mSheetIndex = 0x7C;
        return;
    }

    pSprite->mSheetIndex = 0x9B;
    Sound_Play(pSprite, eSound_Effect_BuildingDoor2, 1);

    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14;
    Data4 -= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
    if (Data4 < 0)
        Data0 = 0;

    Data4 <<= 3;
    Data4 += Data0;
    pSprite->field_12 = Data4;

loc_26580:;
    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->mSheetIndex = 0x7C;

loc_265B1:;
    sSprite* Data2C = 0, *Data30 = 0;

    if (!Sprite_Create_Native(pSprite, Data2C, Data30)) {
        Data2C->mPosY -= 4;
        return;
    }

    pSprite->field_12 = 0;
    pSprite->field_43 = -1;
}

int16 cFodder::Sprite_Create_Civilian_Spear2(sSprite* pSprite) {
    if (mPhase_Completed_Timer)
        return -1;

    if (!pSprite->mWeaponTargetX)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mSheetIndex = 0xD7;
    Data2C->mFrameIndex = 0;
    Data2C->field_12 = 9;
    Data0 = pSprite->mAIAggression;

    Data0 >>= 3;
    Data0 += 8;
    if (Data0 > 0x10)
        Data0 = 0x10;

    Data2C->field_12 = Data0;
    Data2C->mSpriteType = eSprite_Civilian_Spear2;
    Data2C->mHeightFixed = pSprite->mHeightFixed;
    Data2C->mHeightFixed += 0x60000;

    Data2C->mRowsToSkip = pSprite->mRowsToSkip;
    Data2C->mPersonType = pSprite->mPersonType;
    Data2C->mDrawOrder = eSprite_Draw_Second;
    Data0 = 0x32;
    Data0 += pSprite->mAIAggression;

    Data2C->mWeaponFireTimer = Data0;
    Data2C->mSpeed = 0x18;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data2C->mWeaponTargetX = pSprite->mWeaponTargetX;
    Data2C->mWeaponTargetY = pSprite->mWeaponTargetY;
    Data2C->mWeaponTargetX += 7;
    Data2C->mRowsToSkip = 0;
    Data2C->field_3A = 0;
    Data2C->mOwnerSprite = pSprite;
    Data2C->field_2A = 2;
    Data2C->mProjectileOffsetX = 0;
    Data2C->mProjectileOffsetY = 2;

    if (Data2C->mFrameIndex == 4)
        Data2C->mProjectileOffsetY = -1;

    Data2C->mInitialDirection = -1;
    Data2C->field_50 = 0;
    Data2C->mDirectionOverride = 0;

    int16 Data8 = 7;
    Data0 = tool_RandomGet();
    int16 Data4 = Data0;
    Data0 &= Data8;

    if (Data4 < 0)
        Data0 = -Data0;

    Data2C->field_50 = Data0;

    if (mMapLoaded->getTileType() == eTileTypes_Moors) {
        Data0 = tool_RandomGet() & 1;
        if (Data0)
            goto loc_25D9D;
        Data4 = 0x11;
        goto loc_25DA3;

    loc_25D9D:;
        Data4 = 0x10;

    loc_25DA3:;
        Sound_Play(pSprite, Data4, 0);
    }

    //loc_25DB1
    Data2C->field_64 = 0;
    return 0;
}

void cFodder::Sprite_Handle_Hostage_Movement(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data28 = 0;

    if (pSprite->mSpriteType == eSprite_Enemy_Leader)
        goto CheckRescueTent;

    // No known rescue tent?
    if (!mHostage_Rescue_Tent)
        goto loc_2608B;

    if (pSprite->mHeliRestartFrame) {
        pSprite->mHeliRestartFrame -= 1;
        goto loc_2608B;
    }
    //loc_2600B

    pSprite->mHeliRestartFrame = -56;

    Data8 = mHostage_Rescue_Tent->mPosX + 10;
    DataC = mHostage_Rescue_Tent->mPosY - 5;
    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;
    Data10 = 0x80;

    // Distance to rescue tent < 127?
    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data10 < 0x7F)
        pSprite->mRescueTentIndex = (int16)(mHostage_Rescue_Tent - mSprites.data());

loc_2608B:;
    word_3B2ED = 0;
    Data0 = eSprite_Hostage_Rescue_Tent;
    Data4 = eSprite_Enemy;
    Data8 = eSprite_Player;
    DataC = -1;
    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        return;

    // Found Tent Nearby
    if (Data4 == eSprite_Hostage_Rescue_Tent) {

        pSprite->mTargetX = Data28->mPosX + 10;
        pSprite->mTargetY = Data28->mPosY - 5;

        Data0 = pSprite->mPosX;
        Data4 = pSprite->mPosY;
        Data8 = pSprite->mTargetX;
        DataC = pSprite->mTargetY;
        Data10 = 0x10;
        Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
        if (Data10 >= 0x10)
            goto loc_2620F;

        if (Data0 >= 3)
            return;

        mHostage_Count -= 1;
        Sprite_Destroy_Wrapper(pSprite);
        return;

        // Found Enemy near by
    }
    else if (Data4 == eSprite_Enemy) {
        Data28->mFollowTarget = pSprite;
        pSprite->mTargetX = pSprite->mPosX;
        pSprite->mTargetY = pSprite->mPosY;
        word_3B2ED = -1;
        return;
    }

    pSprite->mTargetX = Data28->mPosX + 4;
    pSprite->mTargetY = Data28->mPosY - 6;

    // Not in vehicle?
    if (!Data28->mInVehicle)
        goto loc_2620F;

    // Vehicle Has Speed?
    if (Data28->mCurrentVehicle->mSpeed > 2)
        goto loc_2620F;

    // Vehicle has height?
    if (Data28->mCurrentVehicle->mHeight > 3)
        goto loc_2620F;

    Data0 = Data28->mCurrentVehicle->mPosX;
    Data4 = Data28->mCurrentVehicle->mPosY;
    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosY;
    Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 > 0x0A) {
    loc_2620F:;
        Data0 = tool_RandomGet() & 0x3F;
        if (Data0)
            return;

        Sprite_Advance_SearchIndex(pSprite);
        return;
    }

    pSprite->mInVehicle = -1;
    return;

CheckRescueTent:;
    word_3B2ED = 0;
    Data0 = eSprite_Player;
    Data4 = eSprite_Hostage_Rescue_Tent;
    Data8 = -1;
    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        return;

    if (Data4 == eSprite_Hostage_Rescue_Tent) {

        pSprite->mTargetX = Data28->mPosX + 0x0A;
        pSprite->mTargetY = Data28->mPosY - 0x05;

        Data0 = pSprite->mPosX;
        Data4 = pSprite->mPosY;
        Data8 = pSprite->mTargetX;
        DataC = pSprite->mTargetY;
        Data10 = 0x10;
        Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
        if (Data0 >= 3)
            return;
        mHostage_Count -= 1;

        Sprite_Destroy(pSprite + 1);
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    // Another Player
    word_3B2ED = -1;
    pSprite->mSheetIndex = 0xD8;
    pSprite->mTargetX = Data28->mPosX + 4;
    pSprite->mTargetY = Data28->mPosY - 6;

    // Not in vehicle?
    if (!Data28->mInVehicle)
        goto loc_263E5;

    // Vehicle has speed?
    if (Data28->mCurrentVehicle->mSpeed > 2)
        goto loc_263E5;

    // Vehicle has height
    if (Data28->mCurrentVehicle->mHeight > 3)
        goto loc_263E5;

    Data0 = Data28->mCurrentVehicle->mPosX;
    Data4 = Data28->mCurrentVehicle->mPosY;
    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosY;
    Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 0x0F) {
        pSprite->mInVehicle = -1;
        return;
    }

loc_263E5:;
    Data0 = tool_RandomGet() & 7;
    if (Data0)
        return;

    Sprite_Advance_SearchIndex(pSprite);
}

void cFodder::Sprite_Handle_Hostage_FrameUpdate2(sSprite* pSprite) {
    int16 Data0 = pSprite->mWeaponFireTimer;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (!pSprite->field_2A)
        Data0 += 2;

    Data0 &= 6;

    pSprite->mWeaponFireTimer = Data0;
    pSprite->mFrameIndex = mSprite_Hostage_Frames[Data0 / 2];

}

void cFodder::Sprite_Handle_Hostage_FrameUpdate(sSprite* pSprite) {
    int16 Data0 = pSprite->mDirection;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;

    int16 Data4 = Data0;
    Data4 >>= 1;
    Data4 += Data0;

    pSprite->mFrameIndex += Data4;
}

int16 cFodder::Sprite_Create_Native(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30) {
    int16 Data4;

    if (!pSprite->mWasDrawn)
        return -1;

    if (mPhase_Complete)
        return -1;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax)
        return -1;

    int16 Data0 = 1;

    Sprite_Get_Free_Max29(Data0, pData2C, pData30);
    if (Data0)
        return -1;

    pData2C->Clear();
    pData2C->mSpriteType = mSpawnSpriteType;
    pData2C->mPosX = pSprite->mPosX;
    pData2C->mPosX -= 6;
    pData2C->mPosY = pSprite->mPosY;
    pData2C->mPosY += 4;
    pData2C->mSheetIndex = 0x7C;
    pData2C->mWeaponFireTimer = 0;
    pData2C->mPersonType = eSprite_PersonType_Native;

    Data0 = tool_RandomGet();
    Data4 = Data0;
    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pData2C->mDirection = Data0;
    Data4 &= 0x0F;
    Data4 += 8;
    pData2C->field_44 = static_cast<int8>(Data4);
    return 0;
}

void cFodder::Sprite_Handle_Bird2_Left(sSprite* pSprite) {

    pSprite->mSheetIndex = 0x98;
    pSprite->mDrawOrder = eSprite_Draw_OnTop;
    pSprite->field_12 += 1;
    pSprite->field_12 &= 1;
    if (!pSprite->field_12)
        pSprite->mFrameIndex ^= 1;

    pSprite->mPosX -= 2;
    if (pSprite->mPosX < -64)
        pSprite->mPosX = 0x7D0;
}

void cFodder::Sprite_Handle_Civilian(sSprite* pSprite) {
    if (pSprite->mAnimState) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->mPersonType = eSprite_PersonType_Native;
    pSprite->mSheetIndex = 0xD0;

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);

    if (ax) {
        if (ax >= 0)
            return;

        Sprite_Handle_Civilian_Movement(pSprite);
    }

    pSprite->mSpeed = 6;
    Sprite_Civilian_Update_Movement(pSprite);
    Sprite_WalkAnim_Tick(pSprite);
    Sprite_WalkFrame_From_Direction(pSprite);
}

void cFodder::Sprite_Handle_Civilian2(sSprite* pSprite) {

    if (pSprite->mAnimState) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->mPersonType = eSprite_PersonType_Native;
    pSprite->mSheetIndex = 0xD0;

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);
    if (ax > 0)
        return;
    if (ax < 0)
        Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->mSpeed = 0x0A;
    Sprite_Civilian_Update_Movement(pSprite);
    Sprite_WalkAnim_Tick(pSprite);
    Sprite_WalkFrame_From_Direction(pSprite);
    if (!word_3B2D1[1])
        return;

    Sprite_Handle_Civilian_Unk(pSprite);
}

void cFodder::Sprite_Handle_Bird_Left(sSprite* pSprite) {

    pSprite->mSheetIndex = 0xD3;
    pSprite->mDrawOrder = eSprite_Draw_OnTop;

    if (!pSprite->field_12) {

        pSprite->mWeaponCooldown = 8;
        pSprite->field_12 = 1;

        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 3;
        Data0 += 1;

        pSprite->mFrameIndex = Data0;
        if (Data4 < 0)
            pSprite->field_12 = -1;
    }

    int16 Data0 = pSprite->field_12;
    pSprite->mFrameIndex += Data0;

    if (pSprite->mFrameIndex) {
        if (pSprite->mFrameIndex < 5)
            goto loc_1C82D;
    }

    Data0 = -Data0;
    pSprite->field_12 = Data0;

loc_1C82D:;
    bool cf = false;

    if (pSprite->mPosXFrac >= 0 && pSprite->mPosXFrac - 32768 < 0)
        cf = true;

    pSprite->mPosXFrac -= 32768;
    pSprite->mPosX -= 1 + (cf == true ? 1 : 0);


    if (Data0 < 0) {
        if (pSprite->mPosXFrac >= 0 && pSprite->mPosXFrac - 32768 < 0)
            cf = true;
        else
            cf = false;

        pSprite->mPosXFrac -= 32768;
        pSprite->mPosX -= 0 + (cf == true ? 1 : 0);
    }

    if (pSprite->mWasDrawn)
        goto loc_1C8C5;

    if (!pSprite->mWeaponCooldown)
        goto loc_1C87E;

    pSprite->mWeaponCooldown -= 1;
    goto loc_1C8C5;

loc_1C87E:;

    pSprite->mWeaponCooldown = 0x3F;

	Data0 = (tool_RandomGet() & 0x3F) + (mMapTile_TargetX >> 16) + getWindowWidth();
    pSprite->mPosX = Data0;

    Data0 = (tool_RandomGet() & 0xFF) + (mMapTile_TargetY >> 16);
    pSprite->mPosY = Data0;

loc_1C8C5:;

    if (mMapLoaded->getTileType() == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);

}

void cFodder::Sprite_Handle_Bird_Right(sSprite* pSprite) {
    pSprite->mSheetIndex = 0xD4;
    pSprite->mDrawOrder = eSprite_Draw_OnTop;

    if (!pSprite->field_12) {
        pSprite->mWeaponCooldown = 8;
        pSprite->field_12 = 1;

        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 3;
        Data0 += 1;

        pSprite->mFrameIndex = Data0;
        if (Data4 < 0)
            pSprite->field_12 = -1;
    }

    //loc_1C950
    int16 Data0 = pSprite->field_12;
    pSprite->mFrameIndex += Data0;
    if (!pSprite->mFrameIndex || pSprite->mFrameIndex >= 5) {
        Data0 = -Data0;
        pSprite->field_12 = Data0;
    }

    bool cf = false;

    if (pSprite->mPosXFrac < 0 && pSprite->mPosXFrac + 32768 > 0)
        cf = true;

    pSprite->mPosXFrac += 32768;
    pSprite->mPosX += 1 + (cf == true ? 1 : 0);

    if (Data0 < 0) {
        if (pSprite->mPosXFrac < 0 && pSprite->mPosXFrac + 32768 > 0)
            cf = true;
        else
            cf = false;

        pSprite->mPosXFrac += 32768;
        pSprite->mPosX += 0 + (cf == true ? 1 : 0);
    }

    //loc_1C9B0
    if (pSprite->mWasDrawn)
        goto loc_1CA20;

    if (pSprite->mWeaponCooldown) {
        pSprite->mWeaponCooldown -= 1;
        goto loc_1CA20;
    }

    //loc_1C9D3
    pSprite->mWeaponCooldown = 0x3F;
    Data0 = tool_RandomGet() & 0x3F;
    Data0 = -Data0;
    Data0 += mMapTile_TargetX >> 16;

    pSprite->mPosX = Data0;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += mMapTile_TargetY >> 16;
    pSprite->mPosY = Data0;

loc_1CA20:;
    if (mMapLoaded->getTileType() == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);
}

void cFodder::Sprite_Handle_Seal(sSprite* pSprite) {
    if (Sprite_Update_Death_BloodOrSeal(pSprite))
        return;

    pSprite->mPersonType = eSprite_PersonType_Native;
    pSprite->mSheetIndex = 0xD5;

    if (!pSprite->mWeaponTargetX) {
        pSprite->mWeaponTargetX = -1;
        pSprite->mFiredWeaponType = tool_RandomGet() & 0x0F;
    }

    if (mMission_EngineTicks & 1)
        ++pSprite->mFiredWeaponType;

    int16 Data0 = pSprite->mFiredWeaponType & 0x0F;

    Data0 = mSprite_Seal_AnimFrames[Data0];
    pSprite->mFrameIndex = Data0;

    if (mMapLoaded->getTileType() == eTileTypes_Moors) {
        Data0 = tool_RandomGet() & 3;
        if (Data0 != 3) {
            Data0 += 0x23;
            Sprite_Native_Sound_Play(pSprite, Data0);
        }
    }

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX) {
        Sprite_Native_Sound_Play(pSprite, 0x1E);
    }
}

void cFodder::Sprite_Handle_Civilian_Spear(sSprite* pSprite) {
    pSprite->mPersonType = eSprite_PersonType_AI;
    if (pSprite->mAnimState) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->mPersonType = eSprite_PersonType_Native;
    pSprite->mSheetIndex = 0xD0;

    if (mMapLoaded->getTileType() == eTileTypes_Moors)
        Sprite_Native_Sound_Play(pSprite, 0x26);

    if (mMapLoaded->getTileType() == eTileTypes_Int)
        Sprite_Native_Sound_Play(pSprite, 0x1F);

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);
    if (ax > 0)
        return;
    if (ax)
        Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->mSpeed = 0x0C;

    Sprite_Civilian_Update_Movement(pSprite);
    Sprite_WalkAnim_Tick(pSprite);
    Sprite_WalkFrame_From_Direction(pSprite);
    Sprite_Handle_Civilian_Unk(pSprite);
}

void cFodder::Sprite_Handle_Civilian_Spear2(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data1C;

    ++pSprite->field_64;

    if (!pSprite->field_12)
        goto loc_1D0F6;

    pSprite->field_12 -= 1;
    pSprite->field_3A += 1;

    if (pSprite->field_44)
        goto loc_1CFF7;

    Sprite_XY_Store(pSprite);
    Data1C = pSprite->mDirection;

    if (pSprite->mDirectionOverride)
        goto loc_1CF3E;

    Data0 = pSprite->mWeaponTargetX;
    Data4 = pSprite->mWeaponTargetY;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Data0 = pSprite->field_50;

    if (Data0) {
        pSprite->mDirection += Data0;
        pSprite->mDirection &= 0x1FE;
        pSprite->mDirectionOverride = -1;
    }

    if (pSprite->mInitialDirection < 0)
        pSprite->mInitialDirection = pSprite->mDirection;

loc_1CF3E:;
    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1D17A;

    pSprite->mSpeed = pSprite->mWeaponFireTimer;
    if (pSprite->mDirectionOverride)
        goto loc_1D00F;

    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;
    Data8 = pSprite->mWeaponTargetX;
    DataC = pSprite->mWeaponTargetY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    Data4 = pSprite->mSpeed;
    Data4 >>= 3;
    if (Data0 > Data4)
        goto loc_1D00F;

    pSprite->mDrawOrder = eSprite_Draw_Second;
    pSprite->mPosX = pSprite->mWeaponTargetX;
    pSprite->mPosY = pSprite->mWeaponTargetY;
    pSprite->field_44 = -1;
    goto loc_1D00F;
loc_1CFF7:;
    pSprite->mDrawOrder = eSprite_Draw_First;
    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1D17A;

loc_1D00F:;
    if (pSprite->field_64 > 2) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
            goto loc_1D0F6;
    }

    if (Sprite_Projectile_Collision_Check(pSprite))
        goto loc_1D07B;

    if (pSprite->mSheetIndex != 0xD7)
        return;

    Data0 = pSprite->mDirection;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    Data0 >>= 1;
    pSprite->mFrameIndex = Data0;
    return;
loc_1D07B:;

    if (mSprites_Found_Count == 1)
        goto loc_1D0F6;

    pSprite->field_43 = 1;
    pSprite->mDrawOrder = eSprite_Draw_OnTop;
    pSprite->mPosY -= 5;
    if (pSprite->mPosY < 0)
        goto loc_1D18D;
    pSprite->mSheetIndex = 0x96;
    pSprite->mFrameIndex = 0;

    Sound_Play(pSprite, eSound_Effect_Spear, 0x0F);
    return;

loc_1D0CB:;
    pSprite->mSheetIndex = 0x96;
    pSprite->field_12 = 0;
    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex >= 4)
        goto loc_1D17A;
    return;

loc_1D0F6:;
    if (pSprite->field_43)
        goto loc_1D14D;

    pSprite->field_43 = -1;
    pSprite->mDrawOrder = eSprite_Draw_First;
    pSprite->mPosY -= 5;
    if (pSprite->mPosY < 0)
        goto loc_1D18D;

    pSprite->mFrameIndex = 3;
    Sound_Play(pSprite, eSound_Effect_Spear, 0x0F);
loc_1D14D:;
    if (pSprite->field_43 >= 0)
        goto loc_1D0CB;

    pSprite->field_12 = 0;
    pSprite->mSheetIndex = 0x7F;
    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex >= 8)
        goto loc_1D17A;
    return;

loc_1D17A:;
    --mSprite_Projectile_Counters[2];
    Sprite_Destroy(pSprite);
    return;

loc_1D18D:;
    pSprite->mPosX = 0;
    pSprite->mPosY = 0x1000;
    goto loc_1D17A;
}

void cFodder::Sprite_Handle_Hostage(sSprite* pSprite) {
    int16 Data0, Data4;
    int16 Sprite_Field_10;
    sSprite* Data24 = 0;

    if (pSprite->mInVehicle)
        goto loc_1D349;

    pSprite->mPersonType = eSprite_PersonType_Native;
    pSprite->mSheetIndex = 0xD9;

    word_3B2D1[2] = pSprite->mPosX;
    word_3B2D1[3] = pSprite->mPosXFrac;
    word_3B2D1[4] = pSprite->mPosY;
    word_3B2D1[5] = pSprite->mPosYFrac;

    Sprite_Handle_Hostage_Movement(pSprite);
    Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->mSpeed = 0x0C;
    Sprite_Civilian_Update_Movement(pSprite);
    pSprite->mAnimState = eSprite_Anim_None;

    Sprite_Handle_Hostage_FrameUpdate2(pSprite);
    //seg004:44B6

    Sprite_Field_10 = pSprite->mDirection;
    pSprite->mDirection += 0x20;
    pSprite->mDirection &= 0x1C0;

    Sprite_Handle_Hostage_FrameUpdate(pSprite);
    pSprite->mDirection = Sprite_Field_10;
    Sprite_Hostage_Update_IdleFrame(pSprite);

    if (pSprite->mSpriteType != eSprite_Enemy_Leader)
        return;

    Data24 = pSprite + 1;

    //seg004:4505
    if (pSprite->mRescueTentIndex) {
        Data24->mSpriteType = eSprite_Null;
        Data24->mSheetIndex = 0x7C;
        Data24->mFrameIndex = 0;
    }

    Data24->mSpriteType = eSprite_Flashing_Light;

    Data4 = pSprite->mPosX;
    Data4 += 6;

    Data24->mPosX = Data4;
    Data4 = pSprite->mPosY - 0x0B;
    Data4 += pSprite->mRowsToSkip;
    Data24->mPosY = Data4;
    Data24->mHeight = pSprite->mHeight;

    if (pSprite->mInVehicle) {
        Data24->mSpriteType = eSprite_Null;
        Data24->mSheetIndex = 0x7C;
        Data24->mFrameIndex = 0;
    }

    Data0 = pSprite->mPosX;
    if (Data0 == mStoredSpriteX) {
        Data0 = pSprite->mPosY;
        if (Data0 == mStoredSpriteY)
            return;
    }
    pSprite->mSheetIndex = 0xD8;
    return;

loc_1D349:;
    pSprite->mAnimState = eSprite_Anim_None;
    sSprite* Data28 = &mSprites[pSprite->mRescueTentIndex];
    if (Data28->mInVehicle)
        goto loc_1D44C;

    pSprite->mPosX = Data28->mPosX;
    pSprite->mPosY = Data28->mPosY;
    pSprite->mPosX += 8;
    pSprite->mPosY += 0x0A;
    mStoredSpriteY = pSprite->mPosY;

loc_1D3C6:;
    Data0 = -3;
    Data4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_1D411;

    pSprite->mPosY -= 1;
    if (pSprite->mPosY >= 0)
        goto loc_1D3C6;

    pSprite->mPosY = mStoredSpriteY;
    pSprite->mPosX -= 1;
    if (pSprite->mPosX >= 0)
        goto loc_1D3C6;

    pSprite->mPosX = 0;
    pSprite->mAnimState = eSprite_Anim_Die1;

loc_1D411:;
    Data4 = pSprite->mPosY;
    Data0 = mMapLoaded->getHeightPixels();

    if (Data4 < Data0)
        goto loc_1D441;

    Data0 -= 1;
    pSprite->mPosY = Data0;

loc_1D441:;
    pSprite->mInVehicle = 0;
    return;

loc_1D44C:;
    pSprite->mSheetIndex = 0x7C;
    pSprite->mFrameIndex = 0;
    pSprite->mPosX = Data28->mPosX;
    pSprite->mPosY = Data28->mPosY;
}

void cFodder::Sprite_Handle_Hostage_Rescue_Tent(sSprite* pSprite) {
    pSprite->mSheetIndex = 0xDD;

    mHostage_Rescue_Tent = pSprite;
    if (pSprite->mAnimState == eSprite_Anim_Die3)
        pSprite->mSpriteType = eSprite_Explosion2;
}

void cFodder::Sprite_Handle_Door_Civilian(sSprite* pSprite) {
    mSpawnSpriteType = 0x3D;

    Sprite_CivilianDoor_SpawnUpdate(pSprite);
}

void cFodder::Sprite_Handle_Door2_Civilian(sSprite* pSprite) {
    mSpawnSpriteType = 0x3E;

    Sprite_CivilianDoor_SpawnUpdate(pSprite);
}

void cFodder::Sprite_Handle_Door_Civilian_Spear(sSprite* pSprite) {
    mSpawnSpriteType = 0x46;

    Sprite_CivilianDoor_SpawnUpdate(pSprite);
}

void cFodder::Sprite_Handle_Civilian_Invisible(sSprite* pSprite) {

    if (!pSprite->mAnimState)
        return;

    Sprite_Handle_Civilian_Death(pSprite);

    /* Unused code block

     seg004:4A78 C4 36 20 00                       les     si, ds:20h
    seg004:4A7C 26 C7 44 22 02 00                 mov     word ptr es:[si+22h], 2
    seg004:4A82 C4 36 20 00                       les     si, ds:20h
    seg004:4A86 26 C7 44 08 D0 00                 mov     word ptr es:[si+8], 0D0h ; 'ð'
    */
}

void cFodder::Sprite_Handle_Door_Civilian_Rescue(sSprite* pSprite) {
    mSprite_OpenCloseDoor_Ptr = pSprite;

    if (Sprite_Update_Die2_Explosion_IceAware(pSprite)) {
        mSprite_OpenCloseDoor_Ptr = 0;
        mPhase_Aborted = true;
        return;
    }
    pSprite->mDrawOrder = eSprite_Draw_First;
    if (!pSprite->field_2A) {
        pSprite->mSheetIndex = 0x9B;
        return;
    }

    pSprite->field_2A -= 1;
    if (!pSprite->field_2A)
        Sound_Play(pSprite, eSound_Effect_DoorCloseDoor, 1);

    if (pSprite->mSheetIndex == 0x9B)
        Sound_Play(pSprite, eSound_Effect_DoorCloseDoor, 1);

    pSprite->mSheetIndex = 0x7C;
}

void cFodder::Sprite_Native_Sound_Play(sSprite* pSprite, int16 pSoundID) {

    if (mPhase_Completed_Timer)
        return;

    if ((tool_RandomGet() & 0x7F) != 0x0E)
        return;

    int16 Data8 = 0;

    if (mMapLoaded->getTileType() == eTileTypes_Int || mMapLoaded->getTileType() == eTileTypes_Moors)
        Data8 = 0x7F;

    Sound_Play(pSprite, pSoundID, Data8);
}
