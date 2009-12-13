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
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

Mike::Mike(Resources &resources, Graphics &graphics) {
	_resources = &resources;
	_graphics  = &graphics;

	_x = 0;
	_y = 0;

	_state = kStateStanding;
	_direction = kDirE;

	setWalkMap();
}

Mike::~Mike() {
}

Common::Rect Mike::getArea() const {
	return _animations[_state][_direction]->getArea();
}

bool Mike::init() {
	if (!loadAnimations())
		return false;

	return true;
}

bool Mike::loadAnimations() {
	if (!_animations[kStateWalking][kDirN].load(*_resources, "n"))
		return false;
	if (!_animations[kStateWalking][kDirNE].load(*_resources, "nw"))
		return false;
	if (!_animations[kStateWalking][kDirE].load(*_resources, "w"))
		return false;
	if (!_animations[kStateWalking][kDirSE].load(*_resources, "sw"))
		return false;
	if (!_animations[kStateWalking][kDirS].load(*_resources, "s"))
		return false;
	if (!_animations[kStateWalking][kDirSW].load(*_resources, "sw"))
		return false;
	if (!_animations[kStateWalking][kDirW].load(*_resources, "w"))
		return false;
	if (!_animations[kStateWalking][kDirNW].load(*_resources, "nw"))
		return false;

	if (!_animations[kStateStanding][kDirN].load(*_resources, "n00"))
		return false;
	if (!_animations[kStateStanding][kDirNE].load(*_resources, "nw00"))
		return false;
	if (!_animations[kStateStanding][kDirE].load(*_resources, "w00"))
		return false;
	if (!_animations[kStateStanding][kDirSE].load(*_resources, "sw00"))
		return false;
	if (!_animations[kStateStanding][kDirS].load(*_resources, "s00"))
		return false;
	if (!_animations[kStateStanding][kDirSW].load(*_resources, "sw00"))
		return false;
	if (!_animations[kStateStanding][kDirW].load(*_resources, "w00"))
		return false;
	if (!_animations[kStateStanding][kDirNW].load(*_resources, "nw00"))
		return false;

	_animations[kStateWalking][kDirNE].flipHorizontally();
	_animations[kStateWalking][kDirE].flipHorizontally();
	_animations[kStateWalking][kDirSE].flipHorizontally();
	_animations[kStateStanding][kDirNE].flipHorizontally();
	_animations[kStateStanding][kDirE].flipHorizontally();
	_animations[kStateStanding][kDirSE].flipHorizontally();

	return true;
}

void Mike::getPosition(uint32 &x, uint32 &y) const {
	x = _x;
	y = _y;
}

void Mike::setPosition(uint32 x, uint32 y) {
	_x = x;
	_y = y;

	_graphics->requestRedraw(_animations[_state][_direction]->getArea());

	for (uint j = 0; j < kStateNone; j++)
		for (uint i = 0; i < kDirNone; i++)
			_animations[j][i].moveFeet(x, y);

	_graphics->requestRedraw(_animations[_state][_direction]->getArea());
}

Mike::Direction Mike::getDirection() const {
	return _direction;
}

void Mike::setDirection(Direction direction) {
	if ((direction < 0) || (direction >= kDirNone))
		direction = kDirE;

	_graphics->requestRedraw(_animations[_state][_direction]->getArea());

	_direction = direction;

	_graphics->requestRedraw(_animations[_state][_direction]->getArea());
}

void Mike::redraw(Sprite &sprite, Common::Rect area) {
	if ((_x > 0) && (_y > 0))
		_animations[_state][_direction]->redraw(sprite, area);
}

void Mike::setWalkMap() {
	memset(_walkMap, 0, sizeof(_walkMap));
}

void Mike::setWalkMap(const Sprite &walkMap) {
	if ((walkMap.getWidth() != kWalkMapWidth) || (walkMap.getHeight() != kWalkMapHeight)) {
		warning("Mike::setWalkMap(): Invalid walk map dimensions: %dx%d",
				walkMap.getWidth(), walkMap.getHeight());
		setWalkMap();
		return;
	}

	memcpy(_walkMap, walkMap.getData(), sizeof(_walkMap));
}

byte Mike::getWalkData(uint32 x, uint32 y) const {
	return getWalk(screenCoordToWalkCoord(x), screenCoordToWalkCoord(y));
}

inline uint32 Mike::screenCoordToWalkCoord(uint32 walkCoord) {
	return walkCoord / kWalkMapResolution;
}

inline byte Mike::getWalk(uint32 x, uint32 y) const {
	assert((x < kWalkMapWidth) && (y < kWalkMapHeight));

	return _walkMap[y * kWalkMapWidth + x];
}

} // End of namespace DarkSeed2
