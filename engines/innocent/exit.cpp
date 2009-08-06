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

Exit::Exit(const CodePointer &c)
  :	_enabled(false), _sprite(0) {
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
