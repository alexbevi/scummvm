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

#ifndef RING_EVENTHANDLER_H
#define RING_EVENTHANDLER_H

#include "ring/shared.h"

#include "common/events.h"

namespace Ring {

class DragControl;

class EventHandler {
public:
	virtual ~EventHandler() {}

	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) = 0;
	virtual void onMouseLeftButtonDown(const Common::Event &evt) = 0;
	virtual void onMouseRightButtonUp(const Common::Event &evt) = 0;
	virtual void onKeyDown(Common::Event &evt) = 0;
	virtual void onTimer(TimerId id) = 0;

	virtual void onSound(Id id, SoundType type, uint32 a3) {}
	virtual void onSetup(ZoneId zone, SetupType type) {}
	virtual void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {}
	virtual void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {}
	virtual void onUpdateAfter(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType, const Common::Point &point) {}
	virtual void onUpdateBag(const Common::Point &point) {}
	virtual void onBagClickedObject(ObjectId id) {}
	virtual void onBagZoneSwitch() {}
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {}
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {}
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {}
	virtual void onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {}
	virtual void onAnimation2(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {}
	virtual void onAnimation3(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {}
	virtual void onVisualList(Id id, uint32 type, const Common::Point &point) {}
};

class EventHandlerZone {
public:
	virtual ~EventHandlerZone() {}

	virtual void onInit() = 0;
	virtual void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) = 0;

	virtual void onSetup(SetupType type) {}
	virtual void onSwitch(uint32 type) {}
	virtual void onTimer(TimerId id) {}
	virtual void onKeyDown(const Common::KeyState &keyState) {}
	virtual void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {}
	virtual void onButtonUp2(ObjectId objectId, uint32 index, Id, uint32, const Common::Point &) {}
	virtual void onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {}
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {}
	virtual void onSound(Id id, SoundType type, uint32 a3, bool process) {}
	virtual void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {}
	virtual void onUpdateBag(const Common::Point &point) {}
	virtual void onBagClickedObject(ObjectId objectId) {}
	virtual void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {}
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {}
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {}
	virtual void onVisualList(Id id, uint32 type, const Common::Point &point) {}
};

} // End of namespace LastExpress

#endif // LASTEXPRESS_EVENTHANDLER_H
