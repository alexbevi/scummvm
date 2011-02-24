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

#include "ring/game/event_ring.h"

#include "ring/base/application.h"
#include "ring/base/bag.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"

#include "ring/graphics/dragControl.h"

#include "ring/helpers.h"
#include "ring/ring.h"
#include "ring/shared_ring.h"

using namespace RingGame;

namespace Ring {

EventHandlerRing::EventHandlerRing(Application *application) : _app(application), _controlNotPressed(false) {
	// Data
	_dword_4A1C00 = 0;
}


EventHandlerRing::~EventHandlerRing() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerRing::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	_controlNotPressed = !isControlPressed;

	onMouseLeftButtonUp(evt);
}

void EventHandlerRing::onMouseLeftButtonUp(Common::Event &evt) {
	error("[EventHandlerRing::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerRing::onMouseLeftButtonDown(Common::Event &evt) {
	error("[EventHandlerRing::onMouseLeftButtonDown] Not implemented");
}

void EventHandlerRing::onMouseRightButtonUp(Common::Event &evt) {
	if (getApp()->getDragControl()->getField20() || getApp()->getField6F())
		return;

	Puzzle *puzzleMenu = getApp()->puzzleGet(kPuzzleMenu);
	if (puzzleMenu && puzzleMenu->getField24() == 2)
		return;

	Bag *bag = getApp()->getBag();
	if (bag->getField94()) {
		bag->sub_419350();
	} else {
		getApp()->cursorDelete();
		bag->sub_4192E0();

		if (getApp()->getCurrentRotation())
			bag->setCurrentRotationFre(getApp()->getCurrentRotation()->getFre());

		getApp()->setupCurrentRotation();
	}

}

void EventHandlerRing::onKeyDown(Common::Event &evt) {
	error("[EventHandlerRing::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerRing::onKeyDownZone(Common::KeyCode keycode) {
	if (_app->getCurrentZone() == kZoneSY)
		onKeyDownZoneSY(keycode);
}

void EventHandlerRing::onKeyDownZoneSY(Common::KeyCode keycode) {
	error("[EventHandlerRing::onKeyDownZoneSY] Not implemented");
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerRing::onSetup(Zone zone, SetupType type) {
	switch (zone) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onSetupZoneNI(type);
		break;

	case kZoneRH:
		onSetupZoneRH(type);
		break;

	case kZoneFO:
		onSetupZoneFO(type);
		break;

	case kZoneRO:
		onSetupZoneRO(type);
		break;

	case kZoneWA:
		onSetupZoneWA(type);
		break;

	case kZoneAS:
		onSetupZoneAS(type);
		break;

	case kZoneN2:
		onSetupZoneN2(type);
		break;
	}
}

void EventHandlerRing::onSetupZoneNI(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->playMovie("1540");
		_app->soundPlay(14001, 2);
		_app->playMovie("1541");
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10001, 1);
		break;

	case kSetupType3:
		_app->timerStopAll();
		_app->puzzleSetMovabilityOn(kPuzzle10410, 0, 0);
		_app->playMovie("1550");
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		_app->soundPlay(14001, 2);
		_app->soundPlay(10021, 1);
		_app->varSetByte(10303, 1);
		break;

	case kSetupType10:
		onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectGlug);
		_app->bagAdd(kObjectMinerals);
		_app->bagAdd(kObjectLogeTear);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->varSetByte(10106, 1);
		_app->objectPresentationShow(kObject1);
		_app->soundPlay(10409, 2);
		_app->rotationSetActive(10415);
		break;
	}
}

void EventHandlerRing::onSetupZoneRH(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemove(kObjectAntiGCells);
		_app->soundPlay(23005, 2);
		_app->playMovie("1706");
		_app->puzzleSetActive(kPuzzle22001);
		_app->soundPlay(22001, 1);
		break;

	case kSetupType10:
		onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->bagAdd(kObjectMedallion);
		_app->bagAdd(kObjectKeyIndifference);
		_app->bagAdd(kObjectKeyMistrust);
		_app->bagAdd(kObjectKeySelfishness);
		_app->bagAdd(kObjectKeyDisgust);
		_app->rotationSetActive(20501);
		break;
	}
}

