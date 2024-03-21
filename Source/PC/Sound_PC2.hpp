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

struct sSoundMap {
    int16 mEffectID;
    int16 mUnk;
};

class cSound_PC2 : public cSound {
	bool					mSound;

    Mix_Music*				    mMusicPlaying;
    std::vector<sChunkPlaying>  mMixerChunks;
    std::vector<Mix_Chunk*>     mSoundEffects;

	private:
	bool					devicePrepare();
	void					Sound_Voc_Load();

	public:

						cSound_PC2();
						~cSound_PC2();

	void				Sound_Play( int16 pBx, int16 pData4, int16 pData8, int16 pIndex);

	void				Music_Play( int16 pTrack, int16 pSong);
	void				Music_PlayFile( const char* pFilename );
	void				Music_Stop();

	void				MixerChannelFinished( int32 pChannel );
};
