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

#include "stdafx.hpp"

const struct sRecruit mRecruits[] = {
  //{ "SEGRA ", 2, -1, 4 },
  { "JOOLS ", 2, -1, 4 },
  { "JOPS  ", 2, -1, 6 },
  { "STOO  ", 2, -1, 0 },
  { "RJ    ", 2, -1, 0 },
  { "UBIK  ", 2, -1, 1 },
  { "CJ    ", 2, -1, 0 },
  { "CHRIS ", 2, -1, 0 },
  { "PETE  ", 2, -1, 0 },
  { "TADGER", 3, -1, 3 },
  { "HECTOR", 2, -1, 0 },
  { "ELROY ", 2, -1, 0 },
  { "SOFTY ", 2, -1, 0 },
  { "MAC   ", 2, -1, 0 },
  { "BOMBER", 2, -1, 0 },
  { "STAN  ", 2, -1, 0 },
  { "TOSH  ", 2, -1, 0 },
  { "BRAINS", 2, -1, 1 },
  { "NORM  ", 2, -1, 0 },
  { "BUSTER", 2, -1, 0 },
  { "SPIKE ", 2, -1, 0 },
  { "BROWNY", 2, -1, 0 },
  { "MURPHY", 2, -1, 10 },
  { "KILLER", 2, -1, 0 },
  { "ABDUL ", 2, -1, 0 },
  { "SPOTTY", 2, -1, 0 },
  { "GOOFY ", 2, -1, 0 },
  { "DONALD", 2, -1, 5 },
  { "WINDY ", 2, -1, 0 },
  { "NIFTA ", 2, -1, 0 },
  { "DENZIL", 2, -1, 0 },
  { "CEDRIC", 2, -1, 0 },
  { "ALF   ", 2, -1, 0 },
  { "MARTY ", 2, -1, 0 },
  { "CECIL ", 2, -1, 0 },
  { "WALLY ", 3, -1, 0 },
  { "DONG  ", 2, -1, 0 },
  { "JASON ", 2, -1, 0 },
  { "ROY   ", 2, -1, 0 },
  { "PEEWEE", 2, -1, 0 },
  { "ARNIE ", 2, -1, 3 },
  { "LOFTY ", 2, -1, 0 },
  { "TUBBY ", 2, -1, 0 },
  { "PORKY ", 2, -1, 0 },
  { "NORRIS", 2, -1, 0 },
  { "BUGSY ", 2, -1, 0 },
  { "GREG  ", 2, -1, 0 },
  { "GUS   ", 2, -1, 0 },
  { "GINGER", 2, -1, 0 },
  { "EDDY  ", 2, -1, 0 },
  { "STEVE ", 2, -1, 0 },
  { "HUGO  ", 2, -1, 0 },
  { "ZIPPY ", 2, -1, 0 },
  { "SONNY ", 2, -1, 0 },
  { "WILLY ", 2, -1, 0 },
  { "MARIO ", 2, -1, 0 },
  { "LUIGI ", 2, -1, 0 },
  { "BO    ", 2, -1, 0 },
  { "JOHAN ", 2, -1, 0 },
  { "COLIN ", 2, -1, 0 },
  { "RUBY  ", 2, -1, 0 },
  { "MORGAN", 2, -1, 0 },
  { "REG   ", 2, -1, 0 },
  { "PETER ", 2, -1, 0 },
  { "BRETT ", 2, -1, 0 },
  { "MATT  ", 2, -1, 0 },
  { "VIC   ", 2, -1, 0 },
  { "HUT   ", 2, -1, 0 },
  { "BUD   ", 2, -1, 0 },
  { "BRAD  ", 2, -1, 0 },
  { "ASHLEY", 2, -1, 0 },
  { "LES   ", 2, -1, 0 },
  { "REX   ", 2, -1, 0 },
  { "LOUIS ", 2, -1, 0 },
  { "PEDRO ", 2, -1, 0 },
  { "MARCO ", 2, -1, 0 },
  { "LEON  ", 2, -1, 0 },
  { "ALI   ", 2, -1, 0 },
  { "TYSON ", 2, -1, 0 },
  { "TIGER ", 2, -1, 0 },
  { "FRANK ", 2, -1, 0 },
  { "REUBEN", 2, -1, 0 },
  { "LEYTON", 2, -1, 0 },
  { "JOSH  ", 2, -1, 0 },
  { "JUDAS ", 2, -1, 0 },
  { "AJ    ", 2, -1, 0 },
  { "LEX   ", 2, -1, 0 },
  { "BUTCH ", 2, -1, 0 },
  { "BISON ", 2, -1, 0 },
  { "GARY  ", 2, -1, 0 },
  { "LUTHER", 2, -1, 0 },
  { "KERMIT", 2, -1, 0 },
  { "BRIAN ", 2, -1, 0 },
  { "RAY   ", 2, -1, 0 },
  { "FREAK ", 2, -1, 0 },
  { "LEROY ", 2, -1, 0 },
  { "LEE   ", 2, -1, 0 },
  { "BANJO ", 2, -1, 0 },
  { "BEAKER", 2, -1, 0 },
  { "BASIL ", 2, -1, 0 },
  { "BONZO ", 2, -1, 0 },
  { "KELVIN", 2, -1, 0 },
  { "RONNIE", 2, -1, 0 },
  { "RUPERT", 2, -1, 0 },
  { "ROO   ", 2, -1, 0 },
  { "DAN   ", 2, -1, 0 },
  { "BASHER", 2, -1, 0 },
  { "RONNIE", 2, -1, 0 },
  { "LUCKY ", 2, -1, 0 },
  { "DON   ", 2, -1, 0 },
  { "TOMMY ", 2, -1, 0 },
  { "EDDIE ", 2, -1, 0 },
  { "CYNIC ", 2, -1, 0 },
  { "BLOND ", 2, -1, 0 },
  { "PADDY ", 2, -1, 0 },
  { "ARNOLD", 2, -1, 0 },
  { "TONY  ", 2, -1, 0 },
  { "TEDDY ", 2, -1, 0 },
  { "DOM   ", 2, -1, 0 },
  { "THEO  ", 2, -1, 0 },
  { "MARTIN", 2, -1, 0 },
  { "CHUNKY", 2, -1, 0 },
  { "JON   ", 2, -1, 0 },
  { "BEN   ", 2, -1, 0 },
  { "GIRLY ", 2, -1, 0 },
  { "JULIAN", 2, -1, 0 },
  { "GARY  ", 2, -1, 0 },
  { "PIZZA ", 2, -1, 0 },
  { "MARK  ", 2, -1, 0 },
  { "CIARAN", 2, -1, 0 },
  { "JOCK  ", 2, -1, 0 },
  { "GRAVY ", 2, -1, 0 },
  { "TRENDY", 2, -1, 0 },
  { "NEIL  ", 2, -1, 0 },
  { "DEREK ", 2, -1, 0 },
  { "ED    ", 2, -1, 0 },
  { "STEVE ", 2, -1, 0 },
  { "BIFF  ", 2, -1, 0 },
  { "STEVE ", 2, -1, 0 },
  { "PAUL  ", 2, -1, 0 },
  { "STUART", 2, -1, 0 },
  { "RANDY ", 2, -1, 0 },
  { "LORETA", 2, -1, 0 },
  { "SUZIE ", 2, -1, 0 },
  { "PUMPY ", 2, -1, 0 },
  { "URMER ", 2, -1, 0 },
  { "BUNNY ", 2, -1, 0 },
  { "ADEY  ", 2, -1, 0 },
  { "PEPPER", 2, -1, 0 },
  { "BEEFY ", 2, -1, 0 },
  { "HARRY ", 2, -1, 0 },
  { "TINY  ", 2, -1, 0 },
  { "HOWARD", 2, -1, 0 },
  { "MORRIS", 2, -1, 0 },
  { "THOR  ", 2, -1, 0 },
  { "REV   ", 2, -1, 0 },
  { "DUKE  ", 2, -1, 0 },
  { "SHORTY", 2, -1, 0 },
  { "MICKY ", 2, -1, 0 },
  { "CHAS  ", 2, -1, 0 },
  { "MELONY", 2, -1, 0 },
  { "CRAIG ", 2, -1, 0 },
  { "SIDNEY", 2, -1, 0 },
  { "PARSON", 2, -1, 0 },
  { "ROWAN ", 2, -1, 0 },
  { "SMELLY", 2, -1, 0 },
  { "DOK   ", 2, -1, 0 },
  { "STEW  ", 2, -1, 0 },
  { "DONALD", 2, -1, 0 },
  { "ADRIAN", 2, -1, 0 },
  { "PAT   ", 2, -1, 0 },
  { "ICEMAN", 2, -1, 0 },
  { "GOOSE ", 2, -1, 0 },
  { "DIPPY ", 2, -1, 0 },
  { "VIV   ", 2, -1, 0 },
  { "SI    ", 2, -1, 0 },
  { "BUNTY ", 2, -1, 0 },
  { "NOEL  ", 2, -1, 0 },
  { "BONO  ", 2, -1, 0 },
  { "EDGE  ", 2, -1, 0 },
  { "ROBBIE", 2, -1, 0 },
  { "SEAN  ", 2, -1, 0 },
  { "MILES ", 2, -1, 0 },
  { "JIMI  ", 2, -1, 0 },
  { "GORDON", 2, -1, 0 },
  { "VAL   ", 2, -1, 0 },
  { "HOBO  ", 2, -1, 0 },
  { "FUNGUS", 2, -1, 0 },
  { "JR    ", 2, -1, 0 },
  { "LAMPY ", 2, -1, 0 },
  { "MARCUS", 2, -1, 0 },
  { "PELE  ", 2, -1, 0 },
  { "HUBERT", 2, -1, 0 },
  { "JAMES ", 2, -1, 0 },
  { "TIM   ", 2, -1, 0 },
  { "SAUL  ", 2, -1, 0 },
  { "ANDY  ", 2, -1, 0 },
  { "ALF   ", 2, -1, 0 },
  { "SILKY ", 2, -1, 0 },
  { "SIMON ", 2, -1, 0 },
  { "HANDY ", 2, -1, 0 },
  { "SID   ", 2, -1, 0 },
  { "GEORGE", 2, -1, 0 },
  { "JOFF  ", 2, -1, 0 },
  { "BARRY ", 2, -1, 0 },
  { "DICK  ", 2, -1, 0 },
  { "GIL   ", 2, -1, 0 },
  { "NICK  ", 2, -1, 0 },
  { "TED   ", 2, -1, 0 },
  { "PHIL  ", 2, -1, 0 },
  { "WOODY ", 2, -1, 0 },
  { "WYNN  ", 2, -1, 0 },
  { "ALAN  ", 2, -1, 0 },
  { "PIP   ", 2, -1, 0 },
  { "MICKEY", 2, -1, 0 },
  { "JUSTIN", 2, -1, 0 },
  { "KARL  ", 2, -1, 0 },
  { "MADDOG", 2, -1, 0 },
  { "HORACE", 2, -1, 0 },
  { "HAROLD", 2, -1, 0 },
  { "GAZZA ", 2, -1, 0 },
  { "SPIV  ", 2, -1, 0 },
  { "FOXY  ", 2, -1, 0 },
  { "NED   ", 2, -1, 0 },
  { "BAZIL ", 2, -1, 0 },
  { "OLIVER", 2, -1, 0 },
  { "RETT  ", 2, -1, 0 },
  { "SCOT  ", 2, -1, 0 },
  { "DARREN", 2, -1, 0 },
  { "EDWIN ", 2, -1, 0 },
  { "MOSES ", 2, -1, 0 },
  { "NOAH  ", 2, -1, 0 },
  { "SETH  ", 2, -1, 0 },
  { "HARVEY", 2, -1, 0 },
  { "MARY  ", 2, -1, 0 },
  { "PILOT ", 2, -1, 0 },
  { "MCBETH", 2, -1, 0 },
  { "MCDUFF", 2, -1, 0 },
  { "BELLY ", 2, -1, 0 },
  { "MATHEW", 2, -1, 0 },
  { "MARK  ", 2, -1, 0 },
  { "LUKE  ", 2, -1, 0 },
  { "JOHN  ", 2, -1, 0 },
  { "ASLAM ", 2, -1, 0 },
  { "HAM   ", 2, -1, 0 },
  { "SHEM  ", 2, -1, 0 },
  { "JOSHUA", 2, -1, 0 },
  { "JACOB ", 2, -1, 0 },
  { "ESAW  ", 2, -1, 0 },
  { "OMAR  ", 2, -1, 0 },
  { "SAUL  ", 2, -1, 0 },
  { "ENOCH ", 2, -1, 0 },
  { "OBADIA", 2, -1, 0 },
  { "DANIEL", 2, -1, 0 },
  { "SAMUEL", 2, -1, 0 },
  { "BEN   ", 2, -1, 0 },
  { "ROBBO ", 2, -1, 0 },
  { "JOEBED", 2, -1, 0 },
  { "ISMAEL", 2, -1, 0 },
  { "ISREAL", 2, -1, 0 },
  { "ISABEL", 2, -1, 0 },
  { "ISARAT", 2, -1, 0 },
  { "MONK  ", 2, -1, 0 },
  { "BLIP  ", 2, -1, 0 },
  { "BACON ", 2, -1, 0 },
  { "DANUBE", 2, -1, 0 },
  { "FRIEND", 2, -1, 0 },
  { "DARRYL", 2, -1, 0 },
  { "IZZY  ", 2, -1, 0 },
  { "CROSBY", 2, -1, 0 },
  { "STILLS", 2, -1, 0 },
  { "NASH  ", 2, -1, 0 },
  { "YOUNG ", 2, -1, 0 },
  { "CHEESE", 2, -1, 0 },
  { "SALAMI", 2, -1, 0 },
  { "PRAWN ", 2, -1, 0 },
  { "RADISH", 2, -1, 0 },
  { "EGBERT", 2, -1, 0 },
  { "EDWY  ", 2, -1, 0 },
  { "EDGAR ", 2, -1, 0 },
  { "EDWIN ", 2, -1, 0 },
  { "EDRED ", 2, -1, 0 },
  { "EGGPIE", 2, -1, 0 },
  { "BROS  ", 2, -1, 0 },
  { "CAKEY ", 2, -1, 0 },
  { "ZIGGY ", 2, -1, 0 },
  { "ALFRED", 2, -1, 0 },
  { "SIGGY ", 2, -1, 0 },
  { "HILDA ", 2, -1, 0 },
  { "SNELL ", 2, -1, 0 },
  { "SPARKS", 2, -1, 0 },
  { "ROB   ", 2, -1, 0 },
  { "TOPCAT", 2, -1, 0 },
  { "BENNY ", 2, -1, 0 },
  { "DIBBLE", 2, -1, 0 },
  { "BENKER", 2, -1, 0 },
  { "DOSEY ", 2, -1, 0 },
  { "BEAKY ", 2, -1, 0 },
  { "JOIST ", 2, -1, 0 },
  { "PIVOT ", 2, -1, 0 },
  { "TREE  ", 2, -1, 0 },
  { "BUSH  ", 2, -1, 0 },
  { "GRASS ", 2, -1, 0 },
  { "SEEDY ", 2, -1, 0 },
  { "TIN   ", 2, -1, 0 },
  { "ROLLO ", 2, -1, 0 },
  { "ZIPPO ", 2, -1, 0 },
  { "NANCY ", 2, -1, 0 },
  { "LARRY ", 2, -1, 0 },
  { "IGGY  ", 2, -1, 0 },
  { "NIGEL ", 2, -1, 0 },
  { "JAMIE ", 2, -1, 0 },
  { "JESSE ", 2, -1, 0 },
  { "LEO   ", 2, -1, 0 },
  { "VIRGO ", 2, -1, 0 },
  { "GARTH ", 2, -1, 0 },
  { "FIDEL ", 2, -1, 0 },
  { "IDI   ", 2, -1, 0 },
  { "CHE   ", 2, -1, 0 },
  { "JOHNNY", 2, -1, 0 },
  { "AMBER ", 2, -1, 0 },
  { "LAIRY ", 2, -1, 0 },
  { "LARDO ", 2, -1, 0 },
  { "TONY  ", 2, -1, 0 },
  { "WOODY ", 2, -1, 0 },
  { "CLIFF ", 2, -1, 0 },
  { "FESTER", 2, -1, 0 },
  { "JETHRO", 2, -1, 0 },
  { "JIMBOB", 2, -1, 0 },
  { "DECLAN", 2, -1, 0 },
  { "NEIL  ", 2, -1, 0 },
  { "HICKEY", 2, -1, 0 },
  { "CHOCCO", 2, -1, 0 },
  { "GOCH  ", 2, -1, 0 },
  { "PABLO ", 2, -1, 0 },
  { "RENOIR", 2, -1, 0 },
  { "ROLF  ", 2, -1, 0 },
  { "DALI  ", 2, -1, 0 },
  { "MONET ", 2, -1, 0 },
  { "MANET ", 2, -1, 0 },
  { "GAUGIN", 2, -1, 0 },
  { "CHAGAL", 2, -1, 0 },
  { "KID   ", 2, -1, 0 },
  { "HULLY ", 2, -1, 0 },
  { "ROBERT", 2, -1, 0 },
  { "PIERS ", 2, -1, 0 },
  { "RAITH ", 2, -1, 0 },
  { "JEEVES", 2, -1, 0 },
  { "PASTER", 2, -1, 0 },
  { "GINAN ", 2, -1, 0 },
  { "DEITER", 2, -1, 0 },
  { "DENI  ", 2, -1, 0 },
  { "ZARK  ", 2, -1, 0 },
  { "WIZKID", 2, -1, 0 },
  { "WIZARD", 2, -1, 0 },
  { "IAIN  ", 2, -1, 0 },
  { "KITTEN", 2, -1, 0 },
  { "GONNER", 2, -1, 0 },
  { "WASTER", 2, -1, 0 },
  { "LOSER ", 2, -1, 0 },
  { "FODDER", 2, -1, 0 }
};

