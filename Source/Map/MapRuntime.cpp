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
#include "Map/Random.hpp"

void cFodder::Map_Add_Structure(const sStructure &pStructure, int16 pTileX, int16 pTileY)
{

    for (const auto &Piece : pStructure.mTiles)
    {

        MapTile_Set(pTileX + Piece.mX, pTileY + Piece.mY, Piece.mTileID);
    }

    // Add the sprites
    for (const auto &Sprite : pStructure.mSprites)
    {

        auto Sheet = Sprite_Get_Sheet(Sprite.mSpriteID, 0);

        // - 0x40 because no sidebar
        Sprite_Add(Sprite.mSpriteID, ((pTileX) * 16) + (Sprite.mX - Sheet->mModX) - 0x40,
                   ((pTileY) * 16) + (Sprite.mY - Sheet->mModY));
    }
}

void cFodder::Map_Load_Sprites()
{

    Sprite_Clear_All();

    mSprites = mMapLoaded->getSprites();

    if (mSprites.size() < mParams->mSpritesMax)
    {
        size_t start = mSprites.size();
        mSprites.resize(mParams->mSpritesMax);

        for (; start < mSprites.size(); ++start)
        {
            mSprites[start].Clear();
        }
    }

    Squad_Set_Squad_Leader();
    Map_Count_Sprites();
}

void cFodder::Map_Count_Sprites()
{
    uint16 HumanCount = 0;
    int16 dword_37ABC = 0x0A;

    mTroops_Enemy_Count = 0;
    mHostage_Count = 0;

    for (auto &Sprite : mSprites)
    {

        if (Sprite.mPosX == -1 || Sprite.mPosX == -32768)
            continue;

        Sprite.mSheetIndex = 0x7C;
        Sprite.field_32 = (HumanCount / 8);

        if (Sprite.mSpriteType == eSprite_Enemy_Leader || Sprite.mSpriteType == eSprite_Hostage)
        {

            ++mHostage_Count;
        }

        if (Sprite.mSpriteType == eSprite_Player)
        {

            ++HumanCount;
            Sprite.mWeaponFireTimer = 0;
        }
        else
        {

            if (Sprite.mSpriteType == eSprite_Enemy_Rocket)
            {
                Sprite.mPersonType = eSprite_PersonType_AI;
                ++mTroops_Enemy_Count;
            }
            else
            {
                if (Sprite.mSpriteType == eSprite_Enemy)
                    ++mTroops_Enemy_Count;
            }

            dword_37ABC += 0x0A;
            if (Sprite.mAIAggression > 4)
                dword_37ABC = 0;

            Sprite.mWeaponFireTimer = dword_37ABC;
        }
    }
}

void cFodder::Map_Create(sMapParams pParams)
{

    if (mVersionCurrent->isAmigaPower())
        pParams.mTileSub = eTileSub_1;

    mMapLoaded = std::make_shared<cRandomMap>(pParams);

    mMap = mMapLoaded->getData();

    mMapTile_Ptr = (int32)((0x60 - 8) - (mMapLoaded->getWidth() * 2));
    mMapTile_DrawX = 0;
    mMapTile_DrawY = 0;

    // Clear current sprites
    Sprite_Clear_All();

    // Load the map specific resources
    Map_Load_Resources();
}

void cFodder::Map_Load()
{
    mMapLoaded = mGame_Data.mCampaign.getCMap(mGame_Data.mPhase_Current);
    mMap = mMapLoaded->getData();

    if (!mMap->size())
        return;

    Map_Load_Resources();
}

void cFodder::Map_Collect_TileTracks()
{
    uint8 *tilePtr = mMap->data() + 0x62;

    uint16_t heightCount = 0, widthCount;

    mMapTileTracks.clear();

    while (heightCount < mMapLoaded->getHeight())
    {
        widthCount = 0;

        while (widthCount < mMapLoaded->getWidth())
        {
            uint16_t tile = (readLE<uint16>(tilePtr) & 0xE000) >> 8;

            if (tile != 0)
            {
                mMapTileTracks.emplace_back(widthCount, heightCount, tile >> 5);
            }

            tilePtr += 2;
            widthCount++;
        }

        heightCount++;
    }
}

