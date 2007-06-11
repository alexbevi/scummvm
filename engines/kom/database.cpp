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

#include <stdio.h>
#include <stdarg.h>

#include "common/file.h"
#include "common/util.h"

#include "kom/kom.h"
#include "kom/database.h"

using Common::File;

namespace Kom {

const int Database::_locRoutesSize = 111;
char line[100];

Database::Database(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm) {
	_locations = 0;
	_characters = 0;
	_objects = 0;

	_routes = 0;
	_map = 0;
	_locRoutes = 0;
	_actorScopes = new ActorScope[100];
}

Database::~Database() {
	delete[] _locations;
	delete[] _characters;
	delete[] _objects;
	free(_variables);
	delete[] _processes;
	delete[] _convIndex;
	delete[] _routes;
	delete[] _map;
	delete[] _locRoutes;
	delete[] _actorScopes;
}

void Database::init(Common::String databasePrefix) {
	_databasePrefix = databasePrefix;

	loadConvIndex();
	initLocations();
	initCharacters();
	initObjects();
	initEvents();
	initObjectLocs();
	initCharacterLocs();
	initProcs();
	initRoutes();


	for (int i = 0; i < _procsNum; ++i) {
		for (Common::List<Command>::iterator j = _processes[i].commands.begin();
				j != _processes[i].commands.end(); ++j) {
			if (j->cmd == 312) { // Init
				debug(1, "Processing init in %s", _processes[i].name);
				_vm->game()->doStat(&(*j));
			}
		}
	}
}

void Database::loadConvIndex() {
	File f;

	f.open("conv.idx");
	_convIndexSize = f.size();
	_convIndex = new byte[_convIndexSize];
	f.read(_convIndex, _convIndexSize);
	f.close();
}

void Database::initLocations() {
	File f;

	f.open(_databasePrefix + ".loc");

	// Get number of entries in file
	readLineScanf(f, "%d", &_locationsNum);

	_locations = new Location[_locationsNum];

	for (int i = 0; i < _locationsNum; ++i) {
		int index;

		do {
			f.readLine(line, 100);
		} while (line[0] == '\0');
		sscanf(line, "%d", &index);

		sscanf(line, "%*d %s %d %d",
			_locations[index].name,
			&(_locations[index].xtend),
			&(_locations[index].allowedTime));

		f.readLine(_locations[index].desc, 50);
		stripUndies(_locations[index].desc);

		readLineScanf(f, "-1");
	}

	f.close();

/*	for (int i = 0; i < _locationsNum; ++i) {
		printf("%d %s %d %d %s\n",
			i,
			_locations[i].name,
			_locations[i].xtend,
			_locations[i].allowedTime,
			_locations[i].desc);
	}*/
}

void Database::initCharacters() {
	File f;

	f.open(_databasePrefix + ".chr");

	// Get number of entries in file
	readLineScanf(f, "%d", &_charactersNum);

	_characters = new Character[_charactersNum];

	for (int i = 0; i < _charactersNum; ++i) {
		int index;

		do {
			f.readLine(line, 100);
		} while (line[0] == '\0');
		sscanf(line, "%d", &index);

		sscanf(line, "%*d %s %d %d",
			_characters[index].name,
			&(_characters[index].xtend),
			&(_characters[index].data2));

		f.readLine(_characters[index].desc, 50);
		stripUndies(_characters[index].desc);

		readLineScanf(f, "%d",
			&(_characters[index].proc));
		readLineScanf(f, "%d %d %d %d %d",
			&(_characters[index].locationId),
			&(_characters[index].box),
			&(_characters[index].data5),
			&(_characters[index].data6),
			&(_characters[index].data7));
		readLineScanf(f, "%d %d %d",
			&(_characters[index].data8),
			&(_characters[index].data9),
			&(_characters[index].hitpoints));
		readLineScanf(f, "%d %d %d %d",
			&(_characters[index].strength),
			&(_characters[index].defense),
			&(_characters[index].damageMin),
			&(_characters[index].damageMax));
		readLineScanf(f, "%d %d %d %d",
			&(_characters[index].data14),
			&(_characters[index].data15),
			&(_characters[index].data16),
			&(_characters[index].spellpoints));
	}

	f.close();

/*	for (int i = 0; i < _charactersNum; ++i) {
		printf("%d %s %d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			i,
			_characters[i].name,
			_characters[i].xtend,
			_characters[i].data2,
			_characters[i].desc,
			_characters[i].proc,
			_characters[i].locationId,
			_characters[i].box,
			_characters[i].data5,
			_characters[i].data6,
			_characters[i].data7,
			_characters[i].data8,
			_characters[i].data9,
			_characters[i].hitpoints,
			_characters[i].strength,
			_characters[i].defense,
			_characters[i].damageMin,
			_characters[i].damageMax,
			_characters[i].data14,
			_characters[i].data15,
			_characters[i].data16,
			_characters[i].spellpoints);
	}*/

	initScopes();
}

void Database::initObjects() {
	File f;

	f.open(_databasePrefix + ".obs");

	// Get number of entries in file
	readLineScanf(f, "%d", &_objectsNum);

	_objects = new Object[_objectsNum];

	// There are less objects than reported
	for (int i = 0; i < _objectsNum; ++i) {
		int index;

		do {
			f.readLine(line, 100);
		} while (line[0] == '\0');
		sscanf(line, "%d", &index);

		// Object indices in .pro are smaller than in .obs - adjusting
		index--;

		sscanf(line, "%*d %s %d",
			_objects[index].name,
			&(_objects[index].data1));

		f.readLine(_objects[index].desc, 50);
		stripUndies(_objects[index].desc);

		readLineScanf(f, "%d %d %d",
			&(_objects[index].type),
			&(_objects[index].data2),
			&(_objects[index].proc));
		readLineScanf(f, "%d %d %d %d %d %d %d %d",
			&(_objects[index].data4),
			&(_objects[index].isCarryable),
			&(_objects[index].isContainer),
			&(_objects[index].isVisible),
			&(_objects[index].isSprite),
			&(_objects[index].isUseImmediate),
			&(_objects[index].data9),
			&(_objects[index].isUsable));
		readLineScanf(f, "%d %d %d %d %d",
			&(_objects[index].price),
			&(_objects[index].data11),
			&(_objects[index].spellCost),
			&(_objects[index].data12),
			&(_objects[index].data13));

		f.readLine(line, 100);
		sscanf(line, "%d %d",
			&(_objects[index].ownerType),
			&(_objects[index].ownerId));


		if (_objects[index].ownerType == 1) {
			sscanf(line, "%*d %*d %d %d %d %d",
				&(_objects[index].box),
				&(_objects[index].data16),
				&(_objects[index].data17),
				&(_objects[index].data18));
		}
	}

	f.close();

/*	for (int i = 0; i < _objectsNum; ++i) {
		printf("%d %s %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			i,
			_objects[i].name,
			_objects[i].data1,
			_objects[i].desc,
			_objects[i].type,
			_objects[i].data2,
			_objects[i].proc,
			_objects[i].data4,
			_objects[i].isCarryable,
			_objects[i].isContainer,
			_objects[i].isVisible,
			_objects[i].isSprite,
			_objects[i].isUseImmediate,
			_objects[i].data9,
			_objects[i].isUsable,
			_objects[i].price,
			_objects[i].data11,
			_objects[i].spellCost,
			_objects[i].data12,
			_objects[i].data13,
			_objects[i].ownerType,
			_objects[i].ownerId,
			_objects[i].box,
			_objects[i].data16,
			_objects[i].data17,
			_objects[i].data18);
	}*/
}

void Database::initEvents() {
	int entries;
	File f;

	f.open(_databasePrefix + ".box");

	// Get number of entries in file
	readLineScanf(f, "%d", &entries);

	for (int i = 0; i < entries; ++i) {
		int loc;
		EventLink ev;

		readLineScanf(f, "%d %d %d", &loc, &(ev.data1), &(ev.data2));
		// Skip line
		f.readLine(line, 100);

		_locations[loc].events.push_back(ev);
	}

	f.close();

/*	for (int i = 0; i < _locationsNum; ++i) {
		printf("location %d\n", i);
		for (Common::List<EventLink>::iterator j = _locations[i].events.begin(); j != _locations[i].events.end(); ++j) {
		printf("%d %d\n",
			j->data1, j->data2);
		}
	}*/
}

void Database::initObjectLocs() {
	for (int i = 0; i < _objectsNum; ++i) {
		if (_objects[i].type != 0) {
			if (_objects[i].ownerType == 1)
				_locations[_objects[i].ownerId].objects.push_back(i);
			else
				_characters[_objects[i].ownerId].inventory.push_back(i);
		}
	}
}

void Database::initCharacterLocs() {
	for (int i = 0; i < _charactersNum; ++i)
		_locations[_characters[i].locationId].characters.push_back(i);
	_vm->game()->settings()->currLocation = _characters[0].locationId;
}

void Database::initProcs() {
	File f;

	f.open(_databasePrefix + ".pro");

	// Get number of entries in file
	readLineScanf(f, "%d", &_varSize);

	_variables = (int16 *)calloc(_varSize, sizeof(_variables[0]));

	readLineScanf(f, "%d", &_procsNum);

	_processes = new Process[_procsNum];

	for (int i = 0; i < _procsNum; ++i) {
		int index;
		int cmd, opcode;

		readLineScanf(f, "%d", &index);

		f.readLine(_processes[index].name, 50);

		do {
			f.readLine(line, 100);
		} while (line[0] == '\0');
		sscanf(line, "%d", &cmd);

		// Read commands
		while (cmd != -1) {
			Command cmdObject;
			cmdObject.cmd = cmd;

			// Read special cmd with value
			if (cmd == 319 || cmd == 320 || cmd == 321) {
				sscanf(line, "%*d %hu", &(cmdObject.value));
			}

			// Read opcodes
			do {
				f.readLine(line, 100);
			} while (line[0] == '\0');
			sscanf(line, "%d", &opcode);

			while (opcode != -1) {
				OpCode opObject;
				opObject.opcode = opcode;

				switch (opcode) {

				// string + int/short
				case 474:
					f.readLine(opObject.arg1, 30);
					readLineScanf(f, "%d", &(opObject.arg2));
					break;

				// string
				case 467:
				case 469:
					f.readLine(opObject.arg1, 30);

					// Skip empty line
					f.readLine(line, 100);
					break;

				// string + int + int + int
				case 468:
					f.readLine(opObject.arg1, 30);
					readLineScanf(f, "%d %d %d", &(opObject.arg2),
						   &(opObject.arg3), &(opObject.arg4));
					break;

				// short + string - never reached
				case 99999:
					break;

				// int
				case 331:
				case 337:
				case 338:
				case 373:
				case 374:
				case 381:
				case 382:
				case 383:
				case 384:
				case 387:
				case 405:
				case 406:
				case 407:
				case 408:
				case 409:
				case 410:
				case 411:
				case 412:
				case 413:
				case 414:
				case 419:
				case 433:
				case 432:
				case 444:
				case 445:
				case 446:
				case 448:
				case 491:
					sscanf(line, "%*d %d", &(opObject.arg2));
					break;

				// int + int
				case 327:
				case 328:
				case 334:
				case 340:
				case 345:
				case 346:
				case 350:
				case 353:
				case 356:
				case 359:
				case 376:
				case 377:
				case 379:
				case 380:
				case 393:
				case 398:
				case 399:
				case 404:
				case 416:
				case 417:
				case 418:
				case 420:
				case 422:
				case 423:
				case 424:
				case 425:
				case 426:
				case 427:
				case 428:
				case 430:
				case 431:
				case 434:
				case 436:
				case 437:
				case 439:
				case 458:
				case 459:
				case 462:
				case 465:
				case 466:
				case 476:
				case 477:
				case 481:
				case 482:
				case 483:
				case 484:
				case 487:
				case 489:
				case 490:
				case 492:
					sscanf(line, "%*d %d %d", &(opObject.arg2), &(opObject.arg3));
					break;

				// int + int + int
				case 394:
				case 395:
				case 402:
				case 403:
				case 441:
				case 478:
				case 479:
				case 488:
					sscanf(line, "%*d %d %d %d", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4));
					break;

				// int + int + int + int + int
				case 438:
					sscanf(line, "%*d %d %d %d %d %d", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4),
						   &(opObject.arg5), &(opObject.arg6));
					break;

				// No arguments
				case 391:
				case 392:
				case 449:
				case 450:
				case 451:
				case 452:
				case 453:
				case 454:
				case 473:
				case 475:
				case 485:
				case 486:
				case 494:
					break;

				default:
					error("Unknown opcode %d in command %d in index %d", opcode, cmd, index);
					break;
				}

				cmdObject.opcodes.push_back(opObject);

				do {
					f.readLine(line, 100);
				} while (line[0] == '\0');
				sscanf(line, "%d", &opcode);
			}

			_processes[index].commands.push_back(cmdObject);

			do {
				f.readLine(line, 100);
			} while (line[0] == '\0');
			sscanf(line, "%d", &cmd);
		}
	}

	f.close();
}

