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

int16 cFodder::Sprite_Create_RandomExplosion() {
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30)) {
        Data0 = -1;
        return -1;
    }

    Data2C->field_0 = mMapTile_TargetX >> 16;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += 0x0A;

    Data2C->field_0 += Data0;
    Data2C->field_4 = mMapTile_TargetY >> 16;

    Data0 = tool_RandomGet() & 0xFF;
    Data0 -= 0x14;

    Data2C->field_4 += Data0;
    Data2C->field_18 = eSprite_Explosion;
    return Data0;
}

int16 cFodder::Sprite_Create_Smoke(sSprite* pSprite, sSprite*& pData2C) {
    if (!pSprite->field_5C)
        return -1;

    int16 Data0 = 1;
    sSprite* Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return -1;

    pData2C->field_0 = pSprite->field_0;
    pData2C->field_2 = pSprite->field_2;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_6 = pSprite->field_6;

    if (dword_3B24B) {
        Data0 = tool_RandomGet() & 0x1F;
        Data0 -= 8;
        pData2C->field_0 += Data0;
        Data0 >>= 4;
        Data0 &= 0x0F;
        Data0 -= 8;
        pData2C->field_4 -= Data0;
        dword_3B24B = 0;
    }

    pData2C->field_20 = pSprite->field_20;
    pData2C->field_52 = 0;
    pData2C->field_8 = 0xCC;
    pData2C->field_A = 4;
    pData2C->field_2C = eSprite_Draw_OnTop;
    pData2C->field_18 = eSprite_Smoke;
    return 0;
}

void cFodder::Sprite_Handle_Explosion_MapTiles(sSprite* pSprite) {

    int16 Data0 = pSprite->field_0 + 4;
    int16 Data4 = pSprite->field_4 - 8;

    const int16* Data24 = mSprite_Explosion_Positions;

    for (; *Data24 != -32768;) {

        Data0 += *Data24++;
        Data4 += *Data24++;

        // Dont use negative map x/y coorindates
        if (Data0 < 0 || Data4 < 0) {
            continue;
        }

        mMap_Destroy_Tiles.emplace_back( Data0, (pSprite->field_18 == eSprite_Explosion2) ? -Data4 : Data4 );
    }
}

void cFodder::Sprite_Handle_ShadowSmall(sSprite* pSprite) {
    sSprite* Data24 = pSprite - 1;

    pSprite->field_0 = Data24->field_0;
    pSprite->field_4 = Data24->field_4;
    pSprite->field_52 = Data24->field_52;

}

