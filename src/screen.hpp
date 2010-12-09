
class cVideoWindow;
class cSurface;

class cScreen {
private:
	string					 mWindowTitle;

	cSurface				*mSurface;
	SDL_Surface				*mSDLSurfaceScaled;
	
	cVideoWindow			*mWindow;

	bool					 mFullScreen;
	size_t					 mScale, mWidth, mHeight;

public:

							 cScreen( string pWindowTitle );
	void					 blit( cSurface *pSurface, size_t pDestX = 0, size_t pDestY = 0 );

	void					 scaleSet( byte pScale ) ;
	SDL_Surface				*scaleUp( );
	SDL_Surface				*scaleGet()	{ return mSDLSurfaceScaled; }
	
	void					 resize( size_t pWidth, size_t pHeight, bool pUseScale );

	SDL_Surface				*surfaceGet();

	void					 windowUpdate();
	void					 levelNameSet( string pName );

	void					 wipe( size_t pColor = 0x00 );
	void					 wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor );

	inline size_t			 mWidthGet() { return mWidth; }
	inline size_t			 mHeightGet() { return mHeight; }
	inline size_t			 mScaleGet()	{ return mScale; }

};
