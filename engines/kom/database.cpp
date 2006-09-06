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

#include <stdio.h>

#include "common/file.h"
#include "common/util.h"

#include "kom/kom.h"
#include "kom/database.h"

using Common::File;

namespace Kom {

const int Database::_locRoutesSize = 111;

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
	delete[] _dataSegment;
	delete[] _processes;
	delete[] _convIndex;
	delete[] _routes;
	delete[] _map;
	delete[] _locRoutes;
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
	f.scanf("%d\n\n", &_locationsNum);
	printf("%d loc entries\n", _locationsNum);

	_locations = new Location[_locationsNum];

	for (int i = 0; i < _locationsNum; ++i) {
		int index;

		f.scanf("%d", &index);

		f.scanf(
			"%s %d %d\n"
			"%[^\r]\n"
			"-1\n",
			_locations[index].name,
			&(_locations[index].xtend),
			&(_locations[index].data2),
			_locations[index].desc);

		stripUndies(_locations[index].desc);
	}

	f.close();

/*	for (int i = 0; i < _locationsNum; ++i) {
		printf("%d %s %d %d %s\n",
			i,
			_locations[i].name,
			_locations[i].xtend,
			_locations[i].data2,
			_locations[i].desc);
	}*/
}

void Database::initCharacters() {
	File f;

	f.open(_databasePrefix + ".chr");

	// Get number of entries in file
	f.scanf("%d\n\n", &_charactersNum);
	printf("%d chr entries\n", _charactersNum);

	_characters = new Character[_charactersNum];

	for (int i = 0; i < _charactersNum; ++i) {
		int index;

		f.scanf("%d", &index);

		f.scanf(
			"%s %d %d\n"
			"%[^\r]\n"
			"%d\n"
			"%d %d %d %d %d\n"
			"%d %d %d\n"
			"%d %d %d %d\n"
			"%d %d %d %d\n\n",
			_characters[index].name,
			&(_characters[index].xtend),
			&(_characters[index].data2),
			_characters[index].desc,
			&(_characters[index].proc),
			&(_characters[index].locationId),
			&(_characters[index].data4),
			&(_characters[index].data5),
			&(_characters[index].data6),
			&(_characters[index].data7),
			&(_characters[index].data8),
			&(_characters[index].data9),
			&(_characters[index].hitpoints),
			&(_characters[index].data10),
			&(_characters[index].data11),
			&(_characters[index].data12),
			&(_characters[index].data13),
			&(_characters[index].data14),
			&(_characters[index].data15),
			&(_characters[index].data16),
			&(_characters[index].spellpoints));

		stripUndies(_characters[index].desc);
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
			_characters[i].data4,
			_characters[i].data5,
			_characters[i].data6,
			_characters[i].data7,
			_characters[i].data8,
			_characters[i].data9,
			_characters[i].hitpoints,
			_characters[i].data10,
			_characters[i].data11,
			_characters[i].data12,
			_characters[i].data13,
			_characters[i].data14,
			_characters[i].data15,
			_characters[i].data16,
			_characters[i].spellpoints);
	}*/
}

