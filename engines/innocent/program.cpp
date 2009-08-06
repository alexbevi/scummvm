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

#include "innocent/program.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/actor.h"
#include "innocent/exit.h"
#include "innocent/resources.h"
#include "innocent/util.h"
#include "innocent/value.h"

namespace Innocent {

enum FooterOffsets {
	kExitsCount = 2,
	kActorsCount = 4,
	kExits = 8,
	kActors = 0xA,
	kSpriteMap = 0x0C,
	kEntryPointOffset = 0x0E
};

Program::Program(Common::ReadStream &file, uint16 id)
  :	_exits(0) {
	uint16 length = file.readUint16LE(); // for this length
	if (length > 25000)
		error("too large a program (%d)", length);

	_code = new byte[length];

	file.read(_code + 2, length - 2);
	Resources::descramble(_code + 2, length - 2);

	file.read(_footer, 0x10);
	snprintf(_debugInfo, 50, "block %d", id);
}

void Program::loadActors(Interpreter *in) {
	uint16 nactors = READ_LE_UINT16(_footer + kActorsCount);
	debugC(3, kDebugLevelFiles, "loading %d actors from the program file", nactors);
	uint16 actors = READ_LE_UINT16(_footer + kActors);
	for (int i = 0; i < nactors; ++i) {
		_actors.push_back(new Actor(CodePointer(actors, in)));
		actors += Actor::Size;
	}
}

Program::~Program() {
	delete[] _code;
	clearExits();
}

uint16 Program::begin() {
	return entryPointOffset();
}

uint16 Program::entryPointOffset() {
	return READ_LE_UINT16(_footer + kEntryPointOffset);
}

byte *Program::localVariable(uint16 offset) {
	return _code + offset;
}

uint16 Program::roomHandler(uint16 room) {
	byte *index = _code + 2;

	uint16 r;
	while ((r = READ_LE_UINT16(index)) != 0xffff) {
		index += 2;
		if (r == room) {
			uint16 offset = READ_LE_UINT16(index);
			return offset;
		}
	}

	return 0;
}

SpriteInfo Program::getSpriteInfo(uint16 index) const {
	byte *spritemap = _code + READ_LE_UINT16(_footer + kSpriteMap);

	return SpriteInfo(spritemap, index);
}

void Program::clearExits() {
	if (_exits) {
		for (int i = 0; i < _exitsCount; i++)
			delete _exits[i];
		delete[] _exits;
		_exits = 0;
	}
}

void Program::loadExits(Interpreter *in) {
	uint16 nexits = READ_LE_UINT16(_footer + kExitsCount);
	debugC(3, kDebugLevelFiles, "loading %d exits from the program file", nexits);
	uint16 exits = READ_LE_UINT16(_footer + kExits);

	clearExits();

	_exits = new Exit*[nexits];

	for (int i = 0; i < nexits; ++i) {
		_exits[i] = new Exit(CodePointer(exits, in));
		exits += Exit::Size;
	}

	_exitsCount = nexits;
}

Exit *Program::getExit(uint16 index) const {
	return _exits[index];
}

Common::List<Exit *> Program::exitsForRoom(uint16 room) const {
	Common::List<Exit *> room_exits;
	for (int i = 0; i < _exitsCount; i++)
		if (_exits[i]->room() == room)
			room_exits.push_back(_exits[i]);

	return room_exits;
}

Actor *Program::actor(uint16 index) const {
	Common::List<Actor *>::const_iterator it = _actors.begin();

	while (index) {
		it++;
		index--;
	}

	return *it;
}

} // End of namespace Innocent
