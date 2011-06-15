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

#ifndef RING_EVENTHANDLER_RING_H
#define RING_EVENTHANDLER_RING_H

#include "ring/base/event.h"

namespace Ring {

class ApplicationRing;

class EventAnimationRing;
class EventBagRing;
class EventButtonRing;
class EventInitRing;
class EventInputRing;
class EventRideRing;
class EventSetupRing;
class EventSoundRing;
class EventTimerRing;
class EventZoneRing;

class EventHandlerRing : public EventHandler {
public:
	EventHandlerRing(ApplicationRing *application);
	virtual ~EventHandlerRing();

	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(const Common::Event &evt);
	virtual void onMouseRightButtonUp(const Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);
	virtual void onInitZone(ZoneId zone);

	virtual void onSound(Id id, SoundType type, uint32 a3);
	virtual void onSetup(ZoneId zone, SetupType type);
	virtual void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	virtual void onBagClickedObject(ObjectId id);
	virtual void onBagZoneSwitch();
	virtual void onUpdateBag(const Common::Point &point);
	virtual void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	virtual void onUpdateAfter(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType, const Common::Point &point) {}
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	virtual void onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5);
	virtual void onVisualList(Id id, uint32 type, const Common::Point &point);

protected:
	//////////////////////////////////////////////////////////////////////////
	// Buttons
	//////////////////////////////////////////////////////////////////////////
	void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUp2(ObjectId id, uint32 index, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Zone switching
	//////////////////////////////////////////////////////////////////////////
	void onSwitchZone(ZoneId zone, uint32 type);

	//////////////////////////////////////////////////////////////////////////
	// Helper functions
	//////////////////////////////////////////////////////////////////////////
	void sub_433EE0();
	void sub_445A10();

private:
	ApplicationRing *_app;

	// Event handlers
	EventAnimationRing *_eventAnimation;
	EventBagRing       *_eventBag;
	EventButtonRing    *_eventButton;
	EventInitRing      *_eventInit;
	EventInputRing     *_eventInput;
	EventRideRing      *_eventRide;
	EventSetupRing     *_eventSetup;
	EventSoundRing     *_eventSound;
	EventTimerRing     *_eventTimer;
	EventZoneRing      *_eventZone;

	// Shared data
	int32        _prefsVolume;
	int32        _presentationIndexRO;

	//////////////////////////////////////////////////////////////////////////
	// VisualList
	//////////////////////////////////////////////////////////////////////////
	void onVisualListZoneSY(Id id, uint32 a2, const Common::Point &point);

	friend class EventAnimationRing;
	friend class EventBagRing;
	friend class EventButtonRing;
	friend class EventInputRing;
	friend class EventRideRing;
	friend class EventSoundRing;
	friend class EventTimerRing;
};

} // End of namespace Ring

#endif // RING_EVENTHANDLER_RING_H
