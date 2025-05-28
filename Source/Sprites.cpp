/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

#define SpriteTableFunction(x, y) mSprite_Function[x] = &cFodder::y;

#define SpriteFunction(name) mSprite_Function[eSprite_##name] = &cFodder::Sprite_Handle_##name; mSprite_Names[eSprite_##name] = #name;


void cFodder::Sprite_Table_Setup() {

	for (int16 x = 0; x < 118; ++x) {
		mSprite_Function[x] = &cFodder::Sprite_Handle_Null;
	}

	/* 00 */
	SpriteFunction( Player );
	SpriteTableFunction( 1,							Sprite_Handle_Null );
	SpriteFunction( Grenade );
	SpriteFunction( ShadowSmall );

	/* 04 */
	SpriteFunction( Null );
	SpriteFunction( Enemy );
	SpriteFunction( Bullet );
	SpriteTableFunction( 7,							Sprite_Handle_Null );

	/* 08 */
	SpriteTableFunction( 8,							Sprite_Handle_Null );
	SpriteTableFunction( 9,							Sprite_Handle_Null );
	SpriteTableFunction( 10,						Sprite_Handle_Null );
	SpriteTableFunction( 11,						Sprite_Handle_Null );

	/* 12 */
	SpriteFunction( Explosion );
	SpriteFunction( Shrub );
	SpriteFunction( Tree );
	SpriteFunction( BuildingRoof );

	/* 16 */
	SpriteFunction( Snowman );
	SpriteFunction( Shrub2 );
	SpriteFunction( Waterfall );
	SpriteFunction( Bird2_Left );

	/* 20 */
	SpriteFunction( BuildingDoor );
	SpriteFunction( Player_Rank );
	SpriteFunction( Player_Shadow );
	SpriteFunction( BloodTrail );

	/* 24 */
	SpriteFunction( GroundHole );
	SpriteFunction( BuildingDoor2 );
	SpriteTableFunction( 26,						Sprite_Handle_Null );
	SpriteFunction( Floating_Dead_Soldier );
	
	/* 28 */
	SpriteFunction( Text_Complete );
	SpriteFunction( Text_Mission );
	SpriteFunction( Text_Phase );
	SpriteTableFunction( 31,						Sprite_Handle_Null );

	/* 32 */
	SpriteTableFunction( 32,						Sprite_Handle_Null );
	SpriteFunction( Rocket );
	SpriteFunction( Text_GameOver );
	SpriteFunction( Shadow );

	/* 36 */
	SpriteFunction( Enemy_Rocket );
	SpriteFunction( GrenadeBox );
	SpriteFunction( RocketBox );
	SpriteFunction( Building_Explosion );

	/* 40 */
	SpriteFunction( Helicopter_Grenade_Enemy );
	SpriteFunction( Flashing_Light );
	SpriteFunction( Helicopter_Unarmed_Enemy );
	SpriteFunction( Helicopter_Missile_Enemy );
	
	/* 44 */
	SpriteFunction( Helicopter_Homing_Enemy );
	SpriteFunction( Missile );
	SpriteFunction( MissileHoming );
	SpriteFunction( Sparks );

	/* 48 */
	SpriteFunction( FireTrail );
	SpriteFunction( Helicopter_Grenade_Human );
	SpriteFunction( Helicopter_Unarmed_Human );
	SpriteFunction( Helicopter_Missile_Human );

	/* 52 */
	SpriteFunction( Helicopter_Homing_Human );
	SpriteFunction( Helicopter_PropCrash );
	SpriteFunction( Mine );
	SpriteFunction( Mine2 );

	/* 56 */
	SpriteFunction( Spike );
	SpriteFunction( Smoke );
	SpriteFunction( Text_Try );
	SpriteFunction( Text_Again );

	/* 60 */
	SpriteFunction( BoilingPot );
	SpriteFunction( Civilian );
	SpriteFunction( Civilian2 );
	SpriteFunction( VehicleNoGun_Human );

	/* 64 */
	SpriteFunction( VehicleGun_Human );
	SpriteFunction( Tank_Human );
	SpriteFunction( Bird_Left );
	SpriteFunction( Bird_Right );

	/* 68 */
	SpriteFunction( Seal );
	SpriteFunction( Tank_Enemy );
	SpriteFunction( Civilian_Spear );
	SpriteFunction( Civilian_Spear2 );

	/* 72 */
	SpriteFunction( Hostage );
	SpriteFunction( Hostage_Rescue_Tent );
	SpriteFunction( Door_Civilian );
	SpriteFunction( Door2_Civilian );

	/* 76 */
	SpriteFunction( Door_Civilian_Spear );
	SpriteFunction( Cannon );
	SpriteFunction( Turret_Missile_Human );
	SpriteFunction( Turret_Missile2_Human );

	/* 80 */
	SpriteFunction( VehicleNoGun_Enemy );
	SpriteFunction( VehicleGun_Enemy );
	SpriteFunction( Vehicle_Unk_Enemy );
	SpriteFunction( Civilian_Invisible );

	/* 84 */
	SpriteFunction( Turret_Missile_Enemy );
	SpriteFunction( Turret_Missile2_Enemy );
	SpriteFunction( Vehicle_Sinking_1 );
	SpriteFunction( Vehicle_Sinking_2 );

	/* 88 */
	SpriteFunction( BuildingDoor3 );
	SpriteFunction( Explosion2 );
	SpriteFunction( Door_Civilian_Rescue );
	SpriteFunction( Seal_Mine);

	/* 92 */
	SpriteFunction( Spider_Mine );
	SpriteFunction( Bonus_RankToGeneral );
	SpriteFunction( Bonus_Rockets );
	SpriteFunction( Bonus_Armour );

	/* 96 */
	SpriteFunction( Bonus_RankHomingInvin_SquadLeader );
	SpriteFunction( MissileHoming2 );
	SpriteFunction( Bonus_RankHomingInvin_Squad );
	SpriteFunction( Helicopter_CallPad );

	/* 100 */
	SpriteFunction( BuildingDoor_Reinforced );
	SpriteFunction( Helicopter_Grenade_Human_Called );
	SpriteFunction( Helicopter_Unarmed_Human_Called );
	SpriteFunction( Helicopter_Missile_Human_Called );

	/* 104 */
	SpriteFunction( Helicopter_Homing_Human_Called );
	SpriteFunction( Turret_HomingMissile_Enemy );
	SpriteFunction( Enemy_Leader );
	SpriteFunction( Helicopter_Homing_Enemy2 );

	/* 108 */
	SpriteFunction( Computer_1 );
	SpriteFunction( Computer_2 );
	SpriteFunction( Computer_3 );
	// End of Cannon Fodder Sprites

	//  Cannon Foodder 2 only Sprites
    /* 111 */
    SpriteFunction( UFO_Callpad);
    SpriteFunction( Turret_Cannon_Invulnerable );
    SpriteFunction( Turret_Missile_Invulnerable );

    SpriteFunction( Looping_Vehicle_Left );
    SpriteFunction( Looping_Vehicle_Right );
    
    /* 116 */
    SpriteFunction( Looping_Vehicle_Up );
    SpriteFunction( Looping_Vehicle_Down );

	//  End Cannon Fodder 2 Sprites
}

