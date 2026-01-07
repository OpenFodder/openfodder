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

    if (Data20->field_46_mission_troop) {
        Data0 = Data20->field_46_mission_troop->mRank;
        Data0 += 8;
        if (Data0 > 0x0F)
            Data0 = 0x0F;
    }

    mSprite_Weapon_Data = mSprite_Bullet_UnitData[Data0];
}

int16 cFodder::Sprite_Create_Missile(sSprite* pSprite, sSprite*& pData2C) {
    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Missile_Projectile_Counters[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    int16 Data0 = 2;
    sSprite* Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return 0;

    ++mSprite_Missile_Projectile_Counters[pSprite->field_22];
    Data30 = pData2C + 1;

    pData2C->field_0 = pSprite->field_0;
    pData2C->field_2 = pSprite->field_2;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_6 = pSprite->field_6;

    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;

    pData2C->field_0 += mSprite_Missile_LaunchDistance_X;
    Data30->field_0 += mSprite_Missile_LaunchDistance_X;
    if (Data30->field_0 < 0)
        return 0;

    pData2C->field_4 += mSprite_Missile_LaunchDistance_Y;
    Data30->field_4 += mSprite_Missile_LaunchDistance_Y;

    if (Data30->field_4 < 0)
        return 0;

    pData2C->field_32 = 0;
    if (pSprite->field_6F == eVehicle_Turret_Cannon)
        pData2C->field_32 = -1;

    pData2C->field_1E = pSprite->field_1E;
    pData2C->field_20 = pSprite->field_20;
    pData2C->field_20 -= 0x0D;
    pData2C->field_26 = pSprite->field_2E;
    pData2C->field_28 = pSprite->field_30;
    pData2C->field_28 += 0x10;

    if (pSprite->field_22 != eSprite_PersonType_Human) {
        Sprite_Enemy_Set_Target(pData2C);
    }
    else {
        pData2C->field_26 -= 1;
        pData2C->field_28 += 3;
    }

    pData2C->field_8 = 0xA3;
    Data30->field_8 = 0x7E;
    pData2C->field_A = 0;
    Data30->field_A = 1;
    pData2C->field_36 = pSprite->field_36;
    Data30->field_36 = pSprite->field_36;
    pData2C->field_3A = 1;
    Data30->field_1E = 0;
    Data30->field_20 = 0;
    pData2C->field_4 += 1;
    pData2C->field_0 += 3;
    pData2C->field_18 = eSprite_Missile;
    Data30->field_18 = eSprite_ShadowSmall;
    pData2C->field_52 = 0;
    Data30->field_52 = 0;
    pData2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;
    pData2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    pData2C->field_38 = eSprite_Anim_None;

    // HACK: Disable sound for Amiga Coverdisks (it crashes)
    if (!mVersionCurrent->isCoverDisk())
        Sound_Play(pSprite, eSound_Effect_Missile_Launch, 0x0F);

    return -1;
}

int16 cFodder::Sprite_Create_Grenade2(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;

    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Projectile_Counters[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    Data0 = 2;
    sSprite* Data2C, *Data30;
    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return 0;

    ++mSprite_Projectile_Counters[pSprite->field_22];
    Data30 = Data2C + 1;
    Data2C->field_0 = pSprite->field_0;
    Data30->field_0 = pSprite->field_0;
    Data2C->field_4 = pSprite->field_4;
    Data30->field_4 = pSprite->field_4;
    Data2C->field_8 = 0x7D;
    Data30->field_8 = 0x7E;
    Data2C->field_A = 0;
    Data30->field_A = 0;

    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
    DataC += 6;

    Data2C->field_26 = Data8;
    Data2C->field_28 = DataC;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        Sprite_Enemy_Set_Target(Data2C);

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 < 0x28)
        Data0 = 0x28;

    Data0 = Data0 / 5;
    if (Data0 > 0x64)
        Data0 = 0x64;

    Data2C->field_12 = Data0;
    Data2C->field_36 = 0x32;
    Data30->field_36 = 0x32;
    Data0 = pSprite->field_20;
    Data0 += 2;

    Data2C->field_1E = 0;
    Data2C->field_20 = Data0;
    Data30->field_1E = 0;
    Data30->field_20 = 0;

    Data2C->field_4 += 1;
    Data2C->field_0 += 3;

    int32 Dataa0 = Data2C->field_12 << 16;
    Dataa0 >>= 1;
    if (Dataa0 > 0xE0000)
        Dataa0 = 0xE0000;

    Data2C->field_1A = Dataa0;
    Data2C->field_1A = 0;
    Data2C->field_18 = eSprite_Grenade;
    Data30->field_18 = eSprite_ShadowSmall;

    Data2C->field_52 = 0;
    Data30->field_52 = 0;
    Data2C->field_50 = 0;

    Data2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;

    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_38 = eSprite_Anim_None;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        Data2C->field_12 += 0x0A;

    return -1;
}

int16 cFodder::Sprite_Create_MissileHoming(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34) {

    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Missile_Projectile_Counters[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    int16 Data0 = 2;
    sSprite *Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return 0;

    ++mSprite_Missile_Projectile_Counters[pSprite->field_22];

    Data30 = pData2C;
    ++Data30;

    //seg005:55E0
    pData2C->field_0 = pSprite->field_0;
    pData2C->field_2 = pSprite->field_2;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_6 = pSprite->field_6;

    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;

    pData2C->field_0 += mSprite_Missile_LaunchDistance_X;
    Data30->field_0 += mSprite_Missile_LaunchDistance_X;
    if (Data30->field_0 < 0)
        return 0;

    pData2C->field_4 += mSprite_Missile_LaunchDistance_Y;
    Data30->field_4 += mSprite_Missile_LaunchDistance_Y;

    if (Data30->field_0 < 0)
        return 0;

    //seg005:5672
    pData2C->field_1A_sprite = pData34;
    pData2C->field_1E = pSprite->field_1E;
    pData2C->field_20 = pSprite->field_20;
    pData2C->field_26 = pSprite->field_2E;
    pData2C->field_28 = pSprite->field_30;
    pData2C->field_28 += 0x10;
    pData2C->field_8 = 0xA3;
    Data30->field_8 = 0x7E;
    pData2C->field_A = 0;
    Data30->field_A = 1;
    pData2C->field_36 = 0;
    Data30->field_36 = 0;
    pData2C->field_6A = 0x10000;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        pData2C->field_6A = 0x400;

    Data30->field_1E = 0;
    Data30->field_20 = 0;
    pData2C->field_4 += 1;
    pData2C->field_0 += 3;
    pData2C->field_18 = eSprite_MissileHoming2;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        pData2C->field_18 = eSprite_MissileHoming;
    Data30->field_18 = eSprite_ShadowSmall;
    pData2C->field_52 = 0;
    Data30->field_52 = 0;
    pData2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;
    pData2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    pData2C->field_38 = eSprite_Anim_None;

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

    if (pSprite->field_38)
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

    Data24 = pSprite->field_46_sprite;
    pSprite->field_0 = Data24->field_0;
    pSprite->field_2 = Data24->field_2;
    pSprite->field_4 = Data24->field_4;
    pSprite->field_6 = Data24->field_6;
    pSprite->field_4 += 1;
    pSprite->field_0 += 3;
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
    return;

loc_19701:;
    pSprite->field_8 = 0x7D;
    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= 1)
        pSprite->field_36 = 0;

    if (Data0 <= 4)
        pSprite->field_36 >>= 1;

    if (pSprite->field_36) {
        pSprite->field_36 -= 1;
        Sprite_XY_Store(pSprite);

        mTmp_FrameNumber = pSprite->field_A;
        Data0 = pSprite->field_26;
        Data4 = pSprite->field_28;

        Sprite_Direction_Between_Points(pSprite, Data0, Data4);
        Sprite_Movement_Calculate(pSprite);
        Sprite_Handle_Grenade_Terrain_Check(pSprite);

        (pSprite + 1)->field_10 = pSprite->field_10;
    }

    //loc_197C5
    if (!pSprite->field_50) {
        if (pSprite->field_52) {
            pSprite->field_36 = 0;
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

    if (pSprite->field_20 >= 2)
        goto loc_19877;

    pSprite->field_1E = 0;
    pSprite->field_20 = 0;

loc_19855:;

    Dataa4 = (Dataa4 >> 16) | ((Dataa4 & 0xFFFF) << 16);
    if (pSprite->field_52 >= 0x0A)
        goto loc_198D3;

    pSprite->field_52 -= Dataa4;
    goto loc_198D3;

loc_19877:;
    pSprite->field_1E_Big += Dataa4;

    if (pSprite->field_20 < 0) {
        pSprite->field_1E_Big = 0;

        if (pSprite->field_50)
            goto loc_19855;

        Dataa4 = (int64)pSprite->field_1A;
        Dataa4 = -Dataa4;
        Dataa4 >>= 1;
        pSprite->field_1A = Dataa4;
    }

loc_198D3:;
    if (!pSprite->field_36)
        pSprite->field_20 = 0;

    Data4 = pSprite->field_20;
    Data4 >>= 4;
    if (Data4 > 3)
        Data4 = 3;

    pSprite->field_A = Data4;
    (pSprite + 1)->field_A = Data4;
    pSprite->field_12 -= 1;
    return;

loc_1992D:;

    if (pSprite->field_52 < 8) {
        pSprite->field_18 = eSprite_Explosion;
        Sprite_Projectile_Counters_Decrease(pSprite);
        Sprite_Destroy(pSprite + 1);
        return;
    }
    //loc_19957
    Sprite_Projectile_Counters_Decrease(pSprite);
    pSprite->field_0 = 0;
    pSprite->field_4 = 0;

    Sprite_Destroy(pSprite);
    Sprite_Destroy(pSprite + 1);
}

void cFodder::Sprite_Projectile_Counters_Decrease(sSprite* pSprite) {

    --mSprite_Projectile_Counters[pSprite->field_22];
}

void cFodder::Sprite_Handle_Bullet(sSprite* pSprite) {
    int16 Data8, DataC, Data0, Data4;
    int8 al;

    pSprite->field_64 += 1;

    if (!pSprite->field_2A)
        goto loc_19BA8;

    Data0 = pSprite->field_1A_sprite->field_0;
    Data4 = pSprite->field_1A_sprite->field_4;

    Data0 += pSprite->field_16;
    Data4 += pSprite->field_14;
    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;

    pSprite->field_2A -= 1;
    if (pSprite->field_2A)
        return;

    pSprite->field_8 = 0x7F;
    pSprite->field_A = 0;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        pSprite->field_A = 3;

    Data0 = pSprite->field_1A_sprite->field_0;
    Data4 = pSprite->field_1A_sprite->field_4;
    Data0 += 7;
    Data4 += 5;
    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;

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

    Data0 = pSprite->field_2E;
    Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);

    Data0 = pSprite->field_50;
    if (Data0) {
        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
        pSprite->field_59 = -1;
    }
    //loc_19C4D

    if (pSprite->field_34 < 0) {
        pSprite->field_34 = pSprite->field_10;
    }

loc_19C6B:;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto SpriteDestroy;
    pSprite->field_36 = pSprite->field_4A;

    if (pSprite->field_59)
        goto loc_19D3C;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->field_36;
    Data4 >>= 3;

    if (Data0 > Data4)
        goto loc_19D3C;

    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_0 = pSprite->field_2E;
    pSprite->field_4 = pSprite->field_30;
    pSprite->field_44 = -1;
    goto loc_19D3C;

loc_19D24:;
    pSprite->field_2C = eSprite_Draw_First;

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
    pSprite->field_2C = eSprite_Draw_OnTop;
    pSprite->field_0 -= 4;
    if (pSprite->field_0 < 0)
        goto loc_19E50;

    pSprite->field_4 -= 3;
    if (pSprite->field_4 < 0)
        goto loc_19E50;

    pSprite->field_8 = 0x96;
    pSprite->field_A = 0;
    return;

loc_19DB0:;
    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if (pSprite->field_A >= 4)
        goto SpriteDestroy;

    return;

loc_19DCF:;
    al = pSprite->field_43;
    if (al)
        goto loc_19E1C;

    Sprite_Projectile_Collision_Check(pSprite);
    pSprite->field_43 = -1;
    pSprite->field_2C = eSprite_Draw_First;
    pSprite->field_0 -= 4;
    if (pSprite->field_0 < 0)
        goto loc_19E50;

    pSprite->field_4 -= 3;
    if (pSprite->field_4 < 0)
        goto loc_19E50;

    pSprite->field_A = 3;
    al = pSprite->field_43;

loc_19E1C:;
    if (al >= 0)
        goto loc_19DB0;

    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if (pSprite->field_A < 8)
        return;

SpriteDestroy:;

    mSprite_Projectile_Counters[2] -= 1;
    Sprite_Destroy(pSprite);
    return;

loc_19E50:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
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
        Data24 = pSprite->field_46_sprite;
        pSprite->field_0 = Data24->field_0;
        pSprite->field_2 = Data24->field_2;
        pSprite->field_4 = Data24->field_4;
        pSprite->field_6 = Data24->field_6;

        pSprite->field_4 += 1;
        pSprite->field_0 += 3;
        pSprite->field_8 = 0x7C;
        pSprite->field_A = 0;
        return;
    }

    pSprite->field_8 = 0xA3;
    int16 Data0 = pSprite->field_26;
    int16 Data4 = pSprite->field_28;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= 7)
        goto loc_1B843;

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;
    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_A = Data0;

    Data0 = -9;
    Data4 = 2;

    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        return;

loc_1B843:;
    // Hit Target
    pSprite->field_18 = eSprite_Explosion;
    Sprite_Projectile_HitTarget(pSprite);
    Data24 = pSprite + 1;

    Sprite_Destroy(Data24);
}

