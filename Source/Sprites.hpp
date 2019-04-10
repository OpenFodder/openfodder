/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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
	eSprite_Snowman							= 16,
	eSprite_Shrub2							= 17,
	eSprite_Waterfall						= 18,
	eSprite_Bird2_Left						= 19,

	eSprite_BuildingDoor					= 20,
	eSprite_Player_Rank						= 21,
	eSprite_Player_Shadow					= 22,
	eSprite_BloodTrail						= 23,
	eSprite_GroundHole						= 24,
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
	eSprite_Sparks								= 47,
	eSprite_FireTrail							= 48,
	eSprite_Helicopter_Grenade2_Human			= 49,

	eSprite_Helicopter_Grenade_Human			= 50,
	eSprite_Helicopter_Missile_Human			= 51,
	eSprite_Helicopter_Homing_Human				= 52,
	eSprite_Helicopter_PropCrash				= 53,
	eSprite_Mine								= 54,
	eSprite_Mine2								= 55,
	eSprite_Spike								= 56,
	eSprite_Smoke								= 57,
	eSprite_Text_Try							= 58,
	eSprite_Text_Again							= 59,

	eSprite_BoilingPot							= 60,
	eSprite_Civilian							= 61,
	eSprite_Civilian2							= 62,
	eSprite_VehicleNoGun_Human					= 63,		// Skidu
	eSprite_VehicleGun_Human					= 64,
	eSprite_Tank_Human							= 65,
	eSprite_Bird_Left							= 66,
	eSprite_Bird_Right							= 67,
	eSprite_Seal								= 68,
	eSprite_Tank_Enemy							= 69,

	eSprite_Civilian_Spear					= 70,
	eSprite_Civilian_Spear2					= 71,
	eSprite_Hostage								= 72,
	eSprite_Hostage_Rescue_Tent					= 73,
	eSprite_Door_Civilian						= 74,
	eSprite_Door2_Civilian					= 75,
	eSprite_Door_Civilian_Spear				= 76,
	eSprite_Cannon								= 77,
	eSprite_Turret_Missile_Human				= 78,
	eSprite_Turret_Missile2_Human				= 79,

	eSprite_VehicleNoGun_Enemy					= 80,
	eSprite_VehicleGun_Enemy					= 81,
	eSprite_Vehicle_Unk_Enemy					= 82,
	eSprite_Civilian_Invisible				= 83,
	eSprite_Turret_Missile_Enemy				= 84,
	eSprite_Turret_Missile2_Enemy				= 85,
	eSprite_Vehicle_Sinking_1					= 86,
	eSprite_Vehicle_Sinking_2					= 87,
	eSprite_BuildingDoor3						= 88,
	eSprite_Explosion2							= 89,

	eSprite_Door_Civilian_Rescue				= 90,
	eSprite_Seal_Mine							= 91,
	eSprite_Spider_Mine							= 92,
	eSprite_Bonus_RankToGeneral					= 93,
	eSprite_Bonus_Rockets						= 94,
	eSprite_Bonus_Armour						= 95,
	eSprite_Bonus_RankHomingInvin_SquadLeader	= 96,
	eSprite_MissileHoming2						= 97,
	eSprite_Bonus_RankHomingInvin_Squad			= 98,
	eSprite_Helicopter_CallPad					= 99,

	eSprite_BuildingDoor_Reinforced				= 100,
	eSprite_Helicopter_Grenade2_Human_Called	= 101,
	eSprite_Helicopter_Grenade_Human_Called		= 102,
	eSprite_Helicopter_Missile_Human_Called		= 103,
	eSprite_Helicopter_Homing_Human_Called		= 104,
	eSprite_Turret_HomingMissile_Enemy			= 105,
	eSprite_Enemy_Leader						= 106,
	eSprite_Helicopter_Homing_Enemy2			= 107,
	eSprite_Computer_1							= 108,
	eSprite_Computer_2							= 109,
	eSprite_Computer_3							= 110,

    // Cannon Fodder 2
    eSprite_UFO_Callpad                         = 111,

    eSprite_Turret_Cannon_Invulnerable          = 112,
    eSprite_Turret_Missile_Invulnerable         = 113,
    eSprite_Looping_Vehicle_Left                = 114,
    eSprite_Looping_Vehicle_Right               = 115,

    eSprite_Looping_Vehicle_Up                  = 116,
    eSprite_Looping_Vehicle_Down                = 117
};

