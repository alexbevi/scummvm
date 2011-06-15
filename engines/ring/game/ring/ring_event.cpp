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

#include "ring/game/ring/ring_event.h"

#include "ring/base/accessibility.h"
#include "ring/base/bag.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"

#include "ring/game/ring/ring_animation.h"
#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_bag.h"
#include "ring/game/ring/ring_button.h"
#include "ring/game/ring/ring_init.h"
#include "ring/game/ring/ring_input.h"
#include "ring/game/ring/ring_ride.h"
#include "ring/game/ring/ring_setup.h"
#include "ring/game/ring/ring_shared.h"
#include "ring/game/ring/ring_sound.h"
#include "ring/game/ring/ring_timer.h"
#include "ring/game/ring/ring_zone.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"

#include "ring/sound/soundhandler.h"
#include "ring/sound/soundmanager.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/textconsole.h"

using namespace RingGame;

namespace Ring {

EventHandlerRing::EventHandlerRing(ApplicationRing *application) : _app(application) {
	// Event handlers
	_eventAnimation = new EventAnimationRing(application, this);
	_eventBag       = new EventBagRing(application, this);
	_eventButton    = new EventButtonRing(application, this);
	_eventInit      = new EventInitRing(application);
	_eventInput     = new EventInputRing(application, this);
	_eventRide      = new EventRideRing(application, this);
	_eventSetup     = new EventSetupRing(application);
	_eventSound     = new EventSoundRing(application, this);
	_eventTimer     = new EventTimerRing(application, this);
	_eventZone      = new EventZoneRing(application);

	// Shared data
	_prefsVolume = 0;
	_presentationIndexRO = 0;
}


EventHandlerRing::~EventHandlerRing() {
	SAFE_DELETE(_eventAnimation);
	SAFE_DELETE(_eventBag);
	SAFE_DELETE(_eventButton);
	SAFE_DELETE(_eventInit);
	SAFE_DELETE(_eventInput);
	SAFE_DELETE(_eventRide);
	SAFE_DELETE(_eventSetup);
	SAFE_DELETE(_eventSound);
	SAFE_DELETE(_eventTimer);
	SAFE_DELETE(_eventZone);

	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerRing::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	_eventInput->onMouseLeftButtonUp(evt, isControlPressed);
}

void EventHandlerRing::onMouseLeftButtonDown(const Common::Event &evt) {
	_eventInput->onMouseLeftButtonDown(evt);
}

void EventHandlerRing::onMouseRightButtonUp(const Common::Event &evt) {
	_eventInput->onMouseRightButtonUp(evt);
}

void EventHandlerRing::onKeyDown(Common::Event &evt) {
	_eventInput->onKeyDown(evt);
}

void EventHandlerRing::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonDown (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneNI:
		_eventButton->onButtonDownZoneNI(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		// Function does not seem to change anything (gets the current bag object)
		//onButtonDownZoneFO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		_eventButton->onButtonDownZoneRO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		_eventButton->onButtonDownZoneN2(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerRing::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonUp (ObjectId: %d, target: %d, coords: (%d, %d))", id.id(), target, point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventButton->onButtonUpZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventButton->onButtonUpZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneNI:
		_eventButton->onButtonUpZoneNI(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneRH:
		_eventButton->onButtonUpZoneRH(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		_eventButton->onButtonUpZoneFO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		_eventButton->onButtonUpZoneRO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneWA:
		_eventButton->onButtonUpZoneWA(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneAS:
		_eventButton->onButtonUpZoneAS(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		_eventButton->onButtonUpZoneN2(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerRing::onButtonUp2(ObjectId id, uint32 index, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonUp2 (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneNI:
	case kZoneRH:
	case kZoneRO:
	case kZoneAS:
	case kZoneN2:
		break;

	case kZoneFO:
		// Function does not seem to change anything (gets the current bag object)
		// onButtonDownZoneFO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneWA:
		_eventButton->onButtonUp2ZoneWA(id, index, puzzleRotationId, a4, point);
		break;
	}
}

#pragma endregion

#pragma region Setup

void EventHandlerRing::onSetup(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "onSetup (zone: %s, type: %d)", _app->getZoneFolder(zone).c_str(), type);

	switch (zone) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventSetup->onSetupZoneNI(type);
		break;

	case kZoneRH:
		_eventSetup->onSetupZoneRH(type);
		break;

	case kZoneFO:
		_eventSetup->onSetupZoneFO(type);
		break;

	case kZoneRO:
		_eventSetup->onSetupZoneRO(type);
		break;

	case kZoneWA:
		_eventSetup->onSetupZoneWA(type);
		break;

	case kZoneAS:
		_eventSetup->onSetupZoneAS(type);
		break;

	case kZoneN2:
		_eventSetup->onSetupZoneN2(type);
		break;
	}
}

#pragma endregion

#pragma region Zone

void EventHandlerRing::onInitZone(ZoneId zone) {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		_eventInit->initZoneSY();
		break;

	case kZoneNI:
		_eventInit->initZoneNI();
		break;

	case kZoneRH:
		_eventInit->initZoneRH();
		break;

	case kZoneFO:
		_eventInit->initZoneFO();
		break;

	case kZoneRO:
		_eventInit->initZoneRO();
		break;

	case kZoneWA:
		_eventInit->initZoneWA();
		break;

	case kZoneAS:
		_eventInit->initZoneAS();
		break;

	case kZoneN2:
		_eventInit->initZoneN2();
		break;
	}
}

void EventHandlerRing::onSwitchZone(ZoneId zone, uint32 type) {
	switch (zone) {
	default:
		break;

	case kZoneNI:
		_eventZone->onSwitchZoneNI(type);
		break;

	case kZoneRH:
		_eventZone->onSwitchZoneRH(type);
		break;

	case kZoneFO:
		_eventZone->onSwitchZoneFO(type);
		break;

	case kZoneRO:
		_eventZone->onSwitchZoneRO(type);
		break;

	case kZoneWA:
		_eventZone->onSwitchZoneWA(type);
		break;

	case kZoneAS:
		_eventZone->onSwitchZoneAS(type);
		break;

	case kZoneN2:
		_eventZone->onSwitchZoneN2(type);
		break;
	}
}

void EventHandlerRing::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityIndex == 1 || a4 == 1) {
		_eventZone->onUpdateBeforeZoneSY(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventZone->onUpdateBeforeZoneSY(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZoneNI:
		_eventZone->onUpdateBeforeZoneNI(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZoneN2:
		_eventZone->onUpdateBeforeZoneN2(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;
	}
}

#pragma endregion

#pragma region Timer

void EventHandlerRing::onTimer(TimerId timerId) {
	debugC(kRingDebugLogic, "onTimer (id: %d)", timerId);

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneWA:
		break;

	case kZoneNI:
		_eventTimer->onTimerZoneNI(timerId);
		break;

	case kZoneRH:
		_eventTimer->onTimerZoneRH(timerId);
		break;

	case kZoneFO:
		_eventTimer->onTimerZoneFO(timerId);
		break;

	case kZoneRO:
		_eventTimer->onTimerZoneRO(timerId);
		break;

	case kZoneAS:
		_eventTimer->onTimerZoneAS(timerId);
		break;

	case kZoneN2:
		_eventTimer->onTimerZoneN2(timerId);
		break;
	}

	_app->getTimerHandler()->incrementFiredCount(timerId);
}

#pragma endregion

#pragma region Bag

void EventHandlerRing::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventBag->onBagZoneSY(id, (uint32)target, 1, 1, dragControl, type);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneSY:
		_eventBag->onBagZoneSY(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneNI:
		_eventBag->onBagZoneNI(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneFO:
		_eventBag->onBagZoneFO(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneRO:
		_eventBag->onBagZoneRO(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneN2:
		_eventBag->onBagZoneN2(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void EventHandlerRing::onUpdateBag(const Common::Point &point) {
	if (_app->getCurrentZone() == kZoneSY)
		_eventBag->onUpdateBagZoneSY(point);
}

void EventHandlerRing::onBagClickedObject(ObjectId id) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneFO:
		_eventBag->onBagClickedObjectZoneFO(id);
		break;
	}
}

void EventHandlerRing::onBagZoneSwitch() {
	_eventBag->onBagZoneSwitch();
}

#pragma endregion

#pragma region Rides

void EventHandlerRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventRide->onBeforeRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRH:
		_eventRide->onBeforeRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneFO:
		_eventRide->onBeforeRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRO:
		_eventRide->onBeforeRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneWA:
		_eventRide->onBeforeRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneAS:
		_eventRide->onBeforeRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneN2:
		_eventRide->onBeforeRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void EventHandlerRing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityTo == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventRide->onAfterRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRH:
		_eventRide->onAfterRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneFO:
		_eventRide->onAfterRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRO:
		_eventRide->onAfterRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneWA:
		_eventRide->onAfterRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneAS:
		_eventRide->onAfterRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneN2:
		_eventRide->onAfterRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

#pragma endregion

#pragma region Sound

void EventHandlerRing::onSound(Id id, SoundType type, uint32 a3) {
	debugC(kRingDebugLogic, "onSound (id: %d, type: %d)", id, type);

	bool process = (a3 & 0x1000) != 0;
	a3 &= 239;

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventSound->onSoundZoneSY(id, type, a3, process);
		break;

	case kZoneNI:
		_eventSound->onSoundZoneNI(id, type, a3, process);
		break;

	case kZoneRH:
		_eventSound->onSoundZoneRH(id, type, a3, process);
		break;

	case kZoneFO:
		_eventSound->onSoundZoneFO(id, type, a3, process);
		break;

	case kZoneRO:
		_eventSound->onSoundZoneRO(id, type, a3, process);
		break;

	case kZoneAS:
		_eventSound->onSoundZoneAS(id, type, a3, process);
		break;

	case kZoneWA:
		_eventSound->onSoundZoneWA(id, type, a3, process);
		break;

	case kZoneN2:
		_eventSound->onSoundZoneN2(id, type, a3, process);
		break;
	}
}

#pragma endregion

#pragma region Animation

void EventHandlerRing::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventAnimation->onAnimationNextFrameZoneNI(animationId, name, frame, frameCount);
		break;

	case kZoneRH:
		_eventAnimation->onAnimationNextFrameZoneRH(animationId, name, frame, frameCount);
		break;

	case kZoneFO:
		_eventAnimation->onAnimationNextFrameZoneFO(animationId, name, frame, frameCount);
		break;

	case kZoneRO:
		_eventAnimation->onAnimationNextFrameZoneRO(animationId, name, frame, frameCount);
		break;

	case kZoneAS:
		_eventAnimation->onAnimationNextFrameZoneAS(animationId, name, frame, frameCount);
		break;

	case kZoneWA:
		_eventAnimation->onAnimationNextFrameZoneWA(animationId, name, frame, frameCount);
		break;

	case kZoneN2:
		_eventAnimation->onAnimationNextFrameZoneN2(animationId, name, frame, frameCount);
		break;
	}
}

void EventHandlerRing::onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {
	if (_app->getCurrentZone() == kZoneWA)
		_eventAnimation->onAnimationZoneWA(type, animationId, name, frame, a5);
}

#pragma endregion

#pragma region VisualList

void EventHandlerRing::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (_app->getCurrentZone() == kZoneSY)
		onVisualListZoneSY(id, type, point);
}

void EventHandlerRing::onVisualListZoneSY(Id id, uint32 type, const Common::Point &) {
	if (id == 1) {
		if (type > 0 && type <= 3) {
			_app->objectPresentationHide(kObjectLoadOk, 0);
			_app->objectPresentationHide(kObjectLoadCancel, 0);
		}
	}
}

#pragma endregion

#pragma region Helper functions

void EventHandlerRing::sub_433EE0() {
	if (_app->varGetByte(70012) == 1
		&& _app->varGetByte(70001) == 0
		&& _app->bagHas(kObjectCage)
		&& _app->bagHas(kObjectCentaur)
		&& _app->bagHas(kObjectDragon)
		&& _app->bagHas(kObjectPhoenix1)) {
			_app->varSetByte(70013, 11);
			_app->puzzleSetActive(kPuzzle70305);
			_app->soundPlay(70017);
	}
}

void EventHandlerRing::sub_445A10() {
	int32 val1 = _app->varGetByte(10106) + _app->varGetByte(10431);
	int32 val2 = _app->varGetByte(10420) + _app->varGetByte(10421);

	if (!_app->varGetByte(10107)) {

		if (val1 != 2)
			return;

		_app->soundPlay(10412, kSoundLoop);
		_app->objectPresentationShow(kObject10432, 0);
		_app->soundSetVolume(10412, 80);

		if (val2 == 2) {
			_app->timerStop(kTimer0);
			_app->varSetByte(10107, 0);
			_app->varSetByte(10432, 0);

			_app->timerStart(kTimer1, 1000);
			_app->objectPresentationHide(kObject10431);
			_app->objectPresentationShow(kObject10431, 6);
		} else {
			_app->varSetByte(10432, 0);
			_app->timerStop(kTimer1);
			_app->timerStart(kTimer0, 1000);
			_app->varSetByte(10107, 1);
		}

		return;
	}

	if (val1 == 2) {
		if (val2 == 2) {
			_app->soundPlay(10412, kSoundLoop);
			_app->objectPresentationShow(kObject10432, 0);
			_app->soundSetVolume(10412, 80);
			_app->timerStop(kTimer0);
			_app->varSetByte(10107, 0);
			_app->varSetByte(10432, 0);
			_app->timerStop(kTimer1);
			_app->timerStart(kTimer1, 1000);
			_app->objectPresentationHide(kObject10431);
			_app->objectPresentationShow(kObject10431, 6);
		}
	} else {
		_app->soundStop(10412, 1024);
		_app->objectPresentationHide(kObject10432);
		_app->varSetByte(10107, 0);
		_app->timerStop(kTimer0);
		_app->timerStop(kTimer1);
		_app->objectPresentationHide(kObject10431);
	}
}

#pragma endregion

} // End of namespace Ring
