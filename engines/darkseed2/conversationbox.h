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

#include "common/rect.h"
#include "common/str.h"
#include "common/array.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class Graphics;

class Conversation;

class TextObject;
class Sprite;
class TalkLine;

class ConversationBox {
public:
	static const uint32 _width  = 640;
	static const uint32 _height = 70;

	ConversationBox(Resources &resources, Variables &variables, Graphics &graphics);
	~ConversationBox();

	void newPalette();
	bool start(const Common::String &conversation);

	bool isActive() const;

	void redraw(Sprite &sprite, uint32 x, uint32 y, const Common::Rect &area);

	void notifyMouseMove(uint32 x, uint32 y);
	void notifyClicked(uint32 x, uint32 y);

private:
	static const uint32 _textAreaWidth  = 512;
	static const uint32 _textAreaHeight = 50;
	static const uint32 _textHeight = 14;
	static const uint32 _textMargin = 90;

	struct Line {
		TalkLine *talk;
		Common::StringList texts;
		Common::Array<TextObject *> textObjects;

		Line(TalkLine *line = 0, byte color = 0);
		~Line();

		const Common::String &getName() const;
	};

	struct PhysLineRef {
		uint32 n;
		Common::Array<Line *>::const_iterator it1;
		Common::StringList::const_iterator it2;
		Common::Array<TextObject *>::iterator it3;

		const Common::String &getName() const;
		const Common::String &getString() const;
		TextObject &getTextObject();

		uint32 getLineNum() const;
		bool isTop() const;
	};

	Resources *_resources;
	Variables *_variables;
	Graphics  *_graphics;

	Conversation *_conversation;

	Sprite *_origSprites;
	Sprite *_sprites;

	TextObject *_markerSelect;
	TextObject *_markerUnselect;

	Common::Array<Line *> _lines;

	Common::Rect _textAreas[3];

	uint32 _physLineCount;
	uint32 _physLineTop;

	uint32 _selected;

	Sprite _box;

	byte _colorSelected;
	byte _colorUnselected;
	byte _colorBlack;

	void loadSprites();
	void resetSprites();

	void rebuild();

	void updateColors();

	void clearLines();
	void updateLines();
	void updateScroll();
	void drawLines();
	void redrawLines();

	bool findPhysLine(uint32 n, PhysLineRef &ref) const;
	bool nextPhysLine(PhysLineRef &ref) const;
	bool nextPhysRealLine(PhysLineRef &ref) const;

	int getTextArea(uint32 x, uint32 y);
	Line *getSelectedLine();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATIONBOX_H
