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

class Palette;

class Cursors {
public:
	/** A cursor. */
	struct Cursor {
		uint16 width;    ///< The cursor's width.
		uint16 height;   ///< The cursor's height.
		uint16 hotspotX; ///< The X coordinate of the cursor's hotspot.
		uint16 hotspotY; ///< The Y coordinate of the cursor's hotspot.
		Sprite *sprite;  ///< The cursor's sprite.
	};

	static const int _cursorWidth  = 32; ///< The width of a cursor.
	static const int _cursorHeight = 32; ///< The height of a cursor.

	Cursors(const Common::String &exe = "");
	~Cursors();

	/** Is the cursor visible? */
	bool isVisible() const;
	/** Hide/Show the cursor. */
	void setVisible(bool visible);

	/** Get the cursor with that name. */
	const Cursor *getCursor(const Common::String &cursor = "") const;

	/** Set the current cursor. */
	bool setCursor(const Cursor &cursor);
	/** Set the current cursor. */
	bool setCursor(const Common::String &cursor = "");

	/** Load cursors from a NE EXE. */
	bool loadFromNEEXE(const Common::String &exe);

private:

	typedef Common::HashMap<Common::String, Cursor, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> CursorMap;

	Cursor _default;    ///< The default pointer cursor.
	CursorMap _cursors; ///< The available cursors.

	/** Load the cursors from static data. */
	bool loadFromStatics();

	/** Set the cursor's palette. */
	bool setPalette(const Palette &palette);
};

/** A static cursor datum. */
struct StaticCursor {
	uint16 width;  ///< The cursor's width.
	uint16 height; ///< The cursor's height.

	uint16 hotspotX; ///< The X coordinate of the cursor's hotspot.
	uint16 hotspotY; ///< The Y coordinate of the cursor's hotspot.

	/** The pixel data. */
	byte *pixels;
	/** The mask data. */
	byte *mask;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CURSORS_H
