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

void cFodder::Sprite_Clear_All() {
	mSprites.resize(mParams->mSpritesMax);
	Squad_Set_Squad_Leader();

    for (auto& Sprite : mSprites) {
        Sprite.Clear();
    }
	mSprite_Spare.Clear();

    mSprites[mSprites.size() - 1].mPosX = -1;
    mSprite_SpareUsed = 0;
}

void cFodder::Sprite_Sort_DrawList() {

    mSprite_DrawList_First.clear();
    mSprite_DrawList_Second.clear();
    mSprite_DrawList_Third.clear();
    for(sSprite& sprite : mSprites) {

        if (sprite.mPosX == -32768)
            continue; 

        if (sprite.mDrawOrder == eSprite_Draw_First) {
            mSprite_DrawList_First.push_back(&sprite);
            continue;
        }

        if (sprite.mDrawOrder == eSprite_Draw_Second) {
            mSprite_DrawList_Second.push_back(&sprite);
            continue;
        }

        mSprite_DrawList_Third.push_back(&sprite);
    }

    std::sort(mSprite_DrawList_Second.begin(), mSprite_DrawList_Second.end(), [](sSprite*& pLeft, sSprite*& pRight) {
        return pLeft->mPosY < pRight->mPosY;
    });

    //loc_124AF
    {
        std::lock_guard<std::mutex> lock(mSpriteMtx);

        mSprite_DrawList_Final.clear();
        for (auto spriteptr : mSprite_DrawList_First) {
            mSprite_DrawList_Final.push_back(spriteptr);
        }
        for (auto spriteptr : mSprite_DrawList_Second) {
            mSprite_DrawList_Final.push_back(spriteptr);
        }
        for (auto spriteptr : mSprite_DrawList_Third) {
            mSprite_DrawList_Final.push_back(spriteptr);
        }
    }
}

void cFodder::Sprite_Draw_Frame(sSprite* pDi, int16 pSpriteType, int16 pFrame, cSurface *pDestination) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount - pDi->mRowsToSkip;

#if defined(_OFED) || defined(_OFBOT)
    mMapTile_DrawY = (mMapTile_MovedVertical * 16) + mMapTile_RowOffset;
    mMapTile_DrawX = (mMapTile_MovedHorizontal * 16) + mMapTile_ColumnOffset;
    
    mVideo_Draw_PosX = (int16)((SheetData->mModX + pDi->mPosX) - mMapTile_DrawX);
#else
    mVideo_Draw_PosX = (int16)((SheetData->mModX + pDi->mPosX) - mMapTile_DrawX + 0x40);
#endif

    mVideo_Draw_PosY = (int16)((SheetData->mModY + pDi->mPosY) - mVideo_Draw_Rows - pDi->mHeight - mMapTile_DrawY);
    mVideo_Draw_PosY += 0x10;

    if (Sprite_OnScreen_Check()) {
        pDi->mWasDrawn = 1;
        if(!mStartParams->mDisableVideo)
            mGraphics->Video_Draw_8(pDestination);
    }
    else
        pDi->mWasDrawn = 0;
}

bool cFodder::Sprite_OnScreen_Check() {

    return mGraphics->Sprite_OnScreen_Check();
}

void cFodder::Sprite_Map_Sound_Play(int16 &pData0) {

    mSoundEffectToPlay_Set = pData0;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    goto loc_14D66;
    //seg002:05B0
    // UNUSED BLOCK

loc_14D66:;
    mSoundEffectToPlay = mSoundEffectToPlay_Set;
}

void cFodder::Sprite_Shadow_Update_From_Height(sSprite* pSprite) {
    sSprite* Data24 = pSprite + 1;

    Data24->mSheetIndex = 0x8D;
    int16 Data0 = pSprite->mHeight;
    Data0 >>= 4;
    if (Data0 > 2)
        Data0 = 2;

    Data24->mFrameIndex = Data0;
    Data0 = pSprite->mHeight;
    Data0 >>= 1;

    int16 Data4 = Data0;
    Data0 += 9;
    Data0 += pSprite->mPosX;
    Data24->mPosX = Data0;
    Data4 -= 1;
    Data4 += pSprite->mPosY;
    Data24->mPosY = Data4;
    if (!pSprite->mRowsToSkip)
        return;

    Data24->mFrameIndex = 0;
    Data24->mSheetIndex = 0x7C;
}

int16 cFodder::Sprite_Animation_SlideOrDie(sSprite* pSprite) {
    sSprite* Data24 = 0;

    // Die animation running?
    if (pSprite->mAnimState == eSprite_Anim_Die1 || pSprite->mAnimState == eSprite_Anim_Die3) {
        pSprite->mSpriteType = eSprite_Explosion;
        pSprite->mTargetX = 0x1F56;
        pSprite->mTargetY = -9;
        Data24 = pSprite + 1;

        Sprite_Destroy(Data24);
        if (pSprite->mPersonType == eSprite_PersonType_Human)
            return -1;

        Data24 = pSprite + 2;
        Sprite_Destroy(Data24);
        return -1;
    }

    // Slide animation running?
    if (pSprite->mAnimState == eSprite_Anim_Slide1) {

        pSprite->mSpeed = -pSprite->mSpeed;
        pSprite->mAnimState = eSprite_Anim_Slide2;

    }
    else if (pSprite->mAnimState == eSprite_Anim_Slide2) {
        pSprite->mSpeed += 6;

        if (pSprite->mSpeed >= 0) {
            pSprite->mAnimState = eSprite_Anim_None;
            pSprite->mSpeed = 0;
        }

    }
    else {
        return 0;
    }

    Sprite_Movement_Calculate(pSprite);
    Sprite_Shadow_Update_From_Height(pSprite);
    return -1;
}

int16 cFodder::Sprite_Update_Death_BloodOrSeal(sSprite* pSprite) {
    sSprite* Data2C = 0, *Data30 = 0;
    int16 Data0;

    if (pSprite->mAnimState == eSprite_Anim_None)
        return 0;

    if (pSprite->mAnimState == eSprite_Anim_Die4)
        goto loc_25E8F;

    if (!pSprite->field_2A) {
        pSprite->field_2A = 0x14;
        pSprite->mFrameIndex = 0;
    }
    pSprite->field_2A -= 1;
    if (!pSprite->field_2A)
        return Sprite_Destroy_Wrapper(pSprite);

    //loc_25E19
    if (pSprite->field_2A >= 0x0A)
        goto loc_25E54;

    pSprite->mFrameIndex = 0;
    if (pSprite->mSheetIndex == 0xD5) {
        //loc_25E47
        pSprite->mSheetIndex = 0x7C;
        return -1;
    }

    pSprite->mSheetIndex = 0xD5;
    return -1;

loc_25E54:;
    if (Sprite_Create_BloodTrail(pSprite, Data2C, Data30))
        return -1;

    Data2C->mPosY += tool_RandomGet() & 3;
    Data2C->mPosX -= 4;
    Data2C->mPosX += tool_RandomGet() & 7;
    return -1;

loc_25E8F:;
    if (pSprite->mSheetIndex != 0xE1) {
        pSprite->mSheetIndex = 0xE1;
        pSprite->mFrameIndex = 0;
        pSprite->field_2A = 0;
        return -1;
    }

    pSprite->field_2A += 2;
    Data0 = mSprite_Seal_Frames[pSprite->field_2A / 2];

    if (Data0 >= 0) {

        pSprite->mFrameIndex = Data0;
        return -1;
    }

    pSprite->mSpriteType = eSprite_Explosion;
    pSprite->mTargetX = 0x1F45;
    pSprite->mTargetY = -3;
    return -1;
}

void cFodder::Sprite_Update_DirectionMod(sSprite* pSprite) {

    mDirectionMod = 0;
    pSprite->field_3E -= 1;
    if (pSprite->field_3E >= 0)
        return;

    pSprite->field_3E = 1;
    int16 Data8 = pSprite->mFaceDirection;
    int16 Data0 = pSprite->mDirection;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;
    int16 Data4 = Data0;
    Data4 &= 0x0F;

    int16 DataC = Data8;
    DataC -= Data4;
    DataC &= 0x0F;
    //seg007:0920
    DataC = (int16)mDirectionStepTable[DataC];
    DataC <<= 1;

    mDirectionMod = DataC;
}

