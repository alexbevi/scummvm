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

#include <stdlib.h>

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

	// FIXME - hack
	_vm->database()->getChar(0)->_lastLocation = locId;

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

	if (locId == 0) {
		_vm->panel()->showLoading(false);
		return;
	}

	Location *loc = _vm->database()->getLoc(locId);
	String locName(loc->name);
	locName.toLowercase();
	FilesystemNode locNode(_vm->dataDir()->getChild("kom").getChild("locs").getChild(String(locName.c_str(), 2)).getChild(locName));

	// Load room background and mask

	sprintf(filename, "%s%db.flc", locName.c_str(), loc->xtend + _player.isNight);
	_vm->screen()->loadBackground(locNode.getChild(filename));

	filename[strlen(filename) - 6] = '0';
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
			sprintf(filename, "%s%d", obj->name, _player.isNight);
			RoomObject roomObj;
			roomObj.objectId = *objId;
			roomObj.actorId = _vm->actorMan()->load(locNode, String(filename));
			roomObj.priority = db->getBox(locId, obj->box)->priority;
			Actor *act = _vm->actorMan()->get(roomObj.actorId);
			act->defineScope(0, 0, act->getFramesNum() - 1, 0);
			act->setScope(0, 3);
			act->setPos(0, SCREEN_H - 1);

			_roomObjects.push_back(roomObj);

			// TODO - move this to processGraphics?
			act->setMaskDepth(roomObj.priority, 1); // TODO - check dummy depth value

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
			String exitName(_vm->database()->getLoc(exits[i].exitLoc)->name);
			exitName.toLowercase();

			sprintf(filename, "%s%dd", exitName.c_str(), loc->xtend + _player.isNight);

			// The exit can have no door
			if (locNode.getChild(filename + String(".act")).exists()) {
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

void Game::processTime() {
	if (_settings.dayMode == 0) {
		if (_vm->database()->getChar(0)->_isBusy && _settings.gameCycles >= 6000)
			_settings.gameCycles = 5990;

		if (_vm->database()->getLoc(_settings.currLocation)->allowedTime == 2) {
			_settings.dayMode = 3;
			doActionDusk();
			processChars();
			_settings.dayMode = 1;
			_settings.gameCycles = 0;
		}

		if (_settings.gameCycles < 6000) {

			if (!_vm->database()->getChar(0)->_isBusy)
				(_settings.gameCycles)++;

		} else if (_vm->database()->getLoc(_settings.currLocation)->allowedTime == 0) {
			_settings.dayMode = 3;
			doActionDusk();
			processChars();
			_settings.dayMode = 1;
			_settings.gameCycles = 0;
		}

	} else if (_settings.dayMode == 1) {

		if (_vm->database()->getChar(0)->_isBusy && _settings.gameCycles >= 3600)
			_settings.gameCycles = 3590;

		if (_vm->database()->getLoc(_settings.currLocation)->allowedTime == 1) {
			_settings.dayMode = 2;
			doActionDawn();
			processChars();
			_settings.dayMode = 0;
			_settings.gameCycles = 0;
		}

		if (_settings.gameCycles < 3600) {

			if (!_vm->database()->getChar(0)->_isBusy)
				(_settings.gameCycles)++;

		} else if (_vm->database()->getLoc(_settings.currLocation)->allowedTime == 0) {
			_settings.dayMode = 2;
			doActionDawn();
			processChars();
			_settings.dayMode = 0;
			_settings.gameCycles = 0;

			// TODO - increase hit points and spell points
		}
	}

	processChars();
}

void Game::processChars() {
	for (int i = 0; i < _vm->database()->charactersNum(); ++i) {
		Character *ch = _vm->database()->getChar(i);
		if (ch->_isAlive && ch->_proc != -1 && ch->_mode < 6) {
			switch (ch->_mode) {
			case 0:
			case 3:
				processChar(ch->_proc);
				break;
			case 1:
				warning("TODO: processChars 1");
				break;
			case 2:
				warning("TODO: processChars 2");
				break;
			case 4:
			case 5:
				warning("TODO: processChars 4/5");
				break;
			}
		}
	}
}

void Game::processChar(int proc) {
	bool stop = false;
	Process *p = _vm->database()->getProc(proc);

	for (Common::List<Command>::iterator i = p->commands.begin();
			i != p->commands.end() && !stop; ++i) {
		if (i->cmd == 313) { // Character
			debug(1, "Processing char in %s", p->name);
			stop = doStat(&(*i));
		}
	}
}

bool Game::doStat(const Command *cmd) {
	bool keepProcessing = true;
	bool rc = false;
	Database *db = _vm->database();

	debug(1, "Trying to execute Command %d - value %hd", cmd->cmd, cmd->value);

	for (Common::List<OpCode>::const_iterator j = cmd->opcodes.begin();
			j != cmd->opcodes.end() && keepProcessing; ++j) {

		switch (j->opcode) {
		case 327:
			db->setVar(j->arg2, j->arg3);
			break;
		case 328:
			db->setVar(j->arg2, db->getVar(j->arg3));
			break;
		case 331:
			db->setVar(j->arg2, 0);
			break;
		case 334:
			db->setVar(j->arg2, _rnd.getRandomNumber(j->arg3 - 1));
			break;
		case 337:
			keepProcessing = db->getVar(j->arg2) == 0;
			break;
		case 338:
			keepProcessing = db->getVar(j->arg2) != 0;
			break;
		case 340:
			keepProcessing = db->getVar(j->arg2) == j->arg3;
			break;
		case 345:
			keepProcessing = db->getVar(j->arg2) != j->arg3;
			break;
		case 346:
			keepProcessing = db->getVar(j->arg2) !=
				db->getVar(j->arg3);
			break;
		case 350:
			keepProcessing = db->getVar(j->arg2) > j->arg3;
			break;
		case 353:
			keepProcessing = db->getVar(j->arg2) >= j->arg3;
			break;
		case 356:
			keepProcessing = db->getVar(j->arg2) < j->arg3;
			break;
		case 359:
			keepProcessing = db->getVar(j->arg2) <= j->arg3;
			break;
		case 373:
			db->setVar(j->arg2, db->getVar(j->arg2) + 1);
			break;
		case 374:
			db->setVar(j->arg2, db->getVar(j->arg2) - 1);
			break;
		case 376:
			db->setVar(j->arg2,
				db->getVar(j->arg2) + db->getVar(j->arg3));
			break;
		case 377:
			db->setVar(j->arg2, db->getVar(j->arg2) + j->arg3);
			break;
		case 379:
			db->setVar(j->arg2,
				db->getVar(j->arg2) - db->getVar(j->arg3));
			break;
		case 380:
			db->setVar(j->arg2, db->getVar(j->arg2) - j->arg3);
			break;
		case 381:
			keepProcessing = db->getChar(0)->_locationId == j->arg2;
			break;
		case 382:
			keepProcessing = db->getChar(0)->_locationId != j->arg2;
			break;
		case 383:
			keepProcessing = false;
			if (db->getObj(j->arg2)->ownerType == 3)
				keepProcessing = db->getObj(j->arg2)->ownerId == 0;
			break;
		case 384:
			keepProcessing = true;
			if (db->getObj(j->arg2)->ownerType == 3)
				keepProcessing = db->getObj(j->arg2)->ownerId != 0;
			break;
		case 387:
			keepProcessing = db->giveObject(j->arg2, 0, false);
			break;
		case 391:
			keepProcessing = db->getChar(0)->_gold != 0;
			break;
		case 392:
			keepProcessing = db->getChar(0)->_gold == 0;
			break;
		case 393:
			if (db->getChar(0)->_gold >= j->arg3) {
				db->getChar(0)->_gold -= j->arg3;
				db->getChar(j->arg2)->_gold += j->arg3;
			}
			break;
		case 394:
			keepProcessing = false;
			if (j->arg4 == -1)
				keepProcessing = db->getChar(j->arg2)->_locationId == j->arg3;
			else if (db->getChar(j->arg2)->_locationId == j->arg3)
				keepProcessing = db->getChar(j->arg2)->_box == j->arg4;
			break;
		case 395:
			keepProcessing = true;
			if (j->arg4 == -1)
				keepProcessing = db->getChar(j->arg2)->_locationId != j->arg3;
			else if (db->getChar(j->arg2)->_locationId == j->arg3)
				keepProcessing = db->getChar(j->arg2)->_box != j->arg4;
			break;
		case 398:
			keepProcessing =
				db->getObj(j->arg3)->ownerType == 3 &&
				db->getObj(j->arg3)->ownerId == j->arg2;
			break;
		case 399:
			keepProcessing = true;
			if (db->getObj(j->arg3)->ownerType == 3)
				keepProcessing = db->getObj(j->arg3)->ownerId != j->arg2;
			break;
		case 403:
			// warning("TODO: move actor stub: %d %d %d", j->arg2, j->arg3, j->arg4);
			db->setCharPos(j->arg2, j->arg3, j->arg4);
			doActionMoveChar(j->arg2, j->arg3, j->arg4);
			db->getChar(j->arg2)->_destLoc =
			db->getChar(j->arg2)->_destBox = -4;
			break;
		case 402:
			db->getChar(j->arg2)->_destLoc = j->arg3;
			db->getChar(j->arg2)->_destBox = j->arg4;
			break;
		case 405:
			db->getChar(j->arg2)->_destLoc =
			db->getChar(j->arg2)->_destBox = -2;
			break;
		case 406:
			db->getChar(j->arg2)->_destLoc =
			db->getChar(j->arg2)->_destBox = -3;
			break;
		case 407:
			db->getChar(j->arg2)->_destLoc =
			db->getChar(j->arg2)->_destBox = -4;
			break;
		case 408:
			db->getChar(j->arg2)->_destLoc =
			db->getChar(j->arg2)->_destBox = -5;
			break;
		case 409:
			keepProcessing = db->getChar(j->arg2)->_isBusy;
			break;
		case 410:
			keepProcessing = !(db->getChar(j->arg2)->_isBusy);
			break;
		case 411:
			keepProcessing = db->getChar(j->arg2)->_isAlive;
			break;
		case 412:
			keepProcessing = !(db->getChar(j->arg2)->_isAlive);
			break;
		case 414:
			warning("TODO: unset spell");
			db->getChar(j->arg2)->_isAlive = false;
			break;
		case 416:
			db->getChar(j->arg2)->_hitPoints =
				(db->getVar(j->arg3) ? db->getVar(j->arg3) : 0);
			if (db->getChar(j->arg2)->_hitPoints > db->getChar(j->arg2)->_hitPointsMax)
				db->getChar(j->arg2)->_hitPoints = db->getChar(j->arg2)->_hitPointsMax;
			break;
		case 417:
			db->getChar(j->arg2)->_gold =
				db->getVar(j->arg3);
			break;
		case 418:
			db->getChar(j->arg2)->_spellPoints =
				(db->getVar(j->arg3) ? db->getVar(j->arg3) : 0);
			if (db->getChar(j->arg2)->_spellPoints > db->getChar(j->arg2)->_spellPointsMax)
				db->getChar(j->arg2)->_spellPoints = db->getChar(j->arg2)->_spellPointsMax;
			break;
		case 422:
			db->setVar(j->arg2, db->getChar(j->arg3)->_hitPoints);
			break;
		case 423:
			db->setVar(j->arg2, db->getChar(j->arg3)->_spellMode);
			break;
		case 424:
			db->setVar(j->arg2, db->getChar(j->arg3)->_gold);
			break;
		case 425:
			db->setVar(j->arg2, db->getChar(j->arg3)->_spellPoints);
			break;
		case 426:
			keepProcessing = db->getChar(j->arg2)->_locationId ==
			    db->getChar(j->arg3)->_locationId;
			break;
		case 427:
			keepProcessing = db->getChar(j->arg2)->_locationId !=
			    db->getChar(j->arg3)->_locationId;
			break;
		case 430:
			db->getChar(j->arg2)->_mode = 1;
			db->getChar(j->arg2)->_modeCount = j->arg3;
			break;
		case 431:
			db->getChar(j->arg2)->_mode = 1;
			db->getChar(j->arg2)->_modeCount = db->getVar(j->arg3);
			break;
		case 432:
			db->getChar(j->arg2)->_isVisible = true;
			break;
		case 433:
			db->getChar(j->arg2)->_isVisible = false;
			break;
		case 434:
			//warning("TODO: doActionCollide(%d, %d)", j->arg2, j->arg3);
			keepProcessing = false;
			break;
		case 438:
			db->getChar(j->arg2)->_strength = j->arg3;
			db->getChar(j->arg2)->_defense = j->arg4;
			db->getChar(j->arg2)->_damageMin = j->arg5;
			db->getChar(j->arg2)->_damageMax = j->arg6;
			break;
		case 441:
			keepProcessing = db->giveObject(j->arg2, j->arg3, j->arg4);
			break;
		case 444:
			db->getObj(j->arg2)->isCarryable = 1;
			break;
		case 445:
			db->getObj(j->arg2)->isVisible = 1;
			break;
		case 446:
			db->getObj(j->arg2)->data4 = 0;
			break;
		case 448:
			db->getObj(j->arg2)->isVisible = 0;
			break;
		case 449:
			keepProcessing = _settings.dayMode == 0;
			break;
		case 450:
			keepProcessing = _settings.dayMode == 1;
			break;
		case 451:
			keepProcessing = _settings.dayMode == 2;
			break;
		case 452:
			keepProcessing = _settings.dayMode == 3;
			break;
		case 453:
			setDay();
			break;
		case 454:
			setNight();
			break;
		case 458:
			db->getChar(j->arg2)->_xtend = j->arg3;
			changeMode(j->arg2, 2);
			break;
		case 459:
			db->getLoc(j->arg2)->xtend = j->arg3;
			changeMode(j->arg2, 3);
			break;
		case 465:
			db->setVar(j->arg2, db->getChar(j->arg3)->_xtend);
			break;
		case 466:
			db->setVar(j->arg2, db->getLoc(j->arg3)->xtend);
			break;
		case 467:
			warning("TODO: PlayVideo(%s)", j->arg1);
			break;
		case 468:
			warning("TODO: PlaySpriteScene(%s, %d, %d, %d)", j->arg1, j->arg2, j->arg3, j->arg4);
			break;
		case 469:
			warning("TODO: PlaySample(%s)", j->arg1);
			break;
		case 474:
			if (strcmp(j->arg1, "REFRESH") == 0) {
				warning("TODO: Unhandled external action: REFRESH");
			} else {
				db->setVar(j->arg2, doExternalAction(j->arg1));
			}
			break;
		case 475:
			keepProcessing = false;
			rc = true;
			break;
		case 485:
			_settings.fightEnabled = true;
			break;
		case 486:
			_settings.fightEnabled = false;
			break;
		case 487:
			// warning("TODO: npcFight(%d, %d)", j->arg2, j->arg3);
			keepProcessing = false;
			break;
		case 488:
			warning("TODO: castSpell(%d, %d, %d)", j->arg2, j->arg3, j->arg4);
			break;
		case 494:
			_vm->endGame();
			break;
		default:
			warning("Unhandled OpCode: %d - (%s, %d, %d, %d, %d, %d)", j->opcode,
				j->arg1, j->arg2, j->arg3, j->arg4, j->arg5, j->arg6);
		}
	}

	return rc;
}

void Game::loopMove() {
	// TODO - handle player char

	for (uint16 i = 1; i < _vm->database()->charactersNum(); ++i) {
		Character *chr = _vm->database()->getChar(i);

		if (!(chr->_isAlive)) {
			// TODO - set some stuff
			chr->_screenH = 0;
			chr->_offset10 = 0;
			chr->_offset14 = chr->_offset20 = 262144;
			chr->_scopeWanted = 100;
			chr->moveCharOther();

		} else if (chr->_walkSpeed == 0) {
			chr->_scopeWanted = 17;
			chr->_priority = _vm->database()->getPriority(chr->_lastLocation, chr->_lastBox);
			chr->moveCharOther();

		} else {
			if (chr->_spriteSceneState == 0 && chr->_fightPartner < 0) {
				int16 destBox = chr->_destBox;
				chr->_gotoLoc = chr->_destLoc;

				if (destBox + 5 <= 3)  {
					switch (destBox + 5) {
					case 0:
					case 1:
					case 2:
					case 3:
						break;
					}
					// TODO
				} else {
					chr->_gotoX = _vm->database()->getMidX(chr->_gotoLoc, destBox);
					chr->_gotoY = _vm->database()->getMidY(chr->_gotoLoc, destBox);
				}
			}

			// TODO - collision
			// TODO - fight-related thing

			if (chr->_mode == 1)
				chr->stopChar();

			if (chr->_spriteTimer <= 0 && chr->_fightPartner < 0)
				chr->moveChar(true);
			chr->moveCharOther();

			if (chr->_gotoBox != chr->_lastBox) {
				chr->_gotoBox = chr->_lastBox;
				_vm->database()->setCharPos(i, chr->_lastLocation, chr->_lastBox);
			}
		}
	}

	// TODO - handle magic actors
}

void Game::loopSpriteCut() {
	for (uint16 i = 1; i < _vm->database()->charactersNum(); ++i) {
		Character *chr = _vm->database()->getChar(i);

		if (chr->_actorId < 0)
			continue;

		switch (chr->_spriteSceneState) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			if (chr->_spriteTimer > 0)
				chr->_spriteTimer--;
			if (chr->_spriteTimer <= 1)
				chr->_isBusy = false;

			if (chr->_spriteTimer == 0) {
				chr->_spriteSceneState = 0;
				chr->_lastDirection = 4;
			}
			break;
		default:
			continue;
		}
	}
}