int16 word_35DFE[] = {
	0x21, 0x32, 0x42, 0x55, 0x64, -1,
};

int16 word_35E0A[] = {
	0x21, 0x32, 0x42, 0x55, 0x64, -1
};

int16 word_35E16[] = {
	0x21, 0x30, 0x42, 0x55, 0x64, -1,
};

int16 word_35E22[] = {
	0x21, 0x28, 0x42, 0x55, 0x64, -1,
};

int16 word_35E2E[] = {
	0x21, 0x3C, 0x42, 0x55, 0x64, -1
};

// Hill Recruit Dest: Part 1 / 2 / 3
int16 word_35E3A[] = {
	0x32, 0x52, 0x64, -1
};

const sRecruit_Sprites mRecruitSprite[34] =
{
	{ 0, 0, 48, 40, word_35DFE },
	{ 0, 1, 48, 56, word_35E0A },
	{ 0, 2, 48, 72, word_35E16 },
	{ 0, 3, 48, 88, word_35E22 },
	{ 0, 4, 48, 104, word_35E2E },

	{ 0, 5, 48, 120, word_35E0A },
	{ 0, 6, 48, 136, word_35E0A },
	{ 0, 7, 48, 152, word_35E0A },
	{ 0, 8, 48, 168, word_35E0A },

	{ 1, 0, 144, 40, word_35E3A },
	{ 1, 1, 192, 40, word_35E3A },
	{ 2, 0, 240, 40, word_35E3A },
	{ 2, 1, 144, 56, word_35E3A },
	{ 3, 0, 192, 56, word_35E3A },
	{ 3, 1, 240, 56, word_35E3A },
	{ 4, 0, 144, 72, word_35E3A },
	{ 4, 1, 192, 72, word_35E3A },
	{ 5, 0, 240, 72, word_35E3A },
	{ 5, 1, 144, 88, word_35E3A },
	{ 6, 0, 192, 88, word_35E3A },
	{ 6, 1, 240, 88, word_35E3A },
	{ 7, 0, 144, 104, word_35E3A },
	{ 7, 1, 192, 104, word_35E3A },
	{ 8, 0, 240, 104, word_35E3A },
	{ 8, 1, 144, 120, word_35E3A },
	{ 9, 0, 192, 120, word_35E3A },
	{ 9, 1, 240, 120, word_35E3A },
	{ 10, 0, 144, 136, word_35E3A },
	{ 10, 1, 192, 136, word_35E3A },
	{ 11, 0, 240, 136, word_35E3A },
	{ 11, 1, 144, 152, word_35E3A },
	{ 12, 0, 192, 152, word_35E3A },
	{ 12, 1, 240, 152, word_35E3A },
	{ -1 }
};

