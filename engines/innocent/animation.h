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
	Animation(const CodePointer &code, Common::Point position);
	~Animation();
	void tick();

	void paint(Graphics *g);

private:
	class Sprite;

	uint16 shift();
	void clearSprites();

	template <int opcode>
	void opcodeHandler();

	template <int N>
	void init_opcodes();

	Resources *_resources;

	typedef void (Animation::*OpcodeHandler)();
	OpcodeHandler _handlers[38];

	Common::Point _position;
	byte *_code;
	char _debugInfo[50];
	Common::List<Sprite *> _sprites;
};

}

#endif