void Database::initRoutes() {
	File f;
	char keyword[30];
	int16 locIndex = 0;
	int16 boxIndex = 0;
	int32 parmIndex, parmData;

	f.open("test0r.rou");
	_routesSize = f.size();
	_routes = new byte[_routesSize];
	f.read(_routes, _routesSize);
	f.close();

	f.open("test0r.map");
	_mapSize = f.size();
	_map = new byte[_mapSize];
	f.read(_map, _mapSize);
	f.close();

	_locRoutes = new LocRoute[_locRoutesSize];
	f.open("test0r.ked");

	do {
		f.readLine(line, 100);
	} while (line[0] == '\0');

	while (!f.eof()) {
		sscanf(line, "%s", keyword);

		if (strcmp(keyword, "LOC") == 0) {
			sscanf(line, "%*s %hd", &locIndex);

		} else if (strcmp(keyword, "exits") == 0) {
			sscanf(line, "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exit = parmData;

		} else if (strcmp(keyword, "exit_locs") == 0) {
			sscanf(line, "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exitLoc = parmData;

		} else if (strcmp(keyword, "exit_boxs") == 0) {
			sscanf(line, "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exitBox = parmData;

		} else if (strcmp(keyword, "BOX") == 0) {
			sscanf(line, "%*s %hd", &boxIndex);

		} else if (strcmp(keyword, "x1") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].x1 = parmData;

		} else if (strcmp(keyword, "y1") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].y1 = parmData;

		} else if (strcmp(keyword, "x2") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].x2 = parmData;

		} else if (strcmp(keyword, "y2") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].y2 = parmData;

		} else if (strcmp(keyword, "priority") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].priority = parmData;

		} else if (strcmp(keyword, "z1") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].z1 = parmData;

		} else if (strcmp(keyword, "z2") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].z2 = parmData;

		} else if (strcmp(keyword, "attrib") == 0) {
			sscanf(line, "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].attrib = parmData;

		} else if (strcmp(keyword, "joins") == 0) {
			sscanf(line, "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].boxes[boxIndex].joins[parmIndex] = parmData;
		}

		do {
			f.readLine(line, 100);
		} while (line[0] == '\0');
	}

	f.close();
}

