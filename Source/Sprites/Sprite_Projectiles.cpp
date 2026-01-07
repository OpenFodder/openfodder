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

void cFodder::Sprite_Bullet_SetData() {
    if (mSprite_TroopsAlive[0] == INVALID_SPRITE_PTR || mSprite_TroopsAlive[0] == 0)
        return;

    sSprite* Data20 = mSprite_TroopsAlive[0];
    int16 Data0 = 0;

    if (Data20->mMissionTroop) {
        Data0 = Data20->mMissionTroop->mRank;
        Data0 += 8;
        if (Data0 > 0x0F)
            Data0 = 0x0F;
    }

    mSprite_Weapon_Data = mSprite_Bullet_UnitData[Data0];
}

int16 cFodder::Sprite_Create_Missile(sSprite* pSprite, sSprite*& pData2C) {
    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Missile_Projectile_Counters[pSprite->mPersonType] == 2)
        return 0;

    if (!pSprite->mWeaponTargetX)
        return 0;

    int16 Data0 = 2;
    sSprite* Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return 0;

    ++mSprite_Missile_Projectile_Counters[pSprite->mPersonType];
    Data30 = pData2C + 1;

    pData2C->mPosX = pSprite->mPosX;
    pData2C->mPosXFrac = pSprite->mPosXFrac;
    pData2C->mPosY = pSprite->mPosY;
    pData2C->mPosYFrac = pSprite->mPosYFrac;

    Data30->mPosX = pSprite->mPosX;
    Data30->mPosXFrac = pSprite->mPosXFrac;
    Data30->mPosY = pSprite->mPosY;
    Data30->mPosYFrac = pSprite->mPosYFrac;

    pData2C->mPosX += mSprite_Missile_LaunchDistance_X;
    Data30->mPosX += mSprite_Missile_LaunchDistance_X;
    if (Data30->mPosX < 0)
        return 0;

    pData2C->mPosY += mSprite_Missile_LaunchDistance_Y;
    Data30->mPosY += mSprite_Missile_LaunchDistance_Y;

    if (Data30->mPosY < 0)
        return 0;

    pData2C->field_32 = 0;
    if (pSprite->mVehicleType == eVehicle_Turret_Cannon)
        pData2C->field_32 = -1;

    pData2C->mHeightFrac = pSprite->mHeightFrac;
    pData2C->mHeight = pSprite->mHeight;
    pData2C->mHeight -= 0x0D;
    pData2C->mTargetX = pSprite->mWeaponTargetX;
    pData2C->mTargetY = pSprite->mWeaponTargetY;
    pData2C->mTargetY += 0x10;

    if (pSprite->mPersonType != eSprite_PersonType_Human) {
        Sprite_Enemy_Set_Target(pData2C);
    }
    else {
        pData2C->mTargetX -= 1;
        pData2C->mTargetY += 3;
    }

    pData2C->mSheetIndex = 0xA3;
    Data30->mSheetIndex = 0x7E;
    pData2C->mFrameIndex = 0;
    Data30->mFrameIndex = 1;
    pData2C->mSpeed = pSprite->mSpeed;
    Data30->mSpeed = pSprite->mSpeed;
    pData2C->field_3A = 1;
    Data30->mHeightFrac = 0;
    Data30->mHeight = 0;
    pData2C->mPosY += 1;
    pData2C->mPosX += 3;
    pData2C->mSpriteType = eSprite_Missile;
    Data30->mSpriteType = eSprite_ShadowSmall;
    pData2C->mRowsToSkip = 0;
    Data30->mRowsToSkip = 0;
    pData2C->mPersonType = pSprite->mPersonType;
    Data30->mPersonType = pSprite->mPersonType;
    pData2C->mDrawOrder = eSprite_Draw_Second;
    Data30->mDrawOrder = eSprite_Draw_First;
    pData2C->mAnimState = eSprite_Anim_None;

    // HACK: Disable sound for Amiga Coverdisks (it crashes)
    if (!mVersionCurrent->isCoverDisk())
        Sound_Play(pSprite, eSound_Effect_Missile_Launch, 0x0F);

    return -1;
}

int16 cFodder::Sprite_Create_Grenade2(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;

    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Projectile_Counters[pSprite->mPersonType] == 2)
        return 0;

    if (!pSprite->mWeaponTargetX)
        return 0;

    Data0 = 2;
    sSprite* Data2C, *Data30;
    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return 0;

    ++mSprite_Projectile_Counters[pSprite->mPersonType];
    Data30 = Data2C + 1;
    Data2C->mPosX = pSprite->mPosX;
    Data30->mPosX = pSprite->mPosX;
    Data2C->mPosY = pSprite->mPosY;
    Data30->mPosY = pSprite->mPosY;
    Data2C->mSheetIndex = 0x7D;
    Data30->mSheetIndex = 0x7E;
    Data2C->mFrameIndex = 0;
    Data30->mFrameIndex = 0;

    Data8 = pSprite->mWeaponTargetX;
    DataC = pSprite->mWeaponTargetY;
    DataC += 6;

    Data2C->mTargetX = Data8;
    Data2C->mTargetY = DataC;

    if (pSprite->mPersonType != eSprite_PersonType_Human)
        Sprite_Enemy_Set_Target(Data2C);

    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 < 0x28)
        Data0 = 0x28;

    Data0 = Data0 / 5;
    if (Data0 > 0x64)
        Data0 = 0x64;

    Data2C->field_12 = Data0;
    Data2C->mSpeed = 0x32;
    Data30->mSpeed = 0x32;
    Data0 = pSprite->mHeight;
    Data0 += 2;

    Data2C->mHeightFrac = 0;
    Data2C->mHeight = Data0;
    Data30->mHeightFrac = 0;
    Data30->mHeight = 0;

    Data2C->mPosY += 1;
    Data2C->mPosX += 3;

    int32 Dataa0 = Data2C->field_12 << 16;
    Dataa0 >>= 1;
    if (Dataa0 > 0xE0000)
        Dataa0 = 0xE0000;

    Data2C->field_1A = Dataa0;
    Data2C->field_1A = 0;
    Data2C->mSpriteType = eSprite_Grenade;
    Data30->mSpriteType = eSprite_ShadowSmall;

    Data2C->mRowsToSkip = 0;
    Data30->mRowsToSkip = 0;
    Data2C->field_50 = 0;

    Data2C->mPersonType = pSprite->mPersonType;
    Data30->mPersonType = pSprite->mPersonType;

    Data2C->mDrawOrder = eSprite_Draw_Second;
    Data30->mDrawOrder = eSprite_Draw_First;
    Data2C->mAnimState = eSprite_Anim_None;

    if (pSprite->mPersonType != eSprite_PersonType_Human)
        Data2C->field_12 += 0x0A;

    return -1;
}

