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

#include "stdafx.hpp"

void Mixer_ChannelFinished2(int32 pChannel) {

	((cSound_PC2*)g_Sound.GetSingletonPtr())->MixerChannelFinished( pChannel );
}

cSound_PC2::cSound_PC2() {

	mSound = false;
	mMusicPlaying = 0;
}

cSound_PC2::~cSound_PC2() {
	
	Mix_CloseAudio();
}

void cSound_PC2::Sound_Voc_Load() {

}

// Prepare the local audio device
bool cSound_PC2::devicePrepare() {

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_U8;
	int audio_channels = 2;
	int audio_buffers = 1024;
 
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		mSound = false;
	}
	else {
		mSound = true;
		Mix_ChannelFinished( Mixer_ChannelFinished2 );
	}

	return true;
}

void cSound_PC2::MixerChannelFinished( int32 pChannel ) {

}

void cSound_PC2::Sound_Play( int16 pBx, int16 pData4, int16 pData8 ) {

}

void cSound_PC2::Music_PlayFile( const char* pFilename ) {

	if (mSound == false)
		return;


}

void cSound_PC2::Music_Stop() {
	
	if (mSound == false)
		return;

	Mix_FadeOutMusic(500);
}

void cSound_PC2::Music_Play( int16 pTrack ) {


}
