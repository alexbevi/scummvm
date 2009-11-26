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
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/conversationbox.h"

namespace DarkSeed2 {

Events::Events(DarkSeed2Engine &vm) : _vm(&vm) {
	_cursors = new const Cursors::Cursor**[kCursorModeNone];
	for (int i = 0; i < kCursorModeNone; i++)
		_cursors[i] = new const Cursors::Cursor*[2];

	// Getting cursors
	_cursors[kCursorModeWalk][0] = _vm->_cursors->getCursor();
	_cursors[kCursorModeWalk][1] = _vm->_cursors->getCursor("c4Ways");
	_cursors[kCursorModeLook][0] = _vm->_cursors->getCursor("cLook");
	_cursors[kCursorModeLook][1] = _vm->_cursors->getCursor("cLookAt");
	_cursors[kCursorModeUse] [0] = _vm->_cursors->getCursor("cHand");
	_cursors[kCursorModeUse] [1] = _vm->_cursors->getCursor("cUseIt");

	// Set the default (pointer) cursor
	_cursorMode = kCursorModeWalk;
	_vm->_cursors->setCursor(*_cursors[_cursorMode][0]);
	_vm->_cursors->setVisible(true);
}

Events::~Events() {
	for (int i = 0; i < kCursorModeNone; i++)
		delete[] _cursors[i];
	delete[] _cursors;
}

void Events::mainLoop() {
	bool restarted = false;

	while (!_vm->shouldQuit()) {
		handleInput();

		g_system->delayMillis(10);

		_vm->_talkMan->updateStatus();
		_vm->_graphics->updateStatus();

		_vm->_graphics->retrace();
		g_system->updateScreen();

		if (!_vm->_graphics->getConversationBox().isActive() && !restarted) {
			warning("Restarting conversation");
			_vm->_graphics->getConversationBox().restart();
			restarted = true;
		}
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

		case Common::EVENT_LBUTTONDOWN:
			// If the mouse was moved, handle that beforehand
			if (hasMove)
				mouseMoved(mouseX, mouseY);
			hasMove = false;

			mouseClickedLeft(event.mouse.x, event.mouse.y);
			break;

		case Common::EVENT_RBUTTONDOWN:
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
	uint32 convX = x - Graphics::_conversationX;
	uint32 convY = y - Graphics::_conversationY;

	_vm->_graphics->getConversationBox().notifyMouseMove(convX, convY);
}

void Events::mouseClickedLeft(uint32 x, uint32 y) {
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
	_cursorMode = (_cursorMode + 1) % kCursorModeNone;
	_vm->_cursors->setCursor(*_cursors[_cursorMode][0]);
}

} // End of namespace DarkSeed2