sRecruit_Screen_Pos mRecruit_Screen_Positions[294] = {
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 60, 0 },
	{ 305, 58, 0 },
	{ 305, 56, 0 },
	{ 305, 55, 0 },
	{ 304, 54, 0 },
	{ 303, 59, 1 },
	{ 305, 53, 0 },
	{ 304, 53, 0 },
	{ 303, 54, 0 },
	{ 302, 55, 0 },
	{ 301, 56, 0 },
	{ 301, 57, 0 },
	{ 300, 58, 0 },
	{ 300, 59, 0 },
	{ 299, 60, 0 },
	{ 299, 61, 0 },
	{ 298, 62, 0 },
	{ 298, 63, 0 },
	{ 297, 64, 0 },
	{ 297, 65, 0 },
	{ 296, 66, 0 },
	{ 296, 67, 0 },
	{ 296, 68, 0 },
	{ 295, 69, 0 },
	{ 295, 70, 0 },
	{ 295, 71, 0 },
	{ 294, 72, 0 },
	{ 294, 73, 0 },
	{ 294, 74, 0 },
	{ 294, 75, 0 },
	{ 293, 76, 0 },
	{ 293, 79, 0 },
	{ 292, 82, 0 },
	{ 292, 84, 0 },
	{ 291, 84, 0 },
	{ 292, 84, 0 },
	{ 292, 84, 0 },
	{ 292, 84, 0 },
	{ 292, 84, 0 },
	{ 292, 84, 0 },
	{ 292, 84, 0 },
	{ 292, 84, 0 },
	{ 292, 84, 0 },
	{ 288, 84, 0 },
	{ 287, 82, 0 },
	{ 286, 81, 0 },
	{ 285, 79, 0 },
	{ 285, 77, 0 },
	{ 276, 84, 2 },
	{ 284, 77, 0 },
	{ 283, 78, 0 },
	{ 283, 79, 0 },
	{ 282, 81, 0 },
	{ 281, 82, 0 },
	{ 281, 84, 0 },
	{ 281, 86, 0 },
	{ 281, 87, 0 },
	{ 281, 89, 0 },
	{ 282, 90, 0 },
	{ 282, 92, 0 },
	{ 282, 94, 0 },
	{ 283, 95, 0 },
	{ 283, 97, 0 },
	{ 284, 99, 0 },
	{ 284, 101, 0 },
	{ 285, 103, 0 },
	{ 286, 106, 0 },
	{ 287, 109, 0 },
	{ 287, 112, 0 },
	{ 289, 116, 0 },
	{ 290, 118, 0 },
	{ 292, 121, 0 },
	{ 293, 122, 0 },
	{ 293, 124, 0 },
	{ 294, 126, 0 },
	{ 295, 128, 0 },
	{ 295, 129, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 295, 133, 0 },
	{ 308, 134, 0 },
	{ 308, 134, 0 },
	{ 308, 132, 0 },
	{ 307, 130, 0 },
	{ 307, 128, 0 },
	{ 307, 126, 0 },
	{ 307, 125, 0 },
	{ 306, 124, 0 },
	{ 288, 121, 3 },
	{ 306, 124, 0 },
	{ 306, 125, 0 },
	{ 305, 126, 0 },
	{ 305, 127, 0 },
	{ 305, 129, 0 },
	{ 304, 130, 0 },
	{ 304, 133, 0 },
	{ 304, 135, 0 },
	{ 304, 137, 0 },
	{ 303, 138, 0 },
	{ 303, 139, 0 },
	{ 304, 141, 0 },
	{ 304, 143, 0 },
	{ 303, 145, 0 },
	{ 303, 147, 0 },
	{ 303, 149, 0 },
	{ 303, 150, 0 },
	{ 303, 152, 0 },
	{ 302, 154, 0 },
	{ 301, 156, 0 },
	{ 301, 158, 0 },
	{ 300, 161, 0 },
	{ 299, 162, 0 },
	{ 298, 163, 0 },
	{ 297, 165, 0 },
	{ 295, 167, 0 },
	{ 294, 168, 0 },
	{ 293, 169, 0 },
	{ 292, 170, 0 },
	{ 291, 172, 0 },
	{ 290, 173, 0 },
	{ 289, 174, 0 },
	{ 288, 175, 0 },
	{ 287, 176, 0 },
	{ 285, 178, 0 },
	{ 283, 179, 0 },
	{ 281, 180, 0 },
	{ 280, 182, 0 },
	{ 278, 183, 0 },
	{ 277, 185, 0 },
	{ 275, 186, 0 },
	{ 273, 186, 0 },
	{ 272, 187, 0 },
	{ 271, 188, 0 },
	{ 269, 189, 0 },
	{ 267, 189, 0 },
	{ 265, 189, 0 },
	{ 263, 190, 0 },
	{ 262, 190, 0 },
	{ 260, 190, 0 },
	{ 258, 190, 0 },
	{ 256, 190, 0 },
	{ 254, 190, 0 },
	{ 252, 190, 0 },
	{ 250, 190, 0 },
	{ 249, 190, 0 },
	{ 247, 190, 0 },
	{ 246, 191, 0 },
	{ 244, 191, 0 },
	{ 242, 191, 0 },
	{ 240, 191, 0 },
	{ 238, 191, 0 },
	{ 236, 191, 0 },
	{ 234, 191, 0 },
	{ 232, 191, 0 },
	{ 230, 191, 0 },
	{ 227, 191, 0 },
	{ 225, 191, 0 },
	{ 223, 191, 0 },
	{ 221, 191, 0 },
	{ 219, 191, 0 },
	{ 217, 191, 0 },
	{ 215, 191, 0 },
	{ 213, 191, 0 },
	{ 211, 191, 0 },
	{ 209, 191, 0 },
	{ 207, 191, 0 },
	{ 205, 191, 0 },
	{ 203, 191, 0 },
	{ 201, 191, 0 },
	{ 199, 191, 0 },
	{ 197, 191, 0 },
	{ 195, 191, 0 },
	{ 193, 191, 0 },
	{ 191, 191, 0 },
	{ 189, 191, 0 },
	{ 187, 191, 0 },
	{ 185, 191, 0 },
	{ 183, 191, 0 },
	{ 181, 191, 0 },
	{ 179, 191, 0 },
	{ 177, 192, 0 },
	{ 175, 192, 0 },
	{ 173, 192, 0 },
	{ 171, 192, 0 },
	{ 169, 192, 0 },
	{ 167, 192, 0 },
	{ 165, 192, 0 },
	{ 163, 193, 0 },
	{ 161, 192, 0 },
	{ 159, 192, 0 },
	{ 157, 192, 0 },
	{ 155, 192, 0 },
	{ 153, 192, 0 },
	{ 151, 192, 0 },
	{ 149, 192, 0 },
	{ 147, 192, 0 },
	{ 145, 192, 0 },
	{ 143, 192, 0 },
	{ 141, 192, 0 },
	{ 139, 192, 0 },
	{ 137, 192, 0 },
	{ 135, 192, 0 },
	{ 133, 192, 0 },
	{ 131, 192, 0 },
	{ 131, 192, 0 },
	{ 129, 192, 0 },
	{ 127, 192, 0 },
	{ 125, 192, 0 },
	{ 123, 192, 0 },
	{ 121, 192, 0 },
	{ 119, 192, 0 },
	{ 117, 192, 0 },
	{ 115, 192, 0 },
	{ 113, 192, 0 },
	{ 111, 192, 0 },
	{ 109, 192, 0 },
	{ 107, 192, 0 },
	{ 105, 192, 0 },
	{ 103, 193, 0 },
	{ 101, 192, 0 },
	{ 99, 192, 0 },
	{ 96, 192, 0 },
	{ 94, 192, 0 },
	{ 92, 192, 0 },
	{ 90, 192, 0 },
	{ 88, 192, 0 },
	{ 86, 192, 0 },
	{ 84, 192, 0 },
	{ 82, 192, 0 },
	{ 80, 192, 0 },
	{ 78, 192, 0 },
	{ 76, 192, 0 },
	{ 74, 192, 0 },
	{ 72, 192, 0 },
	{ 70, 192, 0 },
	{ 68, 192, 0 },
	{ 66, 192, 0 },
	{ 64, 192, 0 },
	{ 62, 192, 0 },
	{ 60, 192, 0 },
	{ 58, 192, 0 },
	{ 56, 192, 0 },
	{ 54, 192, 0 },
	{ 52, 192, 0 },
	{ 50, 192, 0 },
	{ 48, 192, 0 },
	{ 46, 192, 0 },
	{ 44, 192, 0 },
	{ 42, 192, 0 },
	{ 40, 192, 0 },
	{ 38, 192, 0 },
	{ 36, 191, 0 },
	{ -1, -1, 0 }
};