enum eSprite_Flags {
	eSprite_Flag_HomingMissiles = 0x01,
	eSprite_Flag_Invincibility  = 0x02,
};

enum eSprite_Anim {
	eSprite_Anim_None = 0x00,
	eSprite_Anim_Hit  = 0x01,
	eSprite_Anim_Hit2 = 0x02,
	eSprite_Anim_Hit3 = 0x03,
	eSprite_Anim_Die1 = 0x05,		// Cause a turret to explode, a soldier to be run over
	eSprite_Anim_Die2 = 0x06,
	eSprite_Anim_Die3 = 0x07,
	eSprite_Anim_Die4 = 0x09,
	eSprite_Anim_Die5 = 0x0A,

	eSprite_Anim_Slide1 = 0x32,
	eSprite_Anim_Slide2 = 0x33,
	eSprite_Anim_Slide3 = 0x34,
	eSprite_Anim_Vehicle_Enter  = 0x5A,
	eSprite_Anim_Vehicle_Inside = 0x5B
};

enum eSprite_PersonType {
	eSprite_PersonType_None = -1,
	eSprite_PersonType_Human = 0,
	eSprite_PersonType_AI = 1,
	eSprite_PersonType_Native = 2
};

enum eVehicles {
    eVehicle_Turret_Cannon = 0,
    eVehicle_Turret_Missile = 1,
    eVehicle_Jeep = 2,
    eVehicle_JeepRocket = 3,
    eVehicle_Tank = 4,
    eVehicle_Helicopter = 5,
    eVehicle_Helicopter_Grenade = 6,
    eVehicle_Helicopter_Missile = 7,
    eVehicle_Helicopter_Homing = 8,
    eVehicle_Turret_Homing = 9,
    eVehicle_DontTargetPlayer = 10
};

enum ePstuff_Sprites {

	eSprite_pStuff_Mouse_Cursor			 = 0x23,
	eSprite_pStuff_Mouse_Target			 = 0x24,
	eSprite_pStuff_Mouse_Arrow_DownRight = 0x25,
	eSprite_pStuff_Mouse_Arrow_UpLeft	 = 0x26,
	eSprite_pStuff_Mouse_Helicopter		 = 0x27,
};

enum eSprite_DrawOrder {
	eSprite_Draw_First	= -1,
	eSprite_Draw_Second	= 0,
	eSprite_Draw_OnTop	= 1
};

struct sMission_Troop;


// The reason none of the fields are labelled, is to simplify adding the remaining CF2 sprites.
// Once this is done, we can name all the fields

struct sSprite {
	int16 field_0;		// X
	int16 field_2;		
	int16 field_4;		// Y
	int16 field_6;
	int16 field_8;		// Animation   mSprite_SheetPtr[field_8][field_a]
	int16 field_A;		// FrameNumber mSprite_SheetPtr[field_8][field_a]
	int16 field_C;
	int16 field_E;
	int16 field_10;		// Next Direction
	int16 field_12;
	int16 field_14;     //  Projectile: Add to Y
	int16 field_16;     //  Projectile: Add to X
	int16 field_18;		// Sprite Type

	union {
		int32		field_1A;
		sSprite*	field_1A_sprite;    // Created by Sprite
	};

	union {

		struct {
			int16 field_1E;
			int16 field_20;	// Height
		};

		int32 field_1E_Big;
	};

