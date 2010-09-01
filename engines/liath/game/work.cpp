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

#include "liath/game/work.h"

#include "liath/game/game.h"
#include "liath/game/hero.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

WorkManager::WorkManager(LiathEngine *engine) : _engine(engine) {}

WorkManager::~WorkManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////
OpcodeRet WorkManager::cel(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	return RET(_currentWork->field_76 == params->param, params->field_5);
}

OpcodeRet WorkManager::celExt(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDBDD);

	HeroIndex heroIndex = 0;
	uint32 val = 0;

	switch (params->param2) {
	default:
		break;

	case kOriginGlobal:
		heroIndex = *GLOBAL(params->param4);
		break;

	case kOriginHero:
		heroIndex = *getHero()->getData(params->param3, params->param4);
		break;

	case kOriginHeroWork:
		heroIndex = *getHero()->getData(_currentWork->heroIndex, params->param4);
		break;

	case kOriginParam:
		heroIndex = params->param3;
		break;
	}

	switch (params->param5) {
	default:
		break;

	case kOriginGlobal:
		val = *GLOBAL(params->param7);
		break;

	case kOriginHero:
		val = *getHero()->getData(params->param6, params->param7);
		break;

	case kOriginHeroWork:
		val = *getHero()->getData(_currentWork->heroIndex, params->param7);
		break;

	case kOriginParam:
		val = params->param7;
		break;
	}

	Work *work = getHero()->get(heroIndex)->work;
	return RET(work && work->field_68 == val && work->field_76 == params->param, params->field_5);
}

OpcodeRet WorkManager::startObj(ObjectIndex object) {
	if (_currentWork->isObjectIndexSet)
		return kOpcodeRetNextOffset;

	_currentWork->isObjectIndexSet = true;
	_currentWork->objectIndex = object;

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::htime(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	_currentWork->time = params->param;
	_currentWork->field_FE = params->param;

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::hdark(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersBDDD);

	_currentWork->field_579 = params->param;

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::restart() {
	error("WorkManager::restart: not implemented!");
}

OpcodeRet WorkManager::popHerous() {
	error("WorkManager::pop_herous: Not implemented!");
}

OpcodeRet WorkManager::pushHerous() {
	error("WorkManager::push_herous: Not implemented!");
}

OpcodeRet WorkManager::stop(OpcodeParameters *parameters) {
	error("WorkManager::stop: Not implemented!");
}

OpcodeRet WorkManager::stopGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *GLOBAL(params->param3);
	return stop(params);
}

OpcodeRet WorkManager::stopHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getHero()->getData(params->param2, params->param3);
	return stop(params);
}

OpcodeRet WorkManager::stopVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getHero()->getData(_currentWork->heroIndex, params->param3);
	return stop(params);
}

OpcodeRet WorkManager::cont(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	Hero *hero = getHero()->get(params->param ? params->param : _currentWork->heroIndex);
	Work *work = params->param ? hero->work : _currentWork;

	if (work && work->status == 2) {
		work->status = 0;
		work->isObjectIndexSet = work->field_59;
	} else if (hero->object->field_1C == 2) {
		hero->object->field_1C = 4;
		hero->object->field_20 = hero->object->field_24;
	}

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::contGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *GLOBAL(params->param3);
	return cont(params);
}

OpcodeRet WorkManager::contHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getHero()->getData(params->param2, params->param3);
	return cont(params);
}

OpcodeRet WorkManager::contVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getHero()->getData(_currentWork->heroIndex, params->param3);
	return cont(params);
}

OpcodeRet WorkManager::glass(OpcodeParameters *parameters, bool doIncrement) {
	error("WorkManager::glass: Not implemented!");
}

OpcodeRet WorkManager::savetop(OpcodeParameters *parameters) {
	_currentWork->field_B1 = _currentWork->field_A9;
	_currentWork->field_B3 = _currentWork->field_AB;
	_currentWork->field_B5 = _currentWork->field_AD;
	_currentWork->field_B7 = _currentWork->field_AF;

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::endtop() {
	_currentWork->field_B1 = 10000;
	return kOpcodeRetDefault;
}


} // End of namespace Liath
