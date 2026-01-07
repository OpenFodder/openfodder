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

void cFodder::Sprite_Helicopter_Update_Direction(sSprite* pSprite, int16& pData0, int16& pData4) {
    if (pSprite->field_44) {
        pSprite->field_44 -= 1;

        if (pSprite->field_44)
            goto loc_23E2F;
    }

    pSprite->field_3C = pSprite->field_10;
    Sprite_Direction_Between_Points(pSprite, pData0, pData4);

loc_23E2F:;
    Sprite_SetDirectionMod(pSprite);

    pSprite->field_10 = pSprite->field_3C;
    pData0 = mDirectionMod;
    pData0 <<= 3;

    if (pSprite->field_36 > 0x18)
        pData0 <<= 1;
    pSprite->field_10 = pData0;
    pSprite->field_10 &= 0x1FE;

    Sprite_Set_Direction_To_Next(pSprite);

    int16 Data8 = pSprite->field_3C;
    if (!pSprite->field_36)
        return;

    pSprite->field_A = Data8 >> 1;
}

void cFodder::Sprite_Count_HelicopterCallPads() {

    int16 Data0 = 0;
    for(auto& Sprite : mSprites) {

        if (Sprite.field_18 == eSprite_Helicopter_CallPad)
            ++Data0;
    }

    mHelicopterCallPadCount = Data0;
    mHelicopterCall_X = -1;
}

void cFodder::Sprite_HelicopterCallPad_Check() {
    if (!mHelicopterCallPadCount)
        return;

    if (mHelicopterCallPadCount != mHelicopterCallPadPressedCount)
        return;

    if (mHelicopterCall_X >= 0)
        return;

    if (!mTroop_InRange_Callpad || mTroop_InRange_Callpad == INVALID_SPRITE_PTR)
        return;

    mHelicopterCall_X = mTroop_InRange_Callpad->field_0;
    mHelicopterCall_Y = mTroop_InRange_Callpad->field_4;

    mHelicopterCall_Y += 0x28;
}

void cFodder::Sprite_Handle_Helicopter_Human(sSprite* pSprite) {

    if (pSprite->field_38) {

        pSprite->field_22 = eSprite_PersonType_Human;
        Sprite_Handle_Helicopter(pSprite);
        pSprite->field_22 = eSprite_PersonType_Human;

        return;
    }

    Sprite_Handle_Helicopter_Human_Deploy_Weapon(pSprite);

    mSprite_Helicopter_DestroyLight = 0;
    pSprite->field_22 = eSprite_PersonType_Human;
    Sprite_Handle_Helicopter(pSprite);
    pSprite->field_22 = eSprite_PersonType_Human;
    mSprite_Helicopter_DestroyLight = 0;

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;
    Data24 = pSprite + 2;
    Data24->field_18 = eSprite_Null;
}

int16 cFodder::Sprite_Handle_Helicopter_Terrain_Check(sSprite* pSprite) {
    int16 Data4 = 0x0F;
    int16 Data0 = -10;

    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    switch (Data4) {
    case eTerrainFeature_Rocky:
    case eTerrainFeature_QuickSand:
    case eTerrainFeature_WaterEdge:
    case eTerrainFeature_Water:
    case eTerrainFeature_Sink:
        Data0 = 0x0C;
        break;

    case eTerrainFeature_Block:
        Data0 = 0x14;
        break;
    case eTerrainFeature_Rocky2:
    case eTerrainFeature_Drop:
    case eTerrainFeature_Drop2:
        Data0 = 0x0E;
        break;

    default:
        return 0;   // Correct?
    }

    Data4 = pSprite->field_20;
    if (Data4 >= Data0)
        return 0;

    ++pSprite->field_20;
    return -1;
}