	int16 field_22;		// Player: eSprite_PersonType
	int16 field_24;
	int16 field_26;		// Target X
	int16 field_28;		// Target Y
	int16 field_2A;
	int16 field_2C;		// eSprite_DrawOrder
	int16 field_2E;		// Weapon Target X
	int16 field_30;		// Weapon Target Y
	int16 field_32;		// Player: Squad Number
	int16 field_34;
	int16 field_36;		// Speed ?
	int16 field_38;		// eSprite_Anim
	int16 field_3A;
	int16 field_3C;		// Player: Face Direction
	int16 field_3E;
	int16 field_40; // Player: Next Squad Walk Target Index
	int8 field_42;  // Player: Finished Walking
	int8 field_43;	// Player: Reached other Troop
	int8 field_44;
	int8 field_45;

	// Eventually this should be replaced
	union {
		int32			field_46;
		sMission_Troop*  field_46_mission_troop;
		sSprite*		field_46_sprite;
	};

	int16 field_4A;		// Player: Fire if mouse pressed
	int8 field_4C;
	int8 field_4D;
	int8 field_4E;
	int8 field_4F;			// Player: -1 = In water
	int16 field_50;
	int16 field_52;			// Video: Number of rows not to draw
	int8 field_54;			// Human: Fired Weapon
	int8 field_55;
	int8 field_56;
	int8 field_57;			// Human: Weapon Fired Cooldown
	int8 field_58;          // Has Shadow
	int8 field_59;
	int8 field_5A;
	int8 field_5B;			// Human: Is Sinking
	int8 field_5C;			// 1 = Was Drawn

	int8 field_5D;          // 5E contains index into mGame_Data.mSoldiers_Allocated

    // Eventually this should be replaced
    // Perhaps a linked list could replace mSprite and we use an iterator here
    union {
		int32 field_5E_Squad;
        size_t field_5E;			// Index inside mSprites (if 5D == false)
        sMission_Troop *field_5E_SoldierAllocated;  // Soldier Ptr who killed sprite
    };

	int8 field_60;			// Current Terrain Type
	int8 field_61;
	int16 field_62;         // AI: Aggression
	int8 field_64;
	int8 field_65;			// -1 = Enabled?
	sSprite* field_66;		// Human-Player: Vehicle Walk Target
    
                            // Eventually this should be replaced
	union {
		int64	 field_6A;
		sSprite* field_6A_sprite;	// Human-Player: Current Vehicle
	};

	int8 field_6E;			// Human-Player: In Vehicle;  Non-Human Player: Can't be run over
	int8 field_6F;			// Vehicle Type
	sSprite* field_70;		// Hostage: Following Sprite
	int8 field_74;			// Helicopter: Restart Anim Frame Number
	int8 field_75;			// Human-Player: 0x02 = invincibility, 0x01 = homing missiles

	sSprite() {
		Clear();
	}

	void Clear();

	cPosition *getPosition() { return new cPosition(field_0, field_4); }

	int getX() const { return field_0; }
	void setX(int pX) { field_0 = pX; }

	int getY() const { return field_4; }
	void setY(int pY) { field_4 = pY; }
};

struct sWeaponData {
	int16	mSpeed;
	int16	mAliveTime;			// Time bullet will travel
	int16	mCooldown;
	int16	mDeviatePotential;	// Potential for a bullet to deviate
	int16	field_8;
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
extern const sWeaponData mSprite_Bullet_UnitData[];
extern const int16 mSprite_Player_MissionOver_Frames[];
extern const int8 mSprite_Seal_AnimFrames[];
extern const int16 mSprite_VehiclePosition_Mod[];
extern const int16 mSprite_Computer_Animation[];
extern const int16 mSprite_Seal_Frames[];
extern const int16 mSprite_Computer_Frames[];
extern const int8 mSprite_Civilian_Sound_Death[];
extern const int8 mSprite_Soldier_Unk[];
extern const int16 mSprite_Direction_Frame_Unk[];
extern const int16* mSprite_AnimationPtrs[];
extern const int16 mSprite_Speed_Direction_Modifier[];
extern const std::vector<std::vector<int16>> mSoldier_Squad_Fire_RotationOrder;
