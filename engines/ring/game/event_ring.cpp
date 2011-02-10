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

#include "ring/base/sound.h"

#include "ring/game/application.h"
#include "ring/game/saveload.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

EventHandlerRing::EventHandlerRing(Application *application) : _app(application),
	_controlNotPressed(false) {
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
	error("[EventHandlerRing::onMouseRightButtonUp] Not implemented");
}

void EventHandlerRing::onKeyDown(Common::Event &evt) {
	error("[EventHandlerRing::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerRing::onSetup(Zone zone, uint32 a2) {
	switch (zone) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onSetupZoneNI(a2);
		break;

	case kZoneRH:
		onSetupZoneRH(a2);
		break;

	case kZoneFO:
		onSetupZoneFO(a2);
		break;

	case kZoneRO:
		onSetupZoneRO(a2);
		break;

	case kZoneWA:
		onSetupZoneWA(a2);
		break;

	case kZoneAS:
		onSetupZoneAS(a2);
		break;

	case kZoneN2:
		onSetupZoneN2(a2);
		break;
	}
}

void EventHandlerRing::onSetupZoneNI(uint32 a1) {
	switch (a1) {
	default:
		break;

	case 0:
		_app->playMovie("1540");
		_app->soundPlay(14001, 2);
		_app->playMovie("1541");
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10001, 1);
		break;

	case 3:
		_app->timerStopAll();
		_app->puzzleSetMovabilityOnOrOffEnableHotspot(kPuzzle10410, 0, 0);
		_app->playMovie("1550");
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		_app->soundPlay(14001, 2);
		_app->soundPlay(10021, 1);
		_app->varSetByte(10303, 1);
		break;

	case 10:
		onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case 999:
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

void EventHandlerRing::onSetupZoneRH(uint32 a1) {
	switch (a1) {
	default:
		break;

	case 0:
		_app->bagRemove(kObjectAntiGCells);
		_app->soundPlay(23005, 2);
		_app->playMovie("1706");
		_app->puzzleSetActive(kPuzzle22001);
		_app->soundPlay(22001, 1);
		break;

	case 10:
		onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case 999:
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

void EventHandlerRing::onSetupZoneFO(uint32 a1) {
	switch (a1) {
	default:
		break;

	case 0:
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

	case 10:
		onSetupLoadTimers("sie", 90019, 90023, 90027);
		break;

	case 999:
		_app->bagAdd(kObjectWolfInstinct);
		_app->bagAdd(kObjectKey);
		_app->bagAdd(kObjectIngot);
		_app->bagAdd(kObjectIngot2);
		_app->bagAdd(kObjectIngot3);
		_app->puzzleSetActive(kPuzzle35011);
		break;
	}
}

void EventHandlerRing::onSetupZoneRO(uint32 a1) {
	switch (a1) {
	default:
		break;

	case 10:
		onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case 999:
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

void EventHandlerRing::onSetupZoneWA(uint32 a1) {
	switch (a1) {
	default:
		break;

	case 0:
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

	case 10:
		onSetupLoadTimers("bru", 90020, 90024, 90028);
		break;

	case 999:
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

void EventHandlerRing::onSetupZoneAS(uint32 a1) {
	switch (a1) {
	default:
		break;

	case 5:
		_app->playMovie("1047");
		_app->rotationSetAlp(80003, 270.0f);
		_app->rotationSetBet(80003, 0.0f);
		_app->rotationSetRan(80003, 85.3f);
		_app->rotationSetActive(80003);
		break;

	case 6:
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

	case 998:
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

	case 999:
		_app->rotationSetAlp(80001, 90.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void EventHandlerRing::onSetupZoneN2(uint32 a1) {
	switch (a1) {
	default:
		break;

	case 0:
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

	case 10:
		onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case 999:
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

#pragma region Rides

void EventHandlerRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
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
	error("[EventHandlerRing::onBeforeRideZoneNI] Not implemented");
}

void EventHandlerRing::onBeforeRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onBeforeRideZoneRH] Not implemented");
}

void EventHandlerRing::onBeforeRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onBeforeRideZoneFO] Not implemented");
}

void EventHandlerRing::onBeforeRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onBeforeRideZoneRO] Not implemented");
}

void EventHandlerRing::onBeforeRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onBeforeRideZoneWA] Not implemented");
}

void EventHandlerRing::onBeforeRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onBeforeRideZoneAS] Not implemented");
}

void EventHandlerRing::onBeforeRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onBeforeRideZoneN2] Not implemented");
}

void EventHandlerRing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
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
	error("[EventHandlerRing::onAfterRideZoneNI] Not implemented");
}

void EventHandlerRing::onAfterRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onAfterRideZoneRH] Not implemented");
}

void EventHandlerRing::onAfterRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onAfterRideZoneFO] Not implemented");
}

void EventHandlerRing::onAfterRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onAfterRideZoneRO] Not implemented");
}

void EventHandlerRing::onAfterRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onAfterRideZoneWA] Not implemented");
}

void EventHandlerRing::onAfterRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onAfterRideZoneAS] Not implemented");
}

void EventHandlerRing::onAfterRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	error("[EventHandlerRing::onAfterRideZoneN2] Not implemented");
}

#pragma endregion

#pragma region Sound

void EventHandlerRing::onSound(Id id, SoundType type, uint32 a3) {
	int a4 = a3 & 0x1000;
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

} // End of namespace Ring