void EventHandlerRing::onSetupZoneFO(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectLogeTear2);
		_app->timerStart(kTimer1, 3000);
		_app->timerStart(kTimer2, 5000);
		_app->timerStart(kTimer3, 17000);
		_app->timerStart(kTimer4, 10000);
		_app->timerStart(kTimer5, 10);
		_app->bagAdd(kObjectWolfInstinct);
		_app->playMovie("1217");
		_app->playMovie("1218");
		_app->rotationSetAlp(30003, 180.0f);
		_app->rotationSetActive(30003);
		break;

	case kSetupType10:
		onSetupLoadTimers("sie", 90019, 90023, 90027);
		break;

	case kSetupType999:
		_app->bagAdd(kObjectWolfInstinct);
		_app->bagAdd(kObjectKey);
		_app->bagAdd(kObjectIngot);
		_app->bagAdd(kObjectIngot2);
		_app->bagAdd(kObjectIngot3);
		_app->puzzleSetActive(kPuzzle35011);
		break;
	}
}

void EventHandlerRing::onSetupZoneRO(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupType10:
		onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case kSetupType999:
		_app->rotationSetAlp(40000, 0.0f);
		_app->rotationSetRan(40000, 85.3f);
		_app->rotationSetActive(40000);
		_app->bagRemove(kObjectFire);
		_app->playMovie("1506");
		_app->bagAdd(kObjectFirePower);
		_app->bagAdd(kObjectRing);
		_app->bagAdd(kObjectCrown);
		_app->puzzleSetActive(kPuzzle40100);
		_app->soundPlay(40700, 1);
		break;
	}
}

void EventHandlerRing::onSetupZoneWA(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectMagicLance);
		_app->bagAdd(kObjectGolem1);
		_app->playMovie("1880");
		_app->playMovie("1881");
		_app->rotationSetAlp(50001, 320.0f);
		_app->rotationSetActive(50001);
		_app->puzzleSetActive(kPuzzle50002);
		_app->soundIsPlaying(50001);
		break;

	case kSetupType10:
		onSetupLoadTimers("bru", 90020, 90024, 90028);
		break;

	case kSetupType999:
		_app->bagAdd(kObjectMagicLance);
		_app->bagAdd(kObjectThread);
		_app->bagAdd(kObjectGolem1);
		_app->bagAdd(kObjectDragonSword);
		_app->bagAdd(kObjectFlower);
		_app->bagAdd(kObjectApple);
		_app->bagAdd(kObjectDeadLeaf);
		_app->bagAdd(kObjectBark);
		_app->varSetFloat(90008, 90.0f);
		_app->objectPresentationShow(kObject1);
		_app->varSetByte(50012, 4);
		_app->puzzleSetActive(kPuzzle50100);
		break;
	}
}