// Types of units which are hostile towards the Player
const int16 mEnemy_Unit_Types[] = {
	eSprite_Enemy, eSprite_Enemy_Rocket,	eSprite_Helicopter_Grenade_Enemy,	eSprite_Helicopter_Unarmed_Enemy,
	eSprite_Helicopter_Missile_Enemy,		eSprite_Helicopter_Homing_Enemy,	eSprite_Tank_Enemy,
	eSprite_VehicleNoGun_Enemy,				eSprite_VehicleGun_Enemy,			eSprite_Vehicle_Unk_Enemy,
	eSprite_Turret_Missile_Enemy,			eSprite_Turret_Missile2_Enemy,		eSprite_Turret_HomingMissile_Enemy,
	eSprite_Computer_1,						eSprite_Computer_2,					eSprite_Computer_3,
   
    // CF2 sprites
    eSprite_Turret_Cannon_Invulnerable, eSprite_Turret_Missile_Invulnerable,
	-1
};

// Sprites which missiles can lock on to
const int16 mSprite_Missile_CanLock[] = {
	0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
	0, 0, 0, -1, -1, -1, -1, -1, 0, 0,
	-1, 0, 0, 0, 0, -1, 0, -1, 0, 0,
	0, 0, 0, 0, 0, 0, -1, 0, 0, 0,
	-1, 0, -1, -1, -1, 0, -1, 0, 0, 0,
	0, 0, 0, 0, -1, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, -1, -1,
	-1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	-1, -1, -1, 0, -1, -1, 0, 0, -1, 0,
	0, -1, -1, 0, 0, 0, 0, 0, 0, 0,
	-1, 0, 0, 0, 0, -1, -1, -1, -1, -1,
	-1,

    // CF2: 111-118
    0, -1, -1, -1, -1, -1, -1
};

