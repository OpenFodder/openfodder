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

struct sRecruit {
	char  mName[7];

	uint8 field_6;
	int8 field_7;
	int8 field_8;
};

struct sRecruit_Sprites {
	int16	mSpriteType;
	int16	mFrame;
	int16	field_4;
	int16	field_6;
	int16*	field_8;
};

struct sRecruit_Screen_Pos {
	int16	mX;
	int16	mY;
	int16	mFrame;
};

struct sRecruitRendered {
	uint32* mDataPtr;
	int16	mPosition;

    sRecruitRendered(uint32* pDataPtr, int16 pPosition) {
        mDataPtr = pDataPtr;
        mPosition = pPosition;
    }
};

struct sGravePosition {
    int16 mX;
    int16 mY;
};

extern const std::vector<sGravePosition> mGravePositions;
extern const struct sRecruit mRecruits[];
extern const struct sRecruit_Sprites mRecruitSprite[34];
extern struct sRecruit_Screen_Pos mRecruit_Screen_Positions[294];

extern const int16 mRecruit_Truck_Anim_Welcome[];
extern const int16 mRecruit_Truck_Anim_SwingArm[];
extern const int16 mRecruit_Truck_Anim_PassTroop[];
extern const int16 mRecruit_Truck_Anim_CloseDoor[];

extern const int16 mRecruit_Shirt_Colors[];
extern const int16 mRecruit_Hill_Position_Gaps[];
