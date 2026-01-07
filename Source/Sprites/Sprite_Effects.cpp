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

    Data2C->mPosX = mMapTile_TargetX >> 16;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += 0x0A;

    Data2C->mPosX += Data0;
    Data2C->mPosY = mMapTile_TargetY >> 16;

    Data0 = tool_RandomGet() & 0xFF;
    Data0 -= 0x14;

    Data2C->mPosY += Data0;
    Data2C->mSpriteType = eSprite_Explosion;
    return Data0;
}

int16 cFodder::Sprite_Create_Smoke(sSprite* pSprite, sSprite*& pData2C) {
    if (!pSprite->mWasDrawn)
        return -1;

    int16 Data0 = 1;
    sSprite* Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return -1;

    pData2C->mPosX = pSprite->mPosX;
    pData2C->mPosXFrac = pSprite->mPosXFrac;
    pData2C->mPosY = pSprite->mPosY;
    pData2C->mPosYFrac = pSprite->mPosYFrac;

    if (dword_3B24B) {
        Data0 = tool_RandomGet() & 0x1F;
        Data0 -= 8;
        pData2C->mPosX += Data0;
        Data0 >>= 4;
        Data0 &= 0x0F;
        Data0 -= 8;
        pData2C->mPosY -= Data0;
        dword_3B24B = 0;
    }

    pData2C->mHeight = pSprite->mHeight;
    pData2C->mRowsToSkip = 0;
    pData2C->mSheetIndex = 0xCC;
    pData2C->mFrameIndex = 4;
    pData2C->mDrawOrder = eSprite_Draw_OnTop;
    pData2C->mSpriteType = eSprite_Smoke;
    return 0;
}

void cFodder::Sprite_Handle_Explosion_MapTiles(sSprite* pSprite) {

    int16 Data0 = pSprite->mPosX + 4;
    int16 Data4 = pSprite->mPosY - 8;

    const int16* Data24 = mSprite_Explosion_Positions;

    for (; *Data24 != -32768;) {

        Data0 += *Data24++;
        Data4 += *Data24++;

        // Dont use negative map x/y coorindates
        if (Data0 < 0 || Data4 < 0) {
            continue;
        }

        mMap_Destroy_Tiles.emplace_back( Data0, (pSprite->mSpriteType == eSprite_Explosion2) ? -Data4 : Data4 );
    }
}

void cFodder::Sprite_Handle_ShadowSmall(sSprite* pSprite) {
    sSprite* Data24 = pSprite - 1;

    pSprite->mPosX = Data24->mPosX;
    pSprite->mPosY = Data24->mPosY;
    pSprite->mRowsToSkip = Data24->mRowsToSkip;

}

