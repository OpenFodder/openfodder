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

const char* DEMO_EXTENSION = ".ofd";
const size_t DEMO_CURRENT_VERSION = 3;

sGamePhaseData::sGamePhaseData() {
    Clear();
}

void sGamePhaseData::Clear() {
    mSprite_Enemy_AggressionAverage = 0;
    mSprite_Enemy_AggressionMin = 0;
    mSprite_Enemy_AggressionMax = 0;
    mSprite_Enemy_AggressionMax = 0;
    mSprite_Enemy_AggressionNext = 0;
    mSprite_Enemy_AggressionIncrement = 0;
    mSprite_Enemy_AggressionCreated_Count = 0;

    mSoldiers_Required = 0;
    mSoldiers_Allocated_Count = 0;
    mSoldiers_Available = 0;
    mSoldiers_Prepare_SetFromSpritePtrs = false;

    mHeroesCount = 0;

    mIsComplete = false;

    for(auto& Goal : mGoals_Remaining)
        Goal = false;
}

sGameRecorded::sGameRecorded() {
    mVersion = DEMO_CURRENT_VERSION;
    mTick = 0;
    mTickDisabled = false;

    mSeed[0] = mSeed[1] = mSeed[2] = mSeed[3] = 0;
    mInputTicks = 0;
    mEngineTicks = 0;
    mRecordedPlatform = ePlatform::Any;
	mParams = std::make_shared<sFodderParameters>();
}
void sGameRecorded::AddEvent(const uint64 pTicks, const cEvent& pEvent) {
    if (mTickDisabled)
        return;

    mEvents.insert(mEvents.end(), std::make_pair(pTicks, pEvent));
}

std::vector<cEvent> sGameRecorded::GetEvents(const uint64 pTicks) {
    std::vector<cEvent> Events;

    if (mTickDisabled)
        return Events;

    auto test = mEvents.equal_range(pTicks);
    for (auto Event = test.first; Event != test.second; ++Event)
        Events.push_back(Event->second);

    return Events;
}

void sGameRecorded::AddState(const uint64 pTicks, const cStateRecorded& pEvent) {
    if (mTickDisabled)
        return;

    mState.insert(mState.end(), std::make_pair(pTicks, pEvent));
}

cStateRecorded* sGameRecorded::GetState(const uint64 pTicks) {
    if (mTickDisabled)
        return 0;

    if (mState.find(pTicks) != mState.end())
        return &mState.find(pTicks)->second;

    return 0;
}

uint64 sGameRecorded::GetTotalTicks() const {
    if (!mState.size())
        return 0;

    return mState.rbegin()->first;
}

void sGameRecorded::removeFrom(const uint64 pTicks) {
    auto from = mState.lower_bound(pTicks);
    mState.erase(from, mState.end());

    auto fromEvent = mEvents.lower_bound(pTicks);
    mEvents.erase(fromEvent, mEvents.end());
}

void sGameRecorded::clear() {
    mVersion = DEMO_CURRENT_VERSION;

    mState.clear();
    mEvents.clear();

    mTick = 0;
    mTickDisabled = false;

	g_Fodder->mRandom.getSeeds(mSeed[0], mSeed[1], mSeed[2], mSeed[3]);

    mInputTicks = g_Fodder->mGame_InputTicks;
    mEngineTicks = g_Fodder->mMission_EngineTicks;

    if (g_Fodder->mVersionCurrent)
        mRecordedPlatform = g_Fodder->mVersionCurrent->mPlatform;
    else
        mRecordedPlatform = g_Fodder->mParams->mDefaultPlatform;

    *mParams = *g_Fodder->mParams;
}

void sGameRecorded::playback() {

	//
    g_Fodder->mMission_EngineTicks = mEngineTicks;
    g_Fodder->mGame_InputTicks = mInputTicks;
    mTick = 0;
    mTickDisabled = false;
    g_Fodder->mRandom.setSeed(mSeed[0], mSeed[1], mSeed[2], mSeed[3]);

    *g_Fodder->mParams = *mParams;
}

void sGameRecorded::DisableTicks() {
    // Version 1 Demos didnt stop ticks 
    // Version 2 Demos stop ticks during image fadeout/fadein
	// Version 3 Demos have an greater number of sprites
    if (mVersion >= 2)
        mTickDisabled = true;
}

void sGameRecorded::EnableTicks() {
    if(mVersion >= 2)
        mTickDisabled = false;
}

