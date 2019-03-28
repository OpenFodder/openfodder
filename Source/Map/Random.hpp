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

class cScriptingEngine;

class cRandomMap : public cOriginalMap {
	friend class cScriptingEngine;
private:

protected:

	virtual void Randomise_Structures(const size_t pCount);
	virtual void Randomise_Sprites(const size_t pHumanCount = 2);

public:
	cRandomMap(const sMapParams& pParams);
	virtual void Randomise();
	
	bool					CheckRadiusTerrain(eTerrainType pType, cPosition* pPosition, int32 pRadius);

	int32					getSpriteTypeCount(size_t pSpriteType);
	std::vector<sSprite*>	getSpritesByType(size_t pSpriteType);
	float					getRandomFloat(float pMin, float pMax);
	int32					getRandomInt(int32 pMin = 0, int32 pMax = 0);
	cPosition*				getRandomXYByTerrainType(eTerrainType pType, size_t pRadius);

};

