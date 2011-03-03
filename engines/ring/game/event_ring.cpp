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

#include "ring/base/accessibility.h"
#include "ring/base/bag.h"
#include "ring/base/object.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"

#include "ring/game/application_ring.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"
#include "ring/ring.h"
#include "ring/shared_ring.h"

using namespace RingGame;

namespace Ring {

EventHandlerRing::EventHandlerRing(ApplicationRing *application) : _app(application), _controlNotPressed(false) {
	// Data
	_dword_4A1C00 = 0;

	// Timer data
	_disableTimerRH = false;
	_frameNumberTimerRH = 0;

	// Sound and bag
	_presentationIndexRO = 0;
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
	debugC(kRingDebugLogic, "onMouseLeftButtonUp");

	error("[EventHandlerRing::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerRing::onMouseLeftButtonDown(Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonDown");

	if (_app->getBag()->getField94())
		return;

	// Handle menu
	Puzzle *puzzleMenu = _app->getPuzzle(kPuzzleMenu);
	if (puzzleMenu) {

		if (puzzleMenu->visualHandleLeftButtonDown(evt.mouse)) {
			_app->update(evt.mouse);
			return;
		}

		Accessibility *accessibility = puzzleMenu->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, puzzleMenu->getAccessibilityIndex(evt.mouse), puzzleMenu->getId(), evt.mouse))
			return;

		if (puzzleMenu->getField24() == 2)
			return;
	}

	// Handle current puzzle
	Puzzle *currentPuzzle = _app->getCurrentPuzzle();
	if (currentPuzzle) {
		if (currentPuzzle->visualHandleLeftButtonDown(evt.mouse)) {
			_app->update(evt.mouse);
			return;
		}

		Accessibility *accessibility = currentPuzzle->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, currentPuzzle->getAccessibilityIndex(evt.mouse), currentPuzzle->getId(), evt.mouse))
			return;
	}

	// Handle current rotation
	Rotation *currentRotation = _app->getCurrentRotation();
	if (currentRotation) {
		Accessibility *accessibility = currentRotation->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, currentRotation->getAccessibilityIndex(evt.mouse), currentRotation->getId(), evt.mouse))
			return;
	}
}

bool EventHandlerRing::handleLeftButtonDown(Accessibility *accessibility, uint32 index, Id id, const Common::Point &point) {
	if (!accessibility)
		return false;

	Object *object = accessibility->getObject();
	Hotspot *hotspot = accessibility->getHotspot();

	if (object->getFieldC() & 2) {
		onButtonDown(object->getId(), hotspot->getField19(), id, 1, point);

		if (_app->getState() == kStateShowMenu)
			return true;
	}

	if (!(object->getFieldC() & 4)) {
		_app->update(point);
		return true;
	}

	_app->getDragControl()->init(point, object->getId(), index, hotspot, hotspot->getField19(), id, 1);
	onBag(object->getId(), hotspot->getField19(), id, 1, _app->getDragControl(), 1);

	if (_app->getState() != kStateShowMenu) {
		if (!_app->getField76())
			_app->getDragControl()->reset();

		_app->setField76(1);
		_app->update(point);
	}

	return true;
}

void EventHandlerRing::onMouseRightButtonUp(Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseRightButtonUp");

	if (getApp()->getDragControl()->getField20() || getApp()->getField6F())
		return;

	Puzzle *puzzleMenu = getApp()->getPuzzle(kPuzzleMenu);
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
	debugC(kRingDebugLogic, "onKeyDown");

	// Stop dialogs if needed
	if (evt.kbd.keycode == Common::KEYCODE_ESCAPE)
		_app->soundStopType(kSoundTypeDialog, 4098);

	// Check menu and simulate button click
	Puzzle *puzzleMenu = getApp()->getPuzzle(kPuzzleMenu);
	if (puzzleMenu) {
		if (puzzleMenu->visualHandleKey(evt.kbd.keycode))
			return;

		if (puzzleMenu->getField24() == 2) {
			Accessibility *accessibility = puzzleMenu->getAccessibility(evt.kbd.keycode);
			if (!accessibility)
				return;

			// Center rotation
			if (_app->hasCurrentRotation())
				_app->getCurrentRotation()->setCoordinates(accessibility->getHotspot()->getCenter());

			bool controlPressed = (evt.kbd.flags & Common::KBD_CTRL);

			// Modify event to be a mouse click on the center of the hotspot
			Hotspot *hotspot = accessibility->getHotspot();
			evt.mouse = hotspot->getCenter();

			// Simulate click
			if (_app->getBag()->getField94()) {
				onMouseLeftButtonUp(evt, controlPressed);
				return;
			}

			onMouseLeftButtonUp(evt, controlPressed);

			if (evt.mouse.y >= 16)
				return;

			// FIXME: Ring calls a method that doesn't do anything useful (get the menu puzzle if the bag has a clicked object)
			return;
		}
	}

	// Handle events on zone
	onKeyDownZone(evt.kbd);

	// Check current puzzle and simulate button click
	Puzzle *currentPuzzle = _app->getCurrentPuzzle();
	if (currentPuzzle) {
		if (currentPuzzle->visualHandleKey(evt.kbd.keycode))
			return;

		if (currentPuzzle->getField24() == 2) {
			Accessibility *accessibility = puzzleMenu->getAccessibility(evt.kbd.keycode);
			if (!accessibility)
				return;

			bool controlPressed = (evt.kbd.flags & Common::KBD_CTRL);

			// Modify event to be a mouse click on the center of the hotspot
			Hotspot *hotspot = accessibility->getHotspot();
			evt.mouse = hotspot->getCenter();

			// Simulate click
			if (_app->getBag()->getField94()) {
				onMouseLeftButtonUp(evt, controlPressed);
				return;
			}

			onMouseLeftButtonUp(evt, controlPressed);

			if (evt.mouse.y >= 16)
				return;

			// FIXME: Ring calls a method that doesn't do anything useful (get the menu puzzle if the bag has a clicked object)
			return;
		}
	}

	// Check current rotation and simulate button click
	Rotation *currentRotation = _app->getCurrentRotation();
	if (currentRotation) {
		Accessibility *accessibility = currentRotation->getAccessibility(evt.kbd.keycode);
		if (!currentRotation)
			return;

		bool controlPressed = (evt.kbd.flags & Common::KBD_CTRL);

		// Modify event to be a mouse click on the center of the hotspot
		Hotspot *hotspot = accessibility->getHotspot();
		evt.mouse = hotspot->getCenter();

		// Center rotation
		currentRotation->setCoordinates(evt.mouse);

		// Simulate click
		if (_app->getBag()->getField94()) {
			onMouseLeftButtonUp(evt, controlPressed);
			return;
		}

		onMouseLeftButtonUp(evt, controlPressed);

		if (evt.mouse.y >= 16)
			return;

		// FIXME: Ring calls a method that doesn't do anything useful (get the menu puzzle if the bag has a clicked object)
	}
}

#pragma endregion

#pragma region Left Button Down

void EventHandlerRing::onButtonDown(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
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
		onButtonDownZoneNI(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		onButtonDownZoneFO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		onButtonDownZoneRO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		onButtonDownZoneN2(id, a2, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerRing::onButtonDownZoneNI(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerRing::onButtonDownZoneNI] Not implemented");
}

void EventHandlerRing::onButtonDownZoneFO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerRing::onButtonDownZoneFO] Not implemented");
}

void EventHandlerRing::onButtonDownZoneRO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerRing::onButtonDownZoneRO] Not implemented");
}

void EventHandlerRing::onButtonDownZoneN2(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventHandlerRing::onButtonDownZoneN2] Not implemented");
}

#pragma endregion

#pragma region Keydown

void EventHandlerRing::onKeyDownZone(Common::KeyState keyState) {
	if (_app->getCurrentZone() == kZoneSY)
		onKeyDownZoneSY(keyState);
}