const int16 mRecruit_Truck_Anim_Welcome[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04,
	0x05, 0x06, 0x09, 0x0A, 0x0B,
	0x09,
	// mRecruit_Truck_Anim_SwingArm
	0x09, 0x0A, 0x0B, 0x09, -1,
};

const int16 mRecruit_Truck_Anim_SwingArm[] = {
	0x09, 0x0A, 0x0B, 0x09, -1 };

const int16 mRecruit_Truck_Anim_PassTroop[] = {
	0x07, 0x07, 0x09, 0x09, 0x08, 0x08, 0x07, 0x07,
	0x07, 0x07, 0x09, 0x09, 0x08, 0x08, 0x07, 0x07, 
	0x09, 0x09, 0x09, -1
};

const int16 mRecruit_Truck_Anim_CloseDoor[] = {
	0x06, 0x05, 0x05, 0x04, 0x04, 0x03, 0x03, 0x02, 
	0x02, 0x01, 0x01, 0x00, -1
};

// Gap between recruits marching on the hill
const int16 mRecruit_Hill_Position_Gaps[] = {
	4, 6, 4, 5, 
	4, 7, 4, 5, 
	4, 4, 7, 5, 
	6, 5, 4, 5
};

const int16 mRecruit_Shirt_Colors[16] =
{
	4,  10, 4,  16,  22,
	10,  4, 22, 16,  10,
	22,  4, 16, 10,  22,
	10
};
const std::vector<sGravePosition> mGravePositions = {
    { 0xA3, 0x18 },
    { 0x10B, 0x25 },
    { 0x50, 0x23 },
    { 0xF8, 0x40 },
    { 0x3C, 0x2D },
    { 0x9D, 0x1B },
    { 0xA7, 0x1D },
    { 0x54, 0x26 },
    { 0xA1, 0x1F },
    { 0x4C, 0x26 },
    { 0x37, 0x31 },
    { 0x40, 0x31 },
    { 0x99, 0x1F },
    { 0xF5, 0x43 },
    { 0xFC, 0x45 },
    { 0x91, 0x1E },
    { 0x95, 0x24 },
    { 0xAE, 0x1D },
    { 0x9E, 0x22 },
    { 0xA6, 0x22 },
    { 0xAC, 0x23 },
    { 0x8C, 0x22 },
    { 0x85, 0x25 },
    { 0x90, 0x26 },
    { 0xB3, 0x21 },
    { 0xBB, 0x23 },
    { 0x9B, 0x28 },
    { 0xA3, 0x26 },
    { 0xAB, 0x29 },
    { 0xB2, 0x27 },
    { 0xB9, 0x29 },
    { 0x8A, 0x28 },
    { 0x80, 0x28 },
    { 0x85, 0x2B },
    { 0x8F, 0x2D },
    { 0x95, 0x2A },
    { 0xA1, 0x2B },
    { 0xB1, 0x2D },
    { 0xA7, 0x2D },
    { 0x99, 0x2E },
    { 0x9E, 0x30 },
    { 0xC0, 0x27 },
    { 0xBF, 0x2B },
    { 0xC7, 0x2A },
    { 0xB8, 0x2F },
    { 0xAC, 0x30 },
    { 0x50, 0x29 },
    { 0x58, 0x29 },
    { 0x55, 0x2D },
    { 0x131, 0xBD },
    { 0x7A, 0x2A },
    { 0x80, 0x2F },
    { 0x88, 0x2F },
    { 0xC4, 0x30 },
    { 0xCD, 0x31 },
    { 0x95, 0x32 },
    { 0x8D, 0x33 },
    { 0x74, 0x2E },
    { 0x79, 0x32 },
    { 0x86, 0x34 },
    { 0xA5, 0x32 },
    { 0xB4, 0x35 },
    { 0xBD, 0x32 },
    { 0xC9, 0x37 },
    { 0xD4, 0x36 },
    { 0x9F, 0x37 },
    { 0x92, 0x38 },
    { 0x7F, 0x36 },
    { 0x73, 0x34 },
    { 0x6D, 0x36 },
    { 0x4B, 0x2B },
    { 0x5C, 0x2C },
    { 0x63, 0x2B },
    { 0x60, 0x30 },
    { 0x34, 0x34 },
    { 0x3D, 0x38 },
    { 0x44, 0x36 },
    { 0x49, 0x33 },
    { 0x4C, 0x38 },
    { 0x45, 0x3B },
    { 0x38, 0x3B },
    { 0x30, 0x38 },
    { 0x33, 0x3F },
    { 0x2E, 0x43 },
    { 0x34, 0x45 },
    { 0x39, 0x41 },
    { 0x40, 0x3F },
    { 0x53, 0x38 },
    { 0x4C, 0x3D },
    { 0x47, 0x41 },
    { 0x3F, 0x45 },
    { 0x39, 0x49 },
    { 0x33, 0x4C },
    { 0x2D, 0x48 },
    { 0x2E, 0x50 },
    { 0x31, 0x55 },
    { 0x37, 0x50 },
    { 0x3F, 0x4A },
    { 0x45, 0x46 },
    { 0xAD, 0xB5 },
    { 0x107, 0x29 },
    { 0x10F, 0x29 },
    { 0x10C, 0x2C },
    { 0x101, 0x27 },
    { 0x115, 0x28 },
    { 0x114, 0x2B },
    { 0x111, 0x30 },
    { 0x109, 0x30 },
    { 0x103, 0x2C },
    { 0xFB, 0x2B },
    { 0xFF, 0x2F },
    { 0xF5, 0x2C },
    { 0x119, 0x2C },
    { 0x11E, 0x2F },
    { 0x119, 0x32 },
    { 0x115, 0x36 },
    { 0x121, 0x35 },
    { 0x10D, 0x36 },
    { 0x104, 0x34 },
    { 0xFA, 0x31 },
    { 0x8A, 0x39 },
    { 0x99, 0x39 },
    { 0x96, 0x3E },
    { 0xA7, 0x37 },
    { 0xA3, 0x3D },
    { 0x9E, 0x3F },
    { 0xAD, 0x35 },
    { 0xAC, 0x3A },
    { 0xB3, 0x3C },
    { 0xAA, 0x40 },
    { 0xB1, 0x41 },
    { 0xB9, 0x35 },
    { 0xC2, 0x35 },
    { 0xBE, 0x39 },
    { 0xBA, 0x3E },
    { 0xC3, 0x3D },
    { 0xCF, 0x3D },
    { 0xCA, 0x40 },
    { 0xD8, 0x3C },
    { 0x8E, 0x3F },
    { 0x84, 0x3C },
    { 0x79, 0x3A },
    { 0x67, 0x37 },
    { 0x72, 0x3B },
    { 0x7F, 0x3E },
    { 0x9B, 0x43 },
    { 0x89, 0x41 },
    { 0x94, 0x44 },
    { 0xA4, 0x42 },
    { 0xB7, 0x43 },
    { 0xC1, 0x43 },
    { 0xD4, 0x42 },
    { 0xDB, 0x44 },
    { 0xF9, 0x48 },
    { 0x101, 0x47 },
    { 0xF3, 0x47 },
    { 0xFF, 0x4C },
    { 0xF3, 0x30 },
    { 0xEE, 0x32 },
    { 0xFE, 0x35 },
    { 0x11C, 0x38 },
    { 0x111, 0x3B },
    { 0x108, 0x3B },
    { 0xF6, 0x36 },
    { 0xE9, 0x33 },
    { 0xF0, 0x37 },
    { 0xE5, 0x37 },
    { 0xEB, 0x3B },
    { 0xF0, 0x3C },
    { 0xFB, 0x39 },
    { 0x103, 0x3B },
    { 0x10D, 0x3F },
    { 0x118, 0x3E },
    { 0x120, 0x3B },
    { 0x112, 0x42 },
    { 0x118, 0x44 },
    { 0x105, 0x4C },
    { 0xFB, 0x4F },
    { 0x103, 0x52 },
    { 0xED, 0x43 },
    { 0xDF, 0x40 },
    { 0x6C, 0x3C },
    { 0x76, 0x40 },
    { 0x7F, 0x44 },
    { 0x86, 0x45 },
    { 0x8E, 0x47 },
    { 0x99, 0x4A },
    { 0xA1, 0x47 },
    { 0xA8, 0x4B },
    { 0xB0, 0x48 },
    { 0xBA, 0x4B },
    { 0xC4, 0x4A },
    { 0xCC, 0x47 },
    { 0xD4, 0x48 },
    { 0xE1, 0x4A },
    { 0xDA, 0x4D },
    { 0xCF, 0x4E },
    { 0xC3, 0x50 },
    { 0x135, 0xB1 },
    { 0x135, 0x4B },
    { 0x132, 0x4F },
    { 0x135, 0x57 },
    { 0x130, 0x5A },
    { 0x135, 0x63 },
    { 0x12F, 0x64 },
    { 0x135, 0x6D },
    { 0x109, 0x52 },
    { 0x61, 0x3C },
    { 0x67, 0x40 },
    { 0x71, 0x45 },
    { 0x7A, 0x49 },
    { 0xB2, 0x50 },
    { 0x93, 0x4E },
    { 0xA3, 0x52 },
    { 0xBB, 0x55 },
    { 0xCC, 0x56 },
    { 0xD6, 0x54 },
    { 0xE2, 0x52 },
    { 0xE9, 0x4C },
    { 0x86, 0x4F },
    { 0x99, 0x56 },
    { 0x6A, 0x48 },
    { 0x5E, 0x47 },
    { 0x72, 0x50 },
    { 0x7E, 0x52 },
    { 0x8C, 0x56 },
    { 0xA1, 0x59 },
    { 0xAE, 0x58 },
    { 0xB7, 0x5D },
    { 0xC4, 0x5D },
    { 0xD3, 0x5D },
    { 0xDD, 0x5B },
    { 0xEB, 0x56 },
    { 0xE7, 0x5F },
    { 0xF4, 0x57 },
    { 0xF1, 0x63 },
    { 0xDE, 0x67 },
    { 0xD1, 0x68 },
    { 0xC9, 0x63 },
    { 0xBB, 0x67 },
    { 0xAA, 0x63 },
    { 0x99, 0x62 },
    { 0x8E, 0x61 },
    { 0x83, 0x5C },
    { 0x77, 0x5D },
    { 0x6B, 0x56 },
    { 0x61, 0x50 },
    { 0x56, 0x4F },
    { 0xA2, 0x69 },
    { 0x54, 0x44 },
    { 0x4A, 0x50 },
    { 0x52, 0x56 },
    { 0x5E, 0x5C },
    { 0x7E, 0x66 },
    { 0x91, 0x69 },
    { 0x9B, 0x6F },
    { 0xAE, 0x6E },
    { 0xBB, 0x71 },
    { 0xC5, 0x6C },
    { 0xCE, 0x73 },
    { 0xDA, 0x70 },
    { 0xE8, 0x6C },
    { 0x89, 0x6D },
    { 0x6C, 0x64 },
    { 0x75, 0x6E },
    { 0xFB, 0x63 },
    { 0xF4, 0x6F },
    { 0xE4, 0x76 },
    { 0x57, 0x62 },
    { 0x49, 0x5C },
    { 0x62, 0x6B },
    { 0x7F, 0x77 },
    { 0x8D, 0x75 },
    { 0x9A, 0x78 },
    { 0xA6, 0x72 },
    { 0xB4, 0x7A },
    { 0xBF, 0x7E },
    { 0xA4, 0x7E },
    { 0xD5, 0x7A },
    { 0xEE, 0x74 },
    { 0x104, 0x64 },
    { 0x6B, 0x6F },
    { 0x58, 0x6B },
    { 0x4D, 0x64 },
    { 0x3F, 0x5D },
    { 0x50, 0x70 },
    { 0x43, 0x66 },
    { 0x37, 0x64 },
    { 0x30, 0x6C },
    { 0x3C, 0x72 },
    { 0x46, 0x73 },
    { 0x59, 0x76 },
    { 0x65, 0x77 },
    { 0x8A, 0x82 },
    { 0x72, 0x7A },
    { 0x95, 0x83 },
    { 0xB0, 0x86 },
    { 0xCB, 0x7D },
    { 0xE1, 0x82 },
    { 0x100, 0x6E },
    { 0xC6, 0x89 },
    { 0xBB, 0x8D },
    { 0xA2, 0x8D },
    { 0x7E, 0x87 },
    { 0x6B, 0x85 },
    { 0x4B, 0x80 },
    { 0x34, 0x7F },
    { 0x5C, 0x8A },
    { 0x40, 0x8A },
    { 0x71, 0x92 },
    { 0x8C, 0x94 },
    { 0x9C, 0x9B },
    { 0xB0, 0x96 },
    { 0xC6, 0x9A },
    { 0xD6, 0x8C },
    { 0xF6, 0x81 },
    { 0x101, 0x7B },
    { 0xEE, 0x8A },
    { 0xE5, 0x94 },
    { 0xD8, 0x9B },
    { 0xFC, 0x8F },
    { 0x109, 0x84 },
    { 0x101, 0x85 },
    { 0x107, 0x95 },
    { 0xF6, 0x99 },
    { 0xED, 0xA2 },
    { 0xE0, 0xA9 },
    { 0xD0, 0xAA },
    { 0xC1, 0xAD },
    { 0xAF, 0xA5 },
    { 0x9D, 0xAB },
    { 0x88, 0x9B },
    { 0x7B, 0xA1 },
    { 0x68, 0x9A },
    { 0x55, 0x97 },
    { 0x44, 0x9C },
    { 0x37, 0x95 },
    { 0x35, 0xA3 },
    { 0x48, 0xA5 },
    { 0x5A, 0xA6 },
    { 0x84, 0xB0 },
    { 0x6E, 0xAA },
    { 0x91, 0xAA },
    { 0xBC, 0x9F },
    { 0xF5, 0xAB },
    { 0x104, 0xA4 },
    { 0xCD, 0x91 },
    { 0x93, 0x8B },
    { 0x79, 0x8D },
    { 0x5F, 0x7F },
    { 0x50, 0x88 },
    { 0x60, 0xAF },
    { 0x43, 0xAF },
    { 0x51, 0xAC },
    { 0xCB, 0xB2 },
    { 0xE9, 0xAF },
    { 0x108, 0x75 },
    { 0x130, 0x6A },
    { 0x95, 0xB6 },
    { 0x115, 0x8A },
};


