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

sGameData::sGameData() {
	mMapNumber = 0;
	mMissionNumber = 0;

	Clear();
}

sGameData::sGameData(const std::string& pFromJson) {

	sGameData();

	if (!FromJson(pFromJson))
		throw new std::exception("Invalid Savegame");
}

void sGameData::Clear() {

	mMapNumber = 0;
	mMissionNumber = 0;
	mMissionPhase = 0;

	mSprite_Enemy_AggressionAverage = 0;
	mSprite_Enemy_AggressionMin = 0;
	mSprite_Enemy_AggressionMax = 0;
	mSprite_Enemy_AggressionNext = 0;
	mSprite_Enemy_AggressionIncrement = 0;

	mRecruits_Available_Count = 0;
	mMission_Troop_Prepare_SetFromSpritePtrs = 0;

	mSprite_Enemy_AggressionCreated_Count = 0;
	mMission_Recruits_AliveCount = 0;
	mMission_Recruitment = 0;
	mMission_TryingAgain = 0;
	mMission_Phases_Remaining = 0;
	mMission_Phases_Total = 0;
	mRecruit_NextID = 0;

	for (unsigned int x = 0; x < 9; ++x) {
		mMission_Troops[x].mRecruitID = 0;
		mMission_Troops[x].mRank = 0;
		mMission_Troops[x].mPhaseCount = 0;
		mMission_Troops[x].mSprite = 0;
		mMission_Troops[x].field_6 = 0;
		mMission_Troops[x].field_8 = 0;
		mMission_Troops[x].mNumberOfKills = 0;
	}

	for (unsigned int x = 0; x < 361; ++x)
		mGraveRanks[x] = 0;

	for (unsigned int x = 0; x < 361; ++x)
		mGraveRecruitID[x] = 0;


	for (unsigned int x = 0; x < 6; ++x) {
		mHeroes[x].mRecruitID = 0;
		mHeroes[x].mRank = 0;
		mHeroes[x].mKills = 0;
	}
	mTroops_Away = 0;
	mTroops_Home = 0;
	mMission_Troops_Required = 0;
	mMission_Troop_Count = 0;
	mMission_Troops_Available = 0;
}

void sGameData::Troops_Clear() {
	mMission_Troops[8].mRecruitID = -1;
	mMission_Troops[8].mSprite = INVALID_SPRITE_PTR;

	for (unsigned int x = 0; x < 8; ++x) {
		mMission_Troops[x].mSprite = INVALID_SPRITE_PTR;
		mMission_Troops[x].mRecruitID = -1;
		mMission_Troops[x].mRank = 0;
		mMission_Troops[x].mPhaseCount = 0;
	}
}

void sGameData::Heroes_Clear() {

	for (unsigned int x = 0; x < 6; ++x) {
		mHeroes[x].mRecruitID = -1;
		mHeroes[x].mRank = -1;
		mHeroes[x].mKills = -1;
	}
}

