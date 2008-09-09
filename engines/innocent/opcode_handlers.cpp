#include "innocent/inter.h"

#include "innocent/animation.h"
#include "innocent/graphics.h"
#include "innocent/util.h"

#include "common/util.h"

namespace Innocent {
#define OPCODE(num) template<> void Interpreter::opcodeHandler<num>(ValueVector a)

OPCODE(0x01) {
	// return (to caller or toplevel)
	debugC(1, kDebugLevelScript, "opcode 0x01: return (partial STUB)");
	// TODO: when it's not toplevel
	goBack();
}

OPCODE(0x12) {
 	// if sound is on then
	// (argument is a set of flags, 1 - adlib, 2 - sb, 4 - roland)
	debugC(1, kDebugLevelScript, "opcode 0x12: if sound is on then STUB");
 	failedCondition();
}

OPCODE(0x13) {
	// if left button is up
	// and current mode isn't null
	debugC(1, kDebugLevelScript, "opcode 0x13: if 1st button is up and mode isn't null partial STUB");
	if (_engine->_eventMan->getButtonState() & Common::EventManager::LBUTTON)
		failedCondition();
}

OPCODE(0x2d) {
	// else
	debugC(3, kDebugLevelScript, "opcode 0x2d: end if");
	endIf();
}

OPCODE(0x36) {
	// call
	debugC(3, kDebugLevelScript, ">>>opcode 0x36: call procedure %s", +a[0]);
	static_cast<CodePointer &>(a[0]).run();
	debugC(3, kDebugLevelScript, "<<<opcode 0x36: called procedure %s", +a[0]);
}

OPCODE(0x3d) {
	// save first arg -- instruction pointer -- for after skipping animation
	debugC(1, kDebugLevelScript, "opcode 0x3d: store position to continue if animation skipped to %s STUB", +a[0]);
}

OPCODE(0x55) {
	// paint text
	// args: left, top, colour, text
	debugC(3, kDebugLevelScript, "opcode 0x55: paint '%s' with colour %s at %s:%s", +a[3], +a[2], +a[0], +a[1]);
	_graphics->paintText(a[0], a[1], a[2], a[3]);
}

OPCODE(0x60) {
	// lookup locally
	// takes a list (1st)
	// a value (2nd)
	// a field (as offset from structure start) (3rd)
	// first word on the list is entry length in words (minus one for index)
	// then are entries, first word being index
	// finds entry matching index == value in the list and
	// saves value of specified field in 4th argument
	uint16 offset = static_cast<CodePointer &>(a[0]).offset();

	uint16 value;
	byte *pos = _base + offset;
	uint16 width = READ_LE_UINT16(pos);
	pos += 2;
	while(true) {
		uint16 index = READ_LE_UINT16(pos);
		if (index == 0xffff) {
			value = index;
			break;
		}
		pos += 2;
		if (index == a[1]) {
			value = READ_LE_UINT16(pos + a[2]);
			break;
		}
		pos += width * 2;
	}

	debugC(3, kDebugLevelScript, "opcode 0x60: %s = %d == search list %s for %s and return field %s", +a[3], value, +a[0], +a[1], +a[2]);
	a[3] = value;
}

OPCODE(0x70) {
	// assign
	debugC(3, kDebugLevelScript, "opcode 0x70: %s = %s", +a[0], +a[1]);
	a[0] = a[1];
}

OPCODE(0x72) {
	// assign 1
	debugC(3, kDebugLevelScript, "opcode 0x72: %s = 1", +a[0]);
	a[0] = 1;
}

OPCODE(0x73) {
	// assign 0
	debugC(3, kDebugLevelScript, "opcode 0x73: %s = 0", +a[0]);
	a[0] = 0;
}

OPCODE(0x9d) {
	// set protagonist
	debugC(3, kDebugLevelScript, "opcode 0x9d: set protagonist(%s)", +a[0]);
	_logic->setProtagonist(a[0]);
}

OPCODE(0xc2) {
	// add animation at cursor
	debugC(3, kDebugLevelScript, "opcode 0xc2: add animation %s at cursor partial STUB", +a[0]);
	_logic->addAnimation(new Animation(static_cast<CodePointer &>(a[0]), _graphics->cursorPosition()));
}

OPCODE(0xc8) {
	// set backdrop
	// (not sure what's the difference to c9)
	debugC(3, kDebugLevelScript, "opcode 0xc8: set backdrop(%s)", +a[0]);
	_graphics->setBackdrop(a[0]);
}

OPCODE(0xc9) {
	// set backdrop
	// does it set the default one?
	debugC(3, kDebugLevelScript, "opcode 0xc9: set backdrop(%s)", +a[0]);
	_graphics->setBackdrop(a[0]);
}

OPCODE(0xcc) {
	// go fullscreen
	debugC(1, kDebugLevelScript, "opcode 0xcc: go fullscreen STUB");
}

OPCODE(0xd6) {
	// change room
	debugC(3, kDebugLevelScript, "opcode 0xd6: change room(%s)", +a[0]);
	_logic->changeRoom(a[0]);
}

OPCODE(0xe6) {
	// set room loop code
	debugC(3, kDebugLevelScript, "opcode 0xe6: set room loop to %s", +a[0]);
	assert(a[0].holdsCode());
	_logic->setRoomLoop(static_cast<CodePointer &>(a[0]));
}

OPCODE(0xef) {
	// random
	uint16 value = _engine->getRandom(a[0]);
	debugC(3, kDebugLevelScript, "opcode 0xef: %s = %d == random(%s)", +a[1], value, +a[0]);
	a[1] = value;
}

// #define ANIMCODE(n) template<> void Animation::handle<n>()
// 
// ANIMCODE(2) {
// 	// set position
// 	uint16 left = READ_LE_UINT16(_code + 2);
// 	uint16 top = READ_LE_UINT16(_code + 4);
// 	setPosition(Common::Point(left, top));
// 	_code += 6;
// }
// 
// ANIMCODE(7) {
// 	// get sprite id from main variable
// 	const uint16 offset = READ_LE_UINT16(_code + 2);
// 	const byte *var = _resources->getGlobalWordVariable(offset/2);
// 	const uint16 sprite = READ_LE_UINT16(var);
// 	setSprite(sprite);
// }
// 
// ANIMCODE(26) {
// 	// set z index
// 	setZIndex(_code[1]);
// 	// TODO set field 16 to 0
// 	_code += 2;
// }

} // End of namespace Innocent