void Database::initScopes() {
	File f;
	char keyword[30];
	uint8 actorIndex = 0;
	uint8 scopeIndex = 0;

	// Temporary variables to bypass scanf's %hhu - not supported by ISO C++
	uint16 tmp1, tmp2, tmp3;

	f.open(_databasePrefix + ".scp");
	do {
		f.readLine(line, 100);
	} while (line[0] == '\0');

	while (!f.eof()) {
		sscanf(line, "%s", keyword);

		if (strcmp(keyword, "ACTOR") == 0) {
			sscanf(line, "%*s %hu", &tmp1);
			actorIndex = tmp1;

		} else if (strcmp(keyword, "SCOPE") == 0) {
			sscanf(line, "%*s %hu", &tmp1);
			scopeIndex = tmp1;

			sscanf(line, "%*s %*u %hu %hu %hu",
				&(tmp1), &(tmp2), &(tmp3));

			_actorScopes[actorIndex].scopes[scopeIndex].minFrame = tmp1;
			_actorScopes[actorIndex].scopes[scopeIndex].maxFrame = tmp2;
			_actorScopes[actorIndex].scopes[scopeIndex].startFrame = tmp3;

		} else if (strcmp(keyword, "SPEED") == 0) {
			sscanf(line, "%*s %hu %hu",
				&(_actorScopes[actorIndex].speed1),
				&(_actorScopes[actorIndex].speed2));

		} else if (strcmp(keyword, "TIMEOUT") == 0) {
			sscanf(line, "%*s %hu",
				&(_actorScopes[actorIndex].timeout));
			_actorScopes[actorIndex].timeout *= 24;

		} else if (strcmp(keyword, "START") == 0) {
			sscanf(line, "%*s %u %u %u %u %u",
				&(_actorScopes[actorIndex].lastLocation),
				&(_actorScopes[actorIndex].lastBox),
				&(_actorScopes[actorIndex].start3),
				&(_actorScopes[actorIndex].start4),
				&(_actorScopes[actorIndex].start5));
		}

		do {
			f.readLine(line, 100);
		} while (line[0] == '\0' && !f.eof());
	}

	f.close();
}

