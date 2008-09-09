#ifndef INNOCENT_ANIMATION_H
#define INNOCENT_ANIMATION_H

#include <memory>

#include "common/list.h"
#include "common/rect.h"

#include "innocent/value.h"

namespace Innocent {
//

class Graphics;
class Resources;
class Sprite;

class Animation {
public:
	enum Status {
		kOk,
		kRemove,
		kFrameDone
	};

	Animation(const CodePointer &code, Common::Point position);
	~Animation();
	Status tick();

	void paint(Graphics *g);

protected:
	class Sprite;

	uint16 shift();
	int8 shiftByte();
	int8 embeddedByte() const;
	void clearSprites();
	void setMainSprite(uint16 sprite);

	template <int opcode>
	Status opcodeHandler();

	template <int N>
	void init_opcodes();

	Resources *_resources;

	typedef Status (Animation::*OpcodeHandler)();
	OpcodeHandler _handlers[38];

	int8 _interval;
	int16 _ticksLeft;
	int8 _zIndex;
	Common::Point _position;
	byte *_base;
	uint16 _offset;
	char _debugInfo[50];
	Common::List<Sprite *> _sprites;
	std::auto_ptr<Innocent::Sprite> _mainSprite;
	int8 _counter;
	uint16 _loopStart;
};

}

#endif
