/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield <robcrossfield@gmail.com>
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

enum eSprites {
	eSprite_Player							= 0,
	eSprite_Grenade							= 2,
	eSprite_ShadowSmall						= 3,
	eSprite_Null							= 4,
	eSprite_Enemy							= 5,
	eSprite_Bullet							= 6,

	eSprite_Explosion						= 12,
	eSprite_Shrub							= 13,
	eSprite_Tree							= 14,
	eSprite_BuildingRoof					= 15,
	eSprite_Shrub2							= 17,
	eSprite_Waterfall						= 18,
	eSprite_Bird2_Left						= 19,

	eSprite_BuildingDoor					= 20,
	eSprite_Player_Rank						= 21,
	eSprite_BuildingDoor2					= 25,
	eSprite_Floating_Dead_Soldier			= 27,
	eSprite_Text_Complete					= 28,
	eSprite_Text_Mission					= 29,
	eSprite_Text_Phase						= 30,
	eSprite_Rocket							= 33,
	eSprite_Text_GameOver					= 34,
	eSprite_Shadow							= 35,
	eSprite_Enemy_Rocket					= 36,
	eSprite_GrenadeBox						= 37,
	eSprite_RocketBox						= 38,
	eSprite_Building_Explosion				= 39,
	eSprite_Helicopter_Grenade_Enemy			= 40,
	eSprite_Flashing_Light						= 41,
	eSprite_Helicopter_Grenade2_Enemy			= 42,
	eSprite_Helicopter_Missile_Enemy			= 43,
	eSprite_Helicopter_Homing_Enemy				= 44,
	eSprite_Missile								= 45,
	eSprite_MissileHoming						= 46,
	eSprite_Helicopter_Grenade2_Human			= 49,
	eSprite_Helicopter_Grenade_Human			= 50,
	eSprite_Helicopter_Missile_Human			= 51,
	eSprite_Helicopter_Homing_Human				= 52,
	eSprite_Mine								= 54,
	eSprite_Mine2								= 55,
	eSprite_Spike								= 56,
	eSprite_Text_Try							= 58,
	eSprite_Text_Again							= 59,

	eSprite_BoilingPot							= 60,
	eSprite_Indigenous							= 61,
	eSprite_Indigenous2							= 62,
	eSprite_VehicleNoGun_Human					= 63,		// Skidu
	eSprite_VehicleGun_Human					= 64,
	eSprite_Tank_Human							= 65,
	eSprite_Bird_Left							= 66,
	eSprite_Bird_Right							= 67,
	eSprite_Seal								= 68,
	eSprite_Tank_Enemy							= 69,

	eSprite_Indigenous_Spear					= 70,
	eSprite_Hostage								= 72,
	eSprite_Hostage_Rescue_Tent					= 73,
	eSprite_Door_Indigenous						= 74,
	eSprite_Door2_Indigenous					= 75,
	eSprite_Door_Indigenous_Spear				= 76,
	eSprite_Turret_Missile_Human				= 78,
	eSprite_Turret_Missile2_Human				= 79,

	eSprite_VehicleNoGun_Enemy					= 80,
	eSprite_VehicleGun_Enemy					= 81,
	eSprite_Vehicle_Unk_Enemy					= 82,
	eSprite_Indigenous_Invisible				= 83,
	eSprite_Turret_Missile_Enemy				= 84,
	eSprite_Turret_Missile2_Enemy				= 85,
	eSprite_BuildingDoor3						= 88,
	eSprite_Explosion2							= 89,

	eSprite_Seal_Mine							= 91,
	eSprite_Spider_Mine							= 92,
	eSprite_Bonus_RankToGeneral					= 93,
	eSprite_Bonus_Rockets						= 94,
	eSprite_Player_Rocket						= 95,
	eSprite_Bonus_RocketsAndGeneral				= 96,
	eSprite_MissileHoming2						= 97,
	eSprite_Bonus_SquadGeneralRockets			= 98,
	eSprite_Helicopter_CallPad					= 99,

	eSprite_Helicopter_Grenade2_Human_Called	= 101,
	eSprite_Helicopter_Grenade_Human_Called		= 102,
	eSprite_Helicopter_Missile_Human_Called		= 103,
	eSprite_Helicopter_Homing_Human_Called		= 104,
	eSprite_Turret_HomingMissile_Enemy			= 105,
	eSprite_Hostage_2							= 106,
	eSprite_Helicopter_Homing_Enemy2			= 107,
	eSprite_Computer_1							= 108,
	eSprite_Computer_2							= 109,
	eSprite_Computer_3							= 110,
};

struct sSprite {
	int16 field_0;		// X
	int16 field_2;		
	int16 field_4;		// Y
	int16 field_6;
	int16 field_8;		// Animation   mSpriteDataPtr[field_8][field_a]
	int16 field_A;		// FrameNumber mSpriteDataPtr[field_8][field_a]
	int16 field_C;
	int16 field_E;
	int16 field_10;
	int16 field_12;
	int16 field_14;
	int16 field_16;
	int16 field_18;
	int32* field_1A;

	int16 field_1E;
	int16 field_20;
	int16 field_22;
	int16 field_24;
	int16 field_26;	// Target X
	int16 field_28;	// Target Y
	int16 field_2A;
	int16 field_2C;
	int16 field_2E;
	int16 field_30;
	int16 field_32;
	int16 field_34;
	int16 field_36;
	int16 field_38;
	int16 field_3A;
	int16 field_3C;
	int16 field_3E;
	int16 field_40;
	int8 field_42;
	int8 field_43;
	int8 field_44;
	int8 field_45;
	int32* field_46;	//sSquad_Member mostly
	int16 field_4A;		// Player: Fire if mouse pressed
	int8 field_4C;
	int8 field_4D;
	int8 field_4E;
	int8 field_4F;
	int16 field_50;
	int16 field_52;
	int8 field_54;
	int8 field_55;
	int8 field_56;
	int8 field_57;
	int8 field_58;
	int8 field_59;
	int8 field_5A;
	int8 field_5B;
	int8 field_5C;
	int8 field_5D;
	int16 field_5E;			// Byte counter inside mSprites
	int8 field_60;
	int8 field_61;
	int16 field_62;
	int8 field_64;
	int8 field_65;
	sSprite* field_66;
	sSprite* field_6A;
	int8 field_6E;
	int8 field_6F;
	sSprite* field_70;
	int8 field_74;
	int8 field_75;
};

extern const int16 mEnemy_Unit_Types[];

extern const int16 mSprite_Missile_CanLock[];
extern const int16 mSprite_Width[];
extern const int16 mSprite_Height_Top[];
extern const int16 mSprite_Height_Bottom[];
extern const int16 mSprite_Can_Be_RunOver[];
extern const int16 mSprite_Explosion_Area_PerFrame[];
extern const int16 mSprite_Explosion_Positions[];
extern const int16 mSprite_Turret_Positions[];
extern const int16 mSprite_Helicopter_Light_Positions[];
extern const int16 mSprite_Helicopter_Sounds[];
extern const int16 mSprite_SpiderMine_Frames[];
extern const int16 mSprite_Hostage_Frames[];
extern const int16 mSprite_Helicopter_CallPad_Frames[];
