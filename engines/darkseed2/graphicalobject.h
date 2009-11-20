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

#ifndef DARKSEED2_GRAPHICALOBJECT_H
#define DARKSEED2_GRAPHICALOBJECT_H

#include "common/rect.h"

#include "engines/darkseed2/darkseed2.h"

namespace Common {
	class String;
}

namespace DarkSeed2 {

class Sprite;

class GraphicalObject {
public:
	GraphicalObject();
	virtual ~GraphicalObject();

	Common::Rect getArea() const;

	virtual void redraw(Sprite &sprite, Common::Rect area) = 0;

protected:
	Common::Rect _area;
};

class TextObject : public GraphicalObject {
public:
	TextObject(const Common::String &text, uint32 x, uint32 y,
			byte color, uint32 maxWidth = 0);
	~TextObject();

	void redraw(Sprite &sprite, Common::Rect area);

private:
	Sprite *_sprite;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_GRAPHICALOBJECT_H
