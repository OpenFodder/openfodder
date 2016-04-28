/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield
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

#define SpriteFunction(name) mSprite_Function[eSprite_##name] = &cFodder::Sprite_Handle_##name;


void cFodder::Sprite_Table_Setup() {

	for (int16 x = 0; x < 117; ++x) {
		mSprite_Function[x] = &cFodder::Sprite_Handle_Null;
	}

	/* 00 */
	SpriteFunction( Player );
	SpriteTableFunction( 0x01,						Sprite_Handle_Null );
	SpriteFunction( Grenade );
	SpriteFunction( ShadowSmall );

	/* 04 */
	SpriteFunction( Null );
	SpriteFunction( Enemy );
	SpriteFunction( Bullet );
	SpriteTableFunction( 0x07,						Sprite_Handle_Null );

	/* 08 */
	SpriteTableFunction( 0x08,						Sprite_Handle_Null );
	SpriteTableFunction( 0x09,						Sprite_Handle_Null );
	SpriteTableFunction( 10,						Sprite_Handle_Null );
	SpriteTableFunction( 11,						Sprite_Handle_Null );

	/* 12 */
	SpriteFunction( Explosion );
	SpriteFunction( Shrub );
	SpriteFunction( Tree );
	SpriteFunction( BuildingRoof );

	/* 16 */
	SpriteTableFunction( 16,						sub_1AC03 );
	SpriteFunction( Shrub2 );
	SpriteFunction( Waterfall );
	SpriteFunction( Bird2_Left );

	/* 20 */
	SpriteFunction( BuildingDoor );
	SpriteFunction( Player_Rank );
	SpriteTableFunction( 22,						sub_1AF78 );
	SpriteTableFunction( 23,						sub_1B07C );

	/* 24 */
	SpriteTableFunction( 24,						sub_1B0C0 );
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
	SpriteFunction( Helicopter_Grenade2_Enemy );
	SpriteFunction( Helicopter_Missile_Enemy );
	
	/* 44 */
	SpriteFunction( Helicopter_Homing_Enemy );
	SpriteFunction( Missile );
	SpriteFunction( MissileHoming );
	SpriteTableFunction( 47,										sub_1C1C0 );

	/* 48 */
	SpriteTableFunction( 48,										sub_1C268 );
	SpriteFunction( Helicopter_Grenade2_Human );
	SpriteFunction( Helicopter_Grenade_Human );
	SpriteFunction( Helicopter_Missile_Human );

	/* 52 */
	SpriteFunction( Helicopter_Homing_Human );
	SpriteTableFunction( 53,										sub_1C2D2 );
	SpriteFunction( Mine );
	SpriteFunction( Mine2 );

	/* 56 */
	SpriteFunction( Spike );
	SpriteTableFunction( 57,										sub_1C4C2 );
	SpriteFunction( Text_Try );
	SpriteFunction( Text_Again );

	/* 60 */
	SpriteFunction( BoilingPot );
	SpriteFunction( Indigenous );
	SpriteFunction( Indigenous2 );
	SpriteFunction( VehicleNoGun_Human );

	/* 64 */
	SpriteFunction( VehicleGun_Human );
	SpriteFunction( Tank_Human );
	SpriteFunction( Bird_Left );
	SpriteFunction( Bird_Right );

	/* 68 */
	SpriteFunction( Seal );
	SpriteFunction( Tank_Enemy );
	SpriteFunction( Indigenous_Spear );
	SpriteTableFunction( 71,										sub_1CE80 );

	/* 72 */
	SpriteFunction( Hostage );
	SpriteFunction( Hostage_Rescue_Tent );
	SpriteFunction( Door_Indigenous );
	SpriteFunction( Door2_Indigenous );

	/* 76 */
	SpriteFunction( Door_Indigenous_Spear );
	SpriteTableFunction( 77,										sub_1D4D2 );
	SpriteFunction( Turret_Missile_Human );
	SpriteFunction( Turret_Missile2_Human );

	/* 80 */
	SpriteFunction( VehicleNoGun_Enemy );
	SpriteFunction( VehicleGun_Enemy );
	SpriteFunction( Vehicle_Unk_Enemy );
	SpriteFunction( Indigenous_Invisible );

	/* 84 */
	SpriteFunction( Turret_Missile_Enemy );
	SpriteFunction( Turret_Missile2_Enemy );
	SpriteTableFunction( 86,										sub_1D7DD );
	SpriteTableFunction( 87,										sub_1D802 );

	/* 88 */
	SpriteFunction( BuildingDoor3 );
	SpriteFunction( Explosion2 );
	SpriteFunction( OpenCloseDoor );
	SpriteFunction( Seal_Mine);

	/* 92 */
	SpriteFunction( Spider_Mine );
	SpriteFunction( Bonus_RankToGeneral );
	SpriteFunction( Bonus_Rockets );
	SpriteFunction( Player_Rocket );

	/* 96 */
	SpriteFunction( Bonus_RocketsAndGeneral );
	SpriteFunction( MissileHoming2 );
	SpriteFunction( Bonus_SquadGeneralRockets );
	SpriteFunction( Helicopter_CallPad );

	/* 100 */
	SpriteTableFunction( 100,										sub_1DFD2 );
	SpriteFunction( Helicopter_Grenade2_Human_Called );
	SpriteFunction( Helicopter_Grenade_Human_Called );
	SpriteFunction( Helicopter_Missile_Human_Called );

	/* 104 */
	SpriteFunction( Helicopter_Homing_Human_Called );
	SpriteFunction( Turret_HomingMissile_Enemy );
	SpriteFunction( Hostage_2 );
	SpriteFunction( Helicopter_Homing_Enemy2 );

	/* 108 */
	SpriteFunction( Computer_1 );
	SpriteFunction( Computer_2 );
	SpriteFunction( Computer_3 );
	// End of Cannon Fodder Sprites

	// Cannon Foodder 2 only Sprites

}

const int16 mEnemy_Unit_Types[] = {
	eSprite_Enemy, eSprite_Enemy_Rocket,	eSprite_Helicopter_Grenade_Enemy,	eSprite_Helicopter_Grenade2_Enemy,
	eSprite_Helicopter_Missile_Enemy,		eSprite_Helicopter_Homing_Enemy,	eSprite_Tank_Enemy,
	eSprite_VehicleNoGun_Enemy,				eSprite_VehicleGun_Enemy,			eSprite_Vehicle_Unk_Enemy,
	eSprite_Turret_Missile_Enemy,			eSprite_Turret_Missile2_Enemy,		eSprite_Turret_HomingMissile_Enemy,
	eSprite_Computer_1,						eSprite_Computer_2,					eSprite_Computer_3,

	-1
};

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
	-1
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
	24
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
	3
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
	1
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
