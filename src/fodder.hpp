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

	// Tools
	void					 extractDat( string pOutPath = "data");
	void					 showImage( string pFilename );

	void					 playfieldDraw();
	void					 Start();

	bool					 windowSize( size_t pWidth, size_t pHeight );

	inline cScreen			*screenGet()	{ return mScreen; }
	inline cResources		*resourcesGet() { return mResources; }
};

extern cFodder	*g_Fodder;