void EventHandlerRing::onKeyDownZoneSY(Common::KeyState keyState) {
	if (!_app->hasCurrentPuzzle())
		return;

	PuzzleId puzzleId = _app->getCurrentPuzzleId();
	switch (puzzleId) {
	default:
		error("[EventHandlerRing::onKeyDownZoneSY] Invalid puzzle Id (%d)", puzzleId.id());

	case kPuzzleLoad:
		if (keyState.keycode == Common::KEYCODE_DELETE) {
			_app->messageGet("DoYouWantToDeleteSavedGame");
			_app->messageShowQuestion(4);
		}
		break;

	case kPuzzleSave:
		switch (keyState.keycode) {
		default:
			if (_app->objectPresentationGetTextWidth(kObjectSaveName, 0, 0) >= 280)
				return;

			*_app->getSaveManager()->getName() += (char)keyState.ascii;
			break;

		case Common::KEYCODE_ESCAPE:
			_app->getSaveManager()->getName()->clear();
			break;

		case Common::KEYCODE_BACKSPACE:
			if (_app->getSaveManager()->getName()->size() != 1)
				_app->getSaveManager()->getName()->deleteLastChar();
			break;

		case Common::KEYCODE_RETURN:
			// Do nothing
			return;
		}

		_app->objectPresentationSetTextToPuzzle(kObjectSaveName, 0, 0, *_app->getSaveManager()->getName());
		_app->objectPresentationSetTextCoordinatesToPuzzle(kObjectSaveName, 0, 0, Common::Point(344, 181));
		_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSaveName, 0, Common::Point(346 + _app->objectPresentationGetTextWidth(kObjectSaveName, 0, 0), 181));
		break;
	}
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerRing::onSetup(Zone zone, SetupType type) {
	debugC(kRingDebugLogic, "onSetup (zone: %s, type: %d)", _app->getZoneString(zone).c_str(), type);

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
		_app->soundPlay(14001, kSoundLoop);
		_app->playMovie("1541");
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10001);
		break;

	case kSetupType3:
		_app->timerStopAll();
		_app->puzzleSetMovabilityOn(kPuzzle10410, 0, 0);
		_app->playMovie("1550");
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		_app->soundPlay(14001, kSoundLoop);
		_app->soundPlay(10021);
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
		_app->soundPlay(10409, kSoundLoop);
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
		_app->soundPlay(23005, kSoundLoop);
		_app->playMovie("1706");
		_app->puzzleSetActive(kPuzzle22001);
		_app->soundPlay(22001);
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
		_app->soundPlay(40700);
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
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1162", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1161", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1160", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle80001);
		_app->soundPlay(80107);
		break;

	case kSetupType998:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1164", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1163", _app->getLanguageChannel());
			break;
		}

		_app->playMovie("1666");
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80100);
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
		_app->soundPlay(72001);
		break;

	case kSetupType10:
		onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->bagAdd(kObjectFire);
		_app->soundPlay(70000, kSoundLoop);
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
	debugC(kRingDebugLogic, "onTimer (id: %d)", timerId);

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
	switch (id) {
	default:
		break;

	case kTimer0: {
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10432) < 70) {
			_app->objectPresentationHide(kObject10431);
			_app->objectPresentationShow(kObject10431, (_app->varGetByte(10432) - 10) / 5);
		} else if (_app->varGetByte(10432) == 100) {
			_app->objectPresentationShow(kObject10431, 12);
		}

		uint32 mod = (120 - _app->varGetByte(10432)) / 10;
		if (mod < 5)
			mod = 5;

		if (!(_app->varGetByte(10432) % mod)) {
			_app->soundPlay(10415);
			_app->objectPresentationShow(kObject10432, rnd(2) + 1);
		}

		uint32 volume = _app->varGetByte(10432) / 5 + 80;
		if (volume > 100)
			volume = 100;

		_app->soundSetVolume(10412, volume);

		if (_app->varGetByte(10412) > 120) {
			_app->timerStop(kTimer0);
			_app->soundStopType(kSoundTypeAmbientEffect, 1024);
			_app->soundStopType(kSoundTypeBackgroundMusic, 1024);
			_app->playMovie("1538");
			_app->exitToMenu(4);
		}
		}
		break;

	case kTimer1:
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10303) == 1) {
			_app->timerStopAll();
			_app->soundStopAll(1024);
			_app->varSetFloat(90005, 100.0f);
			_app->playMovie("1539");
			onSwitchZoneAS(1);
		}
		break;
	}
}

void EventHandlerRing::onTimerZoneRH(TimerId id) {
	if (id != kTimer0)
		return;

	// FIXME: Since we are called synchronously, is this needed? If so, replace by mutex?
	if (_disableTimerRH)
		return;

	_disableTimerRH = true;

	float alp = _app->rotationGetAlp(20401) - 35.0;
	if (alp > 0.0f && alp < 146.0f) {
		uint32 frame = (int)(alp * 0.2631578947368421f);

		if (frame >= 1 && frame <= 28 && _frameNumberTimerRH != frame) {
			_app->objectPresentationSetAnimationActiveFrame(20401, 0, frame);
			_frameNumberTimerRH = frame;
		}
	}

	_disableTimerRH = false;
}

void EventHandlerRing::onTimerZoneFO(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		switch (_app->varGetByte(30073)) {
		default:
			break;

		case 0:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 1);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 1:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 2);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 10);
			break;

		case 2:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 3);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 3:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 0);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 1000 * (rnd(10) + 15));
			break;
		}
		break;

	case kTimer1: {
		Id soundId = rnd(15) + 30301;
		_app->soundSetPan(soundId, 10 - rnd(20));
		_app->soundPlay(soundId);

		_app->timerStop(kTimer1);
		_app->timerStart(kTimer1, 500 * (rnd(10) + 10));
		}
		break;

	case kTimer2:
		_app->objectPresentationUnpauseAnimation(kObjectSleepingPotion, 0);
		_app->timerStop(kTimer2);
		break;

	case kTimer3: {
		Id soundId = rnd(3) + 30316;
		_app->soundSetPan(soundId, 95);
		_app->soundPlay(soundId);

		_app->timerStop(kTimer3);
		_app->timerStart(kTimer3, 500 * (rnd(10) + 30));
		}
		break;

	case kTimer4: {
		Id soundId = rnd(9) + 30319;
		_app->soundSetPan(soundId, 5 - rnd(10));
		_app->soundPlay(soundId);

		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, 4000 * (rnd(10) + 5));
		}
		break;

	case kTimer5:
		_app->objectPresentationUnpauseAnimation(kObject30110, 1);
		_app->objectPresentationUnpauseAnimation(kObject30109, 0);
		_app->objectSetAccessibilityOff(kObject30109, 0, 0);
		_app->timerStop(kTimer5);
		_app->timerStart(kTimer5, 2000 * rnd(10) + 15000);
		break;
	}
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
}

void EventHandlerRing::onTimerZoneAS(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer2:
		_app->soundPlay(80018);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 20);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer3:
		_app->soundPlay(80019);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 30);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer4:
		_app->soundPlay(80020);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 10);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer5:
		_app->varSetByte(80001, _app->varGetByte(80001) + 1);

		_app->getCurrentRotation()->setBet(_app->getCurrentRotation()->getBet() + _app->varGetFloat(80001) * _app->varGetFloat(80002));
		_app->getCurrentRotation()->setAlp(_app->getCurrentRotation()->getAlp() + _app->varGetFloat(80001) * _app->varGetFloat(80002) * 0.5f);

		// Update values
		_app->varSetFloat(80001, _app->varGetFloat(80001) * -1.0f);
		_app->varSetFloat(80002, _app->varGetFloat(80002) * 5.0f / 6.0f);

		if (_app->varGetByte(80001) > 50) {
			_app->varSetByte(80001, 0);
			_app->varSetFloat(80002, 2.0f);

			_app->timerStop(kTimer5);
			_app->timerStop(kTimer6);

			_app->objectPresentationHide(kObject80016);
		}
		break;

	case kTimer6: {
		int32 val = rnd(10) & 0x80000001;

		bool show = (val == 0);
		if (val < 0)
			show = (((val - 1) | -2) == -1);

		if (show)
			_app->objectPresentationShow(kObject80016);
		else
			_app->objectPresentationHide(kObject80016);
		}
		break;
	}
}

void EventHandlerRing::onTimerZoneN2(TimerId id) {
	if (id == kTimer0)
		_app->soundPlay(rnd(12) + 70004);
}

#pragma endregion

#pragma region Bag

