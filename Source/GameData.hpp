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


struct sFodderParameters {
    bool mSkipIntro;            // Skip the OpenFodder intro, and the game intro
    bool mSkipRecruit;        // Skip the recruit screen and go straight into the mission
    bool mSkipBriefing;         // Skip mission briefing
    bool mSkipService;          // Skip mission debrief

    bool mWindowMode;           // Start in a window
    bool mRandom;
    ePlatform mDefaultPlatform;

    bool mDemoRecord;
    bool mDemoPlayback;

    std::string mDemoFile;

    std::string mCampaignName;
    size_t mMissionNumber;
    size_t mPhaseNumber;

    bool mUnitTesting;
    bool mSinglePhase;

    sFodderParameters() {
        mSkipService = false;
        mSkipBriefing = false;
        mSkipIntro = false;
        mSkipRecruit = false;
        mMissionNumber = 0;
        mPhaseNumber = 0;
        mWindowMode = false;
        mRandom = false;
        mDefaultPlatform = ePlatform::Any;
        mDemoRecord = false;
        mDemoPlayback = false;
        mUnitTesting = false;
        mSinglePhase = false;
    }

    std::string ToJson();
    bool		FromJson(const std::string& pJson);
};

struct sMission_Troop {
    int16       mRecruitID;
    uint8       mRank;
    uint8       mPhaseCount;        // Number of phases completed this Mission

    sSprite*    mSprite;

    uint16      field_6;            // Unknown: This value is referenced, but it has no effect
    int8        mSelected;
    uint16      mNumberOfKills;

    sMission_Troop() {
        Clear();

        mRecruitID = -1;
    }

    void Clear() {
        mPhaseCount = 0;
        mRecruitID = 0;
        mRank = 0;
        mNumberOfKills = 0;

        mSprite = (sSprite*)-1;

        field_6 = 0;
        mSelected = 0;
    }

    uint8 GetPromotedRank() const {
        uint8 newRank = (mPhaseCount + mRank);
        return (newRank > 0x0F) ? 0x0F : newRank;
    }

    void Promote() {

        if(mRecruitID!= -1)
            mRank = GetPromotedRank();
    }
};

struct sHero {
    int16   mRecruitID;
    int16   mRank;
    int16   mKills;

    sHero() {
        mRecruitID = -1;
        mRank = 0;
        mKills = 0;
    }

    sHero(const sMission_Troop* pTroop) {
        mRecruitID = pTroop->mRecruitID;
        mRank = pTroop->mRank;
        mKills = pTroop->mNumberOfKills;
    }
};

struct sGamePhaseData {

    int16           mSoldiers_Required;
    int16           mSoldiers_Allocated_Count;
    int16           mSoldiers_Available;
    bool            mSoldiers_Prepare_SetFromSpritePtrs;

    // Aggression is set prior to entering a map
    int16           mSprite_Enemy_AggressionAverage;
    int16           mSprite_Enemy_AggressionMin;
    int16           mSprite_Enemy_AggressionMax;
    int16           mSprite_Enemy_AggressionNext;
    int16           mSprite_Enemy_AggressionIncrement;
    int16           mSprite_Enemy_AggressionCreated_Count;

    bool            mGoals_Remaining[10];

    size_t          mTroops_DiedCount;  // Count of number of heroes who had died before mission started

    bool            mIsComplete;

    sGamePhaseData();

    void Clear();
};

struct cEventRecorded {
    cEvent mEvent;

};

struct cStateRecorded {
    int16 mInputMouseX, mInputMouseY;
    int16 mMouseButtonStatus;
};

struct sGameData;

struct sGameRecorded {
    int16 mSeed[4];
    int16 mInputTicks;
    int16 mEngineTicks;

    sFodderParameters mParams;

    std::multimap< uint64, cEventRecorded > mEvents;
    std::map< uint64, cStateRecorded > mState;

    void AddEvent(const uint64 pTicks, const cEventRecorded& pEvent) {
        mEvents.insert(mEvents.end(), std::make_pair(pTicks, pEvent));
    }

    std::vector<cEventRecorded> GetEvents(const uint64 pTicks) {
        std::vector<cEventRecorded> Events;
        auto test = mEvents.equal_range(pTicks);

        for( auto Event = test.first; Event != test.second; ++Event)
            Events.push_back(Event->second);
        
        return Events;
    }

    void AddState(const uint64 pTicks, const cStateRecorded& pEvent) {
        mState.insert(mState.end(), std::make_pair(pTicks, pEvent));
    }

    cStateRecorded* GetState(const uint64 pTicks) {

        if (mState.find(pTicks) != mState.end())
            return &mState.find(pTicks)->second;

        return 0;
    }

    void clear();
    void playback();

    void save();

    std::string ToJson();
    bool		FromJson(const std::string& pJson);
};

struct sGameData {
    sGamePhaseData  mGamePhase_Data;

    cCampaign       mCampaign;
    sGameRecorded   mDemoRecorded;
    uint64          mGameTicks;

    std::shared_ptr<cMission>        mMission_Current;
    std::shared_ptr<cPhase>          mPhase_Current;
    
    uint16          mMission_Number; // Starts at 1
    uint16          mMission_Phase; // Starts at 1

    uint16          mRecruits_Available_Count;

    uint16          mMission_Recruits_AliveCount;
    int16           mMission_Recruitment;
    uint16          mMission_Phases_Remaining;
    uint16          mRecruit_NextID;

    sMission_Troop  mSoldiers_Allocated[9];

    std::vector<sHero> mSoldiers_Died;

    int16           mScore_Kills_Away;    // Player soldiers killed
    int16           mScore_Kills_Home;    //  Enemy soldiers killed

    bool            mGameWon;

    std::string		mCampaignName;
    std::string		mSavedName;
    sRelease		mSavedVersion;

    sGameData();
    sGameData(const std::string& pFromJson);

    void Clear();

    bool Phase_Start();
    bool Phase_Next();

    void Soldier_Clear();
    void Soldier_Sort();
    void Soldier_Died(const sMission_Troop* pTroop);

    std::vector<sHero> Heroes_Get() const;

    std::string ToJson(const std::string& pName);
    bool		FromJson(const std::string& pJson);
};