int16 cFodder::Sprite_Find_By_Types(sSprite* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite*& pData28) {
    int16* Data2C = mSprite_Find_Types;

    mSprite_Find_Types[0] = pData0;
    mSprite_Find_Types[1] = pData4;
    mSprite_Find_Types[2] = pData8;
    mSprite_Find_Types[3] = pDataC;
    mSprite_Find_Types[4] = pData10;

	// Check if Sprite @ mSpriteScanIndex sprite is one of the types
	pData28 = &mSprites[pSprite->mSpriteScanIndex];

	// The original engine checks 1 sprite per cycle, and we have a max of 43 sprites to check
	// Meaning it takes 43 cycles to check all sprites, including empty sprite slots
	// this becomes problematic when you increase maxsprites, especially insane high values like
	//  1,000,000
	if (!mParams->isOriginalSpriteMax()) {
		bool Looped = false;

		// So if we arnt using default max sprites,
		// We counter the problem by searching all sprites in a single loop (we can do this, as we have faster CPUs than in 1993)
		// 
		do {
		NextSprite2:;
			pData28 = &mSprites[pSprite->mSpriteScanIndex];

			if (pSprite->mSpriteScanIndex >= (mParams->mSpritesMax - 2)) {
				pSprite->mSpriteScanIndex = 0;
				if (Looped == true)
					return -1;

				Looped = true;
				continue;
			}

			if (pData28->mPosX == -32768) {
				++pSprite->mSpriteScanIndex;
				continue;
			}

			Data2C = mSprite_Find_Types;
			do {
				pData0 = *Data2C++;
				if (pData0 < 0) {
					++pSprite->mSpriteScanIndex;
					goto NextSprite2;
				}
			} while (pData0 != pData28->mSpriteType);
		} while (pData28->mPosX == -32768);

	} else {
		if (pData28->mPosX == -32768)
			goto NextSprite;

		do {
			pData0 = *Data2C++;
			if (pData0 < 0)
				goto NextSprite;
		} while (pData0 != pData28->mSpriteType);
	}

    // Found a type match
    pData0 = pSprite->mPosX;
    pData4 = pSprite->mPosY;
    pData8 = pData28->mPosX;
    pDataC = pData28->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(pData0, pData4, pData8, pDataC);
    mSprite_Find_Distance = pData0;

    if (pData0 >= 0xD2)
        goto NextSprite;

    if (pData0 <= 0x28)
        goto FoundSprite;

    pData0 = pSprite->mPosX;
    pData4 = pSprite->mPosY;
    pData8 = pData28->mPosX;
    pDataC = pData28->mPosY;

    if (Map_PathCheck_CalculateTo(pData0, pData4, pData8, pDataC))
        goto loc_2439F;

    pData0 = mSprite_Find_Distance;
    goto FoundSprite;

loc_2439F:;
    if (mSprite_Find_Distance < 0x40)
        goto FoundSprite;

NextSprite:;
    pSprite->mSpriteScanIndex++;
    if (pSprite->mSpriteScanIndex >= (mParams->mSpritesMax - 2))
        pSprite->mSpriteScanIndex = 0;

    goto loc_243DD;
FoundSprite:;
    pData4 = pData28->mSpriteType;
    return 0;

loc_243DD:;
    return -1;
}

void cFodder::Sprite_Handle_Computer(sSprite* pSprite, int16 pData1C) {

    if (pSprite->mAnimState == eSprite_Anim_Die3) {
        if (pSprite->mHeliRestartFrame >= pData1C) {
            pSprite->mSpriteType = eSprite_Explosion2;
            return;
        }
        pSprite->mHeliRestartFrame += 1;
        pSprite->mAnimState = eSprite_Anim_None;
    }
    pSprite->mFrameIndex = 0;

    int32 Data0 = pSprite->mHeliRestartFrame;
    Data0 += 0x28;
    Data0 <<= 8;
    bool of = false;

    if (((uint16)pSprite->mComputerAnimAccumulator) + Data0 > 0xFFFF)
        of = true;

    pSprite->mComputerAnimAccumulator += Data0;
    if (of)
        return;

    Data0 = pSprite->field_2A;
    Data0 += 2;
    if (Data0 >= 8)
        Data0 = 0;

    pSprite->field_2A = Data0;
    pSprite->mSheetIndex = mSprite_Computer_Animation[Data0 / 2];
    pSprite->mHeight = mSprite_Computer_Frames[Data0 / 2];
}

void cFodder::Sprite_Movement_Calculate(sSprite* pSprite) {
    mSprite_Bullet_Destroy = 0;
    if (!pSprite->mSpeed)
        return;

    //loc_2A10D
    pSprite->mDirection &= 0x1FE;
    int16 Data4 = pSprite->mDirection;

    int16 Data8 = mDirectionVectorTable[Data4 / 2];

    Data4 += 0x80;
    Data4 &= 0x1FE;
    int16 DataC = mDirectionVectorTable[Data4 / 2];
    Data8 >>= 2;
    DataC >>= 2;
    //seg007:064F

    int32 DataaC = (int32)DataC;
    int32 Dataa8 = (int32)Data8;

    Data4 = pSprite->mSpeed;

    Dataa8 *= Data4;
    DataaC *= Data4;

    int32 tmp = (pSprite->mPosXFrac & 0xFFFF) | pSprite->mPosX << 16;
    tmp += Dataa8;

    pSprite->mPosXFrac = tmp & 0xFFFF;
    pSprite->mPosX = tmp >> 16;

    if (pSprite->mPosX < 0) {
        pSprite->mPosX = 0;
        pSprite->mPosXFrac = 0;
        mSprite_Bullet_Destroy = -1;
    }

    tmp = (pSprite->mPosYFrac & 0xFFFF) | pSprite->mPosY << 16;
    tmp += DataaC;
    pSprite->mPosYFrac = tmp & 0xFFFF;
    pSprite->mPosY = tmp >> 16;

    if (pSprite->mPosY < 0) {
        pSprite->mPosYFrac = 0;
        pSprite->mPosY = 0;
        mSprite_Bullet_Destroy = -1;
    }
}

int16 cFodder::Sprite_Direction_Between_Points(sSprite* pSprite, int16& pData0, int16& pData4) {
    pData0 -= pSprite->mPosX;
    pData4 -= pSprite->mPosY;

    int16 Data10 = 0;
    if (pData0 < 0)
        Data10 = 1;
    Data10 = -Data10;

    if (pData0 < 0)
        pData0 = -pData0;

    int16 Data14 = 0;
    if (pData4 < 0)
        Data14 = 1;
    Data14 = -Data14;

    if (pData4 < 0)
        pData4 = -pData4;

    int16 Data8 = 0x1F;
    int16 DataC = 0;
    if (pData0 <= Data8)
        if (pData4 <= Data8)
            goto loc_2A2F5;

    //seg007:0755
    ++DataC;
    Data8 <<= 1;
    ++Data8;

    //loc_2A283
    for (;;) {
        if (pData0 <= Data8)
            if (pData4 <= Data8)
                break;
        ++DataC;
        Data8 <<= 1;
        ++Data8;
        if (pData0 <= Data8)
            if (pData4 <= Data8)
                break;

        ++DataC;
        Data8 <<= 1;
        ++Data8;
        if (pData0 <= Data8)
            if (pData4 <= Data8)
                break;

        ++DataC;
        Data8 <<= 1;
        ++Data8;
    }
    //loc_2A2E5
    pData0 >>= DataC;
    pData4 >>= DataC;

loc_2A2F5:;
    pData4 <<= 5;
    pData4 |= pData0;
    //pData4 <<= 1;
    pData4 = mMap_DirectionsBetweenPoints[pData4];
    if (pData4 < 0) {
        word_3B25B = -1;
        return -1;
    }

    pData4 <<= 1;
    if (Data10 < 0) {
        //loc_2A376
        if (Data14 >= 0) {
            pData4 += 0x180;
            pData4 &= 0x1FE;
            pSprite->mDirection = pData4;

        }
        else {
            //loc_2A39F
            pData0 = 0x180;
            pData0 -= pData4;
            pData0 &= 0x1FE;
            pSprite->mDirection = pData0;
        }

        word_3B25B = 0;
        return 0;
    }

    //seg007:0804
    if (Data14 >= 0) {
        pData0 = 0x80;
        pData0 -= pData4;
        pData0 &= 0x1FE;
        pSprite->mDirection = pData0;
        word_3B25B = 0;
        return 0;
    }

    //loc_2A354
    pData4 += 0x80;
    pData4 &= 0x1FE;
    pSprite->mDirection = pData4;
    word_3B25B = 0;
    return 0;
}

