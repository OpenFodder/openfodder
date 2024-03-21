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

struct sSprite;

struct sMapParams {
	size_t		mWidth;
	size_t		mHeight;
	eTileTypes	mTileType;
	eTileSub	mTileSub;
	cPseudorand mRandom;

	sMapParams(size_t pWidth, size_t pHeight, eTileTypes pTileType, eTileSub pTileSub = eTileSub::eTileSub_0);
	sMapParams(size_t pSeed = 0);

	size_t getWidth() { return mWidth; }
	void setWidth(const size_t pWidth) { mWidth = pWidth; }
	
	size_t getHeight() { return mHeight; }
	void setHeight(const size_t pHeight) { mHeight = pHeight; }

	eTileTypes getTiletype() { return mTileType; }
	void setTiletype(eTileTypes pTiletypes) { mTileType = pTiletypes; }

	eTileSub getTileSub() { return mTileSub; }
	void setTileSub(eTileSub pTileSub) { mTileSub = pTileSub; }

};

class cMap {

protected:
	sMapParams mParams;
	tSharedBuffer mData;

	int32 mTile_Ptr;

	std::vector<sSprite> mSprites;

protected:

	int32 Tile_Get(const size_t pTileX, const size_t pTileY);
	void  Tile_Set(const size_t pTileX, const size_t pTileY, const size_t pTileID);
	void  Sprite_Add(size_t pSpriteID, size_t pSpriteX, size_t pSpriteY);
	void  Structure_Add(const sStructure& pStructure, size_t pTileX, size_t pTileY);

public:
	cMap();

	void ClearTiles(const size_t pTileID);

	sMapParams *getMapParams() { return &mParams; }
	eTileTypes getTileType() const { return mParams.mTileType; }
	eTileSub getTileSub() const { return mParams.mTileSub; }

	int32 getArea() const { return static_cast<int32>(mParams.mWidth * mParams.mHeight);  }
	int32 getWidth() const { return static_cast<int32>(mParams.mWidth); }
	int32 getHeight() const { return static_cast<int32>(mParams.mHeight); }

	int32 getAreaPixels() const { return getWidthPixels() * getHeightPixels(); }
	int32 getWidthPixels() const { return static_cast<int32>(mParams.mWidth) << 4; }
	int32 getHeightPixels() const { return static_cast<int32>(mParams.mHeight) << 4; }

	virtual bool load(tSharedBuffer pMapFile, tSharedBuffer pSptFile, const bool pCF2) = 0;
	virtual bool save(std::string pFilename, const bool CF1) = 0;

	tSharedBuffer			getData() const;
	std::vector<sSprite>	getSprites() const;
};
