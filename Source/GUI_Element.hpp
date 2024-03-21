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

class cFodder;

struct sGUI_Element {
	int16	(cFodder::*field_0)();
	int16	mX;
	int16	mWidth;
	int16	mY;
	int16	mHeight;
	void	(cFodder::*mMouseInsideFuncPtr)();	// field_c
};

struct sGUI_SquadVehicleIcon {
	int16 mVehicleType;
	int16 mIconID;
};

// Menu Buttons
extern const sGUI_Element mAfx_Buttons[];
extern const sGUI_Element mPlus_Buttons[];
extern const sGUI_Element mPlusQuiz_Buttons[];
extern const sGUI_Element mAmigaAction_Buttons[];

// Sidebar Squad Icons
extern const int16 mGUI_Squad_Icons[];
extern const int16 mGUI_Squad_Active_Icons[];
extern const int16 mGUI_Squad_Inactive_Icons[];
extern const int16 mGUI_Squad_Split_Icons[];
extern const sGUI_SquadVehicleIcon mGUI_Squad_Vehicle_Icons[];

extern const int8 mGUI_Sidebar_TroopList_Sprite_Modifier[];

