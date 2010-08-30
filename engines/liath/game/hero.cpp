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

#include "liath/game/hero.h"

#include "liath/game/game.h"
#include "liath/game/expression.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

HeroManager::HeroManager(LiathEngine *engine) : _engine(engine), _heroIndex(0) {}

HeroManager::~HeroManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet HeroManager::start(OpcodeParameters *parameters, Work *work, void *unkown) {
	error("HeroManager::start: Not implemented!");
}

OpcodeRet HeroManager::startExt(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	switch (params->field_16) {
	default:
		break;

	case kOriginGlobal:
		params->param2 = *GLOBAL(params->field_1B);
		break;

	case kOriginHero:
		params->param2 = *getData(params->field_17, params->field_1B);
		break;

	case kOriginHeroWork:
		params->param2 = *getData(getWork()->getCurrent()->heroIndex, params->field_1B);
		break;
	}

	switch (params->field_1F) {
	default:
		break;

	case kOriginGlobal:
		params->param = *GLOBAL(params->field_24);
		break;

	case kOriginHero:
		params->param = *getData(params->field_20, params->field_24);
		break;

	case kOriginHeroWork:
		params->param = *getData(getWork()->getCurrent()->heroIndex, params->field_24);
		break;
	}

	return start(params, NULL, NULL);
}

OpcodeRet HeroManager::reset(OpcodeParameters *parameters, void *unkown) {
	error("HeroManager::reset: Not implemented!");
}

OpcodeRet HeroManager::exit(OpcodeParameters *parameters) {
	error("HeroManager::exit: Not implemented!");
}

OpcodeRet HeroManager::save(OpcodeParameters *parameters) {
	error("HeroManager::save_hero: Not implemented!");
}

OpcodeRet HeroManager::load() {
	if (searchHeroForLoad(_heroIndex))
		reset(&_heroParams, NULL);
	else
		start(&_heroParams, NULL, NULL);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::passVariable(OpcodeParameters *parameters) {
	error("HeroManager::passVariable: Not implemented!");
}

OpcodeRet HeroManager::freeze(OpcodeParameters *parameters) {
	error("HeroManager::freeze: Not implemented!");
}

OpcodeRet HeroManager::freezeGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *GLOBAL(params->param3);
	return freeze(params);
}

OpcodeRet HeroManager::freezeHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getData(params->param2, params->param3);
	return freeze(params);
}

OpcodeRet HeroManager::freezeVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getData(getWork()->getCurrent()->heroIndex, params->param3);
	return freeze(params);
}

OpcodeRet HeroManager::unfreeze(OpcodeParameters *parameters) {
	error("HeroManager::unfreeze: Not implemented!");
}

OpcodeRet HeroManager::unfreezeGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *GLOBAL(params->param3);
	return unfreeze(params);
}

OpcodeRet HeroManager::unfreezeHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getData(params->param2, params->param3);
	return unfreeze(params);
}

OpcodeRet HeroManager::unfreezeVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getData(getWork()->getCurrent()->heroIndex, params->param3);
	return unfreeze(params);
}

OpcodeRet HeroManager::herovar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	*getData(params->param, params->param2) = EXPR(params->param4, params->param3);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::herovarGlobal(OpcodeParameters *parameters) {
	error("HeroManager::herovar_glb: Not implemented!");
}

OpcodeRet HeroManager::herovarHeroVar(OpcodeParameters *parameters) {
	error("HeroManager::herovar_hvar: Not implemented!");
}

OpcodeRet HeroManager::herovarVar(OpcodeParameters *parameters) {
	error("HeroManager::herovar_var: Not implemented!");
}

OpcodeRet HeroManager::heroSys(OpcodeParameters *parameters) {
	error("HeroManager::hero_sys: Not implemented!");
}

OpcodeRet HeroManager::var(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	*getData(getWork()->getCurrent()->heroIndex, params->param) = EXPR(params->param3, params->param2);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::sys(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	*getData(getWork()->getCurrent()->heroIndex, params->param) = EXPR(params->param3, params->param2);

	switch (params->param) {
	default:
		break;

	case 12:
		getWork()->getCurrent()->field_57 |= 1;
		break;

	case 13:
		getWork()->getCurrent()->field_57 |= 2;
		break;

	case 2:
		getWork()->getCurrent()->field_57 |= 4;
		break;

	case 14:
		getWork()->getCurrent()->field_57 |= 8;
		break;

	case 0:
		getWork()->getCurrent()->field_57 |= 16;
		break;

	case 1:
		getWork()->getCurrent()->field_57 |= 32;
		break;
	}

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::quitHero(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	return quit(params->param);
}

OpcodeRet HeroManager::quitGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	return quit(*GLOBAL(params->param3));
}

OpcodeRet HeroManager::quitHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	return quit(*getData(params->param2, params->param3));
}

OpcodeRet HeroManager::quitVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	return quit(*getData(getWork()->getCurrent()->heroIndex, params->param3));
}

OpcodeRet HeroManager::auto2hero(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	return RET(auto2(params) == kOpcodeRetNextOffset, params->field_5);
}

OpcodeRet HeroManager::auto2heroGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *GLOBAL(params->param2);
	return auto2(params);
}

OpcodeRet HeroManager::auto2heroHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getData(params->param, params->param2);
	return auto2(params);
}

OpcodeRet HeroManager::auto2heroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getData(getWork()->getCurrent()->heroIndex, params->param2);
	return auto2(params);
}

OpcodeRet HeroManager::face2hero(OpcodeParameters *parameters) {
	error("HeroManager::face2hero: Not implemented!");
}

OpcodeRet HeroManager::face2heroGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *GLOBAL(params->param2);

	if (face2hero(params) != -1)
		*GLOBAL(params->param2) = -1;

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::face2heroHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	uint32 param = params->param;

	params->param = *getData(params->param, params->param2);

	if (face2hero(params) != -1)
		*GLOBAL(params->param2) = -1;

	params->param = param;

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::face2heroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param = *getData(getWork()->getCurrent()->heroIndex, params->param2);

	if (face2hero(params) != -1)
		*GLOBAL(params->param2) = -1;

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::hearGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	OpcodeRet ret = hear(params);

	*GLOBAL(params->param2) = ret;

	if (ret != kOpcodeRetNextOffset)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(params->field_5 ? ret : -ret);
}

OpcodeRet HeroManager::hearHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	OpcodeRet ret = hear(params);

	*getData(params->param, params->param2) = ret;

	if (ret != kOpcodeRetNextOffset)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(params->field_5 ? ret : -ret);
}

OpcodeRet HeroManager::hearVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	OpcodeRet ret = hear(params);

	*getData(getWork()->getCurrent()->heroIndex, params->param2) = ret;

	if (ret != kOpcodeRetNextOffset)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(params->field_5 ? ret : -ret);
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////

uint32 *HeroManager::getData(HeroIndex index, uint32 offset) {
	error("HeroManager::getData: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////

OpcodeRet HeroManager::quit(HeroIndex heroIndex) {
	error("HeroManager::quit: Not implemented!");
}

OpcodeRet HeroManager::auto2(OpcodeParameters *parameters) {
	error("HeroManager::auto2: Not implemented!");
}

OpcodeRet HeroManager::hear(OpcodeParameters *parameters) {
	error("HeroManager::auto2: Not implemented!");
}

Work *HeroManager::searchHeroForSave(HeroIndex index) {
	error("WorkManager::searchHeroForSave: not implemented!");
}

bool HeroManager::searchHeroForLoad(HeroIndex index) {
	error("WorkManager::searchHeroForLoad: not implemented!");
}

} // End of namespace Liath
