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

void cFodder::Sprite_Find_HumanVehicles() {

    if (mSprite_HumanVehicles_Found)
        return;

    mSprite_HumanVehicles_Found = true;

    for(auto& Sprite : mSprites) {

        if (Sprite.mPosX == -32768)
            continue;

        // Is not Human?
        if (Sprite.mPersonType != eSprite_PersonType_Human)
            continue;

        // Not Enabled?
        if (!Sprite.mVehicleEnabled)
            continue;

        mSprites_HumanVehicles.push_back(&Sprite);
    }
}

void cFodder::Sprites_HumanVehicles_Remove(sSprite* pSprite) {

    auto it = remove_if(mSprites_HumanVehicles.begin(), mSprites_HumanVehicles.end(), 
        [pSprite](sSprite* pSpriteFind) { 
            return pSprite == pSpriteFind; 
    });

    mSprites_HumanVehicles.erase(it, mSprites_HumanVehicles.end());
}

void cFodder::Sprite_Handle_Player_Enter_Vehicle(sSprite* pSprite) {
    // Have a target vehicle?
    if (!pSprite->mVehicleWalkTarget)
        return;

    sSprite* Vehicle = pSprite->mVehicleWalkTarget;

    int16 VehicleX = Vehicle->mPosX + 0x10;

    if (Vehicle->mVehicleType == eVehicle_Turret_Cannon || Vehicle->mVehicleType == eVehicle_Turret_Missile)
        VehicleX -= 0x0C;

    int16 VehicleY = Vehicle->mPosY - 9;
    int16 SoldierX = pSprite->mPosX + 4;
    int16 SoldierY = pSprite->mPosY;

    // Is the sprite close enough to enter the vehicle?
    int16 Distance = 0x1F;
    if (Map_Get_Distance_BetweenPoints(VehicleX, VehicleY, SoldierX, Distance, SoldierY) >= 0x0D)
        return;

    // Enter the vehicle
    pSprite->mCurrentVehicle = Vehicle;
    pSprite->mInVehicle = -1;
    sSprite* Data20 = 0;

    Squad_UpdateLeader(Data20);

    // First member of current player squad
    sSprite* eax = mSquads[pSprite->field_32][0];

    // Is this the first member of the squad
    if (eax != pSprite)
        return;

    mSquad_EnteredVehicleTimer[pSprite->field_32] = 0x190;
}

void cFodder::Sprite_Handle_Vehicle_Terrain_Check(sSprite* pSprite) {
    int16 Data4, Data0;
    int32 Dataa0;

    pSprite->field_50 = pSprite->mRowsToSkip;
    if (!pSprite->field_50)
        pSprite->mRowsToSkip = 0;

    if (Map_Sprite_Check_Around_Position(pSprite))
        goto Computer_Vehicle_SoftTerrain;

    Data4 = 0x0F;
    Data0 = -10;
    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);

    pSprite->mTerrainType = static_cast<int8>(Data4);
    if (Data4 == eTerrainFeature_Rocky || Data4 == eTerrainFeature_Rocky2)
        goto loc_23056;

    if (Data4 == eTerrainFeature_Jump)
        goto loc_23100;

    if (Data4 == eTerrainFeature_Block)
        goto Computer_Vehicle_SoftTerrain;

    if (Data4 == eTerrainFeature_Drop || Data4 == 0x0A)
        goto loc_22F06;

    if (pSprite->mDelayCounter)
        pSprite->mAnimState = eSprite_Anim_Die1;

    if (Data4 == eTerrainFeature_Snow)
        goto loc_22FA3;

    if (pSprite->mPersonType == eSprite_PersonType_Human)
        goto Human_Vehicle;

    if (Data4 == eTerrainFeature_QuickSand || Data4 == eTerrainFeature_WaterEdge
        || Data4 == eTerrainFeature_Water || Data4 == eTerrainFeature_Sink)
        goto Computer_Vehicle_SoftTerrain;

    goto loc_22EEB;

Human_Vehicle:;

    if (Data4 == eTerrainFeature_QuickSand)
        goto Human_Vehicle_Quicksand;

    if (Data4 == eTerrainFeature_WaterEdge)
        goto Human_Vehicle_WaterEdge;

    if (Data4 == eTerrainFeature_Water || Data4 == eTerrainFeature_Sink)
        goto AnimDie3;

loc_22EEB:;
    pSprite->mRowsToSkip = 0;
    if (pSprite->field_50)
        goto loc_23056;

    return;

loc_22F06:;
    pSprite->mDelayCounter += 4;
    Data0 = pSprite->mDelayCounter;
    pSprite->mPosY += Data0;
    return;

Computer_Vehicle_SoftTerrain:;

    if (pSprite->mPersonType != eSprite_PersonType_Human) {
        pSprite->mTargetX = pSprite->mPosX;
        pSprite->mTargetY = pSprite->mPosY;
    }
    //loc_22F5F
    pSprite->mSpeed = -pSprite->mSpeed;
    pSprite->mSpeed >>= 1;
    Sprite_XY_Restore(pSprite);

    Data4 = 0x0F;
    Data0 = -10;
    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (Data4 == eTerrainFeature_Block)
        pSprite->mAnimState = eSprite_Anim_Die1;

    Sprite_Animation_SlideOrDie(pSprite);
    return;

loc_22FA3:;

    if (!pSprite->mSpeed)
        return;

    pSprite->mHeight ^= 1;
    if (!pSprite->mHeight)
        return;

    if (pSprite->mSpeed <= 0x10)
        return;

    pSprite->mSpeed -= 2;
    return;

Human_Vehicle_Quicksand:;
    pSprite->mRowsToSkip = 1;
    return;

Human_Vehicle_WaterEdge:;
    pSprite->mRowsToSkip = 3;
    return;

AnimDie3:;
    if (pSprite->mDelayCounter)
        pSprite->mAnimState = eSprite_Anim_None;
    dword_3B24B = -1;

    Sprite_Handle_Vehicle_Sinking(pSprite);
    if (pSprite->mRowsToSkip < 0x50)
        goto loc_23033;

    pSprite->mRowsToSkip = 0x1E;
    pSprite->mAnimState = eSprite_Anim_Die3;
    return;

loc_23033:;
    pSprite->mRowsToSkip += 1;
    pSprite->mSpeed -= 0x0C;
    if (pSprite->mSpeed >= 0)
        return;

    pSprite->mSpeed = 0;
    return;

loc_23056:;
    if (pSprite->mHeight)
        return;

    if (pSprite->mSpeed >= 0x14)
        goto loc_230B0;

    if (!pSprite->mSpeed)
        return;

    Data0 = mInterruptTick;
    Data0 &= 1;
    Data0 += 3;
    pSprite->mHeight = Data0;
    Data0 = -Data0;
    pSprite->mFixedPoint = (((int64)pSprite->mFixedPoint & 0xFFFF) | (Data0 << 16));
    return;

