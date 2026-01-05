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

cWindow::cWindow() {

	mOriginalResolution.mWidth = 320;
	mOriginalResolution.mHeight = 200;

	mScaler = mScalerPrevious = 2;

	mScreenSize.mWidth = mOriginalResolution.mWidth;
	mScreenSize.mHeight = mOriginalResolution.mHeight;

	mWindowMode = true;

	mWindow = 0;

	mRenderer = 0;

    mHasFocus = true;
	mResized = false;
}

cWindow::~cWindow() {

	SDL_DestroyRenderer( mRenderer );
	SDL_DestroyWindow( mWindow );

	SDL_Quit();
}

bool cWindow::InitWindow( const std::string& pWindowTitle ) {
	
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        g_Debugger->Error("Failed to initialise SDL");
		exit( 1 );
		return false;
	}
	
    ToggleFullscreen();

	mWindow = SDL_CreateWindow(pWindowTitle.c_str(), GetWindowSize().mWidth, GetWindowSize().mHeight, 0);
	if (!mWindow) {
        g_Debugger->Error("Failed to create window");
		exit( 1 );
		return false;
	}

    PositionWindow();
	mRenderer = SDL_CreateRenderer(mWindow, nullptr);
	if (!mRenderer) {
        g_Debugger->Error("Failed to create rendered");
		exit( 1 );
		return false;
	}

	SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_WARP_MOTION, "1", SDL_HINT_OVERRIDE);
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SPEED_SCALE, "3.0");


    if (g_Fodder->mParams->mWindowMode) {
        ToggleFullscreen();
        CalculateWindowSize();
    }
    else {
        ToggleFullscreen();
        ToggleFullscreen();
    }

	if (!g_Fodder->mParams->mMouseAlternative || (g_Fodder->mParams->mMouseAlternative && g_Fodder->mParams->mMouseLocked)) {
		SDL_SetWindowRelativeMouseMode(mWindow, true);

		// Warp mouse to 0,0 to fix an issue with the mouse cursor appearing on first click in the window
		SDL_WarpMouseInWindow(mWindow, 0, 0);
	}
	
	SDL_HideCursor();
	
	return true;
}

void cWindow::ToggleVSync(bool pEnabled) {
	#ifdef EMSCRIPTEN
	return;
	#endif
	SDL_SetRenderVSync(mRenderer, pEnabled ? 1 : 0);
}

void cWindow::SetRelativeMouseMode(bool pEnable) {

    if (mWindow) {
        SDL_SetWindowRelativeMouseMode(mWindow, pEnable);
    }
}

std::vector<cEvent>* cWindow::EventGet() {
    return &mEvents;
}

bool cWindow::Cycle() {

    EventCheck();


    return true;
}

