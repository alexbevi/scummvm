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

#include "ring/game/event_ring.h"

#include "ring/base/accessibility.h"
#include "ring/base/bag.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"
#include "ring/base/timer.h"

#include "ring/game/application_ring.h"
#include "ring/game/shared_ring.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/textconsole.h"

using namespace RingGame;

namespace Ring {

EventHandlerRing::EventHandlerRing(ApplicationRing *application) : _app(application), _controlNotPressed(false) {
	// Data
	_dword_4A1C00 = 0;

	// Timer data
	_disableTimerRH = false;
	_frameNumberTimerRH = 0;

	// Local event data
	_presentationIndexRO = 0;
	_presentationIndexBagRO = 0;
	_presentationIndexBagRO2 = 0;

	_stateBagNI = false;
	_presentationIndexNI = 0;
	_indexOffsetNI = 0;
	_distNI = 0;

	_presentationIndexN2 = 0;
	_distN2 = 0;

	_presentationIndexSY = 0;
	_prefsSubtitles = false;
	_prefsVolume = 0;
	_prefsVolumeDialog = 0;
	_object1Visible = false;
	_offsetX = 0;
	_offsetX0 = 0;
	_coordX = 0;
}


EventHandlerRing::~EventHandlerRing() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerRing::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	_controlNotPressed = !isControlPressed;

	onMouseLeftButtonUp(evt);
}

void EventHandlerRing::onMouseLeftButtonUp(const Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonUp");

	// Handle clicks on bag
	Bag *bag = _app->getBag();
	if (bag->isInitialized()) {
		if (bag->checkHotspotClick(evt.mouse) == 1) {

			// Handle clicked object event
			onBagClickedObject(_app->getBag()->getClickedObject());

			if (_app->getField77()) {
				bag->reset();
				_app->setFreOffCurrentRotation();

				if (_app->getField78()) {
					_app->initObjectCursors(bag->getClickedObject());
				} else {
					_app->setField78(true);
					_app->cursorDelete();
				}

				// Center cursor on screen
				g_system->warpMouse(320, 240);

			} else {
				_app->setField77(true);
				_app->cursorDelete();
			}
		}

		return;
	}

	// Handle clicks on drag control
	DragControl *dragControl = _app->getDragControl();
	if (dragControl->getField20()) {
		onBag(dragControl->getObjectId(), dragControl->getTarget(), dragControl->getPuzzleRotationId(), dragControl->getField39(), dragControl, 2);

		if (_app->getState() == kStateShowMenu)
			return;

		dragControl->reset();

		if (dragControl->getField45() == 2)
			return;
	}

	// Handle clicks on menu
	Puzzle *puzzleMenu = _app->getPuzzle(kPuzzleMenu);
	if (puzzleMenu) {
		if (puzzleMenu->visualHandleLeftButtonUp(evt.mouse)) {
			_app->update(evt.mouse);
			return;
		}

		Accessibility *accessibility = puzzleMenu->getAccessibility(evt.mouse);
		if (handleLeftButtonUp(accessibility, puzzleMenu->getId(), evt.mouse))
			return;

		if (puzzleMenu->getField24() == 1) {
			Movability *movability = puzzleMenu->getMovability(evt.mouse);
			if (handleLeftButtonUp(movability, (uint32)puzzleMenu->getMovabilityIndex(evt.mouse), puzzleMenu->getId()))
				return;
		}

		if (puzzleMenu->getField24() == 2)
			return;
	}

	// Handle current puzzle
	Puzzle *currentPuzzle = _app->getCurrentPuzzle();
	if (currentPuzzle) {
		if (currentPuzzle->visualHandleLeftButtonUp(evt.mouse)) {
			_app->update(evt.mouse);
			return;
		}

		Accessibility *accessibility = currentPuzzle->getAccessibility(evt.mouse);
		if (handleLeftButtonUp(accessibility, currentPuzzle->getId(), evt.mouse))
			return;

		Movability *movability = currentPuzzle->getMovability(evt.mouse);
		if (handleLeftButtonUp(movability, (uint32)currentPuzzle->getMovabilityIndex(evt.mouse), currentPuzzle->getId()))
			return;
	}

	// Handle current rotation
	Rotation *currentRotation = _app->getCurrentRotation();
	if (currentRotation) {
		if (!currentRotation->getField28())
			return;

		Accessibility *accessibility = currentRotation->getAccessibility(evt.mouse);
		if (handleLeftButtonUp(accessibility, currentRotation->getId(), evt.mouse))
			return;

		Movability *movability = currentRotation->getMovability(evt.mouse);
		if (handleLeftButtonUp(movability, (uint32)currentRotation->getMovabilityIndex(evt.mouse), currentRotation->getId(), true))
			return;

		if (_app->getField75())
			_app->cursorDelete();

		_app->setField75(true);
	}
}

bool EventHandlerRing::handleLeftButtonUp(Accessibility *accessibility, Id id, const Common::Point &point) {
	if (!accessibility)
		return false;

	Object *object = accessibility->getObject();
	Hotspot *hotspot = accessibility->getHotspot();

	if (object->getFieldC() & 1) {
		onButtonUp(object->getId(), hotspot->getTarget(), id, 1, point);

		if (_app->getState() == kStateShowMenu)
			return true;
	}

	if (!(object->getFieldC() & 8)) {
		_app->update(point);
		return true;
	}

	onButtonUp2(object->getId(), (uint32)hotspot->getTarget(), id, 1, point);

	if (_app->getState() != kStateShowMenu) {
		if (_app->getField74()) {
			_app->cursorDelete();
			_app->getBag()->setClickedObject(object->getId());
			_app->initObjectCursors(object->getId());
		}

		_app->setField74(true);

		_app->update(point);
	}

	return true;
}

bool EventHandlerRing::handleLeftButtonUp(Movability *movability, uint32 index, Id id, bool isRotation) {
	if (!movability)
		return false;

	Hotspot *hotspot = movability->getHotspot();

	//////////////////////////////////////////////////////////////////////////
	// Before Ride
	//////////////////////////////////////////////////////////////////////////
	onBeforeRide(id, movability->getTo(), index, hotspot->getTarget(), movability->getType());

	if (_app->getState() == kStateShowMenu)
		return true;

	bool showRide = _controlNotPressed;

	int updateType = 0;
	if (isRotation) {
		if (_controlNotPressed) {
			updateType = movability->getUpdateType();
		} else {
			updateType = 2;
			showRide = false;
		}

		switch (updateType) {
		default:
			break;

		case 0:
			showRide = _app->getCurrentRotation()->setRolTo(movability->getAlpBefore(), movability->getBetBefore(), movability->getRanBefore());
			break;

		case 1:
			_app->getCurrentRotation()->updateAndDraw(movability->getAlpBefore(), movability->getBetBefore(), movability->getRanBefore());
			break;
		}

	}

	_app->reset();

	switch (movability->getType()) {
	default:
		break;

	case kMovabilityRotationToRotation:
	case kMovabilityPuzzleToRotation:
		if (isRotation)
			break;

		if (_app->getRotation(movability->getTo())) {
			Rotation *rotation = _app->getRotation(movability->getTo());

			float alp = movability->getAlpAfter() - 135.0f;
			if (alp < 0.0f)
				alp += 360.0f;

			rotation->setAlp(alp);

			rotation->updateAmbientSoundPan(false);

			_app->getSoundHandler()->setSounds2(rotation->getSoundItems());
			_app->getSoundHandler()->setField0(true);

			if (!_app->getSoundHandler()->processSounds()) {
				_app->getSoundHandler()->setField0(false);
				_app->getSoundHandler()->turnOffSounds1(true);
			}
		}
		break;

	case kMovabilityRotationToPuzzle:
	case kMovabilityPuzzleToPuzzle:
		if (_app->getPuzzle(movability->getTo().id())) {
			Puzzle *puzzle = _app->getPuzzle(movability->getTo().id());

			_app->getSoundHandler()->setSounds2(puzzle->getSoundItems());
			_app->getSoundHandler()->setField0(true);

			if (!_app->getSoundHandler()->processSounds()) {
				_app->getSoundHandler()->setField0(false);
				_app->getSoundHandler()->turnOffSounds1(true);
			}
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// Ride
	//////////////////////////////////////////////////////////////////////////
	if (showRide)
		_app->playMovie(movability->getRideName());

	//////////////////////////////////////////////////////////////////////////
	// After ride
	//////////////////////////////////////////////////////////////////////////
	if (_app->getSoundHandler()->getField0()) {
		_app->getSoundHandler()->turnOffItems1();

		if (!_app->getSoundHandler()->updateItems(2)) {
			_app->getSoundHandler()->turnOffSounds1(true);
			_app->getSoundHandler()->setField0(false);
		}
	}

	if (_app->getSoundHandler()->getField0()) {
		_app->getSoundHandler()->updateItems2(3);
		_app->getSoundHandler()->updateItems3(3);
	}

	switch (movability->getType()){
	default:
		break;

	case kMovabilityRotationToRotation:
	case kMovabilityPuzzleToRotation:
		_app->rotationSetActive(movability->getTo());
		_app->getCurrentRotation()->updateAndDraw(movability->getAlpAfter(), movability->getBetAfter(), movability->getRanAfter());

		onAfterRide(_app->getCurrentRotationId(), id, index, hotspot->getTarget(), movability->getType());

		if (_app->getState() == kStateShowMenu)
			return true;
		break;

	case kMovabilityRotationToPuzzle:
	case kMovabilityPuzzleToPuzzle:
		_app->puzzleSetActive(movability->getTo().id());

		onAfterRide(_app->getCurrentRotationId(), id, index, hotspot->getTarget(), movability->getType());

		if (_app->getState() == kStateShowMenu)
			return true;
		break;
	}

	if (_app->getField75())
		_app->cursorDelete();

	_app->setField75(true);

	return true;
}

void EventHandlerRing::onMouseLeftButtonDown(const Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonDown");

	if (_app->getBag()->isInitialized())
		return;

	// Handle menu
	Puzzle *puzzleMenu = _app->getPuzzle(kPuzzleMenu);
	if (puzzleMenu) {

		if (puzzleMenu->visualHandleLeftButtonDown(evt.mouse)) {
			_app->update(evt.mouse);
			return;
		}

		Accessibility *accessibility = puzzleMenu->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, (uint32)puzzleMenu->getAccessibilityIndex(evt.mouse), puzzleMenu->getId(), evt.mouse))
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
		if (handleLeftButtonDown(accessibility, (uint32)currentPuzzle->getAccessibilityIndex(evt.mouse), currentPuzzle->getId(), evt.mouse))
			return;
	}

	// Handle current rotation
	Rotation *currentRotation = _app->getCurrentRotation();
	if (currentRotation) {
		Accessibility *accessibility = currentRotation->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, (uint32)currentRotation->getAccessibilityIndex(evt.mouse), currentRotation->getId(), evt.mouse))
			return;
	}
}

bool EventHandlerRing::handleLeftButtonDown(Accessibility *accessibility, uint32 index, Id id, const Common::Point &point) {
	if (!accessibility)
		return false;

	Object *object = accessibility->getObject();
	Hotspot *hotspot = accessibility->getHotspot();

	if (object->getFieldC() & 2) {
		onButtonDown(object->getId(), hotspot->getTarget(), id, 1, point);

		if (_app->getState() == kStateShowMenu)
			return true;
	}

	if (!(object->getFieldC() & 4)) {
		_app->update(point);
		return true;
	}

	_app->getDragControl()->init(point, object->getId(), index, hotspot, hotspot->getTarget(), id, 1);
	onBag(object->getId(), hotspot->getTarget(), id, 1, _app->getDragControl(), 1);

	if (_app->getState() != kStateShowMenu) {
		if (!_app->getField76())
			_app->getDragControl()->reset();

		_app->setField76(true);
		_app->update(point);
	}

	return true;
}

void EventHandlerRing::onMouseRightButtonUp(const Common::Event &) {
	debugC(kRingDebugLogic, "onMouseRightButtonUp");

	if (getApp()->getDragControl()->getField20() || getApp()->getCurrentGameZone())
		return;

	Puzzle *puzzleMenu = getApp()->getPuzzle(kPuzzleMenu);
	if (puzzleMenu && puzzleMenu->getField24() == 2)
		return;

	Bag *bag = getApp()->getBag();
	if (bag->isInitialized()) {
		bag->reset();
	} else {
		getApp()->cursorDelete();
		bag->initialize();

		if (getApp()->getCurrentRotation())
			bag->setRotationFre(getApp()->getCurrentRotation()->getFre());

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
			Common::Point point = accessibility->getHotspot()->getCenter();
			if (_app->hasCurrentRotation())
				_app->getCurrentRotation()->setCoordinates(&point, evt.kbd.keycode);

			// Modify event to be a mouse click on the center of the hotspot
			bool controlPressed = (bool)(evt.kbd.flags & Common::KBD_CTRL);
			evt.mouse = point;

			// Simulate click
			if (_app->getBag()->isInitialized()) {
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
			Accessibility *accessibility = currentPuzzle->getAccessibility(evt.kbd.keycode);
			if (!accessibility)
				return;

			bool controlPressed = (bool)(evt.kbd.flags & Common::KBD_CTRL);

			// Modify event to be a mouse click on the center of the hotspot
			Hotspot *hotspot = accessibility->getHotspot();
			evt.mouse = hotspot->getCenter();

			// Simulate click
			if (_app->getBag()->isInitialized()) {
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

		bool controlPressed = (bool)(evt.kbd.flags & Common::KBD_CTRL);

		// Modify event to be a mouse click on the center of the hotspot
		Hotspot *hotspot = accessibility->getHotspot();
		evt.mouse = hotspot->getCenter();

		// Center rotation
		currentRotation->setCoordinates(&evt.mouse, evt.kbd.keycode);

		// Simulate click
		if (_app->getBag()->isInitialized()) {
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

#pragma region Left Button Up/Down

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
		onButtonDownZoneNI(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		// Function does not seem to change anything (gets the current bag object)
		//onButtonDownZoneFO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		onButtonDownZoneRO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		onButtonDownZoneN2(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerRing::onButtonDownZoneNI(ObjectId id, uint32, Id puzzleRotationId, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject10101:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (puzzleRotationId) {
		default:
			break;

		case 0:
			_app->soundPlay(10103);
			_app->objectSetAccessibilityOff(kObject10101, 1, 2);
			_app->objectSetAccessibilityOff(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOff(kObject10110);
			_app->objectPresentationShow(kObject10101, _app->varGetByte(10102) ? 4 : 3);
			_app->objectPresentationHide(kObject10100);
			_app->objectPresentationHide(kObject10109);
			break;

		case 1:
			_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10102);
			_app->soundPlay(10102);
			_app->objectSetAccessibilityOff(kObject10101, 1, 2);
			_app->objectSetAccessibilityOff(kObject10102, 1, 2);
			_app->objectSetAccessibilityOff(kObject10100);
			_app->objectSetAccessibilityOff(kObject10103);

			if (_app->varGetByte(10102)) {
				if (_app->varGetByte(10113) == 9) {
					_app->objectPresentationHide(kObject10101, 0);
					_app->objectPresentationShow(kObject10101, 5);
				} else {
					_app->objectSetAccessibilityOn(kObject10110);
				}

				_app->objectPresentationShow(kObject10100, 1);
				_app->objectPresentationShow(kObject10101, 2);
			} else {
				_app->objectPresentationShow(kObject10101, 0);
				_app->objectPresentationShow(kObject10101, 1);
			}

			_app->objectPresentationShow(kObject10100, 0);
			break;
		}
		break;

	case kObject10102:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		if (_app->varGetByte(10113) != 9)
			break;

		switch (puzzleRotationId) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject10102, 1, 2);

			if (_app->varGetByte(10103)) {
				_app->soundPlay(10105);
				_app->objectSetAccessibilityOff(kObject10105);
				_app->objectPresentationShow(kObject10102, 3);
				_app->objectPresentationHide(kObject10100);
			} else {
				_app->objectSetAccessibilityOff(kObject10105);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationHide(kObject10102, 0);
				_app->objectPresentationHide(kObject10100);
				_app->objectSetAccessibilityOn(kObject10102, 0, 0);
				_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			}
			break;

		case 1:
			_app->objectPresentationHide(kObject10102);
			if (_app->varGetByte(10102) == 1) {
				_app->soundPlay(10104);
				_app->objectSetAccessibilityOff(kObject10101, 1, 2);
				_app->objectSetAccessibilityOff(kObject10102, 1, 2);
				_app->objectSetAccessibilityOff(kObject10100);
				_app->objectSetAccessibilityOff(kObject10103);
				_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationShow(kObject10102, 0);

				if (_app->varGetByte(10103)) {
					_app->objectPresentationShow(kObject10100, 2);
					_app->objectPresentationShow(kObject10102, 2);
				} else {
					_app->objectPresentationShow(kObject10102, 1);
				}
			} else {
				_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationShow(kObject10102, 4);

				// FIXME: Original calls it 4 times. Check why.
				for (uint32 i = 0; i < 4; i++)
					handleEvents();


				_app->objectPresentationHide(kObject10102, 4);
				_app->objectSetAccessibilityOn(kObject10101, 0, 0);
				_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			}
			break;
		}
		break;

	case kObject10109:
		if (!_app->bagHasClickedObject()) {
			_app->soundPlay(10106);
			_app->objectPresentationHide(kObject10109);
			_app->objectPresentationShow(kObject10109, (uint32)puzzleRotationId);

			if (puzzleRotationId) {
				_app->varSetByte(_app->varGetByte(10113) + 10108, (byte)puzzleRotationId);
				_app->varSetByte(10113, _app->varGetByte(10113) + 1);
			}
		}
		break;

	case kObject10430:
		_app->objectSetAccessibilityOff(kObject10430, 1, 2);
		_app->objectPresentationShow(kObject10430, _app->varGetByte(10430) ? 5 : 3);
		_app->objectPresentationHide(kObject10430, 0);
		_app->objectPresentationHide(kObject10430, 1);
		break;
	}
}

void EventHandlerRing::onButtonDownZoneRO(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	if (id != kObject40202)
		return;

	if (_app->bagHasClickedObject()) {
		_app->cursorDelete();
		return;
	}

	for (uint32 i = 1; i < 16; i++)
		_app->objectPresentationHide(kObject40202, i);

	_app->objectPresentationShow(kObject40202, target + 1);

	if (target >= 7) {
		_app->soundPlay(target + 40500);

		Common::String str = _app->varGetString(40902);
		while (str.size() > 7)
			str.deleteLastChar();

		_app->varSetString(40902, Common::String::format("%s%1d", str.c_str(), target - 7));

		if (_app->varGetString(40902) == "01276534"
		 || _app->varGetString(40902) == "01476534"
		 || _app->varGetString(40902) == "01276532"
		 || _app->varGetString(40902) == "01476532") {
			_app->varSetFloat(90006, 100.0f);
			_app->soundPlay(40603);
		}
	} else {
		_app->soundPlay(target + 40500);
	}
}

void EventHandlerRing::onButtonDownZoneN2(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject70101:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectPresentationShow(kObject70101, 4);
			_app->objectPresentationHide(kObject70100);
			break;

		case 1:
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70102);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70100);
			_app->objectSetAccessibilityOff(kObject70103);
			_app->objectPresentationShow(kObject70101, 5);
			_app->objectPresentationShow(kObject70100, 1);
			_app->objectPresentationShow(kObject70101, 2);
			break;
		}
		break;

	case kObject70102:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70105);
			_app->objectPresentationShow(kObject70102, 3);
			_app->objectPresentationHide(kObject70100);
			break;

		case 1:
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70102);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70100);
			_app->objectSetAccessibilityOff(kObject70103);
			_app->objectPresentationShow(kObject70102, 0);
			_app->objectPresentationShow(kObject70100, 2);
			_app->objectPresentationShow(kObject70102, 2);
			break;
		}
		break;

	case kObject70404:
		if (target == 2) {
			_app->objectSetAccessibilityOff(kObject70404, 1, 2);
			_app->objectPresentationShow(kObject70404, _app->varGetByte(70001) ? 5 : 3);
			_app->objectPresentationHide(kObject70404, 0);
			_app->objectPresentationHide(kObject70404, 1);

		}
		break;
	}
}