void cFodder::Sprite_Handle_Explosion(sSprite* pSprite) {
    if (pSprite->field_8 == 0x7C) {
        Sprite_Destroy_Wrapper_2(pSprite);
        return;
    }

    if (pSprite->field_8 == 0xC0 || pSprite->field_8 == 0x8E) {
        //loc_1A8D1

        int16 X_Left = pSprite->field_0;
        X_Left += 8;

        if (pSprite->field_26 == 0x5F5F)
            X_Left += pSprite->field_28;

        int16 X_Right = pSprite->field_0;
        X_Right += 0x24;

        int16 Y_Top = pSprite->field_4;
        Y_Top -= 0x20;

        int16 Y_Bottom = pSprite->field_4;
        Y_Bottom -= 6;

        const int16* Explosion_Area_PerFrame = mSprite_Explosion_Area_PerFrame;
        int16 Frame = pSprite->field_A;

        if (pSprite->field_8 == 0xC0)
            Frame += 2;
        Frame -= 1;

        if (Frame >= 0) {

            do {
                X_Left -= *Explosion_Area_PerFrame;
                X_Right -= *(Explosion_Area_PerFrame + 1);
                Y_Top += *Explosion_Area_PerFrame;

                ++Explosion_Area_PerFrame;
                Y_Bottom += *Explosion_Area_PerFrame;
                ++Explosion_Area_PerFrame;

            } while (--Frame >= 0);
        }
        //loc_1A99D
        sSprite* Data24 = 0;

        pSprite->field_62 = ~pSprite->field_62;
        if (pSprite->field_62 >= 0)
            Squad_Members_Find_In_Region(pSprite, X_Left, X_Right, Y_Top, Y_Bottom);
        else
            Sprite_Find_In_Region(pSprite, Data24, X_Left, X_Right, Y_Top, Y_Bottom);

        pSprite->field_12 -= 1;
        if (pSprite->field_12 >= 0)
            return;

        pSprite->field_12 = 1;

        Explosion_Area_PerFrame = mSprite_Explosion_Area_PerFrame;
        int16 Data4 = pSprite->field_A;
        if (pSprite->field_8 == 0xC0)
            Data4 += 2;

        //seg004:1CC9
        pSprite->field_0 -= Explosion_Area_PerFrame[Data4];
        pSprite->field_4 += Explosion_Area_PerFrame[Data4 + 1];
        pSprite->field_A += 1;
        if (pSprite->field_8 != 0xC0)
            goto loc_1AA63;

        if (pSprite->field_A < 4)
            return;

        Sprite_Destroy_Wrapper(pSprite);
        return;

    loc_1AA63:;
        if (pSprite->field_20 && pSprite->field_A >= 2) {
            pSprite->field_8 = 0xC0;
            pSprite->field_A = 0;
        }
        if (pSprite->field_A >= 7)
            Sprite_Destroy_Wrapper(pSprite);

        return;
    }

    //loc_1AA9F
    if (pSprite->field_5C) {
        Sprite_Handle_Explosion_MapTiles(pSprite);
        int16 Data0 = tool_RandomGet() & 0x1FE;
        int16 Data18 = Data0;
        Sprite_Create_Sparks(pSprite, Data18);
        Data18 += 0x100;
        Data18 &= 0x1FE;
        Sprite_Create_Sparks(pSprite, Data18);
    }
    //loc_1AAFD
    pSprite->field_0 -= 6;
    if (pSprite->field_0 < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
        return;
    }

    //loc_1AB0D
    pSprite->field_4 += 4;
    if (pSprite->field_4 < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
        return;
    }

    int16 Data0 = pSprite->field_26;
    if (Data0 < 0) {
        Data0 &= 0x7FFF;
        pSprite->field_0 += Data0;
        if (pSprite->field_0 < 0) {
            Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
            return;
        }
        pSprite->field_26 = 0;
    }
    //loc_1AB6F
    int16 Data4 = 5 + (mInterruptTick & 3);

    Sound_Play(pSprite, Data4, 0x1E);
    pSprite->field_8 = 0x8E;
    pSprite->field_A = 0;
    pSprite->field_12 = 1;
    pSprite->field_52 = 0;
    pSprite->field_22 = eSprite_PersonType_None;
    pSprite->field_32 = -1;
    pSprite->field_2C = eSprite_Draw_First;
}

void cFodder::Sprite_Handle_BloodTrail(sSprite* pSprite) {

    if (pSprite->field_20)
        --pSprite->field_20;

    --pSprite->field_12;
    if (pSprite->field_12 >= 0)
        return;

    pSprite->field_12 = 1;
    pSprite->field_A += 1;
    if (pSprite->field_A < 4)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_GroundHole(sSprite* pSprite) {
    if (!mEnemy_BuildingCount)
        return;

    if (pSprite->field_2A) {
        //loc_1B194
        sSprite* Data2C = 0;

        if (!Sprite_Create_Enemy(pSprite, Data2C)) {
            Data2C->field_0 += 6;
            Data2C->field_4 -= 5;
            Data2C->field_52 = 0x0C;
        }

        pSprite->field_2A = 0;
        return;
    }

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0) {
        //loc_1B161
        pSprite->field_12 -= 1;
        if (pSprite->field_12 >= 0)
            return;

        int16 Data0 = tool_RandomGet() & 0x0F;
        Data0 += 6;
        pSprite->field_43 = static_cast<int8>(Data0);
        pSprite->field_2A = 0;
        return;
    }

    pSprite->field_43 -= 1;
    if (pSprite->field_43 != 0x14 && pSprite->field_43 != 0x0A) {

        if (pSprite->field_43 >= 0)
            return;
    }

    //loc_1B11A
    pSprite->field_2A = -1;
    pSprite->field_A = 0;

    int16 Data0 = tool_RandomGet() & 0xFF;
    int16 Data4 = 0x64;
    Data4 -= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;
    Data0 += 0x0A;
    Data4 += mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;

    pSprite->field_12 = Data0;
    return;
}