void Database::initObjects() {
	File f;

	f.open(_databasePrefix + ".obs");

	// Get number of entries in file
	f.scanf("%d", &_objectsNum);
	printf("%d obs entries\n", _objectsNum);

	_objects = new Object[_objectsNum];

	// There are less objects than reported
	for (int i = 0; i < _objectsNum; ++i) {
		int index;

		f.scanf("%d", &index);

		f.scanf(
			"%s %d\n"
			"%[^\r]\n"
			"%d %d %d\n"
			"%d %d %d %d %d %d %d %d\n"
			"%d %d %d %d %d\n"
			"%d %d",
			_objects[index].name,
			&(_objects[index].data1),
			_objects[index].desc,
			&(_objects[index].type),
			&(_objects[index].data2),
			&(_objects[index].proc),
			&(_objects[index].data4),
			&(_objects[index].data5),
			&(_objects[index].isVisible),
			&(_objects[index].data6),
			&(_objects[index].isSprite),
			&(_objects[index].isUseImmediate),
			&(_objects[index].data9),
			&(_objects[index].data10),
			&(_objects[index].price),
			&(_objects[index].data11),
			&(_objects[index].spellCost),
			&(_objects[index].data12),
			&(_objects[index].data13),
			&(_objects[index].locationType),
			&(_objects[index].locationId));

		stripUndies(_objects[index].desc);

		if (_objects[index].locationType == 1) {
			f.scanf("%d %d %d %d",
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
			_objects[i].data5,
			_objects[i].isVisible,
			_objects[i].data6,
			_objects[i].isSprite,
			_objects[i].isUseImmediate,
			_objects[i].data9,
			_objects[i].data10,
			_objects[i].price,
			_objects[i].data11,
			_objects[i].spellCost,
			_objects[i].data12,
			_objects[i].data13,
			_objects[i].locationType,
			_objects[i].locationId,
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
	f.scanf("%d", &entries);

	for (int i = 0; i < entries; ++i) {
		int loc;
		EventLink ev;

		f.scanf("%d %d %d\n_\n", &loc, &(ev.data1), &(ev.data2));

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
			if (_objects[i].locationType == 1)
				_locations[_objects[i].locationId].objects.push_back(i);
			else
				_characters[_objects[i].locationId].inventory.push_back(i);
		}
	}
}

void Database::initCharacterLocs() {
	for (int i = 0; i < _charactersNum; ++i)
		_locations[_characters[i].locationId].characters.push_back(i);
}

void Database::initProcs() {
	File f;

	f.open(_databasePrefix + ".pro");

	// Get number of entries in file
	f.scanf("%d", &_varSize);

	printf("var size: %d\n", _varSize);
	_dataSegment = new uint16[_varSize];

	f.scanf("%d", &_procsNum);

	printf("proc entries: %d\n", _procsNum);
	_processes = new Process[_procsNum];

	for (int i = 0; i < _procsNum; ++i) {
		int index;
		int cmd, opcode;

		f.scanf("%d", &index);

		f.scanf("\n%[^\r]\n", _processes[index].name);

		f.scanf("%d", &cmd);

		// Read commands
		while (cmd != -1) {
			Command cmdObject;
			cmdObject.cmd = cmd;

			// Read special cmd with value
			if (cmd == 319 || cmd == 320 || cmd == 321) {
				f.scanf("%hu", &(cmdObject.value));
			}

			// Read opcodes
			f.scanf("%d", &opcode);

			while (opcode != -1) {
				OpCode opObject;
				opObject.opcode = opcode;

				switch (opcode) {
					// string + int/short
					case 474:
						f.scanf("%s %d", opObject.arg1, &(opObject.arg2));
						break;

					// string
					case 467:
					case 469:
						f.scanf("%s", opObject.arg1);
						break;

					// string + int + int + int
					case 468:
						f.scanf("%s %d %d %d", opObject.arg1, &(opObject.arg2),
						       &(opObject.arg3), &(opObject.arg4));
						break;

					// short + string - never reached
					case 99999:
						f.scanf("%d %s", &(opObject.arg2), opObject.arg1);
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
						f.scanf("%d", &(opObject.arg2));
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
						f.scanf("%d %d", &(opObject.arg2), &(opObject.arg3));
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
						f.scanf("%d %d %d", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4));
						break;

					// int + int + int + int + int
					case 438:
						f.scanf("%d %d %d %d %d", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4),
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

				f.scanf("%d", &opcode);
			}

			_processes[index].commands.push_back(cmdObject);

			f.scanf("%d", &cmd);
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

	while (f.scanf("%s", keyword) != EOF) {

		if (strcmp(keyword, "LOC") == 0) {
			f.scanf("%hd", &locIndex);

		} else if (strcmp(keyword, "exits") == 0) {
			f.scanf("%d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exit = parmData;

		} else if (strcmp(keyword, "exit_locs") == 0) {
			f.scanf("%d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exitLoc = parmData;

		} else if (strcmp(keyword, "exit_boxs") == 0) {
			f.scanf("%d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].exits[parmIndex].exitBox = parmData;

		} else if (strcmp(keyword, "BOX") == 0) {
			f.scanf("%hd", &boxIndex);


		} else if (strcmp(keyword, "x1") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].x1 = parmData;

		} else if (strcmp(keyword, "y1") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].y1 = parmData;

		} else if (strcmp(keyword, "x2") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].x2 = parmData;

		} else if (strcmp(keyword, "y2") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].y2 = parmData;

		} else if (strcmp(keyword, "priority") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].priority = parmData;

		} else if (strcmp(keyword, "z1") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].z1 = parmData;

		} else if (strcmp(keyword, "z2") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].z2 = parmData;

		} else if (strcmp(keyword, "attrib") == 0) {
			f.scanf("%d", &parmData);

			_locRoutes[locIndex].boxes[boxIndex].attrib = parmData;

		} else if (strcmp(keyword, "joins") == 0) {
			f.scanf("%d %d", &parmIndex, &parmData);

			_locRoutes[locIndex].boxes[boxIndex].joins[parmIndex] = parmData;

		// Skip the entire line
		} else {
			f.scanf("%*[^\n]");
		}
	}

	f.close();
}

void Database::stripUndies(char *s) {
	for (int i = 0; s[i] != '\0'; ++i)
		if (s[i] == '_')
			s[i] = ' ';
}
} // End of namespace Kom