void EventHandlerRing::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
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

	case kZoneNI:
		onButtonUpZoneNI(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneRH:
		onButtonUpZoneRH(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		onButtonUpZoneFO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		onButtonUpZoneRO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneWA:
		onButtonUpZoneWA(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneAS:
		onButtonUpZoneAS(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		onButtonUpZoneN2(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerRing::onButtonUpZoneSY(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	RingEngine *engine = ((RingEngine *)g_engine);

	switch (id) {
	default:
		break;

	case kObject1:
		if (!_app->bagHasClickedObject()) {
			if (_object1Visible) {
				_app->objectPresentationHideAndRemove(kObject1);
				_object1Visible = false;
			} else {
				_app->objectPresentationShow(kObject1);
				_app->rotationSet3DSoundOn(50004, 1);
				_app->puzzleSet3DSoundOn(kPuzzle50001, 1);
				_object1Visible = true;
			}
		}
		break;

	case kObject2:
		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHideAndRemove(kObject2);
			_app->objectSetAccessibilityOff(kObject2);
			_app->puzzleSetMod(kPuzzleMenu, 1, 0);
			break;

		case 1:
			Engine::quitGame();
			break;
		}
		break;

	case kObject3:
		_app->messageHideWarning(target);
		break;

	case kObject4:
		switch (target) {
		default:
		case 0:
		case 1:
			_app->messageHideQuestion(0);
			break;

		case 2:
			_app->cursorSelect(kCursorBusy);
			unsetFlag();
			handleEvents();
			_app->exitZone();
			_app->initZones();
			_app->loadPreferences();
			_app->messageHideQuestion(2);
			break;

		case 3:
			_app->messageHideQuestion(2);
			break;

		// Delete savegame
		case 4: {
			int32 imageIndex  = _app->visualListGetImageIndexClicked(1, 90002);
			int32 objectIndex = _app->visualListGetObjectIndexClicked(1, kPuzzleLoad);

			if (!objectIndex) {
				_app->messageHideQuestion(4);
				_app->messageGet("SelectGame");
				_app->messageShowWarning(0);
				_app->messageHideQuestion(4);
				break;
			}

			// Delete selected savegame
			uint32 slot = _app->visualListGetItemCount(1, kObjectMenuLoad) - ((uint32)imageIndex + 1);
			if (!_app->getSaveManager()->remove(slot)
			 || !removeSavedTimers(slot)) {
				_app->messageHideQuestion(4);
				_app->messageGet("CanNotDeleteSavedGame");
				_app->messageShowWarning(0);
				_app->messageHideQuestion(4);
				break;
			}

			// Hide delete question
			_app->messageHideQuestion(4);
			}
			break;

		case 5:
			_app->messageHideQuestion(4);
			break;
		}
		break;

	case kObjectMenuNewGame:
		_app->messageGet("DoYouWantToStartNewGame");
		_app->messageShowQuestion(2);
		break;

	case kObjectMenuPreferences:
		_app->puzzleSetActive(kPuzzlePreferences);

		_prefsVolume = _app->getPreferenceHandler()->getVolume();
		_prefsVolumeDialog = _app->getPreferenceHandler()->getVolumeDialog();
		_presentationIndexSY = _app->getPreferenceHandler()->getReverseStereo() == 1;
		_prefsSubtitles = _app->getPreferenceHandler()->getShowSubtitles();

		// Original forces subtitles on if no sound is available

		if (_prefsSubtitles) {
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 1);
		} else {
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 1);
		}

		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point((int16)(5 * _prefsVolume + 84), 155));
		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderDialog, 0, 0, Common::Point((int16)(5 * _prefsVolumeDialog + 84), 212));
		_app->objectPresentationHide(kObjectPreferences3dSound);
		_app->objectPresentationShow(kObjectPreferences3dSound, _presentationIndexSY);

		break;

	case kObjectPreferences3dSound:
		_presentationIndexSY = (_presentationIndexSY != 1) ? 1 : 0;
		_app->objectPresentationHide(kObjectPreferences3dSound);
		_app->objectPresentationShow(kObjectPreferences3dSound, _presentationIndexSY);
		break;

	case kObjectPreferencesSubtitles:
		if (target) {
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 1);
			_prefsSubtitles = false;
		} else {
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 1);
			_prefsSubtitles = true;
		}
		break;

	case kObjectPreferencesOk:
		_app->puzzleSetActive(kPuzzleGeneralMenu);
		_app->getPreferenceHandler()->save(_prefsVolume, _prefsVolumeDialog, _presentationIndexSY != 0 ? 2 : 0, _prefsSubtitles);
		break;

	case kObjectCredits:
		_app->showCredits();
		break;

	case kObjectMenuLoad: {
		// Get the list of games
		SaveStateList list = engine->listSaves(engine->getGameDescription()->desc.gameid);

		for (uint32 i = 0; i < list.size(); i++) {
			SaveStateDescriptor desc = list[i];

			_app->objectAdd((ObjectId)(90500 + i), Common::String::format("#%d", i), desc.description(), 1);
			_app->visualListAdd(1, kPuzzleLoad, (ObjectId)(90500 + i));
		}
		}
		break;

	case kObjectMenuSave: {
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		// Reset name and description
		_app->getSaveManager()->getName()->clear();

		_app->objectPresentationSetTextToPuzzle(kObjectSaveName, 0, 0, *_app->getSaveManager()->getName());
		_app->objectPresentationSetTextCoordinatesToPuzzle(kObjectSaveName, 0, 0, Common::Point(344, 181));
		_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSaveName, 0, Common::Point(346, 181));

		// Prepare savegame description
		TimeDate date;
		g_system->getTimeAndDate(date);
		Common::String description = Common::String::format("%s %02d:%02d:%02d %02d/%02d/%d",
		                                                    _app->getZoneName(_app->getCurrentGameZone()).c_str(),
		                                                    date.tm_hour, date.tm_min, date.tm_sec,
		                                                    date.tm_mon, date.tm_mday, date.tm_year + 1900);

		// We only save the current zone here
		_app->getSaveManager()->setDescription(_app->getZoneName(_app->getCurrentGameZone()));

		_app->objectPresentationSetTextToPuzzle(kObjectSaveName, 0, 1, description);
		_app->objectPresentationSetTextCoordinatesToPuzzle(kObjectSaveName, 0, 1, Common::Point(344, 155));

		// Store the thumbnail image
		if (_app->getThumbnail() != NULL) {
			Image *thumbnail = _app->getThumbnail()->zoom(0.40645f, 1.0f);
			_app->getSaveManager()->setThumbnail(thumbnail);
			delete thumbnail;
		}

		_app->puzzleSetActive(kPuzzleSave);
		}
		break;

	case kObjectMenuContinue:
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		_app->exitZone();
		_app->initZones();

		if (!_app->getSaveManager()->loadSave(0, kLoadSaveRead)) {
			_app->exitZone();
			_app->initZones();
			_app->loadPreferences();
			_app->messageGet("CanNotCountineGame"); // Typo is normal :S
			_app->messageShowWarning(0);
		}
		break;

	case kObjectLoadOk: {
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		int32 imageIndex  = _app->visualListGetImageIndexClicked(1, 90002);
		if (imageIndex == -1) {
			_app->messageGet("SelectGame");
			_app->messageShowWarning(0);
			break;
		}

		// Compute slot
		uint32 slot = _app->visualListGetItemCount(1, kObjectMenuLoad) - ((uint32)imageIndex + 1);

		// Get a list of saves
		SaveStateList list = engine->listSaves(engine->getGameDescription()->desc.gameid);

		// Prepare loading of game
		_app->visualListRemove(1, kPuzzleLoad, true);
		_app->exitZone();
		_app->initZones();

		// Get info about game
		Common::String name = list[slot].description();

		// Try loading the game, and fallback to autosave if it doesn't work
		if (_app->getSaveManager()->loadSave(slot, kLoadSaveRead)) {
			// Try to reload auto-save
			if (!_app->getSaveManager()->loadSave(0, kLoadSaveRead)) {
				_app->exitZone();
				_app->initZones();
				_app->loadPreferences();
			}

			_app->messageFormat("CanNotLoadGame", name);
			_app->messageShowWarning(0);
		}
		}
		break;

	case kObjectSaveOk: {
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		// Compute the next slot to save to
		uint32 slot = _app->getSaveManager()->getNextSlot();

        Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
		if (!saveFileMan->copySavefile(SaveManager::getSavegameFile(0), SaveManager::getSavegameFile(slot))
		 || !copySavedTimers(slot)) {
			_app->exitZone();
			_app->initZones();

			// Try reloading autosave
			if (_app->getSaveManager()->loadSave(0, kLoadSaveRead))
				break;

			_app->exitZone();
			_app->initZones();
			_app->loadPreferences();

			_app->messageFormat("CanNotSaveGame", *_app->getSaveManager()->getName());
			_app->messageShowWarning(0);
		}
		}
		break;

	case kObjectLoadCancel:
		_app->puzzleSetActive(kPuzzleGeneralMenu);
		_app->visualListRemove(1, kPuzzleLoad, true);
		break;

	case kObjectMenuStatus:
		_app->puzzleSetActive(kPuzzleGameStatus);
		break;

	case kObjectStatusOk:
	case kObjectSaveCancel:
	case kObjectPreferencesCancel:
		_app->puzzleSetActive(kPuzzleGeneralMenu);
		break;

	case kObjectMenuExit:
		if (_app->puzzleSetMod(kPuzzleMenu, 2, 2)) {
			_app->objectPresentationShow(kObject2, 0);
			_app->objectSetAccessibilityOn(kObject2);
		}
		break;

	// Original also handles clicks on Object 90911 (that doesn't exist)
	case kObject90912:
		// Check current Cd
		if (target > 0 && target <=8) {
			// Original checks the cd number in data/cd.ini

			_app->setZoneAndEnableBag((ZoneId)target);
			_app->setZone((ZoneId)target, _app->getSaveManager()->getSetupType());
		}
		break;
	}
}

void EventHandlerRing::onButtonUpZoneNI(ObjectId id, uint32 target, Id puzzleRotationId, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject10100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			g_system->warpMouse(505, 205);
		}
		break;

	case kObject10104:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectTile) {
				if (!_app->varGetByte(10102)) {
					_app->soundPlay(10106);
					_app->varSetByte(10102, 1);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
					_app->objectPresentationShow(kObject10100, 1);
					_app->bagRemove(kObjectTile);
					_app->objectSetAccessibilityOff(kObject10104);
					_app->objectSetAccessibilityOn(kObject10109);
					_app->objectSetAccessibilityOn(kObject10110);
				}
			}

			_app->cursorDelete();
		}
		break;

	case kObject10105:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectBrutality) {
				if (!_app->varGetByte(10103)) {
					_app->varSetByte(10103, 1);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->playMovie("1510");
					_app->objectPresentationShow(kObject10100, 2);
					_app->soundPlay(10104);
					_app->objectPresentationShow(kObject10102, 2);
					_app->objectSetAccessibilityOff(kObject10100);
					_app->objectSetAccessibilityOff(kObject10103);
					_app->objectSetAccessibilityOff(kObject10101);
					_app->objectSetAccessibilityOff(kObject10102);
					_app->objectSetAccessibilityOff(kObject10105, 1, 1);
				}
			}
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(10103) == 1)
			_app->puzzleSetActive(kPuzzle10102);
		break;

	case kObjectGlug:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 1:
				if (_app->bagGetClickedObject() == kObjectGlug) {
					_app->playMovie("1513");
					_app->bagRemove(kObjectGlug);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->puzzleSetActive(kPuzzle10002);
					_app->varSetByte(10000, 1);
					_app->objectPresentationShow(kObjectGlug);
					_app->objectPresentationPauseAnimationFrame(kObjectGlug, 0, 1, 1000, 0);
					_app->soundPlay(10800, kSoundLoop);
					_app->cursorDelete();
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectMinerals) {
					if (!_app->bagHas(kObjectGlug)) {
						_app->soundStop(10800, 1024);
						_app->playMovie("1514");
						_app->rotationSetMovabilityOff(10003, 0, 0);
						_app->rotationSetMovabilityOff(10201, 0, 0);
						_app->rotationSetMovabilityOff(10401, 0, 0);
						_app->rotationSetMovabilityOn(_app->varGetDword(10000), 0, 0);

						if (!_app->soundIsPlaying(10901)) {
							_app->soundPlay(10900);
							_app->soundPlay(10901, kSoundLoop);
						}

						if (_app->varGetDword((int)10000) == 10401) {
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
						} else {
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
						}
					}
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 3:
			_app->varSetDword(10000, puzzleRotationId);
			_app->varSetDword(10001, (uint32)_app->rotationGetAlp(puzzleRotationId));
			_app->puzzleSetActive(kPuzzle10000);

			waitTicks(200);

			if (_app->varGetByte(10000)) {
				_app->puzzleSetActive(kPuzzle10002);
				_app->soundPlay(10800, kSoundLoop);
				_app->soundPlay(10803);
			} else {
				_app->puzzleSetActive(kPuzzle10001);
				_app->soundPlay(10803);
			}
			break;

		case 4:
			_app->puzzleSetActive(kPuzzle10000);

			waitTicks(200);

			_app->soundPlay(10804);
			_app->rotationSetAlp(_app->varGetDword(10000), _app->varGetDword(10001));
			_app->rotationSetBet(_app->varGetDword(10000), 60.0f);
			_app->rotationSetRan(_app->varGetDword(10000), 85.7f);
			_app->rotationSetActive(_app->varGetDword(10000));
			break;
		}
		break;

	case kObject10108:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		break;

	case kObject10110:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->soundPlay(target + 10032);
		break;

	case kObject10109:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (!target) {
			_app->soundPlay(10106);
			_app->objectPresentationHide(kObject10109, 0);

			handleEvents();

			if (_app->varGetByte(10108) != 5
			 || _app->varGetByte(10109) != 2
			 || _app->varGetByte(10110) != 3
			 || _app->varGetByte(10111) != 4
			 || _app->varGetByte(10112) != 1) {
				_app->varSetByte(10108, 0);
				_app->varSetByte(10109, 0);
				_app->varSetByte(10110, 0);
				_app->varSetByte(10111, 0);
				_app->varSetByte(10112, 0);
				_app->varSetByte(10113, 0);
			} else {
				_app->objectSetAccessibilityOff(kObject10109);
				_app->objectSetAccessibilityOff(kObject10110);
				_app->puzzleAddBackgroundImage(kPuzzle10100, "NIS01N01P02.0001.bmp", Common::Point(0, 16), true);
				_app->varSetByte(10113, 9);
				_app->objectPresentationHide(kObject10101, 0);
				_app->objectPresentationShow(kObject10101, 5);
				_app->objectPresentationShow(kObject10101, 6);
				_app->objectPresentationHide(kObject10109);
			}
		}
		break;

	case kObject10200:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectSetAccessibilityOff(kObject10200);
		_app->soundPlay(10022);
		break;

	case kObject10300:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectBrutality) {
			_app->playMovie("1512");

			if (_app->varGetByte(10301) == 1) {
				_app->varSetByte(10301, 2);
				_app->puzzleSetActive(kPuzzle10390);
				_app->soundPlay(10014);
				_app->cursorDelete();
				break;
			}

			_app->puzzleSetActive(kPuzzle10390);
			_app->soundPlay(rnd(3) + 10015);
		}

		_app->cursorDelete();
		break;

	case kObjectMagicFrog:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(10301, 0);
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10010);
		break;

	case kObjectTile:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10012);
		break;

	case kObjectLogeTear:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(10301, 0);
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10005);
		break;

	case kObject10440:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(10106) != 1
		 || _app->varGetByte(10431) != 1) {
			if (_app->varGetByte(10106)) {
				_app->playMovie("1516");

				if (_app->varGetByte(10106) == 1)
					if (!_app->soundIsPlaying(10409))
						_app->soundPlay(10409, kSoundLoop);

				_app->rotationSetAlp(10415, 270.0f);
				_app->rotationSetBet(10415, 0.3f);
				_app->rotationSetRan(10415, 85.7f);
				_app->rotationSetActive(10415);
			} else {
				_app->playMovie("1515");
				_app->rotationSetAlp(10405, 270.0f);
				_app->rotationSetBet(10405, 0.3f);
				_app->rotationSetRan(10405, 85.7f);
				_app->rotationSetActive(10405);
			}
		} else {
			_app->soundPlay(10414);
		}
		break;

	case kObject10430:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectLogeTear) {
				if (target == 1) {
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->objectPresentationShow(kObject10430, 6);
					_app->objectPresentationShow(kObject10430, 1);
					_app->bagRemove(kObjectLogeTear);
					_app->varSetByte(10430, 1);
				}
			}

			_app->cursorDelete();
			sub_445A10();
			break;
		}

		_app->varSetByte(10431, 0);

		switch (target) {
		default:
			sub_445A10();
			break;

		case 0:
			_app->objectPresentationHide(kObject10430);
			_app->puzzleSetMovabilityOff(10411);
			_app->objectSetAccessibilityOff(kObject10430, 0, 0);

			if (_app->varGetByte(10430)) {
				_app->objectPresentationShow(kObject10430, 4);
				_app->objectPresentationShow(kObject10430, 1);
			} else {
				_app->objectPresentationShow(kObject10430, 2);
			}

			_app->objectPresentationShow(kObject10430, 0);

			sub_445A10();
			break;

		case 1:
			if (_app->varGetByte(10430) == 1) {
				_app->objectPresentationHide(kObject10430, 1);
				_app->objectPresentationHide(kObject10430, 6);
				_app->bagAdd(kObjectLogeTear);
				_app->varSetFloat(90005, _app->varGetFloat(90005) - 3.0f);
				_app->varSetByte(10430, 0);
			}

			sub_445A10();
			break;
		}
		break;

	case kObject10450:
		if (_app->bagHasClickedObject()) {
			if (_app->varGetByte(10106) == 1) {
				if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
					_app->playMovie("1517");
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 8.0f);
					_app->rotationSetAlp(10406, 270.0f);
					_app->rotationSetBet(10406, 0.3f);
					_app->rotationSetRan(10406, 85.7f);
					_app->rotationSetActive(10406);
					_app->objectSetAccessibilityOff(kObject10450);
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1518");
				_app->exitToMenu(kMenuAction2);
				break;
			}
			break;
		}

		if (_app->varGetByte(10106) == 1) {
			_app->playMovie("1518");
			_app->exitToMenu(kMenuAction2);
		} else {
			_app->playMovie("1519");
			_app->exitToMenu(kMenuAction1);
		}
		break;

	case kObject10460:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->playMovie("1520");
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
		onSwitchZoneRH(0);
		break;

	case kObjectAntiGCells:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->playMovie("1521");
		_app->varSetByte(10501, 1);
		_app->bagAdd(kObjectAntiGCells);
		break;

	case kObjectDivingHelmet:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (!_app->varGetByte(10500)) {
			_app->objectPresentationShow(kObjectDivingHelmet);
			_app->varSetByte(10500, 1);
			_app->objectSetAccessibilityOff(kObjectDivingHelmet);
			_app->bagAdd(kObjectDivingHelmet);
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);

			if (_app->bagHas(kObjectMagicFrog)) {
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->bagRemove(kObjectMagicFrog);
				_app->bagRemove(kObjectDivingHelmet);
				_app->bagAdd(kObjectDivingHelmet2);
				_app->playMovie("1522");
			}
		}
		break;

	case kObject10600: {
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		int16 val1 = _app->varGetWord(10600) + 36;
		if (val1 > 47)
			val1 -= 48;

		int16 val2 = _app->varGetWord(10602) + 24;
		if (val2 > 47)
			val2 -= 48;

		uint32 index = 0;
		if (_app->varGetWord(10600) == 12)
			index = 1;
		if (!_app->varGetWord(10601))
			++index;
		if (_app->varGetWord(10602) == 24)
			++index;

		while (index < 3) {
			if (!_app->soundIsPlaying(10401))
				_app->soundPlay(10401);

			if (_app->varGetWord(10600) != 12) {
				_app->varSetWord(10600, (val1 <= 36 ? -1 : 1) + _app->varGetWord(10600));

				if (_app->varGetWord(10600) > 47)
					_app->varSetWord(10600, 0);

				if (_app->varGetWord(10600) < 0)
					_app->varSetWord(10600, 47);

				if (_app->varGetWord(10600) == 12)
					++index;

				_app->objectPresentationHideAndRemove(kObject10601);
				_app->objectPresentationShow(kObject10601, (uint16)_app->varGetWord(10600));
			}

			if (_app->varGetWord(10601)) {
				_app->varSetWord(10601, (_app->varGetWord(10601) <= 24 ? -1 : 1) + _app->varGetWord(10601));

				if (_app->varGetWord(10601) > 47)
					_app->varSetWord(10601, 0);

				if (_app->varGetWord(10601) < 0)
					_app->varSetWord(10601, 47);

				if (!_app->varGetWord(10601))
					++index;

				_app->objectPresentationHideAndRemove(kObject10602);
				_app->objectPresentationShow(kObject10602, (uint16)_app->varGetWord(10601));
			}

			if (_app->varGetWord(10602) != 24) {
				_app->varSetWord(10602, (val2 <= 24 ? -1 : 1) + _app->varGetWord(10602));

				if (_app->varGetWord(10602) > 47)
					_app->varSetWord(10602, 0);

				if (_app->varGetWord(10602) < 0)
					_app->varSetWord(10602, 47);

				if (_app->varGetWord(10602) == 24)
					++index;

				_app->objectPresentationHideAndRemove(kObject10603);
				_app->objectPresentationShow(kObject10603, (uint16)_app->varGetWord(10602));
			}

			handleEvents();
		}

		_app->rotationSetActive(10601);
		}
		break;
	}
}

