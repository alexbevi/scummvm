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
#include <string.h>

#include "common/file.h"
#include "common/util.h"
#include "common/endian.h"

#include "kom/kom.h"
#include "kom/database.h"
#include "kom/character.h"

using Common::File;

namespace Kom {

const int Database::_locRoutesSize = 111;
Common::String line;

Database::Database(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm) {
	_locations = 0;
	_characters = 0;
	_objects = 0;

	_routes = 0;
	_map = 0;
	_locRoutes = 0;
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
}

void Database::init(Common::String databasePrefix) {
	_databasePrefix = databasePrefix;

	if (_databasePrefix == "thid")
		_pathPrefix = "install/db0/";
	else
		_pathPrefix = "install/db1/";


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

	f.open(_pathPrefix + "conv.idx");
	_convIndexSize = f.size();
	_convIndex = new byte[_convIndexSize];
	f.read(_convIndex, _convIndexSize);
	f.close();
}

void Database::initLocations() {
	File f;

	f.open(_pathPrefix + _databasePrefix + ".loc");

	// Get number of entries in file
	line = f.readLine();
	sscanf(line.c_str(), "%d", &_locationsNum);

	_locations = new Location[_locationsNum];

	for (int i = 0; i < _locationsNum; ++i) {
		int index;

		// Skip empty line
		f.readLine();

		line = f.readLine();
		sscanf(line.c_str(), "%d", &index);

		sscanf(line.c_str(), "%*d %s %d %d",
			_locations[index].name,
			&(_locations[index].xtend),
			&(_locations[index].allowedTime));

		line = f.readLine();
		strcpy(_locations[index].desc, line.c_str());
		stripUndies(_locations[index].desc);

		// Skip line containing "-1"
		f.readLine();
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

	f.open(_pathPrefix + _databasePrefix + ".chr");

	// Get number of entries in file
	line = f.readLine();
	sscanf(line.c_str(), "%d", &_charactersNum);

	_characters = new Character[_charactersNum];
	Character::_vm = _vm;

	for (int i = 0; i < _charactersNum; ++i) {
		int index;

		do {
			line = f.readLine();
		} while (line.empty());
		sscanf(line.c_str(), "%d", &index);

		_characters[index]._id = index;

		sscanf(line.c_str(), "%*d %s %d %d",
			_characters[index]._name,
			&(_characters[index]._xtend),
			&(_characters[index]._data2));

		line = f.readLine();
		strcpy(_characters[index]._desc, line.c_str());
		stripUndies(_characters[index]._desc);

		line = f.readLine();
		sscanf(line.c_str(), "%d",
			&(_characters[index]._proc));
		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d %d %d",
			&(_characters[index]._locationId),
			&(_characters[index]._box),
			&(_characters[index]._data5),
			&(_characters[index]._data6),
			&(_characters[index]._data7));
		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d",
			&(_characters[index]._data8),
			&(_characters[index]._data9),
			&(_characters[index]._hitPoints));
		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d %d",
			&(_characters[index]._strength),
			&(_characters[index]._defense),
			&(_characters[index]._damageMin),
			&(_characters[index]._damageMax));
		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d %d",
			&(_characters[index]._data14),
			&(_characters[index]._data15),
			&(_characters[index]._data16),
			&(_characters[index]._spellPoints));

		_characters[index]._destLoc = _characters[index]._locationId;
		_characters[index]._destBox = _characters[index]._box;
		_characters[index]._hitPointsMax = _characters[index]._hitPoints;
		_characters[index]._spellPointsMax = _characters[index]._spellPoints;
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
			_characters[i].hitPoints,
			_characters[i].strength,
			_characters[i].defense,
			_characters[i].damageMin,
			_characters[i].damageMax,
			_characters[i].data14,
			_characters[i].data15,
			_characters[i].data16,
			_characters[i].spellPoints);
	}*/

	initScopes();
}