const sSpriteSheet* cFodder::Sprite_Get_Sheet(int16 pSpriteType, int16 pFrame) {
    const sSpriteSheet* Sheet = &mSprite_SheetPtr[pSpriteType][pFrame];

    return Sheet;
}

void cFodder::Sprite_SetDirectionMod(sSprite* pSprite) {
    mDirectionMod = 0;
    int16 Data0 = pSprite->mDirection;

    Data0 -= pSprite->mFaceDirection;
    word_3B25D = Data0;
    if (Data0 < 0)
        word_3B25D = -word_3B25D;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    int16 Data4 = Data0;
    Data4 &= 0x0F;

    mDirectionMod = (int16)mDirectionStepTable[Data4];
}

sSprite* cFodder::Sprite_Add(size_t pSpriteID, int16 pSpriteX, int16 pSpriteY) {

    int16 Data0 = 3;

    sSprite* First = 0, *Second = 0, *Third = 0, *Fourth = 0;

    if (Sprite_Get_Free_Max42(Data0, First, Second))
        return First;

    Third = Second + 1;
    Fourth = Third + 1;

    First->mSpriteType = (int16)pSpriteID;
    First->mPosX = pSpriteX;
    First->mPosY = pSpriteY;
    First->mTargetX = pSpriteX;
    First->mTargetY = pSpriteY;

    switch (pSpriteID) {
    case eSprite_BoilingPot:                        // 1 Null
    case eSprite_Tank_Human:
    case eSprite_VehicleNoGun_Human:
    case eSprite_VehicleGun_Human:
    case eSprite_VehicleNoGun_Enemy:
    case eSprite_VehicleGun_Enemy:
    case eSprite_Vehicle_Unk_Enemy:
	case eSprite_Enemy_Leader:
        Second->mSpriteType = eSprite_Null;
        Second->mPosX = pSpriteX;
        Second->mPosY = pSpriteY;
        Second->mTargetX = pSpriteX;
        Second->mTargetY = pSpriteY;
        break;

    case eSprite_Helicopter_Grenade_Enemy:          // 3 Nulls
    case eSprite_Helicopter_Unarmed_Enemy:
    case eSprite_Helicopter_Missile_Enemy:
    case eSprite_Helicopter_Homing_Enemy:
    case eSprite_Helicopter_Homing_Enemy2:
        Second->mSpriteType = eSprite_Null;
        Second->mPosX = pSpriteX;
        Second->mPosY = pSpriteY;
        Second->mTargetX = pSpriteX;
        Second->mTargetY = pSpriteY;
        Third->mSpriteType = eSprite_Null;
        Third->mPosX = pSpriteX;
        Third->mPosY = pSpriteY;
        Third->mTargetX = pSpriteX;
        Third->mTargetY = pSpriteY;
        Fourth->mSpriteType = eSprite_Null;
        Fourth->mPosX = pSpriteX;
        Fourth->mPosY = pSpriteY;
        Fourth->mTargetX = pSpriteX;
        Fourth->mTargetY = pSpriteY;
        break;

        // Fall Through
    case eSprite_Helicopter_Grenade_Human:         // 2 Nulls
    case eSprite_Helicopter_Unarmed_Human:
    case eSprite_Helicopter_Missile_Human:
    case eSprite_Helicopter_Homing_Human:
    case eSprite_Helicopter_Grenade_Human_Called:
    case eSprite_Helicopter_Unarmed_Human_Called:
    case eSprite_Helicopter_Missile_Human_Called:
    case eSprite_Helicopter_Homing_Human_Called:
    case eSprite_Tank_Enemy:
        Second->mSpriteType = eSprite_Null;
        Second->mPosX = pSpriteX;
        Second->mPosY = pSpriteY;
        Second->mTargetX = pSpriteX;
        Second->mTargetY = pSpriteY;
        Third->mSpriteType = eSprite_Null;
        Third->mPosX = pSpriteX;
        Third->mPosY = pSpriteY;
        Third->mTargetX = pSpriteX;
        Third->mTargetY = pSpriteY;

        break;
    }
  
    (*this.*mSprite_Function[First->mSpriteType])(First);

    if(Second && Second->mPosX != -32768)
        (*this.*mSprite_Function[Second->mSpriteType])(Second);

    if(Third && Third->mPosX != -32768)
        (*this.*mSprite_Function[Third->mSpriteType])(Third);

    if (Fourth && Fourth->mPosX != -32768)
        (*this.*mSprite_Function[Fourth->mSpriteType])(Fourth);

    return First;
}

int16 cFodder::Sprite_Find_In_Region(sSprite* pSprite, sSprite*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14) {

    if (mPhase_Finished) {
        word_3AA45 = 0;
        return 0;
    }

    mSprites_Found_Count = 0;

    pData24 = mSprites.data();

    for (int32 Data1C = mParams->getSpritesMax() - 2; Data1C >= 0; --Data1C, ++pData24) {
        int16 Data4 = pData24->mSpriteType;

        if (!mSprite_Can_Be_RunOver[Data4])
            continue;

        if (pData24->mPosX < 0)
            continue;

        if (pDataC < pData24->mPosX)
            continue;

        int16 Data0 = pData24->mPosX;
        Data0 += mSprite_Width[Data4];
        if (pData8 > Data0)
            continue;

        Data0 = pData24->mPosY;
        Data0 -= mSprite_Height_Top[Data4];
        if (pData14 < Data0)
            continue;

        //seg009:050C
        Data0 = pData24->mPosY;
        Data0 += mSprite_Height_Bottom[Data4];

        if (pData10 > Data0)
            continue;

        int16 Data18 = pSprite->mHeight;
        Data0 = mSprite_Height_Top[Data4];
        Data0 += 8;

        Data0 += pData24->mHeight;

        if (Data18 > Data0)
            continue;
        Data18 += 0x0B;

        if (Data18 < pData24->mHeight)
            continue;

        if (pData24->mSpriteType != eSprite_Player)
            goto loc_2D5FA;

        if (pData24->mAnimState != eSprite_Anim_Die5)
            continue;

    loc_2D5FA:;
        if (pSprite->mSpriteType == eSprite_Explosion2)
            goto AnimDie3;

        if (pSprite->mSpriteType == eSprite_Building_Explosion)
            goto loc_2D62C;

        if (pSprite->mSpriteType != eSprite_Explosion)
            goto loc_2D642;

        goto loc_2D62C;

    AnimDie3:;
        pData24->mAnimState = eSprite_Anim_Die3;
        goto loc_2D636;

    loc_2D62C:;
        pData24->mAnimState = eSprite_Anim_Die1;

    loc_2D636:;
        pData24->field_64 = -1;
        goto loc_2D6ED;

    loc_2D642:;
        if (mSprite_Can_Be_RunOver[Data4] == 2)
            goto loc_2D659;

        mSprites_Found_Count = 1;
        goto loc_2D705;
    loc_2D659:;

        pData24->mAnimState = eSprite_Anim_Hit;
        pData24->field_64 = -1;
        Data0 = pSprite->mDirection;
        Data4 = Data0;
        Data0 += 0x368;
        Data4 += 0x468;

        pData8 = pData24->mDirection;
        pDataC = pData8;
        pData8 += 0x368;
        pDataC += 0x468;

        pData24->mDirectionOverride = 0;
        if (pData8 < Data0)
            goto loc_2D6D2;

        if (pDataC > Data0)
            goto loc_2D6D2;

        pData24->mDirectionOverride = -1;

    loc_2D6D2:;
        if (pData8 < Data4)
            goto loc_2D6ED;

        if (pDataC > Data4)
            goto loc_2D6ED;

        pData24->mDirectionOverride = -1;

    loc_2D6ED:;
        mSprites_Found_Count = -1;
        pData24->mDirection = pSprite->mDirection;

    loc_2D705:;
        if (word_3AA45)
            break;
    }

    word_3AA45 = 0;
    return mSprites_Found_Count;
}

