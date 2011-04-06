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

#include "ring/game/event_faust.h"

#include "ring/base/application.h"
#include "ring/base/object.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"
#include "ring/shared_faust.h"

using namespace FaustGame;

namespace Ring {

EventHandlerFaust::EventHandlerFaust(Application *application) : _app(application) {
}

EventHandlerFaust::~EventHandlerFaust() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerFaust::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerFaust::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void EventHandlerFaust::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[EventHandlerFaust::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void EventHandlerFaust::onMouseRightButtonUp(const Common::Event &evt) {
	error("[EventHandlerFaust::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void EventHandlerFaust::onKeyDown(Common::Event &evt) {
	error("[EventHandlerFaust::onKeyDown] Not implemented (evt: %d)", evt.type);
}

#pragma endregion

#pragma region Left Button Up/Down

void EventHandlerFaust::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonDown (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		onButtonDownZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onButtonDownZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		onButtonDownZone5(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerFaust::onButtonDownZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	if (id == kObject22 && target == 2) {
		_app->objectPresentationShow(kObject12, 2);
		_app->varSetByte(114101, 1);
	}
}

void EventHandlerFaust::onButtonDownZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	if (id == kObject17022) {
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->objectPresentationShow(kObject17022, target);
	}
}

void EventHandlerFaust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonUp (ObjectId: %d, target: %d, coords: (%d, %d))", id.id(), target, point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		onButtonUpZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onButtonUpZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone2:
		onButtonUpZone2(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone3:
		onButtonUpZone3(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone4:
		onButtonUpZone4(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		onButtonUpZone5(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone6:
		onButtonUpZone6(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone7:
		onButtonUpZone7(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone8:
		onButtonUpZone8(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone9:
		onButtonUpZone9(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone10:
		onButtonUpZone10(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone11:
		onButtonUpZone11(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone12:
		onButtonUpZone12(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone13:
		onButtonUpZone13(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone14:
		onButtonUpZone14(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone15:
		onButtonUpZone15(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone16:
		onButtonUpZone16(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone17:
		onButtonUpZone17(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerFaust::onButtonUpZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZoneSY] Not implemented");
}

void EventHandlerFaust::onButtonUpZone2(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone2] Not implemented");
}

void EventHandlerFaust::onButtonUpZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone3] Not implemented");
}

void EventHandlerFaust::onButtonUpZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone4] Not implemented");
}

void EventHandlerFaust::onButtonUpZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone5] Not implemented");
}

void EventHandlerFaust::onButtonUpZone6(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone6] Not implemented");
}

void EventHandlerFaust::onButtonUpZone7(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone7] Not implemented");
}

void EventHandlerFaust::onButtonUpZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone8] Not implemented");
}

void EventHandlerFaust::onButtonUpZone9(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone9] Not implemented");
}

void EventHandlerFaust::onButtonUpZone10(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone10] Not implemented");
}

void EventHandlerFaust::onButtonUpZone11(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone11] Not implemented");
}

void EventHandlerFaust::onButtonUpZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone12] Not implemented");
}

void EventHandlerFaust::onButtonUpZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone13] Not implemented");
}

void EventHandlerFaust::onButtonUpZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone14] Not implemented");
}

void EventHandlerFaust::onButtonUpZone15(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone15] Not implemented");
}

void EventHandlerFaust::onButtonUpZone16(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone16] Not implemented");
}

void EventHandlerFaust::onButtonUpZone17(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onButtonUpZone17] Not implemented");
}

#pragma endregion

#pragma region Keydown

