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

#include "common/events.h"
#include "common/serializer.h"

#include "engines/darkseed2/events.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/room.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/roomconfig.h"
#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/script.h"
#include "engines/darkseed2/inter.h"
#include "engines/darkseed2/movie.h"
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/mike.h"
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

Events::Events(DarkSeed2Engine &vm) : _vm(&vm) {
	_cursors = new ModeCursors[kCursorModeNone];

	_state = kStateStarted;

	_itemMode = false;

	// Set the default (pointer) cursor
	_canSwitchCursors = true;

	_cursorMode   = kCursorModeWalk;
	_cursorActive = false;

	_lastObject = 0;

	_changeRoom = false;

	_loading = false;
}

Events::~Events() {
	delete[] _cursors;
}

bool Events::init() {
	// Getting cursors
	if (!(_cursors[kCursorModeWalk].inactive = _vm->_cursors->getCursor()))
		return false;
	if (!(_cursors[kCursorModeWalk].active   = _vm->_cursors->getCursor("c4Ways")))
		return false;
	if (!(_cursors[kCursorModeUse ].inactive = _vm->_cursors->getCursor("cHand")))
		return false;
	if (!(_cursors[kCursorModeUse ].active   = _vm->_cursors->getCursor("cUseIt")))
		return false;
	if (!(_cursors[kCursorModeLook].inactive = _vm->_cursors->getCursor("cLook")))
		return false;
	if (!(_cursors[kCursorModeLook].active   = _vm->_cursors->getCursor("cLookAt")))
		return false;

	setCursor();
	_vm->_cursors->setVisible(true);

	return true;
}

bool Events::run() {
	switch (_state) {
	case kStateStarted:
	case kStateIntro1:
	case kStateIntro2:
	case kStateIntro3:
		return introSequence();

	case kStateIntro4:
		leaveIntro();
		break;

	case kStateIntro5:
	case kStateRunning:
		mainLoop();
		break;

	default:
		return false;
		break;
	}

	return true;
}

bool Events::introSequence() {
	if (_state < kStateIntro3) {
		if (_state < kStateIntro2) {
			if (_state < kStateIntro1) {
				// Restricting the cursors in the intro
				_canSwitchCursors = false;

				_cursorMode   = kCursorModeUse;
				_cursorActive = false;
				setCursor();

				debugC(-1, kDebugGameflow, "Entering cutscene room");

				// Cutscene room
				if (!roomGo("0001"))
					return false;

				_state = kStateIntro1;
			}

			// Run the main loop as long as scripts are still active
			mainLoop(true);
			if (_loading)
				return true;

			debugC(-1, kDebugGameflow, "Entering title room");

			// Title room
			if (!roomGo("0002"))
				return false;

			_state = kStateIntro2;
		}

		// Run the main loop as long as scripts are still active
		mainLoop(true);
		if (_loading)
			return true;

		// Loading title parts, for hotspot detection
		_titleSprites[0].loadFromImage(*_vm->_resources, "002BTN01");
		_titleSprites[1].loadFromImage(*_vm->_resources, "002BTN02");
		_titleSprites[2].loadFromImage(*_vm->_resources, "002BTN03");
		_titleSprites[3].loadFromImage(*_vm->_resources, "002BTN04");

		for (int i = 0; i < 4; i++) {
			if (_titleSprites[i].empty()) {
				warning("Events::setupIntroSequence(): Couldn't load title screen elements");
				return false;
			}
		}

		_state = kStateIntro3;
	}

	mainLoop(false);

	_state = kStateRunning;
	return true;
}