void cFodder::Sprite_Aggression_Set() {
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin = mGame_Data.mPhase_Current->mAggression.mMin;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax = mGame_Data.mPhase_Current->mAggression.mMax;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage = mGame_Data.mPhase_Current->mAggression.getAverage();

    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext = mGame_Data.mPhase_Current->mAggression.getAverage();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement = 1;

    for(auto& Sprite : mSprites ) {

        if (Sprite.mPosX == -32768)
            continue;

        if (Sprite.mSpriteType == eSprite_Enemy_Rocket || Sprite.mSpriteType == eSprite_Enemy)
            Sprite_Handle_Enemy_Aggression_Set(&Sprite);
    }
}

int16 cFodder::Sprite_Next_WalkTarget_Set(sSprite* pSprite) {

    int16 Data0 = mSquad_WalkTargets[pSprite->field_32][pSprite->mNextWalkTargetIndex].mX;
    int16 Data4 = mSquad_WalkTargets[pSprite->field_32][pSprite->mNextWalkTargetIndex].mY;

    // Do we have a valid walk target?
    if (Data0 >= 0) {
        pSprite->mTargetX = Data0;
        pSprite->mTargetY = Data4;
        pSprite->mNextWalkTargetIndex++;
        pSprite->mFinishedWalking = -1;
        return 0;
    }

    //loc_2D9C0
    if (!pSprite->mFinishedWalking)
        return -1;

    pSprite->mFinishedWalking = 0;

    return Squad_Join(pSprite);
}

void cFodder::Sprite_Frame_Modifier_Update() {

    mSprite_Frame_Modifier_Update_Countdown -= 1;
    if (mSprite_Frame_Modifier_Update_Countdown < 0) {
        mSprite_Frame_Modifier_Update_Countdown = 3;

        mSprite_Frame_1 += mSprite_Frame1_Modifier;
        if (mSprite_Frame_1 == 0 || mSprite_Frame_1 == 2)
            mSprite_Frame1_Modifier ^= -2;

        mSprite_Frame_2 += mSprite_Frame2_Modifier;
        if (mSprite_Frame_2 == 0 || mSprite_Frame_2 == 2)
            mSprite_Frame2_Modifier ^= -2;
    }

    //loc_18DA7
    mSprite_Frame3_ChangeCount -= 1;
    if (mSprite_Frame3_ChangeCount < 0) {
        mSprite_Frame3_ChangeCount = 1;

        mSprite_Frame_3 += mSprite_Frame_3_Modifier;
        if (mSprite_Frame_3 == 0 || mSprite_Frame_3 == 2)
            mSprite_Frame_3_Modifier ^= -2;
    }
}

void cFodder::Sprite_Handle_Loop() {
    sSprite* Data20 = mSprites.data();

    for (int32 Data1C = mParams->getSpritesMax() - 2; Data1C > 0; --Data1C, ++Data20) {

        if (Data20->mPosX == -32768)
            continue;

        (*this.*mSprite_Function[Data20->mSpriteType])(Data20);
    }
}

void cFodder::Sprite_Handle_Null(sSprite* pSprite) {

    switch (pSprite->mSpriteType) {
    case 4:
        return;

    default:
        std::cout << "Sprite Function not implemented: " << pSprite->mSpriteType << "\n";
        break;
    }

}

void cFodder::Sprite_Handle_Shrub(sSprite* pSprite) {
    pSprite->mSheetIndex = 0x8F;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Tree(sSprite* pSprite) {
    pSprite->mSheetIndex = 0x90;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_BuildingRoof(sSprite* pSprite) {
    pSprite->mSheetIndex = 0x91;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Snowman(sSprite* pSprite) {
    pSprite->mSheetIndex = 0x92;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Shrub2(sSprite* pSprite) {
    pSprite->mSheetIndex = 0x93;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Waterfall(sSprite* pSprite) {
    pSprite->mSheetIndex = 0x94;
    pSprite->mDrawOrder = eSprite_Draw_First;

    int16 Data0 = pSprite->field_12;
    Data0 ^= 1;
    Data0 &= 1;
    pSprite->field_12 = Data0;

    if (Data0)
        return;

    pSprite->mFrameIndex += 1;
    if (pSprite->mFrameIndex < 3)
        return;

    pSprite->mFrameIndex = 0;
}

void cFodder::Sprite_Handle_BuildingDoor(sSprite* pSprite) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (Sprite_Handle_BuildingDoor_Explode(pSprite))
        return;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->mSheetIndex = 0x99;
        return;
    }

    //loc_1ACEC
    pSprite->mDrawOrder = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_1AD86;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A) {

        Sprite_Create_Enemy(pSprite, Data2C);
        return;
    }

    if (pSprite->field_43 >= 0) {
        pSprite->mSheetIndex = 0x7C;
        return;
    }

    pSprite->mSheetIndex = 0x99;
    Sound_Play(pSprite, eSound_Effect_BuildingDoor2, 0x01);
    Data0 = tool_RandomGet() & 0x0F;

    Data4 = 0x14 - mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
    if (Data4 < 0)
        Data4 = 0;

    Data4 <<= 3;

    Data4 += Data0;
    pSprite->field_12 = Data4;

loc_1AD86:;
    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->mSheetIndex = 0x7C;

    if (!Sprite_Create_Enemy(pSprite, Data2C))
        return;

    pSprite->field_12 = -1;
    pSprite->field_43 = -1;
}

void cFodder::Sprite_Handle_BuildingDoor2(sSprite* pSprite) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (Sprite_Update_Die2_Explosion_IceAware(pSprite))
        return;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->mSheetIndex = 0x9B;
        return;
    }

    pSprite->mDrawOrder = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_1B285;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14)
        goto loc_1B2B6;

    if (pSprite->field_43 == 0x0A)
        goto loc_1B2B6;

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

loc_1B285:;
    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->mSheetIndex = 0x7C;

loc_1B2B6:;

    if (!Sprite_Create_Enemy(pSprite, Data2C)) {
        Data2C->mPosY -= 4;
        return;
    }

    pSprite->field_12 = 0;
    pSprite->field_43 = -1;
}

void cFodder::Sprite_Handle_BuildingDoor3(sSprite* pSprite) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (Sprite_Update_Die2_Explosion(pSprite))
        return;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->mSheetIndex = 0xE0;
        return;
    }

    pSprite->mDrawOrder = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_1D8DC;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A)
        goto loc_1D928;

    if (pSprite->field_43 >= 0) {
        pSprite->mSheetIndex = 0x7C;
        return;
    }

    pSprite->mSheetIndex = 0xE0;
    Sound_Play(pSprite, eSound_Effect_BuildingDoor2, 1);

    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14 - mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;

    if (Data4 < 0)
        Data0 = 0;

    Data4 <<= 3;
    Data4 += Data0;
    pSprite->field_12 = Data4;

loc_1D8DC:;

    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->mSheetIndex = 0x7C;

    if (!Sprite_Create_Enemy(pSprite, Data2C))
        return;

    pSprite->field_12 = -1;
    pSprite->field_43 = -1;
    return;

loc_1D928:;
    Sprite_Create_Enemy(pSprite, Data2C);
}

void cFodder::Sprite_Handle_BuildingDoor_Reinforced(sSprite* pSprite) {
    if (pSprite->mAnimState != eSprite_Anim_Die3) {
        Sprite_Handle_BuildingDoor3(pSprite);
        return;
    }

    if (pSprite->mHeliRestartFrame >= 0x69) {
        Sprite_Handle_BuildingDoor3(pSprite);
        return;
    }

    pSprite->mHeliRestartFrame += 1;
    pSprite->mAnimState = eSprite_Anim_None;
    Sprite_Handle_BuildingDoor3(pSprite);
}

void cFodder::Sprite_Handle_Computer_1(sSprite* pSprite) {

    Sprite_Handle_Computer(pSprite, 0x46);
}

void cFodder::Sprite_Handle_Computer_2(sSprite* pSprite) {

    Sprite_Handle_Computer(pSprite, 0x69);
}

