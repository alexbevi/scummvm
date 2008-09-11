#include "innocent/actor.h"

#include "common/rect.h"

#include "innocent/innocent.h"
#include "innocent/logic.h"

namespace Innocent {
//

Actor::Actor(const CodePointer &code) : Animation(code, Common::Point()) {
	byte *header = code.code();
	_base = header - code.offset();
	snprintf(_debugInfo, 50, "actor at %s", +code);
	readHeader(header);

	Engine::instance().logic()->addAnimation(this);
}

void Actor::setAnimation(const CodePointer &anim) {
	debugC(3, kDebugLevelScript, "setting animation code of %s to %s", _debugInfo, +anim);
	_base = anim.code();
	_baseOffset = anim.offset();
	_offset = 0;
	_debugInvalid = false;
}

void Actor::setRoom(uint16 r) {
	_room = r;
}

bool Actor::isVisible() const {
	return _room == Log.currentRoom();
}

Animation::Status Actor::tick() {
	if (isVisible())
		return Animation::tick();
	else
		return kOk;
}

void Actor::readHeader(const byte *code) {
	_interval = code[kOffsetInterval];
	_ticksLeft = READ_LE_UINT16(code + kOffsetTicksLeft);
	_zIndex = 0;
	_position = Common::Point(READ_LE_UINT16(code + kOffsetLeft), READ_LE_UINT16(code + kOffsetTop));
	uint16 baseOff = READ_LE_UINT16(code + kOffsetCode);
	_base += baseOff;
	_offset = READ_LE_UINT16(code + kOffsetOffset);
	uint16 sprite = READ_LE_UINT16(code + kOffsetMainSprite);
	_room = READ_LE_UINT16(code + kOffsetRoom);

	debugC(3, kDebugLevelFiles, "loading %s: interv %d ticks %d z%d pos%d:%d code %d offset %d sprite %d room %d", _debugInfo, _interval, _ticksLeft, _zIndex, _position.x, _position.y, baseOff, _offset, sprite, _room);

	if (sprite != 0xffff)
		setMainSprite(sprite);
}

}
