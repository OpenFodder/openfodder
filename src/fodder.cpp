#include "stdafx.h"
#include "resources.hpp"
#include "fodder.hpp"
#include "graphics/window.h"
#include "graphics/surface.hpp"
#include "screen.hpp"
#include "mission.hpp"
#include "map.hpp"
#include "tiles.hpp"

#include<fstream>

int eventFilter( const SDL_Event *e ) {
    if( e->type == SDL_VIDEORESIZE ) {

		if( g_Fodder->windowSize( e->resize.w,e->resize.h ) == false ) {

			return -1;
		}
    }

    return 1; // return 1 so all events are added to queue
}

cFodder::cFodder() {
	mRedraw = true;

	mResources = new cResources();

	mScreen = new cScreen("Open Cannon Fodder");
	mScreen->scaleSet(2);

	mMission = new cMission();

	mQuit = false;
}

cFodder::~cFodder() {

	delete mResources;
	delete mScreen;
	delete mMission;
}

void cFodder::extractDat( string pOutPath ) {
	size_t size = 0;
	string oFilename;
	vector< cResource_File >			*files = mResources->filesGet();
	vector< cResource_File >::iterator  fileIT;

	cout << "Extracting..." << endl;

	for( fileIT = files->begin(); fileIT != files->end(); ++fileIT ) {
		cout << "  " << fileIT->mName;

		if(pOutPath.size()) {
			oFilename = pOutPath;
			
			// No trailing slash?
			if(oFilename[ oFilename.size() - 1 ] != '\\' &&
				oFilename[ oFilename.size() - 1 ] != '/' )
				oFilename.append("\\");
		}

		oFilename.append(fileIT->mName);

		byte *stream = mResources->fileGet(fileIT->mName, size);

		ofstream a(oFilename, ios::binary);
		a.write( (const char*) stream, size );
		a.close();
	}
}

void cFodder::showImage( string pFilename ) {

	cSurface *img = mResources->imageLoad( pFilename, 0x100 );
	
	mScreen->blit( img, 0, 0 );
	mScreen->windowUpdate();
}

bool cFodder::windowSize( size_t pWidth, size_t pHeight ) {
	size_t scale = mScreen->mScaleGet();
	size_t maxX = (mMission->mapGet()->mTilesXGet() + 1) * 16;
	size_t maxY = (mMission->mapGet()->mTilesYGet() + 1) * 16;

	maxX *= scale;
	maxY *= scale;

	if( pWidth > maxX )
		pWidth = maxX;

	if( pHeight > maxY )
		pHeight = maxY;

	if( pWidth < 16 * scale)
		pWidth = 16 * scale;

	if( pHeight < 16 * scale )
		pHeight = 16 * scale;

	mScreen->resize( pWidth, pHeight, true );
	screenDraw();
	return true;
}

void cFodder::screenDraw() {
	mRedraw = false;
	
	playfieldDraw();
}

void cFodder::playfieldDraw() {
	size_t width = mScreen->mWidthGet(), height = mScreen->mHeightGet();

	// number of tiles to draw
	width /= 16; height /= 16; 

	++width; ++height;

	cSurface *surface = mMission->mapGet()->surfaceLandscapeGet(width,height);
	surface->paletteLoad( mMission->mapGet()->tilesGet()->mPaletteGet(0), 0x100 );
	
	mScreen->blit( surface, 0, 0 );
	mScreen->windowUpdate();
}

void cFodder::Start() {
	//showImage( "junsub0.blk" );

	mMission->mapLoad(true);

    while(!mQuit) {
        SDL_Event e;

        while(SDL_PollEvent(&e) ) {

            if( e.type == SDL_QUIT )
                mQuit = true;
        }

		if(mRedraw)
			screenDraw();	
	}

}
