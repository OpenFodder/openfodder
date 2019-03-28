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

#define eTerrainTypes(operation) \
	operation(Land) \
	operation(Rocky) \
	operation(Rocky2) \
	operation(Block) \
	operation(QuickSand) \
	operation(WaterEdge) \
	operation(Water) \
	operation(Snow) \
	operation(QuickSandEdge) \
	operation(Drop) \
	operation(Drop2) \
	operation(Sink) \
	operation(C) \
	operation(D) \
	operation(Jump)

#define CREATE_ENUM(name) \
  eTerrainType_##name,

#define CREATE_STRINGS(name) \
  #name,

enum eTerrainType {
	eTerrainTypes(CREATE_ENUM)
};

extern std::vector<std::string> mTerrainTypeNames;


struct sTileType {
	eTileTypes  mType;
	std::string mName;
	std::string mFullName;
};


struct sStructureTile {
	int16 mX;
	int16 mY;
	uint16 mTileID;

	sStructureTile(int16 pMapX, int16 pMapY, uint16 pID) {
		mX = pMapX;
		mY = pMapY;
		mTileID = pID;
	}

};

struct sStructureSprite {
	int16 mX;
	int16 mY;
	uint16 mSpriteID;

	sStructureSprite(int16 pTileX, int16 pTileY, uint16 pSpriteID) {
		mX = pTileX;
		mY = pTileY;
		mSpriteID = pSpriteID;
	}
};

struct sStructure {
	std::vector<sStructureTile> mTiles;
	std::vector<sStructureSprite> mSprites;

	int16 MaxWidth() const {
		int16 Width = 0;

		for (auto Tile : mTiles) {
			if (Width < Tile.mX)
				Width = Tile.mX;
		}
		return Width;
	}

	int16 MaxHeight() const {
		int16 Height = 0;

		for (auto Tile : mTiles) {
			if (Height < Tile.mY)
				Height = Tile.mY;
		}
		return Height;
	}
};

extern const int8 mTiles_NotFlyable[];
extern const int8 mTiles_NotDriveable[];
extern const int8 mTiles_NotWalkable[];

extern const int16 mTiles_Indestructible_Jungle[];
extern const int16 mTiles_Indestructible_Desert[];
extern const int16 mTiles_Indestructible_Ice[];
extern const int16 mTiles_Indestructible_Moors[];
extern const int16 mTiles_Indestructible_Internal[];

extern const int16* mTiles_Indestructible[];

extern const std::vector<sTileType> mTileTypes;
