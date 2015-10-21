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
	SpriteTableFunction( eSprite_Explosion,			Sprite_Handle_Explosion );
	SpriteTableFunction( eSprite_Shrub,				Sprite_Handle_Shrub );
	SpriteTableFunction( eSprite_Tree,				Sprite_Handle_Tree );
	SpriteTableFunction( eSprite_BuildingRoof,		Sprite_Handle_BuildingRoof );

	/* 16 */
	SpriteTableFunction( 16,						sub_1AC03 );
	SpriteTableFunction( eSprite_Shrub2,			Sprite_Handle_Shrub2 );
	SpriteTableFunction( eSprite_Waterfall,			Sprite_Handle_Waterfall );
	SpriteTableFunction( eSprite_Bird2_Left,		Sprite_Handle_Bird2_Left );

	/* 20 */
	SpriteTableFunction( eSprite_Door,				Sprite_Handle_BuildingDoor );
	SpriteTableFunction( eSprite_Rank,				Sprite_Handle_Player_Rank );
	SpriteTableFunction( 22,						sub_1AF78 );
	SpriteTableFunction( 23,						sub_1B07C );

	/* 24 */
	SpriteTableFunction( 24,						sub_1B0C0 );
	SpriteTableFunction( eSprite_Door2,				Sprite_Handle_Building_Door2 );
	SpriteTableFunction( 26,						Sprite_Handle_Null );
	SpriteTableFunction( eSprite_FloatingSoldier,	Sprite_Handle_Floating_Dead_Soldier );
	
	/* 28 */
	SpriteTableFunction( eSprite_Text_Complete,		Sprite_Handle_Text_Complete );
	SpriteTableFunction( eSprite_Text_Mission,		Sprite_Handle_Text_Mission );
	SpriteTableFunction( eSprite_Text_Phase,		Sprite_Handle_Text_Phase );
	SpriteTableFunction( 31,						Sprite_Handle_Null );

	/* 32 */
	SpriteTableFunction( 32,						Sprite_Handle_Null );
	SpriteTableFunction( eSprite_Rocket,			Sprite_Handle_Rocket );
	SpriteTableFunction( eSprite_Text_GameOver,		Sprite_Handle_Text_GameOver );
	SpriteTableFunction( eSprite_Shadow,			Sprite_Handle_Shadow );

	/* 36 */
	SpriteTableFunction( eSprite_Enemy_Rocket,		Sprite_Handle_Enemy_Rocket );
	SpriteTableFunction( eSprite_GrenadeBox,		Sprite_Handle_GrenadeBox );
	SpriteTableFunction( eSprite_RocketBox,			Sprite_Handle_RocketBox );
	SpriteTableFunction( 39,						sub_1BB11 );

	/* 40 */
	SpriteTableFunction( eSprite_Helicopter_Grenade_Enemy,			Sprite_Handle_Helicopter_Grenade_Enemy );
	SpriteTableFunction( eSprite_Flashing_Light,					Sprite_Handle_Flashing_Light );
	SpriteTableFunction( eSprite_Helicopter_Grenade2_Enemy,			Sprite_Handle_Helicopter_Grenade2_Enemy );
	SpriteTableFunction( eSprite_Helicopter_Missile_Enemy,			Sprite_Handle_Helicopter_Missile_Enemy );
	
	/* 44 */
	SpriteTableFunction( eSprite_Helicopter_Homing_Enemy,			Sprite_Handle_Helicopter_HomingMissile_Enemy );
	SpriteTableFunction( eSprite_Missile,							Sprite_Handle_Missile );
	SpriteTableFunction( eSprite_MissileHoming,						Sprite_Handle_MissileHoming );
	SpriteTableFunction( 47,										sub_1C1C0 );

	/* 48 */
	SpriteTableFunction( 48,										sub_1C268 );
	SpriteTableFunction( eSprite_Helicopter_Grenade2_Human,			Sprite_Handle_Helicopter_Grenade2_Human );
	SpriteTableFunction( eSprite_Helicopter_Grenade_Human,			Sprite_Handle_Helicopter_Grenade_Human );
	SpriteTableFunction( eSprite_Helicopter_Missile_Human,			Sprite_Handle_Helicopter_Missile_Human );

	/* 52 */
	SpriteTableFunction( eSprite_Helicopter_HomingMissile_Human,	Sprite_Handle_Helicopter_HomingMissile_Human );
	SpriteTableFunction( 53,										sub_1C2D2 );
	SpriteTableFunction( eSprite_Mine,								Sprite_Handle_Mine );
	SpriteTableFunction( eSprite_Mine2,								Sprite_Handle_Mine2 );

	/* 56 */
	SpriteTableFunction( eSprite_Spike,								Sprite_Handle_Spike );
	SpriteTableFunction( 57,										sub_1C4C2 );
	SpriteTableFunction( eSprite_Text_Try,							Sprite_Handle_Text_Try );
	SpriteTableFunction( eSprite_Text_Again,						Sprite_Handle_Text_Again );

	/* 60 */
	SpriteTableFunction( eSprite_BoilingPot,						Sprite_Handle_BoilingPot );
	SpriteTableFunction( eSprite_Indigenous,						Sprite_Handle_Indigenous );
	SpriteTableFunction( eSprite_Indigenous2,						Sprite_Handle_Indigenous2 );
	SpriteTableFunction( eSprite_Vehicle_Human,						Sprite_Handle_VehicleNoGun_Human );

	/* 64 */
	SpriteTableFunction( eSprite_VehicleGun_Human,					Sprite_Handle_VehicleGun_Human );
	SpriteTableFunction( eSprite_Tank_Human,						Sprite_Handle_Tank_Human );
	SpriteTableFunction( eSprite_Bird_Left,							Sprite_Handle_Bird_Left );
	SpriteTableFunction( eSprite_Bird_Right,						Sprite_Handle_Bird_Right );

	/* 68 */
	SpriteTableFunction( eSprite_Seal,								Sprite_Handle_Seal );
	SpriteTableFunction( eSprite_Tank_Enemy,						Sprite_Handle_Tank_Enemy );
	SpriteTableFunction( eSprite_Indigenous_Spear,					Sprite_Handle_Indigenous_Spear );
	SpriteTableFunction( 71,										sub_1CE80 );

	/* 72 */
	SpriteTableFunction( eSprite_Hostage,							Sprite_Handle_Hostage );
	SpriteTableFunction( eSprite_Hostage_Rescue_Tent,				Sprite_Handle_Hostage_Rescue_Tent );
	SpriteTableFunction( eSprite_Door_Indigenous,					Sprite_Handle_Door_Indigenous );
	SpriteTableFunction( eSprite_Door2_Indigenous,					Sprite_Handle_Door2_Indigenous );

	/* 76 */
	SpriteTableFunction( eSprite_Door_Indigenous_Spear,				Sprite_Handle_Door_Indigenous_Spear );
	SpriteTableFunction( 77,										sub_1D4D2 );
	SpriteTableFunction( eSprite_Turret_Missile_Human,				Sprite_Handle_Turret_Missile_Human );
	SpriteTableFunction( eSprite_Turret_Missile2_Human,				Sprite_Handle_Turret_Missile2_Human );

	/* 80 */
	SpriteTableFunction( eSprite_Vehicle_Enemy,						Sprite_Handle_VehicleNoGun_Enemey );
	SpriteTableFunction( eSprite_VehicleGun_Enemy,					Sprite_Handle_VehicleGun_Enemy );
	SpriteTableFunction( eSprite_Vehicle_Unk_Enemy,					Sprite_Handle_Vehicle_Unk_Enemy );
	SpriteTableFunction( 83,										sub_1D7B6 );

	/* 84 */
	SpriteTableFunction( eSprite_Turret_Missile_Enemy,				Sprite_Handle_Turret_Missile_Enemy );
	SpriteTableFunction( eSprite_Turret_Missile2_Enemy,				Sprite_Handle_Turret_Missile2_Enemy );
	SpriteTableFunction( 86,										sub_1D7DD );
	SpriteTableFunction( 87,										sub_1D802 );

	/* 88 */
	SpriteTableFunction( eSprite_Door3,								Sprite_Handle_Building_Door3 );
	SpriteTableFunction( eSprite_Explosion2,						Sprite_Handle_Explosion2 );
	SpriteTableFunction( 90,										sub_1DA48 );
	SpriteTableFunction( 91,										sub_1DACF );

	/* 92 */
	SpriteTableFunction( eSprite_Spider_Mine,						Sprite_Handle_Spider_Mine );
	SpriteTableFunction( eSprite_Bonus_General,						Sprite_Handle_RankToGeneral );
	SpriteTableFunction( eSprite_Bonus_Rockets,						Sprite_Handle_Set50Rockets );
	SpriteTableFunction( 95,										sub_1DD4C );

	/* 96 */
	SpriteTableFunction( eSprite_Bonus_RocketsAndGeneral,			Sprite_Handle_Set50RocketsAndRank );
	SpriteTableFunction( eSprite_MissileHoming2,					Sprite_Handle_MissileHoming2 );
	SpriteTableFunction( eSprite_Bonus_SquadGeneralRockets,			Sprite_Handle_SquadToGeneral_Give_50HomingMissiles );
	SpriteTableFunction( eSprite_HelicopterCallPad,					Sprite_Handle_Helicopter_CallPad );

	/* 100 */
	SpriteTableFunction( 100,										sub_1DFD2 );
	SpriteTableFunction( 101,										sub_1E004 );
	SpriteTableFunction( 102,										sub_1E00E );
	SpriteTableFunction( 103,										sub_1E018 );

	/* 104 */
	SpriteTableFunction( 104,										sub_1E022 );
	SpriteTableFunction( eSprite_Turret_HomingMissile_Enemy,		Sprite_Handle_Turret_HomingMissile );
	SpriteTableFunction( eSprite_Hostage2,							Sprite_Handle_Hostage_2 );
	SpriteTableFunction( eSprite_Helicopter_Homing_Enemy2,			Sprite_Handle_Helicopter_HomingMissile_Enemy2 );

	/* 108 */
	SpriteTableFunction( 108,										sub_1E036 );
	SpriteTableFunction( 109,										sub_1E042 );
	SpriteTableFunction( 110,										sub_1E04E );
	// End of Cannon Fodder Sprites

	// Cannon Foodder 2 only Sprites

}
