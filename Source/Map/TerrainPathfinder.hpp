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
#pragma once

#include <functional>
#include <vector>

#include "../Types.hpp"

class cPosition;

class cTerrainPathfinder {
public:
	typedef std::function<int(int32, int32)> tPassableFn;

	cTerrainPathfinder(int32 pMaxX, int32 pMaxY, tPassableFn pPassable);

	std::vector<cPosition> findPath(const cPosition* pStart, const cPosition* pEnd) const;

private:
	int32 mMaxX;
	int32 mMaxY;
	tPassableFn mPassable;
};
