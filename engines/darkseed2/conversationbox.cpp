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

#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/conversation.h"
#include "engines/darkseed2/graphicalobject.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

ConversationBox::Line::Line(TalkLine *line, byte color) {
	talk = line;
	if (talk) {
		uint32 width = TextObject::wrap(talk->getTXT(), texts, 460);

		for (Common::StringList::iterator it = texts.begin(); it != texts.end(); ++it)
			textObjects.push_back(new TextObject(*it, 0, 0, color, width));
	}
}

ConversationBox::Line::~Line() {
	for (Common::Array<TextObject *>::iterator it = textObjects.begin(); it != textObjects.end(); ++it)
		delete *it;

	delete talk;
}

const Common::String &ConversationBox::Line::getName() const {
	return talk->getName();
}


const Common::String &ConversationBox::PhysLineRef::getName() const {
	return (*it1)->getName();
}

const Common::String &ConversationBox::PhysLineRef::getString() const {
	return *it2;
}

TextObject &ConversationBox::PhysLineRef::getTextObject() {
	return **it3;
}

ConversationBox::Line *ConversationBox::PhysLineRef::getLine() {
	return *it1;
}

uint32 ConversationBox::PhysLineRef::getLineNum() const {
	return n;
}

bool ConversationBox::PhysLineRef::isTop() const {
	return it2 == (*it1)->texts.begin();
}


ConversationBox::ConversationBox(Resources &resources, Variables &variables,
		Graphics &graphics, TalkManager &talkManager) {

	_resources = &resources;
	_variables = &variables;
	_graphics  = &graphics;
	_talkMan   = &talkManager;

	_conversation = new Conversation(*_variables);

	_box.create(_width, _height);

	_origSprites = new Sprite[4];
	_sprites     = new Sprite[6];

	_physLineCount = 0;
	_physLineTop   = 0;

	_selected = 0;

	_markerSelect   = 0;
	_markerUnselect = 0;

	_state = kStateWaitUserAction;
	_nextReply = 0;

	updateColors();

	_textAreas[0] = Common::Rect(_textMargin, _textHeight * 1,
			_width - 2 * _textMargin, _textHeight * 2);
	_textAreas[1] = Common::Rect(_textMargin, _textHeight * 2,
			_width - 2 * _textMargin, _textHeight * 3);
	_textAreas[2] = Common::Rect(_textMargin, _textHeight * 3,
			_width - 2 * _textMargin, _textHeight * 4);

	_scrollAreas[0] = Common::Rect(15, 24, 34, 40);
	_scrollAreas[1] = Common::Rect(15, 41, 34, 57);

	loadSprites();
	resetSprites();
}

ConversationBox::~ConversationBox() {
	clearLines();

	delete _conversation;

	delete _markerSelect;
	delete _markerUnselect;

	delete[] _sprites;
	delete[] _origSprites;
}

void ConversationBox::updateColors() {
	_colorSelected   = _graphics->getPalette().findWhite();
	_colorUnselected = _graphics->getPalette().findColor(239, 167, 127);
	_colorBlack      = _graphics->getPalette().findBlack();
}

void ConversationBox::newPalette() {
	updateColors();
	resetSprites();
	rebuild();
}

bool ConversationBox::start(const Common::String &conversation) {
	if (!_conversation->parse(*_resources, conversation))
		return false;

	updateLines();
	drawLines();

	return true;
}

bool ConversationBox::isActive() const {
	return !_conversation->hasEnded();
}

void ConversationBox::loadSprites() {
	bool loaded0, loaded1, loaded2, loaded3;

	loaded0 = _origSprites[0].loadFromBMP(*_resources, "INVNTRY1");
	loaded1 = _origSprites[1].loadFromBMP(*_resources, "DIALOG1");
	loaded2 = _origSprites[2].loadFromBMP(*_resources, "DIALOG2");
	loaded3 = _origSprites[3].loadFromBMP(*_resources, "DIALOG3");

	assert(loaded0 && loaded1 && loaded2 && loaded3);

	_sprites[0].create(_width, _height);
	_box.create(_width, _height);
}