void Events::leaveIntro() {
	if (_state < kStateIntro4) {
		// Throw title parts away again
		for (int i = 0; i < 4; i++)
			_titleSprites[i].clear();

		debugC(-1, kDebugGameflow, "Entering intro movie room");

		// Intro movie room
		if (!roomGo("1501")) {
			warning("Events::leaveIntro(): Failed loading the intro movie room");
			_vm->quitGame();
			return;
		}

		_state = kStateIntro4;
	}

	// Run the main loop as long as scripts are still active
	mainLoop(true);
	if (_loading)
		return;

	// Restore normal cursor mode
	_canSwitchCursors = true;
	_cursorMode   = kCursorModeWalk;
	_cursorActive = false;
	setCursor();

	debugC(-1, kDebugGameflow, "Entering first room 0101");

	// First room
	if (!roomGo("0101")) {
		warning("Events::leaveIntro(): Failed loading the first room");
		_vm->quitGame();
		return;
	}

	_nextRoom = _vm->_graphics->getRoom().getName();

	_state = kStateIntro5;
}

void Events::mainLoop(bool finishScripts) {
	bool scriptStateChanged;

	while (!_vm->shouldQuit()) {
		if (_vm->_movie->isPlaying()) {
			// Special mode for movie playing
			handleMovieInput();
			// If loading a game state was requested, break the loop
			if (_loading)
				break;

			_vm->_movie->updateStatus();

			// Update screen
			_vm->_graphics->retrace();
			g_system->updateScreen();

			// Wait
			g_system->delayMillis(_vm->_movie->getFrameWaitTime());

			continue;
		}

		// Look for user input
		handleInput();

		// Leaving the intro
		if (_state == kStateIntro5) {
			_state = kStateRunning;
			break;
		}

		// If loading a game state was requested, break the loop
		if (_loading)
			break;

		// Always use that mode/activity when the conversation box is visible
		if (_vm->_graphics->getConversationBox().isActive()) {
			if ((_cursorMode != kCursorModeWalk) || (_cursorActive != false)) {
				_cursorMode   = kCursorModeWalk;
				_cursorActive = false;
				setCursor();
			}
		}

		// Update the subsystems' status

		_vm->_talkMan->updateStatus();
		_vm->_roomConfMan->updateStatus();
		_vm->_graphics->updateStatus();

		if (!_vm->_mike->isBusy())
			scriptStateChanged = _vm->_inter->updateStatus();

		if (!_vm->_mike->isBusy()) {
			if (finishScripts) {
				// Just waiting for the scripts to finish

				// No room changing
				_changeRoom = false;
				if (!scriptStateChanged)
					// Scripts finished
					break;
			}

			// If the script variable "LastAction" is set, queue the last object verb scripts again
			if (_vm->_variables->get("LastAction") == 1) {
				_vm->_variables->set("LastAction", 0);
				if (_lastObject)
					_vm->_inter->interpret(_lastObject->getScripts(kObjectVerbUse));
			}

			// Room changing, but not during a syscall
			if (_changeRoom) {
				if (_vm->_variables->get("SysCall") == 0) {
					_changeRoom = false;

					roomGo(_nextRoom);
				}
			}
		}

		_vm->_mike->updateStatus();

		// Update screen
		_vm->_graphics->retrace();
		g_system->updateScreen();

		// Wait
		g_system->delayMillis(10);
	}
}

void Events::handleInput() {
	Common::Event event;

	bool hasMove = false;
	int32 mouseX, mouseY;

	while (g_system->getEventManager()->pollEvent(event)) {
		switch (event.type) {
		case Common::EVENT_MOUSEMOVE:
			hasMove = true;
			mouseX = event.mouse.x;
			mouseY = event.mouse.y;
			break;

		case Common::EVENT_LBUTTONUP:
			// If the mouse was moved, handle that beforehand
			if (hasMove)
				mouseMoved(mouseX, mouseY);
			hasMove = false;

			mouseClickedLeft(event.mouse.x, event.mouse.y);
			break;

		case Common::EVENT_RBUTTONUP:
			// If the mouse was moved, handle that beforehand
			if (hasMove)
				mouseMoved(mouseX, mouseY);
			hasMove = false;

			mouseClickedRight(event.mouse.x, event.mouse.y);
			break;

		case Common::EVENT_KEYDOWN:
			if (event.kbd.keycode == Common::KEYCODE_F5) {
				// Options, handled by the GMM
				_vm->openMainMenuDialog();
			} else if (event.kbd.keycode == Common::KEYCODE_ESCAPE) {
				if (_vm->_talkMan->isTalking())
					// Aborting talks with escape
					_vm->_talkMan->endTalk();
			}

			break;

		default:
			break;

		}
	}

	if (hasMove)
		mouseMoved(mouseX, mouseY);
}

