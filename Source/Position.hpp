/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2017 Robert Crossfield
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
		int64 mX, mY;

	public:
				cPosition() : mX( 0 ), mY( 0 ) {}
				cPosition( unsigned int pX, unsigned int pY ) : mX( pX ), mY( pY ) {}
				cPosition( const cDimension pDimension ) { mX = pDimension.mWidth; mY = pDimension.mHeight; }

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
