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
#include "kom/database.h"

namespace Kom {

Debugger::Debugger(KomEngine *vm) : GUI::Debugger(), _vm(vm) {

	DCmd_Register("room", WRAP_METHOD(Debugger, Cmd_Room));
	DCmd_Register("proc", WRAP_METHOD(Debugger, Cmd_Proc));
}

bool Debugger::Cmd_Room(int argc, const char **argv) {
	if (argc == 2) {
		uint16 roomNum = atoi(argv[1]);
		_vm->game()->enterLocation(roomNum);
		return false;
	}
	return true;
}

bool Debugger::Cmd_Proc(int argc, const char **argv) {
	if (argc == 2) {
		uint16 procNum = atoi(argv[1]);
		Process *proc = _vm->database()->getProc(procNum);
		if (proc == NULL) {
			DebugPrintf("No such process: %hd\n", procNum);
			return true;
		}

		DebugPrintf("Process name is %s\n", proc->name);

		for (Common::List<Command>::iterator i = proc->commands.begin(); i != proc->commands.end(); ++i) {
			DebugPrintf("- Command %d - value %hd\n", i->cmd, i->value);

			for (Common::List<OpCode>::iterator j = i->opcodes.begin(); j != i->opcodes.end(); ++j) {
				DebugPrintf("|- Opcode %d - (%s, %d, %d, %d, %d, %d)\n", j->opcode,
						j->arg1, j->arg2, j->arg3, j->arg4, j->arg5, j->arg6);
			}
		}
	}
	return true;
}
} // End of namespace Kom