loc_230B0:;
    // Not Moving?
    if (!pSprite->mSpeed)
        return;

    pSprite->mHeight = 1;
    Dataa0 = pSprite->mSpeed << 16;
    Dataa0 >>= 3;
    pSprite->mFixedPoint = Dataa0;
    return;

loc_23100:;
    if (!pSprite->mSpeed)
        return;

    pSprite->mHeight = 1;
    Dataa0 = pSprite->mSpeed << 16;
    Dataa0 >>= 2;
    pSprite->mFixedPoint = Dataa0;
    pSprite->mSpeed <<= 1;
    pSprite->mSpeed <<= 1;

}

void cFodder::Sprite_Under_Vehicle(sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C) {

    if (mPhase_Finished)
        return;

    sSprite* Sprite = mSprites.data();

	// TODO: Fix counter
    for (int16 Count = 0x1D; Count >= 0; --Count, ++Sprite) {
        if (Sprite->mPosX == -32768)
            continue;

        if (Sprite == pSprite)
            continue;

        if (!mSprite_Can_Be_RunOver[Sprite->mSpriteType])
            continue;

        if (pSprite->mPersonType == Sprite->mPersonType)
            continue;

        if (Sprite->mSpriteType == eSprite_Hostage)
            continue;

        if (Sprite->mSpriteType == eSprite_Enemy_Leader)
            continue;

        if (pData8 > Sprite->mPosX)
            continue;

        if (pDataC < Sprite->mPosX)
            continue;

        if (pData10 > Sprite->mPosY)
            continue;

        if (pData14 < Sprite->mPosY)
            continue;

        int16 Data0 = Sprite->mHeight;
        if (pData1C < Data0)
            continue;
        Data0 += 0x0E;
        if (pData18 > Data0)
            continue;

        if (Sprite->mSpriteType != eSprite_Player) {
            if (Sprite->mInVehicle)
                continue;
        }

        // Run Over
        Sprite->mAnimState = eSprite_Anim_Die1;
        if (!Sprite->mVehicleEnabled)
            return;

        pSprite->mAnimState = eSprite_Anim_Die1;
        return;
    }
}

void cFodder::Sprite_Handle_Vehicle_Human(sSprite* pSprite) {

    if (pSprite->mAnimState) {

        pSprite->mPersonType = eSprite_PersonType_Human;
        Sprite_Handle_Vehicle(pSprite);
        pSprite->mPersonType = eSprite_PersonType_Human;
        return;
    }

    if (pSprite->mVehicleType == eVehicle_JeepRocket)
        Vehicle_Handle_Cannon_Fire(pSprite);

    mSprite_Helicopter_DestroyLight = 0;
    pSprite->mPersonType = eSprite_PersonType_Human;
    Sprite_Handle_Vehicle(pSprite);
    pSprite->mPersonType = eSprite_PersonType_Human;
    mSprite_Helicopter_DestroyLight = 0;

    sSprite* Data24 = pSprite + 1;
    Data24->mSpriteType = eSprite_Null;
}

void cFodder::Sprite_Handle_Tank(sSprite* pSprite) {
    int16 Data0 = 2;
    int16 Data4 = pSprite->mSpeed;
    int16 Data8, DataC, Data10, Data14, Data18, Data1C;
    sSprite* Data2C = 0;

    if (!Data4) {
        if (pSprite->mPersonType == eSprite_PersonType_Human) {
            if (pSprite != mSquad_CurrentVehicle)
                goto loc_2356B;
        }
    }

    Data4 += 0x10;
    if (Data4 > 0x20)
        Data4 = 0x20;

    Sprite_Map_Sound_Play(Data0);

loc_2356B:;
    pSprite->mVehicleType = eVehicle_Tank;
    pSprite->mVehicleEnabled = -1;
    if (Sprite_Animation_SlideOrDie(pSprite))
        return;

    pSprite->mSheetIndex = 0xD1;
    word_3B2F7 = pSprite->mSpeed + 0x20;

    Data0 = pSprite->mTargetX;
    Data4 = pSprite->mTargetY;

    Sprite_Vehicle_TurnTowardTarget(pSprite, Data0, Data4);
    Data0 = pSprite->mTargetX;
    Data4 = pSprite->mTargetY;

    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 >= 0x1E)
        goto loc_2361A;

    if (Data0 >= pSprite->mSpeed)
        goto loc_23680;

    pSprite->mSpeed = Data0;
    goto loc_23680;

loc_2361A:;
    dword_3B24B = -1;
    if (!Sprite_Create_Smoke(pSprite, Data2C))
        Data2C->mDrawOrder = eSprite_Draw_First;

    if (!pSprite->mSpeed) {
        if (mDirectionMod) {
            pSprite->mSpeed -= 4;
            goto loc_23680;
        }
    }

    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (!pSprite->field_2A) {
        ++pSprite->mSpeed;

        if (pSprite->mSpeed >= 0x18)
            pSprite->mSpeed = 0x18;
    }

loc_23680:;

    Sprite_XY_Store(pSprite);
    Sprite_Movement_Calculate(pSprite);
    Sprite_Handle_Vehicle_Terrain_Check(pSprite);

    pSprite->mHeight = 0;
    Sprite_Vehicle_Turret_Update(pSprite);

    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosX + 0x1E;
    Data10 = pSprite->mPosY - 0x14;
    Data14 = pSprite->mPosY;
    Data18 = pSprite->mHeight;
    Data1C = Data18 + 0x0E;

    Sprite_Under_Vehicle(pSprite, Data8, DataC, Data10, Data14, Data18, Data1C);
}

void cFodder::Sprite_Handle_Tank_FireMissile(sSprite* pSprite) {
    if (!pSprite->mWeaponCooldown)
        if (!pSprite->mFiredWeaponType)
            return;

    pSprite->mFiredWeaponType = 0;
    sSprite* Data24 = pSprite + 1;

    Data24->mWeaponTargetX = pSprite->mWeaponTargetX;
    Data24->mWeaponTargetY = pSprite->mWeaponTargetY;

    int16 Data24_Field_0 = Data24->mPosX;
    int16 Data24_Field_4 = Data24->mPosY;
    int16 Data0 = Data24->mFrameIndex << 1;

    Data24->mPosX += mSprite_Turret_Positions[Data0];
    Data24->mPosY += mSprite_Turret_Positions[Data0 + 1];

    mSprite_Missile_LaunchDistance_X = 0;
    mSprite_Missile_LaunchDistance_Y = 0;
    sSprite* Data2C = 0;

    if (!Sprite_Create_Missile(Data24, Data2C)) {
        Data24->mPosY = Data24_Field_4;
        Data24->mPosX = Data24_Field_0;
        pSprite->mWeaponCooldown = -1;
        return;
    }

    Data2C->mHeight += 0x11;
    Data2C->mSpeed = 0x3C;
    Sound_Play(Data24, 5, 0x1E);
    Sprite_Create_Smoke(Data24, Data2C);

    Data24->mPosY = Data24_Field_4;
    Data24->mPosX = Data24_Field_0;
    pSprite->mWeaponCooldown = 0;
}

