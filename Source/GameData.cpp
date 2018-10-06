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
#include <chrono>

#include "Utils/json.hpp"
using Json = nlohmann::json;

#define INVALID_SPRITE_PTR (sSprite*) -1

sGamePhaseData::sGamePhaseData() {
    Clear();
}

void sGamePhaseData::Clear() {
    mSprite_Enemy_AggressionAverage = 0;
    mSprite_Enemy_AggressionMin = 0;
    mSprite_Enemy_AggressionMax = 0;
    mSprite_Enemy_AggressionNext = 0;
    mSprite_Enemy_AggressionIncrement = 0;
    mSprite_Enemy_AggressionCreated_Count = 0;

    mSoldiers_Required = 0;
    mSoldiers_Allocated_Count = 0;
    mSoldiers_Available = 0;
    mSoldiers_Prepare_SetFromSpritePtrs = false;

    mTroops_DiedCount = 0;

    mIsComplete = false;

    for(auto& Goal : mGoals_Remaining)
        Goal = false;
}

sGameData::sGameData() {
	mMapNumber = 0;
	mMissionNumber = 0;

	Clear();
}

sGameData::sGameData(const std::string& pFromJson) {

	sGameData();

	if (!FromJson(pFromJson))
		throw std::runtime_error("Invalid Savegame");
}

void sGameData::Clear() {

	mMapNumber = 0;
	mMissionNumber = 0;
	mMission_Phase = 0;

	mRecruits_Available_Count = 0;

	mMission_Recruits_AliveCount = 0;
	mMission_Recruitment = 0;
	mMission_Phases_Remaining = 0;
	mMission_Phases_Total = 0;
	mRecruit_NextID = 0;

    for (auto& Troop : mSoldiers_Allocated)
        Troop.Clear();

	mScore_Kills_Away = 0;
	mScore_Kills_Home = 0;

	Soldier_Clear();
	mSoldiers_Died.clear();
}

void sGameData::Soldier_Clear() {

	for( auto& Troop : mSoldiers_Allocated) {
        
        Troop.Clear();
        Troop.mRecruitID = -1;
	}
}

void sGameData::Soldier_Sort() {

    // Remove any 'dead' troops
    for (int16 Data1c = 7; Data1c >= 0; --Data1c) {

        sMission_Troop* Data20 = mSoldiers_Allocated;

        for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

            if (Data20->mRecruitID == -1) {

                sMission_Troop* Data24 = Data20 + 1;

                *Data20 = *Data24;
                Data24->Clear();
                Data24->mRecruitID = -1;
            }
        }
    }

    // Sort by kills
    sMission_Troop* Data20 = mSoldiers_Allocated;
    for (int16 Data1c = 7; Data1c >= 0; --Data1c, ++Data20) {
        sMission_Troop* Data24 = mSoldiers_Allocated;

        for (int16 Data18 = 7; Data18 >= 0; --Data18, ++Data24) {

            if (Data20 == Data24)
                continue;

            if (Data20->mRecruitID == -1 || Data24->mRecruitID == -1)
                continue;

            if (Data20->mRank != Data24->mRank)
                continue;

            if (Data20->mNumberOfKills <= Data24->mNumberOfKills)
                continue;

            sMission_Troop Spare = *Data20;

            *Data20 = *Data24;
            *Data24 = Spare;
        }
    }
}

void sGameData::Soldier_Died(const sMission_Troop* pTroop) {
    if (!pTroop)
        return;

    mSoldiers_Died.push_back(sHero(pTroop));
    /*
    for(size_t x = 0; x < mSoldiers_Allocated.size(); ++x) {
        if (&mSoldiers_Allocated[x] == pTroop) {
            mSoldiers_Allocated.erase(mSoldiers_Allocated.begin() + x);
            break;
        }
    }*/

}

std::vector<sHero> sGameData::Heroes_Get() const {
    std::vector<sHero> Final = mSoldiers_Died;

	sort(Final.begin(), Final.end(),
		[](const sHero & a, const sHero & b) -> bool
	{
		if (!a.mKills)
			return false;
		
		// Has less kills
		if (a.mKills < b.mKills)
			return false;

		// Has more kills
		if (a.mKills > b.mKills)
			return true;

		// If kills are equal, check rank
		if (a.mRank <= b.mRank)
			return false;

		return true;
	});

    Final.resize(5);
    return Final;
}

