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

cWindow::cWindow() {

	mDimensionWindow.mWidth = 1280;
	mDimensionWindow.mHeight = 960;

	mDimensionPlayfield.mWidth = mDimensionWindow.mWidth;
	mDimensionPlayfield.mHeight = mDimensionWindow.mHeight;
}

cWindow::~cWindow() {

	TTF_CloseFont( mFont );

	SDL_DestroyRenderer( mRenderer );
	SDL_DestroyWindow( mWindow );

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool cWindow::InitWindow( const std::string& pWindowTitle ) {
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		// TODO: Log error
		//SDL_GetError();
		return false;
	}
	
	if (TTF_Init() == -1) {
		// TODO: Log error
		//TTF_GetError();
		return false;
	}

	mFont = TTF_OpenFont( "micross.ttf", 20 );
	if (!mFont) {
		// TODO: Log error
		//TTF_GetError();
	}

	mWindow = SDL_CreateWindow(pWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mDimensionWindow.mWidth, mDimensionWindow.mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if (!mWindow) {
		// TODO: Log Error
		//SDL_GetError();
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		// TODO: Log Error
		//SDL_GetError()
		return false;
	}

	SDL_RenderSetLogicalSize(mRenderer, 320, 200);

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
	Src.w = mDimensionPlayfield.mWidth;
	Src.h = mDimensionPlayfield.mHeight;
	Src.x = pSource.mX;
	Src.y = pSource.mY;

	//Draw the texture
	SDL_RenderCopy( mRenderer, pImage->GetTexture(), &Src, NULL);
}
/*
void cWindow::RenderAt( cImage* pImage, cPosition pSource, cDimension pSourceDimension, cPosition pDestination, cDimension pDestinationDimension  ) {
	SDL_Rect Src;
	SDL_Rect Dest;
	
	Src.w = pSourceDimension.mWidth;
	Src.h = pSourceDimension.mHeight;
	Src.x = pSource.mX;
	Src.y = pSource.mY;

	Dest.x = pDestination.mX;
	Dest.y = pDestination.mY;
	Dest.w = pDestinationDimension.mWidth;
	Dest.h = pDestinationDimension.mHeight;

	//Draw the texture
	SDL_RenderCopy( mRenderer, pImage->GetTexture(), &Src, &Dest);
}

cImage* cWindow::RenderText( const std::wstring& pString ) {
	SDL_Color TextColor;
	TextColor.b = 255;

	SDL_Surface* textSurface = TTF_RenderText_Solid( mFont, Sqrat::wstring_to_string( pString ).c_str(), TextColor );
	SDL_Texture* texture = SDL_CreateTextureFromSurface( mRenderer, textSurface );
	SDL_FreeSurface( textSurface );

	cImage* Image = new cImage( texture );

	return Image;
}
*/
void cWindow::FrameEnd() {

	SDL_RenderPresent( mRenderer );

	SDL_RenderClear( mRenderer );
}

void cWindow::SetCursor() {
	SDL_Cursor* cursor;

	cursor = 	SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_CROSSHAIR );
	SDL_SetCursor(cursor);

}

void cWindow::SetMousePosition( const cPosition& pPosition ) {

	SDL_WarpMouseInWindow( mWindow, pPosition.mX, pPosition.mY );
}

/*
const cDimension cWindow::GetDimensionWindow() const {
	
	return mDimensionWindow;
}

const cDimension cWindow::GetDimensionPlayfield() const {

	return mDimensionPlayfield;
}
*/
SDL_Renderer* cWindow::GetRenderer() const {

	return mRenderer;
}
