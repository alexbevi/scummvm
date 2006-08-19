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

void Screen::drawActorFrame(int8 *data, uint16 width, uint16 height, uint16 xPos, uint16 yPos) {
	uint16 pixelsDrawn = 0;
	uint16 dataIndex = 0;
	uint16 lineIndex = 0;
	uint16 screenIndex = xPos + yPos * SCREEN_W;
	int8 prefixSkip, suffixSkip, imageData;

	while (pixelsDrawn < width * height) {
		imageData = data[dataIndex];

		while (imageData > 0) {

			dataIndex++;
			screenIndex += imageData;
			lineIndex += imageData;
			pixelsDrawn += imageData;

			if (lineIndex >= width) {
				screenIndex += (SCREEN_W - width);
				lineIndex = 0;
			}

			imageData = data[dataIndex];
		}

		dataIndex++;
		imageData &= 0x3F;

		memcpy(_screenBuf + screenIndex, data + dataIndex, imageData);
		dataIndex += imageData;
		screenIndex += imageData;
		lineIndex += imageData;
		pixelsDrawn += imageData;

		if (lineIndex >= width) {
			screenIndex += (SCREEN_W - width);
			lineIndex = 0;
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