const int16 mSprite_Width[] = {
	10, 0, 6, 0, 0, 10, 0, 24, 0, 0,
	0, 0, 0, 20, 10, 16, 10, 20, 0, 0,
	20, 0, 0, 0, 0, 20, 0, 10, 0, 0,
	0, 24, 0, 0, 0, 0, 10, 10, 10, 0,
	24, 0, 24, 24, 24, 0, 16, 0, 0, 24,
	24, 24, 24, 0, 10, 10, 0, 0, 0, 0,
	0, 10, 10, 24, 24, 24, 0, 0, 10, 24,
	10, 0, 10, 10, 20, 20, 20, 0, 24, 24,
	24, 24, 24, 10, 24, 24, 0, 0, 20, 0,
	20, 10, 10, 0, 0, 0, 0, 0, 0, 0,
	20, 24, 24, 24, 24, 24, 10, 24, 24, 24,
	24,

    // CF2: 111-118
    0, 24, 24, 24, 24, 24, 24
};

const int16 mSprite_Height_Top[] = {
	2, 0, 2, 0, 0, 2, 0, 5, 0, 0,
	0, 0, 0, 2, 2, 2, 2, 2, 0, 0,
	12, 0, 0, 0, 0, 12, 0, 2, 0, 0,
	0, 5, 0, 0, 0, 0, 2, 2, 2, 0,
	5, 0, 5, 5, 5, 0, 5, 0, 0, 5,
	5, 5, 5, 0, 2, 2, 0, 0, 0, 0,
	0, 2, 2, 5, 5, 5, 0, 0, 2, 5,
	2, 0, 2, 2, 12, 12, 12, 0, 5, 5,
	5, 5, 5, 2, 5, 5, 0, 0, 12, 0,
	12, 2, 2, 0, 0, 0, 0, 0, 0, 0,
	12, 5, 5, 5, 5, 5, 2, 5, 5, 5,
	5,

    // CF2: 111-118
    0, 5, 5, 5, 5, 5, 5
};

const int16 mSprite_Height_Bottom[] = {
	3, 0, 2, 0, 0, 3, 0, 3, 0, 0,
	0, 0, 0, 2, 2, 2, 2, 2, 0, 0,
	3, 0, 0, 0, 0, 3, 0, 3, 0, 0,
	0, 3, 0, 0, 0, 0, 3, 3, 3, 0,
	3, 0, 3, 3, 3, 0, 3, 0, 0, 3,
	3, 3, 3, 0, 3, 3, 0, 0, 0, 0,
	0, 3, 3, 3, 3, 3, 0, 0, 3, 3,
	3, 0, 3, 3, 3, 3, 3, 0, 3, 3,
	3, 3, 3, 3, 3, 3, 0, 0, 3, 0,
	3, 3, 3, 0, 0, 0, 0, 0, 0, 0,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3,

    // CF2: 111-118
    0, 3, 3, 3, 3, 3, 3
};

const int16 mSprite_Can_Be_RunOver[] = {
	2, 0, 1, 0, 0, 2, 0, 1, 0, 0,
	2, 0, 0, 1, 1, 1, 1, 1, 0, 0,
	1, 0, 0, 0, 0, 1, 0, 2, 0, 0,
	0, 1, 0, 0, 0, 0, 2, 2, 2, 0,
	1, 0, 1, 1, 1, 0, 2, 0, 0, 1,
	1, 1, 1, 0, 2, 2, 0, 0, 0, 0,
	0, 2, 2, 1, 1, 1, 0, 0, 2, 1,
	2, 0, 2, 2, 1, 1, 1, 0, 1, 1,
	1, 1, 1, 2, 1, 1, 0, 0, 1, 0,
	1, 2, 2, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
	1,

    // CF2: 111-118
    0, 1, 1, 1, 1, 1, 1
};

