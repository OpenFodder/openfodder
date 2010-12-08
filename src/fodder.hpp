class cResources;
class cScreen;
class cMission;

class cFodder {

private:
	cResources				*mResources;
	cScreen					*mScreen;
	cMission				*mMission;

	size_t					 mMapCurrent;

	bool					 mRedraw, mQuit;


	void					 screenDraw();

public:
		

							 cFodder();
							~cFodder();

	void					 extractDat( string pOutPath = "data");
	void					 showImage( string pFilename );

	void					 Start();

	void					 windowSize( size_t pWidth, size_t pHeight );

	inline cResources		*resourcesGet() { return mResources; }
};

extern cFodder	*g_Fodder;