void Events::handleMovieInput() {
	Common::Event event;

	while (g_system->getEventManager()->pollEvent(event)) {
		switch (event.type) {
		case Common::EVENT_KEYDOWN:
			if (event.kbd.keycode == Common::KEYCODE_ESCAPE)
				// Aborting the movie
				_vm->_movie->stop();
			break;

		default:
			break;
		}
	}
}

void Events::mouseMoved(int32 x, int32 y) {
	if ((_state == kStateIntro1) || (_state == kStateIntro2) || (_state == kStateIntro3)) {
		// Mouse in a button area?
		int titleSprite = checkTitleSprites(x, y);

		_cursorActive = (titleSprite != 0);
		setCursor();

		return;
	}

	// Update conversation box
	if (_vm->_graphics->getConversationBox().isActive()) {
		_vm->_graphics->getConversationBox().notifyMouseMove(x, y);
		return;
	}

	if (_vm->_variables->get("SysCall") == 0) {
	// Check for entering/leaving the inventory box
		if (y >= (_vm->_graphics->getScreenHeight() - 20))
			_vm->_graphics->getInventoryBox().show();
		else if (y < (_vm->_graphics->getScreenHeight() - InventoryBox::kHeight))
			_vm->_graphics->getInventoryBox().hide();

		// Look for hotspots
		checkHotspot(x, y);
	}
}

void Events::mouseClickedLeft(int32 x, int32 y) {
	if ((_state == kStateIntro1) || (_state == kStateIntro2) || (_state == kStateIntro3)) {
		// Mouse in a button area?
		int titleSprite = checkTitleSprites(x, y);

		if        (titleSprite == 1) {
			// New game
			leaveIntro();
		} else if (titleSprite == 2) {
			// Load game
			if (_vm->doLoadDialog())
				// Successful
				leaveIntro();
		} else if (titleSprite == 3) {
			// Options
			_vm->openMainMenuDialog();
		} else if (titleSprite == 4) {
			// Exit
			_vm->quitGame();
		}

		return;
	}

	// Update conversation box
	if (_vm->_graphics->getConversationBox().isActive())
		_vm->_graphics->getConversationBox().notifyClicked(x, y);

	// Did we click any objects? (But ignore if we're doing something important)
	if (_vm->_variables->get("SysCall") == 0) {
		Object *curObject = _vm->_graphics->getRoom().findObject(x, y);
		if (curObject)
			doObjectVerb(*curObject, cursorModeToObjectVerb(_cursorMode));

		if (_vm->_graphics->getInventoryBox().isVisible() && !_itemMode) {
			_itemRef = _vm->_graphics->getInventoryBox().doAction(x, y,
					cursorModeToObjectVerb(_cursorMode), _itemCursor);

			if ((_itemRef != 0) && (_itemCursor != 0)) {
				_itemVerb = cursorModeToObjectVerb(_cursorMode);
				_itemMode = true;
				setCursor(kCursorModeWalk, false);
			}
		}

	}

}

void Events::checkHotspot(int32 x, int32 y) {
	if (_vm->_graphics->getInventoryBox().isVisible()) {
		checkInventoryHotspot(x, y);
		return;
	}

	bool cursorActive = false;

	Object *curObject = _vm->_graphics->getRoom().findObject(x, y);
	if (curObject) {
		if (curObject->hasActiveVerb(cursorModeToObjectVerb(_cursorMode)))
			cursorActive = true;
	}

	if (cursorActive != _cursorActive) {
		_cursorActive = cursorActive;
		setCursor();
	}
}

