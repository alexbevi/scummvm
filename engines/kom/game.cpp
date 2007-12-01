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
	_vm->database()->getCharScope(0)->lastLocation = locId;

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
		if (_vm->database()->getChar(0)->isBusy && _settings.gameCycles >= 6000)
			_settings.gameCycles = 5990;

		if (_vm->database()->getLoc(_settings.currLocation)->allowedTime == 2) {
			_settings.dayMode = 3;
			doActionDusk();
			processChars();
			_settings.dayMode = 1;
			_settings.gameCycles = 0;
		}

		if (_settings.gameCycles < 6000) {

			if (!_vm->database()->getChar(0)->isBusy)
				(_settings.gameCycles)++;

		} else if (_vm->database()->getLoc(_settings.currLocation)->allowedTime == 0) {
			_settings.dayMode = 3;
			doActionDusk();
			processChars();
			_settings.dayMode = 1;
			_settings.gameCycles = 0;
		}

	} else if (_settings.dayMode == 1) {

		if (_vm->database()->getChar(0)->isBusy && _settings.gameCycles >= 3600)
			_settings.gameCycles = 3590;

		if (_vm->database()->getLoc(_settings.currLocation)->allowedTime == 1) {
			_settings.dayMode = 2;
			doActionDawn();
			processChars();
			_settings.dayMode = 0;
			_settings.gameCycles = 0;
		}

		if (_settings.gameCycles < 3600) {

			if (!_vm->database()->getChar(0)->isBusy)
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
		if (ch->isAlive && ch->proc != -1 && ch->mode < 6) {
			switch (ch->mode) {
			case 0:
			case 3:
				processChar(ch->proc);
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
			db->setVar(j->arg2, _rnd.getRandomNumber(j->arg3));
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
			keepProcessing = db->getChar(0)->locationId == j->arg2;
			break;
		case 382:
			keepProcessing = db->getChar(0)->locationId != j->arg2;
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
			keepProcessing = db->getChar(0)->gold != 0;
			break;
		case 392:
			keepProcessing = db->getChar(0)->gold == 0;
			break;
		case 393:
			if (db->getChar(0)->gold >= j->arg3) {
				db->getChar(0)->gold -= j->arg3;
				db->getChar(j->arg2)->gold += j->arg3;
			}
			break;
		case 394:
			keepProcessing = false;
			if (j->arg4 == -1)
				keepProcessing = db->getChar(j->arg2)->locationId == j->arg3;
			else if (db->getChar(j->arg2)->locationId == j->arg3)
				keepProcessing = db->getChar(j->arg2)->box == j->arg4;
			break;
		case 395:
			keepProcessing = true;
			if (j->arg4 == -1)
				keepProcessing = db->getChar(j->arg2)->locationId != j->arg3;
			else if (db->getChar(j->arg2)->locationId == j->arg3)
				keepProcessing = db->getChar(j->arg2)->box != j->arg4;
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
			if (j->arg2 == 0)
				enterLocation(j->arg3);
			break;
		case 402:
			db->getChar(j->arg2)->destLoc = j->arg3;
			db->getChar(j->arg2)->destBox = j->arg4;
			break;
		case 405:
			db->getChar(j->arg2)->destLoc =
			db->getChar(j->arg2)->destBox = -2;
			break;
		case 406:
			db->getChar(j->arg2)->destLoc =
			db->getChar(j->arg2)->destBox = -3;
			break;
		case 407:
			db->getChar(j->arg2)->destLoc =
			db->getChar(j->arg2)->destBox = -4;
			break;
		case 408:
			db->getChar(j->arg2)->destLoc =
			db->getChar(j->arg2)->destBox = -5;
			break;
		case 409:
			keepProcessing = db->getChar(j->arg2)->isBusy;
			break;
		case 410:
			keepProcessing = !(db->getChar(j->arg2)->isBusy);
			break;
		case 411:
			keepProcessing = db->getChar(j->arg2)->isAlive;
			break;
		case 412:
			keepProcessing = !(db->getChar(j->arg2)->isAlive);
			break;
		case 414:
			warning("TODO: unset spell");
			db->getChar(j->arg2)->isAlive = false;
			break;
		case 416:
			db->getChar(j->arg2)->hitPoints =
				(db->getVar(j->arg3) ? db->getVar(j->arg3) : 0);
			if (db->getChar(j->arg2)->hitPoints > db->getChar(j->arg2)->hitPointsMax)
				db->getChar(j->arg2)->hitPoints = db->getChar(j->arg2)->hitPointsMax;
			break;
		case 417:
			db->getChar(j->arg2)->gold =
				db->getVar(j->arg3);
			break;
		case 418:
			db->getChar(j->arg2)->spellPoints =
				(db->getVar(j->arg3) ? db->getVar(j->arg3) : 0);
			if (db->getChar(j->arg2)->spellPoints > db->getChar(j->arg2)->spellPointsMax)
				db->getChar(j->arg2)->spellPoints = db->getChar(j->arg2)->spellPointsMax;
			break;
		case 422:
			db->setVar(j->arg2, db->getChar(j->arg3)->hitPoints);
			break;
		case 423:
			db->setVar(j->arg2, db->getChar(j->arg3)->spellMode);
			break;
		case 424:
			db->setVar(j->arg2, db->getChar(j->arg3)->gold);
			break;
		case 425:
			db->setVar(j->arg2, db->getChar(j->arg3)->spellPoints);
			break;
		case 426:
			keepProcessing = db->getChar(j->arg2)->locationId ==
			    db->getChar(j->arg3)->locationId;
			break;
		case 427:
			keepProcessing = db->getChar(j->arg2)->locationId !=
			    db->getChar(j->arg3)->locationId;
			break;
		case 430:
			db->getChar(j->arg2)->mode = 1;
			db->getChar(j->arg2)->modeCount = j->arg3;
			break;
		case 431:
			db->getChar(j->arg2)->mode = 1;
			db->getChar(j->arg2)->modeCount = db->getVar(j->arg3);
			break;
		case 432:
			db->getChar(j->arg2)->isVisible = true;
			break;
		case 433:
			db->getChar(j->arg2)->isVisible = false;
			break;
		case 434:
			//warning("TODO: doActionCollide(%d, %d)", j->arg2, j->arg3);
			keepProcessing = false;
			break;
		case 438:
			db->getChar(j->arg2)->strength = j->arg3;
			db->getChar(j->arg2)->defense = j->arg4;
			db->getChar(j->arg2)->damageMin = j->arg5;
			db->getChar(j->arg2)->damageMax = j->arg6;
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
			db->getChar(j->arg2)->xtend = j->arg3;
			changeMode(j->arg2, 2);
			break;
		case 459:
			db->getLoc(j->arg2)->xtend = j->arg3;
			changeMode(j->arg2, 3);
			break;
		case 465:
			db->setVar(j->arg2, db->getChar(j->arg3)->xtend);
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
		CharScope *scp = _vm->database()->getCharScope(i);

		if (!(_vm->database()->getChar(i)->isAlive)) {
			// TODO - set some stuff
			scp->screenH = 0;
			scp->offset10 = 0;
			scp->offset14 = scp->offset20 = 262144;
			scp->scopeWanted = 100;
			moveCharOther(i);

		} else {
			if (scp->walkSpeed == 0) {
				scp->scopeWanted = 17;
				scp->priority = _vm->database()->getPriority(scp->lastLocation, scp->lastBox);
				moveCharOther(i);

			} else {
				if (scp->spriteSceneState == 0) {
					if (scp->fightPartner < 0) {
						int16 destBox = _vm->database()->getChar(i)->destBox;
						scp->gotoLoc = _vm->database()->getChar(i)->destLoc;

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
							scp->gotoX = _vm->database()->getMidX(scp->gotoLoc, destBox);
							scp->gotoY = _vm->database()->getMidY(scp->gotoLoc, destBox);
						}

						// TODO - collision
						// TODO - fight-related thing

						if (_vm->database()->getChar(i)->mode == 1) {
							// TODO - stop actor
						}

						if (scp->spriteTimer == 0 && scp->fightPartner < 0) {
							moveChar(i, true);
						}
						moveCharOther(i);

						// if goto_box != last_box:
							// goto_box = last_box
							_vm->database()->setCharPos(i, scp->lastLocation, scp->lastBox);

					}
				} else {

				}
			}
		}
	}

	// TODO - handle magic actors
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
	enterLocation(_vm->database()->getChar(0)->locationId);
}

void Game::doActionDawn() {
	_player.isNight = 0;
	enterLocation(_vm->database()->getChar(0)->locationId);
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

void Game::setScope(uint16 charId, int16 scope) {
	Actor *act;

	// TODO - handle spell effect
	// TODO - pretty much everything

	// TODO - hack
	if (_vm->database()->getCharScope(charId)->actorId == -1) {
		setScopeX(charId, scope);
	}
	act = _vm->actorMan()->get(_vm->database()->getCharScope(charId)->actorId);

}

void Game::setScopeX(uint16 charId, int16 scope) {
	char filename[50];
	Actor *act;
	int scale;

	Character *character = _vm->database()->getChar(charId);
	CharScope *scp = _vm->database()->getCharScope(charId);
	String charName(character->name);
	charName.toLowercase();
	sprintf(filename, "%s%d", charName.c_str(), character->xtend);

	_vm->panel()->showLoading(true);
	scp->actorId =
		_vm->actorMan()->load(_vm->dataDir()->getChild("kom").getChild("actors"), filename);
	_vm->panel()->showLoading(false);


	act = _vm->actorMan()->get(scp->actorId);

	// TODO - lots
	act->defineScope(0, 0, act->getFramesNum() - 1, 0);
	act->setScope(0, scp->animSpeed);

	// TODO - this should be in processGraphics
	scale = (scp->start5 * 88) / 60;
	act->setPos(scp->screenX / 2, (scp->start4 + (scp->screenH + scp->offset78) / scale) / 256 / 2);
	act->setRatio(scp->ratioX / scale, scp->ratioY / scale);

	// TODO - this should be in loopMove
	act->setMaskDepth(_vm->database()->getPriority( scp->lastLocation, scp->lastBox), scp->start5);
}

void Game::moveChar(uint16 charId, bool param) {
	CharScope *scp = _vm->database()->getCharScope(charId);
	int16 nextLoc, targetBox, targetBoxX, targetBoxY, nextBox;

	//printf("moving char %hu\n", charId);

	if (scp->lastLocation == 0) return;

	if (scp->gotoLoc != scp->lastLocation) {

		if (scp->gotoLoc >= 0 && scp->lastLocation >= 0)
			nextLoc = _vm->database()->loc2loc(scp->lastLocation, scp->gotoLoc);
		else
			nextLoc = -1;

		if (nextLoc > 0) {
			targetBox = _vm->database()->getExitBox(scp->lastLocation, nextLoc);

			if (scp->lastLocation >= 0 && targetBox > 0) {
				Box *b = _vm->database()->getBox(scp->lastLocation, targetBox);
				targetBoxX = (b->x2 - b->x1) / 2 + b->x1;
				targetBoxY = (b->y2 - b->y1) / 2 + b->y1;
			} else {
				targetBoxX = 319;
				targetBoxY = 389;
			}

			scp->gotoX = targetBoxX;
			scp->gotoY = targetBoxY;

		} else
			targetBox = scp->lastBox;

	} else {
		targetBox = _vm->database()->whatBox(scp->lastLocation, scp->gotoX, scp->gotoY);
	}

	assert(targetBox != -1);

	if (scp->lastLocation > 127 || scp->lastBox > 127 || targetBox > 127) {
		nextBox = -1;
	} else {
		nextBox = _vm->database()->box2box(scp->lastLocation, scp->lastBox, targetBox);
	}

	int16 x, y, xMove, yMove;

	if (nextBox == scp->lastBox) {
		x = scp->gotoX;
		y = scp->gotoY;
	} else {
		if (nextBox == -1) {
			x = scp->screenX;
			y = scp->screenY;
		} else {
			if (_vm->database()->isInLine(scp->lastLocation, nextBox, scp->screenY, scp->screenY)) {
				if (scp->lastLocation > 0 && nextBox > 0) {
					Box *b = _vm->database()->getBox(scp->lastLocation, nextBox);
					x = b->x1 + (b->x2 - b->x1) / 2;
					y = b->y1 + (b->y2 - b->y1) / 2;
				} else {
					x = 319;
					y = 389;
				}

			} else {
				x = _vm->database()->getMidOverlapX(scp->lastLocation, scp->lastBox, nextBox);
				y = _vm->database()->getMidOverlapY(scp->lastLocation, scp->lastBox, nextBox);
			}
		}
	}

	xMove = (x - scp->screenX);
	yMove = (y - scp->screenY) * 2;

	if (abs(xMove) + abs(yMove) == 0) {
		scp->somethingX = scp->somethingY = 0;
	} else {
		int thing;
		assert(scp->start5 != 0);

		if (scp->relativeSpeed == 1024)
			thing = scp->walkSpeed / ((xMove + yMove) * scp->start5);
		else
			thing = scp->walkSpeed * scp->relativeSpeed / ((xMove + yMove) * scp->start5);

		// Now we should set 64h and 68h
		scp->somethingX = xMove * thing / 256;
		scp->somethingY = yMove * thing / 256;

		if (abs(xMove) / 256 < abs(scp->somethingX)) {
			scp->start3 = x * 256;
			scp->somethingX = 0;
		}

		if (abs(yMove)/ 256 < abs(scp->somethingY)) {
			scp->start4 = y * 256;
			scp->somethingY = 0;
		}
	}

	int futureBox, futureBox2; // TODO: I know, but nextBox is already used. something is screwy

	futureBox = scp->lastBox;
	scp->somethingY /= 2;

	if (!param) {
		Box *box;

		futureBox2 = _vm->database()->whatBox(scp->lastLocation,
				(scp->start3 + scp->somethingX) / 256,
				(scp->start4 + scp->somethingY) / 256);

		// TODO : check if box doesn't exist?
		box = _vm->database()->getBox(scp->lastLocation, futureBox2);

		if (box->attrib == 8)
			futureBox2 = -1;

	} else if (param) {
		futureBox2 = _vm->database()->whatBoxLinked(scp->lastLocation, scp->lastBox,
				(scp->start3 + scp->somethingX) / 256,
				(scp->start4 + scp->somethingY) / 256);
	}

	if (futureBox2 == -1) {
		Box *box;

		if (!param && nextBox == targetBox) {
			scp->lastBox = targetBox;
			futureBox = targetBox;
		}

		box = _vm->database()->getBox(scp->lastLocation, scp->lastBox);

		if (scp->somethingX < box->x1 * 256) {
			if (scp->somethingY < box->y1 * 256) {
				scp->somethingX = box->x1 * 256 - scp->start3;
				scp->somethingY = box->y1 * 256 - scp->start4;
			} else {
				if (scp->somethingY > box->y2 * 256) {
					scp->somethingY = box->y2 * 256 - scp->start4;
				}
				scp->somethingX = box->x1 * 256 - scp->start3;
			}
		} else if (scp->somethingX > box->x2 * 256) {
			if (scp->somethingY < box->y1 * 256) {
				scp->somethingX = box->x2 * 256 - scp->start3;
				scp->somethingY = box->y1 * 256 - scp->start4;
			} else {
				if (scp->somethingY > box->y2 * 256) {
					scp->somethingY = box->y2 * 256 - scp->start4;
				}
				scp->somethingX = box->x2 * 256 - scp->start3;
			}
		} else {
			scp->somethingY = box->y1 * 256 - scp->start4;
		}
	} else {
		futureBox = futureBox2;
	}

	if (nextBox == -1 && scp->somethingX == 0 && scp->somethingY == 0) {
		scp->direction = 0;
		scp->stopped = true;
	} else {
		scp->stopped = false;
	}

	scp->start3 += scp->somethingX;
	scp->start4 += scp->somethingY;
	scp->screenX = scp->start3 / 256;
	scp->screenY = scp->start4 / 256;
	scp->lastBox = futureBox;

	if (scp->lastLocation != 0 || scp->lastBox != 0) {
		scp->priority = _vm->database()->getBox(scp->lastLocation, scp->lastBox)->priority;
	} else {
		scp->priority = -1;
	}

	scp->start5 = _vm->database()->getZValue(scp->lastLocation, scp->lastBox, scp->start4);

	if (scp->lastLocation == _vm->database()->getCharScope(0)->lastLocation) {
		int32 tmp = (scp->start3 - scp->start3Prev) * scp->start5 / 256;
		int32 tmp2 = (scp->start5 - scp->start5PrevPrev) * 256;

		if (scp->direction != 0 && (abs(tmp) - abs(tmp2) & (int32)-128) == 0)
			scp->direction = scp->lastDirection;
		else if (abs(tmp) >= abs(tmp2)) {
			if (tmp < -4)
				scp->direction = 1;
			else if (tmp > 4)
				scp->direction = 2;
			else
				scp->direction = 0;
		} else {
			if (tmp2 < -4)
				scp->direction = 4;
			else if (tmp2 > 4)
				scp->direction = 3;
			else
				scp->direction = 0;
		}
	}

	switch (scp->direction) {
	case 0:
		switch (scp->lastDirection - 1) {
		case 0:
			scp->scopeWanted = 9;
			break;
		case 1:
			scp->scopeWanted = 7;
			break;
		case 2:
			scp->scopeWanted = 4;
			break;
		case 3:
		default:
			scp->scopeWanted = 8;
			break;
		}
		break;
	case 1:
		if (scp->lastDirection == 1)
			scp->scopeWanted = 1;
		else
			scp->lastDirection = 1;
		break;
	case 2:
		if (scp->lastDirection == 2)
			scp->scopeWanted = 0;
		else
			scp->lastDirection = 2;
		break;
	case 3:
		if (scp->lastDirection == 3)
			scp->scopeWanted = 2;
		else
			scp->lastDirection = 3;
		break;
	case 4:
		if (scp->lastDirection == 4)
			scp->scopeWanted = 3;
		else
			scp->lastDirection = 4;
		break;
	}
}

void Game::moveCharOther(uint16 charId) {
	CharScope *scp = _vm->database()->getCharScope(charId);

	if (scp->offset10 != 0 || scp->screenH != 0) {
		if (scp->screenH > scp->offset10)
			scp->offset0c -= 65536;
		else
			scp->offset0c += 65536;
	}

	scp->screenH += scp->offset0c;

	if (scp->screenH > 0) {
		if (scp->offset0c <= 262144) {
			scp->screenH = scp->offset0c = 0;
		} else {
			scp->screenH = 1;
			scp->offset0c /= -4;
		}
	}

	// X ratio
	if (scp->ratioX < scp->offset14) {
		if (scp->offset1c < 0)
			scp->offset1c += 131072;
		else
			scp->offset1c += 65536;
	} else if (scp->ratioX > scp->offset14) {
		if (scp->offset1c > 0)
			scp->offset1c -= 131072;
		else
			scp->offset1c -= 65536;
	}

	if (scp->ratioX != scp->offset14) {
		int32 foo = abs(scp->ratioX - scp->offset14);

		if (foo <= 65536 && abs(scp->offset1c) <= 131072) {
			scp->offset1c = 0;
			scp->ratioX = scp->offset14;
		} else {
			scp->ratioX += scp->offset1c;
		}

		if (scp->ratioX < 1280) {
			scp->offset1c /= -2;
			scp->ratioX = 1280;
		}
	}

	// Y ratio
	if (scp->ratioY < scp->offset20) {
		if (scp->offset28 < 0)
			scp->offset28 += 131072;
		else
			scp->offset28 += 65536;
	} else if (scp->ratioY > scp->offset20) {
		if (scp->offset28 > 0)
			scp->offset28 -= 131072;
		else
			scp->offset28 -= 65536;
	}

	if (scp->ratioY != scp->offset20) {
		int32 foo = abs(scp->ratioY - scp->offset20);

		if (foo <= 65536 && abs(scp->offset28) <= 131072) {
			scp->offset28 = 0;
			scp->ratioY = scp->offset20;
		} else {
			scp->ratioY += scp->offset28;
		}

		if (scp->ratioY < 1280) {
			scp->offset28 /= -2;
			scp->ratioY = 1280;
		}
	}
}

} // End of namespace Kom