const int16 mDirectionStepTable[] = {
	0, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1
};

const int16 mSprite_Explosion_Area_PerFrame[] = {
	5, 7, 0, 0, 1, 4, 2, 3, 1, 0,
	1, 0, 0, -1, -2, -2, -1, -5, -2, 1,
	1, 2, 1, 0, 1, -1, 0, -1
};

const int16 mSprite_Explosion_Positions[] = {
	0, 0, 16, 16, -16, -32, -16, 16, 32, -16,
	-32, 32, 32, -16, -16, 16, -16, -32, 48, 48,
	-64, -48, 48, -16, -32, 64, 48, -64, 0, 32,
	-48, -32, -16, 48, 64, -32, -32, 48, 0, -64,
	-32, 32, 48, 32, -48, -64, 64, 48, -64, 16,
	-32768
};

const int16 mSprite_Turret_Positions[] = {
	0, 7, -9, 6, -14, 3, -16, 0, -19, -7,
	-19, -12, -16, -16, -10, -20, -2, -22, 6, -20,
	12, -15, 16, -12, 18, -6, 16, 0, 12, 2,
	6, 5
};

const int16 mSprite_SpiderMine_Frames[] = {
	0, 0, 0, 0, 1, 1, 1, 1, 2, 2,
	2, 2, 1, 1, 1, 1
};

const int16 mSprite_Hostage_Frames[] = {
	0, 1, 2, 1
};

const int16 mSprite_Helicopter_Light_Positions[] = {
	14, -16, 11, -15, 10, -16, 10, -20, 10, -23,
	10, -23, 11, -24, 13, -25, 14, -25, 16, -25,
	18, -23, 18, -22, 19, -23, 19, -20, 19, -16,
	19, -15
};

const int16 mSprite_Helicopter_CallPad_Frames[] = {
	1, 2, 3, 2
};

const int16 mSprite_Helicopter_Sounds[] = {
	4, 51, 52, 53, 54, 4, 51, 52
};

const sWeaponData mSprite_Bullet_UnitData[] = {
	{ 70, 8, 7, 31, 0 },
	{ 75, 8, 7, 31, 0 },
	{ 80, 8, 7, 31, 0 },
	{ 85, 8, 7, 31, 0 },
	{ 85, 8, 6, 31, 0 },
	{ 100, 7, 6, 15, 0 },
	{ 100, 7, 6, 15, 0 },
	{ 105, 7, 6, 15, 0 },
	{ 105, 7, 5, 15, 0 },	// Rank 0
	{ 110, 7, 5, 15, 0 },	// Rank 1
	{ 130, 6, 5, 15, 0 },	// Rank 2
	{ 125, 7, 5, 7, 0 },	// Rank 3
	{ 125, 7, 4, 7, 0 },	// Rank 4
	{ 130, 7, 4, 7, 0 },	// Rank 5
	{ 115, 8, 4, 7, 0 },	// Rank 6
	{ 120, 8, 4, 7, 0 },	// Rank 7  - 15
	{ 120, 8, 3, 3, 0 },
	{ 125, 8, 3, 3, 0 },
	{ 130, 8, 3, 3, 0 },
	{ 120, 9, 3, 3, 0 },
	{ 120, 9, 3, 1, 0 },
	{ 125, 9, 3, 1, 0 },
	{ 130, 9, 3, 1, 0 },
	{ 120, 10, 3, 1, 0 },
	{ 125, 10, 3, 1, 0 },
	{ 130, 10, 3, 1, 0 }
};

const int16 mSprite_Player_MissionOver_Frames[] = {
	0, 1, 2, 3, 2, 1, 2
};

const int8 mSprite_Seal_AnimFrames[] = {

	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02,
	0x02, 0x02, 0x01, 0x01, 0x01, 0x01
};