void EventHandlerFaust::onKeyDownZone(const Common::KeyState &keyState) {
	error("[EventHandlerFaust::onKeyDownZone] Not implemented");

	// Original goes into switch for all zones if state is not kStateShowMenu, but all functions are empty
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerFaust::onSetup(ZoneId zone, SetupType type) {
	switch (zone) {
	default:
		break;

	case kZone2:
		onSetupZone2(type);
		break;

	case kZone3:
		onSetupZone3(type);
		break;

	case kZone4:
		onSetupZone4(type);
		break;

	case kZone5:
		onSetupZone5(type);
		break;

	case kZone6:
		onSetupZone6(type);
		break;

	case kZone7:
		onSetupZone7(type);
		break;

	case kZone8:
		onSetupZone8(type);
		break;
	}
}

void EventHandlerFaust::onSetupZone2(SetupType type) {
	error("[EventHandlerFaust::onSetupZone2] Not implemented");
}

void EventHandlerFaust::onSetupZone3(SetupType type) {
	error("[EventHandlerFaust::onSetupZone3] Not implemented");
}

void EventHandlerFaust::onSetupZone4(SetupType type) {
	error("[EventHandlerFaust::onSetupZone4] Not implemented");
}

void EventHandlerFaust::onSetupZone5(SetupType type) {
	error("[EventHandlerFaust::onSetupZone5] Not implemented");
}

void EventHandlerFaust::onSetupZone6(SetupType type) {
	error("[EventHandlerFaust::onSetupZone6] Not implemented");
}

void EventHandlerFaust::onSetupZone7(SetupType type) {
	error("[EventHandlerFaust::onSetupZone7] Not implemented");
}

void EventHandlerFaust::onSetupZone8(SetupType type) {
	error("[EventHandlerFaust::onSetupZone8] Not implemented");
}

#pragma endregion

#pragma region Update

void EventHandlerFaust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityIndex == 1 || a4 == 1) {
		onUpdateBeforeZoneSY(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onUpdateBeforeZoneSY(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone3:
		onUpdateBeforeZone3(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone4:
		onUpdateBeforeZone4(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone6:
		onUpdateBeforeZone6(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone8:
		onUpdateBeforeZone8(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone12:
		onUpdateBeforeZone12(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone14:
		onUpdateBeforeZone14(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone15:
		onUpdateBeforeZone15(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone16:
		onUpdateBeforeZone16(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;
	}
}

void EventHandlerFaust::onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZoneSY] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone3] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone4] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone6] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone8] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone12] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone14] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone15] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone16] Not implemented");
}

void EventHandlerFaust::onUpdateBag(const Common::Point &point) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onUpdateBagZoneSY(point);
		break;

	case kZone3:
		onUpdateBagZone3(point);
		break;

	case kZone4:
		onUpdateBagZone4(point);
		break;

	case kZone6:
		onUpdateBagZone6(point);
		break;

	case kZone12:
		onUpdateBagZone12(point);
		break;

	case kZone14:
		onUpdateBagZone14(point);
		break;

	case kZone15:
		onUpdateBagZone15(point);
		break;

	case kZone16:
		onUpdateBagZone16(point);
		break;
	}
}

void EventHandlerFaust::onUpdateBagZoneSY(const Common::Point &point) {
	if (_app->hasCurrentPuzzle() && _app->getCurrentPuzzleId() == kPuzzleMenu) {
		_app->objectPresentationHide(kObject14, 2);
		_app->objectPresentationHide(kObject14, 4);
		_app->objectPresentationHide(kObject14, 6);
	}

	_app->objectPresentationHide(kObject99091, 1);
	_app->objectPresentationHide(kObject99091, 2);
	_app->objectPresentationHide(kObject99091, 3);
	_app->objectPresentationHide(kObject2, 2);
	_app->objectPresentationHide(kObject2, 1);
	_app->objectPresentationHide(kObject4, 1);
	_app->objectPresentationHide(kObject4, 2);
	_app->objectPresentationHide(kObject3, 1);
	_app->objectPresentationHide(kObject99500);
	_app->objectPresentationHide(kObject99010, 2);
	_app->objectPresentationHide(kObject99010, 4);
	_app->objectPresentationHide(kObject99000, 0);

	if (!_app->varGetByte(98003))
		_app->objectPresentationHide(kObject99001, 0);

	if (!_app->varGetByte(98004))
		_app->objectPresentationHide(kObject99002, 0);

	if (!_app->varGetByte(98005))
		_app->objectPresentationHide(kObject99003, 0);

	if (!_app->varGetByte(98006))
		_app->objectPresentationHide(kObject99004, 0);

	if (!_app->varGetByte(98007))
		_app->objectPresentationHide(kObject99005, 0);

	if (!_app->varGetByte(98008))
		_app->objectPresentationHide(kObject99006, 0);

	if (!_app->varGetByte(98009))
		_app->objectPresentationHide(kObject99007, 0);

	if (!_app->varGetByte(98010))
		_app->objectPresentationHide(kObject99008, 0);

	if (!_app->varGetByte(98011))
		_app->objectPresentationHide(kObject99009, 0);

	_app->objectPresentationHide(kObject99023, 0);
	_app->objectPresentationHide(kObject99022, 0);
	_app->objectPresentationHide(kObject99021, 0);
	_app->objectPresentationHide(kObject99025, 0);
	_app->objectPresentationHide(kObject99024, 0);
	_app->objectPresentationHide(kObject99042, 0);
	_app->objectPresentationHide(kObject99041, 0);
	_app->objectPresentationHide(kObject99097, 0);
	_app->objectPresentationHide(kObject55, 0);
	_app->objectPresentationHide(kObject55, 2);
}

