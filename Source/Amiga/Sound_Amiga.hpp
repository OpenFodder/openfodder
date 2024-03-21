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

struct sSoundData {
	const char* mSongData;
	const char* mInstrumentData;
};

struct sSound {
	tSharedBuffer 		mCurrentMusicSongData;
	tSharedBuffer 		mCurrentMusicInstrumentData;

	const sSoundData*					mTrack;

	sSound() {

		mTrack = 0;
	}
};

class cSound_Amiga : public cSound {
	Audio::Rjp1*					    mCurrentMusic;
	sSound								mSound_Music;
	sSound								mSound_Sfx;

	SDL_AudioSpec*						mAudioSpec;
	int									mVal;
	SDL_mutex*							mLock;

	std::vector<Audio::AudioStream*>	mCurrentSfx;


private:

	bool				devicePrepare();

public:

						cSound_Amiga();
						~cSound_Amiga();

	void				audioBufferFill( short *pBuffer, int pBufferSize );

	int16				Track_Load( sSound* pSound, int16 pTrack, int16 pSong );
	void				Sound_Play( int16 pTileset, int16 pSoundEffect, int16 pVolume, int16 pIndex);

	void				Music_Play( int16 pTrack, int16 pSong);
    void				Music_Stop();
	void				Music_SetVolume(int16 pChannel, int16 pVolume);
	int16				Music_GetVolume(int16 pChannel);

	void				Sound_Stop();
        
    
};
