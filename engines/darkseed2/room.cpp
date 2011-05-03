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

#include "common/serializer.h"

#include "darkseed2/room.h"
#include "darkseed2/versionformats.h"
#include "darkseed2/variables.h"
#include "darkseed2/resources.h"
#include "darkseed2/graphics.h"
#include "darkseed2/datfile.h"
#include "darkseed2/roomconfig.h"
#include "darkseed2/script.h"
#include "darkseed2/sprite.h"
#include "darkseed2/graphicalobject.h"
#include "darkseed2/saveload.h"

namespace DarkSeed2 {

Room::Room(Variables &variables, ScriptRegister &scriptRegister, Graphics &graphics) :
	ObjectContainer(variables, scriptRegister) {

	_variables      = &variables;
	_scriptRegister = &scriptRegister;
	_graphics       = &graphics;

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
	ObjectContainer::clear();

	_ready = false;

	if (_confMan)
		_confMan->deinitRoom();

	// Remove graphics
	_graphics->clearAnimations();
	_graphics->unregisterBackground();

	// Remove all local variables
	_variables->clearLocal();

	_name.clear();
	_roomFile.clear();
	_objsFile.clear();
	_backgroundFile.clear();
	_walkMapFile.clear();

	_clipScale = 1;

	delete _background;
	delete _walkMap;

	_background = 0;
	_walkMap    = 0;

	_walkMapYTop        =  0;
	_walkMapYResolution = 10;

	for (int i = 0; i < 3; i++)
		_scaleFactors[i] = 0;

	// Clear entry scripts
	for (Common::List<ScriptChunk *>::iterator it = _entryScripts.begin(); it != _entryScripts.end(); ++it)
		delete *it;
	_entryScripts.clear();
	_entryScriptLines.clear();

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

const Sprite &Room::getWalkMap() const {
	assert(_ready);

	return *_walkMap;
}

int32 Room::getWalkMapYTop() const {
	return _walkMapYTop;
}

int32 Room::getWalkMapYResolution() const {
	return _walkMapYResolution;
}

const int32 *Room::getScaleFactors() const {
	return _scaleFactors;
}

void Room::clipToRoom(Common::Rect &rect) const {
	rect.clip(_area);
}

const Common::Rect &Room::getClipRect() const {
	return _area;
}

Common::List<ScriptChunk *> &Room::getEntryScripts() {
	return _entryScripts;
}

Animation *Room::getAnimation(const Common::String &animation) {
	if (!_animations.contains(animation))
		return 0;

	return _animations.getVal(animation);
}

bool Room::parse(Resources &resources, DATFile &room, DATFile &objects) {
	_clipScale = resources.getVersionFormats().getHotspotScale();

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

bool Room::parse(Resources &resources,
		const Common::String &room, const Common::String &objects) {

	if (!resources.hasResource(room) || !resources.hasResource(objects))
		return false;

	_roomFile = room;
	_objsFile = objects;

	Common::SeekableReadStream *resRoom    = resources.getResource(room);
	Common::SeekableReadStream *resObjects = resources.getResource(objects);

	DATFile roomParser(room, *resRoom);
	DATFile objectsParser(objects, *resObjects);

	bool result = parse(resources, roomParser, objectsParser);

	delete resRoom;
	delete resObjects;

	return result;
}

bool Room::parse(Resources &resources, const Common::String &base) {
	assert(_confMan);

	clear();

	_name = base;

	debugC(-1, kDebugRooms, "Parsing room \"%s\"", _name.c_str());

	return parse(resources, Resources::addExtension("ROOM" + base, "DAT"),
	                        Resources::addExtension("OBJ_" + base, "DAT"));
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
		_walkMapYTop        = atoi(lArgs[1].c_str());
		_walkMapYResolution = atoi(lArgs[2].c_str());
	} else {
		_walkMapYTop        =  0;
		_walkMapYResolution = 10;
	}


	return true;
}

bool Room::setScaleFactor(const Common::String &args) {
	Common::Array<int32> lArgs = DATFile::argGetInts(args, 3, 0);

	for (int i = 0; i < 3; i++)
		_scaleFactors[i] = lArgs[i];

	return true;
}

bool Room::setDimensions(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 4) {
		warning("Room::setDimensions(): Broken arguments");
		return false;
	}

	_area = Common::Rect(atoi(lArgs[0].c_str()) / _clipScale, atoi(lArgs[1].c_str()) / _clipScale,
	                     atoi(lArgs[2].c_str()) / _clipScale, atoi(lArgs[3].c_str()) / _clipScale);

	return true;
}

bool Room::addEntryScript(DATFile &room) {
	// Parse the script chunk
	ScriptChunk *script = new ScriptChunk(*_variables, *_scriptRegister);
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

		_entryScriptLines.push_back(room.getLineNumber());
		if (!addEntryScript(room))
			return false;
	}

