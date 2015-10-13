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

// Call back from Audio Device to fill audio output buffer
void cSound_AudioCallback(void *userdata, Uint8 *stream, int len) {
	cSound_Amiga  *sound = (cSound_Amiga*) userdata;

	sound->audioBufferFill( (short*) stream, len );
}

cSound_Amiga::cSound_Amiga() {

	mAudioSpec = 0;

	devicePrepare();
}

cSound_Amiga::~cSound_Amiga() {

	delete mAudioSpec;
}

void cSound_Amiga::audioBufferFill( short *pBuffer, int pBufferSize ) {

}

// Prepare the local audio device
bool cSound_Amiga::devicePrepare() {
	SDL_AudioSpec *desired;

	desired = new SDL_AudioSpec();
	mAudioSpec = new SDL_AudioSpec();

	// FM Quality, 16Bit Signed, Mono
	desired->freq=22050;
	desired->format=AUDIO_S16LSB;
	desired->channels=0;

	// 2048 Samples, at 2 bytes per sample
	desired->samples=0x800;

	// Function to call when the audio playback buffer is empty
	desired->callback = cSound_AudioCallback;

	// Pass a ptr to this class
	desired->userdata = this;

	// Open the audio device
	mVal = SDL_OpenAudio(desired, mAudioSpec);

	delete desired;

	if(mVal < 0 ) {
		std::cout << "Audio Device Initialization failed: " << SDL_GetError();
		std::cout << std::endl;
		return false;
	}

	return true;
}

void cSound_Amiga::Sound_Play( int16 pBx, int16 pData4 ) {

}

void cSound_Amiga::Music_Play( int16 pTrack ) {

}

void cSound_Amiga::Music_Stop() {

}