void EventHandlerRing::onSetupZoneAS(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupType5:
		_app->playMovie("1047");
		_app->rotationSetAlp(80003, 270.0f);
		_app->rotationSetBet(80003, 0.0f);
		_app->rotationSetRan(80003, 85.3f);
		_app->rotationSetActive(80003);
		break;

	case kSetupType6:
		switch (_app->languageGetCurrent()) {
		default:
			_app->playMovieChannel("1162", _app->languageGetChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1161", _app->languageGetChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1160", _app->languageGetChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle80001);
		_app->soundPlay(80107, 1);
		break;

	case kSetupType998:
		switch (_app->languageGetCurrent()) {
		default:
			_app->playMovieChannel("1164", _app->languageGetChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1163", _app->languageGetChannel());
			break;
		}

		_app->playMovie("1666");
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80100, 1);
		break;

	case kSetupType999:
		_app->rotationSetAlp(80001, 90.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void EventHandlerRing::onSetupZoneN2(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->bagAdd(kObjectFire);
		_app->playMovie("1507");
		_app->playMovie("1508");
		_app->puzzleSetActive(kPuzzle72001);
		_app->objectPresentationHide(kObject72000);
		_app->cursorSelect(kCursorDefault);
		_app->soundPlay(72001, 1);
		break;

	case kSetupType10:
		onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->bagAdd(kObjectFire);
		_app->soundPlay(70000, 2);
		_app->rotationSetActive(70400);
		break;
	}
}

void EventHandlerRing::onSetupLoadTimers(Common::String zoneName, Id testId1, Id puzzleRotationId, Id testId2) {
	if (!_app->getSaveManager()->has(zoneName + ".ars"))
		error("[EventHandlerRing::onSetupLoadTimers] Cannot find savegame (%s.ars)!", zoneName.c_str());

	_app->bagRemoveAll();

	if (_app->varGetByte(testId1)) {
		_app->puzzleSetActive((PuzzleId)_app->varGetDword(puzzleRotationId));
	} else {
		_app->rotationSetActive(_app->varGetDword(puzzleRotationId));

		if (_app->varGetByte(testId2))
			_app->rotationSetFreOn(_app->varGetDword(puzzleRotationId));
		else
			_app->rotationSetFreOff(_app->varGetDword(puzzleRotationId));
	}

	if (!_app->getSaveManager()->loadSaveTimer(zoneName, kLoadSaveRead))
		error("[EventHandlerRing::onSetupLoadTimers] Cannot load timers (%s)!", zoneName.c_str());

	_app->getSoundManager()->sub_4696F0();
}

#pragma endregion

#pragma region Timer

void EventHandlerRing::onTimer(TimerId timerId) {
	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneWA:
		break;

	case kZoneNI:
		onTimerZoneNI(timerId);
		break;

	case kZoneRH:
		onTimerZoneRH(timerId);
		break;

	case kZoneFO:
		onTimerZoneFO(timerId);
		break;

	case kZoneRO:
		onTimerZoneRO(timerId);
		break;

	case kZoneAS:
		onTimerZoneAS(timerId);
		break;

	case kZoneN2:
		onTimerZoneN2(timerId);
		break;
	}
}

void EventHandlerRing::onTimerZoneNI(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneRH(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneFO(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneRO(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetByte(40806, _app->varGetByte(40806) + 1);

		if (_app->varGetByte(40806) == 10) {
			_app->timerStop(kTimer0);
			_app->varSetByte(40601, 10);
		}

		_app->objectPresentationShow(kObject40101, _app->varGetByte(40806));
		break;

	case kTimer1:
		_app->varSetByte(40807, _app->varGetByte(40807) + 1);

		if (_app->varGetByte(40807) == 90) {
			_app->timerStop(kTimer1);
			_app->varSetByte(40602, 90);
		}

		_app->objectPresentationShow(kObject40102, _app->varGetByte(40807));
		break;
	}

	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneAS(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneN2(TimerId id) {
	if (id == kTimer0)
		_app->soundPlay(rnd(12) + 70004, 1);
}

#pragma endregion

#pragma region Bag

void EventHandlerRing::onBag(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	if (puzzleRotationId == 1 && a4 == 1) {
		onBagZoneSY(id, a2, 1, 1, dragControl, a6);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneSY:
		onBagZoneSY(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneNI:
		onBagZoneNI(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneFO:
		onBagZoneFO(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneRO:
		onBagZoneRO(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneN2:
		onBagZoneN2(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;
	}
}

void EventHandlerRing::onBagZoneSY(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneSY] Not implemented");
}

void EventHandlerRing::onBagZoneNI(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneNI] Not implemented");
}

void EventHandlerRing::onBagZoneFO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneFO] Not implemented");
}

void EventHandlerRing::onBagZoneRO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneRO] Not implemented");
}

void EventHandlerRing::onBagZoneN2(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneN2] Not implemented");
}

void EventHandlerRing::onBagClickedObject(ObjectId id) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneFO:
		onBagClickedObjectZoneFO(id);
		break;
	}
}

void EventHandlerRing::onBagClickedObjectZoneFO(ObjectId objectId) {
	if (objectId != kObjectWolfInstinct)
		return;

	Id id = objectId;
	if (_app->hasCurrentRotation())
		id = _app->getCurrentRotationId();

	if (_app->hasCurrentPuzzle())
		id = _app->getCurrentPuzzleId();

	// Rotation
	if (id == 30302 || id == 30303) {
		if (_app->varGetByte(30017) == 1) {
			_app->rotationSetAlp(30302, _app->rotationGetAlp(30303));
			_app->rotationSetBet(30302, _app->rotationGetBet(30303));
			_app->rotationSetRan(30302, _app->rotationGetRan(30303));
			_app->rotationSetActive(30302);

			_app->varSetByte(30017, 0);
		} else {
			_app->rotationSetAlp(30303, _app->rotationGetAlp(30302));
			_app->rotationSetBet(30303, _app->rotationGetBet(30302));
			_app->rotationSetRan(30303, _app->rotationGetRan(30302));
			_app->rotationSetActive(30303);

			_app->varSetByte(30017, 1);
		}
	} else if (id == kPuzzle35020) { //  Puzzle 35020
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectWorms, 0);
			_app->objectPresentationHide(kObjectWorms, 1);
			_app->objectSetAccessibilityOff(kObjectWorms, 0, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectWorms, 0);
			_app->objectPresentationShow(kObjectWorms, 1);
			_app->objectSetAccessibilityOn(kObjectWorms, 0, 0);
		}
	} else if (id == kPuzzle35019) { //  Puzzle 35019
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectNotung);
			_app->objectSetAccessibilityOff(kObjectNotung, 0, 0);
		}

		_app->varSetByte(30017, 1);
		_app->objectSetAccessibilityOn(kObjectNotung, 0, 0);

		if (_app->varGetByte(30066) != 1 && _app->varGetByte(30067) != 1 ) {
			_app->objectPresentationShow(kObjectNotung, 0);
		} else {
			_app->objectPresentationShow(kObjectNotung, 0);
			_app->objectPresentationShow(kObjectNotung, 2);
		}
	} else if (id == kPuzzle35002) { //  Puzzle 35002
		if (_app->varGetByte(30017)) {
			_app->objectPresentationHide(kObjectBerries, 1);
			_app->objectPresentationHide(kObjectBerries, 2);
			_app->objectPresentationHide(kObjectBerries, 3);
			_app->varSetByte(30017, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectBerries, 1);

			if (_app->varGetByte(30075) == 1)
				_app->objectPresentationHide(kObjectBerries, 3);
			else
				_app->objectPresentationShow(kObjectBerries, 3);

			if (_app->varGetByte(30076) == 1)
				_app->objectPresentationHide(kObjectBerries, 2);
			else
				_app->objectPresentationShow(kObjectBerries, 2);
		}
	} else {
		return;
	}

	_app->setField78(false);
}

