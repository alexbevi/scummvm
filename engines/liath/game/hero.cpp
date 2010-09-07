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

namespace Liath {

HeroManager::HeroManager(LiathEngine *engine) : _engine(engine), _storage(NULL) {
	memset(&_heroParams, 0, sizeof(_heroParams));
}

HeroManager::~HeroManager() {
	// Zero-out passed pointers
	_engine = NULL;

	// Clear hero storage
	CLEAR_ARRAY(Hero, _heros);

	SAFE_FREE(_storage);
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void HeroManager::load(uint32 count, size_t varSize) {
	// Open hero data file
	Common::SeekableReadStream *stream = getResource()->createReadStreamForMember("game0001.dat");
	if (!stream)
		error("HeroManager::loadData: Invalid game data stream!");

	// Go to the hero data
	stream->seek(stream->size() - 38 * count, SEEK_SET);

	// Setup hero data area
	for (uint32 i = 0; i <= count + 1; i++) {
		Hero *hero = new Hero();

		if (i > 0)
			stream->read(hero, 38);

		_heros.push_back(hero);
	}

	delete stream;

	// ALlocate storage for hero variables
	_storage = calloc((count + 1) * varSize, 1);

	// Setup hero objects
	for (uint i = 0; i < count + 1; ++i)
		get(i)->object = (HeroObject *)((byte *)_storage + i * varSize);
}

void HeroManager::unload() {
	for (uint index = 1; index < _heros.size(); index++)
		remove(index);
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

	parameters->setDword(parameters->getDword(4),
	                     getGame()->getValue((ParamOrigin)parameters->getByte(16), parameters->getDword(17), parameters->getDword(21), false, false));

	parameters->setDword(parameters->getDword(0),
	                     getGame()->getValue((ParamOrigin)parameters->getByte(25), parameters->getDword(26), parameters->getDword(30), false, false));

	return start(parameters, NULL, NULL);
}

OpcodeRet HeroManager::reset(OpcodeParameters *parameters, void *unkown) {
	error("HeroManager::reset: Not implemented!");
}

OpcodeRet HeroManager::exit(OpcodeParameters *parameters) {
	error("HeroManager::exit: Not implemented!");
}

OpcodeRet HeroManager::save(OpcodeParameters *parameters) {
	Work *work = getWork()->get(parameters->getDword(0));

	if (work->field_C)
		return kOpcodeRetDefault;

	// Update hero params
	_heroParams.setDword(0, work->field_68);
	_heroParams.setDword(4, work->heroIndex);
	_heroParams.setDword(8, work->field_76);
	_heroParams.setDword(34, work->field_35);
	_heroParams.setDword(42, work->field_39);
	_heroParams.setDword(50, work->field_3D);
	_heroParams.setDword(58, work->field_41);
	_heroParams.setDword(66, work->field_45);
	_heroParams.setByte(74, work->field_34);
	_heroParams.setDword(75, 1);
	_heroParams.setDword(76, work->field_FA);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::load() {
	if (getWork()->get(_heroParams.getDword(4)))
		reset((OpcodeParameters *)&_heroParams, NULL);
	else
		start((OpcodeParameters *)&_heroParams, NULL, NULL);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::passVariable(OpcodeParameters *parameters) {
	error("HeroManager::passVariable: Not implemented!");
}

OpcodeRet HeroManager::freeze(OpcodeParameters *parameters) {
	error("HeroManager::freeze: Not implemented!");
}

OpcodeRet HeroManager::freezeGlobal(OpcodeParameters *parameters) {
	parameters->setDword(0, GLOBAL(parameters->getDword(8)));

	return freeze(parameters);
}

OpcodeRet HeroManager::freezeHeroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getData(parameters->getDword(4), parameters->getDword(8)));

	return freeze(parameters);
}

OpcodeRet HeroManager::freezeVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getData(getWork()->getCurrent()->heroIndex, parameters->getDword(8)));

	return freeze(parameters);
}

OpcodeRet HeroManager::unfreeze(OpcodeParameters *parameters) {
	error("HeroManager::unfreeze: Not implemented!");
}

OpcodeRet HeroManager::unfreezeGlobal(OpcodeParameters *parameters) {
	parameters->setDword(0, GLOBAL(parameters->getDword(8)));

	return unfreeze(parameters);
}

OpcodeRet HeroManager::unfreezeHeroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getData(parameters->getDword(4), parameters->getDword(8)));

	return unfreeze(parameters);
}

OpcodeRet HeroManager::unfreezeVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getData(getWork()->getCurrent()->heroIndex, parameters->getDword(8)));

	return unfreeze(parameters);
}

