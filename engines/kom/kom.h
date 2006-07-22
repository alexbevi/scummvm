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

#ifndef __KOM_H__
#define __KOM_H__

#include "base/engine.h"
#include "common/fs.h"

#include "kom/screen.h"
#include "kom/database.h"

class FilesystemNode;

namespace Kom {

class Screen;
class Database;

class KomEngine : public Engine {
public:
	KomEngine(OSystem *system);
	~KomEngine();
	
	int init();
	int go();

	int getSelectedChar() { return _selectedChar; }
	int getSelectedQuest() { return _selectedQuest; }

private:
	Screen *_screen;
	Database *_database;

	FilesystemNode *_fsNode;
	int _selectedChar; // 0 - thidney. 1 - shahron
	int _selectedQuest;
};

} // End of namespace Kom

#endif
