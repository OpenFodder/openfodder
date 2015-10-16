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
};

extern const int8* mMission_Goals[];

extern const sMissionData	mMissionData_Plus;
extern const sMissionData	mMissionData_AmigaFormat;
extern const sMissionData	mMissionData_Retail;
extern const char*			mMissionGoals[];
extern const std::string	mMapTypes[];