void cFodder::Sprite_Handle_Explosion(sSprite* pSprite) {
    if (pSprite->mSheetIndex == 0x7C) {
        Sprite_Destroy_Wrapper_2(pSprite);
        return;
    }

    if (pSprite->mSheetIndex == 0xC0 || pSprite->mSheetIndex == 0x8E) {
        //loc_1A8D1

        int16 X_Left = pSprite->mPosX;
        X_Left += 8;

        if (pSprite->mTargetX == 0x5F5F)
            X_Left += pSprite->mTargetY;

        int16 X_Right = pSprite->mPosX;
        X_Right += 0x24;

        int16 Y_Top = pSprite->mPosY;
        Y_Top -= 0x20;

        int16 Y_Bottom = pSprite->mPosY;
        Y_Bottom -= 6;

        const int16* Explosion_Area_PerFrame = mSprite_Explosion_Area_PerFrame;
        int16 Frame = pSprite->mFrameIndex;

        if (pSprite->mSheetIndex == 0xC0)
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

        pSprite->mAIAggression = ~pSprite->mAIAggression;
        if (pSprite->mAIAggression >= 0)
            Squad_Members_Find_In_Region(pSprite, X_Left, X_Right, Y_Top, Y_Bottom);
        else
            Sprite_Find_In_Region(pSprite, Data24, X_Left, X_Right, Y_Top, Y_Bottom);

        pSprite->field_12 -= 1;
        if (pSprite->field_12 >= 0)
            return;

        pSprite->field_12 = 1;

        Explosion_Area_PerFrame = mSprite_Explosion_Area_PerFrame;
        int16 Data4 = pSprite->mFrameIndex;
        if (pSprite->mSheetIndex == 0xC0)
            Data4 += 2;

        //seg004:1CC9
        pSprite->mPosX -= Explosion_Area_PerFrame[Data4];
        pSprite->mPosY += Explosion_Area_PerFrame[Data4 + 1];
        pSprite->mFrameIndex += 1;
        if (pSprite->mSheetIndex != 0xC0)
            goto loc_1AA63;

        if (pSprite->mFrameIndex < 4)
            return;

        Sprite_Destroy_Wrapper(pSprite);
        return;

    loc_1AA63:;
        if (pSprite->mHeight && pSprite->mFrameIndex >= 2) {
            pSprite->mSheetIndex = 0xC0;
            pSprite->mFrameIndex = 0;
        }
        if (pSprite->mFrameIndex >= 7)
            Sprite_Destroy_Wrapper(pSprite);

        return;
    }

    //loc_1AA9F
    if (pSprite->mWasDrawn) {
        Sprite_Handle_Explosion_MapTiles(pSprite);
        int16 Data0 = tool_RandomGet() & 0x1FE;
        int16 Data18 = Data0;
        Sprite_Create_Sparks(pSprite, Data18);
        Data18 += 0x100;
        Data18 &= 0x1FE;
        Sprite_Create_Sparks(pSprite, Data18);
    }
    //loc_1AAFD
    pSprite->mPosX -= 6;
    if (pSprite->mPosX < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
        return;
    }

    //loc_1AB0D
    pSprite->mPosY += 4;
    if (pSprite->mPosY < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
        return;
    }

    int16 Data0 = pSprite->mTargetX;
    if (Data0 < 0) {
        Data0 &= 0x7FFF;
        pSprite->mPosX += Data0;
        if (pSprite->mPosX < 0) {
            Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
            return;
        }
        pSprite->mTargetX = 0;
    }
    //loc_1AB6F
    int16 Data4 = 5 + (mInterruptTick & 3);

    Sound_Play(pSprite, Data4, 0x1E);
    pSprite->mSheetIndex = 0x8E;
    pSprite->mFrameIndex = 0;
    pSprite->field_12 = 1;
    pSprite->mRowsToSkip = 0;
    pSprite->mPersonType = eSprite_PersonType_None;
    pSprite->field_32 = -1;
    pSprite->mDrawOrder = eSprite_Draw_First;
}

void cFodder::Sprite_Handle_BloodTrail(sSprite* pSprite) {

    if (pSprite->mHeight)
        --pSprite->mHeight;

    --pSprite->field_12;
    if (pSprite->field_12 >= 0)
        return;

    pSprite->field_12 = 1;
    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex < 4)
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
            Data2C->mPosX += 6;
            Data2C->mPosY -= 5;
            Data2C->mRowsToSkip = 0x0C;
        }

        pSprite->field_2A = 0;
        return;
    }

    pSprite->mDrawOrder = eSprite_Draw_First;
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
    pSprite->mFrameIndex = 0;

    int16 Data0 = tool_RandomGet() & 0xFF;
    int16 Data4 = 0x64;
    Data4 -= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;
    Data0 += 0x0A;
    Data4 += mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;

    pSprite->field_12 = Data0;
    return;
}

void cFodder::Sprite_Handle_Text_Complete(sSprite* pSprite) {
    pSprite->mPosX = mMapTile_TargetX >> 16;
    pSprite->mPosX += (getCameraWidth() / 2) - 65;
    pSprite->mPosY -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += getCameraHeight() / 2 + 7;

    if (Data0 < pSprite->mPosY)
        return;

    pSprite->mPosY = Data0;
}

void cFodder::Sprite_Handle_Text_Mission(sSprite* pSprite) {
    pSprite->mPosX = mMapTile_TargetX >> 16;
    pSprite->mPosX += (getCameraWidth() / 2) - 50;
    pSprite->mPosY -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->mPosY)
        return;

    pSprite->mPosY = Data0;
}

