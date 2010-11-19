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
// Loading
//////////////////////////////////////////////////////////////////////////
void WorkManager::unload() {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++) {
		Work *work = (*it);

		if (work->status == 1) {
			SAFE_FREE(work->object->field_18);
			SAFE_FREE(work->field_84);
			SAFE_FREE(work->field_80);
		}

		// SAFE_FREE(work->hGlobal); // This is the hglobal (not need to free it)
	}
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet WorkManager::cel(OpcodeParameters *parameters) {
	return RET(_currentWork->field_76 == parameters->getDword(0), parameters->test);
}

OpcodeRet WorkManager::celExt(OpcodeParameters *parameters) {
	HeroIndex heroIndex = 0;
	uint32 val = 0;

	switch (parameters->getByte(4)) {
	default:
		break;

	case kOriginGlobal:
		heroIndex = GLOBAL(parameters->getDword(9));
		break;

	case kOriginHero:
		heroIndex = getHero()->getData(parameters->getDword(5), parameters->getDword(9));
		break;

	case kOriginHeroWork:
		heroIndex = getHero()->getData(_currentWork->heroIndex, parameters->getDword(9));
		break;

	case kOriginParam:
		heroIndex = parameters->getDword(5);
		break;
	}

	switch (parameters->getByte(13)) {
	default:
		break;

	case kOriginGlobal:
		val = GLOBAL(parameters->getDword(18));
		break;

	case kOriginHero:
		val = getHero()->getData(parameters->getDword(14), parameters->getDword(18));
		break;

	case kOriginHeroWork:
		val = getHero()->getData(_currentWork->heroIndex, parameters->getDword(18));
		break;

	case kOriginParam:
		val = parameters->getDword(18);
		break;
	}

	Work *work = getHero()->get(heroIndex)->work;
	return RET(work && work->field_68 == val && work->field_76 == parameters->getDword(0), parameters->test);
}

OpcodeRet WorkManager::startObj(ObjectIndex object) {
	if (_currentWork->isObjectIndexSet)
		return kOpcodeRetNext;

	_currentWork->isObjectIndexSet = true;
	_currentWork->objectIndex = object;

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::htime(OpcodeParameters *parameters) {
	_currentWork->time = parameters->getDword(0);
	_currentWork->field_FE = parameters->getDword(0);

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::hdark(OpcodeParameters *parameters) {
	_currentWork->field_579 = parameters->getByte(0);

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
	parameters->setDword(0, GLOBAL(parameters->getDword(8)));

	return stop(parameters);
}

OpcodeRet WorkManager::stopHeroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(parameters->getDword(4), parameters->getDword(8)));

	return stop(parameters);
}

OpcodeRet WorkManager::stopVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(_currentWork->heroIndex, parameters->getDword(8)));

	return stop(parameters);
}

OpcodeRet WorkManager::cont(OpcodeParameters *parameters) {
	Hero *hero = getHero()->get(parameters->getDword(0) ? parameters->getDword(0) : _currentWork->heroIndex);
	Work *work = parameters->getDword(0) ? hero->work : _currentWork;

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
	parameters->setDword(0, GLOBAL(parameters->getDword(8)));

	return cont(parameters);
}

OpcodeRet WorkManager::contHeroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(parameters->getDword(4), parameters->getDword(8)));

	return cont(parameters);
}

OpcodeRet WorkManager::contVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(_currentWork->heroIndex, parameters->getDword(8)));

	return cont(parameters);
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

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////

Work *WorkManager::get(HeroIndex index) {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++)
		if ((*it)->heroIndex == index)
			return (*it);

	return NULL;
}

Work *WorkManager::getFromTextIndex(int32 index) {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++)
		if ((*it)->workHeroData && (*it)->workHeroData->textIndex == index)
			return (*it);

	return NULL;
}

} // End of namespace Liath