int16 cFodder::Sprite_Create_MissileHoming(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34) {

    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Missile_Projectile_Counters[pSprite->mPersonType] == 2)
        return 0;

    if (!pSprite->mWeaponTargetX)
        return 0;

    int16 Data0 = 2;
    sSprite *Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return 0;

    ++mSprite_Missile_Projectile_Counters[pSprite->mPersonType];

    Data30 = pData2C;
    ++Data30;

    //seg005:55E0
    pData2C->mPosX = pSprite->mPosX;
    pData2C->mPosXFrac = pSprite->mPosXFrac;
    pData2C->mPosY = pSprite->mPosY;
    pData2C->mPosYFrac = pSprite->mPosYFrac;

    Data30->mPosX = pSprite->mPosX;
    Data30->mPosXFrac = pSprite->mPosXFrac;
    Data30->mPosY = pSprite->mPosY;
    Data30->mPosYFrac = pSprite->mPosYFrac;

    pData2C->mPosX += mSprite_Missile_LaunchDistance_X;
    Data30->mPosX += mSprite_Missile_LaunchDistance_X;
    if (Data30->mPosX < 0)
        return 0;

    pData2C->mPosY += mSprite_Missile_LaunchDistance_Y;
    Data30->mPosY += mSprite_Missile_LaunchDistance_Y;

    if (Data30->mPosX < 0)
        return 0;

    //seg005:5672
    pData2C->field_1A_sprite = pData34;
    pData2C->mHeightFrac = pSprite->mHeightFrac;
    pData2C->mHeight = pSprite->mHeight;
    pData2C->mTargetX = pSprite->mWeaponTargetX;
    pData2C->mTargetY = pSprite->mWeaponTargetY;
    pData2C->mTargetY += 0x10;
    pData2C->mSheetIndex = 0xA3;
    Data30->mSheetIndex = 0x7E;
    pData2C->mFrameIndex = 0;
    Data30->mFrameIndex = 1;
    pData2C->mSpeed = 0;
    Data30->mSpeed = 0;
    pData2C->mHomingAccelFixed = 0x10000;
    if (pSprite->mPersonType != eSprite_PersonType_Human)
        pData2C->mHomingAccelFixed = 0x400;

    Data30->mHeightFrac = 0;
    Data30->mHeight = 0;
    pData2C->mPosY += 1;
    pData2C->mPosX += 3;
    pData2C->mSpriteType = eSprite_MissileHoming2;
    if (pSprite->mPersonType != eSprite_PersonType_Human)
        pData2C->mSpriteType = eSprite_MissileHoming;
    Data30->mSpriteType = eSprite_ShadowSmall;
    pData2C->mRowsToSkip = 0;
    Data30->mRowsToSkip = 0;
    pData2C->mPersonType = pSprite->mPersonType;
    Data30->mPersonType = pSprite->mPersonType;
    pData2C->mDrawOrder = eSprite_Draw_Second;
    Data30->mDrawOrder = eSprite_Draw_First;
    pData2C->mAnimState = eSprite_Anim_None;

    if (mVersionCurrent->isCoverDisk())
        Sound_Play(pSprite, 0x10, 0x0F);
    else
        Sound_Play(pSprite, eSound_Effect_Turret_Fire, 0x0F);
    return -1;
}

void cFodder::Sprite_Handle_Grenade(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data24;
    int16 a1;
    int32 Dataa4;

    if (pSprite->mAnimState)
        goto loc_1992D;

    if (!pSprite->field_12)
        goto loc_1992D;

    if (!pSprite->field_56)
        goto loc_19701;

    --pSprite->field_56;
    if (!pSprite->field_56) {
        // HACK: Disable grenade sound for cannon plus... it seems corrupted and causes a crash
        if (!mVersionCurrent->isCoverDisk())
            Sound_Play(pSprite, eSound_Effect_Grenade, 0x0F);
    }

    Data24 = pSprite->mSourceSprite;
    pSprite->mPosX = Data24->mPosX;
    pSprite->mPosXFrac = Data24->mPosXFrac;
    pSprite->mPosY = Data24->mPosY;
    pSprite->mPosYFrac = Data24->mPosYFrac;
    pSprite->mPosY += 1;
    pSprite->mPosX += 3;
    pSprite->mSheetIndex = 0x7C;
    pSprite->mFrameIndex = 0;
    return;

loc_19701:;
    pSprite->mSheetIndex = 0x7D;
    Data0 = pSprite->mTargetX;
    Data4 = pSprite->mTargetY;
    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= 1)
        pSprite->mSpeed = 0;

    if (Data0 <= 4)
        pSprite->mSpeed >>= 1;

    if (pSprite->mSpeed) {
        pSprite->mSpeed -= 1;
        Sprite_XY_Store(pSprite);

        mTmp_FrameNumber = pSprite->mFrameIndex;
        Data0 = pSprite->mTargetX;
        Data4 = pSprite->mTargetY;

        Sprite_Direction_Between_Points(pSprite, Data0, Data4);
        Sprite_Movement_Calculate(pSprite);
        Sprite_Handle_Grenade_Terrain_Check(pSprite);

        (pSprite + 1)->mDirection = pSprite->mDirection;
    }

    //loc_197C5
    if (!pSprite->field_50) {
        if (pSprite->mRowsToSkip) {
            pSprite->mSpeed = 0;
            goto loc_198D3;
        }
    }
    //loc_197EA
    a1 = (int16)(((int64)pSprite->field_1A) >> 16);

    if (a1 < 0)
        if (a1 < -8)
            pSprite->field_1A = (((int32)pSprite->field_1A & 0xFFFF) | -0x80000);

    Dataa4 = pSprite->field_1A;
    Dataa4 -= 0x18000;

    pSprite->field_1A = Dataa4;
    Dataa4 = pSprite->field_1A;

    if (!pSprite->field_50)
        goto loc_19877;

    if (Dataa4 >= 0)
        goto loc_19877;

    if (pSprite->mHeight >= 2)
        goto loc_19877;

    pSprite->mHeightFrac = 0;
    pSprite->mHeight = 0;

