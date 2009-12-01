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
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/roomconfig.h"
#include "engines/darkseed2/script.h"
#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

Room::Room(Variables &variables, Graphics &graphics) : ObjectContainer(variables) {
	_variables = &variables;
	_graphics  = &graphics;

	_confMan = 0;

	_background = 0;
	_walkMap    = 0;

	clear();
}

Room::~Room() {
	clear();
}

void Room::registerConfigManager(RoomConfigManager &configManager) {
	_confMan = &configManager;
}

void Room::clear() {
	_ready = false;

	if (_confMan)
		_confMan->deinitRoom();

	_graphics->clearRoomAnimations();
	_graphics->unregisterBackground();

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

	// Clear entry scripts
	for (Common::List<ScriptChunk *>::iterator it = _entryScripts.begin(); it != _entryScripts.end(); ++it)
		delete *it;
	_entryScripts.clear();

	// Clear animations
	for (AnimationMap::iterator it = _animations.begin(); it != _animations.end(); ++it)
		delete it->_value;

	_animations.clear();
}

const Common::String &Room::getName() const {
	return _name;
}

const Sprite &Room::getBackground() const {
	assert(_ready);

	return *_background;
}

Common::List<ScriptChunk *> &Room::getEntryScripts() {
	return _entryScripts;
}

Animation *Room::getAnimation(const Common::String &animation) {
	if (!_animations.contains(animation))
		return 0;

	return _animations.getVal(animation);
}

bool Room::parse(const Resources &resources, DATFile &room, DATFile &objects) {
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

		} else if (cmd->matchString("EntryStart")) {
			// Entry logic script block

			if (!parseEntryScripts(room))
				return false;

		} else if (cmd->matchString("*Start")) {
			// Start of a config section

			room.previous();

			if (!_confMan->parseConfig(room))
				return false;

		} else if (cmd->equalsIgnoreCase("EndID")) {
			// Room end

			break;

		} else {
			// Unknown

			warning("Room::parse(): Unkown command \"%s\" (\"%s\")",
					cmd->c_str(), args->c_str());
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

	assert(_confMan);

	clear();

	_name = base;

	Common::String room    = "ROOM";
	Common::String objects = "OBJ_";

	room    += base + ".DAT";
	objects += base + ".DAT";

	debugC(-1, kDebugRooms, "Parsing room \"%s\"", _name.c_str());

	return parse(resources, room, objects);
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

bool Room::addEntryScript(DATFile &room) {
	// Parse the script chunk
	ScriptChunk *script = new ScriptChunk(*_variables);
	if (!script->parse(room)) {
		delete script;
		return false;
	}

	// Add it to our list
	_entryScripts.push_back(script);

	return true;
}

bool Room::parseEntryScripts(DATFile &room) {
	const Common::String *cmd, *args;
	while (room.nextLine(cmd, args)) {
		if (cmd->equalsIgnoreCase("EntryEnd")) {
			// Reached the end of the entry block
			return true;
		} else if (!cmd->equalsIgnoreCase("Cond")) {
			warning("Room::parseEntryScripts(): First command must be a condition!");
			return false;
		}

		room.previous();

		if (!addEntryScript(room))
			return false;
	}

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

	_confMan->initRoom();

	_ready = true;

	return true;
}

bool Room::loadAnimation(Resources &resources, const Common::String &base) {
	if (_animations.contains(base))
		return true;

	Animation *animation = new Animation;

	if (!animation->load(resources, base)) {
		delete animation;
		return false;
	}

	_animations.setVal(base, animation);
	return true;
}

} // End of namespace DarkSeed2
