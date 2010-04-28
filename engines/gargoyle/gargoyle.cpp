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

#include <string.h>

#include "gargoyle/gargoyle.h"
#include "gargoyle/frotz_interp.h"
#include "gargoyle/graphics.h"
#include "gargoyle/staticres.h"

#include "base/plugins.h"
#include "common/config-manager.h"
#include "common/events.h"
#include "common/stream.h"
#include "engines/engine.h"
#include "engines/metaengine.h"
#include "graphics/cursorman.h"
#include "graphics/surface.h"
#include "gui/saveload.h"

namespace Gargoyle {

#define GARGOYLE_CONFIG_FILE "gargoyle.ini"

GargoyleEngine::GargoyleEngine(OSystem *syst, const GargoyleGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc) {

	DebugMan.addDebugChannel(kGargoyleDebugScripts, "script", "Script debug level");
	_interpreter = NULL;
}


GargoyleEngine::~GargoyleEngine() {
}

bool GargoyleEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsRTL) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

Common::Error GargoyleEngine::run() {
	// Set up the correct interpreter
	switch (getEngine()) {
	case GTYPE_ZCODE:
		_interpreter = new Frotz::FrotzInterpreter(*this);
		break;
	default:
		error("Unknown engine type");
	}

	_interpreter->go();
	delete _interpreter;
	return Common::kNoError;
}

void Interpreter::setupBlorbFile() {
	_blorbFile = NULL;

	// Try detecting a Blorb file for the given game
	Common::File f;
	if (!f.open(_engine.getFilename()))
		error("Could not locate specified game file");
	Common::MemoryReadStream *stream = f.readStream(12);
	_blorbGameFile = BlorbFile::isBlorb(stream);
	delete stream;

	if (_blorbGameFile) {
		// The specified game file is a Blorb file
		f.seek(0);
		_blorbFile = new BlorbFile();
		if (_blorbFile->open(f.readStream(f.size()), true) != Gargoyle::bb_err_None)
			error("Corrupt blorb file encountered");
		f.close();
		return;
	} 

	f.close();

	// Check to see if there's an accompanying Blorb file for the game
	// Remove any existing extension from the filename
	Common::String sFilename(_engine.getFilename());
	while ((sFilename.size() > 0) && (sFilename[sFilename.size() - 1] != '\\') && 
			(sFilename[sFilename.size() - 1] != '/') && (sFilename[sFilename.size() - 1] != '.'))
		sFilename.deleteLastChar();
	if (sFilename.size() == 0)
		return;

	const char *blorbExtensions[3] = {"blb", "blorb", "zblorb"};
	for (int i = 0; i < 3; ++i) {
		Common::String s = sFilename;

		if (s[s.size() - 1] != '.')
			// No extension, so add a '.' first
			s += ".";
		s += blorbExtensions[i];

		if (f.exists(s)) {
			f.open(s);
			_blorbFile = new BlorbFile();
			if (_blorbFile->open(f.readStream(f.size())) != Gargoyle::bb_err_None)
				error("Corrupt blorb file encountered");

			break;
		}
	}
}

Graphics::Surface *Interpreter::getPicture(int id) {
	Graphics::Surface *gfx;

	if (!hasBlorbFile())
		return NULL;

	if (id == 0) {
		int height;
		_blorbFile->countResources(bb_ID_Pict,&height,NULL,NULL);

		gfx = new Graphics::Surface();
		gfx->w = _blorbFile->getReleaseNum();
		gfx->h = height;

	} else {
		gfx = _blorbFile->getPicture(id);
	}

	return gfx;
}

void GargoyleEngine::pauseEngine(bool pause) {
	Engine::pauseEngine(pause);
}

bool GargoyleEngine::canLoadGameStateCurrently() {
	// Only allow savegames to be loaded if a line read is active
	return _interpreter->readLineActive();
}

bool GargoyleEngine::canSaveGameStateCurrently() {
	// Only allow savegames to be saved if a line read is active
	return _interpreter->readLineActive();
}

Common::Error GargoyleEngine::loadGameState(int slot) {
	_interpreter->scheduleLoad(slot);
	return Common::kNoError;
}

Common::Error GargoyleEngine::saveGameState(int slot, const char *desc) {
	_interpreter->scheduleSave(slot, desc);
	return Common::kNoError;
}

GUI::Debugger *GargoyleEngine::getDebugger() {
	return _interpreter;
}

/*--------------------------------------------------------------------------*/

