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

#include "Types.hpp"
#include <vector>
#include "IntroData.hpp"


// Intro_0 has had +0x19 added to each string
const sIntroString mIntro_0[] = {
	{ 0x5F, "This game is not in any way" },
	{ 0x77, "endorsed by the" },
	{ 0x91, "ROYAL BRITISH LEGION" },
	{ 0, 0 }
};

const sIntroString mIntro_1[] = {
	{ 0xCC, "A Sensible Software Game" },
	{ 0, 0 }
};

const sIntroString mIntro_2[] = {
	{ 0x64, "STARRING" },
	{ 0, 0 }
};

const sIntroString mIntro_3[] = {
	{ 0xA4, "JOOLS" },
	{ 0xB5, "as" },
	{ 0xC8, "James Dean" },
	{ 0, 0 }
};

const sIntroString mIntro_4[] = {
	{ 0xA4, "STOO" },
	{ 0xB4, "as" },
	{ 0xC8, "Elvis" },
	{ 0, 0 }
};

const sIntroString mIntro_5[] = {
	{ 0xA4, "RICHARD JOSEPH" },
	{ 0xB4, "as" },
	{ 0xC8, "Sgt. Bilko" },
	{ 0, 0 }
};

const sIntroString mIntro_6[] = {
	{ 0x64, "Co. STARRING" },
	{ 0, 0 }
};

const sIntroString mIntro_7[] = {
	{ 0xA4, "CHRIS YATES" },
	{ 0xB4, "as" },
	{ 0xC8, "JR out of Dallas" },
	{ 0, 0 }
};

const sIntroString mIntro_8[] = {
	{ 0xA4, "JON HARE" },
	{ 0xB5, "as" },
	{ 0xC8, "Vera Lynn" },
	{ 0, 0 }
};

const sIntroString mIntro_9[] = {
	{ 0x64, "SPECIAL GUEST STAR" },
	{ 0, 0 }
};

const sIntroString mIntro_10[] = {
	{ 0xA4, "CHRIS CHAPMAN" },
	{ 0xB4, "as" },
	{ 0xC8, "Norman Wisdom" },
	{ 0, 0 }
};

const sIntroString mIntro_11[] = {
	{ 0x50, "Jools" },
	{ 0x8C, "Stoo" },
	{ 0x3C, "Program Design" },
	{ 0x78, "Graphic Design" },
	{ 0, 0 }
};

const sIntroString mIntro_12[] = {
	{ 0x32, "Richard Joseph" },
	{ 0x6E, "Jon Hare" },
	{ 0xAA, "David Hindley" },
	{ 0x1E, "Sound effects and Music" },
	{ 0x5A, "Original Soundtrack"},
	{ 0x96, "Cameraman" },
	{ 0, 0 }
};

const sIntroString mIntro_13[] = {
	{ 0x28, "Steve Cook" },
	{ 0x5A, "Graftgold" },
	{ 0x8C, "Hammonds Hire" },
	{ 0xBE, "Peter Hickman" },
	{ 0x19, "Vehicle and Stunts" },
	{ 0x46, "Flatbed Scanning" },
	{ 0x78, "Costumes" },
	{ 0xAA, "Mr Virgin" },
	{ 0, 0 }
};

const sIntroString mIntro_14[] = {
	{ 0x19, "An Audio Visual Magic" },
	{ 0x2D, "Entertainment Conversion" },
	{ 0x4B, "PC Programming" },
	{ 0x5F, "Adrian Youlden" },
	{ 0x7D, "PC Graphics" },
	{ 0x91, "Mark Smith" },
	{ 0xAF, "PC Sound and Music" },
	{ 0xC3, "Nigel Taylor" },
	{ 0, 0 }
};

const sIntroString mIntro_15[] = {
	{ 0x19, "PC Version Produced" },
	{ 0x2D, "by" },
	{ 0x4B, "Gavin Wade" },
	{ 0x5F, "and" },
	{ 0x73, "Scott Walsh" },
	{ 0, 0 }
};

const sIntroString mIntro_16[] = {
	{ 0xD2, "Sensible Software." },
	{ 0xBE, "Designed By" },
	{ 0, 0 }
};

std::vector< sIntroText > mIntroText_PC = {
	{ 0xFF, mIntro_0 },
	{ 0x31, mIntro_1 },
	{ 0xFF, mIntro_2 },
	{ 0x32, mIntro_3 },
	{ 0x33, mIntro_4 },
	{ 0x34, mIntro_5 },
	{ 0xFF, mIntro_6 },
	{ 0x36, mIntro_7 },
	{ 0x35, mIntro_8 },
	{ 0xFF, mIntro_9 },
	{ 0x37, mIntro_10 },
	{ 0xFF, mIntro_11 },
	{ 0xFF, mIntro_12 },
	{ 0xFF, mIntro_13 },
	{ 0xFF, mIntro_14 },
	{ 0xFF, mIntro_15 },
	{ 0x38, mIntro_16 },
	{ 0, 0 }
};

std::vector< sIntroText > mIntroText_PC2 = {
	{ 0, 0 },
	{ 0x31, 0 },
	{ 0x32, 0 },
	{ 0x33, 0 },
	{ 0x34, 0 },
	{ 0x35, 0 },
	{ 0x36, 0 },
	{ 0x37, 0 },
	{ 0x38, 0 },
	{ 0x39, 0 },
	{ 0x61, 0 },
	{ 0x62, 0 },
	{ 0x63, 0 },
	{ 0x64, 0 },
	{ 0x65, 0 },
	{ 0, 0 }
};
