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

class cWindow {
	
	protected:
		SDL_Window*			mWindow;
		SDL_Renderer*		mRenderer;

        std::vector<cEvent> mEvents;

		cDimension			mOriginalResolution;

		cDimension			mScreenSize;
        cPosition           mMouseGlobal;

		uint8				mScaler, mScalerPrevious;

		bool				mWindowMode;
        bool                mHasFocus;
        bool                mCursorGrabbed;
		bool				mResized;

	protected:


	public:

							cWindow();
		virtual				~cWindow();

		void		        CalculateWindowSize();
		int16				CalculateFullscreenSize();

		bool				CanChangeToMultiplier( const int pNewMultiplier );

        bool                Cycle();

        std::vector<cEvent>* EventGet();
		virtual void		EventCheck();
		virtual void		FrameEnd();

		virtual bool		InitWindow( const std::string& pWindowTitle );
			
		virtual void		PositionWindow();

		virtual void		RenderAt( cSurface* pImage );
		virtual void		RenderShrunk( cSurface* pImage );

		void				WindowIncrease();
		void				WindowDecrease();

        bool                isFullscreen() const;
        bool                isGrabbed() const;
		bool				isMouseInside() const;
		bool				isResized() const;
        bool                isMouseButtonPressed_Global() const;

		void				SetCursor();

        cPosition           GetMousePosition(const bool pRelative = false) const;
        void				SetMousePosition(const cPosition& pPosition);
		void				SetMouseWindowPosition( const cPosition& pPosition );

		void				SetScreenSize( const cDimension& pDimension );
		void				SetOriginalRes( const cDimension& pDimension );

		void				SetWindowTitle( const std::string& pWindowTitle );
		void				SetWindowSize(const int pMultiplier);

		void				ToggleFullscreen();
		void				ClearResized();

		SDL_Renderer*		GetRenderer() const { return mRenderer; };

        cPosition           GetWindowPosition() const;
		int32				GetWindowWidth() const;
		int32				GetWindowHeight() const;

		cDimension	        GetWindowSize() const;
		cDimension	        GetScreenSize() const { return mScreenSize; }
		bool			    GetWindowMode() const { return mWindowMode; }
        bool                HasFocus();
        bool                hasFocusEvent() const { return mHasFocus; }
        bool                GetMouseGrabbed() const { return mCursorGrabbed; }
        cDimension          GetScale() const;
};

class cWindowNull : public cWindow {

public:
	virtual ~cWindowNull() {}
    virtual bool InitWindow(const std::string& pWindowTitle);
};