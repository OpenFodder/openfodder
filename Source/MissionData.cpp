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

const uint16 mRetail_MissionPhaseTable[] = {	1, 2, 1, 4, 3, 2, 3, 4,
												2, 5, 3, 6, 1, 3, 3, 2,
												1, 5, 1, 4, 1, 4, 5, 6 };

const uint16 mRetail2_MissionPhaseTable[] = {
												1, 3, 4, 3, 2, 3, 4, 2, 
												5, 2, 1, 6, 2, 4, 2, 3, 
												1, 2, 4, 1, 4, 5, 2, 6 };

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
	"MORE MOORS",
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
const char* mRetail2_MissionNames[] = {
	"THIS IS HOW IT BEGINS",
	"MY LOVE HAS GOT A GUN",
	"I WANT TO KILL SOMEBODY",
	"I BELIEVE IN SPACE",
	"LADY LOVE YOUR COUNTRY",
	"THE SWORDS OF 1000 MEN",
	"TICKY TICKY TIMEBOMB",
	"1  2  X  U",
	"ALL THIS AND MORE",
	"OOH FAYE DUNAWAY",
	"SMILE WITH MR UKELELE",
	"IDONTMINDBEINGONMYOWN",
	"NEW GUYS IN TOWN",
	"FREAK YOU MELON FARMER",
	"BETWEEN PLANETS",
	"BEEN DRIVIN' FOR DAYS NOW",
	"KASIMIR S PULASKI DAY",
	"TRAVEL'S IN MY BLOOD",
	"FOREST BUTCHER BOY",
	"LOOK AT THAT UGLY MOON",
	"SHOCKER IN GLOOMTOWN",
	"A STAR FOR EVERYONE",
	"EVE OF DESTRUCTION",
	"WISH THEM ALL DEAD"
};

