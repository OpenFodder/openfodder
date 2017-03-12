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

// Amiga Format Phases
const std::vector< size_t > mAmigaFormat_MissionPhaseTable = {
	01, 
	01 
};

// Amiga Plus Phases
const std::vector< size_t > mAmigaPlus_MissionPhaseTable = { 
	01, 
	01, 
	01, 
	01 
};

// Amiga Format Map Names
const std::vector<std::string> mAmigaFormat_MapNames = {
	"LAND OF HOPE AND GLORY",
	"IT'S SNOW TIME!"
};

// Amiga Plus Map Names
const std::vector<std::string> mAmigaPlus_MapNames = {
	"STAGE 1",
	"STAGE 2",
	"STAGE 3",
	"STAGE 4"
};

// Amiga Demo Map Goals
const std::vector< std::vector<eMissionGoals> > mAmigaDemo_Map_Goals = {
	/* mapm1  */{ eGoal_Kill_All_Enemy },
	/* mapm2  */{ eGoal_Kill_All_Enemy },
	/* mapm3  */{ eGoal_Kill_All_Enemy },
	/* mapm4  */{ eGoal_Kill_All_Enemy },
};

// Amiga Demo Aggression
const std::vector<sAggression> mAmigaDemo_Map_Sprite_Enemy_Aggression = {
	{ 2,  4 },
	{ 4,  8 },
	{ 6,  9 },
	{ 8, 12 }
};

// Amiga Plus
const cMissionData mMissionData_Plus = {
	mAmigaPlus_MapNames,
	mAmigaPlus_MapNames,
	mAmigaPlus_MissionPhaseTable,
	mAmigaDemo_Map_Goals,
	mAmigaDemo_Map_Sprite_Enemy_Aggression
};

// Amiga Format Christmas Special
const cMissionData mMissionData_AmigaFormat = {
	mAmigaFormat_MapNames,
	mAmigaFormat_MapNames,
	mAmigaFormat_MissionPhaseTable,
	mAmigaDemo_Map_Goals,
	mAmigaDemo_Map_Sprite_Enemy_Aggression
};
