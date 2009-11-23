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

#include "graphics/cursorman.h"

#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/palette.h"

namespace DarkSeed2 {

#include "engines/darkseed2/cursordata.h"

Cursors::Cursors() {
	bool loaded = loadFromStatics();

	assert(loaded);
}

Cursors::~Cursors() {
	for (CursorMap::iterator it = _cursors.begin(); it != _cursors.end(); ++it)
		delete it->_value.sprite;

	delete _default.sprite;

	_cursors.clear();
}

bool Cursors::loadFromStatics() {
	for (int i = 0; i < staticCursorCount; i++) {
		Cursor cursor;

		cursor.sprite = new Sprite;

		if (!cursor.sprite->loadFromStaticCursor(staticCursors[i])) {
			delete cursor.sprite;
			return false;
		}

		cursor.hotspotX = staticCursors[i].hotspotX;
		cursor.hotspotY = staticCursors[i].hotspotY;

		_cursors.setVal(staticCursorNames[i], cursor);
	}

	_default.hotspotX = 0;
	_default.hotspotY = 0;
	_default.sprite = new Sprite;

	if (!_default.sprite->loadFromStaticCursor(staticCursorPointer)) {
		delete _default.sprite;
		return false;
	}

	return true;
}

bool Cursors::isVisible() const {
	return CursorMan.isVisible();
}

void Cursors::setVisible(bool visible) {
	CursorMan.showMouse(visible);
}

const Cursors::Cursor *Cursors::getCursor(const Common::String &cursor) const {
	if (cursor.empty())
		return &_default;

	if (!_cursors.contains(cursor))
		return 0;

	return &_cursors.getVal(cursor);
}

bool Cursors::setCursor(const Common::String &cursor) {
	const Cursor *cur = getCursor(cursor);

	if (!cur)
		return false;

	return setCursor(*cur);
}

bool Cursors::setCursor(const Cursors::Cursor &cursor) {
	CursorMan.replaceCursor(cursor.sprite->getData(), _cursorWidth, _cursorHeight,
			cursor.hotspotX, cursor.hotspotY, 0);

	return setPalette(cursor.sprite->getPalette());
}

bool Cursors::setPalette(const Palette &palette) {
	byte newPal[12];

	newPal[ 0] = palette[0];
	newPal[ 1] = palette[1];
	newPal[ 2] = palette[2];
	newPal[ 3] = 255;
	newPal[ 4] = palette[3];
	newPal[ 5] = palette[4];
	newPal[ 6] = palette[5];
	newPal[ 7] = 255;
	newPal[ 8] = palette[6];
	newPal[ 9] = palette[7];
	newPal[10] = palette[8];
	newPal[11] = 255;

	CursorMan.replaceCursorPalette(newPal, 0, 3);

	return true;
}

} // End of namespace DarkSeed2
