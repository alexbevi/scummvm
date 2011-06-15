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

#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/textconsole.h"

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

#pragma region Zone

void EventHandlerFaust::onInitZone(ZoneId zone) {
	error("[EventHandlerFaust::onInitZone] Not implemented (zone: %d)", zone);
}

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
	if (movabilityFrom == 12036 && !_app->soundIsPlayingType(kSoundTypeDialog)) {
		switch (movabilityTo) {
		default:
			break;

		case 0:
			_app->soundPlay(12022);
			_app->varSetDword(12001, 12022);
			break;

		case 1:
			_app->soundPlay(12023);
			_app->varSetDword(12001, 12023);
			break;
		}
	}
}

void EventHandlerFaust::onUpdateBeforeZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone4] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityFrom == 31003 && !_app->soundIsPlayingType(kSoundTypeDialog) && movabilityTo == 0) {
		_app->soundPlay(31101);
		_app->varSetDword(31000, 31101);
	}
}

void EventHandlerFaust::onUpdateBeforeZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityFrom == 53123 && movabilityTo == 1) {
		if (_app->varGetByte(53123) == 1 && _app->bagGetClickedObject() == kObjectMatches2) {
			_app->objectPresentationShow(kObject53123, 2);
			_app->varSetByte(53121, 2);
			_app->soundPlay(53126);
			_app->soundPlay(53123, kSoundLoop);
			_app->puzzleSetMovabilityOff(kPuzzle53102, 1, 1);
			_app->soundStop(53124, 1024);
			_app->varSetByte(53123, 50);
			_app->timerStop(kTimer3);
			_app->timerStop(kTimer4);
			_app->timerStop(kTimer5);

			if (_app->varGetByte(53120) == 1) {
				_app->soundPlay(53121, kSoundLoop);
				_app->timerStart(kTimer3, 300);
			}
		}
	}
}

void EventHandlerFaust::onUpdateBeforeZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityFrom == 81900 && !_app->soundIsPlayingType(kSoundTypeDialog)) {
		switch (movabilityTo) {
		default:
			return;

		case 0:
			_app->varSetDword(81001, 81015);
			break;

		case 1:
			_app->varSetDword(81001, 81017);
			break;

		case 2:
			case 3:
			_app->varSetDword(81001, 81042);
			break;

		case 4:
			_app->varSetDword(81001, 81001);
		}

		_app->soundPlay(_app->varGetDword(81001));
	}
}

void EventHandlerFaust::onUpdateBeforeZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone14] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventHandlerFaust::onUpdateBeforeZone15] Not implemented");
}

