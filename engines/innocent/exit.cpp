#include "innocent/exit.h"

#include "innocent/debugger.h"
#include "innocent/graphics.h"
#include "innocent/resources.h"

namespace Innocent {
//

enum Offsets {
	kOffsetRoom = 0,
	kOffsetPosition = 2,
	kOffsetSprite = 6,
	kOffsetWidth = 6,
	kOffsetHeight = 7,
	kOffsetClickHandler = 8,
	kOffsetNoSprite = 0xa,
	kOffsetZIndex = 0xb
};

Exit::Exit(const CodePointer &c) {
	debugC(4, kDebugLevelFiles, "loading exit from %s", +c);

	bool nosprite;
	c.field(nosprite, kOffsetNoSprite);
	if (!nosprite) {
		c.field(_sprite, kOffsetSprite);
		_rect = Common::Rect(_sprite->w, _sprite->h);
	} else {
		byte w, h;
		c.field(w, kOffsetWidth);
		c.field(h, kOffsetHeight);
		_rect = Common::Rect(w, h);
		debugC(5, kDebugLevelFiles, "exit has no sprite");
	}

	c.field(_position, kOffsetPosition);
	_rect.moveTo(_position.x, _position.y);

	if (!nosprite) // these have bottom for some reason
		_rect.translate(0, -(_rect.height() - 1));
	
	c.field(_room, kOffsetRoom);
	c.field(_zIndex, kOffsetZIndex);

	uint16 offset;
	c.field(offset, kOffsetClickHandler);
	_clickHandler = CodePointer(offset, c.interpreter());

	snprintf(_debugInfo, 100, "exit %s%s r%d z%d %s", nosprite ? "n" : "s" , +_rect, _room, _zIndex, +c);
}

void Exit::paint(Graphics *g) {
	if (sprite())
		g->paint(sprite(), _position);
}

byte Exit::zIndex() const {
	return _zIndex;
}

Common::Rect Exit::area() const {
	return _rect;
}

void Exit::clicked() {
	debugC(3, kDebugLevelEvents, "%s got clicked!", +*this);
	Debug.clickHandler();
	_clickHandler.run(kCodeItem);
}

} // end of namespace