void EventHandlerRing::onBag(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

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
				_app->soundPlay(rnd(9) + 13001);
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
				_app->soundPlay(10501);

		if (movabilityFrom == 10501 || movabilityFrom == 10511 || movabilityFrom == 10521)
			if (!movabilityIndex)
				_app->soundPlay(10502);

		if (movabilityFrom == 10001) {
			_app->soundPlay(10804);
		} else if (movabilityFrom == 10002) {
			_app->soundPlay(10804);
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
		_app->soundPlay(40102, kSoundLoop);

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
				_app->soundPlay(51006, kSoundLoop);
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51007, kSoundLoop);
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
			_app->soundPlay(70501);

		if (movabilityFrom == 70501 || movabilityFrom == 70511 || movabilityFrom == 70521)
			_app->soundPlay(70502);
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
			_app->soundPlay(10902);

		_app->soundStop(10901, 1024);

		if (movabilityFrom == 10301) {
			_app->soundPlay(10300, kSoundLoop);
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
				_app->soundPlay(14002, kSoundLoop);
			}
		}

		if (movabilityFrom > 10201 || movabilityFrom == 10101) {
			if (!_app->soundIsPlaying(14003)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14001, 1024);
				_app->soundPlay(14003, kSoundLoop);
			}
		}

		if (movabilityFrom >= 10000 && movabilityFrom <= 10005) {
			if(!_app->soundIsPlaying(14001)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14003, 1024);
				_app->soundPlay(14001, kSoundLoop);
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
				_app->soundPlay(10410, kSoundLoop);
				_app->soundPlay(10411, kSoundLoop);
			}
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (a4 == 55) {
			_app->playMovie("1537", 0.0);
			_app->puzzleSetActive(kPuzzle12001, 1, 1);
			_app->soundPlay(12001);
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
					_app->soundPlay(30117);
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
			_app->soundPlay(40003, kSoundLoop);
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
			_app->soundPlay(51002, kSoundLoop);
			_app->soundPlay(51012, kSoundLoop);
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
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50301:
		if (movabilityTo == 50104) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51004, kSoundLoop);

			if (_app->varGetByte(50003) > 0)
				_app->soundPlay(51013, kSoundLoop);
		}
		break;

	case 50104:
		if (movabilityTo == 50301) {
			_app->soundStop(51004, 1024);
			_app->soundStop(51013, 1024);
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50106:
		if (movabilityTo == 50501) {
			_app->soundStop(51001, 1024);
			_app->soundStop(51011, 1024);
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51007, 1024);
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50108:
		if (movabilityTo == 50601)
			_app->soundPlay(51006, kSoundLoop);
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
	debugC(kRingDebugLogic, "onSound (id: %d, type: %d)", id, type);

	bool process = (a3 & 0x1000) != 0;
	a3 &= 239;

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onSoundZoneSY(id, type, a3, process);
		break;

	case kZoneNI:
		onSoundZoneNI(id, type, a3, process);
		break;

	case kZoneRH:
		onSoundZoneRH(id, type, a3, process);
		break;

	case kZoneFO:
		onSoundZoneFO(id, type, a3, process);
		break;

	case kZoneRO:
		onSoundZoneRO(id, type, a3, process);
		break;

	case kZoneAS:
		onSoundZoneAS(id, type, a3, process);
		break;

	case kZoneWA:
		onSoundZoneWA(id, type, a3, process);
		break;

	case kZoneN2:
		onSoundZoneN2(id, type, a3, process);
		break;
	}
}

void EventHandlerRing::onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	if (id == 90001) {
		_app->objectPresentationHideAndRemove(kObject7, 0);
		_app->showCredits();
		_app->startMenu(false);
	}
}

void EventHandlerRing::onSoundZoneNI(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 10001:
		_app->objectPresentationShow(kObject10300, 2);
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10002);
		return;

	case 10002:
		_app->bagAdd(kObjectGlug);
		_app->bagAdd(kObjectMinerals);
		_app->playMovie("1543");
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10003);
		return;

	case 10003:
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10004);
		return;

	case 10004:
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		break;

	case 10005:
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10006);
		return;

	case 10006:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10007);
		return;

	case 10007:
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10008);
		return;

	case 10008:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10009);
		return;

	case 10009:
		_app->playMovie("1544");
		_app->bagAdd(kObjectLogeTear);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
		_app->objectPresentationHide(kObjectLogeTear);
		_app->objectPresentationShow(kObjectLogeTear, 0);
		_app->objectSetAccessibilityOff(10305);
		_app->rotationSetActive(10301);
		break;

	case 10010:
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10011);
		return;

	case 10011:
		_app->playMovie("1545");
		_app->bagAdd(kObjectMagicFrog);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
		_app->objectSetAccessibilityOff(kObjectMagicFrog);
		_app->varSetByte(10300, 1);

		if (_app->bagHas(kObjectDivingHelmet)) {
			_app->bagRemove(kObjectMagicFrog);
			_app->bagRemove(kObjectDivingHelmet);
			_app->bagAdd(kObjectDivingHelmet2);
			_app->playMovie("1522");
		}

		_app->rotationSetActive(10301);
		break;

	case 10012:
		_app->playMovie("1546");
		_app->varSetByte(10301, 1);
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10013);
		return;

	case 10014:
		_app->playMovie("1547");
		_app->bagAdd(kObjectTile);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
		_app->objectPresentationShow(kObjectTile, 0);
		_app->objectSetAccessibilityOff(10303);
		_app->rotationSetActive(10301);
		break;

	case 10013:
	case 10018:
	case 10019:
	case 10020:
	case 10021:
		_app->rotationSetActive(10301);
		break;

	case 10015:
	case 10016:
	case 10017:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(rnd(3) + 10018);
		break;

	case 10022:
		_app->puzzleSetActive(kPuzzle10202);
		_app->soundPlay(10023);
		break;

	case 10023:
		_app->puzzleSetActive(kPuzzle10200);
		_app->soundPlay(10024);
		break;

	case 10024:
		_app->puzzleSetActive(kPuzzle10202);
		_app->soundPlay(10025);
		break;

	case 10025:
		_app->puzzleSetActive(kPuzzle10200);
		_app->soundPlay(10026);
		break;

	case 10026:
		_app->objectSetAccessibilityOn(kObject10201, 0, 0);
		break;

	case 10027:
		_app->puzzleSetActive(kPuzzle10205);
		_app->soundPlay(10028);
		break;

	case 10028:
		_app->puzzleSetActive(kPuzzle10203);
		_app->soundPlay(10029);
		break;

	case 10029:
		_app->puzzleSetActive(kPuzzle10204);
		_app->soundPlay(10030);
		break;

	case 10030:
		_app->puzzleSetActive(kPuzzle10203);
		_app->soundPlay(10031);
		break;

	case 10031:
		_app->puzzleSetActive(kPuzzle10204);
		_app->soundPlay(10032);
		break;

	case 10032:
		_app->playMovie("1548");
		_app->playMovie("1549");
		_app->objectSetAccessibilityOn(kObject10200);
		_app->rotationSetActive(10201);
		_app->rotationSetMovabilityOff(10201, 0, 0);
		break;

	case 12001:
		_app->puzzleSetActive(kPuzzle12003);
		_app->soundPlay(12003);
		break;

	case 12002:
		_app->playMovie("1542");
		_app->puzzleSetActive(kPuzzle10521);
		break;

	case 12003:
		_app->puzzleSetActive(kPuzzle12002);
		_app->soundPlay(12002);
		break;
	}
}

void EventHandlerRing::onSoundZoneRH(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 20011:
	case 20012:
	case 20013:
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 2);
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 4);
		_app->objectPresentationSetAnimationOnPuzzle((ObjectId)(_app->varGetByte(21001) + 20001), 2, 0, 20002);
		_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20001), 3);
		_app->objectSetAccessibilityOn((ObjectId)(_app->varGetByte(21001) + 20001), 0, 1);
		break;

	case 20021:
	case 20022:
	case 20023:
		_app->playMovie(Common::String::format("rh_%d", _app->varGetByte(21001) + 1));
		_app->playMovie("1696");
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->rotationSetMovabilityOff(10 * (_app->varGetByte(21001) + 2001), 0, 0);
		_app->rotationSetMovabilityOn(10 * (_app->varGetByte(21001) + 2001), 1, 1);
		_app->rotationSetActive(10 * (_app->varGetByte(21001) + 2001));
		break;

	case 20031:
	case 20032:
	case 20033:
		_app->playMovie(Common::String::format("rh_%d_l0", _app->varGetByte(21001) + 1));
		_app->objectSetAccessibilityOff((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->objectSetAccessibilityOn((ObjectId)(_app->varGetByte(21001) + 20004));
		_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20004), 0);
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->puzzleSetActive((PuzzleId)(10 * _app->varGetByte(21001) + 20011));
		break;

	case 20201:
		_app->playMovie("1698");
		_app->bagAdd(kObjectMedallion);
		_app->soundPlay(23009);
		_app->objectSetAccessibilityOff(kObjectDolphin);
		_app->rotationSetActive(20201);
		_app->rotationSetMovabilityOn(20201, 0, 0);
		break;

	case 20202:
		_app->playMovie("1699");
		_app->objectPresentationShow(kObject21003, 0);
		_app->objectPresentationShow(kObject20204, 0);
		_app->rotationSetMovabilityOff(20202, 2, 2);
		_app->rotationSetMovabilityOff(20202, 0, 0);
		_app->rotationSetMovabilityOff(20203, 0, 0);
		_app->rotationSetMovabilityOn(20202, 1, 1);
		_app->rotationSetMovabilityOn(20203, 1, 3);
		_app->rotationSetActive(20202);
		break;

	case 20301:
		_app->objectPresentationHide(kObject20301, 1);
		break;

	case 20302:
		_app->objectPresentationHide(kObject20302, 1);
		_app->bagAdd(kObjectMedallion);
		_app->playMovie("1700");
		_app->rotationSetBet(20302, 0.3f);
		_app->rotationSetActive(20302);
		break;

	case 20303:
		_app->soundStop(23010, 1024);
		_app->playMovie("1687");
		_app->soundPlay(23009);
		_app->rotationSetAlp(20303, 325.0);
		_app->rotationSetBet(20303, 0.3f);
		_app->rotationSetActive(20303);
		break;

	case 20304:
		_app->playMovie("1701");
		_app->objectPresentationShow(kObject20304, 0);
		_app->objectPresentationShow(kObject21003, 1);
		_app->rotationSetMovabilityOff(20304, 2, 2);
		_app->rotationSetMovabilityOff(20304, 0, 0);
		_app->rotationSetMovabilityOff(20305, 0, 0);
		_app->rotationSetMovabilityOn(20304, 1, 1);
		_app->rotationSetMovabilityOn(20305, 1, 2);
		_app->rotationSetActive(20304);
		break;

	case 20401:
		_app->playMovie("1674");
		_app->bagAdd(kObjectAntiGCells2);
		_app->soundPlay(23009);
		_app->objectPresentationHide(kObject20402);
		_app->playMovie("1702");
		_app->rotationSetAlp(20402, 0.0);
		_app->rotationSetActive(20402);
		break;

	case 20402:
		_app->playMovie("1703");
		_app->objectPresentationShow(kObject20404, 0);
		_app->objectPresentationShow(kObject21003, 2);
		_app->rotationSetMovabilityOff(20402, 0, 0);
		_app->rotationSetMovabilityOff(20403, 0, 0);
		_app->rotationSetMovabilityOn(20402, 1, 1);
		_app->rotationSetMovabilityOn(20403, 1, 2);
		_app->rotationSetActive(20402);
		break;

	case 20504:
		_app->playMovie("1705");
		_app->objectPresentationShow(kObject20502, 0);
		_app->rotationSetMovabilityOff(20503, 0, 0);
		_app->rotationSetMovabilityOff(20504, 0, 0);
		_app->rotationSetMovabilityOn(20504, 1, 2);
		_app->rotationSetActive(20503);
		break;

	case 20501:
		_app->puzzleSetActive(kPuzzle20502);
		_app->soundPlay(20502);
		break;

	case 20502:
		_app->objectPresentationHide(kObject20501, 2);
		_app->objectSetAccessibilityOn(kObject20501, 3, 4);
		break;

	case 20503:
		_app->objectPresentationHide(kObject20501, 3);
		_app->playMovie("1704");
		_app->rotationSetAlp(20501, 0.0);
		_app->rotationSetActive(20501);
		break;

	case 22001:
		_app->puzzleSetActive(kPuzzle22003);
		_app->soundPlay(22003);
		break;

	case 22002:
		_app->playMovie("1697");
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->rotationSetAlp(20010, 1.5);
		_app->rotationSetBet(20010, -4.3f);
		_app->rotationSetRan(20010, 79.3f);
		_app->rotationSetActive(20010);
		break;

	case 22003:
		_app->puzzleSetActive(kPuzzle22002);
		_app->soundPlay(22002);
		break;
	}
}

