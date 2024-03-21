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

#ifdef WIN64
	typedef unsigned char            uint8;     //!< One byte
	typedef unsigned short           uint16;    //!< Two bytes
	typedef unsigned int             uint32;    //!< Four bytes
	typedef unsigned long long int	 uint64;    //!< Eight bytes

	typedef signed char              int8;      //!< One byte
	typedef signed short             int16;     //!< Two bytes
	typedef signed int               int32;     //!< Four bytes
	typedef signed long long int     int64;     //!< Eight bytes


#else
	typedef unsigned char            uint8;     //!< One byte
	typedef unsigned short           uint16;    //!< Two bytes
	typedef unsigned int             uint32;    //!< Four bytes
	typedef unsigned long			 uint64;    //!< Eight bytes

	typedef signed char              int8;      //!< One byte
	typedef signed short             int16;     //!< Two bytes
	typedef signed int               int32;     //!< Four bytes
	typedef signed long			     int64;     //!< Eight bytes

#endif