void cFodder::Sprite_Handle_Enemy_Rocket(sSprite* pSprite) {

    if (Sprite_Handle_Soldier_Animation(pSprite))
        return;

    if (pSprite->field_38 != eSprite_Anim_None)
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

    pSprite->field_54 = 3;
    pSprite->field_5A = -1;
    pSprite->field_55 = 0;
    pSprite->field_A = 0;

    Sprite_Handle_Troop_Direct_TowardWeaponTarget_WithRestore(pSprite);
    pSprite->field_A = 2;
}

void cFodder::Sprite_Handle_GrenadeBox(sSprite* pSprite) {

    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xC2;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Grenades[mSquad_Selected] += 4;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_RocketBox(sSprite* pSprite) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xC3;
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

    pSprite->field_8 = 0xA3;
    int16 Data0 = pSprite->field_26;
    int16 Data4 = pSprite->field_28;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;
    int16 Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 7)
        goto loc_1BECD;

    if (pSprite->field_20 > 4) {
        pSprite->field_1E_Big -= 0xA000;
    }

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    if (mSprite_Bullet_Destroy)
        goto loc_1BECD;

    Data0 = pSprite->field_10;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_A = Data0;

    if (pSprite->field_36 < 0x60)
        pSprite->field_36 += pSprite->field_3A;

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

    pSprite->field_18 = eSprite_Explosion2;
    pSprite->field_4 -= 4;
    Sprite_Projectile_HitTarget(pSprite);

    Sprite_Destroy(pSprite + 1);
}