void EventHandlerRing::onSoundZoneFO(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 30100:
		_app->soundPlay(30101);
		break;

	case 30101:
		_app->playMovie("1179", 0.0);
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);

		if (!_app->varGetByte(30074))
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);

		_app->varSetByte(30074, 1);
		break;

	case 30102:
		_app->puzzleSetActive(kPuzzle35103);
		_app->soundPlay(30103);
		break;

	case 30103:
		_app->puzzleSetActive(kPuzzle35104);
		_app->soundPlay(30104);
		break;

	case 30104:
		_app->playMovie("1208");
		_app->puzzleSetActive(kPuzzle35105);
		_app->soundPlay(30106);
		break;

	case 30106:
		_app->playMovie("1209");
		_app->puzzleSetActive(kPuzzle35110);
		_app->soundPlay(30107);
		break;

	case 30107:
		_app->playMovie("1210");
		_app->puzzleSetActive(kPuzzle35106);
		_app->soundPlay(30108);
		break;

	case 30108:
		_app->playMovie("1211");
		_app->puzzleSetActive(kPuzzle35107);
		_app->soundPlay(30161);
		break;

	case 30109:
		_app->playMovie("1213");
		_app->puzzleSetActive(kPuzzle35111);
		_app->objectPresentationShow(kObject30109, 0);
		break;

	case 30118:
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);
		_app->varSetByte(30078, 1);
		break;

	case 30105:
		_app->playMovie("1214");
		_app->bagAdd(kObjectWolfBrooch);
		_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
		_app->objectPresentationShow(kObject30110, 3);
		_app->objectSetAccessibilityOff(kObject30102, 1, 1);
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);
		break;

	case 30120:
		_app->objectPresentationHideAndRemove(kObject6, 0);
		_app->rotationSetActive(30002);
		_app->soundPlay(30121);
		_app->objectPresentationShow(kObject6, 1);
		break;

	case 30121:
		_app->objectPresentationHideAndRemove(kObject6, 1);
		_app->objectPresentationHideAndRemove(kObject6, 5);
		_app->rotationSetActive(30003);
		_app->soundPlay(30122);
		_app->objectPresentationShow(kObject6, 2);
		_app->objectPresentationShow(kObject6, 6);
		break;

	case 30122:
		_app->objectPresentationHideAndRemove(kObject6, 2);
		_app->rotationSetActive(30004);
		_app->soundPlay(30123);
		_app->objectPresentationShow(kObject6, 3);
		break;

	case 30123:
		_app->objectPresentationHideAndRemove(kObject6, 3);
		_app->objectPresentationHideAndRemove(kObject6, 6);
		_app->rotationSetActive(30005);
		_app->soundPlay(30124);
		_app->objectPresentationShow(kObject6, 4);
		_app->objectPresentationShow(kObject6, 7);
		break;

	case 30124:
		_app->objectPresentationHideAndRemove(kObject6, 4);
		_app->objectPresentationHideAndRemove(kObject6, 7);
		_app->rotationSetActive(30006);
		_app->soundPlay(30125);
		_app->objectPresentationShow(kObject6, 8);
		_app->objectPresentationShow(kObject6, 9);
		break;

	case 30125:
		_app->objectPresentationHideAndRemove(kObject6, 9);
		_app->rotationSetActive(30008);
		_app->soundPlay(30126);
		_app->objectPresentationShow(kObject6, 10);
		break;

	case 30126:
		_app->objectPresentationHideAndRemove(kObject6, 8);
		_app->objectPresentationHideAndRemove(kObject6, 10);
		_app->rotationSetActive(30009);
		_app->soundPlay(30127);
		_app->objectPresentationShow(kObject6, 11);
		_app->objectPresentationShow(kObject6, 15);
		break;

	case 30127:
		_app->objectPresentationHideAndRemove(kObject6, 11);
		_app->rotationSetActive(30010);
		_app->soundPlay(30128);
		_app->objectPresentationShow(kObject6, 12);
		break;

	case 30128:
		_app->objectPresentationHideAndRemove(kObject6, 12);
		_app->objectPresentationHideAndRemove(kObject6, 15);
		_app->rotationSetActive(30011);
		_app->soundPlay(30129);
		_app->objectPresentationShow(kObject6, 13);
		_app->objectPresentationShow(kObject6, 16);
		break;

	case 30129:
		_app->objectPresentationHideAndRemove(kObject6, 13);
		_app->rotationSetActive(30012);
		_app->soundPlay(30130);
		_app->objectPresentationShow(kObject6, 14);
		break;

	case 30130:
		_app->objectPresentationHideAndRemove(kObject6, 14);
		_app->objectPresentationHideAndRemove(kObject6, 16);
		_app->rotationSetActive(30701);
		_app->soundPlay(30131);
		_app->objectPresentationShow(kObject6, 17);
		_app->objectPresentationShow(kObject6, 18);
		break;

	case 30131:
		_app->objectPresentationHideAndRemove(kObject6, 18);
		_app->rotationSetActive(30702);
		_app->soundPlay(30132);
		_app->objectPresentationShow(kObject6, 19);
		break;

	case 30132:
		_app->objectPresentationHideAndRemove(kObject6, 17);
		_app->objectPresentationHideAndRemove(kObject6, 19);
		_app->rotationSetActive(30703);
		_app->soundPlay(30133);
		_app->objectPresentationShow(kObject6, 20);
		_app->objectPresentationShow(kObject6, 24);
		break;

	case 30133:
		_app->objectPresentationHideAndRemove(kObject6, 20);
		_app->objectPresentationHideAndRemove(kObject6, 24);
		_app->rotationSetActive(30704);
		_app->soundPlay(30134);
		_app->objectPresentationShow(kObject6, 21);
		_app->objectPresentationShow(kObject6, 25);
		break;

	case 30134:
		_app->objectPresentationHideAndRemove(kObject6, 21);
		_app->rotationSetActive(30401);
		_app->soundPlay(30135);
		_app->objectPresentationShow(kObject6, 22);
		break;

	case 30135:
		_app->objectPresentationHideAndRemove(kObject6, 22);
		_app->objectPresentationHideAndRemove(kObject6, 25);
		_app->rotationSetActive(30402);
		_app->soundPlay(30136);
		_app->objectPresentationShow(kObject6, 23);
		_app->objectPresentationShow(kObject6, 26);
		break;

	case 30136:
		_app->objectPresentationHideAndRemove(kObject6, 23);
		_app->objectPresentationHideAndRemove(kObject6, 26);
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->playMovie("1215", 0.0);
		onSwitchZoneAS(3);
		break;

	case 30161:
		_app->playMovie("1212", 0.0);
		_app->puzzleSetActive(kPuzzle35108);
		_app->soundPlay(30109);
		break;

	case 30506:
		if (_app->varGetByte(30043) == 1 && _app->bagHas(kObjectSleepingPotion2)) {
			_app->playMovie("1216");
			_app->objectPresentationShow(kObject30100, 0);
			_app->objectSetAccessibilityOff(kObject30100, 0, 0);
			_app->objectSetAccessibilityOn(kObject30100, 1, 1);
		} else {
			_app->objectSetAccessibilityOn(kObject30100, 0, 0);
			_app->puzzleSetMovabilityOn(kPuzzle35100, 0, 0);
		}
		break;
	}
}

