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
    if (!pSprite->field_5C)
        return;

    if ((tool_RandomGet() & 0x3F) != 9)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    pSprite->field_2E = mSquad_Leader->field_0;
    pSprite->field_30 = mSquad_Leader->field_4;

    Sprite_Create_Civilian_Spear2(pSprite);
}

void cFodder::Sprite_Handle_Civilian_Movement(sSprite* pSprite) {

    // Destination change cool down
    if (pSprite->field_4C) {
        pSprite->field_4C -= 1;
        if (pSprite->field_4C)
            return;
    }

    if (pSprite->field_18 == eSprite_Hostage || pSprite->field_18 == eSprite_Enemy_Leader) {
        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
    }
    else {
        if (Sprite_Handle_Civilian_RandomMovement(pSprite) < 0)
            return;
    }

    // Random time until destination change
    int16 Data0 = tool_RandomGet() & 0x3F;
    Data0 += 0x14;
    pSprite->field_4C = static_cast<int8>(Data0);
}

int16 cFodder::Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(sSprite* pSprite) {

    if (!mSprite_OpenCloseDoor_Ptr)
        return -1;

    if (pSprite->field_4C) {
        --pSprite->field_4C;
        return 0;
    }

    sSprite* Data28 = mSprite_OpenCloseDoor_Ptr;

    int16 Data0 = pSprite->field_0;
    int16 Data4 = pSprite->field_4;
    int16 Data8 = Data28->field_0;
    int16 DataC = Data28->field_4;

    pSprite->field_26 = Data8;
    pSprite->field_28 = DataC;

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

    pSprite->field_26 = pSprite->field_0;
    pSprite->field_28 = pSprite->field_4;
    pSprite->field_26 -= 0x10;
    pSprite->field_28 -= 0x20;
    pSprite->field_4C = 0x1E;

    return 0;
}

void cFodder::Sprite_Handle_Civilian_Death(sSprite* pSprite) {

    if (pSprite->field_8 != 0xD6) {
        word_3B2D1[2] = -1;

        if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Protect_Civilians - 1])
            mPhase_Aborted = true;

        if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Get_Civilian_Home - 1])
            mPhase_Aborted = true;

        pSprite->field_8 = 0xD6;
        pSprite->field_A = 0;

        int16 Data0 = tool_RandomGet() & 7;
        int16 Data4 = mSprite_Civilian_Sound_Death[Data0];

        Sound_Play(pSprite, Data4, 0x14);
    }
    else {
        //loc_2584A
        pSprite->field_A += 1;
        if (pSprite->field_A < 9)
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

    pSprite->field_26 = X;
    pSprite->field_28 = Y;
    return 0;
}

void cFodder::Sprite_Civilian_Update_Movement(sSprite* pSprite) {
    Sprite_XY_Store(pSprite);

    if (pSprite->field_44)
        --pSprite->field_44;
    else {
        int16 Data0 = pSprite->field_26;
        int16 Data4 = pSprite->field_28;

        if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) < 0)
            mSprite_Reached_Target = -1;
    }

    if (pSprite->field_18 != eSprite_Seal) {
        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 0x0E;
        if (Data4 < 0)
            Data0 = -Data0;

        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
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

    Data4 = pSprite->field_60;

    if (Data4 == 6) {
        Sprite_XY_Restore(pSprite);
        return;
    }

    if (Data4 != 4) {
        if (Data4 != 5)
            return;

        pSprite->field_52 = 5;
        return;
    }

    pSprite->field_52 = 3;
}

void cFodder::Sprite_WalkAnim_Tick(sSprite* pSprite) {

    if (mSprite_Reached_Target || word_3B25B)
        return;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 3;
    if (pSprite->field_2A)
        return;

    pSprite->field_A += 1;
    pSprite->field_A &= 1;
}

void cFodder::Sprite_WalkFrame_From_Direction(sSprite* pSprite) {
    int16 Data0 = pSprite->field_10;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;

    int16 Data4 = pSprite->field_A;
    Data4 &= 1;
    Data0 |= Data4;
    pSprite->field_A = Data0;
}

