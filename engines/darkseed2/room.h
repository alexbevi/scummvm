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

#include "common/frac.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/saveable.h"
#include "engines/darkseed2/objects.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class ScriptRegister;
class Graphics;
class RoomConfigManager;

class DATFile;
class ScriptChunk;
class Sprite;
class Animation;

class Room : public ObjectContainer, public Saveable {
public:
	Room(Variables &variables, ScriptRegister &scriptRegister, Graphics &graphics);
	~Room();

	void registerConfigManager(RoomConfigManager &configManager);

	/** Get the room's name. */
	const Common::String &getName() const;
	/** Get the room's background. */
	const Sprite &getBackground() const;
	/** Get the roon's walk map. */
	const Sprite &getWalkMap() const;
	/** Get the area the walk map applies to. */
	Common::Rect getWalkMapArea() const;

	int32 getWalkMapYTop() const;
	int32 getWalkMapYResolution() const;

	const int32 *getScaleFactors() const;

	/** Clip the rectangle to the room area. */
	void clipToRoom(Common::Rect &rect) const;

	/** Return the room's clipping rectangle. */
	const Common::Rect &getClipRect() const;

	/** Get the room's entry logic scripts. */
	Common::List<ScriptChunk *> &getEntryScripts();

	/** Get the specified animation. */
	Animation *getAnimation(const Common::String &animation);

	/** Set the scaling value of that animation. */
	void scaleAnimation(const Common::String &animation, frac_t scale);

	/** Empty the room. */
	void clear();

	/** Load an animation. */
	Animation *loadAnimation(Resources &resources, const Common::String &base);

	/** Parse a room. */
	bool parse(Resources &resources, const Common::String &base);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	typedef Common::HashMap<Common::String, Animation *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> AnimationMap;

	Variables      *_variables;
	ScriptRegister *_scriptRegister;
	Graphics       *_graphics;

	RoomConfigManager *_confMan;

	/** Was everything set up so that the room can be used? */
	bool _ready;

	Common::String _name;           ///< The room's name.
	Common::String _roomFile;       ///< The room's room file.
	Common::String _objsFile;       ///< The room's objects file.
	Common::String _backgroundFile; ///< The room's background file.
	Common::String _walkMapFile;    ///< The room's walk map file.

	int32 _walkMapYTop;
	int32 _walkMapYResolution;

	Sprite *_background; ///< The room's background.
	Sprite *_walkMap;    ///< The room's walk map.

	Common::Rect _area; ///< The room's area;

	/** The scaling factors. */
	int32 _scaleFactors[3];

	/** Room's entry logic. */
	Common::List<ScriptChunk *> _entryScripts;

	/** All animations. */
	AnimationMap _animations;

	// For saving/loading
	Common::List<uint32> _entryScriptLines;

	// Parsing helpers
	bool setBackground(const Common::String &args);
	bool setWalkMap(const Common::String &args);
	bool setScaleFactor(const Common::String &args);
	bool setDimensions(const Common::String &args);
	bool addEntryScript(DATFile &room);
	bool parseEntryScripts(DATFile &room);

	bool parse(Resources &resources, DATFile &room, DATFile &objects);
	bool parse(Resources &resources, const Common::String &room, const Common::String &objects);

	/** Load the sprites. */
	bool loadSprites(Resources &resources);

	/** Set up the room after parsing. */
	bool setup(Resources &resources);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_ROOM_H
