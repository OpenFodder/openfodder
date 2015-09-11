/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield <robcrossfield@gmail.com>
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

enum eSpriteType {
	eSPRITE_IN_GAME = 0,
};

class cGraphics : public cSingleton<cGraphics> {
	
protected:
	cSurface*			mImage;

public:
						cGraphics();

	virtual uint8*		GetSpriteData( uint16 pSegment ) = 0;
	virtual void		LoadpStuff() = 0;
	virtual void		graphicsBlkPtrsPrepare() = 0;
	virtual void		map_Tiles_Draw() = 0;
	virtual void		map_Load_Resources() = 0;
	virtual void		PaletteSet() = 0;

	virtual void		video_Draw_Sprite() = 0;
	virtual void		SetSpritePtr( eSpriteType pSpriteType ) = 0;
};
