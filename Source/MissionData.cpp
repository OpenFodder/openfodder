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
#if defined(_MSC_VER) && _MSC_VER <= 1800
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#define TypeFunction(x) x&
#define GetArrayFunction(x) x.GetArray()
#define GetIntFunction(x) x.GetInt()
#define GetStringFunction(x) x.GetString()
#define SizeFunction(x) x.Size()

using namespace rapidjson;
#else
#include "Utils/json.hpp"

#define TypeFunction(x) x
#define GetArrayFunction(x) x
#define GetIntFunction(x) x
#define GetStringFunction(x) x
#define SizeFunction(x) x.size()

using Json = nlohmann::json;
#endif

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
cMissionData::cMissionData( const std::vector<std::string>	&pMissionNames, 
							const std::vector<std::string>	&pMissionPhaseNames, 
							const std::vector<size_t>		&pMissionPhases, 
							const std::vector<std::vector<eMissionGoals> > &pMapGoals, 
							const std::vector<sAggression> &pEnemyAggression,
							const std::string& pCampaign) {

	mMissionNames = pMissionNames;
	mMissionPhases = pMissionPhases;

	mMapNames = pMissionPhaseNames;
	mMapGoals = pMapGoals;
	mMapAggression = pEnemyAggression;

	mCustomMission.mAuthor = "Sensible Software";
	mCustomMission.mName = pCampaign;

	//DumpCampaign();
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

void cMissionData::DumpCampaign() {
	auto MissionData = this;

	Json Campaign;
	Campaign["Author"] = "Sensible Software";
	Campaign["Name"] = mCustomMission.mName;

	size_t MissionNumber = 1;
	size_t MapNumber = 0;

	// Each Mission
	for (auto MissionName : MissionData->mMissionNames) {
		
		Json Mission;
		Mission["Name"] = MissionName;

		size_t MissionPhases = MissionData->getNumberOfPhases(MissionNumber);

		// Each Phase of a mission
		for (size_t PhaseCount = 0; PhaseCount < MissionPhases; ++PhaseCount) {
			Json Phase;

			Phase["MapName"] = MissionData->getMapFilename(MapNumber);
			Phase["Name"] = MissionData->getMapName(MapNumber);
			auto Agg = MissionData->getMapAggression(MapNumber);
			
			Phase["Aggression"][0] = Agg.mMin;
			Phase["Aggression"][1] = Agg.mMax;

			for (auto Goal : MissionData->getMapGoals(MapNumber)) {

				Phase["Objectives"].push_back(mMissionGoal_Titles[Goal - 1]);
			}

			++MapNumber;

			Mission["Phases"].push_back(Phase);
		}

		Campaign["Missions"].push_back(Mission);
		++MissionNumber;
	}

	std::string file = "D:\\" + mCustomMission.mName + ".of";
	std::ofstream MissionFile(file);
	if (MissionFile.is_open()) {
		auto ss = Campaign.dump(1);

		MissionFile.write(ss.c_str(), ss.size());
		MissionFile.close();
	}
}


/**
 * Load a custom mission set
 */
bool cMissionData::LoadCustomMissionSet( const std::string& pMissionSet ) {
	Clear();

	std::ifstream MissionSetFile( local_PathGenerate( pMissionSet, "", true ), std::ios::binary );
	if (MissionSetFile.is_open()) {
#if defined(_MSC_VER) && _MSC_VER <= 1800
		IStreamWrapper isw( MissionSetFile );
		Document MissionSet;
		MissionSet.ParseStream( isw );
#else
		Json MissionSet = Json::parse( MissionSetFile );
#endif
		mCustomMission.mAuthor = GetStringFunction(MissionSet["Author"]);
		mCustomMission.mName = GetStringFunction(MissionSet["Name"]);
		
		std::string MapPath = "Custom/Sets/";

		MapPath.append( mCustomMission.mName );
		MapPath.append( "/" );

		// Loop through the missions in this set
		for (TypeFunction(auto) Mission : GetArrayFunction(MissionSet["Missions"])) {
			std::string Name = GetStringFunction(Mission["Name"]);
			transform( Name.begin(), Name.end(), Name.begin(), toupper );

			mMissionNames.push_back( Name );
			mMissionPhases.push_back( SizeFunction(GetArrayFunction(Mission["Phases"])) );

			// Each Map (Phase)
			for (TypeFunction(auto) Phase : GetArrayFunction(Mission["Phases"])) {
				std::vector<eMissionGoals> Goals;
				std::string MapFile = MapPath;
				std::string MapName = GetStringFunction(Phase["MapName"]);

				Name = GetStringFunction(Phase["Name"]);
				transform( Name.begin(), Name.end(), Name.begin(), toupper );
				
				MapFile.append( MapName );
				mMapFilenames.push_back( MapFile );
				mMapNames.push_back( Name );

				// Map Aggression
				if (SizeFunction(GetArrayFunction(Phase["Aggression"])))
					mMapAggression.push_back( { GetIntFunction(Phase["Aggression"][0]), GetIntFunction(Phase["Aggression"][1]) } );
				else
					mMapAggression.push_back( { 4, 8 } );

				// Each map goal
				for (TypeFunction(auto) ObjectiveName : GetArrayFunction(Phase["Objectives"])) {
					std::string ObjectiveNameStr = GetStringFunction(ObjectiveName);
					transform( ObjectiveNameStr.begin(), ObjectiveNameStr.end(), ObjectiveNameStr.begin(), toupper );

					// Check each goal
					int x = 0;
					for (std::string GoalTitle : mMissionGoal_Titles) {
						++x;

						if (GoalTitle == ObjectiveNameStr) {

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

	mMapFilenames.clear();
	mMapNames.clear();
	mMapGoals.clear();
	mMapAggression.clear();
}
