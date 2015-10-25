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
#include "PC/VocTable.hpp"

void Mixer_ChannelFinished(int32 pChannel) {

	((cSound_PC*)g_Sound.GetSingletonPtr())->MixerChannelFinished( pChannel );
}

cSound_PC::cSound_PC() {

	mSound = false;
	mMusicPlaying = 0;

	devicePrepare();

	for (unsigned int x = 0; x < 0x3C; ++x) {
		dword_42320[x].mBuffer = 0;
		dword_42320[x].mCleanup = true;
		dword_42410[x].mBuffer = 0;
		dword_42410[x].mCleanup = true;
		dword_42500[x].mBuffer = 0;
		dword_42500[x].mCleanup = true;
		dword_425F0[x].mBuffer = 0;
		dword_425F0[x].mCleanup = true;
		dword_426E0[x].mBuffer = 0;
		dword_426E0[x].mCleanup = true;
	}

	word_42316[0] = dword_42320;	// Jun
	word_42316[1] = dword_42410;	// Des
	word_42316[2] = dword_42500;	// Ice
	word_42316[3] = dword_425F0;
	word_42316[4] = dword_426E0;
	word_42316[5] = 0;
	word_42316[6] = dword_42500;	// Amiga Format Xmas

	Sound_Voc_Load();
}

cSound_PC::~cSound_PC() {
	
	for (unsigned int x = 0; x < 0x3C; ++x) {

		if (dword_42320[x].mCleanup)
			delete[] dword_42320[x].mBuffer;

		if (dword_42410[x].mCleanup)
			delete[] dword_42410[x].mBuffer;

		if (dword_42500[x].mCleanup)
			delete[] dword_42500[x].mBuffer;

		if (dword_425F0[x].mCleanup)
			delete[] dword_425F0[x].mBuffer;

		if (dword_426E0[x].mCleanup)
			delete[] dword_426E0[x].mBuffer;
	}

	Mix_CloseAudio();
	SDL_CloseAudio();
}

void cSound_PC::Sound_Voc_Load() {

	for (unsigned int x = 0; x < 0x3C; ++x) {
		dword_42320[x].mSize = 0;
		dword_42410[x].mSize = 0;
		dword_42500[x].mSize = 0;
		dword_425F0[x].mSize = 0;
	}

	struct_Voc* Voc = mVocTable;

	for (; Voc->field_0 != 0xFF; ++Voc) {
		size_t bx = 0;

		uint8* VocFile = g_Resource.fileGet( Voc->mFilename, bx );

		if (Voc->field_0 != 9) {
			sVocLoaded* eax = word_42316[Voc->field_0];
			eax[Voc->field_1].mBuffer = VocFile;
			eax[Voc->field_1].mSize = bx;
		} else {

			dword_42320[Voc->field_1].mBuffer = VocFile;
			dword_42320[Voc->field_1].mSize = bx;
			dword_42320[Voc->field_1].mCleanup = true;

			dword_42410[Voc->field_1].mBuffer = VocFile;
			dword_42410[Voc->field_1].mSize = bx;
			dword_42410[Voc->field_1].mCleanup = false;

			dword_42500[Voc->field_1].mBuffer = VocFile;
			dword_42500[Voc->field_1].mSize = bx;
			dword_42500[Voc->field_1].mCleanup = false;

			dword_425F0[Voc->field_1].mBuffer = VocFile;
			dword_425F0[Voc->field_1].mSize = bx;
			dword_425F0[Voc->field_1].mCleanup = false;

			dword_426E0[Voc->field_1].mBuffer = VocFile;
			dword_426E0[Voc->field_1].mSize = bx;
			dword_426E0[Voc->field_1].mCleanup = false;
		}
	}
}

// Prepare the local audio device
bool cSound_PC::devicePrepare() {

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_U8;
	int audio_channels = 2;
	int audio_buffers = 1024;
 
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		mSound = false;
	}
	else {
		mSound = true;
		Mix_ChannelFinished( Mixer_ChannelFinished );
	}

	return true;
}

void cSound_PC::MixerChannelFinished( int32 pChannel ) {

	for (std::vector<sVocPlaying>::iterator ChannelIT = mMixerChunks.begin(); ChannelIT != mMixerChunks.end(); ++ChannelIT) {
		
		if (ChannelIT->mChannel == pChannel) {
			Mix_FreeChunk( ChannelIT->mCurrentChunk );

			mMixerChunks.erase( ChannelIT );
			return;
		}
	}
}

void cSound_PC::Sound_Play( int16 pBx, int16 pData4, int16 pData8 ) {
	sVocPlaying Playing;
	sVocLoaded* eax = &word_42316[pBx][pData4];
	if (eax->mSize == 0 || mSound == false )
		return;

	SDL_RWops *rw = SDL_RWFromMem( eax->mBuffer, (int) eax->mSize );

	Playing.mCurrentChunk = Mix_LoadWAV_RW( rw, 1 );
	Playing.mChannel = Mix_PlayChannel( -1, Playing.mCurrentChunk , 0 );
	Mix_Volume(Playing.mChannel, pData8 );

	if (Playing.mChannel == -1) {
		Mix_FreeChunk( Playing.mCurrentChunk );
		return;
	}

	mMixerChunks.push_back( Playing );
}

void cSound_PC::Music_PlayFile( const char* pFilename ) {

	if (mSound == false)
		return;

	std::string Filename = "Data/WAV/";
	Filename.append( pFilename );
	Filename.append( ".wav" );

	Mix_FreeMusic( mMusicPlaying );
	SDL_Delay( 100 );

	mMusicPlaying = Mix_LoadMUS( Filename.c_str() );
	Mix_VolumeMusic( 0x70 );

	if (mMusicPlaying)
		Mix_PlayMusic( mMusicPlaying, -1 );
}

void cSound_PC::Music_Stop() {
	
	if (mSound == false)
		return;

	Mix_FadeOutMusic(500);
}

void cSound_PC::Music_Play( int16 pTrack ) {
	
	const char* Tracks[] = {
		"rjp.JON(1)",
		"rjp.JON(2)",
		"rjp.JON(3)",
		"rjp.JON(4)",
		"rjp.JON(5)",
		"rjp.JON(6)",
		"rjp.JON(7)",
		"rjp.JON(8)",
		"rjp.JON(9)",
		"rjp.JON(10)",
		"rjp.JON(11)",
		"rjp.JON(12)",
		"rjp.JON(13)",
		"rjp.JON(14)",
		"rjp.JON(15)",
		"rjp.JON(16)",
		"rjp.WARX4(1)",
		"rjp.WARX4(2)",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"rjp.JUNBASE(2)",
		"rjp.DESBASE(2)",
		"rjp.ICEBASE(2)",
		"rjp.MORBASE(2)",
		"rjp.INTBASE(2)",
		"",
		"",
		"",
	};

	Music_PlayFile( Tracks[pTrack] );

}
