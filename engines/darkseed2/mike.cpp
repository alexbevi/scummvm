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

#include "engines/darkseed2/mike.h"

namespace DarkSeed2 {

Mike::Mike() {
	_x = 0;
	_y = 0;

	_direction = kDirE;
}

Mike::~Mike() {
}

void Mike::getPosition(uint32 &x, uint32 &y) const {
	x = _x;
	y = _y;
}

void Mike::setPosition(uint32 x, uint32 y) {
	_x = x;
	_y = y;
}

Mike::Direction Mike::getDirection() const {
	return _direction;
}

void Mike::setDirection(Direction direction) {
	if ((direction < 0) || (direction >= kDirNone))
		direction = kDirE;

	_direction = direction;
}

} // End of namespace DarkSeed2
