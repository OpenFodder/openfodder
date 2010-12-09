#include "stdafx.h"
#include "graphics/window.h"
#include "screen.hpp"
#include "graphics/scale/scalebit.h"
#include "graphics/surface.hpp"
#include "../rev.h"

cScreen::cScreen( string pWindowTitle ) {
	mWidth = 320;
	mHeight = 200;
	
	mSurface = 0;
	mSDLSurfaceScaled = 0;

	mWindow = 0;
	
	mWindowTitle = pWindowTitle;

	mScale = 0;
	mFullScreen = false;
}

void cScreen::levelNameSet( string pName ) {
	stringstream windowTitle;

	windowTitle << mWindowTitle;
	windowTitle << ". '";
	windowTitle << pName;
	windowTitle << "' (SVN: " << SVNREV;
	windowTitle << "-" << SVNDATE << ")";

	if(mWindow)
		mWindow->titleSet( windowTitle.str() );
}

void cScreen::scaleSet( byte pScale ) {
	word width, height;

	// Invalid scale level?
	if(pScale > 4)
		return;

	// Cleanup previous resources
	delete mWindow;
	delete mSurface;
	SDL_FreeSurface( mSDLSurfaceScaled );

	// Set new scale level
	mScale = pScale;

	// Set new width/height
	width = mWidth * mScale;
	height = mHeight * mScale;

	// Create window and a surface to scale to
	mWindow = new cVideoWindow( width, height, 4, mFullScreen );
	mSurface = new cSurface( mWidth, mHeight );

	mSDLSurfaceScaled =	SDL_CreateRGBSurface(	SDL_SWSURFACE,	width, height,	 32, 0, 0, 0, 0);
}

void cScreen::resize( size_t pWidth, size_t pHeight, bool pUseScale ) {
	mWidth = pWidth;
	mHeight = pHeight;
	
	if( pUseScale ) {
		mWidth /= mScale;
		mHeight /= mScale;
	}

	scaleSet(mScale);
}

void cScreen::blit( cSurface *pSurface, size_t pDestX, size_t pDestY ) {
	
	mSurface->blitFrom( pSurface, pDestX, pDestY, 0x00, 0xFF  );
}

SDL_Surface *cScreen::surfaceGet() {
	return mSurface->surfaceGet();
}

SDL_Surface	*cScreen::scaleUp( ) {
	SDL_Surface *src = mSurface->surfaceGet();

	if( mScale < 2 || mScale > 4 ) {

		return src;
	}

	SDL_SetColorKey(mSDLSurfaceScaled, SDL_SRCCOLORKEY, SDL_MapRGB(mSDLSurfaceScaled->format, 0, 0, 0)	);

	// Do the scale
	scale(mScale, mSDLSurfaceScaled->pixels, mSDLSurfaceScaled->pitch, src->pixels, src->pitch, src->format->BytesPerPixel, src->w, src->h);

	return mSDLSurfaceScaled;
}

void cScreen::windowUpdate() {
	mWindow->clear(0);
	mWindow->blit( scaleUp(), 0, 0 );

}

void cScreen::wipe( size_t pColor ) {
	mSurface->wipe( pColor );
}

void cScreen::wipe( size_t pX, size_t pY, size_t pSizeX, size_t pSizeY, size_t pColor ) {
	mSurface->wipe( pX, pY, pSizeX, pSizeY, pColor );
}
