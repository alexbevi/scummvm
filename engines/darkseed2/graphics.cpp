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

#include "common/stream.h"

#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/room.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

Graphics::Graphics(Resources &resources, Variables &variables) {
	_resources = &resources;
	_variables = &variables;

	clearPalette();

	_screen.create(_screenWidth, _screenHeight);

	_dirtyAll = false;

	_background = 0;

	_talk = 0;

	_movieMode = false;

	_conversationBox = 0;
	_room            = 0;
}

Graphics::~Graphics() {
	delete _room;
	delete _conversationBox;
	delete _talk;
}

void Graphics::init(TalkManager &talkManager) {
	_conversationBox = new ConversationBox(*_resources, *_variables, *this, talkManager);
	_room            = new Room(*_variables, *this);

	_screen.clear();

	initPalette();
	dirtyAll();
}

ConversationBox &Graphics::getConversationBox() {
	assert(_conversationBox);

	return *_conversationBox;
}

Room &Graphics::getRoom() {
	assert(_room);

	return *_room;
}

void Graphics::updateStatus() {
	assert(_conversationBox);

	_conversationBox->updateStatus();
}

void Graphics::clearPalette() {
	_gamePalette.clear();
	applyGamePalette();
}

void Graphics::setPalette(const Palette &pal) {
	_gamePalette = pal;

	initPalette();
}

void Graphics::enterMovieMode() {
	_movieMode = true;

	_screen.clear();

	g_system->fillScreen(0);
	redraw(kScreenPartPlayArea);
	g_system->updateScreen();
}

void Graphics::leaveMovieMode() {
	_movieMode = false;

	applyGamePalette();
	redraw(kScreenPartPlayArea);
}

void Graphics::assertPalette0() {
	byte index0[4];

	g_system->grabPalette(index0, 0, 1);

	if ((index0[0] == 0) && (index0[1] == 0) && (index0[2] == 0))
		return;

	index0[0] = 0;
	index0[1] = 0;
	index0[2] = 0;

	g_system->setPalette(index0, 0, 1);
	g_system->updateScreen();
}

void Graphics::initPalette() {
	// Palette entry 0 is transparent
	_gamePalette[0] = 0;
	_gamePalette[1] = 0;
	_gamePalette[2] = 0;

	// Some standard colors
	_gamePalette.addColor(255, 255, 255, true);
	_gamePalette.addColor(  0,   0,   0, true);
	_gamePalette.addColor(239, 167, 127, true);

	applyGamePalette();
}

void Graphics::applyGamePalette() {
	byte pal[1024];

	_gamePalette.makeSystemCompatible(pal);

	g_system->setPalette(pal, 0, 256);
}

Common::Rect Graphics::talkEndTalk() {
	if (!_talk)
		return Common::Rect();

	Common::Rect talkArea = _talk->getArea();

	delete _talk;
	_talk = 0;

	return talkArea;
}

void Graphics::talk(TextObject *talkObject) {
	Common::List<Common::Rect> areas;

	Common::Rect oldTalkArea = talkEndTalk();

	if (!oldTalkArea.isEmpty())
		areas.push_back(oldTalkArea);

	_talk = talkObject;
	areas.push_back(_talk->getArea());

	redrawScreen(areas);
}

void Graphics::talkEnd() {
	redrawScreen(talkEndTalk());
}

void Graphics::blitToScreen(const Sprite &from, Common::Rect area,
		uint32 x, uint32 y, bool transp) {

	_screen.blit(from, area, x, y, transp);

	area.moveTo(x, y);
	dirtyRectsAdd(area);
}

void Graphics::blitToScreen(const Sprite &from, uint32 x, uint32 y, bool transp) {
	_screen.blit(from, x, y, transp);

	Common::Rect area = from.getArea();

	area.moveTo(x, y);
	dirtyRectsAdd(area);
}

void Graphics::blitToScreenDouble(const Sprite &from, Common::Rect area,
		uint32 x, uint32 y, bool transp) {

	_screen.blitDouble(from, area, x, y, transp);

	area.moveTo(x, y);
	area.setWidth (area.width () * 2);
	area.setHeight(area.height() * 2);

	dirtyRectsAdd(area);
}

void Graphics::blitToScreenDouble(const Sprite &from, uint32 x, uint32 y, bool transp) {
	_screen.blitDouble(from, x, y, transp);

	Common::Rect area = from.getArea();

	area.moveTo(x, y);
	area.setWidth (area.width () * 2);
	area.setHeight(area.height() * 2);

	dirtyRectsAdd(area);
}

void Graphics::mergePalette(Sprite &from) {
	debugC(2, kDebugGraphics, "Merging palettes");

	Common::Array<byte> changeSet = _gamePalette.merge(from.getPalette(), true);

	from.applyChangeSet(changeSet);
	applyGamePalette();

	dirtyAll();
}