void EventHandlerRing::onButtonUpZoneRH(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectDivingHelmet2:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
				_app->playMovie("1668");
				_app->soundPlay(23010, kSoundLoop);
				_app->rotationSetAlp(20701, 0.0);
				_app->rotationSetActive(20701);
			}

			_app->cursorDelete();
		} else {
			_app->playMovie("1669");
			_app->exitToMenu(kMenuAction3);
		}
		break;

	case kObject20000:
	case kObject20001:
	case kObject20002:
	case kObject20003:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target) {
			_app->puzzleSetActive(kPuzzle20000);
			_app->objectPresentationShow(kObject21001, 0);
			_app->soundPlay(_app->varGetByte(21001) + 20031);
		} else {
			_app->puzzleSetActive(kPuzzle20000);
			_app->objectPresentationShow(kObject21001, 0);
			_app->soundPlay(_app->varGetByte(21001) + 20021);
		}
		break;

	case kObjectKeyIndifference:
	case kObjectKeyMistrust:
	case kObjectKeySelfishness:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(id);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
		_app->objectPresentationHide(id);
		_app->objectSetAccessibilityOff(id);
		_app->rotationSetMovabilityOff(10 * (_app->varGetByte(21001) + 2001), 0, 0);
		_app->rotationSetMovabilityOn(10 * (_app->varGetByte(21001) + 2001), 1, 1);
		_app->rotationSetActive(10 * (_app->varGetByte(21001) + 2001));
		if (_app->bagHas(kObjectKeyIndifference)
		 && _app->bagHas(kObjectKeyMistrust)
		 && _app->bagHas(kObjectKeySelfishness)
		 && _app->bagHas(kObjectKeyDisgust)) {
			_app->rotationSetMovabilityOn(20101, 1, 1);
			_app->soundPlay(23009);
		}
		break;

	case kObjectKeyDisgust:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				_app->bagAdd(kObjectKeyDisgust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->objectPresentationHide(kObjectKeyDisgust, 0);
				_app->objectSetAccessibilityOff(kObjectKeyDisgust, 0, 1);

				if (_app->bagHas(kObjectKeyIndifference)
				 && _app->bagHas(kObjectKeyMistrust)
				 && _app->bagHas(kObjectKeySelfishness)
				 && _app->bagHas(kObjectKeyDisgust)) {
					_app->rotationSetMovabilityOn(20101, 1, 1);
					_app->soundPlay(23009);
				}
			}
			break;
		}

		_app->cursorDelete();
		_app->setField74(false);
		break;

	case kObjectRedfish:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectRedfish) {
				_app->bagRemove(kObjectRedfish);
				_app->varSetByte(20200, 1);
				_app->objectSetAccessibilityOn(kObjectDolphin, 5, 5);
				_app->objectSetAccessibilityOff(kObjectDolphin, 3, 4);
				_app->objectPresentationShow(kObjectRedfish, 0);
			}

			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		if (target == 1 && _app->varGetByte(20200) == 1) {
			if (_app->varGetByte(20202)) {
				_app->setField74(false);
				_app->playMovie("1694");
			} else {
				_app->playMovie("1693");
				_app->bagAdd(kObjectRedfish);
				_app->varSetByte(20200, 0);
				_app->objectSetAccessibilityOff(kObjectDolphin, 5, 5);
				_app->objectSetAccessibilityOn(kObjectDolphin, 3, 4);
				_app->objectPresentationHide(kObjectRedfish, 0);
			}
		}
		break;

	case kObjectDolphin:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 0:
				if (_app->bagGetClickedObject() == kObjectMedallion) {
					_app->varSetByte(20202, 0);
					_app->bagRemove(kObjectMedallion);
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 0);
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectRedfish) {
					_app->bagRemove(kObjectRedfish);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->objectSetAccessibilityOff(kObjectRedfish);
					_app->varSetByte(20202, 2);
					_app->playMovie("1688");
					_app->objectPresentationHide(kObjectDolphin);
					_app->puzzleSetActive(kPuzzle20203);
					_app->soundPlay(20201);
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20201) == 1) {
				if (!_app->varGetByte(20202)) {
					_app->varSetByte(20202, 1);
					_app->bagAdd(kObjectMedallion);
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 1);
				}
			}
			break;

		case 1:
			if (_app->varGetByte(20201)) {
				if (_app->varGetByte(20202)) {
					_app->playMovie("1690");
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 1);
				} else {
					_app->playMovie("1689");
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 0);
				}
			} else {
				_app->varGetByte(20202);
			}

			_app->puzzleSetActive(kPuzzle20202);
			break;

		case 9:
			_app->playMovie(_app->varGetByte(20202) ? "1692" : "1691");
			_app->rotationSetActive(20201);
			break;
		}
		break;

	case kObject20204:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyDisgust) {
				_app->bagRemove(kObjectKeyDisgust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->objectSetAccessibilityOff(kObject20204);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20202);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20301:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectMedallion) {
				if (target == 2) {
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->bagRemove(kObjectMedallion);
					_app->playMovie("1676");
					_app->objectSetAccessibilityOff(kObject20301, 1, 1);
					_app->rotationSetActive(20301);
				}
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 1:
			_app->rotationSetRolTo(20301, 42.0f, 23.0f, 85.7f);
			_app->playMovie("1679");
			_app->puzzleSetActive(kPuzzle20301);
			_app->soundPlay(20301);
			break;

		case 9:
			_app->playMovie("1680");
			_app->rotationSetActive(20301);
			break;
		}

		if (_app->varGetByte(20202) == 2) {
			if (!_app->bagHas(kObjectMedallion)) {
				_app->playMovie("1678");
				_app->rotationSetAlp(20302, 10.0);
				_app->rotationSetActive(20302);
				break;
			}
		}

		_app->playMovie("1677");
		_app->exitToMenu(kMenuAction1);
		break;

	case kObject20302:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->bagHas(kObjectMedallion)) {
				_app->playMovie("1682");
				_app->rotationSetAlp(20303, 10.0);
				_app->rotationSetActive(20303);
				break;
			}

			_app->playMovie("1681");
			_app->exitToMenu(kMenuAction1);
			break;

		case 1:
			_app->rotationSetRolTo(20302, 320.0f, 26.0f, 85.7f);
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
			_app->objectSetAccessibilityOff(kObject20302, 1, 2);
			_app->playMovie("1683", 0.0);
			_app->puzzleSetActive(kPuzzle20302);
			_app->soundPlay(20302);
			break;
		}
		break;

	case kObject20303:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 1:
				if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
					_app->objectPresentationShow(kObject20303, 0);
					_app->playMovie("1684");
					_app->soundPlay(23010, kSoundLoop);
					_app->puzzleSetActive(kPuzzle20303);
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectMedallion) {
					_app->varSetByte(20301, 1);
					_app->objectSetAccessibilityOn(kObject20303, 0, 0);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->objectSetAccessibilityOff(kObject20303, 1, 2);
					_app->objectPresentationHide(kObject20303, 0);
					_app->soundPlay(20303);
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20301) == 1) {
				_app->playMovie("1685");
				_app->rotationSetActive(20304);
			}
			break;

		case 1:
			_app->rotationSetRolTo(20303, 140.0f, 26.0f, 85.7f);
			_app->playMovie("1686");
			_app->exitToMenu(kMenuAction3);
			break;

		case 9:
			_app->soundStop(23010, 1024);
			_app->playMovie("1687");
			_app->rotationSetActive(20303);
			break;
		}
		break;

	case kObject20304:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyMistrust) {
				_app->bagRemove(kObjectKeyMistrust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->objectSetAccessibilityOff(kObject20304);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20304);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20401:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectBrutality) {
				_app->playMovie("1675");
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
				_app->puzzleSetActive(kPuzzle20401);
			}

			_app->cursorDelete();
		}
		break;

	case kObjectAntiGCells2:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectMedallion) {
				_app->bagRemove(kObjectMedallion);
				_app->objectPresentationHide(kObject20402, 1);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->soundPlay(20401);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20404:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeySelfishness) {
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->bagRemove(kObjectKeySelfishness);
				_app->objectSetAccessibilityOff(kObject20404);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20402);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20501:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20500)) {
				_app->puzzleSetActive(kPuzzle20502);
			} else {
				_app->varSetByte(20500, 1);
				_app->soundStop(23011, 1024);
				_app->puzzleSetActive(kPuzzle20501);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->soundPlay(20501);
			}
			break;

		case 1:
			if (_app->varGetByte(20500) >= 2) {
				if (_app->varGetByte(20500) == 3) {
					_app->playMovie("1671");
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->rotationSetAlp(20503, 0.0);
					_app->rotationSetActive(20503);
				}
			} else {
				_app->playMovie("1670");
				_app->exitToMenu(kMenuAction4);
			}
			break;

		case 2:
			if (_app->varGetByte(20500) == 2) {
				_app->varSetByte(20500, 3);
				_app->soundStop(23011, 1024);
				_app->playMovie("1672");
				_app->objectSetAccessibilityOn(kObject20501, 1, 1);
				_app->objectSetAccessibilityOff(kObject20501, 2, 2);
				_app->puzzleSetActive(kPuzzle20503);
				_app->soundPlay(20503);
			}
			break;

		case 3:
			_app->playMovie("1673");
			_app->varSetByte(20500, 2);
			_app->objectPresentationHide(kObject20501, 0);
			_app->objectSetAccessibilityOff(kObject20501, 0, 1);
			_app->objectSetAccessibilityOn(kObject20501, 2, 2);
			break;

		case 9:
			_app->rotationSetActive(20501);
			break;
		}
		break;

	case kObject20502:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyIndifference) {
				_app->bagRemove(kObjectKeyIndifference);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->varSetByte(20500, 4);
				_app->objectSetAccessibilityOff(kObject20502);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20504);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20700:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(20500) == 4) {
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 1.0f);
			_app->soundStopAll(1024);
			_app->playMovie("1666");
			_app->bagRemove(kObjectAntiGCells2);
			onSwitchZoneNI(3);
		}
		else {
			_app->playMovie("1667");
			_app->exitToMenu(kMenuAction2);
		}
		break;
	}
}

