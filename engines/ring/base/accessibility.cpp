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

#include "ring/base/accessibility.h"

#include "ring/base/object.h"

#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"

namespace Ring {

Accessibility::Accessibility(Object *object) : _object(object), _hotspot(NULL) {
}

Accessibility::~Accessibility() {
	SAFE_DELETE(_hotspot);

	// Zero-out passed pointers
	_object = NULL;
}

Hotspot *Accessibility::getHotspot() {
	if (!_hotspot)
		error("[Accessibility::getHotspot] Hotspot is not initialized properly");

	return _hotspot;
}

void Accessibility::setHotspot(const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a4) {
	_hotspot = new Hotspot(rect, enabled, 1, cursorId, a4);
}

void Accessibility::enableHotspot() {
	if (_hotspot)
		_hotspot->enable();
}

void Accessibility::disableHotspot() {
	if (_hotspot)
		_hotspot->disable();
}

#pragma region Serializable

void Accessibility::saveLoadWithSerializer(Common::Serializer &s) {
	if (!_hotspot)
		error("[Accessibility::saveLoadWithSerializer] Hotspot is not initialized properly");

	_hotspot->saveLoadWithSerializer(s);
}

#pragma endregion

} // End of namespace Ring
