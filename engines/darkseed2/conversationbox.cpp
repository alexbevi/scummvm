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

	fillInBoxProperties(resources.getVersionFormats().getGameVersion());

	_area = Common::Rect(_boxProps.width, _boxProps.height);

	_sprites     = new Sprite[6];

	_physLineCount = 0;
	_physLineTop   = 0;

	_selected = 0;

	_markerSelect   = 0;
	_markerUnselect = 0;

	_state = kStateWaitUserAction;
	_curReply = 0xFFFF;

	updateColors();

	// Regions of the visible lines
	_textAreas = new Common::Rect[_boxProps.numLines];
	for (uint32 i = 0; i < _boxProps.numLines; i++)
		_textAreas[i] = Common::Rect(_boxProps.textMargin, _boxProps.textHeight * (i + 1),
				_boxProps.width - 2 * _boxProps.textMargin, _boxProps.textHeight * (i + 2));

	_scrollAreas[0] = Common::Rect(_boxProps.scrollUp  [0], _boxProps.scrollUp  [1],
	                               _boxProps.scrollUp  [2], _boxProps.scrollUp  [3]);
	_scrollAreas[1] = Common::Rect(_boxProps.scrollDown[0], _boxProps.scrollDown[1],
	                               _boxProps.scrollDown[2], _boxProps.scrollDown[3]);

	loadSprites();
	build();
	redrawLines();
}

ConversationBox::~ConversationBox() {
	clearLines();

	delete[] _textAreas;

	delete _conversation;

	delete _markerSelect;
	delete _markerUnselect;

	delete[] _sprites;
}

int32 ConversationBox::getWidth() const {
	return _boxProps.width;
}

int32 ConversationBox::getHeight() const {
	return _boxProps.height;
}