loc_19855:;

    Dataa4 = (Dataa4 >> 16) | ((Dataa4 & 0xFFFF) << 16);
    if (pSprite->mRowsToSkip >= 0x0A)
        goto loc_198D3;

    pSprite->mRowsToSkip -= Dataa4;
    goto loc_198D3;

loc_19877:;
    pSprite->mHeightFixed += Dataa4;

    if (pSprite->mHeight < 0) {
        pSprite->mHeightFixed = 0;

        if (pSprite->field_50)
            goto loc_19855;

        Dataa4 = (int64)pSprite->field_1A;
        Dataa4 = -Dataa4;
        Dataa4 >>= 1;
        pSprite->field_1A = Dataa4;
    }

loc_198D3:;
    if (!pSprite->mSpeed)
        pSprite->mHeight = 0;

    Data4 = pSprite->mHeight;
    Data4 >>= 4;
    if (Data4 > 3)
        Data4 = 3;

    pSprite->mFrameIndex = Data4;
    (pSprite + 1)->mFrameIndex = Data4;
    pSprite->field_12 -= 1;
    return;

loc_1992D:;

    if (pSprite->mRowsToSkip < 8) {
        pSprite->mSpriteType = eSprite_Explosion;
        Sprite_Projectile_Counters_Decrease(pSprite);
        Sprite_Destroy(pSprite + 1);
        return;
    }
    //loc_19957
    Sprite_Projectile_Counters_Decrease(pSprite);
    pSprite->mPosX = 0;
    pSprite->mPosY = 0;

    Sprite_Destroy(pSprite);
    Sprite_Destroy(pSprite + 1);
}

void cFodder::Sprite_Projectile_Counters_Decrease(sSprite* pSprite) {

    --mSprite_Projectile_Counters[pSprite->mPersonType];
}

void cFodder::Sprite_Handle_Bullet(sSprite* pSprite) {
    int16 Data8, DataC, Data0, Data4;
    int8 al;

    pSprite->field_64 += 1;

    if (!pSprite->field_2A)
        goto loc_19BA8;

    Data0 = pSprite->field_1A_sprite->mPosX;
    Data4 = pSprite->field_1A_sprite->mPosY;

    Data0 += pSprite->mProjectileOffsetX;
    Data4 += pSprite->mProjectileOffsetY;
    pSprite->mPosX = Data0;
    pSprite->mPosY = Data4;

    pSprite->field_2A -= 1;
    if (pSprite->field_2A)
        return;

    pSprite->mSheetIndex = 0x7F;
    pSprite->mFrameIndex = 0;

    if (pSprite->mPersonType != eSprite_PersonType_Human)
        pSprite->mFrameIndex = 3;

    Data0 = pSprite->field_1A_sprite->mPosX;
    Data4 = pSprite->field_1A_sprite->mPosY;
    Data0 += 7;
    Data4 += 5;
    pSprite->mPosX = Data0;
    pSprite->mPosY = Data4;

loc_19BA8:;
    if (pSprite->field_43)
        goto loc_19DCF;

    if (!pSprite->field_12)
        goto loc_19DCF;

    pSprite->field_12 -= 1;
    pSprite->field_3A += 1;

    if (pSprite->field_44)
        goto loc_19D24;

    Sprite_XY_Store(pSprite);

    if (pSprite->field_59)
        goto loc_19C6B;

    Data0 = pSprite->mWeaponTargetX;
    Data4 = pSprite->mWeaponTargetY;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);

    Data0 = pSprite->field_50;
    if (Data0) {
        pSprite->mDirection += Data0;
        pSprite->mDirection &= 0x1FE;
        pSprite->field_59 = -1;
    }
    //loc_19C4D

    if (pSprite->field_34 < 0) {
        pSprite->field_34 = pSprite->mDirection;
    }

loc_19C6B:;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto SpriteDestroy;
    pSprite->mSpeed = pSprite->field_4A;

    if (pSprite->field_59)
        goto loc_19D3C;

    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;
    Data8 = pSprite->mWeaponTargetX;
    DataC = pSprite->mWeaponTargetY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->mSpeed;
    Data4 >>= 3;

    if (Data0 > Data4)
        goto loc_19D3C;

    pSprite->mDrawOrder = eSprite_Draw_Second;
    pSprite->mPosX = pSprite->mWeaponTargetX;
    pSprite->mPosY = pSprite->mWeaponTargetY;
    pSprite->field_44 = -1;
    goto loc_19D3C;

loc_19D24:;
    pSprite->mDrawOrder = eSprite_Draw_First;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto SpriteDestroy;

loc_19D3C:;
    if (pSprite->field_64 > 2) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
            goto loc_19DCF;
    }

    if (!Sprite_Projectile_Collision_Check(pSprite))
        return;

    if (mSprites_Found_Count == 1)
        goto loc_19DCF;

    pSprite->field_43 = 1;
    pSprite->mDrawOrder = eSprite_Draw_OnTop;
    pSprite->mPosX -= 4;
    if (pSprite->mPosX < 0)
        goto loc_19E50;

    pSprite->mPosY -= 3;
    if (pSprite->mPosY < 0)
        goto loc_19E50;

    pSprite->mSheetIndex = 0x96;
    pSprite->mFrameIndex = 0;
    return;

loc_19DB0:;
    pSprite->field_12 = 0;
    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex >= 4)
        goto SpriteDestroy;

    return;

loc_19DCF:;
    al = pSprite->field_43;
    if (al)
        goto loc_19E1C;

    Sprite_Projectile_Collision_Check(pSprite);
    pSprite->field_43 = -1;
    pSprite->mDrawOrder = eSprite_Draw_First;
    pSprite->mPosX -= 4;
    if (pSprite->mPosX < 0)
        goto loc_19E50;

    pSprite->mPosY -= 3;
    if (pSprite->mPosY < 0)
        goto loc_19E50;

    pSprite->mFrameIndex = 3;
    al = pSprite->field_43;

loc_19E1C:;
    if (al >= 0)
        goto loc_19DB0;

    pSprite->field_12 = 0;
    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex < 8)
        return;

SpriteDestroy:;

    mSprite_Projectile_Counters[2] -= 1;
    Sprite_Destroy(pSprite);
    return;