void EventHandlerRing::onSoundZoneRO(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 40400:
		_app->playMovie("1787");
		_app->objectPresentationShow(kObject40202, 0);
		_app->objectSetAccessibilityOn(kObject40202);
		break;

	case 40603:
		_app->soundStop(40003, 1024);
		_app->timerStopAll();
		_app->playMovie("1786");
		onSwitchZoneAS(2);
		break;

	case 40700:
		_app->puzzleSetActive(kPuzzle40101);
		_app->soundPlay(40701);
		break;

	case 40701:
		_app->rotationSetActive(40000);
		break;

	case 40702:
		_app->puzzleSetActive(kPuzzle40102);
		_app->soundPlay(40703);
		break;

	case 40703:
		_app->puzzleSetActive(kPuzzle40103);
		_app->soundPlay(40704);
		break;

	case 40704:
		_app->soundStopAll(1024);
		_app->playMovie("1788");
		_presentationIndexRO = 0;
		_app->rotationSet3DSoundOff(40000, 40002);
		_app->rotationSet3DSoundOff(40001, 40002);
		_app->rotationSet3DSoundOff(40002, 40002);
		_app->rotationSet3DSoundOff(40003, 40002);
		_app->rotationSet3DSoundOff(40004, 40002);
		_app->rotationSetAmbientSoundOff(40000, 40001);
		_app->rotationSetAmbientSoundOff(40001, 40001);
		_app->rotationSetAmbientSoundOff(40002, 40001);
		_app->rotationSetAmbientSoundOff(40003, 40001);
		_app->rotationSetAmbientSoundOff(40004, 40001);
		_app->rotationSetAmbientSoundOff(40005, 40001);
		_app->rotationSetAmbientSoundOn(40000, 40604);
		_app->rotationSetAmbientSoundOn(40001, 40604);
		_app->rotationSetAmbientSoundOn(40002, 40604);
		_app->rotationSetAmbientSoundOn(40003, 40604);
		_app->rotationSetAmbientSoundOn(40004, 40604);
		_app->rotationSetAmbientSoundOn(40005, 40604);
		_app->puzzleSetAmbientSoundOn(kPuzzle40060, 40604);
		_app->objectPresentationShow(kObject40203);
		_app->rotationSetMovabilityOff(40000, 1, 1);
		_app->rotationSetMovabilityOn(40004, 0, 0);
		_app->rotationSetActive(40000);
		_app->rotationSetMovabilityRideName(40000, 0, "ro0102");
		_app->rotationSetMovabilityRideName(40001, 2, "ro0201");
		_app->rotationSetMovabilityRideName(40001, 3, "ro0203");
		_app->rotationSetMovabilityRideName(40002, 0, "ro0302");
		_app->rotationSetMovabilityRideName(40003, 0, "ro0402");
		_app->rotationSetMovabilityRideName(40004, 0, "1796");
		_app->rotationSetMovabilityRideName(40004, 1, "ro0502");
		break;

	case 40706:
		_app->puzzleSetActive(kPuzzle40102);
		_app->soundPlay(40707);
		break;

	case 40707:
		_app->puzzleSetActive(kPuzzle40101);
		_app->soundPlay(40708);
		break;

	case 40708:
		_app->puzzleSetActive(kPuzzle40104);
		_app->soundPlay(40709);
		break;

	case 40709:
		_app->puzzleSetActive(kPuzzle40102);
		_app->soundPlay(40710);
		break;

	case 40710:
		_app->puzzleSetActive(kPuzzle40101);
		_app->soundPlay(40711);
		break;

	case 40711:
		_app->puzzleSetActive(kPuzzle40103);
		_app->soundPlay(40702);
		break;
	}
}

void EventHandlerRing::onSoundZoneWA(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 50021:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1875", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1873", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1874", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51002);
		_app->soundPlay(50022);
		break;

	case 50022:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1878", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1876", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1877", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51003);
		_app->soundPlay(51008, kSoundLoop);
		_app->soundPlay(50026);
		break;

	case 50026:
		_app->puzzleSetActive(kPuzzle51004);
		_app->soundPlay(50027);
		break;

	case 50027:
		_app->puzzleSetActive(kPuzzle51005);
		_app->soundPlay(50028);
		break;

	case 50028:
		_app->puzzleSetActive(kPuzzle51006);
		_app->soundPlay(50029);
		break;

	case 50029:
		_app->puzzleSetActive(kPuzzle51007);
		_app->soundPlay(50030);
		break;

	case 50030:
		_app->puzzleSetActive(kPuzzle51008);
		_app->soundPlay(50031);
		break;

	case 50031:
		_app->puzzleSetActive(kPuzzle51009);
		_app->soundPlay(50032);
		break;

	case 50032:
		_app->puzzleSetActive(kPuzzle51010);
		_app->soundPlay(50033);
		break;

	case 50033:
		_app->puzzleSetActive(kPuzzle51011);
		_app->soundPlay(50034);
		break;

	case 50034:
		_app->puzzleSetActive(kPuzzle51012);
		_app->soundPlay(50035);
		break;

	case 50035:
		_app->puzzleSetActive(kPuzzle51013);
		_app->soundPlay(50036);
		break;

	case 50036:
		_app->playMovie("1879");
		_app->timerStopAll();
		_app->soundStopAll(1024);
		onSwitchZoneAS(4);
		break;

	case 50001:
		_app->puzzleSetActive(kPuzzle50001);
		_app->soundPlay(50002);

		break;
	case 50002:
		_app->rotationSetActive(50001);
		_app->soundPlay(50003);
		break;

	case 50003:
		_app->soundPlay(51006, kSoundLoop);
		break;

	case 50009:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50010);
		break;

	case 50010:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50011);
		break;

	case 50011:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50012);
		break;

	case 50012:
		_app->puzzleSetActive(kPuzzle50701);
		_app->soundPlay(50013);
		break;

	case 50013:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50014);
		break;

	case 50014:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50015);
		break;

	case 50015:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50016);
		break;

	case 50016:
		_app->objectSetAccessibilityOff(kObject50700);
		_app->rotationSetActive(50701);
		break;
	}
}

void EventHandlerRing::onSoundZoneAS(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 80100:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80101);
		break;

	case 80101:
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80102);
		break;

	case 80102:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80103);
		break;

	case 80103:
		_app->playMovie("1157");
		_app->puzzleSetActive(kPuzzle80013);
		_app->soundPlay(80104);
		break;

	case 80104:
	case 80040:
	case 80041:
	case 80042:
	case 80043:
	case 80044:
	case 80045:
	case 80046:
	case 80047:
	case 80049:
	case 80050:
	case 80051:
	case 80052:
	case 80053:
	case 80054:
	case 80055:
	case 80056:
	case 80058:
	case 80059:
	case 80060:
	case 80061:
	case 80062:
	case 80063:
	case 80064:
	case 80065:
	case 80066:
	case 80068:
	case 80069:
	case 80070:
	case 80071:
	case 80072:
	case 80073:
	case 80074:
	case 80075:
	case 80076:
	case 80077:
	case 80078:
	case 80079:
		_app->soundPlay(id + 1);
		break;

	case 80105:
		_app->playMovie("1158");
		_app->puzzleSetActive(kPuzzle80014);
		_app->soundPlay(80106);
		break;

	case 80106:
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		_app->rotationSetAlp(80001, 270.0f);
		_app->rotationSetBet(80001, -26.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		break;

	case 80048:
		_app->soundSetVolume(80201, 100);
		_app->varSetByte(80002, 2);
		break;

	case 80057:
		_app->soundSetVolume(80203, 100);
		_app->varSetByte(80002, 3);
		break;

	case 80067:
		_app->soundSetVolume(80204, 100);
		_app->varSetByte(80002, 4);
		break;

	case 80080:
		_app->soundSetVolume(80205, 100);
		_app->varSetByte(80002, 5);
		break;
	case 80107:
		_app->playMovie("1159");
		_app->showMenu(kZoneAS, 0);
		break;
	}
}

