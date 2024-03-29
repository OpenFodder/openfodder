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

class cSurface;

struct cResource_File {
	std::string		mName;
	uint32			mAddressStart;
	uint32			mSize;

	cResource_File( std::string pName, uint32 pStart, uint32 pEnd ) : 
		mName( pName ), mAddressStart( pStart ), mSize( pEnd ) { }

};

class cResources {
protected:

	tSharedBuffer					fileDeRNC(tSharedBuffer pBuffer);

public:
	 								cResources( );
	virtual							~cResources() { };
						
	virtual tSharedBuffer 			fileGet( std::string pFilename );

	size_t							fileLoadTo( const std::string& pFilename, uint8* pTarget );
};