void cFodder::Sprite_Handle_MissileHoming(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data34 = 0;

    if (pSprite->field_38)
        goto MissileExplode;

    dword_3B24B = -1;
    Sprite_Create_FireTrail(pSprite);

    pSprite->field_8 = 0xA3;
    Data34 = pSprite->field_1A_sprite;

    if (!Data34 || Data34->field_0 == -32768) {
        pSprite->field_36 += 2;
        pSprite->field_20 -= 2;
        if (pSprite->field_20 >= 0) {
            Sprite_Movement_Calculate(pSprite);
            return;
        }

        pSprite->field_20 = 0;
        goto MissileExplode;
    }

    Data0 = Data34->field_0 + 8;
    Data4 = Data34->field_4 + 8;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->field_36 >> 4;
    Data4 += 1;

    if (Data0 <= Data4)
        goto MissileExplode;

    if (Data34->field_20 <= 0x10) {

        // Distance to target > 0x30?
        if (Data0 > 0x30) {
            Data0 = pSprite->field_0;
            Data4 = pSprite->field_4;
            Data8 = Data34->field_0;
            DataC = Data34->field_4;
            Data8 += 8;
            DataC += 8;

            if (!Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC)) {
                if (pSprite->field_20 > 8)
                    pSprite->field_1E_Big -= 0x12000;
            }
            else {

                if (pSprite->field_20 < 0x18)
                    pSprite->field_1E_Big += 0x28000;
            }

        }
        else {
            // Near Target
            Data4 = Data34->field_20;
            Data4 -= pSprite->field_20;
            Data4 >>= 3;
            pSprite->field_20 += Data4;
        }
    }
    else {

        Data4 = pSprite->field_20;
        if (Data4 != Data34->field_20) {

            if (Data4 > Data34->field_20)
                pSprite->field_1E_Big -= 0x8000;
            else
                pSprite->field_1E_Big += 0x8000;
        }
    }

    Data0 = Data34->field_0;
    Data0 += 8;
    Data4 = Data34->field_4;
    Data4 += 8;
    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    Data4 = pSprite->field_A;
    Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_A = Data0;

    if (pSprite->field_22 == eSprite_PersonType_Human || Data4 == Data0) {
        if (pSprite->field_36 <= 0x3C) {

            Data0 = (int16)(pSprite->field_6A >> 16);
            pSprite->field_36 += Data0;
            pSprite->field_6A = pSprite->field_6A + 0x200;
        }
        else {
            pSprite->field_36 = 0x3C;
        }

        return;
    }

    if (pSprite->field_36 > 0x1E) {

        pSprite->field_6A = 0;
        pSprite->field_36 -= 2;
        if (pSprite->field_36 < 0)
            pSprite->field_36 = 0;
    }

    return;

