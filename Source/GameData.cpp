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

	mTroops_Away = 0;
	mTroops_Home = 0;

	Troops_Clear();
	mSoldiers_Died.clear();
}

void sGameData::Troops_Clear() {

	for( auto& Troop : mSoldiers_Allocated) {
        Troop.mSprite = INVALID_SPRITE_PTR;
		Troop.mRecruitID = -1;
		Troop.mRank = 0;
		Troop.mPhaseCount = 0;
	}
}

/*void sGameData::Troops_Sort() {

    sort(mSoldiers_Allocated.begin(), mSoldiers_Allocated.end(),
        [](const sMission_Troop & a, const sMission_Troop & b) -> bool
    {
        // if rank is not equal
        if (a.mRank != b.mRank)
            return false;

        // Has less kills
        if (a.mNumberOfKills <= b.mNumberOfKills)
            return false;
        return true;
    });
}*/

void sGameData::Troop_Died(const sMission_Troop* pTroop) {
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

	Json Save;

	// Not used yet
	Save["SaveVersion"] = 2;

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
		Troop["mPhaseCount"] = MissionTroop.mPhaseCount;
		Troop["field_6"] = MissionTroop.field_6;
		Troop["mSelected"] = MissionTroop.mSelected;
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

	Save["mTroops_Away"] = mTroops_Away;
	Save["mTroops_Home"] = mTroops_Home;

	return Save.dump(1);
}

bool sGameData::FromJson(const std::string& pJson) {

	 try {
		Json LoadedData = Json::parse(pJson);

		Clear();

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
			mSoldiers_Allocated[x].mPhaseCount = MissionTroop["mPhaseCount"];
			mSoldiers_Allocated[x].field_6 = MissionTroop["field_6"];
			mSoldiers_Allocated[x].mSelected = MissionTroop["mSelected"];
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

		mTroops_Away = LoadedData["mTroops_Away"];
		mTroops_Home = LoadedData["mTroops_Home"];

	 } catch (std::exception Exception) {
		 std::cout << "SaveGame JSON Parsing Error: " << Exception.what() << "\n";
		 return false;
	}

	return true;
}