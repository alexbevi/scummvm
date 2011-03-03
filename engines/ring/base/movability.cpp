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

#include "ring/base/movability.h"

#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"

#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"

namespace Ring {

Movability::Movability(BaseId from, BaseId to, Common::String name, MovabilityType type) {
	_field_0 = 0;
	_from = from;
	_hotspot = NULL;
	_to = to;
	_type = type;
	_field_18 = 0.0f;
	_field_1C = 0.0f;
	_field_20 = 85.0f;
	_field_24 = 0.0f;
	_field_28 = 2;
	_field_29 = 0.0f;
	_field_2D = 0.0f;
	_field_31 = 85.0f;
}

Movability::~Movability() {
	SAFE_DELETE(_hotspot);
}

void Movability::update(float a1, float a2, float a3, float a4, byte a5, float a6, float a7, float a8) {
	_field_18 = a1;
	_field_1C = a2;
	_field_20 = a3;
	_field_24 = a4;
	_field_28 = a5;
	_field_29 = a6;
	_field_2D = a7;
	_field_31 = a8;
}

Hotspot *Movability::getHotspot() {
	if (!_hotspot)
		error("[Movability::getHotspot] Hotspot is not initialized properly");

	return _hotspot;
}

void Movability::setHotspot(const Common::Rect &rect, bool enabled, uint32 a3, uint32 a4) {
	_hotspot = new Hotspot(rect, enabled, 1, a3, a4);
}

void Movability::enableHotspot() {
	if (_hotspot)
		_hotspot->enable();
}

void Movability::disableHotspot() {
	if (_hotspot)
		_hotspot->disable();
}

#pragma region Serializable

void Movability::saveLoadWithSerializer(Common::Serializer &s) {
	_hotspot->saveLoadWithSerializer(s);
	s.syncString(_name);
	s.syncAsUint32LE(_to);
	s.syncAsUint32LE(_field_18);
	s.syncAsUint32LE(_field_1C);
	s.syncAsUint32LE(_field_20);
	s.syncAsUint32LE(_field_24);
	s.syncAsByte(_field_28);
	s.syncAsUint32LE(_field_29);
	s.syncAsUint32LE(_field_2D);
	s.syncAsUint32LE(_field_31);
}

#pragma endregion

} // End of namespace Ring