void cFodder::Sprite_Hostage_Update_IdleFrame(sSprite* pSprite) {

    if (word_3B2ED) {
        pSprite->field_8 = 0xD8;
        pSprite->field_A = 1;
        Sprite_XY_Restore(pSprite);
        return;
    }

    if (word_3B2D1[2] != pSprite->field_0 || word_3B2D1[3] != pSprite->field_2)
        return;

    if (word_3B2D1[4] != pSprite->field_4 || word_3B2D1[5] != pSprite->field_6)
        return;

    pSprite->field_8 = 0xDA;

    int16 Data0 = mMission_EngineTicks & 0x0F;
    if (!Data0) {
        Data0 = tool_RandomGet() & 0x06;
        pSprite->field_32 = mSprite_Hostage_Frames[Data0 / 2];
    }

    pSprite->field_A = pSprite->field_32;
}

void cFodder::Sprite_Advance_SearchIndex(sSprite* pSprite) {
    ++pSprite->field_5E;
    if (pSprite->field_5E >= (mParams->mSpritesMax - 2))
		pSprite->field_5E = 0;
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
        pSprite->field_8 = 0x9B;
        return;
    }

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_26580;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A)
        goto loc_265B1;

    if (pSprite->field_43 >= 0) {
        pSprite->field_8 = 0x7C;
        return;
    }

    pSprite->field_8 = 0x9B;
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
    pSprite->field_8 = 0x7C;

loc_265B1:;
    sSprite* Data2C = 0, *Data30 = 0;

    if (!Sprite_Create_Native(pSprite, Data2C, Data30)) {
        Data2C->field_4 -= 4;
        return;
    }

    pSprite->field_12 = 0;
    pSprite->field_43 = -1;
}

int16 cFodder::Sprite_Create_Civilian_Spear2(sSprite* pSprite) {
    if (mPhase_Completed_Timer)
        return -1;

    if (!pSprite->field_2E)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_8 = 0xD7;
    Data2C->field_A = 0;
    Data2C->field_12 = 9;
    Data0 = pSprite->field_62;

    Data0 >>= 3;
    Data0 += 8;
    if (Data0 > 0x10)
        Data0 = 0x10;

    Data2C->field_12 = Data0;
    Data2C->field_18 = eSprite_Civilian_Spear2;
    Data2C->field_1E_Big = pSprite->field_1E_Big;
    Data2C->field_1E_Big += 0x60000;

    Data2C->field_52 = pSprite->field_52;
    Data2C->field_22 = pSprite->field_22;
    Data2C->field_2C = eSprite_Draw_Second;
    Data0 = 0x32;
    Data0 += pSprite->field_62;

    Data2C->field_4A = Data0;
    Data2C->field_36 = 0x18;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data2C->field_2E = pSprite->field_2E;
    Data2C->field_30 = pSprite->field_30;
    Data2C->field_2E += 7;
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

    if (pSprite->field_18 == eSprite_Enemy_Leader)
        goto CheckRescueTent;

    // No known rescue tent?
    if (!mHostage_Rescue_Tent)
        goto loc_2608B;

    if (pSprite->field_74) {
        pSprite->field_74 -= 1;
        goto loc_2608B;
    }
    //loc_2600B

    pSprite->field_74 = -56;

    Data8 = mHostage_Rescue_Tent->field_0 + 10;
    DataC = mHostage_Rescue_Tent->field_4 - 5;
    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data10 = 0x80;

    // Distance to rescue tent < 127?
    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data10 < 0x7F)
        pSprite->field_5E = (int16)(mHostage_Rescue_Tent - mSprites.data());

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

        pSprite->field_26 = Data28->field_0 + 10;
        pSprite->field_28 = Data28->field_4 - 5;

        Data0 = pSprite->field_0;
        Data4 = pSprite->field_4;
        Data8 = pSprite->field_26;
        DataC = pSprite->field_28;
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
        Data28->field_70 = pSprite;
        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
        word_3B2ED = -1;
        return;
    }

    pSprite->field_26 = Data28->field_0 + 4;
    pSprite->field_28 = Data28->field_4 - 6;

    // Not in vehicle?
    if (!Data28->field_6E)
        goto loc_2620F;

    // Vehicle Has Speed?
    if (Data28->field_6A_sprite->field_36 > 2)
        goto loc_2620F;

    // Vehicle has height?
    if (Data28->field_6A_sprite->field_20 > 3)
        goto loc_2620F;

    Data0 = Data28->field_6A_sprite->field_0;
    Data4 = Data28->field_6A_sprite->field_4;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
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

    pSprite->field_6E = -1;
    return;