#pragma endregion

#pragma region Rides

void EventHandlerRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onBeforeRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRH:
		onBeforeRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneFO:
		onBeforeRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRO:
		onBeforeRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneWA:
		onBeforeRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneAS:
		onBeforeRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneN2:
		onBeforeRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 10005)
			if (movabilityTo == 10101) {
				_app->soundStop(10901, 1024);
				_app->soundPlay(rnd(8) + 13001, 1);
			}
		break;

	case kMovabilityRotationToPuzzle:
		switch (movabilityIndex) {
		default:
			break;

		case 41:
			if (_app->varGetByte(10420))
				_app->playMovie("1529");
			else
				_app->playMovie("1528");
			break;

		case 42:
			if (_app->varGetByte(10421))
				_app->playMovie("1531");
			else
				_app->playMovie("1530");
			break;

		case 61:
			_app->varSetWord(10600, 12);
			_app->varSetWord(10601, 0);
			_app->varSetWord(10602, 24);

			_app->objectPresentationHideAndRemove(kObject10601);
			_app->objectPresentationShow(kObject10601, 12);
			_app->objectPresentationHideAndRemove(kObject10602);
			_app->objectPresentationShow(kObject10602, 0);
			_app->objectPresentationHideAndRemove(kObject10603);
			_app->objectPresentationShow(kObject10603, 24);
			break;
		}
		break;

	case kMovabilityPuzzleToRotation:
		switch (movabilityIndex) {
		default:
			break;

		case 41:
			if (_app->varGetByte(10420))
				_app->playMovie("1534");
			else
				_app->playMovie("1533");
			break;

		case 42:
			if (_app->varGetByte(10421))
				_app->playMovie("1536");
			else
				_app->playMovie("1535");
			break;
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (movabilityTo == 10501 || movabilityTo == 10511 || movabilityTo == 10521)
			if (!movabilityIndex)
				_app->soundPlay(10501, 1);

		if (movabilityFrom == 10501 || movabilityFrom == 10511 || movabilityFrom == 10521)
			if (!movabilityIndex)
				_app->soundPlay(10502, 1);

		if (movabilityFrom == 10001) {
			_app->soundPlay(10804, 1);
		} else if (movabilityFrom == 10002) {
			_app->soundPlay(10804, 1);
			_app->soundStop(10800, 1024);
		}
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 20401)
		_app->timerStop(kTimer0);
}