int16 cFodder::Recruit_Show() {

    Map_Load();
    Map_Load_Sprites();

    Phase_Soldiers_Count();
    mGame_Data.Soldier_Sort();
    Phase_Soldiers_Prepare(true);
    Phase_Soldiers_AttachToSprites();

    WindowTitleSet(false);

	if (!mStartParams->mDisableSound)
		mSound->Music_Play(0);

    // Retail / Custom set show the Recruitment Hill
    if (mVersionCurrent->isRetail() || mVersionCurrent->isPCFormat() || mVersionCurrent->isRandom() || mCustom_Mode == eCustomMode_Set) {

        // Recruit Screen
        if (Recruit_Loop())
            return -1;

        Recruit_CheckLoadSaveButtons();

        // Did we just load/save a game?
        if (mRecruit_Button_Load_Pressed || mRecruit_Button_Save_Pressed) {
            mRecruit_Mission_Restarting = true;
            mGame_Data.mMission_Recruitment = -1;
            mPhase_Aborted = true;
            return -3;
        }
    }
    else {

        // Amiga demos have a menu
        if (mVersionCurrent->mPlatform == ePlatform::Amiga) {

            // But not custom games
            if (mCustom_Mode == eCustomMode_None) {

                if (!mVersionCurrent->isAmigaTheOne()) {
                    if (Demo_Amiga_ShowMenu())
                        return -1;
                }
            }
        }
    }

    mRecruit_Mission_Restarting = false;
    GameData_Backup();

    // Retail or Custom Mode
    if (mVersionCurrent->isRetail() ||
        mCustom_Mode != eCustomMode_None) {
        Map_Load();

        // Show the intro for the briefing screen
        Mission_Intro_Play(false, mMapLoaded->getTileType());
    }

    mGraphics->Load_pStuff();

    return 0;
}

