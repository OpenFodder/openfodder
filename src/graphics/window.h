/*
 *  Open Fodder
 *  ------------------------
 *
 *  Copyright (C) 2010 Robert Crossfield
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
 *  ------------------------------------------
 *  SDL Video Window Wrapper
 *  ------------------------------------------
 */
#include<sdl.h>

class cVideoWindow {
protected:
	SDL_Surface		*_surface;
	size_t			 _width, _height;
	size_t			 _pixelBytes;

public:
						cVideoWindow( size_t pWidth, size_t pHeight, size_t pBytesPixel, bool pFullScreen );
					~cVideoWindow();

	void clear( byte color ) {
		
		SDL_FillRect( _surface, NULL, color );
	}

	void blit( SDL_Surface *source, word x, word y, word pSourceX = 0, word pSourceY = 0) {
		SDL_Rect srcRect, destRect;

		srcRect.x = pSourceX;
		srcRect.y = pSourceY;

		srcRect.h = source->h;
		srcRect.w = source->w;

		destRect.x = x;
		destRect.y = y;
		destRect.h = source->h;
		destRect.w = source->w;

		SDL_SetColorKey( source, SDL_SRCCOLORKEY, 0xFF);
		SDL_BlitSurface( source, &srcRect, _surface, &destRect);

		SDL_UpdateRect(  _surface, 0,0,0,0);
	}

	void titleSet( string pWindowTitle ) {
		SDL_WM_SetCaption( pWindowTitle.c_str(), 0 );
	}

};
