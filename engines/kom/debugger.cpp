/* ScummVM - Scumm Interpreter
 * Copyright (C) 2003-2006 The ScummVM project
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

#include "gui/debugger.cpp"

#include "kom/kom.h"
#include "kom/debugger.h"

namespace Kom {

Debugger::Debugger(KomEngine *vm) : GUI::Debugger(), _vm(vm) {

	DCmd_Register("room", WRAP_METHOD(Debugger, Cmd_Room));
}

Debugger::~Debugger() {} // we need this here for __SYMBIAN32__

void Debugger::preEnter() {
}

void Debugger::postEnter() {
}

bool Debugger::Cmd_Room(int argc, const char **argv) {
	if (argc == 2) {
		uint16 roomNum = atoi(argv[1]);
		_vm->game()->enterLocation(roomNum);
		return false;
	}
	return true;
}
} // End of namespace Kom