void cFodder::Sprite_Handle_Text_Phase(sSprite* pSprite) {
    pSprite->mPosX = mMapTile_TargetX >> 16;
    pSprite->mPosX += (getCameraWidth() / 2) - 41;

    pSprite->mPosY -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->mPosY)
        return;

    pSprite->mPosY = Data0;
}

void cFodder::Sprite_Handle_Text_GameOver(sSprite* pSprite) {

    pSprite->mPosX = mMapTile_TargetX >> 16;
    pSprite->mPosX += 0x5B;
    pSprite->mPosY -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += 0x77;

    if (Data0 < pSprite->mPosY)
        return;

    pSprite->mPosY = Data0;
}

void cFodder::Sprite_Handle_Shadow(sSprite* pSprite) {
    sSprite* Data28 = pSprite->field_1A_sprite;

    if (Data28->mSpriteType == eSprite_Explosion) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    if (Data28->mRowsToSkip) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    pSprite->mSheetIndex = 0x8D;
    int16 Data0 = Data28->mHeight;

    Data0 >>= 3;
    Data0 += 1;
    if (Data0 > 2)
        Data0 = 1;

    pSprite->mFrameIndex = Data0;
    pSprite->mPosX = Data28->mPosX;
    pSprite->mPosY = Data28->mPosY;
    Data0 = Data28->mHeight;
    Data0 >>= 2;
    pSprite->mPosX += Data0;
    pSprite->mPosY += Data0;
    pSprite->mPosX += 5;
    pSprite->mPosY -= 3;

    if (Data28->mSheetIndex == 0x7C)
        pSprite->mSheetIndex = 0x7C;
}

void cFodder::Sprite_Handle_Building_Explosion(sSprite* pSprite) {
    int16 Data0, Data4, Data6, Data8, DataC, Data10, Data14;
    const int16* Data2C = mSprite_Explosion_Area_PerFrame;
    sSprite* Data24 = 0;

    if (pSprite->mSheetIndex == 0x7C) {
        Sprite_Destroy_Wrapper_2(pSprite);
        return;
    }

    pSprite->mAIAggression = ~pSprite->mAIAggression;
    if (pSprite->mAIAggression < 0)
        goto loc_1BC07;


    Data8 = pSprite->mPosX;
    Data8 += 8;
    if (pSprite->mTargetX == 0x5F5F)
        Data8 += pSprite->mTargetY;

    DataC = pSprite->mPosX + 0x24;

    Data10 = pSprite->mPosY - 0x20;
    Data14 = pSprite->mPosY - 6;

    Data0 = pSprite->mFrameIndex;

    if (pSprite->mSheetIndex == 0xC0)
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
    Data8 = pSprite->mPosX;
    DataC = pSprite->mPosX + 0x1E;
    Data10 = pSprite->mPosY - 30;
    Data14 = pSprite->mPosY;

    Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14);
