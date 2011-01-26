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

#include "ring/ring.h"

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/EventRecorder.h"

#include "engines/util.h"

namespace Ring {

RingEngine::RingEngine(OSystem *syst, const ADGameDescription *gd) :
	Engine(syst), _gameDescription(gd), _debugger(NULL) {

	// Adding the default directories
	const Common::FSNode gameDataDir(ConfMan.get("path"));
	SearchMan.addSubDirectoryMatching(gameDataDir, "data");
	
	// Initialize the custom debug levels
	DebugMan.addDebugChannel(kRingDebugAll, "All", "Debug everything");
	DebugMan.addDebugChannel(kRingDebugGraphics, "Graphics", "Debug graphics & video playback");
	DebugMan.addDebugChannel(kRingDebugResource, "Resource", "Debug resource management");
	DebugMan.addDebugChannel(kRingDebugSound, "Sound", "Debug sound playback");
	DebugMan.addDebugChannel(kRingDebugSavegame, "Savegame", "Debug savegame");
	DebugMan.addDebugChannel(kRingDebugLogic, "Logic", "Debug logic");
	DebugMan.addDebugChannel(kRingDebugUnknown, "Unknown", "Debug unknown data");

	g_eventRec.registerRandomSource(_random, "Ring");
}

RingEngine::~RingEngine() {
	// Delete the remaining objects
	delete _debugger;

	// Zero passed pointers
	_gameDescription = NULL;
}

Common::Error RingEngine::run() {
	// Initialize the graphics
	const Graphics::PixelFormat dataPixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0);
	initGraphics(640, 480, true, &dataPixelFormat);

	// We do not support color conversion
	if (_system->getScreenFormat() != dataPixelFormat)
		return Common::kUnsupportedColorMode;

	// Create debugger. It requires GFX to be initialized
	_debugger = new Debugger(this);

	while (!shouldQuit()) {
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

bool RingEngine::hasFeature(EngineFeature f) const {
	return (f == kSupportsRTL);
}

void RingEngine::errorString(const char *buf_input, char *buf_output, int buf_output_size) {
	snprintf(buf_output, (uint)buf_output_size, "%s", buf_input);
}

} // End of namespace Ring
