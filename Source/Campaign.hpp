/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

enum ePhaseObjective {
    eObjective_None = 0,
    eObjective_Kill_All_Enemy = 1,
    eObjective_Destroy_Enemy_Buildings = 2,
    eObjective_Rescue_Hostages = 3,
    eObjective_Protect_Civilians = 4,
    eObjective_Kidnap_Leader = 5,
    eObjective_Destroy_Factory = 6,
    eObjective_Destroy_Computer = 7,
    eObjective_Get_Civilian_Home = 8,
    eObjective_Activate_All_Switches = 9,        // CF2
    eObjective_Rescue_Hostage = 10,              // CF2: The CF2 engine has this as goal 3 instead of 10, and vica versa
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

	int16 getMin() const { return mMin; }
	void setMin(const int16& pMin) { mMin = pMin; }
	int16 getMax() const { return mMax; }
	void setMax(const int16& pMax) { mMax = pMax; }
};

class cPhase {
public:
    sAggression mAggression;
    std::string mName;
    std::string mMapFilename;
    std::vector<ePhaseObjective> mGoals;

    int16       mGrenades;
    int16       mRockets;

    cPhase() {
        mGrenades = -1;
        mRockets = -1;
    }

	void SetName(const std::string& pName) {
		mName = pName;
	}

    std::string GetName() const {
        std::string Name = mName;
        transform(Name.begin(), Name.end(), Name.begin(), toupper);
        return Name;
    }

	void SetMapFilename(const std::string& pMapFilename) {
		mMapFilename = pMapFilename;
	}

	std::string GetMapFilename() const {
		return mMapFilename;
	}

    void SetGoal(ePhaseObjective pGoal, int pValue) {
        if (pValue)
            AddGoal(pGoal);
        else
            RemoveGoal(pGoal);
    }

    void AddGoal(ePhaseObjective pGoal) {

        if (std::find(mGoals.begin(), mGoals.end(), pGoal) == mGoals.end())
            mGoals.push_back(pGoal);
    }

    void RemoveGoal(ePhaseObjective pGoal) {

        auto Goal = std::find(mGoals.begin(), mGoals.end(), pGoal);
        if (Goal != mGoals.end())
            mGoals.erase(Goal);
    }

	void ClearGoals() {
		mGoals.clear();
	}

	void SetAggression(const int16& pMin, const int16& pMax) {
		mAggression.setMin(pMin);
		mAggression.setMax(pMax);
	}

	void SetMinAggression(const int16& pMin) {
		mAggression.setMin(pMin);
	}

	void SetMaxAggression(const int16& pMax) {
		mAggression.setMax(pMax);
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

	void SetName(const std::string &pName) {
		mName = pName;
	}

    std::shared_ptr<cPhase> PhaseGet(size_t pPhase) {
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

    bool mIsCustomCampaign;
    bool mUseCustomPath;
    bool mIsRandom;
    bool mIsCustomMap;

protected:

    bool LoadCustomMap(const std::string& pMapName);

public:

    cCampaign();

    void Clear(const std::string& pName = "", const bool pDirectPath = false);

	bool LoadCustomMapFromPath(const std::string& pMapName);

    bool LoadCampaign(const std::string& pName, bool pCustom, bool pDirectPath = false);
    bool SaveCampaign();

    
    std::string GetPathToCampaign() const;
    std::string GetPathToFile(const std::string& pName) const;
    std::string GetPath(const bool pTrailingSeperator = true) const;

	std::shared_ptr<cMap> getCMap(std::shared_ptr<cPhase> pPhase) const;

	tSharedBuffer getMap(std::shared_ptr<cPhase> pPhase) const;
	tSharedBuffer getSprites(std::shared_ptr<cPhase> pPhase) const;

	void missionAdd(std::shared_ptr<cMission> pMission) { mMissions.push_back(pMission); }
    std::vector<std::shared_ptr<cMission>> getMissions() { return mMissions; }
    std::shared_ptr<cMission> getMission(size_t pMissionNumber);

    void SetSingleMapCampaign();
	void SetCustomCampaign();
	void CreateCustomCampaign();

	bool isCustom() const;
    bool isCustomMap() const { return mIsCustomMap; }
    void setCustomMap() { mIsCustomMap = true; mUseCustomPath = true; }

	bool isRandom() const;
    void setRandom(const bool pRandom = false) { mIsRandom = pRandom; }

    std::string getAuthor() { return mAuthor; }
    void setAuthor(const std::string& pAuthor) { mAuthor = pAuthor; }

	std::string getName() const;
	void setName(const std::string& pName) { mName = pName; }

};

extern const std::vector<std::string>   mMissionGoal_Titles;
