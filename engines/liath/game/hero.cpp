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

#include "liath/resource.h"
#include "liath/helpers.h"
#include "liath/liath.h"

#include "common/stream.h"

namespace Liath {

HeroManager::HeroManager(LiathEngine *engine) : _engine(engine), _heroIndex(0), _storage(NULL) {}

HeroManager::~HeroManager() {
	// Zero-out passed pointers
	_engine = NULL;

	// Clear hero storage
	for (Common::Array<Hero *>::iterator it = _heros.begin(); it != _heros.end(); it++)
		SAFE_DELETE(*it);

	_heros.clear();

	if (_storage)
		free(_storage);
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void HeroManager::loadData(uint32 count, size_t varSize) {
	// Open hero data file
	Common::SeekableReadStream *stream = getResource()->createReadStreamForMember("game0001.dat");
	if (!stream)
		error("HeroManager::loadData: Invalid hero data stream!");

	// Go to the hero data
	stream->seek(stream->size() - 38 * count, SEEK_SET);

	// Setup hero data area
	for (uint32 i = 0; i <= count + 1; i++) {
		Hero *hero = new Hero();

		if (i > 0)
			stream->read(hero, 38);

		_heros.push_back(hero);
	}

	// ALlocate storage for hero variables
	_storage = calloc((count + 1) * varSize, 1);

	delete stream;

	// Setup hero objects
	for (uint i = 0; i < count + 1; ++i)
		get(i)->object = (HeroObject *)((byte *)_storage + i * varSize);
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////

Hero *HeroManager::get(HeroIndex index) {
	if (index >= _heros.size())
		error("Hero::get: Invalid index (was:%d, max:%d)", index, _heros.size() - 1);

	return _heros[index];
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
		params->param1 = *GLOBAL(params->field_24);
		break;

	case kOriginHero:
		params->param1 = *getData(params->field_20, params->field_24);
		break;

	case kOriginHeroWork:
		params->param1 = *getData(getWork()->getCurrent()->heroIndex, params->field_24);
		break;
	}

	return start((OpcodeParameters *)params, NULL, NULL);
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

	params->param1 = *GLOBAL(params->param3);
	return freeze((OpcodeParameters *)params);
}

OpcodeRet HeroManager::freezeHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *getData(params->param2, params->param3);
	return freeze((OpcodeParameters *)params);
}

OpcodeRet HeroManager::freezeVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *getData(getWork()->getCurrent()->heroIndex, params->param3);
	return freeze((OpcodeParameters *)params);
}

OpcodeRet HeroManager::unfreeze(OpcodeParameters *parameters) {
	error("HeroManager::unfreeze: Not implemented!");
}

OpcodeRet HeroManager::unfreezeGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *GLOBAL(params->param3);
	return unfreeze((OpcodeParameters *)params);
}

OpcodeRet HeroManager::unfreezeHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *getData(params->param2, params->param3);
	return unfreeze((OpcodeParameters *)params);
}

OpcodeRet HeroManager::unfreezeVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *getData(getWork()->getCurrent()->heroIndex, params->param3);
	return unfreeze((OpcodeParameters *)params);
}

OpcodeRet HeroManager::herovar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	debugC(kLiathDebugInterpreter, "  HeroIndex: %d / Offset: %d  -  Expression: %d - Count: %d\n", params->param1, params->param2, params->param4, params->param3);

	*getData(params->param1, params->param2) = EXPR(params->param4, params->param3);

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

	*getData(getWork()->getCurrent()->heroIndex, params->param1) = EXPR(params->param3, params->param2);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::sys(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	*getData(getWork()->getCurrent()->heroIndex, params->param1) = EXPR(params->param3, params->param2);

	switch (params->param1) {
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

	return quit(params->param1);
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

	return RET(auto2((OpcodeParameters *)params) == kOpcodeRetNextOffset, params->field_5);
}

OpcodeRet HeroManager::auto2heroGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *GLOBAL(params->param2);
	return auto2((OpcodeParameters *)params);
}

OpcodeRet HeroManager::auto2heroHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *getData(params->param1, params->param2);
	return auto2((OpcodeParameters *)params);
}

OpcodeRet HeroManager::auto2heroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *getData(getWork()->getCurrent()->heroIndex, params->param2);
	return auto2((OpcodeParameters *)params);
}

OpcodeRet HeroManager::face2hero(OpcodeParameters *parameters) {
	error("HeroManager::face2hero: Not implemented!");
}

OpcodeRet HeroManager::face2heroGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *GLOBAL(params->param2);

	if (face2hero((OpcodeParameters *)params) != -1)
		*GLOBAL(params->param2) = -1;

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::face2heroHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	uint32 param = params->param1;

	params->param1 = *getData(params->param1, params->param2);

	if (face2hero((OpcodeParameters *)params) != -1)
		*GLOBAL(params->param2) = -1;

	params->param1 = param;

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::face2heroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	params->param1 = *getData(getWork()->getCurrent()->heroIndex, params->param2);

	if (face2hero((OpcodeParameters *)params) != -1)
		*GLOBAL(params->param2) = -1;

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::hearGlobal(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	OpcodeRet ret = hear((OpcodeParameters *)params);

	*GLOBAL(params->param2) = ret;

	if (ret != kOpcodeRetNextOffset)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(params->field_5 ? ret : -ret);
}

OpcodeRet HeroManager::hearHeroVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	OpcodeRet ret = hear((OpcodeParameters *)params);

	*getData(params->param1, params->param2) = ret;

	if (ret != kOpcodeRetNextOffset)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(params->field_5 ? ret : -ret);
}

OpcodeRet HeroManager::hearVar(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	OpcodeRet ret = hear((OpcodeParameters *)params);

	*getData(getWork()->getCurrent()->heroIndex, params->param2) = ret;

	if (ret != kOpcodeRetNextOffset)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(params->field_5 ? ret : -ret);
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
