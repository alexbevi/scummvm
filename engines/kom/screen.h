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

#ifndef KOM_SCREEN_H
#define KOM_SCREEN_H

#include "common/system.h"
#include "common/fs.h"
#include "common/list.h"
#include "common/rect.h"

#include "kom/flicplayer.h"
#include "kom/font.h"

class OSystem;
class FlicPlayer;

namespace Kom {

enum {
	SCREEN_W = 320,
	SCREEN_H = 200,
	MOUSE_W = 40,
	MOUSE_H = 40,
	PANEL_H = 32,
	INVENTORY_OFFSET = 344
};

class KomEngine;

class Screen {
public:

	
	Screen(KomEngine *vm, OSystem *system);
	~Screen();

	bool init();

	void processGraphics(int mode);
	void gfxUpdate();
	void drawActorFrame0(const int8 *data, uint16 width, uint16 height, int16 xStart, int16 yStart,
			uint16 xEnd, uint16 yEnd, int maskDepth);
	void drawActorFrame4(const int8 *data, uint16 width, uint16 height, int16 xStart, int16 yStart);
	void drawMouseFrame(const int8 *data, uint16 width, uint16 height, int16 xOffset, int16 yOffset);

	void setMouseCursor(const byte *buf, uint w, uint h, int hotspotX, int hotspotY);
	void showMouseCursor(bool show);
	void displayMouse();
	void updateCursor();

	void drawPanel(const byte *panelData);
	void updatePanelOnScreen();

	void loadBackground(Common::FilesystemNode node);
	void updateBackground();
	void drawBackground();
	void pauseBackground(bool pause) { _backgroundPaused = pause; }
	void setMask(const uint8 *data);

	void displayDoors();

	void writeTextCentered(byte *buf, const char *text, uint8 row, uint8 color, bool isEmbossed);
	void writeText(byte *buf, const char *text, uint8 row, uint16 col, uint8 color, bool isEmbossed);

private:

	void copyRectListToScreen(const Common::List<Common::Rect> *);

	void drawActorFrameLine(byte *outBuffer, const int8 *rowData, uint16 length);
	byte *loadColorSet(Common::FilesystemNode fsNode);

	uint16 getTextWidth(const char *text);
	void writeTextStyle(byte *buf, const char *text, uint8 startRow, uint16 startCol, uint8 color, bool isBackground);

	OSystem *_system;
	KomEngine *_vm;

	uint8 *_screenBuf;
	uint8 *_panelBuf;
	uint8 *_mouseBuf;
	uint8 *_mask;
	FlicPlayer *_roomBackground;
	Font *_font;
	uint32 _roomBackgroundTime;
	bool _backgroundPaused;

	byte *_c0ColorSet;

	uint32 _lastFrameTime;

	Common::List<Common::Rect> *_dirtyRects;
	Common::List<Common::Rect> *_prevDirtyRects;
};

} // End of namespace Kom

#endif