const int16 mSprite_VehiclePosition_Mod[] = {
	11, 5, 18, 5, 6, 5, 10, 6, 0, 7,
	6, 9, -2, 5, -1, 9, -2, -6, -2, 2,
	-2, -6, -2, 1, 5, -4, 8, -8, 7, -4,
	14, -8, 10, -8, 18, -8, 16, -8, 22, -6,
	17, -8, 22, -2, 19, -2, 25, 2, 22, -4,
	22, 2, 22, -2, 20, 5, 24, 2, 22, 7,
	25, 5, 20, 8
};

const int16 mSprite_Computer_Animation[] = {
	147, 143, 145, 143
};

const int16 mSprite_Seal_Frames[] = {
	1, 1, 1, 0, 0, 0, 1, 1, 0, 0,
	1, 0, 1, 0, 1, 2, 3, -1
};

const int16 mSprite_Computer_Frames[] = {
	1, 0, 4, 0
};

const int8 mSprite_Civilian_Sound_Death[] = {
	0x0B, 0x0C, 0x0D, 0x14, 0x15, 0x16, 0x0D, 0x14
};

const int8 mSprite_Soldier_Unk[] = {
	-34, -1, -56, -1, 0x38, 0x00, 0x22, 0x00
};

const sSpriteSheet_pstuff mSpriteSheet_PStuff[209] = {
	{ 0, 79, 48, 13 },
	{ 48, 79, 48, 13 },
	{ 96, 79, 48, 13 },
	{ 0, 160, 48, 13 },
	{ 48, 160, 48, 13 },
	{ 96, 160, 48, 13 },
	{ 0, 160, 48, 13 },
	{ 96, 160, 48, 13 },
	{ 48, 160, 48, 13 },
	{ 48, 0, 16, 11 },

	{ 64, 0, 16, 11 },
	{ 80, 0, 16, 11 },
	{ 96, 0, 16, 11 },
	{ 112, 0, 16, 11 },
	{ 128, 0, 16, 11 },
	{ 144, 0, 16, 11 },
	{ 160, 0, 16, 11 },
	{ 176, 0, 16, 11 },
	{ 192, 0, 16, 11 },
	{ 208, 0, 16, 11 },

	{ 224, 0, 16, 11 },
	{ 240, 0, 16, 11 },
	{ 256, 0, 16, 11 },
	{ 272, 0, 16, 11 },
	{ 288, 0, 16, 11 },
	{ 304, 0, 16, 11 },
	{ 0, 12, 16, 11 },
	{ 16, 12, 16, 11 },
	{ 32, 12, 16, 11 },
	{ 48, 12, 16, 11 },

	{ 64, 12, 16, 11 },
	{ 80, 12, 16, 11 },
	{ 96, 12, 16, 11 },
	{ 112, 12, 16, 11 },
	{ 128, 12, 16, 11 },
	{ 64, 113, 16, 15 },
	{ 80, 113, 16, 15 },
	{ 0, 113, 16, 15 },
	{ 16, 113, 16, 15 },
	{ 32, 113, 16, 15 },

	{ 48, 113, 16, 15 },
	{ 0, 24, 16, 7 },
	{ 16, 24, 16, 7 },
	{ 32, 24, 16, 7 },
	{ 48, 24, 16, 7 },
	{ 64, 24, 16, 7 },
	{ 80, 24, 16, 7 },
	{ 96, 24, 16, 7 },
	{ 112, 24, 16, 7 },
	{ 128, 24, 16, 7 },
	{ 144, 24, 16, 7 },
	{ 160, 24, 16, 7 },
	{ 176, 24, 16, 7 },
	{ 192, 24, 16, 7 },
	{ 208, 24, 16, 7 },
	{ 224, 24, 16, 7 },
	{ 240, 24, 16, 7 },
	{ 256, 24, 16, 7 },
	{ 272, 24, 16, 7 },
	{ 288, 24, 16, 7 },
	{ 304, 24, 16, 7 },
	{ 0, 32, 16, 7 },
	{ 16, 32, 16, 7 },
	{ 32, 32, 16, 7 },
	{ 48, 32, 16, 7 },
	{ 64, 32, 16, 7 },
	{ 80, 32, 16, 7 },
	{ 96, 32, 16, 7 },
	{ 112, 32, 16, 7 },
	{ 128, 32, 16, 7 },
	{ 144, 32, 16, 7 },
	{ 160, 32, 16, 7 },
	{ 176, 32, 16, 7 },
	{ 192, 32, 16, 7 },
	{ 208, 32, 16, 7 },
	{ 224, 32, 16, 7 },
	{ 240, 32, 16, 7 },
	{ 0, 40, 16, 7 },
	{ 16, 40, 16, 7 },
	{ 32, 40, 16, 7 },
	{ 48, 40, 16, 7 },
	{ 64, 40, 16, 7 },
	{ 80, 40, 16, 7 },
	{ 96, 40, 16, 7 },
	{ 112, 40, 16, 7 },
	{ 128, 40, 16, 7 },
	{ 144, 40, 16, 7 },
	{ 160, 40, 16, 7 },
	{ 176, 40, 16, 7 },
	{ 192, 40, 16, 7 },
	{ 208, 40, 16, 7 },
	{ 224, 40, 16, 7 },
	{ 240, 40, 16, 7 },
	{ 256, 40, 16, 7 },
	{ 272, 40, 16, 7 },
	{ 288, 40, 16, 7 },
	{ 304, 40, 16, 7 },
	{ 0, 48, 16, 7 },
	{ 16, 48, 16, 7 },
	{ 32, 48, 16, 7 },
	{ 48, 48, 16, 7 },
	{ 64, 48, 16, 7 },
	{ 80, 48, 16, 7 },
	{ 96, 48, 16, 7 },
	{ 112, 48, 16, 7 },
	{ 128, 48, 16, 7 },
	{ 144, 48, 16, 7 },
	{ 160, 48, 16, 7 },
	{ 176, 48, 16, 7 },
	{ 192, 48, 16, 7 },
	{ 208, 48, 16, 7 },
	{ 224, 48, 16, 7 },
	{ 240, 48, 16, 7 },
	{ 0, 56, 16, 7 },
	{ 16, 56, 16, 7 },
	{ 32, 56, 16, 7 },
	{ 48, 56, 16, 7 },
	{ 64, 56, 16, 7 },
	{ 80, 56, 16, 7 },
	{ 96, 56, 16, 7 },
	{ 112, 56, 16, 7 },
	{ 128, 56, 16, 7 },
	{ 144, 56, 16, 7 },
	{ 160, 56, 16, 7 },
	{ 176, 56, 16, 7 },
	{ 192, 56, 16, 7 },
	{ 208, 56, 16, 7 },
	{ 224, 56, 16, 7 },
	{ 240, 56, 16, 7 },
	{ 256, 56, 16, 7 },
	{ 272, 56, 16, 7 },
	{ 288, 56, 16, 7 },
	{ 304, 56, 16, 7 },
	{ 0, 64, 16, 7 },
	{ 16, 64, 16, 7 },
	{ 32, 64, 16, 7 },
	{ 48, 64, 16, 7 },
	{ 64, 64, 16, 7 },
	{ 80, 64, 16, 7 },
	{ 96, 64, 16, 7 },
	{ 112, 64, 16, 7 },
	{ 128, 64, 16, 7 },
	{ 144, 64, 16, 7 },
	{ 160, 64, 16, 7 },
	{ 176, 64, 16, 7 },
	{ 192, 64, 16, 7 },
	{ 208, 64, 16, 7 },
	{ 224, 64, 16, 7 },
	{ 240, 64, 16, 7 },
	{ 160, 72, 32, 19 },
	{ 192, 72, 32, 19 },
	{ 224, 72, 32, 19 },
	{ 256, 72, 32, 19 },
	{ 288, 72, 32, 19 },
	{ 0, 93, 32, 19 },
	{ 32, 93, 32, 19 },
	{ 64, 93, 32, 19 },
	{ 96, 93, 32, 19 },
	{ 128, 93, 32, 19 },
	{ 0, 140, 32, 19 },
	{ 32, 140, 32, 19 },
	{ 64, 140, 32, 19 },
	{ 96, 140, 32, 19 },
	{ 128, 140, 32, 19 },
	{ 160, 140, 32, 19 },
	{ 192, 140, 32, 19 },
	{ 224, 140, 32, 19 },
	{ 256, 140, 32, 19 },
	{ 288, 140, 32, 19 },
	{ 0, 0, 48, 11 },
	{ 0, 0, 48, 11 },
	{ 144, 12, 48, 11 },
	{ 192, 12, 48, 11 },
	{ 96, 113, 48, 13 },
	{ 144, 113, 48, 13 },
	{ 0, 72, 16, 6 },
	{ 16, 72, 16, 6 },
	{ 32, 72, 16, 6 },
	{ 48, 72, 16, 6 },
	{ 64, 72, 16, 6 },
	{ 80, 72, 16, 6 },
	{ 96, 72, 16, 6 },
	{ 112, 72, 16, 6 },
	{ 128, 72, 16, 6 },
	{ 144, 72, 16, 6 },
	{ 0, 131, 16, 6 },
	{ 16, 131, 16, 6 },
	{ 32, 131, 16, 6 },
	{ 48, 131, 16, 6 },
	{ 64, 131, 16, 6 },
	{ 80, 131, 16, 6 },
	{ 96, 131, 16, 6 },
	{ 112, 131, 16, 6 },
	{ 128, 131, 16, 6 },
	{ 144, 131, 16, 6 },
	{ 192, 93, 16, 19 },
	{ 224, 93, 16, 19 },
	{ 208, 93, 16, 19 },
	{ 240, 93, 16, 19 },
	{ 256, 93, 16, 19 },
	{ 272, 93, 16, 19 },
	{ 288, 93, 16, 19 },
	{ 160, 93, 32, 19 },
	{ 0, 178, 48, 22 },				// Briefing Helicopter: Front
	{ 48, 178, 16, 14 },			// Briefing Helicopter: Tail
	/* 205 */{ 64, 175, 48, 2 },	// Briefing Helicopter: Blade-1
	/* 206 */{ 64, 178, 48, 2 },	// Briefing Helicopter: Blade-2
	/* 207 */{ 64, 181, 48, 2 },	// Briefing Helicopter: Blade-3
	{ 192, 113, 48, 10 }			// World Map Icon
};

