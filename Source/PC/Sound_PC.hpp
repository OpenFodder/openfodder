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

struct sVocLoaded {
	uint8*		mBuffer;
	size_t		mSize;
	bool		mCleanup;
};
struct sVocPlaying {
	int32		mChannel;
	Mix_Chunk*	mCurrentChunk;
};

class cSound_PC : public cSound {
	bool					mSound;

	Mix_Music*				mMusicPlaying;
	

	std::vector<sVocPlaying> mMixerChunks;

	sVocLoaded*				word_42316[7];
	sVocLoaded				dword_42320[0x3C];
	sVocLoaded				dword_42410[0x3C];
	sVocLoaded				dword_42500[0x3C];
	sVocLoaded				dword_425F0[0x3C];
	sVocLoaded				dword_426E0[0x3C];

	private:
	bool					devicePrepare();
	void					Sound_Voc_Load();

	public:

						cSound_PC();
						~cSound_PC();

	void				Sound_Play( int16 pBx, int16 pData4, int16 pData8 );

	void				Music_Play( int16 pTrack );
	void				Music_PlayFile( const char* pFilename );
	void				Music_Stop();

	void				MixerChannelFinished( int32 pChannel );
};
