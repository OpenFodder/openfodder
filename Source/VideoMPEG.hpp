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

#ifdef OPENFODDER_ENABLE_FFMPEG
class cVideoMPEG {
protected:
	struct sVideoMPEG_FFmpeg* mFF;
	bool			mLoaded;
	bool			mPlaying;
	bool			mLoop;

	double			mPlaybackClock;
	double			mNextVideoTime;

	SDL_Texture*	mTexture;
	SDL_AudioStream* mAudioStream;

	int				mVideoWidth;
	int				mVideoHeight;

protected:
	std::string		FindIntroPath() const;
	void			CleanupFFmpeg();
	void			ResetTiming();
	bool			DecodeNextFrame();

public:
					cVideoMPEG();
					~cVideoMPEG();

	bool			LoadIntroFromData();
	bool			LoadFromFile(const std::string& pPath);

	void			SetLoop(const bool pLoop);
	bool			Play();
	void			Stop();

	bool			Update(const double pDeltaSeconds);
	void			Render();

	bool			isPlaying() const { return mPlaying; }
	bool			isLoaded() const { return mLoaded; }

	int				GetWidth() const { return mVideoWidth; }
	int				GetHeight() const { return mVideoHeight; }
};
#endif
