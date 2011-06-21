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

#ifndef RING_RING_ZONE_RO_H
#define RING_RING_ZONE_RO_H

#include "ring/base/event.h"

#include "ring/shared.h"

namespace Ring {

class ApplicationRing;

class ZoneRORing : public EventHandlerZone {
public:
	ZoneRORing(ApplicationRing *application);
	~ZoneRORing();

	virtual void onInit();
	virtual void onSetup(SetupType type);
	virtual void onSwitch(uint32 type);
	virtual void onTimer(TimerId id);
	virtual void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	virtual void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	virtual void onSound(Id id, SoundType type, uint32 a3, bool process);
	virtual void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);

private:
	ApplicationRing *_app;

	// Init
	int32 _presentationIndexRO;

	// Bag
	int32        _presentationIndexBagRO;
	int32        _presentationIndexBagRO2;

	uint32            _dword_4A1C00;
};

} // End of namespace Ring

#endif // RING_RING_ZONE_RO_H