void cFodder::Map_Load_Resources()
{
    std::string BaseBase, BaseSub, BaseBaseSet, BaseSubSet;

    // Is the tileset available?
    if (!Tiles_Load_Data())
    {

        // Load the default version
        VersionSwitch(mVersionDefault);

        // Check the default version for the tileset
        if (!Tiles_Load_Data())
        {

            // Not found, so lets go find it
            auto Version = mVersions->GetForTileset(mMapLoaded->getTileType(), mMapLoaded->getTileSub());

            // Load it
            if (Version)
            {
                VersionSwitch(Version);
                Tiles_Load_Data();
            }
            else
            {
                DataNotFound();
                return;
            }
        }

        mVersionReturnAfterPhase = true;
    }

    // jun
    BaseBaseSet.append(mMap->data(), mMap->data() + 3);

    // jun
    BaseSubSet.append(mMap->data() + 0x10, mMap->data() + 0x10 + 3);

    // junbase
    BaseBase.append(mMap->data(), mMap->data() + 7);

    // junsub0
    BaseSub.append(mMap->data() + 0x10, mMap->data() + 0x10 + 7);

    mFilenameCopt = BaseBaseSet + "copt";
    mFilenameArmy = BaseSubSet + "army";

    mFilenameBaseSwp = BaseBase + ".swp";
    mFilenameBaseHit = BaseBase + ".hit";
    mFilenameBaseBht = BaseBase + ".bht";
    mFilenameSubSwp = BaseSub + ".swp";
    mFilenameSubHit = BaseSub + ".hit";
    mFilenameSubBht = BaseSub + ".bht";
    mFilenameBasePal = BaseBase + ".pal";

    size_t Size = mResources->fileLoadTo(mFilenameBaseSwp, (uint8 *)&mTile_Destroy_Swap[0]);
    tool_EndianSwap((uint8 *)&mTile_Destroy_Swap[0], Size);

    Size = mResources->fileLoadTo(mFilenameSubSwp, (uint8 *)&mTile_Destroy_Swap[240]);
    tool_EndianSwap((uint8 *)&mTile_Destroy_Swap[240], Size);

    memset(mTile_Hit, 0, 512);

    for (int x = 0; x < 512; ++x)
        for (int y = 0; y < 8; ++y)
            mTile_BHit[x][y] = 0;

    Size = mResources->fileLoadTo(mFilenameBaseHit, (uint8 *)&mTile_Hit[0]);
    tool_EndianSwap((uint8 *)&mTile_Hit[0], Size);

    Size = mResources->fileLoadTo(mFilenameSubHit, (uint8 *)&mTile_Hit[240]);
    tool_EndianSwap((uint8 *)&mTile_Hit[240], Size);

    Size = mResources->fileLoadTo(mFilenameBaseBht, (uint8 *)&mTile_BHit[0][0]);
    Size = mResources->fileLoadTo(mFilenameSubBht, (uint8 *)&mTile_BHit[240][0]);

    mGraphics->Tile_Prepare_Gfx();
    mGraphics->Map_Load_Resources();
    mGraphics->PaletteSet();
}

void cFodder::MapTile_UpdateFromCamera()
{
    mMapTile_SpeedX = mCamera_TileSpeedX;
    mMapTile_SpeedY = mCamera_TileSpeedY;
    mMapTile_TargetX = mCameraX;
    mMapTile_TargetY = mCameraY;
    mMapTile_MoveDirectionX = mCamera_MoveDirectionX;
    mMapTile_MoveDirectionY = mCamera_MoveDirectionY;
}

void cFodder::Map_Overview_Prepare()
{

    if (mParams->mUnitTesting)
        return;

    delete mSurfaceMapOverview;
    size_t Size = mMapLoaded->getWidth() < mMapLoaded->getHeight() ? mMapLoaded->getHeight() : mMapLoaded->getWidth();
    if ((Size * 16) * (Size * 16) >= 0x7FFFFFFF)
        return;

    mSurfaceMapOverview = new cSurface(Size * 16, Size * 16);
    mSurfaceMapOverview->clearBuffer();

    int16 *MapPtr = (int16 *)(mMap->data() + 0x60);

    mSurfaceMapTop = mSurfaceMapLeft = 0;

    if (mMapLoaded->getHeight() < mMapLoaded->getWidth())
    {
        mSurfaceMapTop = (mMapLoaded->getWidth() / 2) - (mMapLoaded->getHeight() / 2);
        if (mSurfaceMapTop < 0)
            mSurfaceMapTop = 0;
    }

    if (mMapLoaded->getWidth() < mMapLoaded->getHeight())
    {
        mSurfaceMapLeft = (mMapLoaded->getHeight() / 2) - (mMapLoaded->getWidth() / 2);
        if (mSurfaceMapLeft < 0)
            mSurfaceMapLeft = 0;
    }

    for (uint16 dx = 0; dx < mMapLoaded->getHeight(); ++dx)
    {

        for (uint16 cx = 0; cx < mMapLoaded->getWidth(); ++cx, ++MapPtr)
        {

            if (MapPtr < (int16 *)mMap->data() || MapPtr >= (int16 *)(mMap->data() + mMap->size()))
                continue;

            mGraphics->MapOverview_Render_Tiles(*MapPtr & 0x1FF, cx + mSurfaceMapLeft, dx + mSurfaceMapTop);
        }
    }
}

