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

enum ePhaseGoals {
    eGoal_None = 0,
    eGoal_Kill_All_Enemy = 1,
    eGoal_Destroy_Enemy_Buildings = 2,
    eGoal_Rescue_Hostages = 3,
    eGoal_Protect_Civilians = 4,
    eGoal_Kidnap_Leader = 5,
    eGoal_Destroy_Factory = 6,
    eGoal_Destroy_Computer = 7,
    eGoal_Get_Civilian_Home = 8,
    eGoal_Activate_All_Switches = 9,        // CF2
    eGoal_Rescue_Hostage = 10,              // CF2: The CF2 engine has this as goal 3 instead of 10, and vica versa

    eGoal_End = -1
};

struct sAggression {
    int16 mMin;
    int16 mMax;

    int16 getAverage() const {

        return (mMin + mMax) / 2;
    }

    sAggression() {
        mMin = 4;
        mMax = 8;
    }
	sAggression(int16 pMin, int16 pMax) { 
		mMin = pMin; 
		mMax = pMax; 
	}
};

class cPhase {
public:
    sAggression mAggression;
    std::string mName;
    std::string mMapFilename;
    std::vector<ePhaseGoals> mGoals;

    std::string GetName() const {
        std::string Name = mName;
        transform(Name.begin(), Name.end(), Name.begin(), toupper);
        return Name;
    }

    void SetGoal(ePhaseGoals pGoal, int pValue) {
        if (pValue)
            AddGoal(pGoal);
        else
            RemoveGoal(pGoal);
    }
    void AddGoal(ePhaseGoals pGoal) {

        if (std::find(mGoals.begin(), mGoals.end(), pGoal) == mGoals.end())
            mGoals.push_back(pGoal);

    }

    void RemoveGoal(ePhaseGoals pGoal) {

        auto Goal = std::find(mGoals.begin(), mGoals.end(), pGoal);
        if (Goal != mGoals.end())
            mGoals.erase(Goal);
    }
};

class cMission {
public:
    std::string mName;
    std::vector<std::shared_ptr<cPhase>> mPhases;

    std::string GetName() const {
        std::string Name = mName;
        transform(Name.begin(), Name.end(), Name.begin(), toupper);
        return Name;
    }

    std::shared_ptr<cPhase> GetPhase(size_t pPhase) {
        if (!pPhase)
            pPhase = 1;

        if (!mPhases.size() || pPhase > mPhases.size())
            return 0;

        return mPhases[pPhase - 1];
    }

    size_t NumberOfPhases() const { return mPhases.size(); }
};

class cCampaign {
private:
    std::string                             mPath;
    std::string                             mName;
    std::string                             mAuthor;

    std::vector<std::shared_ptr<cMission>>  mMissions;

	bool									mIsCustomCampaign;
    bool                                    mUseCustomPath;
	bool									mIsRandom;
    bool                                    mIsCustomMap;

protected:

public:

    cCampaign();

    void Clear(const std::string& pName = "", const bool pDirectPath = false);

	bool LoadCustomMapFromPath(const std::string& pMapName);
    bool LoadCustomMap(const std::string& pMapName);

    bool LoadCampaign(const std::string& pName, bool pCustom, bool pDirectPath = false);
    bool SaveCampaign();

    const std::string getName() const;
    std::string GetPathToCampaign() const;
    std::string GetPathToFile(const std::string& pName) const;
    std::string GetPath(const bool pTrailingSeperator = true) const;

    tSharedBuffer getCampaign() const;
	tSharedBuffer getMap(std::shared_ptr<cPhase> pPhase) const;
	tSharedBuffer getSprites(std::shared_ptr<cPhase> pPhase) const;

    std::vector<std::shared_ptr<cMission>> &getMissions() { return mMissions; }
    std::shared_ptr<cMission> getMission(size_t pMissionNumber);

    void SetSingleMapCampaign();

	void setRandom( const bool pRandom = false ) { mIsRandom = pRandom;  }
	bool isCustom() const;
    bool isCustomMap() const { return mIsCustomMap; }

	bool isRandom() const;
    std::string getAuthor() { return mAuthor; }
    void setAuthor(const std::string& pAuthor) { mAuthor = pAuthor; }
};

extern const std::vector<std::string>   mMissionGoal_Titles;
