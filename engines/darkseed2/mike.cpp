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
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

static const char *kVariableVisible = "ShowMike";

Mike::Mike(Resources &resources, Variables &variables, Graphics &graphics) {
	_resources = &resources;
	_variables = &variables;
	_graphics  = &graphics;

	_visible = false;

	_x = 0;
	_y = 0;

	_animState = kAnimStateStanding;
	_direction = kDirE;

	_state = kStateIdle;

	setWalkMap();
}

Mike::~Mike() {
}

Common::Rect Mike::getArea() const {
	return _animations[_animState][_direction]->getArea();
}

bool Mike::init() {
	if (!loadAnimations())
		return false;

	return true;
}

bool Mike::loadAnimations() {
	if (!_animations[kAnimStateWalking][kDirN ].load(*_resources, "n" ))
		return false;
	if (!_animations[kAnimStateWalking][kDirNE].load(*_resources, "nw"))
		return false;
	if (!_animations[kAnimStateWalking][kDirE ].load(*_resources, "w" ))
		return false;
	if (!_animations[kAnimStateWalking][kDirSE].load(*_resources, "sw"))
		return false;
	if (!_animations[kAnimStateWalking][kDirS ].load(*_resources, "s" ))
		return false;
	if (!_animations[kAnimStateWalking][kDirSW].load(*_resources, "sw"))
		return false;
	if (!_animations[kAnimStateWalking][kDirW ].load(*_resources, "w" ))
		return false;
	if (!_animations[kAnimStateWalking][kDirNW].load(*_resources, "nw"))
		return false;

	if (!_animations[kAnimStateStanding][kDirN ].load(*_resources, "n00" ))
		return false;
	if (!_animations[kAnimStateStanding][kDirNE].load(*_resources, "nw00"))
		return false;
	if (!_animations[kAnimStateStanding][kDirE ].load(*_resources, "w00" ))
		return false;
	if (!_animations[kAnimStateStanding][kDirSE].load(*_resources, "sw00"))
		return false;
	if (!_animations[kAnimStateStanding][kDirS ].load(*_resources, "s00" ))
		return false;
	if (!_animations[kAnimStateStanding][kDirSW].load(*_resources, "sw00"))
		return false;
	if (!_animations[kAnimStateStanding][kDirW ].load(*_resources, "w00" ))
		return false;
	if (!_animations[kAnimStateStanding][kDirNW].load(*_resources, "nw00"))
		return false;

	_animations[kAnimStateWalking ][kDirNE].flipHorizontally();
	_animations[kAnimStateWalking ][kDirE ].flipHorizontally();
	_animations[kAnimStateWalking ][kDirSE].flipHorizontally();
	_animations[kAnimStateStanding][kDirNE].flipHorizontally();
	_animations[kAnimStateStanding][kDirE ].flipHorizontally();
	_animations[kAnimStateStanding][kDirSE].flipHorizontally();

	return true;
}

bool Mike::isBusy() const {
	return _state != kStateIdle;
}

bool Mike::isVisible() {
	updateVisible();
	return _visible;
}

void Mike::setVisible(bool visible) {
	_variables->set(kVariableVisible, visible ? 1 : 0);
	updateVisible();
}

void Mike::getPosition(uint32 &x, uint32 &y) const {
	x = _x;
	y = _y;
}

void Mike::setPosition(uint32 x, uint32 y) {
	if ((_x == x) && (_y == y))
		return;

	_x = x;
	_y = y;

	removeSprite();

	for (uint j = 0; j < kAnimStateNone; j++)
		for (uint i = 0; i < kDirNone; i++)
			_animations[j][i].moveFeetTo(x, y);

	addSprite();
}

Mike::Direction Mike::getDirection() const {
	return _direction;
}

void Mike::setDirection(Direction direction) {
	if ((direction < 0) || (direction >= kDirNone))
		direction = kDirE;

	if (_direction == direction)
		return;

	removeSprite();

	_direction = direction;

	addSprite();
}

void Mike::updateStatus() {
	updateVisible();
}

void Mike::updateVisible() {
	bool visible = _variables->get(kVariableVisible);
	if (_visible != visible) {
		if (visible)
			addSprite();
		else
			removeSprite();

		_visible = visible;
	}
}

void Mike::addSprite() {
	if ((_x == 0) || (_y == 0))
		return;

	_graphics->addAnimation(_animations[_animState][_direction], _spriteRef, -1, -1, -1, true);
}

void Mike::removeSprite() {
	_graphics->removeAnimation(_spriteRef);
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
