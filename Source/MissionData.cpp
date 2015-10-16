/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield <robcrossfield@gmail.com>
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

const std::string mMapTypes[] = {
	"jun",
	"des",
	"ice",
	"mor",
	"int",
	"hid",
	"afx"		// Amiga Format Christmas Special
};

const uint16 mRetail_MissionPhaseTable[] = {	01, 02, 01, 04, 03, 02, 03, 04,
												02, 05, 03, 06, 01, 03, 03, 02,
												01, 05, 01, 04, 01, 04, 05, 06 };

const uint16 mAmigaFormat_MissionPhaseTable[]	= { 01, 01 };
const uint16 mPlus_MissionPhaseTable[]			= { 01, 01, 01, 01 };

const char* mAmigaFormat_MissionNames[] = {
	"LAND OF HOPE AND GLORY",
	"IT'S SNOW TIME!"
};

const char* mPlus_MissionNames[] = {
	"STAGE 1",
	"STAGE 2",
	"STAGE 3",
	"STAGE 4"
};

const char* mRetail_MissionNames[] = {
	"THE SENSIBLE INITIATION",
	"ONWARD VIRGIN SOLDIERS",
	"ANTARCTIC ADVENTURE",
	"SUPER SMASHING NAMTASTIC",
	"THOSE VICIOUS VIKINGS",
	"WESTWARD HO",
	"GREENLAND REDBLOOD",
	"GUERRILLA WARFARE",
	"GREAT SCOTT GOOD SHOT",
	"ONE GIGANTIC DUST BOWL",
	"JUNGLE, MORE JUNGLE",
	"CHILLER THRILLER KILLER",
	"MORE MOORS"
	"BOMB ALLEY",
	"GET ORF MOI LAAND",
	"GOING UNDERGROUND",
	"THE MOOR THE MERRIER",
	"UNDERPANTS ELECTRIC",
	"DIABLO DOWNSTAIRS",
	"MOORS THE PITY",
	"EXPLORE MY HOLE",
	"DESERT DISASTER",
	"MOORS ET MORTEM",
	"BOMB THE BASE",
};

const char* mRetail_MissionPhaseNames[] = {
	"IT'S A JUNGLE OUT THERE",
	"BRIDGE OVER THE RIVER PIE",
	"TRASH ENEMY HQ",
	"BLAST, IT'S COLD",
	"BEACHY HEAD",
	"PIER PRESSURE",
	"VILLAGE PEOPLE",
	"QUICKSAND",
	"THE VALLEY OF ICE",
	"BARMY BAZOOKAS",
	"MY BEAUTIFUL SKIDU",
	"THE GRAND CANYON",
	"TRIGGER HAPPY",
	"THE SLIPPERY STAIRWAY",
	"RETURN TO REYKJAVIK",
	"EVIL KNIEVEL",
	"HAVE A NICE TRIP",
	"BANG BANG YOU'RE DEAD",
	"DELIVERANCE",
	"JEEP JUMP",
	"ROUND THE GARDEN",
	"IN AT THE DEEP END",
	"THE SQUARE DANCE",
	"PENNY FOR THE GUY",
	"TANKY IN THE MIDDLE",
	"IF IT MOVES KILL IT",
	"A GOOD HARD TANK",
	"LORD OF THE FLIES",
	"WHOPPER CHOPPER",
	"DONKEYTASTIC",
	"AN ICICLE MADE FOR TWO",
	"TANK YOU VERY MUCH",
	"DEATH AND GLORY",
	"NORTH FACE OF THE EIGER",
	"RESCUE EL PRESIDENTE",
	"CHILL OUT ICEMAN",
	"MUCH MUCH MOOR",
	"IT'S ALL MINE",
	"SANDY CRACK",
	"AIRLIFT",
	"SHEEP DIP",
	"CHOCS AWAY CHAPPIES",
	"A BRIDGE TOO FAR",
	"ETON RIFLES",
	"SEWERS CANAL",
	"NO WAY IN",
	"TAKE YOUR PARTNERS",
	"THE SCROUNGERS",
	"PLENTY OF ROOM",
	"FRANZ KLAMMER",
	"THE DOORS",
	"LOOK OUT",
	"TANK TOP",
	"TANKTASTIC",
	"THE GREAT ESCAPE",
	"WHOOPEEE",
	"TOO MUCH MAN",
	"ROCKET MAN",
	"RUNNING OUT OF IDEAS",
	"AND THEY'RE OFF",
	"GO FOR IT",
	"SHEEP SHEARER'S DELIGHT",
	"IRELAND RECORDS",
	"GAWD HELP ME",
	"CHOPPER CRAZY",
	"STOP THE PIGEON",
	"SAVE THAT SCUMBAG",
	"ONE MAN THREE CHOPPERS",
	"A WATERY GRAVE",
	"SYSTEM OFF",
	"STICK EM UP COD BREATH",
	"DONKEYDEATHTASTICELASTIC"
};

