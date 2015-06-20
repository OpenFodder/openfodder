
class cPosition {
	public:
		int64 mX, mY;

	public:
				cPosition() : mX( 0 ), mY( 0 ) {}
				cPosition( unsigned int pX, unsigned int pY ) : mX( pX ), mY( pY ) {}

		void	Set( unsigned int pX, unsigned int pY ) { mX = pX; mY = pY; }

		bool operator== (const cPosition& pPosition) const {

			return pPosition.mX == mX && pPosition.mY == mY;

		}

		cPosition& operator+= (const cPosition& pPosition) {

			mX += pPosition.mX;
			mY += pPosition.mY;

			return *this;
		}

		cPosition& operator-= (const cPosition& pPosition) {

			mX -= pPosition.mX;
			mY -= pPosition.mY;

			return *this;
		}
};
