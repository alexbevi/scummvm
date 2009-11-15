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

#ifndef DARKSEED2_GRAPHICS_H
#define DARKSEED2_GRAPHICS_H

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/sprite.h"

#include "common/rect.h"

namespace DarkSeed2 {

class Graphics {
public:
	static const int _screenWidth  = 640;
	static const int _screenHeight = 480;

	Graphics();
	~Graphics();

	void clearPalette();
	void setPalette(const byte *pal);

	void blitToScreen(const Sprite &from,
			uint32 left, uint32 top, uint32 right, uint32 bottom,
			uint32 x, uint32 y, bool transp = false);
	void blitToScreen(const Sprite &from, uint32 x, uint32 y, bool transp = false);

	void retrace();

private:
	byte _gamePalette[768];
	Sprite _screen;

	bool _dirtyAll;
	Common::List<Common::Rect> _dirtyRects;

	void applyGamePalette();

	void dirtyRectsAdd(uint32 left, uint32 top, uint32 right, uint32 bottom);
	bool dirtyRectsApply();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_GRAPHICS_H
