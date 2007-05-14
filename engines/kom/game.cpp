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

#include "common/stdafx.h"
#include "common/fs.h"
#include "common/str.h"

#include "kom/kom.h"
#include "kom/game.h"
#include "kom/panel.h"
#include "kom/database.h"
#include "kom/flicplayer.h"

namespace Kom {

using Common::String;

Game::Game(KomEngine *vm, OSystem *system) : _system(system), _vm(vm) {

	// FIXME: Temporary
    _settings.selectedChar = _settings.selectedQuest = 0;
}

Game::~Game() {
}

void Game::enterLocation(uint16 locId) {
	char filename[50];

	_vm->panel()->showLoading(true);

	// Unload last room elements
	for (uint i = 0; i < _roomObjects.size(); i++) {
		_vm->actorMan()->unload(_roomObjects[i].actorId);
	}
	_roomObjects.clear();

	for (uint i = 0; i < _roomDoors.size(); i++) {
		_vm->actorMan()->unload(_roomDoors[i].actorId);
	}
	_roomDoors.clear();

	Location *loc = _vm->database()->location(locId);
	String locName(loc->name);
	locName.toLowercase();
	FilesystemNode locNode(_vm->dataDir()->getChild("kom").getChild("locs").getChild(String(locName.c_str(), 2)).getChild(locName));

	// Load room background and mask

	sprintf(filename, "%s%db.flc", locName.c_str(), loc->xtend + _settings.dayMode);
	_vm->screen()->loadBackground(locNode.getChild(filename));

	filename[strlen(filename) - 5] = 'm';
	FlicPlayer mask(locNode.getChild(filename));
	mask.decodeFrame();
	_vm->screen()->setMask(mask.getOffscreen());

	Database *db = _vm->database();

	// Load room objects
	Common::List<int> objList = loc->objects;
	for (Common::List<int>::iterator objId = objList.begin(); objId != objList.end(); ++objId) {
		Object *obj = db->object(*objId);
		if (obj->isSprite) {
			sprintf(filename, "%s%d", obj->name, loc->xtend);
			RoomObject roomObj;
			roomObj.actorId = _vm->actorMan()->load(locNode, String(filename));
			Actor *act = _vm->actorMan()->get(roomObj.actorId);
			act->defineScope(0, 0, act->getFramesNum() - 1, 0);
			act->setScope(0, 3);
			act->setPos(0, SCREEN_H - 1);

			_roomObjects.push_back(roomObj);

			Box *box = db->getBox(locId, obj->box);
			act->setMaskDepth(box->priority);

			// TODO:
			// * store actor in screenObjects
			// * load doors
		}
	}

	// Load room doors
	const Exit *exits = db->getExits(locId);
	for (int i = 0; i < 6; ++i) {
		// FIXME: room 45 has one NULL exit. what's it for?
		if (exits[i].exit > 0) {
			String exitName(_vm->database()->location(exits[i].exitLoc)->name);
			exitName.toLowercase();

			// TODO: add day/night modifier to xtend
			sprintf(filename, "%s%dd", exitName.c_str(), loc->xtend);

			// The exit can have no door
			if (locNode.getChild(filename + String(".act")).isValid()) {
				RoomDoor roomDoor;
				roomDoor.actorId = _vm->actorMan()->load(locNode, String(filename));
				Actor *act = _vm->actorMan()->get(roomDoor.actorId);

				// Temporary: have fun with the door
				act->defineScope(0, 0, act->getFramesNum() - 1, 0);
				act->setScope(0, 1);
				act->setPos(0, SCREEN_H - 1);
				act->setEffect(4);

				_roomDoors.push_back(roomDoor);
			}
		}
	}

	_vm->panel()->setLocationDesc(loc->desc);
	_vm->panel()->showLoading(false);
}

void Game::doStat(const Command *cmd) {
	bool keepProcessing = true;

	debug(1, "Trying to execute Command %d - value %hd", cmd->cmd, cmd->value);

	for (Common::List<OpCode>::const_iterator j = cmd->opcodes.begin();
			j != cmd->opcodes.end() && keepProcessing; ++j) {

		switch (j->opcode) {
		case 327:
			_vm->database()->setVar(j->arg2, j->arg3);
			break;
		case 328:
			_vm->database()->setVar(j->arg2, _vm->database()->getVar(j->arg3));
			break;
		case 331:
			_vm->database()->setVar(j->arg2, 0);
			break;
		case 334:
			_vm->database()->setVar(j->arg2, _rnd.getRandomNumber(j->arg3));
			break;
		case 337:
			keepProcessing = _vm->database()->getVar(j->arg2) == 0;
			break;
		case 338:
			keepProcessing = _vm->database()->getVar(j->arg2) != 0;
			break;
		case 340:
			keepProcessing = _vm->database()->getVar(j->arg2) == j->arg3;
			break;
		case 345:
			keepProcessing = _vm->database()->getVar(j->arg2) != j->arg3;
			break;
		case 346:
			keepProcessing = _vm->database()->getVar(j->arg2) !=
				_vm->database()->getVar(j->arg3);
			break;
		case 350:
			keepProcessing = _vm->database()->getVar(j->arg2) > j->arg3;
			break;
		case 353:
			keepProcessing = _vm->database()->getVar(j->arg2) >= j->arg3;
			break;
		case 356:
			keepProcessing = _vm->database()->getVar(j->arg2) < j->arg3;
			break;
		case 359:
			keepProcessing = _vm->database()->getVar(j->arg2) <= j->arg3;
			break;
		case 373:
			_vm->database()->setVar(j->arg2, _vm->database()->getVar(j->arg2) + 1);
			break;
		case 374:
			_vm->database()->setVar(j->arg2, _vm->database()->getVar(j->arg2) - 1);
			break;
		case 376:
			_vm->database()->setVar(j->arg2,
				_vm->database()->getVar(j->arg2) + _vm->database()->getVar(j->arg3));
			break;
		case 377:
			_vm->database()->setVar(j->arg2, _vm->database()->getVar(j->arg2) + j->arg3);
			break;
		case 379:
			_vm->database()->setVar(j->arg2,
				_vm->database()->getVar(j->arg2) - _vm->database()->getVar(j->arg3));
			break;
		case 380:
			_vm->database()->setVar(j->arg2, _vm->database()->getVar(j->arg2) - j->arg3);
			break;
		case 381:
			keepProcessing = _vm->database()->getChar(0)->locationId == j->arg2;
			break;
		case 382:
			keepProcessing = _vm->database()->getChar(0)->locationId != j->arg2;
			break;
		case 426:
			keepProcessing = _vm->database()->getChar(j->arg2)->locationId ==
			    _vm->database()->getChar(j->arg3)->locationId;
			break;
		case 445:
			_vm->database()->getObj(j->arg2)->isVisible = 1;
			break;
		case 448:
			_vm->database()->getObj(j->arg2)->isVisible = 0;
			break;
		case 449:
			keepProcessing = _settings.dayMode == 0;
			break;
		case 450:
			keepProcessing = _settings.dayMode == 1;
			break;
		case 453:
			_settings.dayMode = 0;
			warning("TODO: Quick hack to change day");
			break;
		case 454:
			_settings.dayMode = 1;
			warning("TODO: Quick hack to change day");
			break;
		case 474:
			if (strcmp(j->arg1, "REFRESH") == 0) {
				warning("TODO: Unhandled external action: REFRESH");
			} else {
				_vm->database()->setVar(j->arg2, doExternalAction(j->arg1));
			}
			break;
		case 475:
			keepProcessing = false;
			break;
		case 485:
			_settings.fightEnabled = true;
			break;
		case 486:
			_settings.fightEnabled = false;
			break;
		default:
			warning("Unhandled OpCode: %d - (%s, %d, %d, %d, %d, %d)", j->opcode,
				j->arg1, j->arg2, j->arg3, j->arg4, j->arg5, j->arg6);
		}
	}

}

int16 Game::doExternalAction(const char *action) {
	if (strcmp(action, "getquest") == 0) {
		return _settings.selectedQuest;
	} else {
		warning("Unknown external action: %s", action);
		return 0;
	}
}

} // End of namespace Kom