CheckRescueTent:;
    word_3B2ED = 0;
    Data0 = eSprite_Player;
    Data4 = eSprite_Hostage_Rescue_Tent;
    Data8 = -1;
    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        return;

    if (Data4 == eSprite_Hostage_Rescue_Tent) {

        pSprite->field_26 = Data28->field_0 + 0x0A;
        pSprite->field_28 = Data28->field_4 - 0x05;

        Data0 = pSprite->field_0;
        Data4 = pSprite->field_4;
        Data8 = pSprite->field_26;
        DataC = pSprite->field_28;
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
    pSprite->field_8 = 0xD8;
    pSprite->field_26 = Data28->field_0 + 4;
    pSprite->field_28 = Data28->field_4 - 6;

    // Not in vehicle?
    if (!Data28->field_6E)
        goto loc_263E5;

    // Vehicle has speed?
    if (Data28->field_6A_sprite->field_36 > 2)
        goto loc_263E5;

    // Vehicle has height
    if (Data28->field_6A_sprite->field_20 > 3)
        goto loc_263E5;

    Data0 = Data28->field_6A_sprite->field_0;
    Data4 = Data28->field_6A_sprite->field_4;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 0x0F) {
        pSprite->field_6E = -1;
        return;
    }

loc_263E5:;
    Data0 = tool_RandomGet() & 7;
    if (Data0)
        return;

    Sprite_Advance_SearchIndex(pSprite);
}

void cFodder::Sprite_Handle_Hostage_FrameUpdate2(sSprite* pSprite) {
    int16 Data0 = pSprite->field_4A;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (!pSprite->field_2A)
        Data0 += 2;

    Data0 &= 6;

    pSprite->field_4A = Data0;
    pSprite->field_A = mSprite_Hostage_Frames[Data0 / 2];

}

void cFodder::Sprite_Handle_Hostage_FrameUpdate(sSprite* pSprite) {
    int16 Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;

    int16 Data4 = Data0;
    Data4 >>= 1;
    Data4 += Data0;

    pSprite->field_A += Data4;
}

int16 cFodder::Sprite_Create_Native(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30) {
    int16 Data4;

    if (!pSprite->field_5C)
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
    pData2C->field_18 = mSpawnSpriteType;
    pData2C->field_0 = pSprite->field_0;
    pData2C->field_0 -= 6;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_4 += 4;
    pData2C->field_8 = 0x7C;
    pData2C->field_4A = 0;
    pData2C->field_22 = eSprite_PersonType_Native;

    Data0 = tool_RandomGet();
    Data4 = Data0;
    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pData2C->field_10 = Data0;
    Data4 &= 0x0F;
    Data4 += 8;
    pData2C->field_44 = static_cast<int8>(Data4);
    return 0;
}

void cFodder::Sprite_Handle_Bird2_Left(sSprite* pSprite) {

    pSprite->field_8 = 0x98;
    pSprite->field_2C = eSprite_Draw_OnTop;
    pSprite->field_12 += 1;
    pSprite->field_12 &= 1;
    if (!pSprite->field_12)
        pSprite->field_A ^= 1;

    pSprite->field_0 -= 2;
    if (pSprite->field_0 < -64)
        pSprite->field_0 = 0x7D0;
}

void cFodder::Sprite_Handle_Civilian(sSprite* pSprite) {
    if (pSprite->field_38) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);

    if (ax) {
        if (ax >= 0)
            return;

        Sprite_Handle_Civilian_Movement(pSprite);
    }

    pSprite->field_36 = 6;
    Sprite_Civilian_Update_Movement(pSprite);
    Sprite_WalkAnim_Tick(pSprite);
    Sprite_WalkFrame_From_Direction(pSprite);
}

