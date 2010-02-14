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

#ifndef DARKSEED2_EVENTS_H
#define DARKSEED2_EVENTS_H

#include "common/list.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/saveable.h"
#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/objects.h"
#include "engines/darkseed2/graphicalobject.h"
#include "engines/darkseed2/inventorybox.h"

namespace DarkSeed2 {

class Room;
class ScriptChunk;

/** Cursor modes. */
enum CursorMode {
	kCursorModeWalk = 0, ///< Walk.
	kCursorModeUse  = 1, ///< Use.
	kCursorModeLook = 2, ///< Look.
	kCursorModeNone = 3  ///< None.
};

class Events : public Saveable {
public:
	Events(DarkSeed2Engine &vm);
	~Events();

	/** Run the game's normal course. */
	bool run();

	/** Did we come from that room? */
	bool cameFrom(uint32 room) const;

	/** Register a room transition. */
	void setNextRoom(uint32 room);

	/** Set the loading requested state. */
	void setLoading(bool load);

	bool _loading; ///< Loading a saved game requested?

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	/** A global state of the game. */
	enum State {
		kStateStarted = 0, ///< Game just started.
		kStateIntro1  = 1, ///< Game showing the first part of the intro.
		kStateIntro2  = 2, ///< Game showing the second part of the intro.
		kStateIntro3  = 3, ///< Game showing the third part of the intro.
		kStateIntro4  = 4, ///< Game showing the third part of the intro.
		kStateIntro5  = 5, ///< Game showing the fourth part of the intro.
		kStateRunning = 6  ///< Game running normally.
	};

	/** A mode's cursor. */
	struct ModeCursors {
		const Cursors::Cursor *inactive; ///< Not in hotspot.
		const Cursors::Cursor *active;   ///< In hotspot.
	};

	DarkSeed2Engine *_vm;

	State _state; ///< The current global state of the game.

	// Cursors
	bool         _canSwitchCursors; ///< Is cursor mode switching allowed?
	bool         _cursorActive;     ///< Currently in a hotspot?
	CursorMode   _cursorMode;       ///< The current cursor mode.
	ModeCursors *_cursors;          ///< The cursors.

	SpriteObject _titleSprites[4]; ///< Title elements.

	// Room changing
	bool           _changeRoom; ///< Do we need to change the room?
	Common::String _lastRoom;   ///< The room we came from.
	Common::String _nextRoom;   ///< The room to change to.

	Object *_lastObject; ///< The object the last action was done upon.

	// Item usage
	bool                   _itemMode;   ///< Currently in item mode?
	ObjectVerb             _itemVerb;   ///< The verb that triggered the object mode.
	InventoryBox::ItemRef  _itemRef;    ///< A reference to the active item.
	const Cursors::Cursor *_itemCursor; ///< The item's cursor.

	// Used for loading
	Common::String _lastObjectName;
	Common::String _itemName;
	Common::String _itemCursorName;

	/** Set up the game's hardcoded intro sequences. */
	bool introSequence();

	/** Start the game's main loop. */
	void mainLoop(bool finishScripts = false);

	/** Handle user input. */
	void handleInput();

	/** Handle user input while a movie is playing. */
	void handleMovieInput();

	/** Handle mouse move events. */
	void mouseMoved(int32 x, int32 y);
	/** Handle mouse click left events. */
	void mouseClickedLeft(int32 x, int32 y);
	/** Handle mouse click right events. */
	void mouseClickedRight(int32 x, int32 y);

	/** Look if we're in a hotspot. */
	void checkHotspot(int32 x, int32 y);
	/** Look if we're in an inventory hotspot. */
	void checkInventoryHotspot(int32 x, int32 y);

	/** Cycle the cursor mode. */
	void cycleCursorMode();
	/** Set the current cursor. */
	void setCursor();
	/** Set the specific cursor. */
	void setCursor(CursorMode cursor, bool active);
	/** Set the specific cursor. */
	void setCursor(const Cursors::Cursor &cursor);

	/** The coordinates are in which title sprite area? */
	int checkTitleSprites(int32 x, int32 y) const;

	/** End the intro. */
	void leaveIntro();

	/** Execute the current room's entry logic. */
	bool roomEnter();
	/** Revert any changes dependant on the current room. */
	void roomLeave();
	/** Execute the current room's autostart logic. */
	bool findAutoStart(Room &room);
	/** Go to another room. */
	bool roomGo(const Common::String &room);

	/** Executre an object's script for the given verb. */
	void doObjectVerb(Object &object, ObjectVerb verb);

	static ObjectVerb cursorModeToObjectVerb(CursorMode cursorMode);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_EVENTS_H
