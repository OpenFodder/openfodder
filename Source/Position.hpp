/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

class cPosition {
	public:
		int32 mX, mY;

	public:
				cPosition() : mX( 0 ), mY( 0 ) {}
				cPosition( unsigned int pX, unsigned int pY ) : mX( pX ), mY( pY ) {}

		void	Set( unsigned int pX, unsigned int pY ) { mX = pX; mY = pY; }
        void Clear() { mX = 0; mY = 0; }
		
		int32 distanceTo(const cPosition& pPosition) const {

			int32 X = mX - pPosition.mX;
			int32 Y = mY - pPosition.mY;

			if (X < 0)
				X = -X;
			if (Y < 0)
				Y = -Y;

			return X + Y;
		}

		bool operator== (const cPosition& pPosition) const {

			return pPosition.mX == mX && pPosition.mY == mY;

		}

		bool operator!= (const cPosition& pPosition) const {

			return !(*this == pPosition);

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
        
        cPosition operator-(const cPosition& pPosition) const {
            
            return cPosition( mX - pPosition.mX, mY - pPosition.mY );
        }

        cPosition operator+(const cPosition& pPosition) const {

            return cPosition(mX + pPosition.mX, mY + pPosition.mY);
        }

		cPosition* operator*(const size_t pMultiplier)  {
			mX *= pMultiplier;
			mY *= pMultiplier;

			return this;
		}

		bool operator<(const cPosition& pRight) const {

			if (mY < pRight.mY)
				return true;

			return mX < pRight.mX;
		}
		int getY() const { return (int) mY; }
		void setY(int pY) { mY = pY; }

		int getX() const { return (int) mX; }
		void setX(int pX) { mX = pX; }
        inline int16 x() const {
            return (int16)mX;
        }

        inline int16 y() const {
            return (int16)mY;
        }
};
