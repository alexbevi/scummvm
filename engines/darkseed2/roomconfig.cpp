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

#include "engines/darkseed2/roomconfig.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/room.h"
#include "engines/darkseed2/music.h"

namespace DarkSeed2 {

RoomConfig::RoomConfig(Variables &variables) : _variables(&variables) {
	_loaded  = false;
	_running = false;

	_waitUntil = 0;

	_state        = false;
	_stateChanged = false;
}

RoomConfig::~RoomConfig() {
}

bool RoomConfig::isLoaded() const {
	return _loaded;
}

bool RoomConfig::isRunning() const {
	return _running;
}

bool RoomConfig::stateChanged() const {
	return _stateChanged;
}

bool RoomConfig::parse(DATFile &dat) {
	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if (cmd->matchString("*End")) {
			// Reached the end of this RoomConfig

			dat.previous();
			break;

		} else if (cmd->equalsIgnoreCase("Cond")) {
			// A primary condition

			if (!_conditions.empty()) {
				// Reached the end of this RoomConfig
				dat.previous();
				break;
			}

			_conditions.push_back(*args);
		} else if (cmd->equalsIgnoreCase("Cond2")) {
			// A secondary condition

			_conditions.push_back(*args);
		} else if (cmd->equalsIgnoreCase("Change")) {
			// A variables change set

			_changes.push_back(*args);
		} else {
			// Everything else is RoomConfig-specific

			if (!parseLine(*cmd, *args))
				return false;
		}
	}

	_loaded = true;

	return true;
}

bool RoomConfig::conditionsMet() {
	bool met = false;

	for (Common::List<Common::String>::const_iterator it = _conditions.begin(); it != _conditions.end(); ++it)
		if (_variables->evalCondition(*it)) {
			met = true;
			break;
		}

	_stateChanged = false;
	if (_state != met) {
		_stateChanged = true;
		_state = met;
	}

	return met;
}

void RoomConfig::run() {
	assert(_loaded);

	_running = true;
}

void RoomConfig::stop() {
	assert(_loaded);

	_running = false;
}

void RoomConfig::applyChanges() {
	for (Common::List<Common::String>::const_iterator it = _changes.begin(); it != _changes.end(); ++it) {
		if (!_changes.empty())
			debugC(-1, kDebugRoomConf, "RoomConfig: Apply change set [%s]", it->c_str());

		_variables->evalChange(*it);
	}
}

void RoomConfig::startWait(uint32 millis) {
	_waitUntil = g_system->getMillis() + millis;
}

void RoomConfig::resetWait() {
	_waitUntil = 0;
}

bool RoomConfig::waited() const {
	return g_system->getMillis() >= _waitUntil;
}


RoomConfigMusic::RoomConfigMusic(Variables &variables, Resources &resources, Music &music) :
		RoomConfig(variables) {

	_resources = &resources;
	_music     = &music;
}

RoomConfigMusic::~RoomConfigMusic() {
}

bool RoomConfigMusic::init() {
	return true;
}

void RoomConfigMusic::updateStatus() {
	assert(isLoaded());

	if (!conditionsMet())
		return;

	if (stateChanged())
		debugC(-1, kDebugRoomConf, "RoomConfigMusic: Changing music to \"%s\"", _midi.c_str());

	_music->playMID(*_resources, _midi);
	applyChanges();
}

bool RoomConfigMusic::parseLine(const Common::String &cmd, const Common::String &args) {
	if (cmd.equalsIgnoreCase("Midi")) {
		// Midi music change

		_midi = args;
	} else {
		// Unknown

		warning("RoomConfigMusic::parseLine(): Unkown command \"%s\" (\"%s\")",
				cmd.c_str(), args.c_str());
		return false;
	}

	return true;
}


RoomConfigSprite::RoomConfigSprite(Variables &variables, Resources &resources, Graphics &graphics) :
		RoomConfig(variables) {

	_resources = &resources;
	_graphics  = &graphics;

	for (int i = 0; i < 6; i++)
		_status[i] = 0;

	_spriteIDX = 0;
}

RoomConfigSprite::~RoomConfigSprite() {
}

