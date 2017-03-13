#include "stdafx.hpp"


const std::vector< size_t > mCustom_MissionPhaseTable = {
	01
};

const std::vector< std::vector<eMissionGoals> > mCustom_Map_Goals = {
	{ eGoal_Kill_All_Enemy }
};

// Hardcode custom maps to min 4, max 8 aggression
const std::vector<sAggression> mCustom_Map_Sprite_Enemy_Aggression = {
	{ 4,  8 }
};

const std::vector<std::string> mCustom_MissionNames;

// Custom
cMissionData mMissionData_Custom = {
	mCustom_MissionNames,
	mCustom_MissionNames,
	mCustom_MissionPhaseTable,
	mCustom_Map_Goals,
	mCustom_Map_Sprite_Enemy_Aggression
};
