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
	// TODO - more checks
	if (_vm->_flicLoaded == 0) {
		handleMouse();
	}
}

void Input::handleMouse() {
	static Settings *settings = _vm->game()->settings();
	static Player *player = _vm->game()->player();
	Character *playerChar = _vm->database()->getChar(0);
	uint16 gotoX, gotoY = 0;
	int8 donutState;

	if (settings->mouseMode == 0 && _leftClick) {
		if (settings->mouseY >= INVENTORY_OFFSET) {
			// TODO - inventory
			printf("Inventory click!\n");

		// "Exit to"
		} else {
			if (settings->mouseState == 2) {
				gotoX = settings->collideBoxX;
				gotoY = settings->collideBoxY;
				settings->collideType = 1;

			} else switch (settings->overType) {
			case 2:
				gotoX = settings->collideCharX;
				gotoY = settings->collideCharY;
				settings->collideType = 2;
				break;
			case 3:
				gotoX = settings->collideObjX;
				gotoY = settings->collideObjY;
				settings->collideType = 3;
				break;
			default:
				gotoX = settings->collideBoxX;
				gotoY = settings->collideBoxY;
				settings->collideType = 1;
			}

			if (settings->collideType == 2 || settings->collideType == 3) {
				if (settings->objectNum < 0) {
					donutState = _vm->game()->doDonut(0, false);
				} else {
					donutState = 0;
					// TODO
				}

			} else if (settings->objectNum >= 0) {
				donutState = -1;
			} else {
				donutState = 0;
				player->command = CMD_WALK;
			}

			if (donutState != -1) {
				switch (player->command) {
				case CMD_WALK:
					if (settings->collideType == 0)
						break;
					playerChar->_gotoX = gotoX;
					playerChar->_gotoY = gotoY;
					player->commandState = 1;
					player->collideType = 0;
					player->collideNum = -1;
					break;

				case CMD_NOTHING:
					break;

				case CMD_USE:
					// TODO
					break;

				case CMD_TALK_TO:
					break; // TODO
					if (settings->collideType == 0)
						break;
					playerChar->_gotoX = gotoX;
					playerChar->_gotoY = gotoY;
					player->commandState = 1;
					player->collideType = 2;
					player->collideNum = settings->collideChar;
					playerChar->_isBusy = true;
					break;

				case CMD_PICKUP:
					if (settings->collideType == 0)
						break;
					playerChar->_gotoX = gotoX;
					playerChar->_gotoY = gotoY;
					player->commandState = 1;
					player->collideType = 3;
					player->collideNum = settings->collideObj;
					playerChar->_isBusy = true;
					break;

				case CMD_LOOK_AT:
					if (settings->collideType == 0)
						break;
					playerChar->_gotoX = gotoX;
					playerChar->_gotoY = gotoY;
					player->commandState = 1;

					assert(settings->collideType > 1);

					// Character
					if (settings->collideType == 2) {
						player->collideType = 2;
						player->collideNum = settings->collideChar;
						if (_vm->database()->
								getChar(settings->collideChar)->_isAlive) {
							playerChar->_gotoX = playerChar->_screenX;
							playerChar->_gotoY = playerChar->_screenY;
						}
						playerChar->_isBusy = true;

					// Box
					} else {
						player->collideType = 3;
						player->collideNum = settings->collideObj;
						playerChar->_isBusy = true;
					}
					break;

				case CMD_FIGHT:
					// TODO
					break;

				case CMD_CAST_SPELL:
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


void Input::setMousePos(uint16 x, uint16 y) {
	_mouseX = x;
	_mouseY = y;
	_system->warpMouse(x, y);
}

} // End of namespace Kom