void cFodder::Recruit_Prepare() {

    mPhase_Aborted = false;

    Recruit_Truck_Anim_Prepare();
    Recruit_Render_Sidebar();

    mGraphics->Load_Hill_Data();
    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);
    mGraphics->Recruit_Draw_Hill();

    Recruit_Copy_Sprites();

    if (mVersionCurrent->mPlatform == ePlatform::Amiga)
        GetGraphics<cGraphics_Amiga>()->Hill_Prepare_Overlays();

    mGraphics->SetActiveSpriteSheet(eGFX_HILL);

    Recruit_Draw_Graves();

    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();
    mSurface->Save();

    mRecruit_Rendereds.clear();
    Recruit_Render_Names_UnusedSlots();

    mRecruit_RenderedNext = mRecruit_Rendereds.begin();
    for (int16 ax = mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count - 1; ax >= 0; --ax)
        Recruit_Sidebar_Render_SquadName();
}

bool cFodder::Recruit_Loop() {

    mRecruit_Screen_Active = true;
    mSurface->clearBuffer();

    // If demo data is loaded, we need to load
    if (mVersionCurrent->isDemo()) {

        if (mCustom_Mode == eCustomMode_Set) {
            VersionSwitch(mVersionDefault);
        }
        // Failed to switch mode?
        if (mVersionCurrent->isDemo())
            return 0;
    }

	mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());

    Mouse_Setup();

    Sidebar_Clear_ScreenBufferPtr();
    Recruit_Prepare();

    mGUI_Mouse_Modifier_X = 0;
    mGUI_Mouse_Modifier_Y = 0x1D;

    mMouseCursor_Enabled = -1;

    Recruit_Prepare_Anims();

    Recruit_Update_Actors();
    Recruit_Update_Actors();

    mMouse_Exit_Loop = false;

    mSurfaceRecruit->copyFrom(mSurface);
    mInterruptCallback = [this]() {
        
        int16 DataC = PLATFORM_BASED(0xB6, 0xBE);
        GUI_Draw_Frame_8(0x22, mRecruit_Truck_Frame, 0x31, DataC);

        Recruit_Draw_Soldiers();
        mGraphics->Sidebar_Copy_To_Surface(0x18);

        if (mVersionCurrent->isPC())
            mGraphics->Recruit_Draw_HomeAway();

        if (mMouseCursor_Enabled)
            Mouse_DrawCursor();

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        mSurfaceRecruit->copyFrom(mSurface);
    };

    for (;; ) {
        {
            std::lock_guard<std::mutex> lock(mSurfaceMtx);
            Recruit_Update_Actors();
        }

        if (mMouse_Exit_Loop) {
            mMouse_Exit_Loop = false;

            if (Recruit_Check_Buttons_SaveLoad())
                break;
        }

        if (mPhase_Aborted)
            break;

        Video_Sleep(mSurfaceRecruit, false, false);  
        Video_Sleep(mSurfaceRecruit, false, false);
    }

    mRecruit_Screen_Active = false;

    mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
    mMouseCursor_Enabled = 0;

    mSurface->paletteNew_SetToBlack();

    while (mSurface->isPaletteAdjusting()) {
        {
            std::lock_guard<std::mutex> lock(mSurfaceMtx);
            Recruit_Update_Actors();
        }

        Video_Sleep(mSurfaceRecruit, false, false);
        Video_Sleep(mSurfaceRecruit, false, false);
    }

    mInterruptCallback = nullptr;

    return mPhase_Aborted;
}

void cFodder::Recruit_Draw_String(int32 pParam0, size_t pParam8, size_t pParamC, const std::string& pString) {

    for (std::string::const_iterator Text = pString.begin(); Text != pString.end(); ++Text, pParam8 += 0x0C) {
        char NextChar = *Text;

        if (NextChar == 0x20)
            continue;

        if (NextChar <= 0x39) {
            NextChar -= 0x30;
            NextChar += 0x1A;
        }
        else
            NextChar -= 0x41;

        GUI_Draw_Frame_8(pParam0, NextChar, pParam8, pParamC);
    }
}

void cFodder::Recruit_Truck_Anim_Prepare() {
    uint16* di = mRecruit_Truck_FramesPlay;
    int16  count;

    // Do any troops need to enter the truck?
    if (mGame_Data.mGamePhase_Data.mSoldiers_Required > 0) {

        // Welcome Troops
        Recruit_Truck_Anim_CopyFrames(&di, mRecruit_Truck_Anim_Welcome);

        count = mGame_Data.mGamePhase_Data.mSoldiers_Required;
        count -= 2;

        // Wave Troop Past
        while (count >= 0) {
            Recruit_Truck_Anim_CopyFrames(&di, mRecruit_Truck_Anim_PassTroop);
            --count;
        }

        // Swing Arm to stop Troops
        Recruit_Truck_Anim_CopyFrames(&di, mRecruit_Truck_Anim_SwingArm);

        // Close the truck door
        Recruit_Truck_Anim_CopyFrames(&di, mRecruit_Truck_Anim_CloseDoor);
    }

    // Set end markers
    *di++ = 0;
    *di = -1;
}

void cFodder::Recruit_Truck_Anim_CopyFrames(uint16** pDi, const int16* pSource) {
    int16 ax;

    for (;;) {
        ax = *pSource++;

        if (ax < 0)
            break;

        **pDi = ax;
        (*pDi)++;
    }
}

void cFodder::Recruit_Draw_Grave(int16 pSpriteType, const size_t pPosX, const size_t pPosY) {
    pSpriteType >>= 1;
    if (pSpriteType > 8)
        pSpriteType = 8;

    int32 Frame = (int32)pPosY;
    Frame -= 0x14;
    Frame >>= 5;
    if (Frame >= 5)
        Frame = 4;

    GUI_Draw_Frame_8(pSpriteType, Frame, pPosX, pPosY - 8);
}

void cFodder::Recruit_Draw_Graves() {
    auto GraveIT = mGravePositions.rbegin();

    if (mGame_Data.mHeroes.empty())
        return;

    for (size_t i = mGame_Data.mHeroes.size(); i > 0; --i) {
        ++GraveIT;
    }

    for (auto& Hero : mGame_Data.mHeroes) {
        --GraveIT;
        Recruit_Draw_Grave(Hero.mRank, GraveIT->mX, GraveIT->mY + PLATFORM_BASED(0, 6));
    };

}