void cFodder::Sprite_Handle_Helicopter_Human_Deploy_Weapon(sSprite* pSprite) {
    sSprite* Data2C = 0, *Data34 = 0;

    if (!pSprite->field_57)
        if (!pSprite->field_54)
            return;

    pSprite->field_54 = 0;

    if (pSprite->field_20 <= 0x1F)
        return;

    if (pSprite->field_6F == eVehicle_Helicopter_Grenade)
        goto loc_24905;

    if (pSprite->field_6F == eVehicle_Helicopter_Missile)
        goto loc_24917;

    if (pSprite->field_6F == eVehicle_Helicopter_Homing)
        goto loc_24917;

    return;

loc_24905:;
    if (!Sprite_Create_Grenade2(pSprite))
        goto loc_2490D;
    goto loc_24942;

loc_2490D:;
    pSprite->field_57 = -1;
    return;

loc_24917:;
    mSprite_Missile_LaunchDistance_X = 0x0E;
    mSprite_Missile_LaunchDistance_Y = -12;

    // Homing Missle and can get lock on a target?
    if (pSprite->field_6F == eVehicle_Helicopter_Homing &&
        Sprite_Homing_LockInRange(pSprite, Data34)) {

        // Launch a homing missile
        if (!Sprite_Create_MissileHoming(pSprite, Data2C, Data34))
            goto loc_2490D;

    }
    else {
        // Launch a missile
        if (!Sprite_Create_Missile(pSprite, Data2C))
            goto loc_2490D;
    }

loc_24942:;
    pSprite->field_57 = 0;
}

int16 cFodder::Sprite_Handle_Helicopter_Enemy_Weapon(sSprite* pSprite, sSprite*& pData30) {
    int16 Data0;
    sSprite* Data2C = 0, *Data34 = 0;

    if (pSprite->field_6F == eVehicle_Helicopter_Grenade)
        goto loc_25344;

    if (pSprite->field_6F == eVehicle_Helicopter_Missile)
        goto loc_253E7;

    if (pSprite->field_6F == eVehicle_Helicopter_Homing)
        goto loc_253E7;

loc_25344:;
    pSprite->field_26 = pData30->field_0;
    pSprite->field_26 += 0x14;
    pSprite->field_28 = pData30->field_4;
    pSprite->field_26 -= 0x1E;
    if (pSprite->field_26 < 0)
        pSprite->field_26 = 0;

    pSprite->field_2E = pData30->field_0;
    pSprite->field_30 = pData30->field_4;
    if (pSprite->field_20 <= 0x1F)
        goto loc_253D2;

    if (mMission_EngineTicks & 0x0F)
        goto loc_253D2;

    Sprite_Create_Grenade2(pSprite);
    Data0 = tool_RandomGet() & 0x0F;
    if (!Data0)
        goto loc_253DE;

loc_253D2:;
    return -1;

loc_253DE:;
    return 0;

loc_253E7:;
    pSprite->field_26 = pData30->field_0;
    pSprite->field_28 = pData30->field_4;
    pSprite->field_2E = pData30->field_0;
    pSprite->field_30 = pData30->field_4;

    Data0 = mSprite_DistanceTo_Squad0;
    if (Data0 <= 0x3C)
        goto loc_254C7;
    if (Data0 >= 0x8C)
        goto loc_254E7;

    if (pSprite->field_20 >= 0x19) {
        if (!(mMission_EngineTicks & 0x0F)) {
            mSprite_Missile_LaunchDistance_X = 0x0E;
            mSprite_Missile_LaunchDistance_Y = -12;
            if (pSprite->field_6F == eVehicle_Helicopter_Homing) {

                Data34 = pData30;
                if (Sprite_Create_MissileHoming(pSprite, Data2C, Data34)) {
                    Data2C->field_6A = 0x2000;
                    Data2C->field_36 = 0x14;
                }
            }
            else {
                Sprite_Create_Missile(pSprite, Data2C);
            }
        }
    }

    if (pSprite->field_36 < 0)
        goto loc_254E7;

    pSprite->field_36 -= 4;
    if (pSprite->field_36 < 0)
        pSprite->field_36 = 0;
    goto loc_254E7;

loc_254C7:;

    if (pSprite->field_36 >= 0)
        goto loc_254DE;

    if (pSprite->field_36 < -34)
        goto loc_254E7;
loc_254DE:;
    pSprite->field_36 -= 3;

loc_254E7:;
    Data0 = tool_RandomGet() & 0x0F;
    if (!Data0)
        return 0;

    return -1;
}

