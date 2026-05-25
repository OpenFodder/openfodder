/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
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
	constexpr uint32 RNCHeaderSize = 18;
	constexpr uint32 MaxUnpackedSize = 64 * 1024 * 1024;
	const auto InvalidRNC = []() { return std::make_shared<std::vector<uint8>>(); };

	if (pBuffer->size() < RNCHeaderSize)
		return pBuffer;

	uint32 Header = readBEDWord(pBuffer->data());
	if (Header != 'RNC\01')
		return pBuffer;

	uint32 Size = readBEDWord(pBuffer->data() + 4);
	uint32 PackedSize = readBEDWord(pBuffer->data() + 8);

	if ((PackedSize > (pBuffer->size() - RNCHeaderSize)) || !Size || (Size > MaxUnpackedSize))
		return InvalidRNC();

	auto Unpacked = std::make_shared<std::vector<uint8>>();
	Unpacked->resize(Size);

	long Result = rnc_unpack(pBuffer->data(), Unpacked->data());
	if (Result != static_cast<long>(Size))
		return InvalidRNC();

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
