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

#include "ring/graphics/movability.h"

#include "ring/game/puzzle.h"

#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"

namespace Ring {

Movability::Movability(Puzzle *puzzleFrom, Puzzle *puzzleTo, Common::String name, uint32 a5) {
	_field_0 = 0;
	_puzzleFrom = puzzleFrom;
	_hotspot = NULL;
	_puzzleTo = puzzleTo;
	_field_14 = a5;
	_field_18 = 0;
	_field_1C = 0;
	_field_20 = 85.0;
	_field_24 = 0;
	_field_28 = 2;
	_field_29 = 0;
	_field_2D = 0;
	_field_31 = 85.0;
}

Movability::~Movability() {
	SAFE_DELETE(_hotspot);

	// Zero-out passed pointers
	_puzzleFrom = NULL;
	_puzzleTo = NULL;
}

Hotspot *Movability::getHotspot() {
	if (!_hotspot)
		error("[Movability::getHotspot] Hotspot is not initialized properly");

	return _hotspot;
}

void Movability::setHotspot(Common::Rect rect, bool enabled, uint32 a3, uint32 a4) {
	_hotspot = new Hotspot(rect, enabled, 1, a3, a4);
}

} // End of namespace Ring
