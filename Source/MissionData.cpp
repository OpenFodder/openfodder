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
#include "Utils/json.hpp"

using Json = nlohmann::json;

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

/** 
 *
 */
cMissionData::cMissionData( const std::vector<std::string>& pMissionNames, const std::vector<std::string>& pMissionPhaseNames, const std::vector< size_t >& pMissionPhases, const std::vector< std::vector<eMissionGoals> >& pMapGoals, const std::vector<sAggression>& pEnemyAggression ) {

	mMissionNames = pMissionNames;
	mMissionPhases = pMissionPhases;

	mMapNames = pMissionPhaseNames;
	mMapGoals = pMapGoals;
	mMapAggression = pEnemyAggression;
}

/**
 * Load a single custom map
 */
bool cMissionData::LoadCustomMap( const std::string& pMapName ) {
	Clear();
	std::string CustomMapName = pMapName;
	
	// Remove "Custom/Maps/" and ".map"
	CustomMapName = CustomMapName.substr( 12 );
	CustomMapName.resize( CustomMapName.size() - 4 );

	mCustomMap = pMapName;

	// Map / Phase names must be upper case
	std::transform( CustomMapName.begin(), CustomMapName.end(), CustomMapName.begin(), ::toupper );

	mMapFilenames.push_back( pMapName.substr(0, pMapName.size() - 4) );
	mMissionNames.push_back( CustomMapName );
	mMissionPhases.push_back( 1 );

	// TODO: Try load these from file before using defaults
	mMapNames.push_back( CustomMapName );
	mMapGoals.push_back( { eGoal_Kill_All_Enemy } );
	mMapAggression.push_back( { 4, 8 } );

	return true;
}

/**
 * Load a custom mission set
 */
bool cMissionData::LoadCustomMissionSet( const std::string& pMissionSet ) {
	Clear();

	std::ifstream MissionSetFile( local_PathGenerate( pMissionSet, "", true ), std::ios::binary );
	if (MissionSetFile.is_open()) {
		Json MissionSet = Json::parse( MissionSetFile );

		mCustomMission.mAuthor = MissionSet["Author"];
		mCustomMission.mName = MissionSet["Name"];

		std::string MapPath = "Custom/Sets/";

		MapPath.append( mCustomMission.mName );
		MapPath.append( "/" );

		auto Missions = MissionSet["Missions"];

		// Loop through the missions in this set
		for (auto Mission : Missions) {
			std::string Name = Mission["Name"];
			transform( Name.begin(), Name.end(), Name.begin(), toupper );

			mMissionNames.push_back( Name );
			mMissionPhases.push_back( Mission["Phases"].size() );

			// Each Map (Phase)
			for (auto Phase : Mission["Phases"]) {
				std::vector<eMissionGoals> Goals;
				std::string MapFile = MapPath;
				std::string MapName = Phase["MapName"];
				Name = Phase["Name"];

				transform( Name.begin(), Name.end(), Name.begin(), toupper );
				MapFile.append( MapName );

				mMapFilenames.push_back( MapFile );
				mMapNames.push_back( Name );

				// Map Aggression
				if (Phase["Aggression"].size())
					mMapAggression.push_back( { Phase["Aggression"][0], Phase["Aggression"][1] } );
				else
					mMapAggression.push_back( { 4, 8 } );

				// Each map goal
				for (std::string ObjectiveName : Phase["Objectives"]) {
					transform( ObjectiveName.begin(), ObjectiveName.end(), ObjectiveName.begin(), toupper );

					// Check each goal
					int x = 0;
					for (auto GoalTitle : mMissionGoal_Titles) {
						++x;

						if (GoalTitle == ObjectiveName) {

							Goals.push_back( static_cast<eMissionGoals>(x) );
							break;
						}
					}
				}
				mMapGoals.push_back( Goals );
			}
			
		}

		return true;
	}

	return false;
}

/**
 * Clear all missions/map names, goals and aggression rates
 */
void cMissionData::Clear() {
	mMissionNames.clear();
	mMissionPhases.clear();

	mMapNames.clear();
	mMapGoals.clear();
	mMapAggression.clear();
}