void ConversationBox::resetSprites() {
	for (int i = 0; i < 4; i++) {
		_sprites[i + 2] = _origSprites[i];

		_graphics->mergePalette(_sprites[i + 2]);
	}

	_sprites[1].create(_textAreaWidth, _textAreaHeight);
	_sprites[1].shade(_colorBlack);

	delete _markerSelect;
	delete _markerUnselect;

	_markerSelect   = new TextObject(">", _textMargin - 9, 0, _colorSelected);
	_markerUnselect = new TextObject("-", _textMargin - 8, 0, _colorUnselected);
}

void ConversationBox::rebuild() {
	_box.clear();
	_sprites[0].clear();

	_sprites[0].blit(_sprites[1], (_width  - _textAreaWidth ) / 2,
	                              (_height - _textAreaHeight) / 2, true);
	_sprites[0].blit(_sprites[2], 0, 0, true);

	_box.blit(_sprites[0], 0, 0, false);
}

void ConversationBox::redraw(Sprite &sprite, uint32 x, uint32 y, const Common::Rect &area) {
	Common::Rect boxArea(_box.getWidth(), _box.getHeight());

	boxArea.moveTo(x, y);

	if (!boxArea.intersects(area))
		return;

	boxArea.clip(area);

	boxArea.moveTo(0, 0);

	sprite.blit(_box, boxArea, x, y, true);
}

void ConversationBox::clearLines() {
	delete _nextReply;
	_nextReply = 0;

	for (Common::Array<Line *>::iterator it = _lines.begin(); it != _lines.end(); ++it)
		delete *it;
	_lines.clear();

	_physLineCount = 0;
	_physLineTop   = 0;

	_state = kStateWaitUserAction;
}

void ConversationBox::updateLines() {
	clearLines();

	if (_conversation->hasEnded())
		return;

	Common::Array<TalkLine *> lines = _conversation->getCurrentLines(*_resources);
	for (Common::Array<TalkLine *>::iterator it = lines.begin(); it != lines.end(); ++it) {
		Line *line = new Line(*it, _colorUnselected);

		_lines.push_back(line);
		_physLineCount += line->texts.size();
	}
}

void ConversationBox::updateScroll() {
	Common::Rect scrollArea = _sprites[3].getArea();

	if (!canScroll())
		_sprites[0].blit(_sprites[2], scrollArea, 0, 0, true);
	else if (canScrollUp() && canScrollDown())
		_sprites[0].blit(_sprites[3], 0, 0, true);
	else if (canScrollUp())
		_sprites[0].blit(_sprites[5], 0, 0, true);
	else
		// Can scroll down
		_sprites[0].blit(_sprites[4], 0, 0, true);

	_box.blit(_sprites[0], 0, 0, false);
}

void ConversationBox::drawLines() {
	updateScroll();

	PhysLineRef curLine;

	if (findPhysLine(_physLineTop, curLine)) {
		for (int i = 0; i < 3; i++) {
			TextObject &text = curLine.getTextObject();

			uint32 selected = physLineNumToRealLineNum(_selected);

			if ((curLine.getLineNum() + 1) == selected)
				text.recolor(_colorSelected);
			else
				text.recolor(_colorUnselected);

			text.move(_textAreas[i].left, _textAreas[i].top);
			text.redraw(_box, text.getArea());

			if (curLine.isTop()) {
				TextObject *marker;
				if ((curLine.getLineNum() + 1) == selected)
					marker = _markerSelect;
				else
					marker = _markerUnselect;

				marker->move(marker->getArea().left, text.getArea().top);
				marker->redraw(_box, marker->getArea());
			}

			if (!nextPhysLine(curLine))
				break;
		}
	}

	_graphics->redraw(kScreenPartConversation);
}

void ConversationBox::redrawLines() {
	_box.blit(_sprites[0], 0, 0, false);

	drawLines();
}

bool ConversationBox::findPhysLine(uint32 n, PhysLineRef &ref) const {
	ref.it1 = _lines.begin();
	if (ref.it1 == _lines.end())
		return false;

	ref.it2 = (*ref.it1)->texts.begin();
	ref.it3 = (*ref.it1)->textObjects.begin();
	ref.n = 0;

	if (!nextPhysRealLine(ref))
		return false;

	while (n-- > 0)
		if (!nextPhysLine(ref))
			return false;

	return true;
}

