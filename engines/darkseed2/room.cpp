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
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/script.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

static const char *roomVerb[] = {
	"EntryStart", "MirrorStart", "MusicStart", "PaletteStart", "SpriteStart"
};

Room::Room(Variables &variables, Graphics &graphics) : ObjectContainer(variables) {
	_variables = &variables;
	_graphics  = &graphics;

	_background = 0;
	_walkMap    = 0;

	clear();
}

Room::~Room() {
	clear();
}

void Room::clear() {
	_ready = false;

	// Remove all local variables
	_variables->clearLocal();

	_backgroundFile.clear();
	_walkMapFile.clear();

	delete _background;
	delete _walkMap;

	_background = 0;
	_walkMap    = 0;

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

const Common::String &Room::getName() const {
	return _name;
}

const Sprite &Room::getBackground() const {
	assert(_ready);

	return *_background;
}

Common::List<ScriptChunk *> &Room::getScripts(RoomVerb verb) {
	assert(verb < kRoomVerbNone);

	return _scripts[verb];
}

bool Room::parse(const Resources &resources, DATFile &room, DATFile &objects) {
	RoomVerb curVerb = kRoomVerbNone;

	const Common::String *cmd, *args;
	while (room.nextLine(cmd, args)) {
		if (cmd->equalsIgnoreCase("BackDrop")) {
			// The background image

			if (!setBackground(*args))
				return false;

		} else if (cmd->equalsIgnoreCase("WalkMap")) {
			// Map that shows the walkable areas

			if (!setWalkMap(*args))
				return false;

		} else if (cmd->equalsIgnoreCase("ScaleFactor")) {
			// How the actors will be scaled when walking

			if (!setScaleFactor(*args))
				return false;

		} else if (cmd->equalsIgnoreCase("ObjXY")) {
			// Room coordinates

			if (!setDimensions(*args))
				return false;

		} else if (cmd->equalsIgnoreCase("LocalVar")) {
			// Local Variable

			_variables->addLocal(*args);

		} else if (cmd->matchString("*Start")) {
			// Start of a verb section

			if (!setVerb(*cmd, curVerb))
				return false;

		} else if (cmd->equalsIgnoreCase("EndID")) {
			// Room end

			break;

		} else {
			// Script chunk

			if (!addScriptChunk(*cmd, room, curVerb))
				return false;

		}
	}

	if (!ObjectContainer::parse(objects))
		return false;

	if (!setup(resources))
		return false;

	return true;
}

bool Room::parse(const Resources &resources,
		const Common::String &room, const Common::String &objects) {

	if (!resources.hasResource(room) || !resources.hasResource(objects))
		return false;

	Resource *resRoom    = resources.getResource(room);
	Resource *resObjects = resources.getResource(objects);

	DATFile roomParser(*resRoom);
	DATFile objectsParser(*resObjects);

	bool result = parse(resources, roomParser, objectsParser);

	delete resRoom;
	delete resObjects;

	return result;
}

bool Room::parse(const Resources &resources,
		const Common::String &base) {

	clear();

	_name = base;

	Common::String room    = "ROOM";
	Common::String objects = "OBJ_";

	room    += base + ".DAT";
	objects += base + ".DAT";

	return parse(resources, room, objects);
}

RoomVerb Room::parseRoomVerb(const Common::String &verb) {
	for (int i = 0; i < kRoomVerbNone; i++)
		if (verb.equalsIgnoreCase(roomVerb[i]))
			return (RoomVerb) i;

	return kRoomVerbNone;
}

bool Room::setBackground(const Common::String &args) {
	if (DATFile::argCount(args) != 1) {
		warning("Room::setBackground(): Broken arguments");
		return false;
	}

	_backgroundFile = args;

	return true;
}

bool Room::setWalkMap(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if ((lArgs.size() != 1) && (lArgs.size() != 3)) {
		warning("Room::setWalkMap(): Broken arguments");
		return false;
	}

	_walkMapFile = lArgs[0];

	if (lArgs.size() == 3) {
		_walkMapArg1 = atoi(lArgs[1].c_str());
		_walkMapArg2 = atoi(lArgs[2].c_str());
	}

	return true;
}

bool Room::setScaleFactor(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 3) {
		warning("Room::setScaleFactor(): Broken arguments");
		return false;
	}

	_scaleFactor1 = atoi(lArgs[0].c_str());
	_scaleFactor2 = atoi(lArgs[1].c_str());
	_scaleFactor3 = atoi(lArgs[2].c_str());

	return true;
}

bool Room::setDimensions(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 4) {
		warning("Room::setDimensions(): Broken arguments");
		return false;
	}

	_area = Common::Rect(atoi(lArgs[0].c_str()), atoi(lArgs[1].c_str()),
	                     atoi(lArgs[2].c_str()), atoi(lArgs[3].c_str()));

	return true;
}

bool Room::setVerb(const Common::String &cmd, RoomVerb &curVerb) {
	curVerb = parseRoomVerb(cmd);
	if (curVerb == kRoomVerbNone) {
		warning("Room::setVerb(): Unknown script verb \"%s\"", cmd.c_str());
		return false;
	}

	return true;
}

bool Room::addScriptChunk(const Common::String &cmd, DATFile &room, RoomVerb curVerb) {
	// Need to be in a verb section
	if (curVerb == kRoomVerbNone) {
		warning("Room::addScriptChunk(): Action without a verb \"%s\"", cmd.c_str());
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

	return true;
}

bool Room::setup(const Resources &resources) {
	if (_backgroundFile.empty()) {
		warning("Room::setup(): No background");
		return false;
	}

	if (_walkMapFile.empty()) {
		warning("Room::setup(): No walk map");
		return false;
	}

	delete _background;
	delete _walkMap;

	_background = new Sprite();
	_walkMap    = new Sprite();

	if (!_background->loadFromBMP(resources, _backgroundFile)) {
		warning("Room::setup(): Can't load background");
		return false;
	}

	if (!_walkMap->loadFromBMP(resources, _walkMapFile)) {
		warning("Room::setup(): Can't load walk map");
		return false;
	}

	_ready = true;

	return true;
}

} // End of namespace DarkSeed2
