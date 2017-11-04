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

const std::vector<std::string> mCustom_MissionNames;
const std::vector<size_t> mCustom_MissionPhaseTable;
const std::vector<std::vector<eMissionGoals>> mCustom_Map_Goals;
const std::vector<sAggression> mCustom_Map_Sprite_Enemy_Aggression;

// Custom
cMissionData mMissionData_Custom = {
	mCustom_MissionNames,
	mCustom_MissionNames,
	mCustom_MissionPhaseTable,
	mCustom_Map_Goals,
	mCustom_Map_Sprite_Enemy_Aggression,
	""
};
