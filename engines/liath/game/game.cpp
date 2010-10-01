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

#include "liath/game/game.h"

#include "liath/game/action.h"
#include "liath/game/expression.h"
#include "liath/game/game.h"
#include "liath/game/graphics.h"
#include "liath/game/hero.h"
#include "liath/game/interpreter.h"
#include "liath/game/mouse.h"
#include "liath/game/segment.h"
#include "liath/game/sound.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"
#include "liath/resource.h"

namespace Liath {

GameManager::GameManager(LiathEngine *engine) : _engine(engine) {
	_interpreter = new Interpreter(_engine);

	_gInt = NULL;
	_gQuit = NULL;
	_param = 0;
	_gParam = 0;
	_timer = -1;
	_oldTimer = 0;
	_countHero = 0;
	_countVar = 0;

	// Global var area
	_globalVar = NULL;

	// Variables
	_currentCd = kCdNone;
	_currentAction = kActionNone;
	_currentWorkInfo = NULL;
}

GameManager::~GameManager() {
	// Zero-out passed pointers
	_engine = NULL;

	delete _interpreter;

	// Free global vars
	SAFE_FREE(_globalVar);
}

//////////////////////////////////////////////////////////////////////////
// Game playing
//////////////////////////////////////////////////////////////////////////
void GameManager::playGame(ActionIndex action) {
	// Read game segment
	getSegment()->load(kSegmentGame, 1);

	// Load game data
	GameData *gameData = (GameData *)getSegment()->getData(kSegmentGame, 2);
	getGame()->load(action, gameData);

	// Run the interpreter
	ObjectIndex *pObjectIndex = gameData->objectIndexOffset ? (uint32 *)getSegment()->getData(kSegmentGame, gameData->objectIndexOffset) : NULL;
	_interpreter->interpret(pObjectIndex, getSegment()->get(kSegmentGame));

	while (getAction()->getCurrentAction())
		playAction();

	// Cleanup
	getSegment()->unload(kSegmentGame);

	getGame()->unload();
}

void GameManager::playAction() {

	// Setup action & background
	//_exitAction = 0;
	//_bg = bg;

	// TODO Set cursor

	// Replace current action && work info
	if (_currentAction) {
		getAction()->setCurrentAction(_currentAction);
		getWork()->setWorkInfo(_currentWorkInfo);
		_currentWorkInfo = NULL;
	}


	Common::String actionFile = Common::String::printf("act%04d.dat", getAction()->getCurrentAction());
	CdNumber cd = getResource()->getCd(actionFile);

	if (!cd || cd == _currentCd)
		goto label_load;

	Action *action;
	for (;;) {
		_currentAction = getAction()->getCurrentAction();
		getAction()->setCurrentAction(kAction1);

		_currentWorkInfo = getWork()->getWorkInfo();
		getWork()->setWorkInfo(NULL);

		setGlobal(999, SCR2CEL(cd));

		actionFile = Common::String::printf("act%04d.dat", getAction()->getCurrentAction());
		cd = getResource()->getCd(actionFile);

label_load:
		getAction()->load();
		action = (Action *)getSegment()->getData(kSegmentAction, 2);

		if (!action->backgroundOffset)
			break;

		if (getAction()->getCurrentAction() == 1)
			break;

		if (_currentAction) {
			error("GameManager::playAction: Not implemented!");
		}

		_currentCd = getResource()->getCd((char *)getSegment()->getData(kSegmentAction, action->backgroundOffset));

		if (!cd || cd == _currentCd) {
			if (action->backgroundOffset) {
				if (getResource()->hasFile((char *)getSegment()->getData(kSegmentAction, action->backgroundOffset)))
					break;
			}
		}
	}

	Message message;
	bool hasMessage = getResource()->readMessage(&action->field_B9, &message);

	if (hasMessage) {
		getAction()->setFullname(getAction()->getName());
	} else {
		// TODO check in message.str

		error("GameManager::playAction: Message reading not implemented!");
	}

	getAction()->playVideo();


	if (_currentWorkInfo) {
		loadGameStartHero();
		processAction();
		return;
	}
	warning("GameManager::playAction: Not implemented!");

	ObjectIndex *pObjectIndex = (action->objectIndex ? getSegment()->getData(kSegmentAction, action->objectIndex) : NULL);
	if (_interpreter->interpret(pObjectIndex, getSegment()->get(kSegmentAction)))
		processAction();
}

void GameManager::processAction() {
	error("GameManager::processAction: Not implemented!");
}

void GameManager::loadGameStartHero() {
	error("GameManager::loadGameStartHero: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void GameManager::load(ActionIndex action, GameData *gameData) {
	// Setup interpreter data & param values
	_gInt  = gameData->gIntOffset ? (uint32 *)getSegment()->getData(kSegmentGame, gameData->gIntOffset) : NULL;
	_gQuit = gameData->gQuitOffset ? (uint32 *)getSegment()->getData(kSegmentGame, gameData->gQuitOffset) : NULL;
	_param = gameData->gParam + 1;
	_gParam = gameData->gParam;

	if (_timer == -1)
		_timer = gameData->timer;
	_oldTimer = gameData->timer;

	// Setup current action
	getAction()->setCurrentAction(action ? action : (ActionIndex)gameData->action);

	// Setup hero objects & load hero data
	_countHero = gameData->countHero;
	_countVar = _param + 14;
	getHero()->load(_countHero, 4 * _countVar);

	// Setup global var area
	_globalVar = (uint32 *)calloc(4 * _gParam, 1);

	// Load sound data
	getSound()->load();

	// Load palettes & graphic data
	getGraphics()->load(gameData->paletteOffset);
}

void GameManager::unload() {
	getHero()->unload();
	SAFE_FREE(_globalVar)
	getSound()->unload();
	getGraphics()->unload();
	getWork()->unload();
	getMouse()->unload();
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet GameManager::rnd(OpcodeParameters *parameters) {
	debugC(kLiathDebugInterpreter, " value: %d\n", parameters->getDword(0));

	uint32 val = (parameters->getDword(0) ? _engine->getRandom().getRandomNumber(parameters->getDword(0)): 0);

	return getReturnValue(val + 1 == parameters->getDword(0), parameters->test);
}

OpcodeRet GameManager::global(OpcodeParameters *parameters) {
	debugC(kLiathDebugInterpreter, "  global: %d  -  expression: %d - count: %d\n", parameters->getDword(0), parameters->getDword(8), parameters->getDword(4));

	setGlobal(parameters->getDword(0), EXPR(parameters->getDword(8), parameters->getDword(4)));

	return kOpcodeRetDefault;
}

OpcodeRet GameManager::key(OpcodeParameters *parameters) {
	error("GameManager::key: Not implemented!");
}

OpcodeRet GameManager::bkey(OpcodeParameters *parameters) {
	error("GameManager::bkey: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Helper Functions
//////////////////////////////////////////////////////////////////////////
void GameManager::letValue(ParamOrigin type, HeroIndex index, uint32 offset, uint32 val) {
	switch (type) {
	default:
		break;

	case kOriginGlobal:
		SETGLOBAL(offset, val);
		break;

	case kOriginHero:
		if (getHero()->get(index))
			getHero()->setData(index, offset, val);
		break;

	case kOriginHeroWork:
		if (getHero()->get(getWork()->getCurrent()->heroIndex))
			getHero()->setData(getWork()->getCurrent()->heroIndex, offset, val);
		break;
	}
}

int32 GameManager::getValue(ParamOrigin type, HeroIndex index, uint32 offset, bool handleUnknownParams, bool handleOriginParam) {
	switch (type) {
	default:
		if (!handleUnknownParams)
			error("GameManager::getValue: Invalid type encountered!");

		return 0;

	case kOriginGlobal:
		return GLOBAL(offset);

	case kOriginHero:
		if (getHero()->get(index))
			return getHero()->getData(index, offset);
		else
			return 0;

	case kOriginHeroWork:
		if (getHero()->get(getWork()->getCurrent()->heroIndex))
			return getHero()->getData(getWork()->getCurrent()->heroIndex, offset);
		else
			return 0;

	case kOriginParam:
		if (!handleOriginParam)
			error("GameManager::getValue: kOriginParam encountered!");

		return index;
	}
}

int32 GameManager::getGlobal(uint32 offset) {
	return (int32)READ_UINT32((byte *)_globalVar + offset);
}

void GameManager::setGlobal(uint32 offset, int32 val) {
	return WRITE_UINT32((byte *)_globalVar + offset, (uint32)val);
}

OpcodeRet GameManager::getReturnValue(int val, bool testValue) {
	if (val)
		return (testValue ? kOpcodeRetDefault : kOpcodeRetNext);
	else
		return (testValue ? kOpcodeRetNext : kOpcodeRetDefault);
}

} // End of namespace Liath
