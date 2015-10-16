/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield
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

const uint16 mAmigaFormat_MissionPhaseTable[] = { 01, 01 };

const char* mAmigaFormat_MissionNames[] = {
	"LAND OF HOPE AND GLORY",
	"IT'S SNOW TIME!"
};

const uint16 mPlus_MissionPhaseTable[] = { 01, 01, 01, 01 };

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

const int8 byte_3D711[] = {1, -1 };      
const int8 byte_3D713[] = {1, -1 };      
const int8 byte_3D715[] = {1, 2, -1 };   
const int8 byte_3D718[] = {1, 2, -1 };   
const int8 byte_3D71B[] = {2, -1 };      
const int8 byte_3D71D[] = {1, 2, -1 };   
const int8 byte_3D720[] = {1, 2, -1 };   
const int8 byte_3D723[] = {1, 2, -1 };   
const int8 byte_3D726[] = {1, 2, -1 };   
const int8 byte_3D729[] = {1, 2, -1 };   
const int8 byte_3D72C[] = {2, -1 };      
const int8 byte_3D72E[] = {1, 2, -1 };   
const int8 byte_3D731[] = {2, -1 };      
const int8 byte_3D733[] = {1, 2, -1 };   
const int8 byte_3D736[] = {1, 2, -1 };   
const int8 byte_3D739[] = {2, 4, -1 };   
const int8 byte_3D73C[] = {1, 2, -1 };   
const int8 byte_3D73F[] = {1, 2, -1 };   
const int8 byte_3D742[] = {1, 2, -1 };   
const int8 byte_3D745[] = {2, -1 };      
const int8 byte_3D747[] = {1, 2, -1 };   
const int8 byte_3D74A[] = {1, 2, -1 };   
const int8 byte_3D74D[] = {1, 2, -1 };   
const int8 byte_3D750[] = {1, 2, -1 };   
const int8 byte_3D753[] = {2, 4, -1 };   
const int8 byte_3D756[] = {2, -1 };      
const int8 byte_3D758[] = {2, -1 };      
const int8 byte_3D75A[] = {2, -1 };      
const int8 byte_3D75C[] = {8, -1 };      
const int8 byte_3D75E[] = {1, 2, -1 };   
const int8 byte_3D761[] = {2, -1 };      
const int8 byte_3D763[] = {1, -1 };      
const int8 byte_3D765[] = {1, 2, -1 };   
const int8 byte_3D768[] = {1, 2, -1 };   
const int8 byte_3D76B[] = {3, -1 };      
const int8 byte_3D76D[] = {1, 2, 6, -1 };
const int8 byte_3D771[] = {1, 2, -1 };   
const int8 byte_3D774[] = {2, -1 };      
const int8 byte_3D776[] = {1, 2, -1 };   
const int8 byte_3D779[] = {3, -1 };      
const int8 byte_3D77B[] = {1, 2, 4, -1 };
const int8 byte_3D77F[] = {2, -1 };      
const int8 byte_3D781[] = {4, 1, 2, -1 };
const int8 byte_3D785[] = {1, 2, -1 };   
const int8 byte_3D788[] = {2, -1 };      
const int8 byte_3D78A[] = {1, 3, 2, -1 };
const int8 byte_3D78E[] = {3, -1 };      
const int8 byte_3D790[] = {4, 2, -1 };   
const int8 byte_3D793[] = {3, -1 };      
const int8 byte_3D795[] = {3, -1 };      
const int8 byte_3D797[] = {1, 2, -1 };   
const int8 byte_3D79A[] = {1, 2, -1 };   
const int8 byte_3D79D[] = {2, -1 };      
const int8 byte_3D79F[] = {2, -1 };      
const int8 byte_3D7A1[] = {2, 8, -1 };   
const int8 byte_3D7A4[] = {1, 2, -1 };   
const int8 byte_3D7A7[] = {1, 2, -1 };   
const int8 byte_3D7AA[] = {1, 3, 2, -1 };
const int8 byte_3D7AE[] = {1, 2, -1 };   
const int8 byte_3D7B1[] = {1, 4, -1 };   
const int8 byte_3D7B4[] = {1, 2, -1 };   
const int8 byte_3D7B7[] = {1, 2, -1 };   
const int8 byte_3D7BA[] = {1, 2, -1 };   
const int8 byte_3D7BD[] = {1, -1 };      
const int8 byte_3D7BF[] = {2, -1 };      
const int8 byte_3D7C1[] = {1, -1 };      
const int8 byte_3D7C3[] = {1, -1 };      
const int8 byte_3D7C5[] = {2, -1 };      
const int8 byte_3D7C7[] = {1, 2, -1 };   
const int8 byte_3D7CA[] = {1, 7, 2, -1 };
const int8 byte_3D7CE[] = {5, -1 };      
const int8 byte_3D7D0[] = {2, -1 };      

const int8* mMission_Goals[] = {
	byte_3D711,
	byte_3D713,
	byte_3D715,
	byte_3D718,
	byte_3D71B,
	byte_3D71D,
	byte_3D720,
	byte_3D723,
	byte_3D726,
	byte_3D729,
	byte_3D72C,
	byte_3D72E,
	byte_3D731,
	byte_3D733,
	byte_3D736,
	byte_3D739,
	byte_3D73C,
	byte_3D73F,
	byte_3D742,
	byte_3D745,
	byte_3D747,
	byte_3D74A,
	byte_3D74D,
	byte_3D750,
	byte_3D753,
	byte_3D756,
	byte_3D758,
	byte_3D75A,
	byte_3D75C,
	byte_3D75E,
	byte_3D761,
	byte_3D763,
	byte_3D765,
	byte_3D768,
	byte_3D76B,
	byte_3D76D,
	byte_3D771,
	byte_3D774,
	byte_3D776,
	byte_3D779,
	byte_3D77B,
	byte_3D77F,
	byte_3D781,
	byte_3D785,
	byte_3D788,
	byte_3D78A,
	byte_3D78E,
	byte_3D790,
	byte_3D793,
	byte_3D795,
	byte_3D797,
	byte_3D79A,
	byte_3D79D,
	byte_3D79F,
	byte_3D7A1,
	byte_3D7A4,
	byte_3D7A7,
	byte_3D7AA,
	byte_3D7AE,
	byte_3D7B1,
	byte_3D7B4,
	byte_3D7B7,
	byte_3D7BA,
	byte_3D7BD,
	byte_3D7BF,
	byte_3D7C1,
	byte_3D7C3,
	byte_3D7C5,
	byte_3D7C7,
	byte_3D7CA,
	byte_3D7CE,
	byte_3D7D0
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
