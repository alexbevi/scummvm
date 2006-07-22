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

using Common::File;

namespace Kom {

KomEngine::KomEngine(OSystem *system)
	: Engine(system) {
	_screen = 0;
	_database = 0;

	_fsNode = new FilesystemNode(_gameDataPath);

	// Temporary
	_selectedChar = _selectedQuest = 0;
}

KomEngine::~KomEngine() {
	delete _fsNode;
	delete _screen;
	delete _database;
}

int KomEngine::init() {
	_screen = new Screen(this, _system);
	assert(_screen);
	if (!_screen->init())
		error("_screen->init() failed");

	// Init the following:
	/*
	 * sprites
	 * audio
	 * video
	 * input
	 * actor
	 * flic
	 * panel
	 * text
	 * menu
	 */

	_database = new Database(this, _system);

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
	 */

	FilesystemNode installDir(_fsNode->getChild("install"));
	if (_selectedChar == 0) {
		File::addDefaultDirectory(installDir.getChild("db0"));
		_database->init("thid");
	} else {
		File::addDefaultDirectory(installDir.getChild("db1"));
		_database->init("shar");
	}

	return 0;
}
} // End of namespace Kom