void cFodder::Sprite_Handle_Turret(sSprite* pSprite) {
    sSprite* Data28 = 0;
    sSprite* Data34 = 0;
    int16 Data0, Data4, Data8, DataC, Data10;

    // Turrets in Moors / Interior can't be destroyed
    if (mVersionCurrent->isCannonFodder1()) {
        if (mMapLoaded->getTileType() == eTileTypes_Moors || mMapLoaded->getTileType() == eTileTypes_Int) {

            if (pSprite->mAnimState == eSprite_Anim_Die1)
                pSprite->mAnimState = eSprite_Anim_None;
        }
    }

    if (pSprite->mAnimState == eSprite_Anim_Die3 || pSprite->mAnimState == eSprite_Anim_Die1) {

        pSprite->mSpriteType = eSprite_Explosion;
        pSprite->mTargetX = 0x1F45;
        pSprite->mTargetY = -2;
        if (pSprite->mPersonType == eSprite_PersonType_Human)
            return;

        Sprite_Destroy(pSprite + 1);
        return;
    }

    //loc_23F70
    pSprite->mSheetIndex = 0xD2;
    pSprite->mVehicleEnabled = -1;
    if (pSprite->mPersonType == eSprite_PersonType_Human)
        goto loc_24075;

    mTurretFires_HomingMissile = 0;
    Data0 = eSprite_Player;
    Data4 = -1;
    Data8 = -1;
    DataC = -1;
    Data10 = -1;

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Get_Civilian_Home - 1]) {
        Data4 = eSprite_Civilian;
        Data8 = eSprite_Civilian2;
        DataC = eSprite_Civilian_Spear;
        Data10 = -1;
    }

    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        goto loc_240F3;

    if (mSprite_Find_Distance >= 0x28) {

        Data0 = tool_RandomGet();
        if (mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage < 5)
            Data0 &= 0x3F;
        else
            Data0 &= 0x1F;

        if (Data0 == 4)
            pSprite->mFiredWeaponType = -1;
    }

    // Fire a homing missile
    mTurretFires_HomingMissile = -1;
    Data34 = Data28;

    Data0 = pSprite->mPosX + 8;
    Data4 = pSprite->mPosY - 5;
    Data8 = Data28->mPosX;
    DataC = Data28->mPosY;

    pSprite->mWeaponTargetX = Data8;
    pSprite->mWeaponTargetY = DataC;

    word_3B4ED[1] = Data28->mHeight;
    goto loc_240C8;

loc_24075:;
    if (pSprite != mSquad_CurrentVehicle)
        return;

    Data0 = pSprite->mPosX + 8;
    Data4 = pSprite->mPosY - 5;
    Data8 = mMouseX + (mCameraX >> 16);
    Data8 -= 0x10;
    DataC = mMouseY + (mCameraY >> 16);

loc_240C8:;
    Direction_Between_Points(Data0, Data4, Data8, DataC);
    Data4 -= 0x10;
    Data4 >>= 5;
    Data4 += 8;
    Data4 ^= 0x0F;
    Data4 &= 0x0F;

    pSprite->mFrameIndex = Data4;
loc_240F3:;

    Sprite_Handle_Turret_Fire(pSprite, Data34);

    if (pSprite->mPersonType == eSprite_PersonType_Human)
        return;

    sSprite* Data24 = pSprite + 1;

    Data24->mSpriteType = eSprite_Flashing_Light;
    Data4 = pSprite->mPosX;
    Data4 += 5;
    Data24->mPosX = Data4;
    Data4 = pSprite->mPosY;
    Data4 -= 0x0F;
    Data4 -= pSprite->mRowsToSkip;
    Data24->mPosY = Data4;
    Data24->mHeight = pSprite->mHeight;
}

void cFodder::Sprite_Handle_Turret_Fire(sSprite* pSprite, sSprite* pData34) {
    int16 ax;

    sSprite* Data2C = 0;

    if (!pSprite->mWeaponCooldown) {
        if (!pSprite->mFiredWeaponType)
            return;
    }

    pSprite->mFiredWeaponType = 0;
    int16 Field_0 = pSprite->mPosX;
    int16 Field_4 = pSprite->mPosY;

    int16 Data0 = pSprite->mFrameIndex;
    Data0 <<= 1;

    int16 Data4 = mSprite_Turret_Positions[Data0];
    pSprite->mPosX += Data4;

    Data4 = mSprite_Turret_Positions[Data0 + 1];
    pSprite->mPosY += Data4;

    mSprite_Missile_LaunchDistance_X = 0;
    mSprite_Missile_LaunchDistance_Y = 0;

    if (pSprite->mVehicleType != eVehicle_Turret_Homing)
        goto loc_2421D;

    if (!mTurretFires_HomingMissile)
        goto loc_2421D;

    ax = Sprite_Create_MissileHoming(pSprite, Data2C, pData34);
    goto loc_24234;

loc_2421D:;

    if (pSprite->mPersonType != eSprite_PersonType_Human)
        if (word_3B4ED[1] > 9)
            goto loc_24275;

    ax = Sprite_Create_Missile(pSprite, Data2C);

loc_24234:;
    if (!ax)
        goto loc_2426C;

    Data2C->mHeight += 0x11;
    Data2C->mSpeed = 0x3C;
    Sound_Play(pSprite, eSound_Effect_Turret_Fire, 0x1E);
    Sprite_Create_Smoke(pSprite, Data2C);
    pSprite->mWeaponCooldown = 0;
    goto loc_24275;

loc_2426C:;
    pSprite->mWeaponCooldown = -1;

loc_24275:;
    pSprite->mPosY = Field_4;
    pSprite->mPosX = Field_0;
}