int16 cFodder::Map_Sprite_Check_Around_Position(sSprite *pSprite)
{
    int16 Data4 = 0x0A;
    int16 Data0 = -13;

    int16 ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    Data4 = 0x14;
    Data0 = -13;
    ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    Data4 = 0x0A;
    Data0 = -7;
    ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    Data4 = 0x14;
    Data0 = -7;
    ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    return 0;
}

int16 cFodder::Map_Get_Distance_BetweenSprites_Within_Window(const sSprite *pSprite, const sSprite *pSprite2)
{

    auto X1 = pSprite->mPosX;
    auto Y1 = pSprite->mPosY;

    auto X2 = pSprite2->mPosX;
    auto Y2 = pSprite2->mPosY;

    return Map_Get_Distance_BetweenPoints_Within_Window(X1, Y1, X2, Y2);
}

int16 cFodder::Map_Get_Distance_BetweenPoints_Within_Window(int16 &pX, int16 pY, int16 &pX2, int16 &pY2)
{
    const int8 *Data24 = mMap_Distance_Calculations;
    int16 Data10 = 0;

    pX2 -= pX;
    if (pX2 < 0)
        pX2 = -pX2;

    if (pX2 >= getWindowWidth())
        goto loc_29EBB;

    pY2 -= pY;
    if (pY2 < 0)
        pY2 = -pY2;

    if (pY2 >= getWindowWidth())
        goto loc_29EBB;

    for (;;)
    {
        if (pX2 <= 0x1F)
            if (pY2 <= 0x1F)
                break;

        pX2 >>= 1;
        pY2 >>= 1;
        ++Data10;
    }
    // loc_29E8C
    pY2 <<= 5;
    pY2 |= pX2;
    pX = 0;

    pX = Data24[pY2];
    pX <<= Data10;
    return pX;

loc_29EBB:;
    pX = 0x3E8;

    return 0x3E8;
}

int16 cFodder::Map_PathCheck_CalculateTo(int16 &pX1, int16 &pY1, int16 &pX2, int16 &pY2)
{
    pX1 >>= 4;
    pY1 >>= 4;
    pX2 >>= 4;
    pY2 >>= 4;

    int16 Data18 = 2;

    int16 RowBytes = mMapLoaded->getWidth();
    RowBytes <<= 1;

    mCheckPattern_Position.mX = pX1;
    mCheckPattern_Position.mY = pY1;

    Map_PathCheck_BuildPath(pX1, pY1, pX2, pY2, Data18, RowBytes);

    return Map_PathCheck_IsWalkable(pX1);
}

void cFodder::Map_PathCheck_BuildPath(int16 &pX1, int16 &pY1, int16 &pX2, int16 &pY2, int16 &pColumnWidth, int16 &pRowWidth)
{
    int16 X = pX2 - pX1;

    int16 Y = pY2 - pY1;

    int16 Data28 = pX2;

    int16 X_Move, Y_Move;

    // Target to the left?
    if (X < 0)
    {
        X_Move = -pColumnWidth;
        X = -X;
        pColumnWidth = -1;
    }
    else
    {
        X_Move = pColumnWidth;
        pColumnWidth = 1;
    }

    // loc_2A56A
    //  If target position is above us
    if (Y < 0)
    {
        Y_Move = -pRowWidth;
        Y = -Y;
        pRowWidth = -1;
    }
    else
    {
        Y_Move = pRowWidth;
        pRowWidth = 1;
    }

    // loc_2A59D
    pX2 = 0;
    if (Y == 0)
        pX2 = 1;
    else
        pX2 = 0;

    pX2 = -pX2;

    mMap_PathToDest.clear();

loc_2A5BA:;

    // Reached target?
    if (Data28 == pX1 && pY2 == pY1)
        return;

    // Move up/down
    if (pX2 >= 0)
    {
        pY1 += pRowWidth;
        pX2 -= X;
        mMap_PathToDest.push_back(Y_Move);
        goto loc_2A5BA;
    }

    // Move left/right
    // loc_2A601
    pX1 += pColumnWidth;
    pX2 += Y;
    mMap_PathToDest.push_back(X_Move);
    goto loc_2A5BA;
}

