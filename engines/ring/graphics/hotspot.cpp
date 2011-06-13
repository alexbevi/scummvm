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

#include "ring/graphics/hotspot.h"

namespace Ring {

Hotspot::Hotspot() {
	_enabled = false;
	_field_11 = 0;
	_cursorId = kCursorInvalid;
	_target = 0;
	_key = Common::KEYCODE_INVALID;
}

Hotspot::Hotspot(const Common::Rect &rect, bool enabled, uint32 a2, CursorId cursorId, Id target) {
	_rect = rect;
	_enabled = enabled;
	_field_11 = a2;
	_cursorId = cursorId;
	_target = target;
	_key = Common::KEYCODE_INVALID;
}

Hotspot::~Hotspot() {
}

bool Hotspot::contains(const Common::Point &point) const {
	return _enabled && _rect.contains(point);
}

Common::Point Hotspot::getCenter() const {
	return Common::Point(_rect.left + _rect.width() / 2, _rect.top + _rect.height() / 2);
}

#pragma region Serializable

void Hotspot::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsSint16LE(_rect.left);
	s.syncAsSint16LE(_rect.top);
	s.syncAsSint16LE(_rect.right);
	s.syncAsSint16LE(_rect.bottom);
	s.syncAsByte(_enabled);
	s.syncAsUint32LE(_field_11);
	s.syncAsUint32LE(_cursorId);
	s.syncAsUint32LE(_target);
	s.syncAsUint32LE(_key);
}

#pragma endregion

} // End of namespace Ring
