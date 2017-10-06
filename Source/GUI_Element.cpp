/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2017 Robert Crossfield
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

/* AFX Menu Buttons */
const sGUI_Element mAfx_Buttons[] = {
	{ &cFodder::GUI_Button_NoAction, 0x6B, 0x6B, 0x4A, 0x6B, &cFodder::GUI_Button_NoAction2 },
	{ &cFodder::GUI_Button_NoAction, 0x01, 0x9E, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap0 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9C, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap1 },
	{ &cFodder::GUI_Button_NoAction, 0x02, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap0 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap1 },
	{ 0 }
};

/* Cannon Fodder Plus Menu Buttons */
const sGUI_Element mPlus_Buttons[] = {
	{ &cFodder::GUI_Button_NoAction, 0x6B, 0x6B, 0x4A, 0x6B, &cFodder::GUI_Button_SelectQuiz },
	{ &cFodder::GUI_Button_NoAction, 0x01, 0x9E, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap0 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9C, 0x1B, 0x63, &cFodder::GUI_Button_SelectMap1 },
	{ &cFodder::GUI_Button_NoAction, 0x02, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap2 },
	{ &cFodder::GUI_Button_NoAction, 0xA1, 0x9D, 0x81, 0x63, &cFodder::GUI_Button_SelectMap3 },
};

/* Cannon Fodder Plus Quiz Buttons */
const sGUI_Element mPlusQuiz_Buttons[] = {
	{ &cFodder::GUI_Button_NoAction, 0x0BD, 0x19, 0x5B, 0x48,	&cFodder::GUI_Button_ExitMenu },
	{ &cFodder::GUI_Button_NoAction, 0x0B2, 0x1C, 0xA2, 0x33,	&cFodder::GUI_Button_ExitMenu },
	{ &cFodder::GUI_Button_NoAction, 0x000, 0x31, 0x03, 0x1E,	&cFodder::GUI_Button_Quiz_2 },
	{ &cFodder::GUI_Button_NoAction, 0x10C, 0x31, 0x03, 0x1E,	&cFodder::GUI_Button_Quiz_3 },
	{ &cFodder::GUI_Button_NoAction, 0x000, 0x31, 0xD6, 0x1E,	&cFodder::GUI_Button_Quiz_4 },
	{ &cFodder::GUI_Button_NoAction, 0x10C, 0x31, 0xD6, 0x1E,	&cFodder::GUI_Button_Quiz_5 },
	{ &cFodder::GUI_Button_NoAction, 0x02A, 0x32, 0x10, 0xC9,	&cFodder::GUI_Button_Quiz_6 },
	{ &cFodder::GUI_Button_NoAction, 0x05B, 0x27, 0x1F, 0xB4,	&cFodder::GUI_Button_Quiz_7 },
	{ &cFodder::GUI_Button_NoAction, 0x081, 0x1E, 0x25, 0xA7,	&cFodder::GUI_Button_Quiz_8 },
	{ &cFodder::GUI_Button_NoAction, 0x09E, 0x1F, 0x24, 0xB1,	&cFodder::GUI_Button_Quiz_9 },
	{ &cFodder::GUI_Button_NoAction, 0x0BC, 0x2B, 0x21, 0xAA,	&cFodder::GUI_Button_Quiz_10 },
	{ &cFodder::GUI_Button_NoAction, 0x0E6, 0x3B, 0x13, 0xCE,	&cFodder::GUI_Button_Quiz_11 },
};

const int8 mGUI_Sidebar_TroopList_Sprite_Modifier[] = {
	0x29,	// Squad Selected (Normal)
	0x4D,	// Selected		  (Gold/Yellow)
	0x71	// Squad Not Selected (Grayed Out)
};

const int16 mGUI_Squad_Icons[3] = { 
	0, 
	1, 
	2
};

const int16 mGUI_Squad_Split_Icons[] = {
	0, 3, 4, // Squad 0
	6, 1, 5, // Squad 1
	8, 7, 2  // Squad 2
};

const int16 mGUI_Squad_Active_Icons[] = {
	149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
	158
};

const int16 mGUI_Squad_Inactive_Icons[] = {
	159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
	168
};

// Left value is vehicle ID type
const sGUI_SquadVehicleIcon mGUI_Squad_Vehicle_Icons[] = {
	{ eVehicle_Turret_Cannon,		 2 },
	{ eVehicle_Turret_Missile,		 3 },
	{ eVehicle_Jeep,				 4 },
	{ eVehicle_JeepRocket,			 5 },
	{ eVehicle_Tank,				 6 },
	{ eVehicle_Helicopter,			 7 },
	{ eVehicle_Helicopter_Grenade,	 8 },
	{ eVehicle_Helicopter_Missile,	 9 },
	{ eVehicle_Helicopter_Homing,	10 },
	-1
};
