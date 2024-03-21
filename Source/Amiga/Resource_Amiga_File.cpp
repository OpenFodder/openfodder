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

#include "stdafx.hpp"
#include "Amiga/dernc.hpp"
#include "Amiga/Resource_Amiga_File.hpp"

cResource_Amiga_File::cResource_Amiga_File(  ) : cResources() {

}

cResource_Amiga_File::~cResource_Amiga_File() {

}

tSharedBuffer cResource_Amiga_File::fileGet( std::string pFilename ) {

	auto File = cResources::fileGet( pFilename );
	if (!File->size()) {
		//std::cout << "File " << pFilename << " Not Found!\n";
		//exit( 1 );
		return File;
	}
	uint32 Header = readBEDWord( File->data() );

	if (Header != 'RNC\01')
		return File;

	uint32 Size = readBEDWord( File->data() + 4 );

	auto Unpacked = std::make_shared<std::vector<uint8>>();
	Unpacked->resize( Size );

	rnc_unpack (File->data(), Unpacked->data());

	return Unpacked;
}
