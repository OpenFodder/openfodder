class cResources;
class cVideoWindow;

class cFodder {

private:
	cResources				*mResources;
	cVideoWindow			*mWindow;

public:
		

							 cFodder();
							~cFodder();

	void					 extractDat( string pOutPath = "data");

	void					 Start();
};