int16 cFodder::Map_PathCheck_IsWalkable(int16 &pTileHit)
{

    int32 Data4 = mCheckPattern_Position.mY;

    uint8 *MapTilePtr = mMap->data() + 0x60;
    uint16 TileID = 0;

    Data4 *= mMapLoaded->getWidth();
    Data4 += mCheckPattern_Position.mX;

    MapTilePtr += (Data4 << 1);

    for (size_t x = 0; x < mMap_PathToDest.size(); ++x)
    {
        int16 Movement = mMap_PathToDest[x];

        if (Movement == 0 || (x + 1) >= mMap_PathToDest.size())
            goto loc_2A728;

        if (mMap_PathToDest[x + 1] == 0)
            goto loc_2A728;

        MapTilePtr += Movement;
        Movement = mMap_PathToDest[++x];

    loc_2A728:;
        MapTilePtr += Movement;

        // loc_2A6A1
        if (MapTilePtr > mMap->data() && MapTilePtr < mMap->data() + mMap->size())
        {
            TileID = readLE<uint16>(MapTilePtr);
        }
        else
            TileID = 0;

        pTileHit = mTile_Hit[TileID & 0x1FF];

        // Tile has hit?
        if (pTileHit >= 0)
        {
            pTileHit &= 0x0F;

            // Check if tile is passable
            if (mTiles_NotWalkable[pTileHit])
            {
                pTileHit = -1;
                return -1;
            }
        }
    }

    pTileHit = 0;
    return 0;
}

/**
 * Calculate the distance between two points on the map
 *
 * @return Distance between points
 */
int16 cFodder::Map_Get_Distance_BetweenPoints(int16 &pPosX, int16 &pPosY, int16 &pPosX2, int16 &pDistanceMax, int16 &pPosY2)
{
    const int8 *Data24 = mMap_Distance_Calculations;

    pPosX2 -= pPosX;
    if (pPosX2 < 0)
        pPosX2 = -pPosX2;

    if (pPosX2 >= pDistanceMax)
        goto loc_2A7DB;

    pPosY2 -= pPosY;
    if (pPosY2 < 0)
        pPosY2 = -pPosY2;

    if (pPosY2 >= pDistanceMax)
        goto loc_2A7DB;

    pDistanceMax = 0;
    for (;;)
    {
        if (pPosX2 <= 0x1F)
            if (pPosY2 <= 0x1F)
                break;

        pPosX2 >>= 1;
        pPosY2 >>= 1;
        ++pDistanceMax;
    }
    // loc_2A7AD
    pPosY2 <<= 5;
    pPosY2 |= pPosX2;
    pPosX = 0;

    pPosX = Data24[pPosY2];
    pPosX <<= pDistanceMax;

    return pPosX;

loc_2A7DB:;
    pPosX = 0x3E8;
    return 0x3E8;
}

int32 cFodder::Map_Get_Distance_BetweenPositions(cPosition pPos1, cPosition pPos2, int32 pDistanceMax)
{

    pPos2.mX -= pPos1.mX;
    if (pPos2.mX < 0)
        pPos2.mX = -pPos2.mX;

    if (pPos2.mX >= pDistanceMax)
        goto loc_2A7DB;

    pPos2.mY -= pPos1.mY;
    if (pPos2.mY < 0)
        pPos2.mY = -pPos2.mY;

    if (pPos2.mY >= pDistanceMax)
        goto loc_2A7DB;

    pDistanceMax = 0;
    for (;;)
    {
        if (pPos2.mX <= 0x1F)
            if (pPos2.mY <= 0x1F)
                break;

        pPos2.mX >>= 1;
        pPos2.mY >>= 1;
        ++pDistanceMax;
    }
    // loc_2A7AD
    pPos2.mY <<= 5;
    pPos2.mY |= pPos2.mX;

    if (pPos2.mY >= sizeof(mMap_Distance_Calculations) / sizeof(mMap_Distance_Calculations[0]) - 1)
        goto loc_2A7DB;

    pPos1.mX = mMap_Distance_Calculations[pPos2.mY];
    pPos1.mX <<= pDistanceMax;

    return pPos1.mX;

loc_2A7DB:;
    return 0x3E8;
}