int16 cFodder::Sprite_Handle_Vehicle_Sinking(sSprite* pSprite) {

    Sound_Play(pSprite, eSound_Effect_Vehicle_Sinking, 0x0F);
    if (!pSprite->mWasDrawn)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mPosYFrac = pSprite->mPosYFrac;

    if (dword_3B24B) {
        Data0 = tool_RandomGet() & 0x1F;
        Data0 -= 5;
        Data2C->mPosX += Data0;
        Data0 >>= 4;
        Data0 &= 0x0F;
        Data0 -= 0x0A;
        Data2C->mPosY -= Data0;
        dword_3B24B = 0;
    }
    //loc_24546
    Data2C->mHeight = pSprite->mHeight;
    Data2C->mRowsToSkip = 0;
    Data2C->mDrawOrder = eSprite_Draw_OnTop;

    Data0 = tool_RandomGet() & 1;
    if (!Data0) {
        Data2C->mSheetIndex = 0xDE;
        Data2C->mFrameIndex = 4;
        Data2C->mSpriteType = eSprite_Vehicle_Sinking_1;
    }
    else {
        Data2C->mSheetIndex = 0xDF;
        Data2C->mFrameIndex = 0;
        Data2C->mSpriteType = eSprite_Vehicle_Sinking_2;
    }
    return 0;
}

int16 cFodder::Sprite_Create_Cannon(sSprite* pSprite) {
    if (mPhase_Completed_Timer)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mPosYFrac = pSprite->mPosYFrac;
    Data2C->mSheetIndex = 0x7F;
    Data2C->mFrameIndex = 3;
    Data2C->field_12 = 9;
    Data2C->mSpriteType = eSprite_Cannon;

    Data2C->mHeightFixed += 0x60000;

    Data2C->mRowsToSkip = pSprite->mRowsToSkip;
    Data2C->mPersonType = pSprite->mPersonType;
    Data2C->mDrawOrder = eSprite_Draw_Second;
    Data0 = 0x321;

    Data0 += pSprite->mAIAggression;
    Data2C->mWeaponFireTimer = Data0;
    Data0 = pSprite->mSpeed;
    Data0 += 0x50;
    Data2C->mSpeed = Data0;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data0 = pSprite->mFrameIndex;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    Data0 <<= 5;
    Data2C->mDirection = Data0;
    Data2C->field_3A = 0;
    Data2C->mOwnerSprite = pSprite;
    Data2C->field_2A = 2;
    Data2C->mProjectileOffsetX = 0;
    Data2C->mProjectileOffsetY = 2;
    Data2C->mInitialDirection = -1;
    Data2C->field_50 = 0;
    Data2C->mDirectionOverride = 0;

    int16 Data8 = 7;
    Data0 = tool_RandomGet();
    int16 Data4 = Data0;

    Data0 &= Data8;
    if (Data4 < 0)
        Data0 = -Data0;

    Data2C->mDirection += Data0;
    Data2C->field_64 = 0;
    return 0;
}

void cFodder::Sprite_Handle_Vehicle_Enemy(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data28 = 0;

    if (pSprite->mAnimState) {
        pSprite->mPersonType = eSprite_PersonType_AI;
        Sprite_Handle_Vehicle(pSprite);
        pSprite->mPersonType = eSprite_PersonType_AI;
        return;
    }

    if (pSprite->mVehicleType == eVehicle_DontTargetPlayer)
        goto loc_255DA;

    Data0 = eSprite_Player;
    Data4 = eSprite_Civilian;
    Data8 = 0x3E;
    DataC = 0x46;
    Data10 = -1;
    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        goto loc_255DA;

    Data0 = tool_RandomGet() & 3;
    if (!Data0)
        pSprite->mFiredWeaponType = -1;

    Data0 = pSprite->mPosX;
    Data0 += 8;
    Data4 = pSprite->mPosY;
    Data4 -= 5;
    Data8 = Data28->mPosX;
    pSprite->mWeaponTargetX = Data8;
    pSprite->mTargetX = Data8;
    DataC = Data28->mPosY;
    pSprite->mWeaponTargetY = DataC;
    pSprite->mTargetY = DataC;

loc_255DA:;
    pSprite->mPersonType = eSprite_PersonType_AI;
    Sprite_Handle_Vehicle(pSprite);

    if (pSprite->mVehicleType == eVehicle_JeepRocket)
        Vehicle_Handle_Cannon_Fire(pSprite);

    sSprite* Data24 = pSprite + 1;
    Data24->mSpriteType = eSprite_Null;

    Data24 = pSprite + 2;
    Data24->mSpriteType = eSprite_Flashing_Light;
    Data4 = pSprite->mPosX;
    Data4 += 0x0D;
    Data24->mPosX = Data4;

    Data4 = pSprite->mPosY;
    Data4 -= 0x13;

    Data4 += pSprite->mRowsToSkip;
    Data24->mPosY = Data4;
    Data24->mHeight = pSprite->mHeight;
}

void cFodder::Sprite_Apply_Gravity(sSprite* pSprite) {

    int32 Data0 = pSprite->mFixedPoint;

    pSprite->mHeightFixed += Data0;

    if (pSprite->mHeightFixed < 0) {
        pSprite->mHeightFixed = 0;
        Data0 = -Data0;
        Data0 >>= 1;
    }

    Data0 -= 0x20000;
    pSprite->mFixedPoint = (int32)Data0;
}

void cFodder::Sprite_Vehicle_TurnTowardTarget(sSprite* pSprite, int16& pData0, int16& pData4) {

    int16 Data1C;

    pSprite->mFaceDirection = pSprite->mDirection;

    Sprite_Direction_Between_Points(pSprite, pData0, pData4);

    mSprite_Field10_Saved = pSprite->mDirection;
    pSprite->field_3E = 0;

    Sprite_SetDirectionMod(pSprite);

    pSprite->mDirection = pSprite->mFaceDirection;

    pData0 = word_3B2F7;
    pData0 -= pSprite->mSpeed;

    pData0 *= mDirectionMod;
    int16 Data8 = 0;

    pData4 = pData0;
    if (pData4 < 0) {
        Data8 = -1;
        pData4 = -pData4;
    }

    if (pData4 > word_3B25D) {
        pData0 = word_3B25D;

        if (Data8 < 0)
            pData0 = -pData0;
    }
    //loc_22D7A
    pSprite->mDirection += pData0;
    pSprite->mDirection &= 0x1FE;

    if (pSprite->mVehicleType == eVehicle_Jeep)
        goto loc_22DC4;

    if (pSprite->mVehicleType == eVehicle_JeepRocket)
        goto loc_22DC4;

    if (pSprite->mVehicleType == eVehicle_Tank)
        goto loc_22DCF;

    if (pSprite->mSpeed <= 6)
        return;

    if (!(mMission_EngineTicks & 1))
        return;

loc_22DC4:;
    if (pSprite->mSpeed <= 8)
        return;

loc_22DCF:;
    pData0 = -pData0;
    if (!pData0) {
        Data1C = 0;
    }
    else if (pData0 >= 0) {
        Data1C = 1;
    }
    else {
        Data1C = -1;
    }

    Sprite_Vehicle_Direction_Update(pSprite, Data1C);
}

