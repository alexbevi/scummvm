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
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

ConversationBox::Line::Line(TalkLine *line, const FontManager *fontManager,
		const Common::Array<uint32> *colors, int32 maxWidth) {

	talk = line;
	if (talk && colors && fontManager) {

		if (talk->hasTXT()) {
			int32 width = TextObject::wrap(TextLine(talk->getTXT()), *fontManager, texts, maxWidth);

			for (FontManager::TextList::iterator it = texts.begin(); it != texts.end(); ++it) {
				Common::Array <TextObject *> textLine;
				for (Common::Array<uint32>::const_iterator c = colors->begin(); c != colors->end(); ++c)
					textLine.push_back(new TextObject(*it, *fontManager, 0, 0, *c, width));

				textObjects.push_back(textLine);
			}
		}

	}

}

ConversationBox::Line::~Line() {
	Common::Array< Common::Array<TextObject *> >::iterator line;
	Common::Array<TextObject *>::iterator text;
	for (line = textObjects.begin(); line != textObjects.end(); ++line)
		for (text = line->begin(); text != line->end(); ++text)
			delete *text;

	delete talk;
}

const Common::String &ConversationBox::Line::getName() const {
	return talk->getName();
}


const Common::String &ConversationBox::PhysLineRef::getName() const {
	return (*itLine)->getName();
}

const Common::Array<TextObject *> &ConversationBox::PhysLineRef::getText() const {
	return *itText;
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
		Graphics &graphics, TalkManager &talkManager, const FontManager &fontManager) {

	_resources = &resources;
	_variables = &variables;
	_graphics  = &graphics;
	_talkMan   = &talkManager;

	_fontMan = &fontManager;

	_inited = false;

	_conversation = new Conversation(*_variables);

	_physLineCount = 0;
	_physLineTop   = 0;

	_selected = 0;

	_state = kStateWaitUserAction;
	_curReply = 0xFFFF;

	_box = 0;
}

ConversationBox::~ConversationBox() {
	clearLines();

	delete _box;

	delete _conversation;
}

bool ConversationBox::init() {
	_area = Common::Rect(getWidth(), getHeight());

	if (!loadSprites())
		return false;

	build();
	redrawLines();

	_inited = true;

	return true;
}

bool ConversationBox::start(const Common::String &conversation) {
	if (!_inited)
		return false;

	debugC(-1, kDebugConversation, "Starting conversation \"%s\"", conversation.c_str());

	if (!_conversation->parse(*_resources, conversation))
		return false;

	updateLines();
	drawLines();

	return true;
}

bool ConversationBox::restart() {
	if (!_inited)
		return false;

	if (!_conversation)
		return false;

	debugC(-1, kDebugConversation, "Restarting conversation");

	_conversation->reset();

	updateLines();
	drawLines();

	return true;
}

void ConversationBox::stop() {
	if (!_inited)
		return;

	if (!_conversation)
		return;

	_conversation->clear();
	_state = kStateWaitUserAction;
}

bool ConversationBox::isActive() const {
	if (!_inited)
		return false;

	if (_state == kStateWaitEndTalk)
		return true;

	if (!_conversation)
		return false;

	if (_state != kStateWaitUserAction)
		return true;

	return !_conversation->hasEnded();
}

void ConversationBox::move(int32 x, int32 y) {
	if (!_inited)
		return;

	// Sanity checks
	assert((ABS(x) <= 0x7FFF) && (ABS(y) <= 0x7FFF));

	_area.moveTo(x, y);
}

void ConversationBox::redraw(Sprite &sprite, Common::Rect area) {
	if (!_inited)
		return;

	if (!_area.intersects(area))
		return;

	area.clip(_area);

	int32 x = area.left;
	int32 y = area.top;

	area.moveTo(area.left - _area.left, area.top - _area.top);

	sprite.blit(*_box, area, x, y, true);
}

bool ConversationBox::saveLoad(Common::Serializer &serializer, Resources &resources) {
	return false;
}

bool ConversationBox::loading(Resources &resources) {
	return false;
}

void ConversationBox::clearLines() {
	if (!_inited)
		return;

	clearReplies();

	for (Common::Array<Line *>::iterator it = _lines.begin(); it != _lines.end(); ++it)
		delete *it;
	_lines.clear();

	_physLineCount = 0;
	_physLineTop   = 0;

	_state = kStateWaitUserAction;
}

void ConversationBox::clearReplies() {
	if (!_inited)
		return;

	for (Common::Array<TalkLine *>::iterator it = _nextReplies.begin(); it != _nextReplies.end(); ++it)
		delete *it;
	_nextReplies.clear();
	_curReply = 0xFFFF;

	_curLineNumber = 0;
	_curReplyName.clear();
}

uint32 ConversationBox::physLineNumToRealLineNum(uint32 physLineNum) const {
	if (!_inited)
		return 0;

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

bool ConversationBox::findPhysLine(uint32 n, PhysLineRef &ref) const {
	if (!_inited)
		return false;

	// Put the first level iterator at the beginning
	ref.itLine = _lines.begin();
	if (ref.itLine == _lines.end())
		return false;

	// Put the second level iterators at the beginning
	ref.itString = (*ref.itLine)->texts.begin();
	ref.itText   = (*ref.itLine)->textObjects.begin();
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
	if (!_inited)
		return false;

	// Advance second level iterators
	++ref.itString;
	++ref.itText;

	// Find the next non-empty line
	return nextPhysRealLine(ref);
}

bool ConversationBox::nextPhysRealLine(PhysLineRef &ref) const {
	if (!_inited)
		return false;

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
		ref.itText   = (*ref.itLine)->textObjects.begin();
	}

	return true;
}

ConversationBox::Line *ConversationBox::getSelectedLine() {
	if (!_inited)
		return 0;

	if (_selected == 0)
		return 0;

	PhysLineRef line;
	if (!findPhysLine(_selected - 1, line))
		return 0;

	return line.getLine();
}

void ConversationBox::speakLine(TalkLine &line) {
	if (!_inited)
		return;

	_talkMan->talk(line);
}

void ConversationBox::speakerVariable(uint8 speaker, bool on) {
	if (!_inited)
		return;

	Common::String speakerVar = Common::String::printf("SysTalking%d", speaker);

	_variables->set(speakerVar, on ? 1 : 0);
}

} // End of namespace DarkSeed2
