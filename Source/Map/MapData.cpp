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

sMapParams::sMapParams(size_t pWidth, size_t pHeight, eTileTypes pTileType, eTileSub pTileSub) {
	mTileType = pTileType;
	mTileSub = pTileSub;
	mWidth = pWidth;
	mHeight = pHeight;
}

sMapParams::sMapParams(size_t pSeed) {
	mTileType = eTileTypes_Jungle;
	mTileSub = eTileSub_0;
	mWidth = 30;
	mHeight = 20;

	if (pSeed)
		mRandom.setSeed((int16)pSeed);
}

cMapData::cMapData() {
	mData = std::make_shared<std::vector<uint8_t>>();
	mTile_Ptr = 0;
}

int32 cMapData::Tile_Get(const size_t pTileX, const size_t pTileY) {
	if (pTileX > mParams.mWidth || pTileY > mParams.mHeight)
		return -1;

	size_t Tile = (((pTileY * mParams.mWidth) + pTileX)) + mParams.mWidth;

	uint8* CurrentMapPtr = mData->data() + mTile_Ptr + (Tile * 2);
	if (CurrentMapPtr > mData->data() + mData->size())
		return -1;

	return readLE<int16>(CurrentMapPtr);
}

void cMapData::Tile_Set(const size_t pTileX, const size_t pTileY, const size_t pTileID) {
	if (pTileX > mParams.mWidth || pTileY > mParams.mHeight)
		return;

	size_t Tile = (((pTileY *  mParams.mWidth) + pTileX)) + mParams.mWidth;

	uint8* CurrentMapPtr = mData->data() + mTile_Ptr + (Tile * 2);
	if (CurrentMapPtr > mData->data() + mData->size())
		return;

	writeLEWord(CurrentMapPtr, (uint16)pTileID);
}

void cMapData::Sprite_Add(size_t pSpriteID, size_t pSpriteX, size_t pSpriteY) {
	sSprite First;

	First.mSpriteType = static_cast<int16>(pSpriteID);
	First.mPosX = static_cast<int16>(pSpriteX);
	First.mPosY = static_cast<int16>(pSpriteY);
	First.mTargetX = static_cast<int16>(pSpriteX);
	First.mTargetY = static_cast<int16>(pSpriteY);
	mSprites.push_back(First);

	switch (pSpriteID) {
	case eSprite_BoilingPot:                        // 1 Null
	case eSprite_Tank_Human:
	case eSprite_VehicleNoGun_Human:
	case eSprite_VehicleGun_Human:
	case eSprite_VehicleNoGun_Enemy:
	case eSprite_VehicleGun_Enemy:
	case eSprite_Vehicle_Unk_Enemy:
		First.mSpriteType = eSprite_Null;
		mSprites.push_back(First);
		break;

	case eSprite_Helicopter_Grenade_Enemy:          // 3 Nulls
	case eSprite_Helicopter_Unarmed_Enemy:
	case eSprite_Helicopter_Missile_Enemy:
	case eSprite_Helicopter_Homing_Enemy:
	case eSprite_Helicopter_Homing_Enemy2:
		First.mSpriteType = eSprite_Null;
		mSprites.push_back(First);
		mSprites.push_back(First);
		mSprites.push_back(First);
		break;

		// Fall Through
	case eSprite_Helicopter_Grenade_Human:         // 2 Nulls
	case eSprite_Helicopter_Unarmed_Human:
	case eSprite_Helicopter_Missile_Human:
	case eSprite_Helicopter_Homing_Human:
	case eSprite_Helicopter_Grenade_Human_Called:
	case eSprite_Helicopter_Unarmed_Human_Called:
	case eSprite_Helicopter_Missile_Human_Called:
	case eSprite_Helicopter_Homing_Human_Called:
	case eSprite_Tank_Enemy:
		First.mSpriteType = eSprite_Null;
		mSprites.push_back(First);
		mSprites.push_back(First);
		break;
	}
}

void cMapData::Structure_Add(const sStructure& pStructure, size_t pTileX, size_t pTileY) {

	for (const auto& Piece : pStructure.mTiles) {
		Tile_Set(pTileX + Piece.mX, pTileY + Piece.mY, Piece.mTileID);
	}

	// Add the sprites
	for (const auto& Sprite : pStructure.mSprites) {
		auto Sheet = &mSpriteSheetTypes_InGame_Amiga[Sprite.mSpriteID][0];

		// - 0x40 because no sidebar
		Sprite_Add(Sprite.mSpriteID, ((pTileX) * 16) + (Sprite.mX - Sheet->mModX),
			((pTileY) * 16) + (Sprite.mY - Sheet->mModY));
	}
}

void cMapData::ClearTiles(const size_t pTileID) {

	mData->resize(0x60 + ((mParams.mWidth * mParams.mHeight) * 2), (uint8)pTileID);
	mTile_Ptr = (int32)((0x60) - (mParams.mWidth * 2));
}

tSharedBuffer cMapData::getData() const {
	return mData;
}

std::vector<sSprite> cMapData::getSprites() const {
	return mSprites;
}
