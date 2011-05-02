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
#include "engines/darkseed2/imageconverter.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

static const char *kVariableVisible = "ShowMike";

template<>
void SaveLoad::sync<Position>(Common::Serializer &serializer, Position &position) {
	SaveLoad::sync(serializer, position.x);
	SaveLoad::sync(serializer, position.y);
}

Mike::Mike(Resources &resources, Variables &variables, Graphics &graphics) {
	_resources = &resources;
	_variables = &variables;
	_graphics  = &graphics;

	_pathfinder = new Pathfinder(graphics.getScreenWidth(), graphics.getScreenHeight());

	_visible = false;

	_x         = 0;
	_y         = 0;
	_direction = kDirE;

	_targetX         = 0;
	_targetY         = 0;
	_targetDirection = _direction;

	_currentWayPoint       = _wayPoints.end();
	_currentWayPointNumber = 0xFFFFFFFF;

	_turnTo = kDirNone;

	_state     = kStateIdle;
	_animState = kAnimStateStanding;

	_waitUntil = 0;

	setWalkMap();

	_scaleFactors[0] = 0;
	_scaleFactors[1] = 0;
	_scaleFactors[2] = 0;
	_scale           = FRAC_ONE;
	_scaleMin        = doubleToFrac(0.05);
	_scaleMax        = doubleToFrac(1.05);
}

Mike::~Mike() {
	delete _pathfinder;
}

bool Mike::init(bool needPalette) {
	if (!loadAnimations(needPalette))
		return false;

	return true;
}

