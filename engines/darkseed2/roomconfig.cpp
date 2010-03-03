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

#include "common/frac.h"

#include "engines/darkseed2/roomconfig.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/room.h"
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/music.h"
#include "engines/darkseed2/mike.h"
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

template<>
void SaveLoad::sync<RoomConfigSprite::Frame>(Common::Serializer &serializer, RoomConfigSprite::Frame &var) {
	uint32 scale = (uint32) var.scale;

	sync(serializer, var.frame);
	sync(serializer, scale);
	sync(serializer, var.x);
	sync(serializer, var.y);

	var.scale = (frac_t) scale;
}

template<>
void SaveLoad::sync<RoomConfigSprite::Effect>(Common::Serializer &serializer, RoomConfigSprite::Effect &var) {
	sync(serializer, var.frameNum);
	sync(serializer, var.effect);
}


RoomConfig::RoomConfig(Variables &variables) : _variables(&variables) {
	_type = kTypeNone;

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

RoomConfig::Type RoomConfig::getType() const {
	return _type;
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

	bool met = _variables->evalCondition(_conditions);

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

bool RoomConfig::saveLoad(Common::Serializer &serializer, Resources &resources) {
	SaveLoad::sync(serializer, _loaded);
	SaveLoad::sync(serializer, _running);

	SaveLoad::sync(serializer, _conditionsState);

	SaveLoad::sync(serializer, _conditions);
	SaveLoad::sync(serializer, _changes);

	SaveLoad::syncTimestamp(serializer, _waitUntil);

	return true;
}

bool RoomConfig::loading(Resources &resources) {
	_conditionsCheckedLast = 0;

	_state        = false;
	_stateChanged = false;

	return true;
}


RoomConfigMusic::RoomConfigMusic(Variables &variables, Resources &resources, Music &music) :
		RoomConfig(variables) {

	_resources = &resources;
	_music     = &music;

	_type = kTypeMusic;
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

bool RoomConfigMusic::saveLoad(Common::Serializer &serializer, Resources &resources) {
	if (!RoomConfig::saveLoad(serializer, resources))
		return false;
	if (serializer.isLoading())
		if (!RoomConfig::loading(resources))
			return false;

	SaveLoad::sync(serializer, _midi);

	return true;
}

bool RoomConfigMusic::loading(Resources &resources) {
	return true;
}


RoomConfigSprite::RoomConfigSprite(Variables &variables, Resources &resources,
		Graphics &graphics, Sound &sound, Mike &mike) : RoomConfig(variables) {

	_resources = &resources;
	_graphics  = &graphics;
	_sound     = &sound;
	_mike      = &mike;

	_type = kTypeSprite;

	_curPos = 0;

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

		_frames[i].scale = FRAC_ONE;
	}

	_sequence.clear();
	_posX.clear();
	_posY.clear();

	_animation = _graphics->getRoom().loadAnimation(*_resources, _anim);
	if (!_animation)
		return false;

	for (uint i = 0; (i < _frames.size()) && (i < _scaleVal.size()) ; i++) {
		if (_scaleVal[i] > 0)
			_frames[i].scale = FRAC_ONE;
			// Jack sitting on the bench has scaling values too?!?
			// _frames[i].scale = _animation->calculateScaleVal(_frames[i].frame, _scaleVal[i]);
	}

	return true;
}

bool RoomConfigSprite::updateStatus() {
	assert(isLoaded());

	if (!isRunning()) {
		// Not running

		if (!conditionsMet()) {
			// But conditions are not met, abort
			_graphics->removeAnimation(_currentSprite);
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
		_graphics->removeAnimation(_currentSprite);
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
			_sound->playSound(*_resources, _effects[i].effect);
		}

	_graphics->removeAnimation(_currentSprite);

	if (_curPos < _frames.size()) {
		int32 x      = _frames[_curPos].x;
		int32 y      = _frames[_curPos].y;
		int32 frame  = _frames[_curPos].frame;
		frac_t scale = _frames[_curPos].scale;;

		if (_status[0] & 8) {
			// A Mike sprite, sync position and scaling
			_mike->getPosition(x, y);
			scale = _mike->getScale();
		}

		if (!_currentSprite.isUpToDate(frame, x, y, scale)) {
			// Update animation frame

			_graphics->removeAnimation(_currentSprite);

			_animation->setFrame(_frames[_curPos].frame);

			if ((x >= 0) && (y >= 0)) {
				// Has positional values, set the position and scaling accordingly

				if (!(_status[0] & 8)) {
					x /= _resources->getVersionFormats().getHotspotScale();
					y /= _resources->getVersionFormats().getHotspotScale();
				}

				_animation->moveFeetTo(x, y);
				scale = _mike->calculateScale(y);
			}

			_animation->setScale(scale);

			_graphics->addAnimation(*_animation, _currentSprite);
		}
	}

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
		// The frames' scaling values

		if (!parsePackedIntLine(args, _scaleVal))
			return false;

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
	Common::Array<int32> arg = DATFile::argGetInts(args, 6, 0);

	for (uint i = 0; i < 6; i++)
		_status[i] = arg[i];

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

bool RoomConfigSprite::saveLoad(Common::Serializer &serializer, Resources &resources) {
	if (!RoomConfig::saveLoad(serializer, resources))
		return false;
	if (serializer.isLoading())
		if (!RoomConfig::loading(resources))
			return false;

	SaveLoad::sync(serializer, _anim);

	SaveLoad::sync(serializer, _status[0]);
	SaveLoad::sync(serializer, _status[1]);
	SaveLoad::sync(serializer, _status[2]);
	SaveLoad::sync(serializer, _status[3]);
	SaveLoad::sync(serializer, _status[4]);
	SaveLoad::sync(serializer, _status[5]);

	SaveLoad::sync(serializer, _curPos);

	SaveLoad::sync(serializer, _frames);
	SaveLoad::sync(serializer, _effects);

	SaveLoad::sync(serializer, _loopCond);
	SaveLoad::sync(serializer, _loopStart);
	SaveLoad::sync(serializer, _loopEnd);

	SaveLoad::sync(serializer, _loadCond);
	SaveLoad::sync(serializer, _changeAt);
	SaveLoad::sync(serializer, _speech);
	SaveLoad::sync(serializer, _spriteIDX);
	return true;
}

bool RoomConfigSprite::loading(Resources &resources) {
	_animation = _graphics->getRoom().loadAnimation(*_resources, _anim);
	if (!_animation)
		return false;

	_currentSprite.clear();

	return true;
}


RoomConfigPalette::RoomConfigPalette(Variables &variables, Resources &resources, Graphics &graphics) :
		RoomConfig(variables) {

	_resources = &resources;
	_graphics  = &graphics;

	_type = kTypePalette;

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

bool RoomConfigPalette::saveLoad(Common::Serializer &serializer, Resources &resources) {
	if (!RoomConfig::saveLoad(serializer, resources))
		return false;
	if (serializer.isLoading())
		if (!RoomConfig::loading(resources))
			return false;

	SaveLoad::sync(serializer, _startIndex);
	SaveLoad::sync(serializer, _endIndex);

	return true;
}

bool RoomConfigPalette::loading(Resources &resources) {
	return true;
}


RoomConfigMirror::RoomConfigMirror(Variables &variables, Resources &resources, Graphics &graphics) :
		RoomConfig(variables) {

	_resources = &resources;
	_graphics  = &graphics;

	_type = kTypeMirror;

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

bool RoomConfigMirror::saveLoad(Common::Serializer &serializer, Resources &resources) {
	if (!RoomConfig::saveLoad(serializer, resources))
		return false;
	if (serializer.isLoading())
		if (!RoomConfig::loading(resources))
			return false;

	SaveLoad::sync(serializer, _area);
	SaveLoad::sync(serializer, _posX[0]);
	SaveLoad::sync(serializer, _posX[1]);
	SaveLoad::sync(serializer, _posX[2]);
	SaveLoad::sync(serializer, _posY[0]);
	SaveLoad::sync(serializer, _posY[1]);
	SaveLoad::sync(serializer, _posY[2]);
	SaveLoad::sync(serializer, _scale[0]);
	SaveLoad::sync(serializer, _scale[1]);
	SaveLoad::sync(serializer, _scale[2]);

	return true;
}

bool RoomConfigMirror::loading(Resources &resources) {
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

			if (!parseConfigs(dat, RoomConfig::kTypeMusic))
				return false;

		} else if (cmd->equalsIgnoreCase("SpriteStart")) {

			if (!parseConfigs(dat, RoomConfig::kTypeSprite))
				return false;

		} else if (cmd->equalsIgnoreCase("PaletteStart")) {

			if (!parseConfigs(dat, RoomConfig::kTypePalette))
				return false;

		} else if (cmd->equalsIgnoreCase("MirrorStart")) {

			if (!parseConfigs(dat, RoomConfig::kTypeMirror))
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

bool RoomConfigManager::parseConfigs(DATFile &dat, RoomConfig::Type type) {
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

		RoomConfig *conf = createRoomConfig(type);
		if (!conf->parse(dat)) {
			delete conf;
			return false;
		}

		_configs.push_back(conf);
	}

	return true;
}

RoomConfig *RoomConfigManager::createRoomConfig(RoomConfig::Type type) {
	switch (type) {
	case RoomConfig::kTypeMusic:
		return new RoomConfigMusic(*_vm->_variables, *_vm->_resources, *_vm->_music);

	case RoomConfig::kTypeSprite:
		return new RoomConfigSprite(*_vm->_variables, *_vm->_resources, *_vm->_graphics, *_vm->_sound, *_vm->_mike);

	case RoomConfig::kTypePalette:
		return new RoomConfigPalette(*_vm->_variables, *_vm->_resources, *_vm->_graphics);

	case RoomConfig::kTypeMirror:
		return new RoomConfigMirror(*_vm->_variables, *_vm->_resources, *_vm->_graphics);

	default:
		assert(false);
		return 0;
	}

	return 0;
}

bool RoomConfigManager::saveLoad(Common::Serializer &serializer, Resources &resources) {
	uint32 size = _configs.size();

	SaveLoad::sync(serializer, size);

	if (serializer.isSaving()) {
		for (Common::List<RoomConfig *>::iterator conf = _configs.begin(); conf != _configs.end(); ++conf) {
			byte type = (byte) (*conf)->getType();
			SaveLoad::sync(serializer, type);
			if (!(*conf)->doSaveLoad(serializer, resources))
				return false;
		}
	} else {
		clear();
		for (uint32 i = 0; i < size; i++) {
			byte type;

			SaveLoad::sync(serializer, type);

			RoomConfig *conf = createRoomConfig((RoomConfig::Type) type);

			if (!conf->doSaveLoad(serializer, resources))
				return false;

			_configs.push_back(conf);
		}
	}

	return true;
}

bool RoomConfigManager::loading(Resources &resources) {
	return true;
}

} // End of namespace DarkSeed2
