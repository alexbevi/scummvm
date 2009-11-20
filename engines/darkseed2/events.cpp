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
	delete _cursors;
}

void Events::mainLoop() {
	while (!_vm->shouldQuit()) {
		handleInput();

		g_system->delayMillis(10);

		_vm->_talkMan->updateStatus();

		_vm->_graphics->retrace();
		g_system->updateScreen();
	}
}

void Events::handleInput() {
	Common::Event event;

	while (g_system->getEventManager()->pollEvent(event)) {
		switch (event.type) {
		case Common::EVENT_LBUTTONDOWN:
			{
				TalkLine talkLine(*_vm->_resources, "DNA006");
				_vm->_talkMan->talk(talkLine);
			}
			break;
		case Common::EVENT_RBUTTONDOWN:
			cycleCursorMode();
			break;
		default:
			break;

		}
	}
}

void Events::cycleCursorMode() {
	_cursorMode = (_cursorMode + 1) % kCursorModeNone;
	_vm->_cursors->setCursor(*_cursors[_cursorMode][0]);
}

} // End of namespace DarkSeed2