void cFodder::Recruit_Render_Sidebar() {
    int16 SpriteType = 0xAD;
    int16 PosX = 0;
    int16 PosY = 0x18;

    // Draw Heroes Heading
    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(SpriteType, PosX, PosY);

    int16 Data14 = 0x0E;
    SpriteType = 4;

    for (auto& Hero : mGame_Data.Heroes_Get()) {
        if (Hero.mKills == 0)
            break;

        Data14 += 0x0C;
    }

    // Draw Empty Hero Slots
    PosY = 0x0E;
    do {
        SpriteType = 0xA9;
        if (PosY >= Data14)
            SpriteType = 0xAB;

        PosX = 0;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(SpriteType, PosX, PosY + 0x18);
        PosY += 0x0C;

    } while (PosY < 0x4A);

    //seg003:1E89

    // Draw Squad Heading
    SpriteType = 0xAE;
    PosX = 0;
    PosY = 0x4A + 0x18;
    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(SpriteType, PosX, PosY);

    PosY = 0x58;
    Data14 = mGame_Data.mGamePhase_Data.mSoldiers_Required + mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count;
    Data14 *= 0x0C;
    Data14 += PosY;

    int16 Final = PLATFORM_BASED(0xA0, 0xB8);

    // Draw Used Slot
    do {
        SpriteType = 0xA9;
        if (PosY >= Data14)
            SpriteType = 0xAB;

        PosX = 0;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(SpriteType, PosX, PosY + 0x18);
        PosY += 0x0C;

    } while (PosY < Final);

    mRecruit_Sidebar_Draw_Y_Start = 0x0F;
    mRecruit_Render_Name_SmallGap = -1;

    Recruit_Render_Squad_RankKills();
    Recruit_Render_Squad_Names();

    mRecruit_Render_Name_SmallGap = 0;
    mRecruit_Sidebar_Draw_Y_Start = -59;

    Recruit_Render_HeroList();
    mRecruit_Sidebar_Draw_Y_Start = 0;
}

void cFodder::Recruit_Render_Squad_Names() {

    int16 word_3A3BB = 7;
    mSidebar_Draw_Y = 0;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mRecruitID == -1)
            continue;

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        if (mSquad_Selected != Troop.mSprite->field_32)
            continue;

        const sRecruit* Data28 = &mRecruits[Troop.mRecruitID];
        int16 Data14;

        for (Data14 = 0; Data14 <= 5; ++Data14) {

            if (Data28->mName[Data14] == 0x20)
                break;
        }

        Data14 <<= 2;
        word_3A05F = (0x30 - Data14) >> 1;

        if (mRecruit_Render_Name_SmallGap)
            word_3A05F -= 1;

        // Draw Troop name to list
        for (Data14 = 0; Data14 <= 5; ++Data14) {

            if (Data28->mName[Data14] != 0x20) {
                int16 Data0 = Data28->mName[Data14];
                Data0 -= 0x41;
                Data0 += 0x29;

                int16 X = Data14;
                X <<= 2;
                X += word_3A05F;

                int16 Y = 0x4B;
                Y += mSidebar_Draw_Y;
                Y += mRecruit_Sidebar_Draw_Y_Start;

                mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, X, Y + 0x18);
            }
        }

        mSidebar_Draw_Y += 0x0C;

        if (--word_3A3BB < 0)
            break;
    }

}

void cFodder::Recruit_Render_Squad_RankKills() {
    int16 Data0;

    mSidebar_Draw_Y = 0;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mRecruitID == -1)
            continue;

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        if (mSquad_Selected != Troop.mSprite->field_32)
            continue;

        int16 Data8 = 0;
        int16 DataC = mSidebar_Draw_Y - 1;

        if (!mRecruit_Render_Name_SmallGap) {
            Data0 = Troop.mRank + 9;
            Data8 = 0x23;

            DataC += 0x4A;
            DataC += mRecruit_Sidebar_Draw_Y_Start;

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, Data8, DataC + 0x18);

        }
        else {
            // Draw Kills
            Recruit_Render_Number(Troop.mNumberOfKills, 0x43);

        }

        //Draw Rank
        Data0 = Troop.mRank + 9;

        DataC += 0x4A;
        DataC += mRecruit_Sidebar_Draw_Y_Start;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, 0, DataC + 0x18);
        mSidebar_Draw_Y += 0x0C;
    }
}

void cFodder::Recruit_Render_Number(int16 pNumber, int16 pData10) {

    pData10 -= 0x30;
    std::string Number = tool_StripLeadingZero(std::to_string(pNumber));
    uint16 Data0 = (uint16)Number.length() * 4;

    int16 Data8 = 0x30 - Data0;
    int16 DataC = 0x4B + mSidebar_Draw_Y + mRecruit_Sidebar_Draw_Y_Start;

    for (auto& Char : Number) {
        Data0 = (Char & 0xFF) + pData10;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Data0, Data8, DataC + 0x18);
        Data8 += 4;
    }
}

void cFodder::Recruit_Render_HeroList() {

    int16 HeroesToRender = 4;
    mSidebar_Draw_Y = 0;

    for (auto& Hero : mGame_Data.Heroes_Get()) {

        if (Hero.mRecruitID < 0 || Hero.mKills == 0)
            continue;

        int16 Data8 = 0;
        int16 DataC = mSidebar_Draw_Y - 1;
        DataC += 0x4A + mRecruit_Sidebar_Draw_Y_Start + 0x18;

        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Hero.mRank + 9, Data8, DataC);

        const sRecruit* Troop = &mRecruits[Hero.mRecruitID];
        int16 Position;
        for (Position = 0; Position <= 5; ++Position) {
            if (Troop->mName[Position] == 0x20)
                break;
        }

        Position <<= 2;
        word_3A05F = (0x30 - Position) >> 1;
        --word_3A05F;

        for (Position = 0; Position <= 5; ++Position) {

            uint8 Character = Troop->mName[Position];
            if (Character == 0x20)
                continue;

            Character -= 0x41;
            Character += 0x29;
            Data8 = (Position << 2) + word_3A05F;
            DataC = 0x4B + mSidebar_Draw_Y + mRecruit_Sidebar_Draw_Y_Start;

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(Character, Data8, DataC + 0x18);
        }

        Recruit_Render_Number(Hero.mKills, 0x67);
        mSidebar_Draw_Y += 0x0C;

        if (--HeroesToRender < 0)
            break;
    }
}

void cFodder::Recruit_Render_Names_UnusedSlots() {
    uint32* Data20 = (uint32*)mGraphics->mImageTemporary.mData->data();

    int16 Row = PLATFORM_BASED(0x58, 0x44);

    // Squad Troop Names
    for (; Row < 0xA0; Row += 0x0C) {
        mRecruit_Rendereds.emplace_back(Data20, Row);

        mGraphics->Sidebar_Copy_ScreenBuffer(Row, 0x0C, 0, Data20);
    }

    //seg003:2532
    int16 DataC = 0x58;
    int16 Slots = PLATFORM_BASED(5, 7);    // Empty Slots

    for (; Slots >= 0; --Slots) {
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(0xAC, 0, DataC + 0x18);

        DataC += 0x0C;
    }
}

void cFodder::Recruit_Sidebar_Render_SquadName() {
    if (mRecruit_RenderedNext == mRecruit_Rendereds.end())
        return;

    if (mRecruit_RenderedNext->mPosition < 0)
        return;

    mGraphics->Sidebar_Copy_ScreenBuffer((uint16)mRecruit_RenderedNext->mPosition, 0x0C, -1, mRecruit_RenderedNext->mDataPtr);
    ++mRecruit_RenderedNext;
}

void cFodder::Recruit_Update_Actors() {

    Recruit_Update_Truck();
    Recruit_Update_Soldiers();
}

void cFodder::sub_175C0() {
    sRecruit_Screen_Pos* Data20 = 0;

    if (mRecruit_Truck_Troops_ToEnter_Count > 0) {

        sRecruit_Screen_Pos* Data20 = &mRecruit_Screen_Positions[293];
        do {
            if (Data20 == &mRecruit_Screen_Positions[0])
                return;

            --Data20;

        } while (Data20->mFrame == 0 || Data20->mFrame < 4);

        int16 Data0 = Data20->mFrame;
        Data20->mFrame = 0;
        Data0 ^= 1;


        if ((Data20 + 1)->mX >= 0) {
            (Data20 + 1)->mFrame = Data0;
        }
        else {
            --mRecruit_Truck_Troops_ToEnter_Count;
            Recruit_Sidebar_Render_SquadName();
        }
    }
    //loc_17652

    uint16 aa = ~(dword_3B1CB >> 16);

    dword_3B1CB = (aa << 16) | (dword_3B1CB & 0xFFFF);

    Data20 = &mRecruit_Screen_Positions[293];
    sRecruit_Screen_Pos* Data28 = &mRecruit_Screen_Positions[271];

loc_17686:;
    for (; Data20 != &mRecruit_Screen_Positions[0];) {
        --Data20;

        if (Data20->mFrame == 0)
            continue;

        if (Data20->mFrame < 4)
            continue;

        if (Data20 >= Data28)
            continue;

        int16 Data8 = 3;
        sRecruit_Screen_Pos* Data30 = Data20;
        //loc_176CE

        do {
            ++Data30;
            if (Data30->mFrame) {
                if (Data30->mFrame > 3)
                    goto loc_17686;

                ++Data30;
            }
            --Data8;
        } while (Data8 >= 0);

        // Animate the frame
        int16 Data0 = Data20->mFrame;
        Data20->mFrame = 0;

        if ((dword_3B1CB >> 16) < 0)
            Data0 ^= 1;

        if ((Data20 + 1)->mFrame) {
            (Data20 + 2)->mFrame = Data0;
        }
        else {
            (Data20 + 1)->mFrame = Data0;
        }
    }

}

