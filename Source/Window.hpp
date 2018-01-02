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

class cWindow : public cSingleton<cWindow> {
	
	protected:
		SDL_Window*			mWindow;
		SDL_Renderer*		mRenderer;

		cDimension			mOriginalResolution;

		cDimension			mScreenSize;

		uint8				mWindow_Multiplier, mWindow_MultiplierPrevious;

		bool				mWindowMode;

	protected:

		void				SetWindowSize( const int pMultiplier  );

	public:

							cWindow();
							~cWindow();

		void				CalculateWindowSize();
		int16				CalculateFullscreenSize();

		bool				CanChangeToMultiplier( int pNewMultiplier );

		virtual void		EventCheck();
		virtual void		FrameEnd();

		virtual bool		InitWindow( const std::string& pWindowTitle );
			
		virtual void		PositionWindow();

		virtual void		RenderAt( cSurface* pImage, cPosition pSource = cPosition(0,0) );
		virtual void		RenderShrunk( cSurface* pImage );

		void				WindowIncrease();
		void				WindowDecrease();

		void				SetCursor();
		void				SetMousePosition( const cPosition& pPosition );
		void				SetScreenSize( const cDimension& pDimension );
		void				SetOriginalRes( const cDimension& pDimension );

		void				SetWindowTitle( const std::string& pWindowTitle );

		void				SetFullScreen();

		SDL_Renderer*		GetRenderer() const { return mRenderer; };

		const cDimension	GetWindowSize() const;
		const cDimension	GetScreenSize() const { return mScreenSize; }
		const bool			GetWindowMode() const { return mWindowMode; }
};