void EventHandlerRing::onButtonUpZoneFO(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject30001:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKey) {
				if (!target) {
					_app->objectPresentationShow(kObject30001, 0);
					_app->objectSetAccessibilityOff(kObject30001, 0, 0);
					_app->objectSetAccessibilityOn(kObject30001, 1, 3);

					if (_app->varGetByte(30040) != 1
					 || _app->varGetByte(30041) != 1
					 || _app->varGetByte(30042) != 1) {
						if (_app->varGetByte(30040) == 1) {
							_app->objectPresentationShow(kObject30001, 1);
							_app->objectSetAccessibilityOff(kObject30001, 1, 1);
						}

						if (_app->varGetByte(30041) == 1) {
							_app->objectPresentationShow(kObject30001, 2);
							_app->objectSetAccessibilityOff(kObject30001, 2, 2);
						}

						if (_app->varGetByte(30042) == 1) {
							_app->objectPresentationShow(kObject30001, 3);
							_app->objectSetAccessibilityOff(kObject30001, 3, 3);
						}
					} else {
						_app->objectPresentationShow(kObject30001, 4);
						_app->objectSetAccessibilityOn(kObject30001, 4, 4);
					}
				}
			}

			if (_app->bagGetClickedObject() != kObjectIngot
			 && _app->bagGetClickedObject() != kObjectIngot2
			 && _app->bagGetClickedObject() != kObjectIngot3) {
				_app->cursorDelete();
				break;
			}

			switch (target) {
			default:
				_app->cursorDelete();
				break;

			case 1:
				_app->varSetByte(30040, 1);
				_app->objectPresentationShow(kObject30001, 1);
				_app->objectSetAccessibilityOff(kObject30001, 1, 1);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;

			case 2:
				_app->varSetByte(30041, 1);
				_app->objectPresentationShow(kObject30001, 2);
				_app->objectSetAccessibilityOff(kObject30001, 2, 2);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;

			case 3:
				_app->varSetByte(30042, 1);
				_app->objectPresentationShow(kObject30001, 3);
				_app->objectSetAccessibilityOff(kObject30001, 3, 3);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;
			}

			_app->playMovie("1167");
			_app->objectPresentationHide(kObject30001, 1);
			_app->objectPresentationHide(kObject30001, 2);
			_app->objectPresentationHide(kObject30001, 3);
			_app->objectPresentationShow(kObject30001, 4);
			_app->objectSetAccessibilityOn(kObject30001, 4, 4);
			_app->objectSetAccessibilityOn(kObjectSleepingPotion, 1, 1);
			_app->cursorDelete();

		} else {
			if (target == 4) {
				_app->bagAdd(kObjectWolfBadge);
				_app->objectPresentationHide(kObject30001, 4);
				_app->objectSetAccessibilityOff(kObject30001, 4, 4);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.0f);
				_app->varSetByte(30043, 1);
			}
		}
		break;

	case kObject30002: // byte: 30009 / sound: 30110
	case kObject30003: // byte: 30010 / sound: 30111
	case kObject30004: // byte: 30011 / sound: 30112
	case kObject30005: // byte: 30012 / sound: 30113
	case kObject30006: // byte: 30013 / sound: 30114
	case kObject30007: // byte: 30014 / sound: 30115
	case kObject30008: // byte: 30015 / sound: 30116
		if (_app->bagHasClickedObject()) {
			switch (_app->bagGetClickedObject()) {
			default:
				break;

			case kObjectPatience:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 1);
					_app->bagRemove(kObjectPatience);
					_app->playMovie("1168");
				}
				break;

			case kObjectMovementAndIntuition:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 2);
					_app->bagRemove(kObjectMovementAndIntuition);
					_app->playMovie("1168");
				}
				break;

			case kObjectLove:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 3);
					_app->bagRemove(kObjectLove);
					_app->playMovie("1168");
				}
				break;

			case kObjectImagination:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 4);
					_app->bagRemove(kObjectImagination);
					_app->playMovie("1168");
				}
				break;

			case kObjectDestruction:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 5);
					_app->bagRemove(kObjectDestruction);
					_app->playMovie("1168");
				}
				break;

			case kObjectJudgementAndDirection:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 6);
					_app->bagRemove(kObjectJudgementAndDirection);
					_app->playMovie("1168");
				}
				break;

			case kObjectWill:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 7);
					_app->bagRemove(kObjectWill);
					_app->playMovie("1168");
				}
				break;
			}

			if (_app->varGetByte(30009) == 1
			 && _app->varGetByte(30010) == 2
			 && _app->varGetByte(30011) == 3
			 && _app->varGetByte(30012) == 4
			 && _app->varGetByte(30013) == 5
			 && _app->varGetByte(30014) == 6
			 && _app->varGetByte(30015) == 7) {
				_app->objectSetAccessibilityOff(kObject30002);
				_app->objectSetAccessibilityOff(kObject30003);
				_app->objectSetAccessibilityOff(kObject30004);
				_app->objectSetAccessibilityOff(kObject30005);
				_app->objectSetAccessibilityOff(kObject30006);
				_app->objectSetAccessibilityOff(kObject30007);
				_app->objectSetAccessibilityOff(kObject30008);
				_app->playMovie("1170");
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
				_app->rotationSetActive(30302);
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);
			}

			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(id + 7)) {
			_app->bagAdd((ObjectId)(_app->varGetByte(id + 7) + 30008));
			_app->playMovie("1171");
			_app->soundPlay(id + 108);
			_app->varSetByte(id + 7, 0);
		}
		break;

	case kObject30027:
		if (_app->bagHasClickedObject())
			break;

		if (!_app->varGetByte(30016)) {
			_app->playMovie("1172");
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);

			for (uint32 i = 0; i < 27; i++)
				_app->soundSetVolume(30300 + i, 90);

			_app->varSetByte(30017, 0);
			_app->varSetByte(30035, 1);
			_app->rotationSetMovabilityOn(30402, 2, 2);
			_app->objectPresentationShow(kObject30050, 0);
			_app->objectPresentationShow(kObject30050, 2);
			_app->rotationSetActive(30501);
			_app->rotationSetAlp(30501, 88.0);
			_app->rotationSetBet(30501, 13.0);
			_app->objectSetAccessibilityOff(kObject30002);
			_app->objectSetAccessibilityOff(kObject30003);
			_app->objectSetAccessibilityOff(kObject30004);
			_app->objectSetAccessibilityOff(kObject30005);
			_app->objectSetAccessibilityOff(kObject30006);
			_app->objectSetAccessibilityOff(kObject30007);
			_app->objectSetAccessibilityOff(kObject30008);
		}
		break;

	case kObjectBerries:
		if (_app->bagHasClickedObject()) {
			if (target == 5) {
				switch (_app->bagGetClickedObject()) {
				default:
					break;

				case kObjectSleepingBerries:
					if (!_app->varGetByte(30060)) {
						_app->bagRemove(kObjectSleepingBerries);
						_app->varSetByte(30063, 1);
						_app->objectPresentationShow(kObjectBerries, 4);
					}
					break;

				case kObjectBerries:
					if (!_app->varGetByte(30063)) {
						_app->bagRemove(kObjectBerries);
						_app->varSetByte(30060, 1);
						_app->objectPresentationShow(kObjectBerries, 4);
					}
					break;
				}
			}

			_app->cursorDelete();
			break;
		}


		switch (target) {
		default:
			break;

		case 0:
		case 4:
			if (_app->varGetByte(30017) == 1) {
				_app->bagAdd(kObjectSleepingBerries);

				if (!_app->varGetByte(30201)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
					_app->varSetByte(30201, 1);
				}

				_app->objectPresentationHide(kObjectBerries, 3);
				_app->varSetByte(30075, 1);
				_app->objectSetAccessibilityOff(kObjectBerries, 0, 0);
				_app->bagAdd(kObjectIngot2);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
				_app->objectPresentationHide(kObjectBerries, 0);
				_app->objectPresentationHide(kObjectBerries, 2);
				_app->varSetByte(30076, 1);
			}
			break;

		case 1:
		case 2:
		case 3:
			_app->bagAdd(kObjectBerries);
			break;

		case 5:
			if (!_app->varGetByte(30060) && !_app->varGetByte(30063))
				_app->playMovie("1173");

			if (_app->varGetByte(30060) != 1 || _app->varGetByte(30061)) {
				if (_app->varGetByte(30060) == 1 && _app->varGetByte(30061) == 1) {
					_app->bagAdd(kObjectBerriesJuice);
					_app->objectPresentationHide(kObjectBerries, 5);
					_app->varSetByte(30060, 0);
					_app->varSetByte(30061, 0);
				}
			} else {
				_app->playMovie("1174");
				_app->varSetByte(30061, 1);
				_app->objectPresentationHide(kObjectBerries, 4);
				_app->objectPresentationShow(kObjectBerries, 5);
			}

			if (_app->varGetByte(30063) != 1 || _app->varGetByte(30064)) {
				if (_app->varGetByte(30063) == 1 && _app->varGetByte(30064) == 1) {
					_app->bagAdd(kObjectSleepingPotion2);

					if (!_app->varGetByte(30203)) {
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
						_app->varSetByte(30203, 1);
					}

					_app->objectPresentationHide(kObjectBerries, 5);
					_app->varSetByte(30063, 0);
					_app->varSetByte(30064, 0);
				}
			} else {
				_app->playMovie("1175");
				_app->varSetByte(30064, 1);
				_app->objectPresentationHide(kObjectBerries, 4);
				_app->objectPresentationShow(kObjectBerries, 5);
			}
			break;
		}
		break;

	case kObjectSleepingPotion:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectHare && target == 0) {
				_app->bagRemove(kObjectHare);
				_app->objectSetAccessibilityOff(kObjectSleepingPotion, 0, 0);
				_app->playMovie("1176");
				_app->timerStop(kTimer2);
				_app->soundStop(30501, 1024);

				if (!_app->varGetByte(30077)) {
					_app->playMovie("1177");
					_app->bagAdd(kObjectIngot3);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
					_app->varSetByte(30077, 1);
				}
			}

			_app->cursorDelete();

			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->playMovie("1178");
			break;

		case 1:
			if (_app->varGetByte(30074)) {
				_app->playMovie("1179");
				_app->rotationSetActive(30101);
				_app->rotationSetAlp(30101, 130.0);
				_app->rotationSetBet(30101, 20.0);
			} else {
				if (_app->varGetByte(30056))
					_app->puzzleSetActive(kPuzzle35100);
				else
					_app->playMovie("1178");
			}
			break;
		}
		break;

	case kObjectBow:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(30511);
			_app->objectPresentationShow(kObjectBow);
			_app->objectSetAccessibilityOff(kObjectBow, 0, 0);
			if (_app->varGetByte(30050) == 1)
				_app->objectPresentationHide(kObjectBow, 1);
			if (_app->varGetByte(30051) == 1)
				_app->objectPresentationHide(kObjectBow, 2);
			if (_app->varGetByte(30052) == 1)
				_app->objectPresentationHide(kObjectBow, 3);
			if (_app->varGetByte(30053) == 1)
				_app->objectPresentationHide(kObjectBow, 4);
			if (_app->varGetByte(30054) == 1)
				_app->objectPresentationHide(kObjectBow, 5);
			if (_app->varGetByte(30055) == 1)
				_app->objectPresentationHide(kObjectBow, 6);
			break;

		case 1:
		case 2:
		case 3:
		case 4:
			_app->soundPlay(30512);
			_app->objectPresentationHide(kObjectBow, target);
			_app->objectSetAccessibilityOff(kObjectBow, target, target);
			_app->varSetByte(target + 30049, 1);

			_app->bagAdd(kObjectHare);
			if (!_app->varGetByte(30204)) {
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				_app->varSetByte(30204, 1);
			}
			break;

		case 5:
		case 6:
			_app->soundPlay(30512);
			_app->objectPresentationHide(kObjectBow, target);
			_app->objectSetAccessibilityOff(kObjectBow, target, target);
			_app->varSetByte(target + 30049, 1);

			_app->bagAdd(kObjectBow);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
			_app->objectPresentationHide(kObjectBow, 5);
			_app->objectPresentationHide(kObjectBow, 6);
			_app->objectSetAccessibilityOff(kObjectBow, 5, 6);
			_app->varSetByte(30054, 1);
			_app->varSetByte(30055, 1);
			break;
		}
		break;

	case kObject30028:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				switch (_app->bagGetClickedObject()) {
				default:
					break;

				case kObjectLogeTear2:
					if (_app->varGetByte(30019)) {
						_app->playMovie("1181");
						_app->bagRemove(kObjectLogeTear2);
						_app->varSetByte(30021, 1);
						if (!_app->varGetByte(30020)) {
							_app->objectPresentationHide(kObject30028, 0);
							_app->objectPresentationShow(kObject30028, 1);
						}
					} else {
						_app->playMovie("1180");
					}
					break;

				case kObjectMould:
					if (_app->varGetByte(30019) == 1) {
						if (_app->varGetByte(30021) == 1) {
							_app->objectPresentationHide(kObject30028, 0);
							_app->objectPresentationShow(kObject30028, 2);
							_app->varSetByte(30020, 1);
							_app->bagRemove(kObjectMould);
						}
					}
					break;

				case kObjectMetals:
					if (_app->varGetByte(30020) == 1) {
						_app->bagRemove(kObjectMetals);
						_app->bagAdd(kObjectMeltedGold);
						_app->bagAdd(kObjectMeltedSilver);
						_app->bagAdd(kObjectMeltedCopper);
						_app->bagAdd(kObjectMeltedLead);
						_app->bagAdd(kObjectMeltedSteel);
						_app->bagAdd(kObjectMeltedTin);
						_app->bagAdd(kObjectQuicksilver);
						_app->bagAdd(kObjectLogeTear2);

						if (!_app->varGetByte(30206)) {
							_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
							_app->varSetByte(30206, 1);
						}

						_app->bagAdd(kObjectMould);
						_app->playMovie("1182");
						_app->objectPresentationHide(kObject30028);
						_app->objectPresentationHide(kObject30028, 3);
						_app->varSetByte(30019, 0);
						_app->varSetByte(30020, 0);
						_app->varSetByte(30021, 0);
						_app->objectSetAccessibilityOn(kObject30028, 0, 0);
					}
					break;
				}
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (!_app->varGetByte(30020)) {
				_app->playMovie("1183");
				_app->objectPresentationShow(kObject30028, 0);
				_app->objectPresentationShow(kObject30028, 3);
				_app->varSetByte(30019, 1);
				_app->objectSetAccessibilityOff(kObject30028, 0, 0);
			}
			break;

		case 1:
			if (_app->varGetByte(30021) == 1) {
				if (!_app->varGetByte(30020)) {
					_app->playMovie("1184");
					_app->objectPresentationHide(kObject30028);
					_app->objectPresentationShow(kObject30028, 0);
					_app->varSetByte(30021, 0);
					_app->bagAdd(kObjectLogeTear2);
				}
			}

			if (_app->varGetByte(30021) == 1) {
				if (_app->varGetByte(30020) == 1) {
					_app->objectPresentationHide(kObject30028, 2);
					_app->objectPresentationShow(kObject30028, 1);
					_app->varSetByte(30020, 0);
					_app->bagAdd(kObjectMould);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				}
			}
			break;
		}
		break;

	case kObject30040:
		if (_app->bagHasClickedObject())
			break;

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(30022))
				break;

			if (_app->varGetByte(30024)) {
				_app->objectPresentationShow(kObject30040, 0);
			} else {
				_app->objectPresentationShow(kObject30040, 1);
				_app->objectSetAccessibilityOn(kObject30040, 4, 4);
			}

			_app->varSetByte(30022, 1);
			_app->objectSetAccessibilityOff(kObject30040, 0, 0);
			_app->objectSetAccessibilityOff(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 2, 2);
			_app->soundPlay(30508);
			break;

		case 1:
			if (_app->varGetByte(30023))
				break;

			_app->objectPresentationShow(kObject30040, 2);
			_app->varSetByte(30023, 1);
			_app->objectSetAccessibilityOff(kObject30040, 0, 0);
			_app->objectSetAccessibilityOff(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 3, 3);
			_app->objectSetAccessibilityOn(kObject30040, 5, 5);
			_app->soundPlay(30508);
			break;

		case 2:
			if (_app->varGetByte(30022) != 1)
				break;

			_app->objectPresentationHide(kObject30040, 0);
			_app->objectPresentationHide(kObject30040, 1);
			_app->varSetByte(30022, 0);
			_app->objectSetAccessibilityOff(kObject30040, 2, 2);
			_app->objectSetAccessibilityOff(kObject30040, 4, 4);
			_app->objectSetAccessibilityOn(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 0, 0);
			_app->soundPlay(30508);
			break;

		case 3:
			if (_app->varGetByte(30023) != 1)
				break;

			_app->objectPresentationHide(kObject30040, 2);
			_app->varSetByte(30023, 0);
			_app->objectSetAccessibilityOff(kObject30040, 3, 3);
			_app->objectSetAccessibilityOff(kObject30040, 5, 5);
			_app->objectSetAccessibilityOn(kObject30040, 0, 0);
			_app->objectSetAccessibilityOn(kObject30040, 1, 1);
			_app->soundPlay(30508);
			break;

		case 4:
			_app->bagAdd(kObjectMould);
			_app->objectPresentationShow(kObject30040, 0);
			_app->objectPresentationHide(kObject30040, 1);
			_app->objectSetAccessibilityOff(kObject30040, 4, 4);
			_app->varSetByte(30024, 1);
			break;

		case 5:
			_app->bagAdd(kObjectMetals);

			if (!_app->varGetByte(30205)) {
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				_app->varSetByte(30205, 1);
			}

			_app->objectSetAccessibilityOff(kObject30040, 5, 5);
			_app->objectSetAccessibilityOff(kObject30040, 3, 3);
			_app->objectPresentationShow(kObject30040, 4);
			break;
		}
		break;

	case kObject30042:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				if (_app->varGetByte(30019)) {
					_app->objectPresentationShow(kObject30042, 0);
					_app->objectPresentationHide(kObject30042, 8);
					_app->objectPresentationHide(kObject30028, 0);
					_app->objectPresentationHide(kObject30028, 3);
					_app->varSetByte(30019, 0);
					_app->objectSetAccessibilityOn(kObject30042, 1, 7);
					_app->objectSetAccessibilityOff(kObject30042, 0, 0);
					_app->objectSetAccessibilityOn(kObject30028, 0, 0);
					_app->playMovie("1187");
				} else {
					_app->objectPresentationShow(kObject30042, 8);
					_app->objectPresentationHide(kObject30042, 0);
					_app->objectSetAccessibilityOff(kObject30042, 0, 0);
					_app->soundPlay(30508);
				}
			}
			break;
		}

		if (target >= 1 && target <= 7) {
			_app->soundPlay(30509);

			switch (_app->bagGetClickedObject()) {
			default:
				break;

			case kObjectMeltedGold:
				_app->varSetByte(30025, (byte)target);
				_app->bagRemove(kObjectMeltedGold);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case  kObjectMeltedSilver:
				_app->varSetByte(30026, (byte)target);
				_app->bagRemove(kObjectMeltedSilver);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedCopper:
				_app->varSetByte(30027, (byte)target);
				_app->bagRemove(kObjectMeltedCopper);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedLead:
				_app->varSetByte(30028, (byte)target);
				_app->bagRemove(kObjectMeltedLead);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedSteel:
				_app->varSetByte(30029, (byte)target);
				_app->bagRemove(kObjectMeltedSteel);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedTin:
				_app->varSetByte(30030, (byte)target);
				_app->bagRemove(kObjectMeltedTin);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectQuicksilver:
				_app->varSetByte(30031, (byte)target);
				_app->bagRemove(kObjectQuicksilver);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;
			}
		}

		if (_app->varGetByte(30025) != 1
		 || _app->varGetByte(30026) != 2
		 || _app->varGetByte(30027) != 3
		 || _app->varGetByte(30028) != 4
		 || _app->varGetByte(30029) != 5
		 || _app->varGetByte(30030) != 6
		 || _app->varGetByte(30031) != 7) {
			if (_app->varGetByte(30025)
			 && _app->varGetByte(30026)
			 && _app->varGetByte(30027)
			 && _app->varGetByte(30028)
			 && _app->varGetByte(30029)
			 && _app->varGetByte(30030)
			 && _app->varGetByte(30031)) {
				_app->objectPresentationHide(kObject30042);
				_app->playMovie("1186");
				_app->rotationSetActive(30601);
				_app->objectSetAccessibilityOn(kObject30042, 0, 0);
				_app->varSetByte(30025, 0);
				_app->varSetByte(30026, 0);
				_app->varSetByte(30027, 0);
				_app->varSetByte(30028, 0);
				_app->varSetByte(30029, 0);
				_app->varSetByte(30030, 0);
				_app->varSetByte(30031, 0);
			}
		} else {
			_app->objectPresentationHide(kObject30042);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 9.9f);
			_app->varSetByte(30032, 1);
			_app->playMovie("1185");
			_app->bagAdd(kObjectGolem);
			_app->rotationSetActive(30601);
			_app->objectSetAccessibilityOff(kObject30042);
		}

		_app->cursorDelete();
		break;

	case kObject30044:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectGolem) {
			switch (target) {
			default:
				break;

			case 0:
				_app->objectPresentationShow(kObject30044);
				_app->objectSetAccessibilityOff(kObject30044, 0, 0);
				_app->objectSetAccessibilityOn(kObject30044, 1, 1);
				_app->bagRemove(kObjectGolem);
				_app->playMovie("1188");
				break;

			case 1:
				_app->objectPresentationShow(kObject30044);
				break;
			}
		}

		if (_app->bagGetClickedObject() == kObjectSleepingPotion2) {
			if (target == 1) {
				_app->objectPresentationHide(kObject30044);
				_app->objectSetAccessibilityOff(kObject30044, 1, 1);
				_app->bagAdd(kObjectGolem);
				_app->bagAdd(kObjectIngot);
				_app->playMovie("1189");
				_app->rotationSetMovabilityOff(30601, 9, 9);
				_app->objectSetAccessibilityOff(kObject30044);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
			}
		}

		_app->cursorDelete();
		break;

	case kObject30045:
		if (_app->bagHasClickedObject())
			break;

		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 0);
			_app->soundPlay(30150);
			break;

		case 1:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 1);
			_app->soundPlay(30151);
			break;

		case 2:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 2);
			_app->soundPlay(30154);
			break;

		case 3:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 3);
			_app->soundPlay(30152);
			break;

		case 4:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 4);
			_app->soundPlay(30155);
			break;

		case 5:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 5);
			_app->soundPlay(30156);
			break;

		case 6:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 6);
			_app->soundPlay(30153);
			break;

		case 7:
			_app->soundPlay(30201);
			_app->varSetByte(30047, 1);
			_app->rotationSetRolTo(30602, 175.0f, -23.0f, 85.3f);
			break;
		}
		break;

	case kObjectFishingRod:
		if (_app->bagHasClickedObject()) {
			if (target == 3) {
				if (_app->bagGetClickedObject() == kObjectFishingRodWithWorms) {
					if (_app->varGetByte(30034)) {
						_app->bagAdd(kObjectKey);
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
						_app->playMovie("1191");
					} else {
						_app->bagAdd(kObjectFish);

						if (!_app->varGetByte(30207)) {
							_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);
							_app->varSetByte(30207, 1);
						}

						_app->playMovie("1190");
					}

					_app->bagAdd(kObjectFishingRod);
					_app->bagRemove(kObjectFishingRodWithWorms);
					_app->varSetByte(30038, 0);
				}
				if (_app->bagGetClickedObject() == kObjectFishingRod)
					_app->playMovie("1192");
			}

			_app->cursorDelete();

			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(30508);

			if (_app->varGetByte(30033)) {
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 2, 2);
			} else {
				_app->objectPresentationHide(kObjectFishingRod, 0);
				_app->objectPresentationShow(kObjectFishingRod, 1);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 1, 2);
			}

			_app->varSetByte(30070, 1);
			break;

		case 1:
			if (_app->varGetByte(30038)) {
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->bagAdd(kObjectFishingRodWithWorms);

				if (!_app->varGetByte(30208)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
					_app->varSetByte(30208, 1);
				}

				_app->bagRemove(kObjectWorms);
				_app->varSetByte(30033, 1);
			} else {
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->bagAdd(kObjectFishingRod);
				_app->varSetByte(30033, 1);
			}
			break;

		case 2:
			_app->soundPlay(30511);

			if (_app->varGetByte(30070)) {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 2);
				_app->varSetByte(30070, 0);
			} else {
				if (_app->varGetByte(30033)) {
					_app->objectPresentationShow(kObjectFishingRod, 0);
					_app->varSetByte(30070, 0);
				} else {
					_app->objectPresentationShow(kObjectFishingRod, 1);
					_app->varSetByte(30070, 0);
				}
			}
			break;
		}
		break;

	case kObjectWorms:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != kObjectGolem || _app->varGetByte(30038)) {
				_app->cursorDelete();
				break;
			}

			if (_app->varGetByte(30033)) {
				_app->bagAdd(kObjectFishingRodWithWorms);

				if (!_app->varGetByte(30208)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
					_app->varSetByte(30208, 1);
				}

				_app->playMovie("1193");
				_app->bagRemove(kObjectFishingRod);
				_app->varSetByte(30038, 1);

				if (_app->varGetByte(30076)) {
					_app->cursorDelete();
					break;
				}
			} else {
				_app->bagAdd(kObjectWorms);

				if (!_app->varGetByte(30209)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);
					_app->varSetByte(30209, 1);
				}

				_app->playMovie("1193");
				_app->varSetByte(30038, 1);

				if (_app->varGetByte(30076)) {
					_app->cursorDelete();
					break;
				}
			}

			_app->bagAdd(kObjectIngot2);
			_app->bagAdd(kObjectSleepingBerries);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3);
			_app->varSetByte(30076, 1);
			_app->varSetByte(30039, 1);
			_app->playMovie("1194");
			_app->objectPresentationHide(kObjectWorms, 0);
			_app->objectPresentationHide(kObjectWorms, 1);
			_app->objectPresentationHide(kObjectBerries, 0);
			_app->objectSetAccessibilityOff(kObjectWorms);
			_app->cursorDelete();
		}
		break;

	case kObject30050:
		if (!_app->bagHasClickedObject()) {
			if (!_app->varGetByte(30035)) {
				_app->objectPresentationShow(kObject30050, 1);
				_app->soundPlay(30514);
			}

			if (_app->varGetByte(30035) == 1) {
				if (_app->varGetByte(30037) == 1) {
					_app->playMovie("1195");
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
					_app->objectPresentationShow(kObject30051, 2);
					_app->objectPresentationHide(kObject30050);
					_app->varSetByte(30035, 0);
					_app->varSetByte(30036, 1);
					_app->varSetByte(30037, 0);
					_app->rotationSetMovabilityOff(30402, 2, 2);
					_app->rotationSetMovabilityOff(30011, 1, 1);
				}
			}
		}
		break;

	case kObject30051:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				_app->cursorDelete();
				break;

			case 0:
				if (_app->bagGetClickedObject() == kObjectBurningArrow) {
					_app->bagRemove(kObjectBurningArrow);
					_app->bagAdd(kObjectBow);
					_app->varSetByte(30034, 1);
					_app->playMovie("1196", target);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
					_app->objectPresentationHide(kObject30051, 2);
					_app->objectSetAccessibilityOff(kObject30051, target, target);
					_app->objectSetAccessibilityOff(kObject30051, 3, 3);
				}

				if (_app->bagGetClickedObject() != kObjectBow) {
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1197");
				_app->cursorDelete();
				break;

			case 1:
			case 2:
				if (_app->bagGetClickedObject() == kObjectBow) {
					_app->bagAdd(kObjectBurningArrow);

					if (!_app->varGetByte(30210)) {
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
						_app->varSetByte(30210, 1);
					}

					_app->bagRemove(kObjectBow);
				}
				break;

			case 3:
				if (_app->bagGetClickedObject() == kObjectBurningArrow) {
					_app->playMovie("1198");
					_app->bagRemove(kObjectBurningArrow);
					_app->bagAdd(kObjectBow);
				}

				if (_app->bagGetClickedObject() != kObjectBow) {
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1197");
				_app->cursorDelete();
				break;
			}
		}
		break;

	case kObject30102:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				if (_app->bagGetClickedObject() == kObjectWolfBadge) {
					if (_app->varGetByte(30072) == 1) {
						_app->puzzleSetActive(kPuzzle35104);
						_app->soundPlay(30105);
					}
				}
			}

			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			if (_app->varGetByte(30072)) {
				if (!_app->varGetByte(30078)) {
					_app->puzzleSetActive(kPuzzle35103);
					_app->soundPlay(30118);
				}
			} else {
				_app->rotationSetRolTo(30101, 130.0f, 20.0f, 85.3f);
				_app->puzzleSetActive(kPuzzle35104);
				_app->soundPlay(30102);
			}
		}
		break;

	case kObject30109:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->puzzleSetActive(kPuzzle35109);
		break;

	case kObject30108:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != kObjectSleepingPotion2) {
				_app->cursorDelete();
				break;
			}

			_app->playMovie("1199");
			_app->objectPresentationHide(kObject30110, 1);
			_app->objectPresentationShow(kObject30110, 2);
			_app->rotationSetActive(30101);
			_app->rotationSetAlp(30101, 130.0f);
			_app->rotationSetBet(30101, 20.0f);
			_app->objectSetAccessibilityOff(kObject30102, 0, 0);
			_app->varSetByte(30072, 1);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
			_app->cursorDelete();
		}
		break;

	case kObjectNotung:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectWolfBadge
			 || _app->bagGetClickedObject() == kObjectWolfBrooch) {
				if (_app->bagGetClickedObject() == kObjectWolfBrooch) {
					_app->bagRemove(kObjectWolfBrooch);
					_app->varSetByte(30067, 1);
				} else {
					_app->bagRemove(kObjectWolfBadge);
					_app->varSetByte(30066, 1);
				}

				if (_app->varGetByte(30066) == 1
				 && _app->varGetByte(30067) == 1) {
					_app->objectPresentationShow(kObjectNotung, 3);
					_app->playMovie("1200");
					_app->varSetFloat(90007, 100.0f);
					_app->rotationSetAlp(30001, 180.0);
					_app->rotationSetActive(30001);
					_app->soundSetMultiplier(kSoundTypeAmbientMusic, 0);
					_app->timerStopAll();
					_app->soundPlay(30007);
					_app->objectSetAccessibilityOff(kObject30001);
					_app->objectSetAccessibilityOff(kObject30002);
					_app->objectSetAccessibilityOff(kObject30003);
					_app->objectSetAccessibilityOff(kObject30004);
					_app->objectSetAccessibilityOff(kObject30005);
					_app->objectSetAccessibilityOff(kObject30006);
					_app->objectSetAccessibilityOff(kObject30007);
					_app->objectSetAccessibilityOff(kObject30008);
					_app->objectSetAccessibilityOff(kObjectPatience);
					_app->objectSetAccessibilityOff(kObject30016);
					_app->objectSetAccessibilityOff(kObject30027);
					_app->objectSetAccessibilityOff(kObjectBerries);
					_app->objectSetAccessibilityOff(kObjectSleepingBerries);
					_app->objectSetAccessibilityOff(kObjectWolfInstinct);
					_app->objectSetAccessibilityOff(kObjectHare);
					_app->objectSetAccessibilityOff(kObjectIngot);
					_app->objectSetAccessibilityOff(kObjectIngot2);
					_app->objectSetAccessibilityOff(kObjectIngot3);
					_app->objectSetAccessibilityOff(kObjectSleepingPotion);
					_app->objectSetAccessibilityOff(kObjectBow);
					_app->objectSetAccessibilityOff(kObject30028);
					_app->objectSetAccessibilityOff(kObjectMould);
					_app->objectSetAccessibilityOff(kObjectLogeTear2);
					_app->objectSetAccessibilityOff(kObjectBerriesJuice);
					_app->objectSetAccessibilityOff(kObjectSleepingPotion2);
					_app->objectSetAccessibilityOff(kObjectMeltedGold);
					_app->objectSetAccessibilityOff(kObjectMeltedSilver);
					_app->objectSetAccessibilityOff(kObjectMeltedCopper);
					_app->objectSetAccessibilityOff(kObjectMeltedLead);
					_app->objectSetAccessibilityOff(kObjectMeltedSteel);
					_app->objectSetAccessibilityOff(kObjectMeltedTin);
					_app->objectSetAccessibilityOff(kObjectQuicksilver);
					_app->objectSetAccessibilityOff(kObject30040);
					_app->objectSetAccessibilityOff(kObjectMetals);
					_app->objectSetAccessibilityOff(kObject30042);
					_app->objectSetAccessibilityOff(kObjectGolem);
					_app->objectSetAccessibilityOff(kObject30044);
					_app->objectSetAccessibilityOff(kObject30045);
					_app->objectSetAccessibilityOff(kObjectFishingRod);
					_app->objectSetAccessibilityOff(kObjectFishingRodWithWorms);
					_app->objectSetAccessibilityOff(kObjectFish);
					_app->objectSetAccessibilityOff(kObjectWorms);
					_app->objectSetAccessibilityOff(kObject30050);
					_app->objectSetAccessibilityOff(kObject30051);
					_app->objectSetAccessibilityOff(kObject30052);
					_app->objectSetAccessibilityOff(kObjectBurningArrow);
					_app->objectSetAccessibilityOff(kObjectKey);
					_app->objectSetAccessibilityOff(kObjectWolfBadge);
					_app->objectSetAccessibilityOff(kObjectWolfBrooch);
					_app->objectSetAccessibilityOff(kObject30110);
					_app->objectSetAccessibilityOff(kObjectNotung);
					_app->objectSetAccessibilityOff(kObject30059);
					_app->rotationSetMovabilityOff(30001);
					_app->rotationSetMovabilityOff(30002);
					_app->rotationSetMovabilityOff(30003);
					_app->rotationSetMovabilityOff(30004);
					_app->rotationSetMovabilityOff(30005);
					_app->rotationSetMovabilityOff(30006);
					_app->rotationSetMovabilityOff(30008);
					_app->rotationSetMovabilityOff(30009);
					_app->rotationSetMovabilityOff(30010);
					_app->rotationSetMovabilityOff(30011);
					_app->rotationSetMovabilityOff(30012);
					_app->rotationSetMovabilityOff(30101);
					_app->rotationSetMovabilityOff(30301);
					_app->rotationSetMovabilityOff(30302);
					_app->rotationSetMovabilityOff(30303);
					_app->rotationSetMovabilityOff(30401);
					_app->rotationSetMovabilityOff(30402);
					_app->rotationSetMovabilityOff(30501);
					_app->rotationSetMovabilityOff(30601);
					_app->rotationSetMovabilityOff(30602);
					_app->rotationSetMovabilityOff(30701);
					_app->rotationSetMovabilityOff(30702);
					_app->rotationSetMovabilityOff(30703);
					_app->rotationSetMovabilityOff(30704);
					_app->rotationSetMovabilityOff(30801);
					_app->objectPresentationShow(kObject6, 0);
					_app->objectPresentationShow(kObject6, 5);
					_app->soundPlay(30120);
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1201");
				_app->objectPresentationShow(kObjectNotung, 2);
			}
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(30066) == 1
		 || _app->varGetByte(30067) == 1) {
			_app->playMovie("1202");
			_app->objectPresentationHide(kObjectNotung, 2);

			if (_app->varGetByte(30066) == 1) {
				_app->bagAdd(kObjectWolfBadge);
				_app->varSetByte(30066, 0);
			} else {
				_app->bagAdd(kObjectWolfBrooch);
				_app->varSetByte(30067, 0);
			}
		}
		break;

	case kObject30059:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->playMovie(target ? "1204" : "1203");

		if (!_app->varGetByte(30211)) {
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
			_app->varSetByte(30211, 1);
		}
		break;

	case kObject30100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundSetVolume(30506, rnd(10) + 91);
			_app->puzzleSetMovabilityOff(kPuzzle35100, 0, 0);
			_app->soundPlay(30506);
			_app->objectSetAccessibilityOff(kObject30100, 0, 0);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject30100, 1, 1);
			_app->puzzleSetActive(kPuzzle35101);
			_app->soundPlay(30100);
			break;
		}
		break;

	case kObject30200:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->soundPlay(target + 30162);
		break;
	}
}

