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
#include "engines/darkseed2/imageconverter.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/conversation.h"
#include "engines/darkseed2/graphicalobject.h"
#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

// Coordinates of the different areas
static const int32 kTextAreaWidth  = 512; ///< The width of the raw text area.
static const int32 kTextAreaHeight =  50; ///< The height of the raw text area.
static const int32 kTextHeight     =  14; ///< The height of a text line.
static const int32 kTextMargin     =  90; ///< The maximum width of a text line.

// Sprite file names
static const char *kSpriteFrame        = "INVNTRY1"; ///< The conversation box's general frame.
static const char *kSpriteScrollUpDown = "DIALOG1";  ///< Both scroll buttons active.
static const char *kSpriteScrollDown   = "DIALOG2";  ///< Scroll down active, scroll up grayed out.
static const char *kSpriteScrollUp     = "DIALOG3";  ///< Scroll up active, scroll down grayed out.

// Scroll button coordinates
static const int32 kScrollUp  [4] = {15, 24, 34, 40};
static const int32 kScrollDown[4] = {15, 41, 34, 57};

// Colors
static const byte kColorSelected  [3] = {255, 255, 255};
static const byte kColorUnselected[3] = {239, 167, 127};
static const byte kColorShading   [3] = {  0,   0,   0};

ConversationBox::Line::Line(TalkLine *line, uint32 colorSelected, uint32 colorUnselected) {
	talk = line;
	if (talk) {
		int32 width = TextObject::wrap(talk->getTXT(), texts, 460);

		for (Common::StringList::iterator it = texts.begin(); it != texts.end(); ++it) {
			textObjectsSelected.push_back  (new TextObject(*it, 0, 0, colorSelected  , width));
			textObjectsUnselected.push_back(new TextObject(*it, 0, 0, colorUnselected, width));
		}
	}
}

ConversationBox::Line::~Line() {
	Common::Array<TextObject *>::iterator text;
	for (text = textObjectsSelected.begin(); text != textObjectsSelected.end(); ++text)
		delete *text;
	for (text = textObjectsUnselected.begin(); text != textObjectsUnselected.end(); ++text)
		delete *text;

	delete talk;
}

const Common::String &ConversationBox::Line::getName() const {
	return talk->getName();
}


const Common::String &ConversationBox::PhysLineRef::getName() const {
	return (*itLine)->getName();
}

const Common::String &ConversationBox::PhysLineRef::getString() const {
	return *itString;
}

TextObject *ConversationBox::PhysLineRef::getSelectedText() {
	return *itTextSel;
}

TextObject *ConversationBox::PhysLineRef::getUnselectedText() {
	return *itTextUnsel;
}

ConversationBox::Line *ConversationBox::PhysLineRef::getLine() {
	return *itLine;
}

uint32 ConversationBox::PhysLineRef::getLineNum() const {
	return n;
}

bool ConversationBox::PhysLineRef::isTop() const {
	return itString == (*itLine)->texts.begin();
}


ConversationBox::ConversationBox(Resources &resources, Variables &variables,
		Graphics &graphics, TalkManager &talkManager) {

	_resources = &resources;
	_variables = &variables;
	_graphics  = &graphics;
	_talkMan   = &talkManager;

	_conversation = new Conversation(*_variables);

	_area = Common::Rect(kWidth, kHeight);

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
	_textAreas[0] = Common::Rect(kTextMargin, kTextHeight * 1,
			kWidth - 2 * kTextMargin, kTextHeight * 2);
	_textAreas[1] = Common::Rect(kTextMargin, kTextHeight * 2,
			kWidth - 2 * kTextMargin, kTextHeight * 3);
	_textAreas[2] = Common::Rect(kTextMargin, kTextHeight * 3,
			kWidth - 2 * kTextMargin, kTextHeight * 4);

	_scrollAreas[0] = Common::Rect(kScrollUp  [0], kScrollUp  [1], kScrollUp  [2], kScrollUp  [3]);
	_scrollAreas[1] = Common::Rect(kScrollDown[0], kScrollDown[1], kScrollDown[2], kScrollDown[3]);

	loadSprites();
	build();
	redrawLines();
}

ConversationBox::~ConversationBox() {
	clearLines();

	delete _conversation;

	delete _markerSelect;
	delete _markerUnselect;

	delete[] _sprites;
}

