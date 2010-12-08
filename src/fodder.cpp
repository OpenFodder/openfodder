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

		g_Fodder->windowSize( e->resize.w,e->resize.h );

		//draw();
    }

    return 1; // return 1 so all events are added to queue
}

void cFodder::windowSize( size_t pWidth, size_t pHeight ) {
	mScreen->resize( pWidth, pHeight );
}

cFodder::cFodder() {
	mResources = new cResources();

	mScreen = new cScreen("Open Fodder");
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

void cFodder::Start() {
	//showImage( "junsub0.blk" );

	mMission->mapLoad(true);

	byte *tileB = mMission->mapGet()->tilesGet()->tileGet(387);

	cSurface tile(16,16);
	tile.decode( tileB, 16 * 16, 0, 0 );
	tile.paletteLoad( mMission->mapGet()->tilesGet()->mPaletteGet(), 0x100 );

	mScreen->blit( &tile, 0, 0 );
	mScreen->windowUpdate();

    while(!mQuit) {
        SDL_Event e;

        while(SDL_PollEvent(&e) ) {

            if( e.type == SDL_QUIT )
                mQuit = true;
        }

		
	}

}