void EventHandlerRing::onButtonUpZoneRO(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject40010:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				if (_app->varGetByte(40000)) {
					if (!_app->varGetByte(40701)
					 && !_app->varGetByte(40702)
					 && !_app->varGetByte(40703)) {
						_app->puzzleSetActive(kPuzzle40011);
						_app->objectPresentationShow(kObject40011);
						_app->objectSetAccessibilityOn(kObject40011);
					}

					if (_app->varGetByte(40701) == 1
					 && !_app->varGetByte(40702)
					 && !_app->varGetByte(40703))
						_app->puzzleSetActive(kPuzzle40013);

					if (_app->varGetByte(40701) == 1
					 && _app->varGetByte(40702) == 1
					 && !_app->varGetByte(40703))
						_app->puzzleSetActive(kPuzzle40012);

				} else {
					_app->puzzleSetActive(kPuzzle40010);
				}
			}
			break;
		}

		switch (target) {
		default:
			_app->cursorDelete();
			break;

		case 0:
			if (_app->bagGetClickedObject() != kObject40000 || _app->varGetByte(40000)) {
				_app->cursorDelete();
				break;
			}

			_app->playMovie("1780");
			_app->varSetFloat(90006, 51.8f);
			_app->puzzleSetActive(kPuzzle40011);

			handleEvents();

			if (_app->varGetByte(40701)) {
				handleEvents();

				_app->varSetByte(40000, 1);
				_app->cursorDelete();
				break;
			}

			for (uint32 i = 2000; i >= 1; i--) {
				target = (rnd(4) + 1) + 10 * (rnd(4) + 2);

				if (_app->varGetByte(target + 40501)) {
					uint32 presentationIndex = (uint8)_app->varGetByte(target + 40501) / 10 - 2;
					uint32 imageIndex = (uint8)_app->varGetByte(target + 40501) % 10 - 1;
					Common::Point coords = _app->objectPresentationGetImageCoordinatesOnPuzzle(40011, presentationIndex, imageIndex);

					if (!_app->varGetByte(target + 40491)) {
						_app->varSetByte(target + 40491, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y - _app->varGetByte(40805)));
						continue;
					}

					if (!_app->varGetByte(target + 40511)) {
						_app->varSetByte(target + 40511, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y + _app->varGetByte(40805)));
						continue;
					}

					if (!_app->varGetByte(target + 40500)) {
						_app->varSetByte(target + 40500, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x - _app->varGetByte(40805), coords.y));
						continue;
					}

					if (!_app->varGetByte(target + 40502)) {
						_app->varSetByte(target + 40502, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x + _app->varGetByte(40805), coords.y));
						continue;
					}
				}
			}

			handleEvents();

			_app->varSetByte(40000, 1);
			_app->cursorDelete();
			break;

		case 2:
			if (_app->bagGetClickedObject() == kObjectRing) {
				_app->objectPresentationShow(kObject40010, 0);
				_app->bagRemove(kObjectRing);
				_app->varSetByte(40801, 1);
			}

			if (_app->bagGetClickedObject() == kObjectCrown && _app->varGetByte(40801) == 1) {
				_app->objectPresentationHide(kObject40010, 0);
				_app->playMovie("1781");
				_app->varSetFloat(90006, 78.6f);
				_app->bagRemove(kObjectCrown);
				_app->varSetByte(40703, 1);
				_app->puzzleSetActive(kPuzzle40103);
				_app->soundStop(40002, 1024);
				_app->puzzleSetActive(kPuzzle40101);
				_app->soundPlay(40706);
			}

			_app->cursorDelete();
			break;
		}
		break;

	case kObject40011:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(target + 40501)) {
			uint32 presentationIndex = (uint8)_app->varGetByte(target + 40501) / 10 - 2;
			uint32 imageIndex = (uint8)_app->varGetByte(target + 40501) % 10 - 1;
			Common::Point coords = _app->objectPresentationGetImageCoordinatesOnPuzzle(40011, presentationIndex, imageIndex);

			if (_app->varGetByte(target + 40491)) {
				if (!_app->varGetByte(target + 40511)) {
					_app->varSetByte(target + 40511, _app->varGetByte(target + 40501));
					_app->varSetByte(target + 40501, 0);
					_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y + _app->varGetByte(40805)));

					if (target == 12) {
						uint32 counter = 1;
						for (uint32 i = 0; i < (uint8)_app->varGetByte(40522); i++) {
							++counter;

							if (counter >= 5) {
								//counter = 1;

								// Original does some checks on a2, but they seem useless :S

								handleEvents();

								_app->objectPresentationHide(kObject40011);
								_app->objectSetAccessibilityOff(kObject40011);
								_app->objectSetAccessibilityOff(kObject40010, 1, 1);
								_app->objectSetAccessibilityOn(kObject40010, 2, 2);
								_app->varSetByte(40701, 1);
								_app->playMovie("1782");
								_app->varSetFloat(90006, 64.3f);
								_app->puzzleSetActive(kPuzzle40013);
								_app->objectPresentationShow(kObject40101, 0);
								_app->objectPresentationShow(kObject40102, 0);
								_app->objectPresentationShow(kObject40103, 0);
								_app->objectPresentationShow(kObject40104, 0);
								_app->objectPresentationShow(kObject40105, 0);
								_app->timerStart(kTimer0, 50);
								_app->timerStart(kTimer1, 30);
								break;
							}
						}
					}

					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				if (!_app->varGetByte(target + 40500)) {
					_app->varSetByte(target + 40500, _app->varGetByte(target + 40501));
					_app->varSetByte(target + 40501, 0);
					_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x - _app->varGetByte(40805), coords.y));
					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				if (_app->varGetByte(target + 40502)) {
					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				_app->varSetByte(target + 40502, _app->varGetByte(target + 40501));
				_app->varSetByte(target + 40501, 0);
				_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x + _app->varGetByte(40805), coords.y));
				_app->soundSetVolume(40103, rnd(20) + 80);
				_app->soundPlay(40103);
			} else {
				_app->varSetByte(target + 40491, _app->varGetByte(target + 40501));
				_app->varSetByte(target + 40501, 0);
				_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y - _app->varGetByte(40805)));
				_app->soundSetVolume(40103, rnd(20) + 80);
				_app->soundPlay(40103);
			}
		}
		break;

	case kObject40201:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectSetAccessibilityOff(kObject40060);
		_app->puzzleSetMovabilityOff(kPuzzle40060, 0, 0);
		if ((_presentationIndexRO / 10) == (int32)(target + 1)) {
			_app->objectPresentationShow(kObject40201, target);

			Common::String str = _app->varGetString(40901);
			while (str.size() > 6)
				str.deleteLastChar();

			_app->varSetString(40901, Common::String::format("%s%d", str.c_str(), target));
		} else {
			_app->varSetByte(target + 40200, _app->varGetByte(target + 40200) ? 0 : 1);
			_app->objectPresentationShow(kObject40201, target + 7);
			_app->soundSetVolume(40602, rnd(20) + 80);
			_app->soundPlay(40602);
		}
		break;
	}
}

void EventHandlerRing::onButtonUpZoneWA(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectFlower:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectFlower);
		_app->objectPresentationHide(kObjectFlower);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		_app->objectSetAccessibilityOff(kObjectFlower);
		break;

	case kObjectDragonSword:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectDragonSword);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		_app->objectPresentationShow(kObjectTree, 0);
		_app->objectSetAccessibilityOff(kObjectDragonSword);
		break;

	case kObjectApple:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectApple);
		_app->objectPresentationHide(kObjectTree, 5);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
		_app->objectSetAccessibilityOff(kObjectApple);
		break;

	case kObject50600:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50007)) {
				_app->objectPresentationHide(kObject50600, 2);
				_app->varSetByte(50007, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 2);
				_app->varSetByte(50007, 1);
			}
			break;

		case 1:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50008)) {
				_app->objectPresentationHide(kObject50600, 3);
				_app->varSetByte(50008, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 3);
				_app->varSetByte(50008, 1);
			}
			break;

		case 2:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50009)) {
				_app->objectPresentationHide(kObject50600, 4);
				_app->varSetByte(50009, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 4);
				_app->varSetByte(50009, 1);
			}
			break;

		case 3:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50010)) {
				_app->objectPresentationHide(kObject50600, 5);
				_app->varSetByte(50010, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 5);
				_app->varSetByte(50010, 1);
			}
			break;
		}
		break;

	case kObject50601:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectThread) {
				if (_app->varGetByte(50012) == 6) {
					_app->bagRemove(kObjectThread);
					_app->varSetFloat(90008, 100.0);

					_app->playMovie("1860");
					_app->playMovie("1861");

					switch (_app->getCurrentLanguage()) {
					default:
						_app->playMovieChannel("1862", _app->getLanguageChannel());
						break;

					case kLanguageItalian:
					case kLanguageSpanish:
					case kLanguageDutch:
						_app->playMovieChannel("1863", _app->getLanguageChannel());
						break;

					case kLanguageSwedish:
						_app->playMovieChannel("1864", _app->getLanguageChannel());
						break;
					}

					_app->puzzleSetActive(kPuzzle51001);
					_app->soundPlay(50021);
				}
			}

			_app->cursorDelete();
		}
		break;

	case kObject50700:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle50701);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
		_app->soundPlay(50009);
		break;

	case kObjectTotems:
		if (!_app->bagHasClickedObject()) {
			switch (target) {
			default:
				if (target > 9)
					_app->puzzleSetActive((PuzzleId)(target + 50491));
				break;

			case 0:
				if (_app->varGetWord(50000) % 10 == 1) {
					_app->bagAdd(kObjectFlower);
					_app->objectPresentationHide(kObjectTree, 1);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 1:
				if (_app->varGetWord(50000) % 100 > 1) {
					_app->bagAdd(kObjectApple);
					_app->objectPresentationHide(kObjectTree, 2);
					_app->varSetWord(50000, _app->varGetWord(50000) - 10);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 2:
				if (_app->varGetWord(50000) % 1000 > 11) {
					_app->bagAdd(kObjectDeadLeaf);
					_app->objectPresentationHide(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) - 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 3:
				if (_app->varGetWord(50000) % 10000 > 111) {
					_app->bagAdd(kObjectBark);
					_app->objectPresentationHide(kObjectTree, 4);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1000);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;
			}

			break;
		}

		if (!_app->bagHasClickedObject()) {
			switch (target) {
			default:
				if (target > 9)
					_app->puzzleSetActive((PuzzleId)(target + 50491));
				break;

			case 0:
				if (_app->varGetWord(50000) % 10 == 1) {
					_app->bagAdd(kObjectFlower);
					_app->objectPresentationHide(kObjectTree, 1);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 1:
				if (_app->varGetWord(50000) % 100 > 1) {
					_app->bagAdd(kObjectApple);
					_app->objectPresentationHide(kObjectTree, 2);
					_app->varSetWord(50000, _app->varGetWord(50000) - 10);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 2:
				if (_app->varGetWord(50000) % 1000 > 11) {
					_app->bagAdd(kObjectDeadLeaf);
					_app->objectPresentationHide(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) - 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 3:
				if (_app->varGetWord(50000) % 10000 > 111) {
					_app->bagAdd(kObjectBark);
					_app->objectPresentationHide(kObjectTree, 4);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1000);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;
			}

			break;
		}

		if (_app->bagGetClickedObject() != kObjectFlower || target)	{
			if (_app->bagGetClickedObject() != kObjectApple || target != 1) {
				if (_app->bagGetClickedObject() != kObjectDeadLeaf || target != 2) {
					if (_app->bagGetClickedObject() == kObjectBark && target == 3) {
						_app->bagRemove(_app->bagGetClickedObject());
						_app->objectPresentationShow(kObjectTree, 4);
						_app->varSetWord(50000, _app->varGetWord(50000) + 1000);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
					}
				} else {
					_app->bagRemove(_app->bagGetClickedObject());
					_app->objectPresentationShow(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) + 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
				}
			}
			else
			{
				_app->bagRemove(_app->bagGetClickedObject());
				_app->objectPresentationShow(kObjectTree, 2);
				_app->varSetWord(50000, _app->varGetWord(50000) + 10);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
			}
		}
		else
		{
			_app->bagRemove(kObjectFlower);
			_app->objectPresentationShow(kObjectTree, 1);
			_app->varSetWord(50000, _app->varGetWord(50000) + 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		}

		if (_app->varGetWord(50000) == 1111) {
			_app->varSetWord(50000, 11111);
			_app->objectPresentationShow(kObjectTree, 6);

			handleEvents();

			_app->playMovie("1851");
			_app->rotationSetActive(50502);
			_app->bagAdd(kObjectThread);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
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
		_app->cursorDelete();
		break;

	case kObjectGolem1:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectGolem1) {
				if (!target) {
					_app->bagRemove(kObjectGolem1);
					_app->objectPresentationShow(kObjectGolem1, 1);
					_app->objectPresentationShow(kObjectBackhead, 0);
					_app->objectPresentationShow(kObjectFronthead, 0);
					_app->objectPresentationShow(kObjectBelly, 0);
					_app->objectPresentationShow(kObjectRightArm, 0);
					_app->objectPresentationShow(kObjectLeftArm, 0);
					_app->objectPresentationShow(kObjectLegs, 0);
					_app->objectPresentationShow(kObjectHeart, 0);
					_app->objectSetAccessibilityOn(kObjectBackhead);
					_app->objectSetAccessibilityOn(kObjectFronthead);
					_app->objectSetAccessibilityOn(kObjectBelly);
					_app->objectSetAccessibilityOn(kObjectRightArm);
					_app->objectSetAccessibilityOn(kObjectLeftArm);
					_app->objectSetAccessibilityOn(kObjectLegs);
					_app->objectSetAccessibilityOn(kObjectHeart, 1, 1);
					_app->objectSetAccessibilityOff(kObjectGolem1, 0, 0);
					_app->puzzleSetMovabilityOff(kPuzzle50400);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
				}
			}
			_app->cursorDelete();
		}
		break;

	case kObject50105:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(50011) == 1) {
			_app->objectPresentationPauseAnimationFrame(kObject50100, 8, 50, 1000, 2);
			_app->varSetByte(50012, 5);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
			_app->objectPresentationShow(kObject50100, 8);
			_app->objectPresentationHide(kObject50100, 7);
		}
		break;

	case kObject50100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(50012) == 4) {
			_app->rotationSetRolTo(50108, 250.0, 15.0, 85.7f);
			_app->playMovie("1849");
			_app->puzzleSetActive(kPuzzle50100);
			_app->soundStop(51006, 1024);
		}

		if (_app->varGetByte(50012) > 4) {
			_app->playMovie("1850");
			_app->rotationSetAlp(50601, 250.0);
			_app->rotationSetActive(50601);
			_app->soundStop(51006, 1024);
		}
		break;

	case kObjectBark:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectBark);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
		_app->objectSetAccessibilityOff(kObjectBark);
		break;

	case kObjectCounch1:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != 50000 || _app->varGetByte(50005)) {
				if (_app->bagGetClickedObject() == 50501) {
					if (_app->varGetByte(50005) == 1) {
						_app->bagRemove(kObjectDragonSword);
						_app->rotationSetRolTo(50304, 145.0f, 9.0f, 85.7f);
						_app->varSetByte(50005, 2);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
						_app->rotationSetMovabilityOff(50304, 0, 0);
						_app->rotationSetMovabilityOn(50304, 1, 1);
						_app->objectSetAccessibilityOn(kObjectCounch1, 1, 1);
						_app->objectPresentationHide(kObjectCounch1);

						handleEvents();

						_app->playMovie("1853");
						_app->rotationSetAlp(50304, 190.0);
						_app->rotationSetBet(50304, 15.0);
						_app->rotationSetActive(50304);
						_app->objectPresentationShow(kObjectCounch1, 1);

						if (!_app->varGetByte(50003))
							_app->objectPresentationShow(kObjectCounch1, 4);
					}
				}
				_app->cursorDelete();

			} else {
				_app->objectPresentationShow(kObjectCounch1, 0);
				_app->varSetByte(50005, 1);
				_app->playMovie("1852");
				_app->rotationSetAlp(50304, 190.0);
				_app->rotationSetBet(50304, 15.0);
				_app->rotationSetActive(50304);
				_app->cursorDelete();
			}
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(50005) >= 2) {
				if (_app->varGetByte(50005) >= 2) {
					_app->objectPresentationShow(kObjectCounch1, 5);
					_app->objectSetAccessibilityOff(kObjectCounch1, 0, 0);
					_app->bagAdd(kObjectDragonSword);
				}
			}
			else {
				_app->puzzleSetActive(kPuzzle50304);
			}
			break;

		case 1:
			if (_app->varGetByte(50005) == 2) {
				_app->varSetByte(50005, 3);
				_app->objectPresentationShow(kObjectCounch1, 2);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->objectSetAccessibilityOff(kObjectCounch1, 1, 1);
				_app->bagAdd(kObjectCounch1);
			}
			break;
		}
		break;

	case kObjectPhoenix:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != 50000) {
				_app->rotationSetActive(50303);
				_app->cursorDelete();
				break;
			}

			_app->objectPresentationShow(kObjectPhoenix, 0);
			_app->objectSetAccessibilityOff(kObjectPhoenix);
			_app->objectSetAccessibilityOn(kObjectBark);
			_app->varSetByte(50003, 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			_app->soundPlay(51013, kSoundLoop);
			_app->rotationSetMovabilityOff(50301, 0, 1);
			_app->rotationSetMovabilityOn(50301, 2, 3);
			_app->rotationSetMovabilityOff(50302, 0, 2);
			_app->rotationSetMovabilityOn(50302, 3, 5);
			_app->rotationSetMovabilityOff(50303, 0, 0);
			_app->rotationSetMovabilityOn(50303, 1, 1);

			switch (_app->varGetByte(50005)) {
			default:
				_app->objectPresentationHide(kObjectCounch1);
				_app->objectPresentationShow(kObjectCounch1, 1);
				break;

			case 0:
			case 1:
				_app->objectPresentationShow(kObjectCounch1, 3);
				break;

			case 3:
				_app->objectPresentationHide(kObjectCounch1);
				_app->objectPresentationShow(kObjectCounch1, 1);
				_app->objectPresentationShow(kObjectCounch1, 2);
				break;
			}

			_app->objectPresentationShow(kObjectPhoenix, 0);
			_app->playMovie("1854");
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

			_app->rotationSetActive(50303);
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle50303);
		break;

	case kObjectRopes:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDragonSword) {
				_app->playMovie("1855");
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->objectPresentationShow(kObjectRopes, 0);
				_app->varSetByte(50001, 1);
				_app->objectSetAccessibilityOff(kObjectRopes);
			}

			_app->cursorDelete();
		}
		break;

	case kObjectCloud:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectCounch1) {
			if (_app->bagHas(kObjectFeather)) {
				_app->playMovie("1856");

				if (_app->varGetByte(50001) == 1) {
					_app->objectPresentationHide(kObjectRopes, 0);
					_app->objectPresentationShow(kObjectRopes, 1);
					_app->varSetByte(50001, 2);
					_app->playMovie("1857");
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
					_app->objectSetAccessibilityOff(kObjectCloud);

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
			} else {
				_app->playMovie("1859", 0.0);
			}
		}

		_app->cursorDelete();
		break;
	}
}

void EventHandlerRing::onButtonUpZoneAS(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject80019:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (!_app->varGetByte(90001)) {
				_app->timerStopAll();
				onSwitchZoneNI(0);
			}
			break;

		case 1:
			if (!_app->varGetByte(90002)) {
				_app->timerStopAll();
				onSwitchZoneN2(0);
			}
			break;

		case 2:
			if (!_app->varGetByte(90001)) {
				_app->timerStopAll();
				onSwitchZoneNI(0);
			}
			break;

		case 3:
			if (!_app->varGetByte(90003)) {
				_app->timerStopAll();
				onSwitchZoneFO(0);
			}
			break;

		case 4:
			if (!_app->varGetByte(90002)) {
				_app->timerStopAll();
				onSwitchZoneN2(0);
			}
			break;

		case 5:
			if (!_app->varGetByte(90004)) {
				_app->timerStopAll();
				onSwitchZoneWA(0);
			}
			break;
		}
		break;

	case kObject80021:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(80004) == 1) {
				if (_app->varGetByte(90001)) {
					_app->puzzleSetActive(kPuzzle80006);
					_app->playMovie("1142");
					_app->objectPresentationShow(kObject80019, 0);
				} else {
					_app->puzzleSetActive(kPuzzle80006);
					_app->playMovie("1141");
				}
			}

			if (_app->varGetByte(80004) == 11) {
				_app->puzzleSetActive(kPuzzle80007);
				_app->playMovie("1143");
			}

			if (_app->varGetByte(80004) == 21) {
				_app->puzzleSetActive(kPuzzle80009);
				_app->playMovie("1144");
			}

			if (_app->varGetByte(80004) == 31) {
				_app->puzzleSetActive(kPuzzle80008);
				_app->playMovie("1145");
			}

			if (_app->varGetByte(80004) == 41) {
				_app->puzzleSetActive(kPuzzle80010);
				_app->playMovie("1146");
			}
			break;

		case 1:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 10);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 2:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 20);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 3:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 30);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 4:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 40);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;
		}
		break;

	case kObject80007:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDeath) {
				_app->bagRemoveAll();
				_app->soundStopAll(1024);
				_app->timerStopAll();
				_app->setupZone(kZoneAS, kSetupType6);
			}

			_app->cursorDelete();
		}
		break;

	case kObject80012:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(80028);
			break;

		case 1:
			_app->soundPlay(80025);
			break;
		case 2:
			_app->soundPlay(80021);
			break;
		case 3:
			_app->soundPlay(80024);
			break;
		case 4:
			_app->soundPlay(80022);
			break;
		case 5:
			_app->soundPlay(80026);
			break;
		case 6:
			_app->soundPlay(80027);
			break;
		case 7:
			_app->soundPlay(80023);
			break;
		}
		break;

	case kObject80018:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(90001) == 1) {
				_app->puzzleSetActive(kPuzzle80002);
				_app->soundSetVolume(80201, 80);
				_app->soundPlay(80040);
			}
			break;

		case 1:
			if (_app->varGetByte(90002) == 1) {
				_app->puzzleSetActive(kPuzzle80003);
				_app->soundSetVolume(80203, 90);
				_app->soundPlay(80049);
			}
			break;
		case 2:
			if (_app->varGetByte(90003) == 1) {
				_app->puzzleSetActive(kPuzzle80004);
				_app->soundSetVolume(80204, 90);
				_app->soundPlay(80058);
			}
			break;
		case 3:
			if (_app->varGetByte(90004) == 1) {
				_app->puzzleSetActive(kPuzzle80005);
				_app->soundSetVolume(80205, 80);
				_app->soundPlay(80068);
			}
			break;
		}
		break;

	case kObject80022:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			onSwitchZoneAS(5);
		break;
	}
}

