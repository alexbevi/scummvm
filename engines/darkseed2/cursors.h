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
#include "engines/darkseed2/saveable.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

class Palette;
class NECursor;
class Resources;

class Cursors : public Saveable {
public:
	/** A cursor. */
	struct Cursor {
		Common::String name; ///< The cursor's name.
		int32 width;         ///< The cursor's width.
		int32 height;        ///< The cursor's height.
		int32 hotspotX;      ///< The X coordinate of the cursor's hotspot.
		int32 hotspotY;      ///< The Y coordinate of the cursor's hotspot.
		Sprite *sprite;      ///< The cursor's sprite.
	};

	Cursors();
	virtual ~Cursors();

	/** Load cursors */
	virtual bool load() = 0;

	/** Make sure the class's information on the cursor is in sync with the sytem's. */
	void assertCursorProperties();

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

	/** Get the current cursor's name. */
	const Common::String &getCurrentCursor() const;

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

	typedef Common::HashMap<Common::String, Cursor, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> CursorMap;

	bool _visible; ///< Is the cursor visible?
	Common::String _currentCursor; ///< The name of the current cursor.

	CursorMap _cursors; ///< The available cursors.
};

class CursorsWindows : public Cursors {
public:
	CursorsWindows(const Common::String &exeName);

	bool load();

private:
	Common::String _exeName; ///< Name of the NE EXE

	/** Load a cursor from a NE resource. */
	bool loadFromResource(Cursor &cursor, const NECursor &resource);
};

class CursorsSaturn : public Cursors {
public:
	CursorsSaturn(Resources &resources);

	bool load();

private:
	Resources *_resources;

	static const char *_saturnCursors[];
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CURSORS_H