loc_19E50:;
    pSprite->mPosX = 0;
    pSprite->mPosY = 0x1000;
    goto SpriteDestroy;
}

void cFodder::Sprite_Handle_Rocket(sSprite* pSprite) {
    sSprite* Data24 = 0;

    if (pSprite->field_56) {

        pSprite->field_56 -= 1;
        if (!pSprite->field_56) {
            if (mVersionCurrent->isCoverDisk())
                Sound_Play(pSprite, 0x10, 0x0F);
            else
                Sound_Play(pSprite, eSound_Effect_Rocket, 0x0F);

        }
        Data24 = pSprite->mSourceSprite;
        pSprite->mPosX = Data24->mPosX;
        pSprite->mPosXFrac = Data24->mPosXFrac;
        pSprite->mPosY = Data24->mPosY;
        pSprite->mPosYFrac = Data24->mPosYFrac;

        pSprite->mPosY += 1;
        pSprite->mPosX += 3;
        pSprite->mSheetIndex = 0x7C;
        pSprite->mFrameIndex = 0;
        return;
    }

    pSprite->mSheetIndex = 0xA3;
    int16 Data0 = pSprite->mTargetX;
    int16 Data4 = pSprite->mTargetY;
    int16 Data8 = pSprite->mPosX;
    int16 DataC = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= 7)
        goto loc_1B843;

    Data0 = pSprite->mTargetX;
    Data4 = pSprite->mTargetY;
    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    Data0 = pSprite->mDirection;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->mFrameIndex = Data0;

    Data0 = -9;
    Data4 = 2;

    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        return;

loc_1B843:;
    // Hit Target
    pSprite->mSpriteType = eSprite_Explosion;
    Sprite_Projectile_HitTarget(pSprite);
    Data24 = pSprite + 1;

    Sprite_Destroy(Data24);
}

void cFodder::Sprite_Handle_Enemy_Rocket(sSprite* pSprite) {

    if (Sprite_Handle_Soldier_Animation(pSprite))
        return;

    if (pSprite->mAnimState != eSprite_Anim_None)
        return;

    mSprite_FaceWeaponTarget = 0;
    pSprite->field_43 = -1;

    Sprite_Update_Follower_Target(pSprite);

    mTroop_Weapon_Bullet_Disabled = true;
    mTroop_Weapon_Grenade_Disabled = true;
    mTroop_Weapon_Rocket_Disabled = true;

    Sprite_Handle_Troop_Weapon(pSprite);
    Sprite_Update_DirectionMod(pSprite);

    mSprite_FaceWeaponTarget = 0;
    word_3ABB1 = 0;

    pSprite->mFiredWeaponType = 3;
    pSprite->mTurnTowardEnemy = -1;
    pSprite->field_55 = 0;
    pSprite->mFrameIndex = 0;

    Sprite_Handle_Troop_Direct_TowardWeaponTarget_WithRestore(pSprite);
    pSprite->mFrameIndex = 2;
}

void cFodder::Sprite_Handle_GrenadeBox(sSprite* pSprite) {

    if (pSprite->mAnimState) {
        pSprite->mSpriteType = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->mSheetIndex = 0xC2;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Grenades[mSquad_Selected] += 4;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_RocketBox(sSprite* pSprite) {
    if (pSprite->mAnimState) {
        pSprite->mSpriteType = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->mSheetIndex = 0xC3;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] += 4;

    // Plus uses homing missiles
    if (mVersionCurrent->isCoverDisk())
        mSquad_Leader->field_75 |= eSprite_Flag_HomingMissiles;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Missile(sSprite* pSprite) {

    dword_3B24B = -1;
    Sprite_Create_FireTrail(pSprite);

    pSprite->mSheetIndex = 0xA3;
    int16 Data0 = pSprite->mTargetX;
    int16 Data4 = pSprite->mTargetY;
    int16 Data8 = pSprite->mPosX;
    int16 DataC = pSprite->mPosY;
    int16 Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 7)
        goto loc_1BECD;

    if (pSprite->mHeight > 4) {
        pSprite->mHeightFixed -= 0xA000;
    }

    Data0 = pSprite->mTargetX;
    Data4 = pSprite->mTargetY;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    if (mSprite_Bullet_Destroy)
        goto loc_1BECD;

    Data0 = pSprite->mDirection;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->mFrameIndex = Data0;

    if (pSprite->mSpeed < 0x60)
        pSprite->mSpeed += pSprite->field_3A;

    Data0 = mMission_EngineTicks;
    Data0 &= 3;

    if (!Data0)
        pSprite->field_3A <<= 1;

    if (!pSprite->field_32)
        return;

    Data0 = -9;
    Data4 = 2;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        return;

loc_1BECD:;

    pSprite->mSpriteType = eSprite_Explosion2;
    pSprite->mPosY -= 4;
    Sprite_Projectile_HitTarget(pSprite);

    Sprite_Destroy(pSprite + 1);
}

void cFodder::Sprite_Handle_MissileHoming(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data34 = 0;

    if (pSprite->mAnimState)
        goto MissileExplode;

    dword_3B24B = -1;
    Sprite_Create_FireTrail(pSprite);

    pSprite->mSheetIndex = 0xA3;
    Data34 = pSprite->field_1A_sprite;

    if (!Data34 || Data34->mPosX == -32768) {
        pSprite->mSpeed += 2;
        pSprite->mHeight -= 2;
        if (pSprite->mHeight >= 0) {
            Sprite_Movement_Calculate(pSprite);
            return;
        }

        pSprite->mHeight = 0;
        goto MissileExplode;
    }

    Data0 = Data34->mPosX + 8;
    Data4 = Data34->mPosY + 8;
    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosY;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->mSpeed >> 4;
    Data4 += 1;

    if (Data0 <= Data4)
        goto MissileExplode;

    if (Data34->mHeight <= 0x10) {

        // Distance to target > 0x30?
        if (Data0 > 0x30) {
            Data0 = pSprite->mPosX;
            Data4 = pSprite->mPosY;
            Data8 = Data34->mPosX;
            DataC = Data34->mPosY;
            Data8 += 8;
            DataC += 8;

            if (!Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC)) {
                if (pSprite->mHeight > 8)
                    pSprite->mHeightFixed -= 0x12000;
            }
            else {

                if (pSprite->mHeight < 0x18)
                    pSprite->mHeightFixed += 0x28000;
            }

        }
        else {
            // Near Target
            Data4 = Data34->mHeight;
            Data4 -= pSprite->mHeight;
            Data4 >>= 3;
            pSprite->mHeight += Data4;
        }
    }
    else {

        Data4 = pSprite->mHeight;
        if (Data4 != Data34->mHeight) {

            if (Data4 > Data34->mHeight)
                pSprite->mHeightFixed -= 0x8000;
            else
                pSprite->mHeightFixed += 0x8000;
        }
    }

    Data0 = Data34->mPosX;
    Data0 += 8;
    Data4 = Data34->mPosY;
    Data4 += 8;
    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    Data4 = pSprite->mFrameIndex;
    Data0 = pSprite->mDirection;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->mFrameIndex = Data0;

    if (pSprite->mPersonType == eSprite_PersonType_Human || Data4 == Data0) {
        if (pSprite->mSpeed <= 0x3C) {

            Data0 = (int16)(pSprite->mHomingAccelFixed >> 16);
            pSprite->mSpeed += Data0;
            pSprite->mHomingAccelFixed = pSprite->mHomingAccelFixed + 0x200;
        }
        else {
            pSprite->mSpeed = 0x3C;
        }

        return;
    }

    if (pSprite->mSpeed > 0x1E) {

        pSprite->mHomingAccelFixed = 0;
        pSprite->mSpeed -= 2;
        if (pSprite->mSpeed < 0)
            pSprite->mSpeed = 0;
    }

    return;

MissileExplode:;
    pSprite->mSpriteType = eSprite_Explosion2;
    pSprite->mPosY -= 4;
    Sprite_Projectile_HitTarget(pSprite);
    Sprite_Destroy(pSprite + 1);

}

void cFodder::Sprite_Handle_Sparks(sSprite* pSprite) {
    Sprite_Movement_Calculate(pSprite);
    int32 Data0 = (int32)pSprite->field_1A;

    Data0 += pSprite->mHeightFrac;

    pSprite->mHeightFixed = Data0;

    if (pSprite->mHeightFixed < 0) {
        pSprite->mHeightFixed = 0;
        Data0 = -Data0;
        Data0 >>= 1;
    }

    Data0 -= 0x22000;
    pSprite->field_1A = Data0;

    Data0 = Data0 >> 16;
    if (!pSprite->mFrameIndex)
        goto loc_1C248;

    if (pSprite->mHeight)
        goto loc_1C262;

    if (Data0 > 1)
        goto loc_1C262;

loc_1C248:;
    pSprite->mFrameIndex++;
    if (pSprite->mFrameIndex >= 3) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

loc_1C262:;
    Sprite_Create_FireTrail(pSprite);
}

void cFodder::Sprite_Handle_FireTrail(sSprite* pSprite) {
    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (pSprite->field_2A)
        return;

    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex != 4)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Mine(sSprite* pSprite) {
    pSprite->mDrawOrder = eSprite_Draw_First;

    if (!pSprite->mAnimState) {
        int16 Data0;
        pSprite->mSheetIndex = 0xC7;
        Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0);
    }
    else {
        pSprite->mSpriteType = eSprite_Explosion2;
    }
}

