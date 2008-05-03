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

#ifndef KOM_GAME_H
#define KOM_GAME_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/util.h"

#include "kom/database.h"

namespace Kom {

struct RoomObject {
	uint16 objectId;
	int16 actorId;
	int16 z2;
	int16 priority;
};

struct RoomDoor {
	RoomDoor() : frame(0), boxHit(0), state(0) {}
	int actorId;
	uint8 frame;
	uint8 boxHit;
	int8 boxOpenFast; // Box closest to exit
	int8 boxOpenSlow; // Box farther from exit
	int8 state;
};

struct Settings {
	Settings() : gameCycles(6000), dayMode(1) {}
	uint16 mouseState;
	bool mouseOverExit;
	uint16 mouseX;
	uint16 mouseY;
	int16 mouseBox;
	int16 overType;
	int16 oldOverType;
	int16 overNum;
	int16 oldOverNum;
	int16 collideBox;
	uint16 collideBoxX;
	uint16 collideBoxY;
	int16 collideBoxZ;
	int16 collideObj;
	uint16 collideObjX;
	uint16 collideObjY;
	int32 collideObjZ;
	int16 collideChar;
	uint16 collideCharX;
	uint16 collideCharY;
	int32 collideCharZ;
	int16 objectNum;
	int16 object2Num;
	uint8 dayMode; // 0 - day. 1 - night. 2 - dawn. 3 - dusk
	uint8 currLocation;
	uint16 gameCycles;
	bool fightEnabled;
};

typedef enum {
	CMD_SPRITE_SCENE = 100,
	CMD_NOTHING      = 101,
	CMD_THING1       = 102,
	CMD_LOOK         = 105,
	CMD_THING2       = 106,
	CMD_THING3       = 107
} CommandType;

/** Player settings */
struct Player {
	Player() : isNight(0), sleepTimer(0) {}
	CommandType command;
	int16 collideType;
	int16 collideNum;
	int16 commandState;
	uint8 selectedChar; // 0 - thidney. 1 - shahron
	uint8 selectedQuest;
	uint8 isNight;
	uint16 sleepTimer;
	bool spriteCutMoving;
	int16 spriteCutX;
	int16 spriteCutY;
	int spriteCutTab;
	int16 spriteCutPos;
	int16 spriteCutNum;
};

class Game {
public:
	Game(KomEngine *vm, OSystem *system);
	~Game();

	void enterLocation(uint16 locId);
	void hitExit(uint16 charId, bool something);
	void processTime();
	bool doStat(const Command *cmd);
	void loopMove();
	void loopCollide();
	void loopSpriteCut();
	void loopTimeouts();
	void loopInterfaceCollide();

	Settings* settings() { return &_settings; }
	Player* player() { return &_player; }

	void setDay();
	void setNight();

	void doActionMoveChar(uint16 charId, int16 loc, int16 box);
	void doActionSpriteScene(const char *name, int charId, int loc, int box);

	Common::Array<RoomObject> *getObjects() { return &_roomObjects; }
	Common::Array<RoomDoor> *getDoors() { return &_roomDoors; }

private:

	OSystem *_system;
	KomEngine *_vm;

	Common::Array<RoomObject> _roomObjects;
	Common::Array<RoomDoor> _roomDoors;

	Common::RandomSource _rnd;

    // Settings
    Settings _settings;
    Player _player;

	void processChars();
	void processChar(int proc);
	void changeMode(int value, int mode);
	int16 doExternalAction(const char *action);

	void doActionDusk();
	void doActionDawn();

	void moveChar(uint16 charId, bool param);
	void moveCharOther(uint16 charId);
};

} // End of namespace Kom

#endif
