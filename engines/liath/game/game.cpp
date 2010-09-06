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
	error("GameManager::playAction: Not implemented!");
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
	EXPOSE_PARAMS(OpcodeParametersDefault);

	uint32 val = (params->param1 ? _engine->getRandom().getRandomNumber(params->param1): 0);

	return getReturnValue(val + 1 == params->param1, params->test);
}

OpcodeRet GameManager::global(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	debugC(kLiathDebugInterpreter, "  global: %d  -  expression: %d - count: %d\n", params->param1, params->param3, params->param2);

	setGlobal(params->param1, EXPR(params->param3, params->param2));

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

int32 GameManager::getValue(ParamOrigin type, HeroIndex index, uint32 offset) {
	switch (type) {
	default:
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