bool RoomConfigSprite::init() {
	if (_sequence.empty() && (_status[4] != 0)) {
		// If we've got no sequence, but the status flag shows n changes,
		// create a sequence beginning from the first frame

		for (int i = 0; i < _status[4]; i++)
			_sequence.push_back(i);
	}

	if (!_graphics->getRoom().loadAnimation(*_resources, _anim))
		return false;

	return true;
}

void RoomConfigSprite::updateStatus() {
	assert(isLoaded());

	if (!isRunning()) {
		// Not running

		if (!conditionsMet())
			// But conditions are not met, abort
			return;

		if (stateChanged())
			if (!_sequenceString.empty())
				debugC(-1, kDebugRoomConf, "RoomConfigSprite: Running sequence [%s]", _sequenceString.c_str());

		run();
		resetWait();
		_curPos = 0;
	}

	if (!waited())
		// We still need to wait until we can display the next frame
		return;

	// Start the waiting timer for the next frame
	startWait(100);

	// Update animation frame
	_graphics->addRoomAnimation(_anim, _currentSprite, _sequence[_curPos], _status[0]);

	if (++_curPos >= _sequence.size()) {
		// We've reached the end
		stop();

		// Apply variable changes
		applyChanges();

		// Permanent animation?
		if (_status[4] != 1)
			// If not, remove the animation frame
			_graphics->removeRoomAnimation(_currentSprite);

	}

}

bool RoomConfigSprite::parseLine(const Common::String &cmd, const Common::String &args) {
	if        (cmd.equalsIgnoreCase("Anim")) {
		// An animation name
		_anim = args;

	} else if (cmd.equalsIgnoreCase("Status")) {
		// A status/flags line

		if (!parseStatus(args))
			return false;

	} else if (cmd.equalsIgnoreCase("Sequence")) {
		// A animation frame sequence

		if (!parseSequence(args))
			return false;

	} else if (cmd.equalsIgnoreCase("SpriteIDX")) {
		// Unknown

		_spriteIDX = atoi(args.c_str());

	} else if (cmd.equalsIgnoreCase("Effect")) {
		// Unknown

		_effect = args;

	} else if (cmd.equalsIgnoreCase("ScaleVal")) {
		// Unknown

		_scaleVal = args;

	} else if (cmd.equalsIgnoreCase("PosX")) {
		// Unknown

		_scaleVal = args;

	} else if (cmd.equalsIgnoreCase("PosY")) {
		// Unknown

		_scaleVal = args;

	} else if (cmd.equalsIgnoreCase("LoopCond")) {
		// Looping when this condition is met

		_loopCond = args;

	} else if (cmd.equalsIgnoreCase("LoopPoint")) {
		// Looping start and end index

		_loopPoint = args;

	} else if (cmd.equalsIgnoreCase("LoadCond")) {
		// Unknown

		_loadCond = args;

	} else if (cmd.equalsIgnoreCase("ChangeAt")) {
		// Unknown

		_changeAt = args;

	} else if (cmd.equalsIgnoreCase("Speech")) {
		// Unknown

		_speech = args;

	} else {
		// Unknown

		warning("RoomConfigSprite::parseLine(): Unkown command \"%s\" (\"%s\")",
				cmd.c_str(), args.c_str());
		return false;
	}

	return true;
}

bool RoomConfigSprite::parseStatus(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() > 6) {
		warning("RoomConfigSprite::parseStatus(): Broken arguments");
		return false;
	}

	for (uint i = 0; i < lArgs.size(); i++)
		_status[i] = atoi(lArgs[i].c_str());

	return true;
}

bool RoomConfigSprite::parseSequence(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	for (Common::Array<Common::String>::const_iterator it = lArgs.begin(); it != lArgs.end(); ++it)
		_sequence.push_back(atoi(it->c_str()) - 1);

	_sequenceString.clear();
	for (Common::Array<uint8>::const_iterator it = _sequence.begin(); it != _sequence.end(); ++it)
		_sequenceString += Common::String::printf("%d ", *it);
	_sequenceString.trim();

	return true;
}


