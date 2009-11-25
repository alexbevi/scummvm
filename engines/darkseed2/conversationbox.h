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

#ifndef DARKSEED2_CONVERSATIONBOX_H
#define DARKSEED2_CONVERSATIONBOX_H

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/sprite.h"

namespace Common {
	class String;
}

namespace DarkSeed2 {

class Graphics;

class TextObject;

class ConversationBox {
public:
	ConversationBox(Resources &resources, Graphics &graphics);
	~ConversationBox();

	void loadSprites();
	void resetSprites();

	void build();

	const Sprite &getSprite() const;

private:
	Resources *_resources;
	Graphics  *_graphics;

	Sprite *_origSprites;
	Sprite *_sprites;

	TextObject *_markerSelect;
	TextObject *_markerUnselect;
	TextObject **_texts;
	Common::String *_lines;

	int _selected;

	Sprite _box;

	void createTexts();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATIONBOX_H
