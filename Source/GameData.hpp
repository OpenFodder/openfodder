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

struct sMission_Troop {
	int16       mRecruitID;
	uint8       mRank;
	int8        mPhaseCount;        // Number of phases completed this Mission

	sSprite*    mSprite;

	uint16      field_6;
	int8        field_8;
	int8        mSelected;
	uint16      mNumberOfKills;
};

struct sHero {
	int8    mRecruitID;
	int16   mRank;
	int16   mKills;
};

struct sGameData {
	uint16          mMapNumber;
	int16           mSprite_Enemy_AggressionAverage;
	int16           mSprite_Enemy_AggressionMin;
	int16           mSprite_Enemy_AggressionMax;
	int16           mSprite_Enemy_AggressionNext;
	int16           mSprite_Enemy_AggressionIncrement;
	uint16          mMissionNumber;
	uint16          mMissionPhase;
	uint16          mRecruits_Available_Count;
	int16           Mission_Troop_Prepare_SetFromSpritePtrs;
	int16           mSprite_Enemy_AggressionCreated_Count;
	uint16          mMission_Recruits_AliveCount;
	int16           mMission_Recruitment;
	int16           mMission_TryingAgain;
	uint16          mMission_Phases_Remaining;
	uint16          mMission_Phases_Total;
	uint16          mRecruit_NextID;
	sMission_Troop  mMission_Troops[9];

	int16           mGraveRanks[361];
	int16           mGraveRecruitID[361];

	sHero           mHeroes[6];

	int16           mTroops_Away;
	int16           mTroops_Home;
	int16           mMission_Troops_Required;
	int16           mMission_Troop_Count;
	int16           mMission_Troops_Available;

	sGameData();

	void Clear();
	void Troops_Clear();
	void Heroes_Clear();

	std::string ToJson();
	bool		FromJson(const std::string pJson);
};
