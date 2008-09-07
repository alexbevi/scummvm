#include "innocent/inter.h"

#include "innocent/graphics.h"

#include "common/util.h"

namespace Innocent {
#define OPCODE(num) template<> void Interpreter::opcodeHandler<num>(Argument *args[])

OPCODE(0x01) {
	// return (to caller or toplevel)
	// TODO how does it play with 0x36?
	goBack();
}

OPCODE(0x12) {
	// if sound is on then
	debug(1, "sound test opcode 0x12 stub");
	failedCondition();
}

OPCODE(0x2d) {
	// else
	endIf();
}

OPCODE(0x36) {
	// call
	uint16 offset = args[0]->_ptr - _base;
	debug(2, "calling procedure 0x%04x", offset);
	run(offset);
}

OPCODE(0x3d) {
	// save first arg -- instruction pointer -- for after skipping animation
	debug(1, "set skip point opcode 0x3d stub");
}

OPCODE(0x60) {
	// lookup locally
	// takes a local variable index (1st)
	// a value (2nd)
	// an offset (3rd)
	// searches through a -1-terminated list starting at the variable for the value
	// returns it if found, -1 otherwise
	// if out arg is 4-byte, return a value from position shifted by the offset from the matched position
	// saves result in 4th argument
	byte *pos = _base + uint16(*args[0]);
	debug(2, "looking for %d starting at %p", uint16(*args[1]), pos);
	uint16 result;
	while ((result = READ_LE_UINT16(pos)) != 0xffff) {
		if (result == uint16(*args[1]))
			break;
		pos += 2;
	}

	*args[3] = result;
	debug(2, "result: %p = %d", args[3]->_ptr, result);
}

OPCODE(0x70) {
	// assign
	*args[0] = *args[1];
	debug(2, "%p = %d", args[0]->_ptr, uint16(*args[1]));
}

OPCODE(0x72) {
	// assign 1
	*args[0] = byte(1);
	debug(2, "%p = 1", args[0]->_ptr);
}

OPCODE(0x73) {
	// assign 0
	*args[0] = byte(1);
}

OPCODE(0x9d) {
	// set protagonist
	_logic->setProtagonist(*args[0]);
}

OPCODE(0xc2) {
	addPeriodiCall(args[0]->_ptr);
}

OPCODE(0xc8) {
	// set backdrop
	// (not sure what's the difference to c9)
	_graphics->setBackdrop(*args[0]);
}

OPCODE(0xc9) {
	// set backdrop
	_graphics->setBackdrop(*args[0]);
}

OPCODE(0xcc) {
	// start cutscene -- allow fade, fullscreen, etc.
	debug(1, "start cutscene opcode 0xcc stub");
}

OPCODE(0xd6) {
	// change room
	_logic->changeRoom(*args[0]);
}

OPCODE(0xe6) {
	// set room loop code
	setRoomLoop(args[0]->_ptr);
}

OPCODE(0xef) {
	// random
	*args[1] = _engine->getRandom(*args[0]);
	debug(2, "%p = %d (random, max %d)", args[1]->_ptr, uint16(*args[1]), uint16(*args[0]));
}

} // End of namespace Innocent