void cFodder::Sprite_Handle_Computer_3(sSprite* pSprite) {

    Sprite_Handle_Computer(pSprite, 0xAF);
}

void cFodder::Sprite_PositionNext_AdjustByHeight(sSprite* pSprite) {
    sSprite* Data24 = pSprite + 1;

    Data24->mPosX = pSprite->mPosX + 8;
    Data24->mPosY = pSprite->mPosY;

    Data24->mPosX += (pSprite->mHeight >> 2);
    Data24->mPosY += (pSprite->mHeight >> 2);
    Data24->mDrawOrder = eSprite_Draw_First;
}

void cFodder::Sprite_XY_Store(sSprite* pSprite) {
    mSprite_Reached_Target = 0;

    mStoredSpriteX = (pSprite->mPosX & 0xFFFF) | (pSprite->mPosXFrac << 16);
    mStoredSpriteY = (pSprite->mPosY & 0xFFFF) | (pSprite->mPosYFrac << 16);
}

void cFodder::Sprite_XY_Restore(sSprite* pSprite) {
    mSprite_Reached_Target = -1;

    pSprite->mPosX = mStoredSpriteX & 0xFFFF;
    pSprite->mPosXFrac = mStoredSpriteX >> 16;

    pSprite->mPosY = mStoredSpriteY & 0xFFFF;
    pSprite->mPosYFrac = mStoredSpriteY >> 16;
}

void cFodder::Sprite_Draw_Row_Update(sSprite* pSprite) {
    int16 Data4 = 0;

    // Sprite still moving?
    if (!Sprite_Reached_Target(pSprite)) {
        Sprite_Movement_Speed_Update(pSprite);
        Sprite_Movement_Calculate(pSprite);
    }

    Squad_Join_Check(pSprite);
    Sprite_Reached_MapEdge(pSprite);

    // Check the terrain
    int8 PreviousTileType = pSprite->mTerrainType;
    Sprite_Terrain_Check(pSprite, Data4);

    // Not leaving water?
    if (PreviousTileType != eTerrainFeature_Water)
        goto loc_1F75D;

    // Leaving water and into Quick sand?
    if (pSprite->mTerrainType == eTerrainFeature_QuickSand)
        goto loc_1F753;

    // Leaving water for the edge/bank
    if (pSprite->mTerrainType != eTerrainFeature_WaterEdge)
        goto loc_1F75D;

    pSprite->mRowsToSkip = 5;
    goto loc_1F75D;

loc_1F753:;
    pSprite->mRowsToSkip = 3;

loc_1F75D:;
    Sprite_Handle_Player_Close_To_SquadMember(pSprite);
}

void cFodder::Sprite_Terrain_Check(sSprite* pSprite, int16& pData4) {
    int16 Data0 = -3;   // Y
    pData4 = 8; // X

    // pData4 becomes terrain type
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        goto CheckFalling;

    pSprite->mTerrainType = pData4 & 0xFF;
    pSprite->field_50 = 0;
    pSprite->mInWater = 0;

    if (pData4 == eTerrainFeature_Drop2) {
        if (pSprite->mSpriteType == eSprite_Enemy)
            goto loc_20251;

        if (pSprite->mPersonType == eSprite_PersonType_Native)
            goto loc_20251;

        if (!pSprite->mAnimState || pSprite->mAnimState >= eSprite_Anim_Slide1) {
            pSprite->mAnimState = eSprite_Anim_Hit3;
            return;
        }
    }

    //loc_20044
    if (pData4 == eTerrainFeature_Drop) {

        if (pSprite->mSpriteType == eSprite_Enemy)
            goto loc_20251;

        if (pSprite->mPersonType == eSprite_PersonType_Native)
            goto loc_20251;

        ++pSprite->mDelayCounter;
        return;
    }

    //loc_20072
    pSprite->mDelayCounter = 0;
    if (pData4 == eTerrainFeature_QuickSandEdge)
        goto loc_201CC;

    if (pData4 == eTerrainFeature_Rocky)
        goto loc_20108;

    if (pData4 == eTerrainFeature_Rocky2)
        goto loc_2014D;

    if (!pSprite->mRockyHeightActive)
        goto loc_200C0;

    pSprite->mHeight -= 3;
    if (!pSprite->mHeight)
        goto loc_200B7;
    if (pSprite->mHeight >= 0)
        goto loc_200C0;

    pSprite->mHeight = 0;
loc_200B7:;
    pSprite->mRockyHeightActive = 0;

loc_200C0:;
    if (pData4 == eTerrainFeature_QuickSand)
        goto Soldier_InQuickSand;

    if (pData4 == eTerrainFeature_WaterEdge)
        goto Soldier_WaterEdge;

    if (pData4 != eTerrainFeature_Water)
        goto checkSinking;

    // Is a Native?
    if (pSprite->mPersonType == eSprite_PersonType_Native)
        goto loc_20251;

    // In Water
    pSprite->mInWater = -1;
    pSprite->field_50 = 0;
    pSprite->mRowsToSkip = 0;
    return;

loc_20108:;
    pSprite->mRowsToSkip = 0;
    if (pSprite->mRockyHeightActive) {
        pSprite->mHeight = 2;
        return;
    }
    if (!pSprite->mHeight) {
        pSprite->mRockyHeightActive = -1;
        pSprite->mHeight = 1;
    }
    return;

loc_2014D:;
    pSprite->mRowsToSkip = 0;
    if (pSprite->mRockyHeightActive) {
        if (pSprite->mHeight >= 6)
            return;

        pSprite->mHeight += 2;
        return;
    }
    //loc_2017C
    if (pSprite->mHeight)
        return;
    pSprite->mRockyHeightActive = -1;
    pSprite->mHeight = 1;
    return;

Soldier_InQuickSand:;
    pSprite->field_50 = 3;
    return;

Soldier_WaterEdge:;
    pSprite->field_50 = 6;
    return;

checkSinking:;
    if (pData4 == eTerrainFeature_Sink)
        goto HumanSinking;

    pSprite->mRowsToSkip = 0;
    return;

loc_201CC:;
    if (pSprite->mAnimState)
        return;
    if (!pSprite->mSpeed)
        return;
    if (pSprite->mRowsToSkip)
        return;
    if (pSprite->mHeight)
        return;

    Data0 = tool_RandomGet() & 0x3F;
    if (Data0)
        return;

    if (pSprite->mPersonType == eSprite_PersonType_Native)
        return;

    pSprite->mAnimState = eSprite_Anim_Slide1;
    return;

HumanSinking:;
    pSprite->mIsSinking = 1;
    return;

CheckFalling:;
    if (pData4 == eTerrainFeature_Drop || pData4 == eTerrainFeature_Drop2) {
        pSprite->mAnimState = eSprite_Anim_Hit2;
        return;
    }

loc_20251:;
    pSprite->field_43 = 1;
    mSprite_Reached_Target = -1;
    if (pSprite->mPersonType == eSprite_PersonType_Human)
        goto loc_2035C;

    pSprite->mPosX = mStoredSpriteX & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        goto loc_202E5;

    if (pData4 == eTerrainFeature_Drop)
        goto loc_202E5;

    if (pData4 == eTerrainFeature_Drop2)
        goto loc_202E5;

    if (pSprite->mPersonType == eSprite_PersonType_Native && pData4 == eTerrainFeature_Water)
        goto loc_202E5;

    Data0 = pSprite->mDirection;
    if (Data0 <= 0x80 || Data0 > 0x180) {
        pSprite->mDirection = 0;
        goto loc_20307;
    }

    pSprite->mDirection = 0x100;
    goto loc_20307;

loc_202E5:;
    if (pSprite->mDirection > 0x100) {
        pSprite->mDirection = 0x180;
        goto loc_20307;
    }

    pSprite->mDirection = 0x80;
loc_20307:;

    Data0 = mInterruptTick;
    Data0 >>= 2;
    Data0 &= 0x3F;
    Data0 -= 0x20;
    Data0 &= 0x1FE;
    pSprite->mDirection += Data0;
    pSprite->mPosY = mStoredSpriteY;

    Data0 = mInterruptTick;
    Data0 &= 0x1F;
    Data0 += 0x0C;
    pSprite->field_44 = (int8)Data0;
    return;

loc_2035C:;

    int16 word_3A9E6 = pSprite->mPosX;
    pSprite->mPosX = mStoredSpriteX & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        return;

    pSprite->mPosX = word_3A9E6;
    pSprite->mPosY = mStoredSpriteY & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        return;

    //loc_203BA
    pSprite->mFrameIndex = mTmp_FrameNumber;
    Sprite_XY_Restore(pSprite);
    pSprite->mTargetX = pSprite->mPosX;
    pSprite->mTargetY = pSprite->mPosY;

    sMapTarget* Data30 = mSquad_WalkTargets[pSprite->field_32];
    pData4 = 0;

    for (;;) {
        sMapTarget eax = *Data30++;

        if (eax.asInt < 0)
            break;

        pData4++;
    }

    pSprite->mNextWalkTargetIndex = pData4;
    pSprite->mRetargetCooldown = -1;
}