MissileExplode:;
    pSprite->field_18 = eSprite_Explosion2;
    pSprite->field_4 -= 4;
    Sprite_Projectile_HitTarget(pSprite);
    Sprite_Destroy(pSprite + 1);

}

void cFodder::Sprite_Handle_Sparks(sSprite* pSprite) {
    Sprite_Movement_Calculate(pSprite);
    int32 Data0 = (int32)pSprite->field_1A;

    Data0 += pSprite->field_1E;

    pSprite->field_1E_Big = Data0;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
        Data0 = -Data0;
        Data0 >>= 1;
    }

    Data0 -= 0x22000;
    pSprite->field_1A = Data0;

    Data0 = Data0 >> 16;
    if (!pSprite->field_A)
        goto loc_1C248;

    if (pSprite->field_20)
        goto loc_1C262;

    if (Data0 > 1)
        goto loc_1C262;

loc_1C248:;
    pSprite->field_A++;
    if (pSprite->field_A >= 3) {
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

    pSprite->field_A += 1;
    if (pSprite->field_A != 4)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Mine(sSprite* pSprite) {
    pSprite->field_2C = eSprite_Draw_First;

    if (!pSprite->field_38) {
        int16 Data0;
        pSprite->field_8 = 0xC7;
        Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0);
    }
    else {
        pSprite->field_18 = eSprite_Explosion2;
    }
}