void Database::initObjects() {
	File f;

	f.open(_pathPrefix + _databasePrefix + ".obs");

	// Get number of entries in file
	line = f.readLine();
	sscanf(line.c_str(), "%d", &_objectsNum);

	_objects = new Object[_objectsNum];

	// There are less objects than the maximum object id
	for (int i = 0; i < 327; ++i) {
		int index;

		// Skip empty line
		f.readLine();

		line = f.readLine();
		sscanf(line.c_str(), "%d", &index);

		// Object indices in .pro are smaller than in .obs - adjusting
		index--;

		sscanf(line.c_str(), "%*d %s %d",
			_objects[index].name,
			&(_objects[index].data1));

		line = f.readLine();
		strcpy(_objects[index].desc, line.c_str());
		stripUndies(_objects[index].desc);

		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d",
			&(_objects[index].type),
			&(_objects[index].data2),
			&(_objects[index].proc));
		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d %d %d %d %d %d",
			&(_objects[index].data4),
			&(_objects[index].isCarryable),
			&(_objects[index].isContainer),
			&(_objects[index].isVisible),
			&(_objects[index].isSprite),
			&(_objects[index].isUseImmediate),
			&(_objects[index].isPickable),
			&(_objects[index].isUsable));
		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d %d %d",
			&(_objects[index].price),
			&(_objects[index].data11),
			&(_objects[index].spellCost),
			&(_objects[index].data12),
			&(_objects[index].data13));

		line = f.readLine();
		sscanf(line.c_str(), "%d %d",
			&(_objects[index].ownerType),
			&(_objects[index].ownerId));


		if (_objects[index].ownerType == 1) {
			sscanf(line.c_str(), "%*d %*d %d %d %d %d",
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
			_objects[i].isPickable,
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

	f.open(_pathPrefix + _databasePrefix + ".box");

	// Get number of entries in file
	line = f.readLine();
	sscanf(line.c_str(), "%d", &entries);

	for (int i = 0; i < entries; ++i) {
		int loc;
		EventLink ev;

		line = f.readLine();
		sscanf(line.c_str(), "%d %d %d", &loc, &(ev.exitBox), &(ev.proc));

		// Skip line
		f.readLine();

		_locations[loc].events.push_back(ev);
	}

	f.close();

/*	for (int i = 0; i < _locationsNum; ++i) {
		printf("location %d\n", i);
		for (Common::List<EventLink>::iterator j = _locations[i].events.begin(); j != _locations[i].events.end(); ++j) {
		printf("%d %d\n",
			j->exitBox, j->proc);
		}
	}*/
}

void Database::initObjectLocs() {
	for (int i = 0; i < _objectsNum; ++i) {
		if (_objects[i].type == 0)
			continue;

		if (_objects[i].ownerType == 1)
			_locations[_objects[i].ownerId].objects.push_back(i);
		else switch (_objects[i].type) {
		case 1:
		case 5:
			_characters[_objects[i].ownerId]._inventory.push_back(i);
			break;
		case 2:
			_characters[_objects[i].ownerId]._weapons.push_back(i);
			break;
		case 3:
			_characters[_objects[i].ownerId]._spells.push_back(i);
			break;
		}
	}
}

void Database::initCharacterLocs() {
	for (int i = 0; i < _charactersNum; ++i)
		_locations[_characters[i]._locationId].characters.push_back(i);
	_vm->game()->settings()->currLocation = _characters[0]._locationId;
}

void Database::initProcs() {
	File f;

	f.open(_pathPrefix + _databasePrefix + ".pro");

	// Get number of entries in file
	line = f.readLine();
	sscanf(line.c_str(), "%d", &_varSize);

	_variables = (int16 *)calloc(_varSize, sizeof(_variables[0]));

	line = f.readLine();
	sscanf(line.c_str(), "%d", &_procsNum);

	_processes = new Process[_procsNum];

	for (int i = 0; i < _procsNum; ++i) {
		int index;
		int cmd, opcode;

		do {
			line = f.readLine();
		} while (line.empty());
		sscanf(line.c_str(), "%d", &index);

		line = f.readLine();
		strcpy(_processes[index].name, line.c_str());

		do {
			line = f.readLine();
		} while (line.empty());
		sscanf(line.c_str(), "%d", &cmd);

		// Read commands
		while (cmd != -1) {
			Command cmdObject;
			cmdObject.cmd = cmd;

			// Read special cmd with value
			if (cmd == 319 || cmd == 320 || cmd == 321) {
				sscanf(line.c_str(), "%*d %hu", &(cmdObject.value));
			}

			// Read opcodes
			do {
				line = f.readLine();
			} while (line.empty());
			sscanf(line.c_str(), "%d", &opcode);

			while (opcode != -1) {
				OpCode opObject;
				opObject.opcode = opcode;

				switch (opcode) {

				// string + int/short
				case 474:
					line = f.readLine();
					strcpy(opObject.arg1, line.c_str());
					line = f.readLine();
					sscanf(line.c_str(), "%d", &(opObject.arg2));
					break;

				// string
				case 467:
				case 469:
					line = f.readLine();
					strcpy(opObject.arg1, line.c_str());

					// Skip empty line
					f.readLine();
					break;

				// string + int + int + int
				case 468:
					line = f.readLine();
					strcpy(opObject.arg1, line.c_str());
					line = f.readLine();
					sscanf(line.c_str(), "%d %d %d", &(opObject.arg2),
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
					sscanf(line.c_str(), "%*d %d", &(opObject.arg2));
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
					sscanf(line.c_str(), "%*d %d %d", &(opObject.arg2), &(opObject.arg3));
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
					sscanf(line.c_str(), "%*d %d %d %d", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4));
					break;

				// int + int + int + int + int
				case 438:
					sscanf(line.c_str(), "%*d %d %d %d %d %d", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4),
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
					line = f.readLine();
				} while (line.empty());
				sscanf(line.c_str(), "%d", &opcode);
			}

			_processes[index].commands.push_back(cmdObject);

			do {
				line = f.readLine();
			} while (line.empty());
			sscanf(line.c_str(), "%d", &cmd);
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

	f.open(_pathPrefix + "test0r.rou");
	_routesSize = f.size();
	_routes = new byte[_routesSize];
	f.read(_routes, _routesSize);
	f.close();

	f.open(_pathPrefix + "test0r.map");
	_mapSize = f.size();
	_map = new byte[_mapSize];
	f.read(_map, _mapSize);
	f.close();

	_locRoutes = new LocRoute[_locRoutesSize];
	f.open(_pathPrefix + "test0r.ked");

	do {
		line = f.readLine();
	} while (line.empty());

	while (!f.eos()) {
		sscanf(line.c_str(), "%s", keyword);

		if (strcmp(keyword, "LOC") == 0) {
			sscanf(line.c_str(), "%*s %hd", &locIndex);

		} else if (strcmp(keyword, "exits") == 0) {
			sscanf(line.c_str(), "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exit = parmData;

		} else if (strcmp(keyword, "exit_locs") == 0) {
			sscanf(line.c_str(), "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exitLoc = parmData;

		} else if (strcmp(keyword, "exit_boxs") == 0) {
			sscanf(line.c_str(), "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exitBox = parmData;

		} else if (strcmp(keyword, "BOX") == 0) {
			sscanf(line.c_str(), "%*s %hd", &boxIndex);
			_locRoutes[locIndex].boxes[boxIndex].enabled = true;

		} else if (strcmp(keyword, "x1") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].x1 = parmData;

		} else if (strcmp(keyword, "y1") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].y1 = parmData;

		} else if (strcmp(keyword, "x2") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].x2 = parmData;

		} else if (strcmp(keyword, "y2") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].y2 = parmData;

		} else if (strcmp(keyword, "priority") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].priority = parmData;

		} else if (strcmp(keyword, "z1") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].z1 = parmData;

		} else if (strcmp(keyword, "z2") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].z2 = parmData;

		} else if (strcmp(keyword, "attrib") == 0) {
			sscanf(line.c_str(), "%*s %d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].attrib = parmData;

		} else if (strcmp(keyword, "joins") == 0) {
			sscanf(line.c_str(), "%*s %d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].boxes[boxIndex].joins[parmIndex] = parmData;
		}

		do {
			line = f.readLine();
		} while (line.empty() && !f.eos());
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

	f.open(_pathPrefix + _databasePrefix + ".scp");
	do {
		line = f.readLine();
	} while (line.empty());

	while (!f.eos()) {
		Character *charScope;

		sscanf(line.c_str(), "%s", keyword);

		if (strcmp(keyword, "ACTOR") == 0) {
			sscanf(line.c_str(), "%*s %hu", &tmp1);
			actorIndex = tmp1;
			charScope = getChar(actorIndex);

		} else if (strcmp(keyword, "SCOPE") == 0) {
			sscanf(line.c_str(), "%*s %hu", &tmp1);
			scopeIndex = tmp1;

			sscanf(line.c_str(), "%*s %*u %hu %hu %hu",
				&(tmp1), &(tmp2), &(tmp3));

			charScope->_scopes[scopeIndex].minFrame = tmp1;
			charScope->_scopes[scopeIndex].maxFrame = tmp2;
			charScope->_scopes[scopeIndex].startFrame = tmp3;

		} else if (strcmp(keyword, "SPEED") == 0) {
			sscanf(line.c_str(), "%*s %hu %hu",
				&(charScope->_walkSpeed),
				&(charScope->_animSpeed));

			if (charScope->_walkSpeed == 0) {
				charScope->_stopped = true;
				charScope->_timeout = 50;
				charScope->_offset78 = 0x80000;
			} else {
				charScope->_stopped = false;
				charScope->_timeout = 0;
				charScope->_offset78 = 0xc0000;
			}

		} else if (strcmp(keyword, "TIMEOUT") == 0) {
			sscanf(line.c_str(), "%*s %hu",
				&(charScope->_timeout));
			charScope->_timeout *= 24;

		} else if (strcmp(keyword, "START") == 0) {
			sscanf(line.c_str(), "%*s %d %d %d %d %d",
				&(charScope->_lastLocation),
				&(charScope->_lastBox),
				&(charScope->_start3),
				&(charScope->_start4),
				&(charScope->_start5));

			charScope->_gotoLoc = charScope->_lastLocation; // FIXME: Not in original. just in case
			charScope->_gotoBox = charScope->_lastBox;
			charScope->_screenX = charScope->_gotoX = charScope->_start3 / 256;
			charScope->_screenY = charScope->_gotoY = charScope->_start4 / 256;
			charScope->_start3Prev = charScope->_start3PrevPrev = charScope->_start3;
			charScope->_start4Prev = charScope->_start4PrevPrev = charScope->_start4;
			charScope->_start5Prev = charScope->_start5PrevPrev = charScope->_start5;
		}

		do {
			line = f.readLine();
		} while (line.empty() && !f.eos());
	}

	f.close();
}

int8 Database::box2box(int loc, int fromBox, int toBox) {
	uint32 locOffset;

	if ((loc | fromBox | toBox) > 127)
		return -1;

	locOffset = READ_LE_UINT32(_map + loc * 4);
	return (int8)(_map[locOffset + *(int8*)(_map + locOffset) * toBox + fromBox + 1]);
}

int8 Database::whatBox(int locId, int x, int y) {
	Box *boxes = _locRoutes[locId].boxes;

	for (int i = 0; i < 32; ++i)
		if (boxes[i].enabled &&
			!(boxes[i].attrib & 6) &&
			x >= boxes[i].x1 &&
			x <= boxes[i].x2 &&
			y >= boxes[i].y1 &&
			y <= boxes[i].y2)
			return i;

	return -1;
}

int8 Database::whatBoxLinked(int locId, int8 boxId, int x, int y) {
	Box *box = &(_locRoutes[locId].boxes[boxId]);

	if ((box->attrib & 14) == 0 &&
		x >= box->x1 && x <= box->x2 &&
		y >= box->y1 && y <= box->y2) {

		return boxId;
	}

	// search joined boxes
	for (int i = 0; i < 6; ++i) {
		if (box->joins[i] >= 0) {
			Box *link = &(_locRoutes[locId].boxes[box->joins[i]]);

			if ((link->attrib & 14) == 0 &&
				x >= link->x1 && x <= link->x2 &&
				y >= link->y1 && y <= link->y2) {

				return box->joins[i];
			}
		}
	}

	return -1;
}

int16 Database::getMidOverlapX(int loc, int box1, int box2) {
	int16 x1Max, x2Min;

	Box *b1 = &(_locRoutes[loc].boxes[box1]);
	Box *b2 = &(_locRoutes[loc].boxes[box2]);

	x1Max = MAX(b1->x1, b2->x1);
	x2Min = MIN(b1->x2, b2->x2);

	if (x2Min < x1Max) {
		x2Min ^= x1Max;
		x1Max ^= x2Min;
		x2Min ^= x1Max;
	}

	if (x1Max + x2Min <= 1)
		return x1Max;
	else
		return x1Max + 1;
}

int16 Database::getMidOverlapY(int loc, int box1, int box2) {
	int16 y1Max, y2Min;

	Box *b1 = &(_locRoutes[loc].boxes[box1]);
	Box *b2 = &(_locRoutes[loc].boxes[box2]);

	y1Max = MAX(b1->y1, b2->y1);
	y2Min = MIN(b1->y2, b2->y2);

	if (y2Min < y1Max) {
		y2Min ^= y1Max;
		y1Max ^= y2Min;
		y2Min ^= y1Max;
	}

	if (y1Max + y2Min <= 1)
		return y1Max;
	else
		return y1Max + 1;
}

int16 Database::getZValue(int loc, int box, int32 y) {
	Box *b;

	if (loc < 0 || box < 0) return 1;

	b = getBox(loc, box);

	if (b->z2 == 0) return 1;

	return b->z1 - ((y - b->y1 * 256) / (b->y2 - b->y1) * (b->z1 - b->z2)) / 256;
}

uint16 Database::getExitBox(int currLoc, int nextLoc) {
	for (int i = 0; i < 6; ++i)
		if (_locRoutes[currLoc].exits[i].exitLoc == nextLoc)
			return _locRoutes[currLoc].exits[i].exit;

	return 0;
}

int8 Database::getBoxLink(int loc, int box, int join) {
	// FIXME - the original has an unidentified test here
	return _locRoutes[loc].boxes[box].joins[join];
}

int8 Database::getFirstLink(int loc, int box) {
	int8 linkBox = -1;
	for (int join = 0; join < 6; ++join)
		if ((linkBox = _locRoutes[loc].boxes[box].joins[join]) != -1)
			return linkBox;
	return linkBox;
}

bool Database::getExitInfo(int loc, int box, int *exitLoc, int *exitBox) {
	for (int i = 0; i < 6; ++i)
		if (_locRoutes[loc].exits[i].exit == box) {
			*exitLoc = _locRoutes[loc].exits[i].exitLoc;
			*exitBox = _locRoutes[loc].exits[i].exitBox;

			return true;
		}

	error("no exit info. loc %d, box %d", loc, box);
	return false;
}

bool Database::isInLine(int loc, int box, int x, int y) {
	Box *b = (&_locRoutes[loc].boxes[box]);

	return ((x > b->x1 && x < b->x2) || (y > b->y1 && y < b->y2));
}


void Database::getClosestBox(int loc, uint16 mouseX, uint16 mouseY,
		int16 screenX, int16 screenY,
		int16 *collideBox, uint16 *collideBoxX, uint16 *collideBoxY) {

	int tmp = 399;
	int tmpBox = -1;
	int tmp2 = -1;
	int tmp2Box = -1;
	int tmp3 = -1;
	int tmp3Box = -1;
	int tmp4 = 639;
	int tmp4Box = -1;

	for (int i = 0; i < 32; ++i) {
		Box *box = getBox(loc, i);
		if (!box->enabled)
			continue;

		if (box->attrib != 0 || box->x1 > mouseX || box->x2 < mouseX)
			continue;

		if (box->y2 >= mouseY && box->y1 < tmp) {
			tmp = box->y1;
			tmpBox = i;
		}

		if (box->y1 <= mouseY && box->y2 > tmp2) {
			tmp2 = box->y2;
			tmp2Box = i;
		}
	}

	if (tmpBox == -1 && tmp2Box == -1) {
		for (int i = 0; i < 32; ++i) {
			Box *box = getBox(loc, i);
			if (!box->enabled)
				continue;

			if (box->attrib != 0 || box->y1 > screenY || box->y2 < screenY)
				continue;

			if (box->x2 <= mouseX && box->x2 > tmp3) {
				tmp3 = box->x2;
				tmp3Box = i;
			}

			if (box->x2 >= mouseX && box->x2 < tmp4) {
				tmp4 = box->x1;
				tmp4Box = i;
			}
		}
	}

	if (tmpBox != -1) {
		*collideBoxX = mouseX;

		// TODO: verify
		if ((tmpBox|loc) <= 0) {
			*collideBoxY = 389;
		} else {
			Box *box = getBox(loc, tmpBox);
			*collideBoxY = (box->y2 - box->y1) / 2 + box->y1;
		}
		*collideBox = tmpBox;

	} else if (tmp2Box != -1) {
		*collideBoxX = mouseX;

		// TODO: verify
		if ((tmp2Box|loc) <= 0) {
			*collideBoxY = 389;
		} else {
			Box *box = getBox(loc, tmp2Box);
			*collideBoxY = (box->y2 - box->y1) / 2 + box->y1;
		}
		*collideBox = tmp2Box;

	} else if (mouseX >= screenX) {

		// TODO: verify
		if ((tmp3Box|loc) <= 0) {
			*collideBoxX = 319;
		} else {
			Box *box = getBox(loc, tmp3Box);
			*collideBoxX = (box->x2 - box->x1) / 2 + box->x1;
		}

		*collideBoxY = screenY;
		*collideBox = tmp3Box;

	} else {

		// TODO: verify
		if ((tmp4Box|loc) <= 0) {
			*collideBoxX = 319;
		} else {
			Box *box = getBox(loc, tmp4Box);
			*collideBoxX = (box->x2 - box->x1) / 2 + box->x1;
		}

		*collideBoxY = screenY;
		*collideBox = tmp4Box;
	}
}

void Database::setCharPos(int charId, int loc, int box) {
	_characters[charId]._box = box;

	_locations[_characters[charId]._locationId].characters.remove(charId);

	_characters[charId]._locationId = loc;
	_locations[loc].characters.push_back(charId);

	if (charId == 0) {
		_vm->game()->settings()->currLocation = loc;
		_characters[0]._destLoc = loc;
		_characters[0]._destBox = box;
	}
}

bool Database::giveObject(int obj, int charId, bool noAnimation) {
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
			_characters[charId]._gold += _objects[obj].price;
			// Fall through
		case 0:
		case 4:
			_characters[charId]._inventory.push_back(obj);
			break;
		case 1:
			_characters[charId]._weapons.push_back(obj);
			break;
		case 2:
			_characters[charId]._spells.push_back(obj);
			break;
		}
	}

	if (charId == 0 && !noAnimation)
		_vm->game()->doActionGotObject(obj);

	switch (oldOwnerType) {
	case 1:
		_locations[oldOwner].objects.remove(obj);
		break;
	case 2:
		_objects[oldOwner].contents.remove(obj);
		break;
	case 3:
		if (oldOwner == 0 && !noAnimation) {
			_vm->game()->doActionLostObject(obj);
		}

		switch (type) {
		case 0:
		case 4:
			_characters[oldOwner]._inventory.remove(obj);
			break;
		case 1:
			_characters[oldOwner]._weapons.remove(obj);
			break;
		case 2:
			_characters[oldOwner]._spells.remove(obj);
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

} // End of namespace Kom