void cFodder::Sprite_Handle_Civilian2(sSprite* pSprite) {

    if (pSprite->field_38) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);
    if (ax > 0)
        return;
    if (ax < 0)
        Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->field_36 = 0x0A;
    Sprite_Civilian_Update_Movement(pSprite);
    Sprite_WalkAnim_Tick(pSprite);
    Sprite_WalkFrame_From_Direction(pSprite);
    if (!word_3B2D1[1])
        return;

    Sprite_Handle_Civilian_Unk(pSprite);
}

void cFodder::Sprite_Handle_Bird_Left(sSprite* pSprite) {

    pSprite->field_8 = 0xD3;
    pSprite->field_2C = eSprite_Draw_OnTop;

    if (!pSprite->field_12) {

        pSprite->field_57 = 8;
        pSprite->field_12 = 1;

        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 3;
        Data0 += 1;

        pSprite->field_A = Data0;
        if (Data4 < 0)
            pSprite->field_12 = -1;
    }

    int16 Data0 = pSprite->field_12;
    pSprite->field_A += Data0;

    if (pSprite->field_A) {
        if (pSprite->field_A < 5)
            goto loc_1C82D;
    }

    Data0 = -Data0;
    pSprite->field_12 = Data0;

loc_1C82D:;
    bool cf = false;

    if (pSprite->field_2 >= 0 && pSprite->field_2 - 32768 < 0)
        cf = true;

    pSprite->field_2 -= 32768;
    pSprite->field_0 -= 1 + (cf == true ? 1 : 0);


    if (Data0 < 0) {
        if (pSprite->field_2 >= 0 && pSprite->field_2 - 32768 < 0)
            cf = true;
        else
            cf = false;

        pSprite->field_2 -= 32768;
        pSprite->field_0 -= 0 + (cf == true ? 1 : 0);
    }

    if (pSprite->field_5C)
        goto loc_1C8C5;

    if (!pSprite->field_57)
        goto loc_1C87E;

    pSprite->field_57 -= 1;
    goto loc_1C8C5;

loc_1C87E:;

    pSprite->field_57 = 0x3F;

	Data0 = (tool_RandomGet() & 0x3F) + (mMapTile_TargetX >> 16) + getWindowWidth();
    pSprite->field_0 = Data0;

    Data0 = (tool_RandomGet() & 0xFF) + (mMapTile_TargetY >> 16);
    pSprite->field_4 = Data0;

loc_1C8C5:;

    if (mMapLoaded->getTileType() == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);

}

void cFodder::Sprite_Handle_Bird_Right(sSprite* pSprite) {
    pSprite->field_8 = 0xD4;
    pSprite->field_2C = eSprite_Draw_OnTop;

    if (!pSprite->field_12) {
        pSprite->field_57 = 8;
        pSprite->field_12 = 1;

        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 3;
        Data0 += 1;

        pSprite->field_A = Data0;
        if (Data4 < 0)
            pSprite->field_12 = -1;
    }

    //loc_1C950
    int16 Data0 = pSprite->field_12;
    pSprite->field_A += Data0;
    if (!pSprite->field_A || pSprite->field_A >= 5) {
        Data0 = -Data0;
        pSprite->field_12 = Data0;
    }

    bool cf = false;

    if (pSprite->field_2 < 0 && pSprite->field_2 + 32768 > 0)
        cf = true;

    pSprite->field_2 += 32768;
    pSprite->field_0 += 1 + (cf == true ? 1 : 0);

    if (Data0 < 0) {
        if (pSprite->field_2 < 0 && pSprite->field_2 + 32768 > 0)
            cf = true;
        else
            cf = false;

        pSprite->field_2 += 32768;
        pSprite->field_0 += 0 + (cf == true ? 1 : 0);
    }

    //loc_1C9B0
    if (pSprite->field_5C)
        goto loc_1CA20;

    if (pSprite->field_57) {
        pSprite->field_57 -= 1;
        goto loc_1CA20;
    }

    //loc_1C9D3
    pSprite->field_57 = 0x3F;
    Data0 = tool_RandomGet() & 0x3F;
    Data0 = -Data0;
    Data0 += mMapTile_TargetX >> 16;

    pSprite->field_0 = Data0;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += mMapTile_TargetY >> 16;
    pSprite->field_4 = Data0;

loc_1CA20:;
    if (mMapLoaded->getTileType() == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);
}

