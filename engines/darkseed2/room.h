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

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/objects.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class Graphics;
class RoomConfigManager;

class DATFile;
class ScriptChunk;
class Sprite;
class Animation;

class Room : public ObjectContainer {
public:
	Room(Variables &variables, Graphics &graphics);
	~Room();

	void registerConfigManager(RoomConfigManager &configManager);

	/** Get the room's name. */
	const Common::String &getName() const;
	/** Get the room's background. */
	const Sprite &getBackground() const;

	/** Clip the rectangle to the room area. */
	void clipToRoom(Common::Rect &rect) const;

	/** Get the room's entry logic scripts. */
	Common::List<ScriptChunk *> &getEntryScripts();

	/** Get the specified animation. */
	Animation *getAnimation(const Common::String &animation);

	/** Empty the room. */
	void clear();

	/** Load an animation. */
	bool loadAnimation(Resources &resources, const Common::String &base);

	/** Parse a room. */
	bool parse(const Resources &resources, const Common::String &base);

private:
	typedef Common::HashMap<Common::String, Animation *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> AnimationMap;

	Variables *_variables;
	Graphics  *_graphics;

	RoomConfigManager *_confMan;

	/** Was everything set up so that the room can be used? */
	bool _ready;

	Common::String _name;           ///< The room's name.
	Common::String _backgroundFile; ///< The room's background file.
	Common::String _walkMapFile;    ///< The room's walk map file.

	Sprite *_background; ///< The room's background.
	Sprite *_walkMap;    ///< The room's walk map.

	Common::Rect _area; ///< The room's area;

	uint32 _walkMapArg1, _walkMapArg2;

	int32 _scaleFactor1, _scaleFactor2, _scaleFactor3;

	/** Room's entry logic. */
	Common::List<ScriptChunk *> _entryScripts;

	/** All animations. */
	AnimationMap _animations;

	// Parsing helpers
	bool setBackground(const Common::String &args);
	bool setWalkMap(const Common::String &args);
	bool setScaleFactor(const Common::String &args);
	bool setDimensions(const Common::String &args);
	bool addEntryScript(DATFile &room);
	bool parseEntryScripts(DATFile &room);

	bool parse(const Resources &resources, DATFile &room, DATFile &objects);
	bool parse(const Resources &resources, const Common::String &room, const Common::String &objects);

	/** Set up the room after parsing. */
	bool setup(const Resources &resources);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_ROOM_H