/**
 *
 * @param pY
 * @param pX        Returns Terrain Type
 *
 * @return False if can walk
 */
int16 cFodder::Map_Terrain_Get_Type_And_Walkable(int16 &pY, int16 &pX)
{
    int16 Data10 = pY;
    int16 Data14 = pX;

    return Map_Terrain_Get(pY, pX, Data10, Data14);
}

/**
 *
 * @param pSprite
 * @param pY
 * @param pX        Returns Terrain Type
 *
 * @return False if can walk
 */
int16 cFodder::Map_Terrain_Get_Type_And_Walkable(sSprite *pSprite, int16 &pY, int16 &pX)
{

    pY += pSprite->mPosY;
    if (pY >= 0)
    {

        int16 Data14 = pY;
        pX += pSprite->mPosX;
        if (pX >= 0)
        {
            int16 Data10 = pX;

            return Map_Terrain_Get(pY, pX, Data10, Data14);
        }
    }

    pY = -1;
    pX = 0;

    return 0;
}

int16 cFodder::Tile_GetTerrainFeature(const int16 pTileID, int16 pX, int16 pY)
{
    // There is two tables, the HIT and the BHIT
    // HIT contains the type of terrain used by a tile, a value < 0 indicates
    // the tile contains two terrain types.
    // This is determined by looking up the terrain type and row
    //  then checking if a bit is set for the column.
    //  The bit being set, means we use the upper 4 bits as the terrain type
    //  Not being set, means we use the lower 4 bits

    int16 TerrainType = mTile_Hit[pTileID];

    if (TerrainType < 0)
    {
        pX >>= 1;
        pX &= 0x07;

        int16 TilePixel = 7;
        TilePixel -= pX;

        // Y
        pY >>= 1;
        pY &= 0x07;

        int8 RowTerrainType = mTile_BHit[pTileID][pY];

        // If the bit for this X position is set, we use the UpperBits for the terrain type
        if (RowTerrainType & (1 << TilePixel))
            TerrainType >>= 4;
    }

    return (TerrainType & 0x0F);
}

int16 cFodder::Map_Terrain_Get(int16 &pY, int16 &pX, int16 &pData10, int16 &pData14)
{

    if ((pY >> 4) > mMapLoaded->getHeight() || (pX >> 4) > mMapLoaded->getWidth())
        return 0;

    int32 MapPtr = (pY >> 4) * mMapLoaded->getWidth();
    MapPtr += (pX >> 4);
    MapPtr <<= 1;

    if (mMap->data() + (0x60 + MapPtr) >= mMap->data() + mMap->size())
        return 0;

    uint16 TileID = readLE<uint16>(mMap->data() + (0x60 + MapPtr)) & 0x1FF;

    // There is two tables, the HIT and the BHIT
    // HIT contains the type of terrain used by a tile, a value < 0 indicates
    // the tile contains two terrain types.
    // This is determined by looking up the terrain type and row
    //  then checking if a bit is set for the column.
    //  The bit being set, means we use the upper 4 bits as the terrain type
    //  Not being set, means we use the lower 4 bits

    // eTerrainFeature
    int16 TerrainType = Tile_GetTerrainFeature(TileID, pData10, pData14);

    pY = mTiles_NotWalkable[TerrainType];
    pX = TerrainType;

    return mTiles_NotWalkable[TerrainType];
}

int16 cFodder::Map_Terrain_Get(int16 pX, int16 pY)
{

    if ((pY >> 4) > mMapLoaded->getHeight() || (pX >> 4) > mMapLoaded->getWidth())
        return -1;

    int32 MapPtr = (pY >> 4) * mMapLoaded->getWidth();
    MapPtr += (pX >> 4);
    MapPtr <<= 1;

    if (mMap->data() + (0x60 + MapPtr) >= mMap->data() + mMap->size())
        return -1;

    uint16 TileID = readLE<uint16>(mMap->data() + (0x60 + MapPtr)) & 0x1FF;

    // eTerrainFeature
    return Tile_GetTerrainFeature(TileID, pX, pY);
}

