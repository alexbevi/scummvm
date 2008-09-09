#include "innocent/exit.h"
#include "innocent/graphics.h"

namespace Innocent {
//

enum Offsets {
	kOffsetRoom = 0,
	kOffsetPosition = 2,
	kOffsetSprite = 6,
	kOffsetNoSprite = 0xa
};

Exit::Exit(const CodePointer &c) {
	debugC(4, kDebugLevelFiles, "loading exit from %s", +c);
	bool nosprite;
	c.field(nosprite, kOffsetNoSprite);
	if (!nosprite)
		c.field(_sprite, kOffsetSprite);
	else
		debugC(5, kDebugLevelFiles, "exit has no sprite");
	c.field(_position, kOffsetPosition);
	c.field(_room, kOffsetRoom);
}

void Exit::paint(Graphics *g) {
	if (sprite())
		g->paint(sprite(), _position);
}

}