void EventHandlerRing::onBeforeRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 30402 && movabilityTo == 30501) {
			_app->varSetByte(30037, 1);
			_app->objectPresentationShow(kObject30051, 0);
			_app->objectPresentationHide(kObject30050);
			_app->objectPresentationShow(kObject30050, 3);
			_app->rotationSetMovabilityOff(30011, 1, 2);
			_app->objectSetAccessibilityOn(kObject30051, 0, 0);
			_app->objectSetAccessibilityOn(kObject30051, 3, 3);
		}
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom == 30601 && movabilityTo == 35003)
			_app->objectSetAccessibilityOn(kObjectBow, 0, 0);
		break;

	case kMovabilityPuzzleToRotation:
		switch (movabilityFrom) {
		default:
			break;

		case 35002:
			if (movabilityTo == 30801) {
				if (!_app->varGetByte(30076)) {
					_app->objectPresentationHide(kObjectBerries);
					_app->varSetByte(30017, 0);
				}
			}
			break;

		case 35003:
			if (movabilityTo == 30601)
				_app->objectPresentationHide(kObjectBow);
			break;

		case 35004:
			if (movabilityTo == 30601) {
				if (_app->varGetByte(30091) == 1 && _app->varGetByte(30021) == 1) {
					_app->playMovie("1184");
					_app->bagAdd(kObjectLogeTear2);

					if (_app->varGetByte(30020) == 1) {
						_app->bagAdd(kObjectMould);
						_app->varSetByte(30020, 0);
					}

					_app->objectPresentationHide(kObject30028);
					_app->objectPresentationShow(kObject30028, 0);
					_app->objectPresentationShow(kObject30028, 3);

					_app->varSetByte(30021, 0);
				}
			}
			break;

		case 35006:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30040);
				_app->varSetByte(30022, 0);
				_app->varSetByte(30023, 0);
				_app->objectSetAccessibilityOn(kObject30040, 0, 1);
				_app->objectSetAccessibilityOff(kObject30040, 2, 5);
			}
			break;

		case 35007:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30042);
				_app->objectSetAccessibilityOff(kObject30042, 1, 7);
				_app->objectSetAccessibilityOn(kObject30042, 0, 0);
				_app->varSetByte(30025, 0);
				_app->varSetByte(30026, 0);
				_app->varSetByte(30027, 0);
				_app->varSetByte(30028, 0);
				_app->varSetByte(30029, 0);
				_app->varSetByte(30030, 0);
				_app->varSetByte(30031, 0);
			}
			break;

		case 35008:
			if (movabilityTo == 30601) {
				if (_app->varGetByte(30032) == 1) {
					_app->bagAdd(kObjectGolem);
					_app->objectPresentationHide(30044, 0);
				}
			}
			break;

		case 35009:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30045);
			} else if (movabilityTo == 30602) {
				_app->playMovie("1205");
				_app->objectPresentationShow(kObject30045, 7);
			}
			break;

		case 30701:
		case 30702:
		case 30703:
		case 30704:
			if (_app->varGetByte(30033)) {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 1, 1);
			} else {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 1);
			}
			break;

		case 35111:
			if (movabilityTo == 30101) {
				_app->playMovie("1206");
				_app->timerStop(kTimer5);
			}
			break;
		}

		_app->objectSetAccessibilityOff(kObjectNotung, 0, 0);
		_app->objectPresentationHide(kObjectNotung);
		_app->objectPresentationHide(kObjectWorms, 0);
		_app->objectPresentationHide(kObjectWorms, 1);
		_app->objectPresentationHide(kObjectBerries, 3);
		_app->objectPresentationHide(kObjectBerries, 1);
		_app->varSetByte(30017, 0);
		_app->objectSetAccessibilityOff(kObjectWorms, 0, 0);
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityPuzzleToRotation)
		return;

	if (movabilityFrom == 40060 || movabilityFrom == 40005) {
		_app->soundSetVolume(40102, 100);
		_app->soundPlay(40102, 2);

		if (_app->varGetByte(40804) > 4) {
			do {
				_app->objectPresentationHide(kObject40060);
				_app->objectPresentationShow(kObject40060, _app->varGetByte(40804));

				handleEvents();

				_app->varSetByte(40804, _app->varGetByte(40804) + 1);
			} while (!checkEvents() && _app->varGetByte(40804) > 0);
		}

		_app->varSetByte(40804, 0);
		_app->soundStop(40102, 1024);
		if (_app->varGetByte(40802) == 1)
			_app->playMovie("1785");

		_app->objectSetAccessibilityOff(kObject40060);
		_app->objectSetAccessibilityOn(kObject40060, 0, 0);
		_app->objectSetAccessibilityOn(kObject40201);
		_app->objectSetAccessibilityOff(kObject40202);

		_app->objectPresentationHide(kObject40060);
		_app->objectPresentationHide(kObject40201);
		_app->objectPresentationHide(kObject40202);

		_app->varSetByte(40802, 0);
		_app->varSetString(40902, "00000000");

		_dword_4A1C00 = 0;
	}
}

