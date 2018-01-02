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

enum eMissionGoals {
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

	sAggression(int16 pMin, int16 pMax) { 
		mMin = pMin; 
		mMax = pMax; 
	}
};

class cCampaign {
private:
    std::string                             mName;
    std::string                             mAuthor;

    std::vector<std::string>                mMissionNames;
    std::vector<size_t>                     mMissionPhases;

    std::vector<std::string>                mMapNames;
    std::vector<std::vector<eMissionGoals>> mMapGoals;
    std::vector<sAggression>                mMapAggression;
    std::vector<std::string>                mMapFilenames;

    std::string                             mCustomMap;
	bool									mIsCustomCampaign;
	bool									mIsRandom;

public:

    cCampaign();

    void Clear();

	bool LoadCustomFromPath(const std::string& pMapName);
    bool LoadCustomMap(const std::string& pMapName);

    bool LoadCampaign(const std::string& pName, bool pCustom);

    const std::string getName() const;

	std::string getMapFileName(size_t pMapNumber) const;
	tSharedBuffer getMap(const size_t pMapNumber) const;
	tSharedBuffer getSprites(const size_t pMapNumber) const;

    std::string getMissionName(size_t pMissionNumber) const;
    uint16 getNumberOfPhases(size_t pMissionNumber) const;
    std::string getMapName(const size_t& pMapNumber) const;

    const std::vector<eMissionGoals>& getMapGoals(const uint16& pMapNumber) const;
    const sAggression& getMapAggression(const uint16& pMapNumber) const;
    const size_t getMapCount() const;

	void setAggression(int16 pMin = 0, int16 pMax = 0);
	void setGoals(const std::vector<eMissionGoals>& pGoals);

	void setRandom( const bool pRandom = false ) { mIsRandom = pRandom;  }
	bool isCustom() const;
	bool isRandom() const;
};

extern const std::vector<std::string>   mMissionGoal_Titles;