int16 cFodder::Map_Terrain_GetMoveable_WithCache(const int8 *pMovementData, int16 &pX, int16 &pY, int16 &pData10, int16 &pData14)
{

    pData10 = pX;
    pData14 = pY;

    return Map_Terrain_Get_Moveable(pMovementData, pX, pY);
}

int16 cFodder::Map_Terrain_Get_Moveable(const int8 *pMovementData, int16 &pX, int16 &pY)
{
    uint32 DataC = pY;
    uint32 Data8 = pX;

    if (pY < 0)
        DataC = 0;
    if (pX < 0)
        Data8 = 0;

    DataC >>= 4;

    DataC *= mMapLoaded->getWidth();
    Data8 >>= 4;

    DataC += Data8;
    DataC <<= 1;

    if (mMap->data() + 0x60 + DataC >= mMap->data() + mMap->size())
        return pMovementData[0];

    int16 Data0 = readLE<uint16>(mMap->data() + 0x60 + DataC);
    Data0 &= 0x1FF;

    int16 Data4 = mTile_Hit[Data0];

    if (Data4 >= 0)
    {
        Data4 &= 0x0F;
        return pMovementData[Data4];
    }

    Data8 = Data4;
    Data4 &= 0x0F;

    Data0 = pMovementData[Data4];
    Data8 >>= 4;
    Data8 &= 0x0F;

    Data0 |= pMovementData[Data8];

    pX = Data8;
    pY = DataC;
    return Data0;
}

void cFodder::Map_Distance_Within_DoubleWindow(int16 &pX, int16 &pY, int16 &pX2, int16 &pY2)
{
    int16 Data10 = 0;

    pX2 -= pX;
    if (pX2 < 0)
        pX2 = -pX2;

    if (pX2 >= getWindowWidth() * 2)
        goto loc_2B403;

    pY2 -= pY;
    if (pY2 < 0)
        pY2 = -pY2;

    if (pY2 >= getWindowWidth() * 2)
        goto loc_2B403;

    for (;;)
    {
        if (pX2 <= 31)
            if (pY2 <= 31)
                break;

        pX2 >>= 1;
        pY2 >>= 1;
        Data10 += 1;
    }

    pY2 <<= 5;
    pY2 |= pX2;
    pX = 0;
    pX = mMap_Distance_Calculations[pY2];

    pX <<= Data10;
    return;

loc_2B403:;
    pX = getWindowWidth() * 2;
}

bool cFodder::MapTile_Update_Position()
{

    MapTile_Update_Y();
    MapTile_Update_X();

    int16 TileColumns = mMapTile_DrawX - mMapTile_Column_CurrentScreen;
    int16 TileRows = mMapTile_DrawY - mMapTile_Row_CurrentScreen;

    mMapTile_Column_CurrentScreen = mMapTile_DrawX;
    mMapTile_Row_CurrentScreen = mMapTile_DrawY;

    if (TileColumns)
    {
        if (TileColumns < 0)
            MapTile_Move_Left(-TileColumns);
        else
            MapTile_Move_Right(TileColumns);
    }

    if (TileRows)
    {
        if (TileRows < 0)
            MapTile_Move_Up(-TileRows);
        else
            MapTile_Move_Down(TileRows);
    }

    if (TileColumns || TileRows)
    {
        // if (!mStartParams->mDisableVideo)
        //     mGraphics->MapTiles_Draw();
        return true;
    }

    return false;
}

void cFodder::MapTile_Move_Right(int16 pPanTiles)
{

    for (; pPanTiles > 0; --pPanTiles)
    {

        ++mMapTile_ColumnOffset;
        mMapTile_ColumnOffset &= 0x0F;
        if (!mMapTile_ColumnOffset)
        {
            mMapTile_Ptr += 2;
            ++mMapTile_MovedHorizontal;
        }
    }
}

void cFodder::MapTile_Move_Left(int16 pPanTiles)
{

    for (; pPanTiles > 0; --pPanTiles)
    {

        --mMapTile_ColumnOffset;
        mMapTile_ColumnOffset &= 0x0F;
        if (mMapTile_ColumnOffset == 0x0F)
        {
            mMapTile_Ptr -= 2;
            --mMapTile_MovedHorizontal;
        }
    }
}

void cFodder::MapTile_Move_Down(int16 pPanTiles)
{

    for (; pPanTiles > 0; --pPanTiles)
    {

        ++mMapTile_RowOffset;
        mMapTile_RowOffset &= 0x0F;
        if (!mMapTile_RowOffset)
        {
            mMapTile_Ptr += (mMapLoaded->getWidth() << 1);
            ++mMapTile_MovedVertical;
        }
    }
}

