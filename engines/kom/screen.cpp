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
#include "graphics/surface.h"

#include "kom/screen.h"
#include "kom/kom.h"
#include "kom/panel.h"
#include "kom/actor.h"
#include "kom/game.h"

using Common::File;
using Common::List;
using Common::Rect;

namespace Kom {

ColorSet::ColorSet(Common::FSNode fsNode) {
	File f;

	f.open(fsNode);

	size = f.size() / 3;

	data = new byte[size * 4];

	for (uint i = 0; i < size; ++i) {
		data[4 * i + 0] = f.readByte();
		data[4 * i + 1] = f.readByte();
		data[4 * i + 2] = f.readByte();
		data[4 * i + 3] = 0;
	}

	f.close();
}

ColorSet::~ColorSet() {
	delete[] data;
}

Screen::Screen(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm), _roomBackground(0), _sepiaScreen(0),
	  _freshScreen(false), _paletteChanged(false), _newBrightness(256) {

	_lastFrameTime = 0;

	_screenBuf = new uint8[SCREEN_W * SCREEN_H];
	memset(_screenBuf, 0, SCREEN_W * SCREEN_H);

	_mouseBuf = new uint8[MOUSE_W * MOUSE_H];
	memset(_mouseBuf, 0, MOUSE_W * MOUSE_H);

	_c0ColorSet = new ColorSet(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("c0_127.cl"));
	_orangeColorSet = new ColorSet(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("sepia_or.cl"));
	_greenColorSet = new ColorSet(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("sepia_gr.cl"));

	_mask = new uint8[SCREEN_W * (SCREEN_H - PANEL_H)];
	memset(_mask, 0, SCREEN_W * (SCREEN_H - PANEL_H));

	_font = new Font(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("packfont.fnt"));

	_dirtyRects = new List<Rect>();
	_prevDirtyRects = new List<Rect>();
}

Screen::~Screen() {
	delete[] _screenBuf;
	delete[] _mouseBuf;
	delete _c0ColorSet;
	delete _orangeColorSet;
	delete _greenColorSet;
	delete[] _sepiaScreen;
	delete[] _mask;
	delete _roomBackground;
	delete _font;
	delete _dirtyRects;
	delete _prevDirtyRects;
}

bool Screen::init() {
	_system->beginGFXTransaction();
		initCommonGFX(false);
		_system->initSize(SCREEN_W, SCREEN_H);
	_system->endGFXTransaction();

	useColorSet(_c0ColorSet, 0);

	return true;
}

void Screen::processGraphics(int mode) {
	int scale;

	// handle screen objects
	Common::Array<RoomObject> *roomObjects = _vm->game()->getObjects();

	for (uint i = 0; i < roomObjects->size(); i++) {

		// Remove disappeared objects
		if ((*roomObjects)[i].disappearTimer > 0) {
			if (--(*roomObjects)[i].disappearTimer == 0)
				(*roomObjects)[i].objectId = -1;
		}

		// Set object visibility
		if (mode >= 0 && (*roomObjects)[i].actorId >= 0) {
			Actor *act =
				_vm->actorMan()->get((*roomObjects)[i].actorId);

			if ((*roomObjects)[i].objectId >= 0) {
				Object *obj =
					_vm->database()->getObj((*roomObjects)[i].objectId);

				act->enable(obj->isVisible ? 1 : 0);
			} else
				act->enable(0);
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

		if (mode > 0) {

			// TODO - disable actor if in fight
			if (_vm->database()->getChar(0)->_lastLocation == chr->_lastLocation &&
				chr->_isVisible) {

				int maskDepth;

				chr->setScope(chr->_scopeWanted);
				Actor *act = _vm->actorMan()->get(chr->_actorId);

				if (scale == 256 && chr->_walkSpeed == 0) {
					act->setPos(chr->_screenX / 2,
							(chr->_start4 + (chr->_screenH + chr->_offset78)
							 / scale) / 256 / 2);
					maskDepth = 32767;
				} else if (i == 0 && chr->_spriteTimer != 0 &&
						   _vm->game()->player()->spriteCutMoving) {

					act->setPos(_vm->game()->player()->spriteCutX,
								_vm->game()->player()->spriteCutY);
					maskDepth = chr->_start5;
				} else {
					act->setPos(chr->_screenX / 2,
							(chr->_start4 + (chr->_screenH + chr->_offset78)
							 / scale) / 256 / 2);
					maskDepth = chr->_start5;
				}

				act->setMaskDepth(
						_vm->database()->getPriority(chr->_lastLocation, chr->_lastBox),
						maskDepth);
				act->setRatio(chr->_ratioX / scale, chr->_ratioY / scale);
			}
		}
	}

	// TODO - handle dust clouds again?
	// TODO - handle magic actors

	if (mode > 0) {
		pauseBackground(false);
		updateBackground();
		drawBackground();

		displayDoors();
		_vm->actorMan()->displayAll();
	} else {
		pauseBackground(true);
	}

	// TODO - handle snow
	// TODO - handle fight bars
	// TODO - handle mouse
	// TODO - check hitpoints
	// TODO - check narrator

	// FIXME - add more checks for cursor display
	if (_vm->database()->getChar(0)->_spriteCutState == 0 &&
		(_vm->game()->player()->commandState == 0 ||
		 _vm->game()->player()->command == CMD_WALK ||
		 _vm->game()->player()->command == CMD_NOTHING) &&
		_vm->game()->player()->spriteCutNum == 0) {

		if (_vm->game()->settings()->mouseMode > 0)
			_vm->game()->settings()->mouseMode--;

	} else
		_vm->game()->settings()->mouseMode = 3;

	// TODO - Check menu request
	if (_vm->game()->settings()->mouseMode == 0) {
		updateCursor();
		displayMouse();
	} else {
		showMouseCursor(false);
	}

	updateActionStrings();

	if (mode > 0 && _vm->panel()->isDirty())
		_vm->panel()->update();

	// TODO: check game loop state?

	if (mode == 1)
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

void Screen::drawDirtyRects() {

	// Copy everything
	if (_freshScreen) {
		_system->copyRectToScreen(_screenBuf, SCREEN_W, 0, 0, SCREEN_W, SCREEN_H);

	} else {
		// No need to save a prev, since the background reports
		// all changes
		if (_roomBackground) {
			copyRectListToScreen(_roomBackground->getDirtyRects());
			_roomBackground->clearDirtyRects();
		}

		// Copy dirty rects to screen
		copyRectListToScreen(_prevDirtyRects);
		copyRectListToScreen(_dirtyRects);
		delete _prevDirtyRects;
		_prevDirtyRects = _dirtyRects;
		_dirtyRects = new List<Rect>();
	}

	_freshScreen = false;
}

void Screen::gfxUpdate() {

	// TODO: set palette brightness

	drawDirtyRects();

	_vm->input()->resetInput();

	if (_paletteChanged) {
		_paletteChanged = false;
	}

	while (_system->getMillis() < _lastFrameTime + 41 /* 24 fps */) {
		_vm->input()->checkKeys();
		_system->delayMillis(10);
	}
	_system->updateScreen();
	_lastFrameTime = _system->getMillis();
}

void Screen::clearScreen() {
	_dirtyRects->clear();
	_prevDirtyRects->clear();
	if (_roomBackground)
		_roomBackground->redraw();
	memset(_screenBuf, 0, SCREEN_W * SCREEN_H);
	_freshScreen = true;
}

static byte lineBuffer[SCREEN_W];

void Screen::drawActorFrame0(const int8 *data, uint16 width, uint16 height, int16 xStart, int16 yStart,
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

	div_t widthRatio = div(width, scaledWidth);
	div_t heightRatio = div(height, scaledHeight);

	if (xStart < 0) {
		// frame is entirely off-screen
		if ((visibleWidth += xStart) <= 0)
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
		if ((visibleHeight += yStart) <= 0)
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

		uint16 targetPixel = targetLine * SCREEN_W + xStart;
		drawActorFrameLine(lineBuffer, data + lineOffset, width);

		// Copy line to screen
		uint8 sourcePixel = startCol;
		int16 colThing = scaledWidth - colSkip;

		for (int j = 0; j < visibleWidth; ++j) {
			if (lineBuffer[sourcePixel] != 0 && _mask[targetPixel] >= maskDepth)
				_screenBuf[targetPixel] = lineBuffer[sourcePixel];

			sourcePixel += widthRatio.quot;
			colThing -= widthRatio.rem;

			if (colThing < 0) {
				sourcePixel++;
				colThing += scaledWidth;
			}

			targetPixel++;
		}

		sourceLine += heightRatio.quot;
		rowThing -= heightRatio.rem;

		if (rowThing < 0) {
			sourceLine++;
			rowThing += scaledHeight;
		}

		targetLine++;
	}

	_dirtyRects->push_back(Rect(xStart, yStart, xStart + visibleWidth, yStart + visibleHeight));
}

void Screen::drawActorFrame4(const int8 *data, uint16 width, uint16 height, int16 xStart, int16 yStart) {

	uint16 startLine = 0;
	uint16 startCol = 0;

	int16 visibleWidth = width;
	int16 visibleHeight = height;

	if (visibleWidth == 0 || visibleHeight == 0) return;

	if (xStart < 0) {
		// frame is entirely off-screen
		if ((visibleWidth += xStart) <= 0)
			return;

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
		if ((visibleHeight += yStart) <= 0)
			return;

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

	for (int i = 0; i < visibleHeight; i += 1) {
		uint16 lineOffset = READ_LE_UINT16(data + sourceLine * 2);

		uint16 targetPixel = targetLine * SCREEN_W + xStart;
		drawActorFrameLine(lineBuffer, data + lineOffset, width);

		// Copy line to screen
		uint8 sourcePixel = startCol;

		for (int j = 0; j < visibleWidth; ++j) {
			if (lineBuffer[sourcePixel] != 0)
				_screenBuf[targetPixel] = lineBuffer[sourcePixel];
			sourcePixel++;
			targetPixel++;
		}

		sourceLine++;
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

void Screen::useColorSet(ColorSet *cs, uint start) {
	static const byte black[4] = { 0, 0, 0, 0 };

	_system->setPalette(cs->data, start, cs->size);

	// Index 0 is always black
	_system->setPalette(black, 0, 1);

	_paletteChanged = true;
}

void Screen::setPaletteBrightness() {
	byte newPalette[256 * 4];

	_system->grabPalette(newPalette, 0, 256);

	if (_currBrightness < 256) {

		for (uint i = 0; i < 256 * 4; (i%4==2)?i+=2:i++) {
			newPalette[i] = newPalette[i] * _currBrightness / 256;
		}

		_system->setPalette(newPalette, 0, 256);
		_paletteChanged = true;
		_newBrightness = 9999;
	} else {
		warning("TODO: setPaletteBrightness");
	}
}

void Screen::createSepia(bool shop) {
	_sepiaScreen = new byte[SCREEN_W * (SCREEN_H - PANEL_H)];
	ColorSet *cs = shop ? _greenColorSet : _orangeColorSet;

	drawDirtyRects();

	Graphics::Surface *screen = _system->lockScreen();
	assert(screen);

	_system->grabPalette(_backupPalette, 0, 256);

	for (uint y = 0; y < SCREEN_H - PANEL_H; ++y) {
		for (uint x = 0; x < SCREEN_W; ++x) {
			byte *color = &_backupPalette[((uint8*)screen->pixels)[y * SCREEN_W + x] * 4];

			// FIXME: this does not produce the same shade as the original
			_sepiaScreen[y * SCREEN_W + x] = ((color[0] + color[1] + color[2]) / 3 * 23 + 255 / 2) / 255 + 232;
			//_sepiaScreen[y * SCREEN_W + x] = (color[0] + color[1] + color[2]) / 12 + 232;
		}
	}

	_system->unlockScreen();

	useColorSet(cs, 224);
}

void Screen::freeSepia() {
	if (!_sepiaScreen)
		return;

	delete[] _sepiaScreen;
	_system->setPalette(_backupPalette, 0, 256);

	if (_roomBackground)
		_roomBackground->redraw();
}

void Screen::copySepia() {
	if (!_sepiaScreen)
		return;

	memcpy(_screenBuf, _sepiaScreen, SCREEN_W * (SCREEN_H - PANEL_H));
	// FIXME: behavior is good but a bit hackish. the room background reports
	//        the full screen after a redraw, even when it's not actually drawn.
}

void Screen::setMouseCursor(const byte *buf, uint w, uint h, int hotspotX, int hotspotY) {
	CursorMan.replaceCursor(buf, w, h, hotspotX, hotspotY, 0);
}

void Screen::showMouseCursor(bool show) {
	CursorMan.showMouse(show);
}

void Screen::displayMouse() {
	showMouseCursor(true);
	_vm->actorMan()->getMouse()->display();
}

void Screen::updateCursor() {
	Actor *mouse = _vm->actorMan()->getMouse();
	Settings *settings = _vm->game()->settings();

	if (settings->mouseY >= INVENTORY_OFFSET) {
		mouse->switchScope(5, 2);
	} else {
		if (settings->objectNum >= 0) {
			// TODO - holding item?
		}

		switch (settings->mouseState) {
		case 2: // exit
			mouse->switchScope(2, 2);
			break;
		case 3: // hot spot
			mouse->switchScope(3, 2);
			break;
		case 0: // walk
		default:
			mouse->switchScope(0, 2);
		}
	}

	// FIXME: if the mouse is already over a hotspot when the game starts,
	//        the sound should be playing
	if (settings->overType != settings->oldOverType ||
		settings->overNum != settings->oldOverNum) {

		if (settings->overType <= 1) {
			_vm->sound()->stopSample(_vm->_hotspotSample);
		} else if (settings->overType <= 3) {
			_vm->sound()->playSampleSFX(_vm->_hotspotSample, false);
		}
	}
}

void Screen::updateActionStrings() {
	Settings *settings = _vm->game()->settings();
	int hoverId = -1;
	int hoverType;
	const char *text1, *text2, *text3, *text4, *space1, *space2, *textRIP;
	char panelText[200];

	text1 = text2 = text3 = text4 = space1 = space2 = textRIP = "";

	if (_vm->_flicLoaded != 0)
		return;

	if (_vm->game()->player()->commandState != 0 &&
		_vm->game()->player()->collideType != 0) {

		hoverType = _vm->game()->player()->collideType;
		hoverId = _vm->game()->player()->collideNum;

	} else {
		hoverType = settings->overType;

		switch (settings->overType) {
			case 2:
				hoverId = settings->collideChar;
				break;
			case 3:
				hoverId = settings->collideObj;
				break;
			default:
				hoverId = -1;
		}
	}

	// Use # with #
	if (settings->objectNum >= 0) {

		if (_vm->game()->player()->command == CMD_USE) {

			text1 = "Use";
			text2 = _vm->database()->
				getObj(settings->objectNum)->desc;
			text3 = "with";
			switch (hoverType) {
			case 2:
				text4 = _vm->database()->getChar(hoverId)->_desc;
				break;
			case 3:
				text4 = _vm->database()->getObj(hoverId)->desc;
				break;
			default:
				text4 = "...";
			}
		} else if (_vm->game()->player()->command == CMD_FIGHT ||
		           _vm->game()->player()->command == CMD_CAST_SPELL) {

			if (_vm->game()->player()->command == CMD_FIGHT)
				text3 = "Fight with";
			else
				text3 = "Cast spell at";

			if (hoverId != 2)
				text4 = "...";
			else
				text4 = _vm->database()->getChar(hoverId)->_desc;
		}

	// Use #
	} else {
		text3 = "Walk to";
		if (_vm->game()->player()->commandState > 0) {
			switch (_vm->game()->player()->command) {
			case CMD_USE:
				text3 = "Use";
				break;
            case CMD_TALK_TO:
				text3 = "Talk to";
				break;
            case CMD_PICKUP:
				text3 = "Pickup";
				break;
            case CMD_LOOK_AT:
				text3 = "Look at";
				break;
            case CMD_FIGHT:
				text3 = "Fight with";
				break;
			case CMD_CAST_SPELL:
				text3 = "Cast spell at";
				break;
			case CMD_WALK:
			case CMD_NOTHING:
				break;
			}
		}

		switch (hoverType) {
		case 1:
			if (settings->mouseOverExit) {
				int exitLoc, exitBox;
				text3 = "Exit to";
				_vm->database()->getExitInfo(
						_vm->database()->getChar(0)->_lastLocation,
						settings->collideBox,
						&exitLoc, &exitBox);

				text4 = _vm->database()->getLoc(exitLoc)->desc;
			}
			break;
		case 2:
			text4 = _vm->database()->getChar(hoverId)->_desc;
			break;
		case 3:
			text4 = _vm->database()->getObj(hoverId)->desc;
			break;
		default:
			text4 = "...";
		}
	}

	// Set panel strings

	sprintf(panelText, "%s %s", text1, text2);
	_vm->panel()->setActionDesc(panelText);

	if (text1[0] != 0)
		space1 = " ";
	if (text4[0] != '.')
		space2 = " ";

	if (hoverType == 2) {
		if (!_vm->database()->getChar(hoverId)->_isAlive)
			textRIP = " (RIP)";

		sprintf(panelText, "%s%s%s%s%s", space1, text3, space2, text4, textRIP);

	} else {
		sprintf(panelText, "%s%s%s%s", space1, text3, space2, text4);
	}

	_vm->panel()->setHotspotDesc(panelText);

	// TODO - inventory strings
	if (settings->mouseY >= INVENTORY_OFFSET && settings->mouseMode == 0) {
		_vm->panel()->setActionDesc("");
		_vm->panel()->setHotspotDesc("Inventory...");
	}
}

void Screen::drawPanel(const byte *panelData) {
	memcpy(_screenBuf + SCREEN_W * (SCREEN_H - PANEL_H), panelData, SCREEN_W * PANEL_H);
	_dirtyRects->push_back(Rect(0, SCREEN_H - PANEL_H, SCREEN_W, SCREEN_H));
}

void Screen::updatePanelOnScreen() {
	_system->copyRectToScreen(_screenBuf + SCREEN_W * (SCREEN_H - PANEL_H),
		SCREEN_W, 0, SCREEN_H - PANEL_H, SCREEN_W, PANEL_H);
	_system->updateScreen();
}

void Screen::loadBackground(Common::FSNode node) {
	delete _roomBackground;
	_roomBackground = new FlicPlayer(node);
	_roomBackgroundTime = 0;
	_backgroundPaused = false;

	// Redraw everything
	_dirtyRects->clear();
	_prevDirtyRects->clear();
	// No need to report the rect, since the flic player will report it
}

void Screen::updateBackground() {
	if (_roomBackground != 0) {
		if (_system->getMillis() >= _roomBackgroundTime) {
			_roomBackgroundTime = _system->getMillis() + _roomBackground->speed();

			if (!_backgroundPaused)
				_roomBackground->decodeFrame();

			if (_roomBackground->paletteDirty()) {
				_system->setPalette(_roomBackground->getPalette() + 4 * 128, 128, 128);
				_paletteChanged = true;
			}
		}
	}
}

void Screen::drawBackground() {
	if (_roomBackground) {
		memcpy(_screenBuf, _roomBackground->getOffscreen(), SCREEN_W * (SCREEN_H - PANEL_H));
	}
}

void Screen::setMask(const uint8 *data) {
	memcpy(_mask, data, SCREEN_W * (SCREEN_H - PANEL_H));
}

void Screen::drawInventory(Inventory *inv) {
	Common::List<int> *invList;
	Common::List<int>::iterator invId;
	int invCounter;
	int selectedIndex;
	Settings *settings = _vm->game()->settings();

	_vm->screen()->copySepia();

	inv->selectedInvObj = inv->selectedWeapObj = inv->selectedSpellObj = -1;
	inv->isSelected = false;

	// Draw lines
	drawBoxScreen(76, 13, 1, 152, 0);
	drawBoxScreen(132, 13, 1, 152, 0);

	// Look for selected item
	if (inv->selectedBox2 % 5 < 2) {
		selectedIndex = -1;
	} else {
		selectedIndex = inv->selectedBox2 / 5 * 3 + (inv->selectedBox2 % 5) - 3;
		if (selectedIndex == -1)
			selectedIndex = inv->selectedInvObj = 9999;
	}

	inv->iconX = 160;
	inv->iconY = inv->scrollY;

	if (selectedIndex == 9999) {
		printIcon(inv, 3, 1);
	} else {
		printIcon(inv, 2, 1);
	}

	inv->iconX += 56;

	// Inventory objects
	invList = &_vm->database()->getChar(0)->_inventory;
	for (invId = invList->begin(), invCounter = 0;
		 invId != invList->end(); ++invId, ++invCounter) {

		if (invCounter == selectedIndex) {
			inv->isSelected = true;
			inv->selectedInvObj = *invId;
			printIcon(inv, 1, *invId + 1);
		} else {
			printIcon(inv, 0, *invId + 1);
		}

		inv->iconX += 56;

		// Move to the next line
		if (inv->iconX > 272) {
			inv->iconX = 160;
			inv->iconY += 40;
		}
	}

	// Look for selected item
	if (inv->selectedBox2 % 5 != 1) {
		selectedIndex = -1;
	} else {
		selectedIndex = (inv->selectedBox2 - 1) / 5;
	}

	inv->iconX = 104;
	inv->iconY = inv->scrollY;

	// Inventory weapons
	invList = &_vm->database()->getChar(0)->_weapons;
	for (invId = invList->begin(), invCounter = 0;
		 invId != invList->end(); ++invId, ++invCounter) {

		if (invCounter == selectedIndex) {
			inv->isSelected = true;
			inv->selectedWeapObj = *invId;
			printIcon(inv, 1, *invId + 1);
		} else {
			printIcon(inv, 0, *invId + 1);
		}

		// Move to the next line
		inv->iconY += 40;
	}

	// Look for selected item
	if (inv->selectedBox2 % 5 != 0) {
		selectedIndex = -1;
	} else {
		selectedIndex = inv->selectedBox2 / 5;
	}

	inv->iconX = 48;
	inv->iconY = inv->scrollY;

	// Inventory spells
	invList = &_vm->database()->getChar(0)->_spells;
	for (invId = invList->begin(), invCounter = 0;
		 invId != invList->end(); ++invId, ++invCounter) {

		if (invCounter == selectedIndex) {
			inv->isSelected = true;
			inv->selectedSpellObj = *invId;
			printIcon(inv, 1, *invId + 1);
		} else {
			printIcon(inv, 0, *invId + 1);
		}

		// Move to the next line
		inv->iconY += 40;
	}

	// Draw "dragged" item
	if (settings->objectNum >= 0) {
		Actor *act = _vm->actorMan()->getObjects();
		act->enable(1);
		act->setEffect(4);
		act->setMaskDepth(0, 1);
		act->setPos(inv->mouseX, inv->mouseY);
		act->setRatio(1024, 1024);
		act->setFrame(settings->objectNum); // FIXME: off by 1?
		act->display();
		act->enable(0);
	}

	// Draw headlines
	if (inv->shop) {
		writeText(_screenBuf, "Choose an item to Purchase...", 3, 29, 0, false);
		writeText(_screenBuf, "Choose an item to Purchase...", 2, 28, 31, false);
	} else {
		if (inv->mode & 4) {
			writeText(_screenBuf, "Spells", 3, 29, 0, false);
			writeText(_screenBuf, "Spells", 2, 28, 31, false);
		}
		if (inv->mode & 2) {
			writeText(_screenBuf, "Weapons", 3, 81, 0, false);
			writeText(_screenBuf, "Weapons", 2, 80, 31, false);
		}
		if (inv->mode & 1) {
			writeText(_screenBuf, "Inventory", 3, 191, 0, false);
			writeText(_screenBuf, "Inventory", 2, 190, 31, false);
		}
	}

	inv->selectedObj = inv->selectedInvObj & inv->selectedWeapObj & inv->selectedSpellObj;

	if (inv->selectedObj == settings->objectNum) {
		inv->selectedObj = inv->selectedInvObj = inv->selectedWeapObj = inv->selectedSpellObj = -1;
		inv->isSelected = false;
	}

	if (0 <= inv->selectedObj & inv->selectedObj < 9999) {
		bool useImmediate = _vm->database()->getObj(inv->selectedObj)->isUseImmediate;

		if (useImmediate && (inv->mode & 1) == 0) {
			inv->selectedObj = inv->selectedInvObj = inv->selectedWeapObj = inv->selectedSpellObj = -1;
			inv->isSelected = false;
		} else {
			switch (inv->action) {
			case 0:
				if (inv->shop) {
					warning("TODO: shop");
				} else {
					if (!useImmediate) {
						inv->blinkLight = 4;
					} else {
						// Blink "Use" text
						if ((++inv->blinkLight & 4) == 0)
							_vm->panel()->setActionDesc("Use");
					}
				}
				break;
			case 1:
				inv->blinkLight = 4;
				_vm->panel()->setActionDesc("Look at");
				break;
			case 2:
				inv->blinkLight = 4;
				break;
			}
		}
	}

	if (inv->shop) {
		warning("TODO: Print gold info");
	}

	if (inv->mouseY * 2 > 344) {
		_vm->panel()->setHotspotDesc(inv->shop ? "No sale" : "Exit");
		_vm->panel()->update();
		return;
	}

	const char *descString;
	char panelText[200];

	if (inv->selectedObj < 0 || inv->selectedObj >= 9999) {
		descString = "";
	} else {
		descString = _vm->database()->getObj(inv->selectedObj)->desc;
	}

	if (settings->objectNum > 0) {
		warning("Use item with item");
	} else {
		if (inv->selectedSpellObj >= 0) {
			sprintf(panelText, "%s (%dpts)", descString,
					_vm->database()->getObj(inv->selectedObj)->spellCost);
			_vm->panel()->setHotspotDesc(panelText);
		} else {
			_vm->panel()->setHotspotDesc(descString);
		}
	}

	_vm->panel()->update();
}

void Screen::printIcon(Inventory *inv, int a, int b) {
	Actor *act;

	if (a >= 2)
		act = _vm->actorMan()->getCharIcon();
	else
		act = _vm->actorMan()->getObjects();

	if ((inv->mode & 1) == 0 && b >= 0) {
		warning("TODO: missing code in printIcon");
	} else {
		act->setEffect(4);
		act->setMaskDepth(0, 2);
		act->setPos(inv->iconX, inv->iconY);
		act->setRatio(1024, 1024);

		if (b < 0 || _vm->game()->settings()->objectNum == b) return;

		if (a % 2 == 0) {
			act->setFrame(b);
		} else switch (inv->mouseState) {
		case 0:
		case 4:
		case 5:
		case 6:
			act->setFrame(b);
			break;
		case 1:
		case 3:
			if (inv->blinkLight & 4)
				act->setFrame(0);
			else
				act->setFrame(1);
			act->display();
			act->setFrame(b);
			break;
		case 2:
			if (inv->selectedBox == inv->selectedBox2) {
				act->setEffect(0);
				act->setRatio(768, 768);
				if (inv->blinkLight & 4)
					act->setFrame(0);
				else
					act->setFrame(1);
				act->display();
				act->setFrame(b);
			} else {
				if (inv->blinkLight & 4)
					act->setFrame(0);
				else
					act->setFrame(1);
				act->display();
				act->setFrame(b);
			}
			break;
		}
	}

	act->display();
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

void Screen::drawBoxScreen(int x, int y, int width, int height, byte color) {
	drawBox(_screenBuf, x, y, width, height, color);
	_dirtyRects->push_back(Rect(x, y, x + width, y + height));
}
void Screen::drawBox(byte *surface, int x, int y, int width, int height, byte color) {
	byte *dest = surface + y * SCREEN_W + x;
	for (int i = 0; i < height; i++)
		memset(dest + i * SCREEN_W, color, width);
}

} // End of namespace Kom