Interpreter::Interpreter(GargoyleEngine &engine): _engine(engine) {
	_mousePos.x = -1;
	_mousePos.y = -1;
	_mouseButtons = 0;
	_mouseVisible = false;
	_quitFlag = false;
	_readLineActive = false;
	_saveSlot = -1;
	_lastLeftClick = 0;
	strcpy(_saveDescription, "");
	_historyIndex = 0;

	_saveFileMan = g_system->getSavefileManager();

	setupBlorbFile();

	// Set up a default cursor
	CursorMan.replaceCursor(default_cursor, CURSOR_WIDTH, CURSOR_HEIGHT, 0, 0, 0xff);
}

Interpreter::~Interpreter() {
}

#define MOUSE_MOUSE_MIN 5
#define MOUSE_DBLCLICK_MAX 250
#define GARGOYLE_FRAME_DELAY 50

bool Interpreter::manageEvents() {
	Common::Event event;

	// Handle display of debugger if it's active
	if (isAttached() && (g_system->getMillis() >= (_priorDebugTicks + GARGOYLE_FRAME_DELAY))) {
		_priorDebugTicks = g_system->getMillis();
		onFrame();
	}

	// Check to see if a timeout on a left click has occurred
	if ((_lastLeftClick != 0) && ((_lastLeftClick + MOUSE_DBLCLICK_MAX) <= g_system->getMillis())) {
		_keyPresses.push_back(GARG_MOUSE_CLICK);
		_lastLeftClick = 0;
	}

	Common::EventManager * eventMan = g_system->getEventManager();
	while (eventMan->pollEvent(event)) {
		bool abortFlag = true;

		switch (event.type) {
		case Common::EVENT_LBUTTONDOWN:
			// When the left button is pressed, store the current system time. If another click occurs quickly,
			// it's a double-click. Otherwise, if the time expires, then generate a single click
			if (_lastLeftClick != 0) {
				_keyPresses.push_back(GARG_MOUSE_DBLCLICK);
				_lastLeftClick = 0;
			} else {
				_lastLeftClick = g_system->getMillis();
			}

			_mouseButtons |= GARG_MB_LEFT;
			break;
		case Common::EVENT_LBUTTONUP:
			_mouseButtons &= ~GARG_MB_LEFT;
			break;
		case Common::EVENT_RBUTTONDOWN:
			_mouseButtons |= GARG_MB_RIGHT;
			break;
		case Common::EVENT_RBUTTONUP:
			_mouseButtons &= ~GARG_MB_RIGHT;
			break;
		case Common::EVENT_MOUSEMOVE:
			if (_mousePos.x == -1) {
				_lastMousePos.x = event.mouse.x;
				_lastMousePos.y = event.mouse.y;
			}

			_mousePos.x = event.mouse.x;
			_mousePos.y = event.mouse.y;

			if (!_mouseVisible && ((ABS(_mousePos.x - _lastMousePos.x) >= MOUSE_MOUSE_MIN) ||
				(ABS(_mousePos.x - _lastMousePos.x) >= MOUSE_MOUSE_MIN))) {
				// Mouse has been moved, so make the cursor visible again
				setMouseVisible(true);
				_lastMousePos = _mousePos;
			}

			g_system->updateScreen();
			abortFlag = false;
			break;
		case Common::EVENT_QUIT:
		case Common::EVENT_RTL:
			_quitFlag = true;
			break;
		case Common::EVENT_KEYDOWN:
			// If cursor is vislble and a key pressed, then hide it again
			if (_mouseVisible)
				setMouseVisible(false);

			if ((event.kbd.flags == Common::KBD_CTRL) && (event.kbd.keycode == Common::KEYCODE_d)) {
				// Start the debugger
				attach();
				_priorDebugTicks = g_system->getMillis();
			} else {
				int kc = event.kbd.keycode | (event.kbd.flags << 16);
				_keyPresses.push_back(kc);
			}

		default:
			break;
		}

		if (abortFlag)
			return true;
	}

	return false;
}

void Interpreter::setMouseVisible(bool v) {
	if (v != _mouseVisible)
		CursorMan.showMouse(v);
	_mouseVisible = v;
}