const char* mMissionGoals[] = {
	"KILL ALL ENEMY",
	"DESTROY ENEMY BUILDINGS",
	"RESCUE HOSTAGES",
	"PROTECT ALL CIVILIANS",
	"KIDNAP ENEMY LEADER",
	"DESTROY FACTORY",
	"DESTROY COMPUTER",
	"GET CIVILIAN HOME"
};

const int8 mMap1_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mMap2_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mMap3_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap4_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap5_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap6_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap7_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap8_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap9_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap10_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap11_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap12_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap13_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap14_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap15_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap16_Goal[] = {eGoal_Destroy_Enemy_Buildings,		eGoal_Protect_Civilians,											eGoal_End }; 
const int8 mMap17_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap18_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap19_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap20_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap21_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap22_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap23_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap24_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap25_Goal[] = {eGoal_Destroy_Enemy_Buildings,		eGoal_Protect_Civilians,											eGoal_End }; 
const int8 mMap26_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap27_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap28_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap29_Goal[] = {eGoal_Get_Civilian_Home,																				eGoal_End };
const int8 mMap30_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap31_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap32_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mMap33_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap34_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap35_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mMap36_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,	eGoal_Destroy_Factory,				eGoal_End };
const int8 mMap37_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap38_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap39_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap40_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mMap41_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,	eGoal_Protect_Civilians,			eGoal_End };
const int8 mMap42_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap43_Goal[] = {eGoal_Protect_Civilians,			eGoal_Kill_All_Enemy,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mMap44_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap45_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap46_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Rescue_Hostages,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mMap47_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mMap48_Goal[] = {eGoal_Protect_Civilians,			eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap49_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mMap50_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mMap51_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap52_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap53_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap54_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap55_Goal[] = {eGoal_Destroy_Enemy_Buildings,		eGoal_Get_Civilian_Home,											eGoal_End }; 
const int8 mMap56_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap57_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap58_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Rescue_Hostages,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mMap59_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap60_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Protect_Civilians,											eGoal_End }; 
const int8 mMap61_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap62_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap63_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap64_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mMap65_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap66_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mMap67_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mMap68_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mMap69_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mMap70_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Computer,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mMap71_Goal[] = {eGoal_Kidnap_Leader,																					eGoal_End };
const int8 mMap72_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };

const int8* mMap_Goals[] = {
	mMap1_Goal,
	mMap2_Goal,
	mMap3_Goal,
	mMap4_Goal,
	mMap5_Goal,
	mMap6_Goal,
	mMap7_Goal,
	mMap8_Goal,
	mMap9_Goal,
	mMap10_Goal,
	mMap11_Goal,
	mMap12_Goal,
	mMap13_Goal,
	mMap14_Goal,
	mMap15_Goal,
	mMap16_Goal,
	mMap17_Goal,
	mMap18_Goal,
	mMap19_Goal,
	mMap20_Goal,
	mMap21_Goal,
	mMap22_Goal,
	mMap23_Goal,
	mMap24_Goal,
	mMap25_Goal,
	mMap26_Goal,
	mMap27_Goal,
	mMap28_Goal,
	mMap29_Goal,
	mMap30_Goal,
	mMap31_Goal,
	mMap32_Goal,
	mMap33_Goal,
	mMap34_Goal,
	mMap35_Goal,
	mMap36_Goal,
	mMap37_Goal,
	mMap38_Goal,
	mMap39_Goal,
	mMap40_Goal,
	mMap41_Goal,
	mMap42_Goal,
	mMap43_Goal,
	mMap44_Goal,
	mMap45_Goal,
	mMap46_Goal,
	mMap47_Goal,
	mMap48_Goal,
	mMap49_Goal,
	mMap50_Goal,
	mMap51_Goal,
	mMap52_Goal,
	mMap53_Goal,
	mMap54_Goal,
	mMap55_Goal,
	mMap56_Goal,
	mMap57_Goal,
	mMap58_Goal,
	mMap59_Goal,
	mMap60_Goal,
	mMap61_Goal,
	mMap62_Goal,
	mMap63_Goal,
	mMap64_Goal,
	mMap65_Goal,
	mMap66_Goal,
	mMap67_Goal,
	mMap68_Goal,
	mMap69_Goal,
	mMap70_Goal,
	mMap71_Goal,
	mMap72_Goal
};

const sMissionData mMissionData_Plus = {
	mPlus_MissionNames,
	mPlus_MissionNames,
	mPlus_MissionPhaseTable
};

const sMissionData mMissionData_AmigaFormat = {
	mAmigaFormat_MissionNames,
	mAmigaFormat_MissionNames,
	mAmigaFormat_MissionPhaseTable
};

const sMissionData mMissionData_Retail = {
	mRetail_MissionNames,
	mRetail_MissionPhaseNames,
	mRetail_MissionPhaseTable
};
