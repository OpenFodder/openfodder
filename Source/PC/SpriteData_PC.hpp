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

const int16 mBriefing_ParaHeli_Frames[] = {
	0x00, 0x20, 0x40, 0x60
};

const sSpriteSheet mSpriteSheet_Font_PC[83] = {
	{ 0, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 8, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 16, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 24, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 32, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 40, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 48, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 56, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 64, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 72, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 80, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 88, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 96, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 104, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 112, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 120, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 128, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 136, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 144, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 152, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2720, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2728, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2736, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2744, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2752, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2760, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5536, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5544, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5552, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5560, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5568, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5576, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5584, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5592, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 8160, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 8168, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 10896, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 8192, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 8200, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 8208, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },

	{ 0, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 8, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 16, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 24, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 32, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 40, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 48, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 56, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 64, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 72, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 80, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 88, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 96, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 104, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 112, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 120, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 128, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2768, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2776, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2784, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2792, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2800, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2808, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2816, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2824, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2832, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2840, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2848, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2856, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2864, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 2872, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5440, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5448, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5456, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5464, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5472, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5480, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5488, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5496, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5504, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5512, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5520, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 },
	{ 5528, eGFX_FONT, 0, 0, 16, 17, 208, 0, 0 }
};

const sSpriteSheet* mSpriteSheetTypes_Font_PC[] = {
	mSpriteSheet_Font_PC
};

const sSpriteSheet mRecruitSpriteFrames_Graves_PC[9] =
{
	{ 10880, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10888, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10896, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10904, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10912, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10920, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10928, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10936, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 },
	{ 10944, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 }
};

