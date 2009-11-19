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

#include "common/stream.h"

#include "engines/darkseed2/room.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/script.h"

namespace DarkSeed2 {

static const char *roomVerb[] = {
	"EntryStart", "MirrorStart", "MusicStart", "PaletteStart", "SpriteStart"
};

Room::Room(Variables &variables) : ObjectContainer(variables), _variables(&variables) {
	clear();
}

Room::~Room() {
	clear();
}

void Room::clear() {
	// Remove all local variables
	_variables->clearLocal();

	_background.clear();
	_walkMap.clear();

	_left   = 0;
	_top    = 0;
	_right  = 0;
	_bottom = 0;

	_walkMapArg1 = 0;
	_walkMapArg2 = 0;

	_scaleFactor1 = 0;
	_scaleFactor2 = 0;
	_scaleFactor3 = 0;

	for (uint i = 0; i < _scripts.size(); i++)
		for (Common::List<ScriptChunk *>::iterator it = _scripts[i].begin(); it != _scripts[i].end(); ++it)
			delete *it;

	_scripts.clear();
	_scripts.resize(kRoomVerbNone);
}

bool Room::parse(DATFile &room, DATFile &objects) {
	RoomVerb curVerb = kRoomVerbNone;

	const Common::String *cmd, *args;
	while (room.nextLine(cmd, args)) {
		if (cmd->equalsIgnoreCase("BackDrop")) {
			// The background image

			_background = *args;
		} else if (cmd->equalsIgnoreCase("WalkMap")) {
			// Map that shows the walkable areas

			Common::Array<Common::String> lArgs = DATFile::argGet(*args);

			if ((lArgs.size() != 1) && (lArgs.size() != 3)) {
				warning("Room::parse(): WalkMap arguments broken");
				return false;
			}

			_walkMap = lArgs[0];

			if (lArgs.size() == 3) {
				_walkMapArg1 = atoi(lArgs[1].c_str());
				_walkMapArg2 = atoi(lArgs[2].c_str());
			}

			_scaleFactor1 = atoi(lArgs[0].c_str());
			_scaleFactor2 = atoi(lArgs[1].c_str());
			_scaleFactor3 = atoi(lArgs[2].c_str());

		} else if (cmd->equalsIgnoreCase("ScaleFactor")) {
			// How the actors will be scaled when walking

			Common::Array<Common::String> lArgs = DATFile::argGet(*args);

			if (lArgs.size() != 3) {
				warning("Room::parse(): ScaleFactor arguments broken");
				return false;
			}

		} else if (cmd->equalsIgnoreCase("ObjXY")) {
			// Room coordinates

			Common::Array<Common::String> lArgs = DATFile::argGet(*args);

			// Need to be 4
			if (lArgs.size() != 4) {
				warning("Room::parse(): ObjXY arguments broken");
				return false;
			}

			_left   = atoi(lArgs[0].c_str());
			_top    = atoi(lArgs[1].c_str());
			_right  = atoi(lArgs[2].c_str()) - 1;
			_bottom = atoi(lArgs[3].c_str()) - 1;

		} else if (cmd->equalsIgnoreCase("LocalVar")) {
			// Local Variable
			_variables->addLocal(*args);
		} else if (cmd->matchString("*Start")) {
			// Start of a verb section

			curVerb = parseRoomVerb(*cmd);
			if (curVerb == kRoomVerbNone) {
				warning("Room::parse(): Unknown script verb \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
				return false;
			}

		} else if (cmd->equalsIgnoreCase("EndID")) {
			// Room end
			break;
		} else {
			// Need to be in a verb section
			if (curVerb == kRoomVerbNone) {
				warning("Room::parse(): Action without a verb \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
				return false;
			}

			// Rewind past the line we've just read
			room.previous();

			// Parse the script chunk
			ScriptChunk *script = new ScriptChunk(*_variables);
			if (!script->parse(room)) {
				delete script;
				return false;
			}

			// Add it to our list
			_scripts[(int) curVerb].push_back(script);
		}
	}

	return ObjectContainer::parse(objects);
}

RoomVerb Room::parseRoomVerb(const Common::String &verb) {
	for (int i = 0; i < kRoomVerbNone; i++)
		if (verb.equalsIgnoreCase(roomVerb[i]))
			return (RoomVerb) i;

	return kRoomVerbNone;
}

} // End of namespace DarkSeed2
