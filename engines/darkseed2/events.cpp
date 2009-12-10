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

namespace DarkSeed2 {

Events::Events(DarkSeed2Engine &vm) : _vm(&vm) {
	_cursors = new ModeCursors[kCursorModeNone];

	// Getting cursors
	_cursors[kCursorModeWalk].inactive = _vm->_cursors->getCursor();
	_cursors[kCursorModeWalk].active   = _vm->_cursors->getCursor("c4Ways");
	_cursors[kCursorModeUse ].inactive = _vm->_cursors->getCursor("cHand");
	_cursors[kCursorModeUse ].active   = _vm->_cursors->getCursor("cUseIt");
	_cursors[kCursorModeLook].inactive = _vm->_cursors->getCursor("cLook");
	_cursors[kCursorModeLook].active   = _vm->_cursors->getCursor("cLookAt");

	_inIntro = false;

	_itemMode = false;

	// Set the default (pointer) cursor
	_canSwitchCursors = true;

	_cursorMode   = kCursorModeWalk;
	_cursorActive = false;
	setCursor();

	_vm->_cursors->setVisible(true);

	_changeRoom = false;
}

Events::~Events() {
	delete[] _cursors;
}

bool Events::setupIntroSequence() {
	_canSwitchCursors = false;

	_cursorMode   = kCursorModeUse;
	_cursorActive = false;
	setCursor();

	debugC(-1, kDebugGameflow, "Entering cutscene room");

	// Cutscene room
	if (!roomGo("0001"))
		return false;

	_inIntro = true;

	// Run the main loop as long as scripts are still active
	mainLoop(true);

	debugC(-1, kDebugGameflow, "Entering title room");

	// Title room
	if (!roomGo("0002"))
		return false;

	// Run the main loop as long as scripts are still active
	mainLoop(true);

	// Loading title parts, for hotspot detection
	_titleSprites[0].loadFromBMP(*_vm->_resources, "002BTN01");
	_titleSprites[1].loadFromBMP(*_vm->_resources, "002BTN02");
	_titleSprites[2].loadFromBMP(*_vm->_resources, "002BTN03");
	_titleSprites[3].loadFromBMP(*_vm->_resources, "002BTN04");

	for (int i = 0; i < 4; i++) {
		if (_titleSprites[i].empty()) {
			warning("Events::setupIntroSequence(): Couldn't load title screen elements");
			return false;
		}
	}

	return true;
}

void Events::leaveIntro() {
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

	// Run the main loop as long as scripts are still active
	mainLoop(true);

	_inIntro = false;

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
}

void Events::mainLoop(bool finishScripts) {
	bool scriptStateChanged;

	while (!_vm->shouldQuit()) {
		if (_vm->_movie->isPlaying()) {
			// Special mode for movie playing
			handleMovieInput();

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

		// Always use that mode/activity when the conversation box is visible
		if (_vm->_graphics->getConversationBox().isActive()) {
			if ((_cursorMode != kCursorModeWalk) || (_cursorActive != false)) {
				_cursorMode   = kCursorModeWalk;
				_cursorActive = false;
				setCursor();
			}
		}

		// Update subsystem statuses
		_vm->_talkMan->updateStatus();
		_vm->_roomConfMan->updateStatus();
		_vm->_graphics->updateStatus();

		scriptStateChanged = _vm->_inter->updateStatus();
		if (finishScripts && !scriptStateChanged)
			// We run only to finish the scripts, but the scripts won't do anything
			break;

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
	uint32 mouseX, mouseY;

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

void Events::mouseMoved(uint32 x, uint32 y) {
	if (_inIntro) {
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
		if (y >= (Graphics::kScreenHeight - 20))
			_vm->_graphics->getInventoryBox().show();
		else if (y < (Graphics::kScreenHeight - InventoryBox::kHeight))
			_vm->_graphics->getInventoryBox().hide();

		// Look for hotspots
		checkHotspot(x, y);
	}
}

void Events::mouseClickedLeft(uint32 x, uint32 y) {
	if (_inIntro) {
		// Mouse in a button area?
		int titleSprite = checkTitleSprites(x, y);

		if        (titleSprite == 1) {
			// New game
			leaveIntro();
		} else if (titleSprite == 2) {
			// Load game
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

void Events::checkHotspot(uint32 x, uint32 y) {
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

void Events::checkInventoryHotspot(uint32 x, uint32 y) {
	bool cursorActive = _vm->_graphics->getInventoryBox().hasAction(x, y, cursorModeToObjectVerb(_cursorMode));

	if (cursorActive != _cursorActive) {
		_cursorActive = cursorActive;
		setCursor();
	}
}

void Events::mouseClickedRight(uint32 x, uint32 y) {
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

int Events::checkTitleSprites(uint32 x, uint32 y) const {
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

	// Evaluate the entry logic
	_vm->_inter->interpret(room.getEntryScripts());

	// Look for the autostart object
	findAutoStart(room);

	// Evaluate the room config, to get the initial room sprites set up
	_vm->_roomConfMan->updateStatus();

	return true;
}

void Events::roomLeave() {
	_vm->_graphics->unregisterBackground();
	_vm->_inter->clear();
	_vm->_graphics->getRoom().clear();

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
	if (!_inIntro) {
		Common::String nextRoom = Common::String::printf("%04d", room);

		if (nextRoom != _vm->_graphics->getRoom().getName()) {
			debugC(-1, kDebugGameflow, "Room transition %s->%s", _nextRoom.c_str(), nextRoom.c_str());
			_lastRoom = _nextRoom;
			_nextRoom = nextRoom;
			_changeRoom = true;
		}
	}
}

bool Events::cameFrom(uint32 room) {
	if (room == 0)
		return true;

	return _lastRoom == Common::String::printf("%04d", room);
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

} // End of namespace DarkSeed2
