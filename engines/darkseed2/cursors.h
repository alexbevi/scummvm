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

#ifndef DARKSEED2_CURSORS_H
#define DARKSEED2_CURSORS_H

#include "common/str.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

class Cursors {
public:
	static const int _cursorWidth  = 32;
	static const int _cursorHeight = 32;

	Cursors();
	~Cursors();

	bool isVisible() const;
	void setVisible(bool visible);

	bool setCursor(const Common::String &cursor = "");

private:
	struct Cursor {
		uint16 hotspotX;
		uint16 hotspotY;
		Sprite *sprite;
	};

	typedef Common::HashMap<Common::String, Cursor, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> CursorMap;

	Cursor _default;
	CursorMap _cursors;

	bool loadFromStatics();

	bool setCursor(const Cursor &cursor);
	bool setPalette(const byte *palette);
};

struct StaticCursor {
	uint16 hotspotX;
	uint16 hotspotY;
	byte pixels[Cursors::_cursorWidth * Cursors::_cursorHeight / 8];
	byte mask  [Cursors::_cursorWidth * Cursors::_cursorHeight / 8];
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CURSORS_H
