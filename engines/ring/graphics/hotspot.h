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

#ifndef RING_HOTSPOT_H
#define RING_HOTSPOT_H

#include "ring/shared.h"

#include "common/events.h"
#include "common/serializer.h"

namespace Ring {

class Hotspot : public Common::Serializable {
public:
	Hotspot();
	Hotspot(const Common::Rect &rect, bool enabled, uint32 a2, CursorId cursorId, Id target);
	~Hotspot();

	void update(const Common::Rect &rect) { _rect = rect;}
	void enable() { _enabled = true; }
	void disable() { _enabled = false; }
	bool isEnabled() { return _enabled; }
	bool contains(const Common::Point &point) const;

	Common::Point getCenter() const;

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

	// Accessors
	void setKey(Common::KeyCode key) { _key = key; }

	CursorId getCursorId() { return _cursorId; }
	Id getTarget() { return _target; }
	Common::KeyCode getKey() { return _key; }

private:
	Common::Rect    _rect;
	bool            _enabled;
	uint32          _field_11;
	CursorId        _cursorId;
	Id              _target;
	Common::KeyCode _key;
};

} // End of namespace Ring

#endif // RING_HOTSPOT_H