void ConversationBox::fillInBoxProperties(GameVersion gameVersion) {
	switch(gameVersion) {
	case kGameVersionWindows:
		_boxProps.width =  640;
		_boxProps.height =  70;

		_boxProps.frameFile = "INVNTRY1";

		_boxProps.frameTopFile    = 0;
		_boxProps.frameBottomFile = 0;
		_boxProps.frameLeftFile   = 0;
		_boxProps.frameRightFile  = 0;

		_boxProps.frameLeftRightWidth = 0;
		_boxProps.frameTopDownHeight  = 0;

		_boxProps.scrollUp[0]   = 15; _boxProps.scrollUp[1]   = 24;
		_boxProps.scrollUp[2]   = 34; _boxProps.scrollUp[3]   = 40;
		_boxProps.scrollDown[0] = 15; _boxProps.scrollDown[1] = 41;
		_boxProps.scrollDown[2] = 34; _boxProps.scrollDown[3] = 57;

		_boxProps.scrollUpFile     = "DIALOG3";
		_boxProps.scrollDownFile   = "DIALOG2";
		_boxProps.scrollUpDownFile = "DIALOG1";

		_boxProps.textAreaWidth  = 512;
		_boxProps.textAreaHeight =  50;
		_boxProps.textHeight     =  14;
		_boxProps.textMargin     =  90;

		_boxProps.numLines = 3;
		break;

	case kGameVersionSaturn:
		_boxProps.width =  320;
		_boxProps.height =  48;

		_boxProps.frameFile = 0;

		_boxProps.frameTopFile    = "DLG_TOP";
		_boxProps.frameBottomFile = "DLG_BTM";
		_boxProps.frameLeftFile   = "DLG_L";
		_boxProps.frameRightFile  = "DLG_R";

		_boxProps.frameLeftRightWidth = 40;
		_boxProps.frameTopDownHeight  =  7;

		_boxProps.scrollUp[0]   = 15; _boxProps.scrollUp[1]   = 24;
		_boxProps.scrollUp[2]   = 34; _boxProps.scrollUp[3]   = 40;
		_boxProps.scrollDown[0] = 15; _boxProps.scrollDown[1] = 41;
		_boxProps.scrollDown[2] = 34; _boxProps.scrollDown[3] = 57;

		_boxProps.scrollUpFile     = "TEXT_U";
		_boxProps.scrollDownFile   = "TEXT_D";
		_boxProps.scrollUpDownFile = "TEXT_UD";

		_boxProps.textAreaWidth  = 320;
		_boxProps.textAreaHeight =  34;
		_boxProps.textHeight     =  12;
		_boxProps.textMargin     =  50;

		_boxProps.numLines = 2;
		break;

	default:
		error("Unknown game version");
	}
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
	ImageType boxImageType = _resources->getVersionFormats().getBoxImageType();

	if (boxImageType == kImageType256) {
		bool loaded0, loaded1, loaded2, loaded3, loaded4, loaded5, loaded6;

		Palette palette;

		if (!palette.loadFromPAL555(*_resources, "MENU"))
			error("Failed to load MENU.PAL");

		ImgConv.registerStandardPalette(palette);

		_sprites[2].create(_boxProps.width, _boxProps.height);

		Sprite boxPart;

		loaded0 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameLeftFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);

		_sprites[2].blit(boxPart, 0, 0);

		loaded1 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameRightFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);

		_sprites[2].blit(boxPart, _boxProps.width - _boxProps.frameLeftRightWidth, 0);

		loaded2 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameTopFile,
				_boxProps.width - (2 * _boxProps.frameLeftRightWidth), _boxProps.frameTopDownHeight);

		_sprites[2].blit(boxPart, _boxProps.frameLeftRightWidth, 0);

		loaded3 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameBottomFile,
				_boxProps.width - (2 * _boxProps.frameLeftRightWidth), _boxProps.frameTopDownHeight);

		_sprites[2].blit(boxPart, _boxProps.frameLeftRightWidth,
				_boxProps.height - _boxProps.frameTopDownHeight);

		assert(loaded0 && loaded1 && loaded2 && loaded3);

		loaded4 = _sprites[3].loadFromBoxImage(*_resources, _boxProps.scrollUpDownFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);
		loaded5 = _sprites[4].loadFromBoxImage(*_resources, _boxProps.scrollDownFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);
		loaded6 = _sprites[5].loadFromBoxImage(*_resources, _boxProps.scrollUpFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);

		assert(loaded4 && loaded5 && loaded6);

		ImgConv.unregisterStandardPalette();

	} else {
		bool loaded0, loaded1, loaded2, loaded3;

		loaded0 = _sprites[2].loadFromImage(*_resources, _boxProps.frameFile);
		loaded1 = _sprites[3].loadFromImage(*_resources, _boxProps.scrollUpDownFile);
		loaded2 = _sprites[4].loadFromImage(*_resources, _boxProps.scrollDownFile);
		loaded3 = _sprites[5].loadFromImage(*_resources, _boxProps.scrollUpFile);

		assert(loaded0 && loaded1 && loaded2 && loaded3);
	}
}

void ConversationBox::build() {
	// The shading grid
	_sprites[1].create(_boxProps.textAreaWidth, _boxProps.textAreaHeight);
	_sprites[1].shade(_colorShading);

	_sprites[0].create(_boxProps.width, _boxProps.height);
	_box.create(_boxProps.width, _boxProps.height);

	_markerSelect   = new TextObject(">", _boxProps.textMargin - 9, 0, _colorSelected);
	_markerUnselect = new TextObject("-", _boxProps.textMargin - 8, 0, _colorUnselected);

	// Put the shading grid
	_sprites[0].blit(_sprites[1], (_boxProps.width  - _boxProps.textAreaWidth ) / 2,
	                              (_boxProps.height - _boxProps.textAreaHeight) / 2, true);
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
		for (uint32 i = 0; i < _boxProps.numLines; i++) {
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
	for (uint32 i = 0; i < _boxProps.numLines; i++)
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
	return _physLineCount > _boxProps.numLines;
}

bool ConversationBox::canScrollUp() const {
	return _physLineTop > 0;
}

bool ConversationBox::canScrollDown() const {
	return (_physLineTop + _boxProps.numLines) < _physLineCount;
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
