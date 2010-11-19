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
#include "liath/game/segment.h"
#include "liath/game/work.h"

#include "liath/resource.h"
#include "liath/helpers.h"
#include "liath/liath.h"
#include "liath/shared.h"

namespace Liath {

HeroManager::HeroManager(LiathEngine *engine) : _engine(engine), _storage(NULL), _nHero(0) {
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
OpcodeRet HeroManager::start(OpcodeParameters *parameters, Work **pWork, WorkData *data) {
	_nHero = parameters->getDword(4);

	byte init[4];
	memset(&init, 0, sizeof(init));
	create(get(_nHero), &init[3], &init[2], &init[1], &init[0]);

	Work *work = new Work();
	work->heroIndex = _nHero;
	work->oldHeroData = get(_nHero)->oldData;
	work->field_578 = init[3];
	work->field_579 = 0;
	work->field_55 = init[2] ? 300 : 0;
	work->field_34 = parameters->getDword(74);
	work->field_68 = parameters->getWord(0);
	work->field_76 = parameters->getWord(8);
	// FIXME properly get data
	//work->field_72 = (work->oldHeroData->field_8 + 41);
	work->time = 0;
	work->field_B1 = 10000;
	work->field_F2 = 0;
	work->field_F6 = 0;
	work->field_FA = getGame()->getTimer();
	work->field_FE = getGame()->getTimer();
	work->field_10A = 0;
	work->field_10E = 0xBFF00000;
	work->field_102 = 0;
	work->field_106 = 0xBFF00000;
	work->data.resetField_E00();
	work->data.field_1106 = 0;
	work->field_16BF = 0;

	work->xScroll = 0;
	work->yScroll = GLOBAL(995) >> 16;

	// Copy work data
	// FIXME add copy operator
	if (data) {
		memcpy(&work->data.field_0, data->field_0, sizeof(data->field_0));
		memcpy(&work->data.field_200, data->field_200, sizeof(data->field_200));
		memcpy(&work->data.field_E00, data->field_E00, sizeof(data->field_E00));
		memcpy(&work->data.field_E06, data->field_E06, sizeof(data->field_E06));
		work->data.field_1106 = data->field_1106;
	}

	if (init[0]) {
		error("HeroManager::start: Not implemented!");
	}

	if (work->field_34 & 2 || work->field_34 & 8) {
		work->field_35 = 0;
		work->field_39 = 0;
		work->field_3D = 0;
		work->field_41 = 0;
		work->field_45 = 0;
	} else {
		if (init[1])
			work->field_34 |= 0x20;

		if (parameters->getByte(75) == 1) {
			work->field_41 = parameters->getDword(58);
			work->field_45 = parameters->getDword(66);
			work->field_35 = parameters->getDword(34);
			work->field_39 = parameters->getDword(42);
			work->field_3D = parameters->getDword(50);
			work->field_FA = parameters->getDword(76);
			work->field_FE = parameters->getDword(76);
		} else {
			work->field_35 = EXPR(parameters->getDword(34), parameters->getDword(38));
			work->field_39 = EXPR(parameters->getDword(42), parameters->getDword(46));
			work->field_3D = EXPR(parameters->getDword(50), parameters->getDword(54));
			work->field_41 = EXPR(parameters->getDword(58), parameters->getDword(62));
			work->field_45 = EXPR(parameters->getDword(66), parameters->getDword(70));
		}
	}

	//work->hGlobalHeroData = NULL;
	work->workHeroData = NULL;

	if (work->field_34 & 8) {
		*pWork = work;

		// Setup WorkHeroData
		error("HeroManager::start: Not implemented!");
	}

	get(_nHero)->work = work;

	error("HeroManager::start: Not implemented!");

	// Check sprite count
	// FIXME: wrong data for now
	//Object *object = (Object *)(93 * work->field_68 + get(_nHero)->oldData->field_8);
	//if (object->field_0 < work->field_76)
	//	error("[HeroManager::start] Out of range SPRITE COUNT in the object!");

	////getGame()->readObjectFromDisk()

	//work->object = object;

	/*getGame()->calcBoxObj(work);
	getAction()->inskvant(work);*/

	return kOpcodeRetDefault;
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
void HeroManager::create(Hero *hero, byte *data1, byte *data2, byte *data3, byte *data4) {
	Hero *currentHero = get(hero->index);

	if (!hero->oldData) {
		HeroData *data = currentHero->data;
		if (!data) {
			data = new HeroData();

			// Read hero data
			/*getSegment()->load(kSegmentHero, hero->index);
			data->heroData = (HeroData *)getSegment()->getData(kSegmentHero, hero->index);*/

			/*if (data->heroData->)*/

			warning("HeroManager::remove: Not implemented!");
		}

		currentHero->data = data;
		hero->oldData = currentHero->data;
		++currentHero->field_10;
	}

	++hero->field_14;
}

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
