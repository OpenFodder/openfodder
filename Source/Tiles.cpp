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

const std::vector<sTileType> mTileTypes = {
	{ eTileTypes_Jungle, "jun", "Jungle"		},
	{ eTileTypes_Desert, "des", "Desert"		},
	{ eTileTypes_Ice,	 "ice", "Ice"			},
	{ eTileTypes_Moors,  "mor", "Moors"			},
	{ eTileTypes_Int,	 "int", "Interior"		},
	{ eTileTypes_Hid,	 "hid", "Hid"			},      // Unknown
	{ eTileTypes_AFX,	 "afx", "AmigaFormat"	}		// Amiga Format Christmas Special
};

const int16 TILE_WIDTH_PIXELS = 16;
const int16 TILE_HEIGHT_PIXELS = 16;

const int16* mTiles_Indestructible[] = {
	mTiles_Indestructible_Jungle,		// JUN       
	mTiles_Indestructible_Desert,		// DES
	mTiles_Indestructible_Ice,			// ICE
	mTiles_Indestructible_Moors,		// MOR
	mTiles_Indestructible_Internal,		// INT
	mTiles_Indestructible_Jungle,		// HID
	mTiles_Indestructible_Ice			// AFX
};

const int8 mTiles_NotFlyable[] = {
	0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1
};

const int8 mTiles_NotDriveable[] = {
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
};

const int8 mTiles_NotWalkable[] = {
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int16 mTiles_Indestructible_Jungle[] = {
	267, 268, 269, 287, 288, 289, 307, 308, 309, 98,
	177, 217, -1
};

const int16 mTiles_Indestructible_Desert[] = {
	9, 10, 11, 29, 30, 31, 50, -1
};

const int16 mTiles_Indestructible_Ice[] = {
	307, 308, 309, 327, 328, 329,
	348, 349, 363, 378, 379, -1
};

const int16 mTiles_Indestructible_Moors[] = {
	257, 276, 277, 160, 161, 162, 180,
	181, 182, 201, 306, 384, 398, -1
};

const int16 mTiles_Indestructible_Internal[] = {
	23, 40, 41, 42, 63, 103, 50, 51, 70,
	71, 90, 91, 346, 331, 351, -1,
};