void EventHandlerFaust::onUpdateBagZone3(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(12001), 1024);
	_app->varSetDword(12001, 0);
}

void EventHandlerFaust::onUpdateBagZone4(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(13001), 1024);
	_app->varSetDword(13001, 0);
}

void EventHandlerFaust::onUpdateBagZone6(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(31000), 1024);
	_app->varSetDword(31000, 0);
}

void EventHandlerFaust::onUpdateBagZone12(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(81001), 1024);
	_app->varSetDword(81001, 0);
}

void EventHandlerFaust::onUpdateBagZone14(const Common::Point &point) {
	if (_app->varGetByte(112023)) {
		_app->soundStop(111000 + _app->varGetByte(112023), 1024);
		_app->varSetByte(112023, 0);
	}
}

void EventHandlerFaust::onUpdateBagZone15(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(120006), 1024);
	_app->varSetDword(120006, 0);

	_app->objectPresentationHide(kObject120058);
}

void EventHandlerFaust::onUpdateBagZone16(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(311001), 1024);
	_app->varSetDword(311001, 0);
}

#pragma endregion

#pragma region Timer

void EventHandlerFaust::onTimer(TimerId timerId) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onTimerZoneSY(timerId);
		break;

	case kZone2:
		onTimerZone2(timerId);
		break;

	case kZone3:
		onTimerZone3(timerId);
		break;

	case kZone4:
		onTimerZone4(timerId);
		break;

	case kZone5:
		onTimerZone5(timerId);
		break;

	case kZone6:
		onTimerZone6(timerId);
		break;

	case kZone8:
		onTimerZone8(timerId);
		break;

	case kZone9:
		onTimerZone9(timerId);
		break;

	case kZone10:
		onTimerZone10(timerId);
		break;

	case kZone11:
		onTimerZone11(timerId);
		break;

	case kZone12:
		onTimerZone12(timerId);
		break;

	case kZone13:
		onTimerZone13(timerId);
		break;

	case kZone14:
		onTimerZone14(timerId);
		break;

	case kZone15:
		onTimerZone15(timerId);
		break;

	case kZone16:
		onTimerZone16(timerId);
		break;
	}

	if (_app->getState() != kStateShowMenu)
		_app->getTimerHandler()->incrementFiredCount(timerId);
}

void EventHandlerFaust::onTimerZoneSY(TimerId id) {
	if (id == kTimer12) {
		_app->timerStop(kTimer12);
		_app->varSetByte(97006, 1);
		_app->objectPresentationHide(kObject14, 7);
		_app->objectPresentationShow(kObject14, 8);
	}
}

void EventHandlerFaust::onTimerZone2(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->timerStart(kTimer0, 8000 + rnd(5000));
		_app->soundPlay(11102 + rnd(2));
		break;

	case kTimer1:
		_app->timerStop(kTimer1);
		_app->soundSetVolume(11106 + rnd(7), 80);
		_app->soundPlay(1);
		break;
	}
}

void EventHandlerFaust::onTimerZone3(TimerId id) {
	error("[EventHandlerFaust::onTimerZone3] Not implemented");
}