void cFodder::Sprite_Reached_MapEdge(sSprite* pSprite) {

    if (pSprite->mPosY < 0 || pSprite->mPosY <= 13) {
        pSprite->mPosY = mStoredSpriteY;
        mSprite_Reached_Target = -1;
    }

    if (pSprite->mPosY >= mMapLoaded->getHeightPixels()) {
        if (pSprite->mAnimState == eSprite_Anim_None || pSprite->mAnimState >= eSprite_Anim_Slide1) {
            pSprite->mPosY = mStoredSpriteY;
            mSprite_Reached_Target = -1;
        }
    }
    //loc_204E1
    if (pSprite->mPosX <= 0) {
        if (mStoredSpriteX <= 0)
            return;

        goto loc_20521;
    }

    if (pSprite->mPosX + 12 < mMapLoaded->getWidthPixels())
        return;

    if (mStoredSpriteX + 16 >= mMapLoaded->getWidthPixels())
        return;

loc_20521:;
    pSprite->mPosX = mStoredSpriteX;
    mSprite_Reached_Target = -1;
}

int16 cFodder::Sprite_Destroy_Wrapper(sSprite* pSprite) {

    return Sprite_Destroy(pSprite);
}

int16 cFodder::Sprite_Destroy(sSprite* pSprite) {

    pSprite->mVehicleEnabled = 0;
    pSprite->mSheetIndex = 0x7C;
    pSprite->mSpriteType = eSprite_Null;
    pSprite->mFrameIndex = 0;
    pSprite->mPendingDestroy = 1;

    return 1;
}

void cFodder::Sprite_Set_Direction_To_Next(sSprite* pSprite) {
    int16 Data0 = pSprite->mDirection;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->mFaceDirection = Data0;

    mDirectionMod = 0;
}

int16 cFodder::Sprite_Reached_Target(sSprite* pSprite) {

    // Sprite is moving?
    if (pSprite->mSpeed <= 8)
        return 0;

    int16 Data0 = pSprite->mTargetX;
    int16 Data4 = pSprite->mTargetY;

    int16 Data8 = pSprite->mPosX;
    int16 DataC = pSprite->mPosY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 2)
        return 0;

    pSprite->mPosX = pSprite->mTargetX;
    pSprite->mPosY = pSprite->mTargetY;

    mSprite_Reached_Target = -1;
    return -1;
}

void cFodder::Sprite_Movement_Speed_Update(sSprite* pSprite) {
    int16 Data0 = pSprite->mDirection;

    if (pSprite->mTerrainType != eTerrainFeature_D)
        goto loc_20FBB;

    //seg005:1843

    if (Data0 < 0xD0)
        goto loc_20F63;

    if (Data0 < 0x130)
        goto loc_20FD1;

    if (Data0 <= 0x170)
        goto loc_20FD9;

    if (Data0 <= 0x190)
        return;

    if (Data0 <= 0x1C0)
        goto loc_20F9B;

    goto loc_20F7B;

loc_20F63:;
    if (Data0 >= 0x90)
        goto loc_20FD9;

    if (Data0 >= 0x70)
        return;

    if (Data0 >= 0x40)
        goto loc_20F9B;

loc_20F7B:;
    Data0 = pSprite->mSpeed;
    Data0 >>= 2;
    pSprite->mSpeed += Data0;
    return;

loc_20F9B:;
    Data0 = pSprite->mSpeed;
    Data0 >>= 3;
    pSprite->mSpeed += Data0;
    return;

loc_20FBB:;
    if (pSprite->mTerrainType == eTerrainFeature_Rocky)
        goto loc_20FD1;

    if (pSprite->mTerrainType != eTerrainFeature_Rocky2)
        goto loc_20FE3;

loc_20FD1:;
    pSprite->mSpeed >>= 1;

loc_20FD9:;
    pSprite->mSpeed >>= 1;
    return;
loc_20FE3:;
    if (pSprite->mTerrainType != eTerrainFeature_Snow)
        return;

    Data0 = pSprite->mPosX;
    Data0 += pSprite->mPosY;
    Data0 &= 3;
    if (Data0 == 0)
        return;
    Data0 = pSprite->mSpeed;
    Data0 >>= 2;
    pSprite->mSpeed -= Data0;
}

int16 cFodder::Sprite_Get_Free_Max42(int16& pData0, sSprite*& pData2C, sSprite*& pData30) {

    // 
    if (!mSprite_SpareUsed) {

        // Looking for two sprites?
        if (pData0 == 3) {
            pData2C = mSprites.data();

            // Loop all sprites
             for (int32_t Data1C = mParams->getSpritesMax() - 5; Data1C >= 0; --Data1C, ++pData2C) {

                // Sprite free?
                if (pData2C->mPosX != -32768)
                    continue;

                if ((pData2C + 1)->mPosX != -32768)
                    continue;

                // Second sprite free?
                if ((pData2C + 2)->mPosX == -32768) {
                    pData30 = pData2C + 1;

                    pData2C->Clear();
					pData30->Clear();
					(pData30 + 1)->Clear();	// Yuck

                    pData0 = 0;
                    return 0;
                }
            }
        } else if (pData0 == 2) {
            pData2C = mSprites.data();

            // Loop all sprites
             for (int32_t Data1C = mParams->getSpritesMax() - 4; Data1C >= 0; --Data1C, ++pData2C) {

                // Sprite free?
                if (pData2C->mPosX != -32768)
                    continue;

                // Second sprite free?
                if ((pData2C + 1)->mPosX == -32768) {
                    pData30 = pData2C + 1;

                    pData2C->Clear();
					pData30->Clear();
                    pData0 = 0;
                    return 0;
                }
            }
        }
        else {
            // Only looking for 1 sprite
            pData2C = &mSprites[mParams->getSpritesMax() - 3];

            for (int32 Data1C = mParams->getSpritesMax() - 3; Data1C >= 0; --Data1C) {

                // Free?
                if (pData2C->mPosX == -32768) {
                    pData2C->Clear();
                    pData0 = 0;
                    return 0;
                }

                --pData2C;
            }
        }
    }

    //loc_211F0
    pData2C = &mSprite_Spare;
    pData30 = &mSprite_Spare;
    pData0 = -1;
    mSprite_SpareUsed = pData0;
    return -1;
}

int16 cFodder::Sprite_Get_Free_Max29(int16& pData0, sSprite*& pData2C, sSprite*& pData30) {
    if (mSprite_SpareUsed2)
        goto loc_21B4B;

    if (pData0 == 2)
        goto loc_21B91;

    pData2C = &mSprites[mParams->getSpritesMax() - 16];
     for (int32_t Data1C = mParams->getSpritesMax() - 16; Data1C >= 0; --Data1C, --pData2C) {

        if (pData2C->mPosX == -32768) {
            pData2C->Clear();
            pData0 = 0;
            return 0;
        }
    }

loc_21B4B:;
    pData2C = &mSprite_Spare;
    pData30 = &mSprite_Spare;
    pData0 = -1;
    mSprite_SpareUsed2 = pData0;
    return -1;

loc_21B91:;
    pData2C = mSprites.data();

     for (int32_t Data1C = mParams->getSpritesMax() - 17; Data1C >= 0; --Data1C, ++pData2C) {

        if (pData2C->mPosX != -32768)
            continue;

        if ((pData2C + 1)->mPosX == -32768) {
            pData30 = pData2C + 1;
            pData0 = 0;
        }
    }

    pData2C = &mSprite_Spare;
    pData30 = &mSprite_Spare;
    pData0 = -1;
    mSprite_SpareUsed2 = pData0;
    return -1;
}

