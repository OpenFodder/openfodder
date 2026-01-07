/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
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

static float ClampGain(float gain) {
    if (gain < 0.0f) {
        return 0.0f;
    }
    if (gain > 1.0f) {
        return 1.0f;
    }
    return gain;
}

cSound_PC2::cSound_PC2() {

	mSound = false;
    mMixer = nullptr;
    mMusicTrack = nullptr;
    mMusicAudio = nullptr;

    devicePrepare();
    Sound_Voc_Load();
}

cSound_PC2::~cSound_PC2() {
	Music_Stop();

    if (mMixer) {
        MIX_StopAllTracks(mMixer, 0);
    }

    for (auto &track : mMixerTracks) {
        if (track.mTrack) {
            MIX_DestroyTrack(track.mTrack);
        }
        if (track.mAudio) {
            MIX_DestroyAudio(track.mAudio);
        }
    }
    mMixerTracks.clear();

    if (mMusicTrack) {
        MIX_DestroyTrack(mMusicTrack);
        mMusicTrack = nullptr;
    }
    if (mMusicAudio) {
        MIX_DestroyAudio(mMusicAudio);
        mMusicAudio = nullptr;
    }

    for (auto soundEffect : mSoundEffects) {
        MIX_DestroyAudio(soundEffect);
    }
    mSoundEffects.clear();

    if (mMixer) {
        MIX_DestroyMixer(mMixer);
        mMixer = nullptr;
    }

    MIX_Quit();
}

void cSound_PC2::Sound_Voc_Load() {

    if (!mMixer) {
        return;
    }

    for (auto File : gSoundEffects) {

        auto file = g_Resource->fileGet(File);

        SDL_AudioSpec srcSpec;
        srcSpec.format = SDL_AUDIO_U8;
        srcSpec.channels = 1;
        srcSpec.freq = 8000;

        SDL_AudioSpec dstSpec;
        dstSpec.format = SDL_AUDIO_U8;
        dstSpec.channels = 2;
        dstSpec.freq = 22050;

        Uint8 *converted = nullptr;
        int convertedLen = 0;
        if (!SDL_ConvertAudioSamples(&srcSpec, file->data(), (int) file->size(), &dstSpec, &converted, &convertedLen)) {
            continue;
        }

        MIX_Audio *audio = MIX_LoadRawAudioNoCopy(mMixer, converted, (size_t) convertedLen, &dstSpec, true);
        if (!audio) {
            SDL_free(converted);
            continue;
        }

        mSoundEffects.push_back(audio);
    }
}

// Prepare the local audio device
bool cSound_PC2::devicePrepare() {

	SDL_AudioSpec spec;
    spec.freq = 22050;
    spec.format = SDL_AUDIO_U8;
    spec.channels = 2;

    if (!MIX_Init()) {
        mSound = false;
        return false;
    }

    mMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!mMixer) {
        mSound = false;
        MIX_Quit();
        return false;
    }
    mSound = true;

	return true;
}

void cSound_PC2::CleanupFinishedTracks() {
    for (auto it = mMixerTracks.begin(); it != mMixerTracks.end();) {
        if (!it->mTrack || !MIX_TrackPlaying(it->mTrack)) {
            if (it->mTrack) {
                MIX_DestroyTrack(it->mTrack);
            }
            if (it->mAudio) {
                MIX_DestroyAudio(it->mAudio);
            }
            it = mMixerTracks.erase(it);
        } else {
            ++it;
        }
    }
}

void cSound_PC2::Sound_Play( int16 pTileset, int16 pSoundEffect, int16 pVolume, int16 pIndex) {
    MIX_Audio* audio = 0;

    if (!mSound || !mMixer) {
        return;
    }

    if (pSoundEffect >= 65)
        return;

    auto map = stru_496DB[pSoundEffect];
    if (map.mEffectID == -1)
        return;

    if (map.mEffectID < 0 || (size_t)map.mEffectID >= mSoundEffects.size()) {
        return;
    }

    audio = mSoundEffects[map.mEffectID];
    if (!audio) {
        return;
    }

    CleanupFinishedTracks();

    MIX_Track *track = MIX_CreateTrack(mMixer);
    if (!track) {
        return;
    }

    MIX_SetTrackAudio(track, audio);
    MIX_SetTrackGain(track, ClampGain((100 - pVolume) / 128.0f));

    if (!MIX_PlayTrack(track, 0)) {
        MIX_DestroyTrack(track);
        return;
    }

    mMixerTracks.push_back({ track, nullptr });
}

void cSound_PC2::Music_PlayFile( const char* pFilename ) {

	if (mSound == false)
		return;

    std::string Filename = g_Fodder->mVersionCurrent->getDataFilePath(std::string(pFilename) + ".AMF");
    if (mMusicTrack) {
        MIX_StopTrack(mMusicTrack, 0);
    }
    if (mMusicAudio) {
        MIX_DestroyAudio(mMusicAudio);
        mMusicAudio = nullptr;
    }

    mMusicAudio = MIX_LoadAudio(mMixer, Filename.c_str(), false);
    if (mMusicAudio) {
        if (!mMusicTrack) {
            mMusicTrack = MIX_CreateTrack(mMixer);
        }
        if (mMusicTrack) {
            SDL_PropertiesID options = SDL_CreateProperties();
            MIX_SetTrackAudio(mMusicTrack, mMusicAudio);
            MIX_SetTrackGain(mMusicTrack, 0x70 / 128.0f);
            if (options) {
                SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
                MIX_PlayTrack(mMusicTrack, options);
                SDL_DestroyProperties(options);
            } else {
                MIX_PlayTrack(mMusicTrack, 0);
            }
        } else {
            MIX_DestroyAudio(mMusicAudio);
            mMusicAudio = nullptr;
        }
    }
}

void cSound_PC2::Music_Stop() {
    mPlayingTrack = -1;

	if (mSound == false)
		return;

    if (mMusicTrack) {
        MIX_StopTrack(mMusicTrack, MIX_TrackMSToFrames(mMusicTrack, 500));
    }
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