void cFodder::Sprite_Handle_Text_Complete(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 65;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += getCameraHeight() / 2 + 7;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Mission(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 50;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Phase(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 41;

    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_GameOver(sSprite* pSprite) {

    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += 0x5B;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += 0x77;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Shadow(sSprite* pSprite) {
    sSprite* Data28 = pSprite->field_1A_sprite;

    if (Data28->field_18 == eSprite_Explosion) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    if (Data28->field_52) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    pSprite->field_8 = 0x8D;
    int16 Data0 = Data28->field_20;

    Data0 >>= 3;
    Data0 += 1;
    if (Data0 > 2)
        Data0 = 1;

    pSprite->field_A = Data0;
    pSprite->field_0 = Data28->field_0;
    pSprite->field_4 = Data28->field_4;
    Data0 = Data28->field_20;
    Data0 >>= 2;
    pSprite->field_0 += Data0;
    pSprite->field_4 += Data0;
    pSprite->field_0 += 5;
    pSprite->field_4 -= 3;

    if (Data28->field_8 == 0x7C)
        pSprite->field_8 = 0x7C;
}

void cFodder::Sprite_Handle_Building_Explosion(sSprite* pSprite) {
    int16 Data0, Data4, Data6, Data8, DataC, Data10, Data14;
    const int16* Data2C = mSprite_Explosion_Area_PerFrame;
    sSprite* Data24 = 0;

    if (pSprite->field_8 == 0x7C) {
        Sprite_Destroy_Wrapper_2(pSprite);
        return;
    }

    pSprite->field_62 = ~pSprite->field_62;
    if (pSprite->field_62 < 0)
        goto loc_1BC07;


    Data8 = pSprite->field_0;
    Data8 += 8;
    if (pSprite->field_26 == 0x5F5F)
        Data8 += pSprite->field_28;

    DataC = pSprite->field_0 + 0x24;

    Data10 = pSprite->field_4 - 0x20;
    Data14 = pSprite->field_4 - 6;

    Data0 = pSprite->field_A;

    if (pSprite->field_8 == 0xC0)
        Data0 += 2;

    Data0 -= 1;

    for (; Data0 >= 0; --Data0) {
        Data8 -= *Data2C;
        DataC -= *Data2C++;
        Data10 += *Data2C;
        Data14 += *Data2C++;
    }

    Squad_Members_Find_In_Region(pSprite, Data8, DataC, Data10, Data14);
    goto loc_1BC48;

loc_1BC07:;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_0 + 0x1E;
    Data10 = pSprite->field_4 - 30;
    Data14 = pSprite->field_4;

    Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14);
loc_1BC48:;

    Data2C = mSprite_Explosion_Area_PerFrame;
    Data4 = pSprite->field_A;
    if (pSprite->field_8 == 0xC0)
        Data4 += 2;

    Data4 <<= 1;
    Data2C += Data4;

    Data4 = *(Data2C + 1);
    Data6 = *Data2C;

    pSprite->field_0 -= Data4;
    pSprite->field_4 += Data6;

    pSprite->field_A += 1;
    if (pSprite->field_8 == 0xC0) {
        if (pSprite->field_A < 4)
            return;

        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    if (!pSprite->field_20 || pSprite->field_A < 2)
        goto loc_1BD06;

    pSprite->field_8 = 0xC0;
    pSprite->field_A = 0;
loc_1BD06:;

    if (pSprite->field_A < 3)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Flashing_Light(sSprite* pSprite) {

    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_8 = 0xC4;

    pSprite->field_43++;
    pSprite->field_43 &= 1;
    if (!pSprite->field_43) {
        pSprite->field_A++;
        pSprite->field_A &= 3;
    }

    pSprite->field_4 += 0x18;
    pSprite->field_20 += 0x18;
}

void cFodder::Sprite_Handle_Smoke(sSprite* pSprite) {
    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (pSprite->field_2A)
        return;

    pSprite->field_A -= 1;
    if (pSprite->field_A >= 0)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Text_Try(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 23;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Again(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 49;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += getCameraHeight() / 2 + 5;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_BoilingPot(sSprite* pSprite) {
    pSprite->field_8 = 0xCD;
    pSprite->field_A += 1;
    pSprite->field_A &= 3;

    sSprite* Data24 = pSprite + 1;
    Data24->field_8 = 0xCE;
    Data24->field_2A += 1;
    Data24->field_2A &= 1;

    if (!Data24->field_2A) {
        Data24->field_A += 1;

        if (Data24->field_A >= 6)
            Data24->field_A = 0;
    }

    Data24->field_20 = 0x10;
}

void cFodder::Sprite_Handle_Explosion2(sSprite* pSprite) {

    Sprite_Handle_Explosion(pSprite);
}

void cFodder::Sprite_Handle_Bonus_Armour(sSprite* pSprite) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0xE3;
    pSprite->field_A = 0;
    int16 Data0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mSquad_Leader->field_75 |= eSprite_Flag_Invincibility;

    Sprite_Destroy_Wrapper(pSprite);
}

int16 cFodder::Sprite_Create_BloodTrail(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30) {
    int16 Data0;

    Data0 = 1;

    if (!Sprite_Get_Free_Max42(Data0, pData2C, pData30)) {
        pData2C->field_0 = pSprite->field_0;
        pData2C->field_2 = pSprite->field_2;
        pData2C->field_4 = pSprite->field_4;
        pData2C->field_6 = pSprite->field_6;

        pData2C->field_0 += 3;
        pData2C->field_4 -= 8;
        pData2C->field_8 = 0x96;
        pData2C->field_A = 0;
        pData2C->field_18 = eSprite_BloodTrail;
        pData2C->field_20 = pSprite->field_20;
        pData2C->field_32 = -1;
        pData2C->field_2C = eSprite_Draw_OnTop;
        pData2C->field_52 = 0;
        pData2C->field_12 = 1;
        Data0 = 0;
    }

    return Data0;
}

void cFodder::Sprite_Create_Shadow(sSprite* pSprite) {
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data2C->field_1A_sprite = pSprite;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;

    Data2C->field_0 += 0x0E;
    Data2C->field_4 += 0x10;
    Data2C->field_8 = 0x8D;
    Data2C->field_A = 0;
    Data2C->field_18 = eSprite_Shadow;
    Data2C->field_20 = 0;
    Data2C->field_32 = -1;
    Data2C->field_2C = eSprite_Draw_First;
    Data2C->field_52 = 0;
}

void cFodder::Sprite_Turn_Into_Building_Explosion(sSprite* pSprite) {
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    pSprite->Clear();
    Sprite_Create_Building_Explosion(pSprite, Data8, DataC);
}

int16 cFodder::Sprite_Create_Building_Explosion_Wrapper(int16& pX, int16& pY) {
    int16 Data0 = 1;
    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    Data2C->Clear();

    pX &= -16;
    pY &= -16;

    return Sprite_Create_Building_Explosion(Data2C, pX, pY);
}

int16 cFodder::Sprite_Create_Building_Explosion(sSprite* pData2C, int16& pX, int16& pY) {

    pData2C->field_0 = pX;
    pData2C->field_4 = pY;
    pData2C->field_4 += 0x10;

    int16 Data4 = mInterruptTick;
    Data4 &= 3;
    Data4 += 5;

    Sound_Play(pData2C, Data4, 0x1E);

    pData2C->field_18 = eSprite_Building_Explosion;
    pData2C->field_8 = 0x8E;
    pData2C->field_A = 0;
    pData2C->field_12 = 1;
    pData2C->field_22 = eSprite_PersonType_None;
    pData2C->field_32 = -1;
    pData2C->field_2C = eSprite_Draw_OnTop;

    return 0;
}
