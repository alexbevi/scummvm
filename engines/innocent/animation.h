#ifndef INNOCENT_ANIMATION_H
#define INNOCENT_ANIMATION_H

#include "common/list.h"
#include "common/rect.h"

#include "innocent/value.h"

namespace Innocent {
//

class Graphics;
class Resources;

class Animation {
public:
	enum Status {
		kOk,
		kRemove
	};

	Animation(const CodePointer &code, Common::Point position);
	~Animation();
	Status tick();

	void paint(Graphics *g);

private:
	class Sprite;

	uint16 shift();
	int8 shiftByte();
	int8 embeddedByte() const;
	void clearSprites();

	template <int opcode>
	Status opcodeHandler();

	template <int N>
	void init_opcodes();

	Resources *_resources;

	typedef Status (Animation::*OpcodeHandler)();
	OpcodeHandler _handlers[38];

	int8 _zIndex;
	Common::Point _position;
	byte *_code;
	char _debugInfo[50];
	Common::List<Sprite *> _sprites;
};

}

#endif