void Events::checkInventoryHotspot(int32 x, int32 y) {
	bool cursorActive = _vm->_graphics->getInventoryBox().hasAction(x, y, cursorModeToObjectVerb(_cursorMode));

	if (cursorActive != _cursorActive) {
		_cursorActive = cursorActive;
		setCursor();
	}
}

void Events::mouseClickedRight(int32 x, int32 y) {
	if (!_vm->_graphics->getConversationBox().isActive() && !_vm->_talkMan->isTalking()) {
		// If no one is talking and the conversation box isn't shown, cycle the mouse cursors
		if (_vm->_variables->get("SysCall") == 0) {
			// If we're not doing something important
			cycleCursorMode();
			checkHotspot(x, y);
		}
	}

	if (_vm->_talkMan->isTalking())
		// Aborting talks with the right mouse button
		_vm->_talkMan->endTalk();
}

void Events::cycleCursorMode() {
	if (!_canSwitchCursors)
		return;

	if (_itemMode) {
		_vm->_graphics->getInventoryBox().undoAction(_itemRef, _itemVerb);
		_itemMode = false;
	}

	_cursorMode = (CursorMode) ((((int) _cursorMode) + 1) % kCursorModeNone);
	setCursor();
}

void Events::setCursor() {
	if (_itemMode) {
		if (_cursorActive)
			setCursor(*_itemCursor);
		else
			setCursor(kCursorModeWalk, false);

		return;
	}

	setCursor(_cursorMode, _cursorActive);
}

void Events::setCursor(CursorMode cursor, bool active) {
	if (_cursorMode >= kCursorModeNone)
		return;

	if (active)
		setCursor(*_cursors[(int) cursor].active);
	else
		setCursor(*_cursors[(int) cursor].inactive);
}

void Events::setCursor(const Cursors::Cursor &cursor) {
	_vm->_cursors->setCursor(cursor);
}

void Events::doObjectVerb(Object &object, ObjectVerb verb) {
	debugC(-1, kDebugGameflow, "Doing verb %d on object \"%s\"", verb, object.getName().c_str());

	_lastObject = &object;
	_vm->_inter->interpret(object.getScripts(verb));
}

int Events::checkTitleSprites(int32 x, int32 y) const {
	for (int i = 0; i < 4; i++)
		if (_titleSprites[i].isIn(x, y))
			return i + 1;

	return 0;
}

bool Events::roomEnter() {
	Room &room = _vm->_graphics->getRoom();

	debugC(-1, kDebugRooms, "Entering room \"%s\"", room.getName().c_str());

	// Set the background
	_vm->_graphics->registerBackground(room.getBackground());

	// Initialize the room
	room.init();

	// Evaluate the entry logic
	_vm->_inter->interpret(room.getEntryScripts());

	// Look for the autostart object
	findAutoStart(room);

	// Update Mike
	_vm->_mike->setVisible(true);
	_vm->_mike->setWalkMap(room.getWalkMap(), room.getWalkMapYTop(), room.getWalkMapYResolution());
	_vm->_mike->setScaleFactors(room.getScaleFactors());

	// Evaluate the room config, to get the initial room sprites set up
	_vm->_roomConfMan->updateStatus();

	// Allow for the transition movie to play at once
	for (int i = 0; (i < 10) && !_vm->_movie->isPlaying(); i++)
		_vm->_inter->updateStatus();

	return true;
}

void Events::roomLeave() {
	// Stop all sounds
	_vm->_sound->stopAll();

	// Reset Mike
	_vm->_mike->setWalkMap();

	// Clear graphics
	_vm->_graphics->unregisterBackground();
	// Clear scripts
	_vm->_inter->clear();
	_vm->_scriptRegister->clear();
	// Clear room
	_vm->_graphics->getRoom().clear();

	// No last object
	_lastObject = 0;
}

