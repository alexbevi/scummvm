#include "innocent/inter.h"

#include "innocent/actor.h"
#include "innocent/animation.h"
#include "innocent/exit.h"
#include "innocent/graphics.h"
#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/movie.h"
#include "innocent/util.h"

#include "common/events.h"
#include "common/util.h"

namespace Innocent {
#define OPCODE(num) template<> void Interpreter::opcodeHandler<num>(ValueVector a)

OPCODE(0x00) {
	// nop
	debugC(3, kDebugLevelScript, "opcode 0x00: nop");
}

OPCODE(0x01) {
	// exit
	// (some peculiarities in conj. with op 0x38, needs research TODO)
	debugC(3, kDebugLevelScript, "opcode 0x01: exit");
	goBack();
}

OPCODE(0x02) {
	// check equality
	debugC(3, kDebugLevelScript, "opcode 0x02: if %s == %s", +a[0], +a[1]);
	unless (a[0] == a[1])
		failedCondition();
}

OPCODE(0x03) {
	// check inequality
	debugC(3, kDebugLevelScript, "opcode 0x03: if %s != %s", +a[0], +a[1]);
	if (a[0] == a[1])
		failedCondition();
}

OPCODE(0x04) {
	// less than
	debugC(3, kDebugLevelScript, "opcode 0x04: if %s < %s", +a[0], +a[1]);
	unless (a[0] < a[1])
		failedCondition();
}

OPCODE(0x05) {
	// greater than
	debugC(3, kDebugLevelScript, "opcode 0x05: if %s > %s", +a[0], +a[1]);
	unless (a[0] > a[1])
		failedCondition();
}

OPCODE(0x0f) {
	// check room
	debugC(3, kDebugLevelScript, "opcode 0x0f: if current room == %s then", +a[0]);
	unless (a[0] == _logic->currentRoom())
		failedCondition();
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

OPCODE(0x1f) {
	// if actor in current room then whatever
	debugC(1, kDebugLevelScript, "opcode 0x1f: if actor %s in current room and STUB then", +a[1]);
	if (_logic->getActor(a[1])->room() == _logic->currentRoom())
		error("case with condition true unhandled");
	else
		failedCondition();
}

OPCODE(0x24) {
	// check nonzeroness
	debugC(3, kDebugLevelScript, "opcode 0x24: if (%s)", +a[0]);
	if (a[0] == 0)
		failedCondition();
}

OPCODE(0x2c) {
	// else
	debugC(3, kDebugLevelScript, "opcode 0x2c: else");

	// this won't work for nested ifs, but that's how it is in the original code
	if (_failedCondition < 2)
		_failedCondition ^= 1;
}

OPCODE(0x2d) {
	// end if
	debugC(3, kDebugLevelScript, "opcode 0x2d: end if");
	endIf();
}

OPCODE(0x35) {
	// jump
	debugC(3, kDebugLevelScript, "opcode 0x35: jump to %s", +a[0]);
	_code = static_cast<CodePointer &>(a[0]).code();
}

OPCODE(0x36) {
	// call
	debugC(3, kDebugLevelScript, ">>>opcode 0x36: call procedure %s", +a[0]);
	CodePointer &p = static_cast<CodePointer &>(a[0]);
/*	if (p.offset() == 0x3100)
		_code = _base + 0x335c;
	else*/
		p.run();
	debugC(3, kDebugLevelScript, "<<<opcode 0x36: called procedure %s", +a[0]);
}

OPCODE(0x39) {
	// run later
	debugC(3, kDebugLevelScript, "opcode 0x39: execute main %s later", +a[0]);
	_logic->runLater(CodePointer(static_cast<CodePointer &>(a[0]).offset(),
								  _logic->mainInterpreter()));
}

OPCODE(0x3d) {
	// save first arg -- instruction pointer -- for after skipping animation
	debugC(1, kDebugLevelScript, "opcode 0x3d: store position to continue if animation skipped to %s STUB", +a[0]);
}

OPCODE(0x54) {
	debugC(3, kDebugLevelScript, "opcode 0x54: ask about '%s' at %s:%s %sx%s", +a[4], +a[0], +a[1], +a[2], +a[3]);

	uint16 result;
	unless ((result = _graphics->ask(a[0], a[1], a[2], a[3], a[4])) == 0xffff)
		_code = _base + result;
}

OPCODE(0x55) {
	// paint text
	// args: left, top, colour, text
	debugC(3, kDebugLevelScript, "opcode 0x55: paint '%s' with colour %s at %s:%s", +a[3], +a[2], +a[0], +a[1]);
	_graphics->paintText(a[0], a[1], a[2], a[3]);
}

OPCODE(0x56) {
	// say text
	debugC(3, kDebugLevelScript, "opcode 0x86: say %s for %s frames", +a[1], +a[0]);
	Graf.say(a[1], a[1], a[0]);
}

OPCODE(0x57) {
	// wait until said
	debugC(3, kDebugLevelScript, "opcode 0x57: wait until text is said");
	Graf.runWhenSaid(nextInstruction());
	_return = true;
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

OPCODE(0x63) {
	// get actor property
	Actor *actor = _logic->getActor(a[0]);
	const char *desc;

	switch (uint16(a[1]) & 0xff) {
	case Actor::kOffsetRoom:
		desc = "Room";
		a[2] = actor->room();
		break;
/*	case Actor::kOffsetOffset:
		desc = "Offset";
		a[2] = actor->offset();
		break;
	case Actor::kOffsetLeft:
		desc = "Left";
		a[2] = actor->left();
		break;
	case Actor::kOffsetTop:
		desc = "Top";
		a[2] = actor->top();
		break;
	case Actor::kOffsetMainSprite:
		desc = "MainSprite";
		a[2] = actor->mainSprite();
		break;
	case Actor::kOffsetTicksLeft:
		desc = "TicksLeft";
		a[2] = actor->ticksLeft();
		break;
	case Actor::kOffsetCode:
		desc = "Code";
		a[2] = actor->code();
		break;
	case Actor::kOffsetInterval:
		desc = "Interval";
		a[2] = actor->interval();
		break;*/
	default:
		error("unhandled actor property %s", +a[1]);
	}

	debugC(3, kDebugLevelScript, "opcode 0x63: %s = get %s of actor %s", +a[2], desc, +a[0]);
}

OPCODE(0x6d) {
	// increment
	debugC(3, kDebugLevelScript, "opcode 0x6d: %s++", +a[0]);
	a[0]++;
}

OPCODE(0x6f) {
	// decrement
	debugC(3, kDebugLevelScript, "opcode 0x6d: %s--", +a[0]);
	a[0]--;
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

OPCODE(0x77) {
	// initialize protagonist
	debugC(1, kDebugLevelScript, "opcode 0x77: go to room %s facing %s partial STUB", +a[0], +a[1]);
	_logic->protagonist()->setRoom(a[0]);
	_logic->changeRoom(a[0]);
}

OPCODE(0x79) {
	// move actor to another room
	debugC(3, kDebugLevelScript, "opcode 0x79: move actor %s to room %s (and set current animation frame to %s STUB)", +a[0], +a[1], +a[2]);
	_logic->getActor(a[0])->setRoom(a[1]);
}

OPCODE(0x7c) {
	// toggle exit state
	Exit *exit = _logic->blockProgram()->getExit(a[0]);
	debugC(3, kDebugLevelScript, "opcode 0x7c: toggling exit %s to %s", +a[0], exit->isEnabled() ? "disabled" : "enabled");
	exit->setEnabled(!exit->isEnabled());
}

OPCODE(0x9a) {
	// if actor in current room then whatever
	debugC(1, kDebugLevelScript, "opcode 0x9a: if actor %s in current room then STUB", +a[0]);
	if (_logic->getActor(a[0])->room() == _logic->currentRoom())
		error("case with condition true unhandled");
}

OPCODE(0x9b) {
	// delay
	debugC(3, kDebugLevelScript, "opcode 0x9b: delay %s frames", +a[0]);
	_logic->runLater(CodePointer(_code - _base, this), a[0]);
	_return = true;
}

OPCODE(0x9c) {
	// if actor in current room then whatever
	debugC(1, kDebugLevelScript, "opcode 0x9c: if actor %s in current room then STUB", +a[0]);
	if (_logic->getActor(a[0])->room() == _logic->currentRoom())
		error("case with condition true unhandled");
}

OPCODE(0x9d) {
	// set protagonist
	debugC(3, kDebugLevelScript, "opcode 0x9d: set protagonist(%s)", +a[0]);
	_logic->setProtagonist(a[0]);
}

OPCODE(0xad) {
	// turn actor
	debugC(1, kDebugLevelScript, "opcode 0xad: turn actor %s to %s or wait on actor STUB", +a[0], +a[1]);
	if (_logic->getActor(a[0])->room() != _logic->currentRoom()) {
		// stash further code for when actor shows up
		_return = true;
	} else
		error("case with condition true unhandled");
}

OPCODE(0xc2) {
	// add animation at cursor
	debugC(3, kDebugLevelScript, "opcode 0xc2: add animation %s at cursor partial STUB", +a[0]);
	_logic->addAnimation(new Animation(static_cast<CodePointer &>(a[0]), _graphics->cursorPosition()));
}

OPCODE(0xd2) {
	// prepare interface palette
	debugC(3, kDebugLevelScript, "opcode 0xd2: will fadein partially");
	Graf.willFadein(Graphics::kPartialFade);
}

OPCODE(0xc6) {
	// suspend execution until an animation's ip points to 0xff
	debugC(3, kDebugLevelScript, "opcode 0xc6: wait on animation %s", +a[0]);
	_logic->animation(a[0])->runOnNextFrame(CodePointer(_code - _base, this));
	goBack();
}

OPCODE(0xc7) {
	// play movie
	debugC(3, kDebugLevelScript, "opcode 0xc7: play movie %s with slowness %s", +a[0], +a[1]);
	Movie *m = Movie::fromFile(reinterpret_cast<char *>((byte *)(a[0])));
	m->setFrameDelay(a[1]);
	m->play();
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

OPCODE(0xce) {
	// start cutscene
	debugC(1, kDebugLevelScript, "opcode 0xce: start cutscene STUB");
}

OPCODE(0xcf) {
	// fade out
	debugC(1, kDebugLevelScript, "opcode 0xcf: fadeout");
	_graphics->fadeOut();
}

OPCODE(0xd0) {
	debugC(1, kDebugLevelScript, "opcode 0xd0: partial fadeout");
	Graf.fadeOut(Graphics::kPartialFade);
}

OPCODE(0xd1) {
	debugC(1, kDebugLevelScript, "opcode 0xd1: fadein next palette change");
	_graphics->willFadein();
}

OPCODE(0xd6) {
	// change room
	debugC(3, kDebugLevelScript, "opcode 0xd6: change room(%s)", +a[0]);
	if (a[0] == 81 && !_engine->_copyProtection) {
		debugC(3, kDebugLevelScript, "copy protection not active, going to room 65 instead");
//		_logic->changeRoom(65);
		_logic->changeRoom(84); // for debugging
	} else
		_logic->changeRoom(a[0]);
}

OPCODE(0xe5) {
	// hide all exits from the map
	debugC(1, kDebugLevelScript, "opcode 0xe5: hide exits from map STUB");
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

OPCODE(0xf7) {
	// stop music
	debugC(1, kDebugLevelScript, "opcode 0xf7: stop music STUB");
}

OPCODE(0xfc) {
	// quit
	debugC(3, kDebugLevelScript, "opcode 0xfc: quit%s", a[0] == 0 ? "" : " unconditionally");
	if (a[0] == 0)
		error("asking for quitting not implemented");

	_engine->quitGame();
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