RoomConfigPalette::RoomConfigPalette(Variables &variables, Resources &resources, Graphics &graphics) :
		RoomConfig(variables) {

	_resources = &resources;
	_graphics  = &graphics;

	_startIndex = 0;
	_endIndex   = 0;
}

RoomConfigPalette::~RoomConfigPalette() {
}

bool RoomConfigPalette::init() {
	return true;
}

void RoomConfigPalette::updateStatus() {
	assert(isLoaded());
}

bool RoomConfigPalette::parseLine(const Common::String &cmd, const Common::String &args) {
	if        (cmd.equalsIgnoreCase("StartIndex")) {
		// The start index

		_startIndex = atoi(args.c_str());
	} else if (cmd.equalsIgnoreCase("EndIndex")) {
		// The end index

		_endIndex = atoi(args.c_str());
	} else {
		// Unknown

		warning("RoomConfigPalette::parseLine(): Unkown command \"%s\" (\"%s\")",
				cmd.c_str(), args.c_str());
		return false;
	}

	return true;
}


RoomConfigMirror::RoomConfigMirror(Variables &variables, Resources &resources, Graphics &graphics) :
		RoomConfig(variables) {

	_resources = &resources;
	_graphics  = &graphics;

	for (int i = 0; i < 3; i++) {
		_posX [i] = 0;
		_posY [i] = 0;
		_scale[i] = 0;
	}
}

RoomConfigMirror::~RoomConfigMirror() {
}

bool RoomConfigMirror::init() {
	return true;
}

void RoomConfigMirror::updateStatus() {
	assert(isLoaded());
}

bool RoomConfigMirror::parseLine(const Common::String &cmd, const Common::String &args) {
	if        (cmd.equalsIgnoreCase("ClipXY")) {
		// The mirror's area

		if (!parseArea(args))
			return false;

	} else if (cmd.equalsIgnoreCase("PosX")) {
		// The mirror's X coordinate

		if (!parsePosX(args))
			return false;

	} else if (cmd.equalsIgnoreCase("PosY")) {
		// The mirror's Y coordinate

		if (!parsePosY(args))
			return false;

	} else if (cmd.equalsIgnoreCase("ScaleVal")) {
		// The mirror's scale

		if (!parseScale(args))
			return false;

	} else {
		// Unknown

		warning("RoomConfigMirror::parseLine(): Unkown command \"%s\" (\"%s\")",
				cmd.c_str(), args.c_str());
		return false;
	}

	return true;
}

bool RoomConfigMirror::parseArea(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 4) {
		warning("RoomConfigSprite::parseArea(): Broken arguments");
		return false;
	}

	_area.left   = atoi(lArgs[0].c_str());
	_area.top    = atoi(lArgs[1].c_str());
	_area.right  = atoi(lArgs[2].c_str());
	_area.bottom = atoi(lArgs[3].c_str());

	return true;
}

bool RoomConfigMirror::parsePosX(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() > 3) {
		warning("RoomConfigSprite::parsePosX(): Broken arguments");
		return false;
	}

	for (uint i = 0; i < lArgs.size(); i++)
		_posX[i] = atoi(lArgs[i].c_str());

	return true;
}

bool RoomConfigMirror::parsePosY(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() > 3) {
		warning("RoomConfigSprite::parsePosY(): Broken arguments");
		return false;
	}

	for (uint i = 0; i < lArgs.size(); i++)
		_posY[i] = atoi(lArgs[i].c_str());

	return true;
}

bool RoomConfigMirror::parseScale(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() > 3) {
		warning("RoomConfigSprite::parseScale(): Broken arguments");
		return false;
	}

	for (uint i = 0; i < lArgs.size(); i++)
		_scale[i] = atoi(lArgs[i].c_str());

	return true;
}


RoomConfigManager::RoomConfigManager(DarkSeed2Engine &vm) : _vm(&vm) {
}

RoomConfigManager::~RoomConfigManager() {
	clear();
}

void RoomConfigManager::initRoom() {
	for (Common::List<RoomConfig *>::iterator it = _configs.begin(); it != _configs.end(); ++it)
		if (!(*it)->init()) {
			// Delete it and hope for the best
			warning("Failed initializing a room config, removing it");
			delete *it;
			*it = 0;
		}

	// Remove all configs we've deleted from our list
	Common::List<RoomConfig *>::iterator config = _configs.begin();
	while (config != _configs.end()) {
		if (!*config)
			config = _configs.erase(config);
		else
			++config;
	}
}

