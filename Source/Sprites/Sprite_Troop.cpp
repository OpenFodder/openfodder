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

void cFodder::Sprite_Enemy_Set_Target(sSprite* pData2C) {

    int16 Data0 = tool_RandomGet();
    int16 Data4 = Data0;

    // Bigger X movement range for low aggression
    if (mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage < 5)
        Data0 &= 0x3F;
    else
        Data0 &= 0x1F;

    if (Data4 < 0)
        Data0 = -Data0;

    pData2C->field_26 += Data0;

    Data0 = tool_RandomGet();
    Data4 = Data0;

    // Bigger Y movement range for low aggression
    if (mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage < 5)
        Data0 &= 0x3F;
    else
        Data0 &= 0x1F;
    if (Data4 < 0)
        Data0 = -Data0;

    pData2C->field_28 += Data0;
}

void cFodder::Sprite_Handle_Player_DestroyAll() {

    for(auto& Sprite : mSprites) {

        if (Sprite.field_0 == -32768)
            continue;

        if (Sprite.field_18 == eSprite_Player)
            Sprite_Troop_Dies(&Sprite);
    }
}

void cFodder::Sprite_Handle_Player_Destroy_Unk() {

    for (auto& Sprite : mSprites) {

        if (Sprite.field_0 == -32768)
            continue;

        if (Sprite.field_18 != eSprite_Player)
            continue;

        if (!Sprite.field_38)
            continue;

        if (Sprite.field_38 >= eSprite_Anim_Slide1)
            continue;

        Sprite_Troop_Dies(&Sprite);
    }
}

/**
 * Set the aggression of a sprite, then calculate the next aggression rate
 *
 * This moves by 'Increment' in an up or down
 * direction inside the min/max range for this map
 */
void cFodder::Sprite_Handle_Enemy_Aggression_Set(sSprite* pSprite) {

    int16 Next = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext;
    pSprite->field_62 = Next;

    int16 Increment = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement;
    Next += Increment;

    if (Increment >= 0) {

        if (Next >= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax) {
            Next = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
            Increment = -Increment;
        }
    }
    else {
        if (Next < 0 || Next <= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin) {
            Next = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin;
            Increment = -Increment;
        }
    }
    //loc_2D90D
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement = Increment;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext = Next;
}

void cFodder::Sprite_Handle_Player(sSprite *pSprite) {
    int16 Data0, Data4, eax;
    sSprite* Data28 = 0;

    // player soldier in vehicle?
    if (pSprite->field_6E) {
        Sprite_Handle_Player_InVehicle(pSprite);
        return;
    }

    // If player is invincible
    if (pSprite->field_75 & eSprite_Flag_Invincibility) {

        // And dying
        if (pSprite->field_38 <= eSprite_Anim_Slide1)
            pSprite->field_38 = eSprite_Anim_None;

        // No longer Sinking
        pSprite->field_5B = 0;

        // Unknown
        pSprite->field_64 = 0;
    }

    if (Sprite_Handle_Soldier_Animation(pSprite))
        return;

    // Animation running?
    if (pSprite->field_38)
        return;

    if (Sprite_Handle_Player_MissionOver(pSprite))
        return;

    //loc_18E8D
    Sprite_Handle_Player_Enter_Vehicle(pSprite);
    mSprite_FaceWeaponTarget = 0;

    word_3AA1D = word_3BED5[pSprite->field_32];
    //seg004:0183

    if (mSquad_Join_TargetSquad[pSprite->field_32] < 0 && pSprite->field_32 != mSquad_Selected) {
        //loc_18F12
        mSprite_FaceWeaponTarget = -1;

		Data28 = &mSprites[pSprite->field_5E];

		// The original engine checked 1 sprite per cycle, this is problematic 
		// with max sprites gets bigger, especially at 100,000. So we check every sprite
		//  every cycle, if not using the original parameters.
		if (!mParams->isOriginalSpriteMax()) {
			bool looped = false;

			while (Data28->field_0 == -32768) {
				++pSprite->field_5E;
				if (pSprite->field_5E >= (mParams->mSpritesMax - 2)) {
					pSprite->field_5E = 0;
					if (looped)
						break;

					looped = true;
				}
				Data28 = &mSprites[pSprite->field_5E];
			};
		}

        if (Data28->field_0 != -32768) {

            if (Data28->field_18 == eSprite_Enemy) {
                if (Data28->field_38 == eSprite_Anim_None) {

                    //seg004:021A 
                    if (pSprite->field_4F == 0) {
                        //seg004:0228
                        Data0 = pSprite->field_0;
                        Data4 = pSprite->field_4;

                        int16 Data8 = Data28->field_0;
                        int16 DataC = Data28->field_4;

                        Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
                        if (Data0 < 0xD2) {

                            if (Data0 <= 0x28)
                                goto loc_1904A;

                            //seg004:0270
                            mSprite_Find_Distance = Data0;
                            Data0 = pSprite->field_0;
                            Data4 = pSprite->field_4;
                            Data8 = Data28->field_0;
                            DataC = Data28->field_4;

                            if (Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC) == 0) {
                                Data0 = mSprite_Find_Distance;
                                goto loc_1904A;
                            }
                            if (mSprite_Find_Distance < 0x40)
                                goto loc_1904A;
                        }
                    }
                }
            }
        }

    loc_1901C:;
        pSprite->field_4A = 0;
        pSprite->field_5E++;
        if (pSprite->field_5E >= (mParams->mSpritesMax - 2))
			pSprite->field_5E = 0;

        goto loc_191C3;

    // Enemy in range
    loc_1904A:;
        Data0 = tool_RandomGet() & 0x1F;

        if (Data0 == 5)
            goto loc_1901C;

        pSprite->field_5A = -1;
        mTroop_Weapon_Grenade_Disabled = false;
        mTroop_Weapon_Bullet_Disabled = false;
        mTroop_Weapon_Rocket_Disabled = false;

        pSprite->field_2E = Data28->field_0;
        pSprite->field_30 = Data28->field_4 - 0x0E;

        // Fired Weapon?
        if (pSprite->field_4A <= 0) {
            //loc_190B9
            Data0 = tool_RandomGet() & 0x0F;
            ++Data0;
            pSprite->field_4A = Data0;

            Data0 = tool_RandomGet() & 0x3F;
            if (Data0 == 0x2A)
                goto loc_1918C;
        }

    HandleWeapon:;
        mTroop_Weapon_Grenade_Disabled = false;
        mTroop_Weapon_Bullet_Disabled = false;
        mTroop_Weapon_Rocket_Disabled = false;

        if (pSprite->field_4F) {
            mTroop_Weapon_Rocket_Disabled = true;
            mTroop_Weapon_Bullet_Disabled = true;
            mTroop_Weapon_Grenade_Disabled = true;
        }
        else {
            //loc_19118
            if (pSprite == mSquad_Leader && mMouse_Button_LeftRight_Toggle) {

                if (mSquad_CurrentWeapon[pSprite->field_32] == eWeapon_Rocket) {
                    mTroop_Weapon_Grenade_Disabled = true;
                    mTroop_Weapon_Bullet_Disabled = true;
                } else {
                    if (mSquad_CurrentWeapon[pSprite->field_32] == eWeapon_Grenade) {
                        mTroop_Weapon_Rocket_Disabled = true;
                        mTroop_Weapon_Bullet_Disabled = true;
                    }
                    else
                        goto loc_19198;
                }
            loc_1918C:;
                pSprite->field_4A = 1;
                goto loc_191BF;
            }

        loc_19198:;
            mTroop_Weapon_Grenade_Disabled = true;
            mTroop_Weapon_Rocket_Disabled = true;

            if(pSprite->field_46_mission_troop)
                Data0 = pSprite->field_46_mission_troop->field_6;

        loc_191BF:;
            Sprite_Handle_Troop_Weapon(pSprite);
        }
    }
    else {

        if (mSprite_Player_CheckWeapon) {
            goto HandleWeapon;

        }
    }

