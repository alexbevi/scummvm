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
#include "common/fs.h"
#include "common/util.h"

#include "kom/kom.h"
#include "kom/database.h"

#define LINE_BUFFER_SIZE 50

using Common::File;

namespace Kom {

char line[LINE_BUFFER_SIZE];

Database::Database(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm) {
	_locations = 0;
	_characters = 0;
	_objects = 0;
}

Database::~Database() {
	delete[] _locations;
	delete[] _characters;
	delete[] _objects;
	delete[] _dataSegment;
	delete[] _processes;
	delete[] _convIndex;
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

	assert(f.readLine(line, LINE_BUFFER_SIZE));

	// Get number of entries in file
	sscanf(line, "%d", &_locationsNum);
	printf("%d loc entries\n", _locationsNum);

	// Skip empty line
	f.readLine(line, LINE_BUFFER_SIZE);

	_locations = new Location[_locationsNum];

	for (int i = 0; i < _locationsNum; ++i) {
		int index;

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d", &index);

		sscanf(line, "%*d %s %d %d", 
			_locations[index].name,
			&(_locations[index].data1),
			&(_locations[index].data2));

		f.readLine(_locations[index].desc, 50);
		stripUndies(_locations[index].desc);

		// Skip seperator lines
		f.readLine(line, LINE_BUFFER_SIZE);
		f.readLine(line, LINE_BUFFER_SIZE);
	}

	f.close();
/*	for (int i = 0; i < _locationsNum; ++i) {
		printf("%d %s %d %d %s\n", 
			i,
			_locations[i].name,
			_locations[i].data1,
			_locations[i].data2,
			_locations[i].desc);
	}*/
}

void Database::initCharacters() {
	File f;

	f.open(_databasePrefix + ".chr");

	assert(f.readLine(line, LINE_BUFFER_SIZE));

	// Get number of entries in file
	sscanf(line, "%d", &_charactersNum);
	printf("%d chr entries\n", _charactersNum);

	// Skip empty line
	f.readLine(line, LINE_BUFFER_SIZE);

	_characters = new Character[_charactersNum];

	for (int i = 0; i < _charactersNum; ++i) {
		int index;

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d", &index);

		sscanf(line, "%*d %s %d %d", 
			_characters[index].name,
			&(_characters[index].data1),
			&(_characters[index].data2));

		f.readLine(_characters[index].desc, 50);
		stripUndies(_characters[index].desc);
		
		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d", &(_characters[index].proc));

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d %d %d %d", 
			&(_characters[index].locationId),
			&(_characters[index].data4),
			&(_characters[index].data5),
			&(_characters[index].data6),
			&(_characters[index].data7));

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d %d", 
			&(_characters[index].data8),
			&(_characters[index].data9),
			&(_characters[index].hitpoints));

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d %d %d", 
			&(_characters[index].data10),
			&(_characters[index].data11),
			&(_characters[index].data12),
			&(_characters[index].data13));

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d %d %d", 
			&(_characters[index].data14),
			&(_characters[index].data15),
			&(_characters[index].data16),
			&(_characters[index].spellpoints));

		// Skip seperator lines
		f.readLine(line, LINE_BUFFER_SIZE);
		f.readLine(line, LINE_BUFFER_SIZE);
	}

	f.close();
/*	for (int i = 0; i < _charactersNum; ++i) {
		printf("%d %s %d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", 
			i,
			_characters[i].name,
			_characters[i].data1,
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

	assert(f.readLine(line, LINE_BUFFER_SIZE));

	// Get number of entries in file
	sscanf(line, "%d", &_objectsNum);
	printf("%d obs entries\n", _objectsNum);

	// Skip empty line
	f.readLine(line, LINE_BUFFER_SIZE);

	_objects = new Object[_objectsNum];

	for (int i = 0; i < _objectsNum; ++i) {
		int index;

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d", &index);

		sscanf(line, "%*d %s %d", 
			_objects[index].name,
			&(_objects[index].data1));

		f.readLine(_objects[index].desc, 50);
		stripUndies(_objects[index].desc);
		
		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d %d", 
			&(_objects[index].type),
			&(_objects[index].data2),
			&(_objects[index].proc));

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d %d %d %d %d %d %d", 
			&(_objects[index].data4),
			&(_objects[index].data5),
			&(_objects[index].data6),
			&(_objects[index].isUseImmediate),
			&(_objects[index].isVisible),
			&(_objects[index].data8),
			&(_objects[index].data9),
			&(_objects[index].data10));

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d %d %d %d", 
			&(_objects[index].price),
			&(_objects[index].data11),
			&(_objects[index].spellCost),
			&(_objects[index].data12),
			&(_objects[index].data13));

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d %d", 
			&(_objects[index].locationType),
			&(_objects[index].locationId));

		if (_objects[index].locationType == 1)
			sscanf(line, "%*d %*d %d %d %d %d", 
				&(_objects[index].data15),
				&(_objects[index].data16),
				&(_objects[index].data17),
				&(_objects[index].data18));

		// Skip seperator line
		f.readLine(line, LINE_BUFFER_SIZE);
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
			_objects[i].data6,
			_objects[i].isUseImmediate,
			_objects[i].isVisible,
			_objects[i].data8,
			_objects[i].data9,
			_objects[i].data10,
			_objects[i].price,
			_objects[i].data11,
			_objects[i].spellCost,
			_objects[i].data12,
			_objects[i].data13,
			_objects[i].data14,
			_objects[i].locationType,
			_objects[i].locationId,
			_objects[i].data16,
			_objects[i].data17,
			_objects[i].data18);
	}*/
}

