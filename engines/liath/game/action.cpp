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

#include "liath/game/action.h"

#include "liath/game/game.h"
#include "liath/game/hero.h"
#include "liath/game/segment.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

ActionManager::ActionManager(LiathEngine *engine) : _engine(engine),
	_currentAction(kActionNone), _previousAction(kActionNone),
	_aviType(0), _avsX(0), _avsY(0) {

}

ActionManager::~ActionManager() {
	// Zero-out passed pointers
	_engine = NULL;
}


//////////////////////////////////////////////////////////////////////////
// Data
//////////////////////////////////////////////////////////////////////////
void ActionManager::load() {
	_name = getSegment()->load(kSegmentAction, _currentAction);
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet ActionManager::exitGame(OpcodeParameters *parameters) {
	_currentAction = kActionNone;
	return kOpcodeRetExit;
}

OpcodeRet ActionManager::start(OpcodeParameters *parameters) {
	_currentAction = (ActionIndex)parameters->getDword(0);

	if (parameters->objectIndex == kParamClearAvi) {
		_aviFilename = "";
	} else {
		_aviFilename = Common::String::format("%s", &parameters->params + 16);
		_aviType = parameters->getByte(36);
		_avsX = parameters->getWord(37);
		_avsY = parameters->getWord(39);
	}

	if (GLOBAL(121) == 131072 || GLOBAL(121) == 65536)
		SETGLOBAL(121, 196608);

	return kOpcodeRetExit;
}

OpcodeRet ActionManager::startGlobal(OpcodeParameters *parameters) {
	_currentAction = (ActionIndex)GLOBAL(parameters->getDword(12));

	return kOpcodeRetExit;
}

OpcodeRet ActionManager::startHeroVariable(OpcodeParameters *parameters) {
	if (parameters->objectIndex == kParamClearAvi) {
		_aviFilename = "";
	} else {
		_aviFilename = Common::String::format("%s", &parameters->params + 16);
		_aviType = parameters->getByte(36);
	}

	_currentAction = (ActionIndex)getHero()->getData(parameters->getDword(4), parameters->getDword(8));

	return kOpcodeRetExit;
}

OpcodeRet ActionManager::startVariable(OpcodeParameters *parameters) {
	if (parameters->objectIndex == kParamClearAvi) {
		_aviFilename = "";
	} else {
		_aviFilename = Common::String::format("%s", (&parameters->params + 16));
		_aviType = parameters->getByte(36);
	}

	_currentAction = (ActionIndex)getHero()->getData(getWork()->getCurrent()->heroIndex, parameters->getDword(8));

	return kOpcodeRetExit;
}

OpcodeRet ActionManager::curAction(OpcodeParameters *parameters) {
	return RET(_previousAction == (ActionIndex)parameters->getDword(0), parameters->test);
}

OpcodeRet ActionManager::number(OpcodeParameters *parameters) {

	getGame()->letValue((ParamOrigin)parameters->getByte(0),
	                    parameters->getDword(1),
	                    parameters->getDword(5),
	                    _previousAction);

	return kOpcodeRetDefault;
}

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

void ActionManager::playVideo() {
	if (_aviFilename.empty())
		return;

	if (_aviType)
		playAvs(_avsX, _avsY, 0);
	else
		playAvi();
}

void ActionManager::playAvi() {
	error("ActionManager::playAvi: Not implemented!");
}

void ActionManager::playAvs(int16 x, int16 y, int param3) {
	error("ActionManager::playAvs: Not implemented!");
}

} // End of namespace Liath