void EventHandlerRing::onSoundZoneN2(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id){
	default:
		break;

	case 70001:
		_app->puzzleSetActive(kPuzzle70306);
		_app->soundPlay(70002);
		break;

	case 70002:
		_app->puzzleSetActive(kPuzzle70303);
		_app->soundPlay(70003);
		break;

	case 70003:
		_app->playMovie("1504");
		_app->playMovie("1505");
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->varSetFloat(90006, 50.0);
		onSwitchZoneRO(0);
		break;

	case 71002:
	case 71003:
		_app->playMovie(Common::String::format("N2_%dA", _app->varGetByte(70014)));
		_app->objectPresentationShow(kObject70001, _app->varGetByte(70014) - 1);
		break;

	case 72001:
		_app->puzzleSetActive(kPuzzle72002);
		_app->soundPlay(72002);
		break;

	case 72002:
		_app->playMovie("1499");
		_app->puzzleSetActive(kPuzzle72003);
		_app->soundPlay(72003);
		break;

	case 72003:
		_app->puzzleSetActive(kPuzzle72004);
		_app->soundPlay(72004);
		break;

	case 72004:
		_app->puzzleSetActive(kPuzzle72005);
		_app->soundPlay(72005);
		break;

	case 72005:
		_app->playMovie("1500");
		_app->puzzleSetActive(kPuzzle72006);
		_app->soundPlay(72006);
		break;

	case 72006:
		_app->puzzleSetActive(kPuzzle72007);
		_app->soundPlay(72007);
		break;

	case 72007:
		_app->puzzleSetActive(kPuzzle72008);
		_app->soundPlay(72008);
		break;

	case 72008:
		_app->puzzleSetActive(kPuzzle72009);
		_app->soundPlay(72009);
		break;

	case 72009:
		_app->playMovie("1501");
		_app->puzzleSetActive(kPuzzle72010);
		_app->soundPlay(72010);
		break;

	case 72010:
		_app->playMovie("1502");
		_app->soundPlay(70000, kSoundLoop);
		_app->varSetByte(70013, 0);
		_app->puzzleSetActive(kPuzzle70301);
		_app->soundPlay(70056);
		break;

	case 70043:
	case 70044:
	case 70045:
	case 70046:
	case 70047:
	case 70048:
	case 70049:
	case 70050:
	case 70051:
	case 70052:
	case 70053:
	case 70054: {
		byte index = _app->varGetByte(70013);
		char str = _app->varGetString(70099)[index];
		if ( str > '0' )
			_app->puzzleSetActive((PuzzleId)(str + 70551));

		_app->soundPlay(id + 1);

		_app->varSetByte(70013, index + 1);
		}
		break;

	case 70055:
		_app->objectSetAccessibilityOff(kObject70700, 0, 0);
		_app->objectSetAccessibilityOn(kObject70700, 1, 1);
		_app->rotationSetActive(70200);
		break;

	case 70017:
	case 70018:
	case 70019:
	case 70020:
	case 70021:
	case 70024:
	case 70025:
	case 70026:
	case 70056:
	case 70057:
	case 70058:
	case 70059:
	case 70060:
	case 70061:
	case 70062:
	case 70063:
	case 70064:
	case 70065: {
		byte index = _app->varGetByte(70013);
		char str = _app->varGetString(70099)[index];
		_app->puzzleSetActive((PuzzleId)(str + 70251));

		_app->soundPlay(id + 1);

		_app->varSetByte(70013, index + 1);
		}
		break;

	case 70066:
		_app->rotationSetAlp(70300, 160.0f);
		_app->rotationSetActive(70300);
		break;

	case 70022:
		sub_433EE0();
		_app->rotationSetActive(70001);
		_app->playMovie("1503");
		_app->puzzleSetActive(kPuzzle70303);
		_app->varSetByte(70013, 51);
		_app->soundPlay(70024);
		break;

	case 70101:
		_app->puzzleSetActive(kPuzzle70305);
		_app->soundPlay(70023);
		break;

	case 70023:
		_app->objectSetAccessibilityOff(kObject70108);
		_app->puzzleSetActive(kPuzzle70102);
		break;

	case 70027:
		_app->puzzleSetActive(kPuzzle70000);
		_app->soundPlay(71001);
		break;

	case 71001:
		_app->playMovie(Common::String::format("N2_%dA", _app->varGetByte(70014)));
		_app->objectSetAccessibilityOn(kObjectCentaur, 1, 1);
		_app->objectPresentationShow(kObject70001, _app->varGetByte(70014) - 1);
		break;

	case 71010:
	case 71011:
	case 71012:
		_app->objectPresentationHide(kObject70001);
		_app->playMovie(Common::String::format("N2_%d%c", _app->varGetByte(70014), _app->varGetByte(70015) + 67));
		_app->soundPlay(_app->varGetByte(70015) + 10 * (_app->varGetByte(70014) + 7109));
		break;

	case 71100:
	case 71101:
	case 71102:
	case 71103:
	case 71104:
	case 71105:
	case 71106:
	case 71107:
	case 71108:
	case 71109:
	case 71110:
	case 71111:
	case 71112:
	case 71113:
	case 71114:
	case 71115:
	case 71116:
	case 71117:
	case 71118:
	case 71119:
	case 71120:
	case 71121:
	case 71122:
	case 71123:
	case 71124:
	case 71125:
	case 71126:
	case 71127:
	case 71128:
	case 71129:
		if (_app->varGetByte(70014) >= 3) {
			if (_app->varGetByte(70015) != 2) {
				_app->exitToMenu(2);
				break;
			}

			_app->puzzleSetActive(kPuzzle70303);
			_app->soundPlay(70001);
		} else {
			if (_app->varGetByte(70014) == 2 && !_app->varGetByte(70015)) {
				_app->exitToMenu(2);
				break;
			}

			_app->soundPlay(_app->varGetByte(70014) + 71001);
			_app->varSetByte(70014, _app->varGetByte(70014) + 1);

			_app->objectSetAccessibilityOn(kObjectCentaur);
			_app->objectSetAccessibilityOn(kObjectDragon);
			_app->objectSetAccessibilityOn(kObjectPhoenix1);
		}
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
		onAnimationNextFrameZoneNI(animationId, name, frame, frameCount);
		break;

	case kZoneRH:
		onAnimationNextFrameZoneRH(animationId, name, frame, frameCount);
		break;

	case kZoneFO:
		onAnimationNextFrameZoneFO(animationId, name, frame, frameCount);
		break;

	case kZoneRO:
		onAnimationNextFrameZoneRO(animationId, name, frame, frameCount);
		break;

	case kZoneAS:
		onAnimationNextFrameZoneAS(animationId, name, frame, frameCount);
		break;

	case kZoneWA:
		onAnimationNextFrameZoneWA(animationId, name, frame, frameCount);
		break;

	case kZoneN2:
		onAnimationNextFrameZoneN2(animationId, name, frame, frameCount);
		break;
	}
}

void EventHandlerRing::onAnimationNextFrameZoneNI(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 10000:
		if (frame == 1) {
			uint32 val = rnd(10);
			if (val > 4)
				val = 0;

			_app->objectPresentationPauseAnimationFrame(kObjectGlug, 0, 1, 300 * val, 0);
		}
		break;

	case 10101:
		if (frame == 36) {
			_app->objectSetAccessibilityOn(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOn(kObject10101, 1, 1);
		}
		break;

	case 10103:
		if (frame == 1) {
			_app->objectSetAccessibilityOff(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOn(kObject10101, 1, 1);

			if (_app->varGetByte(10113) < 9)
				_app->objectSetAccessibilityOn(kObject10109);
		}
		break;

	case 10100:
		if (frame == 1) {
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10101, 3);
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10102:
		if (frame == 36) {
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10101, 4);
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectPresentationHide(kObject10101, 5);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10105:
		if (frame == 36) {
			_app->objectSetAccessibilityOn(kObject10105, 1, 1);
			_app->objectSetAccessibilityOn(kObject10102, 1, 1);
		}
		break;

	case 10104:
		if (frame == 15) {
			_app->objectSetAccessibilityOn(kObject10102, 1, 1);
			_app->objectSetAccessibilityOn(kObject10105, 0, 0);
		}
		break;

	case 10106:
		if (frame == 1) {
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectPresentationHide(kObject10102, 0);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10200:
		if (frame == 1) {
			uint32 val = rnd(10);
			if (val > 4)
				val = 0;

			_app->objectPresentationPauseAnimationFrame(kObject10200, 1, 1, 1000 * val, 0);
		}
		break;

	case 10300:
		if (frame == 3) {
			_app->rotationSet3DSoundOn(10301, 10301);
			_app->rotationSet3DSoundOff(10301, 10301);
			_app->soundPlay(10301);
		}
		break;

	case 10422:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject10430, 1, 2);
			break;

		case 2:
			_app->soundStop(10407, 1024);
			break;

		case 14:
			_app->soundPlay(10407);
			break;

		case 15:
			_app->soundStop(10405, 1024);
			break;

		case 26:
			_app->soundPlay(10405);
			break;
		}
		break;

	case 10423:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(10408);
			break;

		case 15:
			_app->soundPlay(10406);
			break;

		case 26:
			_app->soundStop(10406, 1024);
			_app->soundStop(10408, 1024);
			break;

		case 27:
			_app->puzzleSetMovabilityOn(kPuzzle10411);
			_app->objectSetAccessibilityOn(kObject10430, 0, 0);
			break;
		}
		break;

	case 10424:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectPresentationHide(kObject10432, 0);
			_app->objectPresentationShow(kObject10430, 6);
			_app->objectSetAccessibilityOn(kObject10430, 1, 2);
			break;

		case 2:
			_app->soundStop(10407, 1024);
			break;

		case 14:
			_app->soundPlay(10407);
			break;
		case 15:
			_app->soundStop(10405, 1024);
			break;

		case 26:
			_app->soundPlay(10405);
			break;
		}
		break;

	case 10425:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(10408);
			_app->objectPresentationHide(kObject10430, 6);
			break;

		case 15:
			_app->soundPlay(10406);
			break;

		case 26:
			_app->puzzleSetMovabilityOn(kPuzzle10411);
			_app->objectSetAccessibilityOn(kObject10430, 0, 0);
			_app->varSetByte(10431, 1);
			_app->objectPresentationShow(kObject10432, 0);
			sub_445A10();
			_app->soundStop(10406, 1024);
			_app->soundStop(10408, 1024);
			break;
		}
		break;
	}
}

