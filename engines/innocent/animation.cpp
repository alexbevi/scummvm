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
void Animation::opcodeHandler(){
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
		_position(position)
{
	_code = code.code();
	_resources = code.interpreter()->resources();
	init_opcodes<37>();
	snprintf(_debugInfo, 50, "animation at %s", +code);
}

Animation::~Animation() {
	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete *it;
}

void Animation::tick() {
	debugC(3, kDebugLevelAnimation, "ticking animation %s", _debugInfo);

	clearSprites();

	int8 opcode = -*_code;
	if (opcode < 0 || opcode >= 0x27)
		error("invalid animation opcode 0x%02x while handling %s", *_code, _debugInfo);
	_code += 2;

	(this->*_handlers[opcode])();
}

void Animation::clearSprites() {
	debugC(5, kDebugLevelAnimation, "clearing sprite list");
	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete (*it);
	_sprites.clear();
}

void Animation::paint(Graphics *g) {
	debugC(4, kDebugLevelAnimation | kDebugLevelGraphics, "painting sprites for animation %s", _debugInfo);
	for (Common::List<Sprite *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		(*it)->paint(g);
}

void Animation::Sprite::paint(Graphics *g) const {
	assert(isAbsolute());
	g->paint(sprite(), _position);
}

uint16 Animation::shift() {
	uint16 value = READ_LE_UINT16(_code);
	_code += 2;
	return value;
}

#define OPCODE(n) template<> void Animation::opcodeHandler<n>()

OPCODE(0x08) {
	uint16 left = shift();
	uint16 top = shift();

	debugC(4, kDebugLevelAnimation, "anim opcode 0x08: move to %d:%d", left, top);

	_position = Common::Point(left, top);
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
}

}
