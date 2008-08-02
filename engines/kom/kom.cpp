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

#include "common/util.h"
#include "common/file.h"

#include "kom/kom.h"
#include "kom/actor.h"
#include "kom/database.h"
#include "kom/input.h"
#include "kom/sound.h"
#include "kom/game.h"

using Common::File;
using Common::String;

namespace Kom {

KomEngine::KomEngine(OSystem *system)
	: Engine(system) {
	_screen = 0;
	_database = 0;
	_actorMan = 0;
	_input = 0;
	_sound = 0;
	_game = 0;
	_gameLoopState = GAMELOOP_RUNNING;

	_fsNode = new FilesystemNode(_gameDataPath);
}

KomEngine::~KomEngine() {
	delete _fsNode;
	delete _screen;
	delete _database;
	delete _actorMan;
	delete _input;
	delete _sound;
	delete _debugger;
	delete _panel;
	delete _game;
}

int KomEngine::init() {
	_actorMan = new ActorManager(this);

	_screen = new Screen(this, _system);
	assert(_screen);
	if (!_screen->init())
		error("_screen->init() failed");

	_database = new Database(this, _system);
	_input = new Input(this, _system);
	_sound = new Sound(this, _mixer);
	_debugger = new Debugger(this);
	_panel = new Panel(this, _fsNode->getChild("kom").getChild("oneoffs").getChild("pan1.img"));
	_game = new Game(this, _system);

	// Init the following:
	/*
	 * sprites
	 * audio
	 * video
	 * actor
	 * flic
	 * panel
	 * menu
	 */


	return 0;
}

int KomEngine::go() {
	// Stuff to do here
	/*
	 * Play sci logo movie
	 * load inventory icons
	 * load sounds
	 * play intro movie
	 * choose character
	 * choose quest
	 * check cd
	 */

	FilesystemNode installDir(_fsNode->getChild("install"));
	if (_game->player()->selectedChar == 0) {
		File::addDefaultDirectory(installDir.getChild("db0"));
		_database->init("thid");
	} else {
		File::addDefaultDirectory(installDir.getChild("db1"));
		_database->init("shar");
	}

	_actorMan->loadMouse(_fsNode->getChild("kom").getChild("oneoffs"), String("m_icons"));

	_screen->showMouseCursor(true);

	_panel->enable(true);

	// temporary test
	_game->setDay();

	while (_gameLoopState != GAMELOOP_QUIT) {
		gameLoop();
	}

	return 0;
}

void KomEngine::errorString(const char *buf1, char *buf2) {
	strcpy(buf2, buf1);
}

void KomEngine::gameLoop() {
	// TODO:
	// setFrameRate(24)
	// setBrightness(256)
	// clearWorkScreen
	// init some global vars, Player settings
	_game->player()->command = CMD_NOTHING;
	_game->player()->commandState = 0;
	_database->getChar(0)->_isBusy = false;
	_game->settings()->objectNum = _game->settings()->object2Num = -1;
	_game->settings()->overType = _game->settings()->oldOverType = 0;
	_game->settings()->overNum = _game->settings()->oldOverNum = -1;
	// init something in the procs struct
	// init some more vars
	// some tricks with the loop input based on day/night

	_game->player()->isNight = (_game->settings()->dayMode == 1 || _game->settings()->dayMode == 3) ? 1 : 0;
	// fadeTo(target = 256, speed = 16)
	// TODO: loop actually starts with the menu, and then switches to RUNNING
	_gameLoopState = GAMELOOP_RUNNING;
	_flicLoaded = 2;

	// Main game loop
	while (_gameLoopState == GAMELOOP_RUNNING) {

		if (_debugger->isAttached()) {
			_debugger->onFrame();
		}

		if (_game->player()->sleepTimer == 0)
			_input->loopInput();

		_game->processTime();

		//if (_gameLoopState == GAMELOOP_DEATH)
			// ambientStart(currLocation)
		_game->loopMove();
		_game->loopCollide();
		// if in a fight, do something
		// collision stuff
		if (_game->player()->commandState != 0) {
			switch (_game->player()->command) {
			case 0x64:
				_game->player()->command = CMD_NOTHING;
				_game->player()->commandState = 0;
				_game->player()->collideType = 0;
				_game->player()->collideNum = -1;
				break;

			case 0x66:
				break;

			case 0x67:
				break;
			case 0x68:
				break;
			case 0x69:
				break;
			case 0x6A:
				break;
			case 0x6B:
				break;
			case 0x6C:
				break;
			case 0x65:
			default:
				error("Illegal player mode");
			}
		}
		// if player stopped:
		//     do command (walk, use, talk, pickup, look, fight, exit, magic)
		_game->loopSpriteCut();
		// loopSpells
		_game->loopTimeouts();
		// lose/get gold
		// stop narrator if needed
		// handle angry mob
		// handle greetings
		//
		// TODO

		// TODO - handle other graphics modes
		if (_game->player()->sleepTimer != 0)
			_screen->processGraphics(0);
		else if (_flicLoaded < 2)
			_screen->processGraphics(1);
		else
			_screen->processGraphics(2);

		if (_flicLoaded != 0)
			_flicLoaded--;

		if (_flicLoaded == 0)
			_game->loopInterfaceCollide();

		if (_input->debugMode()) {
			_input->resetDebugMode();
			_debugger->attach();
		}
	}

	// stopNarrator()
	// stopGreeting()
	// ambientStop()
	if (_gameLoopState == GAMELOOP_DEATH) {
		// TODO
		// play death video
		// play credits
	}

}

} // End of namespace Kom
