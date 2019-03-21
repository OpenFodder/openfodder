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
#include "Utils/SimplexNoise.hpp"
#include "Utils/diamondsquare.hpp"

sMapParams::sMapParams(size_t pWidth, size_t pHeight, eTileTypes pTileType, eTileSub pTileSub) {
	mAlgorithm = eRandom_SimplexNoise;

	mTileType = pTileType;
	mTileSub = pTileSub;
	mWidth = pWidth;
	mHeight = pHeight;
}

sMapParams::sMapParams(size_t pSeed) {
	mTileType = eTileTypes_Jungle;
	mTileSub = eTileSub_0;
	mWidth = 0;
	mHeight = 0;

	if(pSeed)
		Randomise(pSeed);
}

void sMapParams::Randomise(const size_t pSeed) {
	mRandom.setSeed((int16)pSeed);

	mAlgorithm = (eRandom_Algorithms)(mRandom.getu() % eRandom_SimplexNoise);

	do {
		mTileType = (eTileTypes)(mRandom.getu() % eTileTypes_AFX);
	} while (mTileType == eTileTypes_Hid);

	// HACK: While we only support jungle random
	mTileType = eTileTypes_Jungle;
	mTileSub = eTileSub_0;

	mWidth = (mRandom.getu() % 100) + 0x15;
	mHeight = (mRandom.getu() % 100) + 0x10;

	// TODO: Normalise width/height
}

cMap::cMap() {
	mData = std::make_shared<std::vector<uint8_t>>();
}

cMap::cMap(const sMapParams& pParams) {
	mParams = pParams;

	mData = std::make_shared<std::vector<uint8_t>>();
	ClearTiles(0);

	CreateHeader();
}

cMap::cMap(tSharedBuffer pMapFile, tSharedBuffer pSptFile, const bool pCF2) {

	loadCF1Map(pMapFile);
	loadCF1Spt(pSptFile, pCF2);
}

// persistance closer to 0 produces more rivers,
// but a higher number produces more mountains

void cMap::Randomise_Tiles() {
	float scale = mParams.mRandom.getf(0.01f, 0.1f);
	float lacunarity =  mParams.mRandom.getf(0.1f, 0.5f);
	float persistance = mParams.mRandom.getf(0.1f, 1.f);	// higher produces more trees
	float offset_z = mParams.mRandom.getf(0.1f, 10.f);

	const SimplexNoise simplex(scale, scale, lacunarity, persistance);
	const int octaves = static_cast<int>(5 + std::log(scale)); 

	// jungle only
	int16 TileWater		= 326;
	//int16 TileQuickSand = 167;
	int16 TileLand		= 123;
	int16 TileBounce	= 82;

	int16* MapPtr = (int16*)(mData->data() + 0x60);

	for (size_t Y = 0; Y < mParams.mHeight; ++Y) {

		for (size_t X = 0; X < mParams.mWidth; ++X) {

			const float noise = simplex.fractal(octaves, (float)X, (float)Y, offset_z);

			if (noise < -0.500f) {
				*MapPtr = TileWater;
			}
			else if (noise < -0.020f) {
				*MapPtr = TileWater;
			}
			else if (noise < -0.000f) {
				*MapPtr = TileLand;
			} 
			else if (noise < 0.005f) {
				*MapPtr = TileLand;
			}
			else if (noise > 0.300f && noise < 0.400f) {
				*MapPtr = TileLand;// TileQuickSand;
			}
			else if (noise < 0.500f) {
				*MapPtr = TileLand;
			}
			else if (noise < 0.700f) {
				*MapPtr = TileLand;
			}
			else if (noise < 0.900f) {
				*MapPtr = TileBounce;
			}
			else {
				*MapPtr = TileBounce;
			}
			++MapPtr;
		}
	}

}

