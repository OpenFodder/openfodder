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

#include "stdafx.hpp"

const sSoundData Tracks1[] = {
	{ "JON.SNG", "JON.INS" },
	{ "WARX4.SNG", "WARX1.INS" },
	{ "JUNBASE.SNG", "JUNBASE.INS" },
	{ "DESBASE.SNG", "DESBASE.INS" },
	{ "ICEBASE.SNG", "ICEBASE.INS" },
	{ "MORBASE.SNG", "MORBASE.INS" },
	{ "INTBASE.SNG", "INTBASE.INS" },
	{ "", "" },
	{ "AFXBASE.SNG", "AFXBASE.INS" }
};

const sSoundData Tracks2[] = {
    { "JON.SNG", "JON.INS" },
    { "KILLER.SNG", "KILLER.INS" },
    { "JUNBASE.SNG", "JUNBASE.INS" },
    { "DESBASE.SNG", "DESBASE.INS" },
    { "ICEBASE.SNG", "ICEBASE.INS" },
    { "MORBASE.SNG", "MORBASE.INS" },
    { "INTBASE.SNG", "INTBASE.INS" },
    { "", "" },
    { "AFXBASE.SNG", "AFXBASE.INS" }
};

// Call back from Audio Device to fill audio output buffer
void cSound_AudioCallback(void *userdata, Uint8 *stream, int len) {
	cSound_Amiga  *sound = (cSound_Amiga*) userdata;

    try {
        sound->audioBufferFill((short*)stream, len);
    } catch (...) {

    }
}

cSound_Amiga::cSound_Amiga() {
	
	mVal = 0;
	mLock = SDL_CreateMutex();

	mAudioSpec = 0;
	mCurrentMusic = 0;

	devicePrepare();
}

cSound_Amiga::~cSound_Amiga() {

	Stop();

	SDL_Delay(100);
	SDL_CloseAudioDevice(mVal);
	SDL_DestroyMutex(mLock);
	SDL_Delay(100);

	delete mAudioSpec;
}

void cSound_Amiga::audioBufferFill( short *pBuffer, int pBufferSize ) {
	memset(pBuffer, 0, pBufferSize);
	
	if (SDL_LockMutex( mLock ) == 0) {

		if (mCurrentMusic && !mCurrentMusic->endOfData())
			mCurrentMusic->readBuffer( pBuffer, pBufferSize / 2 );

		if (mCurrentSfx.size()) {
			for ( auto SfxIT : mCurrentSfx) {

				if (!SfxIT->endOfStream())
					SfxIT->readBuffer( pBuffer, pBufferSize / 2 );
			}

			for ( auto SfxIT = mCurrentSfx.begin(); SfxIT != mCurrentSfx.end();) {

				if ((*SfxIT)->endOfStream()) {
					delete (*SfxIT);

					mCurrentSfx.erase( SfxIT );
					SfxIT = mCurrentSfx.begin();
					continue;
				}

				++SfxIT;
			}
		}
		SDL_UnlockMutex( mLock );
	}
}

// Prepare the local audio device
bool cSound_Amiga::devicePrepare() {
	SDL_AudioSpec *desired;

	desired = new SDL_AudioSpec();
	mAudioSpec = new SDL_AudioSpec();

	// FM Quality, 16Bit Signed, Mono
	desired->freq=44100;
	desired->format=AUDIO_S16LSB;
	desired->channels=2;

	// 2048 Samples, at 2 bytes per sample
	desired->samples=0x800;

	// Function to call when the audio playback buffer is empty
	desired->callback = cSound_AudioCallback;

	// Pass a ptr to this class
	desired->userdata = this;

	// Open the audio device
    mVal = SDL_OpenAudioDevice(NULL, 0, desired, mAudioSpec, 0);

	delete desired;

	if(mVal < 0 ) {
		std::cout << "Audio Device Initialization failed: " << SDL_GetError();
		std::cout << std::endl;
		return false;
	}

	return true;
}

int16 cSound_Amiga::Track_Load( sSound* pSound, int16 pTrack ) {
	int16 Number = 0;
    const sSoundData *Tracks = CANNON_BASED(Tracks1, Tracks2);
    const sSoundData *Track = 0;
	
	// Intro
	if (pTrack >= 16 && pTrack <= 20) {
		Track = &Tracks[1];
		Number = (pTrack - 16);

        if (!Number)
            Number = 1;
	}

	// Mission
	if (pTrack >= 0x32) {
		Track = &Tracks[2 + (pTrack - 0x32)];
		Number = 1;
	}

	// Jon 
	if (pTrack < 16) {
		Track = &Tracks[0];
		Number = pTrack;
	}

	if (pSound->mTrack != Track) {

		pSound->mCurrentMusicSongData = g_Resource->fileGet( Track->mSongData );
		pSound->mCurrentMusicInstrumentData = g_Resource->fileGet( Track->mInstrumentData );
		pSound->mTrack = Track;
	}

	return Number;
}

void cSound_Amiga::Sound_Play( int16 pTileset, int16 pSoundEffect, int16 pVolume ) {

	Track_Load( &mSound_Sfx, pTileset + 0x32 );

	if (SDL_LockMutex( mLock ) == 0) {
		if (mSound_Sfx.mCurrentMusicSongData && mSound_Sfx.mCurrentMusicInstrumentData) {
			Audio::AudioStream* Sfx = Audio::makeRjp1Stream( mSound_Sfx.mCurrentMusicSongData, mSound_Sfx.mCurrentMusicInstrumentData, -pSoundEffect );
			Sfx->mVolume = pVolume;
			mCurrentSfx.push_back( Sfx );
		}

		SDL_UnlockMutex( mLock );
	}

    SDL_PauseAudioDevice(mVal, 0);
}

void cSound_Amiga::Music_Play( int16 pTrack ) {
	
	int16 Number = Track_Load( &mSound_Music, pTrack );
		
	Music_Stop();

    if (mSound_Music.mCurrentMusicSongData && mSound_Music.mCurrentMusicInstrumentData) {
        if (mSound_Music.mCurrentMusicSongData->size() && mSound_Music.mCurrentMusicInstrumentData->size())
            mCurrentMusic = Audio::makeRjp1Stream(mSound_Music.mCurrentMusicSongData, mSound_Music.mCurrentMusicInstrumentData, Number);
    }

    SDL_PauseAudioDevice(mVal, 0);
}

void cSound_Amiga::Music_Stop() {

	if (SDL_LockMutex( mLock ) == 0) {

		delete mCurrentMusic;
		mCurrentMusic = 0;

		SDL_UnlockMutex(mLock);
	}
}

void cSound_Amiga::Sound_Stop() {
	
	if (SDL_LockMutex( mLock ) == 0) {

		for (auto SfxIT : mCurrentSfx) {

			delete SfxIT;
		}

		mCurrentSfx.clear();

		SDL_UnlockMutex(mLock);
	}
}