void ConversationBox::updateColors() {
	_colorSelected   = ImgConv.getColor(kColorSelected  [0], kColorSelected  [1], kColorSelected  [2]);
	_colorUnselected = ImgConv.getColor(kColorUnselected[0], kColorUnselected[1], kColorUnselected[2]);
	_colorShading    = ImgConv.getColor(kColorShading   [0], kColorShading   [1], kColorShading   [2]);
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

void ConversationBox::stop() {
	if (!_conversation)
		return;

	_conversation->clear();
	_state = kStateWaitUserAction;
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

	loaded0 = _sprites[2].loadFromImage(*_resources, kSpriteFrame);
	loaded1 = _sprites[3].loadFromImage(*_resources, kSpriteScrollUpDown);
	loaded2 = _sprites[4].loadFromImage(*_resources, kSpriteScrollDown);
	loaded3 = _sprites[5].loadFromImage(*_resources, kSpriteScrollUp);

	assert(loaded0 && loaded1 && loaded2 && loaded3);

}

void ConversationBox::build() {
	// The shading grid
	_sprites[1].create(kTextAreaWidth, kTextAreaHeight);
	_sprites[1].shade(_colorShading);

	_sprites[0].create(kWidth, kHeight);
	_box.create(kWidth, kHeight);

	_markerSelect   = new TextObject(">", kTextMargin - 9, 0, _colorSelected);
	_markerUnselect = new TextObject("-", kTextMargin - 8, 0, _colorUnselected);

	// Put the shading grid
	_sprites[0].blit(_sprites[1], (kWidth  - kTextAreaWidth ) / 2,
	                              (kHeight - kTextAreaHeight) / 2, true);
	// Put the frame
	_sprites[0].blit(_sprites[2], 0, 0, true);

	_box.blit(_sprites[0], 0, 0, false);
}

void ConversationBox::move(int32 x, int32 y) {
	// Sanity checks
	assert((ABS(x) <= 0x7FFF) && (ABS(y) <= 0x7FFF));

	_area.moveTo(x, y);
}

void ConversationBox::redraw(Sprite &sprite, Common::Rect area) {
	if (!_area.intersects(area))
		return;

	area.clip(_area);

	int32 x = area.left;
	int32 y = area.top;

	area.moveTo(area.left - _area.left, area.top - _area.top);

	sprite.blit(_box, area, x, y, true);
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

	_curLineNumber = 0;
	_curReplyName.clear();
}

void ConversationBox::updateLines() {
	clearLines();

	if (_conversation->hasEnded())
		return;

	Common::Array<TalkLine *> lines = _conversation->getCurrentLines(*_resources);
	for (Common::Array<TalkLine *>::iterator it = lines.begin(); it != lines.end(); ++it) {
		Line *line = new Line(*it, _colorSelected, _colorUnselected);

		line->lineNumber = _lines.size();

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
			uint32 selected = physLineNumToRealLineNum(_selected);

			TextObject *text;

			// Is that line a selected line?
			if ((curLine.getLineNum() + 1) == selected)
				text = curLine.getSelectedText();
			else
				text = curLine.getUnselectedText();

			// Move the line to the correct place and draw it
			text->moveTo(_textAreas[i].left, _textAreas[i].top);
			text->redraw(_box, text->getArea());

			// If that line is a top line, place the correct selected/unselected marker
			if (curLine.isTop()) {
				TextObject *marker;
				if ((curLine.getLineNum() + 1) == selected)
					marker = _markerSelect;
				else
					marker = _markerUnselect;

				marker->moveTo(marker->getArea().left, text->getArea().top);
				marker->redraw(_box, marker->getArea());
			}

			if (!nextPhysLine(curLine))
				// No next line, stop
				break;
		}
	}

	_graphics->requestRedraw(_area);
}

void ConversationBox::redrawLines() {
	_box.blit(_sprites[0], 0, 0, false);

	drawLines();
}

bool ConversationBox::findPhysLine(uint32 n, PhysLineRef &ref) const {
	// Put the first level iterator at the beginning
	ref.itLine = _lines.begin();
	if (ref.itLine == _lines.end())
		return false;

	// Put the second level iterators at the beginning
	ref.itString = (*ref.itLine)->texts.begin();
	ref.itTextSel = (*ref.itLine)->textObjectsSelected.begin();
	ref.itTextUnsel = (*ref.itLine)->textObjectsUnselected.begin();
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
	++ref.itString;
	++ref.itTextSel;
	++ref.itTextUnsel;

	// Find the next non-empty line
	return nextPhysRealLine(ref);
}

bool ConversationBox::nextPhysRealLine(PhysLineRef &ref) const {
	if (ref.itLine == _lines.end())
		// First level iterator is at its end => no next lines
		return false;

	// Are the second level iterators at their end?
	while (ref.itString == (*ref.itLine)->texts.end()) {
		// Advance the first level iterator
		++ref.itLine;
		++ref.n;

		if (ref.itLine == _lines.end())
			// First level iterator is at its end => no next lines
			return false;

		// Set the second level iterator to the beginning
		ref.itString = (*ref.itLine)->texts.begin();
		ref.itTextSel = (*ref.itLine)->textObjectsSelected.begin();
		ref.itTextUnsel = (*ref.itLine)->textObjectsUnselected.begin();
	}

	return true;
}