void cFodder::Recruit_Draw_Soldiers() {
    sRecruit_Screen_Pos* Data20 = mRecruit_Screen_Positions;
    sRecruit_Screen_Pos* dword_3B1C7;

    for (; ; Data20 = dword_3B1C7) {

        dword_3B1C7 = Data20;

        if (Data20->mX < 0)
            break;

        int16 Data0;
        int16 Data8 = Data20->mX;
        int16 DataC = Data20->mY;
        int16 Data4 = Data20->mFrame;
        ++Data20;
        dword_3B1C7 = Data20;

        if (Data4 == 0)
            continue;

        --Data4;
        if (Data4 >= 3) {
            //loc_177E3
            Data4 -= 3;
            Data0 = 0x0A;
            Data0 += Data4;

            // Sprite reached end of last curve
            if (DataC >= 0xB7)
                Data0 += 4;

            // Sprite on last curve
            else if (DataC > 0x7C)
                Data0 += 2;

            // First dip of mountain is frame 0
            Data4 = 0;

            // Second Dip is frame 1
            if (Data20 > &mRecruit_Screen_Positions[0x46]) {
                Data4 = 1;

                // Last leg of mountain
                if (Data20 > &mRecruit_Screen_Positions[0x7A])
                    Data4 = 2;
            }
        }
        else
            Data0 = 9;  // Hill Piece

        //loc_1784C
        if (mVersionCurrent->isPC()) {
            DataC -= 8;

            // Hill pieces are 16bit graphics
            if (Data0 != 9)
                GUI_Draw_Frame_8(Data0, Data4, Data8, DataC);
            else {
                ++Data8;
                --DataC;
                GUI_Draw_Frame_16(Data0, Data4, Data8, DataC);
            }

        }
        else {
            GUI_Draw_Frame_8(Data0, Data4, Data8, DataC);
        }
    }
}

void cFodder::Recruit_Update_Soldiers() {

    if (mRecruit_Truck_Animation_Play) {
        sub_175C0();
    }
    else {
        Recruit_Frame_Check();
        Recruit_Position_Troops();
    }

    //Recruit_Draw_Soldiers();
}

void cFodder::Recruit_Prepare_Anims() {

    if (mRecruit_Mission_Restarting)
        return;

    mRecruit_Truck_Troops_ToEnter_Count = mGame_Data.mGamePhase_Data.mSoldiers_Required;
    mRecruit_Truck_Animation_Play = 0;

    int16 Data0 = 0x1E;
    int16* Data20 = mRecruit_Hill_Positions_Use;
    const int16* Data24 = mRecruit_Hill_Position_Gaps;

    // Loop the 15 positions
    for (int16 Data4 = 0x0E; Data4 >= 0; --Data4) {

        *Data20++ = Data0;
        Data0 -= *Data24++;

    }
}

void cFodder::Recruit_Frame_Check() {
    sRecruit_Screen_Pos* Data20 = mRecruit_Screen_Positions;

    for (; Data20->mX >= 0; ++Data20) {

        if (Data20->mFrame >= 4)
            Data20->mFrame = 0;
    }
}

void cFodder::Recruit_Position_Troops() {
    const int16*    Recruit_Next_Shirt = mRecruit_Shirt_Colors;

    uint64 Data8 = mGame_Data.mHeroes.size();
    int16 Recruits, Data4;

    // Calculate number of dead troops
    Data8 &= 0xF;

    if (mRecruit_Mission_Restarting)
        Recruits = mGame_Data.mRecruits_Available_Count;
    else {
        Recruits = mGame_Data.mMission_Recruits_AliveCount;
        if (!Recruits)
            goto loc_179B2;
    }

    // loc_1795E
    --Recruits;
    Data4 = 0x110;

    // Set the recruit position frames
    for (; Recruits >= 0; --Recruits) {

        if (Data4 >= 0) {
            if (mRecruit_Screen_Positions[Data4].mFrame)
                --Data4;

            mRecruit_Screen_Positions[Data4].mFrame = mRecruit_Shirt_Colors[Data8];
        }

        Data8 += 1;
        Data8 &= 0xF;
        Data4 -= 4;
    }

loc_179B2:;
    // If the mission is restarting, troops are already over the hill
    if (mRecruit_Mission_Restarting)
        return;

    mRecruit_Truck_Reached = 0;
    int16 Data0 = mRecruit_Hill_Positions_Use[0];
    Data8 = 5;

    for (; ; Data0++) {

        Data4 = mRecruit_Screen_Positions[Data0].mFrame;
        if (Data4) {
            if (Data4 > 3) {
                mRecruit_Truck_Reached = -1;
                break;
            }
            else
                continue;

        }
        else {
            if (Data0 == 0x110) {
                mRecruit_Truck_Reached = -1;
                break;
            }
            --Data8;
            if (!Data8)
                break;
        }
    }
    //seg003:2B6D
    if (!mRecruit_Truck_Reached) {
        dword_3B1CB += 0x8000;
        dword_3B1CB &= 0x1FFFF;
    }
    else
        dword_3B1CB = 0x10000;

    int16* Data20 = mRecruit_Hill_Positions_Use;

    // 14 moving recruits
    for (int16 DData8 = 0x0E; DData8 >= 0; --DData8) {

        // Get the next position
        Data0 = *Data20;

        if (Data0 >= 0) {

            // Frame
            Data4 = (dword_3B1CB >> 16);
            Data4 += *Recruit_Next_Shirt++;

            if (mRecruit_Screen_Positions[Data0].mX < 0)
                Data0 = 0;

            if (mRecruit_Screen_Positions[Data0].mFrame)
                ++Data0;

            mRecruit_Screen_Positions[Data0].mFrame = Data4;
        }
        // 17A9D

        if (!mRecruit_Truck_Reached)
            Data0++;

        //loc_17AA9
        *Data20 = Data0;
        ++Data20;
    }

    if (mRecruit_Truck_Reached)
        mRecruit_Truck_Animation_Play = -1;
}

void cFodder::Recruit_Update_Truck() {
    int16 DataC = PLATFORM_BASED(0xB6, 0xBE);

   // GUI_Draw_Frame_8(0x22, mRecruit_Truck_Frame, 0x31, DataC);

    // If no troop has reached the truck, don't do animation
    if (!mRecruit_Truck_Animation_Play)
        return;

    int16 Data0 = *(mRecruit_Truck_FramesPlay + mRecruit_Truck_FrameAnimBufferPtr);
    if (Data0 < 0)
        return;

    ++mRecruit_Truck_FrameAnimBufferPtr;
    mRecruit_Truck_Frame = Data0;
}

void cFodder::Recruit_Copy_Sprites() {

    const sRecruit_Sprites* stru = mRecruitSprite;

    for (; stru->mSpriteType != -1;) {
        int16 word_3B1A3 = stru->field_4;
        int16 word_3B1A5 = stru->field_6;
        int16* Data34 = stru->field_8;

        for (;;) {
            auto SpriteSheet = Sprite_Get_Sheet(stru->mSpriteType, stru->mFrame);

            int16 Columns = SpriteSheet->mColCount;
            int16 Rows = SpriteSheet->mRowCount;
            int16 Data8 = *Data34++;
            int16 DataC = 0;

            if (Data8 < 0) {
                ++stru;
                break;
            }

            // Originally Inside sub_A094C
            if (mVersionCurrent->isAmiga()) {
                Columns -= 1;
                Columns <<= 4;
            }

            // Multiply 
            sub_2AEB6(Columns, Rows, &Data8, &DataC);

            int16 Data10 = word_3B1A3 + 0x08;
            int16 Data14 = word_3B1A5;
            mGraphics->Recruit_Sprite_Draw(Columns, Rows,
                Data8, Data10,
                Data14, DataC, SpriteSheet->GetGraphicsPtr());
            word_3B1A3 += 0x10;
        }
    }
}

bool cFodder::Recruit_Check_Buttons_SaveLoad() {
    mRecruit_Button_Load_Pressed = 0;
    mRecruit_Button_Save_Pressed = 0;

    if (mMouseX <= -16 && mMouseY <= 26) {
        mRecruit_Button_Load_Pressed = -1;
        return true;
    }

    if (mMouseX >= 0x10F) {
        if (mMouseY > 0x1A)
            return true;

        if (mMission_Save_Blocked[mGame_Data.mMission_Number - 1])
            return false;

        mRecruit_Button_Save_Pressed = -1;
        return true;
    }

    if (mMouseY < 0x1A)
        return false;

    return true;
}

void cFodder::Recruit_CheckLoadSaveButtons() {

    mVersionPlatformSwitchDisabled = true;
    if (mRecruit_Button_Load_Pressed) {
        Game_Load();

    }
    else if (mRecruit_Button_Save_Pressed) {
        Game_Save_Wrapper();
    }
    mVersionPlatformSwitchDisabled = false;
}
