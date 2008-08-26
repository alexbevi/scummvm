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

#include <stdio.h>

#include "common/system.h"
#include "common/events.h"
#include "common/str.h"

#include "kom/kom.h"
#include "kom/actor.h"
#include "kom/input.h"
#include "kom/screen.h"

using Common::String;

namespace Kom {

Input::Input(KomEngine *vm, OSystem *system) : _system(system), _vm(vm), _debugMode(false),
	_mouseX(0), _mouseY(0), _leftClick(false), _rightClick(false) {
}

Input::~Input() {

}

void Input::loopInput() {
	if (_vm->_flicLoaded == 0) {
		handleMouse();
	}

	_leftClick = _rightClick = false;
}

void Input::handleMouse() {
	static Settings *settings = _vm->game()->settings();
	Character *playerChar = _vm->database()->getChar(0);
	uint16 gotoX, gotoY = 0;
	uint16 collideType;
	int donutState;

	if (1 && _leftClick) { // FIXME - check "invisible"?
		if (settings->mouseY >= INVENTORY_OFFSET) {
			// TODO - inventory
			printf("Inventory click!\n");

		// "Exit to"
		} else {
			if (settings->mouseState == 2) {
				gotoX = settings->collideBoxX;
				gotoY = settings->collideBoxY;
				collideType = 1;

			} else switch (settings->overType) {
			case 2:
				gotoX = settings->collideCharX;
				gotoY = settings->collideCharY;
				collideType = 2;
				break;
			case 3:
				gotoX = settings->collideObjX;
				gotoY = settings->collideObjY;
				collideType = 3;
				break;
			default:
				gotoX = settings->collideBoxX;
				gotoY = settings->collideBoxY;
				collideType = 1;
			}

			if (collideType == 2 || collideType == 3) {
				donutState = 0;
				// TODO

			} else if (settings->objectNum >= 0) {
				donutState = -1;
			} else {
				donutState = 0;
				_vm->game()->player()->command = CMD_SPRITE_SCENE;
			}

			if (donutState != -1) {
				switch (_vm->game()->player()->command) {
				case 0x64:
					if (collideType != 0) {
						playerChar->_gotoX = gotoX;
						playerChar->_gotoY = gotoY;
						_vm->game()->player()->commandState = 1;
						_vm->game()->player()->collideNum = -1;
						_vm->game()->player()->collideType = 0;
					}
					break;

				case 0x65:
					break;

				case 0x66:
					// TODO
					break;

				case 0x67:
					// TODO
					break;
				case 0x68:
					// TODO
					break;
				case 0x69:
					// TODO
					break;
				case 0x6A:
					// TODO
					break;
				case 0x6B:
					// TODO
					break;
				case 0x6C:
					// TODO
					break;
				default:
					error("Wrong player command");
				}
			}
		}
	}

	// TODO - handle right click, menu request
}

// TODO: hack
void Input::checkKeys() {
		Common::Event event;

		Common::EventManager *eventMan = _system->getEventManager();
		while (eventMan->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_KEYDOWN:
				if (event.kbd.flags == Common::KBD_CTRL) {
					if (event.kbd.keycode == 'd') {
						_debugMode = true;
					}
				} else {
					_inKey = event.kbd.keycode;
					switch (_inKey) {
						case Common::KEYCODE_ESCAPE:
							_system->quit();
							break;
						default:
							break;
					}
				}
				break;

			case Common::EVENT_MOUSEMOVE:
				_mouseX = event.mouse.x;
				_mouseY = event.mouse.y;
				break;

			case Common::EVENT_LBUTTONDOWN:
				_leftClick = true;
				break;

			case Common::EVENT_RBUTTONDOWN:
				_rightClick = true;
				break;

			case Common::EVENT_QUIT:
				_system->quit();
				break;

			default:
				break;
			}
		}
}

} // End of namespace Kom
