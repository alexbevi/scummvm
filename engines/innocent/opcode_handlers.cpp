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

#include "innocent/inter.h"

#include <vector>

#include "innocent/actor.h"
#include "innocent/animation.h"
#include "innocent/exit.h"
#include "innocent/graphics.h"
#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/movie.h"
#include "innocent/musicparser.h"
#include "innocent/room.h"
#include "innocent/util.h"

#include "common/events.h"
#include "common/util.h"

namespace Innocent {
#define OPCODE(num) template<> Interpreter::OpResult Interpreter::opcodeHandler<num>(ValueVector a, CodePointer current, CodePointer next)

OPCODE(0x00) {
	// nop
	debugC(2, kDebugLevelScript, "opcode 0x00: nop");
	return kThxBye;
}

OPCODE(0x01) {
	// exit
	// (some peculiarities in conj. with op 0x38, needs research TODO)
	debugC(2, kDebugLevelScript, "opcode 0x01: exit");
	return kReturn;
}

OPCODE(0x02) {
	// check equality
	debugC(2, kDebugLevelScript, "opcode 0x02: if %s == %s", +a[0], +a[1]);
	unless (a[0] == a[1])
		return kFail;
	return kThxBye;
}

OPCODE(0x03) {
	// check inequality
	debugC(2, kDebugLevelScript, "opcode 0x03: if %s != %s", +a[0], +a[1]);
	if (a[0] == a[1])
		return kFail;
	return kThxBye;
}

OPCODE(0x04) {
	// less than
	debugC(2, kDebugLevelScript, "opcode 0x04: if %s < %s", +a[0], +a[1]);
	unless (a[0] < a[1])
		return kFail;
	return kThxBye;
}

OPCODE(0x05) {
	// greater than
	debugC(2, kDebugLevelScript, "opcode 0x05: if %s > %s", +a[0], +a[1]);
	unless (a[0] > a[1])
		return kFail;
	return kThxBye;
}

OPCODE(0x0f) {
	// check room
	debugC(2, kDebugLevelScript, "opcode 0x0f: if current room == %s then", +a[0]);
	unless (a[0] == _logic->currentRoom())
		return kFail;
	return kThxBye;
}

enum {
	kSoundAdlib =  1,
	kSoundSB =     2,
	kSoundRoland = 4
};

OPCODE(0x12) {
 	// if sound is on then
	// (argument is a set of flags, 1 - adlib, 2 - sb, 4 - roland)
	debugC(2, kDebugLevelScript, "opcode 0x12: if sound is on then partial STUB");
	// just say roland+sb for now
	unless (a[0] & (kSoundRoland | kSoundSB))
		return kFail;
	return kThxBye;
}

OPCODE(0x13) {
	// if left button is up
	// and current mode isn't null
	debugC(1, kDebugLevelScript, "opcode 0x13: if 1st button is up and mode isn't null partial STUB");
	if (_engine->_eventMan->getButtonState() & Common::EventManager::LBUTTON)
		return kFail;
	return kThxBye;
}

OPCODE(0x1f) {
	// if actor in current room then whatever
	debugC(1, kDebugLevelScript, "opcode 0x1f: if actor %s is in current room but not at %s then", +a[1], +a[0]);

	Actor *ac = Log.getActor(a[1]);
	if (ac->room() == Log.currentRoom()) {
		if (ac->frameId() == a[0])
			return kFail;
	} else
		return kFail;
	return kThxBye;
}

OPCODE(0x24) {
	// check nonzeroness
	debugC(2, kDebugLevelScript, "opcode 0x24: if (%s)", +a[0]);
	if (a[0] == 0)
		return kFail;
	return kThxBye;
}

OPCODE(0x2c) {
	// else
	debugC(2, kDebugLevelScript, "opcode 0x2c: else");

	return kElse;
}

OPCODE(0x2d) {
	// end if
	debugC(2, kDebugLevelScript, "opcode 0x2d: end if");
	return kEndIf;
}

OPCODE(0x35) {
	// jump
	debugC(2, kDebugLevelScript, "opcode 0x35: jump to %s", +a[0]);
	return static_cast<CodePointer &>(a[0]);
}

OPCODE(0x36) {
	// call
	debugC(2, kDebugLevelScript, ">>>opcode 0x36: call procedure %s", +a[0]);
	CodePointer &p = static_cast<CodePointer &>(a[0]);
	p.run();
	debugC(2, kDebugLevelScript, "<<<opcode 0x36: called procedure %s", +a[0]);
	return kReturn;
}

OPCODE(0x39) {
	// run later
	debugC(2, kDebugLevelScript, "opcode 0x39: execute main %s later", +a[0]);
	_logic->runLater(CodePointer(static_cast<CodePointer &>(a[0]).offset(), _logic->mainInterpreter()));
	return kThxBye;
}

OPCODE(0x3b) {
	// run local code later
	debugC(2, kDebugLevelScript, "opcode 0x3b: execute %s later", +a[0]);
	_logic->runLater(CodePointer(static_cast<CodePointer &>(a[0]).offset(), _logic->blockInterpreter()));
	return kThxBye;
}

OPCODE(0x3d) {
	// save first arg -- instruction pointer -- for after skipping cutscene
	debugC(2, kDebugLevelScript, "opcode 0x3d: store position to continue if cutscene skipped to %s", +a[0]);
	Log.setSkipPoint(static_cast<CodePointer &>(a[0]));
	return kThxBye;
}

OPCODE(0x41) {
	// say (protagonist)
	debugC(2, kDebugLevelScript, "opcode 0x41: say %s", +a[0]);

	if (Log.protagonist()->isSpeaking()) {
		Log.protagonist()->callMeWhenSilent(current);
		return kReturn;
	}

	if (Log.protagonist()->isMoving()) {
		Log.protagonist()->callMeWhenStill(current);
		return kReturn;
	}

	Log.protagonist()->say(a[0]);
	return kThxBye;
}

OPCODE(0x43) {
	// say
	debugC(2, kDebugLevelScript, "opcode 0x43: %s says %s", +a[0], +a[1]);

	Actor *ac = Log.getActor(a[0]);
	if (ac->isSpeaking()) {
		ac->callMeWhenSilent(current);
		return kReturn;
	}

	if (ac->isMoving()) {
		ac->callMeWhenStill(current);
		return kReturn;
	}

	ac->say(a[1]);
	return kThxBye;
}

OPCODE(0x47) {
	// say (no actor)
	debugC(1, kDebugLevelScript, "opcode 0x47: say at [%s:%s] with colour %s in max %s lines text %s STUB", +a[0], +a[1], +a[2], +a[3], +a[4]);

	return kThxBye;
}

OPCODE(0x4a) {
	// wait until silent (protagonist)
	debugC(2, kDebugLevelScript, "opcode 0x4a: wait until protagonist is silent");

	Log.protagonist()->callMeWhenSilent(next);
	return kReturn;
}

OPCODE(0x4b) {
	// wait until silent
	debugC(2, kDebugLevelScript, "opcode 0x4b: wait %s is silent", +a[0]);

	Log.getActor(a[0])->callMeWhenSilent(next);
	return kReturn;
}

OPCODE(0x54) {
	debugC(2, kDebugLevelScript, "opcode 0x54: ask about '%s' at %s:%s %sx%s", +a[4], +a[0], +a[1], +a[2], +a[3]);

	uint16 result;
	unless ((result = _graphics->ask(a[0], a[1], a[2], a[3], a[4])) == 0xffff)
		return CodePointer(result, this);
	return kThxBye;
}

OPCODE(0x55) {
	// paint text
	// args: left, top, colour, text
	debugC(2, kDebugLevelScript, "opcode 0x55: paint '%s' with colour %s at %s:%s", +a[3], +a[2], +a[0], +a[1]);
	_graphics->paintText(a[0], a[1], a[2], a[3]);
	return kThxBye;
}

OPCODE(0x56) {
	// say text
	debugC(2, kDebugLevelScript, "opcode 0x86: say %s for %s frames", +a[1], +a[0]);
	Graf.say(a[1], a[1], a[0]);
	return kThxBye;
}

OPCODE(0x57) {
	// wait until said
	debugC(2, kDebugLevelScript, "opcode 0x57: wait until text is said");
	Graf.runWhenSaid(next);
	return kReturn;
}

OPCODE(0x60) {
	// lookup locally
	// takes a list (1st)
	// a value (2nd)
	// a field (as offset from structure start) (2rd)
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

	debugC(2, kDebugLevelScript, "opcode 0x60: %s = %d == search list %s for %s and return field %s", +a[3], value, +a[0], +a[1], +a[2]);
	a[3] = value;
	return kThxBye;
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

	debugC(2, kDebugLevelScript, "opcode 0x63: %s = get %s of actor %s", +a[2], desc, +a[0]);
	return kThxBye;
}

OPCODE(0x6d) {
	// increment
	debugC(2, kDebugLevelScript, "opcode 0x6d: %s++", +a[0]);
	a[0]++;
	return kThxBye;
}

OPCODE(0x6f) {
	// decrement
	debugC(2, kDebugLevelScript, "opcode 0x6d: %s--", +a[0]);
	a[0]--;
	return kThxBye;
}

OPCODE(0x70) {
	// assign
	debugC(2, kDebugLevelScript, "opcode 0x70: %s = %s", +a[0], +a[1]);
	a[0] = a[1];
	return kThxBye;
}

OPCODE(0x72) {
	// assign 1
	debugC(2, kDebugLevelScript, "opcode 0x72: %s = 1", +a[0]);
	a[0] = 1;
	return kThxBye;
}

OPCODE(0x73) {
	// assign 0
	debugC(2, kDebugLevelScript, "opcode 0x73: %s = 0", +a[0]);
	a[0] = 0;
	return kThxBye;
}

OPCODE(0x77) {
	// initialize protagonist
	debugC(2, kDebugLevelScript, "opcode 0x77: go to room %s facing %s", +a[0], +a[1]);
	_logic->changeRoom(a[0]);
	_logic->protagonist()->setRoom(a[0], a[1]);
	return kThxBye;
}

OPCODE(0x79) {
	// move actor to another room
	debugC(1, kDebugLevelScript, "opcode 0x79: move actor %s to room %s (and set current animation frame to %s STUB)", +a[0], +a[1], +a[2]);
	_logic->getActor(a[0])->setRoom(a[1]);
	return kThxBye;
}

OPCODE(0x7c) {
	// toggle exit state
	Exit *exit = _logic->blockProgram()->getExit(a[0]);
	debugC(2, kDebugLevelScript, "opcode 0x7c: toggling exit %s to %s", +a[0], exit->isEnabled() ? "disabled" : "enabled");
	exit->setEnabled(!exit->isEnabled());
	return kThxBye;
}

OPCODE(0x95) {
	// unlock control
	debugC(1, kDebugLevelScript, "opcode 0x95: unlock control STUB");
	return kThxBye;
}

OPCODE(0x96) {
	// disallow user interaction
	debugC(1, kDebugLevelScript, "opcode 0x96: lock control STUB");
	return kThxBye;
}

OPCODE(0x99) {
	// wait for protagonist to exit
	debugC(2, kDebugLevelScript, "opcoe 0x99: wait for protagonist to exit");

	Actor *ac = _logic->protagonist();
	if (ac->room() != _logic->currentRoom())
		return kThxBye;

	ac->callMe(next);
	return kReturn;
}

OPCODE(0x9a) {
	// wait for actor to exit
	debugC(2, kDebugLevelScript, "opcode 0x9a: wait for actor %s to exit", +a[0]);

	Actor *ac = _logic->getActor(a[0]);
	if (ac->room() != _logic->currentRoom())
		return kThxBye;

	ac->callMe(next);
	return kReturn;
}

OPCODE(0x9b) {
	// delay
	debugC(2, kDebugLevelScript, "opcode 0x9b: delay %s frames", +a[0]);
	_logic->runLater(next, a[0]);
	return kReturn;
}

OPCODE(0x9c) {
	// wait until another room
	debugC(2, kDebugLevelScript, "opcode 0x9a: wait until actor %s enters or %s ticks", +a[0], +a[1]);

	Actor *ac = _logic->getActor(a[0]);
	if (ac->room() != _logic->currentRoom()) {
		ac->tellMe(next, a[1]);
		return kReturn;
	}
	return kThxBye;
}

OPCODE(0x9d) {
	// set protagonist
	debugC(2, kDebugLevelScript, "opcode 0x9d: set protagonist(%s)", +a[0]);
	_logic->setProtagonist(a[0]);
	return kThxBye;
}

OPCODE(0x9e) {
	// warp protagonist to frame
	debugC(2, kDebugLevelScript, "opcode 0x9e: warp protagonist to frame %s", +a[0]);

//	Log.protagonist()->warpTo(a[0]);
	Log.protagonist()->setFrame(a[0]);
	return kThxBye;
}

OPCODE(0xab) {
	// set protagonist frame
	debugC(2, kDebugLevelScript, "opcode 0xab: set protagonist frame to %s", +a[0]);

	Actor *ac = Log.protagonist();
	if (ac->isFine()) {
		ac->callMe(current);
		return kReturn;
	}

	ac->moveTo(a[0]);
	return kThxBye;
}

OPCODE(0xad) {
	// turn actor
	debugC(2, kDebugLevelScript, "opcode 0xad: move actor %s to frame %s next", +a[0], +a[1]);

	Actor *ac = _logic->getActor(a[0]);
	if (ac->isFine()) {
		ac->callMe(current);
		return kReturn;
	}

	// TODO: special handling for protagonist
	ac->setFrame(a[1]);

	return kThxBye;
}

OPCODE(0xb9) {
	// set local animation
	debugC(2, kDebugLevelScript, "opcode 0xb9: set actor %s animation to %s", +a[0], +a[1]);

	Actor *ac = Log.getActor(a[0]);
	if (ac->isFine()) {
		ac->callMe(current);
		return kReturn;
	}

	ac->setAnimation(static_cast<CodePointer &>(a[1]));
	return kThxBye;
}

OPCODE(0xbc) {
	// hide actor
	debugC(2, kDebugLevelScript, "opcode 0xbc: hide actor %s", +a[0]);
	_logic->getActor(a[0])->hide();
	return kThxBye;
}

OPCODE(0xbd) {
	// set protagonist animation
	CodePointer p(static_cast<CodePointer &>(a[0]).offset(), Log.mainInterpreter());
	debugC(2, kDebugLevelScript, "opcode 0xbd: set protagonist animation to %s", +p);

	Actor *ac = Log.protagonist();
	if (ac->isFine()) {
		ac->callMe(current);
		return kReturn;
	}

	ac->setAnimation(p);
	return kThxBye;
}

OPCODE(0xbe) {
	// set protagonist animation
	debugC(2, kDebugLevelScript, "opcode 0xbe: set protagonist animation to %s", +a[0]);

	Actor *ac = Log.protagonist();
	if (ac->isFine()) {
		ac->callMe(current);
		return kReturn;
	}

	ac->setAnimation(static_cast<CodePointer &>(a[0]));
	return kThxBye;
}

OPCODE(0xc2) {
	// add animation at cursor
	debugC(2, kDebugLevelScript, "opcode 0xc2: add animation %s at cursor partial STUB", +a[0]);
	_logic->addAnimation(new Animation(static_cast<CodePointer &>(a[0]), _graphics->cursorPosition()));
	return kThxBye;
}

OPCODE(0xc6) {
	// suspend execution until an animation's ip points to 0xff
	debugC(2, kDebugLevelScript, "opcode 0xc6: wait on animation %s", +a[0]);
	_logic->animation(a[0])->runOnNextFrame(next);
	return kReturn;
}

OPCODE(0xc7) {
	// play movie
	debugC(2, kDebugLevelScript, "opcode 0xc7: play movie %s with slowness %s", +a[0], +a[1]);
	Movie *m = Movie::fromFile(reinterpret_cast<char *>((byte *)(a[0])));
	m->setFrameDelay(a[1]);
	if (m->play())
		return kThxBye;
	else
		return kReturn;
}

OPCODE(0xc8) {
	// set backdrop
	// (not sure what's the difference to c9)
	debugC(2, kDebugLevelScript, "opcode 0xc8: set backdrop(%s)", +a[0]);
	_graphics->setBackdrop(a[0]);
	return kThxBye;
}

OPCODE(0xc9) {
	// set backdrop
	// does it set the default one?
	debugC(2, kDebugLevelScript, "opcode 0xc9: set backdrop(%s)", +a[0]);
	_graphics->setBackdrop(a[0]);
	return kThxBye;
}

OPCODE(0xcb) {
	// load graphic
	debugC(1, kDebugLevelScript, "opcode 0xcb: load graphic %s STUB", +a[0]);
	return kThxBye;
}

OPCODE(0xcc) {
	// go fullscreen
	debugC(1, kDebugLevelScript, "opcode 0xcc: go fullscreen");
	Graf.goFullscreen();
	return kThxBye;
}

OPCODE(0xce) {
	// start cutscene
	debugC(2, kDebugLevelScript, "opcode 0xce: start cutscene partial STUB");
	Graf.hideCursor();
	// hide objects
	// set game area height to 200
	return kThxBye;
}

OPCODE(0xcf) {
	// fade out
	debugC(1, kDebugLevelScript, "opcode 0xcf: fadeout");
	if (_graphics->fadeOut())
		return kThxBye;
	else
		return kReturn;
}

OPCODE(0xd0) {
	debugC(1, kDebugLevelScript, "opcode 0xd0: partial fadeout");
	if (Graf.fadeOut(Graphics::kPartialFade))
		return kThxBye;
	else
		return kReturn;
}

OPCODE(0xd1) {
	debugC(2, kDebugLevelScript, "opcode 0xd1: fadein next paint");
	_graphics->willFadein();
	return kThxBye;
}

OPCODE(0xd2) {
	debugC(2, kDebugLevelScript, "opcode 0xd2: will fadein partially");
	Graf.willFadein(Graphics::kPartialFade);
	return kThxBye;
}

enum {
	kCopyProtectionRoom = 81,
	kIntroOffset	    = 0x33a3
};

OPCODE(0xd6) {
	// change room
	debugC(1, kDebugLevelScript, "opcode 0xd6: change room(%s)", +a[0]);
	uint16 room = a[0];
	if (room == kCopyProtectionRoom) {
		if (_engine->_startRoom)
			room = Eng._startRoom;
	}
	_logic->changeRoom(room);
	if (room == kCopyProtectionRoom && !Eng._copyProtection) {
		Log.runLater(CodePointer(kIntroOffset, Log.blockInterpreter()));
		return kReturn;
	}
	return kThxBye;
}

OPCODE(0xdb) {
	// add active rect
	debugC(1, kDebugLevelScript, "opcode 0xdb: add rect %s:%s-%s:%s::%s", +a[0], +a[1], +a[2], +a[3], +a[4]);
	Log.room()->addRect(Room::Rect(a[4].signd(), Common::Rect(a[0].signd(), a[1].signd(), a[2].signd(), a[3].signd())));
	return kThxBye;
}

OPCODE(0xdf) {
	// add actor frame
	std::vector<byte> nexts(8);
	for (int i = 0; i < 4; i++) {
		uint16 val = a[i+2];
		nexts[2*i] = val & 0xff;
		nexts[2*i+1] = val >> 8;
	}
	const int16 left = a[0].signd();
	const int16 top = a[1].signd();
	debugC(2, kDebugLevelScript, "opcode 0xdf: add actor frame %d %d %d %d %d %d %d %d %d %d", left, top, nexts[0], nexts[1], nexts[2], nexts[3], nexts[4], nexts[5], nexts[6], nexts[7]);
	Log.room()->addActorFrame(Common::Point(left, top), nexts);
	return kThxBye;
}

OPCODE(0xe5) {
	// hide all exits from the map
	debugC(1, kDebugLevelScript, "opcode 0xe5: hide exits from map STUB");
	return kThxBye;
}

OPCODE(0xe6) {
	// set room loop code
	debugC(2, kDebugLevelScript, "opcode 0xe6: set room loop to %s", +a[0]);
	assert(a[0].holdsCode());
	_logic->setRoomLoop(static_cast<CodePointer &>(a[0]));
	return kThxBye;
}

OPCODE(0xef) {
	// random
	uint16 value = _engine->getRandom(a[0]);
	debugC(2, kDebugLevelScript, "opcode 0xef: %s = %d == random(%s)", +a[1], value, +a[0]);
	a[1] = value;
	return kThxBye;
}

OPCODE(0xf0) {
	// load sfx set
	debugC(1, kDebugLevelScript, "opcode 0xf0: load sfx set %s STUB", +a[0]);
	return kThxBye;
}

OPCODE(0xf4) {
	// play music
	debugC(1, kDebugLevelScript, "opcode 0xf4: play music script at %s", +a[0]);
	Music.loadMusic(static_cast<CodePointer &>(a[0]).offset() + Log.mainInterpreter()->_base);
	return kThxBye;
}

OPCODE(0xf7) {
	// stop music
	debugC(2, kDebugLevelScript, "opcode 0xf7: stop music");
	Music.unloadMusic();
	Music.silence();
	return kThxBye;
}

OPCODE(0xf9) {
	// set sound on
	
	debugC(1, kDebugLevelScript, "opcode 0xf9: set %s to %s STUB", a[0] == 1 ? "music" : "sfx", +a[1]);
	return kThxBye;
}

OPCODE(0xfc) {
	// quit
	debugC(2, kDebugLevelScript, "opcode 0xfc: quit%s", a[0] == 0 ? "" : " unconditionally");
	if (a[0] == 0)
		error("asking for quitting not implemented");

	_engine->quitGame();
	return kThxBye;
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
