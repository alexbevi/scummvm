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

#include "kom/input.h"

namespace Kom {

Input::Input(OSystem *system) : _system(system), _debugMode(false) {

}

Input::~Input() {

}

// TODO: hack
void Input::checkKeys() {
	uint32 end = _system->getMillis() + 10;
	do {
		OSystem::Event event;
		while (_system->pollEvent(event)) {
			printf("got event\n");
			switch (event.type) {
			case OSystem::EVENT_KEYDOWN:
				if (event.kbd.flags == OSystem::KBD_CTRL) {
					if (event.kbd.keycode == 'd')
						printf("go go debugger mode!\n");
						_debugMode = true;
				} else {
					_inKey = event.kbd.keycode;
					if (_inKey == 27) // escape
						_system->quit();
				}
				break;

			case OSystem::EVENT_QUIT:
				_system->quit();
				break;

			default:
				break;
			}
		}

		_system->delayMillis(10);
	} while (_system->getMillis() < end);

}

} // End of namespace Kom
