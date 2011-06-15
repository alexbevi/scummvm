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

#ifndef RING_FAUST_EVENT_H
#define RING_FAUST_EVENT_H

#include "ring/base/event.h"

namespace Ring {

class ApplicationFaust;

class EventAnimationFaust;
class EventBagFaust;
class EventButtonFaust;
class EventInitFaust;
class EventInputFaust;
class EventRideFaust;
class EventSetupFaust;
class EventSoundFaust;
class EventTimerFaust;
class EventVisualFaust;
class EventZoneFaust;

class EventHandlerFaust : public EventHandler {
public:
	EventHandlerFaust(ApplicationFaust *application);
	virtual ~EventHandlerFaust();

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
	virtual void onUpdateBag(const Common::Point &point);
	virtual void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);\
	virtual void onVisualList(Id id, uint32 type, const Common::Point &point);

protected:
	//////////////////////////////////////////////////////////////////////////
	// Buttons
	//////////////////////////////////////////////////////////////////////////
	void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Zone switching
	//////////////////////////////////////////////////////////////////////////
	void onSwitchZone(ZoneId zone, uint32 type);

	//////////////////////////////////////////////////////////////////////////
	// Helper functions
	//////////////////////////////////////////////////////////////////////////
	void setupRotation(uint32 type);
	void sub_45FF30();
	void sub_468290(uint32 a1);
	void sub_4A2CF0();

private:
	ApplicationFaust *_app;

	// Event handlers
	EventAnimationFaust *_eventAnimation;
	EventBagFaust       *_eventBag;
	EventButtonFaust    *_eventButton;
	EventInitFaust      *_eventInit;
	EventInputFaust     *_eventInput;
	EventRideFaust      *_eventRide;
	EventSetupFaust     *_eventSetup;
	EventSoundFaust     *_eventSound;
	EventTimerFaust     *_eventTimer;
	EventVisualFaust    *_eventVisual;
	EventZoneFaust      *_eventZone;

	friend class EventAnimationFaust;
	friend class EventBagFaust;
	friend class EventButtonFaust;
	friend class EventInputFaust;
	friend class EventRideFaust;
	friend class EventSoundFaust;
	friend class EventTimerFaust;
	friend class EventZoneFaust;
};

} // End of namespace Ring

#endif // RING_FAUST_EVENT_H