void cMap::Randomise_TileSmooth() {
	/*
	for (int32 y = 1; y < mHeight; ++y) {
		for (int32 x = 1; x < mWidth; ++x) {

			int32 TileX = x * 16;
			int32 TileY = y * 16;

			int16 Tile          = Tile_Get(TileX, TileY);

			int16 TileUp        = Tile_Get(TileX,      TileY - 1);
			int16 TileLeftUp    = Tile_Get(TileX - 1,  TileY - 1);
			int16 TileLeft      = Tile_Get(TileX - 1,  TileY);
			int16 TileLeftDown  = Tile_Get(TileX - 1,  TileY + 1);
			int16 TileDown      = Tile_Get(TileX,      TileY + 1);
			int16 TileRightDown = Tile_Get(TileX + 1,  TileY + 1);
			int16 TileRight     = Tile_Get(TileX + 1,  TileY);
			int16 TileRightUp   = Tile_Get(TileX + 1,  TileY - 1);

		} // Width
	} // Height
	*/
}


void cMap::Randomise_Structures(const size_t pCount) {

	size_t StructsCount = 0;

	int16 TileLand = 123;

	// This is very lame :)
	while (StructsCount < pCount) {
		auto Struct = mStructuresBarracksWithSoldier[mParams.mTileType];

		size_t StartTileX = (size_t) g_Fodder->tool_RandomGet(Struct.MaxWidth() + 2, mParams.mWidth - Struct.MaxWidth());
		size_t StartTileY = (size_t) g_Fodder->tool_RandomGet(Struct.MaxHeight() + 2, mParams.mHeight - Struct.MaxWidth());

		// TODO: Check if we will overlap an existing structure,
		//       or place on water
		auto Tile = Tile_Get(StartTileX, StartTileY);
		if (Tile != TileLand)
			continue;

		Structure_Add(Struct, StartTileX, StartTileY);

		// Add an enemy below each building
		Sprite_Add(eSprite_Enemy, StartTileX * 16, (StartTileY + 3) * 16);

		++StructsCount;
	}
}

void cMap::Randomise_Sprites(const size_t pHumanCount) {
	int16 DistanceY = 8;
	int16 DistanceX = 8;

	int16 MiddleX = 0;
	int16 MiddleY = 0;

	int16 TileLand = 123;

	size_t Count = 0;
	while (Count < 100000) {
		int16 StartTileX = g_Fodder->tool_RandomGet(2, mParams.mWidth - 2);
		int16 StartTileY = g_Fodder->tool_RandomGet(2, mParams.mHeight - 2);

		// TODO: Check if we will overlap an existing structure,
		//       or place on water
		auto Tile = Tile_Get(StartTileX, StartTileY);
		if (Tile != TileLand)
			continue;

		MiddleX = StartTileX * 16;
		MiddleY = StartTileY * 16;
		break;
	}

	// Add atleast two sprites
	for (size_t x = 0; x < pHumanCount; ++x) {
		if (g_Fodder->tool_RandomGet() % 2)
			Sprite_Add(eSprite_Player, MiddleX - DistanceX, MiddleY + DistanceY);
		else
			Sprite_Add(eSprite_Player, MiddleX + DistanceX, MiddleY - DistanceY);

		DistanceX += 6;
		DistanceY += 6;
	}


	// Add some weapons
	Sprite_Add(eSprite_RocketBox, MiddleX, MiddleY + DistanceY);
	Sprite_Add(eSprite_GrenadeBox, MiddleX + DistanceX, MiddleY + DistanceY);
}

void cMap::CreateHeader() {
	uint16* Map = (uint16*)mData->data();

	// Map Marker ('ofed')
	Map[0x28] = 'fo'; Map[0x29] = 'de';

	// Put the map size
	writeBEWord(&Map[0x2A], (uint16)mParams.mWidth);
	writeBEWord(&Map[0x2B], (uint16)mParams.mHeight);

	SetTileTypeInHeader();
}

