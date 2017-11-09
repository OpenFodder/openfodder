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
#include <experimental/filesystem>

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
    "ACTIVATE ALL SWITCHES",    // CF2
    "RESCUE HOSTAGE"            // CF2
};

cCampaign::cCampaign() {

    mHasMaps = false;

}

/**
 * Load a single custom map
 */
bool cCampaign::LoadCustomMap( const std::string& pMapName ) {
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
/*
void cCampaign::DumpCampaign() {
    auto MissionData = this;

    Json Campaign;
    Campaign["Author"] = "Sensible Software";
    Campaign["Name"] = mName;

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

    std::string file = "D:\\" + mName + ".of";
    std::ofstream MissionFile(file);
    if (MissionFile.is_open()) {
        auto ss = Campaign.dump(1);

        MissionFile.write(ss.c_str(), ss.size());
        MissionFile.close();
    }
}
*/

/**
 * Load a campaign
 */
bool cCampaign::LoadCampaign( const std::string& pName) {
    Clear();

    if (!pName.size())
        return false;

    std::ifstream MissionSetFile(local_PathGenerate(pName + ".ofc", "", eDataType::eCampaign));
    if (MissionSetFile.is_open()) {
#if defined(_MSC_VER) && _MSC_VER <= 1800
        IStreamWrapper isw( MissionSetFile );
        Document MissionSet;
        MissionSet.ParseStream( isw );
#else
        Json MissionSet = Json::parse( MissionSetFile );
#endif
        mAuthor = GetStringFunction(MissionSet["Author"]);
        mName = GetStringFunction(MissionSet["Name"]);
        
        // Loop through the missions in this set
        for (TypeFunction(auto) Mission : GetArrayFunction(MissionSet["Missions"])) {
            std::string Name = GetStringFunction(Mission["Name"]);
            transform( Name.begin(), Name.end(), Name.begin(), toupper );

            mMissionNames.push_back( Name );
            mMissionPhases.push_back( SizeFunction(GetArrayFunction(Mission["Phases"])) );

            // Each Map (Phase)
            for (TypeFunction(auto) Phase : GetArrayFunction(Mission["Phases"])) {
                std::vector<eMissionGoals> Goals;

                std::string MapName = GetStringFunction(Phase["MapName"]);

                Name = GetStringFunction(Phase["Name"]);
                transform( Name.begin(), Name.end(), Name.begin(), toupper );
                
                mMapFilenames.push_back(MapName);
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
void cCampaign::Clear() {
    mHasMaps = false;
    mMissionNames.clear();
    mMissionPhases.clear();

    mMapFilenames.clear();
    mMapNames.clear();
    mMapGoals.clear();
    mMapAggression.clear();
}

/**
 * Get the filename for a map
 */
std::string cCampaign::getMapFilename(const size_t pMapNumber) const {

    std::string Filename;

    if (pMapNumber >= mMapFilenames.size()) {

        std::stringstream   filename;

        filename << "mapm" << (pMapNumber + 1);

        return filename.str();
    } else {

        Filename = local_PathGenerate(mName, "", eDataType::eCampaign);
    }

    // If a campaign folder exists, return a path inside it
    if (!std::experimental::filesystem::exists(Filename))
        Filename = "";

    // Otherwise fallback to loading the map from the currently loaded
    return Filename + mMapFilenames[pMapNumber];
}

/**
* Get the mission name
*/
std::string cCampaign::getMissionName(size_t pMissionNumber) const {
    // Mission Number is always + 1
    pMissionNumber -= 1;

    if (pMissionNumber >= mMissionNames.size())
        return mCustomMap;

    return mMissionNames[pMissionNumber];
}

/**
* Number of phases on this mission
*/
uint16 cCampaign::getNumberOfPhases(size_t pMissionNumber) const {
    // Mission Number is always + 1
    pMissionNumber -= 1;

    if (pMissionNumber >= mMissionPhases.size())
        return 1;

    return (uint16)mMissionPhases[pMissionNumber];
}

/**
* Get the map name
*/
std::string cCampaign::getMapName(const size_t& pMapNumber) const {

    if (pMapNumber >= mMapNames.size())
        return mCustomMap;

    return mMapNames[pMapNumber];
}

/**
* Get the goals for this map
*/
const std::vector<eMissionGoals>& cCampaign::getMapGoals(const uint16& pMapNumber) const {

    if (pMapNumber >= mMapGoals.size())
        return mMapGoals[0];

    return mMapGoals[pMapNumber];
}

/**
* Get the enemy aggression for this map
*/
const sAggression& cCampaign::getMapAggression(const uint16& pMapNumber) const {

    if (pMapNumber >= mMapAggression.size())
        return mMapAggression[0];

    return mMapAggression[pMapNumber];
}

/**
* Get the number of available maps
*/
const size_t cCampaign::getMapCount() const {

    return mMapNames.size();
}

const std::string cCampaign::getName() const {
    return mName;
}