void RoomConfigManager::deinitRoom() {
	clear();
}

void RoomConfigManager::updateStatus() {
	for (Common::List<RoomConfig *>::iterator it = _configs.begin(); it != _configs.end(); ++it)
		(*it)->updateStatus();
}

bool RoomConfigManager::parseConfig(DATFile &dat) {
	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if        (cmd->equalsIgnoreCase("MusicStart")) {

			if (!parseMusicConfigs(dat))
				return false;

		} else if (cmd->equalsIgnoreCase("SpriteStart")) {

			if (!parseSpriteConfigs(dat))
				return false;

		} else if (cmd->equalsIgnoreCase("PaletteStart")) {

			if (!parsePaletteConfigs(dat))
				return false;

		} else if (cmd->equalsIgnoreCase("MirrorStart")) {

			if (!parseMirrorConfigs(dat))
				return false;

		} else if (cmd->equalsIgnoreCase("EndID")) {
			// Reached the end of the room

			dat.previous();
			return true;

		} else {
			// Unknown

			warning("RoomConfigManager::parseConfig(): Unkown command \"%s\" (\"%s\")",
					cmd->c_str(), args->c_str());
			return false;
		}
	}

	return true;
}

void RoomConfigManager::clear() {
	for (Common::List<RoomConfig *>::iterator it = _configs.begin(); it != _configs.end(); ++it)
		delete *it;

	_configs.clear();
}

bool RoomConfigManager::parseMusicConfigs(DATFile &dat) {
	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if (cmd->matchString("*End")) {
			// Reached the end of this config block
			return true;
		} else if (!cmd->equalsIgnoreCase("Cond")) {
			warning("RoomConfigManager::parseMusicConfigs(): First command must be a condition!");
			return false;
		}

		dat.previous();

		RoomConfigMusic *music = new RoomConfigMusic(*_vm->_variables, *_vm->_resources, *_vm->_music);
		if (!music->parse(dat)) {
			delete music;
			return false;
		}

		_configs.push_back(music);
	}

	return true;
}

bool RoomConfigManager::parseSpriteConfigs(DATFile &dat) {
	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if (cmd->matchString("*End")) {
			// Reached the end of this config block
			return true;
		} else if (!cmd->equalsIgnoreCase("Cond")) {
			warning("RoomConfigManager::parseSpriteConfigs(): First command must be a condition!");
			return false;
		}

		dat.previous();

		RoomConfigSprite *music = new RoomConfigSprite(*_vm->_variables, *_vm->_resources, *_vm->_graphics);
		if (!music->parse(dat)) {
			delete music;
			return false;
		}

		_configs.push_back(music);
	}

	return true;
}

bool RoomConfigManager::parsePaletteConfigs(DATFile &dat) {
	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if (cmd->matchString("*End")) {
			// Reached the end of this config block
			return true;
		} else if (!cmd->equalsIgnoreCase("Cond")) {
			warning("RoomConfigManager::parsePaletteConfigs(): First command must be a condition!");
			return false;
		}

		dat.previous();

		RoomConfigPalette *music = new RoomConfigPalette(*_vm->_variables, *_vm->_resources, *_vm->_graphics);
		if (!music->parse(dat)) {
			delete music;
			return false;
		}

		_configs.push_back(music);
	}

	return true;
}

bool RoomConfigManager::parseMirrorConfigs(DATFile &dat) {
	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if (cmd->matchString("*End")) {
			// Reached the end of this config block
			return true;
		} else if (!cmd->equalsIgnoreCase("Cond")) {
			warning("RoomConfigManager::parseMirrorConfigs(): First command must be a condition!");
			return false;
		}

		dat.previous();

		RoomConfigMirror *music = new RoomConfigMirror(*_vm->_variables, *_vm->_resources, *_vm->_graphics);
		if (!music->parse(dat)) {
			delete music;
			return false;
		}

		_configs.push_back(music);
	}

	return true;
}

} // End of namespace DarkSeed2