	return true;
}

bool Room::loadSprites(Resources &resources) {
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

	if (!_background->loadFromRoomImage(resources, _backgroundFile)) {
		warning("Room::setup(): Can't load background");
		return false;
	}

	switch (resources.getVersionFormats().getWalkMapType()) {
	case kWalkMapTypeMAP:
		warning("TODO: Sega Saturn walk maps");
		break;
	case kWalkMapTypeMac:
		warning("TODO: Mac walk maps");
		break;
	case kWalkMapTypeBMP:
		if (!_walkMap->loadFromImage(resources, _walkMapFile)) {
			warning("Room::setup(): Can't load walk map");
			return false;
		}
		break;
	}

	return true;
}

bool Room::setup(Resources &resources) {
	if (!loadSprites(resources))
		return false;

	_ready = true;

	return true;
}

void Room::init() {
	_confMan->initRoom();
}

Animation *Room::loadAnimation(Resources &resources, const Common::String &base) {
	if (_animations.contains(base))
		return _animations.getVal(base);

	Animation *animation = new Animation;

	if (!animation->load(resources, base)) {
		delete animation;
		return 0;
	}

	_animations.setVal(base, animation);

	return animation;
}

void Room::scaleAnimation(const Common::String &animation, frac_t scale) {
	Animation *anim = getAnimation(animation);
	if (!anim)
		// No animation
		return;

	anim->setScale(scale);
}

bool Room::saveLoad(Common::Serializer &serializer, Resources &resources) {
	SaveLoad::sync(serializer, _ready);
	if (!_ready)
		return true;

	SaveLoad::sync(serializer, _name);
	SaveLoad::sync(serializer, _roomFile);
	SaveLoad::sync(serializer, _objsFile);
	SaveLoad::sync(serializer, _backgroundFile);
	SaveLoad::sync(serializer, _walkMapFile);

	SaveLoad::sync(serializer, _walkMapYTop);
	SaveLoad::sync(serializer, _walkMapYResolution);

	SaveLoad::sync(serializer, _area);

	SaveLoad::sync(serializer, _scaleFactors[0]);
	SaveLoad::sync(serializer, _scaleFactors[1]);
	SaveLoad::sync(serializer, _scaleFactors[2]);

	SaveLoad::sync(serializer, _entryScriptLines);

	return true;
}

bool Room::loading(Resources &resources) {
	ObjectContainer::clear();

	if (!_ready)
		return true;

	if (!loadSprites(resources))
		return false;

	_entryScripts.clear();

	Common::SeekableReadStream *resRoom    = resources.getResource(_roomFile);
	Common::SeekableReadStream *resObjects = resources.getResource(_objsFile);

	DATFile roomParser(_roomFile, *resRoom);
	DATFile objectsParser(_objsFile, *resObjects);

	delete resRoom;
	delete resObjects;

	Common::List<uint32>::const_iterator line;
	for (line = _entryScriptLines.begin(); line != _entryScriptLines.end(); ++line) {
		roomParser.seekTo(*line);

		ScriptChunk *script = new ScriptChunk(*_variables, *_scriptRegister);
		if (!script->parse(roomParser)) {
			delete script;
			return false;
		}

		_entryScripts.push_back(script);
	}

	if (!ObjectContainer::parse(objectsParser))
		return false;

	return true;
}

} // End of namespace DarkSeed2