void EventHandlerFaust::onTimerZone4(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->objectPresentationHide(kObject13121, 4);
		_app->timerStop(kTimer0);
		break;

	case kTimer1:
		if (_app->varGetByte(13019) == 5)
			_app->timerStop(kTimer1);

		_app->soundPlay(_app->varGetByte(13019) + 13322);
		_app->varSetByte(13019, _app->varGetByte(13019) + 1);
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->soundPlay(13327);
		break;

	case kTimer3:
		_app->varSetByte(13900, _app->varGetByte(13900) + 1);
		_app->varGetByte(13900);
		if (_app->varGetByte(13900) == 5)
			_app->timerStop(kTimer3);
		break;

	case kTimer4:
		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, rnd(5000) + 10000);
		_app->objectPresentationShow(kObject13140);
		_app->timerStart(kTimer5, 100);
		_app->soundPlay(13072);
		break;

	case kTimer5:
		_app->timerStop(kTimer5);
		_app->objectPresentationHide(kObject13140);
		break;
	}
}

void EventHandlerFaust::onTimerZone5(TimerId id) {
	error("[EventHandlerFaust::onTimerZone5] Not implemented");
}

void EventHandlerFaust::onTimerZone6(TimerId id) {
	error("[EventHandlerFaust::onTimerZone6] Not implemented");
}

void EventHandlerFaust::onTimerZone8(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->timerStart(kTimer0, rnd(5000) + 2000);
		_app->soundPlay(51003 + rnd(4));
		break;

	case kTimer1:
		_app->soundPlay(52211, kSoundLoop);
		_app->objectPresentationShow(kObject52231, 0);
		_app->timerStop(kTimer1);
		break;

	case kTimer3:
		_app->timerStop(kTimer3);

		if (!_app->varGetByte(53124)) {
			_app->objectPresentationShow(kObject53121, 1);
			_app->varSetByte(53124, 1);
			_app->puzzleSetMovabilityOff(53102, 0, 0);
		}

		_app->timerStart(kTimer4, 10);

		if (_app->varGetByte(53000) == 1) {
			_app->soundPlay(53122, kSoundLoop);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer4:
		if (_app->varGetByte(53123) >= 100) {
			_app->timerStop(kTimer4);
		} else {
			_app->varSetByte(53123, _app->varGetByte(53123) + 5);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer5:
		if (_app->varGetByte(53123) <= _app->varGetByte(53126)) {
			_app->timerStop(kTimer5);
			_app->soundStop(53122, 1024);
			_app->varSetByte(53123, 50);
		} else {
			_app->varSetByte(53123, _app->varGetByte(53123) - 5);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);

		_app->timerStart(kTimer6, rnd(30000) + 30000);

		if (_app->varGetByte(53103)) {
			_app->rotationSet3DSoundOff(53011, 53102);
			_app->rotationSet3DSoundOff(53021, 53102);
			_app->soundPlay(53104);
			_app->soundSetVolume(53104, 90);
			_app->soundStop(53103, 1024);
			_app->varSetByte(53103, 0);
		} else {
			_app->rotationSet3DSoundOn(53011, 53102);
			_app->rotationSet3DSoundOn(53021, 53102);
			_app->soundPlay(53101);
			_app->soundPlay(53103, kSoundLoop);
			_app->soundSetVolume(53103, 90);
			_app->soundSetVolume(53101, 90);
			_app->varSetByte(53103, 1);
		}
		break;
	}
}

void EventHandlerFaust::onTimerZone9(TimerId id) {
	error("[EventHandlerFaust::onTimerZone9] Not implemented");
}

void EventHandlerFaust::onTimerZone10(TimerId id) {
	if (id == kTimer0) {
		if (rnd(5) == 3) {
			_app->objectPresentationHide(kObject71201, 2);
			_app->objectPresentationShow(kObject71201, 0);

			handleEvents();

			_app->objectPresentationHide(kObject71201, 0);
			_app->objectPresentationShow(kObject71201, 2);
		}
	}
}

void EventHandlerFaust::onTimerZone11(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0: {
		_app->timerStop(kTimer0);
		_app->timerStart(kTimer0, rnd(2000) + 500);

		uint32 index = rnd(6);
		_app->objectPresentationShow(kObject72000, index);
		_app->soundPlay(72006 + index);
		}
		break;

	case kTimer1:
		_app->timerStop(kTimer1);
		_app->objectPresentationHideAndRemove(kObject8);
		_app->objectPresentationHideAndRemove(kObject9);
		_app->objectPresentationHideAndRemove(kObject10);
		_app->exitToMenu(kMenuAction0);
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->objectPresentationHideAndRemove(kObject8);
		_app->objectPresentationHideAndRemove(kObject9);
		_app->objectPresentationHideAndRemove(kObject10);
		_app->exitToMenu(kMenuAction1);
		break;

	case kTimer3:
		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject10, 0, Common::Point(_app->varGetWord(72000), 18));
		_app->varSetWord(72000, _app->varGetWord(72000) + 1);

		if (_app->varGetWord(72000) == 434) {
			_app->timerStop(kTimer3);
			_app->objectPresentationHideAndRemove(kObject8);
			_app->objectPresentationHideAndRemove(kObject9);
			_app->objectPresentationHideAndRemove(kObject10);
			_app->exitToMenu(kMenuAction1);
		}
		break;

	case kTimer5:
		_app->soundPlay(72013, kSoundLoop);
		_app->rotationSet3DSoundOn(72014, 72014 + rnd(3));
		_app->timerStop(kTimer5);
		break;
	}
}