void cFodder::MapTile_Move_Up(int16 pPanTiles)
{

    for (; pPanTiles > 0; --pPanTiles)
    {

        --mMapTile_RowOffset;
        mMapTile_RowOffset &= 0x0F;
        if (mMapTile_RowOffset == 0x0F)
        {
            mMapTile_Ptr -= (mMapLoaded->getWidth() << 1);
            --mMapTile_MovedVertical;
        }
    }
}

void cFodder::MapTile_Update_Y()
{
    int16 Data0 = mMapTile_MoveDirectionY;

    if (Data0 < 0)
        Data0 = -1;
    else
        Data0 = 1;
    // Direction Change?
    if (Data0 != mMapTile_MoveDirectionY_Previous)
    {
        mMapTile_MoveDirectionY_Previous = Data0;
        Data0 = mMapTile_SpeedY >> 16;
        int16 Data4 = mMapTile_SpeedY_Previous;

        Data0 >>= 4;
        Data4 >>= 4;
        if (Data4 != Data0)
            return;
    }

    mCamera_MovePauseY = 0;
    mMapTile_DrawY = mMapTile_TargetY >> 16;
    mMapTile_SpeedY_Previous = mMapTile_SpeedY >> 16;
}

void cFodder::MapTile_Update_X()
{
    int16 Data0 = mMapTile_MoveDirectionX;

    if (Data0 < 0)
        Data0 = -1;
    else
        Data0 = 1;

    if (Data0 != mMapTile_MoveDirectionX_Previous)
    {
        mMapTile_MoveDirectionX_Previous = Data0;
        Data0 = mMapTile_SpeedX >> 16;
        int16 Data4 = mMapTile_SpeedX_Previous;
        Data0 >>= 4;
        Data4 >>= 4;
        if (Data0 != Data4)
            return;
    }

    mCamera_MovePauseX = 0;
    mMapTile_DrawX = mMapTile_TargetX >> 16;
    mMapTile_SpeedX_Previous = mMapTile_SpeedX >> 16;
}

int32 cFodder::MapTile_Get(const size_t pTileX, const size_t pTileY)
{

    if ((int32)pTileX > mMapLoaded->getWidth() || (int32)pTileY > mMapLoaded->getHeight())
        return -1;

    size_t Tile = (((pTileY * mMapLoaded->getWidth()) + pTileX)) + mMapLoaded->getWidth();

    uint8 *CurrentMapPtr = mMap->data() + mMapTile_Ptr + (Tile * 2);
    if (CurrentMapPtr > mMap->data() + mMap->size())
        return -1;

    return readLE<int16>(CurrentMapPtr);
}

/**
 * Sets a map tile to a certain ID, positioned based on the current camera
 */
void cFodder::MapTile_Set(const size_t pTileX, const size_t pTileY, const size_t pTileID)
{

    if ((int32)pTileX > mMapLoaded->getWidth() || (int32)pTileY > mMapLoaded->getHeight())
        return;

    size_t Tile = (((pTileY * mMapLoaded->getWidth()) + pTileX)) + mMapLoaded->getWidth();

    uint8 *CurrentMapPtr = mMap->data() + mMapTile_Ptr + (Tile * 2);
    if (CurrentMapPtr > mMap->data() + mMap->size())
        return;

    writeLEWord(CurrentMapPtr, (uint16)pTileID);
}

