/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2017 Robert Crossfield
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
	uint8*								mCurrentMusicSongData;
	uint8*								mCurrentMusicInstrumentData;
	size_t								mCurrentSongDataSize;
	size_t								mCurrentInstrumentDataSize;

	const sSoundData*					mTrack;

	sSound() {
		mCurrentMusicSongData = 0;
		mCurrentMusicInstrumentData = 0;
		mCurrentSongDataSize = 0;
		mCurrentInstrumentDataSize = 0;
		mTrack = 0;
	}
};

class cSound_Amiga : public cSound {
	Audio::AudioStream*					mCurrentMusic;
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

	int16				Track_Load( sSound* pSound, int16 pTrack );
	void				Sound_Play( int16 pTileset, int16 pSoundEffect, int16 pVolume );

	void				Music_Play( int16 pTrack );
	void				Music_Stop();
	void				Sound_Stop();
};