bool Mike::loadAnimations(bool needPalette) {
	Palette palette;

	if (needPalette) {
		// Loading the fixed palette for Mike
		bool foundPal = palette.loadFromPALRGBA(*_resources, "fix");

		if (!foundPal)
			foundPal = palette.loadFromPALRGBA(*_resources, "walkcyc");

		if (!foundPal)
			return false;

		ImgConv.registerStandardPalette(palette);
	}

	// Walking animations
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

	// Standing animations
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

	// Flip where necessary
	_animations[kAnimStateWalking ][kDirNE].flipHorizontally();
	_animations[kAnimStateWalking ][kDirE ].flipHorizontally();
	_animations[kAnimStateWalking ][kDirSE].flipHorizontally();
	_animations[kAnimStateStanding][kDirNE].flipHorizontally();
	_animations[kAnimStateStanding][kDirE ].flipHorizontally();
	_animations[kAnimStateStanding][kDirSE].flipHorizontally();

	if (needPalette)
		ImgConv.unregisterStandardPalette();

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

void Mike::getPosition(int32 &x, int32 &y) const {
	x = _x;
	y = _y;
}

void Mike::setPosition(int32 x, int32 y) {
	x /= _resources->getVersionFormats().getHotspotScale();
	y /= _resources->getVersionFormats().getHotspotScale();

	// Sanity checks
	assert((ABS(x) <= 0x7FFF) && (ABS(y) <= 0x7FFF));

	if ((_x == x) && (_y == y))
		return;

	_x = x;
	_y = y;

	removeSprite();

	updateAnimPositions();

	addSprite();
}

frac_t Mike::getScale() const {
	return _scale;
}

frac_t Mike::calculateScale(int32 y) const {
	y *= _resources->getVersionFormats().getHotspotScale();

	// Sanity checks
	assert(ABS(y) <= 0x7FFF);

	frac_t scale = intToFrac(y - _scaleFactors[0]) / _scaleFactors[1];

	if (scale < 0)
		return FRAC_ONE;

	return CLIP(scale, _scaleMin, _scaleMax);
}

void Mike::updateScale() {
	_scale = calculateScale(_y);
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
			// Time for a new frame

			if (_state == kStateWalking) {
				// Walk
				advanceWalk();
				if (_animState == kAnimStateStanding)
					// Done walking
					_state = kStateIdle;

			} else if (_state == kStateTurning)
				// Turn
				advanceTurn();

			if (_state != kStateIdle)
				// New next frame time
				_waitUntil = g_system->getMillis() + 100;

		}

		if (_state == kStateIdle) {
			// Done walking

			if (_direction != _targetDirection) {
				// We still need to turn

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
		// Visibility changed

		if (visible)
			// Sprite needs to be drawn
			addSprite();
		else
			// Sprite needs not to be drawn
			removeSprite();

		_visible = visible;
	}
}

void Mike::addSprite() {
	if ((_x == 0) || (_y == 0))
		return;

	if (!_visible)
		return;

	_graphics->addAnimation(_animations[_animState][_direction], _spriteRef, true);
}

void Mike::removeSprite() {
	_graphics->removeAnimation(_spriteRef);
}

void Mike::setWalkMap() {
	_pathfinder->clear();
}

void Mike::setWalkMap(const Sprite &walkMap, int32 arg1, int32 arg2) {
	_pathfinder->setWalkMap(walkMap, arg1, arg2);
}

void Mike::setScaleFactors(const int32 *scaleFactors) {
	for (int i = 0; i < 3; i++)
		_scaleFactors[i] = scaleFactors[i];
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

	_animations[_animState][_direction].setFrame(0);
	addSprite();
}

void Mike::advanceWalk() {
	int32 targetX = (_currentWayPoint != _wayPoints.end()) ? _currentWayPoint->x : _x;
	int32 targetY = (_currentWayPoint != _wayPoints.end()) ? _currentWayPoint->y : _y;

	if ((_x != targetX) || (_y != targetY)) {
		// Direction we're walking
		bool east  = _x > targetX;
		bool south = _y > targetY;

		// Advance position
		_x += getStepOffsetX();
		_y += getStepOffsetY();

		// Overshooting?
		if (east) {
			if (_x <= targetX)
				_x = targetX;
		} else {
			if (_x >= targetX)
				_x = targetX;
		}
		if (south) {
			if (_y <= targetY)
				_y = targetY;
		} else {
			if (_y >= targetY)
				_y = targetY;
		}
	}

	if ((_x == targetX) && (_y == targetY)) {
		if (_currentWayPoint != _wayPoints.end()) {
			++_currentWayPoint;
			++_currentWayPointNumber;
		} else
			// Reached our target
			_animState = kAnimStateStanding;
	}

	Direction direction = getDirection(_x, _y, targetX, targetY);
	if ((_direction != direction) && (direction != kDirNone)) {
		// We need to turn to a new direction
		_state = kStateTurning;
		_turnTo = direction;
	}

	if ((_x != targetX) || (_y != targetY)) {
		removeSprite();
		_animations[_animState][_direction]++;
		updateAnimPositions();
		addSprite();
	}
}

void Mike::go(int32 x, int32 y, Direction direction) {
	x /= _resources->getVersionFormats().getHotspotScale();
	y /= _resources->getVersionFormats().getHotspotScale();

	// Sanity checks
	assert((ABS(x) <= 0x7FFF) && (ABS(y) <= 0x7FFF));

	if ((x == 0) || (y == 0)) {
		x = _x;
		y = _y;
	}
	if ((direction < 0) || (direction >= kDirNone))
		direction = _direction;

	// Set target
	_targetX         = x;
	_targetY         = y;
	_targetDirection = direction;

	_wayPoints = _pathfinder->findPath(_x, _y, _targetX, _targetY);
	_currentWayPoint = _wayPoints.begin();
	_currentWayPointNumber = 0;

	// Set states to walking
	_state     = kStateWalking;
	_animState = kAnimStateWalking;

	_animations[_animState][_direction].setFrame(0);

	// Update at once
	_waitUntil = g_system->getMillis();
}

int32 Mike::getStepOffsetX() const {
	int32 offset = 0;

	// Offset
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

	// Scale offset
	int32 scaledOffset = fracToInt(offset * _scale);

	if (scaledOffset == 0) {
		// If we scaled it down to 0, return the minimum, 1/-1

		if      (offset > 0)
			return  1;
		else if (offset < 0)
			return -1;
	}

	if (scaledOffset == 1)
		return 1;

	return scaledOffset / _resources->getVersionFormats().getHotspotScale();
}

int32 Mike::getStepOffsetY() const {
	int32 offset = 0;

	// Offset
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

	// Scale offset
	int32 scaledOffset = fracToInt(offset * _scale);

	if (scaledOffset == 0) {
		// If we scaled it down to 0, return the minimum, 1/-1

		if      (offset > 0)
			return  1;
		else if (offset < 0)
			return -1;
	}

	if (scaledOffset == 1)
		return 1;

	return scaledOffset / _resources->getVersionFormats().getHotspotScale();
}

Mike::Direction Mike::getDirection(int32 x1, int32 y1, int32 x2, int32 y2) {
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

bool Mike::saveLoad(Common::Serializer &serializer, Resources &resources) {
	byte direction       = _direction;
	byte targetDirection = _targetDirection;
	byte turnTo          = _turnTo;

	uint32 scale = _scale;

	byte state     = _state;
	byte animState = _animState;

	SaveLoad::sync(serializer, _visible);
	SaveLoad::sync(serializer, _x);
	SaveLoad::sync(serializer, _y);
	SaveLoad::sync(serializer, direction);

	SaveLoad::sync(serializer, _targetX);
	SaveLoad::sync(serializer, _targetY);
	SaveLoad::sync(serializer, targetDirection);

	SaveLoad::sync(serializer, _wayPoints);
	SaveLoad::sync(serializer, _currentWayPointNumber);

	SaveLoad::sync(serializer, turnTo);

	SaveLoad::sync(serializer, _scaleFactors[0]);
	SaveLoad::sync(serializer, _scaleFactors[1]);
	SaveLoad::sync(serializer, _scaleFactors[2]);

	SaveLoad::sync(serializer, scale);

	SaveLoad::sync(serializer, state);
	SaveLoad::sync(serializer, animState);

	SaveLoad::syncTimestamp(serializer, _waitUntil);

	_direction       = (Direction) direction;
	_targetDirection = (Direction) targetDirection;
	_turnTo          = (Direction) turnTo;

	_scale = (frac_t) scale;

	_state     = (State) state;
	_animState = (AnimState) animState;

	return true;
}

bool Mike::loading(Resources &resources) {
	uint32 currentWayPointNumber = _currentWayPointNumber;
	_currentWayPointNumber = 0;

	_currentWayPoint = _wayPoints.begin();
	while ((_currentWayPoint != _wayPoints.end()) && (currentWayPointNumber != _currentWayPointNumber)) {
		++_currentWayPoint;
		++_currentWayPointNumber;
	}

	_spriteRef.clear();

	updateAnimPositions();
	addSprite();

	return true;
}

} // End of namespace DarkSeed2