void cFodder::Sprite_Handle_Mine2(sSprite* pSprite) {
    int16 Data0;

    pSprite->mDrawOrder = eSprite_Draw_First;
    if (pSprite->mAnimState)
        goto loc_1C406;

    if (pSprite->field_2A) {
        pSprite->field_2A += 1;
        if (pSprite->field_2A >= 0x0A)
            goto loc_1C406;
    }

    Data0 = 0;
    pSprite->mSheetIndex = 0xC8;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (mSquad_Leader->mAnimState)
        return;

    if (!mSquad_Leader->mSpeed)
        return;

    if (mSquad_Leader->mRowsToSkip)
        return;

    if (mSquad_Leader->mHeight)
        return;

    mSquad_Leader->mAnimState = eSprite_Anim_Slide1;
    mSquad_Leader->field_2A = 1;
    return;

loc_1C406:;
    pSprite->mSpriteType = eSprite_Explosion;
}

void cFodder::Sprite_Handle_Spike(sSprite* pSprite) {
    sSprite* Data28 = mSquad_Leader;
    int16 Data0 = 0;

    pSprite->mSheetIndex = 0xC9;

    if (pSprite->mFrameIndex)
        goto loc_1C4AD;

    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (Data0 >= 6)
        return;

    if (Data0 == 5)
        goto loc_1C4AD;

    Data28->mPosX = pSprite->mPosX;
    Data28->mPosX -= 3;
    Data28->mPosY = pSprite->mPosY;
    Data28->mPosY += 1;
    Data28->mAnimState = eSprite_Anim_Die4;
    Data28->field_64 = -1;
    Data28->mHeight = 7;

loc_1C4AD:;
    if (pSprite->mFrameIndex >= 2)
        return;

    pSprite->mFrameIndex += 1;
}

void cFodder::Sprite_Handle_Seal_Mine(sSprite* pSprite) {
    Sprite_Handle_Seal(pSprite);

    if (pSprite->mSheetIndex == 0xE1)
        return;

    if (pSprite->mAnimState)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    sSprite* Data24 = mSquad_Leader;
    int16 Data0 = Data24->mPosX;
    int16 Data4 = Data24->mPosY;
    int16 Data8 = pSprite->mPosX;
    int16 DataC = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x14)
        return;

    pSprite->mAnimState = eSprite_Anim_Die4;
}

void cFodder::Sprite_Handle_Spider_Mine(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data14 = 0;

    if (pSprite->mAnimState)
        goto loc_1DC50;

    pSprite->mSheetIndex = 0xE2;
    if (!pSprite->mWeaponTargetX) {
        pSprite->mWeaponTargetX = -1;
        Data0 = tool_RandomGet() & 0x1E;
        pSprite->field_2A = Data0;
    }

    Data0 = pSprite->field_2A;
    Data0 += 2;
    Data0 &= 0x1E;
    pSprite->field_2A = Data0;

    pSprite->mFrameIndex = mSprite_SpiderMine_Frames[Data0 / 2];
    Data14 = mSquad_Leader;

    if (Data14 == INVALID_SPRITE_PTR)
        return;

    Data0 = Data14->mPosX;
    Data4 = Data14->mPosY;
    Data8 = pSprite->mPosX;
    Data8 += 4;
    DataC = pSprite->mPosY;
    DataC -= 2;
    Data10 = 0x20;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x0A)
        return;

