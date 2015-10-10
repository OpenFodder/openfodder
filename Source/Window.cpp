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

void Mixer_ChannelFinished(int32 pChannel) {

	g_Fodder.MixerChannelFinished( pChannel );
}

cWindow::cWindow() {

	mDimensionWindow.mWidth = 1280;
	mDimensionWindow.mHeight = 800 - 16;

	mDimensionPlayfield.mWidth = mDimensionWindow.mWidth;
	mDimensionPlayfield.mHeight = mDimensionWindow.mHeight;

	mScreenSize.mWidth = 320;
	mScreenSize.mHeight = 200;

	mSound = false;
	mWindowMode = true;
}

cWindow::~cWindow() {

	SDL_DestroyRenderer( mRenderer );
	SDL_DestroyWindow( mWindow );

	SDL_Quit();
}

bool cWindow::InitWindow( const std::string& pWindowTitle ) {
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		// TODO: Log error
		//SDL_GetError();
		std::cout << "Failed to initialise SDL\n";
		return false;
	}
	
	mWindow = SDL_CreateWindow(pWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mDimensionWindow.mWidth, mDimensionWindow.mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if (!mWindow) {
		// TODO: Log Error
		//SDL_GetError();
		std::cout << "Failed to create window\n";
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		// TODO: Log Error
		//SDL_GetError()
		std::cout << "Failed to create rendered\n";
		return false;
	}

	int audio_rate = 22050;// 22050;
	Uint16 audio_format = AUDIO_U8;// MIX_DEFAULT_FORMAT;
	int audio_channels = 2;
	int audio_buffers = 1024;
 
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		mSound = false;
	}
	else {
		mSound = true;
		Mix_ChannelFinished( Mixer_ChannelFinished );
	}

	SDL_RenderSetLogicalSize(mRenderer, 352, 216);

	SetCursor();
	return true;
}

void cWindow::EventCheck() {

	SDL_Event SysEvent;
	
	while (SDL_PollEvent( &SysEvent )) {

		cEvent Event;

		switch (SysEvent.type) {
			case SDL_KEYDOWN:
				Event.mType = eEvent_KeyDown;
				Event.mButton = SysEvent.key.keysym.scancode;
				break;

			case SDL_KEYUP:
				Event.mType = eEvent_KeyUp;
				Event.mButton = SysEvent.key.keysym.scancode;
				break;

			case SDL_MOUSEMOTION:
				Event.mType =eEvent_MouseMove;
				Event.mPosition = cPosition( SysEvent.motion.x, SysEvent.motion.y );
				break;

			case SDL_MOUSEBUTTONDOWN:

				switch (SysEvent.button.button) {

					case 1:
						Event.mType = eEvent_MouseLeftDown;
						Event.mButton = 1;
						break;

					case 3:
						Event.mType = eEvent_MouseRightDown;
						Event.mButton = 3;
						break;
				}

				Event.mPosition = cPosition( SysEvent.motion.x, SysEvent.motion.y );
				Event.mButtonCount = SysEvent.button.clicks;
				break;

			case SDL_MOUSEBUTTONUP:

				switch (SysEvent.button.button) {

					case 1:
						Event.mType = eEvent_MouseLeftUp;
						Event.mButton = 1;
						break;

					case 3:
						Event.mType = eEvent_MouseRightUp;
						Event.mButton = 3;
						break;
				}

				Event.mPosition = cPosition( SysEvent.motion.x, SysEvent.motion.y );
				Event.mButtonCount = SysEvent.button.clicks;
				break;

			case SDL_QUIT:
				Event.mType = eEvent_Quit;
				break;
		}

		if ( Event.mType != eEvent_None )
			g_Fodder.EventAdd( Event );
	}

}

void cWindow::RenderAt( cSurface* pImage, cPosition pSource ) {
	SDL_Rect Src;
	Src.w = mScreenSize.mWidth;
	Src.h = mScreenSize.mHeight;
	Src.x = pSource.mX + 16;
	Src.y = pSource.mY + 16;

	pImage->draw();

	//Draw the texture
	SDL_RenderCopy( mRenderer, pImage->GetTexture(), &Src, NULL);
}

void cWindow::RenderShrunk( cSurface* pImage ) {
	SDL_Rect Src;
	Src.w = pImage->GetWidth();
	Src.h = pImage->GetHeight();
	Src.x = 0;
	Src.y = 0;

	pImage->draw();

	//Draw the texture
	SDL_RenderCopy( mRenderer, pImage->GetTexture(), &Src, NULL);
}

void cWindow::FrameEnd() {

	SDL_RenderPresent( mRenderer );

	SDL_RenderClear( mRenderer );
}

void cWindow::SetCursor() {

	SDL_ShowCursor(0);

}

void cWindow::SetFullScreen() {

	if (mWindowMode) {
		SDL_SetWindowFullscreen( mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
		SDL_SetWindowFullscreen( mWindow, SDL_WINDOW_FULLSCREEN );
		mWindowMode = false;
	}
	else {
		SDL_SetWindowFullscreen( mWindow, 0 );
		mWindowMode = true;
	}


}

void cWindow::SetMousePosition( const cPosition& pPosition ) {

	SDL_WarpMouseInWindow( mWindow, pPosition.mX, pPosition.mY );
}

void cWindow::SetScreenSize( const cDimension& pDimension ) {

	mScreenSize = pDimension;
}

void cWindow::SetLogicalSize( const cDimension& pDimension ) {
	
	SDL_RenderSetLogicalSize(mRenderer, pDimension.mWidth, pDimension.mHeight);
}

void cWindow::SetWindowTitle( const std::string& pWindowTitle ) {

	SDL_SetWindowTitle( mWindow, pWindowTitle.c_str() );
}

void cWindow::SetWindowSize( const cDimension& pDimension ) {

	SDL_SetWindowSize( mWindow, pDimension.mWidth, pDimension.mHeight );
}

SDL_Renderer* cWindow::GetRenderer() const {

	return mRenderer;
}