void cFodder::Sprite_Handle_Helicopter_Enemy(sSprite* pSprite) {
    int16 Data0;
    int16 Data4;
    int16 Data8;
    int16 DataC;
    int16 Data10, Data14, Data1C;
    sSprite* Data24 = 0, *Data30 = 0;

    // Bullets don't hit choppers
    if (pSprite->field_38 == eSprite_Anim_Hit)
        pSprite->field_38 = eSprite_Anim_None;

    if (pSprite->field_38) {
        mSprite_Helicopter_DestroyLight = -1;

        pSprite->field_22 = eSprite_PersonType_AI;
        Sprite_Handle_Helicopter(pSprite);
        pSprite->field_22 = eSprite_PersonType_AI;

        mSprite_Helicopter_DestroyLight = 0;
        goto loc_25288;
    }

    if (!pSprite->field_43) {
        pSprite->field_46 = (pSprite->field_0 << 16) | (pSprite->field_4 & 0xFFFF);
        pSprite->field_43 = -1;
    }

    if (pSprite->field_4C)
        --pSprite->field_4C;

    if (pSprite->field_20)
        goto loc_250D2;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = ((int64)pSprite->field_46) >> 16;
    DataC = ((int64)pSprite->field_46) & 0xFFFF;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x14)
        goto loc_250D2;

    Data8 = pSprite->field_62;
    if ((uint16)Data8 >= 0x1F4) {
        if (pSprite->field_18 == eSprite_Helicopter_Homing_Enemy2)
            if (mHelicopterCall_X < 0)
                goto loc_24FF1;

        if (!(tool_RandomGet() & 1))
            goto loc_2500F;
    }

loc_24FF1:;
    if (mVersionCurrent->isDemo()) {
        Data8 += 0x20;
        if (Data8)
            pSprite->field_62 = Data8;

    }
    else {
        Data8 += 1;
        Data8 += mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;

        if (Data8 >= 0)
            pSprite->field_62 = Data8;
    }

    goto loc_25239;

