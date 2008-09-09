#include "innocent/exit.h"
#include "innocent/graphics.h"

namespace Innocent {
//

enum Offsets {
	kOffsetRoom = 0,
	kOffsetPosition = 2,
	kOffsetSprite = 6,
	kOffsetNoSprite = 0xa,
	kOffsetZIndex = 0xb
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
	c.field(_zIndex, kOffsetZIndex);

	snprintf(_debugInfo, 100, "exit %s%d:%d r%d z%d %s", nosprite ? "n" : "s" , _position.x, _position.y, _room, _zIndex, +c);
}

void Exit::paint(Graphics *g) {
	if (sprite())
		g->paint(sprite(), _position);
}

}
