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
 */

#include "ring/graphics/dragControl.h"

#include "ring/base/application.h"

#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

DragControl::DragControl() {
	_field_20 = false;
	_objectId = kObjectInvalid;
	_accessibilityIndex = 0;
	_object = NULL;
	_hotspot2 = NULL;
	_target = 0;
	_puzzleRotationId = 0;
	_field_39 = 0;
	_ticks = 0;
	_field_41 = 0;
	_field_45 = 1;
	_hotspot = new Hotspot(Common::Rect(0, 16, 640, 461), true, 0, kCursorInvalid, 0);
}

DragControl::~DragControl() {
	_object = NULL;
	_hotspot2 = NULL;

	SAFE_DELETE(_hotspot);
}

void DragControl::init(const Common::Point &coords, const ObjectId &objectId, uint32 accessibilityIndex, Hotspot *hostpot, Id target, Id puzzleRotationId, uint32 a7) {
	_coords0 = _coords1 = _oldCoords = _currentCoords = coords;
	_field_20 = true;
	_objectId = objectId;
	_accessibilityIndex = accessibilityIndex;
	_object = getApp()->getObject(objectId);
	_hotspot2 = hostpot;
	_target = target;
	_puzzleRotationId = puzzleRotationId;
	_field_39 = a7;
	_ticks = g_system->getMillis();
	_field_41 = 0;
	_field_45 = 1;

	getApp()->initObjectCursors(objectId);
}

void DragControl::reset() {
	_coords0 = _coords1 = _oldCoords = _currentCoords = Common::Point(0, 0);
	_field_20 = false;
	_objectId = kObjectInvalid;
	_accessibilityIndex = 0;
	_object = NULL;
	_hotspot2 = NULL;
	_target = 0;
	_puzzleRotationId = 0;
	_field_39 = 0;
	_ticks = 0;
	_field_41 = 0;
	_field_45 = 1;

	setHotspot(Common::Rect(0, 16, 640, 464));

	getApp()->cursorRemoveByType(kCursorTypeImage);
}

void DragControl::updateCoordinates(const Common::Point &point) {
	_oldCoords = _currentCoords;
	_currentCoords = point;

	++_field_41;
}

uint32 DragControl::getOffsetX() const {
	return (uint32)abs(_currentCoords.x - _oldCoords.x);
}

uint32 DragControl::getOffsetY() const {
	return (uint32)abs(_currentCoords.y - _oldCoords.y);
}

uint32 DragControl::getDistance() const {
	return (uint32)sqrt(pow((double)getOffsetX(), 2) + pow((double)getOffsetY(), 2));
}

void DragControl::setHotspot(const Common::Rect &rect) {
	if (_hotspot)
		_hotspot->update(rect);
}

} // End of namespace Ring