loc_1BC48:;

    Data2C = mSprite_Explosion_Area_PerFrame;
    Data4 = pSprite->mFrameIndex;
    if (pSprite->mSheetIndex == 0xC0)
        Data4 += 2;

    Data4 <<= 1;
    Data2C += Data4;

    Data4 = *(Data2C + 1);
    Data6 = *Data2C;

    pSprite->mPosX -= Data4;
    pSprite->mPosY += Data6;

    pSprite->mFrameIndex += 1;
    if (pSprite->mSheetIndex == 0xC0) {
        if (pSprite->mFrameIndex < 4)
            return;

        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    if (!pSprite->mHeight || pSprite->mFrameIndex < 2)
        goto loc_1BD06;

    pSprite->mSheetIndex = 0xC0;
    pSprite->mFrameIndex = 0;
loc_1BD06:;

    if (pSprite->mFrameIndex < 3)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Flashing_Light(sSprite* pSprite) {

    pSprite->mDrawOrder = eSprite_Draw_Second;
    pSprite->mSheetIndex = 0xC4;

    pSprite->field_43++;
    pSprite->field_43 &= 1;
    if (!pSprite->field_43) {
        pSprite->mFrameIndex++;
        pSprite->mFrameIndex &= 3;
    }

    pSprite->mPosY += 0x18;
    pSprite->mHeight += 0x18;
}

void cFodder::Sprite_Handle_Smoke(sSprite* pSprite) {
    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (pSprite->field_2A)
        return;

    pSprite->mFrameIndex -= 1;
    if (pSprite->mFrameIndex >= 0)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Text_Try(sSprite* pSprite) {
    pSprite->mPosX = mMapTile_TargetX >> 16;
    pSprite->mPosX += (getCameraWidth() / 2) - 23;
    pSprite->mPosY -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->mPosY)
        return;

    pSprite->mPosY = Data0;
}

void cFodder::Sprite_Handle_Text_Again(sSprite* pSprite) {
    pSprite->mPosX = mMapTile_TargetX >> 16;
    pSprite->mPosX += (getCameraWidth() / 2) - 49;
    pSprite->mPosY -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += getCameraHeight() / 2 + 5;

    if (Data0 < pSprite->mPosY)
        return;

    pSprite->mPosY = Data0;
}

void cFodder::Sprite_Handle_BoilingPot(sSprite* pSprite) {
    pSprite->mSheetIndex = 0xCD;
    pSprite->mFrameIndex += 1;
    pSprite->mFrameIndex &= 3;

    sSprite* Data24 = pSprite + 1;
    Data24->mSheetIndex = 0xCE;
    Data24->field_2A += 1;
    Data24->field_2A &= 1;

    if (!Data24->field_2A) {
        Data24->mFrameIndex += 1;

        if (Data24->mFrameIndex >= 6)
            Data24->mFrameIndex = 0;
    }

    Data24->mHeight = 0x10;
}

void cFodder::Sprite_Handle_Explosion2(sSprite* pSprite) {

    Sprite_Handle_Explosion(pSprite);
}

void cFodder::Sprite_Handle_Bonus_Armour(sSprite* pSprite) {
    if (pSprite->mAnimState) {
        pSprite->mSpriteType = eSprite_Explosion;
        return;
    }

    pSprite->mSheetIndex = 0xE3;
    pSprite->mFrameIndex = 0;
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
        pData2C->mPosX = pSprite->mPosX;
        pData2C->mPosXFrac = pSprite->mPosXFrac;
        pData2C->mPosY = pSprite->mPosY;
        pData2C->mPosYFrac = pSprite->mPosYFrac;

        pData2C->mPosX += 3;
        pData2C->mPosY -= 8;
        pData2C->mSheetIndex = 0x96;
        pData2C->mFrameIndex = 0;
        pData2C->mSpriteType = eSprite_BloodTrail;
        pData2C->mHeight = pSprite->mHeight;
        pData2C->field_32 = -1;
        pData2C->mDrawOrder = eSprite_Draw_OnTop;
        pData2C->mRowsToSkip = 0;
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

    Data2C->mPosX = pSprite->mPosX;
    Data2C->mPosXFrac = pSprite->mPosXFrac;
    Data2C->mPosY = pSprite->mPosY;
    Data2C->mPosYFrac = pSprite->mPosYFrac;

    Data2C->mPosX += 0x0E;
    Data2C->mPosY += 0x10;
    Data2C->mSheetIndex = 0x8D;
    Data2C->mFrameIndex = 0;
    Data2C->mSpriteType = eSprite_Shadow;
    Data2C->mHeight = 0;
    Data2C->field_32 = -1;
    Data2C->mDrawOrder = eSprite_Draw_First;
    Data2C->mRowsToSkip = 0;
}

void cFodder::Sprite_Turn_Into_Building_Explosion(sSprite* pSprite) {
    int16 Data8 = pSprite->mPosX;
    int16 DataC = pSprite->mPosY;

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

    pData2C->mPosX = pX;
    pData2C->mPosY = pY;
    pData2C->mPosY += 0x10;

    int16 Data4 = mInterruptTick;
    Data4 &= 3;
    Data4 += 5;

    Sound_Play(pData2C, Data4, 0x1E);

    pData2C->mSpriteType = eSprite_Building_Explosion;
    pData2C->mSheetIndex = 0x8E;
    pData2C->mFrameIndex = 0;
    pData2C->field_12 = 1;
    pData2C->mPersonType = eSprite_PersonType_None;
    pData2C->field_32 = -1;
    pData2C->mDrawOrder = eSprite_Draw_OnTop;

    return 0;
}