void sGameRecorded::Tick() {
    if(!mTickDisabled)
        ++mTick;
}

void sGameRecorded::save() {

    if (g_Fodder->mParams->mDemoRecord) {
        std::string Filename = g_Fodder->mParams->mDemoFile;
        if (Filename == "-") {
            Filename = std::to_string(g_Fodder->mGame_Data.mMission_Number);
            Filename += "-";
            Filename += std::to_string(g_Fodder->mGame_Data.mMission_Phase);
        }

        if (Filename.find(".") == Filename.npos)
            Filename += DEMO_EXTENSION;

        g_Debugger->Notice("Writing demo to " + Filename);
        std::ofstream outfile(Filename, std::ofstream::binary);
        outfile << ToJson();
        outfile.close();
    }
}

std::string sGameRecorded::ToJson() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    Json Save;

    Save["SaveVersion"] = mVersion;

    Save["Timestamp"] = in_time_t;
    Save["Seed1"] = mSeed[0];
    Save["Seed2"] = mSeed[1];
    Save["Seed3"] = mSeed[2];
    Save["Seed4"] = mSeed[3];
    Save["InputTicks"] = mInputTicks;
    Save["mEngineTicks"] = mEngineTicks;

    Save["mParams"] = mParams->ToJson();
    Save["mPlatform"] = mRecordedPlatform;

    for (auto& Event : mEvents) {
        Json JsonHero;

        JsonHero["1"] = Event.first;
        JsonHero["2"] = Event.second.mButton;
        JsonHero["3"] = Event.second.mButtonCount;
        JsonHero["4"] = Event.second.mPosition.mX;
        JsonHero["5"] = Event.second.mPosition.mY;
        JsonHero["6"] = Event.second.mType;

        Save["mEvents"].push_back(JsonHero);
    }

    for (auto& State : mState) {
        Json JsonHero;

        JsonHero["1"] = State.first;
        JsonHero["2"] = State.second.mInputMouseX;
        JsonHero["3"] = State.second.mInputMouseY;
        JsonHero["4"] = State.second.mMouseButtonStatus;

        Save["mStates"].push_back(JsonHero);
    }

    return Save.dump(-1);
}

bool sGameRecorded::FromJson(const std::string& pJson) {
    Json LoadedData;

    try {
        LoadedData = Json::parse(pJson);
    }
    catch (std::exception Exception) {
        std::cout << "SaveGame JSON Parsing Error: " << Exception.what() << "\n";
        return false;
    }

    clear();

    mVersion = LoadedData["SaveVersion"];
    if (mVersion >= 1) {
        try {
            mSeed[0] = LoadedData["Seed1"];
            mSeed[1] = LoadedData["Seed2"];
            mSeed[2] = LoadedData["Seed3"];
            mSeed[3] = LoadedData["Seed4"];
            mInputTicks = LoadedData["InputTicks"];
            mEngineTicks = LoadedData["mEngineTicks"];
            mRecordedPlatform = LoadedData["mPlatform"];

            mParams->FromJson(LoadedData["mParams"]);
            for (auto& Event : LoadedData["mEvents"]) {
                uint32 Ticks = Event["1"];

                cEvent EventRecorded;

                EventRecorded.mButton = Event["2"];
                EventRecorded.mButtonCount = Event["3"];
                EventRecorded.mPosition.mX = Event["4"];
                EventRecorded.mPosition.mY = Event["5"];
                EventRecorded.mType = Event["6"];

                mEvents.insert(mEvents.end(), std::make_pair(Ticks, EventRecorded ));
            }

            for (auto& State : LoadedData["mStates"]) {
                uint32 Ticks = State["1"];

                cStateRecorded StateRecorded;

                StateRecorded.mInputMouseX = State["2"];
                StateRecorded.mInputMouseY = State["3"];
                StateRecorded.mMouseButtonStatus = State["4"];

                mState.insert(mState.end(), std::make_pair( Ticks, StateRecorded ));
            }
        }
        catch (std::exception Exception) {
            std::cout << "V1 Elements not found: " << Exception.what() << "\n";
            return false;
        }
    }

    return true;
}

sGameData::sGameData() {

	Clear();
}

sGameData::sGameData(const std::string& pFromJson) {

	sGameData();

	if (!FromJson(pFromJson))
		throw std::runtime_error("Invalid Savegame");
}