loc_2500F:;
    if (pSprite->field_4C)
        goto loc_25239;

	Data0 = map_GetRandomX();
	Data0 += 4;
	if (Data0 > mMapLoaded->getWidth())
		goto loc_25239;

	Data8 = Data0;
	Data0 = map_GetRandomY();
	Data0 += 4;

    if (Data0 > mMapLoaded->getHeight())
        goto loc_25239;

    DataC = Data0;

    Data8 <<= 4;
    Data0 = tool_RandomGet() & 0x0F;
    Data8 += Data0;

	DataC <<= 4;
    Data0 = tool_RandomGet() & 0x0F;
    DataC += Data0;

    if (Map_Terrain_GetMoveable_WithCache(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
        goto loc_25239;

    pSprite->field_26 = Data10;
    pSprite->field_28 = Data14;
    pSprite->field_4C = 0x5A;
    goto loc_25239;

loc_250D2:;

    if (pSprite->field_6F == eVehicle_Helicopter)
        goto loc_251D2;

    Data1C = pSprite->field_5E_Squad;

    if (mSquads[Data1C / 9] == (sSprite**)INVALID_SPRITE_PTR)
        goto loc_251B4;

    Data30 = mSquads[Data1C / 9][Data1C % 9];
    if (Data30 == INVALID_SPRITE_PTR)
        goto loc_251B4;

    if (Data30->field_52)
        goto loc_251B4;

    Data8 = Data30->field_0;
    Data8 += 8;
    DataC = Data30->field_4;
    DataC += -5;

    if (Map_Terrain_GetMoveable_WithCache(mTiles_NotDriveable, Data8, DataC, Data10, Data14))
        goto loc_251B4;

    Data0 = Data30->field_0;
    Data4 = Data30->field_4;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    mSprite_DistanceTo_Squad0 = Data0;
    if (Data0 < 0xFA)
        if (Sprite_Handle_Helicopter_Enemy_Weapon(pSprite, Data30))
            goto loc_251D2;

loc_251B4:;
    pSprite->field_5E_Squad += 1;
    if (pSprite->field_5E_Squad >= 0x1E)
        pSprite->field_5E_Squad = 0;

loc_251D2:;
    if (pSprite->field_62) {
        --pSprite->field_62;
    }
    else {
        pSprite->field_26 = ((int64)pSprite->field_46) >> 16;
        pSprite->field_28 = ((int64)pSprite->field_46) & 0xFFFF;
        goto loc_25239;
    }

    Data0 = tool_RandomGet() & 0x1F;
    if (!Data0)
        goto loc_2500F;

    if (!pSprite->field_20) {
        Data0 = tool_RandomGet() & 0x0F;
        if (!Data0) {
            sSprite* Data2C;
            Sprite_Create_Enemy(pSprite, Data2C);
        }
    }

loc_25239:;
    mSprite_Helicopter_DestroyLight = -1;
    pSprite->field_22 = eSprite_PersonType_AI;
    Sprite_Handle_Helicopter(pSprite);

    pSprite->field_22 = eSprite_PersonType_AI;
    mSprite_Helicopter_DestroyLight = 0;
    Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;

    ++Data24;
    Data24->field_18 = eSprite_Null;

loc_25288:;

    Data24 = pSprite + 3;
    Data24->field_18 = eSprite_Flashing_Light;
    Data0 = pSprite->field_A << 1;

    Data4 = pSprite->field_0;
    Data4 += mSprite_Helicopter_Light_Positions[Data0];
    Data24->field_0 = Data4;

    Data4 = pSprite->field_4;
    Data4 += mSprite_Helicopter_Light_Positions[Data0 + 1];
    Data24->field_4 = Data4;

    Data24->field_20 = pSprite->field_20;
}

int16 cFodder::Sprite_Handle_Helicopter_Callpad_InRange(sSprite* pSprite, sSprite*& pData2C) {
    sSprite** Data28 = mSprite_TroopsAlive;
    int16 Data0 = 0;

    do {
        if (*Data28 == INVALID_SPRITE_PTR || *Data28 == 0)
            return -1;

        pData2C = *Data28++;

        if (pData2C->field_6E)
            continue;
        if (pData2C->field_38)
            continue;

        Data0 = pSprite->field_0 - 2;
        int16 Data4 = pSprite->field_4 - 8;
        int16 Data8 = pData2C->field_0;
        int16 DataC = pData2C->field_4;
        int16 Data10 = 0x20;

        Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);

        if (Data0 < 9)
            break;

    } while (1);

    return 0;
}

void cFodder::Sprite_Handle_Helicopter_Human_CallCheck(sSprite* pSprite) {
    if (pSprite->field_75)
        return;

    pSprite->field_26 = pSprite->field_0;
    pSprite->field_28 = pSprite->field_4;
    if (mHelicopterCall_X < 0)
        return;

    pSprite->field_26 = mHelicopterCall_X;
    pSprite->field_28 = mHelicopterCall_Y;
    int16 Data0 = pSprite->field_0;
    int16 Data4 = pSprite->field_4;
    int16 Data8 = pSprite->field_26;
    int16 DataC = pSprite->field_28;
    int16 Data10 = 0x3F;
    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 >= 0x2C)
        return;

    pSprite->field_75 = -1;
    pSprite->field_6E = -1;
}

