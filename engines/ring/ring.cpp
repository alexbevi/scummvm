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

#include "ring/base/application.h"
#include "ring/base/event.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"

#include "ring/game/application_faust.h"
#include "ring/game/application_jerusalem.h"
#include "ring/game/application_pilgrim2.h"
#include "ring/game/application_pilgrim3.h"
#include "ring/game/application_pompeii.h"
#include "ring/game/application_ring.h"

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/EventRecorder.h"

#include "engines/util.h"

namespace Ring {

RingEngine::RingEngine(OSystem *syst, const RingGameDescription *gd) :
	Engine(syst), _gameDescription(gd), _debugger(NULL),
	_application(NULL), _mouseButtonPressed(false) , _flag(true) {

	// Adding the default directories
	const Common::FSNode gameDataDir(ConfMan.get("path"));

	// Add the game path to the directory search list.
	// ScummVM defaults to 4, we needs 5
	SearchMan.remove(gameDataDir.getPath());
	SearchMan.addDirectory(gameDataDir.getPath(), gameDataDir, 0, 5);

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
	// Delete application
	delete _application;

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

	// Create application
	switch (_gameDescription->gameType) {
	default:
		error("[RingEngine::run] Unknown game type!");
		break;

	case Ring::GameTypePilgrim:
		error("[RingEngine::run] Pilgrim support not implemented yet!");

	case Ring::GameTypeRing:
		_application = new ApplicationRing(this);
		break;

	case Ring::GameTypeFaust:
		_application = new ApplicationFaust(this);
		break;

	case Ring::GameTypePompeii:
		_application = new ApplicationPompeii(this);
		break;

	case Ring::GameTypePilgrim2:
		_application = new ApplicationPilgrim2(this);
		break;

	case Ring::GameTypePilgrim3:
		_application = new ApplicationPilgrim3(this);

	case Ring::GameTypeJerusalem:
		_application = new ApplicationJerusalem(this);
		break;
	}

	// Init application
	_application->init();
	_application->setup();
	_application->initZones();

	// Start application
	_application->showStartupScreen();
	_application->startMenu(false);

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

				_application->getEventHandler()->onKeyDown(ev);
				break;

			case Common::EVENT_MAINMENU:
				// Closing the GMM

			case Common::EVENT_MOUSEMOVE:
				_coordinates = ev.mouse;
				break;

			case Common::EVENT_LBUTTONUP:
				_mouseButtonPressed = false;

				// Check if CTRL is pressed
				_application->getEventHandler()->onMouseLeftButtonUp(ev, (_eventMan->getModifierState() & Common::KBD_CTRL) != 0);
				break;

			case Common::EVENT_LBUTTONDOWN:
				_mouseButtonPressed = true;

				if (ev.mouse.y <= 464)
					_application->getEventHandler()->onMouseLeftButtonDown(ev);
				break;

			case Common::EVENT_RBUTTONUP:
				_application->getEventHandler()->onMouseRightButtonUp(ev);
				break;

			case Common::EVENT_QUIT:
				quitGame();
				break;

			default:
				break;
			}
		}

		// Draw screen, update state and play sounds
		update();

		// Update the screen
		_system->updateScreen();
		_system->delayMillis(10);

		// The event loop may have triggered the quit status. In this case,
		// stop the execution.
		if (shouldQuit())
			continue;
	}

	return Common::kNoError;
}

bool RingEngine::pollEvents(bool updateCoords) {
	Common::Event ev;
	_eventMan->pollEvent(ev);

	switch (ev.type) {
	default:
		break;

	case Common::EVENT_MOUSEMOVE:
		if (updateCoords)
			_coordinates = ev.mouse;
		break;

	case Common::EVENT_KEYDOWN:
		if (ev.kbd.keycode == Common::KEYCODE_ESCAPE)
			return true;
		break;
	}

	return false;
}

void RingEngine::update() {
	if (!_application)
		error("[RingEngine::update] Application not initialized properly");

	_application->draw();
	_application->getSoundManager()->updateQueue();
}

bool RingEngine::isMultiLanguage() {
	// Check if the game supports multiple languages
	switch (_gameDescription->features) {
	default:
		return true;

	case LANG_ENG:
	case LANG_FRA:
	case LANG_GER:
	case LANG_GRE:
	case LANG_HEB:
	case LANG_HOL:
	case LANG_ITA:
	case LANG_SLO:
	case LANG_SPA:
	case LANG_SWE:
		return false;
	}

	return false;
}

void RingEngine::errorString(const char *buf_input, char *buf_output, int buf_output_size) {
	snprintf(buf_output, (uint)buf_output_size, "%s", buf_input);
}

} // End of namespace Ring