void cWindow::EventCheck() {
	SDL_Event SysEvent;

	while (SDL_PollEvent(&SysEvent)) {

		cEvent Event;

		switch (SysEvent.type) {
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			Event.mType = eEvent_Focus;
			Event.mHasFocus = false;
			mHasFocus = false;
			break;

		case SDL_EVENT_WINDOW_FOCUS_GAINED:
			Event.mType = eEvent_Focus;
			Event.mHasFocus = true;
			mHasFocus = true;
			break;

		case SDL_EVENT_KEY_DOWN:
			Event.mType = eEvent_KeyDown;
			Event.mButton = SysEvent.key.scancode;
			break;

		case SDL_EVENT_KEY_UP:
			Event.mType = eEvent_KeyUp;
			Event.mButton = SysEvent.key.scancode;
			break;

		case SDL_EVENT_FINGER_MOTION:
			Event.mType = eEvent_MouseMove;
			Event.mPosition = cPosition((unsigned int)(SysEvent.tfinger.x * GetWindowWidth()),
										(unsigned int)(SysEvent.tfinger.y * GetWindowHeight()));
			break;

		case SDL_EVENT_FINGER_DOWN:
			
			Event.mType = eEvent_MouseLeftDown;
			Event.mButton = 1;
			Event.mPosition = cPosition((unsigned int)(SysEvent.tfinger.x * GetWindowWidth()),
										(unsigned int)(SysEvent.tfinger.y * GetWindowHeight()));

			Event.mButtonCount = 1;

			break;

		case SDL_EVENT_FINGER_UP:
			Event.mType = eEvent_MouseLeftUp;
			Event.mButton = 1;

			Event.mPosition = cPosition((unsigned int)(SysEvent.tfinger.x * GetWindowWidth()),
										(unsigned int)(SysEvent.tfinger.y * GetWindowHeight()));

			Event.mButtonCount = 1;
			mEvents.push_back(Event);

			Event.mType = eEvent_MouseRightUp;
			Event.mButton = 3;

			break;

		case SDL_EVENT_MOUSE_MOTION:
			if (SysEvent.motion.xrel < -100 || 
				SysEvent.motion.yrel < -100 || 
				SysEvent.motion.xrel > 100 || 
				SysEvent.motion.yrel > 100) {
				break;
			}

			Event.mType = eEvent_MouseMove;
			Event.mPosition = cPosition((int)SysEvent.motion.x, (int)SysEvent.motion.y);
			Event.mPositionRelative = cPosition((int)SysEvent.motion.xrel, (int)SysEvent.motion.yrel);
			break;

		case SDL_EVENT_MOUSE_WHEEL:
			Event.mType = eEvent_MouseWheel;
			Event.mPosition = cPosition((int)SysEvent.wheel.x, (int)SysEvent.wheel.y);
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:

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

			Event.mPosition = cPosition((int)SysEvent.button.x, (int)SysEvent.button.y);
			Event.mButtonCount = SysEvent.button.clicks;
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:

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

			Event.mPosition = cPosition((int)SysEvent.button.x, (int)SysEvent.button.y);
			Event.mButtonCount = SysEvent.button.clicks;
			break;

		case SDL_EVENT_QUIT:
			Event.mType = eEvent_Quit;
			break;
		}

#ifdef EMSCRIPTEN
		// Drop events which have no x/y
		if (SysEvent.type == SDL_EVENT_MOUSE_MOTION || 
			SysEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN || 
			SysEvent.type == SDL_EVENT_MOUSE_BUTTON_UP) {

			if (Event.mPosition.mX == 0 || Event.mPosition.mY == 0)
				continue;
		}
#endif
		if ( Event.mType != eEvent_None )
			mEvents.push_back( Event );
	}
}

void cWindow::CalculateWindowSize() {
	SDL_DisplayID display = mWindow ? SDL_GetDisplayForWindow(mWindow) : SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* current = SDL_GetCurrentDisplayMode(display);
	if (!current) {
		return;
	}

	while ((mOriginalResolution.mWidth * mScaler) <= (unsigned int) (current->w / 2) && 
			(mOriginalResolution.mHeight * mScaler) <= (unsigned int) (current->h / 2) ) {
		++mScaler;
	}

	SetWindowSize( mScaler );
}

int16 cWindow::CalculateFullscreenSize() {
	SDL_DisplayID display = mWindow ? SDL_GetDisplayForWindow(mWindow) : SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* current = SDL_GetCurrentDisplayMode(display);
	if (!current) {
		return 1;
	}
	int16 Multiplier = 1;

	while ((mOriginalResolution.mWidth * Multiplier) <= (unsigned int) current->w && (mOriginalResolution.mHeight * Multiplier) <= (unsigned int) current->h ) {
		++Multiplier;
	}

	return --Multiplier;
}

bool cWindow::CanChangeToMultiplier( const int pNewMultiplier ) {
	SDL_DisplayID display = mWindow ? SDL_GetDisplayForWindow(mWindow) : SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* current = SDL_GetCurrentDisplayMode(display);
	if (!current) {
		return false;
	}

	if (	(mOriginalResolution.getWidth()  * pNewMultiplier >= current->w ||
			mOriginalResolution.getHeight() * pNewMultiplier >= current->h) ||
			pNewMultiplier <= 0 )
		return false;

	return true;
}

void cWindow::FrameEnd() {
    #ifndef EMSCRIPTEN
	SDL_RenderPresent( mRenderer );
    
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
	SDL_RenderClear( mRenderer );
	#else
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
	//SDL_RenderClear( mRenderer );
#endif
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

		mWindowMode = true;
		SetWindowSize(mScaler);
		//ToggleFullscreen();
		return;
	}

	if (!CanChangeToMultiplier(mScaler - 1))
		return;

	SetWindowSize( mScaler - 1 );
}