void EventHandlerFaust::onTimerZone12(TimerId id) {
	error("[EventHandlerFaust::onTimerZone12] Not implemented");
}

void EventHandlerFaust::onTimerZone13(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetFloat(91001, _app->varGetFloat(91001) - 1.0f);
		_app->objectPresentationHide(kObject91004);
		_app->objectPresentationShow(kObject91004, (uint32)_app->varGetFloat(91001));

		if (floor(_app->varGetFloat(91001)) <= 1.0f) {
			_app->timerStop(kTimer0);
			_app->soundPlay(91107);
			_app->waitForEscape(10);
			_app->soundStop(91105, 1024);
			_app->soundStop(91106, 1024);
		}
		break;

	case kTimer1:
		_app->timerStop(kTimer1);
		_app->timerStart(kTimer1, rnd(15000) + 25000);
		_app->soundPlay(91117 + rnd(3));
		break;
	}
}

void EventHandlerFaust::onTimerZone14(TimerId id) {
	error("[EventHandlerFaust::onTimerZone14] Not implemented");
}

void EventHandlerFaust::onTimerZone15(TimerId id) {
	error("[EventHandlerFaust::onTimerZone15] Not implemented");
}

void EventHandlerFaust::onTimerZone16(TimerId id) {
	error("[EventHandlerFaust::onTimerZone16] Not implemented");
}

#pragma endregion

#pragma region Bag

void EventHandlerFaust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		onBagZoneSY(id, (uint32)target, 1, 1, dragControl, type);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onBagZoneSY(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone3:
		onBagZone3(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone4:
		onBagZone4(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone5:
		onBagZone5(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone8:
		onBagZone8(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone12:
		onBagZone12(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone13:
		onBagZone13(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone14:
		onBagZone14(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void EventHandlerFaust::onBagZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZoneSY] Not implemented");
}

void EventHandlerFaust::onBagZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZone3] Not implemented");
}

void EventHandlerFaust::onBagZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZone4] Not implemented");
}

void EventHandlerFaust::onBagZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZone5] Not implemented");
}

void EventHandlerFaust::onBagZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZone8] Not implemented");
}

void EventHandlerFaust::onBagZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZone12] Not implemented");
}

void EventHandlerFaust::onBagZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZone13] Not implemented");
}

void EventHandlerFaust::onBagZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventHandlerFaust::onBagZone14] Not implemented");
}

void EventHandlerFaust::onBagClickedObject(ObjectId id) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZone2:
		onBagClickedObjectZone2(id);
		break;

	case kZone3:
		onBagClickedObjectZone3(id);
		break;

	case kZone4:
		onBagClickedObjectZone4(id);
		break;

	case kZone8:
		onBagClickedObjectZone8(id);
		break;

	case kZone14:
		onBagClickedObjectZone14(id);
		break;

	case kZone15:
		onBagClickedObjectZone15(id);
		break;
	}
}

void EventHandlerFaust::onBagClickedObjectZone2(ObjectId id) {
	if (id == kObjectHomunculus)
		if (_app->varGetByte(11002) == 1)
			_app->objectSetAccessibilityOn(kObject11004, 2, 2);
}

