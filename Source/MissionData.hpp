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

struct sMissionData {
	const char**	mMissionNames;
	const char**	mMissionPhaseNames;
	const uint16*	mMissionPhases;
	const int8**	mMapGoals;
	const int16*	mEnemyAggression;
};

enum eMissionGoals {
	eGoal_Kill_All_Enemy = 1, 
	eGoal_Destroy_Enemy_Buildings = 2,
	eGoal_Rescue_Hostages = 3,
	eGoal_Protect_Civilians = 4,
	eGoal_Kidnap_Leader = 5,
	eGoal_Destroy_Factory = 6,
	eGoal_Destroy_Computer = 7,
	eGoal_Get_Civilian_Home = 8,
	eGoal_End = -1
};

extern const sMissionData	mMissionData_Plus;
extern const sMissionData	mMissionData_AmigaFormat;
extern const sMissionData	mMissionData_Retail;
extern const sMissionData	mMissionData_Retail2;
extern const sMissionData   mMissionData_Custom;

extern const char*			mMissionGoals[];
