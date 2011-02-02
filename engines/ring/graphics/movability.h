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

#ifndef RING_MOVABILITY_H
#define RING_MOVABILITY_H

#include "ring/shared.h"

namespace Ring {

class Hotspot;
class Puzzle;

class Movability {
public:
	Movability(Puzzle *puzzleFrom, Puzzle *puzzleTo, Common::String name, uint32 a5);
	~Movability();

	void setHotspot(Common::Rect rect, bool enabled, uint32 a3, uint32 a4);
	Hotspot *getHotspot();

	Common::String getRideName() { return _name; }
	void setRideName(Common::String name) { _name = name; }

private:
	uint32   _field_0;
	Puzzle  *_puzzleFrom;
	Hotspot *_hotspot;
	Puzzle  *_puzzleTo;
	Common::String _name;
	uint32   _field_14;
	uint32   _field_18;
	uint32   _field_1C;
	float    _field_20;
	uint32   _field_24;
	byte     _field_28;
	uint32   _field_29;
	uint32   _field_2D;
	uint32   _field_31;
};

} // End of namespace Ring

#endif // RING_MOVABILITY_H
