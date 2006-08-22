/* ScummVM - Scumm Interpreter
 * Copyright (C) 2005-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
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

#include "common/stdafx.h"
#include "common/system.h"
#include "common/str.h"

#include "kom/kom.h"
#include "kom/actor.h"
#include "kom/input.h"

using Common::String;

namespace Kom {

Input::Input(KomEngine *vm, OSystem *system) : _system(system), _vm(vm), _debugMode(false) {

	_mouseActor = _vm->actorMan()->load(_vm->dataDir()->getChild("kom").getChild("oneoffs"), String("m_icons"));
	_vm->actorMan()->get(_mouseActor)->defineScope(0, 0, 3, 0);
	_vm->actorMan()->get(_mouseActor)->defineScope(1, 4, 4, 4);
	_vm->actorMan()->get(_mouseActor)->defineScopeAlias(2, Actor::_exitCursorAnimation, 16);
	_vm->actorMan()->get(_mouseActor)->defineScope(3, 15, 20, 15);
	_vm->actorMan()->get(_mouseActor)->defineScope(4, 14, 14, 14);
	_vm->actorMan()->get(_mouseActor)->defineScopeAlias(5, Actor::_inventoryCursorAnimation, 23);
	_vm->actorMan()->get(_mouseActor)->defineScope(6, 31, 31, 31);

	_vm->actorMan()->get(_mouseActor)->setScope(0, 2);
	_vm->actorMan()->get(_mouseActor)->setPos(100, 100);
}

Input::~Input() {

}

// TODO: hack
void Input::checkKeys() {
	uint32 end = _system->getMillis() + 50;
	do {
		OSystem::Event event;
		while (_system->pollEvent(event)) {
			switch (event.type) {
			case OSystem::EVENT_KEYDOWN:
				if (event.kbd.flags == OSystem::KBD_CTRL) {
					if (event.kbd.keycode == 'd') {
						printf("go go debugger mode!\n");
						_debugMode = true;
					}
				} else {
					_inKey = event.kbd.keycode;
					switch (_inKey) {
						case 27:
							_system->quit();
							break;
						case '1':
							_vm->actorMan()->get(_mouseActor)->setScope(0, 2);
							break;
						case '2':
							_vm->actorMan()->get(_mouseActor)->setScope(1, 2);
							break;
						case '3':
							_vm->actorMan()->get(_mouseActor)->setScope(2, 2);
							break;
						case '4':
							_vm->actorMan()->get(_mouseActor)->setScope(3, 2);
							break;
						case '5':
							_vm->actorMan()->get(_mouseActor)->setScope(4, 2);
							break;
						case '6':
							_vm->actorMan()->get(_mouseActor)->setScope(5, 2);
							break;
						case '7':
							_vm->actorMan()->get(_mouseActor)->setScope(6, 0);
							break;
						default:
							break;
					}
				}
				break;

			case OSystem::EVENT_MOUSEMOVE:
				_vm->actorMan()->get(_mouseActor)->setPos(event.mouse.x, event.mouse.y);
				break;

			case OSystem::EVENT_QUIT:
				_system->quit();
				break;

			default:
				break;
			}
		}

		_system->delayMillis(50);
	} while (_system->getMillis() < end);

}

} // End of namespace Kom
