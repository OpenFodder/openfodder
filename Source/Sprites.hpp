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

struct sSprite {
	int16 field_0;		// X
	int16 field_2;		
	int16 field_4;		// Y
	int16 field_6;
	int16 field_8;		// mSpriteDataPtr[field_8][field_a]
	int16 field_A;		// mSpriteDataPtr[field_8][field_a]
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