loc_1DC50:;
    pSprite->mSpriteType = eSprite_Explosion;
    pSprite->mTargetX = -32763;
    pSprite->mTargetY = -3;
}

void cFodder::Sprite_Handle_Bonus_Rockets(sSprite* pSprite) {
    int16 Data0;

    // Blow up?
    if (pSprite->mAnimState) {
        pSprite->mSpriteType = eSprite_Explosion;
        return;
    }

    pSprite->mSheetIndex = 0xE4;
    pSprite->mFrameIndex = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    // Has Homing missiles
    mSquad_Leader->field_75 |= eSprite_Flag_HomingMissiles;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 50;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_MissileHoming2(sSprite* pSprite) {

    Sprite_Handle_MissileHoming(pSprite);
}

int16 cFodder::Sprite_Create_Bullet(sSprite* pSprite) {
    int16 Data0 = 1, Data8, Data4;
    sSprite* Data2C = 0, *Data30 = 0;

    if (mPhase_Completed_Timer)
        return -1;

    if (!pSprite->mWeaponTargetX)
        return -1;

    if (pSprite == mSquad_Leader)
        if (pSprite->mFiredWeaponType == 1)
            return -1;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    pSprite->mFiredWeaponType = 2;
    pSprite->mTurnTowardEnemy = -1;
    pSprite->mWeaponCooldown = 8;
    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mPosYFrac = pSprite->mPosYFrac;
    Data2C->mSheetIndex = 0x9D;
    //seg005:1023 
    Data0 = pSprite->mFaceDirection;
    Data0 >>= 1;
    Data0 &= 0x07;
    Data2C->mFrameIndex = Data0;
    Data2C->field_12 = 9;

    if (pSprite->mSpriteType != eSprite_Player) {

        Data0 = pSprite->mAIAggression;
        Data0 >>= 3;
        Data0 += 8;
        if (Data0 > 0x10)
            Data0 = 0x10;
        Data2C->field_12 = Data0;
    }
    else {

        Data2C->mKillerTroop = pSprite->mMissionTroop;
        Data2C->mHasKillerTroop = -1;

        // Bullet Travel time
        Data0 = mSprite_Weapon_Data.mAliveTime;
        Data0 += mSprite_Bullet_Time_Modifier;
        Data2C->field_12 = Data0;
    }

    Data2C->mSpriteType = eSprite_Bullet;
    Data2C->mHeightFixed = pSprite->mHeightFixed;
    Data2C->mHeight += 6;
    Data2C->mRowsToSkip = pSprite->mRowsToSkip;
    Data2C->mPersonType = pSprite->mPersonType;
    Data2C->field_32 = pSprite->field_32;
    Data2C->mDrawOrder = eSprite_Draw_Second;
    if (pSprite->mPersonType != eSprite_PersonType_Human)
        goto loc_2087D;

    Data0 = mSprite_Weapon_Data.mSpeed;
    Data0 += mSprite_Bullet_Fire_Speed_Modifier;
    Data2C->field_4A = Data0;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 <<= 3;
    Data2C->mSpeed = Data0;
    goto loc_208A6;

loc_2087D:;
    // AI Fire Speed
    Data0 = 0x3C;
    Data0 += pSprite->mAIAggression;
    Data2C->field_4A = Data0;
    Data2C->mSpeed = 0x18;

loc_208A6:;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data2C->mWeaponTargetX = pSprite->mWeaponTargetX;
    Data2C->mWeaponTargetY = pSprite->mWeaponTargetY;
    if (pSprite->mSpriteType != eSprite_Player)
        Data2C->mWeaponTargetX += 7;
    else
        Data2C->mWeaponTargetY += 0x0F;

    Data2C->mRowsToSkip = 0;
    Data2C->field_3A = 0;

    Data2C->field_1A_sprite = pSprite;
    Data2C->field_2A = 2;
    Data2C->mProjectileOffsetX = 0;
    Data2C->mProjectileOffsetY = 2;

    if (Data2C->mFrameIndex == 4)
        Data2C->mProjectileOffsetY = -1;

    Data2C->field_34 = -1;
    Data2C->field_50 = 0;
    Data2C->field_59 = 0;
    Data8 = 7;

    // Is Human?
    if (pSprite->mPersonType == eSprite_PersonType_Human) {

        if (pSprite == mSquad_Leader) {

            mSprite_Bullet_Deviate_Counter += 1;
            mSprite_Bullet_Deviate_Counter &= 3;

            if (!mSprite_Bullet_Deviate_Counter)
                goto loc_209F3;

            Data8 = mSprite_Weapon_Data.mDeviatePotential;
        }
        else {

            Data8 = Mission_Troop_GetDeviatePotential(pSprite->mMissionTroop);
        }
    }

    Data0 = tool_RandomGet();
    Data4 = Data0;

    Data0 &= Data8;
    if (Data4 < 0)
        Data0 = -Data0;

    Data2C->field_50 = Data0;

loc_209F3:;
    Data2C->field_64 = 0;
    Data0 = tool_RandomGet() & 1;
    if (!Data0)
        Data4 = 0x11;
    else
        Data4 = 0x10;

    Sound_Play(pSprite, Data4, 0);
    return 0;
}

int16 cFodder::Sprite_Create_Grenade(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data2C = 0, *Data30 = 0;

    if (mPhase_Completed_Timer)
        goto loc_20ADE;

    if (mSprite_Projectile_Counters[pSprite->mPersonType] == 2)
        goto loc_20ADE;

    if (!pSprite->mWeaponTargetX)
        goto loc_20ADE;

    if (pSprite->mSpriteType != eSprite_Enemy)
        goto loc_20AC1;

    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;
    Data8 = pSprite->mWeaponTargetX;
    DataC = pSprite->mWeaponTargetY;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 >= 0x82)
        goto loc_20ADE;

loc_20AC1:;

    Data0 = 2;

    if (!Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        goto loc_20B0A;

    return -1;

loc_20ADE:;
    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = false;

    return -1;

loc_20B0A:;
    if (pSprite->mSpriteType != eSprite_Player)
        goto loc_20B6E;

    Data0 = pSprite->field_32;
    Data4 = Data0;
    if (!mSquad_Grenades[Data0])
        goto loc_20ADE;

    --mSquad_Grenades[Data0];
    mGUI_RefreshSquadGrenades[Data4] = -1;

loc_20B6E:;

    ++mSprite_Projectile_Counters[pSprite->mPersonType];
    pSprite->field_55 = 1;
    pSprite->mTurnTowardEnemy = -1;
    pSprite->mFiredWeaponType = 1;

    Data30 = Data2C;
    ++Data30;
    //seg005:14CE
    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data30->mPosX = pSprite->mPosX;
    Data30->mPosXFrac = pSprite->mPosXFrac;
    //seg005:14F4
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mPosYFrac = pSprite->mPosYFrac;
    Data30->mPosY = pSprite->mPosY;
    Data30->mPosYFrac = pSprite->mPosYFrac;
    Data2C->mSheetIndex = 0x7D;
    Data30->mSheetIndex = 0x7E;
    Data2C->mFrameIndex = 0;
    Data30->mFrameIndex = 0;
    Data2C->field_12 = 0x10;
    Data0 = tool_RandomGet();
    Data0 &= 0x0F;
    Data2C->field_12 = Data0;
    Data8 = pSprite->mWeaponTargetX;
    DataC = pSprite->mWeaponTargetY;
    DataC += 0x06;
    //seg005:158C
    Data2C->mTargetX = Data8;
    Data2C->mTargetY = DataC;
    Data0 = 0;
    Data0 = pSprite->mPosX;
    Data4 = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 < 0x14)
        Data0 = 0x14;

    Data0 /= 5;

    if (Data0 > 0x64)
        Data0 = 0x64;

    //loc_20CEA
    Data2C->field_12 = Data0;
    Data2C->mSpeed = 0x32;
    Data30->mSpeed = 0x32;
    Data2C->mHeightFrac = 0x0000;
    Data2C->mHeight = 0x7;
    Data30->mHeightFrac = 0;
    Data30->mHeight = 0;
    Data2C->mPosY += 1;
    Data2C->mPosX += 3;
    int32 Dataa0 = Data2C->field_12 << 16;

    Dataa0 >>= 1;
    if (Dataa0 > 0x0E0000)
        Dataa0 = 0x0E0000;

    Data2C->field_1A = Dataa0;
    Data2C->mSpriteType = eSprite_Grenade;
    Data30->mSpriteType = eSprite_ShadowSmall;
    Data2C->mRowsToSkip = 0;
    Data30->mRowsToSkip = 0;
    Data2C->field_50 = 0;
    Data2C->mPersonType = pSprite->mPersonType;
    Data30->mPersonType = pSprite->mPersonType;
    Data2C->field_32 = pSprite->field_32;
    Data2C->mDrawOrder = eSprite_Draw_Second;
    Data30->mDrawOrder = eSprite_Draw_First;
    Data2C->mAnimState = eSprite_Anim_None;
    if (pSprite->mSpriteType == eSprite_Enemy)
        Data2C->field_12 += 0x1C;
    Data2C->field_56 = 4;
    Data2C->mSourceSprite = pSprite;

    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = false;

    return 0;
}

