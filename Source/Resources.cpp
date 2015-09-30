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

#include "stdafx.hpp"

cResources::cResources( std::string pDataPath ) {


	mDataPath = pDataPath;

}

uint8* cResources::fileGet( std::string pFilename, size_t &pFileSize ) {
	std::vector< cResource_File >::iterator		fileIT;

	std::transform( pFilename.begin(), pFilename.end(), pFilename.begin(), ::tolower );
	
	uint8* File = local_FileRead( pFilename, "", pFileSize );
	if (File)
		return File;

	File = local_FileRead( pFilename, mDataPath.c_str(), pFileSize );
	if (File)
		return File;

	// This is rather hacky
	std::transform( pFilename.begin(), pFilename.end(), pFilename.begin(), ::toupper );
	File = local_FileRead( pFilename, "", pFileSize );
	if (File)
		return File;

	File = local_FileRead( pFilename, mDataPath.c_str(), pFileSize );
	if (File)
		return File;

	return 0;
}

size_t cResources::fileLoadTo( const std::string& pFilename, uint8* pTarget ) {
	size_t Size = 0;

	uint8* File = fileGet( pFilename, Size );

	memcpy( pTarget, File, Size );

	delete[] File;

	return Size;
}