void EventHandlerRing::onButtonUpZoneN2(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectFire:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectFire) {
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 2.0f);
				_app->playMovie("1389");
				_app->rotationSetAlp(70500, 243.0f);
				_app->rotationSetRan(70500, 85.7f);
				_app->rotationSetActive(70500);
			}

			_app->cursorDelete();
		}
		break;

	case kObject70100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);

			g_system->warpMouse(505, 205);
		}
		break;

	case kObject70105:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectPresentationShow(kObject70106, (uint16)_app->varGetWord(70016));
		_app->puzzleSetActive(kPuzzle70102);

		break;

	case kObject70300:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectChrysoberyl) {
				_app->bagRemove(kObjectChrysoberyl);
				_app->playMovie("1495");
				_app->objectPresentationShow(kObjectCage);
				_app->bagAdd(kObjectCage);
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
			}

			_app->cursorDelete();
		}
		break;

	case kObject70404:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(70005, 0);

		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject70404);
			_app->puzzleSetMovabilityOff(kPuzzle70411);
			_app->objectSetAccessibilityOff(kObject70404, 0, 0);

			if (_app->varGetByte(70001)) {
				_app->objectPresentationShow(kObject70404, 4);
				_app->objectPresentationShow(kObject70404, 1);
			} else {
				_app->objectPresentationShow(kObject70404, 2);
			}

			_app->objectPresentationShow(kObject70404, 0);
			break;

		case 1:
			if (_app->varGetByte(70001) == 1) {
				_app->objectPresentationHide(kObject70404, 1);
				_app->bagAdd(kObjectLogeTear3);
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0);
				_app->objectPresentationHide(kObject70404, 6);
				_app->varSetByte(70001, 0);
			}
			break;
		}
		break;

	case kObjectCentaur:
	case kObjectDragon:
	case kObjectPhoenix1:
	case kObjectChrysoberyl:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				if (_app->bagGetClickedObject() == kObjectCentaur
				 || _app->bagGetClickedObject() == kObjectDragon
				 || _app->bagGetClickedObject() == kObjectPhoenix1) {
					_app->bagRemove(_app->bagGetClickedObject());
					_app->varSetByte(70015, (byte)(_app->bagGetClickedObject() - 100));
					_app->objectSetAccessibilityOff(kObjectCentaur);
					_app->objectSetAccessibilityOff(kObjectDragon);
					_app->objectSetAccessibilityOff(kObjectPhoenix1);
					_app->soundPlay(_app->varGetByte(70015) + 71010);
				}
			}

			_app->cursorDelete();

		} else {
			if (!target) {
				if (id == kObjectChrysoberyl) {
					_app->objectPresentationShow(kObjectChrysoberyl);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
				} else {
					_app->objectPresentationHide(id);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 3.0f);
				}

				_app->objectSetAccessibilityOff(id);
				_app->bagAdd(id);
			}
		}
		break;

	case kObject70700:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectFire) {
				if (target == 1) {
					_app->objectSetAccessibilityOff(id);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0);
					_app->objectPresentationHide(id);
					_app->soundStop(70701, 1024);
					_app->rotationSet3DSoundOff(70200, 70701);
					_app->playMovie("1494");
				}
			}

			_app->cursorDelete();

		} else {
			if (!target) {
				_app->varSetByte(70013, 31);
				_app->puzzleSetActive(kPuzzle70600);
				_app->soundPlay(70043);
			}
		}
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
		onButtonUp2ZoneWA(id, index, puzzleRotationId, a4, point);
		break;
	}
}

void EventHandlerRing::onButtonUp2ZoneWA(ObjectId objectId, uint32 index, Id, uint32, const Common::Point &) {
	switch (objectId) {
	default:
		break;

	case kObjectMagnet:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() >= kObject50451
			 && _app->bagGetClickedObject() <= kObject50457) {

				_app->objectPresentationSetImageCoordinatesOnPuzzle(_app->bagGetClickedObject(), 0, Common::Point((int16)(53 * index / 10 + 162), (int16)(37 * index % 10 + 115)));
				_app->varSetDword(index + 51000, _app->bagGetClickedObject());
				_app->objectSetAccessibilityOn(_app->bagGetClickedObject(), index / 10 + 1 + 7 * index % 10, index / 10 + 1 + 7 * index % 10);
				_app->objectSetAccessibilityOff(kObjectMagnet, index / 10 + 7 * index % 10, index / 10 + 7 * index % 10);
				_app->objectPresentationShow(_app->bagGetClickedObject());

				if (_app->varGetDword(51030) == 50451
				 && _app->bagGetClickedObject() == kObject50451)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51061) == 50452
				 && _app->bagGetClickedObject() == kObject50452)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51001) == 50453
				 && _app->bagGetClickedObject() == kObject50453)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51033) == 50454
				 && _app->bagGetClickedObject() == kObject50454)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51026) == 50455
				 && _app->bagGetClickedObject() == kObject50455)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51046) == 50456
				 && _app->bagGetClickedObject() == kObject50456)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51042) == 50457
				 && _app->bagGetClickedObject() == kObject50457)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetDword(51030) == 50451
				 && _app->varGetDword(51061) == 50452
				 && _app->varGetDword(51001) == 50453
				 && _app->varGetDword(51033) == 50454
				 && _app->varGetDword(51026) == 50455
				 && _app->varGetDword(51046) == 50456) {

					_app->objectSetAccessibilityOn(kObject50457, 0, 0);

					if (_app->varGetDword(51042) == 50457) {
						_app->playMovie("1867");
						_app->rotationSetMovabilityOff(50402, 2, 2);
						_app->bagAdd(kObjectFeather);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
						_app->rotationSetAlp(50402, 180.0f);
						_app->rotationSetBet(50402, 0.3f);
						_app->rotationSetActive(50402);
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
			}

			_app->cursorDelete();
		}

		_app->setField74(false);
		break;

	case kObject50451:
	case kObject50452:
	case kObject50453:
	case kObject50454:
	case kObject50455:
	case kObject50456:
	case kObject50457:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() < kObject50451
			 || _app->bagGetClickedObject() > kObject50457
			 || (index && !_app->varGetDword(index + 50999))) {
				_app->setField74(false);
				_app->cursorDelete();
				break;
			}

			_app->objectPresentationSetImageOriginalCoordinatesOnPuzzle(_app->bagGetClickedObject(), 0);

			_app->objectSetAccessibilityOn(_app->bagGetClickedObject(), 0, 0);
			_app->objectPresentationShow(_app->bagGetClickedObject());
			_app->bagRemove(_app->bagGetClickedObject());
			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		switch (index) {
		default: {
			if (_app->varGetDword(51030) == 50451
				&& objectId == kObject50451)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51061) == 50452
				&& objectId == kObject50452)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51001) == 50453
				&& objectId == kObject50453)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51033) == 50454
				&& objectId == kObject50454)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51026) == 50455
				&& objectId == kObject50455)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51046) == 50456
				&& objectId == kObject50456)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51042) == 50457
				&& objectId == kObject50457)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			_app->varSetDword(index + 50999, 0);

			uint32 idx = (index - 1) / 10 + 7 * (index - 1) % 10;
			_app->objectSetAccessibilityOff(objectId, idx + 1, idx + 1);
			_app->objectSetAccessibilityOn(kObjectMagnet, idx, idx);
			_app->objectPresentationHide(objectId);
			_app->bagAdd(objectId);
			}
			break;

		case 0:
			_app->objectPresentationHide(objectId);
			_app->bagAdd(objectId);
			_app->objectSetAccessibilityOff(objectId, index, index);
			break;
		}
		break;

	case kObjectFronthead:
	case kObjectBackhead:
	case kObjectBelly:
	case kObjectRightArm:
	case kObjectLeftArm:
	case kObjectLegs:
	case kObjectHeart:
		if (!_app->bagHasClickedObject()) {
			if (!index && !_app->bagHas(objectId)) {
				_app->objectPresentationHide(objectId, 0);
				_app->bagAdd(objectId);
				break;
			}

			_app->setField74(false);
			break;
		}

		if (_app->bagGetClickedObject().id() == objectId && index > 0) {
			_app->objectPresentationShow(objectId, 1);
			_app->objectSetAccessibilityOff(objectId, 0, 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);
			_app->varSetDword(50000, (int32)index + _app->varGetDword(50000));

			if (_app->varGetDword(50000) == 7654321) {
				_app->playMovie("1865");
				_app->playMovie("1866");
				_app->rotationSetMovabilityOff(50401, 1, 1);
				_app->rotationSetMovabilityOn(50401, 2, 2);
				_app->rotationSetMovabilityOff(50402, 1, 1);
				_app->rotationSetMovabilityOn(50402, 2, 2);
				_app->objectPresentationShow(kObjectGolem1, 0);
				_app->rotationSetAlp(50402, 180.0f);
				_app->rotationSetBet(50402, 0.3f);
				_app->rotationSetActive(50402);
			} else {
				if (_app->varGetDword(50000) == 654321)
					_app->objectSetAccessibilityOn(kObjectHeart);
			}

			_app->bagRemove(objectId);
			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		if (_app->bagGetClickedObject() != kObjectFronthead
		 && _app->bagGetClickedObject() != kObjectBackhead
		 && _app->bagGetClickedObject() != kObjectBelly
		 && _app->bagGetClickedObject() != kObjectRightArm
		 && _app->bagGetClickedObject() != kObjectLeftArm
		 && _app->bagGetClickedObject() != kObjectLegs
		 && _app->bagGetClickedObject() != kObjectHeart) {
		 	_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		_app->objectPresentationShow(_app->bagGetClickedObject(), 0);
		_app->bagRemove(_app->bagGetClickedObject());
		_app->setField74(false);
		_app->cursorDelete();
		break;

	case kObjectInk:
	case kObjectPaper:
	case kObjectStylet:
	case kObjectInkedStylet:
		if (_app->bagHasClickedObject()) {
			switch (index) {
			default:
				break;

			case 0:
			case 1:
			case 2:
				if (_app->bagGetClickedObject().id() == objectId) {
					_app->bagRemove(objectId);
					_app->objectPresentationShow(kObject50100, index);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;

			case 8:
				if (_app->bagGetClickedObject() == kObjectPaper) {
					_app->objectPresentationShow(kObject50100, 3);
					_app->bagRemove(kObjectPaper);
					_app->objectSetAccessibilityOff(kObjectPaper, 0, 0);
					_app->varSetWord(50103, 102);
					_app->setField74(false);
					_app->cursorDelete();
				}

				if (_app->bagGetClickedObject() == kObjectInkedStylet
				 && _app->varGetWord(50103) > 0) {
					_app->objectPresentationShow(kObject50100, 5);
					_app->bagRemove(kObjectInkedStylet);
					_app->varSetWord(50105, 104);
					_app->objectPresentationPauseAnimationFrame(kObject50100, 7, 30, 10000, 2);
					_app->objectPresentationShow(kObject50100, 7);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;

			case 9:
				if (_app->bagGetClickedObject() == kObjectInk
				 && !_app->varGetWord(50101)) {
					_app->objectPresentationShow(kObject50100, 4);
					_app->bagRemove(kObjectInk);
					_app->objectSetAccessibilityOff(kObjectInk, 0, 0);
					_app->varSetWord(50101, 101);
					_app->setField74(false);
					_app->cursorDelete();
				}

				if (_app->bagGetClickedObject() == kObjectStylet
				 && _app->varGetWord(50101) > 0) {
					_app->objectPresentationHide(kObject50100, 4);
					_app->objectPresentationShow(kObject50100, 6);
					_app->objectSetAccessibilityOff(kObjectStylet, 0, 0);
					_app->bagRemove(kObjectStylet);
					_app->varSetWord(50102, 103);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;
			}

			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		switch (index) {
		default:
			break;

		case 0:
		case 1:
		case 2:
			if (!_app->bagHas(objectId)) {
				_app->bagAdd(objectId);
				_app->objectPresentationHide(kObject50100, index);
			}
			break;

		case 9:
			if (_app->varGetWord(50102) > 0
			&& !_app->bagHas(kObjectInkedStylet)) {
				_app->objectPresentationHide(kObject50100, 6);
				_app->bagAdd(kObjectInkedStylet);
			}
			break;
		}
		_app->setField74(false);
		break;
	}
}

#pragma endregion

#pragma region Keydown

void EventHandlerRing::onKeyDownZone(const Common::KeyState &keyState) {
	if (_app->getCurrentZone() == kZoneSY)
		onKeyDownZoneSY(keyState);
}

void EventHandlerRing::onKeyDownZoneSY(const Common::KeyState &keyState) {
	if (!_app->hasCurrentPuzzle())
		return;

	PuzzleId puzzleId = _app->getCurrentPuzzleId();
	switch (puzzleId) {
	default:
		break;

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
		_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSaveName, 0, Common::Point(346 + (int16)_app->objectPresentationGetTextWidth(kObjectSaveName, 0, 0), 181));
		break;
	}
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerRing::onSetup(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "onSetup (zone: %s, type: %d)", _app->getZoneFolder(zone).c_str(), type);

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
	if (!_app->getSaveManager()->hasTimer(zoneName))
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

	_app->getSoundManager()->playSounds();
}

#pragma endregion

#pragma region Update

void EventHandlerRing::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
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

	case kZoneNI:
		onUpdateBeforeZoneNI(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZoneN2:
		onUpdateBeforeZoneN2(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;
	}
}

void EventHandlerRing::onUpdateBag(const Common::Point &point) {
	if (_app->getCurrentZone() == kZoneSY)
		onUpdateBagZoneSY(point);
}

void EventHandlerRing::onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32, uint32, const Common::Point &) {
	switch (movabilityFrom) {
	default:
		break;

	case 2:
		switch (movabilityTo) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject2, 1);
			_app->objectPresentationShow(kObject2, 2);
			break;

		case 1:
			_app->objectPresentationHide(kObject2, 2);
			_app->objectPresentationShow(kObject2, 1);
			break;
		}
		break;

	case 3:
		_app->objectPresentationShow(kObject3, 1);
		break;

	case 4: {
		int32 val = (uint32)(movabilityTo) & 0x80000001;
		bool state = (val < 0 ? ((int32)((val - 1) | 0xFFFFFFFE)) == -1 : val == 0);

		if (state) {
			_app->objectPresentationHide(kObject4, 2);
			_app->objectPresentationShow(kObject4, 1);
		} else {
			_app->objectPresentationHide(kObject4, 1);
			_app->objectPresentationShow(kObject4, 2);
		}
		}
		break;

	case 90000:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuNewGame, 0);
		break;

	case 90001:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuPreferences, 0);
		break;

	case 90002:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuLoad, 0);
		break;

	case 90003:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuSave, 0);
		break;

	case 90005:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuStatus, 0);
		break;

	case 90004:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuContinue, 0);
		break;

	case 90006:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuExit, 0);
		break;

	case 90101:
		_app->objectPresentationShow(kObjectPreferencesCancel, 0);
		_app->objectPresentationHide(kObjectPreferencesOk, 0);
		break;

	case 90102:
		_app->objectPresentationHide(kObjectPreferencesCancel, 0);
		_app->objectPresentationShow(kObjectPreferencesOk, 0);
		break;

	case 90104:
		_app->objectPresentationShow(kObjectPreferences3dSound, 2);
		break;

	case 90207:
		_app->objectPresentationHide(kObjectLoadOk, 0);
		_app->objectPresentationShow(kObjectLoadCancel, 0);
		break;

	case 90208:
		_app->objectPresentationShow(kObjectLoadOk, 0);
		_app->objectPresentationHide(kObjectLoadCancel, 0);
		break;

	case 90401:
		_app->objectPresentationShow(kObjectStatusOk, 0);
		break;

	case 90309:
		_app->objectPresentationShow(kObjectSaveOk, 0);
		_app->objectPresentationHide(kObjectSaveCancel, 0);
		break;

	case 90310:
		_app->objectPresentationHide(kObjectSaveOk, 0);
		_app->objectPresentationShow(kObjectSaveCancel, 0);
		break;

	case 90912:
		_app->objectPresentationShow(kObject90912, 0);
		break;
	}
}

void EventHandlerRing::onUpdateBeforeZoneNI(Id movabilityFrom, Id movabilityTo, uint32, uint32, const Common::Point &point) {
	if (movabilityFrom == 10100 && movabilityTo == 0) {
		_app->rotationSetActive(10101);
		_app->objectSetAccessibilityOn(kObject10101, 0, 0);
		_app->objectSetAccessibilityOn(kObject10102, 0, 0);
		_app->objectSetAccessibilityOn(kObject10101, 2, 2);
		_app->objectSetAccessibilityOn(kObject10102, 2, 2);
		_app->objectSetAccessibilityOn(kObject10100, 4, 4);

		g_system->warpMouse(point.x, point.y);
	}
}

void EventHandlerRing::onUpdateBeforeZoneN2(Id movabilityFrom, Id movabilityTo, uint32, uint32, const Common::Point &point) {
	if (movabilityFrom == 70100 && movabilityTo == 0) {
		_app->rotationSetActive(70100);
		_app->objectSetAccessibilityOn(kObject70101, 0, 0);
		_app->objectSetAccessibilityOn(kObject70102, 0, 0);
		_app->objectSetAccessibilityOn(kObject70101, 2, 2);
		_app->objectSetAccessibilityOn(kObject70102, 2, 2);
		_app->objectSetAccessibilityOn(kObject70100, 4, 4);

		g_system->warpMouse(point.x, point.y);
	}
}

void EventHandlerRing::onUpdateBagZoneSY(const Common::Point &) {
	_app->objectPresentationHide(kObjectMenuNewGame, 0);
	_app->objectPresentationHide(kObjectMenuPreferences, 0);
	_app->objectPresentationHide(kObjectMenuLoad, 0);
	_app->objectPresentationHide(kObjectMenuSave, 0);
	_app->objectPresentationHide(kObjectMenuContinue, 0);
	_app->objectPresentationHide(kObjectMenuExit, 0);
	_app->objectPresentationHide(kObjectPreferencesCancel, 0);
	_app->objectPresentationHide(kObjectPreferencesOk, 0);
	_app->objectPresentationHide(kObjectPreferences3dSound, 2);
	_app->objectPresentationHide(kObjectLoadOk, 0);
	_app->objectPresentationHide(kObjectLoadCancel, 0);
	_app->objectPresentationHide(kObjectSaveOk, 0);
	_app->objectPresentationHide(kObjectSaveCancel, 0);
	_app->objectPresentationHide(kObjectMenuStatus, 0);
	_app->objectPresentationHide(kObjectStatusOk, 0);
	_app->objectPresentationHide(kObject2, 1);
	_app->objectPresentationHide(kObject2, 2);
	_app->objectPresentationHide(kObject4, 1);
	_app->objectPresentationHide(kObject4, 2);
	_app->objectPresentationHide(kObject3, 1);
	_app->objectPresentationHide(kObject90912, 0);

	_app->cursorSelect(kCursorMenuIdle);
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

	_app->getTimerHandler()->incrementFiredCount(timerId);
}