void cFodder::Sprite_Handle_Grenade_Terrain_Check(sSprite* pSprite) {
    int16 Data0 = -3;
    int16 Data4 = 2;

    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_21599;

    if (Data4 == eTerrainFeature_Drop || Data4 == eTerrainFeature_Drop2) {

        if (pSprite->mHeight <= 1) {
            pSprite->field_12 = 1;
            return;
        }
    }
    //loc_21561
    if (Data4 == eTerrainFeature_QuickSand || Data4 == eTerrainFeature_WaterEdge
        || Data4 == eTerrainFeature_Water) {

        if (pSprite->mHeight)
            return;

        pSprite->mRowsToSkip = 6;
        (pSprite + 1)->mRowsToSkip = 6;
        return;
    }
    //loc_21597
    return;

loc_21599:;
    if (pSprite->mHeight >= 8)
        return;

    pSprite->mPosX = mStoredSpriteX >> 16;
    Data0 = -3;
    Data4 = 2;

    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4)) {
        Data0 = 0x100;
        Data0 -= pSprite->mDirection;
        Data0 &= 0x1FE;
        pSprite->mDirection = Data0;
    }
    else {
        Data0 = 0x200;
        Data0 -= pSprite->mDirection;
        Data0 &= 0x1FE;
        pSprite->mDirection = Data0;
    }
    Sprite_XY_Restore(pSprite);
}

int16 cFodder::Sprite_Projectile_Collision_Check(sSprite* pSprite) {

    // If the projectile doesn't belong to a human, check if it hit a squad member
    if (pSprite->mPersonType != eSprite_PersonType_Human) {

        int16 Data8 = pSprite->mPosX;
        int16 DataC = pSprite->mPosX + 6;
        int16 Data10 = pSprite->mPosY - 9;
        int16 Data14 = pSprite->mPosY - 4;
        word_3AA45 = 1;

        return Squad_Members_Find_In_Region(pSprite, Data8, DataC, Data10, Data14);
    }

    //loc_21673
    int16 Data8 = pSprite->mPosX - 6;
    int16 DataC = pSprite->mPosX + 10;
    int16 Data10 = pSprite->mPosY - 10;
    int16 Data14 = pSprite->mPosY + 6;

    if (mVersionCurrent->isAmigaTheOne()) {
        Data8 = pSprite->mPosX;
        DataC = pSprite->mPosX + 4;
        Data10 = pSprite->mPosY - 4;
        Data14 = pSprite->mPosY;
    }

    word_3AA45 = 1;
    sSprite* Data24 = 0;
    if (Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14) >= 0)
        return 0;

    if (Data24->mSpriteType == eSprite_Enemy) {

		Data24->field_5E_Squad = pSprite->field_5E_Squad;
        Data24->field_5E = pSprite->field_5E;
        Data24->mHasKillerTroop = pSprite->mHasKillerTroop;
    }

    return -1;
}

