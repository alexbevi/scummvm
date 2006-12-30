/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include <memory.h>
#include "common/stdafx.h"
#include "common/system.h"
#include "common/file.h"
#include "common/fs.h"
#include "common/endian.h"
#include "graphics/cursorman.h"

#include "kom/screen.h"
#include "kom/kom.h"
#include "kom/actor.h"

using Common::File;

namespace Kom {

Screen::Screen(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm), _roomBackground(0) {

	_screenBuf = new uint8[SCREEN_W * SCREEN_H];
	memset(_screenBuf, 0, SCREEN_W * SCREEN_H);

	_mouseBuf = new uint8[MOUSE_W * MOUSE_H];
	memset(_mouseBuf, 0, MOUSE_W * MOUSE_H);

	_c0ColorSet = loadColorSet(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("c0_127.cl"));

	_mask = new uint8[SCREEN_W * (SCREEN_H - PANEL_H)];
	memset(_mask, 0, SCREEN_W * (SCREEN_H - PANEL_H));

	_font = new Font(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("packfont.fnt"));
}

Screen::~Screen() {
	delete[] _screenBuf;
	delete[] _mouseBuf;
	delete[] _c0ColorSet;
	delete[] _mask;
	delete _roomBackground;
	delete _font;
}

bool Screen::init() {
	_system->beginGFXTransaction();
		_vm->initCommonGFX(false);
		_system->initSize(SCREEN_W, SCREEN_H);
	_system->endGFXTransaction();

	_system->setPalette(_c0ColorSet, 0, 128);

	return true;
}

void Screen::update() {
	memset(_screenBuf, 0, SCREEN_W * SCREEN_H);

	updateCursor();
	displayMouse();
	_vm->panel()->update();
	updateBackground();
	drawBackground();
	_vm->actorMan()->displayAll();
	_system->copyRectToScreen(_screenBuf, SCREEN_W, 0, 0, SCREEN_W, SCREEN_H);
	_system->updateScreen();
}

void Screen::drawActorFrame(const int8 *data, uint16 width, uint16 height, uint16 xPos, uint16 yPos,
                            int16 xOffset, int16 yOffset, int maskDepth) {

	// Check which lines and colums to draw
	int16 realX = xPos + xOffset;
	int16 realY = yPos + yOffset;

	uint16 startCol = (realX < 0 ? -realX : 0);
	uint16 startLine = (realY < 0 ? -realY : 0);
	uint16 endCol = (realX + width > SCREEN_W ? SCREEN_W - realX : width);
	uint16 endLine = (realY + height - 1 > SCREEN_H ? SCREEN_H - realY : height - 1);

	for (int line = startLine; line <= endLine; ++line) {
		uint16 lineOffset = READ_LE_UINT16(data + line * 2);

		drawActorFrameLine(_screenBuf, SCREEN_W, data + lineOffset, (realX < 0 ? 0 : realX),
		                   (realY < 0 ? 0 : realY) + line - startLine, startCol, endCol, maskDepth);
	}
}

void Screen::drawMouseFrame(const int8 *data, uint16 width, uint16 height, int16 xOffset, int16 yOffset) {

	memset(_mouseBuf, 0, MOUSE_W * MOUSE_H);

	for (int line = 0; line <= height - 1; ++line) {
		uint16 lineOffset = READ_LE_UINT16(data + line * 2);

		drawActorFrameLine(_mouseBuf, MOUSE_W, data + lineOffset, 0, line, 0, width, 0);
	}

	setMouseCursor(_mouseBuf, MOUSE_W, MOUSE_H, -xOffset, -yOffset);
}

void Screen::drawActorFrameLine(uint8 *buf, uint16 bufWidth, const int8 *data,
                                uint16 xPos, uint16 yPos, uint16 startPixel, uint16 endPixel,
                                int maskDepth) {
	uint16 dataIndex = 0;
	uint16 pixelsDrawn = 0;
	uint16 pixelsParsed = 0;
	int8 imageData;
	bool shouldDraw = false;

	if (startPixel == 0)
		shouldDraw = true;

	while (pixelsDrawn < endPixel - startPixel) {

		imageData = data[dataIndex];
		dataIndex++;

		// Handle transparent pixels
		if (imageData > 0) {

			// Skip pixels
			if (!shouldDraw && pixelsParsed < startPixel && pixelsParsed + imageData >= startPixel) {
				uint8 pixelsToSkip = startPixel - pixelsParsed;

				pixelsParsed += imageData;
				imageData -= pixelsToSkip;
				shouldDraw = true;

			// Not at start pos yet
			} else {
				pixelsParsed += imageData;
			}

			if (shouldDraw) {
				pixelsDrawn += imageData;
			}

		// Handle visible pixels
		} else {

			imageData &= 0x7F;

			// Skip pixels
			if (!shouldDraw && pixelsParsed < startPixel && pixelsParsed + imageData >= startPixel) {
				uint8 pixelsToSkip = startPixel - pixelsParsed;

				pixelsParsed += imageData;
				imageData -= pixelsToSkip;
				dataIndex += pixelsToSkip;

				shouldDraw = true;

			// Not at start pos yet
			} else {
				pixelsParsed += imageData;
			}

			if (shouldDraw) {
				// Don't draw over the edge!
				if (pixelsDrawn + imageData > endPixel)
					imageData = endPixel - pixelsDrawn;

				for (int i = 0; i < imageData; ++i) {
					// Check with background mask
					if (yPos > SCREEN_H - PANEL_H ||
					    _mask[(yPos * bufWidth) + xPos + pixelsDrawn + i] >= maskDepth)

						buf[(yPos * bufWidth) + xPos + pixelsDrawn + i] =
							data[dataIndex + i];
				}

				dataIndex += imageData;
				pixelsDrawn += imageData;
			} else {
				dataIndex += imageData;
			}
		}
	}
}

byte *Screen::loadColorSet(FilesystemNode fsNode) {
	File f;

	f.open(fsNode);

	byte *pal = new byte[128 * 4];

	for (int i = 0; i < 128; ++i) {
		pal[4 * i + 0] = f.readByte();
		pal[4 * i + 1] = f.readByte();
		pal[4 * i + 2] = f.readByte();
		pal[4 * i + 3] = 0;
	}

	f.close();
	return pal;
}

void Screen::setMouseCursor(const byte *buf, uint w, uint h, int hotspotX, int hotspotY) {
	CursorMan.replaceCursor(buf, w, h, hotspotX, hotspotY, 0);
}

void Screen::showMouseCursor(bool show) {
	CursorMan.showMouse(show);
}

void Screen::displayMouse() {
	_vm->actorMan()->getMouse()->display();
}

void Screen::updateCursor() {
	Actor *mouse = _vm->actorMan()->getMouse();

	if (_vm->input()->getMouseY() >= SCREEN_H - PANEL_H) {
		mouse->switchScope(5, 2);
	} else {
		mouse->switchScope(0, 2);
	}

	// The scopes:
	// setScope(0, 2);
	// setScope(1, 2);
	// setScope(2, 2);
	// setScope(3, 2);
	// setScope(4, 2);
	// setScope(5, 2);
	// setScope(6, 0);
}

void Screen::drawPanel(const byte *panelData) {
	memcpy(_screenBuf + SCREEN_W * (SCREEN_H - PANEL_H), panelData, SCREEN_W * PANEL_H);
}

void Screen::updatePanelArea() {
	_system->copyRectToScreen(_screenBuf + SCREEN_W * (SCREEN_H - PANEL_H), SCREEN_W, 0, SCREEN_H - PANEL_H, SCREEN_W, PANEL_H);
	_system->updateScreen();
}

void Screen::loadBackground(FilesystemNode node) {
	delete _roomBackground;
	_roomBackground = new FlicPlayer(node);
	_roomBackgroundTime = _system->getMillis() + _roomBackground->speed();
}

void Screen::updateBackground() {
	if (_roomBackground != 0) {
		if (_system->getMillis() >= _roomBackgroundTime) {
			_roomBackgroundTime = _system->getMillis() + _roomBackground->speed();

			_roomBackground->decodeFrame();

			if (_roomBackground->paletteDirty()) {
				_system->setPalette(_roomBackground->getPalette(), 0, 1);
				_system->setPalette(_roomBackground->getPalette() + 4 * 128, 128, 128);
			}
		}
	}
}

void Screen::drawBackground() {
	if (_roomBackground != 0) {
		memcpy(_screenBuf, _roomBackground->getOffscreen(), SCREEN_W * (SCREEN_H - PANEL_H));
	}
}

void Screen::setMask(const uint8 *data) {
	memcpy(_mask, data, SCREEN_W * (SCREEN_H - PANEL_H));
}

uint16 Screen::getTextWidth(const char *text) {
	uint16 w = 0;
	for (int i = 0; text[i] != '\0'; ++i) {
		switch (text[i]) {
		case ' ':
			w += 4;
			break;
		case '\t':
			w += 12;
			break;
		default:
			w += *((const uint8 *)(_font->getCharData(text[i])));
		}
		++w;
	}
	if (w > 0) --w;

	return w;
}

void Screen::writeTextCentered(const char *text, uint8 row, uint8 color, bool isEmbossed) {
	uint8 col = (SCREEN_W - getTextWidth(text)) / 2;
	writeText(text, row, col, color, isEmbossed);
}
void Screen::writeText(const char *text, uint8 startRow, uint16 startCol, uint8 color, bool isEmbossed) {

	uint16 col = startCol;
	uint8 charWidth;
	const byte *data;

	for (int i = 0; text[i] != '\0'; ++i) {
		switch (text[i]) {
		case ' ':
			col += 4;
			break;
		case '\t':
			col += 12;
			break;
		default:
			data = _font->getCharData(text[i]);
			charWidth = (uint8)*data;
			++data;

			for (uint w = 0; w < charWidth; ++w) {
				for (uint8 h = 0; h < 8; ++h) {
					if (*data != 0)
						_screenBuf[SCREEN_W * (startRow + h) + col + w] = color;
					++data;
				}
			}
			col += charWidth;
		}

		col += 1;
	}
}

} // End of namespace Kom
