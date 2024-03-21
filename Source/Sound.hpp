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

enum eSound_Effect {
	eSound_Effect_Grenade			= 0x0F,
	eSound_Effect_BuildingDoor2		= 0x10,
	eSound_Effect_DoorCloseDoor		= 0x29,
	eSound_Effect_Spear				= 0x2A,
	eSound_Effect_Vehicle_Sinking	= 0x2B,
	eSound_Effect_Turret_Fire		= 0x2C,
	eSound_Effect_Missile_Launch	= 0x2D,
	eSound_Effect_Rocket			= 0x2E,
};

struct sChunkPlaying {
    int32		mChannel;
    Mix_Chunk*	mCurrentChunk;
};

class cSound {

	virtual bool		devicePrepare() = 0;

protected:
	int16				mPlayingTrack;
	int16				mPlayingSong;

	public:

						cSound();
	virtual				~cSound() { };

	virtual void		Sound_Play( int16 pTileset, int16 pSoundEffect, int16 pVolume, int16 pIndex ) = 0;
	virtual void		Sound_Stop() { }

	virtual void		Music_Play( int16 pTrack, int16 pSong = -1) = 0;
	virtual void		Music_Stop() = 0;
	virtual void		Music_SetVolume(int16 pChannel, int16 pVolume) {};
	virtual int16		Music_GetVolume(int16 pChannel) { return 0; }
	virtual void		Stop() {
		Music_Stop();
		Sound_Stop();
	}
};
