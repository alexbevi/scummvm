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

#include "innocent/debugger.h"

#include "common/endian.h"
#include "common/rect.h"

#include "innocent/actor.h"
#include "innocent/exit.h"
#include "innocent/eventmanager.h"
#include "innocent/graphics.h"
#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/resources.h"
#include "innocent/room.h"
#include "innocent/util.h"

namespace Common {
	DECLARE_SINGLETON(Innocent::Debugger);
}

namespace Innocent {
//

Debugger::Debugger()
  :	_stepOpcodes(false),
	_breakOnClickHandler(false),
	_vm(0) {
	DCmd_Register("setBackdrop", WRAP_METHOD(Debugger, cmd_setBackdrop));
	DCmd_Register("paintText", WRAP_METHOD(Debugger, cmd_paintText));
	DCmd_Register("listExits", WRAP_METHOD(Debugger, cmd_listExits));
	DCmd_Register("showClickable", WRAP_METHOD(Debugger, cmd_showClickable));
	DCmd_Register("paintSprite", WRAP_METHOD(Debugger, cmd_paintSprite));
	DCmd_Register("break", WRAP_METHOD(Debugger, cmd_break));
	DCmd_Register("step", WRAP_METHOD(Debugger, cmd_step));
	DCmd_Register("setVar", WRAP_METHOD(Debugger, cmd_setVar));
#define CMD(x) DCmd_Register(#x, WRAP_METHOD(Debugger, cmd_##x));
	CMD(debugActor);
#undef CMD
}

void Debugger::setEngine(Engine *vm) {
	_vm = vm;
	DVar_Register("currentRoom", &(_vm->logic()->_currentRoom), DVAR_INT, 0);
}

Logic *Debugger::logic() const {
	return _vm->logic();
}

#define CMD(x) bool Debugger::cmd_##x(int argc, const char **argv)

CMD(debugActor) {
	if (argc == 2) {
		int actorNum = atoi(argv[1]);
		Log.getActor(actorNum)->toggleDebug();
		DebugPrintf("Toggled debugging on actor %d. Remember to toggle proper levels, too!\n", actorNum);
	} else
		DebugPrintf("Syntax: debugActor <id>\n");
	return true;
}

CMD(break) {
	if (argc == 2) {
		if (!strcmp(argv[1], "click")) {
			DebugPrintf("Will break execution on click handler.\n");
			_breakOnClickHandler = true;
		}
	} else
		DebugPrintf("Syntax: break <event>     (events are: click)\n");
	return true;
}

void Debugger::clickHandler() {
	if (_breakOnClickHandler) {
		attach();
		onFrame();
	}
}

CMD(step) {
	_stepOpcodes = true;
	//_detach_now = true;
	return false;
}

CMD(setVar) {
	if (argc == 4) {
		if (!strcmp(argv[1], "word")) {
			int var = atoi(argv[2]);
			int val = atoi(argv[3]);
			DebugPrintf("word[%d] = %d\n", var, val);
			WRITE_LE_UINT16(_vm->resources()->getGlobalWordVariable(var), val);
		}
	} else
		DebugPrintf("Syntax: break <event>     (events are: click)\n");
	return true;
}

CMD(showClickable) {
	EventManager::instance().toggleDebug();
	return true;
}

CMD(listExits) {
	DebugPrintf("Room exits:\n");
	foreach_const (Exit *, logic()->room()->exits())
		DebugPrintf("  %s\n", +(**it));
	DebugPrintf("\n");
	return true;
}

bool Debugger::cmd_setBackdrop(int argc, const char **argv) {
	if (argc == 2) {
		_vm->graphics()->setBackdrop(atoi(argv[1]));
		_vm->graphics()->paintBackdrop();
	} else
		DebugPrintf("Syntax: set_backdrop <index>\n");

	return true;
}

bool Debugger::cmd_paintText(int argc, const char **argv) {
	if (argc >= 2) {
		int left = 10;
		int top = 10;
		byte colour = 235;
		if (argc >= 4) {
			left = atoi(argv[2]);
			top = atoi(argv[3]);
			if (argc >= 5)
				colour = atoi(argv[4]);
		}
		_vm->graphics()->paintText(left, top, colour, const_cast<byte *>(reinterpret_cast<const byte *>(argv[1])));
	} else
		DebugPrintf("Syntax: paint_text <text> [<left> <top> [<colour>]]\n");

	return true;
}

bool Debugger::cmd_paintSprite(int argc, const char **argv) {
	if (argc >= 2) {
		int sprite = atoi(argv[1]);
		int left = 10;
		int top = 10;
		if (argc >= 4) {
			left = atoi(argv[2]);
			top = atoi(argv[3]);
		}
		Sprite *s = _vm->resources()->loadSprite(sprite);
		_vm->graphics()->paint(s, Common::Point(left, top));
		_vm->graphics()->updateScreen();
	} else
		DebugPrintf("Syntax: paintSprite <text> [<left> <top>]\n");

	return true;
}

} // End of namespace Innocent
