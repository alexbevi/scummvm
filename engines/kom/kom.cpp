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

#include "common/stdafx.h"
#include "common/util.h"
#include "common/file.h"

#include "kom/kom.h"
#include "kom/actor.h"
#include "kom/database.h"
#include "kom/input.h"

using Common::File;
using Common::String;

namespace Kom {

KomEngine::KomEngine(OSystem *system)
	: Engine(system) {
	_screen = 0;
	_database = 0;
	_actorMan = 0;
	_input = 0;
	_gameLoopState = GAMELOOP_RUNNING;

	_fsNode = new FilesystemNode(_gameDataPath);

	// FIXME: Temporary
	_selectedChar = _selectedQuest = 0;
}

KomEngine::~KomEngine() {
	delete _fsNode;
	delete _screen;
	delete _database;
	delete _actorMan;
	delete _input;
	delete _debugger;
	delete _panel;
}

int KomEngine::init() {
	_actorMan = new ActorManager(this);

	_screen = new Screen(this, _system);
	assert(_screen);
	if (!_screen->init())
		error("_screen->init() failed");

	_database = new Database(this, _system);
	_input = new Input(this, _system);
	_debugger = new Debugger(this);
	_panel = new Panel(this, _fsNode->getChild("kom").getChild("oneoffs").getChild("pan1.img"));

	// Init the following:
	/*
	 * sprites
	 * audio
	 * video
	 * actor
	 * flic
	 * panel
	 * text
	 * menu
	 */


	return 0;
}

int KomEngine::go() {
	// Stuff to do here
	/*
	 * Play sci logo movie
	 * load inventory icons
	 * load fonts
	 * load sounds
	 * play intro movie
	 * choose character
	 * choose quest
	 * check cd
	 */

	FilesystemNode installDir(_fsNode->getChild("install"));
	if (_selectedChar == 0) {
		File::addDefaultDirectory(installDir.getChild("db0"));
		_database->init("thid");
	} else {
		File::addDefaultDirectory(installDir.getChild("db1"));
		_database->init("shar");
	}

	_actorMan->loadMouse(_fsNode->getChild("kom").getChild("oneoffs"), String("m_icons"));

	_screen->showMouseCursor(true);

	// temporary test
	_screen->loadBackground(_fsNode->getChild("kom").getChild("locs").getChild("ft").getChild("ftroad").getChild("ftroad0b.flc"));

	while (_gameLoopState != GAMELOOP_QUIT) {
		gameLoop();
	}

	return 0;
}

void KomEngine::errorString(const char *buf1, char *buf2) {
	strcpy(buf2, buf1);
}

void KomEngine::gameLoop() {
	if (_debugger->isAttached()) {
		_debugger->onFrame();
	}

	// setFrameRate(24)
	// setBrightness(256)
	// clearWorkScreen
	// init some global vars
	// fadeTo(target = 256, speed 16)
	// init something in the procs struct
	// init some more vars
	// some tricks with the loop input based on day/night
	// processChars
	// conditioned: ambientStart
	// loopMove
	// loopCollide
	// TODO more

	_screen->update();
	_input->checkKeys();
	if (_input->debugMode()) {
		_input->resetDebugMode();
		_debugger->attach();
	}
}

} // End of namespace Kom
