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

#ifndef DARKSEED2_ROOM_H
#define DARKSEED2_ROOM_H

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/objects.h"

namespace DarkSeed2 {

class Variables;
class DATFile;
class ScriptChunk;
class Resources;

enum RoomVerb {
	kRoomVerbEntry = 0,
	kRoomVerbMirror,
	kRoomVerbMusic,
	kRoomVerbPalette,
	kRoomVerbSprite,
	kRoomVerbNone
};


class Room : public ObjectContainer {
public:
	Room(Variables &variables);
	~Room();

	void clear();

	bool parse(DATFile &room, DATFile &objects);
	bool parse(const Resources &resources, const Common::String &room, const Common::String &objects);
	bool parse(const Resources &resources, const Common::String &base);

private:
	Variables *_variables;

	Common::String _background;
	Common::String _walkMap;

	uint32 _left, _top, _right, _bottom;

	uint32 _walkMapArg1, _walkMapArg2;

	int32 _scaleFactor1, _scaleFactor2, _scaleFactor3;

	Common::Array< Common::List<ScriptChunk *> > _scripts;

	static RoomVerb parseRoomVerb(const Common::String &verb);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_ROOM_H