void ConversationBox::notifyMouseMove(int32 x, int32 y) {
	if (!isActive())
		// Not active => ignore user events
		return;

	if (_state != kStateWaitUserAction)
		// Not at a user action state => ignore user events
		return;

	x -= _area.left;
	y -= _area.top;

	// Which line was selected?
	uint32 selected = getTextArea(x, y);

	if (selected != _selected) {
		// Selection changed, update graphics

		_selected = selected;
		redrawLines();
	}
}

void ConversationBox::notifyClicked(int32 x, int32 y) {
	if (!isActive())
		// Not active => ignore user events
		return;

	notifyMouseMove(x, y);

	if (_state != kStateWaitUserAction)
		// Not at a user action state => ignore user events
		return;

	x -= _area.left;
	y -= _area.top;

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

		speakerVariable(_curSpeaker, false);

		// Playing the replies
		_curReply = 0;
		if (_curReply < _nextReplies.size()) {
			speakLine(*_nextReplies[_curReply]);

			_curSpeaker = _nextReplies[_curReply]->getSpeakerNum();
			speakerVariable(_curSpeaker, true);
		}

		_state = kStatePlayingReply;
		return;
	}

	if (_state == kStatePlayingReply) {
		// We're playing a reply

		if (_talkMan->isTalking())
			// Still talking, we'll continue waiting
			return;

		speakerVariable(_curSpeaker, false);

		_curReply++;
		if (_curReply < _nextReplies.size()) {
			speakLine(*_nextReplies[_curReply]);
			_curSpeaker = _nextReplies[_curReply]->getSpeakerNum();
			speakerVariable(_curSpeaker, true);
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

	_curLineNumber = line->lineNumber;
	_curReplyName = line->talk->getName();

	// Set state
	_state = kStatePlayingLine;

	_curSpeaker = line->talk->getSpeakerNum();

	speakerVariable(_curSpeaker, true);

	// And advance the conversation
	_conversation->pick(line->talk->getName());
}

int ConversationBox::getTextArea(int32 x, int32 y) {
	for (int i = 0; i < 3; i++)
		if (_textAreas[i].contains(x, y))
			return _physLineTop + i + 1;

	return 0;
}

ConversationBox::ScrollAction ConversationBox::getScrollAction(int32 x, int32 y) {
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

void ConversationBox::speakerVariable(uint8 speaker, bool on) {
	Common::String speakerVar = Common::String::printf("SysTalking%d", speaker);

	_variables->set(speakerVar, on ? 1 : 0);
}

bool ConversationBox::saveLoad(Common::Serializer &serializer, Resources &resources) {
	byte state = _state;

	if (!_conversation->doSaveLoad(serializer, resources))
		return false;

	SaveLoad::sync(serializer, _physLineTop);
	SaveLoad::sync(serializer, state);
	SaveLoad::sync(serializer, _curSpeaker);
	SaveLoad::sync(serializer, _curReply);

	SaveLoad::sync(serializer, _curLineNumber);
	SaveLoad::sync(serializer, _curReplyName);

	_state = (State) state;

	return true;
}

bool ConversationBox::loading(Resources &resources) {
	uint32 physLineTop = _physLineTop;

	State state = _state;

	uint8  curSpeaker    = _curSpeaker;
	uint16 curReply      = _curReply;
	uint32 curLineNumber = _curLineNumber;

	Common::String curReplyName = _curReplyName;

	updateLines();

	_physLineTop = physLineTop;

	_state = state;

	_curSpeaker    = curSpeaker;
	_curReply      = curReply;
	_curLineNumber = curLineNumber;
	_curReplyName  = curReplyName;

	redrawLines();

	if (_state == kStatePlayingLine) {
		pickLine(_lines[_curLineNumber]);
	} else if (_state == kStatePlayingReply) {
		pickLine(_lines[_curLineNumber]);

		_curSpeaker    = curSpeaker;
		_curReply      = curReply;
		_curLineNumber = curLineNumber;
		_curReplyName  = curReplyName;
		_state         = state;

		_curSpeaker = _nextReplies[_curReply]->getSpeakerNum();
		speakerVariable(_curSpeaker, true);
		speakLine(*_nextReplies[_curReply]);
	}

	return true;
}

} // End of namespace DarkSeed2