void cFodder::Sprite_Handle_Mine2(sSprite* pSprite) {
    int16 Data0;

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_38)
        goto loc_1C406;

    if (pSprite->field_2A) {
        pSprite->field_2A += 1;
        if (pSprite->field_2A >= 0x0A)
            goto loc_1C406;
    }

    Data0 = 0;
    pSprite->field_8 = 0xC8;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (mSquad_Leader->field_38)
        return;

    if (!mSquad_Leader->field_36)
        return;

    if (mSquad_Leader->field_52)
        return;

    if (mSquad_Leader->field_20)
        return;

    mSquad_Leader->field_38 = eSprite_Anim_Slide1;
    mSquad_Leader->field_2A = 1;
    return;

loc_1C406:;
    pSprite->field_18 = eSprite_Explosion;
}

void cFodder::Sprite_Handle_Spike(sSprite* pSprite) {
    sSprite* Data28 = mSquad_Leader;
    int16 Data0 = 0;

    pSprite->field_8 = 0xC9;

    if (pSprite->field_A)
        goto loc_1C4AD;

    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (Data0 >= 6)
        return;

    if (Data0 == 5)
        goto loc_1C4AD;

    Data28->field_0 = pSprite->field_0;
    Data28->field_0 -= 3;
    Data28->field_4 = pSprite->field_4;
    Data28->field_4 += 1;
    Data28->field_38 = eSprite_Anim_Die4;
    Data28->field_64 = -1;
    Data28->field_20 = 7;

loc_1C4AD:;
    if (pSprite->field_A >= 2)
        return;

    pSprite->field_A += 1;
}

void cFodder::Sprite_Handle_Seal_Mine(sSprite* pSprite) {
    Sprite_Handle_Seal(pSprite);

    if (pSprite->field_8 == 0xE1)
        return;

    if (pSprite->field_38)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    sSprite* Data24 = mSquad_Leader;
    int16 Data0 = Data24->field_0;
    int16 Data4 = Data24->field_4;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x14)
        return;

    pSprite->field_38 = eSprite_Anim_Die4;
}

void cFodder::Sprite_Handle_Spider_Mine(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data14 = 0;

    if (pSprite->field_38)
        goto loc_1DC50;

    pSprite->field_8 = 0xE2;
    if (!pSprite->field_2E) {
        pSprite->field_2E = -1;
        Data0 = tool_RandomGet() & 0x1E;
        pSprite->field_2A = Data0;
    }

    Data0 = pSprite->field_2A;
    Data0 += 2;
    Data0 &= 0x1E;
    pSprite->field_2A = Data0;

    pSprite->field_A = mSprite_SpiderMine_Frames[Data0 / 2];
    Data14 = mSquad_Leader;

    if (Data14 == INVALID_SPRITE_PTR)
        return;

    Data0 = Data14->field_0;
    Data4 = Data14->field_4;
    Data8 = pSprite->field_0;
    Data8 += 4;
    DataC = pSprite->field_4;
    DataC -= 2;
    Data10 = 0x20;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x0A)
        return;

loc_1DC50:;
    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = -32763;
    pSprite->field_28 = -3;
}