void EventHandlerRing::onBeforeRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityRotationToRotation)
		return;

	switch (movabilityFrom){
	default:
		break;

	case 50001:
		if (movabilityTo == 50101)
			if (!_app->soundIsPlaying(51006))
				_app->soundPlay(51006, 2);
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51007, 2);
		}
		break;

	case 50304:
		if (movabilityTo == 50302) {
			if (_app->varGetByte(50003)) {
				if (_app->varGetByte(50005) >= 2)
					_app->playMovie("1871");
				else
					_app->playMovie("1870");
			} else if (_app->varGetByte(50005)) {
				_app->playMovie("1869");
			} else {
				_app->playMovie("1868");
			}
		}
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityPuzzleToRotation)
		return;

	switch (movabilityFrom){
	default:
		break;

	case 80006:
		if (movabilityTo == 80101) {
			if (_app->varGetByte(90001))
				_app->playMovie("1152");
			else
				_app->playMovie("1151");
		}
		break;

	case 80007:
		if (movabilityTo == 80101)
			_app->playMovie("1153");
		break;

	case 80008:
		if (movabilityTo == 80101)
			_app->playMovie("1155");
		break;

	case 80009:
		if (movabilityTo == 80101)
			_app->playMovie("1154");
		break;

	case 80010:
		if (movabilityTo == 80101)
			_app->playMovie("1156");
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		_app->timerStop(kTimer0);

	if (movabilityType == kMovabilityPuzzleToPuzzle) {
		if (movabilityTo == 70501 || movabilityTo == 70511 || movabilityTo == 70521)
			_app->soundPlay(70501, 1);

		if (movabilityFrom == 70501 || movabilityFrom == 70511 || movabilityFrom == 70521)
			_app->soundPlay(70502, 1);
	}
}

void EventHandlerRing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityTo == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onAfterRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRH:
		onAfterRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneFO:
		onAfterRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRO:
		onAfterRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneWA:
		onAfterRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneAS:
		onAfterRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneN2:
		onAfterRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;
	}
}

void EventHandlerRing::onAfterRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (a4 == 100) {
		_app->rotationSetMovabilityOff(movabilityFrom, 0, 0);
	} else if (a4 == 110) {
		if (_app->soundIsPlaying(10800))
			_app->soundStop(10800, 1024);

		if (_app->soundIsPlaying(10901))
			_app->soundPlay(10902, 1);

		_app->soundStop(10901, 1024);

		if (movabilityFrom == 10301) {
			_app->soundPlay(10300, 2);
			_app->varSetByte(10301, 0);
		}
	}

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 10201) {
			if (!_app->soundIsPlaying(14002)) {
				_app->soundStop(14001, 1024);
				_app->soundStop(14003, 1024);
				_app->soundPlay(14002, 2);
			}
		}

		if (movabilityFrom > 10201 || movabilityFrom == 10101) {
			if (!_app->soundIsPlaying(14003)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14001, 1024);
				_app->soundPlay(14003, 2);
			}
		}

		if (movabilityFrom >= 10000 && movabilityFrom <= 10005) {
			if(!_app->soundIsPlaying(14001)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14003, 1024);
				_app->soundPlay(14001, 2);
			}
		}

		switch (a4) {
		default:
			break;

		case 1:
			_app->rotationSetMovabilityOff(movabilityFrom);
			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);

			if (movabilityFrom == 10000 || movabilityFrom == 10002)
				_app->rotationSetMovabilityOn(movabilityFrom, 2, 2);
			break;

		case 2:
			_app->rotationSetMovabilityOff(movabilityFrom);
			_app->rotationSetMovabilityOn(movabilityFrom, 1, 1);
			break;

		case 3:
			_app->rotationSetMovabilityOff(movabilityFrom, 0, 0);
			break;

		case 5:
		case 21:
			_app->objectPresentationHide(kObject10003, 8);
			_app->objectPresentationHide(kObject10003, 9);
			_app->objectPresentationHide(kObject10003, 10);
			_app->objectPresentationHide(kObject10003, 11);
			_app->objectPresentationHide(kObject10003, 12);
			_app->objectPresentationShow(kObject10003, 3);
			_app->objectPresentationShow(kObject10003, 4);
			_app->objectPresentationShow(kObject10003, 5);
			_app->objectPresentationShow(kObject10003, 6);
			_app->objectPresentationShow(kObject10003, 7);

			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);
			break;

		case 16:
			_app->exitToMenu(3);
			return;

		case 41:
			_app->objectPresentationHide(kObject10003, 3);
			_app->objectPresentationHide(kObject10003, 4);
			_app->objectPresentationHide(kObject10003, 5);
			_app->objectPresentationHide(kObject10003, 6);
			_app->objectPresentationHide(kObject10003, 7);
			_app->objectPresentationShow(kObject10003, 8);
			_app->objectPresentationShow(kObject10003, 9);
			_app->objectPresentationShow(kObject10003, 10);
			_app->objectPresentationShow(kObject10003, 11);
			_app->objectPresentationShow(kObject10003, 12);

			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);
			break;
		}

		if (movabilityTo == 10005) {
			if (movabilityFrom == 10101) {
				_app->soundStop(13001, 1024);
				_app->soundStop(13002, 1024);
				_app->soundStop(13003, 1024);
				_app->soundStop(13004, 1024);
				_app->soundStop(13005, 1024);
				_app->soundStop(13006, 1024);
				_app->soundStop(13007, 1024);
				_app->soundStop(13008, 1024);
				_app->soundStop(13009, 1024);

				return;
			}
		} else if (movabilityTo == 10415) {
			_app->soundStop(10409, 1024);
		}

		if (movabilityFrom == 10406) {
			if (!_app->soundIsPlaying(10410)) {
				_app->soundPlay(10410, 2);
				_app->soundPlay(10411, 2);
			}
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (a4 == 55) {
			_app->playMovie("1537", 0.0);
			_app->puzzleSetActive(kPuzzle12001, 1, 1);
			_app->soundPlay(12001, 1);
		}
		break;
	}
}

