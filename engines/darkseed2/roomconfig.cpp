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
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/music.h"

namespace DarkSeed2 {

RoomConfig::RoomConfig(Variables &variables) : _variables(&variables) {
	_loaded  = false;
	_running = false;

	_waitUntil = 0;

	_state        = false;
	_stateChanged = false;

	_conditionsState       = false;
	_conditionsCheckedLast = 0;
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
	uint32 changedLast = _variables->getLastChanged();
	if (changedLast <= _conditionsCheckedLast) {
		// Nothing changed, return the cached result
		return _conditionsState;
	}

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

	_conditionsState       = met;
	_conditionsCheckedLast = changedLast;

	return _conditionsState;
}

bool RoomConfig::conditionsMet(const Common::String &cond) {
	return _variables->evalCondition(cond);
}

void RoomConfig::run() {
	assert(_loaded);

	_running = true;
}

void RoomConfig::stop() {
	assert(_loaded);

	_running = false;
}

bool RoomConfig::applyChanges() {
	if (_changes.empty())
		return false;

	for (Common::List<Common::String>::const_iterator it = _changes.begin(); it != _changes.end(); ++it) {
		if (!_changes.empty())
			debugC(-1, kDebugRoomConf, "RoomConfig: Apply change set [%s]", it->c_str());

		_variables->evalChange(*it);
	}

	return true;
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

bool RoomConfigMusic::updateStatus() {
	assert(isLoaded());

	if (!conditionsMet())
		return false;

	if (stateChanged())
		debugC(-1, kDebugRoomConf, "RoomConfigMusic: Changing music to \"%s\"", _midi.c_str());

	_music->playMID(*_resources, _midi);
	return applyChanges();
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


RoomConfigSprite::RoomConfigSprite(Variables &variables, Resources &resources,
		Graphics &graphics, Sound &sound) : RoomConfig(variables) {

	_resources = &resources;
	_graphics  = &graphics;
	_sound     = &sound;

	for (int i = 0; i < 6; i++)
		_status[i] = 0;

	_spriteIDX = 0;

	_loopStart = -1;
	_loopEnd   = -1;
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

	_frames.resize(_sequence.size());
	for (uint i = 0; i < _sequence.size(); i++) {
		_frames[i].frame = _sequence[i];

		if ((i >= _posX.size()) || (i >= _posY.size())) {
			_frames[i].x = -1;
			_frames[i].y = -1;
		} else {
			_frames[i].x = _posX[i];
			_frames[i].y = _posY[i];
		}
	}

	_sequence.clear();
	_posX.clear();
	_posY.clear();

	if (!_graphics->getRoom().loadAnimation(*_resources, _anim))
		return false;

	return true;
}

bool RoomConfigSprite::updateStatus() {
	assert(isLoaded());

	if (!isRunning()) {
		// Not running

		if (!conditionsMet()) {
			// But conditions are not met, abort
			_graphics->removeRoomAnimation(_currentSprite);
			return false;
		}

		if (stateChanged())
			if (!_sequenceString.empty())
				debugC(-1, kDebugRoomConf, "RoomConfigSprite: Running sequence [%s]", _sequenceString.c_str());

		run();
		resetWait();
		_curPos = 0;
	}

	if (!conditionsMet()) {
		stop();
		_graphics->removeRoomAnimation(_currentSprite);
		return true;
	}

	if (!waited())
		// We still need to wait until we can display the next frame
		return false;

	// Start the waiting timer for the next frame
	startWait(100);

	// Looping
	if ((_loopEnd > 0) && (_loopStart > 0) && (_curPos > ((uint32) _loopEnd)))
		if (conditionsMet(_loopCond))
			_curPos = _loopStart;

	// SFX playing
	for (uint i = 0; i < _effects.size(); i++)
		if (_effects[i].frameNum == _curPos) {
			debugC(-1, kDebugRoomConf, "RoomConfigSprite: Playing effect \"%s\"", _effects[i].effect.c_str());
			_sound->playWAV(*_resources, _effects[i].effect);
		}

	if (_curPos < _frames.size())
		// Update animation frame
		_graphics->addRoomAnimation(_anim, _currentSprite, _frames[_curPos].frame,
				_status[0], _frames[_curPos].x, _frames[_curPos].y);

	if (++_curPos >= _frames.size()) {
		_curPos = 0;

		// Apply variable changes
		return applyChanges();
	}

	return false;
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
		// SFX

		if (!parseEffect(args))
			return false;

	} else if (cmd.equalsIgnoreCase("ScaleVal")) {
		// Unknown

		_scaleVal = args;

	} else if (cmd.equalsIgnoreCase("PosX")) {
		// The frames' X coordinates

		if (!parsePackedIntLine(args, _posX))
			return false;

	} else if (cmd.equalsIgnoreCase("PosY")) {
		// The frames' Y coordinates

		if (!parsePackedIntLine(args, _posY))
			return false;

	} else if (cmd.equalsIgnoreCase("LoopCond")) {
		// Looping when this condition is met

		_loopCond = args;

	} else if (cmd.equalsIgnoreCase("LoopPoint")) {
		// Looping start and end index

		if (!parseLoopPoint(args))
			return false;

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
	if (!parsePackedIntLine(args, _sequence))
		return false;

	_sequenceString.clear();
	for (Common::Array<int32>::const_iterator it = _sequence.begin(); it != _sequence.end(); ++it)
		_sequenceString += Common::String::printf("%d ", *it);
	_sequenceString.trim();

	return true;
}

bool RoomConfigSprite::parseEffect(const Common::String &args) {
	Effect effect;

	const char *space = strchr(args.c_str(), ' ');
	if (!space) {
		effect.frameNum = 0;
		effect.effect   = args;

		_effects.push_back(effect);
		return true;
	}

	effect.frameNum = atoi(args.c_str()) - 1;

	while (*space == ' ')
		space++;

	effect.effect = Common::String(space);

	_effects.push_back(effect);
	return true;
}

bool RoomConfigSprite::parseLoopPoint(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 2) {
		warning("RoomConfigSprite::parseLoopPoint(): Broken arguments");
		return false;
	}

	_loopStart = atoi(lArgs[0].c_str());
	_loopEnd   = atoi(lArgs[1].c_str());

	return true;
}

bool RoomConfigSprite::parsePackedIntLine(const Common::String &args, Common::Array<int32> &ints) {
	if (args.empty())
		return true;

	const char *str = args.c_str();
	char *squashedStr = new char[args.size() + 1];

	// Squashing multiple spaces into one space
	char *squash = squashedStr;
	while (*str) {
		if (*str == ' ') {
			*squash++ = ' ';
			while (*str == ' ')
				*str++;
		} else if (*str == '(') {
			*squash++ = *str++;
			while (*str == ' ')
				*str++;
		} else
			*squash++ = *str++;
	}
	*squash = '\0';

	str = squashedStr;

	while (*str) {
		const char *space       = strchr(str, ' ');
		const char *openParen   = strchr(str, '(');
		const char *closedParen = strchr(str, ')');

		if ((space && !openParen) || (space && (space < openParen)) || (!space && !openParen)) {

			ints.push_back(atoi(str) - 1);

			if (!space) {
				delete[] squashedStr;
				return true;
			}

			str = space + 1;

			continue;
		}

		if (!openParen || !closedParen) {
			warning("RoomConfigSprite::parsePackedIntLine(): No parenthesis: \"%s\"",
					args.c_str());
			delete[] squashedStr;
			return false;
		}

		int start;
		int count;
		int inc = 0;

		if (space && (openParen < space) && (space < closedParen)) {
			start = atoi(str) - 1;
			count = atoi(openParen + 1);
			inc   = atoi(space + 1);
		} else {
			start = atoi(str) - 1;
			count = atoi(openParen + 1);
		}

		for (int i = 0; i < count; i++, start += inc)
			ints.push_back(start);

		str = closedParen + 1;
		while (*str == ' ')
			str++;
	}

	delete[] squashedStr;
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

bool RoomConfigPalette::updateStatus() {
	assert(isLoaded());

	return false;
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

bool RoomConfigMirror::updateStatus() {
	assert(isLoaded());

	return false;
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
	Common::List<RoomConfig *>::iterator it = _configs.begin();
	while (it != _configs.end()) {
		if ((*it)->updateStatus()) {
			it = _configs.begin();
			continue;
		}
		++it;
	}
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

		RoomConfigSprite *music =
			new RoomConfigSprite(*_vm->_variables, *_vm->_resources, *_vm->_graphics, *_vm->_sound);
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
