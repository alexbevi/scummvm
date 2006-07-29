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
			&(_objects[index].data7),
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
			_objects[i].data7,
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
	fileBuffer = originalFileBuffer = new char[f.size()];
	f.read(fileBuffer, f.size());
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

	for (int i = 0; i < 1 /* XXX: _procsNum*/; ++i) {
		int index;
		Command com;

		sscanf(fileBuffer, "%d%n", &index, &bytesRead);
		fileBuffer += bytesRead;

		sscanf(fileBuffer, "\n%[^\n]\n%n", _processes[index].name, &bytesRead); 
		fileBuffer += bytesRead;

		sscanf(fileBuffer, "%d%n", &(com.opcode), &bytesRead); 
		fileBuffer += bytesRead;

		while (com.opcode != -1) {
			break;
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