void cWindow::RenderAt( cSurface* pImage ) {
	SDL_FRect Src, Dest;

	Src.w = (float)mScreenSize.mWidth;
	Src.h = (float)mScreenSize.mHeight;
	Src.x = 16.0f;
	Src.y = 16.0f;

	if (g_Fodder->mParams->mIntegerScaling || mWindowMode) {
		Dest.w = (float)GetWindowSize().mWidth;
		Dest.h = (float)GetWindowSize().mHeight;
	}
	else {
		int windowW = 0;
		int windowH = 0;
		SDL_GetWindowSize(mWindow, &windowW, &windowH);
		Dest.h = (float)windowH;
		Dest.w = (float)(Dest.h * (float)(4.0/3.0));
	}

	if (mWindowMode) {
		Dest.x = 0.0f;
		Dest.y = 0.0f;
	}
	else {
		SDL_DisplayID display = mWindow ? SDL_GetDisplayForWindow(mWindow) : SDL_GetPrimaryDisplay();
		const SDL_DisplayMode* current = SDL_GetCurrentDisplayMode(display);
		if (current) {
			Dest.x = (float)((current->w - Dest.w) / 2.0f);
			Dest.y = (float)((current->h - Dest.h) / 2.0f);
		} else {
			Dest.x = 0.0f;
			Dest.y = 0.0f;
		}
	}

	SDL_RenderTexture( mRenderer, pImage->GetTexture(), &Src, &Dest );
}

void cWindow::RenderShrunk( cSurface* pImage ) {
	SDL_FRect Src, Dest;
	Src.w = (float)pImage->GetWidth();
	Src.h = (float)pImage->GetHeight();
	Src.x = 0.0f;
	Src.y = 0.0f;

	Dest.w = (float)GetWindowSize().mWidth;
	Dest.h = (float)GetWindowSize().mHeight;

	if (mWindowMode) {
		Dest.x = 0.0f;
		Dest.y = 0.0f;
	}
	else {
		SDL_DisplayID display = mWindow ? SDL_GetDisplayForWindow(mWindow) : SDL_GetPrimaryDisplay();
		const SDL_DisplayMode* current = SDL_GetCurrentDisplayMode(display);
		if (current) {
			Dest.x = (float)((current->w - Dest.w) / 2.0f);
			Dest.y = (float)((current->h - Dest.h) / 2.0f);
		} else {
			Dest.x = 0.0f;
			Dest.y = 0.0f;
		}
	}

	SDL_RenderTexture( mRenderer, pImage->GetTexture(), &Src, &Dest);
}

bool cWindow::isFullscreen() const {
    return !mWindowMode;
}

bool cWindow::isMouseInside() const {
	return mWindow == SDL_GetMouseFocus();
}

bool cWindow::isResized() const {
	return mResized;
}

/**
 * Is either mouse button currently pressed
 */
bool cWindow::isMouseButtonPressed_Global() const {
    return  (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) ||
            (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
}

/**
 * Is the window currently grabbed
 */
bool cWindow::isGrabbed() const {
    if (!mWindow) {
        return false;
    }
    return SDL_GetWindowMouseGrab(mWindow);
}

void cWindow::ToggleFullscreen() {

	if (mWindowMode) {

		mScalerPrevious = mScaler;
		SetWindowSize( CalculateFullscreenSize() );

        if (mWindow) {
		    SDL_SetWindowFullscreen( mWindow, true );
        }
		mWindowMode = false;
	} else {
		mWindowMode = true;
		SetWindowSize( mScalerPrevious );
	}
}

void cWindow::ClearResized() {
	mResized = false;
}

void cWindow::SetMousePosition(const cPosition& pPosition) {

    SDL_WarpMouseGlobal((float)pPosition.getX(), (float)pPosition.getY());
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

    if (mOriginalResolution == pDimension)
        return;

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

	if (pMultiplier < 1 )
		return;

	mScaler = pMultiplier;

	if (mWindow) {
		if (mWindowMode) {
			SDL_SetWindowFullscreen( mWindow, false );
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

int32 cWindow::GetWindowWidth() const {
	return mOriginalResolution.mWidth * mScaler;
}

int32 cWindow::GetWindowHeight() const {
	return mOriginalResolution.mHeight * mScaler;
}

cDimension cWindow::GetScale() const {
    cDimension Result = GetWindowSize() / GetScreenSize();
    if (Result.mHeight == 0)
        Result.mHeight = 1;
    if (Result.mWidth == 0)
        Result.mWidth = 1;
    return Result;
}

int cWindow::GetRefreshRate() {
	SDL_DisplayID display = mWindow ? SDL_GetDisplayForWindow(mWindow) : SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
	if (!mode) {
		std::cerr << "SDL_GetCurrentDisplayMode failed: " << SDL_GetError() << std::endl;
		return 50; // Fallback to 50Hz if query fails
	}
	return (int)mode->refresh_rate;
}

bool cWindowNull::InitWindow(const std::string& pWindowTitle) {


    return true;
}
