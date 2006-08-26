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

#ifndef KOM_DATABASE_H
#define KOM_DATABASE_H

#include "common/scummsys.h"
#include "common/str.h"
#include "common/list.h"
#include "base/engine.h"

//class OSystem;

namespace Kom {

class KomEngine;

struct EventLink {
	int data1;
	int data2;
};

struct Object {
	Object() : data1(0), type(0), data2(0), proc(0),
	           data4(0), data5(0), isVisible(0), data6(0), isSprite(0), isUseImmediate(0),
	           data9(0), data10(0), price(0), data11(0), spellCost(0), data12(0),
	           data13(0), locationType(0), locationId(0), data15(0), data16(0), data17(0),
	           data18(0) {}

	char name[7];
	int data1;
	char desc[50];
	int type;
	int data2;
	int proc;
	int data4;
	int data5;
	int isVisible;
	int data6;
	int isSprite;
	int isUseImmediate;
	int data9;
	int data10;
	int price;
	int data11;
	int spellCost;
	int data12;
	int data13;
	int locationType;
	int locationId;
	int data15;
	int data16;
	int data17;
	int data18;
};

struct Character {
	char name[7];
	int xtend;
	int data2;
	char desc[50];
	int proc;
	int locationId;
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
	Common::List<int> inventory;
};

struct Location {
	char name[7];
	int xtend;
	int data2;
	char desc[50];
	Common::List<EventLink> events;
	Common::List<int> objects;
	Common::List<int> characters;
};

struct OpCode {
	int opcode;
	char arg1[30];
	int arg2;
	int arg3;
	int arg4;
	int arg5;
	int arg6;
};

struct Command {
	int cmd;
	uint16 value;
	Common::List<OpCode> opcodes;
};

struct Process {
	char name[30];
	Common::List<Command> commands;
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
	void initObjectLocs();
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

	Process *_processes;
	int _procsNum;

	int _varSize;
	uint16 *_dataSegment;

	int _convIndexSize;
	byte *_convIndex;
};
} // End of namespace Kom

#endif