void EventHandlerFaust::onBagClickedObjectZone3(ObjectId id) {
	if (id == kObjectStrainer) {
		if (_app->hasCurrentPuzzle() && _app->getCurrentPuzzleId() == kPuzzle12001) {
			_app->objectPresentationShow(kObjectStrainer, 1);
			_app->objectSetAccessibilityOn(kObjectStrainer, 1, 1);
			_app->cursorDelete();
		}
	}
}

void EventHandlerFaust::onBagClickedObjectZone4(ObjectId id) {
	if (id == kObjectMatches)
		_app->soundPlay(13033);
}

void EventHandlerFaust::onBagClickedObjectZone8(ObjectId id) {
	switch (id) {
	default:
		break;

	case kObjectMatches2:
		_app->soundPlay(53125);
		break;

	case kObjectTeddyBear:
		if (_app->getCurrentPuzzleId() != kPuzzle53110) {
			_app->puzzleSetMod(kPuzzleMenu, 2, 17);
			_app->objectPresentationShow(kObject17);
			_app->objectSetAccessibilityOn(kObject17);
		}
		break;
	}
}

void EventHandlerFaust::onBagClickedObjectZone14(ObjectId id) {
	error("[EventHandlerFaust::onBagClickedObjectZone14] Not implemented");
}

void EventHandlerFaust::onBagClickedObjectZone15(ObjectId id) {
	if (id == kObjectMatches4)
		_app->soundPlay(120022);
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
		onBeforeRideZone2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone4:
		onBeforeRideZone4(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone5:
		onBeforeRideZone5(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone8:
		onBeforeRideZone8(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone14:
		onBeforeRideZone14(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone15:
		onBeforeRideZone15(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void EventHandlerFaust::onBeforeRideZone2(Id, Id, uint32, uint32, MovabilityType) {
	_app->objectPresentationHide(kObject25);
}

void EventHandlerFaust::onBeforeRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onBeforeRideZone4] Not implemented");
}

void EventHandlerFaust::onBeforeRideZone5(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onBeforeRideZone5] Not implemented");
}

void EventHandlerFaust::onBeforeRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	_app->objectPresentationHide(kObject25);

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom == 52021 && movabilityTo == 52220)
			if (_app->varGetByte(52230) == 1)
				_app->soundPlay(52212);
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityFrom == 53110 && movabilityTo == 53011)
			if (_app->varGetByte(531101) == 1)
				_app->soundPlay(53218);
		break;
	}
}

void EventHandlerFaust::onBeforeRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onBeforeRideZone14] Not implemented");
}

void EventHandlerFaust::onBeforeRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityPuzzleToRotation) {
		if (movabilityFrom == 120006 && movabilityTo == 120402) {
			if (_app->varGetByte(120013) == 1 && _app->varGetByte(120003) == 1) {
				_app->fadeOut(15, Color(0, 0, 0), 0);
				_app->displayFade("a12_black.bmp", "a12s01_e04m10_S.bmp", 12, 0, kLoadFromCd, kArchiveArt);
				_app->playMovie("2130");
				_app->displayFade("a12s01_e04m10_E.bmp", "a12_black.bmp", 12, 0, kLoadFromCd, kArchiveArt);
				_app->varSetByte(98088, 0);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);
				_app->exitToMenu(kMenuAction0);
			}
		}
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
		onAfterRideZone2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone3:
		onAfterRideZone3(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone4:
		onAfterRideZone4(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone6:
		onAfterRideZone6(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone8:
		onAfterRideZone8(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone10:
		onAfterRideZone10(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone12:
		onAfterRideZone12(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone13:
		onAfterRideZone13(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone14:
		onAfterRideZone14(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone15:
		onAfterRideZone15(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone16:
		onAfterRideZone16(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone17:
		onAfterRideZone17(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void EventHandlerFaust::onAfterRideZone2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone2] Not implemented");
}

void EventHandlerFaust::onAfterRideZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone3] Not implemented");
}

void EventHandlerFaust::onAfterRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone4] Not implemented");
}

void EventHandlerFaust::onAfterRideZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone6] Not implemented");
}

void EventHandlerFaust::onAfterRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone8] Not implemented");
}

void EventHandlerFaust::onAfterRideZone10(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	_app->soundPlay(71009);

	if (movabilityType == kMovabilityRotationToRotation) {
		switch (movabilityFrom) {
		default:
			break;

		case 71011:
			if (movabilityTo == 71008)
				_app->soundPlay(71010);
			break;

		case 71008:
			if (movabilityTo == 71011)
				_app->soundPlay(71010);
			break;
		}
	}
}

void EventHandlerFaust::onAfterRideZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone12] Not implemented");
}