void cFodder::Sprite_Handle_Bonus_Rockets(sSprite* pSprite) {
    int16 Data0;

    // Blow up?
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0xE4;
    pSprite->field_A = 0;
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

    if (!pSprite->field_2E)
        return -1;

    if (pSprite == mSquad_Leader)
        if (pSprite->field_54 == 1)
            return -1;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    pSprite->field_54 = 2;
    pSprite->field_5A = -1;
    pSprite->field_57 = 8;
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;
    Data2C->field_8 = 0x9D;
    //seg005:1023 
    Data0 = pSprite->field_3C;
    Data0 >>= 1;
    Data0 &= 0x07;
    Data2C->field_A = Data0;
    Data2C->field_12 = 9;

    if (pSprite->field_18 != eSprite_Player) {

        Data0 = pSprite->field_62;
        Data0 >>= 3;
        Data0 += 8;
        if (Data0 > 0x10)
            Data0 = 0x10;
        Data2C->field_12 = Data0;
    }
    else {

        Data2C->field_5E_SoldierAllocated = pSprite->field_46_mission_troop;
        Data2C->field_5D = -1;

        // Bullet Travel time
        Data0 = mSprite_Weapon_Data.mAliveTime;
        Data0 += mSprite_Bullet_Time_Modifier;
        Data2C->field_12 = Data0;
    }

    Data2C->field_18 = eSprite_Bullet;
    Data2C->field_1E_Big = pSprite->field_1E_Big;
    Data2C->field_20 += 6;
    Data2C->field_52 = pSprite->field_52;
    Data2C->field_22 = pSprite->field_22;
    Data2C->field_32 = pSprite->field_32;
    Data2C->field_2C = eSprite_Draw_Second;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        goto loc_2087D;

    Data0 = mSprite_Weapon_Data.mSpeed;
    Data0 += mSprite_Bullet_Fire_Speed_Modifier;
    Data2C->field_4A = Data0;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 <<= 3;
    Data2C->field_36 = Data0;
    goto loc_208A6;

loc_2087D:;
    // AI Fire Speed
    Data0 = 0x3C;
    Data0 += pSprite->field_62;
    Data2C->field_4A = Data0;
    Data2C->field_36 = 0x18;

loc_208A6:;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data2C->field_2E = pSprite->field_2E;
    Data2C->field_30 = pSprite->field_30;
    if (pSprite->field_18 != eSprite_Player)
        Data2C->field_2E += 7;
    else
        Data2C->field_30 += 0x0F;

    Data2C->field_52 = 0;
    Data2C->field_3A = 0;

    Data2C->field_1A_sprite = pSprite;
    Data2C->field_2A = 2;
    Data2C->field_16 = 0;
    Data2C->field_14 = 2;

    if (Data2C->field_A == 4)
        Data2C->field_14 = -1;

    Data2C->field_34 = -1;
    Data2C->field_50 = 0;
    Data2C->field_59 = 0;
    Data8 = 7;

    // Is Human?
    if (pSprite->field_22 == eSprite_PersonType_Human) {

        if (pSprite == mSquad_Leader) {

            mSprite_Bullet_Deviate_Counter += 1;
            mSprite_Bullet_Deviate_Counter &= 3;

            if (!mSprite_Bullet_Deviate_Counter)
                goto loc_209F3;

            Data8 = mSprite_Weapon_Data.mDeviatePotential;
        }
        else {

            Data8 = Mission_Troop_GetDeviatePotential(pSprite->field_46_mission_troop);
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

    if (mSprite_Projectile_Counters[pSprite->field_22] == 2)
        goto loc_20ADE;

    if (!pSprite->field_2E)
        goto loc_20ADE;

    if (pSprite->field_18 != eSprite_Enemy)
        goto loc_20AC1;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
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
    if (pSprite->field_18 != eSprite_Player)
        goto loc_20B6E;

    Data0 = pSprite->field_32;
    Data4 = Data0;
    if (!mSquad_Grenades[Data0])
        goto loc_20ADE;

    --mSquad_Grenades[Data0];
    mGUI_RefreshSquadGrenades[Data4] = -1;

loc_20B6E:;

    ++mSprite_Projectile_Counters[pSprite->field_22];
    pSprite->field_55 = 1;
    pSprite->field_5A = -1;
    pSprite->field_54 = 1;

    Data30 = Data2C;
    ++Data30;
    //seg005:14CE
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    //seg005:14F4
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;
    Data2C->field_8 = 0x7D;
    Data30->field_8 = 0x7E;
    Data2C->field_A = 0;
    Data30->field_A = 0;
    Data2C->field_12 = 0x10;
    Data0 = tool_RandomGet();
    Data0 &= 0x0F;
    Data2C->field_12 = Data0;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
    DataC += 0x06;
    //seg005:158C
    Data2C->field_26 = Data8;
    Data2C->field_28 = DataC;
    Data0 = 0;
    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 < 0x14)
        Data0 = 0x14;

    Data0 /= 5;

    if (Data0 > 0x64)
        Data0 = 0x64;

    //loc_20CEA
    Data2C->field_12 = Data0;
    Data2C->field_36 = 0x32;
    Data30->field_36 = 0x32;
    Data2C->field_1E = 0x0000;
    Data2C->field_20 = 0x7;
    Data30->field_1E = 0;
    Data30->field_20 = 0;
    Data2C->field_4 += 1;
    Data2C->field_0 += 3;
    int32 Dataa0 = Data2C->field_12 << 16;

    Dataa0 >>= 1;
    if (Dataa0 > 0x0E0000)
        Dataa0 = 0x0E0000;

    Data2C->field_1A = Dataa0;
    Data2C->field_18 = eSprite_Grenade;
    Data30->field_18 = eSprite_ShadowSmall;
    Data2C->field_52 = 0;
    Data30->field_52 = 0;
    Data2C->field_50 = 0;
    Data2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;
    Data2C->field_32 = pSprite->field_32;
    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_38 = eSprite_Anim_None;
    if (pSprite->field_18 == eSprite_Enemy)
        Data2C->field_12 += 0x1C;
    Data2C->field_56 = 4;
    Data2C->field_46_sprite = pSprite;

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

        if (pSprite->field_20 <= 1) {
            pSprite->field_12 = 1;
            return;
        }
    }
    //loc_21561
    if (Data4 == eTerrainFeature_QuickSand || Data4 == eTerrainFeature_WaterEdge
        || Data4 == eTerrainFeature_Water) {

        if (pSprite->field_20)
            return;

        pSprite->field_52 = 6;
        (pSprite + 1)->field_52 = 6;
        return;
    }
    //loc_21597
    return;

loc_21599:;
    if (pSprite->field_20 >= 8)
        return;

    pSprite->field_0 = mStoredSpriteX >> 16;
    Data0 = -3;
    Data4 = 2;

    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4)) {
        Data0 = 0x100;
        Data0 -= pSprite->field_10;
        Data0 &= 0x1FE;
        pSprite->field_10 = Data0;
    }
    else {
        Data0 = 0x200;
        Data0 -= pSprite->field_10;
        Data0 &= 0x1FE;
        pSprite->field_10 = Data0;
    }
    Sprite_XY_Restore(pSprite);
}