bool Events::roomGo(const Common::String &room) {
	Room &curRoom = _vm->_graphics->getRoom();

	roomLeave();

	// Primitive "garbadge collector"
	_vm->_resources->clearUncompressedData();

	if (!curRoom.parse(*_vm->_resources, room))
		return false;

	if (!roomEnter())
		return false;

	return true;
}

void Events::setNextRoom(uint32 room) {
	if (!((_state == kStateIntro1) || (_state == kStateIntro2) || (_state == kStateIntro3))) {
		Common::String nextRoom = Common::String::printf("%04d", room);

		if (nextRoom != _vm->_graphics->getRoom().getName()) {
			debugC(-1, kDebugGameflow, "Room transition %s->%s", _nextRoom.c_str(), nextRoom.c_str());
			_lastRoom = _nextRoom;
			_nextRoom = nextRoom;
			_changeRoom = true;
		}
	}
}

bool Events::cameFrom(uint32 room) const {
	if (room == 0)
		return true;

	return _lastRoom == Common::String::printf("%04d", room);
}

void Events::setLoading(bool load) {
	_vm->_cursors->assertCursorProperties();
	_loading = load;
}

bool Events::findAutoStart(Room &room) {
	Object *autoStart = room.findAutoObject();
	if (autoStart) {
		_lastObject = autoStart;
		return true;
	}

	return false;
}

ObjectVerb Events::cursorModeToObjectVerb(CursorMode cursorMode) {
	switch (cursorMode) {
	case kCursorModeWalk:
		return kObjectVerbGo;
	case kCursorModeUse:
		return kObjectVerbUse;
	case kCursorModeLook:
		return kObjectVerbLook;
	default:
		return kObjectVerbNone;
	}
}

bool Events::saveLoad(Common::Serializer &serializer, Resources &resources) {
	_lastObjectName.clear();
	_itemName.clear();
	_itemCursorName.clear();

	byte state      = (byte) _state;
	byte cursorMode = (byte) _cursorMode;
	byte itemVerb   = (byte) _itemVerb;

	if (serializer.isSaving()) {
		if (_lastObject)
			_lastObjectName = _lastObject->getName();
		if (_itemMode && _itemRef && _itemCursor) {
			_itemName       = _itemRef->name;
			_itemCursorName = _itemCursor->name;
		}
	}

	SaveLoad::sync(serializer, state);
	SaveLoad::sync(serializer, _canSwitchCursors);
	SaveLoad::sync(serializer, _cursorActive);
	SaveLoad::sync(serializer, cursorMode);
	SaveLoad::sync(serializer, _changeRoom);
	SaveLoad::sync(serializer, _itemMode);
	SaveLoad::sync(serializer, itemVerb);
	SaveLoad::sync(serializer, _lastObjectName);
	SaveLoad::sync(serializer, _itemName);
	SaveLoad::sync(serializer, _itemCursorName);
	SaveLoad::sync(serializer, _lastRoom);
	SaveLoad::sync(serializer, _nextRoom);

	_state      = (State) state;
	_cursorMode = (CursorMode) cursorMode;
	_itemVerb   = (ObjectVerb) itemVerb;

	return true;
}

bool Events::loading(Resources &resources) {
	_lastObject = 0;
	_itemRef    = 0;

	if (!_lastObjectName.empty())
		_lastObject = _vm->_graphics->getRoom().findObject(_lastObjectName);
	if (!_itemName.empty())
		_itemRef = _vm->_graphics->getInventoryBox().findItem(_itemName);
	_itemCursor = _vm->_cursors->getCursor(_itemCursorName);

	Room &room = _vm->_graphics->getRoom();
	_vm->_mike->setWalkMap(room.getWalkMap(), room.getWalkMapYTop(), room.getWalkMapYResolution());

	return true;
}

} // End of namespace DarkSeed2