void cFodder::Sprite_Vehicle_Turret_Update(sSprite* pSprite) {
    sSprite* Data24 = pSprite + 1;
    int16 Data0, Data4, Data8, DataC;

    Data24->mSheetIndex = 0xD2;
    Data24->mPosX = pSprite->mPosX + 0x0A;
    Data24->mPosY = pSprite->mPosY + 1;

    Data0 = pSprite->mRowsToSkip;
    Data24->mPosY += Data0;
    Data24->mHeight = pSprite->mHeight;
    Data24->mHeight += 9;
    Data0 = Data24->mPosX + 8;
    Data4 = Data24->mPosY - 5;

    Data4 -= Data24->mHeight;
    Data24->mRowsToSkip = pSprite->mRowsToSkip;

    if (pSprite->mPersonType == eSprite_PersonType_Human)
        goto loc_23815;

    Data8 = pSprite->mWeaponTargetX;
    if (Data8 >= 0)
        goto loc_23806;

    Data0 = mMission_EngineTicks & 0x0F;
    if (Data0)
        return;

    Data0 = tool_RandomGet() & 1;
    if (!Data0)
        Data0 -= 1;

    Data24->mFrameIndex += Data0;
    Data24->mFrameIndex &= 0x0F;
    return;

loc_23806:;
    DataC = pSprite->mWeaponTargetY;
    goto loc_23843;

loc_23815:;
    if (pSprite != mSquad_CurrentVehicle)
        return;

    Data8 = mMouseX + (mCameraX >> 16);
    Data8 -= 0x10;

    DataC = mMouseY + (mCameraY >> 16);
loc_23843:;
    Direction_Between_Points(Data0, Data4, Data8, DataC);

    Data4 -= 0x10;
    Data4 >>= 5;
    Data4 += 8;
    Data4 ^= 0x0F;
    Data4 &= 0x0F;
    Data24->mFrameIndex = Data4;
}

void cFodder::Vehicle_Handle_Cannon_Fire(sSprite* pSprite) {

    if (pSprite->mPersonType == eSprite_PersonType_Human)
        goto loc_245DA;

    if (pSprite->mFiredWeaponType)
        goto loc_24617;

    return;

loc_245DA:;
    if (pSprite != mSquad_CurrentVehicle)
        return;

    if (!mButtonPressRight)
        return;

    if (pSprite->mFiredWeaponType)
        goto loc_24617;
    pSprite->mFiredWeaponType = 0;
    pSprite->mWeaponAnimTick = ~pSprite->mWeaponAnimTick;
    if (pSprite->mWeaponAnimTick)
        return;

loc_24617:;
    pSprite->mFiredWeaponType = 0;

    int16 Field_0 = pSprite->mPosX;
    int16 Field_4 = pSprite->mPosY;

    int16 Data0 = pSprite->mFrameIndex;

    // seg005:4F58 
    Data0 <<= 1;

    if (pSprite->mIsSinking < 0)
        Data0 += 2;

    int16 Data4 = mSprite_VehiclePosition_Mod[Data0];
    pSprite->mPosX += Data4;
    Data4 = mSprite_VehiclePosition_Mod[Data0 + 1];
    pSprite->mPosY += Data4;

    if (!Sprite_Create_Cannon(pSprite)) {
        pSprite->mIsSinking = ~pSprite->mIsSinking;
        Sound_Play(pSprite, 0x10, 0x1E);
    }

    //loc_246BC
    pSprite->mPosY = Field_4;
    pSprite->mPosX = Field_0;
}

void cFodder::Sprite_Vehicle_Direction_Update(sSprite* pSprite, int16& pData1C) {
    int16 Data4 = mSprite_Field10_Saved;
    Data4 >>= 5;
    Data4 -= 1;
    Data4 ^= 0x0F;

    int16 Data0 = pSprite->mFrameIndex;

    Data0 -= Data4;
    Data4 = Data0;
    Data4 &= 0x0F;
    int16 DataC = mDirectionStepTable[Data4];
    if (!DataC)
        return;

    if (DataC == pData1C)
        goto loc_2B21D;

    if (!pData1C)
        goto loc_2B21D;

    DataC = pData1C;

loc_2B21D:;
    pSprite->mFrameIndex += DataC;
    pSprite->mFrameIndex &= 0x0F;

}

void cFodder::Sprite_Handle_Vehicle(sSprite* pSprite) {
    int16 Data0 = 2;
    int16 Data4 = pSprite->mSpeed;
    int16 Data8, DataC;

    if (!Data4) {
        if (pSprite->mPersonType == eSprite_PersonType_Human) {

            if (pSprite != mSquad_CurrentVehicle)
                goto loc_1B523;
        }
    }

    Data4 += 0x10;
    if (Data4 > 0x40)
        Data4 = 0x40;
    Data0 = Data4;

    Data0 >>= 4;
    if (Data0 > 3)
        Data0 = 3;
    Data0 += 0x38;

    Sprite_Map_Sound_Play(Data0);

loc_1B523:;
    pSprite->mVehicleEnabled = -1;
    if (Sprite_Animation_SlideOrDie(pSprite))
        return;

    pSprite->mSheetIndex = 0xA5;
    Sprite_Apply_Gravity(pSprite);
    if (pSprite->mHeight < 3) {
        word_3B2F7 = 0x48;
        Data0 = pSprite->mTargetX;
        Data4 = pSprite->mTargetY;

        Sprite_Vehicle_TurnTowardTarget(pSprite, Data0, Data4);
    }

    Data0 = pSprite->mTargetX;
    Data4 = pSprite->mTargetY;
    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 >= 0x1E)
        goto loc_1B5D2;

    if (Data0 >= pSprite->mSpeed)
        goto loc_1B655;

    pSprite->mSpeed = Data0;
    goto loc_1B655;

loc_1B5D2:;

    if (!pSprite->mRowsToSkip) {
        dword_3B24B = -1;
        sSprite* Data2C = 0;

        if (!Sprite_Create_Smoke(pSprite, Data2C))
            Data2C->mDrawOrder = eSprite_Draw_First;
    }

    if (!pSprite->mSpeed) {
        if (mDirectionMod)
            goto loc_1B655;
    }

    // Sinking? No, Then we can speed up
    if (pSprite->mRowsToSkip < 4) {

        pSprite->mSpeed += 3;

        // This vehicle only moves at a single speed
        if (pSprite->mSpriteType == eSprite_Vehicle_Unk_Enemy) {

            pSprite->mSpeed = 0x14;

        }
        else {
            if (pSprite->mSpeed >= 0x32)
                pSprite->mSpeed = 0x32;
        }

    }
