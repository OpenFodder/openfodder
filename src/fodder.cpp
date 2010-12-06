#include "stdafx.h"
#include "resources.hpp"
#include "fodder.hpp"
#include "graphics/window.h"

#include<fstream>

cFodder::cFodder() {

	mResources = new cResources();
	mWindow	= new cVideoWindow(320,200,4,false);
}

cFodder::~cFodder() {

	delete mResources;
	delete mWindow;
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

void cFodder::Start() {
	//size_t size = 0;

	//extractDat();

	//byte *stream = mResources->fileGet("", size);


//	tool_EndianSwap( stream, size );
}
