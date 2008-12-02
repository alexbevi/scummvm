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

#include <stdio.h>

#include "common/util.h"
#include "common/file.h"
#include "common/error.h"
#include "common/archive.h"

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
	_playingMusicId = _playingMusicVolume = 0;
}

KomEngine::~KomEngine() {
	delete _screen;
	delete _database;
	delete _actorMan;
	delete _input;
	delete _sound;
	delete _debugger;
	delete _panel;
	delete _game;
}

Common::Error KomEngine::init() {
	// Add the game path to the directory search list.
	// ScummVM defaults to 4, KoM needs 5
	SearchMan.remove(_gameDataDir.getPath());
	SearchMan.addDirectory(_gameDataDir.getPath(), _gameDataDir, 0, 5);

	_actorMan = new ActorManager(this);

	_screen = new Screen(this, _system);
	assert(_screen);
	if (!_screen->init())
		error("_screen->init() failed");

	_database = new Database(this, _system);
	_input = new Input(this, _system);
	_sound = new Sound(this, _mixer);
	_debugger = new Debugger(this);
	_panel = new Panel(this, "kom/oneoffs/pan1.img");
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


	return Common::kNoError;
}

Common::Error KomEngine::go() {
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

	// Load sound effects
	static String samplesDir("kom/samples/");
	_hotspotSample.loadFile(samplesDir + "hotspot.raw");
	_doorsSample.loadFile(samplesDir + "doors.raw");
	_clickSample.loadFile(samplesDir + "mouse_l.raw");
	_swipeSample.loadFile(samplesDir + "swipe.raw");
	_cashSample.loadFile(samplesDir + "cash.raw");
	_loseItemSample.loadFile(samplesDir + "loseitem.raw");

	if (_game->player()->selectedChar == 0) {
		_database->init("thid");
	} else {
		_database->init("shar");
	}

	_actorMan->loadExtras();

	_screen->showMouseCursor(true);

	_panel->enable(true);

	// temporary test
	_game->setDay();

	while (_gameLoopState != GAMELOOP_QUIT) {
		gameLoop();
		_panel->setLocationDesc("");
		_panel->setActionDesc("");
		_panel->setHotspotDesc("");
	}

	return Common::kNoError;
}

void KomEngine::errorString(const char *buf1, char *buf2) {
	strcpy(buf2, buf1);
}

void KomEngine::gameLoop() {
	// TODO:
	// setFrameRate(24)
	_screen->setBrightness(256);
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
	_game->cb()->talkInitialized = false;
	// TODO: loop actually starts with the menu, and then switches to RUNNING
	_gameLoopTimer = 0;
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

		if (_gameLoopTimer == 1)
			ambientStart(_database->getChar(0)->_lastLocation);

		_game->loopMove();
		_game->loopCollide();

		// TODO: if in a fight, do something

		if (_game->player()->commandState != 0 && _database->getChar(0)->_stopped) {
			switch (_game->player()->command) {
			case CMD_WALK:
				_game->player()->command = CMD_WALK;
				_game->player()->commandState = 0;
				_game->player()->collideType = 0;
				_game->player()->collideNum = -1;
				break;

			case CMD_USE:
				_game->exeUse();
				break;

			case CMD_TALK_TO:
				break;
			case CMD_PICKUP:
				_game->exePickup();
				break;
			case CMD_LOOK_AT:
				_game->exeLookAt();
				break;
			case CMD_FIGHT:
				break;
			case CMD_CAST_SPELL:
				_game->exeMagic();
				break;

			// Unused
			case 0x6C:
				break;

			case CMD_NOTHING:
			default:
				error("Illegal player mode");
			}
		}

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

		if (_gameLoopTimer++ > 0x20000)
			_gameLoopTimer -= 0x10000;

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

void KomEngine::ambientStart(int locId) {

	char musicIdStr[11];

	// Each loc has 3 values:
	// 1) Night music
	// 2) Day music
	// 3) Volume diff
	static int16 musicTable[] = {
		0, 0, 0, 290, 290, 30, 110, 110, 30, 80, 90, -25,
		480, 480, 0, 250, 250, 0, 210, 210, 0, 290, 290, 50,
		290, 290, 30, 240, 240, 0, 240, 240, 0, 40, 40, 0,
		220, 220, 0, 230, 230, 0, 220, 220, 0, 240, 240, 0,
		240, 240, 0, 240, 240, 0, 110, 110, 30, 20, 20, 0,
		110, 110, 50, 110, 110, 75, 260, 260, 0, 240, 240, 0,
		20, 20, 0, 10, 10, 0, 190, 201, 0, 240, 240, 0,
		420, 420, 0, 420, 420, 30, 190, 201, 0, 270, 270, 0,
		270, 270, 0, 40, 40, 0, 20, 20, 0, 20, 20, 0,
		20, 20, 0, 40, 40, 0, 20, 20, 0, 290, 290, 0,
		290, 290, 0, 280, 280, 25, 280, 280, 15, 280, 280, 0,
		240, 240, 0, 110, 110, 60, 430, 430, 0, 380, 380, 0,
		250, 250, 25, 250, 250, 0, 410, 410, 0, 80, 121, 0,
		80, 90, -25, 110, 110, 50, 101, 101, 0, 240, 240, 0,
		320, 320, 50, 350, 350, 0, 30, 30, 0, 320, 320, 75,
		460, 141, 0, 470, 181, 0, 460, 141, 0, 130, 141, 25,
		330, 330, 40, 110, 110, 30, 250, 250, 25, 240, 240, 0,
		440, 440, 0, 240, 240, 0, 240, 240, 0, 370, 370, 0,
		400, 400, 0, 240, 240, 0, 70, 70, 0, 70, 70, 0,
		70, 70, 0, 70, 70, 0, 340, 340, 0, 50, 50, 0,
		70, 70, 0, 70, 70, 0, 310, 310, 0, 70, 70, 0,
		240, 240, 0, 330, 330, 50, 360, 360, 0, 130, 141, 0,
		130, 141, 0, 260, 260, 60, 170, 181, 0, 260, 260, 0,
		40, 40, 0, 60, 60, 0, 260, 260, 0, 240, 240, 0,
		240, 240, 0, 240, 240, 0, 260, 260, 0, 220, 220, 0,
		290, 290, 0, 250, 250, 0, 300, 300, 0, 240, 240, 0,
		20, 20, 0, 260, 260, 0, 360, 360, 0, 360, 360, 0,
		40, 40, 0, 240, 240, 0
	};

	int16 musicId, musicVolume;

	static String musicDir("kom/music/");

	// Special handling for the honeymoon suite
   	if (locId == 21 && _database->getLoc(locId)->xtend == 2) {
		musicId = 500;
		musicVolume = 35;
	} else {
		musicId = musicTable[locId * 3 + _game->player()->isNight];
		musicVolume = musicTable[locId * 3 + 2];
	}

	// TODO: use volume information

	if (musicId != _playingMusicId && _ambientSample.isLoaded()) {
		_sound->stopSample(_ambientSample);
		_ambientSample.unload();
	}

	if (musicId == 0) {
		_playingMusicId = 0;
		_playingMusicVolume = 0;

	} else if (musicId != _playingMusicId) {

		sprintf(musicIdStr, "amb%d.raw", musicId);
		_ambientSample.loadFile(musicDir + musicIdStr);
		_sound->playSampleMusic(_ambientSample);

		_playingMusicId = musicId;
		_playingMusicVolume = musicVolume;

	// TODO: Change volume
	} else if (musicVolume != _playingMusicVolume ) {
	}

}

} // End of namespace Kom
