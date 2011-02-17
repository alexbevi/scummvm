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
 * $Id$
 *
 */

#ifndef RING_SCREEN_H
#define RING_SCREEN_H

#include "graphics/surface.h"

namespace Ring {

class Image;

class ScreenManager {
public:
	ScreenManager();
	~ScreenManager();

	void sub_4028D0(int a1, int a2);

	void init();
	//
	void clear();
	//
	//
	//
	//
	//
	void update(Image *image, Common::Point point, byte a3);
	//
	//void readPalette();
	//
	//
	//
	//

	void updateScreen();

private:
	Graphics::Surface _screen;
};

} // End of namespace Ring

#endif // RING_SCREEN_H