OpcodeRet HeroManager::herovar(OpcodeParameters *parameters) {
	debugC(kLiathDebugInterpreter, "  heroIndex: %d / offset: %d  -  expression: %d - count: %d\n",
	       parameters->getDword(0), parameters->getDword(4), parameters->getDword(12), parameters->getDword(8));

	setData(parameters->getDword(0), parameters->getDword(4), EXPR(parameters->getDword(12), parameters->getDword(8)));

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
	setData(getWork()->getCurrent()->heroIndex, parameters->getDword(0), EXPR(parameters->getDword(8), parameters->getDword(4)));

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::sys(OpcodeParameters *parameters) {
	setData(getWork()->getCurrent()->heroIndex, parameters->getDword(0), EXPR(parameters->getDword(8), parameters->getDword(4)));

	switch (parameters->getDword(0)) {
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
	return quit(parameters->getDword(0));
}

OpcodeRet HeroManager::quitGlobal(OpcodeParameters *parameters) {
	return quit(GLOBAL(parameters->getDword(8)));
}

OpcodeRet HeroManager::quitHeroVar(OpcodeParameters *parameters) {
	return quit(getData(parameters->getDword(4), parameters->getDword(8)));
}

OpcodeRet HeroManager::quitVar(OpcodeParameters *parameters) {
	return quit(getData(getWork()->getCurrent()->heroIndex, parameters->getDword(8)));
}

OpcodeRet HeroManager::auto2hero(OpcodeParameters *parameters) {
	return RET(auto2(parameters) == kOpcodeRetNext, parameters->test);
}

OpcodeRet HeroManager::auto2heroGlobal(OpcodeParameters *parameters) {
	parameters->setDword(0, GLOBAL(parameters->getDword(4)));

	return RET(auto2(parameters) == kOpcodeRetNext, parameters->test);
}

OpcodeRet HeroManager::auto2heroHeroVar(OpcodeParameters *parameters) {
	uint32 param = parameters->getDword(0);
	parameters->setDword(0, getData(parameters->getDword(0), parameters->getDword(4)));

	OpcodeRet ret = auto2hero(parameters);
	parameters->setDword(0, param);

	return RET(ret == kOpcodeRetNext, parameters->test);
}

OpcodeRet HeroManager::auto2heroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getData(getWork()->getCurrent()->heroIndex, parameters->getDword(4)));

	return RET(auto2(parameters) == kOpcodeRetNext, parameters->test);
}

OpcodeRet HeroManager::face2hero(OpcodeParameters *parameters) {
	error("HeroManager::face2hero: Not implemented!");
}

OpcodeRet HeroManager::face2heroGlobal(OpcodeParameters *parameters) {
	parameters->setDword(0, GLOBAL(parameters->getDword(4)));

	if (face2hero(parameters) != kOpcodeRetNext)
		SETGLOBAL(parameters->getDword(4), -1);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::face2heroHeroVar(OpcodeParameters *parameters) {
	uint32 param = parameters->getDword(0);

	parameters->setDword(0, getData(parameters->getDword(0), parameters->getDword(4)));

	if (face2hero(parameters) != kOpcodeRetNext)
		setData(param, parameters->getDword(4), -1);

	parameters->setDword(0, param);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::face2heroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getData(getWork()->getCurrent()->heroIndex, parameters->getDword(4)));

	if (face2hero(parameters) != kOpcodeRetNext)
		setData(getWork()->getCurrent()->heroIndex, parameters->getDword(4), -1);

	return kOpcodeRetDefault;
}

OpcodeRet HeroManager::hearGlobal(OpcodeParameters *parameters) {
	OpcodeRet ret = hear(parameters);

	SETGLOBAL(parameters->getDword(4),  ret);

	if (ret != kOpcodeRetNext)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(parameters->test ? ret : -ret);
}

OpcodeRet HeroManager::hearHeroVar(OpcodeParameters *parameters) {
	OpcodeRet ret = hear(parameters);

	setData(parameters->getDword(0), parameters->getDword(4), ret);

	if (ret != kOpcodeRetNext)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(parameters->test ? ret : -ret);
}

OpcodeRet HeroManager::hearVar(OpcodeParameters *parameters) {
	OpcodeRet ret = hear(parameters);

	setData(getWork()->getCurrent()->heroIndex, parameters->getDword(4), ret);

	if (ret != kOpcodeRetNext)
		ret = kOpcodeRetDefault;

	return (OpcodeRet)(parameters->test ? ret : -ret);
}

//////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////

void HeroManager::remove(HeroIndex index) {
	error("HeroManager::remove: Not implemented!");
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

} // End of namespace Liath
