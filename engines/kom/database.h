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

#include "common/str.h"
#include "common/list.h"
#include "common/file.h"
#include "engines/engine.h"

#include "kom/actor.h"

#if defined(__GNUC__)
	#define KOM_GCC_SCANF(x,y) __attribute__((format(scanf, x, y)))
#else
	#define KOM_GCC_SCANF(x,y)
#endif

namespace Kom {

class KomEngine;

struct EventLink {
	int data1;
	int data2;
};

struct Object {
	Object() : data1(0), type(0), data2(0), proc(0),
	           data4(0), isCarryable(0), isContainer(0), isVisible(0), isSprite(0), isUseImmediate(0),
	           data9(0), isUsable(0), price(0), data11(0), spellCost(0), data12(0),
	           data13(0), locationType(0), locationId(0), box(0), data16(0), data17(0),
	           data18(0) {}

	char name[7];
	int data1;
	char desc[50];
	int type;
	int data2;
	int proc;
	int data4;
	int isCarryable;
	int isContainer;
	int isVisible;
	int isSprite;
	int isUseImmediate;
	int data9;
	int isUsable;
	int price;
	int data11;
	int spellCost;
	int data12;
	int data13;
	int locationType;
	int locationId;
	int box;
	int data16;
	int data17;
	int data18;
};

struct Character {
	Character() : mode(0), modeCount(0), isBusy(false), isAlive(true), isVisible(true),
		spellMode(0), gold(0) {}
	char name[7];
	int xtend;
	int data2;
	char desc[50];
	int proc;
	int locationId;
	int box;
	int data5;
	int data6;
	int data7;
	int data8;
	int data9;
	int hitpoints;
	uint16 mode;
	uint16 modeCount;
	bool isBusy;
	bool isAlive;
	bool isVisible;
	uint8 spellMode;
	int data10;
	int data11;
	int data12;
	int data13;
	int data14; // spell speed - unused
	int data15; // spell time - unused
	int data16;
	int spellpoints;
	int16 destLoc;
	int16 destBox;
	int gold;
	Common::List<int> inventory;
};

struct Location {
	char name[7];
	int xtend;
	int allowedTime;
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
	OpCode() : arg2(0), arg3(0), arg4(0), arg5(0), arg6(0) { arg1[0] = '\0'; }; 
};

struct Command {
	int cmd;
	uint16 value;
	Common::List<OpCode> opcodes;
	Command() : value(0) {};
};

struct Process {
	char name[30];
	Common::List<Command> commands;
};

struct Exit {
	int16 exit;
	int16 exitLoc;
	int16 exitBox;
};

struct Box {
	int16 x1, y1, x2, y2, priority;
	int32 z1, z2;
	int16 attrib;
	int8 joins[6];
};

struct LocRoute {
	Exit exits[6];
	Box boxes[32];
};

struct ActorScope {
	Scope scopes[18];
	uint16 speed1;
	uint16 speed2;
	uint16 timeout;
	uint32 start1;
	uint32 start2;
	uint32 start3;
	uint32 start4;
	uint32 start5;
};

class Database {
public:
	Database(KomEngine *vm, OSystem *system);
	~Database();

	void init(Common::String databasePrefix);

	static void stripUndies(char *s);
	int CDECL readLineScanf(Common::File &f, const char *format, ...) KOM_GCC_SCANF(3, 4);

	Location *location(uint16 locId) const { return &(_locations[locId]); }

	int8 loc2loc(int from, int to) { return ((int8)(_routes[(int8)(_routes[0]) * to + from + 1])); }

	Object *object(int obj) const { return &(_objects[obj]); }

	Box *getBox(int locId, int boxId) const { return &(_locRoutes[locId].boxes[boxId]); }
	Exit *getExits(int locId) const { return _locRoutes[locId].exits; }

	Process *getProc(uint16 procIndex) const { return procIndex < _procsNum ? &(_processes[procIndex]) : NULL; }
	Character *getChar(uint16 charIndex) const { return charIndex < _charactersNum ? &(_characters[charIndex]) : NULL; }
	Object *getObj(uint16 objIndex) const { return objIndex < _objectsNum ? &(_objects[objIndex]) : NULL; }
	Location *getLoc(uint16 locIndex) const { return locIndex < _locationsNum ? &(_locations[locIndex]) : NULL; }

	int charactersNum() { return _charactersNum; }

	int16 getVar(uint16 index) { assert(index < _varSize); return _variables[index]; }
	void setVar(uint16 index, int16 value) { assert(index < _varSize); _variables[index] = value; }

private:
	void loadConvIndex();
	void initLocations();
	void initCharacters();
	void initObjects();
	void initEvents();
	void initObjectLocs();
	void initCharacterLocs();
	void initProcs();
	void initRoutes();
	void initScopes();

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

	ActorScope *_actorScopes;

	int _varSize;
	int16 *_variables;

	int _convIndexSize;
	byte *_convIndex;

	int _mapSize;
	byte *_map;

	int _routesSize;
	byte *_routes;

	static const int _locRoutesSize;
	LocRoute *_locRoutes;
};
} // End of namespace Kom

#endif
