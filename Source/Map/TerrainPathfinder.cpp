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
#include "TerrainPathfinder.hpp"

#include <cstdint>
#include <limits>
#include <queue>
#include <unordered_map>

static inline int32 pathAbs(int32 pValue)
{
	return pValue < 0 ? -pValue : pValue;
}

static inline std::uint64_t terrainPathKey(int32 pX, int32 pY)
{
	return (static_cast<std::uint64_t>(static_cast<std::uint32_t>(pX)) << 32) |
		static_cast<std::uint32_t>(pY);
}

static inline int32 terrainPathKeyX(std::uint64_t pKey)
{
	return static_cast<int32>(pKey >> 32);
}

static inline int32 terrainPathKeyY(std::uint64_t pKey)
{
	return static_cast<int32>(pKey & 0xFFFFFFFFu);
}

static float terrainPathHeuristic(const cPosition& pStart, const cPosition& pEnd)
{
	const int32 dx = pathAbs(pStart.mX - pEnd.mX);
	const int32 dy = pathAbs(pStart.mY - pEnd.mY);
	if (dx + dy <= 8)
		return 0.0f;

	const float stepDx = dx / 8.0f;
	const float stepDy = dy / 8.0f;
	const float diagonal = stepDx < stepDy ? stepDx : stepDy;
	const float straight = (stepDx > stepDy ? stepDx : stepDy) - diagonal;
	return straight + (1.41421356f * diagonal);
}

struct TerrainPathOpenNode {
	std::uint64_t key;
	cPosition position;
	float costFromStart;
	float estimatedTotal;
	std::uint32_t order;
};

struct TerrainPathOpenCompare {
	bool operator()(const TerrainPathOpenNode& pLeft, const TerrainPathOpenNode& pRight) const
	{
		if (pLeft.estimatedTotal != pRight.estimatedTotal)
			return pLeft.estimatedTotal > pRight.estimatedTotal;
		return pLeft.order > pRight.order;
	}
};

struct TerrainPathRecord {
	TerrainPathRecord()
		: costFromStart(std::numeric_limits<float>::infinity()),
		parentKey(0),
		hasParent(false),
		closed(false)
	{
	}

	float costFromStart;
	std::uint64_t parentKey;
	bool hasParent;
	bool closed;
};

cTerrainPathfinder::cTerrainPathfinder(int32 pMaxX, int32 pMaxY, tPassableFn pPassable)
	: mMaxX(pMaxX),
	mMaxY(pMaxY),
	mPassable(pPassable)
{
}

std::vector<cPosition> cTerrainPathfinder::findPath(const cPosition* pStart, const cPosition* pEnd) const
{
	std::vector<cPosition> path;
	if (!pStart || !pEnd || !mPassable)
		return path;

	if (*pStart == *pEnd) {
		path.push_back(*pStart);
		return path;
	}

	if (pStart->mX < 0 || pStart->mY < 0 || pStart->mX > mMaxX || pStart->mY > mMaxY)
		return path;
	if (pEnd->mX < 0 || pEnd->mY < 0 || pEnd->mX > mMaxX || pEnd->mY > mMaxY)
		return path;

	const int32 dx[8] = { 8, 8, 0, -8, -8, -8, 0, 8 };
	const int32 dy[8] = { 0, 8, 8, 8, 0, -8, -8, -8 };
	const float stepCost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

	std::priority_queue<TerrainPathOpenNode, std::vector<TerrainPathOpenNode>, TerrainPathOpenCompare> open;
	std::unordered_map<std::uint64_t, TerrainPathRecord> records;
	records.reserve(2048);

	std::uint32_t order = 0;
	const std::uint64_t startKey = terrainPathKey(pStart->mX, pStart->mY);
	TerrainPathRecord& startRecord = records[startKey];
	startRecord.costFromStart = 0.0f;
	open.push({ startKey, *pStart, 0.0f, terrainPathHeuristic(*pStart, *pEnd), order++ });

	while (!open.empty()) {
		const TerrainPathOpenNode current = open.top();
		open.pop();

		auto currentRecordIt = records.find(current.key);
		if (currentRecordIt == records.end())
			continue;

		TerrainPathRecord& currentRecord = currentRecordIt->second;
		if (current.costFromStart != currentRecord.costFromStart || currentRecord.closed)
			continue;

		currentRecord.closed = true;
		if (current.position == *pEnd || current.position.distanceTo(*pEnd) <= 8) {
			std::vector<cPosition> reversed;
			for (std::uint64_t key = current.key;;) {
				reversed.push_back(cPosition(
					static_cast<unsigned int>(terrainPathKeyX(key)),
					static_cast<unsigned int>(terrainPathKeyY(key))));

				const TerrainPathRecord& record = records[key];
				if (!record.hasParent)
					break;
				key = record.parentKey;
			}

			path.reserve(reversed.size() + 1);
			for (auto it = reversed.rbegin(); it != reversed.rend(); ++it)
				path.push_back(*it);

			if (path.empty() || path.back() != *pEnd)
				path.push_back(*pEnd);

			return path;
		}

		for (int direction = 0; direction < 8; ++direction) {
			const int32 nx = current.position.mX + dx[direction];
			const int32 ny = current.position.mY + dy[direction];
			if (nx < 0 || ny < 0 || nx > mMaxX || ny > mMaxY)
				continue;

			const int pass = mPassable(nx, ny);
			if (pass <= 0)
				continue;

			const float moveCost = pass == 1 ? stepCost[direction] : stepCost[direction] + pass;
			const float tentativeCost = currentRecord.costFromStart + moveCost;
			const std::uint64_t nextKey = terrainPathKey(nx, ny);
			TerrainPathRecord& nextRecord = records[nextKey];
			if (tentativeCost >= nextRecord.costFromStart)
				continue;

			nextRecord.costFromStart = tentativeCost;
			nextRecord.parentKey = current.key;
			nextRecord.hasParent = true;
			nextRecord.closed = false;

			const cPosition nextPosition(static_cast<unsigned int>(nx), static_cast<unsigned int>(ny));
			open.push({
				nextKey,
				nextPosition,
				tentativeCost,
				tentativeCost + terrainPathHeuristic(nextPosition, *pEnd),
				order++
			});
		}
	}

	return path;
}