int16 cFodder::Sprite_Projectile_Collision_Check(sSprite* pSprite) {

    // If the projectile doesn't belong to a human, check if it hit a squad member
    if (pSprite->field_22 != eSprite_PersonType_Human) {

        int16 Data8 = pSprite->field_0;
        int16 DataC = pSprite->field_0 + 6;
        int16 Data10 = pSprite->field_4 - 9;
        int16 Data14 = pSprite->field_4 - 4;
        word_3AA45 = 1;

        return Squad_Members_Find_In_Region(pSprite, Data8, DataC, Data10, Data14);
    }

    //loc_21673
    int16 Data8 = pSprite->field_0 - 6;
    int16 DataC = pSprite->field_0 + 10;
    int16 Data10 = pSprite->field_4 - 10;
    int16 Data14 = pSprite->field_4 + 6;

    if (mVersionCurrent->isAmigaTheOne()) {
        Data8 = pSprite->field_0;
        DataC = pSprite->field_0 + 4;
        Data10 = pSprite->field_4 - 4;
        Data14 = pSprite->field_4;
    }

    word_3AA45 = 1;
    sSprite* Data24 = 0;
    if (Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14) >= 0)
        return 0;

    if (Data24->field_18 == eSprite_Enemy) {

		Data24->field_5E_Squad = pSprite->field_5E_Squad;
        Data24->field_5E = pSprite->field_5E;
        Data24->field_5D = pSprite->field_5D;
    }

    return -1;
}

