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

#ifndef DARKSEED2_ROOMCONFIG_H
#define DARKSEED2_ROOMCONFIG_H

#include "common/str.h"
#include "common/list.h"
#include "common/rect.h"
#include "common/frac.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/saveable.h"
#include "engines/darkseed2/graphics.h"

namespace DarkSeed2 {

class Resources;
class Variables;

class DATFile;
class Sound;
class Music;
class Mike;

/** A generic RoomConfig base class. */
class RoomConfig : public Saveable {
public:
	/** A specific config type. */
	enum Type {
		kTypeMusic   = 0,
		kTypeSprite  = 1,
		kTypePalette = 2,
		kTypeMirror  = 3,
		kTypeNone    = 4
	};

	RoomConfig(Variables &variables);
	virtual ~RoomConfig();

	/** Return the specific config type. */
	Type getType() const;

	/** Is the RoomConfig loaded and ready to run? */
	bool isLoaded()  const;
	/** Is the RoomConfig running? */
	bool isRunning() const;

	/** Has the conditions state changed? */
	bool stateChanged() const;

	/** Parse the RoomConfig out of a DAT file. */
	bool parse(DATFile &dat);

	virtual bool init() = 0;

	/** Check for status changes and run the RoomConfig if possible. */
	virtual bool updateStatus() = 0;

protected:
	Type _type; ///< The specific config type.

	/** Are the conditions to run the RoomConfig met? */
	bool conditionsMet();
	/** Are these conditions met? */
	bool conditionsMet(const Common::String &cond);

	/** Enter the running state. */
	void run();
	/** Leave the running state. */
	void stop();

	/** Apply the variable change sets. */
	bool applyChanges();

	/** Start a waiting of that amount of milliseconds. */
	void startWait(uint32 millis);
	/** Reset the waiting condition. */
	void resetWait();
	/** Has the RoomConfig waited enough? */
	bool waited() const;

	/** Parse a DAT line. */
	virtual bool parseLine(const Common::String &cmd, const Common::String &args) = 0;

	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	Variables *_variables;

	bool _loaded;  ///< Is the RoomConfig loaded and ready to run?
	bool _running; ///< Is the RoomConfig running?

	/** Wait until that time stamp. */
	uint32 _waitUntil;

	// For debug info purposes
	bool _state;        ///< Conditions state.
	bool _stateChanged; ///< Conditions state changed.

	// For caching purposes
	bool   _conditionsState;       ///< The last remembered conditions state.
	uint32 _conditionsCheckedLast; ///< When were the conditions checked last?

	/** The conditions required for this RoomConfig. */
	Common::List<Common::String> _conditions;
	/** The variables change set to be applied once the RoomConfig finished. */
	Common::List<Common::String> _changes;
};

/** A music RoomConfig. */
class RoomConfigMusic : public RoomConfig {
public:
	RoomConfigMusic(Variables &variables, Resources &resources, Music &music);
	~RoomConfigMusic();

	bool init();

	bool updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	Resources *_resources;
	Music     *_music;

	Common::String _midi;
};

/** A sprite RoomConfig. */
class RoomConfigSprite : public RoomConfig {
public:
	RoomConfigSprite(Variables &variables, Resources &resources,
			Graphics &graphics, Sound &sound, Mike &mike);
	~RoomConfigSprite();

	bool init();

	bool updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	friend class SaveLoad;

	/** A sprite animation frame. */
	struct Frame {
		int32  frame; ///< The frame number.
		frac_t scale; ///< The sprite's scaling value.
		int32  x;     ///< The sprite's x coordinate.
		int32  y;     ///< The sprite's y coordinate.
	};

	/** A sound effect. */
	struct Effect {
		uint32         frameNum; ///< Playing on that frame.
		Common::String effect;   ///< The sound effect's name.
	};

	Resources *_resources;
	Graphics  *_graphics;
	Sound     *_sound;
	Mike      *_mike;

	Graphics::SpriteRef _currentSprite; ///< The current sprite.

	Common::String _anim; ///< The animation name.

	Animation *_animation; ///< The animation.

	int32 _status[6]; ///< Status flags.

	uint32 _curPos; ///< The current position within the sequence.

	// Actions
	Common::Array<Frame>  _frames;  ///< All frames to be played.
	Common::Array<Effect> _effects; ///< All effects to be played.

	// Looping
	Common::String _loopCond;  ///< Looping while this condition is true.
	int32          _loopStart; ///< Looping sequence starts here.
	int32          _loopEnd;   ///< Looping sequence ends here.

	// Unknown/Unhandled
	Common::String _loadCond;  ///< Loading condition?
	Common::String _changeAt;  ///< Advance at specific condition?
	Common::String _speech;    ///< Speech to be played?
	int32          _spriteIDX; ///< Sprite index?

	// Loading helpers/temporaries
	Common::String       _sequenceString; ///< The sequence string.
	Common::Array<int32> _sequence;       ///< The sequence numbers.
	Common::Array<int32> _posX;           ///< The X positions.
	Common::Array<int32> _posY;           ///< The Y positions.
	Common::Array<int32> _scaleVal;       ///< The scaling values.

	// Parsing helpers
	bool parseStatus(const Common::String &args);
	bool parseSequence(const Common::String &args);
	bool parseEffect(const Common::String &args);
	bool parseLoopPoint(const Common::String &args);

	static bool parsePackedIntLine(const Common::String &args, Common::Array<int32> &ints);
};

/** A palette RoomConfig. */
class RoomConfigPalette : public RoomConfig {
public:
	RoomConfigPalette(Variables &variables, Resources &resources, Graphics &graphics);
	~RoomConfigPalette();

	bool init();

	bool updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	Resources *_resources;
	Graphics  *_graphics;

	byte _startIndex;
	byte _endIndex;
};

/** A mirror RoomConfig. */
class RoomConfigMirror : public RoomConfig {
public:
	RoomConfigMirror(Variables &variables, Resources &resources, Graphics &graphics);
	~RoomConfigMirror();

	bool init();

	bool updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	Resources *_resources;
	Graphics  *_graphics;

	Common::Rect _area;
	int32 _posX[3];
	int32 _posY[3];
	int32 _scale[3];

	bool parseArea(const Common::String &args);
	bool parsePosX(const Common::String &args);
	bool parsePosY(const Common::String &args);
	bool parseScale(const Common::String &args);
};

class RoomConfigManager : public Saveable {
public:
	RoomConfigManager(DarkSeed2Engine &vm);
	~RoomConfigManager();

	void initRoom();
	void deinitRoom();

	void updateStatus();

	bool parseConfig(DATFile &dat);

	RoomConfig *createRoomConfig(RoomConfig::Type type);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	DarkSeed2Engine *_vm;

	Common::List<RoomConfig *> _configs;

	void clear();

	bool parseConfigs(DATFile &dat, RoomConfig::Type type);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_ROOMCONFIG_H
