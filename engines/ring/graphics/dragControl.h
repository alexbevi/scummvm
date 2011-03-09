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

#ifndef RING_DRAGCONTROL_H
#define RING_DRAGCONTROL_H

#include "ring/shared.h"

namespace Ring {

class Object;
class Hotspot;

class DragControl {
public:
	DragControl();
	~DragControl();

	void init(Common::Point coords, ObjectId objectId, uint32 accessibilityIndex, Hotspot *hostpot, Id target, Id puzzleRotationId, uint32 a7);
	void reset();

	void updateCoordinates(const Common::Point &point);
	void setHotspot(const Common::Rect &rect);

	uint32 getOffsetX();
	uint32 getOffsetY();
	uint32 getDistance();

	// Accessors
	Common::Point getCoords0() { return _coords0; }
	Common::Point getCoords1() { return _coords1; }
	Common::Point getOldCoords() { return _oldCoords; }
	Common::Point getCurrentCoords() { return _currentCoords; }

	bool getField20() { return _field_20; }
	ObjectId getObjectId() { return _objectId; }
	uint32 getAccessibilityIndex() { return _accessibilityIndex; }
	Hotspot *getHotspot2() { return _hotspot2; }
	Id getTarget() { return _target; }
	uint32 getPuzzleRotationId() { return _puzzleRotationId; }
	uint32 getField39() { return _field_39; }
	uint32 getField45() { return _field_45; }
	Hotspot *getHotspot() { return _hotspot; }

	void SetCoords1(const Common::Point &coords) { _coords1 = coords; }
	void setField45(uint32 val) { _field_45 = val; }

private:
	Common::Point _coords0;
	Common::Point _coords1;
	Common::Point _oldCoords;
	Common::Point _currentCoords;
	bool          _field_20;
	ObjectId      _objectId;
	uint32        _accessibilityIndex;
	Object       *_object;
	Hotspot      *_hotspot2;
	Id            _target;
	Id            _puzzleRotationId;
	uint32        _field_39;
	uint32        _ticks;
	uint32        _field_41;
	uint32        _field_45;
	Hotspot      *_hotspot;
};

} // End of namespace Ring

#endif // RING_DRAGCONTROL_H
