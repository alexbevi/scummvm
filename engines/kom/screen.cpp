/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004-2006 The ScummVM project
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

#include <memory.h>
#include <stdlib.h>

#include "common/system.h"
#include "common/file.h"
#include "common/fs.h"
#include "common/endian.h"
#include "common/list.h"
#include "common/rect.h"
#include "graphics/cursorman.h"

#include "kom/screen.h"
#include "kom/kom.h"
#include "kom/panel.h"
#include "kom/actor.h"
#include "kom/game.h"

using Common::File;
using Common::List;
using Common::Rect;

namespace Kom {

Screen::Screen(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm), _roomBackground(0) {

	_lastFrameTime = 0;

	_screenBuf = new uint8[SCREEN_W * SCREEN_H];
	memset(_screenBuf, 0, SCREEN_W * SCREEN_H);

	_mouseBuf = new uint8[MOUSE_W * MOUSE_H];
	memset(_mouseBuf, 0, MOUSE_W * MOUSE_H);

	_c0ColorSet = loadColorSet(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("c0_127.cl"));

	_mask = new uint8[SCREEN_W * (SCREEN_H - PANEL_H)];
	memset(_mask, 0, SCREEN_W * (SCREEN_H - PANEL_H));

	_font = new Font(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("packfont.fnt"));

	_dirtyRects = new List<Rect>();
	_prevDirtyRects = new List<Rect>();
}

Screen::~Screen() {
	delete[] _screenBuf;
	delete[] _mouseBuf;
	delete[] _c0ColorSet;
	delete[] _mask;
	delete _roomBackground;
	delete _font;
	delete _dirtyRects;
	delete _prevDirtyRects;
}

bool Screen::init() {
	_system->beginGFXTransaction();
		_vm->initCommonGFX(false);
		_system->initSize(SCREEN_W, SCREEN_H);
	_system->endGFXTransaction();

	_system->setPalette(_c0ColorSet, 0, 128);

	return true;
}

void Screen::processGraphics(int mode) {
	//memset(_screenBuf, 0, SCREEN_W * SCREEN_H);
	int scale;

	// handle screen objects
	if (mode > 0) {
	Common::Array<RoomObject> *roomObjects = _vm->game()->getObjects();

		for (uint i = 0; i < roomObjects->size(); i++) {
			Actor *act = _vm->actorMan()->get((*roomObjects)[i].actorId);
			Object *obj = _vm->database()->object((*roomObjects)[i].objectId);

			// TODO - handle disappear delay

			act->enable(obj->isVisible);
		}
	}

	// handle dust clouds
	
	// unload actors in other rooms
	for (int i = 1; i < _vm->database()->charactersNum(); ++i) {
		Character *chr = _vm->database()->getChar(i);

		if (_vm->database()->getChar(0)->_lastLocation != chr->_lastLocation ||
			!_vm->database()->getChar(i)->_isVisible) {

			if (chr->_loadedScopeXtend != -1) {
				chr->_spriteTimer = chr->_spriteCutState = 0;
				chr->_isBusy = false;

				if (chr->_actorId >= 0) {
					_vm->actorMan()->unload(chr->_actorId);
					chr->_actorId = chr->_loadedScopeXtend = chr->_scopeInUse = -1;
				}
			}
		}
	}

	// load actors in this room
	for (int i = 0; i < _vm->database()->charactersNum(); ++i) {
		Character *chr = _vm->database()->getChar(i);

		chr->_start5PrevPrev = chr->_start5Prev;
		chr->_start5Prev = chr->_start5;
		chr->_start4PrevPrev = chr->_start4Prev;
		chr->_start4Prev = chr->_start4;
		chr->_start3PrevPrev = chr->_start3Prev;
		chr->_start3Prev = chr->_start3;

		scale = (chr->_start5 * 88) / 60;

		// TODO - init scope stuff
		// TODO - disable actor if in fight
		if (_vm->database()->getChar(0)->_lastLocation == chr->_lastLocation &&
			chr->_isVisible) {

			chr->setScope(chr->_scopeWanted);

			// FIXME: this is here just for testing

			Actor *act = _vm->actorMan()->get(chr->_actorId);

			act->setPos(chr->_screenX / 2, (chr->_start4 + (chr->_screenH + chr->_offset78) / scale) / 256 / 2);
			act->setRatio(chr->_ratioX / scale, chr->_ratioY / scale);

			act->setMaskDepth(_vm->database()->getPriority( chr->_lastLocation, chr->_lastBox), chr->_start5);

			// FIXME end above
		}
	}

	updateCursor();
	displayMouse();
	updateBackground();
	drawBackground();
	displayDoors();
	_vm->actorMan()->displayAll();

	// Copy dirty rects to screen
	copyRectListToScreen(_prevDirtyRects);
	copyRectListToScreen(_dirtyRects);
	delete _prevDirtyRects;
	_prevDirtyRects = _dirtyRects;
	_dirtyRects = new List<Rect>();

	// No need to save a prev, since the background reports
	// all changes
	if (_roomBackground)
		copyRectListToScreen(_bgDirtyRects);

	if (_vm->panel()->isDirty())
		_vm->panel()->update();

	gfxUpdate();
}

void Screen::copyRectListToScreen(const Common::List<Common::Rect> *list) {

	for (Common::List<Rect>::const_iterator rect = list->begin(); rect != list->end(); ++rect) {
		debug(9, "copyRectToScreen(%hu, %hu, %hu, %hu)", rect->left, rect->top,
				rect->width(), rect->height());
		_system->copyRectToScreen(_screenBuf + SCREEN_W * rect->top + rect->left,
				SCREEN_W, rect->left, rect->top, rect->width(), rect->height());
	}
}

void Screen::gfxUpdate() {
	while (_system->getMillis() < _lastFrameTime + 41 /* 24 fps */) {
		_vm->input()->checkKeys();
		_system->delayMillis(10);
	}
	_system->updateScreen();
	_lastFrameTime = _system->getMillis();
}

static byte lineBuffer[SCREEN_W];

void Screen::drawActorFrame(const int8 *data, uint16 width, uint16 height, int16 xStart, int16 yStart,
                            uint16 xEnd, uint16 yEnd, int maskDepth) {

	uint16 startLine = 0;
	uint16 startCol = 0;

	int16 visibleWidth = xEnd - xStart;
	int16 scaledWidth = visibleWidth;
	int16 visibleHeight = yEnd - yStart;
	int16 scaledHeight = visibleHeight;
	int32 colSkip = 0;
	int32 rowSkip = 0;
	div_t d;

	//printf("drawActorFrame(%hu, %hu, %hd, %hd, %hu, %hu)\n", width, height, xStart, yStart, xEnd, yEnd);

	if (visibleWidth == 0 || visibleHeight == 0) return;

	if (xStart < 0) {
		// frame is entirely off-screen
		if ((visibleWidth += xStart) < 0)
			return;

		d = div(-xStart * width, scaledWidth);
		startCol = d.quot;
		colSkip = d.rem;
		//printf("startCol: %hu, colSkip: %d\n", startCol, colSkip);

		xStart = 0;
	}

	// frame is entirely off-screen
	if (xStart >= SCREEN_W) return;

	// check if frame spills over the edge
	if (visibleWidth + xStart >= SCREEN_W)
		if ((visibleWidth -= visibleWidth + xStart - SCREEN_W) <= 0)
			return;

	if (yStart < 0) {
		// frame is entirely off-screen
		if ((visibleHeight += yStart) < 0)
			return;

		d = div(-yStart * height, scaledHeight);
		startLine = d.quot;
		rowSkip = d.rem;
		//printf("startLine: %hu, rowSkip: %d\n", startLine, rowSkip);

		yStart = 0;
	}

	// frame is entirely off-screen
	if (yStart >= SCREEN_H) return;

	// check if frame spills over the edge
	if (visibleHeight + yStart >= SCREEN_H)
		if ((visibleHeight -= visibleHeight + yStart - SCREEN_H + 1) <= 0)
			return;

	uint8 sourceLine = startLine;
	uint8 targetLine = yStart;
	int16 rowThing = scaledHeight - rowSkip;

	for (int i = 0; i < visibleHeight; i += 1) {
		uint16 lineOffset = READ_LE_UINT16(data + sourceLine * 2);

		// FIXME: the original doesn't have this check, but the room mask doesn't
		// cover the panel area -- check
		if (sourceLine < SCREEN_H - PANEL_H) {
			uint16 targetPixel = targetLine * SCREEN_W + xStart;
			drawActorFrameLine(lineBuffer, data + lineOffset, width);

			// Copy line to screen
			uint8 sourcePixel = startCol;
			int16 colThing = scaledWidth - colSkip;

			for (int j = 0; j < visibleWidth; ++j) {
				if (lineBuffer[sourcePixel] != 0 && _mask[targetPixel] >= maskDepth)
					_screenBuf[targetPixel] = lineBuffer[sourcePixel];

				sourcePixel += width / scaledWidth;
				colThing -= width % scaledWidth;

				if (colThing < 0) {
					sourcePixel++;
					colThing += scaledWidth;
				}

				targetPixel++;
			}
		}

		sourceLine += height / scaledHeight;
		rowThing -= height % scaledHeight;

		if (rowThing < 0) {
			sourceLine++;
			rowThing += scaledHeight;
		}

		targetLine++;
	}

	_dirtyRects->push_back(Rect(xStart, yStart, xStart + visibleWidth, yStart + visibleHeight));
}

void Screen::drawMouseFrame(const int8 *data, uint16 width, uint16 height, int16 xOffset, int16 yOffset) {

	memset(_mouseBuf, 0, MOUSE_W * MOUSE_H);

	for (int line = 0; line <= height - 1; ++line) {
		uint16 lineOffset = READ_LE_UINT16(data + line * 2);

		drawActorFrameLine(lineBuffer, data + lineOffset, width);
		for (int i = 0; i < width; ++i)
			if (lineBuffer[i] != 0)
				_mouseBuf[line * MOUSE_W + i] = lineBuffer[i];
	}

	setMouseCursor(_mouseBuf, MOUSE_W, MOUSE_H, -xOffset, -yOffset);
}

void Screen::drawActorFrameLine(byte *outBuffer, const int8 *rowData, uint16 length) {
	uint8 dataIndex = 0;
	uint8 outIndex = 0;

	while (outIndex < length) {
		// FIXME: valgrind reports invalid read of size 1
		int8 imageData = rowData[dataIndex++];
		if (imageData > 0)
			while (imageData-- > 0)
				outBuffer[outIndex++] = 0;
		else if ((imageData &= 0x7F) > 0)
			while (imageData-- > 0)
				// FIXME: valgrind reports invalid read of size 1
				outBuffer[outIndex++] = rowData[dataIndex++];
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

void Screen::refreshPanelArea() {
	_system->copyRectToScreen(_screenBuf + SCREEN_W * (SCREEN_H - PANEL_H),
		SCREEN_W, 0, SCREEN_H - PANEL_H, SCREEN_W, PANEL_H);
	_system->updateScreen();
}

void Screen::copyPanelToScreen(const byte *data) {
	memcpy(_screenBuf + SCREEN_W * (SCREEN_H - PANEL_H), data, SCREEN_W * PANEL_H);
}

void Screen::loadBackground(FilesystemNode node) {
	delete _roomBackground;
	_roomBackground = new FlicPlayer(node);
	_roomBackgroundTime = 0;

	// Redraw everything
	_dirtyRects->clear();
	_prevDirtyRects->clear();
	// No need to report the rect, since the flic player will report it
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
		_bgDirtyRects = _roomBackground->getDirtyRects();
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
		// This check isn't in the original engine
		/*case '\t':
			w += 12;
			break;*/
		default:
			w += *((const uint8 *)(_font->getCharData(text[i])));
		}
		++w;
	}
	if (w > 0) --w;

	return w;
}

void Screen::displayDoors() {
	Common::Array<RoomDoor> *roomDoors = _vm->game()->getDoors();

	for (uint i = 0; i < roomDoors->size(); i++) {
		Actor *act = _vm->actorMan()->get((*roomDoors)[i].actorId);

		act->setFrame((*roomDoors)[i].frame);
	}
}

void Screen::writeTextCentered(byte *buf, const char *text, uint8 row, uint8 color, bool isEmbossed) {
	uint8 col = (SCREEN_W - getTextWidth(text)) / 2;
	writeText(buf, text, row, col, color, isEmbossed);
}

void Screen::writeText(byte *buf, const char *text, uint8 row, uint16 col, uint8 color, bool isEmbossed) {
	if (isEmbossed)
		writeTextStyle(buf, text, row, col, 0, true);

	writeTextStyle(buf, text, row, col, color, false);
}
void Screen::writeTextStyle(byte *buf, const char *text, uint8 startRow, uint16 startCol, uint8 color, bool isBackground) {
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
						if (isBackground) {
							buf[SCREEN_W * (startRow + h) + col + w - 1] = 3;
							buf[SCREEN_W * (startRow + h - 1) + col + w] = 3;
							buf[SCREEN_W * (startRow + h - 1) + col + w - 1] = 3;
							buf[SCREEN_W * (startRow + h) + col + w + 1] = 53;
							buf[SCREEN_W * (startRow + h + 1) + col + w] = 53;
							buf[SCREEN_W * (startRow + h + 1) + col + w + 1] = 53;
						} else {
							buf[SCREEN_W * (startRow + h) + col + w] = color;
						}
					++data;
				}
			}
			col += charWidth;
		}

		col += 1;
	}
}

} // End of namespace Kom
