#include "innocent/inter.h"

#include "innocent/graphics.h"

#include "common/util.h"

namespace Innocent {
#define OPCODE(num) template<> void Interpreter::opcodeHandler<num>(ValueVector a)

// OPCODE(0x01) {
// 	// return (to caller or toplevel)
// 	// TODO how does it play with 0x36?
// 	goBack();
// }
//

OPCODE(0x12) {
 	// if sound is on then
	// (argument is a set of flags, 1 - adlib, 2 - sb, 4 - roland)
	debugC(1, kDebugLevelScript, "opcode 0x12: if sound is on then STUB");
 	failedCondition();
}
// 
// OPCODE(0x2d) {
// 	// else
// 	endIf();
// }
// 
// OPCODE(0x36) {
// 	// call
// 	uint16 offset = args[0]->_ptr - _base;
// 	run(offset);
// }
// 
// OPCODE(0x3d) {
// 	// save first arg -- instruction pointer -- for after skipping animation
// }
// 
// OPCODE(0x55) {
// 	// paint text
// 	// args: left, top, colour, text
// 	_graphics->paintText(*args[0], *args[1], *args[2], static_cast<StringArgument *>(args[3])->translated());
// }
// 
// OPCODE(0x60) {
// 	// lookup locally
// 	// takes a list (1st)
// 	// a value (2nd)
// 	// a field (as offset from structure start) (3rd)
// 	// first word on the list is entry length in words (minus one for index)
// 	// then are entries, first word being index
// 	// finds entry matching index == value in the list and
// 	// saves value of specified field in 4th argument
// 	byte *pos = args[0]->_ptr;
// 	uint16 width = READ_LE_UINT16(pos);
// 	pos += 2;
// 	while(true) {
// 		uint16 index = READ_LE_UINT16(pos);
// 		if (index == 0xffff) {
// 			*args[3] = index;
// 			break;
// 		}
// 		pos += 2;
// 		if (index == uint16(*args[1])) {
// 			*args[3] = READ_LE_UINT16(pos + uint16(*args[2]));
// 			break;
// 		}
// 		pos += width * 2;
// 	}
// 
// }
// 
// OPCODE(0x70) {
// 	// assign
// 	*args[0] = *args[1];
// }
// 
// OPCODE(0x72) {
// 	// assign 1
// 	*args[0] = byte(1);
// }
// 
// OPCODE(0x73) {
// 	// assign 0
// 	*args[0] = byte(1);
// }
// 
OPCODE(0x9d) {
	// set protagonist
	debugC(3, kDebugLevelScript, "opcode 0x9d: set protagonist(%s)", +a[0]);
	_logic->setProtagonist(a[0]);
}
// 
// OPCODE(0xc2) {
// 	// add animation
// 	addAnimation(args[0]->_ptr);
// }
// 
// OPCODE(0xc8) {
// 	// set backdrop
// 	// (not sure what's the difference to c9)
// 	_graphics->setBackdrop(*args[0]);
// }
// 
// OPCODE(0xc9) {
// 	// set backdrop
// 	_graphics->setBackdrop(*args[0]);
// }
// 
// OPCODE(0xcc) {
// 	// go fullscreen
// }
// 
// OPCODE(0xd6) {
// 	// change room
// 	_logic->changeRoom(*args[0]);
// }
// 
// OPCODE(0xe6) {
// 	// set room loop code
// 	setRoomLoop(args[0]->_ptr);
// }
// 
// OPCODE(0xef) {
// 	// random
// 	*args[1] = uint16(_engine->getRandom(*args[0]));
// }

#define ANIMCODE(n) template<> void Animation::handle<n>()

ANIMCODE(2) {
	// set position
	uint16 left = READ_LE_UINT16(_code + 2);
	uint16 top = READ_LE_UINT16(_code + 4);
	setPosition(Common::Point(left, top));
	_code += 6;
}

ANIMCODE(7) {
	// get sprite id from main variable
	const uint16 offset = READ_LE_UINT16(_code + 2);
	const byte *var = _resources->getGlobalWordVariable(offset/2);
	const uint16 sprite = READ_LE_UINT16(var);
	setSprite(sprite);
}

ANIMCODE(26) {
	// set z index
	setZIndex(_code[1]);
	// TODO set field 16 to 0
	_code += 2;
}

} // End of namespace Innocent