loc_191C3:;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        goto loc_1921E;

    // seg004:047F
    eax = mSquad_WalkTargets[pSprite->field_32][pSprite->field_40].mY;

    //seg004:04CC
    if (eax < 0) {
    loc_1921E:;
        if (mSprite_FaceWeaponTarget) {

            if (pSprite->field_0 == pSprite->field_26 &&
                pSprite->field_4 == pSprite->field_28)
                goto loc_1931E;
        }
    }
    //loc_1925C
    // Just Fired a bullet or grenade?
    if (pSprite->field_54 != 2 && pSprite->field_54 != 1)
        goto loc_19338;


    // Just Fired bullet or grenade
    //loc_19274
    if (word_3AA1D != 2)
        goto loc_1931E;

    mTmp_FrameNumber = pSprite->field_A;
    pSprite->field_5A = 0;

    if (mSprite_FaceWeaponTarget)
        Sprite_Handle_Troop_Direct_TowardWeaponTarget(pSprite);
    else
        Sprite_Handle_Troop_Direct_TowardMouse(pSprite);

    Data0 = pSprite->field_26;
    if (Data0 < 0)
        goto loc_1946D;

    Data4 = pSprite->field_28;
    if (Data4 < 0)
        goto loc_1946D;

    Sprite_XY_Store(pSprite);

    if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) >= 0) {
        Sprite_Handle_Player_Adjust_Movement_Speed(pSprite);
        Sprite_Draw_Row_Update(pSprite);
    }

    if (pSprite->field_0 == pSprite->field_26) {

        if (pSprite->field_4 == pSprite->field_28)
            Sprite_Next_WalkTarget_Set(pSprite);
    }

    //loc_19314
    if (mSprite_Reached_Target == 0)
        return;

loc_1931E:;

    if (pSprite->field_5A)
        goto loc_19403;

    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    Sprite_Handle_Troop_Animation(pSprite);
    return;

loc_19338:;
    pSprite->field_43 = 0;
    mTmp_FrameNumber = pSprite->field_A;
    mSprite_Bumped_Into_SquadMember = 0;

    Sprite_Handle_Troop_Speed(pSprite);
    Data0 = pSprite->field_26;
    if (Data0 < 0)
        goto loc_1946D;
    Data4 = pSprite->field_28;
    if (Data4 < 0)
        goto loc_1946D;

    if (pSprite->field_44) {
        pSprite->field_44 -= 1;
        goto loc_193D6;

    }

    // Reached near target?
    //loc_19392
    if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) >= 0)
        goto loc_193D6;

    // Exactly on target?
    //  Then move toward the next target
    if (pSprite->field_0 == pSprite->field_26) {
        if (pSprite->field_4 == pSprite->field_28)
            Sprite_Next_WalkTarget_Set(pSprite);
    }
    //loc_193D3
    goto loc_1946D;

loc_193D6:;
    Sprite_XY_Store(pSprite);
    Sprite_Update_DirectionMod(pSprite);

    if (!pSprite->field_5A) {
        if (!pSprite->field_45)
            goto loc_19424;

        pSprite->field_45 -= 1;
        return;
    }

loc_19403:;
    pSprite->field_5A = 0;
    Sprite_Handle_Troop_Direct_TowardWeaponTarget_WithRestore(pSprite);
    return;

loc_19414:;
    pSprite->field_5A = 0;
    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    return;

loc_19424:; // Soldier Walking
    Sprite_Draw_Row_Update(pSprite);

    if (pSprite->field_0 == pSprite->field_26) {
        if (pSprite->field_4 == pSprite->field_28)
            Sprite_Next_WalkTarget_Set(pSprite);
    }
    //loc_19463
    if (mSprite_Reached_Target)
        goto loc_19414;

    goto loc_1957A;

loc_1946D:;
    if (pSprite->field_5A)
        goto loc_19403;

    if (!pSprite->field_45)
        goto loc_194A0;

    pSprite->field_45 -= 1;
    goto loc_19414;

loc_19490:;
    pSprite->field_5A = 0;
    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    return;

loc_194A0:;
    pSprite->field_43 = -1;

    if (!word_3AA1D) {
        mDirectionMod = 0;
        Sprite_Handle_Troop_Animation(pSprite);
        pSprite->field_A = 0;
        return;
    }

    //loc_194CB
    if (mSquad_Selected == pSprite->field_32)
        goto loc_19490;

    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    return;

loc_1957A:;
    pSprite->field_3A = 0;
    Sprite_Handle_Troop_Animation(pSprite);

}

void cFodder::Sprite_Handle_Enemy(sSprite* pSprite) {

    if (Sprite_Handle_Soldier_Animation(pSprite))
        return;

    if (pSprite->field_38)
        return;

    mSprite_FaceWeaponTarget = 0;
    if (pSprite->field_44) {
        pSprite->field_44--;
    }
    else {
        Sprite_Update_Follower_Target(pSprite);
        Sprite_Handle_Troop_Weapon(pSprite);

        mTmp_FrameNumber = pSprite->field_A;
        int16 Data0 = pSprite->field_26;
        if (Data0 < 0)
            goto loc_19A89;

        int16 Data4 = pSprite->field_28;
        if (Data4 < 0)
            goto loc_19A89;

        if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) < 0)
            goto loc_19A89;
    }
    //loc_19A5D
    Sprite_XY_Store(pSprite);
    Sprite_Update_DirectionMod(pSprite);
    Sprite_Handle_Troop_Speed(pSprite);

    if (pSprite->field_45) {
        pSprite->field_45--;
        if (pSprite->field_45)
            return;
    }
    Sprite_Draw_Row_Update(pSprite);
    goto loc_19A96;

loc_19A89:;
    word_3ABB1 = 0;
    Sprite_Handle_Troop_Direct_TowardWeaponTarget_WithRestore(pSprite);
    goto loc_19A9C;

loc_19A96:;
    word_3ABB1 = -1;

loc_19A9C:;
    pSprite->field_22 = eSprite_PersonType_AI;
    Sprite_Handle_Troop_Animation(pSprite);

    if (word_3ABB1)
        return;
    if (mSprite_FaceWeaponTarget)
        return;

    pSprite->field_A = 1;
}

void cFodder::Sprite_Handle_Player_Rank(sSprite* pSprite) {
    sSprite* Data24 = mSquad_Leader;
    int16 Data4, Data0;

    if (mSquad_Selected < 0)
        goto loc_1AF63;

    if (Data24 == INVALID_SPRITE_PTR)
        goto loc_1AF63;

    if (!mPhase_Completed_Timer) {
        Data4 = mSquad_Selected;

        if (mSquad_Walk_Target_Steps[Data4])
            goto loc_1AF63;
    }

    // No rank for non human soldiers, or if they're sinking
    if (Data24->field_18 != eSprite_Player || Data24->field_5B)
        goto loc_1AE64;

    if (!Data24->field_38)
        goto loc_1AE76;

    if (Data24->field_38 >= eSprite_Anim_Slide1)
        goto loc_1AE76;

loc_1AE64:;
    Squad_UpdateLeader(Data24);
    goto loc_1AF63;

loc_1AE76:;

    Data0 = Data24->field_0;
    if (Data0 < 0)
        goto loc_1AF63;

    Data0 += 3;
    Data4 = Data24->field_4;

    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;

    if (Data24->field_6E)
        goto loc_1AF63;

    pSprite->field_8 = 0x95;

    Data0 = Data24->field_46_mission_troop->mRank;

    pSprite->field_A = Data0;
    Data0 = pSprite->field_3A;
    Data0 += pSprite->field_10;

    if (!Data0)
        goto loc_1AF1F;

    if (Data0 < 6)
        goto loc_1AF27;

loc_1AF1F:;
    pSprite->field_10 = -pSprite->field_10;

loc_1AF27:;
    Data0 = 0;
    pSprite->field_20 = 1;
    pSprite->field_3A = Data0;

    Data0 += Data24->field_20;
    Data0 += 0x12;
    pSprite->field_20 = Data0;
    return;

loc_1AF63:;
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
}