void EventHandlerFaust::onUpdateBeforeZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (_app->soundIsPlayingType(kSoundTypeDialog))
		return;

	switch (movabilityFrom) {
	default:
		break;

	case 310001: {
		Id soundId = 310001 + _app->varGetWord(311012 + movabilityTo);
		_app->soundPlay(soundId);
		_app->varSetDword(311011, soundId);
		}
		break;

	case 310016:
		_app->soundPlay(310013);
		_app->varSetDword(311011, 310013);
		break;
	}
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
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->soundStop(17047, 1024);
		_app->soundPlay(17044);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->puzzleSetActive(kPuzzle17022);
		break;

	case kTimer3:
		if (_app->varGetByte(17030) == 5) {
			_app->varSetByte(17030, 0);
			_app->timerStop(kTimer3);

			if (_app->varGetByte(17041) == 4) {
				_app->varSetByte(17102, 1);

				if ((_app->varGetByte(17101) + _app->varGetByte(17102)) >= 2) {
					_app->playMovie("1356");
					_app->fadeOut(15, Color(0, 0, 0), 0);

					if (_app->getSaveManager()->getData()->progressState == 5 )
						_app->getSaveManager()->getData()->progress++;

					setupRotation(2);
				}
			}

			_app->objectSetAccessibilityOn(kObjectHandle, 2, 2);

		} else {
			_app->varSetByte(17030, _app->varGetByte(17030) + 1);

			uint32 index = _app->varGetByte(_app->varGetByte(17030) + 17030) + 1;
			_app->objectPresentationSetAnimationOnPuzzle(kObjectHandle, index, 0, 17006);
			_app->objectPresentationShow(kObjectHandle, index);
			_app->soundPlay(17056);
		}
		break;

	case kTimer4: {
		_app->timerStop(kTimer4);
		_app->objectSetAccessibilityOff(17301, 1, 5);

		uint32 index = _app->varGetByte(_app->varGetByte(17030) + 17030);
		_app->objectPresentationUnpauseAnimation(17303, index + 1);
		_app->objectPresentationShow(kObjectHandle, index + 6);
		_app->objectPresentationHide(kObjectHandle, index + 1);
		_app->soundPlay(17055);
		}
		break;

	case kTimer5:
		if (rnd(2) == 0)
			_app->soundPlay(17041 + rnd(3));
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->soundStop(17047, 1024);
		_app->soundPlay(17044);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}

		_app->varSetByte(17042, 1);
		break;

	case kTimer7:
		_app->timerStop(kTimer7);
		_app->timerStart(kTimer7, rnd(5000) + 8000);
		_app->soundPlay(17020);
		break;

	case kTimer8:
		_app->timerStop(kTimer8);
		_app->soundStop(17047, 1024);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}
		break;
	}
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
	switch (id) {
	default:
		break;

	case kTimer0:
		if (_app->varGetFloat(81001) == 0.0f) {
			_app->timerStop(kTimer0);

			if (!_app->hasTimer(kTimer1)) {
				_app->objectSetAccessibilityOn(kObject81201);
				_app->objectSetAccessibilityOn(kObject81202);
				_app->rotationSetAlp(82021, 152.0f);
				_app->rotationSetBet(82021, 0);
				_app->rotationSetRan(82021, 87.0f);
				_app->rotationSetActive(82021);
				_app->soundStop(81204, 1024);
				_app->objectPresentationShow(kObject81221, 1);
			}
		} else {
			_app->varSetFloat(81001, _app->varGetFloat(81001) - 1.0f);
			if (_app->varGetFloat(81001) < 0.0f)
				_app->varSetFloat(81001, 0);

			_app->objectPresentationHide(kObject81203);
			_app->objectPresentationShow(kObject81203, (uint32)_app->varGetFloat(81001));

			if (_app->varGetFloat(81001) != 25.0f
				&& _app->varGetFloat(81001) != 50.0f)
				break;

			_app->soundPlay(81203);
		}
		break;

	case kTimer1:
		if (_app->varGetFloat(81002) == 10.0f) {
			_app->timerStop(kTimer1);

			if (!_app->hasTimer(kTimer0)) {
				_app->objectSetAccessibilityOn(kObject81201);
				_app->objectSetAccessibilityOn(kObject81202);
				_app->rotationSetAlp(82021, 152.0f);
				_app->rotationSetBet(82021, 0);
				_app->rotationSetRan(82021, 87.0f);
				_app->rotationSetActive(82021);
				_app->soundStop(81204, 1024);
				_app->objectPresentationShow(kObject81221, 1);
			}
		} else {
			_app->varSetFloat(81002, _app->varGetFloat(81002) + 1.0f);
			if (_app->varGetFloat(81002) > 59.0f)
				_app->varSetFloat(81002, 0);

			_app->objectPresentationHide(kObject81202);
			_app->objectPresentationShow(kObject81202, (uint32)_app->varGetFloat(81002));


			if (_app->varGetFloat(81002) != 25.0f
				&& _app->varGetFloat(81002) != 50.0f)
				break;

			_app->soundPlay(81203);
		}
		break;

	case kTimer2:
		_app->objectPresentationHide(kObject81404, 1);
		_app->timerStop(kTimer2);

		if (_app->varGetByte(81007) == 1) {
			_app->objectPresentationHide(kObject81404);
			_app->objectPresentationShow(kObject81404, 0);
			_app->timerStart(kTimer3, 100);
			_app->varSetByte(81007, 0);
		} else {
			_app->objectPresentationHide(kObject81404);
			_app->rotationSetAlp(84021, 53.0f);
			_app->rotationSetBet(84021, 0);
			_app->rotationSetRan(84021, 87.0f);
			_app->rotationSetActive(84021);

			if (_app->varGetByte(81006) == 1)
				_app->soundPlay(81409);

			_app->soundStop(81410, 1024);
			_app->varSetByte(81006, 0);
		}

		if (_app->varGetByte(81012) == 1
		 && _app->varGetByte(81022) == 1
		 && _app->bagHas(kObjectDiaryOfASeducer)) {
			_app->soundStopAll(1024);
			_app->exitToMenu(kMenuAction0);
		}
		break;

	case kTimer3:
		_app->objectPresentationHide(kObject81404, 0);
		_app->bagAdd(81207);
		_app->timerStop(kTimer3);
		_app->rotationSetAlp(84021, 53.0f);
		_app->rotationSetBet(84021, 0);
		_app->rotationSetRan(84021, 87.0f);
		_app->rotationSetActive(84021);

		if (_app->varGetByte(81006) == 1)
			_app->soundPlay(81409);
		_app->soundStop(81410, 1024);
		_app->varSetByte(81006, 0);
		break;

	case kTimer4:
		_app->objectPresentationHide(kObject81306, _app->varGetByte(81021));
		_app->timerStop(kTimer4);
		break;

	case kTimer5:
		if (floor(_app->varGetFloat(81005)) == 0.0f
		 && floor(_app->varGetFloat(81004)) == 0.0f) {
			 _app->soundStop(81301, 1024);
			 _app->timerStop(kTimer5);
			 _app->rotationSetAlp(83031, 180.0f);
			 _app->rotationSetBet(83031, -26.0f);
			 _app->rotationSetRan(83031, 87.0f);
			 _app->rotationSetActive(83031);
			 break;
		}

		if (floor(_app->varGetFloat(81005)) != 0.0f) {
			_app->varSetFloat(81005, _app->varGetFloat(81005) - 1.0f);
			_app->objectPresentationHide(kObject81308);
			_app->objectPresentationShow(kObject81308, (uint32)_app->varGetFloat(81005));
		}

		if (floor(_app->varGetFloat(81004)) != 0.0f) {
			_app->varSetFloat(81004, _app->varGetFloat(81004) - 1.0f);
			_app->objectPresentationHide(kObject81309);
			_app->objectPresentationShow(kObject81309, (uint32)_app->varGetFloat(81004));
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->objectPresentationHide(kObject81303, 6);
		_app->objectPresentationHide(kObject81303, 8);
		_app->rotationSetAlp(83031, 334.0f);
		_app->rotationSetBet(83031, 16.0f);
		_app->rotationSetRan(83031, 87.0f);
		_app->rotationSetActive(83031);
		break;

	case kTimer7: {
		_app->timerStop(kTimer7);
		_app->timerStart(kTimer7, rnd(15000) + 15000);

		Id soundId = 81102 + rnd(2);
		_app->soundSetVolume(soundId, rnd(10) + 90);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer8: {
		_app->timerStop(kTimer8);
		_app->timerStart(kTimer8, rnd(3000) + 6000);
		Id soundId = 81104 + rnd(3);
		_app->soundSetVolume(soundId, rnd(10) + 90);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer9: {
		_app->timerStop(kTimer9);
		_app->timerStart(kTimer9, rnd(5000) + 5000);

		Id soundId = 81415 + rnd(4);
		_app->soundSetVolume(soundId, rnd(10) + 90);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer10: {
		_app->timerStop(kTimer10);
		_app->timerStart(kTimer10, rnd(10000) + 5000);

		_app->objectPresentationShow(kObject81901);
		if (!_app->varGetByte(81023))
			_app->objectPresentationShow(kObject81902);

		Id soundId = 81217 + rnd(6);
		_app->soundSetVolume(soundId, 80 + rnd(10));
		_app->soundPlay(soundId);
		}
		break;

	case kTimer11:
		_app->objectPresentationHide(kObject81222);
		_app->timerStop(kTimer11);
		break;
	}
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
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->objectPresentationHide(kObject120003, 1);
		_app->timerStop(kTimer0);
		break;

	case kTimer1:
		if (_app->varGetWord(120001) != 400 || _app->varGetByte(97001) != 1) {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject6, 1, Common::Point(_app->varGetWord(120001), 16));
			_app->varSetWord(120001, _app->varGetWord(120001) + 1);
		} else {
			if (_app->varGetWord(120002) == 400) {
				_app->varSetWord(120001, 401);
				_app->objectPresentationHide(kObject6, 5);
				_app->objectPresentationShow(kObject6, 3);
				_app->objectPresentationHide(kObject6, 4);
			}
		}

		if (_app->varGetWord(120001) == 400
		 && _app->varGetByte(97001) == 0) {
			_app->objectPresentationShow(kObject6, 3);
			_app->objectPresentationHide(kObject6, 4);
		}

		if (_app->varGetWord(120001) == 440) {
			if (_app->varGetByte(120003)) {
				_app->objectSetAccessibilityOn(kObject120001, 1, 1);
				_app->rotationSetMovabilityOn(120104, 3, 3);
				_app->rotationSetMovabilityOn(120106, 1, 1);
				_app->rotationSetMovabilityOn(120108, 3, 3);
				_app->varSetByte(98088, 0);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer2);
				_app->rotationSetActive(120401);
			} else {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer2);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);

				Id rotationId = _app->getCurrentRotationId();

				_app->varSetDword(120007, rotationId);
				_app->varSetWord(120006, (int16)_app->rotationGetAlp(rotationId));
				_app->varSetWord(120007, (int16)_app->rotationGetBet(rotationId));
				_app->varSetWord(120008, (int16)_app->rotationGetRan(rotationId));

				_app->objectSetAccessibilityOn(kObject1);
				_app->fadeOut(15, Color(0, 0, 0), 0);
				_app->objectPresentationShow(kObject15);
				_app->soundPlay(120150);
			}
		}
		break;

	case kTimer2:
		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject6, 2, Common::Point(_app->varGetWord(120002), 46));
		_app->varSetWord(120002, _app->varGetWord(120002) + 1);
		break;

	case kTimer3:
		_app->timerStop(kTimer3);
		_app->objectPresentationShow(kObject120054, 0);
		_app->soundPlay(120072);
		break;

	case kTimer4: {
		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, rnd(3000) + 2000);

		Id soundId = 120003 + rnd(11);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer5: {
		_app->timerStop(kTimer5);
		_app->timerStart(kTimer5, rnd(3000) + 3000);

		Id soundId = 120013 + rnd(4);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->timerStart(kTimer6, rnd(3000) + 3000);
		_app->objectPresentationShow(kObject120061);
		_app->soundPlay(120067);
		break;

	case kTimer7:
		_app->timerStop(kTimer7);
		_app->varSetByte(120035, 0);
		_app->objectPresentationHide(kObject120054, 1);
		_app->objectSetAccessibilityOn(kObject120054, 2, 2);
		_app->soundStop(120071, 1024);
		break;
	}
}