void sGameData::Clear() {

    mGameWon = false;
    mMission_Current = 0;
    mPhase_Current = 0;

	mMission_Number = 1;
	mMission_Phase = 1;

	mRecruits_Available_Count = 0;

	mMission_Recruits_AliveCount = 0;
	mMission_Recruitment = 0;
	mMission_Phases_Remaining = 0;
	mRecruit_NextID = 0;

    for (auto& Troop : mSoldiers_Allocated)
        Troop.Clear();

	mScore_Kills_Away = 0;
	mScore_Kills_Home = 0;

	Soldier_Clear();
	mHeroes.clear();
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

    mHeroes.push_back(sHero(pTroop));
}

std::vector<sHero> sGameData::Heroes_Get() const {
    std::vector<sHero> Final = mHeroes;

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

bool sGameData::Phase_Start() {
    mMission_Current = mCampaign.getMission(mMission_Number);
    if (!mMission_Current)
        return false;

    mPhase_Current = mMission_Current->PhaseGet(mMission_Phase);
    if (!mPhase_Current)
        return false;

    mMission_Phases_Remaining = (int16)mMission_Current->NumberOfPhases() - (mMission_Phase-1);

    // Mission Complete
    for (auto& Troop : mSoldiers_Allocated) {
        Troop.mPhaseCount = 0;
    }

    mRecruits_Available_Count += 0x0F;
    mMission_Recruits_AliveCount = mRecruits_Available_Count;
    mMission_Recruits_AliveCount -= 0x0F;
    mMission_Recruitment = -1;

    mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs = false;
    mGamePhase_Data.mHeroesCount = mHeroes.size();
    return true;
}

/**
 * Proceed to next phase/mission
 *
 * @return false If no more missions
 */
bool sGameData::Phase_Next() {

    if (!mMission_Current) {
        mMission_Current = mCampaign.getMission(mMission_Number);
        if (!mMission_Current)
            return false;

        mMission_Phases_Remaining = (int16)mMission_Current->NumberOfPhases();
    }

    // Next Phase
    ++mMission_Phase;
    --mMission_Phases_Remaining;

    mPhase_Current = mMission_Current->PhaseGet(mMission_Phase);

    // Still got phases to complete?
    if (mMission_Phases_Remaining)
        return true;

    // Mission Complete
    for (auto& Troop : mSoldiers_Allocated) {
        Troop.mPhaseCount = 0;
    }

    mMission_Current = mCampaign.getMission(++mMission_Number);
    if (!mMission_Current)
        return false;

    mMission_Phase = 1;

    mMission_Phases_Remaining = (int16) mMission_Current->NumberOfPhases();

    mPhase_Current = mMission_Current->PhaseGet(mMission_Phase);
    if (!mPhase_Current)
        return false;

    mRecruits_Available_Count += 0x0F;
    mMission_Recruits_AliveCount = mRecruits_Available_Count;
    mMission_Recruits_AliveCount -= 0x0F;
    mMission_Recruitment = -1;

    mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs = false;
    mGamePhase_Data.mHeroesCount = mHeroes.size();
    return true;
}

std::string sGameData::ToJson(const std::string& pSaveName) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

	Json Save;

	// Not used yet
	Save["SaveVersion"] = 1;
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
	Save["Campaign"] = mCampaign.getName();
	Save["SaveName"] = pSaveName;

	// Actual game states
	Save["mMissionNumber"] = mMission_Number;

	Save["mMission_Recruits_AliveCount"] = mMission_Recruits_AliveCount;
	Save["mMission_Recruitment"] = mMission_Recruitment;

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

	for (auto& Hero : mHeroes) {
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

             mMission_Number = LoadedData["mMissionNumber"];
             mRecruits_Available_Count = LoadedData["mRecruits_Available_Count"];

             mMission_Recruits_AliveCount = LoadedData["mMission_Recruits_AliveCount"];
             mMission_Recruitment = LoadedData["mMission_Recruitment"];
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

                 mHeroes.push_back(Heroes);
             }

             mScore_Kills_Away = LoadedData["mTroops_Away"];
             mScore_Kills_Home = LoadedData["mTroops_Home"];

         }
         catch (std::exception Exception) {
             std::cout << "V1 Elements not found: " << Exception.what() << "\n";
             return false;
         }
     }

    mMission_Current = mCampaign.getMission(mMission_Number);
    if (mMission_Current) {
        mPhase_Current = mMission_Current->PhaseGet(0);
        mMission_Phases_Remaining = (int16)mMission_Current->NumberOfPhases();
    }
	return true;
}