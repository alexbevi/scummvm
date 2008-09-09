#include "innocent/debugger.h"

#include "innocent/exit.h"
#include "innocent/graphics.h"
#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/room.h"
#include "innocent/util.h"

namespace Innocent {

Debugger::Debugger(Engine *vm) : _vm(vm) {
	DCmd_Register("setBackdrop", WRAP_METHOD(Debugger, cmd_setBackdrop));
	DCmd_Register("paintText", WRAP_METHOD(Debugger, cmd_paintText));
	DCmd_Register("listExits", WRAP_METHOD(Debugger, cmd_listExits));

	DVar_Register("currentRoom", &(vm->logic()->_currentRoom), DVAR_INT);
}

Logic *Debugger::logic() const {
	return _vm->logic();
}

#define CMD(x) bool Debugger::cmd_##x(int argc, const char **argv)

CMD(listExits) {
	DebugPrintf("Room exits:\n");
	foreach (Exit *, logic()->room()->exits())
		DebugPrintf("\t%s\n", +(**it));
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

} // End of namespace Innocent