const char* mRetail2_MissionPhaseNames[] = {
	"HAPPY DAYS ARE HERE AGAIN",
	"BOOM BANG A BANG",
	"THIS AIN'T PIZMO BEACH",
	"TAKE ME I'M YOURS",
	"COMPOUND FRACTURE",
	"SAY IT WITH ROCKETS",
	"TIPPER TOPPER",
	"IS IT A BIRD",
	"I'M GOING OUT OF MY WAY",
	"HANG WITH ME JOE",
	"UH OH WE'RE IN TROUBLE",
	"A BIT OF BULLY",
	"WIZARD PRANKS",
	"AN ENGLISHMAN'S HOME",
	"ASSAULT AND BATTERING",
	"BACK TO THE FUTURE",
	"FEERSUM ENDJINN",
	"NEVER SAW IT COMING",
	"IN THE MIDDLE WITH YOU",
	"JUST THIS ONE TO GO",
	"TERMINAL BEACH",
	"DESERT EAGLES",
	"CHOPPA STOPPA",
	"A DIP IN THE POOL",
	"COLLATERAL DAMAGE",
	"YOU CAN RUN",
	"DON'T WANNA GET STONED",
	"WATUSI RODEO",
	"A KNIGHT ON THE TOWN",
	"VEGETABLE MEN",
	"LOST IN A FOREST",
	"SPEEDWAY STAR",
	"IDIOT COUNTRY",
	"WISH THE LADS WERE HERE",
	"SOMEONE GIVE HER A GUN",
	"ERUTUF EHT OT KCAB",
	"MEAN STREETS",
	"THE OLD MAIN DRAG",
	"A RAIN'S GONNA COME",
	"WILLIAM TELL ME",
	"FREEWAY ENTERPRISE",
	"AMBUSH CITY LIMITS",
	"QUARANTINE",
	"UNCHAINED MELODIES",
	"GRIDLOCK",
	"SIDEWALKING",
	"RIGHT ACROSS THE STREET",
	"I WISH I COULD FLY",
	"LOVER'S LEAP",
	"THE ROAD FROM HELL",
	"F KLAMMER STRIKES AGAIN",
	"ALL THE SAME TO ME",
	"TWO'S COMPANY",
	"CHARLIE WHISKEY",
	"THE WEB IN FRONT",
	"THERE'S A RIOT GOIN' ON",
	"SHOPPER'S PARADISE",
	"CHEDDINGTON",
	"KISS KISS MOLLY'S LIPS",
	"TAKE IT TO THE BRIDGE",
	"NO QUARTERS GIVEN",
	"FEEL THE PAIN",
	"GARY GILMORE'S EYES",
	"CLOSING IN ON DEATH",
	"SIX MILLION AND FOUR",
	"PLACES NOT TO GO TO",
	"FARAWAY BUT TOO CLOSE",
	"RETURN TO BLOODY DOVE",
	"HOTDOGS WITH EVERYTHING",
	"JESUS HAD A TWIN",
	"A SEA WITH THREE STARS",
	"CATCH 23"
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

const int8 mRetail_Map1_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mRetail_Map2_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mRetail_Map3_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map4_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map5_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map6_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map7_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map8_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map9_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map10_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map11_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map12_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map13_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map14_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map15_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map16_Goal[] = {eGoal_Destroy_Enemy_Buildings,		eGoal_Protect_Civilians,											eGoal_End }; 
const int8 mRetail_Map17_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map18_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map19_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map20_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map21_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map22_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map23_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map24_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map25_Goal[] = {eGoal_Destroy_Enemy_Buildings,		eGoal_Protect_Civilians,											eGoal_End }; 
const int8 mRetail_Map26_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map27_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map28_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map29_Goal[] = {eGoal_Get_Civilian_Home,																				eGoal_End };
const int8 mRetail_Map30_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map31_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map32_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mRetail_Map33_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map34_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map35_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mRetail_Map36_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,	eGoal_Destroy_Factory,				eGoal_End };
const int8 mRetail_Map37_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map38_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map39_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map40_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mRetail_Map41_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,	eGoal_Protect_Civilians,			eGoal_End };
const int8 mRetail_Map42_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map43_Goal[] = {eGoal_Protect_Civilians,			eGoal_Kill_All_Enemy,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mRetail_Map44_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map45_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map46_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Rescue_Hostages,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mRetail_Map47_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mRetail_Map48_Goal[] = {eGoal_Protect_Civilians,			eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map49_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mRetail_Map50_Goal[] = {eGoal_Rescue_Hostages,																					eGoal_End };
const int8 mRetail_Map51_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map52_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map53_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map54_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map55_Goal[] = {eGoal_Destroy_Enemy_Buildings,		eGoal_Get_Civilian_Home,											eGoal_End }; 
const int8 mRetail_Map56_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map57_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map58_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Rescue_Hostages,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mRetail_Map59_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map60_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Protect_Civilians,											eGoal_End }; 
const int8 mRetail_Map61_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map62_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map63_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map64_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mRetail_Map65_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map66_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mRetail_Map67_Goal[] = {eGoal_Kill_All_Enemy,																					eGoal_End };
const int8 mRetail_Map68_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };
const int8 mRetail_Map69_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Enemy_Buildings,										eGoal_End }; 
const int8 mRetail_Map70_Goal[] = {eGoal_Kill_All_Enemy,				eGoal_Destroy_Computer,			eGoal_Destroy_Enemy_Buildings,		eGoal_End };
const int8 mRetail_Map71_Goal[] = {eGoal_Kidnap_Leader,																					eGoal_End };
const int8 mRetail_Map72_Goal[] = {eGoal_Destroy_Enemy_Buildings,																			eGoal_End };

