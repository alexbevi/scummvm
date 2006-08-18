/* ScummVM - Scumm Interpreter
 * Copyright (C) 2005-2006 The ScummVM project
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

#ifndef KOM_H
#define KOM_H

#include "base/engine.h"
#include "common/fs.h"

#include "kom/screen.h"
#include "kom/database.h"
#include "kom/input.h"
#include "kom/debugger.h"

class FilesystemNode;

namespace Kom {

class Screen;
class Database;
class Debugger;

class KomEngine : public Engine {
public:
	KomEngine(OSystem *system);
	~KomEngine();
	
	int init();
	int go();

	void errorString(const char *buf1, char *buf2);

	int getSelectedChar() { return _selectedChar; }
	int getSelectedQuest() { return _selectedQuest; }

private:
	void gameLoop();

	Screen *_screen;
	Database *_database;
	Input *_input;
	Debugger *_debugger;

	FilesystemNode *_fsNode;
	int _selectedChar; // 0 - thidney. 1 - shahron
	int _selectedQuest;
	bool _quit;
};

} // End of namespace Kom

#endif