void cMap::SetTileTypeInHeader() {
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

void cMap::SetTileTypeFromHeader() {
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

int32 cMap::Tile_Get(const size_t pTileX, const size_t pTileY) {
	if (pTileX > mParams.mWidth || pTileY > mParams.mHeight)
		return -1;

	size_t Tile = (((pTileY * mParams.mWidth) + pTileX)) + mParams.mWidth;

	uint8* CurrentMapPtr = mData->data() + mTile_Ptr + (Tile * 2);
	if (CurrentMapPtr > mData->data() + mData->size())
		return -1;

	return readLE<int16>(CurrentMapPtr);
}

void cMap::Tile_Set(const size_t pTileX, const size_t pTileY, const size_t pTileID) {
	if (pTileX > mParams.mWidth || pTileY > mParams.mHeight)
		return;

	size_t Tile = (((pTileY *  mParams.mWidth) + pTileX)) + mParams.mWidth;

	uint8* CurrentMapPtr = mData->data() + mTile_Ptr + (Tile * 2);
	if (CurrentMapPtr > mData->data() + mData->size())
		return;

	writeLEWord(CurrentMapPtr, (uint16)pTileID);
}

void cMap::Sprite_Add(size_t pSpriteID, size_t pSpriteX, size_t pSpriteY) {
	sSprite First;

	First.field_18 = static_cast<int16>(pSpriteID);
	First.field_0 = static_cast<int16>(pSpriteX);
	First.field_4 = static_cast<int16>(pSpriteY);
	First.field_26 = static_cast<int16>(pSpriteX);
	First.field_28 = static_cast<int16>(pSpriteY);
	mSprites.push_back(First);

	switch (pSpriteID) {
	case eSprite_BoilingPot:                        // 1 Null
	case eSprite_Tank_Human:
	case eSprite_VehicleNoGun_Human:
	case eSprite_VehicleGun_Human:
	case eSprite_VehicleNoGun_Enemy:
	case eSprite_VehicleGun_Enemy:
	case eSprite_Vehicle_Unk_Enemy:
		First.field_18 = eSprite_Null;
		mSprites.push_back(First);
		break;

	case eSprite_Helicopter_Grenade_Enemy:          // 3 Nulls
	case eSprite_Helicopter_Grenade2_Enemy:
	case eSprite_Helicopter_Missile_Enemy:
	case eSprite_Helicopter_Homing_Enemy:
	case eSprite_Helicopter_Homing_Enemy2:
		First.field_18 = eSprite_Null;
		mSprites.push_back(First);
		mSprites.push_back(First);
		mSprites.push_back(First);
		break;

		// Fall Through
	case eSprite_Helicopter_Grenade2_Human:         // 2 Nulls
	case eSprite_Helicopter_Grenade_Human:
	case eSprite_Helicopter_Missile_Human:
	case eSprite_Helicopter_Homing_Human:
	case eSprite_Helicopter_Grenade2_Human_Called:
	case eSprite_Helicopter_Grenade_Human_Called:
	case eSprite_Helicopter_Missile_Human_Called:
	case eSprite_Helicopter_Homing_Human_Called:
	case eSprite_Tank_Enemy:
		First.field_18 = eSprite_Null;
		mSprites.push_back(First);
		mSprites.push_back(First);
		break;
	}
}

void cMap::Structure_Add(const sStructure& pStructure, size_t pTileX, size_t pTileY) {

	for (const auto& Piece : pStructure.mTiles) {
		Tile_Set(pTileX + Piece.mX, pTileY + Piece.mY, Piece.mTileID);
	}

	// Add the sprites
	for (const auto& Sprite : pStructure.mSprites) {
		auto Sheet = &mSpriteSheetTypes_InGame_Amiga[Sprite.mSpriteID][0];

		// - 0x40 because no sidebar
		Sprite_Add(Sprite.mSpriteID, ((pTileX) * 16) + (Sprite.mX - Sheet->mModX) - 0x40,
			((pTileY) * 16) + (Sprite.mY - Sheet->mModY));
	}
}

void cMap::ClearTiles(const size_t pTileID) {

	mData->resize(0x60 + ((mParams.mWidth * mParams.mHeight) * 2), (uint8) pTileID);
	mTile_Ptr = (int32)((0x60 - 8) - (mParams.mWidth * 2));
}

void cMap::Randomise() {

	Randomise_Tiles();
	Randomise_TileSmooth();
	Randomise_Structures(2);
	Randomise_Sprites(2);
}

tSharedBuffer cMap::getData() const {
	return mData; 
}

std::vector<sSprite> cMap::getSprites() const {
	return mSprites;
}

void cMap::loadCF1Map(tSharedBuffer pMapData) {
	mData = pMapData;

	SetTileTypeFromHeader();

	mParams.mWidth = readBEWord(mData->data() + 0x54);
	mParams.mHeight = readBEWord(mData->data() + 0x56);

	tool_EndianSwap(mData->data() + 0x60, mData->size() - 0x60);
	/*
	// Check Editor used
	switch (readBEDWord(mMap->data() + 0x50)) {
	default:        // Unknown Editor
	case 'cfed':    // Original Engine Map
		break;

	case 'ofed':    // Open Fodder Map
		break;
	}*/
}

void cMap::loadCF1Spt(tSharedBuffer pSpriteData, bool pCF2) {
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

bool cMap::save(const std::string& pFilename, const bool CF1) {
	
	saveCF1Map(pFilename);
	return saveCF1Sprites(pFilename);
}

bool cMap::saveCF1Map(const std::string& pFilename) {

	std::ofstream outfile(pFilename, std::ofstream::binary);

	// The original game stores the maps in big endian
	tool_EndianSwap(mData->data() + 0x60, mData->size() - 0x60);
	outfile.write((const char*)mData->data(), mData->size());
	outfile.close();

	// Now we can swap it back to little endian
	tool_EndianSwap(mData->data() + 0x60, mData->size() - 0x60);

	return true;
}

bool cMap::saveCF1Sprites(std::string pFilename) {

	mSprites = g_Fodder->mSprites;

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
	for (const auto SpriteIT : mSprites) {

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
	for (const auto SpriteIT : mSprites) {

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


void cMap::Randomise_Tiles_DS() {
	int32 PowerOf = 0;
	size_t Size;

	if (mParams.mWidth < mParams.mHeight)
		Size = mParams.mHeight;
	else
		Size = mParams.mWidth;
	while (Size > 0) {
		PowerOf++;
		Size = Size >> 1;
	}

	cDiamondSquare DS(PowerOf, mParams.mRandom.getStartingSeed());
	auto HeightMap = DS.generate();

	int16* MapPtr = (int16*)(mData->data() + 0x60);

	// Find the highest and lowest points in the height map
	double HeightMin = 0;
	double HeightMax = 0;
	for (auto& Row : HeightMap) {
		for (auto& Column : Row) {

			if (Column > HeightMax) HeightMax = Column;
			if (Column < HeightMin) HeightMin = Column;
		}
	}

	// Calcukate the difference between top/bottom
	double diff = HeightMax - HeightMin;
	double flood = 0.3;
	double mount = 0.7;

	// Calculate the flood/moutain levels
	flood *= diff;
	mount *= diff;

	// Jungle
	int16 TileWater = 326;
	int16 TileLand = 123;
	int16 TileBounce = 82;
	size_t X = 0;
	size_t Y = 0;

	switch (mParams.mTileType) {
	case eTileTypes_Ice:
		TileLand = 1;
		break;
	default:
		break;
	}

	static bool Found = false;

	// Loop each tile row
	for (auto Row : HeightMap) {
		X = 0;
		// Loop each tile column
		for (auto Column : Row) {
			Column -= HeightMin;

			if (Column < flood) {
				//if(!Found)
				*MapPtr = TileWater;
				Found = true;
			}
			else if (Column > mount) {
				*MapPtr = TileBounce;
			}
			else {
				*MapPtr = TileLand;
			}
			++MapPtr;

			if (++X >= mParams.mWidth)
				break;
		}

		if (++Y >= mParams.mHeight)
			break;
	}
}