int Interpreter::readLine(char *buf, uint maxSize, uint maxWidth, uint timeout, int editIndex) {
	static uint prev_pos = 0;
	bool breakFlag;
	_readLineActive = true;

	uint32 timeoutTicks = 0;
	if (timeout != 0)
		timeoutTicks = g_system->getMillis() + timeout;

	_screen.flushText();

	// Find the editing position
	const Common::Point origPos = _screen.getTextSettings().textPosition;
	uint pos = 0;
	if (editIndex) {
		if (prev_pos <= strlen(buf))
		pos = prev_pos;
	} else
		pos = strlen(buf);

	bool redrawLine = true;
	while (!_quitFlag) {
		if (redrawLine) {
			// Draw the input line
			_screen.getTextSettings().textPosition.x = origPos.x;
			_screen.drawInput(buf, pos, maxWidth, true);
			_screen.update();

			redrawLine = false;
		}

		// Wait until there's a keypress to process
		while (!_quitFlag && _keyPresses.empty()) {
			manageEvents();
			g_system->delayMillis(10);

			if ((timeoutTicks != 0) && (g_system->getMillis() >= timeoutTicks))
				// Use KEYCODE_INVALID to signal a timeout
				_keyPresses.push_back(GARG_KBD_TIMEOUT);
		}
		if (_quitFlag)
			break;

		// Get the top keypress off the stack
		int kc = *_keyPresses.begin();
		_keyPresses.erase(_keyPresses.begin());
		int flags = kc >> 16;
		kc &= 0xffff;

		switch (kc) {
		case Common::KEYCODE_DELETE:
			// Delete the character under the cursor
			if (pos < strlen(buf)) {
				memmove(buf + pos, buf + pos + 1, strlen(buf) - pos);
				redrawLine = true;
			}
			break;

		case Common::KEYCODE_BACKSPACE:
			// Delete the character to the left of the cursor
			if (pos > 0) {
				memmove(buf + pos - 1, buf + pos, strlen(buf) - pos + 1);
				pos--;
				redrawLine = true;
			}
			break;

		case Common::KEYCODE_HOME:
		case Common::KEYCODE_KP7:
			// Move the cursor to the start of the line
			pos = 0;
			redrawLine = true;
			break;

		case Common::KEYCODE_END:
		case Common::KEYCODE_KP1:
			// Move the cursor to the end of the line
			pos = strlen(buf);
			redrawLine = true;
			break;

		case Common::KEYCODE_LEFT:
		case Common::KEYCODE_KP4:
			if (flags == Common::KBD_CTRL) {
				// Move cursor left one word
				if (pos > 0) --pos;
				while ((pos > 0) && (buf[pos] == ' ')) --pos;
				while ((pos > 0) && (buf[pos - 1] != ' ')) --pos;
			} else {
				// Move the cursor left one character
				if (pos > 0)
					pos--;
			}
			redrawLine = true;
			break;

		case Common::KEYCODE_RIGHT:
		case Common::KEYCODE_KP6:
			if (flags == Common::KBD_CTRL) {
				// Move cursor right one word
				if (pos < strlen(buf)) ++pos;
				while ((pos < strlen(buf)) && (buf[pos] != ' ')) ++pos;
				while ((pos < strlen(buf)) && (buf[pos] == ' ')) ++pos;
			} else {
				// Move the cursor right one character
				if (pos < strlen(buf))
					pos++;
			}
			redrawLine = true;
			break;

		case Common::KEYCODE_UP:
		case Common::KEYCODE_KP8:
			// Move backwards through the input history
			if (_historyIndex > 0) {
				strcpy(buf, _history[--_historyIndex].c_str());
				pos = strlen(buf);
				redrawLine = true;
			}
			break;

		case Common::KEYCODE_DOWN:
		case Common::KEYCODE_KP2:
			if (_historyIndex < ((int)_history.size() - 1)) {
				strcpy(buf, _history[++_historyIndex].c_str());
				pos = strlen(buf);
				redrawLine = true;
			}
			break;

		case Common::KEYCODE_RETURN:
		case Common::KEYCODE_KP_ENTER:
		case GARG_MOUSE_CLICK:
		case GARG_MOUSE_DBLCLICK:
		case GARG_SAVE_GAME:
		case GARG_LOAD_GAME:
		case GARG_KBD_TIMEOUT:
			// Terminate the current input
			breakFlag = (kc == Gargoyle::GARG_KBD_TIMEOUT) || (kc == Gargoyle::GARG_MOUSE_CLICK) ||
				(kc == Gargoyle::GARG_MOUSE_DBLCLICK);

			// Handle translating save/load hotkeys to text
			if (kc == GARG_SAVE_GAME) {
				strcpy(buf, "save");
				kc = Common::KEYCODE_RETURN;
			} else if (kc == GARG_LOAD_GAME) {
				strcpy(buf, "restore");
				kc = Common::KEYCODE_RETURN;
			} else if (!breakFlag) {
				// Add entered line to the history
				if (_history.size() >= SIZE_LINE_HISTORY)
					// History full, so remove earliest entry
					_history.remove_at(0);

				// Add new entry to the history list
				_history.push_back(Common::String(buf));
			}

			// Whenever a valid line is complete, reset the the history position to the end
			if (!breakFlag)
				_historyIndex = _history.size();


			// Redraw the line without the cursor
			_screen.getTextSettings().textPosition.x = origPos.x;
			_screen.drawInput(buf, strlen(buf), maxWidth, false);
			_screen.getTextSettings().textPosition.x = origPos.x;

			// Keycode translation
			if (kc == Common::KEYCODE_KP_ENTER)
				kc = Common::KEYCODE_RETURN;

			prev_pos = pos;
			_readLineActive = false;
			return kc;

		default:
			if ((kc >= Common::KEYCODE_SPACE) && (kc <= Common::KEYCODE_z)) {
				// Add a valid character to the input line
				if (strlen(buf) < maxSize) {
					// Get the width of the new input line
					uint len = _screen.getTextSettings().getStringWidth(buf);
					len += _screen.getTextSettings().getCharWidth(kc);
					len += _screen.getTextSettings().getCharWidth();

					// Only allow if the width limit is not exceeded
					if (len <= maxWidth) {
						memmove(buf + pos + 1, buf + pos, strlen(buf) - pos + 1);
						*(buf + pos) = kc;
						pos++;
						redrawLine = true;
					}
				}
			}
			break;
		}
	}

	return GARG_KBD_QUIT;
}