void EventHandlerRing::onAfterRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 20401)
		_app->timerStart(kTimer0, 50);

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom >= 20010) {
			_app->varSetByte(21001, 0);

			if (_app->bagHas(kObjectKeyIndifference)) {
				_app->rotationSetMovabilityOn(20010, 0, 0);
				_app->rotationSetMovabilityOff(20010, 1, 1);
			}
		}

		if (movabilityFrom >= 20020) {
			_app->varSetByte(21001, 1);

			if (_app->bagHas(kObjectKeyMistrust)) {
				_app->rotationSetMovabilityOn(20020, 0, 0);
				_app->rotationSetMovabilityOff(20020, 1, 1);
			}
		}

		if (movabilityFrom >= 20030) {
			_app->varSetByte(21001, 2);

			if (_app->bagHas(kObjectKeySelfishness)) {
				_app->rotationSetMovabilityOn(20030, 0, 0);
				_app->rotationSetMovabilityOff(20030, 1, 1);
			}
		}
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom >= 20011 && movabilityFrom <= 20031) {
			ObjectId id = 20001 + _app->varGetByte(21001);
			_app->objectPresentationShow(id, 0);
			_app->objectPresentationSetAnimationOnPuzzle(id, 2, 0, 20001);
			_app->objectPresentationShow(id, 2);
		}
		break;
	}
}

void EventHandlerRing::onAfterRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		switch (movabilityFrom) {
		default:
			break;

		case 30006:
			if (movabilityTo == 30008 ) {
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 100);
			} else if (movabilityTo == 30005) {
				_app->rotationSetMovabilityRideName(30005, 1, "fom");
			}
			break;

		case 30008:
			if (movabilityTo == 30006)
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 80);
			break;

		case 30011:
			if (movabilityTo == 30401)
				for (uint32 i = 0; i < 27; i++)
					_app->soundSetVolume(i + 30300, 100);
			break;

		case 30012:
			if (movabilityTo == 30501) {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);
				_app->timerStart(kTimer1, 10);
				_app->timerStart(kTimer3, 10);
			}
			break;

		case 30101:
			if (movabilityTo == 30003) {
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 80);

				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 100);
			}
			break;

		case 30401:
			if (movabilityTo == 30011)
				for (uint32 i = 0; i < 27; i++)
					_app->soundSetVolume(i + 30300, 85);
			break;

		case 30501:
			if (movabilityTo == 30012) {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);

				if (!_app->varGetByte(30071)) {
					_app->soundPlay(30117, 1);
					_app->varSetByte(30071, 1);
				}
			}
			break;
		}
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityFrom == 30703 && movabilityTo == 35010) {
			_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
			_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 2);
			_app->objectPresentationHide(kObjectFishingRod);
		} else if (movabilityFrom == 30301 && movabilityTo == 35011) {
			_app->objectPresentationHide(kObject30001);
			_app->objectSetAccessibilityOff(kObject30001, 1, 3);
			if (!_app->varGetByte(30043))
				_app->objectSetAccessibilityOn(kObject30001, 0, 0);
		}
		break;
	}
}

void EventHandlerRing::onAfterRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityTo == 40000 || movabilityFrom == 40010)
			_app->varSetByte(40804, 0);

		if (movabilityTo == 40005 || movabilityFrom == 40060) {
			_app->soundPlay(40003, 2);
			_app->soundSetVolume(40003, 88);
		}
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityTo == 40060 || movabilityFrom == 40005)
			_app->soundSetVolume(40003, 82);
		break;
	}
}