const Palette &Graphics::getPalette() const {
	return _gamePalette;
}

void Graphics::redraw(ScreenPart part) {
	Common::Rect rect;

	switch (part) {
	case kScreenPartPlayArea:
		rect = Common::Rect(0, 0, _screenWidth, _screenHeight);
		break;

	case kScreenPartConversation:
		rect = Common::Rect(0, 0, ConversationBox::_width, ConversationBox::_height);
		break;

	default:
		return;
	}

	redraw(part, rect);
}

void Graphics::redraw(ScreenPart part, Common::Rect &rect) {
	if (rect.isEmpty())
		return;

	Common::List<Common::Rect> rects;

	rects.push_back(rect);

	redraw(part, rects);
}

void Graphics::redraw(ScreenPart part, Common::List<Common::Rect> &rects) {
	debugC(3, kDebugGraphics, "Redraw part %d", part);

	switch (part) {
	case kScreenPartPlayArea:
		redrawScreen(rects);
		break;

	case kScreenPartConversation:
		for (Common::List<Common::Rect>::iterator it = rects.begin(); it != rects.end(); ++it)
			it->moveTo(_conversationX, _conversationY);

		redrawScreen(rects);
		break;
	}
}

void Graphics::retrace() {
	if (dirtyRectsApply())
		g_system->updateScreen();
}

void Graphics::dirtyAll() {
	_dirtyAll = true;
	_dirtyRects.clear();
}

void Graphics::dirtyRectsAdd(const Common::Rect &rect) {
	if (_dirtyAll)
		return;

	if ((rect.left == 0) && (rect.top == 0) &&
	    (rect.right >= ((int) _screenWidth)) && (rect.bottom >= ((int) _screenHeight))) {

		dirtyAll();
		return;
	}

	// Upper limit. Dirty rectangle overhead isn't that great either :P
	if (_dirtyRects.size() >= 30) {
		dirtyAll();
		return;
	}

	_dirtyRects.push_back(rect);
}

void Graphics::dirtyRectsAdd(const Common::List<Common::Rect> &rects) {
	for (Common::List<Common::Rect>::const_iterator it = rects.begin(); it != rects.end(); ++it)
		dirtyRectsAdd(*it);
}

bool Graphics::dirtyRectsApply() {
	if (_dirtyAll) {
		debugC(5, kDebugGraphics, "Refreshing the whole screen");
		// Everything is dirty, copy the whole screen

		g_system->copyRectToScreen(_screen.getData(), _screen.getWidth(),
					0, 0, _screen.getWidth(), _screen.getHeight());
		_dirtyAll = false;
		return true;
	}

	if (_dirtyRects.empty())
		return false;

	debugC(5, kDebugGraphics, "Refreshing %d rectangle(s)", _dirtyRects.size());

	int screenWidth  = _screen.getWidth();
	int screenHeight = _screen.getHeight();

	Common::Rect screenArea(screenWidth, screenHeight);

	Common::List<Common::Rect>::iterator it;
	for (it = _dirtyRects.begin(); it != _dirtyRects.end(); ++it) {
		it->clip(screenArea);

		if (it->isEmpty())
			continue;

		const byte *data = _screen.getData() + it->top * screenWidth + it->left;

		g_system->copyRectToScreen(data, screenWidth, it->left, it->top, it->width(), it->height());
	}

	_dirtyRects.clear();
	return true;
}

void Graphics::registerBackground(const Sprite &background) {
	debugC(-1, kDebugGraphics, "New background");

	assert(_conversationBox);

	_background = &background;

	setPalette(_background->getPalette());

	_conversationBox->newPalette();

	redrawScreen(_background->getArea());
}

void Graphics::unregisterBackground() {
	_background = 0;
}

void Graphics::redrawScreen(const Common::Rect &rect) {
	if (rect.isEmpty())
		return;

	Common::List<Common::Rect> rects;

	rects.push_back(rect);

	redrawScreen(rects);
}

void Graphics::redrawScreen(const Common::List<Common::Rect> &rects) {
	assert(_conversationBox);

	if (_movieMode) {
		dirtyRectsAdd(rects);
		return;
	}

	if (_background)
		for (Common::List<Common::Rect>::const_iterator it = rects.begin(); it != rects.end(); ++it)
			blitToScreen(*_background, *it, it->left, it->top, false);

	if (_talk)
		for (Common::List<Common::Rect>::const_iterator it = rects.begin(); it != rects.end(); ++it)
			_talk->redraw(_screen, *it);

	if (_conversationBox->isActive())
		for (Common::List<Common::Rect>::const_iterator it = rects.begin(); it != rects.end(); ++it)
			_conversationBox->redraw(_screen, _conversationX, _conversationY, *it);

	dirtyRectsAdd(rects);
}

} // End of namespace DarkSeed2