// indexes inside mSpriteSheetTypes_InGame_PC
int16 mSprite_AnimationHumanPtrs[] = {
	0x00, 0x01, 0x02, 0x03,		// 0x00: Walk Anim
	0x04, 0x05, 0x06, 0x07,

	0x20, 0x21, 0x22, 0x23,		// 0x08: Death Anim
	0x24, 0x25, 0x26, 0x27,

	0x28, 0x29, 0x22, 0x2B,		// 0x10: Death-2 Anim
	0x2C, 0x2D, 0x2E, 0x2F,

	0x30, 0x31, 0x32, 0x33,		// 0x18: Death-3
	0x34, 0x35, 0x36, 0x37,

	0x18, 0x19, 0x1A, 0x1B,		// 0x20: Swim
	0x1C, 0x1D, 0x1E, 0x1F,

	0x08, 0x09, 0x0A, 0x0B,		// 0x28: Throw
	0x0C, 0x0D, 0x0E, 0x0F,

	0x10, 0x11, 0x12, 0x13,		// 0x30: Prone
	0x14, 0x15, 0x16, 0x17,

	0x39, 0x3A, 0x3B, 0x3C,		// 0x38: Fired Rocket
	0x3D, 0x3E, 0x3F, 0x40,

	0xB0, 0xB1, 0xB2, 0xB3,		// 0x40:
	0xB4, 0xB5, 0xB6, 0xB7
};

