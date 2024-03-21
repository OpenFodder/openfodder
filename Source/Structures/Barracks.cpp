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

const std::vector<sStructure> mStructuresBarracksWithSoldier = {

	// Jungle
	{
		{
			{ 1, 0, 333 },
			{ 2, 0, 334 },

			{ 0, 1, 352 },
			{ 1, 1, 353 },
			{ 2, 1, 354 },

			{ 0, 2, 372 },
			{ 1, 2, 373 },
			{ 2, 2, 374 },
			{ 3, 2, 375 },

			{ 0, 3, 392 },
			{ 1, 3, 393 },
			{ 2, 3, 394 },
		},

		{
            { 13, 18, eSprite_BuildingRoof },
			{ 9, 50, eSprite_BuildingDoor }
		},
	},

	// Ice
	{
		{
			{ 1, 0, 245 },
			{ 2, 0, 246 },
			{ 3, 0, 247 },

			{ 0, 1, 264 },
			{ 1, 1, 265 },
			{ 2, 1, 266 },
			{ 3, 1, 267 },

			{ 0, 2, 284 },
			{ 1, 2, 285 },
			{ 2, 2, 286 },
			{ 3, 2, 287 },
		},
		{
			{ 23, 11, eSprite_BuildingRoof },
			{ 20, 43, eSprite_BuildingDoor }
		},
	},


	// Desert
	{
		{
			{ 0, 0, 196 },
			{ 1, 0, 197 },
			{ 2, 0, 198 },

			{ 0, 1, 216 },
			{ 1, 1, 217 },
			{ 2, 1, 218 },

			{ 0, 2, 236 },
			{ 1, 2, 237 },
			{ 2, 2, 238 },
		},
		{
			{ 12, 1, eSprite_BuildingRoof },
			{ 7, 32, eSprite_BuildingDoor }
		},
	},


	// Moors
	{
		{
			{ 1, 0, 335 },
			{ 2, 0, 336 },

			{ 0, 1, 354 },
			{ 1, 1, 355 },
			{ 2, 1, 356 },

			{ 0, 2, 374 },
			{ 1, 2, 375 },
			{ 2, 2, 376 },

			{ 0, 3, 394 },
			{ 1, 3, 395 },
			{ 2, 3, 396 },
		},
		{
			{ 15, 17, eSprite_BuildingRoof },
			{ 7, 49, eSprite_BuildingDoor }
		},
	},


	// Int
	{
		{
			{ 0, 0, 246 },
			{ 0, 1, 266 }
		},
		{
			{ 3, 21, eSprite_BuildingDoor }
		},
	},

	// HID
	{
		{
			{ 0, 0, 246 },
			{ 0, 1, 266 }
		},
		{
			{ 3, 21, eSprite_BuildingDoor }
		},
	},

	// AFX
	{
		{
			{ 0, 0, 371 },
			{ 1, 0, 372 },
			{ 2, 0, 373 },

			{ 0, 1, 391 },
			{ 1, 1, 392 },
			{ 2, 1, 393 },

			{ 0, 2, 375 },
			{ 1, 2, 376 },
			{ 2, 2, 377 },

			{ 0, 3, 395 },
			{ 1, 3, 396 },
			{ 2, 3, 397 },
		},
		{
			{ 21, 56, eSprite_BuildingDoor }
		},
	},
};
