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

	mScaler = mScalerPrevious = 2;

	mScreenSize.mWidth = mOriginalResolution.mWidth;
	mScreenSize.mHeight = mOriginalResolution.mHeight;

	mWindowMode = true;

	mWindow = 0;

	mRenderer = 0;

    mHasFocus = false;
    mCursorGrabbed = false;
	mResized = false;
}

cWindow::~cWindow() {

	SDL_DestroyRenderer( mRenderer );
	SDL_DestroyWindow( mWindow );

	SDL_Quit();
}

bool cWindow::InitWindow( const std::string& pWindowTitle ) {
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        g_Debugger->Error("Failed to initialise SDL");
		exit( 1 );
		return false;
	}
	
    ToggleFullscreen();

	mWindow = SDL_CreateWindow(pWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GetWindowSize().mWidth, GetWindowSize().mHeight, SDL_WINDOW_SHOWN );
	if (!mWindow) {
        g_Debugger->Error("Failed to create window");
		exit( 1 );
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
        g_Debugger->Error("Failed to create rendered");
		exit( 1 );
		return false;
	}

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, 0 );

	SetCursor();

    if (g_Fodder->mParams.mWindowMode) {
        ToggleFullscreen();
        CalculateWindowSize();
    }


	return true;
}

std::vector<cEvent>* cWindow::EventGet() {
    return &mEvents;
}

bool cWindow::Cycle() {
    //cPosition Mouse;

    EventCheck();

    // Update the mouse position over the window
    /*for (auto& Event : mEvents) {
        switch (Event.mType) {
        default:
            break;
        case eEvent_MouseLeftDown:
        case eEvent_MouseRightDown:
        case eEvent_MouseLeftUp:
        case eEvent_MouseRightUp:
        case eEvent_MouseMove:
            Mouse = Event.mPosition;
            break;
        }
    }*/

    // TODO: Move Cursor logic here

    return true;
}

