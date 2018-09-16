/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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

cWindow::cWindow() {

	mOriginalResolution.mWidth = 320;
	mOriginalResolution.mHeight = 200;

	mWindow_Multiplier = mWindow_MultiplierPrevious = 2;

	mScreenSize.mWidth = mOriginalResolution.mWidth;
	mScreenSize.mHeight = mOriginalResolution.mHeight;

	mWindowMode = true;

	mWindow = 0;

	mRenderer = 0;

    mHasMouse = false;
    mHasFocus = false;

}

cWindow::~cWindow() {

	SDL_DestroyRenderer( mRenderer );
	SDL_DestroyWindow( mWindow );

	SDL_Quit();
}

bool cWindow::InitWindow( const std::string& pWindowTitle ) {
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		std::cout << "Failed to initialise SDL\n";
		exit( 1 );
		return false;
	}
	
    SetFullScreen();

	mWindow = SDL_CreateWindow(pWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GetWindowSize().mWidth, GetWindowSize().mHeight, SDL_WINDOW_SHOWN );
	if (!mWindow) {
		std::cout << "Failed to create window\n";
		exit( 1 );
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		std::cout << "Failed to create rendered\n";
		exit( 1 );
		return false;
	}

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, 0 );

	SetCursor();

#ifdef _DEBUG
    SetFullScreen();
    CalculateWindowSize();
#endif

	return true;
}