void cFodder::Sprite_Handle_Player_Shadow(sSprite* pSprite) {
    sSprite* Data28 = pSprite->field_1A_sprite;
    int16 Data0;

    if (Data28 != pSprite->field_6A_sprite)
        goto loc_1B06D;

    Data0 = pSprite->field_62;

    if (Data0 != Data28->field_18)
        goto loc_1B06D;

    if (Data28->field_8 == 0x7C)
        goto loc_1B06D;

    if (Data28->field_52)
        goto loc_1B06D;

    pSprite->field_8 = 0x97;
    pSprite->field_A = 1;
    if (Data28->field_20 > 8)
        pSprite->field_A = 0;

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
    return;

loc_1B06D:;
    Data28->field_58 = 0;
    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Floating_Dead_Soldier(sSprite* pSprite) {
    int16 Data0, Data4;

    pSprite->field_8 = 0x9E;

    if (!pSprite->field_12) {
        pSprite->field_12 = 0x10;
        pSprite->field_10 = 0x100;
    }

    if (pSprite->field_10 <= 0x100)
        goto loc_1B35A;

    if (pSprite->field_12 == 0x10)
        goto loc_1B350;

    pSprite->field_12 = 0x10;
    goto loc_1B35A;

loc_1B350:;
    pSprite->field_12 = -16;

loc_1B35A:;

    Sprite_XY_Store(pSprite);
    Sprite_Movement_Calculate(pSprite);
    Data0 = -3;
    Data4 = 8;
    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (Data4 < eTerrainFeature_QuickSand || Data4 > eTerrainFeature_Water) {
        pSprite->field_0 = mStoredSpriteX & 0xFFFF;
        pSprite->field_4 = mStoredSpriteY & 0xFFFF;
        pSprite->field_36 = 0;
        Data0 = pSprite->field_12;

        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
    }

    //loc_1B3CA
    if (pSprite->field_36 < 4)
        pSprite->field_36++;

    pSprite->field_2C = eSprite_Draw_Second;
}

void cFodder::Sprite_Handle_Bonus_RankToGeneral(sSprite* pSprite) {
    int16 Data0;

    // Blow up?
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0x95;
    pSprite->field_A = 0x0F;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mSquad_Leader->field_46_mission_troop->mRank = 0x0F;
    mGUI_Sidebar_Setup = 0;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Bonus_RankHomingInvin_SquadLeader(sSprite* pSprite) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xE5;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    // Invincible + HomingMissiles
    mSquad_Leader->field_75 |= (eSprite_Flag_HomingMissiles | eSprite_Flag_Invincibility);
    mSquad_Leader->field_46_mission_troop->mRank = 0x0F;
    mGUI_Sidebar_Setup = 0;

    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 50;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Bonus_RankHomingInvin_Squad(sSprite* pSprite) {

    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xE6;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (mSquad_Selected < 0)
        return;

    sSprite** Data28 = mSquads[mSquad_Selected];
    for (; *Data28 != INVALID_SPRITE_PTR;) {

        sSprite* Data2C = *Data28++;
        Data2C->field_75 |= (eSprite_Flag_HomingMissiles | eSprite_Flag_Invincibility);

        Data2C->field_46_mission_troop->mRank = 0x0F;
    }

    mGUI_Sidebar_Setup = 0;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 0x32;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Enemy_Leader(sSprite* pSprite) {

    Sprite_Handle_Hostage(pSprite);
}

int16 cFodder::Sprite_Handle_Soldier_Animation(sSprite* pSprite) {
    int32 Dataa4;
    int16 Data0, Data4, Data8;
    const int16* Data28;
    sSprite* Data20 = 0, *Data2C = 0, *Data30 = 0;
    int16 Field_52;
    int16 Field_0;
    int16 Field_4;

    if (pSprite->field_38 == eSprite_Anim_Hit3)
        goto loc_1E831;

    if (pSprite->field_38 == eSprite_Anim_Die2)
        goto loc_1EA48;

    if (pSprite->field_38 == eSprite_Anim_Hit2)
        goto loc_1EB87;

    Data0 = pSprite->field_4;
    if (Data0 >= mMapLoaded->getHeightPixels())
        pSprite->field_4 = mMapLoaded->getHeightPixels();

    //loc_1E0A4
    if (pSprite->field_56)
        goto loc_1E831;

    if (pSprite->field_5B == 1)
        goto loc_1E74C;

    if (pSprite->field_38 == eSprite_Anim_Die5)
        goto loc_1E2F4;

    if (pSprite->field_38 == eSprite_Anim_Slide1)
        goto loc_1ECA6;

    if (pSprite->field_38 == eSprite_Anim_Slide2)
        goto loc_1ED5B;

    if (pSprite->field_38 == eSprite_Anim_Slide3)
        goto loc_1EE59;

    if (pSprite->field_38 == eSprite_Anim_Vehicle_Enter)
        goto Sprite_EnteringVehicle;

    if (pSprite->field_38 == eSprite_Anim_Vehicle_Inside)
        goto Sprite_EnteredVehicle;

    //seg004:53BD
    // Is Sprite Dying
    if (!pSprite->field_38)
        goto loc_1EA3F;

    if (!pSprite->field_64)
        goto loc_1E3D2;

    pSprite->field_64 = 0;
    if (mPhase_TryAgain)
        pSprite->field_40 = -26506;

    if (!pSprite->field_58) {
        Sprite_Create_Player_Shadow(pSprite);
        pSprite->field_58 = -1;
    }

    //loc_1E15D
    Data8 = pSprite->field_10;
    Data8 >>= 5;
    Data8 -= 1;
    Data8 ^= 0x0F;
    Data8 &= 0x0E;
    Data0 = pSprite->field_22;

    Data28 = mSprite_AnimationPtrs[Data0];
    pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

    //loc_1E1E9
    pSprite->field_A = 0;
    pSprite->field_2A = 0;

    if (pSprite->field_1A < 0xB0000) {
        int32 Dataa0 = tool_RandomGet() & 0x07;
        Dataa0 += 2;
        Dataa0 = (Dataa0 << 16) | (Dataa0 >> 16);
        Dataa0 += pSprite->field_1A;

        pSprite->field_1A = Dataa0;
    }
    //loc_1E232
    pSprite->field_12 = 2;
    Data0 = tool_RandomGet();
    Data4 = 0;
    Data0 &= 7;
    Data4 = mSprite_Civilian_Sound_Death[Data0];
    //seg004:5508
    Sound_Play(pSprite, Data4, 0x14);

    if (pSprite->field_36 < 0x1E)
        pSprite->field_36 += 0x0F;

    pSprite->field_3E = pSprite->field_10;
    pSprite->field_3A = -1;
    Data0 = tool_RandomGet() & 0x01;
    if (!Data0)
        pSprite->field_3A = pSprite->field_10;

    Sprite_Create_BloodTrail(pSprite, Data2C, Data30);
    pSprite->field_26 = -1;
    if (pSprite->field_38 != eSprite_Anim_Die4)
        goto loc_1E3D2;

    // Die on spike
    pSprite->field_36 = 0;
    pSprite->field_38 = eSprite_Anim_Die5;
loc_1E2F4:;
    pSprite->field_3A -= 1;
    if (pSprite->field_3A < 0) {
        pSprite->field_38 = eSprite_Anim_Hit;
        return -1;
    }
    //loc_1E30C
    Data0 = 0x21;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        Data0 = 0x63;

    pSprite->field_8 = Data0;
    Data0 = pSprite->field_2A & 7;
    //seg004:55F3 

    if (!Data0) {
        sSprite* Data2C = 0;
        Data30 = 0;

        Sprite_Create_BloodTrail(pSprite, Data2C, Data30);

        if (pSprite->field_A)
            pSprite->field_A = 0;
        else
            pSprite->field_A = 1;
    }
    //loc_1E36C

    pSprite->field_2A -= 1;
    if (pSprite->field_2A >= 0)
        return -1;

    Data0 = tool_RandomGet() & 7;
    Data0 += 8;
    pSprite->field_2A = Data0;
    Data0 = tool_RandomGet() & 7;

    Data4 = mSprite_Civilian_Sound_Death[Data0];
    Data8 = 0;
    Sound_Play(pSprite, Data4, Data8);
    return -1;

loc_1E3D2:;

    if (pSprite->field_40 == -26506) {
        sSprite* Data2C = 0;
        Data30 = 0;

        pSprite->field_28 += 1;
        Sprite_Create_BloodTrail(pSprite, Data2C, Data30);

        if (!(pSprite->field_28 & 7)) {
            Data0 = tool_RandomGet() & 7;
            Data4 = mSprite_Civilian_Sound_Death[Data0];
            Data8 = 0x14;
            Sound_Play(pSprite, Data4, Data8);
        }
    }
    //loc_1E437

    if (pSprite->field_3A >= 0) {
        pSprite->field_3E = pSprite->field_3A;

        Data0 = pSprite->field_36 << 2;
        pSprite->field_3A += Data0;
        //seg004:5724
        pSprite->field_3A &= 0x1FE;
        Data8 = pSprite->field_3A;

        Data8 >>= 5;
        Data8 -= 1;
        Data8 ^= 0x0F;
        Data8 &= 0x0E;

        Data28 = mSprite_AnimationPtrs[pSprite->field_22];
        pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

        if (pSprite->field_59)
            pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

        // FIX: Added as only 2 frames exist for -most- sprites in mSprite_AnimationPtrs
        pSprite->field_A %= 2;
    }
    //loc_1E50A
    Sprite_XY_Store(pSprite);
    mTmp_FrameNumber = pSprite->field_A;
    Sprite_Movement_Calculate(pSprite);

    if (pSprite->field_20 < 0x0C) {

        int16 Field10 = pSprite->field_10;
        Sprite_Terrain_Check(pSprite, Data4);
        pSprite->field_10 = Field10;
    }
    else {
        //loc_1E542
        int16 Field0 = pSprite->field_0;
        int16 Field4 = pSprite->field_4;
        int16 Field10 = pSprite->field_10;

        Sprite_Terrain_Check(pSprite, Data4);

        pSprite->field_10 = Field10;
        pSprite->field_4 = Field4;
        pSprite->field_0 = Field0;
    }

    //loc_1E579
    if (!pSprite->field_20) {
        if (pSprite->field_4F || pSprite->field_50) {
            pSprite->field_52 = 6;
        }
    }
    //loc_1E5A7
    Dataa4 = (int64)pSprite->field_1A;

    pSprite->field_1E_Big += pSprite->field_1A;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;

        Dataa4 = -Dataa4;
        Dataa4 >>= 2;

        if (pSprite->field_52) {
            pSprite->field_36 = 0;
            Dataa4 = 0;
        }
    }
    //loc_1E619
    Dataa4 -= 0x18000;
    pSprite->field_1A = Dataa4;
    if (pSprite->field_36) {
        pSprite->field_36 -= 2;

        if (pSprite->field_36 < 0)
            pSprite->field_36 = 0;
    }
    //loc_1E653
    if (pSprite->field_26) {

        if (!pSprite->field_20) {
            sSprite* Data2C = 0;
            Data30 = 0;
            Sprite_Create_BloodTrail(pSprite, Data2C, Data30);
            pSprite->field_26 = 0;
        }
    }
    //loc_1E67A
    pSprite->field_12 -= 1;
    if (!pSprite->field_12) {

        pSprite->field_12 = 6;
        if (pSprite->field_A < 1)
            pSprite->field_A += 1;
    }
    //loc_1E6A3
    if (pSprite->field_2A <= 0x14) {

        pSprite->field_2A++;
        return -1;      // DOS version jumped to loc_1EB5D; here, which was different from Amiga and caused a bug
    }
    //loc_1E6BA
    if (pSprite->field_20)
        return -1;     // DOS version jumped to loc_1EB5D; here, which was different from Amiga and caused a bug

    //seg004:5978
    if (!pSprite->field_52) {
        if (!pSprite->field_50) {

            Data0 = tool_RandomGet() & 0x7F;
            if (!Data0) {

                pSprite->field_3A = 0x1F4;
                pSprite->field_38 = eSprite_Anim_Die5;
                mGUI_Sidebar_Setup = 0;
                return -1;
            }
        }
    }
    //loc_1E708
    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_10 = pSprite->field_3E;
    pSprite->field_12 = 0;
    pSprite->field_45 = 1;

    return -1;

loc_1E74C:;
    if (pSprite->field_52 < 5) {
        pSprite->field_52 = 5;
        Data4 = 0x0E;
        Data8 = 0x0A;

        Sound_Play(pSprite, Data4, Data8);
        pSprite->field_8 = 0x38;
        if (pSprite->field_22 != eSprite_PersonType_Human)
            pSprite->field_8 = 0x7A;

        pSprite->field_A = 0;
        pSprite->field_12 = 0x0A;

        return 1;
    }
    //loc_1E7AC
    pSprite->field_12 -= 1;
    if (!pSprite->field_12) {

        pSprite->field_52 += 1;
        if (pSprite->field_52 >= 0x0D) {

            pSprite->field_38 = eSprite_Anim_Die2;
            pSprite->field_10 = pSprite->field_3E;
            pSprite->field_12 = 0;
            pSprite->field_45 = 1;

            goto loc_1E825;
        }
        else {
            pSprite->field_12 = 0x0A;
        }
    }
    //loc_1E806
    if (!(pSprite->field_12 & 2) && pSprite->field_A < 5)
        pSprite->field_A += 1;
loc_1E825:;
    Data0 = -1;
    return -1;

loc_1E831:;
    if (pSprite->field_56 == 1) {
        pSprite->field_12 = 1;
        pSprite->field_28 += 2;
    }
    Data0 = pSprite->field_12;
    pSprite->field_12 += 1;

    if (pSprite->field_38 == eSprite_Anim_Hit3) {
        pSprite->field_52 += Data0;

        if (pSprite->field_52 < 0x0E)
            return -1;

        pSprite->field_52 = 0x0E;
        pSprite->field_12 = 0x14;
        Data0 = tool_RandomGet() & 0x07;
        Data4 = mSprite_Civilian_Sound_Death[Data0];
        Data8 = 0x0A;
        Sound_Play(pSprite, Data4, Data8);
        goto loc_1E9EC;
    }
    //loc_1E8D6
    pSprite->field_4 += Data0;
    Data0 = pSprite->field_4;
    if (Data0 >= mMapLoaded->getHeightPixels())
        pSprite->field_38 = eSprite_Anim_Hit2;

    mStoredSpriteY = pSprite->field_4;
    Data0 = 8;
    Data0 += pSprite->field_26;
    Data4 = -3;
    Data4 += pSprite->field_28;

    Map_Terrain_Get_Type_And_Walkable(Data0, Data4);

    if (Data4 == eTerrainFeature_Drop || Data4 == eTerrainFeature_Drop2) {
        Data0 = pSprite->field_12;
        pSprite->field_28 += Data0;
        Data8 = pSprite->field_28;

        sMapTarget* WalkTarget = mSquad_WalkTargets[pSprite->field_32];
        //seg004:5C49
        Data0 = pSprite->field_40;
        if (WalkTarget[Data0].mX >= 0)
            WalkTarget[Data0].mY = Data8;
    }
    //loc_1E9CD;
    Sprite_Terrain_Check(pSprite, Data4);
    if (pSprite->field_12 > 5)
        Sprite_Update_Direction_Anim(pSprite);

    if (Data4 == 9)
        return -1;

loc_1E9EC:; // Troop Falling?
    if (pSprite->field_12 < 0x0C) {
        pSprite->field_3E = 0;
        pSprite->field_56 = 0;

        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_10 = pSprite->field_3E;
    pSprite->field_12 = 0;
    pSprite->field_45 = 1;
    return -1;

loc_1EA3F:;
    Data0 = 0;
    return 0;

loc_1EA48:;
    pSprite->field_12 += 1;
    if (pSprite->field_12 >= 0x0F)
        return Sprite_Troop_Dies(pSprite);

    if (pSprite->field_12 >= 0x07 && pSprite->field_8 != 0x7C) {
        pSprite->field_8 = 0x7C;
        goto loc_1EB0E;
    }

loc_1EA82:;
    Data8 = pSprite->field_10;
    Data8 >>= 5;
    Data8 -= 1;
    Data8 ^= 0x0F;
    Data8 &= 0x0E;

    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

    // FIX: Added as only 2 frames exist for all sprites in mSprite_AnimationPtrs
    pSprite->field_A %= 2;
loc_1EB0E:;
    Field_52 = pSprite->field_52;
    Field_0 = pSprite->field_0;
    Field_4 = pSprite->field_4;
    Sprite_Terrain_Check(pSprite, Data4);
    pSprite->field_4 = Field_4;
    pSprite->field_0 = Field_0;
    pSprite->field_52 = Field_52;

    if (pSprite->field_4F || pSprite->field_50) {

        if (pSprite->field_52 >= 0x0D)
            pSprite->field_52 = 0x0B;

        pSprite->field_52 += 2;
    }

    return -1;

loc_1EB87:;
    pSprite->field_12 += 1;
    if (pSprite->field_12 >= 0x0F)
        return Sprite_Troop_Dies(pSprite);

    if (pSprite->field_12 < 0x07)
        goto loc_1EA82;

    if (pSprite->field_8 != 0x7C) {
        pSprite->field_8 = 0x7C;
        goto loc_1EB0E;
    }

    Data8 = pSprite->field_10;
    Data8 >>= 5;
    Data8 -= 1;
    Data8 ^= 0x0F;
    Data8 &= 0x0E;

    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

    pSprite->field_A = 0;
    //loc_1EC4F
    Field_52 = pSprite->field_52;
    Sprite_Terrain_Check(pSprite, Data4);
    pSprite->field_52 = Field_52;

    if (pSprite->field_4F || pSprite->field_50) {
        if (pSprite->field_52 >= 0x0D)
            pSprite->field_52 = 0x0B;
        pSprite->field_52 += 2;
    }

    //loc_1EC9A
    return -1;

loc_1ECA6:;
    if (pSprite->field_60 <= eTerrainFeature_Water && pSprite->field_60 >= eTerrainFeature_QuickSand) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Slide2;
    pSprite->field_36 = 0x24;

    Data0 = tool_RandomGet() & 6;
    Data0 = mSprite_Soldier_Unk[Data0];

    Data0 += pSprite->field_10;
    Data0 &= 0x1FE;
    pSprite->field_10 = Data0;
    pSprite->field_8 = 0xA4;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        pSprite->field_8 = 0xA7;

    Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction(pSprite);
    Data8 >>= 1;
    pSprite->field_A = Data8;
    return -1;

loc_1ED5B:;
    Sprite_XY_Store(pSprite);
    mTmp_FrameNumber = pSprite->field_A;

    Sprite_Movement_Calculate(pSprite);
    Sprite_Reached_MapEdge(pSprite);
    Sprite_Terrain_Check(pSprite, Data4);

    if (pSprite->field_38 != eSprite_Anim_Slide2)
        goto loc_1EE3E;

    if (pSprite->field_60 > eTerrainFeature_Water || pSprite->field_60 < eTerrainFeature_QuickSand) {
        pSprite->field_36 -= 5;
        if (pSprite->field_36) {
            if (pSprite->field_36 >= 0)
                return -1;

            pSprite->field_36 = 0;
        }
    }
    Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction(pSprite);

    //seg004:606C
    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    Data0 = Data28[(0x60 + Data8) / 2];

    if (Data0 != pSprite->field_8) {
        pSprite->field_8 = Data28[(0x60 + Data8) / 2];
        pSprite->field_A = 0;
        return -1;
    }
    //loc_1EE34
    pSprite->field_38 = eSprite_Anim_None;

loc_1EE3E:;
    pSprite->field_43 = 0;
    Squad_UpdateLeader(Data20);
    return -1;

loc_1EE59:;
    if (pSprite->field_60 <= eTerrainFeature_Water && pSprite->field_60 >= eTerrainFeature_QuickSand) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Slide2;
    pSprite->field_36 = 0x2E;

    Data0 = tool_RandomGet() & 0x1F;

    pSprite->field_10 += Data0;
    pSprite->field_8 = 0xA4;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        pSprite->field_8 = 0xA7;

    Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction(pSprite);
    Data8 >>= 1;
    pSprite->field_A = Data8;
    return -1;

Sprite_EnteringVehicle:;

    // Not walking to a vehicle?
    if (!pSprite->field_66) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Vehicle_Inside;
    pSprite->field_8 = 0xCC;
    pSprite->field_A = 4;
    return -1;

Sprite_EnteredVehicle:;
    pSprite->field_A -= 1;
    if (pSprite->field_A < 0)
        return Sprite_Troop_EnterVehicle_Finish(pSprite);

    pSprite->field_36 = 0x30;
    Sprite_Draw_Row_Update(pSprite);
    return -1;

}

int16 cFodder::Sprite_Troop_EnterVehicle_Finish(sSprite* pSprite) {
    if (!pSprite->field_66) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    Sprite_Troop_EnterVehicle_Position(pSprite, (sSprite*)pSprite->field_66);
    pSprite->field_A = 0;
    pSprite->field_8 = 0x7C;
    pSprite->field_38 = eSprite_Anim_None;
    return 0;
}

void cFodder::Sprite_Troop_EnterVehicle_Position(sSprite* pSprite, sSprite* pData24) {
    int16 Data0 = pData24->field_0;
    Data0 += 0x10;
    if (pData24->field_6F == eVehicle_Turret_Cannon)
        goto loc_1F0B9;

    if (pData24->field_6F == eVehicle_Turret_Missile)
        goto loc_1F0BE;
loc_1F0B9:;
    Data0 -= 0x0C;

loc_1F0BE:;

    int16 Data4 = pData24->field_4;
    Data4 -= 0x09;

    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;
}

int16 cFodder::Sprite_Troop_Dies(sSprite* pSprite) {
    sMission_Troop* Troop = 0;

    // Is Player?
    if (pSprite->field_22 == eSprite_PersonType_Human) {
        //Yes, is player

        ++mGame_Data.mScore_Kills_Away;

        if (pSprite->field_46_mission_troop) {

            mGame_Data.Soldier_Died(pSprite->field_46_mission_troop);

            pSprite->field_46_mission_troop->mSprite = INVALID_SPRITE_PTR;
            pSprite->field_46_mission_troop->mRecruitID = -1;
            pSprite->field_46_mission_troop->mRank = 0;
        }

        if (mSquad_Selected < 0) {
            mGUI_Sidebar_Setup = 0;
            mSquad_Select_Timer = 0;
        }
        else {
            mGUI_Sidebar_Setup = 0;
        }

        //loc_1F03D
        return Sprite_Destroy_Wrapper(pSprite);
    }

    if (pSprite->field_18 == eSprite_Enemy_Rocket || pSprite->field_18 == eSprite_Enemy) {

        ++mGame_Data.mScore_Kills_Home;
        --mTroops_Enemy_Count;
        if (pSprite->field_5D) {
            pSprite->field_5D = 0;

            // Default to first human soldier
            Troop = mGame_Data.mSoldiers_Allocated;

            if (pSprite->field_5E_SoldierAllocated) {
                Troop = pSprite->field_5E_SoldierAllocated;
                goto loc_1F1E9;
            }
        }

        // Sprite didnt have a soldier who killed it attached
        if (mSquad_Selected != -1) {
            //loc_1F13E
            // use the first squad member
            if (mSquads[mSquad_Selected][0] != INVALID_SPRITE_PTR && mSquads[mSquad_Selected][0] != 0)
                Troop = mSquads[mSquad_Selected][0]->field_46_mission_troop;
        }
        loc_1F1E9:;

        if (Troop >= mGame_Data.mSoldiers_Allocated && Troop <= &mGame_Data.mSoldiers_Allocated[8]) {

            if (Troop->mNumberOfKills >= 999)
                Troop->mNumberOfKills = 998;

            ++Troop->mNumberOfKills;
        }
    }

    return Sprite_Destroy_Wrapper(pSprite);
}

int16 cFodder::Sprite_Handle_Player_MissionOver(sSprite* pSprite) {

    if (!mPhase_Aborted)
        if (!mPhase_Complete)
            return 0;

    int16 Data0 = -1;
    int16 Data4 = 8;

    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);

    if (Data4 == eTerrainFeature_Water) {
        Data0 = pSprite->field_20;
        Data0 -= 8;

        if (Data0 >= 0) {
            Data0 = 0;
        }
        else {
            Data0 = -Data0;
            if (Data0 > 8)
                Data0 = 8;
        }

        pSprite->field_52 = Data0;
    }
    //loc_1F283

    if (pSprite->field_8 == 0x41)
        goto loc_1F3C4;

    if (pSprite->field_8 == 0x9F)
        goto loc_1F340;

    if (pSprite->field_8 == 0x38)
        goto loc_1F3A2;

    pSprite->field_A = 0;
    pSprite->field_36 = 0;

    if (!mPhase_Aborted) {

        Data0 = tool_RandomGet() & 1;
        if (Data0 == 0)
            goto loc_1F32C;

        if (!pSprite->field_52) {
            Data0 = tool_RandomGet() & 1;

            if (!Data0) {
                //loc_1F2F9
                pSprite->field_8 = 0x41;
                Data0 = tool_RandomGet() & 3;
                pSprite->field_A = Data0;
                pSprite->field_2A = 1;
                Sprite_Create_Player_Shadow(pSprite);
                goto loc_1F3C4;
            }
        }
        else
            goto loc_1F32C;
    }

    //loc_1F2EC
    pSprite->field_8 = 0x38;
    goto loc_1F3A2;

loc_1F32C:;
    pSprite->field_8 = 0x9F;
    pSprite->field_2A = 0;

loc_1F340:;

    Data0 = pSprite->field_2A;

    pSprite->field_A = mSprite_Player_MissionOver_Frames[Data0];
    pSprite->field_36 ^= -32768;
    if (pSprite->field_36 < 0)
        goto loc_1F41D;

    pSprite->field_2A += 1;
    if (pSprite->field_2A < 7)
        goto loc_1F41D;

    pSprite->field_2A = 3;
    goto loc_1F41D;

loc_1F3A2:;
    pSprite->field_36 ^= -32768;
    if (pSprite->field_36 < 0)
        goto loc_1F3C2;

    if (pSprite->field_A >= 5)
        goto loc_1F41D;

    pSprite->field_A += 1;
loc_1F3C2:;
    goto loc_1F41D;

loc_1F3C4:;

    pSprite->field_A += pSprite->field_2A;
    if (!pSprite->field_A)
        goto loc_1F3E9;

    if (pSprite->field_A != 5)
        goto loc_1F3F1;

loc_1F3E9:;
    pSprite->field_2A = -pSprite->field_2A;

loc_1F3F1:;
    Data0 = pSprite->field_A;
    Data0 -= 1;
    if (Data0 < 0)
        Data0 = 0;

    Data0 <<= 3;
    pSprite->field_20 = Data0;

loc_1F41D:;
    Data0 = -1;

    return -1;
}

void cFodder::Sprite_Handle_Troop_Weapon(sSprite* pSprite) {

    if (pSprite->field_22 == eSprite_PersonType_Human) {

        // Fired Weapon?
        if (!pSprite->field_4A)
            return;

        // Delay before weapon fires?
        if (pSprite->field_4A >= 0) {

            pSprite->field_4A--;
            if (pSprite->field_4A)
                return;
        }

    }
    else {
        // No weapon firing?
        if (!pSprite->field_4A)
            return;

        if (pSprite->field_4A >= 0) {

            pSprite->field_4A -= 1;
            if (pSprite->field_4A)
                return;

            pSprite->field_4A = -1;
        }
    }

    // In water? no weapons
    if (pSprite->field_4F)
        return;

    // Are we enemy with rockets?
    if (pSprite->field_18 == eSprite_Enemy_Rocket) {
        mTroop_Weapon_Bullet_Disabled = true;
        mTroop_Weapon_Grenade_Disabled = true;
        mTroop_Weapon_Rocket_Disabled = true;

        // Only fire is unit is more than 24 away
        if (mSprite_Find_Distance > 0x18)
            mTroop_Weapon_Rocket_Disabled = false;

    }
    else {

        // Enemy, after 4th map (can throw grenades)
        //loc_1F4B0
        if (pSprite->field_18 == eSprite_Enemy) {

            // Retail CF didn't allow enemy troops to use grenades until mission 4
            if(!(mVersionCurrent->isRetail() && mGame_Data.mMission_Number < 4)) {

            // Use a grenade?
                if (!(tool_RandomGet() & 0x1F)) {

                    mTroop_Weapon_Bullet_Disabled = true;
                    mTroop_Weapon_Grenade_Disabled = false;
                }
            }
        }
    }

    // bullet weapon disabled?
    if (!mTroop_Weapon_Bullet_Disabled) {

        if (Sprite_Create_Bullet(pSprite))
            pSprite->field_4A = -1;
        else
            pSprite->field_4A = 0;

        if (word_3AA1D != 2)
            pSprite->field_45 = 0x0F;
    }
    mTroop_Weapon_Bullet_Disabled = false;

    // Grenade disabled?
    if (!mTroop_Weapon_Grenade_Disabled) {

        if (!Sprite_Create_Grenade(pSprite))
            pSprite->field_4A = 0;
        else
            pSprite->field_4A = -1;

        if (word_3AA1D != 2)
            pSprite->field_45 = 0x0C;

    }
    mTroop_Weapon_Grenade_Disabled = false;

    // Second weapon disabled?
    if (!mTroop_Weapon_Rocket_Disabled) {

        if (!Sprite_Create_Rocket(pSprite))
            pSprite->field_4A = 0;
        else
            pSprite->field_4A = -1;

        // 
        if (pSprite->field_45 < 0x0A) {

            pSprite->field_45 += 5;
            pSprite->field_44 = pSprite->field_45;
        }
    }
    mTroop_Weapon_Rocket_Disabled = false;
}

void cFodder::Sprite_Handle_Troop_Direct_TowardWeaponTarget(sSprite* pSprite) {

    int16 Data0 = pSprite->field_2E;
    int16 Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Set_Direction_To_Next(pSprite);
    Sprite_Handle_Troop_Animation(pSprite);
}

void cFodder::Sprite_Handle_Troop_Direct_TowardMouse(sSprite* pSprite) {

    int16 Data0 = mMouseX;
    Data0 += mCameraX >> 16;
    Data0 -= 0x18;

    int16 Data4 = mMouseY;
    Data4 += mCameraY >> 16;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Set_Direction_To_Next(pSprite);

    int StoredAnim = pSprite->field_8;
    mStoredSpriteFrame = pSprite->field_A;
    Sprite_Handle_Troop_Animation(pSprite);

    if (!pSprite->field_43)
        return;

    // FIX: This isn't original. and was added due to a case where the animation is changed
    //      in Sprite_Handle_Troop_Animation, but the frame is pointing to a frame not
    //      available in the new animation (issue 64)
    if ((pSprite->field_8 >= 0x10 && pSprite->field_8 < 0x18) ||
        (pSprite->field_8 >= 0x20 && pSprite->field_8 < 0x28)) {
        if(mStoredSpriteFrame >= 1)
            mStoredSpriteFrame = 0;
    }

    if ((pSprite->field_8 >= 0x18 && pSprite->field_8 < 0x20) ||
        (pSprite->field_8 >= 0x28 && pSprite->field_8 < 0x30)) {
        if (mStoredSpriteFrame >= 2)
            mStoredSpriteFrame = 0;
    }

    pSprite->field_A = mStoredSpriteFrame;
}

void cFodder::Sprite_Handle_Player_Adjust_Movement_Speed(sSprite* pSprite) {
    Sprite_Handle_Troop_Speed(pSprite);

    if (pSprite->field_4F)
        return;

    if (pSprite->field_50)
        return;

    int16 Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;

    int16 Data4 = 0;
    for (Data4 = 0; Data4 < 0x0E; Data4 += 2) {
        if (Data0 == pSprite->field_3C)
            break;

        Data0 += 2;
        Data0 &= 0x0E;
    }

    pSprite->field_36 = mSprite_Speed_Direction_Modifier[Data4 / 2];
}

void cFodder::Sprite_Handle_Troop_Animation(sSprite* pSprite) {
    int16 Data0, Data8;
    sSprite* Dataa30 = 0;
    sMapTarget* Data30 = 0;
    const int16* Data28 = 0;

    // Is Human
    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto AlterAnimation;

    // Hostage Following Sprite?
    if (!pSprite->field_70)
        goto AlterAnimation;

    // Hasnt Reached X Target
    if (pSprite->field_0 != pSprite->field_26)
        goto AlterAnimation;

    // Hasnt Reached Y Target
    if (pSprite->field_4 != pSprite->field_28)
        goto AlterAnimation;

    // Idle
    pSprite->field_8 = 0xDC;
    pSprite->field_A = 0;
    word_3ABB1 = -1;
    return;

AlterAnimation:;
    if (word_3AA1D == 2)
        goto loc_1F7FF;

    if (pSprite->field_5A)
        goto loc_1F7F9;

    if (!pSprite->field_43)
        goto loc_1F7FF;

    if (pSprite->field_4F)
        goto loc_1F7FF;

loc_1F7F9:;
    mSprite_Bumped_Into_SquadMember = -1;

loc_1F7FF:;
    Data8 = pSprite->field_3C;
    Data8 += mDirectionMod;
    Data8 &= 0x0E;

    pSprite->field_3C = Data8;
    Data8 /= 2;

    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    if (!mSprite_FaceWeaponTarget)
        goto loc_1F9C0;

    if (pSprite->field_22 == eSprite_PersonType_Human) {
        Data30 = mSquad_WalkTargets[pSprite->field_32];
        if (Data30[pSprite->field_40].mX >= 0)
            goto loc_1F9C0;

        //seg005:01CC
        Dataa30 = mSquads[pSprite->field_32][0];

        if (Dataa30 == INVALID_SPRITE_PTR || Dataa30 == 0)
            goto loc_1F9C0;

        //seg005:020C
        if (Dataa30 != pSprite) {

            if (Dataa30->field_0 != Dataa30->field_26)
                goto loc_1F9C0;
            if (Dataa30->field_4 != Dataa30->field_28)
                goto loc_1F9C0;

            if (!word_3B2F3)
                goto loc_1F9C0;
        }
        else {
            if (pSprite->field_0 != pSprite->field_26)
                goto loc_1F9C0;

            //seg005:0246
            if (pSprite->field_4 != pSprite->field_28)
                goto loc_1F9C0;
        }
    }

    if (pSprite->field_60 == eTerrainFeature_Water)
        goto loc_1F9C0;

    //seg005:02A6
    pSprite->field_8 = *(Data28 + Data8 + 0x30);
    pSprite->field_A = 0;
    mStoredSpriteFrame = 0;
    return;

loc_1F9C0:;
    // Fired Bullet
    if (pSprite->field_54 == 2) {

        if (word_3AA1D) {
            if (word_3AA1D == 2) {
                pSprite->field_8 = *(Data28 + Data8);
                pSprite->field_A = mSprite_Frame_3 % 3;
            }
            else {
                //loc_1F9FF
                pSprite->field_8 = *(Data28 + Data8);
                pSprite->field_A = 0;
            }
        }
        //loc_1FA20
        --pSprite->field_57;
        if (pSprite->field_57)
            return;

        pSprite->field_54 = 0;
        goto loc_1FB00;
    }

    // loc_1FA39: Chucked Grenade
    if (pSprite->field_54 == 1) {
        Data0 = *(Data28 + Data8 + 0x28);

        if (Data0 != pSprite->field_8) {
            pSprite->field_8 = Data0;
            pSprite->field_55 = 0;
            pSprite->field_A = 0;
            return;
        }
        //loc_1FA93
        pSprite->field_55++;
        if (pSprite->field_55 != 2)
            return;
        pSprite->field_55 = 0;
        pSprite->field_A++;
        mStoredSpriteFrame = pSprite->field_A;

        if (pSprite->field_A < 3)
            return;

        mStoredSpriteFrame = 0;
        pSprite->field_A = 0;
        pSprite->field_54 = 0;
        pSprite->field_5A = 0;
        pSprite->field_55 = 0;
    }

loc_1FB00:;
    // Fired Rocket
    if (pSprite->field_54 == 3) {

        Data0 = *(Data28 + Data8 + 0x38);
        if (Data0 != pSprite->field_8) {
            pSprite->field_8 = Data0;
            pSprite->field_55 = 0;
            pSprite->field_A = 0;
            return;
        }
        //loc_1FB5A
        pSprite->field_55++;
        if (pSprite->field_55 != 7)
            return;

        pSprite->field_55 = 0;
        pSprite->field_A = 0;
        pSprite->field_54 = 0;
        pSprite->field_5A = 0;
        pSprite->field_55 = 0;
    }

    // loc_1FBA4: If in water
    if (pSprite->field_4F) {
        pSprite->field_8 = *(Data28 + Data8 + 0x20);
        // Reached another troop?
        pSprite->field_A = (pSprite->field_43) ? mSprite_Frame_2 : mSprite_Frame_1;
        pSprite->field_A &= 1;
    }
    else if (!word_3AA1D) {
        //loc_1FBF8
        pSprite->field_8 = *(Data28 + Data8 + 0x18);
        pSprite->field_A = mSprite_Frame_2;
    }
    else if (pSprite->field_8 == *(Data28 + Data8 + 0x18) && pSprite->field_A) {
        //loc_1FC29
        pSprite->field_A = 0;
    }
    else {
        //loc_1FC61
        Data0 = (pSprite->field_46_mission_troop) ? 
                 pSprite->field_46_mission_troop->field_6 : 0x70;

        pSprite->field_8 = (Data0 == 3) ? *(Data28 + Data8 + 0) : *(Data28 + Data8);
        pSprite->field_A = (Data0 != 2) ? mSprite_Frame_1 : mSprite_Frame_3;
    }

    if (mSprite_Bumped_Into_SquadMember) {
        // FIX: Added % as only 2 frames exist for -most- sprites in mSprite_AnimationPtrs
        // There is also one case where mTmp_FrameNumber isnt set by the current sprite
        pSprite->field_A = mTmp_FrameNumber % 2;
        mSprite_Bumped_Into_SquadMember = 0;
    }
}

void cFodder::Sprite_Handle_Troop_Speed(sSprite* pSprite) {
    int16 Data0;

    pSprite->field_36 = 0x10;

    if (pSprite->field_18 == eSprite_Enemy) {
        Data0 = 0x0C;
        Data0 += pSprite->field_62;
        if (Data0 > 0x1A)
            Data0 = 0x1A;
        pSprite->field_36 = Data0;
    }
    else {

        //loc_1FD36
        Data0 = word_3BED5[pSprite->field_32];
        if (!Data0)
            pSprite->field_36 >>= 1;
        else {
            if (Data0 != 1)
                pSprite->field_36 = 0x18;
        }
    }

    if (pSprite->field_4F)
        goto loc_1FDDC;

    Data0 = pSprite->field_52;
    if (Data0 == pSprite->field_50)
        goto loc_1FDD5;
    if (Data0 < pSprite->field_50)
        goto loc_1FDC3;

    Data0--;
    if (Data0 == pSprite->field_50)
        goto loc_1FDD5;
    Data0--;
    goto loc_1FDC8;

loc_1FDC3:;
    Data0++;
loc_1FDC8:;
    pSprite->field_52 = Data0;
loc_1FDD5:;
    if (!Data0)
        return;
loc_1FDDC:;
    pSprite->field_36 = 6;
}

void cFodder::Sprite_Handle_Troop_Direct_TowardWeaponTarget_WithRestore(sSprite* pSprite) {

    int16 Data0 = pSprite->field_2E;
    int16 Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Set_Direction_To_Next(pSprite);

    mStoredSpriteFrame = pSprite->field_A;

    Sprite_Handle_Troop_Animation(pSprite);
    if (!pSprite->field_43)
        return;

    pSprite->field_A = mStoredSpriteFrame;
}

void cFodder::Sprite_Create_Player_Shadow(sSprite* pSprite) {
    int16 Data0 = 0;

    sSprite* Data2C, *Data30;

    if (pSprite->field_5C == 0)
        return;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data2C->field_1A_sprite = pSprite;
    Data2C->field_6A_sprite = pSprite;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_0 += 4;
    Data2C->field_8 = 0x8D;
    Data2C->field_A = 2;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_6 = pSprite->field_6;
    Data2C->field_18 = eSprite_Player_Shadow;
    Data2C->field_62 = pSprite->field_18;
    Data2C->field_20 = 0;
    Data2C->field_32 = -1;
    Data2C->field_2C = eSprite_Draw_First;
    Data2C->field_52 = 0;
}

int16 cFodder::Sprite_Handle_Troop_Get_Frame_For_Direction(sSprite* pSprite) {
    int16 FrameNumber = pSprite->field_10;

    FrameNumber >>= 5;
    FrameNumber -= 1;
    FrameNumber ^= 0x0F;
    FrameNumber &= 0x0E;

    return FrameNumber;
}

void cFodder::Sprite_Handle_Player_Close_To_SquadMember(sSprite* pSprite) {

    // Not Human?
    if (pSprite->field_22 != eSprite_PersonType_Human) {
        pSprite->field_4D = 0;
        return;
    }

    word_3B2F3 = 0;

    // Find this sprite in the squad
    sSprite** PreviousMember = mSquads[pSprite->field_32];

    // Loop until we find this squad member
    for (;;) {
        if (*PreviousMember == INVALID_SPRITE_PTR)
            return;

        sSprite* eax = *PreviousMember;
        ++PreviousMember;

        // Found this troop?
        if (pSprite == eax)
            break;
    }

    // Get the previous squad member
    --PreviousMember;

    // If PreviousMember is Squad Leader, return
    if (mSquads[pSprite->field_32] == PreviousMember)
        return;

    // Get the previous members, previous member
    PreviousMember--;

    //seg005:1A13
    sSprite* Data24 = *PreviousMember;

    // Near the other member?
    if (pSprite->field_0 + 8 < Data24->field_0)
        return;

    if (Data24->field_0 + 8 < pSprite->field_0)
        return;

    if (Data24->field_4 + 2 < pSprite->field_4 - 2)
        return;

    if (pSprite->field_4 + 2 < Data24->field_4 - 2)
        return;

    // In Vehicle?
    if (Data24->field_6E)
        return;

    // Different Walk Target?
    int16 Data1C = pSprite->field_40;
    if (Data1C != Data24->field_40)
        return;

    // Restore the previous XY
    Sprite_XY_Restore(pSprite);
    mSprite_Bumped_Into_SquadMember = -1;

    pSprite->field_43 = 1;
    word_3B2F3 = -1;
}

int16 cFodder::Sprite_Create_Enemy(sSprite* pSprite, sSprite*& pData2C) {

    if (mPhase_Complete || mTroops_Enemy_Count >= mParams->mSpawnEnemyMax)
        return -1;
    int16 Data0 = 1;
    sSprite* Data30 = 0;

    if (Sprite_Get_Free_Max29(Data0, pData2C, Data30))
        return -1;

    //loc_21A1C:;
    pData2C->Clear();
    pData2C->field_18 = eSprite_Enemy;
    pData2C->field_0 = pSprite->field_0;
    pData2C->field_0 -= 6;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_4 += 4;
    pData2C->field_8 = 0x7C;
    pData2C->field_4A = 0;
    pData2C->field_22 = eSprite_PersonType_AI;

    Data0 = tool_RandomGet() & 0xFF;
    Data0 += 0x78;
    Data0 = -Data0;
    pData2C->field_5E = Data0;
    Data0 = tool_RandomGet();
    int16 Data4 = Data0;
    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pData2C->field_10 = Data0;
    Data4 &= 0x0F;
    Data4 += 8;
    pData2C->field_44 = (int8)Data4;
    Sprite_Enemy_Aggression_Update(pData2C);

    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count += 1;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count &= 0x0F;
    if (!mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count && mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax < 0x1E)
        ++mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;

    ++mTroops_Enemy_Count;
    return 0;
}

void cFodder::Sprite_Enemy_Aggression_Update(sSprite* pData2C) {
    int16 Data8 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext;
    pData2C->field_62 = Data8;

    int16 Data4 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement;
    Data8 += Data4;

    if (Data4 < 0)
        goto loc_21C42;

    if (Data8 < mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax)
        goto loc_21C5E;

    Data8 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
    Data4 = -Data4;
    goto loc_21C5E;

loc_21C42:;
    if (Data8 < 0)
        goto loc_21C52;

    if (Data8 > mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin)
        goto loc_21C5E;

loc_21C52:;
    Data8 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin;
    Data4 = -Data4;

loc_21C5E:;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement = Data4;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext = Data8;
}

void cFodder::Sprite_Create_Rank() {
    sSprite* Sprite = 0, *Data30 = 0;
    int16 Data0 = 1;

    if (Sprite_Get_Free_Max42(Data0, Sprite, Data30))
        return;

    Sprite->field_0 = 0;
    Sprite->field_4 = 0;
    Sprite->field_8 = 0x7C;
    Sprite->field_A = 0;
    Sprite->field_10 = 1;
    Sprite->field_18 = eSprite_Player_Rank;
    Sprite->field_22 = eSprite_PersonType_None;
    Sprite->field_3A = 0;
}
