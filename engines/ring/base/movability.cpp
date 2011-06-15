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

#include "ring/base/movability.h"

#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"

namespace Ring {

Movability::Movability(const BaseId &from, const BaseId &to, Common::String name, MovabilityType type) {
	_field_0 = 0;
	_from = from;
	_hotspot = NULL;
	_to = to;
	_name = name;
	_type = type;
	_beforeRide.alp = 0.0f;
	_beforeRide.bet = 0.0f;
	_beforeRide.ran = 85.0f;
	_field_24 = 0.0f;
	_updateType = 2;
	_afterRide.alp = 0.0f;
	_afterRide.bet = 0.0f;
	_afterRide.ran = 85.0f;
}

Movability::~Movability() {
	SAFE_DELETE(_hotspot);
}

void Movability::update(float alpBefore, float betBefore, float ranBefore, float a4, byte updateType, float alpAfter, float betAfter, float ranAfter) {
	_beforeRide.alp = alpBefore;
	_beforeRide.bet = betBefore;
	_beforeRide.ran = ranBefore;
	_field_24 = a4;
	_updateType = updateType;
	_afterRide.alp = alpAfter;
	_afterRide.bet = betAfter;
	_afterRide.ran = ranAfter;
}

Hotspot *Movability::getHotspot() {
	if (!_hotspot)
		error("[Movability::getHotspot] Hotspot is not initialized properly");

	return _hotspot;
}

void Movability::setHotspot(const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a4) {
	_hotspot = new Hotspot(rect, enabled, 1, cursorId, a4);
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
	SaveManager::syncWithFlag(s, _hotspot);

	s.syncString(_name);
	s.syncAsUint32LE(_to);
	s.syncAsUint32LE(_beforeRide.alp);
	s.syncAsUint32LE(_beforeRide.bet);
	s.syncAsUint32LE(_beforeRide.ran);
	s.syncAsUint32LE(_field_24);
	s.syncAsByte(_updateType);
	s.syncAsUint32LE(_afterRide.alp);
	s.syncAsUint32LE(_afterRide.bet);
	s.syncAsUint32LE(_afterRide.ran);
}

#pragma endregion

} // End of namespace Ring