void EventHandlerRing::onAnimationNextFrameZoneRH(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId){
	default:
		break;

	case 20001:
		if (frame == 5) {
			_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 2);
			_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20001), 4);
			_app->soundPlay(_app->varGetByte(21001) + 20011);
		}
		break;

	case 20003:
		if (frame == 2)
			_app->soundPlay(23014);
		break;
	}
}

void EventHandlerRing::onAnimationNextFrameZoneFO(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 30001:
		switch (frame) {
		default:
			break;

		case 10:
			_app->soundPlay(30505);
			_app->soundSetVolume(30505, 90);
			break;

		case 90:
			_app->soundPlay(30503);
			_app->soundSetVolume(30503, 90);
			break;

		case 125:
			_app->soundPlay(30504);
			_app->soundSetVolume(30504, 95);
			break;

		case 202:
			_app->objectPresentationPauseAnimation(kObject30110, 1);
			break;
		}
		break;

	case 30007:
		switch (frame) {
		default:
			break;

		case 10:
			_app->soundPlay(30505);
			_app->soundSetVolume(30505, 95);
			break;

		case 90:
			_app->soundPlay(30503);
			_app->soundSetVolume(30503, 95);
			break;

		case 125:
			_app->soundPlay(30504);
			_app->soundSetVolume(30504, 100);
			break;

		case 202:
			_app->objectPresentationPauseAnimation(kObject30109, 0);
			_app->objectSetAccessibilityOn(kObject30109, 0, 0);
			break;
		}
		break;

	case 30000:
		if (_app->varGetByte(30047) == 1 && frame == 28) {
			_app->playMovie("1207");
			_app->varSetByte(30047, 0);
			_app->puzzleSetActive(kPuzzle35009);
		}
		break;

	case 30002:
		if (frame == 19)
			_app->objectPresentationShow(kObject30110, 6);
		break;

	case 30004:
		if (frame == 19)
			_app->objectPresentationShow(kObject30110, 9);
		break;

	case 30003:
		if (frame == 19)
			_app->objectPresentationHide(kObject30110, 6);
		break;

	case 30005:
		if (frame == 19)
			_app->objectPresentationHide(kObject30110, 9);
		break;

	case 30006:
		switch (frame){
		default:
			break;

		case 10:
			_app->soundPlay(30502);
			break;

		case 26:
			_app->objectPresentationPauseAnimation(kObjectSleepingPotion, 0);
			_app->timerStop(kTimer2);
			_app->timerStart(kTimer2, 1000 * (rnd(10) + 5));
			break;
		}
		break;

	case 30008:
		if (frame == 25) {
			_app->objectSetAccessibilityOn(kObject30040, 5, 5);
			_app->objectSetAccessibilityOn(kObject30040, 3, 3);
		}
		break;
	}
}

void EventHandlerRing::onAnimationNextFrameZoneRO(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 40100:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40200))
				_app->soundPlay(40200);
			else
				_app->soundPlay(40300);
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 1, 1);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);

			if (_app->varGetString(40901) == "6543210") {
				if ((_app->varGetByte(40200) + _app->varGetByte(40201) + _app->varGetByte(40202) + _app->varGetByte(40203)
				   + _app->varGetByte(40204) + _app->varGetByte(40205) + _app->varGetByte(40206)) == 7 ) {
					_app->varSetByte(40802, 1);
					_app->varSetString(40901, "0000000");
					_app->soundPlay(40400);
					_app->varSetFloat(90006, 85.69999694824219);
					_app->objectSetAccessibilityOff(kObject40060);
					_app->objectSetAccessibilityOff(kObject40201);

					for (int i = 10; i > 0; i--) {
						_app->objectPresentationHide(kObject40060);
						_app->objectPresentationShow(kObject40060, i);
						handleEvents();

						if (checkEscape())
							break;
					}
				}
			}
			break;
		}

		_app->varSetByte(40911, frame);
		return;

	case 40101:
		switch (frame) {
		default:
			_app->varSetByte(40912, frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40912, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40201)) {
				_app->soundPlay(40201);
				_app->varSetByte(40912, frame);
			} else {
				_app->soundPlay(40301);
				_app->varSetByte(40912, frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 2, 2);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40912, frame);
			break;
		}
		break;

	case 40102:
		switch (frame) {
		default:
			_app->varSetByte(40913, frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40913, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40202)) {
				_app->soundPlay(40202);
				_app->varSetByte(40913, frame);
			} else {
				_app->soundPlay(40302);
				_app->varSetByte(40913, frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 3, 3);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40913, frame);
			break;
		}
		break;

	case 40103:
		switch (frame) {
		default:
			_app->varSetByte(40914, frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40914, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);


			if (_app->varGetByte(40203)) {
				_app->soundPlay(40203);
				_app->varSetByte(40914, frame);
			} else {
				_app->soundPlay(40303);
				_app->varSetByte(40914, frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 4, 4);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40914, frame);
			break;
		}
		break;

	case 40104:
		switch (frame) {
		default:
			_app->varSetByte(40915, frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40915, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40204)) {
				_app->soundPlay(40204);
				_app->varSetByte(40915, frame);
			} else {
				_app->soundPlay(40304);
				_app->varSetByte(40915, frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 5, 5);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40915, frame);
			break;
		}
		break;

	case 40105:
		switch (frame) {
		default:
			_app->varSetByte(40916, frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40916, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40205)) {
				_app->soundPlay(40205);
				_app->varSetByte(40916, frame);
			} else {
				_app->soundPlay(40305);
				_app->varSetByte(40916, frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 6, 6);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40916, frame);
			break;
		}
		break;

	case 40106:
		switch (frame) {
		default:
			_app->varSetByte(40917, frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40917, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);


			if (_app->varGetByte(40206)) {
				_app->soundPlay(40206);
				_app->varSetByte(40917, frame);
			} else {
				_app->soundPlay(40306);
				_app->varSetByte(40917, frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 7, 7);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40917, frame);
			break;
		}
		break;

	case 40201:
	case 40202:
	case 40203:
	case 40204:
	case 40205:
	case 40206:
	case 40207:
		_app->varSetByte(frame + 700, frame); // id: frame + 700 = 4090x

		if (_app->varGetByte(40901) == 26 && _app->varGetByte(40902) == 26 && _app->varGetByte(40903) == 26 && _app->varGetByte(40904) == 26 \
		 && _app->varGetByte(40905) == 26 && _app->varGetByte(40906) == 26 && _app->varGetByte(40907) == 26 && _app->varGetByte(40911) == 70 \
		 && _app->varGetByte(40912) == 70 && _app->varGetByte(40913) == 70 && _app->varGetByte(40914) == 70 && _app->varGetByte(40915) == 70 \
		 && _app->varGetByte(40916) == 70 && _app->varGetByte(40917) == 70) {
			_app->objectSetAccessibilityOn(kObject40060, _app->varGetByte(40804) / 10, _app->varGetByte(40804) / 10);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
		}
		break;
	}
}

