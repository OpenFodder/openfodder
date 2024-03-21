/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

class cDimension {
	public:
	unsigned int mWidth, mHeight;

	public:
		cDimension() : mWidth( 0 ), mHeight( 0 ) {}
		cDimension( unsigned int pWidth, unsigned int pHeight ) : mWidth( pWidth ), mHeight( pHeight ) {}

		void Set( unsigned int pWidth, unsigned int pHeight ) { mWidth = pWidth; mHeight = pHeight; }

		unsigned int WidthByHeight() { return mWidth * mHeight; }
		int getWidth() const { return mWidth; }
		int getHeight() const { return mHeight; }

        cPosition getCentre() const { return { mWidth / 2, mHeight / 2 }; };

        cDimension operator/(const cDimension& pDim) {
            return cDimension(mWidth / pDim.mWidth, mHeight / pDim.mHeight);
        }

        bool operator==(const cDimension& pRight) {
            return mWidth == pRight.mWidth && mHeight == pRight.mHeight;
        }

		cDimension operator+(const cDimension& pRight) {

			return cDimension(mWidth + pRight.mWidth, mHeight + pRight.mHeight);
		}
};