void cWindow::EventCheck() {

	SDL_Event SysEvent;
	
	while (SDL_PollEvent( &SysEvent )) {

		cEvent Event;

		switch ( SysEvent.type ) {
            case SDL_WINDOWEVENT:

                switch ( SysEvent.window.event ) {
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        mHasFocus = false;
                        break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        mHasFocus = true;
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
				Event.mType = eEvent_MouseMove;
				Event.mPosition = cPosition( SysEvent.motion.x, SysEvent.motion.y );
				break;

            case SDL_MOUSEWHEEL:
                Event.mType = Event.mType = eEvent_MouseWheel;
                Event.mPosition = cPosition(SysEvent.wheel.x, SysEvent.wheel.y);
                
                break;

			case SDL_MOUSEBUTTONDOWN:

				switch ( SysEvent.button.button ) {

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

				switch ( SysEvent.button.button ) {

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
			mEvents.push_back( Event );
	}

    SDL_GetGlobalMouseState(&mMouseGlobal.mX, &mMouseGlobal.mY);
}

void cWindow::CalculateWindowSize() {
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	while ((mOriginalResolution.mWidth * mScaler) <= (unsigned int) (current.w / 2) && 
			(mOriginalResolution.mHeight * mScaler) <= (unsigned int) (current.h / 2) ) {
		++mScaler;
	}

	SetWindowSize( mScaler );
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

bool cWindow::CanChangeToMultiplier( const int pNewMultiplier ) {
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
	if (!CanChangeToMultiplier( mScaler + 1 )) {

		ToggleFullscreen();
		mWindowMode = false;
		return;
	}

	if (!mWindowMode)
		return;

	SetWindowSize( mScaler + 1 );
}

void cWindow::WindowDecrease() {

	// If we're in full screen mode remove it
	if (!mWindowMode) {
		/*
		while (CanChangeToMultiplier( mScaler )) {
			--mScaler;
		}

		++mScaler;*/

		//mWindowMode = true;
		//SetWindowSize( mScalerPrevious );
		ToggleFullscreen();
		return;
	}

	if (!CanChangeToMultiplier(mScaler - 1 ))
		return;

	SetWindowSize( mScaler - 1 );
}

void cWindow::RenderAt( cSurface* pImage ) {
	SDL_Rect Src, Dest;

	Src.w = mScreenSize.mWidth;
	Src.h = mScreenSize.mHeight;
	Src.x = (int) 16;
	Src.y = (int) 16;

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

bool cWindow::isFullscreen() const {
    return !mWindowMode;
}

bool cWindow::isMouseInside() const {
    const cPosition MouseGlobalPos = GetMousePosition();
    const cPosition WindowPos = GetWindowPosition();
    const cDimension WindowSize = GetWindowSize();

    return (MouseGlobalPos.mX >= WindowPos.mX && MouseGlobalPos.mX < WindowPos.mX + WindowSize.getWidth() &&
            MouseGlobalPos.mY >= WindowPos.mY && MouseGlobalPos.mY < WindowPos.mY + WindowSize.getHeight());
}

bool cWindow::isResized() const {
	return mResized;
}

/**
 * Is either mouse button currently pressed
 */
bool cWindow::isMouseButtonPressed_Global() const {
    return  (SDL_GetGlobalMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) ||
            (SDL_GetGlobalMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
}

/**
 * Is the window currently grabbed
 */
bool cWindow::isGrabbed() const {
    if (SDL_GetWindowGrab(mWindow) == SDL_TRUE)
        return true;

    return false;
}

void cWindow::ToggleFullscreen() {

	if (mWindowMode) {

		mScalerPrevious = mScaler;
		SetWindowSize( CalculateFullscreenSize() );

		SDL_SetWindowFullscreen( mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
		mWindowMode = false;
	} else {
		mWindowMode = true;
		SetWindowSize( mScalerPrevious );
	}
}

void cWindow::ClearResized() {
	mResized = false;
}

void cWindow::SetMouseWindowPosition( const cPosition& pPosition ) {

	SDL_WarpMouseInWindow( mWindow, pPosition.getX(), pPosition.getY() );
}

cPosition cWindow::GetMousePosition( const bool pRelative ) const {
    if(!pRelative)
        return mMouseGlobal;

    return (mMouseGlobal - GetWindowPosition());
}

void cWindow::SetMousePosition(const cPosition& pPosition) {

    SDL_WarpMouseGlobal(pPosition.getX(), pPosition.getY());
}

void cWindow::SetScreenSize( const cDimension& pDimension ) {

	mScreenSize = pDimension;
}

/**
 * Set the window size / resolution using the aspect ratio of the original resolution
 * 
 * @param pDimension Original resolution
 */
void cWindow::SetOriginalRes( const cDimension& pDimension ) {

	mOriginalResolution = pDimension;
	
	if (!mWindowMode) {
		ToggleFullscreen();
		ToggleFullscreen();
	} else
		SetWindowSize( mScaler );
}

void cWindow::SetWindowTitle( const std::string& pWindowTitle ) {

	SDL_SetWindowTitle( mWindow, pWindowTitle.c_str() );
}

void cWindow::SetWindowSize( const int pMultiplier ) {
	mScaler = pMultiplier;

	if (mWindow) {
		if (mWindowMode) {
			SDL_SetWindowFullscreen( mWindow, 0 );
		}
		SDL_SetWindowSize( mWindow, GetWindowSize().mWidth, GetWindowSize().mHeight );
		PositionWindow();
		
		mResized = true;
	}
}

cPosition cWindow::GetWindowPosition() const {
    cPosition Pos;

    SDL_GetWindowPosition(mWindow, &Pos.mX, &Pos.mY);

    return Pos;
}

cDimension cWindow::GetWindowSize() const {
	return cDimension( mOriginalResolution.mWidth * mScaler, mOriginalResolution.mHeight * mScaler ); 
}

bool cWindow::HasFocus() {
    if (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MOUSE_FOCUS)
        return true;

    return false;
}

cDimension cWindow::GetScale() const {
    cDimension Result = GetWindowSize() / GetScreenSize();
    if (Result.mHeight == 0)
        Result.mHeight = 1;
    if (Result.mWidth == 0)
        Result.mWidth = 1;
    return Result;
}