loc_1B655:;
    // VEhicle in the air, increase speed
    if (pSprite->mHeight > 4)
        pSprite->mSpeed += 0x1C;

    Sprite_XY_Store(pSprite);
    Sprite_Movement_Calculate(pSprite);

    if (pSprite->mHeight < 9) {
        Sprite_Handle_Vehicle_Terrain_Check(pSprite);

        if (pSprite->mHeight <= 4)
            pSprite->mHeight = 0;
    }
    else {
        pSprite->mSpeed += 0x40;
    }

    Sprite_Shadow_Update_From_Height(pSprite);

    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosX;
    DataC += 0x1C;
    int16 Data10 = pSprite->mPosY;
    Data10 -= 0x10;
    int16 Data14 = pSprite->mPosY;

    int16 Data18 = pSprite->mHeight;
    int16 Data1C = Data18;
    Data1C += 0x0E;
    Sprite_Under_Vehicle(pSprite, Data8, DataC, Data10, Data14, Data18, Data1C);

}

void cFodder::Sprite_Handle_VehicleNoGun_Human(sSprite* pSprite) {

    pSprite->mVehicleType = eVehicle_Jeep;
    Sprite_Handle_Vehicle_Human(pSprite);
}

void cFodder::Sprite_Handle_VehicleGun_Human(sSprite* pSprite) {

    pSprite->mVehicleType = eVehicle_JeepRocket;
    Sprite_Handle_Vehicle_Human(pSprite);
}

void cFodder::Sprite_Handle_Tank_Human(sSprite* pSprite) {

    if (pSprite->mAnimState == eSprite_Anim_Die1)
        pSprite->mAnimState = eSprite_Anim_None;

    if (pSprite->mAnimState) {
        pSprite->mPersonType = eSprite_PersonType_Human;

        Sprite_Handle_Tank(pSprite);
        pSprite->mPersonType = eSprite_PersonType_Human;
        return;
    }
    pSprite->mPersonType = eSprite_PersonType_Human;
    Sprite_Handle_Tank(pSprite);
    pSprite->mPersonType = eSprite_PersonType_Human;
    Sprite_Handle_Tank_FireMissile(pSprite);

    sSprite* Data24 = pSprite + 1;
    Data24->mSpriteType = eSprite_Null;
}

void cFodder::Sprite_Handle_Tank_Enemy(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10, Data14;
    sSprite* Data24 = 0, *Data30 = 0;

    if (pSprite->mAnimState == eSprite_Anim_Die1)
        pSprite->mAnimState = eSprite_Anim_None;

    if (pSprite->mAnimState) {
        pSprite->mPersonType = eSprite_PersonType_AI;
        Sprite_Handle_Tank(pSprite);
        pSprite->mPersonType = eSprite_PersonType_AI;

        Data24 = pSprite + 2;
        if (Data24->mSpriteType == eSprite_Flashing_Light)
            Sprite_Destroy(Data24);

        return;
    }

    if (pSprite->mRetargetCooldown)
        pSprite->mRetargetCooldown--;

    int16 Data1C = pSprite->field_5E_Squad;
    if (mSquads[Data1C / 9] == (sSprite**)INVALID_SPRITE_PTR)
        goto NextSquadMember;

    Data30 = mSquads[Data1C / 9][Data1C % 9];
    if (Data30 == INVALID_SPRITE_PTR)
        goto NextSquadMember;

    if (Data30->mRowsToSkip)
        goto NextSquadMember;

    Data8 = Data30->mPosX;
    Data8 += 8;

    DataC = Data30->mPosY;
    DataC += -5;

    if (Map_Terrain_GetMoveable_WithCache(mTiles_NotDriveable, Data8, DataC, Data10, Data14))
        goto NextSquadMember;

    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;
    Data8 = Data30->mPosX;
    DataC = Data30->mPosY;

    mSprite_Tank_SpriteX = Data0;
    mSprite_Tank_SpriteY = Data4;
    mSprite_Tank_Squad0_X = Data8;
    mSprite_Tank_DistanceTo_Squad0 = DataC;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    mSprite_DistanceTo_Squad0 = Data0;
    if (Data0 >= 250)
        goto NextSquadMember;

    Data0 = mSprite_Tank_SpriteX;
    Data4 = mSprite_Tank_SpriteY;
    Data8 = mSprite_Tank_Squad0_X;
    DataC = mSprite_Tank_DistanceTo_Squad0;
    Data8 += 0x0F;
    DataC -= 0x0A;

    if (Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC))
        goto NextSquadMember;

    pSprite->mWeaponTargetX = Data30->mPosX;
    pSprite->mWeaponTargetY = Data30->mPosY;
    pSprite->mRetargetCooldown = 0x5A;

    // If we have reached our current target
    if (pSprite->mPosX == pSprite->mTargetX && pSprite->mPosY == pSprite->mTargetY) {

        if (!(tool_RandomGet() & 0x3F)) {
            pSprite->mTargetX = Data30->mPosX;
            pSprite->mTargetY = Data30->mPosY;
        }
    }

    if (mSprite_DistanceTo_Squad0 <= 0x32)
        goto loc_1CDA3;

    Data0 = tool_RandomGet() & 0x1F;
    if (Data0 != 4)
        goto loc_1CDA3;

    pSprite->mFiredWeaponType = -1;
    Sprite_Handle_Tank_FireMissile(pSprite);
    goto loc_1CDA3;

NextSquadMember:;
    pSprite->field_5E_Squad += 1;
    if (pSprite->field_5E_Squad >= 0x1E) {
        pSprite->field_5E_Squad = 0;
        pSprite->mWeaponTargetX = -1;
    }

loc_1CDA3:;
    mSprite_Helicopter_DestroyLight = -1;
    pSprite->mPersonType = eSprite_PersonType_AI;
    Sprite_Handle_Tank(pSprite);
    pSprite->mPersonType = eSprite_PersonType_AI;
    mSprite_Helicopter_DestroyLight = 0;

    if (mSprite_Reached_Target) {
        pSprite->mRetargetCooldown = 0;
        pSprite->mTargetX = pSprite->mPosX;
        pSprite->mTargetY = pSprite->mPosY;
    }

    Data24 = pSprite + 1;
    Data24->mSpriteType = eSprite_Null;

    Data24 = pSprite + 2;
    Data24->mSpriteType = eSprite_Flashing_Light;
    Data24->mPosX = pSprite->mPosX + 0x0F;
    Data24->mPosY = (pSprite->mPosY - 0x16) - pSprite->mRowsToSkip;
    Data24->mHeight = pSprite->mHeight;
}

