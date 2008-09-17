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

#include "innocent/animation.h"

#include "innocent/debug.h"
#include "innocent/graphics.h"
#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/logic.h"
#include "innocent/resources.h"
#include "innocent/util.h"

namespace Innocent {
//
ENAME(Animation::Status_, Animation::kOk, "ok");
ENAME(Animation::Status_, Animation::kRemove, "remove");
ENAME(Animation::Status_, Animation::kFrameDone, "frame done");

class Animation::Sprite {
public:
	Sprite(Innocent::Sprite *s) : _sprite(s), _isRelative(true) {}

	void setPosition(Common::Point p) {
		_position = p;
	}

	void setAbsolute() {
		_isRelative = false;
	}

	bool isAbsolute() const {
		return !_isRelative;
	}

	void paint(Graphics *g) const;

	const Innocent::Sprite *sprite() const {
		return _sprite.get();
	}
private:
	std::auto_ptr<Innocent::Sprite> _sprite;
	Common::Point _position;
	bool _isRelative;
};

template <int opcode>
Animation::Status Animation::opcodeHandler(){
	error("unhandled animation opcode %d [=0x%02x]", opcode, opcode);
}

template<int N>
void Animation::init_opcodes() {
	_handlers[N] = &Innocent::Animation::opcodeHandler<N>;
	init_opcodes<N-1>();
}

template<>
void Animation::init_opcodes<-1>() {}

Animation::Animation(const CodePointer &code, Common::Point position) :
	_position(position),
	_offset(0),
	_interval(1),
	_ticksLeft(0),
	_counter(0),
	_debugInvalid(false) {
	_base = code.code();
	_baseOffset = code.offset();
	_resources = code.interpreter()->resources();
	init_opcodes<37>();
	snprintf(_debugInfo, 50, "animation at %s", +code);
	code.interpreter()->rememberAnimation(this);
}

Animation::~Animation() {
	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete *it;
	Log.removeAnimation(this);
}

Animation::Status Animation::tick() {
	debugC(5, kDebugLevelAnimation, "ticking animation %s (ticks left: %d)", _debugInfo, _ticksLeft);

	if (_ticksLeft) {
		_ticksLeft--;
		return kOk;
	}

	clearSprites();

	Status status = kOk;
	while (status == kOk && _base) {
		int8 opcode = -*(_base + _offset);
		if (opcode < 0 || opcode >= 0x27) {
			error("invalid animation opcode 0x%02x while handling %s", *(_base + _offset), _debugInfo);
		}
		_offset += 2;

		_debugInvalid = false;
		status = op(opcode - 1);
	}

	if (status == kFrameDone && !_ticksLeft)
		_ticksLeft = _interval;

	if (status == kRemove)
		return status;

	return kOk;
}

void Animation::handleTrigger() {
	unless (_frameTrigger.isEmpty()) {
//		Graf.updateScreen();
		Log.runLater(_frameTrigger);
	}
	_frameTrigger.reset();
}

void Animation::runOnNextFrame(const CodePointer &cp) {
	_frameTrigger = cp;
}


void Animation::setMainSprite(uint16 sprite) {
	_mainSprite.reset(_resources->loadSprite(sprite));
}

void Animation::clearMainSprite() {
	_mainSprite.reset(0);
}

void Animation::clearSprites() {
	debugC(5, kDebugLevelAnimation, "clearing sprite list");
	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete (*it);
	_sprites.clear();
}

void Animation::paint(Graphics *g) {
	if (!_mainSprite.get())
		return;
	debugC(5, kDebugLevelAnimation | kDebugLevelGraphics, "painting sprites for animation %s", _debugInfo);

	g->paint(_mainSprite.get(), _position);

	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		(*it)->paint(g);
}

void Animation::Sprite::paint(Graphics *g) const {
	assert(isAbsolute());
	g->paint(sprite(), _position);
}

uint16 Animation::shift() {
	uint16 value = READ_LE_UINT16((_base + _offset));
	_offset += 2;
	return value;
}

int8 Animation::shiftByte() {
	byte value = *(_base + _offset);
	_offset += 1;
	return value;
}

int8 Animation::embeddedByte() const {
	return reinterpret_cast<int8 *>((_base + _offset))[-1];
}

Animation::Status Animation::op(byte opcode) {
	return (this->*_handlers[opcode])();
}

#define OPCODE(n) template<> Animation::Status Animation::opcodeHandler<n>()

OPCODE(0x00) {
	debugC(3, kDebugLevelAnimation, "anim opcode 0x00: remove animation");
	handleTrigger();

	return kRemove;
}

OPCODE(0x01) {
	debugC(3, kDebugLevelAnimation, "anim opcode 0x01: hide");
	_base = 0;
	_offset = _baseOffset = 0;

	return kFrameDone;
}

OPCODE(0x02) {
	uint16 left = shift();
	uint16 top = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x02: move to %d:%d", left, top);

	_position = Common::Point(left, top);

	return kOk;
}

OPCODE(0x03) {
	uint16 interval = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x03: set interval to %d", interval);

	_interval = interval;

	return kOk;
}

OPCODE(0x04) {
	uint16 offset = shift();
	uint16 interval = READ_LE_UINT16(_resources->getGlobalWordVariable(offset/2));

	debugC(3, kDebugLevelAnimation, "anim opcode 0x04: set interval to %d (from var %d)", interval, offset / 2);

	_interval = interval;

	return kOk;
}

OPCODE(0x05) {
	byte x = shiftByte();
	int8 xoff = *reinterpret_cast<int8 *>(&x);
	byte y = shiftByte();
	int8 yoff = *reinterpret_cast<int8 *>(&y);
	uint16 sprite = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x05: move by %d:%d, set main sprite to %d, frame done", xoff, yoff, sprite);

	_position.x += xoff;
	_position.y += yoff;
	setMainSprite(sprite);

	return kFrameDone;
}

OPCODE(0x06) {
	uint16 sprite = shift();

	setMainSprite(sprite);

	debugC(3, kDebugLevelAnimation, "anim opcode 0x06: set main sprite to %d, frame done", sprite);

	return kFrameDone;
}

OPCODE(0x07) {
	uint16 var = shift();
	uint16 sprite = READ_LE_UINT16(_resources->getGlobalWordVariable(var/2));

	setMainSprite(sprite);

	debugC(3, kDebugLevelAnimation, "anim opcode 0x07: set main sprite to %d (from global word 0x%04x), frame done", sprite, var/2);

	return kFrameDone;
}

OPCODE(0x08) {
	int8 left = shiftByte();
	int8 top = shiftByte();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x08: move by %d:%d", left, top);

	_position += Common::Point(left, top);

	return kOk;
}

OPCODE(0x0a) {
	uint16 left, top, sprite;
	left = shift();
	top = shift();
	sprite = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x0a: run sprite %d at %d:%d", sprite, left, top);

	_position = Common::Point(left, top);
	setMainSprite(sprite);

	return kFrameDone;
}

OPCODE(0x0d) {
	_counter = embeddedByte();
	_loopStart = _offset;

	debugC(3, kDebugLevelAnimation, "anim opcode 0x0d: %d times do", _counter);

	return kOk;
}

OPCODE(0x0e) {
	if (_counter)
		_counter--;

	if (_counter)
		_offset = _loopStart;

	debugC(3, kDebugLevelAnimation, "anim opcode 0x0e: done (%d times left)", _counter);

	return kOk;
}

OPCODE(0x0f) {
	uint16 offset = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x0f: jump to 0x%04x", offset);

	_offset = offset;

	return kOk;
}

OPCODE(0x10) {
	uint16 var = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x10: set bvar %d", var);
	*_resources->getGlobalByteVariable(var) = 1;

	return kOk;
}

OPCODE(0x11) {
	uint16 var = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x11: reset flag %d", var);
	*_resources->getGlobalByteVariable(var) = 0;

	return kOk;
}

OPCODE(0x12) {
	uint16 var = shift();
	uint16 off = shift();
	byte ok = *_resources->getGlobalByteVariable(var);

	debugC(3, kDebugLevelAnimation, "anim opcode 0x12: jump to 0x%x if byte var %d (%s)", off, var, ok ? "yes" : "not");

	if (ok)
		_offset = off;
	return kOk;
}

OPCODE(0x13) {
	uint16 max = shift();
	uint16 off = shift();
	uint16 res = Eng.getRandom(max);

	debugC(3, kDebugLevelAnimation, "anim opcode 0x13: jump to 0x%x 1 in %d times (%s)", off, max, res == max ? "do now" : "not now");

	if (res == max)
		_offset = off;
	return kOk;
}

OPCODE(0x19) {
	uint16 delay = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x19: hide for %d frames", delay);

	clearMainSprite();
	_ticksLeft = delay;

	return kFrameDone;
}

OPCODE(0x1a) {
	int8 index = embeddedByte();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x1a: set z index to %d", index);

	_zIndex = index;

	return kOk;
}

OPCODE(0x1b) {
	uint16 left = shift();
	uint16 top = shift();
	uint16 sprite = shift();

	debugC(3, kDebugLevelAnimation, "anim opcode 0x1b: add absolute sprite %d: %d:%d", sprite, left, top);

	Sprite *s = new Sprite(_resources->loadSprite(sprite));
	s->setPosition(Common::Point(left, top));
	s->setAbsolute();
	_sprites.push_back(s);

	return kOk;
}

}
