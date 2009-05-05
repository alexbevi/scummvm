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
#include "graphics/video/flic_player.h"

#include "kom/kom.h"
#include "kom/font.h"
#include "kom/game.h"

class OSystem;

namespace Kom {

enum {
	SCREEN_W = 320,
	SCREEN_H = 200,
	MOUSE_W = 40,
	MOUSE_H = 40,
	PANEL_H = 32,
	INVENTORY_OFFSET = 344
};

struct ColorSet {
	ColorSet(const char *filename);
	~ColorSet();

	uint size;
	byte *data;
};

class Screen {
public:

	Screen(KomEngine *vm, OSystem *system);
	~Screen();

	bool init();

	void processGraphics(int mode);
	void drawDirtyRects();
	void gfxUpdate();
	void clearScreen();
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

	void loadBackground(const char *filename);
	void updateBackground();
	void drawBackground();
	void pauseBackground(bool pause) { _backgroundPaused = pause; }
	void setMask(const uint8 *data);

	void drawInventory(Inventory *inv);

	void useColorSet(ColorSet *cs, uint start);
	void createSepia(bool shop);
	void freeSepia();
	void copySepia();

	void setBrightness(uint16 brightness) { _newBrightness = brightness; }

	void displayDoors();

	void narratorScrollInit(char *text);
	void narratorScrollUpdate();
	void narratorScrollDelete();
	void narratorWriteLine(byte *buf);

	void writeTextCentered(byte *buf, const char *text, uint8 row, uint8 color, bool isEmbossed);
	void writeText(byte *buf, const char *text, uint8 row, uint16 col, uint8 color, bool isEmbossed);

	void drawBoxScreen(int x, int y, int width, int height, byte color);
	void drawBox(byte *surface, int x, int y, int width, int height, byte color);

private:

	void copyRectListToScreen(const Common::List<Common::Rect> *);

	void drawActorFrameLine(byte *outBuffer, const int8 *rowData, uint16 length);

	uint16 getTextWidth(const char *text);
	void writeTextStyle(byte *buf, const char *text, uint8 startRow, uint16 startCol, uint8 color, bool isBackground);

	void setPaletteBrightness();

	void printIcon(Inventory *inv, int a, int b);

	void updateActionStrings();

	OSystem *_system;
	KomEngine *_vm;

	uint8 *_screenBuf;
	uint8 *_panelBuf;
	uint8 *_mouseBuf;
	uint8 *_mask;
	Graphics::FlicPlayer _roomBackground;
	Font *_font;
	uint32 _roomBackgroundTime;
	bool _backgroundPaused;

	bool _freshScreen;

	byte *_sepiaScreen;
	byte _backupPalette[256 * 4];

	ColorSet *_c0ColorSet;
	ColorSet *_orangeColorSet;
	ColorSet *_greenColorSet;
	bool _paletteChanged;

	uint16 _currBrightness;
	uint16 _newBrightness;

	uint32 _lastFrameTime;

	Common::List<Common::Rect> *_dirtyRects;
	Common::List<Common::Rect> *_prevDirtyRects;

	char *_narratorScrollText;
	char *_narratorWord;
	byte *_narratorTextSurface;
	uint8 _narratorScrollStatus;
};

} // End of namespace Kom

#endif
