#include "innocent/animation.h"
#include "innocent/debug.h"
#include "innocent/graphics.h"
#include "innocent/inter.h"
#include "innocent/resources.h"

namespace Innocent {
//

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
		_position(position), _offset(0), _interval(1), _ticksLeft(0)
{
	_base = code.code();
	_resources = code.interpreter()->resources();
	init_opcodes<37>();
	snprintf(_debugInfo, 50, "animation at %s", +code);
}

Animation::~Animation() {
	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete *it;
}

Animation::Status Animation::tick() {
	debugC(3, kDebugLevelAnimation, "ticking animation %s (ticks left: %d)", _debugInfo, _ticksLeft);

	if (_ticksLeft) {
		_ticksLeft--;
		return kOk;
	}

	clearSprites();

	Status status = kOk;
	while (status == kOk) {
		int8 opcode = -*(_base + _offset);
		if (opcode < 0 || opcode >= 0x27)
			error("invalid animation opcode 0x%02x while handling %s", *(_base + _offset), _debugInfo);
		_offset += 2;

		status = (this->*_handlers[opcode-1])();
	}

	if (status == kRemove)
		return status;

	return kOk;
}

void Animation::clearSprites() {
	debugC(5, kDebugLevelAnimation, "clearing sprite list");
	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete (*it);
	_sprites.clear();
}

void Animation::setMainSprite(uint16 sprite) {
	_mainSprite.reset(_resources->loadSprite(sprite));
}

void Animation::paint(Graphics *g) {
	debugC(4, kDebugLevelAnimation | kDebugLevelGraphics, "painting sprites for animation %s", _debugInfo);
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

#define OPCODE(n) template<> Animation::Status Animation::opcodeHandler<n>()

OPCODE(0x00) {
	debugC(4, kDebugLevelAnimation, "anim opcode 0x00: remove animation");
	return kRemove;
}

OPCODE(0x02) {
	uint16 left = shift();
	uint16 top = shift();

	debugC(4, kDebugLevelAnimation, "anim opcode 0x02: move to %d:%d", left, top);

	_position = Common::Point(left, top);

	return kOk;
}

OPCODE(0x07) {
	uint16 var = shift();
	uint16 sprite = READ_LE_UINT16(_resources->getGlobalWordVariable(var/2));

	setMainSprite(sprite);

	debugC(4, kDebugLevelAnimation, "anim opcode 0x07: set main sprite to %d (from global word 0x%04x), frame done", sprite, var/2);

	_ticksLeft = _interval;

	return kFrameDone;
}

OPCODE(0x08) {
	int8 left = shiftByte();
	int8 top = shiftByte();

	debugC(4, kDebugLevelAnimation, "anim opcode 0x08: move by %d:%d", left, top);

	_position += Common::Point(left, top);

	return kOk;
}

OPCODE(0x0f) {
	uint16 offset = shift();

	debugC(4, kDebugLevelAnimation, "anim opcode 0x0f: jump to 0x%04x", offset);

	_offset = offset;

	return kOk;
}

OPCODE(0x1a) {
	int8 index = embeddedByte();

	debugC(4, kDebugLevelAnimation, "anim opcode 0x1a: set z index to %d", index);

	_zIndex = index;

	return kOk;
}

OPCODE(0x1b) {
	uint16 left = shift();
	uint16 top = shift();
	uint16 sprite = shift();

	debugC(4, kDebugLevelAnimation, "anim opcode 0x1b: add absolute sprite %d: %d:%d", sprite, left, top);

	Sprite *s = new Sprite(_resources->loadSprite(sprite));
	s->setPosition(Common::Point(left, top));
	s->setAbsolute();
	_sprites.push_back(s);

	return kOk;
}

}