void cFodder::Sprite_Handle_Seal(sSprite* pSprite) {
    if (Sprite_Update_Death_BloodOrSeal(pSprite))
        return;

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD5;

    if (!pSprite->field_2E) {
        pSprite->field_2E = -1;
        pSprite->field_54 = tool_RandomGet() & 0x0F;
    }

    if (mMission_EngineTicks & 1)
        ++pSprite->field_54;

    int16 Data0 = pSprite->field_54 & 0x0F;

    Data0 = mSprite_Seal_AnimFrames[Data0];
    pSprite->field_A = Data0;

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
    pSprite->field_22 = eSprite_PersonType_AI;
    if (pSprite->field_38) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    if (mMapLoaded->getTileType() == eTileTypes_Moors)
        Sprite_Native_Sound_Play(pSprite, 0x26);

    if (mMapLoaded->getTileType() == eTileTypes_Int)
        Sprite_Native_Sound_Play(pSprite, 0x1F);

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);
    if (ax > 0)
        return;
    if (ax)
        Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->field_36 = 0x0C;

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
    Data1C = pSprite->field_10;

    if (pSprite->field_59)
        goto loc_1CF3E;

    Data0 = pSprite->field_2E;
    Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Data0 = pSprite->field_50;

    if (Data0) {
        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
        pSprite->field_59 = -1;
    }

    if (pSprite->field_34 < 0)
        pSprite->field_34 = pSprite->field_10;

loc_1CF3E:;
    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1D17A;

    pSprite->field_36 = pSprite->field_4A;
    if (pSprite->field_59)
        goto loc_1D00F;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    Data4 = pSprite->field_36;
    Data4 >>= 3;
    if (Data0 > Data4)
        goto loc_1D00F;

    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_0 = pSprite->field_2E;
    pSprite->field_4 = pSprite->field_30;
    pSprite->field_44 = -1;
    goto loc_1D00F;
loc_1CFF7:;
    pSprite->field_2C = eSprite_Draw_First;
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

    if (pSprite->field_8 != 0xD7)
        return;

    Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    Data0 >>= 1;
    pSprite->field_A = Data0;
    return;
loc_1D07B:;

    if (mSprites_Found_Count == 1)
        goto loc_1D0F6;

    pSprite->field_43 = 1;
    pSprite->field_2C = eSprite_Draw_OnTop;
    pSprite->field_4 -= 5;
    if (pSprite->field_4 < 0)
        goto loc_1D18D;
    pSprite->field_8 = 0x96;
    pSprite->field_A = 0;

    Sound_Play(pSprite, eSound_Effect_Spear, 0x0F);
    return;

loc_1D0CB:;
    pSprite->field_8 = 0x96;
    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if (pSprite->field_A >= 4)
        goto loc_1D17A;
    return;

loc_1D0F6:;
    if (pSprite->field_43)
        goto loc_1D14D;

    pSprite->field_43 = -1;
    pSprite->field_2C = eSprite_Draw_First;
    pSprite->field_4 -= 5;
    if (pSprite->field_4 < 0)
        goto loc_1D18D;

    pSprite->field_A = 3;
    Sound_Play(pSprite, eSound_Effect_Spear, 0x0F);
loc_1D14D:;
    if (pSprite->field_43 >= 0)
        goto loc_1D0CB;

    pSprite->field_12 = 0;
    pSprite->field_8 = 0x7F;
    pSprite->field_A += 1;
    if (pSprite->field_A >= 8)
        goto loc_1D17A;
    return;

loc_1D17A:;
    --mSprite_Projectile_Counters[2];
    Sprite_Destroy(pSprite);
    return;

loc_1D18D:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
    goto loc_1D17A;
}

