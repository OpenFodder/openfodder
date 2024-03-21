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

const char* gSoundEffects[] = {
    "BIRD.SMP",
    "DIE1.SMP",
    "DIE2.SMP",
    "DIE3.SMP",
    "DUCK.SMP",
    "ENGINE.SMP",
    "EXPLOSN.SMP",
    "GRENADE.SMP",
    "GUN.SMP",      // 0x11
    "HELI.SMP",
    "MGUN.SMP",
    "MISSILE.SMP",
    "SHEEP.SMP",
    "ROOK.SMP",
};

sSoundMap stru_496DB[] = {
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { 6, 6302 },
  { 6, 6302 },
  { 6, 8363 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { 1, 8363 },
  { 2, 8363 },
  { 3, 8363 },
  { -1, -1 },
  { 7, 8363 },
  { 8, 8363 },
  { 8, 8363 },
  { -1, -1 },
  { -1, -1 },
  { 1, 8363 },
  { 2, 8363 },
  { 3, 8363 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { 10, 8363 },
  { 11, 16726 },
  { 11, 16726 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
  { -1, -1 },
};

void Mixer_ChannelFinished2(int32 pChannel) {

    g_Fodder->GetSound<cSound_PC2>()->MixerChannelFinished( pChannel );
}

cSound_PC2::cSound_PC2() {

	mSound = false;
	mMusicPlaying = 0;

    devicePrepare();
    Sound_Voc_Load();
}

cSound_PC2::~cSound_PC2() {
	
	Mix_FreeMusic(mMusicPlaying);
	SDL_Delay(100);

	Mix_CloseAudio();
	SDL_CloseAudio();

	SDL_Delay(100);
	for (std::vector<sChunkPlaying>::iterator ChannelIT = mMixerChunks.begin(); ChannelIT != mMixerChunks.end(); ++ChannelIT) {
		Mix_FreeChunk(ChannelIT->mCurrentChunk);
	}
	for (auto SoundEffect : mSoundEffects) {
		Mix_FreeChunk(SoundEffect);
	}
}

void cSound_PC2::Sound_Voc_Load() {

    for (auto File : gSoundEffects) {

        auto file = g_Resource->fileGet(File);

        SDL_AudioCVT cvt;
        SDL_BuildAudioCVT(&cvt, AUDIO_U8, 1, 8000, AUDIO_U8, 2, 22050);
        SDL_assert(cvt.needed);

        cvt.len = (int) file->size();
        cvt.buf = (Uint8 *)SDL_malloc(cvt.len * cvt.len_mult);

        memcpy(cvt.buf, file->data(), file->size());

        SDL_ConvertAudio(&cvt);

        Mix_Chunk* chunk = Mix_QuickLoad_RAW(cvt.buf, cvt.len_cvt);
        mSoundEffects.push_back(chunk);
        //SDL_free(cvt.buf);
    }
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

    auto end = std::remove_if(mMixerChunks.begin(), mMixerChunks.end(), [pChannel](sChunkPlaying& a) {
        return a.mChannel == pChannel;
    });

    mMixerChunks.erase(end, mMixerChunks.end());
}

void cSound_PC2::Sound_Play( int16 pTileset, int16 pSoundEffect, int16 pVolume, int16 pIndex) {
    Mix_Chunk* chunk = 0;

    if (pSoundEffect >= 65)
        return;

    auto map = stru_496DB[pSoundEffect];
    if (map.mEffectID == -1)
        return;

    chunk = mSoundEffects[map.mEffectID];
    if (!chunk) {
        return;
    }

    auto Channel = Mix_PlayChannel(-1, chunk, 0);
    Mix_Volume(Channel, 100 - pVolume);
    mMixerChunks.push_back({ Channel, chunk });
}

void cSound_PC2::Music_PlayFile( const char* pFilename ) {

	if (mSound == false)
		return;

    std::string Filename = g_Fodder->mVersionCurrent->getDataFilePath(std::string(pFilename) + ".AMF");
    Mix_FreeMusic(mMusicPlaying);
    SDL_Delay(100);

    mMusicPlaying = Mix_LoadMUS(Filename.c_str());
    Mix_VolumeMusic(0x70);

    if (mMusicPlaying)
        Mix_PlayMusic(mMusicPlaying, -1);
}

void cSound_PC2::Music_Stop() {
    mPlayingTrack = -1;

	if (mSound == false)
		return;

	Mix_FadeOutMusic(500);
}

void cSound_PC2::Music_Play( int16 pTrack, int16 pSong) {
    std::string filename;

    if (mPlayingTrack == pTrack)
        return;

    mPlayingTrack = pTrack;

    switch (pTrack) {
        case 0:
            filename = "MENU";
            break;

        case 6:
            filename = "WINZONE";
            break;

        case 12:
            filename = "WINLEVEL";
            break;

        case 7:
            filename = "HELIANIM";
            break;

       // case xx:
       //   filename = "LOADER.AMF";
       //   break;

        case 15:
        case 8:
            filename = "GAMEOVER";
            break;

        case 20:
            filename = "TITLE";
            break;

    }

    if(filename.size())
        Music_PlayFile(filename.c_str());

}