void EventHandlerRing::onAnimationNextFrameZoneWA(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	if (animationId == 50001) {
		if (_app->varGetByte(50007) == 1 && _app->varGetByte(50009) == 1) {
			if (frame == 35 || frame == 12) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 6);
				_app->objectPresentationHide(kObject50600, 7);
				_app->soundStop(50018, 1024);

				if (_app->varGetByte(50012) == 6) {
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 5.0f);
					_app->varSetByte(50012, 5);
				}
			}
		}

		if (_app->varGetByte(50008) == 1 && _app->varGetByte(50010) == 1) {
			if (frame == 49 || frame == 24) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 6);
				_app->objectPresentationShow(kObject50600, 7);
				_app->soundStop(50018, 1024);

				if (_app->varGetByte(50012) == 6) {
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 5.0f);
					_app->varSetByte(50012, 5);
				}
			}
		}

		if (_app->varGetByte(50008) == 1 && _app->varGetByte(50009) == 1) {
			if (frame == 17 || frame == 41) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 7);
				_app->objectPresentationShow(kObject50600, 6);
				_app->soundStop(50018, 1024);
				_app->varSetByte(50012, 6);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			}
		}

		if (_app->varGetByte(50007) == 1 && _app->varGetByte(50010) == 1) {
			if (frame == 4 || frame == 30) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 7);
				_app->objectPresentationShow(kObject50600, 6);
				_app->soundStop(50018, 1024);
				_app->varSetByte(50012, 6);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			}
		}
	}
}

void EventHandlerRing::onAnimationNextFrameZoneAS(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	if (animationId == 80001) {
		if (frame == _app->varGetByte(80004)) {
			_app->objectPresentationPauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOn(kObject80021, 0, 4);
			_app->soundStop(80210, 1024);
			_app->soundPlay(80209);
		}

		_app->varSetByte(80005, frame);
	}
}

void EventHandlerRing::onAnimationNextFrameZoneN2(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 70300:
		if (frame == 3)
			_app->soundPlay(70301);
		break;

	case 70102:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundPlay(70103);
			break;

		case 36:
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70101, 4);
			_app->objectPresentationHide(kObject70101, 0);
			_app->objectPresentationHide(kObject70101, 5);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);
			_app->objectSetAccessibilityOn(kObject70103);
			break;
		}
		break;

	case 70103:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject70101, 1, 1);
			_app->objectPresentationShow(kObject70100, 1);
			break;

		case 36:
			_app->soundPlay(70102);
			break;
		}
		break;

	case 70104:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundPlay(70104);
			break;

		case 15:
			_app->soundStop(70104, 1024);
			_app->objectSetAccessibilityOn(kObject70102, 1, 1);
			_app->objectSetAccessibilityOn(kObject70105, 0, 0);
			break;
		}
		break;

	case 70106:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundStop(70105, 1024);
			_app->objectPresentationHide(kObject70101, 0);
			_app->objectPresentationHide(kObject70102, 0);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);
			_app->objectSetAccessibilityOn(kObject70103);
			break;

		case 15:
			_app->soundPlay(70105);
			break;
		}
		break;

	case 70422:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject70404, 1, 2);
			break;

		case 2:
			_app->soundStop(70407, 1024);
			break;

		case 14:
			_app->soundPlay(70407);
			break;

		case 15:
			_app->soundStop(70405, 1024);
			break;

		case 26:
			_app->soundPlay(70405);
			break;
		}
		break;

	case 70423:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(70408);
			break;

		case 15:
			_app->soundPlay(70406);
			break;

		case 26:
			_app->soundStop(70406, 1024);
			_app->soundStop(70408, 1024);
			break;

		case 27:
			_app->puzzleSetMovabilityOn(70411);
			_app->objectSetAccessibilityOn(kObject70404, 0, 0);
			break;
		}
		break;

	case 70424:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectPresentationShow(kObject70404, 6);
			_app->objectSetAccessibilityOn(kObject70404, 1, 2);
			break;

		case 2:
			_app->soundStop(70407, 1024);
			break;

		case 14:
			_app->soundPlay(70407);
			break;

		case 15:
			_app->soundStop(70405, 1024);
			break;

		case 26:
			_app->soundPlay(70405);
			_app->objectPresentationHide(kObject70406);
			sub_433FA0();
			break;
		}
		break;

	case 70425:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(70408);
			_app->objectPresentationHide(kObject70404, 6);
			break;

		case 15:
			_app->soundPlay(70406);
			break;

		case 26:
			_app->puzzleSetMovabilityOn(70411);
			_app->objectSetAccessibilityOn(kObject70404, 0, 0);
			_app->varSetByte(70005, 1);
			_app->objectPresentationShow(kObject70406);
			sub_4340C0();

			if (!_app->varGetByte(70012))
				_app->objectPresentationShow(kObjectFire);
			break;
		}
		break;
	}
}

#pragma endregion

#pragma region OnSwitchZone functions

void EventHandlerRing::onSwitchZoneNI(uint32 type) {
	switch (type) {
	default:
		break;

	case 0:
		if (_app->varGetByte(90009))
			_app->setupZone((Zone)_app->varGetDword(90013), kSetupType10);
		else
			_app->setupZone(kZoneNI, kSetupTypeNone);
		break;

	case 3:
		_app->setupZone(kZoneNI, kSetupType3);
		break;

	case 9:
		_app->timerStart(kTimer9, 1000);
		break;
	}
}

void EventHandlerRing::onSwitchZoneRH(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRH, kSetupTypeNone);
}

void EventHandlerRing::onSwitchZoneFO(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90011))
			_app->setupZone((Zone)_app->varGetDword(90015), kSetupType10);
		else
			_app->setupZone(kZoneRO, kSetupTypeNone);
	}
}

void EventHandlerRing::onSwitchZoneRO(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRO, kSetupTypeNone);
}

void EventHandlerRing::onSwitchZoneAS(uint32 type) {
	switch (type) {
	default:
		break;

	case 1:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 0, 0);
		_app->varSetByte(90001, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 2);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80040);
		_app->objectSetAccessibilityOff(kObject80019, 0, 0);
		_app->objectSetAccessibilityOn(kObject80019, 1, 1);
		break;

	case 2:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 1, 1);
		_app->varSetByte(90002, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 3);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80049);
		break;

	case 3:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->soundSetMultiplier(kSoundTypeAmbientMusic, 100);
		_app->objectSetAccessibilityOn(kObject80018, 2, 2);
		_app->varSetByte(90003, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);


		_app->objectPresentationShow(kObject80018, 4);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80058);
		break;

	case 4:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 3, 3);
		_app->varSetByte(90004, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 5);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80068);
		break;

	case 5:
		_app->setupZone(kZoneAS, kSetupType5);
		break;
	case 13:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->setZoneAndEnableBag(kZoneAS);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);

		_app->getBag()->sub_419350();

		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void EventHandlerRing::onSwitchZoneWA(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90012))
			_app->setupZone((Zone)_app->varGetDword(90016), kSetupType10);
		else
			_app->setupZone(kZoneWA, kSetupTypeNone);
	}
}

void EventHandlerRing::onSwitchZoneN2(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90010))
			_app->setupZone((Zone)_app->varGetDword(90014), kSetupType10);
		else
			_app->setupZone(kZoneN2, kSetupTypeNone);
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
			_app->puzzleSetActive(kPuzzle70305, 1, 1);
			_app->soundPlay(70017);
	}
}

void EventHandlerRing::sub_433FA0() {
	_app->puzzleSet3DSoundOff(kPuzzle70410, 70412);
	_app->rotationSet3DSoundOff(70400, 70412);

	for (int i = 95; i > 70; i--) {
		_app->puzzleSet3DSoundVolume(kPuzzle70411, 70412, i);
		handleEvents();
	}

	_app->puzzleSet3DSoundOff(kPuzzle70411, 70412);
	_app->rotationSet3DSoundOff(70000, 70107);
	_app->rotationSet3DSoundOff(70001, 70107);
	_app->rotationSet3DSoundOff(70400, 70107);
	_app->rotationSet3DSoundOff(70100, 70107);
	_app->rotationSet3DSoundOff(70101, 70107);
	_app->puzzleSet3DSoundOff(kPuzzle70100, 70107);
	_app->puzzleSet3DSoundOff(kPuzzle70102, 70107);
	_app->soundPlay(70000);

}

void EventHandlerRing::sub_4340C0() {
	_app->soundStop(70000, 1024);
	_app->puzzleSet3DSoundOn(kPuzzle70411, 70412);
	_app->puzzleSet3DSoundOn(kPuzzle70410, 70412);
	_app->rotationSet3DSoundOn(70400, 70412);

	for (int i = 70; i < 95; i--) {
		_app->puzzleSet3DSoundVolume(kPuzzle70411, 70412, i);
		handleEvents();
	}

	_app->rotationSet3DSoundOn(70000, 70107);
	_app->rotationSet3DSoundOn(70001, 70107);
	_app->rotationSet3DSoundOn(70400, 70107);
	_app->rotationSet3DSoundOn(70100, 70107);
	_app->rotationSet3DSoundOn(70101, 70107);
	_app->puzzleSet3DSoundOn(kPuzzle70100, 70107);
	_app->puzzleSet3DSoundOn(kPuzzle70102, 70107);
}

void EventHandlerRing::sub_445A10() {
	uint32 val1 = _app->varGetByte(10106) + _app->varGetByte(10431);
	uint32 val2 = _app->varGetByte(10420) + _app->varGetByte(10421);

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
