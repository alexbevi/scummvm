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

#ifndef GARGOYLE_H
#define GARGOYLE_H

#include "common/scummsys.h"
#include "common/error.h"
#include "common/keyboard.h"
#include "common/list.h"
#include "common/rect.h"
#include "common/str.h"
#include "common/util.h"
#include "engines/engine.h"
#include "gui/debugger.h"

#include "gargoyle/blorb.h"
#include "gargoyle/graphics.h"

namespace Gargoyle {

enum GargoyleEngineType {
	GTYPE_ZCODE = 1
};

enum GargoyleGameId {
	// Infocom games
	GID_ZCODE_UNKNOWN = 0,
	GID_ZCODE_AMFV = 1,
	GID_ZCODE_ARTHUR = 2,
	GID_ZCODE_BEYOND_ZORK = 3,
	GID_ZCODE_HHGTTG = 4,
	GID_ZCODE_JOURNEY = 5,
	GID_ZCODE_LURKING_HORROR = 6,
	GID_ZCODE_SHERLOCK = 7,
	GID_ZCODE_SHOGUN = 8,
	GID_ZCODE_ZORK_ZERO = 9,
	GID_ZCODE_ZORK1 = 10,
	GID_ZCODE_ZORK2 = 11,
	GID_ZCODE_ZORK3 = 12,

	GID_UNKNOWN
};

enum GargoyleGameFeatures {
	GF_FANMADE =     (1 << 0),
	GF_MOUSE =       (1 << 1)
};

enum {
	kGargoyleDebugScripts = 1 << 0
};

#define MESSAGE_BASIC 1
#define MESSAGE_INTERMEDIATE 2
#define MESSAGE_DETAILED 3

enum {
	GARG_MB_LEFT = 1,
	GARG_MB_RIGHT = 2
};

enum {
	GARG_KBD_TIMEOUT = 0,
	GARG_KBD_QUIT = 1,
	GARG_MOUSE_CLICK = 2,
	GARG_MOUSE_DBLCLICK = 3,
	GARG_SAVE_GAME = 4,
	GARG_LOAD_GAME = 5
};

struct GargoyleGameDescription;

#define GARGOYLE_DOMAIN "gargoyle"

#define SIZE_LINE_HISTORY 100

class Interpreter;

class GargoyleEngine : public Engine {
private:
	const GargoyleGameDescription *_gameDescription;
	Interpreter *_interpreter;
protected:
	virtual Common::Error run();

public:
	GargoyleEngine(OSystem *syst, const GargoyleGameDescription *gameDesc);
	virtual ~GargoyleEngine();
	virtual bool hasFeature(EngineFeature f) const;

	GargoyleEngineType getEngine() const;
	const Common::String &getFilename() const;
	uint32 getGameID() const;
	const char *getGameId() const;
	uint16 getVersion() const;
	uint32 getFeatures() const;
	int getGameType() const;
	Common::Language getLanguage() const;
	Common::Platform getPlatform() const;
	GUI::Debugger *getDebugger();
	Interpreter *interp() { return _interpreter; };

	virtual void pauseEngine(bool pause);
	virtual bool canLoadGameStateCurrently();
	virtual bool canSaveGameStateCurrently();
	virtual Common::Error loadGameState(int slot);
	virtual Common::Error saveGameState(int slot, const char *desc);
};

class Interpreter: public GUI::Debugger {
private:
	BlorbFile *_blorbFile;
	bool _blorbGameFile;
	bool _mouseVisible;
	uint32 _priorDebugTicks;
	bool _readLineActive;
	uint32 _lastLeftClick;

	void setupBlorbFile();
protected:
	GargoyleEngine &_engine;
	Common::SaveFileManager *_saveFileMan;
	ScreenSurface _screen;
	Common::List<int> _keyPresses;
	Common::Point _mousePos;
	uint8 _mouseButtons;
	Common::Point _lastMousePos;
	bool _quitFlag;
	char _saveDescription[100];
	int _saveSlot;
	Common::Array<Common::String> _history;
	int _historyIndex;

	bool manageEvents();
	bool isMouseVisible() const { return _mouseVisible; }
	void setMouseVisible(bool v);

public:
	Interpreter(GargoyleEngine &engine);
	virtual ~Interpreter();
	virtual void go() {}

	bool quitFlag() const { return _quitFlag; }
	void quit() { _quitFlag = true; }
	Common::Point mousePos() const { return _mousePos; }
	uint8 mouseButtons() const { return _mouseButtons; }
	bool readLineActive() const { return _readLineActive; }
	int readLine(char *buf, uint maxSize, uint maxWidth, uint timeout, int editIndex);
	int readKey(uint timeout);
	void waitForPress();
	void scheduleLoad(int slotNumber);
	void scheduleSave(int slotNumber, const char *saveDescription);
	void showSaveLoadDialog(bool saveFlag);
	void getSaveFilename(char *saveName, int slotNumber);

	ScreenSurface &screen() { return _screen; }
	bool hasBlorbFile() const { return _blorbFile != NULL; }
	BlorbFile *getBlorbFile() { return _blorbFile; }
	bool isBlorbGame() const { return _blorbGameFile; }
	Graphics::Surface *getPicture(int id);

	bool isValidChar(unsigned short c);
	uint8 getDefaultColour(bool fore) const;
	uint8 getColourIndex(uint32 rgb) const;
};

} // End of namespace Gargoyle

#endif
