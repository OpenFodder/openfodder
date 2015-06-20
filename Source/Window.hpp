class cWindow : public cSingleton<cWindow> {
	
	private:
		SDL_Window*			mWindow;
		SDL_Renderer*		mRenderer;
		cDimension			mDimensionWindow, mDimensionPlayfield;
		
		TTF_Font*			mFont;

	public:

							cWindow();
							~cWindow();

		bool				InitWindow( const std::string& pWindowTitle, bool pExtended = false );
		void				EventCheck();

		void				FrameEnd();

		void				RenderAt( cSurface* pImage, cPosition pSource );
	/*	void				RenderAt( cImage* pImage, cPosition pSource, cDimension pSourceDimension, cPosition pDestination, cDimension pDestinationDimension );
		cImage*				RenderText( const std::wstring& pString );*/

		void				SetCursor();
		void				SetMousePosition( const cPosition& pPosition );

		SDL_Renderer*		GetRenderer() const;

};
