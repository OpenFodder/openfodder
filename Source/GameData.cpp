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
	mMission_Troop_Prepare_SetFromSpritePtrs = 0;

	mMission_Recruits_AliveCount = 0;
	mMission_Recruitment = 0;
	mMission_Phases_Remaining = 0;
	mMission_Phases_Total = 0;
	mRecruit_NextID = 0;

    for (auto& Troop : mMission_Troops)
        Troop.Clear();

	mTroops_Away = 0;
	mTroops_Home = 0;
	mMission_Troops_Required = 0;
	mMission_Troop_Count = 0;
	mMission_Troops_Available = 0;
	
   
	Troops_Clear();
	mTroops_Died.clear();
}

void sGameData::Troops_Clear() {

	for( auto& Troop : mMission_Troops) {
        Troop.mSprite = INVALID_SPRITE_PTR;
		Troop.mRecruitID = -1;
		Troop.mRank = 0;
		Troop.mPhaseCount = 0;
	}
}

void sGameData::Troop_Died(const sMission_Troop* pTroop) {
    if (!pTroop)
        return;

    mTroops_Died.push_back(sHero(pTroop));
}

std::vector<sHero> sGameData::Heroes_Get() const {
    std::vector<sHero> Final = mTroops_Died;

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
	Save["mRecruits_Available_Count"] = mRecruits_Available_Count;
	Save["Mission_Troop_Prepare_SetFromSpritePtrs"] = mMission_Troop_Prepare_SetFromSpritePtrs;

	Save["mMission_Recruits_AliveCount"] = mMission_Recruits_AliveCount;
	Save["mMission_Recruitment"] = mMission_Recruitment;
	Save["mMission_Phases_Remaining"] = mMission_Phases_Remaining;
	Save["mMission_Phases_Total"] = mMission_Phases_Total;
	Save["mRecruit_NextID"] = mRecruit_NextID;

	for (auto& MissionTroop : mMission_Troops) {
		Json Troop;
		Troop["mRecruitID"] = MissionTroop.mRecruitID;
		Troop["mRank"] = MissionTroop.mRank;
		Troop["mPhaseCount"] = MissionTroop.mPhaseCount;
		Troop["field_6"] = MissionTroop.field_6;
		Troop["mSelected"] = MissionTroop.mSelected;
		Troop["mNumberOfKills"] = MissionTroop.mNumberOfKills;

		Save["mMission_Troops"].push_back(Troop);
	}

	for (auto& Hero : mTroops_Died) {
		Json JsonHero;

		JsonHero["mRecruitID"] = Hero.mRecruitID;
		JsonHero["mRank"] = Hero.mRank;
		JsonHero["mKills"] = Hero.mKills;

		Save["mHeroes"].push_back(JsonHero);
	}

	Save["mTroops_Away"] = mTroops_Away;
	Save["mTroops_Home"] = mTroops_Home;
	Save["mMission_Troops_Required"] = mMission_Troops_Required;
	Save["mMission_Troop_Count"] = mMission_Troop_Count;
	Save["mMission_Troops_Available"] = mMission_Troops_Available;

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
		mMission_Troop_Prepare_SetFromSpritePtrs = LoadedData["Mission_Troop_Prepare_SetFromSpritePtrs"];
		
		mMission_Recruits_AliveCount = LoadedData["mMission_Recruits_AliveCount"];
		mMission_Recruitment = LoadedData["mMission_Recruitment"];
		mMission_Phases_Remaining = LoadedData["mMission_Phases_Remaining"];
		mMission_Phases_Total = LoadedData["mMission_Phases_Total"];
		mRecruit_NextID = LoadedData["mRecruit_NextID"];

		int x = 0;
		for (auto& MissionTroop : LoadedData["mMission_Troops"]) {
			mMission_Troops[x].mRecruitID = MissionTroop["mRecruitID"];
			mMission_Troops[x].mRank = MissionTroop["mRank"];
			mMission_Troops[x].mPhaseCount = MissionTroop["mPhaseCount"];
			mMission_Troops[x].field_6 = MissionTroop["field_6"];
			mMission_Troops[x].mSelected = MissionTroop["mSelected"];
			mMission_Troops[x].mNumberOfKills = MissionTroop["mNumberOfKills"];
			if (++x == 9)
				break;
		}

		for (auto& Hero : LoadedData["mHeroes"]) {
			sHero Heroes;
			Heroes.mRecruitID = Hero["mRecruitID"];
			Heroes.mRank = Hero["mRank"];
			Heroes.mKills = Hero["mKills"];

			mTroops_Died.push_back(Heroes);
		}

		mTroops_Away = LoadedData["mTroops_Away"];
		mTroops_Home = LoadedData["mTroops_Home"];
		mMission_Troops_Required = LoadedData["mMission_Troops_Required"];
		mMission_Troop_Count = LoadedData["mMission_Troop_Count"];
		mMission_Troops_Available = LoadedData["mMission_Troops_Available"];

	 } catch (std::exception Exception) {
		 std::cout << "SaveGame JSON Parsing Error: " << Exception.what() << "\n";
		 return false;
	}

	return true;
}