void cFodder::Sprite_Create_Sparks(sSprite* pSprite, int16 pData18) {
    int16 Data0 = 2;
    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data30 = Data2C + 1;
    Data2C->mPosX = pSprite->mPosX + 2;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data2C->mPosY = pSprite->mPosY - 4;
    Data2C->mPosYFrac = pSprite->mPosYFrac;

    Data30->mPosX = Data2C->mPosX;
    Data30->mPosY = Data2C->mPosY;

    Data2C->mHeightFixed = pSprite->mHeightFixed;

    Data30->mHeight = 0;
    Data2C->mSheetIndex = 0xC5;
    Data30->mSheetIndex = 0x7E;
    Data2C->mFrameIndex = 0;
    Data30->mFrameIndex = 0;
    Data2C->mSpeed = 0x0C;
    Data30->mSpeed = 0x0C;
    Data2C->mSpriteType = eSprite_Sparks;
    Data30->mSpriteType = eSprite_ShadowSmall;
    Data2C->mRowsToSkip = 0;
    Data30->mRowsToSkip = 0;
    Data2C->mDrawOrder = eSprite_Draw_Second;
    Data30->mDrawOrder = eSprite_Draw_First;
    Data2C->mAnimState = eSprite_Anim_None;

    Data2C->mDirection = pData18;
    Data2C->field_1A = 0x0A8000;

}

void cFodder::Sprite_Create_FireTrail(sSprite* pSprite) {

    if (!pSprite->mWasDrawn)
        return;

    //  cmp     ds:word_3B1A9, 14h
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mPosYFrac = pSprite->mPosYFrac;

    if (dword_3B24B == 0) {
        Data2C->mPosY -= 3;
    }
    else {
        Data2C->mPosY += 2;
        dword_3B24B = 0;
    }

    Data2C->mHeight = pSprite->mHeight;
    Data2C->mRowsToSkip = 0;
    Data2C->mSheetIndex = 0xC6;
    Data2C->mFrameIndex = 0;
    Data2C->mSpriteType = eSprite_FireTrail;
}

int16 cFodder::Sprite_Create_Rocket(sSprite* pSprite) {
    sSprite* Data2C = 0, *Data30 = 0, *Data34 = 0;
    int16 Data0, Data4, Data8, DataC;

    if (mPhase_Completed_Timer) {
    loc_22592:;

        if (pSprite == mSquad_Leader)
            mMouse_Button_LeftRight_Toggle = false;

        return -1;
    }

    if (mSprite_Missile_Projectile_Counters[pSprite->mPersonType] == 2)
        goto loc_22592;

    if (!pSprite->mWeaponTargetX)
        goto loc_22592;

    //seg005:2E39
    if (pSprite->mSpriteType == eSprite_Enemy) {
        Data0 = pSprite->mPosX;
        Data4 = pSprite->mPosY;
        Data8 = pSprite->mWeaponTargetX;
        DataC = pSprite->mWeaponTargetY;

        // Don't let the AI fire further than 129
        Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
        if (Data0 >= 0x82)
            goto loc_22592;
    }

    Data0 = 2;
    Sprite_Get_Free_Max42(Data0, Data2C, Data30);

    // No Free sprites?
    if (Data0)
        return -1;

    if (pSprite->mSpriteType == eSprite_Player) {
        Data0 = pSprite->field_32;
        Data4 = Data0;

        if (!mSquad_Rockets[Data0])
            goto loc_22592;

        --mSquad_Rockets[Data0];
        mGUI_RefreshSquadRockets[Data4] = -1;
    }

    ++mSprite_Missile_Projectile_Counters[pSprite->mPersonType];
    pSprite->field_55 = 1;
    pSprite->mTurnTowardEnemy = -1;
    pSprite->mFiredWeaponType = 3;
    Data30 = Data2C;
    Data30++;

    //seg005:2F86 
    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data30->mPosX = pSprite->mPosX;
    Data30->mPosXFrac = pSprite->mPosXFrac;
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mPosYFrac = pSprite->mPosYFrac;
    Data30->mPosY = pSprite->mPosY;
    Data30->mPosYFrac = pSprite->mPosYFrac;

    //seg005:2FD2
    Data2C->mTargetX = pSprite->mWeaponTargetX;
    Data2C->mTargetY = pSprite->mWeaponTargetY;
    Data2C->mTargetY += 0x10;
    Data2C->mSheetIndex = 0x7C;
    Data30->mSheetIndex = 0x7E;
    Data2C->mDirection = 0;
    Data30->mDirection = 1;
    Data2C->mSpeed = 0x64;
    Data30->mSpeed = 0x64;
    Data2C->mHeightFrac = 0x0000;
    Data2C->mHeight = 0x0A;

    Data30->mHeightFrac = 0;
    Data30->mHeight = 0;

    Data2C->mPosY++;
    Data2C->mPosX += 3;

    // Amiga Power always has homing missiles
    if (pSprite->mPersonType == eSprite_PersonType_Human &&
        (mVersionCurrent->isCoverDisk() || (pSprite->field_75 & eSprite_Flag_HomingMissiles))) {

        // Within lock on range?
        if (Sprite_Homing_LockInRange(pSprite, Data34)) {

            Data2C->field_1A_sprite = Data34;
            Data2C->mSpriteType = eSprite_MissileHoming2;
            Data2C->mHomingAccelFixed = 0x10000;
            Data2C->mTargetX = pSprite->mWeaponTargetX;
            Data2C->mTargetY = pSprite->mWeaponTargetY;
            Data2C->mTargetY += 0x10;
            Data2C->mAnimState = eSprite_Anim_None;
            Data2C->mSpeed = 0;
            Data30->mSpeed = 0;

        }
        else {
            Data2C->field_32 = 0;
            Data2C->mSpriteType = eSprite_Missile;
        }

    }
    else {

        Data2C->mSpriteType = eSprite_Rocket;
        Data2C->mSourceSprite = pSprite;
    }

    Data30->mSpriteType = eSprite_ShadowSmall;
    Data2C->mRowsToSkip = 0;
    Data30->field_32 = 0;
    Data2C->mPersonType = pSprite->mPersonType;
    Data30->mPersonType = Data2C->mPersonType;
    Data2C->field_32 = pSprite->field_32;
    Data2C->mDrawOrder = eSprite_Draw_Second;
    Data30->mDrawOrder = eSprite_Draw_First;
    Data2C->field_56 = 6;

    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = false;

    return 0;
}

void cFodder::Sprite_Projectile_HitTarget(sSprite* pSprite) {

    mSprite_Missile_Projectile_Counters[pSprite->mPersonType] -= 1;
}
