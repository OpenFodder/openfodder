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
#include "PC/VocTable.hpp"

static float ClampGain(float gain) {
    if (gain < 0.0f) {
        return 0.0f;
    }
    if (gain > 1.0f) {
        return 1.0f;
    }
    return gain;
}

cSound_PC::cSound_PC() {

	mSound = false;
    mMixer = nullptr;
    mMusicTrack = nullptr;
    mMusicAudio = nullptr;

	devicePrepare();

	for (unsigned int x = 0; x < 0x3C; ++x) {
		dword_42320[x] = nullptr;
		dword_42410[x] = nullptr;
		dword_42500[x] = nullptr;
		dword_425F0[x] = nullptr;
		dword_426E0[x] = nullptr;
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

    if (mMixer) {
        MIX_DestroyMixer(mMixer);
        mMixer = nullptr;
    }

    MIX_Quit();
}

void cSound_PC::Sound_Voc_Load() {

	const struct_Voc* Voc = mVocTable;

	for (; Voc->field_0 != 0xFF; ++Voc) {

		auto VocFile = g_Resource->fileGet( Voc->mFilename );

		if (Voc->field_0 != 9) {
			word_42316[Voc->field_0][Voc->field_1] = VocFile;
		} else {

			dword_42320[Voc->field_1] = VocFile;
			dword_42410[Voc->field_1] = VocFile;
			dword_42500[Voc->field_1] = VocFile;
			dword_425F0[Voc->field_1] = VocFile;
			dword_426E0[Voc->field_1] = VocFile;
		}
	}
}

// Prepare the local audio device
bool cSound_PC::devicePrepare() {

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

void cSound_PC::CleanupFinishedTracks() {
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

void cSound_PC::Sound_Play( int16 pTileset, int16 pSoundEffect, int16 pVolume, int16 pIndex) {
	auto eax = word_42316[pTileset][pSoundEffect];
	if (!eax || !eax->size() || mSound == false )
		return;

    CleanupFinishedTracks();

	SDL_IOStream *io = SDL_IOFromMem( const_cast<uint8*>(eax->data()), (size_t) eax->size() );
    if (!io) {
        return;
    }

    MIX_Audio *audio = MIX_LoadAudio_IO(mMixer, io, true, true);
    if (!audio) {
        return;
    }

    MIX_Track *track = MIX_CreateTrack(mMixer);
    if (!track) {
        MIX_DestroyAudio(audio);
        return;
    }

    MIX_SetTrackAudio(track, audio);
    MIX_SetTrackGain(track, ClampGain(pVolume / 128.0f));

    if (!MIX_PlayTrack(track, 0)) {
        MIX_DestroyTrack(track);
        MIX_DestroyAudio(audio);
        return;
    }

    mMixerTracks.push_back({ track, audio });
}

void cSound_PC::Music_PlayFile( const std::string& pFilename ) {

	if (mSound == false)
		return;

    if (mMusicTrack) {
        MIX_StopTrack(mMusicTrack, 0);
    }
    if (mMusicAudio) {
        MIX_DestroyAudio(mMusicAudio);
        mMusicAudio = nullptr;
    }

	if(g_ResourceMan->FileExists(pFilename.c_str())) {
        mMusicAudio = MIX_LoadAudio(mMixer, pFilename.c_str(), false);
    }

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
	else {
		if(pFilename.size())
			g_Debugger->Error(SDL_GetError());
	}
}

void cSound_PC::Music_Stop() {
	mPlayingTrack = -1;

	if (mSound == false)
		return;

    if (mMusicTrack) {
        MIX_StopTrack(mMusicTrack, MIX_TrackMSToFrames(mMusicTrack, 500));
    }
}

void cSound_PC::Music_Play( int16 pTrack, int16 pSong) {
	
	if (mPlayingTrack == pTrack)
		return;

	mPlayingTrack = pTrack;

	static std::string Tracks[] = {
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

	if (g_Fodder->mVersionCurrent->isPCFormat() && pTrack == 16)
		Music_PlayFile(g_ResourceMan->GetFilePath(g_Fodder->mVersionCurrent, "CANNON2.DAT"));
	else {
		Music_PlayFile(g_ResourceMan->GetWavPath(Tracks[pTrack] + ".wav"));
	}


}
