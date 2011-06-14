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
 * $Id: liath.cpp 391 2010-05-04 16:07:30Z julien.templier $
 *
 */

#include "liath/liath.h"

#include "liath/data/resource.h"
#include "liath/data/segment.h"

#include "liath/game/action.h"
#include "liath/game/array.h"
#include "liath/game/expression.h"
#include "liath/game/game.h"
#include "liath/game/graphics.h"
#include "liath/game/hero.h"
#include "liath/game/mouse.h"
#include "liath/game/position.h"
#include "liath/game/progress.h"
#include "liath/game/savegame.h"
#include "liath/game/sound.h"
#include "liath/game/text.h"
#include "liath/game/position.h"
#include "liath/game/work.h"

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/EventRecorder.h"

#include "engines/util.h"

namespace Liath {

LiathEngine::LiathEngine(OSystem *syst, const ADGameDescription *gd) :
    Engine(syst), _gameDescription(gd), _debugger(NULL), _random("liath"),
	_actionMan(NULL), _arrayMan(NULL), _expressionMan(NULL), _gameMan(NULL),
	_graphicsMan(NULL), _heroMan(NULL), _mouseMan(NULL), _positionMan(NULL),
	_progressMan(NULL), _resMan(NULL), _saveMan(NULL), _segmentMan(NULL),
	_soundMan(NULL), _textMan(NULL), _workMan(NULL) {

	// Adding the default directories
	const Common::FSNode gameDataDir(ConfMan.get("path"));
	SearchMan.addSubDirectoryMatching(gameDataDir, "cut");
	SearchMan.addSubDirectoryMatching(gameDataDir, "data");
	SearchMan.addSubDirectoryMatching(gameDataDir, "dialog");
	SearchMan.addSubDirectoryMatching(gameDataDir, "heros");

	// Initialize the custom debug levels
	DebugMan.addDebugChannel(kLiathDebugAll, "All", "Debug everything");
	DebugMan.addDebugChannel(kLiathDebugGraphics, "Graphics", "Debug graphics & video playback");
	DebugMan.addDebugChannel(kLiathDebugResource, "Resource", "Debug resource management");
	DebugMan.addDebugChannel(kLiathDebugSound, "Sound", "Debug sound playback");
	DebugMan.addDebugChannel(kLiathDebugSavegame, "Savegame", "Debug savegame");
	DebugMan.addDebugChannel(kLiathDebugLogic, "Logic", "Debug logic");
	DebugMan.addDebugChannel(kLiathDebugUnknown, "Unknown", "Debug unknown data");
}

LiathEngine::~LiathEngine() {
	// Delete the remaining objects
	delete _debugger;

	delete _actionMan;
	delete _arrayMan;
	delete _expressionMan;
	delete _gameMan;
	delete _graphicsMan;
	delete _heroMan;
	delete _mouseMan;
	delete _positionMan;
	delete _progressMan;
	delete _resMan;
	delete _saveMan;
	delete _segmentMan;
	delete _soundMan;
	delete _textMan;
	delete _workMan;

	// Zero passed pointers
	_gameDescription = NULL;
}

// TODO: which error should we return when some game files are missing/corrupted?
Common::Error LiathEngine::run() {
	// Initialize the graphics
	const Graphics::PixelFormat dataPixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0);
	initGraphics(640, 480, true, &dataPixelFormat);

	// We do not support color conversion
	if (_system->getScreenFormat() != dataPixelFormat)
		return Common::kUnsupportedColorMode;

	// Create debugger. It requires GFX to be initialized
	_debugger = new Debugger(this);

	// Create managers
	_actionMan     = new ActionManager(this);
	_arrayMan      = new ArrayManager(this);
	_expressionMan = new ExpressionManager(this);
	_gameMan       = new GameManager(this);
	_graphicsMan   = new GraphicsManager(this);
	_heroMan       = new HeroManager(this);
	_mouseMan      = new MouseManager(this);
	_positionMan   = new PositionManager(this);
	_progressMan   = new ProgressManager(this);
	_resMan        = new ResourceManager();
	_saveMan       = new SavegameManager(this);
	_segmentMan    = new SegmentManager(this);
	_soundMan      = new SoundManager(this);
	_textMan       = new TextManager(this);
	_workMan       = new WorkManager(this);

	// Setup data
	_resMan->readPathFile();
	_resMan->readMultiData();

	_gameMan->playGame(kActionNone);

	while (!shouldQuit()) {
		// Execute stored commands
		if (_debugger->hasCommand()) {
			_debugger->callCommand();

			// re-attach the debugger
			_debugger->attach();
		}

		// Show the debugger if required
		_debugger->onFrame();

		// Handle input
		Common::Event ev;
		while (_eventMan->pollEvent(ev)) {
			switch (ev.type) {

			case Common::EVENT_KEYDOWN:
				// CTRL-D: Attach the debugger
				if ((ev.kbd.flags & Common::KBD_CTRL) && ev.kbd.keycode == Common::KEYCODE_d)
					_debugger->attach();

				// DEBUG: Quit game on escape
				if (ev.kbd.keycode == Common::KEYCODE_ESCAPE)
					quitGame();

				break;

			case Common::EVENT_MAINMENU:
				// Closing the GMM

			case Common::EVENT_MOUSEMOVE:
				break;

			case Common::EVENT_LBUTTONDOWN:
			case Common::EVENT_LBUTTONUP:
			case Common::EVENT_RBUTTONDOWN:
				break;

			case Common::EVENT_QUIT:
				quitGame();
				break;

			default:
				break;
			}
		}

		// Update the screen
		_system->updateScreen();
		_system->delayMillis(10);

		// The event loop may have triggered the quit status. In this case,
		// stop the execution.
		if (shouldQuit()) {
			continue;
		}
	}

	return Common::kNoError;
}

bool LiathEngine::hasFeature(EngineFeature f) const {
	return (f == kSupportsRTL);
}

void LiathEngine::errorString(const char *buf_input, char *buf_output, int buf_output_size) {
	snprintf(buf_output, (uint)buf_output_size, "%s", buf_input);
}

} // End of namespace Liath
