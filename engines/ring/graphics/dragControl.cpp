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

#include "ring/graphics/dragControl.h"

#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"

namespace Ring {

DragControl::DragControl() {
	_field_20 = 0;
	_objectId = kObjectInvalid;
	_field_25 = 0;
	_object = NULL;
	_field_2D = NULL;
	_field_31 = 0;
	_puzzleRotationId = 0;
	_field_39 = 0;
	_ticks = 0;
	_field_41 = 0;
	_field_45 = 1;
	_hotspot = new Hotspot(Common::Rect(0, 16, 640, 461), true, 0, 0, 0);
}

DragControl::~DragControl() {
	reset();

	SAFE_DELETE(_hotspot);
}

void DragControl::init() {
	error("[DragControl::init] Not implemented!");
}

void DragControl::reset() {
	error("[DragControl::reset] Not implemented!");
}

void DragControl::updateCoordinates(const Common::Point &point) {
	error("[DragControl::updateCoordinates] Not implemented!");
}

void DragControl::setPreferences(int32 volume, int32 volumeDialog, int32 reverseStereo, int32 pref4) {
	error("[DragControl::setPreferences] Not implemented!");
}

} // End of namespace Ring
