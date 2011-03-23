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

#ifndef RING_MOVABILITY_H
#define RING_MOVABILITY_H

#include "ring/shared.h"

namespace Ring {

class Hotspot;
class Puzzle;
class Rotation;

class Movability : public Common::Serializable {
public:
	Movability(const BaseId &from, const BaseId &to, Common::String name, MovabilityType type);
	~Movability();

	void update(float a1, float a2, float a3, float a4, byte a5, float alpAfter, float betAfter, float ranAfter);

	void setHotspot(const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a4);
	Hotspot *getHotspot();
	void enableHotspot();
	void disableHotspot();

	// Accessors
	BaseId getTo() { return _to; }
	MovabilityType getType() { return _type; }
	Common::String getRideName() { return _name; }
	float getAlpBefore() { return _beforeRide.alp; }
	float getBetBefore() { return _beforeRide.bet; }
	float getRanBefore() { return _beforeRide.ran; }
	float getAlpAfter() { return _afterRide.alp; }
	float getBetAfter() { return _afterRide.bet; }
	float getRanAfter() { return _afterRide.ran; }
	byte getUpdateType() { return _updateType; }

	void setRideName(Common::String name) { _name = name; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	struct Coords {
		float alp;
		float bet;
		float ran;

		Coords() {
			alp	= 0.0f;
			bet	= 0.0f;
			ran	= 0.0f;
		}
	};

	uint32            _field_0;
	BaseId            _from;
	Hotspot          *_hotspot;
	BaseId            _to;
	Common::String    _name;
	MovabilityType    _type;
	Coords            _beforeRide;
	float             _field_24;
	byte              _updateType;
	Coords            _afterRide;
};

} // End of namespace Ring

#endif // RING_MOVABILITY_H
