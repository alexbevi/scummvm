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

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/graphics.h"

namespace DarkSeed2 {

class Resources;
class Variables;

class DATFile;
class Music;

/** A generic RoomConfig base class. */
class RoomConfig {
public:
	RoomConfig(Variables &variables);
	virtual ~RoomConfig();

	/** Is the RoomConfig loaded and ready to run? */
	bool isLoaded()  const;
	/** Is the RoomConfig running? */
	bool isRunning() const;

	/** Parse the RoomConfig out of a DAT file. */
	bool parse(DATFile &dat);

	virtual bool init() = 0;

	/** Check for status changes and run the RoomConfig if possible. */
	virtual void updateStatus() = 0;

protected:
	/** Are the conditions to run the RoomConfig met? */
	bool conditionsMet() const;

	/** Enter the running state. */
	void run();
	/** Leave the running state. */
	void stop();

	/** Apply the variable change sets. */
	void applyChanges();

	/** Start a waiting of that amount of milliseconds. */
	void startWait(uint32 millis);
	/** Reset the waiting condition. */
	void resetWait();
	/** Has the RoomConfig waited enough? */
	bool waited() const;

	/** Parse a DAT line. */
	virtual bool parseLine(const Common::String &cmd, const Common::String &args) = 0;

private:
	Variables *_variables;

	bool _loaded;  ///< Is the RoomConfig loaded and ready to run?
	bool _running; ///< Is the RoomConfig running?

	/** Wait until that time stamp. */
	uint32 _waitUntil;

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

	void updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

private:
	Resources *_resources;
	Music     *_music;

	Common::String _midi;
};

/** A sprite RoomConfig. */
class RoomConfigSprite : public RoomConfig {
public:
	RoomConfigSprite(Variables &variables, Resources &resources, Graphics &graphics);
	~RoomConfigSprite();

	bool init();

	void updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

private:
	Resources *_resources;
	Graphics  *_graphics;

	Graphics::SpriteRef _currentSprite;

	/** The animation name. */
	Common::String _anim;
	uint16 _status[6];
	/** The frame sequence to cycle through. */
	Common::Array<uint8> _sequence;
	uint8 _spriteIDX;
	Common::String _effect;
	Common::String _scaleVal;
	Common::String _posX;
	Common::String _posY;
	Common::String _loopCond;
	Common::String _loopPoint;
	Common::String _loadCond;
	Common::String _changeAt;
	Common::String _speech;

	/** The current position within the sequence. */
	uint _curPos;

	// Parsing helpers
	bool parseStatus(const Common::String &args);
	bool parseSequence(const Common::String &args);
};

/** A palette RoomConfig. */
class RoomConfigPalette : public RoomConfig {
public:
	RoomConfigPalette(Variables &variables, Resources &resources, Graphics &graphics);
	~RoomConfigPalette();

	bool init();

	void updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

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

	void updateStatus();

	bool parseLine(const Common::String &cmd, const Common::String &args);

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

class RoomConfigManager {
public:
	RoomConfigManager(DarkSeed2Engine &vm);
	~RoomConfigManager();

	void initRoom();
	void deinitRoom();

	void updateStatus();

	bool parseConfig(DATFile &dat);

private:
	DarkSeed2Engine *_vm;

	Common::List<RoomConfig *> _configs;

	void clear();

	bool parseMusicConfigs(DATFile &dat);
	bool parseSpriteConfigs(DATFile &dat);
	bool parsePaletteConfigs(DATFile &dat);
	bool parseMirrorConfigs(DATFile &dat);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_ROOMCONFIG_H
