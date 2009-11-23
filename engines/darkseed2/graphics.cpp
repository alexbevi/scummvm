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
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

Graphics::Graphics() {
	clearPalette();

	_screen.create(_screenWidth, _screenHeight);

	_dirtyAll = false;

	_background = 0;

	_talk = 0;
}

Graphics::~Graphics() {
	delete _talk;
}

void Graphics::clearPalette() {
	_gamePalette.clear();
	applyGamePalette();
}

void Graphics::setPalette(const Palette &pal) {
	_gamePalette = pal;

	// Palette entry 0 is transparent
	_gamePalette[0] = 0;
	_gamePalette[1] = 0;
	_gamePalette[2] = 0;

	applyGamePalette();
}

void Graphics::applyGamePalette() {
	byte pal[1024];

	_gamePalette.makeSystemCompatible(pal);

	g_system->setPalette(pal, 0, 256);
}

void Graphics::talk(TextObject *talkObject) {
	talkEnd();

	_talk = talkObject;
	redrawScreen(_talk->getArea());
}

void Graphics::talkEnd() {
	if (!_talk)
		return;

	Common::Rect talkArea = _talk->getArea();

	delete _talk;
	_talk = 0;
	redrawScreen(talkArea);
}

void Graphics::blitToScreen(const Sprite &from,
		uint32 left, uint32 top, uint32 right, uint32 bottom,
		uint32 x, uint32 y, bool transp) {

	_screen.blit(from, left, top, right, bottom, x, y, transp);

	dirtyRectsAdd(x, y, x + (right - left), y + (bottom - top));
}

void Graphics::blitToScreen(const Sprite &from, uint32 x, uint32 y, bool transp) {
	_screen.blit(from, x, y, transp);

	dirtyRectsAdd(x, y, x + from.getWidth() - 1, y + from.getHeight() - 1);
}

void Graphics::mergePalette(Sprite &from) {
	Common::Array<byte> changeSet = _gamePalette.merge(from.getPalette());

	from.applyChangeSet(changeSet);
	applyGamePalette();

	dirtyAll();
}

const Palette &Graphics::getPalette() const {
	return _gamePalette;
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

	if (_dirtyRects.size() >= 30)
		dirtyAll();

	_dirtyRects.push_back(rect);
}

void Graphics::dirtyRectsAdd(uint32 left, uint32 top, uint32 right, uint32 bottom) {
	dirtyRectsAdd(Common::Rect(left, top, right + 1, bottom + 1));
}

bool Graphics::dirtyRectsApply() {
	if (_dirtyAll) {
			g_system->copyRectToScreen(_screen.getData(), _screen.getWidth(),
					0, 0, _screen.getWidth(), _screen.getHeight());
		_dirtyAll = false;
		return true;
	}

	if (_dirtyRects.empty())
		return false;

	int screenWidth  = _screen.getWidth();
	int screenHeight = _screen.getHeight();

	Common::List<Common::Rect>::const_iterator it;
	for (it = _dirtyRects.begin(); it != _dirtyRects.end(); ++it) {
		int left   = MAX<int>(0, it->left);
		int top    = MAX<int>(0, it->top);
		int right  = MIN<int>(screenWidth , it->right);
		int bottom = MIN<int>(screenHeight, it->bottom);
		int width  = right  - left;
		int height = bottom - top;

		if ((width <= 0) || (height <= 0))
			continue;

		const byte *data = _screen.getData() + top * screenWidth + left;

		g_system->copyRectToScreen(data, screenWidth, left, top, width, height);
	}

	_dirtyRects.clear();
	return true;
}

void Graphics::registerBackground(Sprite &background) {
	_background = &background;

	redrawScreen(0, 0, _background->getWidth() - 1, _background->getHeight() - 1);
}

void Graphics::unregisterBackground() {
	_background = 0;
}

void Graphics::redrawScreen(const Common::Rect &rect) {
	redrawScreen(rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}

void Graphics::redrawScreen(uint32 left, uint32 top, uint32 right, uint32 bottom) {
	blitToScreen(*_background, left, top, right, bottom, left, top, false);

	if (_talk)
		_talk->redraw(_screen, Common::Rect(left, top, right + 1, bottom + 1));

	dirtyRectsAdd(left, top, right, bottom);
}

} // End of namespace DarkSeed2
