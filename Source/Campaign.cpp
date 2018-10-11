/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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
    "ACTIVATE ALL SWITCHES",    // CF2
    "RESCUE HOSTAGE"            // CF2
};

cCampaign::cCampaign() {
    Clear();
}

std::string cCampaign::GetPath(const std::string& pName, const std::string& pPath) const {

    auto Filename = local_PathGenerate(pName, pPath, eDataType::eCampaign);

    if (mDirectPath)
        Filename = pName;

    return Filename;
}

bool cCampaign::LoadCustomMapFromPath(const std::string& pPath) {
    Clear();
    std::string CustomMapName = pPath;

    auto a = CustomMapName.find_last_of("/") + 1;
    CustomMapName.erase(CustomMapName.begin(), CustomMapName.begin() + a);

    LoadCustomMap(CustomMapName);
    mCustomMap = pPath;
    mCustomMap.resize(mCustomMap.size() - 4);

    // Change the path to this map
    std::shared_ptr<cPhase> Phase = mMissions.back()->mPhases.back();
    Phase->mMapFilename = pPath.substr(0, pPath.size() - 4);

    return true;
}

/**
 * Load a single custom map
 */
bool cCampaign::LoadCustomMap(const std::string& pMapName) {
    Clear();
    std::string CustomMapName = pMapName;

    // ".map"
    CustomMapName.resize(CustomMapName.size() - 4);

    mCustomMap = pMapName;
    mCustomMap.resize(mCustomMap.size() - 4);

    // Map / Phase names must be upper case
    std::transform(CustomMapName.begin(), CustomMapName.end(), CustomMapName.begin(), ::toupper);

    SetSingleMapCampaign();
    std::shared_ptr<cMission> Mission = mMissions.back();
    std::shared_ptr<cPhase> Phase = Mission->mPhases.back();

    Mission->mName = CustomMapName;

    Phase->mMapFilename = pMapName.substr(0, pMapName.size() - 4);

    // TODO: Try load these from file before using defaults
    Phase->mName = CustomMapName;
    Phase->mGoals.push_back({ eGoal_Kill_All_Enemy });
    Phase->mAggression = { 4, 8 };

    return true;
}
/* This code was used to dump the original campaigns to JSON
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
bool cCampaign::LoadCampaign(const std::string& pName, bool pCustom, bool pDirectPath) {

    if (!pName.size())
        return false;


    Clear();
    mDirectPath = pDirectPath;

    std::ifstream MissionSetFile(GetPath(pName + ".ofc"));
    if (MissionSetFile.is_open()) {
        Json MissionSet = Json::parse(MissionSetFile);

        mAuthor = MissionSet["Author"];
        mName = MissionSet["Name"];

        // Loop through the missions in this set
        for (auto& Mission : MissionSet["Missions"]) {
            std::shared_ptr<cMission> newMission = std::make_shared<cMission>();
            mMissions.push_back(newMission);

            newMission->mName = Mission["Name"];

            // Each Map (Phase)
            for (auto& Phase : Mission["Phases"]) {
                std::shared_ptr<cPhase> newPhase = std::make_shared<cPhase>();
                newMission->mPhases.push_back(newPhase);

                newPhase->mName = Phase["Name"];
                newPhase->mMapFilename = Phase["MapName"];
                if (Phase["Aggression"].size()) {
                    newPhase->mAggression.mMin = Phase["Aggression"][0];
                    newPhase->mAggression.mMax = Phase["Aggression"][1];
                }

                // Each map goal
                for (auto& ObjectiveName : Phase["Objectives"]) {
                    std::string ObjectiveNameStr = ObjectiveName;
                    transform(ObjectiveNameStr.begin(), ObjectiveNameStr.end(), ObjectiveNameStr.begin(), toupper);

                    // Check each goal
                    int x = 0;
                    for (std::string GoalTitle : mMissionGoal_Titles) {
                        ++x;

                        if (GoalTitle == ObjectiveNameStr) {

                            newPhase->mGoals.push_back(static_cast<ePhaseGoals>(x));
                            break;
                        }
                    }
                }
            }
        }

        mIsCustomCampaign = pCustom;
        return true;
    }

    return false;
}

/**
 * Clear all missions/map names, goals and aggression rates
 */
void cCampaign::Clear() {
    mDirectPath = false;
    mIsRandom = false;
    mIsCustomCampaign = false;
    mName = "";
    mCustomMap = "";

    mMissions.clear();
}

tSharedBuffer cCampaign::getMap(std::shared_ptr<cPhase> pPhase) const {
    std::string FinalName = pPhase->mMapFilename + ".map";
    std::string FinalPath = GetPath(FinalName, mName);

    // Custom map in use?
    if (mCustomMap.size()) {

        // Generated path doesnt exist, try load the absolute path
        if (local_FileExists(mCustomMap + ".map"))
            return local_FileRead(mCustomMap + ".map", "", eNone);

        // Otherwise use the custom map folder
        return local_FileRead(FinalName, "Custom/Maps/", eData);
    }

    // If a campaign folder exists, return a path inside it
    if (!local_FileExists(FinalPath))
        return g_Resource->fileGet(FinalName);

    // Otherwise fallback to loading the map from the currently loaded
    return local_FileRead(FinalPath, "", eNone);
}

tSharedBuffer cCampaign::getSprites(std::shared_ptr<cPhase> pPhase) const {
    std::string FinalName = pPhase->mMapFilename + ".spt";
    std::string FinalPath = GetPath(FinalName, mName);

    // Custom map in use?
    if (mCustomMap.size()) {

        // Generated path doesnt exist, try load the absolute path
        if (local_FileExists(mCustomMap + ".spt"))
            return local_FileRead(mCustomMap + ".spt", "", eNone);

        // Otherwise use the custom map folder
        return local_FileRead(FinalName, "Custom/Maps/", eData);
    }


    // If a campaign folder exists, return a path inside it
    if (!local_FileExists(FinalPath))
        return g_Resource->fileGet(FinalName);

    // Otherwise fallback to loading the map from the currently loaded
    return local_FileRead(FinalPath, "", eNone);
}

/**
 * Get the mission
 */
std::shared_ptr<cMission> cCampaign::getMission(size_t pMissionNumber) {
    if(!pMissionNumber)
        pMissionNumber = 1;

    if (!mMissions.size() || pMissionNumber > mMissions.size())
        return 0;

    return mMissions[pMissionNumber - 1];
}

void cCampaign::SetSingleMapCampaign() {
    mName = "Single Map";

    mMissions.clear();
    mMissions.push_back(std::make_shared<cMission>());
    mMissions.back()->mPhases.push_back(std::make_shared<cPhase>());
}

const std::string cCampaign::getName() const {
    return mName;
}

bool cCampaign::isCustom() const {

    return mIsCustomCampaign;
}

bool cCampaign::isRandom() const {

    return mIsRandom;
}
