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

	_targetX = 0;
	_targetY = 0;
	_turnTo = kDirNone;

	_animState = kAnimStateStanding;
	_direction = kDirE;

	_state = kStateIdle;

	_waitUntil = 0;

	setWalkMap();

	for (int i = 0; i < 3; i++)
		_scaleFactors[i] = 0;
	_scale = FRAC_ONE;

	_scaleMin = doubleToFrac(0.05);
	_scaleMax = doubleToFrac(1.05);
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

	updateAnimPositions();

	addSprite();
}

void Mike::updateScale() {
	_scale = CLIP(intToFrac(_y - _scaleFactors[0]) / _scaleFactors[1], _scaleMin, _scaleMax);
}

void Mike::updateAnimPositions() {
	updateScale();

	for (uint j = 0; j < kAnimStateNone; j++)
		for (uint i = 0; i < kDirNone; i++) {
			_animations[j][i].moveFeetTo(_x, _y);
			_animations[j][i].setScale(_scale);
		}
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

	if (_state != kStateIdle) {
		if (g_system->getMillis() >= _waitUntil) {
			if (_state == kStateWalking) {
				advanceWalk();
				if (_animState == kAnimStateStanding)
					_state = kStateIdle;
			} else if (_state == kStateTurning)
				advanceTurn();

			if (_state != kStateIdle)
				_waitUntil = g_system->getMillis() + 100;
		}

		if (_state == kStateIdle) {
			if (_direction != _targetDirection) {
				_targetX = _x;
				_targetY = _y;
				_turnTo = _targetDirection;
				_state = kStateTurning;
				_waitUntil = g_system->getMillis();
			}
		}
	}
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

	if (!_visible)
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

void Mike::setScaleFactors(const int32 *scaleFactors) {
	for (int i = 0; i < 3; i++)
		_scaleFactors[i] = scaleFactors[i];
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

void Mike::advanceTurn() {
	if (_direction == _turnTo) {
		// Reached the target direction, continue walking
		_state     = kStateWalking;
		_animState = kAnimStateWalking;
		_animations[_animState][_direction].setFrame(0);
		return;
	}

	removeSprite();

	_animState = kAnimStateStanding;

	// Always turn the shortest way round
	if (_turnTo > _direction) {
		if ((_turnTo - _direction) < (_direction + (kDirNone - 1 - _turnTo))) {
			_direction = (Direction) ((_direction + 1) % kDirNone);
		} else {
			_direction = (Direction) (_direction - 1);
			if ((_direction < 0) || (_direction >= kDirNone))
				_direction = (Direction) (kDirNone - 1);
		}
	} else {
		if ((_direction - _turnTo) < (_turnTo + (kDirNone - 1 - _direction))) {
			_direction = (Direction) (_direction - 1);
			if ((_direction < 0) || (_direction >= kDirNone))
				_direction = (Direction) (kDirNone - 1);
		} else {
			_direction = (Direction) ((_direction + 1) % kDirNone);
		}
	}

	addSprite();
}

void Mike::advanceWalk() {
	removeSprite();

	if ((_x != _targetX) || (_y != _targetY)) {
		bool east  = _x > _targetX;
		bool south = _y > _targetY;

		_x += getStepOffsetX();
		_y += getStepOffsetY();

		// Overshooting?
		if (east) {
			if (_x <= _targetX)
				_x = _targetX;
		} else {
			if (_x >= _targetX)
				_x = _targetX;
		}
		if (south) {
			if (_y <= _targetY)
				_y = _targetY;
		} else {
			if (_y >= _targetY)
				_y = _targetY;
		}
	}

	if ((_x == _targetX) && (_y == _targetY)) {
		// Reached our target
		_animState = kAnimStateStanding;
	}

	Direction direction = getDirection(_x, _y, _targetX, _targetY);
	if ((_direction != direction) && (direction != kDirNone)) {
		// We need to turn to a new direction
		_state = kStateTurning;
		_turnTo = direction;
	}

	_animations[_animState][_direction]++;

	updateAnimPositions();

	addSprite();
}

void Mike::go(uint32 x, uint32 y, Direction direction) {
	if ((x == 0) || (y == 0)) {
		x = _x;
		y = _y;
	}
	if ((direction < 0) || (direction >= kDirNone))
		direction = _direction;

	_targetX = x;
	_targetY = y;
	_targetDirection = direction;

	_state = kStateWalking;
	_animState = kAnimStateWalking;
	_waitUntil = g_system->getMillis();
}

int32 Mike::getStepOffsetX() const {
	int32 offset = 0;

	switch (_direction) {
	case kDirNE:
		offset = 7;
		break;

	case kDirE:
		offset = 12;
		break;

	case kDirSE:
		offset = 7;
		break;

	case kDirSW:
		offset = -7;
		break;

	case kDirW:
		offset = -12;
		break;

	case kDirNW:
		offset = -7;
		break;

	default:
		return 0;
		break;
	}

	offset = fracToInt(offset * _scale);

	return offset;
}

int32 Mike::getStepOffsetY() const {
	int32 offset = 0;

	switch (_direction) {
	case kDirN:
		offset = -4;
		break;

	case kDirNE:
		offset = -2;
		break;

	case kDirSE:
		offset = 2;
		break;

	case kDirS:
		offset = 4;
		break;

	case kDirSW:
		offset = 2;
		break;

	case kDirNW:
		offset = -2;
		break;

	default:
		return 0;
		break;
	}

	offset = fracToInt(offset * _scale);

	return offset;
}

Mike::Direction Mike::getDirection(uint32 x1, uint32 y1, uint32 x2, uint32 y2) {
	if ((x1 == x2) && (y1 > y2))
		return kDirN;

	if ((x1 == x2) && (y1 < y2))
		return kDirS;

	if ((y1 == y2) && (x1 > x2))
		return kDirW;

	if ((y1 == y2) && (x1 < x2))
		return kDirE;

	if ((x1 > x2) && (y1 > y2))
		return kDirNW;

	if ((x1 > x2) && (y1 < y2))
		return kDirSW;

	if ((x1 < x2) && (y1 > y2))
		return kDirNE;

	if ((x1 < x2) && (y1 < y2))
		return kDirSE;

	return kDirNone;
}

} // End of namespace DarkSeed2