void Database::initEvents() {
	int entries;
	File f;

	f.open(_databasePrefix + ".box");

	assert(f.readLine(line, LINE_BUFFER_SIZE));

	// Get number of entries in file
	sscanf(line, "%d", &entries);
	printf("%d event entries\n", entries);

	for (int i = 0; i < entries; ++i) {
		int loc;
		EventLink ev;

		f.readLine(line, LINE_BUFFER_SIZE);
		sscanf(line, "%d", &loc);

		sscanf(line, "%*d %d %d", &(ev.data1), &(ev.data2)); 

		_locations[loc].events.push_back(ev);

		// Skip unused data line
		f.readLine(line, LINE_BUFFER_SIZE);
	}

	f.close();
/*	for (int i = 0; i < entries; ++i) {
		printf("%d %s %d %d %s\n", 
			i,
			_locations[i].name,
			_locations[i].data1,
			_locations[i].data2,
			_locations[i].desc);
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
	char *fileBuffer, *originalFileBuffer;
	int bytesRead;

	f.open(_databasePrefix + ".pro");
	fileBuffer = originalFileBuffer = new char[f.size() + 1];
	f.read(fileBuffer, f.size());
	fileBuffer[f.size()] = '\0';
	f.close();

	// Get number of entries in file
	sscanf(fileBuffer, "%d%n", &_varSize, &bytesRead);
	fileBuffer += bytesRead;

	printf("var size: %d\n", _varSize);
	_dataSegment = new uint16[_varSize];

	sscanf(fileBuffer, "%d%n", &_procsNum, &bytesRead);
	fileBuffer += bytesRead;

	printf("proc entries: %d\n", _procsNum);
	_processes = new Process[_procsNum];

	for (int i = 0; i < _procsNum; ++i) {
		int index;
		int cmd, opcode;

		sscanf(fileBuffer, "%d%n", &index, &bytesRead);
		fileBuffer += bytesRead;

		sscanf(fileBuffer, "\n%[^\n]\n%n", _processes[index].name, &bytesRead); 
		fileBuffer += bytesRead;

		sscanf(fileBuffer, "%d%n", &cmd, &bytesRead); 
		fileBuffer += bytesRead;

		// Read commands
		while (cmd != -1) {
			Command cmdObject;
			cmdObject.cmd = cmd;

			// Read special cmd with value
			if (cmd == 319 || cmd == 320 || cmd == 321) {
				sscanf(fileBuffer, "%hu%n", &(cmdObject.value), &bytesRead); 
				fileBuffer += bytesRead;
			}

			// Read opcodes
			sscanf(fileBuffer, "%d%n", &opcode, &bytesRead); 
			fileBuffer += bytesRead;

			while (opcode != -1) {
				OpCode opObject;
				opObject.opcode = opcode;

				switch (opcode) {
					// string + int/short
					case 474:
						sscanf(fileBuffer, "%s %d%n", opObject.arg1, &(opObject.arg2), &bytesRead); 
						fileBuffer += bytesRead;
						break;

					// string
					case 467:
					case 469:
						sscanf(fileBuffer, "%s%n", opObject.arg1, &bytesRead); 
						fileBuffer += bytesRead;
						break;

					// string + int + int + int
					case 468:
						sscanf(fileBuffer, "%s %d %d %d%n", opObject.arg1, &(opObject.arg2),
						       &(opObject.arg3), &(opObject.arg4), &bytesRead); 
						fileBuffer += bytesRead;
						break;

					// short + string - never reached
					case 99999:
						sscanf(fileBuffer, "%d %s%n", &(opObject.arg2), opObject.arg1, &bytesRead); 
						fileBuffer += bytesRead;
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
						sscanf(fileBuffer, "%d%n", &(opObject.arg2), &bytesRead); 
						fileBuffer += bytesRead;
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
						sscanf(fileBuffer, "%d %d%n", &(opObject.arg2), &(opObject.arg3), &bytesRead); 
						fileBuffer += bytesRead;
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
						sscanf(fileBuffer, "%d %d %d%n", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4), &bytesRead); 
						fileBuffer += bytesRead;
						break;

					// int + int + int + int + int
					case 438:
						sscanf(fileBuffer, "%d %d %d %d %d%n", &(opObject.arg2), &(opObject.arg3), &(opObject.arg4),
							   &(opObject.arg5), &(opObject.arg6), &bytesRead); 
						fileBuffer += bytesRead;
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

				sscanf(fileBuffer, "%d%n", &opcode, &bytesRead); 
				fileBuffer += bytesRead;
			}

			_processes[index].commands.push_back(cmdObject);

			sscanf(fileBuffer, "%d%n", &cmd, &bytesRead); 
			fileBuffer += bytesRead;
		}
	}

	delete[] originalFileBuffer;
}

void Database::stripUndies(char *s) {
	for (int i = 0; s[i] != '\0'; ++i)
		if (s[i] == '_')
			s[i] = ' ';
}
} // End of namespace Kom
