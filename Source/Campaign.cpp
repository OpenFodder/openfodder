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

std::string cCampaign::GetPathToCampaign() const {

    return GetPath(false) + ".ofc";
}

std::string cCampaign::GetPathToFile(const std::string& pName) const {

    return GetPath() + pName;
}

/**
 * Get the path to the current campaign
 */
std::string cCampaign::GetPath( const bool pTrailingSeperator ) const {
    std::string path;

	if (mUseCustomPath)
		path = mPath;
	else
		path = g_ResourceMan->GetCampaignData(mName);

    if(pTrailingSeperator && path.size())
        path += gPathSeperator;

    return path;
}

bool cCampaign::LoadCustomMapFromPath(const std::string& pPath) {
    Clear();
    std::string CustomMapName = pPath;

    auto a = CustomMapName.find_last_of("/") + 1;
    CustomMapName.erase(CustomMapName.begin(), CustomMapName.begin() + a);

    LoadCustomMap(CustomMapName);

    // Change the path to this map
    std::shared_ptr<cPhase> Phase = mMissions.back()->mPhases.back();
    Phase->mMapFilename = pPath.substr(0, pPath.size() - 4);

    mUseCustomPath = true;
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

    // Map / Phase names must be upper case
    std::transform(CustomMapName.begin(), CustomMapName.end(), CustomMapName.begin(), ::toupper);

    SetSingleMapCampaign();
    std::shared_ptr<cMission> Mission = mMissions.back();
    std::shared_ptr<cPhase> Phase = Mission->mPhases.back();

    Mission->mName = CustomMapName;

    Phase->mMapFilename = pMapName.substr(0, pMapName.size() - 4);

    // TODO: Try load these from file before using defaults
    Phase->mName = CustomMapName;
    Phase->mGoals.push_back({ eObjective_Kill_All_Enemy });
    Phase->mAggression = { 4, 8 };

    mIsCustomMap = true;
    return true;
}

bool cCampaign::SaveCampaign() {
    Json Campaign;
    Campaign["Author"] = mAuthor;
    Campaign["Name"] = mName;

    // Each Mission
    for (auto CurrentMission : mMissions) {
        Json Mission;
        Mission["Name"] = CurrentMission->mName;

        // Each Phase of a mission
        for(auto CurrentPhase : CurrentMission->mPhases) {
            Json Phase;
            Phase["MapName"] = CurrentPhase->mMapFilename;
            Phase["Name"] = CurrentPhase->mName;
            Phase["Aggression"][0] = CurrentPhase->mAggression.mMin;
            Phase["Aggression"][1] = CurrentPhase->mAggression.mMax;

            Phase["Grenades"] = CurrentPhase->mGrenades;
            Phase["Rockets"] = CurrentPhase->mRockets;

            for (auto Goal : CurrentPhase->mGoals)
                Phase["Objectives"].push_back(mMissionGoal_Titles[Goal - 1]);

            Mission["Phases"].push_back(Phase);
        }
        Campaign["Missions"].push_back(Mission);
    }

    // Save the campaign
    std::ofstream MissionFile(GetPathToCampaign());
    if (MissionFile.is_open()) {
        MissionFile << Campaign.dump(1);
        MissionFile.close();
        return true;
    }

    return false;
}

/**
 * Load a campaign
 */
bool cCampaign::LoadCampaign(const std::string& pName, bool pCustom, bool pDirectPath) {

    if (!pName.size())
        return false;

    Clear();
    mName = pName;
    mPath = pName;
    mUseCustomPath = pDirectPath;

    if (mUseCustomPath) {
        auto Final = pName.find_last_of(gPathSeperator);

        mName = pName.substr(0, Final);
    }

    std::ifstream MissionSetFile(GetPathToCampaign());
    if (MissionSetFile.is_open()) {
        Json MissionSet = Json::parse(MissionSetFile);

        mAuthor = MissionSet["Author"];
        mName = MissionSet["Name"];

        mIsCustomCampaign = pCustom;

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

                if(Phase["Grenades"].size())
                    newPhase->mGrenades = Phase["Grenades"];

                if(Phase["Rockets"].size())
                    newPhase->mRockets = Phase["Rockets"];


                // Each map goal
                for (std::string ObjectiveName : Phase["Objectives"]) {
                    transform(ObjectiveName.begin(), ObjectiveName.end(), ObjectiveName.begin(), toupper);

                    // Check each goal
                    int x = 0;
                    for (const std::string& GoalTitle : mMissionGoal_Titles) {
                        ++x;
                        if (GoalTitle == ObjectiveName) {
                            newPhase->mGoals.push_back(static_cast<ePhaseObjective>(x));
                            break;
                        }
                    }
                }
            }
        }
        return true;
    }

    return false;
}

/**
 * Clear all missions/map names, goals and aggression rates
 */
void cCampaign::Clear(const std::string& pName, const bool pDirectPath) {
    mIsCustomMap = false;
    mUseCustomPath = pDirectPath;
    mIsRandom = false;
    mIsCustomCampaign = false;
    mName = pName;
    mAuthor = "";
    mPath = pName;

    if (mUseCustomPath) {
        auto Final = pName.find_last_of("/");
        mName = pName.substr(0, Final);
    }

    mMissions.clear();
}

std::shared_ptr<cMap> cCampaign::getCMap(std::shared_ptr<cPhase> pPhase) const {

	auto map = std::make_shared<cOriginalMap>(getMap(pPhase), getSprites(pPhase), mName == "Cannon Fodder 2");

	return map;
}

tSharedBuffer cCampaign::getMap(std::shared_ptr<cPhase> pPhase) const {
    std::string FinalName = pPhase->mMapFilename + ".map";
    std::string FinalPath = GetPathToFile(FinalName);

    // If no campaign folder exists, load from the currently loaded resource
    if (!g_ResourceMan->FileExists(FinalPath))
        return g_Resource->fileGet(FinalName);

    // Otherwise load it from the campaign path
    return g_ResourceMan->FileRead(FinalPath);
}

tSharedBuffer cCampaign::getSprites(std::shared_ptr<cPhase> pPhase) const {
    std::string FinalName = pPhase->mMapFilename + ".spt";
    std::string FinalPath = GetPathToFile(FinalName);

    // If no campaign folder exists, load from the currently loaded resource
    if (!g_ResourceMan->FileExists(FinalPath))
        return g_Resource->fileGet(FinalName);

    // Otherwise load it from the campaign path
    return g_ResourceMan->FileRead(FinalPath);
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
    Clear("Single Map", false);
    mPath = "";

    mMissions.push_back(std::make_shared<cMission>());
    mMissions.back()->mPhases.push_back(std::make_shared<cPhase>());
}

void cCampaign::SetCustomCampaign() {
	mIsCustomCampaign = true; 
	mUseCustomPath = false;

	Clear();
}

void cCampaign::CreateCustomCampaign() {
	SetCustomCampaign();

	mMissions.push_back(std::make_shared<cMission>());
	mMissions.back()->mPhases.push_back(std::make_shared<cPhase>());
}

std::string cCampaign::getName() const {
    return mName;
}

bool cCampaign::isCustom() const {

    return mIsCustomCampaign;
}

bool cCampaign::isRandom() const {

    return mIsRandom;
}
