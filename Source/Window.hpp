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

class cWindow : public cSingleton<cWindow> {
	
	private:
		SDL_Window*			mWindow;
		SDL_Renderer*		mRenderer;
		cDimension			mDimensionWindow, mDimensionPlayfield;
		cDimension			mScreenSize;

	public:

							cWindow();
							~cWindow();

		bool				InitWindow( const std::string& pWindowTitle );
		void				EventCheck();

		void				FrameEnd();

		void				RenderAt( cSurface* pImage, cPosition pSource );

		void				SetCursor();
		void				SetMousePosition( const cPosition& pPosition );
		void				SetScreenSize( const cDimension& pDimension );
		void				SetLogicalSize( const cDimension& pDimension );
		void				SetWindowTitle( const std::string& pWindowTitle );

		SDL_Renderer*		GetRenderer() const;

		const cDimension	GetWindowSize() const { return mDimensionWindow; }
};