void EventHandlerRing::onAfterRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityRotationToRotation)
		return;

	switch (movabilityFrom) {
	default:
		break;

	case 50201:
		if (movabilityTo == 50102) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51002, 2);
			_app->soundPlay(51012, 2);
		}

		if (_app->varGetByte(50001) == 2) {
			if (_app->varGetByte(50003) == 1) {
				_app->objectPresentationHide(kObjectRopes, 1);
				_app->objectPresentationShow(kObjectRopes, 2);
				_app->varSetByte(50001, 3);
				_app->playMovie("1858");
				_app->bagAdd(kObjectDeadLeaf);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->varSetByte(50012, _app->varGetByte(50012) + 1);

				if (_app->varGetByte(50012) == 2) {
					_app->objectPresentationShow(kObject50700, 0);
					_app->objectSetAccessibilityOff(kObject50700);
					_app->objectSetAccessibilityOn(kObject50700, 1, 1);
					_app->rotationSetMovabilityOff(50103, 2, 2);
					_app->rotationSetMovabilityOn(50103, 3, 3);
				}

				if (_app->varGetByte(50012) == 4) {
					_app->objectPresentationShow(kObject50700, 1);
					_app->objectSetAccessibilityOff(kObject50700);
					_app->objectSetAccessibilityOn(kObject50700, 2, 2);
				}
			}
		}
		break;

	case 50102:
		if (movabilityTo == 50201) {
			_app->soundStop(51002, 1024);
			_app->soundStop(51012, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50301:
		if (movabilityTo == 50104) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51004, 2);

			if (_app->varGetByte(50003) > 0)
				_app->soundPlay(51013, 2);
		}
		break;

	case 50104:
		if (movabilityTo == 50301) {
			_app->soundStop(51004, 1024);
			_app->soundStop(51013, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50106:
		if (movabilityTo == 50501) {
			_app->soundStop(51001, 1024);
			_app->soundStop(51011, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51007, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50108:
		if (movabilityTo == 50601)
			_app->soundPlay(51006, 2);
		break;
	}
}

void EventHandlerRing::onAfterRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityPuzzleToRotation && movabilityFrom == 80101)
		if (movabilityTo == 80006 || movabilityTo == 80007 || movabilityTo == 80008
		 || movabilityTo == 80009 || movabilityTo == 80010)
			_app->varSetByte(80003, 0);
}

void EventHandlerRing::onAfterRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		if (!_app->varGetByte(70012))
			_app->timerStart(kTimer0, 10000);

	if (movabilityType == kMovabilityRotationToRotation) {
		if (a4 == 7)
			sub_433EE0();
		else if (a4 == 16)
			_app->exitToMenu(1);
	}
}

#pragma endregion

#pragma region Sound

void EventHandlerRing::onSound(Id id, SoundType type, uint32 a3) {
	uint32 a4 = a3 & 0x1000;
	a3 &= 239;

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onSoundZoneSY(id, type, a3, a4);
		break;

	case kZoneNI:
		onSoundZoneNI(id, type, a3, a4);
		break;

	case kZoneRH:
		onSoundZoneRH(id, type, a3, a4);
		break;

	case kZoneFO:
		onSoundZoneFO(id, type, a3, a4);
		break;

	case kZoneRO:
		onSoundZoneRO(id, type, a3, a4);
		break;

	case kZoneAS:
		onSoundZoneAS(id, type, a3, a4);
		break;

	case kZoneWA:
		onSoundZoneWA(id, type, a3, a4);
		break;

	case kZoneN2:
		onSoundZoneN2(id, type, a3, a4);
		break;
	}
}

void EventHandlerRing::onSoundZoneSY(Id id, SoundType type, uint32 a3, uint32 a4) {
	if (a4 && id == 90001) {
		_app->objectPresentationHideAndRemove(kObject7, 0);
		_app->showCredits();
		_app->startMenu(false);
	}
}

void EventHandlerRing::onSoundZoneNI(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneNI] Not implemented");
}

void EventHandlerRing::onSoundZoneRH(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneRH] Not implemented");
}

void EventHandlerRing::onSoundZoneFO(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneFO] Not implemented");
}

void EventHandlerRing::onSoundZoneRO(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneRO] Not implemented");
}

void EventHandlerRing::onSoundZoneWA(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneWA] Not implemented");
}

void EventHandlerRing::onSoundZoneAS(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneAS] Not implemented");
}

void EventHandlerRing::onSoundZoneN2(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneN2] Not implemented");
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
		_app->puzzleSetActive(kPuzzle70305, 1, 1);
		_app->soundPlay(70017, 1);
	}
}

#pragma endregion

} // End of namespace Ring