// Indexes inside mSpriteSheetTypes_InGame_PC
int16 mSprite_AnimationEnemyPtrs[] = {
	0x42, 0x43, 0x44, 0x45,   // Walk Anim
	0x46, 0x47, 0x48, 0x49, 

    0x62, 0x63,	0x64, 0x65,   // Death Anim
    0x66, 0x67, 0x68, 0x69, 

    0x6A, 0x6B, 0x64, 0x6D,   // Death-2 Anim
    0x6E, 0x6F, 0x70, 0x71, 
    
    0x72, 0x73, 0x74, 0x75,
    0x76, 0x77, 0x78, 0x79, 
    
    0x5A, 0x5B, 0x5C, 0x5D,   // Swim
	0x5E, 0x5F, 0x60, 0x61,

	0x4A, 0x4B, 0x4C, 0x4D,   // Throw
	0x4E, 0x4F, 0x50, 0x51,
    
    0x52, 0x53, 0x54, 0x55,   // Prone
	0x56, 0x57, 0x58, 0x59,
    
    0xA8, 0xA9, 0xAA, 0xAB,   // With Rocket Launcher
	0xAC, 0xAD, 0xAE, 0xAF,

    0xB8, 0xB9, 0xBA, 0xBB,   // Still with gun out
	0xBC, 0xBD, 0xBE, 0xBF,
};