void cFodder::Sprite_Handle_Helicopter(sSprite* pSprite) {
    word_3B4ED[0] = 0;
    int16 Data0 = tool_RandomGet() & 0x0E;
    int16 Data4, Data8, DataC, Data10, Data14, Data18, Data1C;
    sSprite* Data24 = 0, *Data2C = 0;

    if (mVersionCurrent->isCoverDisk()) {
        static int FF = 0;

        if (++FF == 100) {
            FF = 0;
            //TODO: Fix chopper
            Data0 = 0x01;
        }
        else
            Data0 = 0;
    }
    else
        Data0 = mSprite_Helicopter_Sounds[Data0 / 2];

    if (pSprite->field_22 != eSprite_PersonType_Human) {
        if (pSprite->field_20 <= 1)
            goto loc_19EE5;
    }
    else {

        // Just off the ground?
        if (pSprite->field_20 < 2) {
            if (pSprite != mSquad_CurrentVehicle)
                goto loc_19EE5;
        }
    }

    word_3B4ED[0] = -1;
    Data4 = pSprite->field_20;
    if (Data4 < 0x10)
        Data4 = 0x10;

    Sprite_Map_Sound_Play(Data0);

loc_19EE5:;
    pSprite->field_65 = -1;

    if (pSprite->field_38 == eSprite_Anim_None)
        goto loc_1A149;

    if (pSprite->field_18 == eSprite_Helicopter_Homing_Enemy2) {

        if (mHelicopterCall_X >= 0) {

            if (pSprite->field_75 != 0x71) {
                pSprite->field_75 = 0x71;
                pSprite->field_74 = static_cast<int8>(pSprite->field_A);
            }

            pSprite->field_A += 1;
            pSprite->field_A &= 0x0F;

            if (pSprite->field_A == pSprite->field_74) {
                pSprite->field_75 = 0;
                pSprite->field_38 = eSprite_Anim_None;
            }

            Data24 = pSprite + 1;

            Data24->field_8 = 0x8C;
            Data24->field_0 = pSprite->field_0;
            Data24->field_4 = pSprite->field_4 + 1;
            Data24->field_20 = pSprite->field_20 + 0x0E;

            if (!word_3B4ED[0])
                if (!pSprite->field_1E && !pSprite->field_20)
                    return;

            if (pSprite->field_20 < 0x0C)
                if (mMission_EngineTicks & 1)
                    return;

            Data24->field_A += 1;
            Data24->field_A &= 3;
            return;
        }
    }

    if (pSprite->field_26 != 0x7171) {

        pSprite->field_26 = 0x7171;
        pSprite->field_1A = 0x10000;

        Data0 = tool_RandomGet() & 0x0F;
        Data0 += 0x13;

        pSprite->field_2A = Data0;
    }

    dword_3B24B = -1;

    Data2C = 0;
    Sprite_Create_Smoke(pSprite, Data2C);
    Sprite_Movement_Calculate(pSprite);

    pSprite->field_1A = (((int64)pSprite->field_1A) + 0x2000);
    Data0 = ((int64)pSprite->field_1A) >> 16;
    pSprite->field_A += Data0;
    pSprite->field_A &= 0x0F;
    pSprite->field_2A -= 1;
    if (pSprite->field_2A >= 0) {

        pSprite->field_1E_Big -= 0x18000;
        if (pSprite->field_1E_Big >= 0)
            goto loc_1A404;

        pSprite->field_1E_Big = 0;
    }

    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = 0x1F50;
    pSprite->field_28 = -9;
    (pSprite + 1)->field_18 = eSprite_Helicopter_PropCrash;

    Data0 = tool_RandomGet() & 0x1FE;
    (pSprite + 1)->field_10 = Data0;
    (pSprite + 1)->field_36 = 0x60;

    Sprite_Destroy(pSprite + 2);

    if (pSprite->field_22 == eSprite_PersonType_Human)
        Sprites_HumanVehicles_Remove(pSprite);

    if (mSprite_Helicopter_DestroyLight)
        Sprite_Destroy(pSprite + 3);

    return;

loc_1A149:;
    pSprite->field_8 = 0x8B;
    if (pSprite->field_44)
        goto loc_1A217;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_26;
    DataC = pSprite->field_28;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    dword_3B24B = Data0;

    if (Data0 >= 0x60)
        goto loc_1A217;

    Data0 >>= 1;

    if (Data0 > pSprite->field_36)
        goto loc_1A217;

    if (pSprite->field_22 != eSprite_PersonType_Human) {
        pSprite->field_36 = Data0;
        if (!Data0) {
            Data8 = pSprite->field_0;
            DataC = pSprite->field_4;

            if (Map_Terrain_GetMoveable_WithCache(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
                goto loc_1A316;

            Data8 = pSprite->field_0;
            Data8 -= 0x10;
            DataC = pSprite->field_4;

            if (Map_Terrain_GetMoveable_WithCache(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
                goto loc_1A316;

            Data8 = pSprite->field_0;
            Data8 += 0x10;
            DataC = pSprite->field_4;

            if (Map_Terrain_GetMoveable_WithCache(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
                goto loc_1A316;

            if (pSprite->field_20) {
                pSprite->field_1E_Big -= 0x8000;
            }
            goto loc_1A316;
        }

        goto loc_1A316;
    }

    pSprite->field_36 = Data0;
    if (!pSprite->field_6E)
        goto loc_1A316;

    pSprite->field_1E_Big -= 0xC000;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
    }
    Sprite_Handle_Helicopter_Terrain_Check(pSprite);
    goto loc_1A316;

loc_1A217:;
    if (pSprite->field_20 < 0x20) {
        pSprite->field_20++;
        if (pSprite->field_20 < 0x12)
            goto loc_1A316;
    }
    //loc_1A239
    if (pSprite->field_36 >= 0x30)
        goto loc_1A316;

    pSprite->field_36 += 2;
    if (dword_3B24B >= 0x60)
        goto loc_1A316;

    if (!pSprite->field_6E)
        goto loc_1A316;

    pSprite->field_1E_Big -= 0xC000;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
    }
    goto loc_1A316;

loc_1A316:;
    if (pSprite->field_26 >= 0 && pSprite->field_28 >= 0) {

        if (pSprite->field_20)
            Sprite_Movement_Calculate(pSprite);
    }

    if (pSprite->field_22 == eSprite_PersonType_Human && !pSprite->field_36 &&
        pSprite->field_20 >= 0x20) {
        Data0 = mMouseX + (mCameraX >> 16);
        Data0 -= 0x10;

        Data4 = mMouseY + (mCameraY >> 16);
        Data4 += 0x20;
        pSprite->field_36 = 0x1E;
        Sprite_Helicopter_Update_Direction(pSprite, Data0, Data4);
        pSprite->field_36 = 0;

        Data8 = pSprite->field_3C;
        if (pSprite->field_22 != eSprite_PersonType_Human) {
            if (!pSprite->field_36)
                goto loc_1A404;
        }
        pSprite->field_A = Data8;
    }
    else {

        word_3B2F7 = 0x3A;
        Data0 = pSprite->field_26;
        Data4 = pSprite->field_28;
        Sprite_Vehicle_TurnTowardTarget(pSprite, Data0, Data4);
    }

loc_1A404:;
    Data24 = pSprite + 1;
    Data24->field_8 = 0x8C;
    Data24->field_0 = pSprite->field_0;
    Data24->field_4 = pSprite->field_4;
    Data24->field_4 += 1;
    Data0 = pSprite->field_20;
    Data0 += 0x0E;
    Data24->field_20 = Data0;

    if (!word_3B4ED[0])
        if (!pSprite->field_1E_Big)
            goto loc_1A49C;

    if (pSprite->field_20 < 0x0C)
        if (mMission_EngineTicks & 1)
            goto loc_1A49C;

    Data24->field_A += 1;
    Data24->field_A &= 3;

loc_1A49C:;
    Data24++;
    Data24->field_8 = 0x8D;

    Data0 = pSprite->field_20;
    Data0 >>= 4;
    if (Data0 > 2)
        Data0 = 2;

    Data24->field_A = Data0;
    Data24->field_2C = eSprite_Draw_First;

    Data0 = pSprite->field_20;
    Data0 >>= 1;
    Data4 = Data0;
    Data0 += 9;

    Data0 += pSprite->field_0;
    Data24->field_0 = Data0;
    Data4 -= 1;
    Data4 += pSprite->field_4;
    Data24->field_4 = Data4;

    if (pSprite->field_22 == eSprite_PersonType_Human) {
        if (pSprite->field_20) {

            Data8 = pSprite->field_0;
            DataC = pSprite->field_0;
            DataC += 0x1E;
            Data10 = pSprite->field_4;
            Data10 -= 0x14;
            Data14 = pSprite->field_4;
            Data18 = pSprite->field_20;
            Data1C = Data18;
            Data1C += 0x0E;

            Sprite_Under_Vehicle(pSprite, Data8, DataC, Data10, Data14, Data18, Data1C);
        }
    }

    pSprite->field_22 = eSprite_PersonType_Human;
}

void cFodder::Sprite_Handle_Helicopter_Grenade_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Grenade;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Unarmed_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Missile_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Missile;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Homing;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Unarmed_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter;
    Sprite_Handle_Helicopter_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Grenade_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Grenade;
    Sprite_Handle_Helicopter_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Missile_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Missile;
    Sprite_Handle_Helicopter_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Homing;
    Sprite_Handle_Helicopter_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_PropCrash(sSprite* pSprite) {
    pSprite->field_A += 1;
    pSprite->field_A &= 3;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1C321;

    pSprite->field_36 -= 1;
    if (pSprite->field_36 < 0)
        pSprite->field_36 = 0;

    pSprite->field_1E_Big -= 0x8000;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
    }

loc_1C321:;
    pSprite->field_18 = eSprite_Explosion;
    Sprite_Create_FireTrail(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_CallPad(sSprite* pSprite) {
    sSprite* Data2C = 0;

    pSprite->field_8 = 0xE7;
    pSprite->field_2C = eSprite_Draw_First;

    // Find a troop inrange 
    if (Sprite_Handle_Helicopter_Callpad_InRange(pSprite, Data2C)) {
        pSprite->field_75 = 0;
    }
    else {
        ++mHelicopterCallPadPressedCount;

        if (!pSprite->field_75) {
            mTroop_InRange_Callpad = Data2C;
            pSprite->field_75 = -1;
        }
    }

    if (mHelicopterCall_X < 0) {
        pSprite->field_A = 0;
        return;
    }

    int16 Data0 = pSprite->field_32;
    Data0 += 2;
    Data0 &= 6;

    pSprite->field_A = mSprite_Helicopter_CallPad_Frames[Data0 / 2];
    pSprite->field_32 = Data0;
}

void cFodder::Sprite_Handle_Helicopter_Grenade_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Grenade_Human(pSprite);

}

void cFodder::Sprite_Handle_Helicopter_Unarmed_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Unarmed_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Missile_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Missile_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Homing_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Enemy2(sSprite* pSprite) {

    Sprite_Handle_Helicopter_Homing_Enemy(pSprite);
}

void cFodder::Sprite_Handle_UFO_Callpad(sSprite* pSprite) {
    
    if (mHelicopterCall_X > 0) {

        mSwitchesActivated = true;
        pSprite->field_8 = 0xE8;
        pSprite->field_A = 0;

        sSprite* Data24 = pSprite + 1;
        Data24->field_8 = 0x8D;
        Data24->field_A = 0x01;
        if (pSprite->field_20 >= 0x14)
            pSprite->field_1E_Big -= 0xC000;
        
        int16 Data0 = mHelicopterCall_X;
        int16 Data4 = mHelicopterCall_Y - 0x28;

        Sprite_Direction_Between_Points(pSprite, Data0, Data4);

        Data0 = mHelicopterCall_X;
        Data4 = mHelicopterCall_Y - 0x28;
        int16 Data8 = pSprite->field_0;
        int16 DataC = pSprite->field_4;
        int16 Data10 = 0x60;

        Data0 = Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
        Data0 >>= 3;
        Data0 += 1;
        if (Data0 < 8)
            Data0 = 8;

        pSprite->field_36 = Data0;
        Sprite_Movement_Calculate(pSprite);
        Sprite_PositionNext_AdjustByHeight(pSprite);
    } else {
        // loc_2DBE8
        pSprite->field_8 = 0x7C;
        pSprite->field_A = 0;

        sSprite* Data24 = pSprite + 1;
        Data24->field_8 = 0x7C;
        Data24->field_A = 0;

        if (mSquad_Leader && mSquad_Leader != INVALID_SPRITE_PTR) {
 
            pSprite->field_0 = mSquad_Leader->field_0 + 0x190;
            pSprite->field_4 = mSquad_Leader->field_4;
        }

        pSprite->field_20 = 0x64;
        Sprite_PositionNext_AdjustByHeight(pSprite);
    }
}
