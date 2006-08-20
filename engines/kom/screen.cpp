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
#include "common/endian.h"

#include "kom/screen.h"
#include "kom/kom.h"
#include "kom/actor.h"

using Common::File;

namespace Kom {

Screen::Screen(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm), _c0ColorSet(0) {

	_screenBuf = new uint8[SCREEN_W * SCREEN_H];
	memset(_screenBuf, 0, SCREEN_W * SCREEN_H);

	_c0ColorSet = loadColorSet(_vm->dataDir()->getChild("kom").getChild("oneoffs").getChild("c0_127.cl"));
}

Screen::~Screen() {
	delete[] _screenBuf;
	delete[] _c0ColorSet;
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
	_vm->actorMan()->displayAll();
	_system->copyRectToScreen(_screenBuf, SCREEN_W, 0, 0, SCREEN_W, SCREEN_H);
	_system->updateScreen();
}

void Screen::drawActorFrame(const int8 *data, uint16 width, uint16 height, uint16 xPos, uint16 yPos,
                            int16 xOffset, int16 yOffset) {

	// Check which lines and colums to draw
	int16 realX = xPos + xOffset;
	int16 realY = yPos + yOffset;

	uint16 startCol = (realX < 0 ? -realX : 0);
	uint16 startLine = (realY < 0 ? -realY : 0);
	uint16 endCol = (realX + width > SCREEN_W ? SCREEN_W - realX : width);
	uint16 endLine = (realY + height - 1 > SCREEN_H ? SCREEN_H - realY : height - 1);

	for (int line = startLine; line <= endLine; ++line) {
		uint16 lineOffset = READ_LE_UINT16(data + line * 2);

		drawActorFrameLine(data + lineOffset, (realX < 0 ? 0 : realX), (realY < 0 ? 0 : realY) + line - startLine, startCol, endCol);
	}
}


void Screen::drawActorFrameLine(const int8 *data, uint16 xPos, uint16 yPos, uint16 startPixel, uint16 endPixel) {
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

			imageData &= 0x3F;

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

				memcpy(_screenBuf + (yPos * SCREEN_W) + xPos + pixelsDrawn, data + dataIndex,
					   imageData);
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

} // End of namespace Kom
