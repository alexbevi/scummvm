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
	_curReply = 0xFFFF;

	updateColors();

	// Regions of the three visible lines
	_textAreas[0] = Common::Rect(_textMargin, _textHeight * 1,
			_width - 2 * _textMargin, _textHeight * 2);
	_textAreas[1] = Common::Rect(_textMargin, _textHeight * 2,
			_width - 2 * _textMargin, _textHeight * 3);
	_textAreas[2] = Common::Rect(_textMargin, _textHeight * 3,
			_width - 2 * _textMargin, _textHeight * 4);

	_scrollAreas[0] = Common::Rect(15, 24, 34, 40); // Scroll up button
	_scrollAreas[1] = Common::Rect(15, 41, 34, 57); // Scroll down button

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
	debugC(-1, kDebugConversation, "Starting conversation \"%s\"", conversation.c_str());

	if (!_conversation->parse(*_resources, conversation))
		return false;

	updateLines();
	drawLines();

	return true;
}

bool ConversationBox::restart() {
	if (!_conversation)
		return false;

	debugC(-1, kDebugConversation, "Restarting conversation");

	_conversation->reset();

	updateLines();
	drawLines();

	return true;
}

bool ConversationBox::isActive() const {
	if (!_conversation)
		return false;

	if (_state != kStateWaitUserAction)
		return true;

	return !_conversation->hasEnded();
}

void ConversationBox::loadSprites() {
	bool loaded0, loaded1, loaded2, loaded3;

	// The box's frame
	loaded0 = _origSprites[0].loadFromBMP(*_resources, "INVNTRY1");
	// Both scroll buttons active
	loaded1 = _origSprites[1].loadFromBMP(*_resources, "DIALOG1");
	// Scroll down active, scroll up grayed out
	loaded2 = _origSprites[2].loadFromBMP(*_resources, "DIALOG2");
	// Scroll up active, scroll down grayed out
	loaded3 = _origSprites[3].loadFromBMP(*_resources, "DIALOG3");

	assert(loaded0 && loaded1 && loaded2 && loaded3);

	_sprites[0].create(_width, _height);
	_box.create(_width, _height);
}

void ConversationBox::resetSprites() {
	// _origSprites[i] -> _sprites[i + 2], for all 4 box elements
	for (int i = 0; i < 4; i++) {
		_sprites[i + 2] = _origSprites[i];

		_graphics->mergePalette(_sprites[i + 2]);
	}

	// The shading grid
	_sprites[1].create(_textAreaWidth, _textAreaHeight);
	_sprites[1].shade(_colorBlack);

	delete _markerSelect;
	delete _markerUnselect;

	_markerSelect   = new TextObject(">", _textMargin - 9, 0, _colorSelected);
	_markerUnselect = new TextObject("-", _textMargin - 8, 0, _colorUnselected);
}

void ConversationBox::rebuild() {
	// Clear everything
	_box.clear();
	_sprites[0].clear();

	// Put the shading grid
	_sprites[0].blit(_sprites[1], (_width  - _textAreaWidth ) / 2,
	                              (_height - _textAreaHeight) / 2, true);
	// Put the frame
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
	clearReplies();

	for (Common::Array<Line *>::iterator it = _lines.begin(); it != _lines.end(); ++it)
		delete *it;
	_lines.clear();

	_physLineCount = 0;
	_physLineTop   = 0;

	_state = kStateWaitUserAction;
}