int8 Database::whatBox(int locId, int x, int y) {
	Box *boxes = _locRoutes[locId].boxes;

	for (int i = 0; i < 32; ++i)
		if (boxes[i].attrib == 6 &&
			x >= boxes[i].x1 &&
			x <= boxes[i].x2 &&
			y >= boxes[i].y1 &&
			y <= boxes[i].y2)
			return i;

	return -1;
}

void Database::setCharPos(int charId, int loc, int box) {
	_characters[charId].box = box;

	_locations[_characters[charId].locationId].characters.remove(charId);

	_characters[charId].locationId = loc;
	_locations[loc].characters.push_back(charId);

	if (charId == 0) {
		_characters[0].destLoc = loc;
		_characters[0].destBox = box;
	}
}

bool Database::giveObject(int charId, int obj, bool something) {
	int type;
	int oldOwner, oldOwnerType;

	if (!(_objects[obj].data4) || !(_objects[obj].isCarryable))
		return false;

	type = _objects[obj].type - 1;
	oldOwnerType = _objects[obj].ownerType;
	oldOwner = _objects[obj].ownerId;
 

	if (type <= 4) {

		_objects[obj].ownerType = 3;
		_objects[obj].ownerId = charId;

		switch (type) {
		case 3:
			_characters[charId].gold += _objects[obj].price;
			// Fall through
		case 0:
		case 4:
			_characters[charId].inventory.push_back(obj);
			break;
		case 1:
			_characters[charId].weapons.push_back(obj);
			break;
		case 2:
			_characters[charId].spells.push_back(obj);
			break;
		}
	}

	if (charId == 0 && !something) {
		warning("TODO: actionGotObject");
	}

	switch (oldOwnerType) {
	case 1:
		_locations[oldOwner].objects.remove(obj);
		break;
	case 2:
		_objects[oldOwner].contents.remove(obj);
		break;
	case 3:
		if (oldOwner == 0 && !something) {
			warning("TODO: actionLostObject");
		}

		switch (type) {
		case 0:
		case 4:
			_characters[charId].inventory.remove(obj);
			break;
		case 1:
			_characters[charId].weapons.remove(obj);
			break;
		case 2:
			_characters[charId].spells.remove(obj);
			break;
		}

		break;
	}

	return true;
}

void Database::stripUndies(char *s) {
	for (int i = 0; s[i] != '\0'; ++i)
		if (s[i] == '_')
			s[i] = ' ';
}

int CDECL Database::readLineScanf(File &f, const char *format, ...) {
	va_list arg;
	int done;

	do {
		f.readLine(line, 100);
	} while (line[0] == '\0');

	if (f.eos())
		return EOF;

	va_start(arg, format);
	done = vsscanf(line, format, arg);
	va_end(arg);

	return done;
}
} // End of namespace Kom