void cFodder::Sprite_Create_Sparks(sSprite* pSprite, int16 pData18) {
    int16 Data0 = 2;
    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data30 = Data2C + 1;
    Data2C->field_0 = pSprite->field_0 + 2;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4 - 4;
    Data2C->field_6 = pSprite->field_6;

    Data30->field_0 = Data2C->field_0;
    Data30->field_4 = Data2C->field_4;

    Data2C->field_1E_Big = pSprite->field_1E_Big;

    Data30->field_20 = 0;
    Data2C->field_8 = 0xC5;
    Data30->field_8 = 0x7E;
    Data2C->field_A = 0;
    Data30->field_A = 0;
    Data2C->field_36 = 0x0C;
    Data30->field_36 = 0x0C;
    Data2C->field_18 = eSprite_Sparks;
    Data30->field_18 = eSprite_ShadowSmall;
    Data2C->field_52 = 0;
    Data30->field_52 = 0;
    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_38 = eSprite_Anim_None;

    Data2C->field_10 = pData18;
    Data2C->field_1A = 0x0A8000;

}

void cFodder::Sprite_Create_FireTrail(sSprite* pSprite) {

    if (!pSprite->field_5C)
        return;

    //  cmp     ds:word_3B1A9, 14h
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;

    if (dword_3B24B == 0) {
        Data2C->field_4 -= 3;
    }
    else {
        Data2C->field_4 += 2;
        dword_3B24B = 0;
    }

    Data2C->field_20 = pSprite->field_20;
    Data2C->field_52 = 0;
    Data2C->field_8 = 0xC6;
    Data2C->field_A = 0;
    Data2C->field_18 = eSprite_FireTrail;
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

    if (mSprite_Missile_Projectile_Counters[pSprite->field_22] == 2)
        goto loc_22592;

    if (!pSprite->field_2E)
        goto loc_22592;

    //seg005:2E39
    if (pSprite->field_18 == eSprite_Enemy) {
        Data0 = pSprite->field_0;
        Data4 = pSprite->field_4;
        Data8 = pSprite->field_2E;
        DataC = pSprite->field_30;

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

    if (pSprite->field_18 == eSprite_Player) {
        Data0 = pSprite->field_32;
        Data4 = Data0;

        if (!mSquad_Rockets[Data0])
            goto loc_22592;

        --mSquad_Rockets[Data0];
        mGUI_RefreshSquadRockets[Data4] = -1;
    }

    ++mSprite_Missile_Projectile_Counters[pSprite->field_22];
    pSprite->field_55 = 1;
    pSprite->field_5A = -1;
    pSprite->field_54 = 3;
    Data30 = Data2C;
    Data30++;

    //seg005:2F86 
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;

    //seg005:2FD2
    Data2C->field_26 = pSprite->field_2E;
    Data2C->field_28 = pSprite->field_30;
    Data2C->field_28 += 0x10;
    Data2C->field_8 = 0x7C;
    Data30->field_8 = 0x7E;
    Data2C->field_10 = 0;
    Data30->field_10 = 1;
    Data2C->field_36 = 0x64;
    Data30->field_36 = 0x64;
    Data2C->field_1E = 0x0000;
    Data2C->field_20 = 0x0A;

    Data30->field_1E = 0;
    Data30->field_20 = 0;

    Data2C->field_4++;
    Data2C->field_0 += 3;

    // Amiga Power always has homing missiles
    if (pSprite->field_22 == eSprite_PersonType_Human &&
        (mVersionCurrent->isCoverDisk() || (pSprite->field_75 & eSprite_Flag_HomingMissiles))) {

        // Within lock on range?
        if (Sprite_Homing_LockInRange(pSprite, Data34)) {

            Data2C->field_1A_sprite = Data34;
            Data2C->field_18 = eSprite_MissileHoming2;
            Data2C->field_6A = 0x10000;
            Data2C->field_26 = pSprite->field_2E;
            Data2C->field_28 = pSprite->field_30;
            Data2C->field_28 += 0x10;
            Data2C->field_38 = eSprite_Anim_None;
            Data2C->field_36 = 0;
            Data30->field_36 = 0;

        }
        else {
            Data2C->field_32 = 0;
            Data2C->field_18 = eSprite_Missile;
        }

    }
    else {

        Data2C->field_18 = eSprite_Rocket;
        Data2C->field_46_sprite = pSprite;
    }

    Data30->field_18 = eSprite_ShadowSmall;
    Data2C->field_52 = 0;
    Data30->field_32 = 0;
    Data2C->field_22 = pSprite->field_22;
    Data30->field_22 = Data2C->field_22;
    Data2C->field_32 = pSprite->field_32;
    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_56 = 6;

    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = false;

    return 0;
}

void cFodder::Sprite_Projectile_HitTarget(sSprite* pSprite) {

    mSprite_Missile_Projectile_Counters[pSprite->field_22] -= 1;
}
