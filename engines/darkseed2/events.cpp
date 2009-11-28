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
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/room.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/inter.h"

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

	// Set the default (pointer) cursor
	_canSwitchCursors = true;

	_cursorMode   = kCursorModeWalk;
	_cursorActive = false;
	setCursor();

	_vm->_cursors->setVisible(true);
}

Events::~Events() {
	delete[] _cursors;
}

bool Events::setupIntroSequence() {
	_canSwitchCursors = false;

	_cursorMode   = kCursorModeUse;
	_cursorActive = false;
	setCursor();

	// Cutscene room
	if (!_vm->_graphics->getRoom().parse(*_vm->_resources, "0001"))
		return false;

	_inIntro = true;

	if (!roomEnter())
		return false;

	// Title room
	if (!_vm->_graphics->getRoom().parse(*_vm->_resources, "0002"))
		return false;

	if (!roomEnter())
		return false;

	// Loading title parts
	_titleSprites[0].loadFromBMP(*_vm->_resources, "002TIT01", 145,  27);
	_titleSprites[1].loadFromBMP(*_vm->_resources, "002BTN01", 164, 196);
	_titleSprites[2].loadFromBMP(*_vm->_resources, "002BTN02", 164, 256);
	_titleSprites[3].loadFromBMP(*_vm->_resources, "002BTN03", 164, 314);
	_titleSprites[4].loadFromBMP(*_vm->_resources, "002BTN04", 164, 376);

	for (int i = 0; i < 5; i++) {
		// Draw those parts

		if (_titleSprites[i].empty()) {
			warning("Events::setupIntroSequence(): Couldn't load title screen elements");
			return false;
		}

		_vm->_graphics->blitToScreen(_titleSprites[i].getSprite(),
				_titleSprites[i].getX(), _titleSprites[i].getY(), true);
	}

	return true;
}

void Events::leaveIntro() {
	// Throw title parts away again
	for (int i = 0; i < 5; i++)
		_titleSprites[i].clear();

	// Intro movie room
	if (!_vm->_graphics->getRoom().parse(*_vm->_resources, "1501")) {
		warning("Events::leaveIntro(): Failed loading the intro movie room");
		_vm->quitGame();
		return;
	}

	if (!roomEnter()) {
		warning("Events::leaveIntro(): Failed entering the intro movie room");
		_vm->quitGame();
		return;
	}

	_inIntro = false;

	// Restore normal cursor mode
	_canSwitchCursors = true;
	_cursorMode   = kCursorModeWalk;
	_cursorActive = false;
	setCursor();

	// First room
	if (!_vm->_graphics->getRoom().parse(*_vm->_resources, "0101")) {
		warning("Events::leaveIntro(): Failed loading the first room");
		_vm->quitGame();
		return;
	}

	if (!roomEnter()) {
		warning("Events::leaveIntro(): Failed entering the first room");
		_vm->quitGame();
		return;
	}
}

void Events::mainLoop() {
	while (!_vm->shouldQuit()) {
		// Look for user input
		handleInput();

		// Update subsystem statuses
		_vm->_talkMan->updateStatus();
		_vm->_graphics->updateStatus();

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
			if (event.kbd.keycode == Common::KEYCODE_F5)
				// Options, handled by the GMM
				_vm->openMainMenuDialog();
			break;

		default:
			break;

		}
	}

	if (hasMove)
		mouseMoved(mouseX, mouseY);
}

void Events::mouseMoved(uint32 x, uint32 y) {
	if (_inIntro) {
		// Mouse in a button area?
		int titleSprite = checkTitleSprites(x, y);

		_cursorActive = (titleSprite != 0);
		setCursor();

		return;
	}

	uint32 convX = x - Graphics::_conversationX;
	uint32 convY = y - Graphics::_conversationY;

	_vm->_graphics->getConversationBox().notifyMouseMove(convX, convY);
}

void Events::mouseClickedLeft(uint32 x, uint32 y) {
	if (_inIntro) {
		// Mouse in a button area?
		int titleSprite = checkTitleSprites(x, y);

		if        (titleSprite == 2) {
			// New game
			leaveIntro();
		} else if (titleSprite == 3) {
			// Load game
		} else if (titleSprite == 4) {
			// Options
			_vm->openMainMenuDialog();
		} else if (titleSprite == 5) {
			// Exit
			_vm->quitGame();
		}

		return;
	}

	uint32 convX = x - Graphics::_conversationX;
	uint32 convY = y - Graphics::_conversationY;

	_vm->_graphics->getConversationBox().notifyClicked(convX, convY);
}

void Events::mouseClickedRight(uint32 x, uint32 y) {
	if (!_vm->_graphics->getConversationBox().isActive() && !_vm->_talkMan->isTalking())
		// If no one is talking and the conversation box isn't shown, cycle the mouse cursors
		cycleCursorMode();

	if (_vm->_talkMan->isTalking())
		// Aborting talks with the right mouse button
		_vm->_talkMan->endTalk();
}

void Events::cycleCursorMode() {
	if (!_canSwitchCursors)
		return;

	_cursorMode = (CursorMode) ((((int) _cursorMode) + 1) % kCursorModeNone);
	setCursor();
}

void Events::setCursor() {
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

int Events::checkTitleSprites(uint32 x, uint32 y) const {
	for (int i = 1; i < 5; i++)
		if (_titleSprites[i].isIn(x, y))
			return i + 1;

	return 0;
}

bool Events::roomEnter() {
	Room &room = _vm->_graphics->getRoom();

	// Set the background
	_vm->_graphics->registerBackground(room.getBackground());

	// Evaluate music changes
	if (!_vm->_inter->interpret(room.getScripts(kRoomVerbMusic)))
		return false;

	// Evaluate the entry logic
	if (!_vm->_inter->interpret(room.getScripts(kRoomVerbEntry)))
		return false;

	// Evaluate the autostart objects
	executeAutoStart(room);
	return true;
}

static const char *autoStartName[] = {
	"autostart", "auto start", "autoroom", "auto room"
};

bool Events::executeAutoStart(Room &room) {
	bool has = false;
	Object *autoStart;

	// Looking for all different auto start objects
	for (int i = 0; i < ARRAYSIZE(autoStartName); i++) {
		autoStart = room.findObject(autoStartName[i]);
		if (autoStart) {
			_vm->_inter->interpret(autoStart->getScripts(kObjectVerbUse));
			has = true;
		}
	}

	return has;
}

} // End of namespace DarkSeed2
