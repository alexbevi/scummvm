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

#ifndef KOM_DATABASE_H
#define KOM_DATABASE_H

#include "common/str.h"
#include "common/list.h"
#include "common/file.h"
#include "engines/engine.h"

#include "kom/character.h"
#include "kom/actor.h"

#if defined(__GNUC__)
	#define KOM_GCC_SCANF(x,y) __attribute__((format(scanf, x, y)))
#else
	#define KOM_GCC_SCANF(x,y)
#endif

namespace Kom {

class KomEngine;

struct EventLink {
	int exitBox;
	int proc;
};

struct Object {
	Object() : data1(0), type(0), data2(0), proc(0),
	           data4(0), isCarryable(0), isContainer(0), isVisible(0), isSprite(0), isUseImmediate(0),
	           isPickable(0), isUsable(0), price(0), data11(0), spellCost(0), data12(0),
	           data13(0), ownerType(0), ownerId(0), box(0), data16(0), data17(0),
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
	int isPickable;
	int isUsable;
	int price;
	int data11;
	int spellCost;
	int data12;
	int data13;
	int ownerType;
	int ownerId;
	int box;
	int data16;
	int data17;
	int data18;
	Common::List<int> contents;
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
	Box() : enabled(false) {}
	bool enabled;
	int16 x1, y1, x2, y2, priority;
	int32 z1, z2;
	uint8 attrib;
	int8 joins[6];
};

struct LocRoute {
	Exit exits[6];
	Box boxes[32];
};

class Database {
public:
	Database(KomEngine *vm, OSystem *system);
	~Database();

	void init(Common::String databasePrefix);

	static void stripUndies(char *s);
	int CDECL readLineScanf(Common::File &f, const char *format, ...) KOM_GCC_SCANF(3, 4);

	int8 loc2loc(int from, int to) { return (int8)(_routes[(int8)(_routes[0]) * to + from + 1]); }
	int8 box2box(int loc, int fromBox, int toBox);

	Object *object(int obj) const { return &(_objects[obj]); }

	Box *getBox(int locId, int boxId) const { return &(_locRoutes[locId].boxes[boxId]); }
	Exit *getExits(int locId) const { return _locRoutes[locId].exits; }

	int8 whatBox(int locId, int x, int y);
	int8 whatBoxLinked(int locId, int8 boxId, int x, int y);
	int16 getMidX(int loc, int box) { return _locRoutes[loc].boxes[box].x1 +
		(_locRoutes[loc].boxes[box].x2 - _locRoutes[loc].boxes[box].x1) / 2; }
	int16 getMidY(int loc, int box) { return _locRoutes[loc].boxes[box].y1 +
		(_locRoutes[loc].boxes[box].y2 - _locRoutes[loc].boxes[box].y1) / 2; }
	int16 getMidOverlapX(int loc, int box1, int box2);
	int16 getMidOverlapY(int loc, int box1, int box2);
	int16 getPriority(int loc, int box) { return _locRoutes[loc].boxes[box].priority; }
	int16 getZValue(int loc, int box, int32 y);
	uint16 getExitBox(int currLoc, int nextLoc);
	int8 getBoxLink(int loc, int box, int join);
	int8 getFirstLink(int loc, int box);
	bool getExitInfo(int loc, int box, int *exitLoc, int *exitBox);
	bool isInLine(int loc, int box, int x, int y);
	void getClosestBox(int loc, uint16 mouseX, uint16 mouseY,
			int16 screenX, int16 screenY,
			int16 *collideBox, uint16 *collideBoxX, uint16 *collideBoxY);

	void setCharPos(int charId, int loc, int box);
	bool giveObject(int obj, int charId, bool something);

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