const int16* mSprite_AnimationPtrs[] = {
	mSprite_AnimationHumanPtrs,	// Human
	mSprite_AnimationEnemyPtrs	// Computer
};

const int16 mSprite_Speed_Direction_Modifier[] = {
	0x18, 0x14, 0x0E, 0x0A, 0x08,
	0x0A, 0x0E, 0x14
};

const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf1 = {
	0,
	-1
};

const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf2 = {
	0, 1,
	-1
};

const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf3 = {
	0, 1, 0, 2,
	-1
};

const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf4 = {
	0, 1, 0, 2, 0, 3, 0, 2,
	-1
};

const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf5 = {
	0, 1, 0, 2, 0, 3, 0, 4, 0, 3,
	0, 2,
	-1,
};


const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf6 = {
	0, 1, 0, 2, 0, 3, 0, 4, 0, 5,
	0, 4, 0, 3, 0, 2,
	-1
};

const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf7 = {
	0, 1, 0, 2, 0, 3, 0, 4, 0, 5,
	0, 6, 0, 5, 0, 4, 0, 3, 0, 2,
	-1,
};

const std::vector<int16> mSoldier_Squad_Fire_RotationSquadOf8 = {
	0, 1, 0, 2, 0, 3, 0, 4, 0, 5,
	0, 6, 0, 5, 0, 4, 0, 3, 0, 2,
	-1
};

const std::vector<std::vector<int16>> mSoldier_Squad_Fire_RotationOrder = {
	mSoldier_Squad_Fire_RotationSquadOf1,
	mSoldier_Squad_Fire_RotationSquadOf2,
	mSoldier_Squad_Fire_RotationSquadOf3,
	mSoldier_Squad_Fire_RotationSquadOf4,
	mSoldier_Squad_Fire_RotationSquadOf5,
	mSoldier_Squad_Fire_RotationSquadOf6,
	mSoldier_Squad_Fire_RotationSquadOf7,
	mSoldier_Squad_Fire_RotationSquadOf8
};

void sSprite::Clear() {
	field_0 = -32768;
	field_2 = 0;
	field_4 = 0;
	field_6 = 0;
	field_8 = 0;
	field_A = 0;
	field_C = 0;
	field_E = 0;
	field_10 = 0;
	field_12 = 0;
	field_14 = 0;
	field_16 = 0;
	field_18 = 0;
	field_1A_sprite = 0;
	field_1E = 0;
	field_20 = 0;
	field_22 = eSprite_PersonType_Human;
	field_24 = 0;
	field_26 = 0;
	field_28 = 0;
	field_2A = 0;
	field_2C = eSprite_Draw_Second;
	field_2E = 0;
	field_30 = 0;
	field_32 = 0;
	field_34 = 0;
	field_36 = 0;
	field_38 = eSprite_Anim_None;
	field_3A = 0;
	field_3C = 0;
	field_3E = 0;
	field_40 = 0;
	field_42 = 0;
	field_43 = 0;
	field_44 = 0;
	field_45 = 0;
	field_46_sprite = 0;
	field_4A = 0;
	field_4C = 0;
	field_4D = 0;
	field_4E = 0;
	field_4F = 0;
	field_50 = 0;
	field_52 = 0;
	field_54 = 0;
	field_55 = 0;
	field_56 = 0;
	field_57 = 0;
	field_58 = 0;
	field_59 = 0;
	field_5A = 0;
	field_5B = 0;
	field_5C = 0;
	field_5D = 0;
	field_5E = 0;
	field_5E_Squad = 0;
	field_5E_SoldierAllocated = 0;
	field_60 = 0;
	field_61 = 0;
	field_62 = 0;
	field_64 = 0;
	field_65 = 0;
	field_66 = 0;
	field_6A_sprite = 0;
	field_6E = 0;
	field_6F = 0;
	field_70 = 0;
	field_74 = 0;
	field_75 = 0;
}