std::string sGameData::ToJson(const std::string& pSaveName) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

	Json Save;

	// Not used yet
	Save["SaveVersion"] = 2;
    Save["Timestamp"] = in_time_t;

	// Save the current game data version
	{
		Json Version;
		Version["mName"] = g_Fodder->mVersionCurrent->mName;
		Version["mGame"] = g_Fodder->mVersionCurrent->mGame;
		Version["mPlatform"] = g_Fodder->mVersionCurrent->mPlatform;
		Version["mRelease"] = g_Fodder->mVersionCurrent->mRelease;

		Save["DataVersion"] = Version;
	}
	
	// Keep the campaign
	Save["Campaign"] = g_Fodder->mCampaign.getName();
	Save["SaveName"] = pSaveName;

	// Actual game states
	Save["mMapNumber"] = mMapNumber;

	Save["mMissionNumber"] = mMissionNumber;
	Save["mMissionPhase"] = mMission_Phase;
	
	Save["mMission_Recruits_AliveCount"] = mMission_Recruits_AliveCount;
	Save["mMission_Recruitment"] = mMission_Recruitment;
	Save["mMission_Phases_Remaining"] = mMission_Phases_Remaining;
	Save["mMission_Phases_Total"] = mMission_Phases_Total;
    
    Save["mRecruits_Available_Count"] = mRecruits_Available_Count; 
    Save["mRecruit_NextID"] = mRecruit_NextID;

	for (auto& MissionTroop : mSoldiers_Allocated) {
		Json Troop;
		Troop["mRecruitID"] = MissionTroop.mRecruitID;
		Troop["mRank"] = MissionTroop.mRank;
		Troop["field_6"] = MissionTroop.field_6;
		Troop["mNumberOfKills"] = MissionTroop.mNumberOfKills;

		Save["mMission_Troops"].push_back(Troop);
	}

	for (auto& Hero : mSoldiers_Died) {
		Json JsonHero;

		JsonHero["mRecruitID"] = Hero.mRecruitID;
		JsonHero["mRank"] = Hero.mRank;
		JsonHero["mKills"] = Hero.mKills;

		Save["mHeroes"].push_back(JsonHero);
	}

	Save["mTroops_Away"] = mScore_Kills_Away;
	Save["mTroops_Home"] = mScore_Kills_Home;

	return Save.dump(1);
}

bool sGameData::FromJson(const std::string& pJson) {
    Json LoadedData;

	 try {
		LoadedData = Json::parse(pJson);
     }
     catch (std::exception Exception) {
         std::cout << "SaveGame JSON Parsing Error: " << Exception.what() << "\n";
         return false;
     }

     Clear();

     uint64 Version = LoadedData["SaveVersion"];
     if (Version >= 2) {
         try {
             // Save["Timestamp"]
         }
         catch (std::exception Exception) {
             std::cout << "V2 Elements not found: " << Exception.what() << "\n";
             return false;
         }
     }

     if (Version >= 1) {
         try {
             mSavedName = LoadedData["SaveName"];
             mCampaignName = LoadedData["Campaign"];

             mSavedVersion.mName = LoadedData["DataVersion"]["mName"];
             mSavedVersion.mGame = LoadedData["DataVersion"]["mGame"];
             mSavedVersion.mPlatform = LoadedData["DataVersion"]["mPlatform"];
             mSavedVersion.mRelease = LoadedData["DataVersion"]["mRelease"];

             mMapNumber = LoadedData["mMapNumber"];

             mMissionNumber = LoadedData["mMissionNumber"];
             mMission_Phase = LoadedData["mMissionPhase"];
             mRecruits_Available_Count = LoadedData["mRecruits_Available_Count"];

             mMission_Recruits_AliveCount = LoadedData["mMission_Recruits_AliveCount"];
             mMission_Recruitment = LoadedData["mMission_Recruitment"];
             mMission_Phases_Remaining = LoadedData["mMission_Phases_Remaining"];
             mMission_Phases_Total = LoadedData["mMission_Phases_Total"];
             mRecruit_NextID = LoadedData["mRecruit_NextID"];

             int x = 0;
             for (auto& MissionTroop : LoadedData["mMission_Troops"]) {
                 mSoldiers_Allocated[x].mRecruitID = MissionTroop["mRecruitID"];
                 mSoldiers_Allocated[x].mRank = MissionTroop["mRank"];
                 mSoldiers_Allocated[x].field_6 = MissionTroop["field_6"];
                 mSoldiers_Allocated[x].mNumberOfKills = MissionTroop["mNumberOfKills"];
                 if (++x == 9)
                     break;
             }

             for (auto& Hero : LoadedData["mHeroes"]) {
                 sHero Heroes;
                 Heroes.mRecruitID = Hero["mRecruitID"];
                 Heroes.mRank = Hero["mRank"];
                 Heroes.mKills = Hero["mKills"];

                 mSoldiers_Died.push_back(Heroes);
             }

             mScore_Kills_Away = LoadedData["mTroops_Away"];
             mScore_Kills_Home = LoadedData["mTroops_Home"];

         }
         catch (std::exception Exception) {
             std::cout << "V1 Elements not found: " << Exception.what() << "\n";
             return false;
         }
     }

	return true;
}