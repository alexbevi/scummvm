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

#ifndef __KOMDATABASE_H__
#define __KOMDATABASE_H__

#include "base/engine.h"
#include "common/str.h"
#include "common/fs.h"
#include "common/list.h"

//class OSystem;

namespace Kom {

class KomEngine;

struct EventLink {
	int data1;
	int data2;
};

struct Location {
	char name[7];
	int data1;
	int data2;
	char desc[50];
	Common::List<EventLink> *events;
};

struct Character {
	char name[7];
	int data1;
	int data2;
	char desc[50];
	int data3;
	int loc;
	int data4;
	int data5;
	int data6;
	int data7;
	int data8;
	int data9;
	int hitpoints;
	int data10;
	int data11;
	int data12;
	int data13;
	int data14; // spell speed - unused
	int data15; // spell time - unused
	int data16;
	int spellpoints;
};

struct Object {
	char name[7];
	int data1;
	char desc[50];
	int type;
	int data2;
	int data3;
	int data4;
	int data5;
	int data6;
	int data7;
	int isVisible;
	int data8;
	int data9;
	int data10;
	int price;
	int data11;
	int spellCost;
	int data12;
	int data13;
	int data14;
	int loc;
	int data15;
	int data16;
	int data17;
	int data18;
	// events list
};

class Database {
public:
	Database(KomEngine *vm, OSystem *system);
	~Database();

	void init(Common::String databasePrefix);

	static void stripUndies(char *s);

private:
	void loadConvIndex();
	void initLocations();
	void initCharacters();
	void initObjects();
	void initEvents();
	void initObjectsLocs();
	void initCharacterLocs();
	void initProcs();

	OSystem *_system;
	KomEngine *_vm;

	Common::String _databasePrefix;

	Location *_locations;
	int _locationsNum;

	Character *_characters;
	int _charactersNum;

	Object *_objects;
	int _objectsNum;
};
} // End of namespace Kom

#endif
