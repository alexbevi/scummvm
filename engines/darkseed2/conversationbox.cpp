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

#include "common/str.h"

#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

ConversationBox::ConversationBox(Resources &resources, Graphics &graphics) {
	_resources = &resources;
	_graphics  = &graphics;

	_box.create(640, 70);

	_origSprites = new Sprite[4];
	_sprites     = new Sprite[5];

	_lines = new Common::String[3];
	_texts = new TextObject*[3];
	for (int i = 0; i < 3; i++)
		_texts[i] = 0;

	_selected = 1;

	_lines[0] = "- Am I a suspect, Sheriff?";
	_lines[1] = "- Didn't we go all over this before?";
	_lines[2] = "- Rita and I weren't that close, Sheriff.";

	loadSprites();
	resetSprites();
}

ConversationBox::~ConversationBox() {
	delete[] _lines;
	for (int i = 0; i < 3; i++)
		delete _texts[i];
	delete[] _texts;

	delete[] _sprites;
	delete[] _origSprites;
}

void ConversationBox::loadSprites() {
	bool loaded0, loaded1, loaded2, loaded3;

	loaded0 = _origSprites[0].loadFromBMP(*_resources, "INVNTRY1");
	loaded1 = _origSprites[1].loadFromBMP(*_resources, "DIALOG1");
	loaded2 = _origSprites[2].loadFromBMP(*_resources, "DIALOG2");
	loaded3 = _origSprites[3].loadFromBMP(*_resources, "DIALOG3");

	assert(loaded0 && loaded1 && loaded2 && loaded3);
}

void ConversationBox::resetSprites() {
	for (int i = 0; i < 4; i++) {
		_sprites[i + 1] = _origSprites[i];

		_graphics->mergePalette(_sprites[i + 1]);
	}

	_sprites[0].create(512, 50);
	_sprites[0].fill(_graphics->getPalette().findBlack());

	_box.create(640, 70);
}

void ConversationBox::build() {
	_box.clear();

	_box.blit(_sprites[0], 64, 10, true);
	_box.blit(_sprites[1], 0, 0, true);
	_box.blit(_sprites[3], 0, 0, true);

	createTexts();

	for (int i = 0; i < 3; i++)
		if (_texts[i])
			_texts[i]->redraw(_box, _texts[i]->getArea());
}

const Sprite &ConversationBox::getSprite() const {
	return _box;
}

void ConversationBox::createTexts() {
	int y = 14;
	byte white = _graphics->getPalette().findWhite();

	for (int i = 0; i < 3; i++) {
		delete _texts[i];

		_texts[i] = new TextObject(_lines[i], 85, y, white);
		y += 14;
	}
}

} // End of namespace DarkSeed2
