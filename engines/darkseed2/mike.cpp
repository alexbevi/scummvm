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
#include "engines/darkseed2/graphics.h"

namespace DarkSeed2 {

Mike::Mike(Resources &resources, Graphics &graphics) {
	_resources = &resources;
	_graphics  = &graphics;

	_x = 0;
	_y = 0;

	_direction = kDirE;
}

Mike::~Mike() {
}

Common::Rect Mike::getArea() const {
	return _animations[_direction]->getArea();
}

bool Mike::init() {
	if (!loadAnimations())
		return false;

	return true;
}

bool Mike::loadAnimations() {
	if (!_animations[kDirN].load(*_resources, "n"))
		return false;
	if (!_animations[kDirNE].load(*_resources, "nw"))
		return false;
	if (!_animations[kDirE].load(*_resources, "w"))
		return false;
	if (!_animations[kDirSE].load(*_resources, "sw"))
		return false;
	if (!_animations[kDirS].load(*_resources, "s"))
		return false;
	if (!_animations[kDirSW].load(*_resources, "sw"))
		return false;
	if (!_animations[kDirW].load(*_resources, "w"))
		return false;
	if (!_animations[kDirNW].load(*_resources, "nw"))
		return false;

	_animations[kDirNE].flipHorizontally();
	_animations[kDirE].flipHorizontally();
	_animations[kDirSE].flipHorizontally();

	return true;
}

void Mike::getPosition(uint32 &x, uint32 &y) const {
	x = _x;
	y = _y;
}

void Mike::setPosition(uint32 x, uint32 y) {
	_x = x;
	_y = y;

	_graphics->requestRedraw(_animations[_direction]->getArea());

	for (uint i = 0; i < kDirNone; i++)
		_animations[i].moveFeet(x, y);

	_graphics->requestRedraw(_animations[_direction]->getArea());
}

Mike::Direction Mike::getDirection() const {
	return _direction;
}

void Mike::setDirection(Direction direction) {
	if ((direction < 0) || (direction >= kDirNone))
		direction = kDirE;

	_graphics->requestRedraw(_animations[_direction]->getArea());

	_direction = direction;

	_graphics->requestRedraw(_animations[_direction]->getArea());
}

void Mike::redraw(Sprite &sprite, Common::Rect area) {
	if ((_x > 0) && (_y > 0))
		_animations[_direction]->redraw(sprite, area);
}

} // End of namespace DarkSeed2
