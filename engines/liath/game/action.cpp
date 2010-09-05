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
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

ActionManager::ActionManager(LiathEngine *engine) : _engine(engine),
	_action(kActionNone), _previousAction(kActionNone),
	_aviType(0), _avsX(0), _avsY(0)
{

}

ActionManager::~ActionManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////
OpcodeRet ActionManager::exitGame(OpcodeParameters *parameters) {
	_action = kActionNone;
	return kOpcodeRetExit;
}

OpcodeRet ActionManager::start(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	_action = (ActionIndex)params->param1;

	if (params->objectIndex == kParamClearAvi) {
		_aviFilename = "";
	} else {
		_aviFilename = Common::String::printf("%s", &params->param5);
		_aviType = params->param13;
		_avsX = params->param14;
		_avsY = params->param15;
	}

	if (GLOBAL(121) == 131072 || GLOBAL(121) == 65536)
		SETGLOBAL(121, 196608);

	return kOpcodeRetExit;
}

OpcodeRet ActionManager::startGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	_action = (ActionIndex)GLOBAL(params->param4);
	return kOpcodeRetExit;
}

OpcodeRet ActionManager::startHeroVariable(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	if (params->objectIndex == kParamClearAvi) {
		_aviFilename = "";
	} else {
		_aviFilename = Common::String::printf("%s", &params->param5);
		_aviType = params->param13;
	}

	_action = (ActionIndex)getHero()->getData(params->param2, params->param3);

	return kOpcodeRetExit;
}

OpcodeRet ActionManager::startVariable(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	if (params->objectIndex == kParamClearAvi) {
		_aviFilename = "";
	} else {
		_aviFilename = Common::String::printf("%s", &params->param5);
		_aviType = params->param13;
	}

	_action = (ActionIndex)getHero()->getData(getWork()->getCurrent()->heroIndex, params->param3);

	return kOpcodeRetExit;
}

OpcodeRet ActionManager::curAction(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	return RET(_previousAction == (ActionIndex)params->param1, params->test);
}

OpcodeRet ActionManager::number(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersBDDD);

	switch (params->param1) {
	default:
		break;

	case kOriginGlobal:
		SETGLOBAL(params->param3, _previousAction);
		break;

	case kOriginHero:
		getHero()->setData(params->param3, params->param4, _previousAction);
		break;

	case kOriginHeroWork:
		getHero()->setData(getWork()->getCurrent()->heroIndex, params->param4, _previousAction);
		break;
	}

	return kOpcodeRetDefault;
}

} // End of namespace Liath
