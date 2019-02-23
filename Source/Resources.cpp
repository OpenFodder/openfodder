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

#include "stdafx.hpp"
#include "Amiga/dernc.hpp"

cResources::cResources( ) {

}

tSharedBuffer cResources::fileGet( std::string pFilename ) {
	auto File = g_ResourceMan->FileRead(g_Fodder->mVersionCurrent->getDataFilePath(pFilename));

	if (File->size()) {

		return fileDeRNC(File);
	}
	return File;
}

tSharedBuffer cResources::fileDeRNC(tSharedBuffer pBuffer) {
	uint32 Header = readBEDWord(pBuffer->data());
	if (Header != 'RNC\01')
		return pBuffer;

	uint32 Size = readBEDWord(pBuffer->data() + 4);

	auto Unpacked = std::make_shared<std::vector<uint8>>();
	Unpacked->resize(Size);
	rnc_unpack(pBuffer->data(), Unpacked->data());
	return Unpacked;
}

size_t cResources::fileLoadTo( const std::string& pFilename, uint8* pTarget ) {

	auto File = fileGet( pFilename );

	if (File->size()) {
		std::memcpy( pTarget, File->data(), File->size() );
		return File->size();
	}

	return false;
}