void Game::changeMode(int value, int mode) {
	warning("TODO: changeMode - unsupported mode");
}

int16 Game::doExternalAction(const char *action) {
	if (strcmp(action, "getquest") == 0) {
		return _settings.selectedQuest;
	} else {
		// TODO - warning("Unknown external action: %s", action);
		return 0;
	}
}
void Game::doActionDusk() {
	_player.isNight = 1;
	enterLocation(_vm->database()->getChar(0)->_locationId);
}

void Game::doActionDawn() {
	_player.isNight = 0;
	enterLocation(_vm->database()->getChar(0)->_locationId);
}

void Game::setDay() {
	if (_vm->database()->getLoc(_settings.currLocation)->allowedTime != 2 && _settings.dayMode != 0) {
		_settings.gameCycles = 3600;
		_settings.dayMode = 1;
	}
}

void Game::setNight() {
	if (_vm->database()->getLoc(_settings.currLocation)->allowedTime != 1 && _settings.dayMode != 1) {
		_settings.gameCycles = 6000;
		_settings.dayMode = 0;
	}
}

void Game::doActionMoveChar(uint16 charId, int16 loc, int16 box) {
	Character *chr = _vm->database()->getChar(charId);
	if (charId == 0 && loc > 0) {
		enterLocation(loc);
		// TODO: flicUpdateBG()
	}

	if (_vm->database()->loc2loc(loc, chr->_lastLocation) == -1) {
		// TODO: magicActors thing
		//for (int i = 0; i < 10; ++i) {
		//}
	}

	chr->_lastLocation = loc;
	chr->_lastBox = box;
	chr->_gotoBox = -1;

	chr->_screenX = _vm->database()->getMidX(loc, box);
	chr->_start3 = chr->_screenX * 256;
	chr->_screenY = _vm->database()->getMidY(loc, box);
	chr->_start4 = chr->_screenY * 256;
	chr->_start5 = _vm->database()->getZValue(loc, box, chr->_start4);
	chr->stopChar();
	chr->_lastDirection = 4;
}

} // End of namespace Kom