void EventHandlerRing::onTimerZoneNI(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0: {
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10432) < 70) {
			_app->objectPresentationHide(kObject10431);
			_app->objectPresentationShow(kObject10431, ((uint8)_app->varGetByte(10432) - 10) / 5);
		} else if (_app->varGetByte(10432) == 100) {
			_app->objectPresentationShow(kObject10431, 12);
		}

		uint32 mod = (uint8)(120 - _app->varGetByte(10432)) / 10;
		if (mod < 5)
			mod = 5;

		if (!((uint8)(_app->varGetByte(10432)) % mod)) {
			_app->soundPlay(10415);
			_app->objectPresentationShow(kObject10432, rnd(2) + 1);
		}

		uint32 volume = (uint8)_app->varGetByte(10432) / 5 + 80;
		if (volume > 100)
			volume = 100;

		_app->soundSetVolume(10412, volume);

		if (_app->varGetByte(10412) > 120) {
			_app->timerStop(kTimer0);
			_app->soundStopType(kSoundTypeAmbientEffect, 1024);
			_app->soundStopType(kSoundTypeBackgroundMusic, 1024);
			_app->playMovie("1538");
			_app->exitToMenu(kMenuAction4);
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
		uint32 frame = (uint32)(alp * 0.2631578947368421f);

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

		_app->objectPresentationShow(kObject40101, (uint8)_app->varGetByte(40806));
		break;

	case kTimer1:
		_app->varSetByte(40807, _app->varGetByte(40807) + 1);

		if (_app->varGetByte(40807) == 90) {
			_app->timerStop(kTimer1);
			_app->varSetByte(40602, 90);
		}

		_app->objectPresentationShow(kObject40102, (uint8)_app->varGetByte(40807));
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

void EventHandlerRing::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		onBagZoneSY(id, (uint32)target, 1, 1, dragControl, type);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneSY:
		onBagZoneSY(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneNI:
		onBagZoneNI(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneFO:
		onBagZoneFO(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneRO:
		onBagZoneRO(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneN2:
		onBagZoneN2(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void EventHandlerRing::onBagZoneSY(ObjectId id, uint32, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObjectPreferencesSliderVolume:
	case kObjectPreferencesSliderDialog:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);

			if (id == kObjectPreferencesSliderVolume)
				_app->dragControlSetHotspot(Common::Rect(310, 140, 600, 180));
			else
				_app->dragControlSetHotspot(Common::Rect(310, 197, 600, 237));

			_offsetX = (_app->getDragControl()->getCurrentCoords().x - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;
			} else {
				_offsetX = 54;
			}

			_coordX = (int16)(5 * _offsetX + 314);
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;

		case 2:
			_offsetX = (_offsetX0 + _coordX - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;

				_prefsVolume = _offsetX + 46;
			} else {
				_offsetX = 54;
				_prefsVolume = 100;
			}

			_coordX = (int16)(5 * _offsetX + 314);
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;

		case 3:
			_offsetX0 = (int16)_app->dragControlGetOffsetX0();
			if (_app->dragControlXLower0())
				_offsetX0 = -(int16)_app->dragControlGetOffsetX0();

			_offsetX = (_offsetX0 + _coordX - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;
			} else {
				_offsetX = 54;
			}

			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_offsetX0 + _coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;
		}
		break;
	}
}

void EventHandlerRing::onBagZoneNI(ObjectId id, uint32 a2, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject10103:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexNI = 0;
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(495, 194, 598, 284));
			break;

		case 2:
			if (!_app->soundIsPlaying(10401))
				_app->soundStop(10401, 1024);

			_app->objectSetAccessibilityOff(kObject10103);

			if (_presentationIndexNI <= 6) {
				while (_presentationIndexNI > 0) {
					--_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_presentationIndexNI);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject10103);
			} else {
				while (_presentationIndexNI < 12) {
					++_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_presentationIndexNI);

					handleEvents();
				}

				uint32 startTick = g_system->getMillis();
				while (g_system->getMillis() - startTick < 1000)
					handleEvents();

				while (_presentationIndexNI > 0) {
					--_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_presentationIndexNI);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject10103);
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
				_app->playMovie("1511");
				_app->rotationSetAlp(10005, 270.0f);
				_app->rotationSetRan(10005, 85.7f);
				_app->rotationSetActive(10005);
				_app->rotationSetMovabilityOff(10005);
				_app->rotationSetMovabilityOn(10005, 1, 1);
				_app->soundPlay(10901, kSoundLoop);
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				_presentationIndexNI = _app->dragControlGetOffsetX0() / 5;

				if (_app->dragControlXLower0()) {
					_presentationIndexNI = 0;
				} else if (_presentationIndexNI >= 0) {
					if (_presentationIndexNI > 12)
						_presentationIndexNI = 12;
				} else {
					_presentationIndexNI = 0;
				}

				_app->objectPresentationHide(kObject10103);
				_app->objectPresentationShow(10103, (uint32)_presentationIndexNI);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;

	case kObject10106:
		switch (type) {
		default:
			break;

		case 1: {
			_app->dragControlSetCoords1(Common::Point(243, 276));

			int32 val1 = 0;
			int32 val2 = 0;
			int32 offsetX1 = 0;
			int32 offsetY1 = 0;

			if (_app->dragControlXLower1())
				offsetX1 = -(int32)_app->dragControlGetOffsetX1();
			else
				offsetX1 = _app->dragControlGetOffsetX1();

			if (_app->dragControlYLower1())
				offsetY1 = -(int32)_app->dragControlGetOffsetY1();
			else
				offsetY1 = _app->dragControlGetOffsetY1();

			if (offsetX1 <= 0) {
				if (offsetY1 > 0) {
					val1 = (offsetY1 + offsetX1 + 40) / 6;
					val2 = 30;
				} else {
					val1 = ((offsetY1 - offsetX1) + 40) / 6;
					val2 = 20;
				}
			} else {
				if (offsetY1 > 0) {
					val1 = ((offsetX1 - offsetY1) + 40) / 6;
					val2 = 0;
				} else {
					val1 = (40 - (offsetX1 + offsetY1)) / 6;
					val2 = 10;
				}
			}

			if (val1 < 0)
				val1 = 0;

			_distNI = val1 + val2 - _app->varGetWord(10100);
			if (_distNI < 0)
				_distNI	= 0;

			if (_distNI > 18)
				_distNI %= 19;

			_indexOffsetNI = (_app->varGetByte(10105) == 1) ? 19 : 256;
			}
			break;

		case 2: {
			int32 offset = 0;
			bool update = true;

			switch (_presentationIndexNI) {
			default:
				offset = 2;
				break;

			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 13:
			case 14:
			case 15:
				offset = -1;
				break;

			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 16:
			case 17:
			case 18:
				offset = 1;
				break;

			case 12:
				// Skip update
				update = false;
				break;
			}

			if (update) {
				do {
					_presentationIndexNI += offset;

					if (_presentationIndexNI <= 18) {
						if (_presentationIndexNI < 0)
							_presentationIndexNI = 18;
					} else {
						_presentationIndexNI = 0;
					}

					_app->objectPresentationHideAndRemove(kObject10106);
					_app->objectPresentationShow(kObject10106, (uint32)(_presentationIndexNI + _indexOffsetNI));
					sub_445930();

					handleEvents();

				} while (_presentationIndexNI != 12);
			}

			_app->varSetWord(10100, (int16)_presentationIndexNI);

			if (_presentationIndexNI == 12) {
				_app->varSetByte(10104, 1);

				sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

				_app->rotationSetMovabilityOff(10102, 1, 1);
				_app->rotationSetMovabilityOn(10102, 2, 2);

				sub_445930();
			} else {
				_app->varSetByte(10104, 0);

				sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

				_app->rotationSetMovabilityOn(10102, 1, 1);
				_app->rotationSetMovabilityOff(10102, 2, 2);
				sub_445930();
			}
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				int32 val1 = 0;
				int32 val2 = 0;
				int32 offsetX1 = 0;
				int32 offsetY1 = 0;

				if (_app->dragControlXLower1())
					offsetX1 = -(int32)_app->dragControlGetOffsetX1();
				else
					offsetX1 = _app->dragControlGetOffsetX1();

				if (_app->dragControlYLower1())
					offsetY1 = -(int32)_app->dragControlGetOffsetY1();
				else
					offsetY1 = _app->dragControlGetOffsetY1();

				// Check interval
				if (offsetX1 > 40 || offsetY1 > 40 || offsetX1 < -40 || offsetY1 < -40)
					break;

				if (offsetX1 <= 0) {
					if (offsetY1 > 0) {
						val1 = (offsetY1 + offsetX1 + 40) / 6;
						val2 = 30;
					} else {
						val1 = ((offsetY1 - offsetX1) + 40) / 6;
						val2 = 20;
					}
				} else {
					if (offsetY1 > 0) {
						val1 = ((offsetX1 - offsetY1) + 40) / 6;
						val2 = 0;
					} else {
						val1 = (40 - (offsetX1 + offsetY1)) / 6;
						val2 = 10;
					}
				}

				if (val1 < 0)
					val1 = 0;

				_presentationIndexNI = val1 + val2 + _distNI;
				if (_presentationIndexNI < 0)
					_presentationIndexNI	= 0;

				if (_presentationIndexNI > 18)
					_presentationIndexNI %= 19;

				_app->objectPresentationHide(kObject10106);
				_app->objectPresentationShow(kObject10106, (uint32)(_presentationIndexNI + _indexOffsetNI));
				_app->varSetWord(10100, (int16)_presentationIndexNI);
				sub_445930();
			}
			break;
		}
		break;

	case kObject10107:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(295, 255, 345, 375));
			break;

		case 2:
			_app->objectPresentationHide(kObject10107);

			if (a2) {
				if (_app->dragControlGetOffsetY0() < 20) {
					_app->objectPresentationShow(kObject10107, 13);
					_app->varSetByte(10105, 0);
					sub_445A10();
					sub_445930();
					break;
				}

				if (_app->dragControlYHigher0()) {
					_app->objectPresentationShow(kObject10107, 13);
					_app->varSetByte(10105, 1);
				} else {
					_app->objectPresentationShow(kObject10107, 0);
					_app->objectSetAccessibilityOn(kObject10107, 0, 0);
					_app->objectSetAccessibilityOff(kObject10107, 1, 1);
					_app->varSetByte(10105, 0);
				}

				sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

			} else {
				if (_app->dragControlGetOffsetY0() < 20) {
					_app->objectPresentationShow(kObject10107, 0);
					_app->varSetByte(10105, 0);
					sub_445A10();
					sub_445930();
					break;
				}

				if (_app->dragControlYLower0()) {
					_app->objectPresentationShow(kObject10107, 0);
					_app->varSetByte(10105, 0);
				} else {
					_app->objectPresentationShow(kObject10107, 13);
					_app->objectSetAccessibilityOff(kObject10107, 0, 0);
					_app->objectSetAccessibilityOn(kObject10107, 1, 1);
					_app->varSetByte(10105, 1);
				}

				sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);
			}

			sub_445930();
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 index = 0;

				if (a2) {
					index = 13 - _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYHigher0())
						index = 13;
				} else {
					index = _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYLower0())
						index = 0;
				}

				_app->varSetByte(10105, 0);

				if (index >= 0) {
					if (index > 12) {
						index = 13;
						_app->varSetByte(10105, 1);
					}
				} else {
					index = 0;
				}

				sub_445930();
				_app->objectPresentationHide(kObject10107);
				_app->objectPresentationShow(kObject10107, (uint32)index);
				sub_445930();
			}
			break;
		}
		break;

	case kObject10201:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(299, 214, 431, 356));
			break;

		case 2:
			if (!_app->soundIsPlaying(10401))
				_app->soundStop(10401, 1024);

			if (_presentationIndexNI <= 3) {
				while (_presentationIndexNI > 0) {
					--_presentationIndexNI;

					_app->objectPresentationHide(kObject10201);
					_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

					handleEvents();
				}
			} else {
				if (_presentationIndexNI <= 10) {
					int32 offset = (_presentationIndexNI >= 6) ? -1 : 1;
					while (_presentationIndexNI != 6) {
						_presentationIndexNI += offset;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					uint32 startTick = g_system->getMillis();
					while (g_system->getMillis() - startTick < 1000)
						handleEvents();

					while (_presentationIndexNI > 0) {
						--_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					_app->playMovie("1527");

					if (_app->varGetByte(10200)) {
						_app->puzzleSetActive(kPuzzle10202);
						_app->soundPlay(10030);
					} else {
						_app->varSetByte(10200, 1);
						_app->puzzleSetActive(kPuzzle10203);
						_app->soundPlay(10027);
						_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					}
				} else {
					_app->objectSetAccessibilityOff(kObject10201);
					while (_presentationIndexNI < 13) {
						++_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					uint32 startTick = g_system->getMillis();
					while (g_system->getMillis() - startTick < 1000)
						handleEvents();

					while (_presentationIndexNI > 0) {
						--_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					_app->playMovie("1525");
					_app->playMovie("1526");
					_app->objectSetAccessibilityOn(kObject10200);
				}
			}
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				_presentationIndexNI = _app->dragControlGetOffsetY0() / 5;

				if (_app->dragControlYLower0()) {
					_presentationIndexNI = 0;
				} else if (_presentationIndexNI >= 0) {
					if (_presentationIndexNI > 13)
						_presentationIndexNI = 13;
				} else {
					_presentationIndexNI = 0;
				}

				_app->objectPresentationHide(kObject10201);
				_app->objectPresentationShow(10201, (uint32)_presentationIndexNI);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;

	case kObject10420:
	case kObject10421:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(263, 206, 390, 341));
			_stateBagNI = (a2 != 0);
			break;

		case 2:
			_app->objectPresentationHide(id);
			if (a2) {
				if (_app->dragControlGetOffsetY0() < 30 || _app->dragControlYHigher0()) {
					_app->objectPresentationShow(id, 12);
					_app->varSetByte(id, 1);
				} else {
					_app->objectPresentationShow(id, 1);
					_app->objectSetAccessibilityOn(id, 0, 0);
					_app->objectSetAccessibilityOff(id, 1, 1);
					_app->varSetByte(id, 0);
				}
			} else {
				if (_app->dragControlGetOffsetY0() < 30 || _app->dragControlYLower0()) {
					_app->objectPresentationShow(id, 1);
					_app->varSetByte(id, 0);
				} else {
					_app->objectPresentationShow(id, 12);
					_app->objectSetAccessibilityOff(id, 0, 0);
					_app->objectSetAccessibilityOn(id, 1, 1);
					_app->varSetByte(id, 1);
				}
			}

			sub_445A10();
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 offset = 0;
				if (a2) {
					offset = 11 - _app->dragControlGetOffsetY0() / 5;

					if (_app->dragControlXHigher0())
						offset = 12;
				} else {
					offset = _app->dragControlGetOffsetY0() / 5 + 1;

					if (_app->dragControlXLower0())
						offset = 1;
				}

				if (offset < 1)
					offset = 1;

				if (offset > 12)
					offset = 12;

				_app->objectPresentationHideAndRemove(id);
				_app->objectPresentationShow(id, (uint32)offset);

				if (offset <= 8 || _stateBagNI) {
					if (offset < 5 && _stateBagNI) {
						_app->soundStop(10404, 1024);
						_app->soundPlay(10403);
						_app->varSetFloat(90005, _app->varGetFloat(90005) - 3.0f);

						_stateBagNI = false;
					}
				} else {
					_app->soundPlay(10402);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);

					_stateBagNI = true;

					if (_app->varGetByte(10107)) {
						_app->varSetByte(10432, _app->varGetByte(10432) / 2);

						if (id == kObject10420)
							_app->objectSetAccessibilityOff(kObject10420);

						_app->soundPlay(10404);
						sub_445A10();
					}
				}
			}
			break;
		}
		break;

	case kObject10601:
	case kObject10602:
	case kObject10603:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexNI = _app->varGetWord(id - 1);
			break;

		case 2:
			_app->varSetWord(id - 1, (int16)_presentationIndexNI);

			if (!_app->varGetWord(10600)
			 && !_app->varGetWord(10601)
			 && !_app->varGetWord(10602)) {
				_app->varSetWord(10600, 12);
				_app->varSetWord(10601, 0);
				_app->varSetWord(10602, 24);
				_app->playMovie("1524");
				_app->rotationSetAlp(10501, 232.0f);
				_app->rotationSetActive(10501);
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				if (_app->dragControlXLower0()) {
					_presentationIndexNI = _app->varGetWord(id - 1) - ((int32)_app->dragControlGetOffsetX0() / 12);
				} else if (_app->dragControlXHigher0()) {
					_presentationIndexNI = _app->varGetWord(id - 1) + ((int32)_app->dragControlGetOffsetX0() / 12);
				}

				if (_presentationIndexNI < 0)
					_presentationIndexNI  = 47;

				if (_presentationIndexNI > 47)
					_presentationIndexNI %= 48;

				_app->objectPresentationHideAndRemove(id);
				_app->objectPresentationShow(id);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;
	}
}

void EventHandlerRing::onBagZoneFO(ObjectId id, uint32, Id, uint32, DragControl *, byte type) {
	if (id != kObject30016)
		return;

	switch (type) {
	default:
		break;

	case 1:
		_app->dragControlSetField45(2);
		_app->soundPlay(30500);
		_app->dragControlSetCoords1(Common::Point(440, 248));
		break;

	case 2:
		_app->soundStop(30500, 1024);
		break;

	case 3:
		if (_app->dragControlXHigher1() && _app->dragControlYLower1()) {
			if (_app->dragControlXHigher() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXLower() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXHigher1() && _app->dragControlYHigher1()) {
			if (_app->dragControlXLower() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXHigher() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXLower1() && _app->dragControlYHigher1()) {
			if (_app->dragControlXLower() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXHigher() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXLower1() && _app->dragControlYLower1()) {
			if (_app->dragControlXHigher() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXLower() &&_app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		_app->soundSetVolume(30500, _app->dragControlGetDistance() + 80);
		break;
	}
}

void EventHandlerRing::onBagZoneRO(ObjectId id, uint32 a2, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject40060:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(0, 0, 640, 480));

			_app->soundPlay(40102, kSoundLoop);

			_presentationIndexBagRO2 = 71;
			_presentationIndexBagRO = _app->varGetByte(40804);
			break;

		case 2: {
			_app->soundStop(40102, 1024);

			_presentationIndexRO = _presentationIndexBagRO;
			float test = 10 * (_presentationIndexBagRO * 0.1000000014901161f);
			float index = test;
			int direction = -1;

			if ((test + 5) < _presentationIndexBagRO) {
				direction = 1;
				index = test + 10;
			}

			if (_presentationIndexBagRO != index) {
				do {
					_presentationIndexRO += direction;

					_app->objectPresentationHide(kObject40060);
					_app->objectPresentationShow(kObject40060, (uint32)_presentationIndexRO);

					handleEvents();
					if (checkEscape())
						break;

				} while (_presentationIndexRO != index);
			}

			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOn(kObject40060, (uint32)_presentationIndexRO / 10, (uint32)_presentationIndexRO / 10);
			_app->varSetByte(40804, (uint8)_presentationIndexRO);
			}
			break;

		case 3: {
			if (_app->bagHasClickedObject()) {
				_app->cursorDelete();
				break;
			}

			if (_app->dragControlXEqual()) {
				_app->soundStop(40102, 1024);
				break;
			}

			if (!_app->soundIsPlaying(40102))
				_app->soundPlay(40102, kSoundLoop);

			_app->soundSetVolume(40102, _app->dragControlGetOffsetX() / 2 + 80);

			int32 offset = (int32)(_app->dragControlGetOffsetX() * 0.1666666666666667f);

			if (_app->dragControlXLower())
				_presentationIndexBagRO += offset;

			if (_app->dragControlXHigher())
				_presentationIndexBagRO -= offset;

			if (_presentationIndexBagRO < 0)
				_presentationIndexBagRO = 0;

			if (_presentationIndexBagRO > _presentationIndexBagRO2)
				_presentationIndexBagRO = _presentationIndexBagRO2;

			_app->objectPresentationHide(kObject40060);
			_app->objectPresentationShow(kObject40060, (uint32)_presentationIndexBagRO);
			}
			break;
		}
		break;

	case kObject40101:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexBagRO = _app->varGetByte(a2 + 40601);
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(0, 0, 640, 480));
			_app->soundPlay(40102, kSoundLoop);
			// Original sets unused value
			_presentationIndexBagRO2 = 97;
			break;

		case 2: {
			_presentationIndexRO = _presentationIndexBagRO;

			float test = 10 * (_presentationIndexBagRO * 0.1000000014901161f);
			float index = test;
			int direction = -1;

			if ((test + 5) < _presentationIndexBagRO) {
				direction = 1;
				index = test + 10;
			}

			if (_presentationIndexBagRO != index) {
				do {
					_presentationIndexRO += direction;

					_app->objectPresentationHide((ObjectId)(a2 + 40101));
					_app->objectPresentationShow((ObjectId)(a2 + 40101), (uint32)_presentationIndexRO);

					handleEvents();
					if (checkEscape())
						break;

				} while (_presentationIndexRO != index);
			}

			_app->soundStop(40102, 1024);
			_app->varSetByte(a2 + 40601, (int8)_presentationIndexRO);

			if (_app->varGetByte(40601) == 10
			 && _app->varGetByte(40602) == 90
			 && _app->varGetByte(40603) == 60
			 && _app->varGetByte(40604) == 50
			 && (_app->varGetByte(40605) == 50 || _app->varGetByte(40605) == 40)) {
				_app->varSetByte(40702, 1);
				_app->objectPresentationHide(kObject40101);
				_app->objectPresentationHide(kObject40102);
				_app->objectPresentationHide(kObject40103);
				_app->objectPresentationHide(kObject40104);
				_app->objectPresentationHide(kObject40105);
				_app->objectSetAccessibilityOff(kObject40101);
				_app->objectSetAccessibilityOff(kObject40102);
				_app->objectSetAccessibilityOff(kObject40103);
				_app->objectSetAccessibilityOff(kObject40104);
				_app->objectSetAccessibilityOff(kObject40105);

				if (_app->varGetByte(40605) == 50)
					_app->playMovie("1783");

				if (_app->varGetByte(40605) == 40)
					_app->playMovie("1784");

				_app->varSetFloat(90006, 75.0);

				_app->puzzleSetActive(kPuzzle40012);
			}
			}
			break;

		case 3:
			if (_app->bagHasClickedObject()) {
				_app->cursorDelete();
				break;
			}

			if (!_app->dragControlYEqual()) 	{
				if (!_app->soundIsPlaying(40102))
					_app->soundPlay(40102, kSoundLoop);

				// Original sets unused value

				_app->soundSetVolume(40102, _app->dragControlGetOffsetY() / 2 + 80);

				if (_app->dragControlYHigher())
					_presentationIndexBagRO += (int32)_app->dragControlGetOffsetY();

				if (_app->dragControlYLower())
					_presentationIndexBagRO -= (int32)_app->dragControlGetOffsetY();

				if (_presentationIndexBagRO < 0)
					_presentationIndexBagRO = _presentationIndexBagRO2;

				if (_presentationIndexBagRO2 < _presentationIndexBagRO)
					_presentationIndexBagRO = 0;

				_app->objectPresentationHide((ObjectId)(a2 + 40101));
				_app->objectPresentationShow((ObjectId)(a2 + 40101), (uint32)_presentationIndexBagRO);
				break;
			}

			_app->soundStop(40102, 1024);
			break;
		}
		break;
	}
}