void EventHandlerFaust::onAfterRideZone13(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone13] Not implemented");
}

void EventHandlerFaust::onAfterRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone14] Not implemented");
}

void EventHandlerFaust::onAfterRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventHandlerFaust::onAfterRideZone15] Not implemented");
}

void EventHandlerFaust::onAfterRideZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityRotationToPuzzle && movabilityFrom == 310001 && movabilityTo == 311251) {
		_app->soundPlay(310103, kSoundLoop);
		_app->timerStart(kTimer0, 30);
	}
}

void EventHandlerFaust::onAfterRideZone17(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityRotationToRotation) {
		switch (movabilityFrom) {
		default:
			break;

		case 141102:
			if (movabilityTo == 141201)
				_app->soundPlay(142004);
			break;

		case 141201:
		case 141202:
			_app->soundPlay(142004);
			break;
		}
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
		onSoundZoneSY(id, type, a3, process);
		break;

	case kZone3:
		onSoundZone3(id, type, a3, process);
		break;

	case kZone4:
		onSoundZone4(id, type, a3, process);
		break;

	case kZone5:
		onSoundZone5(id, type, a3, process);
		break;

	case kZone8:
		onSoundZone8(id, type, a3, process);
		break;

	case kZone12:
		onSoundZone12(id, type, a3, process);
		break;

	case kZone14:
		onSoundZone14(id, type, a3, process);
		break;

	case kZone15:
		onSoundZone15(id, type, a3, process);
		break;

	case kZone17:
		onSoundZone17(id, type, a3, process);
		break;
	}
}

void EventHandlerFaust::onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 90806:
		_app->exitToMenu(kMenuAction1);
		break;

	case 111042:
		_app->objectPresentationHide(kObject15);
		_app->objectSetAccessibilityOff(kObject1);
		sub_468290(3);
		break;
	}
}

void EventHandlerFaust::onSoundZone3(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 12001:
		_app->soundPlay(12002);
		break;

	case 12002:
		_app->soundPlay(12003);
		break;

	case 12004:
		_app->soundPlay(12005);
		break;

	case 12005:
		_app->soundPlay(12006);
		break;

	case 12008:
		_app->soundPlay(12009);
		break;

	case 12009:
		_app->soundPlay(12010);
		break;

	case 12010:
		_app->soundPlay(12011);
		break;

	case 12014:
		_app->soundPlay(12015);
		break;

	case 12015:
		_app->soundPlay(12016);
		break;

	case 12017:
		_app->soundPlay(12018);
		break;

	case 12018:
		_app->soundPlay(12019);
		break;

	case 12024:
	case 12025:
		_app->objectPresentationHide(kObject16);
		break;
	}
}

void EventHandlerFaust::onSoundZone4(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerFaust::onSoundZone4] Not implemented");
}

void EventHandlerFaust::onSoundZone5(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	if (id == 17002) {
		_app->playMovie("1357");
		_app->objectPresentationHide(kObject17204, 0);
		_app->rotationSetMovabilityOn(17002);
		_app->objectSetAccessibilityOff(kObject18, 1, 1);
	}
}

void EventHandlerFaust::onSoundZone8(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerFaust::onSoundZone8] Not implemented");
}

void EventHandlerFaust::onSoundZone12(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 81030:
		_app->soundPlay(81031);
		break;

	case 81040:
		if (_app->varGetByte(81012) == 1
		 && _app->varGetByte(81022) == 1
		 && _app->bagHas(kObjectDiaryOfASeducer)) {
			_app->soundStopAll(1024);
			_app->exitToMenu(kMenuAction0);
		}
		break;

	case 81041:
		_app->playMovie("1717");
		break;
	}
}