void cFodder::Sprite_Handle_Exploidable(sSprite* pSprite) {
    int64 Dat0;
    int32 Dat4;
    int16 Data0, Data4;

    if (pSprite->mIsSinking)
        goto loc_2132A;

    if (pSprite->mHeight)
        goto loc_2132A;

    if (pSprite->mAnimState == eSprite_Anim_Die1)
        goto loc_213F7;

    if (pSprite->mAnimState > eSprite_Anim_Die1)
        goto loc_2132A;

    pSprite->mIsSinking = 0;
    return;

loc_2132A:;

    Sprite_Movement_Calculate(pSprite);

    Dat0 = (int64)pSprite->mFixedPoint;
    Dat4 = (pSprite->mHeightFrac & 0xFFFF) | (pSprite->mHeight << 16);

    Dat0 -= 0x28000;
    Dat4 += (int32)Dat0;

    if (Dat4 < 0) {
        Dat4 = 0;
        Dat0 = -Dat0;
        Dat0 >>= 2;
    }

    pSprite->mFixedPoint = (int32)Dat0;
    pSprite->mHeightFixed = Dat4;

    if (!(Dat4 >> 16)) {
        if (!(Dat0 >> 16))
            goto loc_21464;
    }

    Data0 = -3;
    Data4 = 0x0C;

    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);

    if (pSprite->mHeight)
        return;

    if (Data4 == eTerrainFeature_WaterEdge)
        goto loc_21464;

    if (Data4 == eTerrainFeature_Water)
        goto loc_21464;

    if (Data4 == eTerrainFeature_Drop)
        goto loc_21464;

    if (Data4 == eTerrainFeature_Drop2)
        goto loc_21464;

    return;

loc_213F7:;

    if (mPhase_Completed_Timer)
        return;

    pSprite->mAnimState = eSprite_Anim_Die2;
    pSprite->mIsSinking = -1;
    pSprite->mHeightFrac = 0x0000;
    pSprite->mHeight = 0x1;

    Data0 = tool_RandomGet() & 0x1FE;
    pSprite->mDirection = Data0;

    Data0 = tool_RandomGet() & 0x1F;
    Data0 += 0x14;

    pSprite->mSpeed = Data0;
    pSprite->mFixedPoint = 0x1C0000;
    Sprite_Create_Shadow(pSprite);
    return;

loc_21464:;
    pSprite->mSpriteType = eSprite_Explosion;
    pSprite->mTargetX = 0x5F5F;
    pSprite->mTargetY = 8;
}

int16 cFodder::Sprite_Handle_BuildingDoor_Explode(sSprite* pSprite) {
    int16 Data0, Data4;

    if (pSprite->mAnimState == eSprite_Anim_Die3 || pSprite->mAnimState == eSprite_Anim_Die1)
        goto loc_221D0;

    if (pSprite->mAnimState == eSprite_Anim_Die2)
        goto loc_22235;

    return 0;
loc_221D0:; // Building Explodes
    pSprite->mAnimState = eSprite_Anim_Die2;
    pSprite->mSheetIndex = 0x99;

    Data0 = tool_RandomGet();
    Data4 = Data0;

    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pSprite->mDirection = Data0;
    Data4 &= 3;
    Data4 += 5;
    pSprite->field_44 = (int8)Data4;
    pSprite->mSpeed = 0x78;

loc_22235:; // Door moving
    pSprite->field_44 -= 1;
    if (!pSprite->field_44)
        goto loc_2227F;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_2227F;

    Data0 = -3;
    Data4 = 2;
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_2227F;

    if (pSprite->mSpeed <= 0)
        goto loc_2227F;

    return -1;

loc_2227F:; // Door Finished
    pSprite->mPosX -= 8;
    pSprite->mPosY -= 8;

    Sprite_Turn_Into_Building_Explosion(pSprite);

    return -1;
}

int16 cFodder::Sprite_Homing_LockInRange(sSprite* pSprite, sSprite*& pFoundSprite) {

    int16 MouseX = mMouseX;
    int16 MouseY = mMouseY;

    MouseX += mCameraX >> 16;
    MouseY += mCameraY >> 16;

    MouseX -= 0x10;
    if (!MouseX)
        MouseX = 1;

    MouseY += 0x08;

    for (auto& Sprite : mSprites) {
        pFoundSprite = &Sprite;

        if (pFoundSprite->mPosX == -32768)
            continue;

        if (!mSprite_Missile_CanLock[pFoundSprite->mSpriteType])
            continue;

        if (pFoundSprite->mPersonType == pSprite->mPersonType)
            continue;

        int16 Data0 = pFoundSprite->mPosX + 5;
        int16 Data4 = pFoundSprite->mPosY - 5;

        Data4 -= pFoundSprite->mHeight;
        int16 Distance = 0x1F;
        int16 S_Data8 = MouseX;
        int16 S_DataC = MouseY;

        Map_Get_Distance_BetweenPoints(Data0, Data4, MouseX, Distance, MouseY);

        MouseX = S_Data8;
        MouseY = S_DataC;

        if (Data0 < 0x16) {
            Data0 = -1;
            return -1;
        }
    }

    return 0;
}

void cFodder::Sprite_Destroy_Wrapper_At_TopLeft(sSprite* pSprite) {
    pSprite->mPosX = 1;
    pSprite->mPosY = 1;

    Sprite_Destroy_Wrapper_2(pSprite);
}

int16 cFodder::Sprite_Destroy_Wrapper_2(sSprite* pSprite) {
    return Sprite_Destroy_Wrapper(pSprite);
}


