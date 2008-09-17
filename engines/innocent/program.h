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

#ifndef INNOCENT_PROGRAM_H
#define INNOCENT_PROGRAM_H

#include "common/list.h"
#include "common/stream.h"

#include "innocent/debug.h"
#include "innocent/resources.h"

namespace Innocent {
//

class Exit;
class Actor;

class Program : public StaticInspectable {
	DEBUG_INFO
public:
	Program(Common::ReadStream &file, uint16 id);
	~Program();

	uint16 begin();
	byte *localVariable(uint16 offset);
	uint16 roomHandler(uint16 room);
	byte *base() const { return _code; }

	SpriteInfo getSpriteInfo(uint16 index) const;

	void loadActors(Interpreter *i);
	void loadExits(Interpreter *i);

	Exit *getExit(uint16 index) const;
	Common::List<Exit *> exitsForRoom(uint16 room) const;

	Actor *actor(uint16 index) const;

private:
	Program() { /* can only be created from a file */ }

	void clearExits();

	uint16 entryPointOffset();

	byte *_code;
	byte _footer[0x10];
	Common::List<Actor *> _actors;
	Exit **_exits;
	uint16 _exitsCount;
};

} // End of namespace Innocent

#endif // INNOCENT_PROGRAM_H
