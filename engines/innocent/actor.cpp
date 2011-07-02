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

#include "innocent/actor.h"

#include "common/rect.h"

#include "innocent/graphics.h"
#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/logic.h"
#include "innocent/resources.h"
#include "innocent/room.h"
#include "innocent/util.h"

namespace Innocent {
//

Actor::Actor(const CodePointer &code) : Animation(code, Common::Point()) {
	byte *header = code.code();
	_base = header - code.offset();
	readHeader(header);
	snprintf(_debugInfo, 50, "actor at %s", +code);
	_direction = kDirNone;
	_frame = 0;
	_debug = false;
	_attentionNeeded = false;
	_nextDirection = kDirNone;

	Engine::instance().logic()->addAnimation(this);

	init_opcodes<37>();
}

bool Actor::isFine() const {
	return _room == Log.currentRoom() && _base && !_attentionNeeded &&
			(!_framequeue.empty() || !_confused || _nextDirection);
}

void Actor::setAnimation(uint16 offset) {
	_base = _base - _baseOffset + offset;
	_baseOffset = offset;
	_offset = 0;
	_debugInvalid = false;
	_confused = _attentionNeeded = false;
	clearMainSprite();
	_interval = 1;
	_counter = _ticksLeft = 0;
	_nextDirection = kDirNone;
}

void Actor::setAnimation(const CodePointer &anim) {
	debugC(3, kDebugLevelScript, "setting animation code of %s to %s", _debugInfo, +anim);
	_base = anim.code();
	_baseOffset = anim.offset();
	_offset = 0;
	_debugInvalid = false;
	_confused = _attentionNeeded = false;
	clearMainSprite();
	_interval = 1;
	_counter = _ticksLeft = 0;
	_nextDirection = kDirNone;
}

void Actor::hide() {
	_base = 0;
	_baseOffset = _offset = 0;
}

void Actor::callMe(const CodePointer &code) {
	debugC(3, kDebugLevelScript, "actor will call %s when needed", +code);
	_callBacks.push(code);
}

void Actor::tellMe(const CodePointer &code, uint16 timeout) {
	_roomCallbacks.push_back(RoomCallback(timeout, code));
}

bool Actor::isSpeaking() const {
	return _speech.active();
}

void Actor::callMeWhenSilent(const CodePointer &cp) {
	_speech.callWhenDone(cp);
}

void Actor::say(const Common::String &text) {
	_speech = Speech(this, text);
}

Actor::Speech::~Speech() { while (!_cb.empty()) Log.runLater(_cb.pop()); }

bool Actor::isMoving() const {
	//TODO stub
	return false;
}

void Actor::callMeWhenStill(const CodePointer &cp) {
	assert(false);
}

void Actor::setFrame(uint16 frame) {
	_frame = frame;
	Frame f(Log.room()->getFrame(frame));
	if (f.position().x == 999)
		return;
	_position = f.position();
	debugC(5, kDebugLevelActor, "actor set to frame %d, position %d:%d", f.index(), _position.x, _position.y);
}

Common::List<Actor::Frame> Actor::findPath(Actor::Frame from, uint16 to) {
	Common::List<Common::List<Frame> > reachable;

	Common::List<Frame> zero;
	zero.push_back(from);
	reachable.push_back(zero);

	bool found = false;
	while (!found) {
		Common::List<Common::List<Frame> >::iterator back = reachable.end();
		back--;
		Common::List<Frame>::iterator current = back->begin();
		Common::List<Frame> next;
		Common::String s;
		while (!found && current != back->end()) {
			std::vector<byte> nexts = current->nexts();
			for (int i = 0; i < 8; i++)
				if (nexts[i]) {
					s += Common::String::format(", %d", int(nexts[i]));
					next.push_back(Log.room()->getFrame(nexts[i]));
					if (nexts[i] == to) {
						found = true;
						break;
					}
				}
			current++;
		}
		reachable.push_back(next);
		debugC(4, kDebugLevelActor, "reachable on this level:%s", s.c_str());
	}

	Common::List<Frame> path;

	Common::List<Common::List<Frame> >::iterator level = reachable.end();
	level--;
	Common::List<Frame>::iterator current = level->end();
	current--;
	uint16 index = level->size() - 1;

	forever {
		path.push_front(*current);
		if (*current == from)
			break;
		level--;
		current = level->begin();
		uint16 new_index = 0;
		while (index >= current->nextCount()) {
			index -= current->nextCount();
			new_index++;
			current++;
		}
		index = new_index;
	}

	return path;
}

void Actor::moveTo(uint16 frame) {
	Frame cur = Log.room()->getFrame(_frame);

	Common::List<Frame> path = findPath(cur, frame);

	Common::List<Frame>::iterator it = path.end();
	it--;
	if (it->index() != frame) {
		Common::List<Frame> p;
		p.push_back(Log.room()->getFrame(frame));
		path = p;
	}

	Common::String s;
	it = path.begin();
	it++;
	while (it != path.end()) {
		_framequeue.push(*it);
		s += Common::String::format(" %d", int(it->index()));
		it++;
	}

	debugC(3, kDebugLevelActor, "found path: %s", s.c_str());
	if (!_base)
		nextFrame();
}

void Actor::setRoom(uint16 r, uint16 frame, uint16 next_frame) {
	_room = r;
	unless (next_frame)
		next_frame = frame;
	_nextFrame = next_frame;
	setFrame(frame);

	setAnimation(CodePointer(_puppeteer.mainCodeOffset(), Log.mainInterpreter()));
}

bool Actor::nextFrame() {
	if (_framequeue.empty())
		return false;

	Frame next = _framequeue.front();
	Frame current = Log.room()->getFrame(_frame);

	Direction direction = next - current;

	debugC(4, kDebugLevelActor, "switching frames %d -> %d", current.index(), next.index());
	if (turnTo(direction))
		return true;

//	setFrame(_framequeue.front().index());
	setAnimation(_puppeteer.moveAnimator(direction));
	_frame = next.index();
	_framequeue.pop();
	return true;
}

bool Actor::turnTo(Direction dir) {
	if (dir == _direction)
		return false;

	Direction d = _direction>>dir;
	debugC(4, kDebugLevelActor, "turning %d -> %d >> %d", _direction, d, dir);
	setAnimation(_puppeteer.turnAnimator(d));
	return true;
}

void Actor::animate() {
	unless (_puppeteer.valid())
		return;

	unless (_attentionNeeded || _confused/* || _timedOut*/)
		return;

	debugC(4, kDebugLevelActor, "attention needed");

	if (nextFrame()) {
		return;
	}

	if (_nextAnimator) {
		setAnimation(_nextAnimator);
		_nextAnimator = 0;
		return;
	}

	if (_nextDirection) {
		if (turnTo(_nextDirection))
			return;
		_direction = _nextDirection;
		_nextDirection = kDirNone;
/*		ax = _nextPuppeteer;
		_nextPuppeteer = 0;
		if (ax)
			goto set_anim;*/
		setAnimation(_puppeteer.offset());
/*	} else if (_nextPuppeteer) {
		ax = _nextPuppeteer;
		_nextPuppeteer = 0;
		if (ax)
			goto set_anim;*/
	}

	if (_confused) {
		if (turnTo(kDirDown))
			return;
		_direction = kDirDown;
	}
/*	} else {
		if (turnTo(kDirUp))
			return;
		_direction = kDirUp;
		setAnimation(_puppeteer.offset());
	}*/
}

Animation::Status Actor::tick() {
	_speech.tick();
	animate();
	callBacks();

	if (_room == Log.currentRoom()) {
		Animation::Status s;
		if (_debug) gDebugLevel += 3;
			s = Animation::tick();
		if (_debug) gDebugLevel -= 3;
		return s;
	} else
		return kOk;
}

void Actor::toggleDebug() {
	_debug = !_debug;
}

void Actor::readHeader(const byte *code) {
//	uint16 segment = READ_LE_UINT16(code + kOffsetCode);
/*	if (segment == 0)
		_base = Log.mainInterpreter()->rawCode(0);
	else if (segment == 1)
		_base = Log.blockInterpreter()->rawCode(0);
	else error("segment %x", segment);*/
	_interval = code[kOffsetInterval];
	_ticksLeft = READ_LE_UINT16(code + kOffsetTicksLeft);
	_zIndex = 0;
	_position = Common::Point(READ_LE_UINT16(code + kOffsetLeft), READ_LE_UINT16(code + kOffsetTop));
	uint16 baseOff = READ_LE_UINT16(code + kOffsetCode);
	_offset = READ_LE_UINT16(code + kOffsetOffset);
	if (_offset || baseOff) {
		_base += baseOff;
		_baseOffset = baseOff;
	} else {
		_base = 0;
		_baseOffset = _offset = 0;
	}
	uint16 sprite = READ_LE_UINT16(code + kOffsetMainSprite);
	_room = READ_LE_UINT16(code + kOffsetRoom);

	debugC(3, kDebugLevelFiles, "loading %s: interv %d ticks %d z%d pos%d:%d code %d offset %d sprite %d room %d", _debugInfo, _interval, _ticksLeft, _zIndex, _position.x, _position.y, baseOff, _offset, sprite, _room);

	if (sprite != 0xffff)
		setMainSprite(sprite);
}

void Actor::callBacks() {
	unless (isFine()) {
		Common::Queue<CodePointer> cb = _callBacks;
		_callBacks.clear();
		while (!cb.empty())
			cb.pop().run();
	}

	foreach (RoomCallback, _roomCallbacks) {
		if (_room == Log.currentRoom() || !it->timeout) {
			it->callback.run();
			Common::List<RoomCallback>::iterator done = it;
			it++;
			_roomCallbacks.erase(done);
		} else
			it->timeout--;
	}
}

void Puppeteer::parse(const byte *data) {
	_actorId = READ_LE_UINT16(data + kActorId);
	_offset = READ_LE_UINT16(data + kMainCode);

	assert (kTurnAnimators == kMoveAnimators + 16);
	const byte *d = data + kMoveAnimators;
	for (int i = 0; i < 16; i++) {
		_animators[i] = READ_LE_UINT16(d);
		d += 2;
	}
}

static const Direction mixeddirs[] = {kDirUp, kDirRight, kDirDown, kDirLeft,
									kDirUpRight, kDirDownRight, kDirDownLeft, kDirUpLeft};

CodePointer Puppeteer::moveAnimator(Direction d) {
	uint16 off = mainCodeOffset();
	for (int i = 0; i < 8; i++) {
		if (mixeddirs[i] == d)
			off = _animators[i];
	}

	return CodePointer(off, Log.mainInterpreter());
}

CodePointer Puppeteer::turnAnimator(Direction d) {
	uint16 off = mainCodeOffset();
	for (int i = 0; i < 8; i++) {
		if (mixeddirs[i] == d)
			off = _animators[i + 8];
	}

	return CodePointer(off, Log.mainInterpreter());
}

Actor::Speech::Speech(Actor *parent, const Common::String &text) : _text(text), _ticksLeft(20), _actor(parent) {
	const Common::Point &position = parent->getSpeechPosition();
	debugC(1, kDebugLevelActor, "adding speech \"%s\" for %s at %d:%d", text.c_str(), parent->_debugInfo, position.x, position.y);
	_image = new Innocent::Sprite;
	_rect = Graf.paintSpeechInBubble(position, 235, reinterpret_cast<const byte *>(text.c_str()), _image);
}

void Actor::Speech::tick() {
	unless (_ticksLeft--) {
		_text.clear();
		while (!_cb.empty())
			Log.runLater(_cb.pop());
	}
}

Common::Point Actor::getSpeechPosition() const {
	Common::Point speechPosition(_position);
	if (_mainSprite.get()) {
		speechPosition.y -= _mainSprite.get()->h - _mainSprite.get()->_hotPoint.y;
		speechPosition.x -= _mainSprite.get()->_hotPoint.x;
	}
	return speechPosition;
}

void Actor::paint(Graphics *g) {
	Animation::paint(g);
	_speech.paint(g);
}

void Actor::Speech::paint(Graphics *g) {
	if (_text.empty())
		return;

	g->paint(_image, Common::Point(_rect.left, _rect.top), Graphics::kPaintSemiTransparent | Graphics::kPaintPositionIsTop);
}

Direction Actor::Frame::operator-(const Actor::Frame &other) const {
	for (int i = 0; i < 8; i++)
		if (other._nexts[i] == _index)
			return mixeddirs[i];

	return kDirNone;
}

Direction operator>>(Direction _a, Direction _b) {
	assert(sizeof(Direction) == sizeof(int32));
	int32 a(_a), b(_b);

	b -= a;

	if (b < -3)
		b += 8;
	if (b > 4)
		b -= 8;

	if (b > 0)
		a++;
	if (b < 0)
		a--;

	if (a < 1)
		a += 8;
	if (a > 8)
		a -= 8;
	return *reinterpret_cast<Direction *>(&a);
}

template <int opcode>
Animation::Status Actor::opcodeHandler(){
	return Animation::opcodeHandler<opcode>();
}

template<int N>
void Actor::init_opcodes() {
	_handlers[N] = &Innocent::Actor::opcodeHandler<N>;
	init_opcodes<N-1>();
}

template<>
void Actor::init_opcodes<-1>() {}

Animation::Status Actor::op(byte opcode) {
	return (this->*_handlers[opcode])();
}

#define OPCODE(n) template<> Animation::Status Actor::opcodeHandler<n>()

OPCODE(0x01) {
	debugC(1, kDebugLevelActor, "actor opcode 0x01: I don't know what to do");

	_offset = 0;
	_base = 0;
	_confused = true;
	return kFrameDone;
}

OPCODE(0x14) {
	uint16 off = shift();

	debugC(1, kDebugLevelAnimation, "actor opcode 0x14: jump to 0x%04x if I'm speaking STUB", off);

	// also, some check for non-protagonists

	return kOk;
}

OPCODE(0x15) {
	debugC(1, kDebugLevelAnimation, "actor opcode 0x15: look at cursor direction if its mode is 'See' STUB");

	return kOk;
}

OPCODE(0x16) {
	byte val = embeddedByte();
	uint16 off = shift();

	debugC(1, kDebugLevelAnimation, "actor opcode 0x16: if look direction is %d then jump to 0x%04x STUB", val, off);

	return kOk;
}

OPCODE(0x17) {
	byte val = embeddedByte();
	uint16 off = shift();

	debugC(3, kDebugLevelAnimation, "actor opcode 0x17: if facing (currently %d) is %d then change code to 0x%04x", _direction, val, off);

	if (val == _direction) {
		setAnimation(off);
	}

	return kOk;
}

OPCODE(0x18) {
	uint16 val = shift();

	debugC(3, kDebugLevelAnimation, "actor opcode 0x18: set next animator to %x", val);
	_nextAnimator = val;

	return kOk;
}

OPCODE(0x23) {
	byte dir = embeddedByte();

	debugC(3, kDebugLevelAnimation, "actor opcode 0x23: face %d", dir);

	switch (dir) {
	case 1:
		_direction = kDirUp;
		break;
	case 2:
		_direction = kDirUpRight;
		break;
	case 3:
		_direction = kDirRight;
		break;
	case 4:
		_direction = kDirDownRight;
		break;
	case 5:
		_direction = kDirDown;
		break;
	case 6:
		_direction = kDirDownLeft;
		break;
	case 7:
		_direction = kDirLeft;
		break;
	case 8:
		_direction = kDirUpLeft;
		break;
	default:
		assert(false);
	}

	return kOk;
}

OPCODE(0x24) {
	byte dir = embeddedByte();

	debugC(3, kDebugLevelAnimation, "actor opcode 0x24: set attention needed flag to %d", dir);
	_attentionNeeded = true;

	return kOk;
}

} // end of namespace