std::string sGameData::ToJson(const std::string& pName) {

	Json Save;

	// Not used yet
	Save["SaveVersion"] = 1;

	// Save the current game data version
	{
		Json Version;
		Version["mName"] = g_Fodder.mVersionCurrent->mName;
		Version["mGame"] = g_Fodder.mVersionCurrent->mGame;
		Version["mPlatform"] = g_Fodder.mVersionCurrent->mPlatform;
		Version["mRelease"] = g_Fodder.mVersionCurrent->mRelease;

		Save["DataVersion"] = Version;
	}
	
	// Keep the campaign
	Save["Campaign"] = g_Fodder.mCampaign.getName();
	Save["Name"] = pName;

	// Actual game states
	Save["mMapNumber"] = mMapNumber;
	Save["mSprite_Enemy_AggressionAverage"] = mSprite_Enemy_AggressionAverage;
	Save["mSprite_Enemy_AggressionMin"] = mSprite_Enemy_AggressionMin;
	Save["mSprite_Enemy_AggressionMax"] = mSprite_Enemy_AggressionMax;
	Save["mSprite_Enemy_AggressionNext"] = mSprite_Enemy_AggressionNext;
	Save["mSprite_Enemy_AggressionIncrement"] = mSprite_Enemy_AggressionIncrement;
	Save["mMissionNumber"] = mMissionNumber;
	Save["mMissionPhase"] = mMissionPhase;
	Save["mRecruits_Available_Count"] = mRecruits_Available_Count;
	Save["Mission_Troop_Prepare_SetFromSpritePtrs"] = mMission_Troop_Prepare_SetFromSpritePtrs;
	Save["mSprite_Enemy_AggressionCreated_Count"] = mSprite_Enemy_AggressionCreated_Count;
	Save["mMission_Recruits_AliveCount"] = mMission_Recruits_AliveCount;
	Save["mMission_Recruitment"] = mMission_Recruitment;
	Save["mMission_TryingAgain"] = mMission_TryingAgain;
	Save["mMission_Phases_Remaining"] = mMission_Phases_Remaining;
	Save["mMission_Phases_Total"] = mMission_Phases_Total;
	Save["mRecruit_NextID"] = mRecruit_NextID;

	for (auto& MissionTroop : mMission_Troops) {
		Json Troop;

		Troop["mRecruitID"] = MissionTroop.mRecruitID;
		Troop["mRank"] = MissionTroop.mRank;
		Troop["mPhaseCount"] = MissionTroop.mPhaseCount;
		Troop["field_6"] = MissionTroop.field_6;
		Troop["field_8"] = MissionTroop.field_8;
		Troop["mSelected"] = MissionTroop.mSelected;
		Troop["mNumberOfKills"] = MissionTroop.mNumberOfKills;

		Save["mMission_Troops"].push_back(Troop);
	}

	for (auto& GraveRank : mGraveRanks) {

		Save["mGraveRanks"].push_back(GraveRank);

	}

	for (auto& GraveRank : mGraveRecruitID) {

		Save["mGraveRecruitID"].push_back(GraveRank);
	}

	for (auto& Hero : mHeroes) {
		Json JsonHeroe;

		JsonHeroe["mRecruitID"] = Hero.mRecruitID;
		JsonHeroe["mRank"] = Hero.mRank;
		JsonHeroe["mKills"] = Hero.mKills;

		Save["mHeroes"].push_back(JsonHeroe);
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

		mSavedName = LoadedData["Name"];
		mCampaignName = LoadedData["Campaign"];

		mSavedVersion.mName = LoadedData["DataVersion"]["mName"];
		mSavedVersion.mGame = LoadedData["DataVersion"]["mGame"];
		mSavedVersion.mPlatform = LoadedData["DataVersion"]["mPlatform"];
		mSavedVersion.mRelease = LoadedData["DataVersion"]["mRelease"];

		mMapNumber = LoadedData["mMapNumber"];
		mSprite_Enemy_AggressionAverage = LoadedData["mSprite_Enemy_AggressionAverage"];
		mSprite_Enemy_AggressionMin = LoadedData["mSprite_Enemy_AggressionMin"];
		mSprite_Enemy_AggressionMax = LoadedData["mSprite_Enemy_AggressionMax"];
		mSprite_Enemy_AggressionNext = LoadedData["mSprite_Enemy_AggressionNext"];
		mSprite_Enemy_AggressionIncrement = LoadedData["mSprite_Enemy_AggressionIncrement"];
		mMissionNumber = LoadedData["mMissionNumber"];
		mMissionPhase = LoadedData["mMissionPhase"];
		mRecruits_Available_Count = LoadedData["mRecruits_Available_Count"];
		mMission_Troop_Prepare_SetFromSpritePtrs = LoadedData["Mission_Troop_Prepare_SetFromSpritePtrs"];
		mSprite_Enemy_AggressionCreated_Count = LoadedData["mSprite_Enemy_AggressionCreated_Count"];
		mMission_Recruits_AliveCount = LoadedData["mMission_Recruits_AliveCount"];
		mMission_Recruitment = LoadedData["mMission_Recruitment"];
		mMission_TryingAgain = LoadedData["mMission_TryingAgain"];
		mMission_Phases_Remaining = LoadedData["mMission_Phases_Remaining"];
		mMission_Phases_Total = LoadedData["mMission_Phases_Total"];
		mRecruit_NextID = LoadedData["mRecruit_NextID"];

		int x = 0;

		for (auto& MissionTroop : LoadedData["mMission_Troops"]) {
			mMission_Troops[x].mRecruitID = MissionTroop["mRecruitID"];
			mMission_Troops[x].mRank = MissionTroop["mRank"];
			mMission_Troops[x].mPhaseCount = MissionTroop["mPhaseCount"];
			mMission_Troops[x].field_6 = MissionTroop["field_6"];
			mMission_Troops[x].field_8 = MissionTroop["field_8"];
			mMission_Troops[x].mSelected = MissionTroop["mSelected"];
			mMission_Troops[x].mNumberOfKills = MissionTroop["mNumberOfKills"];

			++x;
			if (x == 9)
				break;
		}

		x = 0;
		for (auto& GraveRank : LoadedData["mGraveRanks"]) {
			mGraveRanks[x++] = GraveRank;
		}

		x = 0;
		for (auto& GraveRank : LoadedData["mGraveRecruitID"]) {
			mGraveRecruitID[x++] = GraveRank;
		}

		x = 0;
		for (auto& Hero : LoadedData["mHeroes"]) {

			mHeroes[x].mRecruitID = Hero["mRecruitID"];
			mHeroes[x].mRank = Hero["mRank"];
			mHeroes[x++].mKills = Hero["mKills"];
		}

		mTroops_Away = LoadedData["mTroops_Away"];
		mTroops_Home = LoadedData["mTroops_Home"];
		mMission_Troops_Required = LoadedData["mMission_Troops_Required"];
		mMission_Troop_Count = LoadedData["mMission_Troop_Count"];
		mMission_Troops_Available = LoadedData["mMission_Troops_Available"];

	 }
	 catch (std::exception Exception) {
		 std::cout << "SaveGame JSON Parsing Error: " << Exception.what() << "\n";
		 return false;
	}

	return true;
}