const sSpriteSheet stru_32420[2] =
{
	{ 5600, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 5608, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32440[2] =
{
	{ 5616, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 5624, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32460[2] =
{
	{ 5632, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 5640, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32480[2] =
{
	{ 5648, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 5656, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_324A0[2] =
{
	{ 5664, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 5672, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_324C0[2] =
{
	{ 5680, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 5688, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_324E0[2] =
{
	{ 8160, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 8168, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32500[2] =
{
	{ 8176, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 8184, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32520[2] =
{
	{ 8192, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 8200, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32540[2] =
{
	{ 8208, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 8216, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32560[2] =
{
	{ 8224, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 8232, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32580[2] =
{
	{ 8240, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 8248, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet mRecruitSpriteFrames_AlphaNum_PC[36] =
{
/* 0x00 */	{ 0, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x01 */	{ 8, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x02 */	{ 16, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x03 */	{ 24, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x04 */	{ 32, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x05 */	{ 40, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x06 */	{ 48, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x07 */	{ 56, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x08 */	{ 64, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x09 */	{ 72, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x0A */	{ 80, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x0B */	{ 88, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x0C */	{ 96, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x0D */	{ 104, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x0E */	{ 112, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x0F */	{ 120, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x10 */	{ 128, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x11 */	{ 136, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x12 */	{ 144, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x13 */	{ 152, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x14 */	{ 2560, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x15 */	{ 2568, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x16 */	{ 2576, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x17 */	{ 2584, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x18 */	{ 2592, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x19 */	{ 2600, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x1A */	{ 2608, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x1B */	{ 2616, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x1C */	{ 2624, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x1D */	{ 2632, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x1E */	{ 2640, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x1F */	{ 2648, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x20 */	{ 2656, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x21 */	{ 2664, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x22 */	{ 2672, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
/* 0x23 */	{ 2680, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_327E0[1] = {
	{ 2688, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};
const sSpriteSheet stru_327F0[1] = {
	{ 2696, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32800[1] = {
	{ 2704, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 }
};

const sSpriteSheet stru_32810[23] =
{
	{ 13440, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13448, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13456, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13464, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13472, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13480, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13488, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13496, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13504, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13512, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13520, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13528, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13536, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13544, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13552, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13560, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13568, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13576, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13584, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 13592, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 16488, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 16496, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 },
	{ 16504, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 }
};

const sSpriteSheet stru_32980[1] = {
	{ 16480, eGFX_RECRUIT, 0, 0, 16, 19, 176, 0, 0 }
};

const sSpriteSheet stru_32990[1] = {
	{ 38528, eGFX_RECRUIT, 0, 0, 16, 1, 176, 0, 0 }
};

const sSpriteSheet stru_329A0[3] =
{
	{ 13544, eGFX_RECRUIT, 0, 0, 16, 13, 176, 0, 0 },
	{ 13560, eGFX_RECRUIT, 0, 0, 32, 16, 176, 0, 0 },
	{ 13580, eGFX_RECRUIT, 0, 0, 32, 30, 176, 0, 0 }
};

const sSpriteSheet mRecruitSpriteFrames_Truck_PC[12] =
{
	{ 5856, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5864, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5872, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5880, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5888, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5896, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9696, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9704, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9712, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9720, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9728, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9736, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 }
};

const sSpriteSheet mRecruitSpriteFrames_Disk_PC[1] = {
	{ 5904, eGFX_RECRUIT, 0, 0, 32, 22, 176, 0, 0 }
};

const sSpriteSheet mRecruitSpriteFrames_Disk_Part_PC[1] = {
	{ 5904, eGFX_RECRUIT, 0, 0, 16, 16, 176, 0, 0 }
};

const sSpriteSheet mRecruitSpriteFrames_Disk_Load_PC[1] = {
	{ 10952, eGFX_RECRUIT, 0, 0, 16, 24, 176, 0, 0 }
};

const sSpriteSheet mRecruitSpriteFrames_Disk_Save_PC[2] =
{
	{ 10960, eGFX_RECRUIT, 0, 0, 16, 24, 176, 0, 0 },
	{ 10968, eGFX_RECRUIT, 0, 0, 16, 24, 176, 0, 0 }
};

sSpriteSheet stru_35ECE[5] =
{
	{ 5144, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5152, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5160, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5168, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5176, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_35F1E[5] =
{
	{ 7704, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7712, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7720, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7728, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7736, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_35F6E[5] =
{
	{ 10264, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10272, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10280, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10288, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10296, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_35FBE[5] =
{
	{ 12824, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12832, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12840, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12848, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12856, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3600E[5] =
{
	{ 15384, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15392, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15400, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15408, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15416, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3605E[5] =
{
	{ 17944, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 17952, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 17960, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 17968, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 17976, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_360AE[5] =
{
	{ 20504, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20512, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20520, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20528, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20536, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_360FE[5] =
{
	{ 23064, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23072, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23080, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23088, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23096, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3614E[5] =
{
	{ 25624, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 25632, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 25640, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 25648, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 25656, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3619E[3] =
{
	{ 41234, eGFX_HILL, 0, 0, 16, 13, 176, 0, 0 },
	{ 46034, eGFX_HILL, 0, 0, 32, 16, 176, 0, 0 },
	{ 51794, eGFX_HILL, 0, 0, 32, 32, 176, 0, 0 }
};

sSpriteSheet stru_361CE[3] =
{
	{ 5192, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5200, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5208, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_361FE[3] =
{
	{ 5216, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5224, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5232, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3622E[3] =
{
	{ 5240, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5248, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 5256, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3625E[3] =
{
	{ 7752, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7760, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7768, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3628E[3] =
{
	{ 7776, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7784, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7792, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_362BE[3] =
{
	{ 7800, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7808, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 7816, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_362EE[3] =
{
	{ 10312, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10320, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10328, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3631E[3] =
{
	{ 10336, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10344, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10352, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3634E[3] =
{
	{ 10360, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10368, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 10376, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3637E[3] =
{
	{ 12872, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12880, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12888, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_363AE[3] =
{
	{ 12896, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12904, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12912, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_363DE[3] =
{
	{ 12920, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12928, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 12936, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3640E[3] =
{
	{ 15432, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15440, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15448, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3643E[3] =
{
	{ 15456, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15464, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15472, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3646E[3] =
{
	{ 15480, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15488, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 15496, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3649E[3] =
{
	{ 17992, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 18000, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 18008, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_364CE[3] =
{
	{ 18016, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 18024, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 18032, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_364FE[3] =
{
	{ 18040, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 18048, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 18056, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3652E[3] =
{
	{ 20552, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20560, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20568, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3655E[3] =
{
	{ 20576, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20584, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20592, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3658E[3] =
{
	{ 20600, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20608, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 20616, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_365BE[3] =
{
	{ 23112, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23120, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23128, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_365EE[3] =
{
	{ 23136, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23144, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23152, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3661E[3] =
{
	{ 23160, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23168, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 },
	{ 23176, eGFX_HILL, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3664E[12] =
{
	{ 5856, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5864, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5872, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5880, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5888, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 5896, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9696, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9704, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9712, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9720, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9728, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 },
	{ 9736, eGFX_RECRUIT, 0, 0, 16, 18, 176, 0, 0 }
};

const sSpriteSheet* mSpriteSheetTypes_Recruit_PC[] = {
/* 0x00 */	mRecruitSpriteFrames_Graves_PC,
/* 0x01 */	stru_32420,							// Recruit Face Camera
/* 0x02 */	stru_32440,							// Recruit Face Camera / Left
/* 0x03 */	stru_32460,							// Recruit Face Left
/* 0x04 */	stru_32480,							// Recruit Face Camera        (Color2)
/* 0x05 */	stru_324A0,							// Recruit Face Camera / Left (Color2)
/* 0x06 */	stru_324C0,							// Recruit Face Left          (Color2)
/* 0x07 */	stru_324E0,							// Recruit Face Camera        (Color3)
/* 0x08 */	stru_32500,							// Recruit Face Camera / Left (Color3)
/* 0x09 */	stru_32520,							// Recruit Face Left          (Color3)
/* 0x0A */	stru_32540,							// Recruit Face Camera        (Color4)
/* 0x0B */	stru_32560,							// Recruit Face Camera / Left (Color4)
/* 0x0C */	stru_32580,							// Recruit Face Left          (Color4)
/* 0x0D */	mRecruitSpriteFrames_AlphaNum_PC,	// Letters/Numbers
/* 0x0E */	stru_327E0,							// Colon (:)
/* 0x0F */	stru_327F0,							// Cursor
/* 0x10 */	stru_32800,							// 16: Unknown - Blank?
/* 0x11 */	stru_32810,							// 
/* 0x12 */	stru_32980,							// Symbol ^
/* 0x13 */	stru_32990,							// Unknown ___
/* 0x14 */	stru_329A0,							// 20: Hill Overlays
/* 0x15 */	mRecruitSpriteFrames_Truck_PC,
/* 0x16 */	mRecruitSpriteFrames_Disk_PC,
/* 0x17 */	mRecruitSpriteFrames_Disk_Part_PC,
/* 0x18 */	mRecruitSpriteFrames_Disk_Load_PC,
/* 0x19 */	mRecruitSpriteFrames_Disk_Save_PC
};

const sSpriteSheet* mSpriteSheetTypes_Hill_PC[] = {
	stru_35ECE,
	stru_35F1E,
	stru_35F6E,
	stru_35FBE,
	stru_3600E,
	stru_3605E,
	stru_360AE,
	stru_360FE,
	stru_3614E,
	stru_3619E,	// Hill Pieces
	stru_361CE,
	stru_361FE,
	stru_3622E,
	stru_3625E,
	stru_3628E,
	stru_362BE,
	stru_362EE,
	stru_3631E,
	stru_3634E,
	stru_3637E,
	stru_363AE,
	stru_363DE,
	stru_3640E,
	stru_3643E,
	stru_3646E,
	stru_3649E,
	stru_364CE,
	stru_364FE,
	stru_3652E,
	stru_3655E,
	stru_3658E,
	stru_365BE,
	stru_365EE,
	stru_3661E,
	stru_3664E
};

sSpriteSheet stru_42928[37] =
{
	{ 25672, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25680, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25688, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25696, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25704, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25712, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25720, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25728, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25736, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25744, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 25752, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26632, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26640, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26648, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26656, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26664, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26672, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26680, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26688, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26696, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26704, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 26712, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27592, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27600, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27608, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27616, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27624, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27632, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27640, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27648, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27656, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27664, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 27672, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 28552, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 28560, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 28568, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 },
	{ 28576, eGFX_PSTUFF, 0, 0, 16, 6, 240, 0, 0 }
};

sSpriteSheet stru_42B78[38] =
{
	{ 32000, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32008, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32016, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32024, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32032, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32040, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32048, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32056, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32064, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32072, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32080, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32088, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32096, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32104, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32112, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32120, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32128, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32136, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32144, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 32152, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35200, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35208, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35216, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35224, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35232, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35240, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35256, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35264, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35272, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35280, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35288, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35296, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35304, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35312, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35320, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35328, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35344, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 },
	{ 35248, eGFX_PSTUFF, 0, 0, 16, 20, 240, 0, 0 }
};

sSpriteSheet stru_42DD8[1] = {
	{ 32160, eGFX_PSTUFF, 0, 0, 16, 1, 240, 0, 0 }
};

sSpriteSheet stru_42DE8[38] =
{
	{ 32320, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32328, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32336, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32344, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32352, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32360, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32368, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32376, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32384, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32392, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32400, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32408, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32416, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32424, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32432, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32440, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32448, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32456, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32464, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 32472, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35520, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35528, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35536, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35544, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35552, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35560, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35576, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35584, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35592, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35600, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35608, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35616, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35624, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35632, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35640, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35648, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 },
	{ 35344, eGFX_PSTUFF, 0, 0, 15, 20, 240, 0, 0 },
	{ 35568, eGFX_PSTUFF, 0, 0, 16, 16, 240, 0, 0 }
};

const sSpriteSheet* mSpriteSheetTypes_Briefing_PC[] = {
	stru_42928,
	stru_42B78,
	stru_42DD8,
	stru_42DE8
};

sSpriteSheet stru_32FAC[3] =
{
	{ 0, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 8, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 16, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_32FDC[3] =
{
	{ 24, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 32, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3300C[3] =
{
	{ 48, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 56, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 64, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3303C[3] =
{
	{ 72, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 80, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 88, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3306C[3] =
{
	{ 96, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 104, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 112, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3309C[3] =
{
	{ 120, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 128, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 136, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_330CC[3] =
{
	{ 144, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 152, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2240, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_330FC[3] =
{
	{ 2248, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2256, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2264, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3312C[3] =
{
	{ 2272, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2280, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2288, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3315C[3] =
{
	{ 2296, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2304, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2312, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3318C[3] =
{
	{ 2320, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2328, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2336, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_331BC[3] =
{
	{ 2344, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2352, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2360, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_331EC[3] =
{
	{ 2368, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2376, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 2384, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3321C[3] =
{
	{ 2392, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4480, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4488, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3324C[3] =
{
	{ 4496, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4504, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4512, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3327C[3] =
{
	{ 4520, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4528, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4536, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_332AC[] = {
	{ 4544, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_332BC[] = {
	{ 4552, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_332CC[] = {
	{ 4560, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_332DC[] = {
	{ 4568, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_332EC[] = {
	{ 4576, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_332FC[] = {
	{ 4584, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3330C[] = {
	{ 4592, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3331C[] = {
	{ 4600, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3332C[2] =
{
	{ 4608, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4616, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3334C[2] =
{
	{ 4624, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 4632, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3336C[2] =
{
	{ 6720, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6728, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3338C[2] =
{
	{ 6736, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6744, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_333AC[2] =
{
	{ 6752, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6760, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_333CC[2] =
{
	{ 6768, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6776, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_333EC[2] =
{
	{ 6784, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6792, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3340C[2] =
{
	{ 6800, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6808, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3342C[] = {
	{ 6816, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3343C[] = {
	{ 6824, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3344C[2] =
{
	{ 6832, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6840, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3346C[2] =
{
	{ 6848, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6856, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3348C[2] =
{
	{ 6864, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 6872, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_334AC[2] =
{
	{ 8960, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 8968, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_334CC[2] =
{
	{ 8976, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 8984, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_334EC[2] =
{
	{ 8992, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9000, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3350C[2] =
{
	{ 9008, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9016, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3352C[2] =
{
	{ 9024, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9032, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3354C[2] =
{
	{ 9040, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9048, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3356C[2] =
{
	{ 9056, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9064, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3358C[2] =
{
	{ 9072, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9080, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_335AC[2] =
{
	{ 9088, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9096, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_335CC[2] =
{
	{ 9104, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 9112, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_335EC[2] =
{
	{ 11200, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11208, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3360C[2] =
{
	{ 11216, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11224, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3362C[3] =
{
	{ 11232, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11240, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11248, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3365C[3] =
{
	{ 11256, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11264, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11272, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3368C[3] =
{
	{ 11280, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11288, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11296, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_336BC[3] =
{
	{ 11304, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11312, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11320, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_336EC[3] =
{
	{ 11328, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11336, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11344, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3371C[3] =
{
	{ 11352, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13440, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13448, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3374C[3] =
{
	{ 13456, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13464, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13472, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3377C[3] =
{
	{ 13480, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13488, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13496, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_337AC[6] =
{
	{ 13504, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13512, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13520, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13528, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13536, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13544, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3380C[3] =
{
	{ 15712, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15720, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15728, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3383C[3] =
{
	{ 15736, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15744, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15752, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3386C[3] =
{
	{ 15760, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15768, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15776, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3389C[3] =
{
	{ 15784, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15792, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15800, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_338CC[3] =
{
	{ 15808, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15816, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15824, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_338FC[4] =
{
	{ 15832, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17920, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17928, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17936, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3393C[3] =
{
	{ 17936, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17944, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17952, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3396C[3] =
{
	{ 17960, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17968, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17976, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3399C[6] =
{
	{ 13552, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13560, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13568, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13576, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13584, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13592, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_339FC[3] =
{
	{ 22400, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22408, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22416, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33A2C[3] =
{
	{ 22424, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22432, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22440, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33A5C[3] =
{
	{ 22448, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22456, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22464, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33A8C[3] =
{
	{ 22472, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22480, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22488, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33ABC[3] =
{
	{ 22496, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22504, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22512, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33AEC[3] =
{
	{ 22520, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22528, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22536, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33B1C[3] =
{
	{ 22544, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 22552, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24640, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33B4C[3] =
{
	{ 24648, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24656, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24664, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33B7C[3] =
{
	{ 24672, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24680, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24688, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33BAC[3] =
{
	{ 24696, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24704, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24712, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33BDC[3] =
{
	{ 24720, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24728, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24736, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33C0C[3] =
{
	{ 24744, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24752, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24760, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33C3C[3] =
{
	{ 24768, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24776, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 24784, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33C6C[3] =
{
	{ 24792, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 26880, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 26888, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33C9C[3] =
{
	{ 26896, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 26904, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 26912, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33CCC[3] =
{
	{ 26920, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 26928, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 26936, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33CFC[] = {
	{ 26944, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D0C[] = {
	{ 26952, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D1C[] = {
	{ 26960, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D2C[] = {
	{ 26968, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D3C[] = {
	{ 26976, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D4C[] = {
	{ 26984, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D5C[] = {
	{ 26992, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D6C[] = {
	{ 27000, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D7C[2] =
{
	{ 27008, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 27016, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33D9C[2] =
{
	{ 27024, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 27032, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33DBC[2] =
{
	{ 29120, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29128, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33DDC[2] =
{
	{ 29136, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29144, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33DFC[2] =
{
	{ 29152, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29160, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33E1C[2] =
{
	{ 29168, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29176, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33E3C[2] =
{
	{ 29184, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29192, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33E5C[2] =
{
	{ 29200, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29208, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33E7C[2] =
{
	{ 29216, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29224, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33E9C[2] =
{
	{ 29232, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29240, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33EBC[2] =
{
	{ 29248, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29256, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33EDC[2] =
{
	{ 29264, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 29272, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33EFC[2] =
{
	{ 31360, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31368, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33F1C[2] =
{
	{ 31376, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31384, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33F3C[2] =
{
	{ 31392, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31400, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33F5C[3] =
{
	{ 31408, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31416, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31424, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33F8C[2] =
{
	{ 31424, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31432, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33FAC[2] =
{
	{ 31440, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31448, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33FCC[2] =
{
	{ 31456, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31464, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_33FEC[2] =
{
	{ 31472, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31480, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3400C[2] =
{
	{ 31488, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31496, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3402C[2] =
{
	{ 31504, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 31512, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3404C[2] =
{
	{ 33600, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33608, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3406C[2] =
{
	{ 33616, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33624, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3408C[3] =
{
	{ 33632, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33640, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33648, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_340BC[3] =
{
	{ 33656, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33664, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33672, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_340EC[3] =
{
	{ 33680, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33688, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33696, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3411C[3] =
{
	{ 33704, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33712, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33720, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3414C[3] =
{
	{ 33728, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33736, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33744, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3417C[3] =
{
	{ 33752, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35840, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35848, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_341AC[3] =
{
	{ 35856, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35864, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35872, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_341DC[3] =
{
	{ 35880, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35888, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35896, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3420C[6] =
{
	{ 35904, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35912, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35920, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35928, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35936, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35944, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3426C[8] =
{
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 },
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 },
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 },
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 },
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 },
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 },
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 },
	{ 2240, eGFX_IN_GAME2, 0, 0, 16, 1, 176, 0, 0 }
};

sSpriteSheet stru_342EC[4] =
{
	{ 7936, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 7944, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 7952, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 7960, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 }
};

sSpriteSheet stru_3432C[4] =
{
	{ 7968, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 7976, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 7984, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 7992, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 }
};

sSpriteSheet stru_3436C[8] =
{
	{ 8896, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 8904, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 8912, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 8920, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 9856, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 9864, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 9872, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 9880, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 }
};

sSpriteSheet stru_343EC[16] =
{
	{ 13440, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13456, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13472, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13488, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13504, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13520, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13536, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13872, eGFX_IN_GAME2, 0, 0, 32, 30, 208, 0, 2 },
	{ 13568, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 13584, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 18560, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 18576, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 18592, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 18608, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 18624, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 },
	{ 18640, eGFX_IN_GAME2, 0, 0, 32, 32, 208, 0, 0 }
};

sSpriteSheet stru_344EC[4] =
{
	{ 18656, eGFX_IN_GAME2, 0, 0, 32, 16, 176, 0, 0 },
	{ 18672, eGFX_IN_GAME2, 0, 0, 32, 16, 176, 0, 0 },
	{ 18688, eGFX_IN_GAME2, 0, 0, 32, 16, 176, 0, 0 },
	{ 18704, eGFX_IN_GAME2, 0, 0, 32, 16, 176, 0, 0 }
};

sSpriteSheet stru_3452C[3] =
{
	{ 21216, eGFX_IN_GAME2, 0, 0, 32, 11, 176, 0, 0 },
	{ 21232, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 22352, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 }
};

sSpriteSheet stru_3455C[7] =
{
	{ 0, eGFX_IN_GAME2, 0, 0, 16, 15, 192, 0, 0 },
	{ 8, eGFX_IN_GAME2, 0, 0, 32, 25, 192, 0, 0 },
	{ 120, eGFX_IN_GAME2, 0, 0, 32, 28, 192, 0, 0 },
	{ 136, eGFX_IN_GAME2, 0, 0, 48, 38, 192, 0, 0 },
	{ 4000, eGFX_IN_GAME2, 0, 0, 48, 43, 192, 0, 0 },
	{ 5464, eGFX_IN_GAME2, 0, 0, 48, 44, 192, 0, 0 },
	{ 6768, eGFX_IN_GAME2, 0, 0, 48, 42, 192, 0, 0 }
};

sSpriteSheet stru_345CC[] = {
	{ 7752, eGFX_IN_GAME2, 0, 0, 32, 16, 176, 0, 0 }
};

sSpriteSheet stru_345DC[] = {
	{ 7768, eGFX_IN_GAME2, 0, 0, 16, 28, 176, 0, 0 }
};
sSpriteSheet stru_345EC[] = {
	{ 10976, eGFX_IN_GAME2, 0, 0, 32, 12, 144, 0, 0 }
};

sSpriteSheet stru_345FC[] = {
	{ 10880, eGFX_IN_GAME2, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_3460C[] = {
	{ 10888, eGFX_IN_GAME2, 0, 0, 32, 15, 176, 0, 0 }
};

sSpriteSheet stru_3461C[3] =
{
	{ 23840, eGFX_IN_GAME2, 0, 0, 16, 30, 176, 0, 0 },
	{ 23848, eGFX_IN_GAME2, 0, 0, 16, 30, 176, 0, 0 },
	{ 23856, eGFX_IN_GAME2, 0, 0, 16, 30, 176, 0, 0 }
};

sSpriteSheet stru_3464C[16] =
{
	{ 36160, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36168, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36176, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36184, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36192, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36200, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36208, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36216, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36224, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36232, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36240, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36248, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36256, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36264, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36272, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 36280, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 }
};

sSpriteSheet stru_3474C[4] =
{
	{ 9088, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 9096, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 9104, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 9112, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 }
};

sSpriteSheet stru_3478C[2] =
{
	{ 10992, eGFX_IN_GAME2, 0, 0, 16, 3, 176, 0, 0 },
	{ 11000, eGFX_IN_GAME2, 0, 0, 16, 4, 176, 0, 0 }
};

sSpriteSheet stru_347AC[2] =
{
	{ 23864, eGFX_IN_GAME2, 0, 0, 16, 13, 176, 0, 0 },
	{ 23872, eGFX_IN_GAME2, 0, 0, 16, 13, 176, 0, 0 }
};

sSpriteSheet stru_347CC[] = {
	{ 23880, eGFX_IN_GAME2, 0, 0, 16, 15, 176, 0, 0 }
};
sSpriteSheet stru_347DC[4] =
{
	{ 25176, eGFX_IN_GAME2, 0, 0, 16, 11, 176, 0, 0 },
	{ 25184, eGFX_IN_GAME2, 0, 0, 16, 11, 176, 0, 0 },
	{ 25192, eGFX_IN_GAME2, 0, 0, 16, 11, 176, 0, 0 },
	{ 25200, eGFX_IN_GAME2, 0, 0, 16, 11, 176, 0, 0 }
};

sSpriteSheet stru_3481C[] = {
	{ 23888, eGFX_IN_GAME2, 0, 0, 16, 15, 176, 0, 0 }
};

sSpriteSheet stru_3482C[4] =
{
	{ 23896, eGFX_IN_GAME2, 0, 0, 16, 8, 176, 0, 0 },
	{ 23904, eGFX_IN_GAME2, 0, 0, 16, 8, 176, 0, 0 },
	{ 23912, eGFX_IN_GAME2, 0, 0, 16, 8, 176, 0, 0 },
	{ 23920, eGFX_IN_GAME2, 0, 0, 16, 8, 176, 0, 0 }
};

sSpriteSheet stru_3486C[8] =
{
	{ 11328, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 11336, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 11344, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 11352, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 12448, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 12456, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 12464, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 },
	{ 12472, eGFX_IN_GAME2, 0, 0, 16, 7, 176, 0, 0 }
};

sSpriteSheet stru_348EC[2] =
{
	{ 26264, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 },
	{ 26272, eGFX_IN_GAME2, 0, 0, 16, 10, 176, 0, 0 }
};

sSpriteSheet stru_3490C[4] =
{
	{ 15680, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15688, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15696, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 15704, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3494C[] = {
	{ 31840, eGFX_IN_GAME2, 0, 0, 128, 20, 192, 0, 0 }
};

sSpriteSheet stru_3495C[] = {
	{ 28216, eGFX_IN_GAME2, 0, 0, 80, 20, 192, 0, 0 }
};

sSpriteSheet stru_3496C[] = {
	{ 28640, eGFX_IN_GAME2, 0, 0, 112, 20, 192, 0, 0 }
};

sSpriteSheet stru_3497C[16] =
{
	{ 21080, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21088, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21096, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21104, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21112, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21880, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21888, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21896, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21904, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 21912, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 22680, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 22688, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 22696, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 22704, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 22712, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 },
	{ 23480, eGFX_IN_GAME2, 0, 0, 16, 5, 176, 0, 0 }
};

sSpriteSheet stru_34A7C[8] =
{
	{ 17984, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 17992, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 18000, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 18008, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 18016, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 18024, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 18032, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 18040, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34AFC[16] =
{
	{ 26992, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 49680, eGFX_IN_GAME2, 0, 0, 32, 27, 224, 0, 0 },
	{ 27008, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 45840, eGFX_IN_GAME2, 0, 0, 32, 24, 224, 0, 0 },
	{ 27024, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 45856, eGFX_IN_GAME2, 0, 0, 32, 23, 224, 0, 0 },
	{ 31440, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 45872, eGFX_IN_GAME2, 0, 0, 32, 25, 224, 0, 0 },
	{ 31456, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 45888, eGFX_IN_GAME2, 0, 0, 32, 25, 224, 0, 0 },
	{ 31472, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 45904, eGFX_IN_GAME2, 0, 0, 32, 23, 224, 0, 0 },
	{ 31488, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 49872, eGFX_IN_GAME2, 0, 0, 32, 25, 224, 0, 0 },
	{ 31504, eGFX_IN_GAME2, 0, 0, 32, 28, 224, 0, 0 },
	{ 49536, eGFX_IN_GAME2, 0, 0, 32, 27, 224, 0, 0 }
};

sSpriteSheet stru_34BFC[8] =
{
	{ 40384, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40392, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40400, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40408, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40416, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40424, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40432, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40440, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34C7C[3] =
{
	{ 38112, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38120, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38128, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34CAC[3] =
{
	{ 38136, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38144, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38152, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34CDC[3] =
{
	{ 38160, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38168, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38176, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34D0C[3] =
{
	{ 38184, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38192, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38200, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34D3C[3] =
{
	{ 38208, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38216, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38224, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34D6C[4] =
{
	{ 38232, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40320, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40328, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40336, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34DAC[3] =
{
	{ 40336, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40344, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40352, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34DDC[3] =
{
	{ 40360, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40368, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40376, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_34E0C[] = {
	{ 11232, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E1C[] = {
	{ 11240, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E2C[] = {
	{ 11248, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E3C[] = {
	{ 11256, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E4C[] = {
	{ 11264, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E5C[] = {
	{ 11272, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E6C[] = {
	{ 11280, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E7C[] = {
	{ 11288, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E8C[] = {
	{ 33632, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34E9C[] = {
	{ 33640, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34EAC[] = {
	{ 33648, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34EBC[] = {
	{ 33656, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34ECC[] = {
	{ 33664, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34EDC[] = {
	{ 33672, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34EEC[] = {
	{ 33680, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34EFC[] = {
	{ 33688, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};
sSpriteSheet stru_34F0C[4] =
{
	{ 24, eGFX_IN_GAME2, 0, 0, 48, 34, 192, 0, 0 },
	{ 48, eGFX_IN_GAME2, 0, 0, 48, 42, 192, 0, 0 },
	{ 72, eGFX_IN_GAME2, 0, 0, 48, 48, 192, 0, 0 },
	{ 96, eGFX_IN_GAME2, 0, 0, 48, 48, 192, 0, 0 }
};

sSpriteSheet stru_34F4C[] = {
	{ 24064, eGFX_IN_GAME2, 0, 0, 96, 25, 192, 0, 0 }
};

sSpriteSheet stru_34F5C[] = {
	{ 10312, eGFX_IN_GAME2, 0, 0, 16, 13, 176, 0, 0 }
};

sSpriteSheet stru_34F6C[] = {
	{ 10320, eGFX_IN_GAME2, 0, 0, 16, 13, 176, 0, 0 }
};

sSpriteSheet stru_34F7C[4] =
{
	{ 23648, eGFX_IN_GAME2, 0, 0, 16, 4, 176, 0, 0 },
	{ 23656, eGFX_IN_GAME2, 0, 0, 16, 4, 176, 0, 0 },
	{ 23664, eGFX_IN_GAME2, 0, 0, 16, 4, 176, 0, 0 },
	{ 23672, eGFX_IN_GAME2, 0, 0, 16, 4, 176, 0, 0 }
};

sSpriteSheet stru_34FBC[3] =
{
	{ 24280, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 25240, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 25248, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 }
};

sSpriteSheet stru_34FEC[4] =
{
	{ 24288, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 24296, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 24304, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 },
	{ 24312, eGFX_IN_GAME2, 0, 0, 16, 6, 176, 0, 0 }
};

sSpriteSheet stru_3502C[] = {
	{ 26272, eGFX_IN_GAME2, 0, 0, 16, 4, 176, 0, 0 }
};

sSpriteSheet stru_3503C[] = {
	{ 26280, eGFX_IN_GAME2, 0, 0, 16, 13, 176, 0, 0 }
};

sSpriteSheet stru_3504C[5] =
{
	{ 38080, eGFX_IN_GAME2, 0, 0, 16, 17, 176, 0, 0 },
	{ 38088, eGFX_IN_GAME2, 0, 0, 16, 17, 176, 0, 0 },
	{ 38096, eGFX_IN_GAME2, 0, 0, 16, 17, 176, 0, 0 },
	{ 38104, eGFX_IN_GAME2, 0, 0, 16, 17, 176, 0, 0 },
	{ 38112, eGFX_IN_GAME2, 0, 0, 16, 17, 176, 0, 0 }
};

sSpriteSheet stru_3509C[] = {
	{ 38120, eGFX_IN_GAME2, 0, 0, 64, 17, 192, 0, 0 }
};

sSpriteSheet SpriteSheet_Text_Again[] = {
	{ 38152, eGFX_IN_GAME2, 0, 0, 96, 17, 192, 10, 0 }
};
sSpriteSheet SpriteSheet_Text_Again_CF2[] = {
    { 38152, eGFX_IN_GAME2, 0, 0, 80, 17, 192, 10, 0 }
};

sSpriteSheet stru_350BC[5] =
{
	{ 20184, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20192, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20200, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20208, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20216, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3510C[4] =
{
	{ 18072, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20160, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20168, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20176, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3514C[7] =
{
	{ 20256, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20264, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20272, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20280, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20288, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20296, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20304, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_351BC[16] =
{
	{ 33696, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33704, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33712, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33720, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33728, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33736, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33744, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 33752, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35840, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35848, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35856, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35864, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35872, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35880, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35888, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35896, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_352BC[16] =
{
	{ 41072, eGFX_IN_GAME2, 0, 0, 32, 30, 208, 0, 0 },
	{ 45824, eGFX_IN_GAME2, 0, 0, 32, 31, 208, 0, 0 },
	{ 40960, eGFX_IN_GAME2, 0, 0, 32, 30, 208, 0, 0 },
	{ 49584, eGFX_IN_GAME2, 0, 0, 32, 23, 208, 0, -2 },
	{ 41776, eGFX_IN_GAME2, 0, 0, 32, 23, 208, 0, -2 },
	{ 45760, eGFX_IN_GAME2, 0, 0, 32, 23, 208, 0, -2 },
	{ 40992, eGFX_IN_GAME2, 0, 0, 32, 29, 208, 0, 0 },
	{ 45776, eGFX_IN_GAME2, 0, 0, 32, 31, 208, 0, 0 },
	{ 41008, eGFX_IN_GAME2, 0, 0, 32, 30, 208, 0, 0 },
	{ 45792, eGFX_IN_GAME2, 0, 0, 32, 31, 208, 0, 0 },
	{ 41024, eGFX_IN_GAME2, 0, 0, 32, 29, 208, 0, 0 },
	{ 49440, eGFX_IN_GAME2, 0, 0, 32, 23, 208, 0, -2 },
	{ 41840, eGFX_IN_GAME2, 0, 0, 32, 23, 208, 0, -2 },
	{ 49888, eGFX_IN_GAME2, 0, 0, 32, 23, 208, 0, -2 },
	{ 41056, eGFX_IN_GAME2, 0, 0, 32, 30, 208, 0, 0 },
	{ 45808, eGFX_IN_GAME2, 0, 0, 32, 31, 208, 0, 0 }
};

sSpriteSheet stru_353BC[16] =
{
	{ 28256, eGFX_IN_GAME2, 0, 0, 16, 19, 208, 0, 0 },
	{ 28264, eGFX_IN_GAME2, 0, 0, 16, 17, 208, -2, -2 },
	{ 39344, eGFX_IN_GAME2, 0, 0, 32, 15, 208, -13, -4 },
	{ 41888, eGFX_IN_GAME2, 0, 0, 32, 12, 208, -13, -7 },
	{ 43968, eGFX_IN_GAME2, 0, 0, 32, 12, 208, -13, -7 },
	{ 41904, eGFX_IN_GAME2, 0, 0, 32, 12, 208, -13, -7 },
	{ 36128, eGFX_IN_GAME2, 0, 0, 32, 15, 208, -10, -7 },
	{ 31904, eGFX_IN_GAME2, 0, 0, 16, 18, 208, -1, -7 },
	{ 31912, eGFX_IN_GAME2, 0, 0, 16, 19, 208, -1, -7 },
	{ 39008, eGFX_IN_GAME2, 0, 0, 16, 18, 208, -1, -7 },
	{ 5400, eGFX_IN_GAME2, 0, 0, 32, 15, 208, -5, -7 },
	{ 38744, eGFX_IN_GAME2, 0, 0, 32, 13, 208, -2, -7 },
	{ 43984, eGFX_IN_GAME2, 0, 0, 32, 12, 208, -6, -7 },
	{ 11952, eGFX_IN_GAME2, 0, 0, 32, 12, 208, -10, -7 },
	{ 36144, eGFX_IN_GAME2, 0, 0, 32, 14, 208, -6, -5 },
	{ 39016, eGFX_IN_GAME2, 0, 0, 16, 17, 208, -1, -2 }
};

sSpriteSheet stru_354BC[6] =
{
	{ 44832, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44840, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44848, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44856, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44864, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 1 },
	{ 44872, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 3 }
};

sSpriteSheet stru_3551C[6] =
{
	{ 44880, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44888, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44896, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44904, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44912, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 1 },
	{ 44920, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 3 }
};

sSpriteSheet stru_3557C[3] =
{
	{ 40448, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40456, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40464, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_355AC[10] =
{
	{ 11296, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11304, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11312, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11320, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11328, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11336, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11344, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 11352, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13440, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13448, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3564C[8] =
{
	{ 13448, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13456, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13464, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 13472, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20224, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20232, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20240, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 20248, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_356CC[24] =
{
	{ 44928, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44936, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44944, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44952, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47040, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47048, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47056, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47064, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47072, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47080, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47088, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47096, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47104, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47112, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47120, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47128, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47136, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47144, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47152, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47160, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47168, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47176, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47184, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 47192, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_3584C[24] =
{
	{ 49280, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49288, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49296, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49304, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49312, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49320, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49328, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49336, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49344, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49352, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49360, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49368, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49376, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49384, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49392, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49400, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49408, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49416, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49424, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49432, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51520, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51528, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51536, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51544, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_359CC[3] =
{
	{ 51552, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 49288, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51560, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_359FC[10] =
{
	{ 38080, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38088, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38096, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 38104, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51632, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51640, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51648, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51656, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51664, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51672, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35A9C[] = {
	{ 35960, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35AAC[] = {
	{ 50736, eGFX_IN_GAME2, 0, 0, 32, 20, 208, 0, 0 }
};

sSpriteSheet stru_35ABC[5] =
{
	{ 50752, eGFX_IN_GAME2, 0, 0, 16, 16, 176, 0, 0 },
	{ 50760, eGFX_IN_GAME2, 0, 0, 16, 16, 176, 0, 0 },
	{ 50768, eGFX_IN_GAME2, 0, 0, 16, 16, 176, 0, 0 },
	{ 50776, eGFX_IN_GAME2, 0, 0, 16, 16, 176, 0, 0 },
	{ 50784, eGFX_IN_GAME2, 0, 0, 16, 16, 176, 0, 0 }
};

sSpriteSheet stru_35B0C[6] =
{
	{ 51584, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51592, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51600, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51608, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51616, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51624, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35B6C[] = {
	{ 26288, eGFX_IN_GAME2, 0, 0, 16, 13, 176, 0, 0 }
};

sSpriteSheet stru_35B7C[4] =
{
	{ 51568, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 51576, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 40472, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35992, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35BBC[3] =
{
	{ 35968, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35976, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 35984, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35BEC[] = {
	{ 42624, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35BFC[] = {
	{ 42640, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35C0C[] = {
	{ 42648, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35C1C[] = {
	{ 42656, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

sSpriteSheet stru_35C2C[4] =
{
	{ 44800, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44808, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44816, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 },
	{ 44824, eGFX_IN_GAME, 0, 0, 16, 14, 160, 0, 0 }
};

// CF2 UFO
sSpriteSheet stru_1B108[] = {
    { 38192, eGFX_IN_GAME2, 0, 0, 32, 16, 176, 0, 0 }
};

const sSpriteSheet* mSpriteSheetTypes_InGame_PC[] = {
	/* 0x00 */ stru_32FAC,		// Soldier Walk		Face Forward
	/* 0x01 */ stru_32FDC,		// Soldier Walk		Face Forward + Left
	/* 0x02 */ stru_3300C,		// Soldier Walk		Face Left
	/* 0x03 */ stru_3303C,		// Soldier Walk		Face Left + Away
	/* 0x04 */ stru_3306C,		// Soldier Walk		Face Away
	/* 0x05 */ stru_3309C,		// Soldier Walk		Face Away + Right
	/* 0x06 */ stru_330CC,		// Soldier Walk		Face Right
	/* 0x07 */ stru_330FC,		// Soldier Walk		Face Right + Forward

	/* 0x08 */ stru_3312C,		// Soldier Throw	Face Forward
	/* 0x09 */ stru_3315C, 		// Soldier Throw	Face Forward + Left
	/* 0x0A */ stru_3318C, 		// Soldier Throw	Face Left
	/* 0x0B */ stru_331BC, 		// Soldier Throw	Face Left + Away
	/* 0x0C */ stru_331EC, 		// Soldier Throw	Face Away
	/* 0x0D */ stru_3321C, 		// Soldier Throw	Face Away + Right
	/* 0x0E */ stru_3324C, 		// Soldier Throw	Face Right
	/* 0x0F */ stru_3327C, 		// Soldier Throw	Face Right + Forward

	/* 0x10 */ stru_332AC,		// Soldier Prone	Face Forward
	/* 0x11 */ stru_332BC, 		// Soldier Prone	Face Forward + Left
	/* 0x12 */ stru_332CC, 		// Soldier Prone	Face Left
	/* 0x13 */ stru_332DC, 		// Soldier Prone	Face Left + Away
	/* 0x14 */ stru_332EC, 		// Soldier Prone	Face Away
	/* 0x15 */ stru_332FC, 		// Soldier Prone	Face Away + Right
	/* 0x16 */ stru_3330C, 		// Soldier Prone	Face Right
	/* 0x17 */ stru_3331C, 		// Soldier Prone	Face Right + Forward

	/* 0x18 */ stru_3332C,		// Soldier Swim		Face Forward
	/* 0x19 */ stru_3334C,		// Soldier Swim		Face Forward + Left
	/* 0x1A */ stru_3336C,		// Soldier Swim		Face Left
	/* 0x1B */ stru_3338C,		// Soldier Swim		Face Left + Away
	/* 0x1C */ stru_333AC,		// Soldier Swim		Face Away
	/* 0x1D */ stru_333CC,		// Soldier Swim		Face Away + Right
	/* 0x1E */ stru_333EC,		// Soldier Swim		Face Right
	/* 0x1F */ stru_3340C,		// Soldier Swim		Face Right + Forward

	/* 0x20 */ stru_3342C,		// Soldier Death	Face Forward
	/* 0x21 */ stru_3344C,		// Soldier Death	Face Forward + Left
	/* 0x22 */ stru_3346C,		// Soldier Death	Face Left
	/* 0x23 */ stru_3348C,		// Soldier Death	Face Left + Away
	/* 0x24 */ stru_334AC,		// Soldier Death	Face Away
	/* 0x25 */ stru_334CC,		// Soldier Death	Face Away + Right
	/* 0x26 */ stru_334EC,		// Soldier Death	Face Right
	/* 0x27 */ stru_3350C,		// Soldier Death	Face Right + Forward

	/* 0x28 */ stru_3352C,		// Soldier Death	Face Forward
	/* 0x29 */ stru_3354C,		// Soldier Death	Face Forward + Left
	/* 0x2A */ stru_3356C,		// Soldier Death	Face Left
	/* 0x2B */ stru_3358C,		// Soldier Death	Face Left + Away
	/* 0x2C */ stru_335AC,		// Soldier Death	Face Away
	/* 0x2D */ stru_335CC,		// Soldier Death	Face Away + Right
	/* 0x2E */ stru_335EC,		// Soldier Death	Face Right
	/* 0x2F */ stru_3360C,		// Soldier Death	Face Right + Forward

	/* 0x30 */ stru_3362C,		// Start Face Forward
	/* 0x31 */ stru_3365C,		// Start Face Left
	/* 0x32 */ stru_3368C,		// Start Right

	/* 0x33 */ stru_336BC,
	/* 0x34 */ stru_336EC,		// Bones
	/* 0x35 */ stru_3371C,
	/* 0x36 */ stru_3374C,
	/* 0x37 */ stru_3377C,
	/* 0x38 */ stru_337AC,

	/* 0x39 */ stru_3380C,		// Soldier Fired Rocket
	/* 0x3A */ stru_3383C,
	/* 0x3B */ stru_3386C,
	/* 0x3C */ stru_3389C,
	/* 0x3D */ stru_338CC,
	/* 0x3E */ stru_338FC,
	/* 0x3F */ stru_3393C,
	/* 0x40 */ stru_3396C,

	/* 0x41 */ stru_3399C,

	/* 0x42 */ stru_339FC,     // Enemy Soldier Walk Face Forward
	/* 0x43 */ stru_33A2C,     // Enemy Soldier Walk Face Forward + Left
	/* 0x44 */ stru_33A5C,     // Enemy Soldier Walk Face Left
	/* 0x45 */ stru_33A8C,     // Enemy Soldier Walk Face Left + Away
	/* 0x46 */ stru_33ABC,     // Enemy Soldier Walk Face Away
	/* 0x47 */ stru_33AEC,     // Enemy Soldier Walk Face Away + Right
	/* 0x48 */ stru_33B1C,     // Enemy Soldier Walk Face Right
	/* 0x49 */ stru_33B4C,     // Enemy Soldier Walk Face Right + Forward

	/* 0x4A */ stru_33B7C,     // Enemy Soldier Throw Face Forward
	/* 0x4B */ stru_33BAC,     // Enemy Soldier Throw Face Forward + Left          
	/* 0x4C */ stru_33BDC,     // Enemy Soldier Throw Face Left          
	/* 0x4D */ stru_33C0C,     // Enemy Soldier Throw Face Left + Away          
	/* 0x4E */ stru_33C3C,     // Enemy Soldier Throw Face Away          
	/* 0x4F */ stru_33C6C,     // Enemy Soldier Throw Face Away + Right          
                                                      
	/* 0x50 */ stru_33C9C,     // Enemy Soldier Throw Face Right                      
    /* 0x51 */ stru_33CCC,     // Enemy Soldier Throw Face Right + Forward        
	/* 0x52 */ stru_33CFC,     // Enemy Soldier Prone Face Forward
    /* 0x53 */ stru_33D0C,     // Enemy Soldier Prone Face Forward + Left
    /* 0x54 */ stru_33D1C,     // Enemy Soldier Prone Face Left
    /* 0x55 */ stru_33D2C,     // Enemy Soldier Prone Face Left + Away
    /* 0x56 */ stru_33D3C,     // Enemy Soldier Prone Face Away
    /* 0x57 */ stru_33D4C,     // Enemy Soldier Prone Face Away + Right
    /* 0x58 */ stru_33D5C,     // Enemy Soldier Prone Face Right
    /* 0x59 */ stru_33D6C,     // Enemy Soldier Prone Face Right + Forward

	/* 0x5A */ stru_33D7C,     // Enemy Soldier Swim Face Forward
	/* 0x5B */ stru_33D9C,     // Enemy Soldier Swim Face Forward + Left
	/* 0x5C */ stru_33DBC,     // Enemy Soldier Swim Face Left
	/* 0x5D */ stru_33DDC,     // Enemy Soldier Swim Face Left + Away
	/* 0x5E */ stru_33DFC,     // Enemy Soldier Swim Face Away
	/* 0x5F */ stru_33E1C,     // Enemy Soldier Swim Face Away + Right

	/* 0x60 */ stru_33E3C,     // Enemy Soldier Swim Face Right
	/* 0x61 */ stru_33E5C,     // Enemy Soldier Swim Face Right + Forward

	/* 0x62 */ stru_33E7C,     // Enemy Soldier Death	Face Forward
	/* 0x63 */ stru_33E9C,     // Enemy Soldier Death	Face Forward + Left
	/* 0x64 */ stru_33EBC,     // Enemy Soldier Death	Face Left
	/* 0x65 */ stru_33EDC,     // Enemy Soldier Death	Face Left + Away
	/* 0x66 */ stru_33EFC,     // Enemy Soldier Death	Face Away
	/* 0x67 */ stru_33F1C,     // Enemy Soldier Death	Face Away + Right
	/* 0x68 */ stru_33F3C,     // Enemy Soldier Death	Face Right
	/* 0x69 */ stru_33F5C,     // Enemy Soldier Death	Face Right + Forward 

	/* 0x6A */ stru_33F8C,     // Enemy Soldier Death-2 Face Forward 
	/* 0x6B */ stru_33FAC,     // Enemy Soldier Death-2 Face Forward + Left 
	/* 0x6C */ stru_33FCC,     // Enemy Soldier Death-2 Face Left 
	/* 0x6D */ stru_33FEC,     // Enemy Soldier Death-2 Face Left + Away 
	/* 0x6E */ stru_3400C,     // Enemy Soldier Death-2 Face Away 
	/* 0x6F */ stru_3402C,     // Enemy Soldier Death-2 Face Away + Right                    
	/* 0x70 */ stru_3404C,     // Enemy Soldier Death-2 Face Right
	/* 0x71 */ stru_3406C,     // Enemy Soldier Death-2 Face Right + Forward 

	/* 0x72 */ stru_3408C,     // Enemy Soldier Still Face Forward 
	/* 0x73 */ stru_340BC,     // Enemy Soldier Still Face Forward + Left 
	/* 0x74 */ stru_340EC,     // Enemy Soldier Still Face Left 
	/* 0x75 */ stru_3411C,     // Enemy Soldier Still Face Left + Away 
	/* 0x76 */ stru_3414C,     // Enemy Soldier Still Face Away 
	/* 0x77 */ stru_3417C,     // Enemy Soldier Still Face Away + Right     
	/* 0x78 */ stru_341AC,     // Enemy Soldier Still Face Right
	/* 0x79 */ stru_341DC,     // Enemy Soldier Still Face Right + Forward 

	/* 0x7A */ stru_3420C,     // Soldier Salute
	/* 0x7B */ stru_3426C,     // 
	/* 0x7C */ stru_3426C,     // 
	/* 0x7D */ stru_342EC,
	/* 0x7E */ stru_3432C,
	/* 0x7F */ stru_3436C,

	/* 0x80 */ stru_343EC,
	/* 0x81 */ stru_343EC,
	/* 0x82 */ stru_343EC,
	/* 0x83 */ stru_343EC,
	/* 0x84 */ stru_343EC,
	/* 0x85 */ stru_343EC,
	/* 0x86 */ stru_343EC,
	/* 0x87 */ stru_343EC,
	/* 0x88 */ stru_343EC,
	/* 0x89 */ stru_343EC,
	/* 0x8A */ stru_343EC,
	/* 0x8B */ stru_343EC,
	/* 0x8C */ stru_344EC,
	/* 0x8D */ stru_3452C,
	/* 0x8E */ stru_3455C,
	/* 0x8F */ stru_345CC,

	/* 0x90 */ stru_345DC,
	/* 0x91 */ stru_345EC,
	/* 0x92 */ stru_345FC,
	/* 0x93 */ stru_3460C,
	/* 0x94 */ stru_3461C,
	/* 0x95 */ stru_3464C,
	/* 0x96 */ stru_3474C,
	/* 0x97 */ stru_3478C,
	/* 0x98 */ stru_347AC,
	/* 0x99 */ stru_347CC,
	/* 0x9A */ stru_347DC,
	/* 0x9B */ stru_3481C,
	/* 0x9C */ stru_3482C,
	/* 0x9D */ stru_3486C,
	/* 0x9E */ stru_348EC,
	/* 0x9F */ stru_3490C,

	/* 0xA0 */ stru_3494C,
	/* 0xA1 */ stru_3495C,
	/* 0xA2 */ stru_3496C,
	/* 0xA3 */ stru_3497C,
	/* 0xA4 */ stru_34A7C,
	/* 0xA5 */ stru_34AFC,
	/* 0xA6 */ stru_34BFC,
	/* 0xA7 */ stru_34BFC,
	/* 0xA8 */ stru_34C7C,
	/* 0xA9 */ stru_34CAC,
	/* 0xAA */ stru_34CDC,
	/* 0xAB */ stru_34D0C,
	/* 0xAC */ stru_34D3C,
	/* 0xAD */ stru_34D6C,
	/* 0xAE */ stru_34DAC,
	/* 0xAF */ stru_34DDC,

	/* 0xB0 */ stru_34E0C,
	/* 0xB1 */ stru_34E1C,
	/* 0xB2 */ stru_34E2C,
	/* 0xB3 */ stru_34E3C,
	/* 0xB4 */ stru_34E4C,
	/* 0xB5 */ stru_34E5C,
	/* 0xB6 */ stru_34E6C,
	/* 0xB7 */ stru_34E7C,
	/* 0xB8 */ stru_34E8C,
	/* 0xB9 */ stru_34E9C,
	/* 0xBA */ stru_34EAC,
	/* 0xBB */ stru_34EBC,
	/* 0xBC */ stru_34ECC,
	/* 0xBD */ stru_34EDC,
	/* 0xBE */ stru_34EEC,
	/* 0xBF */ stru_34EFC,

	/* 0xC0 */ stru_34F0C,
	/* 0xC1 */ stru_34F4C,
	/* 0xC2 */ stru_34F5C,
	/* 0xC3 */ stru_34F6C,
	/* 0xC4 */ stru_34F7C,
	/* 0xC5 */ stru_34FBC,
	/* 0xC6 */ stru_34FEC,
	/* 0xC7 */ stru_3502C,
	/* 0xC8 */ stru_3503C,
	/* 0xC9 */ stru_3504C,
	/* 0xCA */ stru_3509C,
	/* 0xCB */ SpriteSheet_Text_Again,
	/* 0xCC */ stru_350BC,
	/* 0xCD */ stru_3510C,
	/* 0xCE */ stru_3514C,
	/* 0xCF */ stru_351BC,

	/* 0xD0 */ stru_351BC,
	/* 0xD1 */ stru_352BC,
	/* 0xD2 */ stru_353BC,
	/* 0xD3 */ stru_354BC,
	/* 0xD4 */ stru_3551C,
	/* 0xD5 */ stru_3557C,
	/* 0xD6 */ stru_355AC,
	/* 0xD7 */ stru_3564C,
	/* 0xD8 */ stru_356CC,
	/* 0xD9 */ stru_3584C,
	/* 0xDA */ stru_359CC,
	/* 0xDB */ stru_359FC,
	/* 0xDC */ stru_35A9C,
	/* 0xDD */ stru_35AAC,
	/* 0xDE */ stru_35ABC,
	/* 0xDF */ stru_35B0C,

	/* 0x00 */ stru_35B6C,
	/* 0x00 */ stru_35B7C,
	/* 0x00 */ stru_35BBC,
	/* 0x00 */ stru_35BEC,
	/* 0x00 */ stru_35BFC,
	/* 0x00 */ stru_35C0C,
	/* 0x00 */ stru_35C1C,
	/* 0x00 */ stru_35C2C
};

const sSpriteSheet* mSpriteSheetTypes_InGame_PC2[] = {
    /* 0x00 */ stru_32FAC,		// Soldier Walk		Face Forward
    /* 0x01 */ stru_32FDC,		// Soldier Walk		Face Forward + Left
    /* 0x02 */ stru_3300C,		// Soldier Walk		Face Left
    /* 0x03 */ stru_3303C,		// Soldier Walk		Face Left + Away
    /* 0x04 */ stru_3306C,		// Soldier Walk		Face Away
    /* 0x05 */ stru_3309C,		// Soldier Walk		Face Away + Right
    /* 0x06 */ stru_330CC,		// Soldier Walk		Face Right
    /* 0x07 */ stru_330FC,		// Soldier Walk		Face Right + Forward

    /* 0x08 */ stru_3312C,		// Soldier Throw	Face Forward
    /* 0x09 */ stru_3315C, 		// Soldier Throw	Face Forward + Left
    /* 0x0A */ stru_3318C, 		// Soldier Throw	Face Left
    /* 0x0B */ stru_331BC, 		// Soldier Throw	Face Left + Away
    /* 0x0C */ stru_331EC, 		// Soldier Throw	Face Away
    /* 0x0D */ stru_3321C, 		// Soldier Throw	Face Away + Right
    /* 0x0E */ stru_3324C, 		// Soldier Throw	Face Right
    /* 0x0F */ stru_3327C, 		// Soldier Throw	Face Right + Forward

    /* 0x10 */ stru_332AC,		// Soldier Prone	Face Forward
    /* 0x11 */ stru_332BC, 		// Soldier Prone	Face Forward + Left
    /* 0x12 */ stru_332CC, 		// Soldier Prone	Face Left
    /* 0x13 */ stru_332DC, 		// Soldier Prone	Face Left + Away
    /* 0x14 */ stru_332EC, 		// Soldier Prone	Face Away
    /* 0x15 */ stru_332FC, 		// Soldier Prone	Face Away + Right
    /* 0x16 */ stru_3330C, 		// Soldier Prone	Face Right
    /* 0x17 */ stru_3331C, 		// Soldier Prone	Face Right + Forward

    /* 0x18 */ stru_3332C,		// Soldier Swim		Face Forward
    /* 0x19 */ stru_3334C,		// Soldier Swim		Face Forward + Left
    /* 0x1A */ stru_3336C,		// Soldier Swim		Face Left
    /* 0x1B */ stru_3338C,		// Soldier Swim		Face Left + Away
    /* 0x1C */ stru_333AC,		// Soldier Swim		Face Away
    /* 0x1D */ stru_333CC,		// Soldier Swim		Face Away + Right
    /* 0x1E */ stru_333EC,		// Soldier Swim		Face Right
    /* 0x1F */ stru_3340C,		// Soldier Swim		Face Right + Forward

    /* 0x20 */ stru_3342C,		// Soldier Death	Face Forward
    /* 0x21 */ stru_3344C,		// Soldier Death	Face Forward + Left
    /* 0x22 */ stru_3346C,		// Soldier Death	Face Left
    /* 0x23 */ stru_3348C,		// Soldier Death	Face Left + Away
    /* 0x24 */ stru_334AC,		// Soldier Death	Face Away
    /* 0x25 */ stru_334CC,		// Soldier Death	Face Away + Right
    /* 0x26 */ stru_334EC,		// Soldier Death	Face Right
    /* 0x27 */ stru_3350C,		// Soldier Death	Face Right + Forward

    /* 0x28 */ stru_3352C,		// Soldier Death	Face Forward
    /* 0x29 */ stru_3354C,		// Soldier Death	Face Forward + Left
    /* 0x2A */ stru_3356C,		// Soldier Death	Face Left
    /* 0x2B */ stru_3358C,		// Soldier Death	Face Left + Away
    /* 0x2C */ stru_335AC,		// Soldier Death	Face Away
    /* 0x2D */ stru_335CC,		// Soldier Death	Face Away + Right
    /* 0x2E */ stru_335EC,		// Soldier Death	Face Right
    /* 0x2F */ stru_3360C,		// Soldier Death	Face Right + Forward

    /* 0x30 */ stru_3362C,		// Start Face Forward
    /* 0x31 */ stru_3365C,		// Start Face Left
    /* 0x32 */ stru_3368C,		// Start Right

    /* 0x33 */ stru_336BC,
    /* 0x34 */ stru_336EC,
    /* 0x35 */ stru_3371C,
    /* 0x36 */ stru_3374C,
    /* 0x37 */ stru_3377C,
    /* 0x38 */ stru_337AC,
    /* 0x39 */ stru_3380C,
    /* 0x3A */ stru_3383C,
    /* 0x3B */ stru_3386C,
    /* 0x3C */ stru_3389C,
    /* 0x3D */ stru_338CC,
    /* 0x3E */ stru_338FC,
    /* 0x3F */ stru_3393C,

    /* 0x40 */ stru_3396C,
    /* 0x41 */ stru_3399C,
    /* 0x42 */ stru_339FC,
    /* 0x43 */ stru_33A2C,
    /* 0x44 */ stru_33A5C,
    /* 0x45 */ stru_33A8C,
    /* 0x46 */ stru_33ABC,
    /* 0x47 */ stru_33AEC,
    /* 0x48 */ stru_33B1C,
    /* 0x49 */ stru_33B4C,
    /* 0x4A */ stru_33B7C,
    /* 0x4B */ stru_33BAC,
    /* 0x4C */ stru_33BDC,
    /* 0x4D */ stru_33C0C,
    /* 0x4E */ stru_33C3C,
    /* 0x4F */ stru_33C6C,

    /* 0x50 */ stru_33C9C,
    /* 0x51 */ stru_33CCC,
    /* 0x52 */ stru_33CFC,
    /* 0x53 */ stru_33D0C,
    /* 0x54 */ stru_33D1C,
    /* 0x55 */ stru_33D2C,
    /* 0x56 */ stru_33D3C,
    /* 0x57 */ stru_33D4C,
    /* 0x58 */ stru_33D5C,
    /* 0x59 */ stru_33D6C,
    /* 0x5A */ stru_33D7C,
    /* 0x5B */ stru_33D9C,
    /* 0x5C */ stru_33DBC,
    /* 0x5D */ stru_33DDC,
    /* 0x5E */ stru_33DFC,
    /* 0x5F */ stru_33E1C,

    /* 0x60 */ stru_33E3C,
    /* 0x61 */ stru_33E5C,
    /* 0x62 */ stru_33E7C,
    /* 0x63 */ stru_33E9C,
    /* 0x64 */ stru_33EBC,
    /* 0x65 */ stru_33EDC,
    /* 0x66 */ stru_33EFC,
    /* 0x67 */ stru_33F1C,
    /* 0x68 */ stru_33F3C,
    /* 0x69 */ stru_33F5C,
    /* 0x6A */ stru_33F8C,
    /* 0x6B */ stru_33FAC,
    /* 0x6C */ stru_33FCC,
    /* 0x6D */ stru_33FEC,
    /* 0x6E */ stru_3400C,
    /* 0x6F */ stru_3402C,

    /* 0x70 */ stru_3404C,
    /* 0x71 */ stru_3406C,
    /* 0x72 */ stru_3408C,
    /* 0x73 */ stru_340BC,
    /* 0x74 */ stru_340EC,
    /* 0x75 */ stru_3411C,
    /* 0x76 */ stru_3414C,
    /* 0x77 */ stru_3417C,
    /* 0x78 */ stru_341AC,
    /* 0x79 */ stru_341DC,
    /* 0x7A */ stru_3420C,
    /* 0x7B */ stru_3426C,
    /* 0x7C */ stru_3426C,
    /* 0x7D */ stru_342EC,
    /* 0x7E */ stru_3432C,
    /* 0x7F */ stru_3436C,

    /* 0x80 */ stru_343EC,
    /* 0x81 */ stru_343EC,
    /* 0x82 */ stru_343EC,
    /* 0x83 */ stru_343EC,
    /* 0x84 */ stru_343EC,
    /* 0x85 */ stru_343EC,
    /* 0x86 */ stru_343EC,
    /* 0x87 */ stru_343EC,
    /* 0x88 */ stru_343EC,
    /* 0x89 */ stru_343EC,
    /* 0x8A */ stru_343EC,
    /* 0x8B */ stru_343EC,
    /* 0x8C */ stru_344EC,
    /* 0x8D */ stru_3452C,
    /* 0x8E */ stru_3455C,
    /* 0x8F */ stru_345CC,

    /* 0x90 */ stru_345DC,
    /* 0x91 */ stru_345EC,
    /* 0x92 */ stru_345FC,
    /* 0x93 */ stru_3460C,
    /* 0x94 */ stru_3461C,
    /* 0x95 */ stru_3464C,
    /* 0x96 */ stru_3474C,
    /* 0x97 */ stru_3478C,
    /* 0x98 */ stru_347AC,
    /* 0x99 */ stru_347CC,
    /* 0x9A */ stru_347DC,
    /* 0x9B */ stru_3481C,
    /* 0x9C */ stru_3482C,
    /* 0x9D */ stru_3486C,
    /* 0x9E */ stru_348EC,
    /* 0x9F */ stru_3490C,

    /* 0xA0 */ stru_3494C,
    /* 0xA1 */ stru_3495C,
    /* 0xA2 */ stru_3496C,
    /* 0xA3 */ stru_3497C,
    /* 0xA4 */ stru_34A7C,
    /* 0xA5 */ stru_34AFC,
    /* 0xA6 */ stru_34BFC,
    /* 0xA7 */ stru_34BFC,
    /* 0xA8 */ stru_34C7C,
    /* 0xA9 */ stru_34CAC,
    /* 0xAA */ stru_34CDC,
    /* 0xAB */ stru_34D0C,
    /* 0xAC */ stru_34D3C,
    /* 0xAD */ stru_34D6C,
    /* 0xAE */ stru_34DAC,
    /* 0xAF */ stru_34DDC,

    /* 0xB0 */ stru_34E0C,
    /* 0xB1 */ stru_34E1C,
    /* 0xB2 */ stru_34E2C,
    /* 0xB3 */ stru_34E3C,
    /* 0xB4 */ stru_34E4C,
    /* 0xB5 */ stru_34E5C,
    /* 0xB6 */ stru_34E6C,
    /* 0xB7 */ stru_34E7C,
    /* 0xB8 */ stru_34E8C,
    /* 0xB9 */ stru_34E9C,
    /* 0xBA */ stru_34EAC,
    /* 0xBB */ stru_34EBC,
    /* 0xBC */ stru_34ECC,
    /* 0xBD */ stru_34EDC,
    /* 0xBE */ stru_34EEC,
    /* 0xBF */ stru_34EFC,

    /* 0xC0 */ stru_34F0C,
    /* 0xC1 */ stru_34F4C,
    /* 0xC2 */ stru_34F5C,
    /* 0xC3 */ stru_34F6C,
    /* 0xC4 */ stru_34F7C,
    /* 0xC5 */ stru_34FBC,
    /* 0xC6 */ stru_34FEC,
    /* 0xC7 */ stru_3502C,
    /* 0xC8 */ stru_3503C,
    /* 0xC9 */ stru_3504C,
    /* 0xCA */ stru_3509C,
    /* 0xCB */ SpriteSheet_Text_Again_CF2,
    /* 0xCC */ stru_350BC,
    /* 0xCD */ stru_3510C,
    /* 0xCE */ stru_3514C,
    /* 0xCF */ stru_351BC,

    /* 0xD0 */ stru_351BC,
    /* 0xD1 */ stru_352BC,
    /* 0xD2 */ stru_353BC,
    /* 0xD3 */ stru_354BC,
    /* 0xD4 */ stru_3551C,
    /* 0xD5 */ stru_3557C,
    /* 0xD6 */ stru_355AC,
    /* 0xD7 */ stru_3564C,
    /* 0xD8 */ stru_356CC,
    /* 0xD9 */ stru_3584C,
    /* 0xDA */ stru_359CC,
    /* 0xDB */ stru_359FC,
    /* 0xDC */ stru_35A9C,
    /* 0xDD */ stru_35AAC,
    /* 0xDE */ stru_35ABC,
    /* 0xDF */ stru_35B0C,

    /* 0x00 */ stru_35B6C,
    /* 0x00 */ stru_35B7C,
    /* 0x00 */ stru_35BBC,
    /* 0x00 */ stru_35BEC,
    /* 0x00 */ stru_35BFC,
    /* 0x00 */ stru_35C0C,
    /* 0x00 */ stru_35C1C,
    /* 0x00 */ stru_35C2C,
    /* 0x00 */ stru_1B108
};

/**
 * Service Screen
 */
sSpriteSheet stru_4398B[] = {
	{ 3000, eGFX_RANKFONT, 0, 0, 24, 22, 128, 0, 0 }
};

sSpriteSheet stru_4399B[] = {
	{ 3016, eGFX_RANKFONT, 0, 0, 24, 22, 64, 0, 0 }
};

sSpriteSheet stru_439AB[26] =
{
	{ 0, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 8, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 16, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 24, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 32, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 40, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 48, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 56, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 64, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 72, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 80, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 88, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 96, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 104, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 112, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 120, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 128, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 136, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 144, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 152, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 2880, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 2888, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 2896, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 2904, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 2912, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 },
	{ 2920, eGFX_RANKFONT, 0, 0, 16, 18, 64, 0, 0 }
};

sSpriteSheet stru_43B4B[26] =
{
	{ 6400, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6408, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6416, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6424, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6432, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6440, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6448, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6456, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6464, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6472, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6480, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6488, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6496, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6504, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6512, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6520, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6528, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6536, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6544, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 6552, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 8160, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 8168, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 8176, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 8184, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 8192, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 },
	{ 8200, eGFX_RANKFONT, 0, 0, 16, 11, 64, 0, 0 }
};

sSpriteSheet stru_43CEB[76] =
{
	{ 9920, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9928, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9936, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9944, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9952, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9960, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9968, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9976, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9984, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9992, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10000, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10008, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10016, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10024, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10032, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10040, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10048, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10056, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10064, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 10072, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12320, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12328, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12336, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12344, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12352, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12360, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14816, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14824, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14832, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14840, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14848, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14856, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14864, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14872, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 27096, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 27104, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9920, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9928, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9936, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 9944, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 0, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 8, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 16, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 24, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 32, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 40, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 48, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 56, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 64, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 72, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12368, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12376, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12384, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12392, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12400, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12408, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12416, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12424, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12432, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12440, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12448, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12456, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12464, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 12472, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14720, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14728, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14736, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14744, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14752, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14760, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14768, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14776, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14784, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14792, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14800, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 },
	{ 14808, eGFX_RANKFONT, 0, 0, 16, 15, 64, 0, 0 }
};

sSpriteSheet stru_441AB[] = {
	{ 34240, eGFX_RANKFONT, 0, 0, 216, 28, 64, 0, 0 }
};

sSpriteSheet stru_441BB[] = {
	{ 29760, eGFX_RANKFONT, 0, 0, 216, 28, 64, 0, 0 }
};

sSpriteSheet stru_441CB[] = {
	{ 0, eGFX_SERVICE, 0, 0, 80, 142, 0, 0, 0 }
};

sSpriteSheet stru_441DB[] = {
	{ 80, eGFX_SERVICE, 0, 0, 80, 142, 0, 0, 0 }
};

sSpriteSheet stru_441EB[] = {
	{ 2936, eGFX_RANKFONT, 0, 0, 128, 22, 64, 0, 0 }
};

const sSpriteSheet* mSpriteSheetTypes_Service_PC[] = {
	stru_4398B, // rank font
	stru_4399B,
	stru_439AB,
	stru_43B4B,
	stru_43CEB,
	stru_441AB,
	stru_441BB,
	stru_441CB,	// service
	stru_441DB,	// service
	stru_441EB
};
