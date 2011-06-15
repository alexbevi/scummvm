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

#include "ring/game/ring/ring_input.h"

#include "ring/base/application.h"
#include "ring/base/accessibility.h"
#include "ring/base/bag.h"
#include "ring/base/event.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_event.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/hotspot.h"

#include "ring/sound/soundhandler.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/textconsole.h"

using namespace RingGame;

namespace Ring {

EventInputRing::EventInputRing(ApplicationRing *application, EventHandlerRing *eventHandler) : _app(application), _event(eventHandler) {
	_controlNotPressed = false;
}

EventInputRing::~EventInputRing() {
	// Zero-out passed pointers
	_app = NULL;
	_event = NULL;
}

#pragma region Mouse

void EventInputRing::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	_controlNotPressed = !isControlPressed;

	onMouseLeftButtonUp(evt);
}

void EventInputRing::onMouseLeftButtonUp(const Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonUp");

	// Handle clicks on bag
	Bag *bag = _app->getBag();
	if (bag->isInitialized()) {
		if (bag->checkHotspotClick(evt.mouse) == 1) {

			// Handle clicked object event
			_app->getEventHandler()->onBagClickedObject(_app->getBag()->getClickedObject());

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
		_app->getEventHandler()->onBag(dragControl->getObjectId(), dragControl->getTarget(), dragControl->getPuzzleRotationId(), dragControl->getField39(), dragControl, 2);

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

bool EventInputRing::handleLeftButtonUp(Accessibility *accessibility, Id id, const Common::Point &point) {
	if (!accessibility)
		return false;

	Object *object = accessibility->getObject();
	Hotspot *hotspot = accessibility->getHotspot();

	if (object->getFieldC() & 1) {
		((EventHandlerRing *)_app->getEventHandler())->onButtonUp(object->getId(), hotspot->getTarget(), id, 1, point);

		if (_app->getState() == kStateShowMenu)
			return true;
	}

	if (!(object->getFieldC() & 8)) {
		_app->update(point);
		return true;
	}

	((EventHandlerRing *)_app->getEventHandler())->onButtonUp2(object->getId(), (uint32)hotspot->getTarget(), id, 1, point);

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

bool EventInputRing::handleLeftButtonUp(Movability *movability, uint32 index, Id id, bool isRotation) {
	if (!movability)
		return false;

	Hotspot *hotspot = movability->getHotspot();

	//////////////////////////////////////////////////////////////////////////
	// Before Ride
	//////////////////////////////////////////////////////////////////////////
	_app->getEventHandler()->onBeforeRide(id, movability->getTo(), index, hotspot->getTarget(), movability->getType());

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

		_app->getEventHandler()->onAfterRide(_app->getCurrentRotationId(), id, index, hotspot->getTarget(), movability->getType());

		if (_app->getState() == kStateShowMenu)
			return true;
		break;

	case kMovabilityRotationToPuzzle:
	case kMovabilityPuzzleToPuzzle:
		_app->puzzleSetActive(movability->getTo().id());

		_app->getEventHandler()->onAfterRide(_app->getCurrentRotationId(), id, index, hotspot->getTarget(), movability->getType());

		if (_app->getState() == kStateShowMenu)
			return true;
		break;
	}

	if (_app->getField75())
		_app->cursorDelete();

	_app->setField75(true);

	return true;
}

void EventInputRing::onMouseLeftButtonDown(const Common::Event &evt) {
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

bool EventInputRing::handleLeftButtonDown(Accessibility *accessibility, uint32 index, Id id, const Common::Point &point) {
	if (!accessibility)
		return false;

	Object *object = accessibility->getObject();
	Hotspot *hotspot = accessibility->getHotspot();

	if (object->getFieldC() & 2) {
		((EventHandlerRing *)_app->getEventHandler())->onButtonDown(object->getId(), hotspot->getTarget(), id, 1, point);

		if (_app->getState() == kStateShowMenu)
			return true;
	}

	if (!(object->getFieldC() & 4)) {
		_app->update(point);
		return true;
	}

	_app->getDragControl()->init(point, object->getId(), index, hotspot, hotspot->getTarget(), id, 1);
	_app->getEventHandler()->onBag(object->getId(), hotspot->getTarget(), id, 1, _app->getDragControl(), 1);

	if (_app->getState() != kStateShowMenu) {
		if (!_app->getField76())
			_app->getDragControl()->reset();

		_app->setField76(true);
		_app->update(point);
	}

	return true;
}

void EventInputRing::onMouseRightButtonUp(const Common::Event &) {
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



void EventInputRing::onKeyDown(Common::Event &evt) {
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
		if (!accessibility)
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

#pragma region endregion

#pragma region Key

void EventInputRing::onKeyDownZone(const Common::KeyState &keyState) {
	if (_app->getCurrentZone() == kZoneSY)
		onKeyDownZoneSY(keyState);
}

void EventInputRing::onKeyDownZoneSY(const Common::KeyState &keyState) {
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

} // End of namespace Ring
