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

#include "stdafx.hpp"

/** Goals **/
const std::vector<std::string> mMissionGoal_Titles = {
	"KILL ALL ENEMY",
	"DESTROY ENEMY BUILDINGS",
	"RESCUE HOSTAGES",
	"PROTECT ALL CIVILIANS",
	"KIDNAP ENEMY LEADER",
	"DESTROY FACTORY",
	"DESTROY COMPUTER",
	"GET CIVILIAN HOME",
	"ACTIVATE ALL SWITCHES",	// CF2
	"RESCUE HOSTAGE"			// CF2
};

const std::vector< uint16 > mCustom_MissionPhaseTable = { 
	01 
};

const std::vector< std::vector<eMissionGoals> > mCustom_Map_Goals = {
	{ eGoal_Kill_All_Enemy }
};

// Hardcode custom maps to min 4, max 8 aggression
const std::vector<sAggression> mCustom_Map_Sprite_Enemy_Aggression = {
	{ 4,  8 }
};

const std::vector<std::string> mCustom_MissionNames;

// Custom
cMissionData mMissionData_Custom = {
	mCustom_MissionNames,
	mCustom_MissionNames,
	mCustom_MissionPhaseTable,
	mCustom_Map_Goals,
	mCustom_Map_Sprite_Enemy_Aggression
};

/** 
 *
 */
cMissionData::cMissionData( const std::vector<std::string>& pMissionNames, const std::vector<std::string>& pMissionPhaseNames, const std::vector< uint16 >& pMissionPhases, const std::vector< std::vector<eMissionGoals> >& pMapGoals, const std::vector<sAggression>& pEnemyAggression ) {

	mMissionNames = pMissionNames;
	mMissionPhases = pMissionPhases;

	mMapNames = pMissionPhaseNames;
	mMapGoals = pMapGoals;
	mMapAggression = pEnemyAggression;
}

/**
 *
 */
bool cMissionData::LoadCustomMap( std::string pCustomMapName ) {
	Clear();

	mCustomMap = pCustomMapName.substr( 7 );

	// Map / Phase names must be upper case
	std::transform( mCustomMap.begin(), mCustomMap.end(), mCustomMap.begin(), ::toupper );

	mMissionNames.push_back( mCustomMap );
	mMissionPhases.push_back( 1 );

	// TODO: Load these from file
	mMapNames.push_back( mCustomMap );
	mMapGoals.push_back( { eGoal_Kill_All_Enemy } );
	mMapAggression.push_back( { 4, 8 } );

	return true;
}

/**
 *
 */
void cMissionData::Clear() {
	mMissionNames.clear();
	mMissionPhases.clear();

	mMapNames.clear();
	mMapGoals.clear();
	mMapAggression.clear();
}