int Interpreter::readKey(uint timeout) {
	uint32 timeoutTicks = 0;
	if (timeout != 0)
		timeoutTicks = g_system->getMillis() + timeout;

	// Get the next input
	while (!_quitFlag && _keyPresses.empty()) {
		manageEvents();
		g_system->delayMillis(10);

		if ((timeoutTicks != 0) && (g_system->getMillis() >= timeoutTicks))
			return GARG_KBD_TIMEOUT;
	}

	if (_quitFlag)
		return GARG_KBD_QUIT;

	// Get the top keypress off the stack
	int kc = *_keyPresses.begin();
	_keyPresses.erase(_keyPresses.begin());

	return kc & 0xffff;
}

void Interpreter::waitForPress() {
	while (readKey(0) > 255) ;
}

void Interpreter::scheduleLoad(int slotNumber) {
	_saveSlot = slotNumber;
	_keyPresses.push_back(GARG_LOAD_GAME);
}

void  Interpreter::scheduleSave(int slotNumber, const char *saveDescription) {
	_saveSlot = slotNumber;
	strncpy(_saveDescription, saveDescription, 99);
	_keyPresses.push_back(GARG_SAVE_GAME);
}

/**
 * Shows the ScummVM GMM Save or Load dialog
 */
void Interpreter::showSaveLoadDialog(bool saveFlag) {
	const EnginePlugin *plugin = 0;
	EngineMan.findGame(((GargoyleEngine *)g_engine)->getGameId(), &plugin);
	GUI::SaveLoadChooser *dialog;
	if (saveFlag)
		dialog = new GUI::SaveLoadChooser("Save game:", "Save");
	else
		dialog = new GUI::SaveLoadChooser("Load game:", "Load");

	dialog->setSaveMode(saveFlag);
	_saveSlot = dialog->runModal(plugin, ConfMan.getActiveDomainName());

	if ((_saveSlot >= 0) && saveFlag) {
		Common::String result(dialog->getResultString());
		if (!result.empty())
			strncpy(_saveDescription, result.c_str(), 99);
		else {
			// If the user was lazy and entered no save name, come up with a default name.
			char buf[20];
			snprintf(buf, 20, "Save %d", _saveSlot + 1);

			strcpy(_saveDescription, buf);
		}
	}

	delete dialog;
}

void Interpreter::getSaveFilename(char *saveName, int slotNumber) {
	sprintf(saveName, "%s.s%.2d", _engine.getGameId(), slotNumber);
}

uint8 Interpreter::getDefaultColour(bool fore) const {
	return fore ?  FOREGROUND_COLOUR : BACKGROUND_COLOUR;
}

uint8 Interpreter::getColourIndex(uint32 rgb) const {
	uint32 palette[256];
	g_system->grabPalette((byte *)&palette[0], 0, 256);

	for (int i = 0; i < 256; ++i) {
		if (palette[i] == rgb)
			return i;
	}

	return 0;
}

bool Interpreter::isValidChar(unsigned short c) {
	if (c >= ZC_ASCII_MIN && c <= ZC_ASCII_MAX)
		return true;
	if (c >= ZC_LATIN1_MIN && c <= ZC_LATIN1_MAX)
		return true;
	if (c >= 0x100)
		return true;

	return false;
}

} // End of namespace Gargoyle
