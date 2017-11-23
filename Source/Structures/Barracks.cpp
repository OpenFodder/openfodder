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
			{ 13, 2, eSprite_BuildingRoof },
			{ 9, 34, eSprite_BuildingDoor }
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
			{ 23, -5, eSprite_BuildingRoof },
			{ 20, 27, eSprite_BuildingDoor }
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
			{ 12, -15, eSprite_BuildingRoof },
			{ 7, 16, eSprite_BuildingDoor }
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
			{ 15, 1, eSprite_BuildingRoof },
			{ 7, 33, eSprite_BuildingDoor }
		},
	},


	// Int
	{
		{
			{ 0, 0, 246 },
			{ 0, 1, 266 }
		},
		{
			{ 3, 5, eSprite_BuildingDoor }
		},
	},
};
