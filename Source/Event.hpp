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

enum eEventType {
	eEvent_None				= 0,
	eEvent_KeyDown			= 1,
	eEvent_KeyUp			= 2,
	eEvent_MouseMove		= 3,
	eEvent_MouseLeftDown	= 4,
	eEvent_MouseLeftUp		= 5,
	eEvent_MouseRightDown	= 6,
	eEvent_MouseRightUp		= 7,
	eEvent_MouseWheel		= 8,
	eEvent_Quit				= 10,
	eEvent_Focus			= 11,
};

class cEvent {
	public:
		eEventType			mType;

		unsigned int		mButton;
		unsigned int		mButtonCount;

		cPosition			mPosition;
		cPosition			mPositionRelative;
		bool				mHasFocus;

	public:
							cEvent( const eEventType& pType = eEvent_None );
		virtual				~cEvent();
};
