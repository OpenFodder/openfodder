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
	SpriteTableFunction( 39,						sub_1BB11 );

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
	SpriteTableFunction( 90,										sub_1DA48 );
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
	SpriteFunction( Flash1 );
	SpriteFunction( Flash2 );
	SpriteFunction( Flash3 );
	// End of Cannon Fodder Sprites

	// Cannon Foodder 2 only Sprites

}

