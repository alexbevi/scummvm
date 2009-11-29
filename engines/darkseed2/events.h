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

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/objects.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

class Room;

/** Cursor modes. */
enum CursorMode {
	kCursorModeWalk = 0, ///< Walk.
	kCursorModeUse,      ///< Use.
	kCursorModeLook,     ///< Look.
	kCursorModeNone      ///< None.
};

class Events {
public:
	Events(DarkSeed2Engine &vm);
	~Events();

	/** Set up the game's hardcoded intro sequences. */
	bool setupIntroSequence();

	/** Start the game's main loop. */
	void mainLoop(bool finishScripts = false);

	/** Register a room transition. */
	void setNextRoom(uint32 room);

private:
	/** A mode's cursor. */
	struct ModeCursors {
		const Cursors::Cursor *inactive; ///< Not in hotspot.
		const Cursors::Cursor *active;   ///< In hotspot.
	};

	DarkSeed2Engine *_vm;

	bool _inIntro; ///< Currently in the intro?

	bool _canSwitchCursors; ///< Is cursor mode switching allowed?

	ModeCursors *_cursors;  ///< The cursors.
	CursorMode _cursorMode; ///< The current cursor mode.
	bool _cursorActive;     ///< Currently in a hotspot?

	SpriteObject _titleSprites[5]; ///< Title elements.

	Common::String _nextRoom;
	bool _changeRoom;

	/** Handle user input. */
	void handleInput();

	/** Handle mouse move events. */
	void mouseMoved(uint32 x, uint32 y);
	/** Handle mouse click left events. */
	void mouseClickedLeft(uint32 x, uint32 y);
	/** Handle mouse click right events. */
	void mouseClickedRight(uint32 x, uint32 y);

	/** Look if we're in a hotspot. */
	void checkHotspot(uint32 x, uint32 y);

	/** Cycle the cursor mode. */
	void cycleCursorMode();
	/** Set the current cursor. */
	void setCursor();
	/** Set the specific cursor. */
	void setCursor(CursorMode cursor, bool active);
	/** Set the specific cursor. */
	void setCursor(const Cursors::Cursor &cursor);

	/** The coordinates are in which title sprite area? */
	int checkTitleSprites(uint32 x, uint32 y) const;

	/** End the intro. */
	void leaveIntro();

	/** Execute the current room's entry logic. */
	bool roomEnter();
	/** Revert any changes dependant on the current room. */
	void roomLeave();
	/** Execute the current room's autostart logic. */
	bool executeAutoStart(Room &room);
	/** Go to another room. */
	bool roomGo(const Common::String &room);

	/** Executre an object's script for the given verb. */
	void doObjectVerb(Object &object, ObjectVerb verb);

	static ObjectVerb cursorModeToObjectVerb(CursorMode cursorMode);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_EVENTS_H