void cFodder::Sprite_Update_Follower_Target(sSprite* pSprite) {
    sSprite* Data30 = 0;

    mTroop_Weapon_Bullet_Disabled = false;
    mTroop_Weapon_Grenade_Disabled = true;
    mTroop_Weapon_Rocket_Disabled = true;
    mDirectionMod = 0;
    mSprite_FaceWeaponTarget = -1;

    int16 Dataa0, Data4, Data8, DataC, Data10;

    sSprite* Data0, *Following = 0, *Squad0_Member = 0;

    if (pSprite->field_5E_Squad < 0) {

        pSprite->field_5E_Squad += 1;
        mSprite_FaceWeaponTarget = 0;

        Data0 = mSquad_Leader;
        if (mSquad_Leader == INVALID_SPRITE_PTR)
            goto loc_22125;

    }
    else {

        Squad0_Member = mSquad_0_Sprites[pSprite->field_5E_Squad];
        if (Squad0_Member == INVALID_SPRITE_PTR)
            goto loc_21E4A;

        if ((tool_RandomGet() & 0x3F) == 0)
            goto loc_21E4A;

        if (Squad0_Member->mPosX == -32768)
            goto loc_21E4A;

        if (Squad0_Member->mSpriteType != eSprite_Player)
            goto loc_21E4A;

        if (Squad0_Member->mAnimState < eSprite_Anim_Slide1) {
            if (Squad0_Member->mAnimState)
                goto loc_21E4A;
        }

        if (Squad0_Member->mHeight >= 0x0D)
            goto loc_21E4A;

        Dataa0 = pSprite->mPosX;
        Data4 = pSprite->mPosY;

        Data8 = Squad0_Member->mPosX;
        DataC = Squad0_Member->mPosY;

        Map_Get_Distance_BetweenPoints_Within_Window(Dataa0, Data4, Data8, DataC);

        mSprite_Find_Distance = Dataa0;
        if (Dataa0 > 0xC8)
            goto loc_21E4A;

        if (Dataa0 <= 0x28)
            goto loc_21F77;

        Dataa0 = pSprite->mPosX;
        Data4 = pSprite->mPosY;
        Data8 = Squad0_Member->mPosX;
        DataC = Squad0_Member->mPosY;

        if (!Map_PathCheck_CalculateTo(Dataa0, Data4, Data8, DataC)) {
            Dataa0 = mSprite_Find_Distance;
            goto loc_21F77;
        }

        if (mSprite_Find_Distance < 0x40)
            goto loc_21F8A;

    loc_21E4A:;

        // Following a Sprite?
        if (pSprite->mFollowTarget) {

            Following = pSprite->mFollowTarget;

            if (Following->mPosX != -32768) {

                // Following a hostage?
                if (Following->mSpriteType == eSprite_Hostage) {

                    pSprite->field_5E_Squad += 1;
                    if (pSprite->field_5E_Squad >= 0x1E)
                        pSprite->field_5E_Squad = 0;

                    mSprite_FaceWeaponTarget = 0;
                    pSprite->mTargetX = Following->mPosX + 0x0C;
                    pSprite->mTargetY = Following->mPosY;
                    return;
                }
            }

            pSprite->mFollowTarget = 0;
        }

        if (!(tool_RandomGet() & 0x0F)) {
            Dataa0 = tool_RandomGet() & 0xFF;
            Dataa0 = (int16)Dataa0;
            pSprite->mWeaponTargetX += Dataa0;

            Dataa0 = tool_RandomGet() & 0xFF;
            pSprite->mWeaponTargetY += Dataa0;
        }

        pSprite->mTargetX = pSprite->mPosX;
        pSprite->mTargetY = pSprite->mPosY;
        pSprite->mWeaponFireTimer = 0;

        pSprite->field_5E_Squad += 1;
        if (pSprite->field_5E_Squad > 0x1E)
            pSprite->field_5E_Squad = 0;

        goto loc_22000;

    loc_21F77:;
        pSprite->mFollowTarget = 0;
        mSprite_FaceWeaponTarget = 0;

    loc_21F8A:;
        if (Dataa0 > 0x87)
            goto loc_22000;

        //seg005:28A2
        // This is a strange segment, I can't reliably tell where data30 comes from,
        // in atleast one situation, its possible its not even pointing at an sSprite structure
        // or other times it might be pointing towards a completely irrelevant sprite from a previous use case.
        // i set it to spare, because that *could* happen if we are out of sprites and a previous sprite create had
        // failed... 
        Data30 = &mSprite_Spare;

        // Logic wise, it doesnt really make sense either... which makes me think perhaps Data30 should be Squad0_Member


        if (Data30->mPosX != Data30->mTargetX)
            goto loc_22000;

        if (Data30->mPosY != Data30->mTargetY)
            goto loc_22000;

        pSprite->mWeaponTargetX = Squad0_Member->mPosX;
        pSprite->mWeaponTargetY += 7;                         // TODO: Is this an original bug and we should be adding to weaponTargetX, code/logic wise it makes sense if it is

        pSprite->mWeaponTargetY = Squad0_Member->mPosY;
        pSprite->mWeaponTargetY -= 0x0E;
        Sprite_Reset_Move_Timer(pSprite);


    loc_22000:;
        if (mSprite_FaceWeaponTarget)
            goto loc_22125;

        if (mSquad_0_Sprites[pSprite->field_5E_Squad] == INVALID_SPRITE_PTR)
            goto loc_22125;

        Data0 = mSquad_0_Sprites[pSprite->field_5E_Squad];
    }

    // "Sort of" Random Movement Target
    // Depending on the sprite index, we add a factor to the X/Y target of a movement
    Data8 = (int16)(pSprite - mSprites.data());
    Data8 *= 0x76;
    Data8 &= 0x1FE;

    DataC = mDirectionVectorTable[Data8 / 2];
    Data8 += 0x80;
    Data8 &= 0x1FE;
    Data10 = mDirectionVectorTable[Data8 / 2];

    //seg005:29B2
    DataC >>= 8;
    Data10 >>= 8;
    DataC >>= 2;
    Data10 >>= 2;

    pSprite->mWeaponTargetX = Data0->mPosX;
    pSprite->mTargetX = Data0->mPosX + DataC;      // Target X

    pSprite->mWeaponTargetY = Data0->mPosY;
    pSprite->mTargetY = Data0->mPosY + Data10;      // Target Y

loc_22125:;
    Sprite_Reset_Move_Timer(pSprite);
}

void cFodder::Sprite_Reset_Move_Timer(sSprite* pSprite) {

    if (pSprite->mWeaponFireTimer)
        return;

    int16 Data0 = tool_RandomGet();

    int16 Data8 = pSprite->mAIAggression;
    int16 DataC = Data8;
    DataC <<= 2;
    Data8 >>= 2;
    int16 Data4 = 0x1F;
    Data4 >>= Data8;
    Data0 &= Data4;
    Data4 = 0x32;
    Data4 -= DataC;
    if (Data4 < 0)
        Data4 = 0;

    Data0 += Data4;
    if (Data0 < 5)
        Data0 = 5;

    pSprite->mWeaponFireTimer = Data0;
}


int16 cFodder::Sprite_Update_Die2_Explosion_IceAware(sSprite* pSprite) {
    int16 Data0, Data4;

    if (pSprite->mAnimState != eSprite_Anim_Die3 && pSprite->mAnimState != eSprite_Anim_Die1) {

        if (pSprite->mAnimState == eSprite_Anim_Die2)
            goto loc_22339;

        return 0;
    }

    //loc_222CD
    pSprite->mAnimState = eSprite_Anim_Die2;
    pSprite->mSheetIndex = 0x9B;

    Data0 = tool_RandomGet();
    Data4 = Data0;
    Data0 &= 0x1E;

    if (Data4 < 0)
        Data0 = -Data0;

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Data0 += 0x1C0;

    pSprite->mDirection = Data0;
    Data4 &= 3;
    Data4 += 5;

    pSprite->field_44 = (int8)Data4;
    pSprite->mSpeed = 0x6E;

loc_22339:;
    pSprite->field_44 -= 1;
    if (pSprite->field_44 == 1)
        goto loc_22383;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_22383;

    Data0 = -3;
    Data4 = 2;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        if (pSprite->mSpeed > 0)
            return -1;

loc_22383:;

    pSprite->mPosX -= 8;
    if (pSprite->mPosX < 0)
        pSprite->mPosX = 0;

    pSprite->mPosY -= 8;
    Sprite_Turn_Into_Building_Explosion(pSprite);
    return -1;
}

int16 cFodder::Sprite_Update_Die2_Explosion(sSprite* pSprite) {
    int16 Data0, Data4;

    if (pSprite->mAnimState == eSprite_Anim_Die3)
        goto loc_1D95B;

    if (pSprite->mAnimState == eSprite_Anim_Die2)
        goto loc_1D9C9;

    if (pSprite->mHasShadow)
        goto loc_1D9C9;

    return 0;

loc_1D95B:;
    pSprite->mAnimState = eSprite_Anim_Die2;
    pSprite->mSheetIndex = 0xE0;

    Data0 = tool_RandomGet();
    Data4 = Data0;
    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pSprite->mDirection = Data0;

    Data4 &= 3;
    Data4 += 5;

    pSprite->field_44 = (int8)Data4;
    pSprite->mSpeed = 0x78;
    pSprite->mHasShadow = -1;

loc_1D9C9:;
    pSprite->field_44 -= 1;
    if (!pSprite->field_44)
        goto loc_1DA13;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1DA13;

    Data0 = -3;
    Data4 = 2;
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_1DA13;

    if (pSprite->mSpeed <= 0)
        goto loc_1DA13;

    return -1;

loc_1DA13:;
    pSprite->mPosX -= 8;
    if (pSprite->mPosX < 0)
        pSprite->mPosX = 0;

    pSprite->mPosY -= 8;
    Sprite_Turn_Into_Building_Explosion(pSprite);
    return -1;
}

void cFodder::Sprite_Update_Direction_Anim(sSprite* pSprite) {
    int16 Data8 = pSprite->mFaceDirection;

    Data8 += mDirectionMod;
    Data8 &= 0x0E;

    pSprite->mFaceDirection = Data8;

    const int16* Data28 = mSprite_AnimationPtrs[pSprite->mPersonType];
    Data28 += ((0x80 + Data8) / 2);

    pSprite->mSheetIndex = *Data28;
    pSprite->field_3E = pSprite->mDirection;
    pSprite->mFrameIndex = 0;
}