void cFodder::Sprite_Handle_Hostage(sSprite* pSprite) {
    int16 Data0, Data4;
    int16 Sprite_Field_10;
    sSprite* Data24 = 0;

    if (pSprite->field_6E)
        goto loc_1D349;

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD9;

    word_3B2D1[2] = pSprite->field_0;
    word_3B2D1[3] = pSprite->field_2;
    word_3B2D1[4] = pSprite->field_4;
    word_3B2D1[5] = pSprite->field_6;

    Sprite_Handle_Hostage_Movement(pSprite);
    Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->field_36 = 0x0C;
    Sprite_Civilian_Update_Movement(pSprite);
    pSprite->field_38 = eSprite_Anim_None;

    Sprite_Handle_Hostage_FrameUpdate2(pSprite);
    //seg004:44B6

    Sprite_Field_10 = pSprite->field_10;
    pSprite->field_10 += 0x20;
    pSprite->field_10 &= 0x1C0;

    Sprite_Handle_Hostage_FrameUpdate(pSprite);
    pSprite->field_10 = Sprite_Field_10;
    Sprite_Hostage_Update_IdleFrame(pSprite);

    if (pSprite->field_18 != eSprite_Enemy_Leader)
        return;

    Data24 = pSprite + 1;

    //seg004:4505
    if (pSprite->field_5E) {
        Data24->field_18 = eSprite_Null;
        Data24->field_8 = 0x7C;
        Data24->field_A = 0;
    }

    Data24->field_18 = eSprite_Flashing_Light;

    Data4 = pSprite->field_0;
    Data4 += 6;

    Data24->field_0 = Data4;
    Data4 = pSprite->field_4 - 0x0B;
    Data4 += pSprite->field_52;
    Data24->field_4 = Data4;
    Data24->field_20 = pSprite->field_20;

    if (pSprite->field_6E) {
        Data24->field_18 = eSprite_Null;
        Data24->field_8 = 0x7C;
        Data24->field_A = 0;
    }

    Data0 = pSprite->field_0;
    if (Data0 == mStoredSpriteX) {
        Data0 = pSprite->field_4;
        if (Data0 == mStoredSpriteY)
            return;
    }
    pSprite->field_8 = 0xD8;
    return;

loc_1D349:;
    pSprite->field_38 = eSprite_Anim_None;
    sSprite* Data28 = &mSprites[pSprite->field_5E];
    if (Data28->field_6E)
        goto loc_1D44C;

    pSprite->field_0 = Data28->field_0;
    pSprite->field_4 = Data28->field_4;
    pSprite->field_0 += 8;
    pSprite->field_4 += 0x0A;
    mStoredSpriteY = pSprite->field_4;

loc_1D3C6:;
    Data0 = -3;
    Data4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_1D411;

    pSprite->field_4 -= 1;
    if (pSprite->field_4 >= 0)
        goto loc_1D3C6;

    pSprite->field_4 = mStoredSpriteY;
    pSprite->field_0 -= 1;
    if (pSprite->field_0 >= 0)
        goto loc_1D3C6;

    pSprite->field_0 = 0;
    pSprite->field_38 = eSprite_Anim_Die1;

loc_1D411:;
    Data4 = pSprite->field_4;
    Data0 = mMapLoaded->getHeightPixels();

    if (Data4 < Data0)
        goto loc_1D441;

    Data0 -= 1;
    pSprite->field_4 = Data0;

loc_1D441:;
    pSprite->field_6E = 0;
    return;

loc_1D44C:;
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
    pSprite->field_0 = Data28->field_0;
    pSprite->field_4 = Data28->field_4;
}

void cFodder::Sprite_Handle_Hostage_Rescue_Tent(sSprite* pSprite) {
    pSprite->field_8 = 0xDD;

    mHostage_Rescue_Tent = pSprite;
    if (pSprite->field_38 == eSprite_Anim_Die3)
        pSprite->field_18 = eSprite_Explosion2;
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

    if (!pSprite->field_38)
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
    pSprite->field_2C = eSprite_Draw_First;
    if (!pSprite->field_2A) {
        pSprite->field_8 = 0x9B;
        return;
    }

    pSprite->field_2A -= 1;
    if (!pSprite->field_2A)
        Sound_Play(pSprite, eSound_Effect_DoorCloseDoor, 1);

    if (pSprite->field_8 == 0x9B)
        Sound_Play(pSprite, eSound_Effect_DoorCloseDoor, 1);

    pSprite->field_8 = 0x7C;
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
