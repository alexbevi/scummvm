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

#include "ring/game/faust/faust_event.h"

#include "ring/base/application.h"
#include "ring/base/object.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"

#include "ring/game/faust/faust_animation.h"
#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_bag.h"
#include "ring/game/faust/faust_button.h"
#include "ring/game/faust/faust_init.h"
#include "ring/game/faust/faust_input.h"
#include "ring/game/faust/faust_ride.h"
#include "ring/game/faust/faust_setup.h"
#include "ring/game/faust/faust_shared.h"
#include "ring/game/faust/faust_sound.h"
#include "ring/game/faust/faust_timer.h"
#include "ring/game/faust/faust_visual.h"
#include "ring/game/faust/faust_zone.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

using namespace FaustGame;

namespace Ring {

EventHandlerFaust::EventHandlerFaust(ApplicationFaust *application) : _app(application) {
	// Event handlers
	_eventAnimation = new EventAnimationFaust(application);
	_eventBag       = new EventBagFaust(application, this);
	_eventButton    = new EventButtonFaust(application, this);
	_eventInit      = new EventInitFaust(application);
	_eventInput     = new EventInputFaust(application, this);
	_eventRide      = new EventRideFaust(application, this);
	_eventSetup     = new EventSetupFaust(application);
	_eventSound     = new EventSoundFaust(application, this);
	_eventTimer     = new EventTimerFaust(application, this);
	_eventVisual    = new EventVisualFaust(application);
	_eventZone      = new EventZoneFaust(application, this);
}

EventHandlerFaust::~EventHandlerFaust() {
	SAFE_DELETE(_eventAnimation);
	SAFE_DELETE(_eventBag);
	SAFE_DELETE(_eventButton);
	SAFE_DELETE(_eventInit);
	SAFE_DELETE(_eventInput);
	SAFE_DELETE(_eventRide);
	SAFE_DELETE(_eventSetup);
	SAFE_DELETE(_eventSound);
	SAFE_DELETE(_eventTimer);
	SAFE_DELETE(_eventVisual);
	SAFE_DELETE(_eventZone);

	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerFaust::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	_eventInput->onMouseLeftButtonUp(evt, isControlPressed);
}

void EventHandlerFaust::onMouseLeftButtonDown(const Common::Event &evt) {
	_eventInput->onMouseLeftButtonDown(evt);
}

void EventHandlerFaust::onMouseRightButtonUp(const Common::Event &evt) {
	_eventInput->onMouseRightButtonUp(evt);
}

void EventHandlerFaust::onKeyDown(Common::Event &evt) {
	_eventInput->onKeyDown(evt);
}

#pragma endregion

#pragma region Left Button Up/Down

void EventHandlerFaust::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonDown (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventButton->onButtonDownZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventButton->onButtonDownZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		_eventButton->onButtonDownZone5(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerFaust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
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

	case kZone2:
		_eventButton->onButtonUpZone2(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone3:
		_eventButton->onButtonUpZone3(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone4:
		_eventButton->onButtonUpZone4(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		_eventButton->onButtonUpZone5(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone6:
		_eventButton->onButtonUpZone6(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone7:
		_eventButton->onButtonUpZone7(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone8:
		_eventButton->onButtonUpZone8(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone9:
		_eventButton->onButtonUpZone9(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone10:
		_eventButton->onButtonUpZone10(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone11:
		_eventButton->onButtonUpZone11(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone12:
		_eventButton->onButtonUpZone12(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone13:
		_eventButton->onButtonUpZone13(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone14:
		_eventButton->onButtonUpZone14(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone15:
		_eventButton->onButtonUpZone15(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone16:
		_eventButton->onButtonUpZone16(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone17:
		_eventButton->onButtonUpZone17(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

#pragma endregion

#pragma region Setup

void EventHandlerFaust::onSetup(ZoneId zone, SetupType type) {
	switch (zone) {
	default:
		break;

	case kZone2:
		_eventSetup->onSetupZone2(type);
		break;

	case kZone3:
		_eventSetup->onSetupZone3(type);
		break;

	case kZone4:
		_eventSetup->onSetupZone4(type);
		break;

	case kZone5:
		_eventSetup->onSetupZone5(type);
		break;

	case kZone6:
		_eventSetup->onSetupZone6(type);
		break;

	case kZone7:
		_eventSetup->onSetupZone7(type);
		break;

	case kZone8:
		_eventSetup->onSetupZone8(type);
		break;
	}
}

#pragma endregion

#pragma region Zone

void EventHandlerFaust::onInitZone(ZoneId zone) {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		_eventInit->initZoneSY();
		break;

	case kZone2:
		_eventInit->initZone2();
		break;

	case kZone3:
		_eventInit->initZone3();
		break;

	case kZone4:
		_eventInit->initZone4();
		break;

	case kZone5:
		_eventInit->initZone5();
		break;

	case kZone6:
		_eventInit->initZone6();
		break;

	case kZone7:
		_eventInit->initZone7();
		break;

	case kZone8:
		_eventInit->initZone8();
		break;

	case kZone9:
		_eventInit->initZone9();
		break;

	case kZone10:
		_eventInit->initZone10();
		break;

	case kZone11:
		_eventInit->initZone11();
		break;

	case kZone12:
		_eventInit->initZone12();
		break;

	case kZone13:
		_eventInit->initZone13();
		break;

	case kZone14:
		_eventInit->initZone14();
		break;

	case kZone15:
		_eventInit->initZone15();
		break;

	case kZone16:
		_eventInit->initZone16();
		break;

	case kZone17:
		_eventInit->initZone17();
		break;
	}
}

void EventHandlerFaust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
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

	case kZone3:
		_eventZone->onUpdateBeforeZone3(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone4:
		_eventZone->onUpdateBeforeZone4(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone6:
		_eventZone->onUpdateBeforeZone6(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone8:
		_eventZone->onUpdateBeforeZone8(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone12:
		_eventZone->onUpdateBeforeZone12(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone14:
		_eventZone->onUpdateBeforeZone14(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone15:
		_eventZone->onUpdateBeforeZone15(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone16:
		_eventZone->onUpdateBeforeZone16(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;
	}
}

#pragma endregion

#pragma region Timer

void EventHandlerFaust::onTimer(TimerId timerId) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventTimer->onTimerZoneSY(timerId);
		break;

	case kZone2:
		_eventTimer->onTimerZone2(timerId);
		break;

	case kZone3:
		_eventTimer->onTimerZone3(timerId);
		break;

	case kZone4:
		_eventTimer->onTimerZone4(timerId);
		break;

	case kZone5:
		_eventTimer->onTimerZone5(timerId);
		break;

	case kZone6:
		_eventTimer->onTimerZone6(timerId);
		break;

	case kZone8:
		_eventTimer->onTimerZone8(timerId);
		break;

	case kZone9:
		_eventTimer->onTimerZone9(timerId);
		break;

	case kZone10:
		_eventTimer->onTimerZone10(timerId);
		break;

	case kZone11:
		_eventTimer->onTimerZone11(timerId);
		break;

	case kZone12:
		_eventTimer->onTimerZone12(timerId);
		break;

	case kZone13:
		_eventTimer->onTimerZone13(timerId);
		break;

	case kZone14:
		_eventTimer->onTimerZone14(timerId);
		break;

	case kZone15:
		_eventTimer->onTimerZone15(timerId);
		break;

	case kZone16:
		_eventTimer->onTimerZone16(timerId);
		break;
	}

	if (_app->getState() != kStateShowMenu)
		_app->getTimerHandler()->incrementFiredCount(timerId);
}

#pragma endregion

#pragma region Bag

void EventHandlerFaust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventBag->onBagZoneSY(id, (uint32)target, 1, 1, dragControl, type);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventBag->onBagZoneSY(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone3:
		_eventBag->onBagZone3(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone4:
		_eventBag->onBagZone4(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone5:
		_eventBag->onBagZone5(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone8:
		_eventBag->onBagZone8(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone12:
		_eventBag->onBagZone12(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone13:
		_eventBag->onBagZone13(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone14:
		_eventBag->onBagZone14(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void EventHandlerFaust::onUpdateBag(const Common::Point &point) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventBag->onUpdateBagZoneSY(point);
		break;

	case kZone3:
		_eventBag->onUpdateBagZone3(point);
		break;

	case kZone4:
		_eventBag->onUpdateBagZone4(point);
		break;

	case kZone6:
		_eventBag->onUpdateBagZone6(point);
		break;

	case kZone12:
		_eventBag->onUpdateBagZone12(point);
		break;

	case kZone14:
		_eventBag->onUpdateBagZone14(point);
		break;

	case kZone15:
		_eventBag->onUpdateBagZone15(point);
		break;

	case kZone16:
		_eventBag->onUpdateBagZone16(point);
		break;
	}
}

void EventHandlerFaust::onBagClickedObject(ObjectId id) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZone2:
		_eventBag->onBagClickedObjectZone2(id);
		break;

	case kZone3:
		_eventBag->onBagClickedObjectZone3(id);
		break;

	case kZone4:
		_eventBag->onBagClickedObjectZone4(id);
		break;

	case kZone8:
		_eventBag->onBagClickedObjectZone8(id);
		break;

	case kZone14:
		_eventBag->onBagClickedObjectZone14(id);
		break;

	case kZone15:
		_eventBag->onBagClickedObjectZone15(id);
		break;
	}
}

#pragma endregion

#pragma region Rides

void EventHandlerFaust::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZone2:
		_eventRide->onBeforeRideZone2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone4:
		_eventRide->onBeforeRideZone4(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone5:
		_eventRide->onBeforeRideZone5(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone8:
		_eventRide->onBeforeRideZone8(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone14:
		_eventRide->onBeforeRideZone14(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone15:
		_eventRide->onBeforeRideZone15(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void EventHandlerFaust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityTo == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZone2:
		_eventRide->onAfterRideZone2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone3:
		_eventRide->onAfterRideZone3(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone4:
		_eventRide->onAfterRideZone4(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone6:
		_eventRide->onAfterRideZone6(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone8:
		_eventRide->onAfterRideZone8(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone10:
		_eventRide->onAfterRideZone10(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone12:
		_eventRide->onAfterRideZone12(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone13:
		_eventRide->onAfterRideZone13(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone14:
		_eventRide->onAfterRideZone14(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone15:
		_eventRide->onAfterRideZone15(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone16:
		_eventRide->onAfterRideZone16(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone17:
		_eventRide->onAfterRideZone17(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

#pragma endregion

#pragma region Sound

void EventHandlerFaust::onSound(Id id, SoundType type, uint32 a3) {
	debugC(kRingDebugLogic, "onSound (id: %d, type: %d)", id, type);

	bool process = (a3 & 0x1000) != 0;
	a3 &= 239;

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventSound->onSoundZoneSY(id, type, a3, process);
		break;

	case kZone3:
		_eventSound->onSoundZone3(id, type, a3, process);
		break;

	case kZone4:
		_eventSound->onSoundZone4(id, type, a3, process);
		break;

	case kZone5:
		_eventSound->onSoundZone5(id, type, a3, process);
		break;

	case kZone8:
		_eventSound->onSoundZone8(id, type, a3, process);
		break;

	case kZone12:
		_eventSound->onSoundZone12(id, type, a3, process);
		break;

	case kZone14:
		_eventSound->onSoundZone14(id, type, a3, process);
		break;

	case kZone15:
		_eventSound->onSoundZone15(id, type, a3, process);
		break;

	case kZone17:
		_eventSound->onSoundZone17(id, type, a3, process);
		break;
	}
}

#pragma endregion

#pragma region Animation

void EventHandlerFaust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventAnimation->onAnimationNextFrameZoneSY(animationId, name, frame, frameCount);
		break;

	case kZone3:
		_eventAnimation->onAnimationNextFrameZone3(animationId, name, frame, frameCount);
		break;

	case kZone4:
		_eventAnimation->onAnimationNextFrameZone4(animationId, name, frame, frameCount);
		break;

	case kZone5:
		_eventAnimation->onAnimationNextFrameZone5(animationId, name, frame, frameCount);
		break;

	case kZone6:
		_eventAnimation->onAnimationNextFrameZone6(animationId, name, frame, frameCount);
		break;

	case kZone7:
		_eventAnimation->onAnimationNextFrameZone7(animationId, name, frame, frameCount);
		break;

	case kZone8:
		_eventAnimation->onAnimationNextFrameZone8(animationId, name, frame, frameCount);
		break;

	case kZone9:
		_eventAnimation->onAnimationNextFrameZone9(animationId, name, frame, frameCount);
		break;

	case kZone10:
		_eventAnimation->onAnimationNextFrameZone10(animationId, name, frame, frameCount);
		break;

	case kZone11:
		_eventAnimation->onAnimationNextFrameZone11(animationId, name, frame, frameCount);
		break;

	case kZone12:
		_eventAnimation->onAnimationNextFrameZone12(animationId, name, frame, frameCount);
		break;

	case kZone13:
		_eventAnimation->onAnimationNextFrameZone13(animationId, name, frame, frameCount);
		break;

	case kZone14:
		_eventAnimation->onAnimationNextFrameZone14(animationId, name, frame, frameCount);
		break;

	case kZone15:
		_eventAnimation->onAnimationNextFrameZone15(animationId, name, frame, frameCount);
		break;
	}
}

#pragma endregion

#pragma region VisualList

void EventHandlerFaust::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (_app->getCurrentZone() == kZoneSY)
		_eventVisual->onVisualListZoneSY(id, type, point);
}

#pragma endregion

#pragma region Helpers

void EventHandlerFaust::setupRotation(uint32 type) {
	switch (type) {
	default:
		break;

	case 2:
		_app->soundStop(17019, 1024);
		_app->setSpace(kZone10);
		_app->rotationSetAlp(71009, 64.0f);
		_app->rotationSetBet(71009, 0);
		_app->rotationSetRan(71009, 87.0f);
		_app->rotationSetActive(71009);
		break;

	case 3:
		_app->setSpace(kZone9);
		_app->soundPlay(61306, kSoundLoop);
		_app->rotationSetAlp(61200, 0.0f);
		_app->rotationSetBet(61200, 0);
		_app->rotationSetRan(61200, 87.0f);
		_app->rotationSetActive(61200);
		break;

	case 4:
		_app->setSpace(kZone10);
		_app->soundStop(61306, 1024);
		_app->rotationSetAlp(71010, 270.0f);
		_app->rotationSetBet(71010, 0);
		_app->rotationSetRan(71010, 87.0f);
		_app->rotationSetActive(71010);
		_app->objectPresentationHide(kObject5);
		break;
	}
}

void EventHandlerFaust::sub_45FF30() {
	if (_app->soundIsPlaying(112016)) {
		_app->objectPresentationHide(kObject12, 1);
		_app->objectSetAccessibilityOff(kObject22);
		_app->soundStop(112016, 1024);
		_app->varSetByte(114100, 0);
	}
}

void EventHandlerFaust::sub_468290(uint32 a1) {
	error("[EventHandlerFaust::sub_468290] Not implemented");
}

void EventHandlerFaust::sub_4A2CF0() {
	error("[EventHandlerFaust::sub_4A2CF0] Not implemented");
}

#pragma endregion

} // End of namespace Ring