void cFodder::Sprite_Handle_Cannon(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data1C;
    ++pSprite->field_64;

    if (!pSprite->field_12)
        goto loc_1D65C;

    pSprite->field_12 -= 1;
    pSprite->field_3A++;

    if (pSprite->field_44)
        goto loc_1D5B3;

    Sprite_XY_Store(pSprite);
    Data1C = pSprite->mDirection;

    Sprite_Movement_Calculate(pSprite);

    if (mSprite_Bullet_Destroy)
        goto loc_1D6CA;

    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;
    Data8 = pSprite->mWeaponTargetX;
    DataC = pSprite->mWeaponTargetY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->mSpeed;
    Data4 >>= 3;

    if (Data0 > Data4)
        goto loc_1D5CB;

    pSprite->mDrawOrder = eSprite_Draw_Second;
    pSprite->mPosX = pSprite->mWeaponTargetX;
    pSprite->mPosY = pSprite->mWeaponTargetY;
    pSprite->field_44 = -1;
    goto loc_1D5CB;
loc_1D5B3:;
    pSprite->mDrawOrder = eSprite_Draw_First;
    Sprite_Movement_Calculate(pSprite);

    if (mSprite_Bullet_Destroy)
        goto loc_1D6CA;

loc_1D5CB:;
    if (pSprite->field_64 > 3) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
            goto loc_1D65C;
    }

    if (!Sprite_Projectile_Collision_Check(pSprite))
        return;

    if (mSprites_Found_Count == 1)
        goto loc_1D65C;

    pSprite->field_43 = 1;
    pSprite->mDrawOrder = eSprite_Draw_OnTop;
    pSprite->mPosY -= 5;
    if (pSprite->mPosY < 0)
        goto loc_1D6DD;
    pSprite->mSheetIndex = 0x96;
    pSprite->mFrameIndex = 0;
    return;

loc_1D633:;
    pSprite->mSheetIndex = 0x96;
    pSprite->field_12 = 0;
    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex >= 4)
        goto loc_1D6CA;

loc_1D65C:;

    if (pSprite->field_43)
        goto loc_1D69F;

    Sprite_Projectile_Collision_Check(pSprite);
    pSprite->field_43 = -1;
    pSprite->mDrawOrder = eSprite_Draw_First;
    pSprite->mPosY -= 5;
    if (pSprite->mPosY < 0)
        goto loc_1D6DD;

    pSprite->mFrameIndex = 3;

loc_1D69F:;
    if (pSprite->field_43 >= 0)
        goto loc_1D633;

    pSprite->field_12 = 0;
    pSprite->mSheetIndex = 0x7F;
    pSprite->mFrameIndex += 1;

    if (pSprite->mSheetIndex >= 8)
        goto loc_1D6CA;

    return;

loc_1D6CA:;

    mSprite_Projectile_Counters[2] -= 1;
    Sprite_Destroy(pSprite);
    return;

loc_1D6DD:;
    pSprite->mPosX = 0;
    pSprite->mPosY = 0x1000;
    goto loc_1D6CA;

}