void ConversationBox::clearReplies() {
	for (Common::Array<TalkLine *>::iterator it = _nextReplies.begin(); it != _nextReplies.end(); ++it)
		delete *it;
	_nextReplies.clear();
	_curReply = 0xFFFF;
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

	// Look which scroll directions are possible and blit the fitting sprite
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
	// Update the scroll sprite
	updateScroll();

	PhysLineRef curLine;

	// Update the lines
	if (findPhysLine(_physLineTop, curLine)) {
		for (int i = 0; i < 3; i++) {
			TextObject &text = curLine.getTextObject();

			uint32 selected = physLineNumToRealLineNum(_selected);

			// Is that line a selected line? Color it accordingly
			if ((curLine.getLineNum() + 1) == selected)
				text.recolor(_colorSelected);
			else
				text.recolor(_colorUnselected);

			// Move the line to the correct place and draw it
			text.move(_textAreas[i].left, _textAreas[i].top);
			text.redraw(_box, text.getArea());

			// If that line is a top line, place the correct selected/unselected marker
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
				// No next line, stop
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
	// Put the first level iterator at the beginning
	ref.it1 = _lines.begin();
	if (ref.it1 == _lines.end())
		return false;

	// Put the second level iterators at the beginning
	ref.it2 = (*ref.it1)->texts.begin();
	ref.it3 = (*ref.it1)->textObjects.begin();
	ref.n = 0;

	// Find the first non-empty line
	if (!nextPhysRealLine(ref))
		return false;

	// Iterate to the nth line
	while (n-- > 0)
		if (!nextPhysLine(ref))
			return false;

	return true;
}

bool ConversationBox::nextPhysLine(PhysLineRef &ref) const {
	// Advance second level iterators
	++ref.it2;
	++ref.it3;

	// Find the next non-empty line
	return nextPhysRealLine(ref);
}

bool ConversationBox::nextPhysRealLine(PhysLineRef &ref) const {
	if (ref.it1 == _lines.end())
		// First level iterator is at its end => no next lines
		return false;

	// Are the second level iterators at their end?
	while (ref.it2 == (*ref.it1)->texts.end()) {
		// Advance the first level iterator
		++ref.it1;
		++ref.n;

		if (ref.it1 == _lines.end())
			// First level iterator is at its end => no next lines
			return false;

		// Set the second level iterator to the beginning
		ref.it2 = (*ref.it1)->texts.begin();
		ref.it3 = (*ref.it1)->textObjects.begin();
	}

	return true;
}

void ConversationBox::notifyMouseMove(uint32 x, uint32 y) {
	if (!isActive())
		// Not active => ignore user events
		return;

	// Remember mouse coordinates
	_mouseX = x;
	_mouseY = y;

	if (_state != kStateWaitUserAction)
		// Not at a user action state => ignore user events
		return;

	// Which line was selected?
	uint32 selected = getTextArea(x, y);

	if (selected != _selected) {
		// Selection changed, update graphics

		_selected = selected;
		redrawLines();
	}
}

void ConversationBox::notifyClicked(uint32 x, uint32 y) {
	if (!isActive())
		// Not active => ignore user events
		return;

	notifyMouseMove(x, y);

	if (_state != kStateWaitUserAction)
		// Not at a user action state => ignore user events
		return;

	// Line scrolling
	doScroll(getScrollAction(x, y));
	// Line picking
	pickLine(getSelectedLine());
}

void ConversationBox::updateStatus() {
	if (_state == kStateWaitUserAction)
		// We're waiting for a user action
		return;

	if (_state == kStatePlayingLine) {
		// We're playing a line

		if (_talkMan->isTalking())
			// Still talking, we'll continue waiting
			return;

		// Playing the replies
		_curReply = 0;
		if (_curReply < _nextReplies.size())
			speakLine(*_nextReplies[_curReply]);

		_state = kStatePlayingReply;
		return;
	}

	if (_state == kStatePlayingReply) {
		// We're playing a reply

		if (_talkMan->isTalking())
			// Still talking, we'll continue waiting
			return;

		_curReply++;
		if (_curReply < _nextReplies.size()) {
			speakLine(*_nextReplies[_curReply]);
			return;
		}

		clearReplies();

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

	// Get new replies
	clearReplies();
	_nextReplies = _conversation->getReplies(*_resources, line->talk->getName());

	// Start talking the line
	speakLine(*line->talk);

	// Set state
	_state = kStatePlayingLine;

	// And advance the conversation
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
		// Iterate through all lines, substract the number of sub-lines as long as it's >= 0.
		// As soon as it's < 0, we've found our real line number

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