void cFodder::Map_Destroy_Tiles()
{
    uint8 *MapPtr = 0;
    const int16 *IndestructibleTypes = 0;
    int32 Data0, Data4, MapTile, TileType;
    sMapPosition LastTile;

    if (mMap_Destroy_Tiles_Countdown)
    {
        --mMap_Destroy_Tiles_Countdown;
        goto loc_2DFC7;
    }

    for (auto &Tile : mMap_Destroy_Tiles)
    {
        bool TriggerExplosion = false;

        LastTile = Tile;

        Data0 = Tile.mX;
        Data4 = Tile.mY;

        mMap_Destroy_Tile_X = Tile.mX;
        mMap_Destroy_Tile_Y = Tile.mY;

        if (mMap_Destroy_Tile_Y < 0)
        {
            TriggerExplosion = true;
            Data4 = -Data4;
            mMap_Destroy_Tile_Y = -Tile.mY;
        }
        // loc_2DE89
        Data4 >>= 4;
        Data4 *= mMapLoaded->getWidth();

        Data0 >>= 4;
        Data4 += Data0;
        Data4 <<= 1;

        // In some cases, tiles outside the map can be 'destroyed'. This continue prevents memory corruption
        if ((size_t)(0x60 + Data4) >= mMap->size())
            continue;

        MapPtr = mMap->data() + 0x60 + Data4;
        MapTile = readLE<uint16>(MapPtr);
        MapTile &= 0x1FF;
        TileType = MapTile;

        Data4 = mTile_Destroy_Swap[MapTile];
        if (Data4 < 0)
            continue;

        if (TriggerExplosion)
            goto loc_2DF7B;

        IndestructibleTypes = mTiles_Indestructible[mMapLoaded->getTileType()];

        int16 ax;
        do
        {
            if (*IndestructibleTypes < 0)
                goto loc_2DF7B;

            ax = *IndestructibleTypes;
            IndestructibleTypes++;

        } while (TileType != ax);
    }

    mMap_Destroy_Tiles.clear();
    return;

    // seg010:064C
    // UNUNSED BLOCK

loc_2DF7B:;
    Data0 = Sprite_Create_Building_Explosion_Wrapper(mMap_Destroy_Tile_X, mMap_Destroy_Tile_Y);
    if (Data0)
        return;

    mMap_Destroy_Tiles_Countdown = 1;
    writeLEWord(MapPtr, Data4);
    mMap_Destroy_Tile_LastTile = Data4;

    while (!(*mMap_Destroy_Tiles.begin() == LastTile))
        Map_Destroy_Tiles_Next();

    Map_Destroy_Tiles_Next();

loc_2DFC7:;
    int16 ax = (mMap_Destroy_Tile_X >> 4);
    ax -= mMapTile_MovedHorizontal;
    ax <<= 4;
    ax -= mMapTile_ColumnOffset;
    ax += 0x40;

    mVideo_Draw_PosX = ax;

    ax = (mMap_Destroy_Tile_Y >> 4);
    ax -= mMapTile_MovedVertical;
    ax <<= 4;
    ax -= mMapTile_RowOffset;
    ax += 0x10;

    mVideo_Draw_PosY = ax;

    mVideo_Draw_Columns = 0x10;
    mVideo_Draw_Rows = 0x10;
    // if (!mStartParams->mDisableVideo)
    //     mGraphics->MapTiles_Draw();
}

void cFodder::Map_Destroy_Tiles_Next()
{

    if (mMap_Destroy_Tiles.size())
        mMap_Destroy_Tiles.erase(mMap_Destroy_Tiles.begin());
}

/**
 * Check the distance between the provided sprite, and the squad leader
 *
 * @param pSprite
 * @param pData0
 *
 * @return 0 if within range, 1 if not
 */
int16 cFodder::Map_Get_Distance_Between_Sprite_And_Squadleader(sSprite *pSprite, int16 &pData0)
{
    int16 Data4, Data8, DataC, Data10;

    // Current Squad in vehicle?
    if (mSquad_CurrentVehicle)
        return 1;

    // No Squad Leader?
    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return 1;

    // Vehicle?
    if (mSquad_Leader->mInVehicle)
        return 1;

    // Anim playing?
    if (mSquad_Leader->mAnimState)
        return 1;

    pData0 = pSprite->mPosX;

    Data4 = pSprite->mPosY;
    Data8 = mSquad_Leader->mPosX;
    DataC = mSquad_Leader->mPosY;
    Data10 = 0x20;

    DataC += 2;

    Map_Get_Distance_BetweenPoints(pData0, Data4, Data8, Data10, DataC);
    if (pData0 >= 6)
        return 1;

    Data4 = 0;
    return 0;
}

void cFodder::MapTiles_ResetScrollState()
{
    mMapTile_ColumnOffset = 0;
    mMapTile_RowOffset = 0;

    // 0x60 - SidebarWidth - MapWidth
    mMapTile_Ptr = (0x60 - 8) - (mMapLoaded->getWidth() << 1);

    // No sidebar in OFED
#ifdef _OFED
    mMapTile_Ptr += 8;
#endif

    mMapTile_MovedHorizontal = 0;
    mMapTile_MovedVertical = 0;
    mMapTile_Column_CurrentScreen = 0;
    mMapTile_Row_CurrentScreen = 0;
}