void EventHandlerRing::onBagZoneN2(ObjectId id, uint32 a2, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject70103:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexN2 = 0;
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(495, 194, 598, 284));
			break;

		case 2:
			if (!_app->soundIsPlaying(70401))
				_app->soundStop(70401, 1024);

			_app->objectSetAccessibilityOff(kObject70103);

			if (_presentationIndexN2 <= 6) {
				while (_presentationIndexN2 > 0) {
					--_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject70103);
			} else {
				while (_presentationIndexN2 < 12) {
					++_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				uint32 startTick = g_system->getMillis();
				while (g_system->getMillis() - startTick < 1000)
					handleEvents();

				while (_presentationIndexN2 > 0) {
					--_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject70103);
				_app->playMovie("1498");
				_app->rotationSetAlp(70001, 270.0f);
				_app->rotationSetRan(70001, 85.7f);
				_app->rotationSetActive(70001);
				sub_433EE0();
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				_presentationIndexN2 = _app->dragControlGetOffsetX0() / 5;

				if (_app->dragControlXLower0()) {
					_presentationIndexN2 = 0;
				} else if (_presentationIndexN2 >= 0) {
					if (_presentationIndexN2 > 12)
						_presentationIndexN2 = 12;
				} else {
					_presentationIndexN2 = 0;
				}

				_app->objectPresentationHide(kObject70103);
				_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

				if (!_app->soundIsPlaying(70401))
					_app->soundPlay(70401);
			}
			break;
		}
		break;

	case kObject70106:
		switch (type) {
		default:
			break;

		case 1: {
			_app->dragControlSetCoords1(Common::Point(243, 276));

			int32 val1 = 0;
			int32 val2 = 0;
			int32 offsetX1 = 0;
			int32 offsetY1 = 0;

			if (_app->dragControlXLower1())
				offsetX1 = -(int32)_app->dragControlGetOffsetX1();
			else
				offsetX1 = _app->dragControlGetOffsetX1();

			if (_app->dragControlYLower1())
				offsetY1 = -(int32)_app->dragControlGetOffsetY1();
			else
				offsetY1 = _app->dragControlGetOffsetY1();

			if (offsetX1 <= 0) {
				if (offsetY1 > 0) {
					val1 = (offsetY1 + offsetX1 + 40) / 6;
					val2 = 30;
				} else {
					val1 = ((offsetY1 - offsetX1) + 40) / 6;
					val2 = 20;
				}
			} else {
				if (offsetY1 > 0) {
					val1 = ((offsetX1 - offsetY1) + 40) / 6;
					val2 = 0;
				} else {
					val1 = (40 - (offsetX1 + offsetY1)) / 6;
					val2 = 10;
				}
			}

			if (val1 < 0)
				val1 = 0;

			_distN2 = val1 + val2 - _app->varGetWord(70016);
			if (_distN2 < 0)
				_distN2	= 0;

			if (_distN2 > 18)
				_distN2 %= 19;
			}
			break;

		case 2: {
			int32 offset = 0;
			bool update = true;

			switch (_presentationIndexN2) {
			default:
				offset = 2;
				break;

			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 13:
			case 14:
			case 15:
				offset = -1;
				break;

			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 16:
			case 17:
			case 18:
				offset = 1;
				break;

			case 12:
				// Skip update
				update = false;
				break;
			}

			if (update) {
				do {
					_presentationIndexN2 += offset;

					if (_presentationIndexN2 <= 18) {
						if (_presentationIndexN2 < 0)
							_presentationIndexN2 = 18;
					} else {
						_presentationIndexN2 = 0;
					}

					_app->objectPresentationHideAndRemove(kObject70106);
					_app->objectPresentationShow(kObject70106, (uint32)_presentationIndexN2);

					handleEvents();

				} while (_presentationIndexN2 != 12);
			}

			_app->varSetWord(70016, (int16)offset);

			if (_presentationIndexN2 == 12 ) {
				_app->varSetByte(10104, 1);
				_app->rotationSetMovabilityOff(70101, 1, 1);
				_app->rotationSetMovabilityOn(70101, 2, 2);
			} else {
				_app->varSetByte(10104, 0);
				_app->rotationSetMovabilityOn(70101, 1, 1);
				_app->rotationSetMovabilityOff(70101, 2, 2);
			}
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				int32 val1 = 0;
				int32 val2 = 0;
				int32 offsetX1 = 0;
				int32 offsetY1 = 0;

				if (_app->dragControlXLower1())
					offsetX1 = -(int32)_app->dragControlGetOffsetX1();
				else
					offsetX1 = _app->dragControlGetOffsetX1();

				if (_app->dragControlYLower1())
					offsetY1 = -(int32)_app->dragControlGetOffsetY1();
				else
					offsetY1 = _app->dragControlGetOffsetY1();

				// Check interval
				if (offsetX1 > 40 || offsetY1 > 40 || offsetX1 < -40 || offsetY1 < -40)
					break;

				if (offsetX1 <= 0) {
					if (offsetY1 > 0) {
						val1 = (offsetY1 + offsetX1 + 40) / 6;
						val2 = 30;
					} else {
						val1 = ((offsetY1 - offsetX1) + 40) / 6;
						val2 = 20;
					}
				} else {
					if (offsetY1 > 0) {
						val1 = ((offsetX1 - offsetY1) + 40) / 6;
						val2 = 0;
					} else {
						val1 = (40 - (offsetX1 + offsetY1)) / 6;
						val2 = 10;
					}
				}

				if (val1 < 0)
					val1 = 0;

				_presentationIndexNI = val1 + val2 + _distN2;
				if (_presentationIndexNI < 0)
					_presentationIndexNI	= 0;

				if (_presentationIndexNI > 18)
					_presentationIndexNI %= 19;

				_app->objectPresentationHide(kObject70106);
				_app->objectPresentationShow(kObject70106, (uint32)_presentationIndexN2);
				_app->varSetWord(70016, (int16)_presentationIndexN2);
			}
			break;
		}
		break;

	case kObject70108:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(389, 270, 434, 390));
			break;

		case 2:
			_app->objectPresentationHide(kObject70108);

			if (a2) {
				if (_app->dragControlGetOffsetY0() < 20 || _app->dragControlYHigher0()) {
					_app->objectPresentationShow(kObject70108, 13);
					_app->varSetByte(70012, 1);
					_app->objectPresentationShow(kObject70108, 14);
					break;
				}

				_app->objectPresentationShow(kObject70108, 0);
				_app->objectSetAccessibilityOn(kObject70108, 0, 0);
				_app->objectSetAccessibilityOff(kObject70108, 1, 1);
			} else {
				if (_app->dragControlGetOffsetY0() >= 20 && !_app->dragControlYLower0()) {
					_app->objectPresentationShow(kObject70108, 13);
					_app->objectSetAccessibilityOff(kObject70108, 0, 0);
					_app->objectSetAccessibilityOn(kObject70108, 1, 1);
					_app->varSetByte(70012, 1);
					_app->objectPresentationShow(kObject70108, 14);
					_app->soundPlay(70101);

					_app->playMovie("1496");
					_app->playMovie("1497");

					_app->varSetFloat(90006, _app->varGetFloat(90006) + 10.0f);
					_app->objectPresentationHide(kObjectFire);
					_app->objectPresentationHide(kObject70108);
					_app->objectSetAccessibilityOff(kObject70700);
					_app->objectPresentationHide(kObject70700);
					_app->soundStop(70701, 1024);
					_app->rotationSet3DSoundOff(70200, 70701);
					break;
				}

				_app->objectPresentationShow(kObject70108, 0);
			}

			_app->varSetByte(70012, 0);
			_app->objectPresentationHide(kObject70108, 14);
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 offset = 0;

				if (a2) {
					offset = 13 - _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYHigher0())
						offset = 13;
				} else {
					offset = _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYLower0())
						offset = 0;
				}

				_app->objectPresentationHide(kObject70108);

				if (offset < 0)
					offset = 0;

				if (offset > 12)
					offset = 13;

				_app->objectPresentationShow(kObject70108, 14);
				_app->objectPresentationShow(kObject70108, (uint32)offset);
			}
			break;
		}
		break;
	}
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

void EventHandlerRing::onBagZoneSwitch() {
	switch (getApp()->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
	case kZoneAS:
		error("[EventHandlerRing::onBagZoneSwitch] Erda should not be active on this zone (%d)", getApp()->getCurrentZone());

	case kZoneNI:
	case kZoneRH:
		if (!getApp()->getSaveManager()->loadSaveTimer("alb", kLoadSaveWrite))
			error("[EventHandlerRing::onBagZoneSwitch] Failed loading timer (alb)");

		_app->varSetDword(90013, _app->getCurrentZone());
		_app->varSetByte(90009, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90017, 1);
			_app->varSetDword(90021, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90017, 0);
			_app->varSetDword(90021, _app->getCurrentRotationId());
			_app->varSetByte(90025, getApp()->getBag()->getRotationFre());
		}

		onSwitchZoneAS(13);
		break;

	case kZoneFO:
		if (!getApp()->getSaveManager()->loadSaveTimer("sie", kLoadSaveWrite))
			error("[EventHandlerRing::onBagZoneSwitch] Failed loading timer (sie)");

		_app->varSetDword(90015, _app->getCurrentZone());
		_app->varSetByte(90011, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90019, 1);
			_app->varSetDword(90023, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90019, 0);
			_app->varSetDword(90023, _app->getCurrentRotationId());
			_app->varSetByte(90027, getApp()->getBag()->getRotationFre());
		}

		onSwitchZoneAS(13);
		break;

	case kZoneRO:
	case kZoneN2:
		if (!getApp()->getSaveManager()->loadSaveTimer("log", kLoadSaveWrite))
			error("[EventHandlerRing::onBagZoneSwitch] Failed loading timer (log)");

		_app->varSetDword(90014, _app->getCurrentZone());
		_app->varSetByte(90010, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90018, 1);
			_app->varSetDword(90022, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90018, 0);
			_app->varSetDword(90022, _app->getCurrentRotationId());
			_app->varSetByte(90026, getApp()->getBag()->getRotationFre());
		}

		onSwitchZoneAS(13);
		break;

	case kZoneWA:
		if (!getApp()->getSaveManager()->loadSaveTimer("bru", kLoadSaveWrite))
			error("[EventHandlerRing::onBagZoneSwitch] Failed loading timer (bru)");

		_app->varSetDword(90016, _app->getCurrentZone());
		_app->varSetByte(90012, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90020, 1);
			_app->varSetDword(90024, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90020, 0);
			_app->varSetDword(90024, _app->getCurrentRotationId());
			_app->varSetByte(90028, getApp()->getBag()->getRotationFre());
		}

		onSwitchZoneAS(13);
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

		if (_app->varGetByte(30066) != 1 && _app->varGetByte(30067) != 1) {
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

void EventHandlerRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onBeforeRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRH:
		onBeforeRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneFO:
		onBeforeRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRO:
		onBeforeRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneWA:
		onBeforeRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneAS:
		onBeforeRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneN2:
		onBeforeRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32, MovabilityType movabilityType) {
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

void EventHandlerRing::onBeforeRideZoneRH(Id movabilityFrom, Id, uint32, uint32, MovabilityType) {
	if (movabilityFrom == 20401)
		_app->timerStop(kTimer0);
}

void EventHandlerRing::onBeforeRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
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

void EventHandlerRing::onBeforeRideZoneRO(Id movabilityFrom, Id, uint32, uint32, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityPuzzleToRotation)
		return;

	if (movabilityFrom == 40060 || movabilityFrom == 40005) {
		_app->soundSetVolume(40102, 100);
		_app->soundPlay(40102, kSoundLoop);

		if (_app->varGetByte(40804) > 4) {
			do {
				_app->objectPresentationHide(kObject40060);
				_app->objectPresentationShow(kObject40060, (uint8)_app->varGetByte(40804));

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

void EventHandlerRing::onBeforeRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
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

void EventHandlerRing::onBeforeRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
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

void EventHandlerRing::onBeforeRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		_app->timerStop(kTimer0);

	if (movabilityType == kMovabilityPuzzleToPuzzle) {
		if (movabilityTo == 70501 || movabilityTo == 70511 || movabilityTo == 70521)
			_app->soundPlay(70501);

		if (movabilityFrom == 70501 || movabilityFrom == 70511 || movabilityFrom == 70521)
			_app->soundPlay(70502);
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
		onAfterRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRH:
		onAfterRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneFO:
		onAfterRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRO:
		onAfterRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneWA:
		onAfterRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneAS:
		onAfterRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneN2:
		onAfterRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void EventHandlerRing::onAfterRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32, uint32 target, MovabilityType movabilityType) {
	if (target == 100) {
		_app->rotationSetMovabilityOff(movabilityFrom, 0, 0);
	} else if (target == 110) {
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

		switch (target) {
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
			_app->exitToMenu(kMenuAction3);
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
		if (target == 55) {
			_app->playMovie("1537");
			_app->puzzleSetActive(kPuzzle12001);
			_app->soundPlay(12001);
		}
		break;
	}
}

void EventHandlerRing::onAfterRideZoneRH(Id movabilityFrom, Id, uint32, uint32, MovabilityType movabilityType) {
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

void EventHandlerRing::onAfterRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		switch (movabilityFrom) {
		default:
			break;

		case 30006:
			if (movabilityTo == 30008) {
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

void EventHandlerRing::onAfterRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
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

void EventHandlerRing::onAfterRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
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

void EventHandlerRing::onAfterRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32, uint32, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityPuzzleToRotation && movabilityFrom == 80101)
		if (movabilityTo == 80006 || movabilityTo == 80007 || movabilityTo == 80008
		 || movabilityTo == 80009 || movabilityTo == 80010)
			_app->varSetByte(80003, 0);
}

void EventHandlerRing::onAfterRideZoneN2(Id movabilityFrom, Id, uint32, uint32 target, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		if (!_app->varGetByte(70012))
			_app->timerStart(kTimer0, 10000);

	if (movabilityType == kMovabilityRotationToRotation) {
		if (target == 7)
			sub_433EE0();
		else if (target == 16)
			_app->exitToMenu(kMenuAction1);
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

void EventHandlerRing::onSoundZoneSY(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	if (id == 90001) {
		_app->objectPresentationHideAndRemove(kObject7, 0);
		_app->showCredits();
		_app->startMenu(false);
	}
}

void EventHandlerRing::onSoundZoneNI(Id id, SoundType, uint32, bool process) {
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

void EventHandlerRing::onSoundZoneRH(Id id, SoundType, uint32, bool process) {
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

void EventHandlerRing::onSoundZoneFO(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 30100:
		_app->soundPlay(30101);
		break;

	case 30101:
		_app->playMovie("1179");
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
		_app->playMovie("1215");
		onSwitchZoneAS(3);
		break;

	case 30161:
		_app->playMovie("1212");
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

void EventHandlerRing::onSoundZoneRO(Id id, SoundType, uint32, bool process) {
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

void EventHandlerRing::onSoundZoneWA(Id id, SoundType, uint32, bool process) {
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

void EventHandlerRing::onSoundZoneAS(Id id, SoundType, uint32, bool process) {
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
		_app->showMenu(kZoneAS, kMenuAction0);
		break;
	}
}

void EventHandlerRing::onSoundZoneN2(Id id, SoundType, uint32, bool process) {
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
		_app->objectPresentationShow(kObject70001, (uint8)_app->varGetByte(70014) - 1);
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
		int8 index = _app->varGetByte(70013);
		char str = _app->varGetString(70099)[index];
		if (str > '0')
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
		int8 index = _app->varGetByte(70013);
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
		_app->objectPresentationShow(kObject70001, (uint8)_app->varGetByte(70014) - 1);
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
				_app->exitToMenu(kMenuAction2);
				break;
			}

			_app->puzzleSetActive(kPuzzle70303);
			_app->soundPlay(70001);
		} else {
			if (_app->varGetByte(70014) == 2 && !_app->varGetByte(70015)) {
				_app->exitToMenu(kMenuAction2);
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

void EventHandlerRing::onAnimationNextFrameZoneNI(Id animationId, const Common::String &, uint32 frame, uint32) {
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

void EventHandlerRing::onAnimationNextFrameZoneRH(Id animationId, const Common::String &, uint32 frame, uint32) {
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

void EventHandlerRing::onAnimationNextFrameZoneFO(Id animationId, const Common::String &, uint32 frame, uint32) {
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

void EventHandlerRing::onAnimationNextFrameZoneRO(Id animationId, const Common::String &, uint32 frame, uint32) {
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
				   + _app->varGetByte(40204) + _app->varGetByte(40205) + _app->varGetByte(40206)) == 7) {
					_app->varSetByte(40802, 1);
					_app->varSetString(40901, "0000000");
					_app->soundPlay(40400);
					_app->varSetFloat(90006, 85.7f);
					_app->objectSetAccessibilityOff(kObject40060);
					_app->objectSetAccessibilityOff(kObject40201);

					for (uint32 i = 10; i > 0; i--) {
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

		_app->varSetByte(40911, (int8)frame);
		return;

	case 40101:
		switch (frame) {
		default:
			_app->varSetByte(40912, (int8)frame);
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
				_app->varSetByte(40912, (int8)frame);
			} else {
				_app->soundPlay(40301);
				_app->varSetByte(40912, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 2, 2);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40912, (int8)frame);
			break;
		}
		break;

	case 40102:
		switch (frame) {
		default:
			_app->varSetByte(40913, (int8)frame);
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
				_app->varSetByte(40913, (int8)frame);
			} else {
				_app->soundPlay(40302);
				_app->varSetByte(40913, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 3, 3);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40913, (int8)frame);
			break;
		}
		break;

	case 40103:
		switch (frame) {
		default:
			_app->varSetByte(40914, (int8)frame);
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
				_app->varSetByte(40914, (int8)frame);
			} else {
				_app->soundPlay(40303);
				_app->varSetByte(40914, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 4, 4);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40914, (int8)frame);
			break;
		}
		break;

	case 40104:
		switch (frame) {
		default:
			_app->varSetByte(40915, (int8)frame);
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
				_app->varSetByte(40915, (int8)frame);
			} else {
				_app->soundPlay(40304);
				_app->varSetByte(40915, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 5, 5);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40915, (int8)frame);
			break;
		}
		break;

	case 40105:
		switch (frame) {
		default:
			_app->varSetByte(40916, (int8)frame);
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
				_app->varSetByte(40916, (int8)frame);
			} else {
				_app->soundPlay(40305);
				_app->varSetByte(40916, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 6, 6);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40916, (int8)frame);
			break;
		}
		break;

	case 40106:
		switch (frame) {
		default:
			_app->varSetByte(40917, (int8)frame);
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
				_app->varSetByte(40917, (int8)frame);
			} else {
				_app->soundPlay(40306);
				_app->varSetByte(40917, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 7, 7);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40917, (int8)frame);
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
		_app->varSetByte(frame + 700, (int8)frame); // id: frame + 700 = 4090x

		if (_app->varGetByte(40901) == 26 && _app->varGetByte(40902) == 26 && _app->varGetByte(40903) == 26 && _app->varGetByte(40904) == 26 \
		 && _app->varGetByte(40905) == 26 && _app->varGetByte(40906) == 26 && _app->varGetByte(40907) == 26 && _app->varGetByte(40911) == 70 \
		 && _app->varGetByte(40912) == 70 && _app->varGetByte(40913) == 70 && _app->varGetByte(40914) == 70 && _app->varGetByte(40915) == 70 \
		 && _app->varGetByte(40916) == 70 && _app->varGetByte(40917) == 70) {
			_app->objectSetAccessibilityOn(kObject40060, (uint8)_app->varGetByte(40804) / 10, (uint8)_app->varGetByte(40804) / 10);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
		}
		break;
	}
}

void EventHandlerRing::onAnimationNextFrameZoneWA(Id animationId, const Common::String &, uint32 frame, uint32) {
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

void EventHandlerRing::onAnimationNextFrameZoneAS(Id animationId, const Common::String &, uint32 frame, uint32) {
	if (animationId == 80001) {
		if ((int32)frame == _app->varGetByte(80004)) {
			_app->objectPresentationPauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOn(kObject80021, 0, 4);
			_app->soundStop(80210, 1024);
			_app->soundPlay(80209);
		}

		_app->varSetByte(80005, (int8)frame);
	}
}

void EventHandlerRing::onAnimationNextFrameZoneN2(Id animationId, const Common::String &, uint32 frame, uint32) {
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

void EventHandlerRing::onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {
	if (_app->getCurrentZone() == kZoneWA)
		onAnimationZoneWA(type, animationId, name, frame, a5);
}

void EventHandlerRing::onAnimationZoneWA(uint32 type, Id animationId, const Common::String &, uint32, uint32) {
	switch (animationId) {
	default:
		break;

	case 50003:
		if (type == 1) {
			_app->varSetByte(50011, 1);
		} else if (type == 2) {
			_app->varSetByte(50011, 0);
			_app->objectPresentationHide(kObject50100, 3);
			_app->objectPresentationHide(kObject50100, 4);
			_app->objectPresentationHide(kObject50100, 5);
			_app->objectPresentationHide(kObject50100, 6);
			_app->objectPresentationShow(kObject50100, 0);
			_app->objectPresentationShow(kObject50100, 1);
			_app->objectPresentationShow(kObject50100, 2);
			_app->varSetWord(50101, 0);
			_app->varSetWord(50102, 0);
			_app->varSetWord(50103, 0);
			_app->varSetWord(50104, 0);
			_app->varSetWord(50105, 0);
			_app->objectSetAccessibilityOn(kObjectInk);
			_app->objectSetAccessibilityOn(kObjectPaper);
			_app->objectSetAccessibilityOn(kObjectStylet);
			_app->objectSetAccessibilityOn(kObjectInkedStylet);

		}
		break;

	case 50004:
		if (type == 2) {
			_app->objectPresentationHide(kObject50100);
			_app->rotationSetAlp(50601, 250.0);
			_app->rotationSetActive(50601);
			_app->playMovie("1872");
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
			_app->setupZone((ZoneId)_app->varGetDword(90013), kSetupType10);
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
			_app->setupZone((ZoneId)_app->varGetDword(90015), kSetupType10);
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

		_app->getBag()->reset();

		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void EventHandlerRing::onSwitchZoneWA(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90012))
			_app->setupZone((ZoneId)_app->varGetDword(90016), kSetupType10);
		else
			_app->setupZone(kZoneWA, kSetupTypeNone);
	}
}

void EventHandlerRing::onSwitchZoneN2(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90010))
			_app->setupZone((ZoneId)_app->varGetDword(90014), kSetupType10);
		else
			_app->setupZone(kZoneN2, kSetupTypeNone);
	}
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

	for (int i = 70; i < 95; i++) {
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

void EventHandlerRing::sub_445930() {
	if (_app->varGetByte(10105) == 1) {

		_app->objectPresentationHide(kObject10106);
		_app->objectPresentationShow(kObject10106, 38);
		_app->objectPresentationShow(kObject10106, (uint16)_app->varGetWord(10100) + 19);

		if (!_app->varGetWord(10100)) {
			_app->objectPresentationShow(kObject10106, 39);
			_app->varSetByte(10106, 1);
		}

	} else {
		_app->objectPresentationHide(kObject10106);
		_app->objectPresentationShow(kObject10106, (uint16)_app->varGetWord(10100));
		_app->varSetByte(10106, 0);
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

void EventHandlerRing::waitTicks(uint32 ticks) const {
	uint32 startTicks = g_system->getMillis();
	while (g_system->getMillis() - startTicks < ticks)
		handleEvents();
}

bool EventHandlerRing::copySavedTimers(uint32 slot) const {
    Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("ALB", 0), SaveManager::getTimerFile("ALB", slot)))
		return false;

	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("LOG", 0), SaveManager::getTimerFile("LOG", slot)))
		return false;

	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("SIE", 0), SaveManager::getTimerFile("SIE", slot)))
		return false;

	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("BRU", 0), SaveManager::getTimerFile("BRU", slot)))
		return false;

	return true;
}

bool EventHandlerRing::removeSavedTimers(uint32 slot) const {
    Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("ALB", slot)))
		return false;

	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("LOG", slot)))
		return false;

	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("SIE", slot)))
		return false;

	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("BRU", slot)))
		return false;


	return true;
}

#pragma endregion

} // End of namespace Ring