void EventHandlerFaust::onSoundZone14(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerFaust::onSoundZone14] Not implemented");
}

void EventHandlerFaust::onSoundZone15(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerFaust::onSoundZone15] Not implemented");
}

void EventHandlerFaust::onSoundZone17(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 143001:
		_app->soundPlay(143002);
		break;

	case 143002:
		_app->playMovie("2183");
		_app->soundPlay(143003);
		break;

	case 143003:
		_app->playMovie("2184");
		_app->fadeOut(15, Color(0, 0, 0), 0);
		_app->exitToMenu(kMenuAction0);
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
		onAnimationNextFrameZoneSY(animationId, name, frame, frameCount);
		break;

	case kZone3:
		onAnimationNextFrameZone3(animationId, name, frame, frameCount);
		break;

	case kZone4:
		onAnimationNextFrameZone4(animationId, name, frame, frameCount);
		break;

	case kZone5:
		onAnimationNextFrameZone5(animationId, name, frame, frameCount);
		break;

	case kZone6:
		onAnimationNextFrameZone6(animationId, name, frame, frameCount);
		break;

	case kZone7:
		onAnimationNextFrameZone7(animationId, name, frame, frameCount);
		break;

	case kZone8:
		onAnimationNextFrameZone8(animationId, name, frame, frameCount);
		break;

	case kZone9:
		onAnimationNextFrameZone9(animationId, name, frame, frameCount);
		break;

	case kZone10:
		onAnimationNextFrameZone10(animationId, name, frame, frameCount);
		break;

	case kZone11:
		onAnimationNextFrameZone11(animationId, name, frame, frameCount);
		break;

	case kZone12:
		onAnimationNextFrameZone12(animationId, name, frame, frameCount);
		break;

	case kZone13:
		onAnimationNextFrameZone13(animationId, name, frame, frameCount);
		break;

	case kZone14:
		onAnimationNextFrameZone14(animationId, name, frame, frameCount);
		break;

	case kZone15:
		onAnimationNextFrameZone15(animationId, name, frame, frameCount);
		break;
	}
}

void EventHandlerFaust::onAnimationNextFrameZoneSY(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	if (animationId == 2 && frame == _app->getSaveManager()->getProgress())
		_app->objectPresentationPauseAnimation(kObject99090, 0);
}

void EventHandlerFaust::onAnimationNextFrameZone3(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone3] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone4(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone4] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone5(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone5] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone6(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	if (animationId == 31000 && frame == 10) {
		_app->objectPresentationShow(kObject31002, 0);
		_app->varSetByte(31001, 1);
	}
}

void EventHandlerFaust::onAnimationNextFrameZone7(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone7] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone8(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone8] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone9(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone9] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone10(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone10] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone11(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone11] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone12(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone12] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone13(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone13] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone14(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone14] Not implemented");
}

void EventHandlerFaust::onAnimationNextFrameZone15(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventHandlerFaust::onAnimationNextFrameZone15] Not implemented");
}

#pragma endregion

#pragma region VisualList

void EventHandlerFaust::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (_app->getCurrentZone() == kZoneSY)
		onVisualListZoneSY(id, type, point);
}

void EventHandlerFaust::onVisualListZoneSY(Id id, uint32 type, const Common::Point &) {
	switch (id) {
	default:
		break;

	case 1:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzleGeneralMenu);
			_app->objectPresentationSetTextToPuzzle(kObject99601, 1, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99601, 1, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(width + 233, 384));
		}
		break;

	case 3:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzlePreferences);
			_app->objectPresentationSetTextToPuzzle(kObject99501, 0, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99501, 0, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99501, 0, Common::Point(width + 233, 334));
		}
		break;

	case 4:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzleGeneralMenu);
			_app->objectPresentationSetTextToPuzzle(kObject99601, 1, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99601, 1, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(width + 233, 384));

			_app->objectSetAccessibilityOn(kObject99023);
			_app->objectPresentationHide(kObject99023, 1);
			_app->objectPresentationShow(kObject99023, 2);
		}
		break;
	}
}

#pragma endregion

#pragma region Helpers

void EventHandlerFaust::sub_468290(uint32 a1) {
	error("[EventHandlerFaust::sub_468290] Not implemented");
}

#pragma endregion

} // End of namespace Ring
