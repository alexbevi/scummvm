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

using Common::String;

namespace Kom {

Input::Input(KomEngine *vm, OSystem *system) : _system(system), _vm(vm), _debugMode(false),
	_mouseX(0), _mouseY(0) {
}

Input::~Input() {

}

void Input::loopInput() {
	// TODO: if flic not loaded
	handleMouse();
}

void Input::handleMouse() {
	if (1) {// FIXME - check "invisible"?
	} else {
	}
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

			case Common::EVENT_QUIT:
				_system->quit();
				break;

			default:
				break;
			}
		}
}

} // End of namespace Kom
