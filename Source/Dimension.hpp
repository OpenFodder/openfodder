
class cDimension {
	public:
	unsigned int mWidth, mHeight;

	public:
		cDimension() : mWidth( 0 ), mHeight( 0 ) {}
		cDimension( unsigned int pWidth, unsigned int pHeight ) : mWidth( pWidth ), mHeight( pHeight ) {}

		void Set( unsigned int pWidth, unsigned int pHeight ) { mWidth = pWidth; mHeight = pHeight; }

};
