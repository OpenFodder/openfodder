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

cOriginalMap::cOriginalMap(tSharedBuffer pMapFile, tSharedBuffer pSptFile, const bool pCF2) {

	load(pMapFile, pSptFile, pCF2);
}

void cOriginalMap::saveHeader() {
	uint16* Map = (uint16*)mData->data();

	// Map Marker ('ofed')
	Map[0x28] = 'fo'; Map[0x29] = 'de';

	// Put the map size
	writeBEWord(&Map[0x2A], (uint16)mParams.mWidth);
	writeBEWord(&Map[0x2B], (uint16)mParams.mHeight);

	SetTileTypeInHeader();
}

void cOriginalMap::SetTileTypeInHeader() {
	std::string mBaseName = mTileTypes[mParams.mTileType].mName + "base.blk";
	std::string mSubName = mTileTypes[mParams.mTileType].mName;

	// Only Jungle has a sub1
	if (mParams.mTileSub == eTileSub_0 || mParams.mTileType != eTileTypes_Jungle)
		mSubName.append("sub0.blk");
	else
		mSubName.append("sub1.blk");

	// Write the base/sub filenames
	std::memcpy(mData->data(), mBaseName.c_str(), 11);
	std::memcpy(mData->data() + 16, mSubName.c_str(), 11);
}

void cOriginalMap::SetTileTypeFromHeader() {
	// Default to Jungle
	mParams.mTileType = eTileTypes_Jungle;
	mParams.mTileSub = eTileSub_0;

	for (auto& TileType : mTileTypes) {
		if (TileType.mName[0] != mData->data()[0])
			continue;
		if (TileType.mName[1] != mData->data()[1])
			continue;
		if (TileType.mName[2] != mData->data()[2])
			continue;
		mParams.mTileType = TileType.mType;
		break;
	}

	// Tile Sub
	if (mData->data()[16] == '1')
		mParams.mTileSub = eTileSub_1;
}

void cOriginalMap::loadCF1Map(tSharedBuffer pMapData) {
	mData = pMapData;

	SetTileTypeFromHeader();

	mParams.mWidth = readBEWord(mData->data() + 0x54);
	mParams.mHeight = readBEWord(mData->data() + 0x56);

	tool_EndianSwap(mData->data() + 0x60, mData->size() - 0x60);
}

void cOriginalMap::loadCF1Spt(tSharedBuffer pSpriteData, bool pCF2) {
	tool_EndianSwap((uint8*)pSpriteData->data(), pSpriteData->size());

	uint16* SptPtr = (uint16*)pSpriteData->data();
	uint16* SptFileEnd = SptPtr + (pSpriteData->size() / 2);

	size_t Sprites = pSpriteData->size() / 8;
	mSprites.resize(Sprites < g_Fodder->mParams->mSpritesMax ? g_Fodder->mParams->mSpritesMax : Sprites);
	sSprite* Sprite = mSprites.data();


	for (uint16 HumanCount = 0; SptPtr != SptFileEnd; ++Sprite) {
		++SptPtr;
		Sprite->field_8 = 0x7C;

		uint16 ax = HumanCount / 8;
		Sprite->field_32 = ax;
		++SptPtr;

		ax = SptPtr[0];
		++SptPtr;
		Sprite->field_0 = ax + 0x10;
		Sprite->field_26 = ax + 0x10;

		ax = SptPtr[0];
		++SptPtr;
		Sprite->field_4 = ax;
		Sprite->field_28 = ax;

		ax = SptPtr[0];
		++SptPtr;
		Sprite->field_18 = ax;

		// 114B

		// HACK: Some SPT files don't contain enough null sprites between entries
		//       This causes some sprites to be overwritten by null/lights on later missions in CF2
		if (pCF2) {
			if (Sprite > mSprites.data())
				if (Sprite->field_18 == 4 && (Sprite - 1)->field_18 >= 114 && (Sprite - 1)->field_18 <= 117) {
					++Sprite;
				}
		}
	}
}

bool cOriginalMap::load(tSharedBuffer pMapFile, tSharedBuffer pSptFile, const bool pCF2) {
	loadCF1Map(pMapFile);
	loadCF1Spt(pSptFile, pCF2);

	return true;
}

bool cOriginalMap::save(std::string pFilename, const bool CF1) {
	
	if (pFilename.find(".map") == pFilename.npos) {
		pFilename.append(".map");
	}

	saveHeader();
	saveCF1Map(pFilename);
	return saveCF1Sprites(pFilename);
}

bool cOriginalMap::saveCF1Map(const std::string& pFilename) {

	std::ofstream outfile(pFilename, std::ofstream::binary);

	// The original game stores the maps in big endian
	tool_EndianSwap(mData->data() + 0x60, mData->size() - 0x60);
	outfile.write((const char*)mData->data(), mData->size());
	outfile.close();

	// Now we can swap it back to little endian
	tool_EndianSwap(mData->data() + 0x60, mData->size() - 0x60);

	return true;
}

bool cOriginalMap::saveCF1Sprites(std::string pFilename) {

	// Replace .map with .spt
	pFilename.replace(pFilename.length() - 3, pFilename.length(), "spt");
	std::ofstream outfile(pFilename, std::ofstream::binary);

	// Number of sprites in use
	size_t SpriteCount = std::count_if(std::begin(mSprites), std::end(mSprites), [](sSprite& l) {
		return l.field_0 != -32768 && l.field_0 != -1;
	});

	auto MapSpt = tSharedBuffer(new std::vector<uint8>());
	MapSpt->resize(SpriteCount * 0x0A);

	uint8* SptPtr = MapSpt->data();

	// Cheap way of writing human players first
	for (const auto& SpriteIT : mSprites) {

		if (SpriteIT.field_0 == -1 || SpriteIT.field_0 == -32768)
			continue;

		if (SpriteIT.field_18 != eSprite_Player)
			continue;

		writeBEWord(SptPtr, 0x7C);  SptPtr += 2;                        // Direction
		writeBEWord(SptPtr, 0x00);  SptPtr += 2;                        // Ignored
		writeBEWord(SptPtr, SpriteIT.field_0 - 0x10);  SptPtr += 2;    // X    
		writeBEWord(SptPtr, SpriteIT.field_4);  SptPtr += 2;            // Y    
		writeBEWord(SptPtr, SpriteIT.field_18); SptPtr += 2;            // Type 
	}

	// Now write out all other players
	for (const auto& SpriteIT : mSprites) {

		if (SpriteIT.field_0 == -1 || SpriteIT.field_0 == -32768)
			continue;

		if (SpriteIT.field_18 == eSprite_Player)
			continue;

		writeBEWord(SptPtr, 0x7C);  SptPtr += 2;                // Direction
		writeBEWord(SptPtr, 0x00);  SptPtr += 2;                // Ignored
		writeBEWord(SptPtr, SpriteIT.field_0 - 0x10);  SptPtr += 2;    // X    
		writeBEWord(SptPtr, SpriteIT.field_4);  SptPtr += 2;    // Y    
		writeBEWord(SptPtr, SpriteIT.field_18); SptPtr += 2;    // Type 
	}
	outfile.write((const char*)MapSpt->data(), MapSpt->size());
	outfile.close();
	return true;
}