bool ConversationBox::nextPhysLine(PhysLineRef &ref) const {
	++ref.it2;
	++ref.it3;

	return nextPhysRealLine(ref);
}

bool ConversationBox::nextPhysRealLine(PhysLineRef &ref) const {
	if (ref.it1 == _lines.end())
		return false;

	while (ref.it2 == (*ref.it1)->texts.end()) {
		++ref.it1;
		++ref.n;

		if (ref.it1 == _lines.end())
			return false;

		ref.it2 = (*ref.it1)->texts.begin();
		ref.it3 = (*ref.it1)->textObjects.begin();
	}

	return true;
}

void ConversationBox::notifyMouseMove(uint32 x, uint32 y) {
	if (!isActive())
		return;

	_mouseX = x;
	_mouseY = y;

	if (_state != kStateWaitUserAction)
		return;

	uint32 selected = getTextArea(x, y);

	if (selected != _selected) {
		_selected = selected;
		redrawLines();
	}
}

void ConversationBox::notifyClicked(uint32 x, uint32 y) {
	if (!isActive())
		return;

	notifyMouseMove(x, y);

	if (_state != kStateWaitUserAction)
		return;

	doScroll(getScrollAction(x, y));
	pickLine(getSelectedLine());
}

void ConversationBox::updateStatus() {
	if (_state == kStateWaitUserAction)
		return;

	if (_state == kStatePlayingLine) {
		if (_talkMan->isTalking())
			// Still talking, we'll continue waiting
			return;

		// Playing the reply
		if (_nextReply)
			speakLine(*_nextReply);

		_state = kStatePlayingReply;
		return;
	}

	if (_state == kStatePlayingReply) {
		if (_talkMan->isTalking())
			// Still talking, we'll continue waiting
			return;

		delete _nextReply;
		_nextReply = 0;

		// Done playing, show the next lines
		updateLines();
		drawLines();

		_state = kStateWaitUserAction;
		return;
	}
}

void ConversationBox::pickLine(Line *line) {
	if (!line)
		return;

	_nextReply = _conversation->getReply(*_resources, line->talk->getName());

	speakLine(*line->talk);

	_state = kStatePlayingLine;

	_conversation->pick(line->talk->getName());
}

int ConversationBox::getTextArea(uint32 x, uint32 y) {
	for (int i = 0; i < 3; i++)
		if (_textAreas[i].contains(x, y))
			return _physLineTop + i + 1;

	return 0;
}

ConversationBox::ScrollAction ConversationBox::getScrollAction(uint32 x, uint32 y) {
	for (int i = 0; i < 2; i++)
		if (_scrollAreas[i].contains(x, y))
			return (ScrollAction) i;

	return kScrollActionNone;
}

ConversationBox::Line *ConversationBox::getSelectedLine() {
	if (_selected == 0)
		return 0;

	PhysLineRef line;
	if (!findPhysLine(_selected - 1, line))
		return 0;

	return line.getLine();
}

bool ConversationBox::canScroll() const {
	return _physLineCount > 3;
}

bool ConversationBox::canScrollUp() const {
	return _physLineTop > 0;
}

bool ConversationBox::canScrollDown() const {
	return (_physLineTop + 3) < _physLineCount;
}

void ConversationBox::doScroll(ScrollAction scroll) {
	switch (scroll) {
	case kScrollActionNone:
		return;

	case kScrollActionUp:
		if (!canScrollUp())
			return;

		_physLineTop--;
		break;

	case kScrollActionDown:
		if (!canScrollDown())
			return;

		_physLineTop++;
		break;
	}

	drawLines();
}

uint32 ConversationBox::physLineNumToRealLineNum(uint32 physLineNum) const {
	if (physLineNum == 0)
		return 0;

	int realLineNum = 1;
	for (Common::Array<Line *>::const_iterator it = _lines.begin(); it != _lines.end(); ++it) {
		uint32 size = (*it)->texts.size();

		if (size >= physLineNum)
			return realLineNum;

		physLineNum -= size;
		realLineNum++;
	}

	return 0;
}

void ConversationBox::speakLine(TalkLine &line) {
	_talkMan->talk(line);
}

} // End of namespace DarkSeed2
