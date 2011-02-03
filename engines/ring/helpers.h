/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id: helpers.h 606 2010-08-17 21:30:59Z julien.templier $
 *
 */

#ifndef RING_HELPERS_H
#define RING_HELPERS_H

//////////////////////////////////////////////////////////////////////////
// Misc helpers
//////////////////////////////////////////////////////////////////////////

// Misc
#define getApp() ((RingEngine *)g_engine)->getApplication()
#define getSoundDirection() (((RingEngine *)g_engine)->getApplication()->getSoundHandler()->getDirection())

// Shared objects

//////////////////////////////////////////////////////////////////////////
// Cleanup
//////////////////////////////////////////////////////////////////////////

#define SAFE_DELETE(_p) do { delete (_p); (_p) = NULL; } while (false)
#define SAFE_FREE(_p)   do { free (_p); (_p) = NULL; } while (false)

#define CLEAR_ARRAY(type, name) { \
	for (Common::Array<type *>::iterator it = name.begin(); it != name.end(); it++) \
		SAFE_DELETE(*it); \
	name.clear(); \
}

#endif // RING_HELPERS_H