void EventHandlerFaust::onTimerZone16(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetWord(311024, _app->varGetWord(311024) + 5);

		if (_app->varGetWord(311024) < 155) {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject310015, 0, Common::Point(120, 174 - _app->varGetWord(311024)));
		} else {
			_app->objectSetAccessibilityOn(kObject310015, 0, 0);
			_app->objectSetAccessibilityOn(kObject310001);
			_app->soundStop(310103, 1024);
			_app->soundPlay(310104);
			_app->timerStop(kTimer0);
		}
		break;

	case kTimer1:
		_app->varSetWord(311024, _app->varGetWord(311024) - 5);

		if (_app->varGetWord(311024) > -10) {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject310015, 0, Common::Point(120, 174 - _app->varGetWord(311024)));
		} else {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject310015, 0, Common::Point(120, 174));
			_app->soundStop(310103, 1024);
			_app->timerStop(kTimer1);
			_app->rotationSetAlp(311251, 206.0f);
			_app->rotationSetBet(311251, -26.0f);
			_app->rotationSetRan(311251, 87.0f);
			_app->rotationSetActive(311251);
			_app->soundPlay(310104);
		}
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->timerStart(kTimer2, rnd(200));

		if (rnd(2))
			_app->objectPresentationHide(kObject310018);
		else
			_app->objectPresentationShow(kObject310018);
		break;
	}
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
	switch (id) {
	default:
		break;

	case kObjectPipe:
		if (!_app->varGetByte(114522)) {
			_app->cursorDelete();
			_app->puzzleSetMod(kPuzzleMenu, 2, 21);
			_app->objectSetAccessibilityOff(20, 13, 13);
			_app->objectSetAccessibilityOff(kObject114503);
			_app->objectPresentationHide(kObject11);
			_app->objectPresentationShow(kObject11, 16);
			_app->objectSetAccessibilityOn(kObjectPipe, 0, 0);
			_app->objectSetAccessibilityOn(kObjectPipe, 2, 2);
			_app->varSetByte(114522, 1);
		}
		break;

	case kObjectCodingWheel:
		if (!_app->varGetByte(114520)) {
			_app->cursorDelete();
			_app->puzzleSetMod(kPuzzleMenu, 2, 23);
			_app->objectPresentationShow(kObjectCodingWheel, 35 - _app->varGetByte(114521));
			_app->objectSetAccessibilityOn(kObject23);
			_app->varSetByte(114520, 1);
		}
		break;

	case kObjectPaperKnife:
		if (_app->varGetByte(114100) > 0) {
			if (_app->soundIsPlaying(112016)) {
				sub_45FF30();
			} else {
				_app->objectPresentationShow(kObject12, 1);
				_app->cursorDelete();
				_app->objectSetAccessibilityOff(kObject22);
				_app->objectSetAccessibilityOn(kObject22, 1, 2);
				_app->soundPlay(112016, kSoundLoop);
				_app->soundSetVolume(112016, 1);
			}
		}
		break;
	}
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
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityTo == 13611) {
			if (movabilityFrom == 13531)
				_app->timerStop(kTimer4);
		} else if (movabilityTo == 13531) {
			if (movabilityTo == 13611)
				_app->timerStart(kTimer4, rnd(4000));
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (movabilityFrom == 13902)
			sub_4A2CF0();
		break;
	}
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
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 53125:
		if (_app->bagHasClickedObject()
		 && _app->bagGetClickedObject() == kObjectMatches2)
			_app->cursorDelete();
		break;

	case 53198:
		_app->soundPlay(53145);
		break;

	case 522011:
	case 522012:
	case 522013:
	case 531411:
	case 531412:
	case 531413:
	case 542021:
		_app->soundPlay(id + 1);
		break;

	case 531021:
		if (_app->varGetByte(52999) > 3) {
			_app->soundStop(52000, 1024);
			_app->soundStop(53103, 1024);
			_app->timerStop(kTimer6);
			_app->rotationSet3DSoundOff(53011, 53102);
			_app->rotationSet3DSoundOff(53021, 53102);

			_app->displayFade("a05_black.bmp", "A15_E07M09a.bmp", 15, 0, kLoadFromCd, kArchiveArt);
			_app->playMovie("1509");
			_app->displayFade("A15_E07M09b.bmp", "a05_black", 15, 0, kLoadFromCd, kArchiveArt);

			_app->rotationSetAlp(55011, 0.0f);
			_app->rotationSetBet(55011, 0.0f);
			_app->rotationSetRan(55011, 87.0f);
			_app->rotationSetActive(55011);
			_app->soundPlay(51001, kSoundLoop);
		}
		break;
	}
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
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 120101:
	case 120102:
	case 120110:
	case 120120:
	case 120130:
	case 120131:
	case 120132:
	case 120133:
	case 120134:
	case 120136:
	case 120137:
	case 120138:
	case 120145:
	case 120146:
	case 120147:
		_app->soundPlay(id + 1);
		break;

	case 120022:
		if (_app->bagHasClickedObject()
		 && _app->bagGetClickedObject() == kObjectMatches4)
			_app->cursorDelete();
		break;

	case 120139:
		_app->playMovie("2148");
		_app->displayFade("A12S06N02R01_FADE_02.bmp", "A12S06N02R01_FADE_01.bmp", 12, 0, kLoadFromCd, kArchiveArt);
		_app->rotationSetAlp(120602, 98.0f);
		_app->rotationSetBet(120602, 16.0f);
		_app->rotationSetRan(120602, 71.0f);
		_app->rotationSetActive(120602);
		_app->rotationSetRolTo(120602, 98.0f, 16.0f, 87.0f);
		break;

	case 120150:
		_app->objectPresentationHide(kObject15);
		_app->objectSetAccessibilityOff(kObject1);
		_app->rotationSetAlp(_app->varGetDword(120007), _app->varGetWord(120006));
		_app->rotationSetBet(_app->varGetDword(120007), _app->varGetWord(120007));
		_app->rotationSetRan(_app->varGetDword(120007), _app->varGetWord(120008));
		_app->rotationSetActive(_app->varGetDword(120007));

		_app->varSetWord(120001, 130);
		_app->varSetWord(120002, 130);
		_app->varSetByte(98088, 1);
		_app->objectPresentationShow(kObject6, 0);
		_app->objectPresentationShow(kObject6, 1);
		_app->objectPresentationShow(kObject6, 2);
		_app->timerStart(kTimer1, 2090);
		_app->timerStart(kTimer2, 3136);
		_app->objectSetAccessibilityOn(kObject6);
		break;
	}
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
	if (animationId == 2 && frame == _app->getSaveManager()->getData()->progress)
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
	switch (animationId) {
	default:
		break;

	case 61300:
		if (frame == frameCount) {
			_app->objectPresentationShow((ObjectId)(kObject61302 + _app->varGetByte(61909), _app->varGetByte(61901 + _app->varGetByte(61909))));
			_app->objectSetAccessibilityOn(kObjectWhip);

			if (_app->varGetByte(61901) == 2 && _app->varGetByte(61902) == 2)
				_app->timerStart(kTimer2, 1000);
		}
		break;

	case 61301:
		if (frame == frameCount) {
			_app->objectPresentationShow(kObject61303, _app->varGetByte(61902));
			_app->objectSetAccessibilityOn(kObjectWhip);
		}
		break;
	}
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
	switch (animationId) {
	default:
		break;

	case 91001:
		if (frame == 15) {
			_app->objectPresentationHide(kObject91002);
			_app->puzzleSetActive(kPuzzle91001);
		}
		break;

	case 91002:
		if (frame == 1) {
			_app->soundPlay(91103);
			_app->objectSetAccessibilityOn(kObject91002, 0, 0);
		}
		break;

	case 91003:
		if (frame == 10) {
			_app->rotationSetAlp(91007, 320.0f);
			_app->rotationSetBet(91007, 0.0f);
			_app->rotationSetRan(91007, 87.0f);
			_app->rotationSetActive(91007);
			_app->objectPresentationHide(kObject91001, 0);
			_app->objectSetAccessibilityOn(kObject91001, 0, 0);
			_app->soundPlay(91121);
		}
		break;
	}
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

#pragma endregion

} // End of namespace Ring
