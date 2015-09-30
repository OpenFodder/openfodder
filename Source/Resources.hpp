/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield <robcrossfield@gmail.com>
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

class cSurface;

struct cResource_File {
	std::string		 mName;
	uint32		 mAddressStart;
	uint32		 mSize;

	cResource_File( std::string pName, uint32 pStart, uint32 pEnd ) : 
		mName( pName ), mAddressStart( pStart ), mSize( pEnd ) { }

};

class cResources : public cSingleton<cResources> {
protected:
	std::string						mDataPath;



public:
	 								cResources( std::string pDataPath = "" );
	virtual							~cResources() { };
									 
	virtual uint8*					fileGet( std::string pFilename, size_t &pFileSize );
	size_t							fileLoadTo( const std::string& pFilename, uint8* pTarget );
};
