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
#include "engines/darkseed2/conversation.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/graphicalobject.h"
#include "engines/darkseed2/talk.h"

namespace DarkSeed2 {

// Size
static const int32 kWidth  = 640;
static const int32 kHeight =  70;

// Sprites
static const char *kFileFrame        = "INVNTRY1";
static const char *kFileScrollUpDown = "DIALOG1";
static const char *kFileScrollDown   = "DIALOG2";
static const char *kFileScrollUp     = "DIALOG3";

// Scroll button coordinates
static const int32 kScrollUp  [4] = {  15,  24,  34,  40};
static const int32 kScrollDown[4] = {  15,  41,  34,  57};

// Area coordinates
static const int32 kTextAreaWidth  = 512;
static const int32 kTextAreaHeight =  50;
static const int32 kTextHeight     =  14;
static const int32 kTextMargin     =  90;
static const int32 kTextLineWidth  = 460;

static const uint32 kNumLines = 3;

// Colors
static const byte kColorSelected  [3] = {255, 255, 255};
static const byte kColorUnselected[3] = {239, 167, 127};
static const byte kColorShading   [3] = {  0,   0,   0};

ConversationBoxWindows::ConversationBoxWindows(Resources &resources, Variables &variables,
		Graphics &graphics, TalkManager &talkManager, const FontManager &fontManager) :
		ConversationBox(resources, variables, graphics, talkManager, fontManager) {

	_sprites = 0;
}

ConversationBoxWindows::~ConversationBoxWindows() {
	delete[] _sprites;

	delete[] _textAreas;

	for (Common::Array<TextObject *>::iterator marker = _marker.begin(); marker != _marker.end(); ++marker)
		delete *marker;
}

int32 ConversationBoxWindows::getWidth() const {
	return kWidth;
}

int32 ConversationBoxWindows::getHeight() const {
	return kHeight;
}

void ConversationBoxWindows::notifyMouseMove(int32 x, int32 y) {
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

void ConversationBoxWindows::notifyClicked(int32 x, int32 y) {
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

void ConversationBoxWindows::updateStatus() {
	if (!_inited)
		return;

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

bool ConversationBoxWindows::loadSprites() {
	_sprites = new Sprite[6];

	if (!_sprites[2].loadFromImage(*_resources, kFileFrame))
		return false;
	if (!_sprites[3].loadFromImage(*_resources, kFileScrollUpDown))
		return false;
	if (!_sprites[4].loadFromImage(*_resources, kFileScrollDown))
		return false;
	if (!_sprites[5].loadFromImage(*_resources, kFileScrollUp))
		return false;

	return true;
}

void ConversationBoxWindows::build() {
	_colorShading =      ImgConv.getColor(kColorShading   [0], kColorShading   [1], kColorShading   [2]);
	_colorText.push_back(ImgConv.getColor(kColorSelected  [0], kColorSelected  [1], kColorSelected  [2]));
	_colorText.push_back(ImgConv.getColor(kColorUnselected[0], kColorUnselected[1], kColorUnselected[2]));

	_textAreas = new Common::Rect[kNumLines];
	for (uint32 i = 0; i < kNumLines; i++)
		_textAreas[i] = Common::Rect(kTextMargin             , kTextHeight * (i + 1),
		                             kWidth - 2 * kTextMargin, kTextHeight * (i + 2));

	_scrollAreas[0] = Common::Rect(kScrollUp  [0], kScrollUp  [1],
	                               kScrollUp  [2], kScrollUp  [3]);
	_scrollAreas[1] = Common::Rect(kScrollDown[0], kScrollDown[1],
	                               kScrollDown[2], kScrollDown[3]);

	_box = new Sprite;
	_box->create(kWidth, kHeight);

	// The shading grid
	_sprites[1].create(kWidth, kHeight);
	_sprites[1].shade(_colorShading);

	_sprites[0].create(kWidth, kHeight);

	for (Common::Array<uint32>::const_iterator color = _colorText.begin(); color != _colorText.end(); ++color)
		_marker.push_back(new TextObject(TextLine(">"), *_fontMan, kTextMargin - 9, 0, *color));

	// Put the shading grid
	_sprites[0].blit(_sprites[1], (kWidth  - kTextAreaWidth)  / 2,
	                              (kHeight - kTextAreaHeight) / 2, true);
	// Put the frame
	_sprites[0].blit(_sprites[2], 0, 0, true);

	_box->blit(_sprites[0], 0, 0, false);
}

void ConversationBoxWindows::updateLines() {
	clearLines();

	if (_conversation->hasEnded())
		return;

	Common::Array<TalkLine *> lines = _conversation->getCurrentLines(*_resources);
	for (Common::Array<TalkLine *>::iterator it = lines.begin(); it != lines.end(); ++it) {
		Line *line = new Line(*it, _fontMan, &_colorText, kTextLineWidth);

		line->lineNumber = _lines.size();

		_lines.push_back(line);
		_physLineCount += line->texts.size();
	}
}

void ConversationBoxWindows::updateScroll() {
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

	_box->blit(_sprites[0], 0, 0, false);
}

void ConversationBoxWindows::drawLines() {
	// Update the scroll sprite
	updateScroll();

	PhysLineRef curLine;

	// Update the lines
	if (findPhysLine(_physLineTop, curLine)) {
		for (uint32 i = 0; i < kNumLines; i++) {
			// Selected line
			uint32 selected = physLineNumToRealLineNum(_selected);

			// Current line a selected line?
			int partNumber = ((curLine.getLineNum() + 1) == selected) ? 0 : 1;

			// Line's text object
			const Common::Array<TextObject *> &textLine = curLine.getText();
			TextObject *text = textLine[partNumber];

			// Move the line to the correct place and draw it
			text->moveTo(_textAreas[i].left, _textAreas[i].top);
			text->redraw(*_box, text->getArea());

			// If that line is a top line, place the correct marker
			if (curLine.isTop()) {
				TextObject *marker = _marker[partNumber];

				marker->moveTo(marker->getArea().left, text->getArea().top);
				marker->redraw(*_box, marker->getArea());
			}

			if (!nextPhysLine(curLine))
				// No next line, stop
				break;
		}
	}

	_graphics->requestRedraw(_area);
}

void ConversationBoxWindows::redrawLines() {
	_box->blit(_sprites[0], 0, 0, false);

	drawLines();
}

bool ConversationBoxWindows::canScroll() const {
	return _physLineCount > kNumLines;
}

bool ConversationBoxWindows::canScrollUp() const {
	return _physLineTop > 0;
}

bool ConversationBoxWindows::canScrollDown() const {
	return (_physLineTop + kNumLines) < _physLineCount;
}

int ConversationBoxWindows::getTextArea(int32 x, int32 y) {
	for (uint32 i = 0; i < kNumLines; i++)
		if (_textAreas[i].contains(x, y))
			return _physLineTop + i + 1;

	return 0;
}

ConversationBoxWindows::ScrollAction ConversationBoxWindows::getScrollAction(int32 x, int32 y) {
	for (int i = 0; i < 2; i++)
		if (_scrollAreas[i].contains(x, y))
			return (ScrollAction) i;

	return kScrollActionNone;
}

void ConversationBoxWindows::doScroll(ScrollAction scroll) {
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

void ConversationBoxWindows::pickLine(Line *line) {
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

} // End of namespace DarkSeed2