void cFodder::Sprite_Handle_VehicleNoGun_Enemy(sSprite* pSprite) {
    pSprite->mVehicleType = eVehicle_Jeep;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_VehicleGun_Enemy(sSprite* pSprite) {
    pSprite->mVehicleType = eVehicle_JeepRocket;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Vehicle_Unk_Enemy(sSprite* pSprite) {

    pSprite->mVehicleType = eVehicle_DontTargetPlayer;
    
    pSprite->mTargetX = pSprite->mPosX;
    pSprite->mTargetX += 0x28;

    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile_Enemy(sSprite* pSprite) {
    pSprite->mPersonType = eSprite_PersonType_AI;
    pSprite->mVehicleType = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile2_Enemy(sSprite* pSprite) {
    pSprite->mPersonType = eSprite_PersonType_AI;
    pSprite->mVehicleType = eVehicle_Turret_Missile;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_HomingMissile_Enemy(sSprite* pSprite) {
    pSprite->mPersonType = eSprite_PersonType_AI;
    pSprite->mVehicleType = eVehicle_Turret_Homing;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile_Human(sSprite* pSprite) {
    pSprite->mPersonType = eSprite_PersonType_Human;
    pSprite->mVehicleType = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile2_Human(sSprite* pSprite) {
    pSprite->mPersonType = eSprite_PersonType_Human;
    pSprite->mVehicleType = eVehicle_Turret_Missile;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Vehicle_Sinking_1(sSprite* pSprite) {
    Sound_Play(pSprite, eSound_Effect_Vehicle_Sinking, 0x0F);
    pSprite->mFrameIndex -= 1;

    if (pSprite->mFrameIndex < 0)
        Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Vehicle_Sinking_2(sSprite* pSprite) {
    pSprite->mFrameIndex += 1;

    if (pSprite->mFrameIndex == 6)
        Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Turret_Cannon_Invulnerable(sSprite* pSprite) {
    
    if (pSprite->mAnimState == eSprite_Anim_Die1)
        pSprite->mAnimState = eSprite_Anim_None;

    pSprite->mPersonType = eSprite_PersonType_AI;
    pSprite->mVehicleType = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile_Invulnerable(sSprite* pSprite) {
    
    if (pSprite->mAnimState == eSprite_Anim_Die1)
        pSprite->mAnimState = eSprite_Anim_None;

    pSprite->mPersonType = eSprite_PersonType_AI;
    pSprite->mVehicleType = eVehicle_Turret_Missile;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Looping_Vehicle_Left(sSprite* pSprite) {
    
    pSprite->mVehicleType = eVehicle_DontTargetPlayer;

    if (pSprite->mPosX <= 6) {
        pSprite->mPosX = mMapLoaded->getWidthPixels() - 4;
        pSprite->field_75 = 0;
    }

    pSprite->mTargetX = pSprite->mPosX - 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);

    if (tool_RandomGet() & 0x0F)
        return;

    int16 Saved_F0 = pSprite->mPosX;
    int16 Saved_F4 = pSprite->mPosY;

    int16 Data0 = (tool_RandomGet() & 0x07) + 0x0A;
    pSprite->mPosX += Data0;
    pSprite->mPosY += -8;
    if (pSprite->mPosY < 0)
        pSprite->mPosY = 0;

    if (!Sprite_Create_Cannon(pSprite)) {
        Sound_Play(pSprite, 0x10, 0x1E);
    }

    pSprite->mPosX = Saved_F0;
    pSprite->mPosY = Saved_F4;
}

void cFodder::Sprite_Handle_Looping_Vehicle_Right(sSprite* pSprite) {

    pSprite->mVehicleType = eVehicle_DontTargetPlayer;

    if (pSprite->mPosX >= mMapLoaded->getWidthPixels()) {
        pSprite->mPosX = 0;
        pSprite->field_75 = 0;
    }

    pSprite->mTargetX = pSprite->mPosX + 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Looping_Vehicle_Up(sSprite* pSprite) {
    
    pSprite->mVehicleType = eVehicle_DontTargetPlayer;

    if (pSprite->mPosY <= 6) {
        pSprite->mPosY = mMapLoaded->getHeightPixels() - 4;
        pSprite->field_75 = 0;
    }

    pSprite->mTargetY = pSprite->mPosY - 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Looping_Vehicle_Down(sSprite* pSprite) {

    pSprite->mVehicleType = eVehicle_DontTargetPlayer;

    if (pSprite->mPosY >= mMapLoaded->getHeightPixels()) {
        pSprite->mPosY = 0;
        pSprite->field_75 = 0;
    }

    pSprite->mTargetY = pSprite->mPosY + 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Player_InVehicle(sSprite* pSprite) {
    pSprite->mSheetIndex = 0x7C;
    pSprite->mFrameIndex = 0;
    pSprite->mAnimState = eSprite_Anim_None;

    sSprite* Data24 = pSprite->mCurrentVehicle;
    if (Data24->mPosX == -32768 || Data24->mSpriteType == eSprite_Explosion) {
        //loc_22AEE
        pSprite->mInVehicle = 0;
        pSprite->mVehicleWalkTarget = 0;
        pSprite->mCurrentVehicle = 0;
        pSprite->mAnimState = eSprite_Anim_Hit;
        pSprite->field_64 = -1;

        pSprite->mDirection = tool_RandomGet() & 0x1FE;
    }

    //loc_22B3C

    pSprite->mPosX = Data24->mPosX;
    pSprite->mPosY = Data24->mPosY;
    pSprite->mHeight = Data24->mHeight;
}

int16 cFodder::Vehicle_Try_Exit_On_Mouse() {
    const int16* Data2C = mSprite_Width;
    const int16* Data30 = mSprite_Height_Top;

    int16 Data0 = mMouseX;
    int16 Data4 = mMouseY;
    int16 Data8, Data18;

    sSprite* Data20, *Dataa30;
    sSprite** Dataa2C;

    Data0 += mCameraX >> 16;
    Data4 += mCameraY >> 16;

    Data0 -= 0x0F;
    Data4 -= 3;

    if (!mSquad_CurrentVehicle)
        goto loc_31692;

    Data20 = mSquad_CurrentVehicle;
    if (Data20->mPosX == -32768)
        goto loc_31689;

    if (Data20->mPersonType != eSprite_PersonType_Human)
        goto loc_31689;

    Data18 = Data20->mSpriteType;
    Data8 = Data20->mPosX;
    if (Data0 < Data8)
        goto loc_31689;

    Data8 += Data2C[Data18];
    if (Data0 > Data8)
        goto loc_31689;

    //seg011:29DA
    Data8 = Data20->mPosY;
    Data8 -= Data20->mHeight;

    Data8 -= Data30[Data18];
    Data8 -= 0x14;
    if (Data4 < Data8)
        goto loc_31689;

    Data8 = Data20->mPosY;
    Data8 -= Data20->mHeight;
    if (Data4 > Data8)
        goto loc_31689;

    if (!Data20->mHeight)
        goto loc_31514;

    Data20->mInVehicle = -1;
    mVehicle_Input_Disabled = true;
    goto loc_3167D;

loc_31514:;
    Data0 = -3;
    Data4 = 8;

    if (Map_Terrain_Get_Type_And_Walkable(Data20, Data0, Data4))
        goto loc_31689;

    Data0 = mSquad_Selected;
    if (Data0 < 0)
        goto loc_31668;

    //seg011:2A84
    mSquad_EnteredVehicleTimer[Data0] = 0;
    Dataa2C = mSquads[Data0];

    //loc_31578
    for (; Data18 >= 0; --Data18) {

        if (*Dataa2C == INVALID_SPRITE_PTR)
            goto loc_31668;

        sSprite* Sprite = *Dataa2C++;

        if (!Sprite->mInVehicle)
            continue;

        //seg011:2AE9 

        Sprite->mInVehicle = 0;
        Dataa30 = Sprite->mCurrentVehicle;
        Sprite->mCurrentVehicle = 0;
        Sprite->mVehicleWalkTarget = 0;

        Sprite->mPosX = Dataa30->mPosX;
        Sprite->mPosY = Dataa30->mPosY;

        if (Sprite->mVehicleType == eVehicle_Turret_Cannon)
            goto loc_3162B;

        if (Sprite->mVehicleType == eVehicle_Turret_Missile)
            goto loc_3162B;

        Sprite->mPosX += 0x0F;
        Sprite->mPosY += -10;

    loc_3162B:;
        Sprite->mTargetX = Sprite->mPosX;
        Sprite->mTargetY = Sprite->mPosY;
        Sprite->mTargetX -= 6;
        Sprite->mTargetY += 0x10;
    }

loc_31668:;
    mVehicle_Input_Disabled = true;
    Data20->mInVehicle = -1;
    mMouse_Button_LeftRight_Toggle = false;
loc_3167D:;
    Data0 = -1;
    return -1;

loc_31689:;
    Data20->mInVehicle = 0;
loc_31692:;
    Data0 = 0;
    return 0;
}

void cFodder::Vehicle_Target_Set() {

    if (Mouse_Button_Right_Toggled() < 0)
        return;

    sSprite* Vehicle = mSquad_CurrentVehicle;
    Vehicle->mFiredWeaponType = -1;

    int16 PosX = mMouseX;
    int16 PosY = mMouseY;
    PosX += mCameraX >> 16;
    PosY += mCameraY >> 16;
    PosX -= 0x10;

    if (!PosX)
        PosX = 1;

    PosY -= 8;
    Vehicle->mWeaponTargetX = PosX;
    Vehicle->mWeaponTargetY = PosY;
}

void cFodder::Vehicle_Input_Handle() {

    if (!mButtonPressLeft) {
        mVehicle_Input_Disabled = false;
        return;
    }

    if (mVehicle_Input_Disabled)
        return;

    if (Mouse_Button_Left_Toggled() >= 0)
        if (Vehicle_Try_Exit_On_Mouse() < 0)
            return;

    sSprite* Data20 = mSquad_Leader;

    if (Data20 == INVALID_SPRITE_PTR)
        return;
    int16 Data0 = mMouseX + (mCameraX >> 16);
    int16 Data4 = mMouseY + (mCameraY >> 16);

    Data0 -= 0x1C;

    if (Data0 < 0)
        Data0 = 0;
    Data4 += 6;
    if (Data4 < 0)
        Data4 = 0x14;

    if (Data4 < 0x14)
        Data4 = 0x14;

    if (Data20->mVehicleType >= eVehicle_Helicopter) {
        if (Data20->field_50 <= 8)
            Data4 += 0x20;
    }

    mCamera_PanTargetX = Data0;
    mCamera_PanTargetY = Data4;

    mMouse_Locked = false;

    Data20->mTargetX = Data0;
    Data20->mTargetY = Data4;
}