const int8* mRetail_Map_Goals[] = {
	mRetail_Map1_Goal,
	mRetail_Map2_Goal,
	mRetail_Map3_Goal,
	mRetail_Map4_Goal,
	mRetail_Map5_Goal,
	mRetail_Map6_Goal,
	mRetail_Map7_Goal,
	mRetail_Map8_Goal,
	mRetail_Map9_Goal,
	mRetail_Map10_Goal,
	mRetail_Map11_Goal,
	mRetail_Map12_Goal,
	mRetail_Map13_Goal,
	mRetail_Map14_Goal,
	mRetail_Map15_Goal,
	mRetail_Map16_Goal,
	mRetail_Map17_Goal,
	mRetail_Map18_Goal,
	mRetail_Map19_Goal,
	mRetail_Map20_Goal,
	mRetail_Map21_Goal,
	mRetail_Map22_Goal,
	mRetail_Map23_Goal,
	mRetail_Map24_Goal,
	mRetail_Map25_Goal,
	mRetail_Map26_Goal,
	mRetail_Map27_Goal,
	mRetail_Map28_Goal,
	mRetail_Map29_Goal,
	mRetail_Map30_Goal,
	mRetail_Map31_Goal,
	mRetail_Map32_Goal,
	mRetail_Map33_Goal,
	mRetail_Map34_Goal,
	mRetail_Map35_Goal,
	mRetail_Map36_Goal,
	mRetail_Map37_Goal,
	mRetail_Map38_Goal,
	mRetail_Map39_Goal,
	mRetail_Map40_Goal,
	mRetail_Map41_Goal,
	mRetail_Map42_Goal,
	mRetail_Map43_Goal,
	mRetail_Map44_Goal,
	mRetail_Map45_Goal,
	mRetail_Map46_Goal,
	mRetail_Map47_Goal,
	mRetail_Map48_Goal,
	mRetail_Map49_Goal,
	mRetail_Map50_Goal,
	mRetail_Map51_Goal,
	mRetail_Map52_Goal,
	mRetail_Map53_Goal,
	mRetail_Map54_Goal,
	mRetail_Map55_Goal,
	mRetail_Map56_Goal,
	mRetail_Map57_Goal,
	mRetail_Map58_Goal,
	mRetail_Map59_Goal,
	mRetail_Map60_Goal,
	mRetail_Map61_Goal,
	mRetail_Map62_Goal,
	mRetail_Map63_Goal,
	mRetail_Map64_Goal,
	mRetail_Map65_Goal,
	mRetail_Map66_Goal,
	mRetail_Map67_Goal,
	mRetail_Map68_Goal,
	mRetail_Map69_Goal,
	mRetail_Map70_Goal,
	mRetail_Map71_Goal,
	mRetail_Map72_Goal
};

const int16 mRetail_Map_Sprite_Enemy_Aggression[] = {
	0, 0, 1, 2, 0, 2, 1, 3, 2, 4, 
	2, 4, 2, 4, 4, 8, 2, 6, 4, 9, 
	6, 9, 7, 10, 8, 13, 6, 9, 6, 9, 
	6, 9, 3, 7, 1, 3, 7, 12, 1, 4, 
	2, 4, 0, 0, 6, 12, 3, 6, 4, 8, 
	5, 7, 5, 10, 7, 13, 1, 4, 11, 16, 
	4, 10, 5, 7, 8, 12, 2, 4, 6, 10, 
	0, 0, 0, 0, 6, 10, 6, 10, 6, 10, 
	6, 10, 6, 10, 6, 10, 6, 10, 6, 10, 
	7, 12, 6, 10, 6, 10, 6, 10, 6, 10, 
	2, 4, 2, 9, 3, 7, 6, 10, 6, 10, 
	6, 10, 3, 9, 6, 10, 6, 10, 3, 7, 
	3, 7, 15, 20, 3, 6, 6, 10, 6, 10, 
	6, 10, 6, 10, 10, 20, 6, 10, 6, 10, 
	6, 10, 6, 10
};

const sMissionData mMissionData_Plus = {
	mPlus_MissionNames,
	mPlus_MissionNames,
	mPlus_MissionPhaseTable,
	mRetail_Map_Goals,
	mRetail_Map_Sprite_Enemy_Aggression
};

const sMissionData mMissionData_AmigaFormat = {
	mAmigaFormat_MissionNames,
	mAmigaFormat_MissionNames,
	mAmigaFormat_MissionPhaseTable,
	mRetail_Map_Goals,
	mRetail_Map_Sprite_Enemy_Aggression
};

const sMissionData mMissionData_Retail = {
	mRetail_MissionNames,
	mRetail_MissionPhaseNames,
	mRetail_MissionPhaseTable,
	mRetail_Map_Goals,
	mRetail_Map_Sprite_Enemy_Aggression
};

const sMissionData mMissionData_Retail2 = {
	mRetail2_MissionNames,
	mRetail2_MissionPhaseNames,
	mRetail2_MissionPhaseTable,
	mRetail_Map_Goals,
	mRetail_Map_Sprite_Enemy_Aggression
};
