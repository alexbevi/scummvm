/* ScummVM - Scumm Interpreter
 * Copyright (C) 2005-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef KOM_GAME_H
#define KOM_GAME_H

#include "common/scummsys.h"
#include "common/array.h"

namespace Kom {

struct RoomObject {
	int actorId;
};

struct RoomDoor {
	int actorId;
};

struct Settings {
	uint8 selectedChar; // 0 - thidney. 1 - shahron
	uint8 selectedQuest;
}

class Game {
public:
	Game(KomEngine *vm, OSystem *system);
	~Game();

	void enterLocation(uint16 locId);

	Settings* settings() const { return &_settings; }

private:

	OSystem *_system;
	KomEngine *_vm;

	Common::Array<RoomObject> _roomObjects;
	Common::Array<RoomDoor> _roomDoors;

    // Settings
    Settings _settings;
};

} // End of namespace Kom

#endif