void cWindow::EventCheck() {

	SDL_Event SysEvent;
	
	while (SDL_PollEvent( &SysEvent )) {

		cEvent Event;

		switch (SysEvent.type) {
            case SDL_WINDOWEVENT:

                switch (SysEvent.window.event ) {
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        mHasFocus = false;
                        break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        mHasFocus = true;
                        break;

                    case SDL_WINDOWEVENT_ENTER:
                        mHasMouse = true;
                        break;

                    case SDL_WINDOWEVENT_LEAVE:
                        mHasMouse = false;
                        break;
                }
            break;

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

void cWindow::CalculateWindowSize() {
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	while ((mOriginalResolution.mWidth * mWindow_Multiplier) <= (unsigned int) (current.w / 2) && 
			(mOriginalResolution.mHeight * mWindow_Multiplier) <= (unsigned int) (current.h / 2) ) {
		++mWindow_Multiplier;
	}

	SetWindowSize( mWindow_Multiplier );
}

int16 cWindow::CalculateFullscreenSize() {
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	int16 Multiplier = 1;

	while ((mOriginalResolution.mWidth * Multiplier) <= (unsigned int) current.w && (mOriginalResolution.mHeight * Multiplier) <= (unsigned int) current.h ) {
		++Multiplier;
	}

	return --Multiplier;
}

bool cWindow::CanChangeToMultiplier( int pNewMultiplier ) {
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	if (	(mOriginalResolution.getWidth()  * pNewMultiplier >= current.w ||
			mOriginalResolution.getHeight() * pNewMultiplier >= current.h) ||
			pNewMultiplier <= 0 )
		return false;

	return true;
}

void cWindow::FrameEnd() {

	SDL_RenderPresent( mRenderer );
	SDL_RenderClear( mRenderer );
}

void cWindow::PositionWindow() {
	
	SDL_SetWindowPosition( mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
}

void cWindow::WindowIncrease() {
	
	if (!mWindowMode)
			return;

	// Once we reach the max window size, go to full screen
	if (!CanChangeToMultiplier( mWindow_Multiplier + 1 )) {

		SetFullScreen();
		mWindowMode = false;
		return;
	}

	if (!mWindowMode)
		return;

	SetWindowSize( mWindow_Multiplier + 1 );
}

void cWindow::WindowDecrease() {

	// If we're in full screen mode remove it
	if (!mWindowMode) {
		/*
		while (CanChangeToMultiplier( mWindow_Multiplier )) {
			--mWindow_Multiplier;
		}

		++mWindow_Multiplier;*/

		//mWindowMode = true;
		//SetWindowSize( mWindow_MultiplierPrevious );
		SetFullScreen();
		return;
	}

	if (!CanChangeToMultiplier(mWindow_Multiplier - 1 ))
		return;

	SetWindowSize( mWindow_Multiplier - 1 );
}

void cWindow::RenderAt( cSurface* pImage, cPosition pSource ) {
	SDL_Rect Src, Dest;

	Src.w = mScreenSize.mWidth;
	Src.h = mScreenSize.mHeight;
	Src.x = (int) pSource.mX + 16;
	Src.y = (int) pSource.mY + 16;

	Dest.w = GetWindowSize().mWidth;
	Dest.h = GetWindowSize().mHeight;

	if (mWindowMode) {
		Dest.x = 0;
		Dest.y = 0;
	}
	else {
		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);

		Dest.x = (current.w - Dest.w) / 2;
		Dest.y = (current.h - Dest.h) / 2;
	}
	pImage->draw();

	SDL_RenderCopy( mRenderer, pImage->GetTexture(), &Src, &Dest );
}

void cWindow::RenderShrunk( cSurface* pImage ) {
	SDL_Rect Src, Dest;
	Src.w = (int) pImage->GetWidth();
	Src.h = (int) pImage->GetHeight();
	Src.x = 0;
	Src.y = 0;

	Dest.w = GetWindowSize().mWidth;
	Dest.h = GetWindowSize().mHeight;

	if (mWindowMode) {
		Dest.x = 0;
		Dest.y = 0;
	}
	else {
		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);

		Dest.x = (current.w - Dest.w) / 2;
		Dest.y = (current.h - Dest.h) / 2;
	}
	pImage->draw();

	SDL_RenderCopy( mRenderer, pImage->GetTexture(), &Src, &Dest);
}

void cWindow::SetCursor() {

	SDL_ShowCursor(0);
}

bool cWindow::isGrabbed() {
    if (SDL_GetWindowGrab(mWindow) == SDL_TRUE)
        return true;

    return false;
}
void cWindow::GrabMouse() {
    SDL_SetWindowGrab(mWindow, SDL_TRUE);
}

void cWindow::ReleaseMouse() {
    SDL_SetWindowGrab(mWindow, SDL_FALSE);
}

void cWindow::SetFullScreen() {

	if (mWindowMode) {

		mWindow_MultiplierPrevious = mWindow_Multiplier;

		SetWindowSize( CalculateFullscreenSize());

		SDL_SetWindowFullscreen( mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );

		mWindowMode = false;
	}
	else {
		mWindowMode = true;
		SetWindowSize( mWindow_MultiplierPrevious );
	}
}

void cWindow::SetMouseWindowPosition( const cPosition& pPosition ) {

	SDL_WarpMouseInWindow( mWindow, pPosition.getX(), pPosition.getY() );
}

const cPosition cWindow::GetMousePosition() {
    cPosition Position;

    SDL_GetGlobalMouseState(&Position.mX, &Position.mY);

    return Position;
}

void cWindow::SetMousePosition(const cPosition& pPosition) {

    SDL_WarpMouseGlobal(pPosition.getX(), pPosition.getY());
}

void cWindow::SetScreenSize( const cDimension& pDimension ) {

	mScreenSize = pDimension;
}

void cWindow::SetOriginalRes( const cDimension& pDimension ) {

	mOriginalResolution = pDimension;
	
	if (!mWindowMode) {
		SetFullScreen();
		SetFullScreen();
	} else
		SetWindowSize( mWindow_Multiplier );
}

void cWindow::SetWindowTitle( const std::string& pWindowTitle ) {

	SDL_SetWindowTitle( mWindow, pWindowTitle.c_str() );
}

void cWindow::SetWindowSize( const int pMultiplier ) {
	mWindow_Multiplier = pMultiplier;

	if (mWindow) {
		if (mWindowMode) {
			SDL_SetWindowFullscreen( mWindow, 0 );
		}
		SDL_SetWindowSize( mWindow, GetWindowSize().mWidth, GetWindowSize().mHeight );
		PositionWindow();
	}
}

const cPosition cWindow::GetWindowPosition() const {
    cPosition Pos;

    SDL_GetWindowPosition(mWindow, &Pos.mX, &Pos.mY);

    return Pos;
}

const cDimension cWindow::GetWindowSize() const {

	return cDimension( mOriginalResolution.mWidth * mWindow_Multiplier, mOriginalResolution.mHeight * mWindow_Multiplier ); 
}

const bool cWindow::HasFocus() {

    if (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MOUSE_FOCUS)
        return true;

    return false;
}
