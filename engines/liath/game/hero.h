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

#ifndef LIATH_HERO_H
#define LIATH_HERO_H

#include "liath/shared.h"

#include "common/array.h"

namespace Liath {

class LiathEngine;

class HeroManager {
public:
	HeroManager(LiathEngine *engine);
	~HeroManager();

	void loadData(uint32 count, size_t size);

	// Opcodes
	OpcodeRet start(OpcodeParameters *parameters, Work *work, void *unkown);
	OpcodeRet startExt(OpcodeParameters *parameters);
	OpcodeRet reset(OpcodeParameters *parameters, void *unkown);
	OpcodeRet exit(OpcodeParameters *parameters);
	OpcodeRet save(OpcodeParameters *parameters);
	OpcodeRet load();
	OpcodeRet passVariable(OpcodeParameters *parameters);
	OpcodeRet freeze(OpcodeParameters *parameters);
	OpcodeRet freezeGlobal(OpcodeParameters *parameters);
	OpcodeRet freezeHeroVar(OpcodeParameters *parameters);
	OpcodeRet freezeVar(OpcodeParameters *parameters);
	OpcodeRet unfreeze(OpcodeParameters *parameters);
	OpcodeRet unfreezeGlobal(OpcodeParameters *parameters);
	OpcodeRet unfreezeHeroVar(OpcodeParameters *parameters);
	OpcodeRet unfreezeVar(OpcodeParameters *parameters);
	OpcodeRet herovar(OpcodeParameters *parameters);
	OpcodeRet herovarGlobal(OpcodeParameters *parameters);
	OpcodeRet herovarHeroVar(OpcodeParameters *parameters);
	OpcodeRet herovarVar(OpcodeParameters *parameters);
	OpcodeRet heroSys(OpcodeParameters *parameters);
	OpcodeRet var(OpcodeParameters *parameters);
	OpcodeRet sys(OpcodeParameters *parameters);
	OpcodeRet quitHero(OpcodeParameters *parameters);
	OpcodeRet quitGlobal(OpcodeParameters *parameters);
	OpcodeRet quitHeroVar(OpcodeParameters *parameters);
	OpcodeRet quitVar(OpcodeParameters *parameters);
	OpcodeRet auto2hero(OpcodeParameters *parameters);
	OpcodeRet auto2heroGlobal(OpcodeParameters *parameters);
	OpcodeRet auto2heroHeroVar(OpcodeParameters *parameters);
	OpcodeRet auto2heroVar(OpcodeParameters *parameters);
	OpcodeRet face2hero(OpcodeParameters *parameters);
	OpcodeRet face2heroGlobal(OpcodeParameters *parameters);
	OpcodeRet face2heroHeroVar(OpcodeParameters *parameters);
	OpcodeRet face2heroVar(OpcodeParameters *parameters);
	OpcodeRet hearGlobal(OpcodeParameters *parameters);
	OpcodeRet hearHeroVar(OpcodeParameters *parameters);
	OpcodeRet hearVar(OpcodeParameters *parameters);

	// Accessors
	Hero *get(HeroIndex index);
	uint32 *getData(HeroIndex index, uint32 offset) { return get(index)->getData(offset); }

private:
#include "common/pack-start.h"
	struct OpcodeParametersHero : OpcodeParameters {
		uint32 param1;
		uint32 param2;
		uint32 param3;
		uint32 param4;
		uint32 param5;
		uint32 param6;
		uint32 param7;
		uint32 param8;
		uint16 param9;
		uint32 param10;
		uint32 param11;
		uint32 param12;
		uint32 param13;
		uint32 param14;
		uint32 param15;
		uint32 param16;
		uint32 param17;
		uint32 param18;
		uint32 param19;
		byte param20;
		byte param21;
		uint32 param22;
	} PACKED_STRUCT;
#include "common/pack-end.h"

	LiathEngine* _engine;

	// Storage
	Common::Array<Hero *> _heros;
	void *_storage;

	// Current saved hero
	OpcodeParametersHero _heroParams;

	// Opcode helpers
	OpcodeRet quit(HeroIndex heroIndex);
	OpcodeRet auto2(OpcodeParameters *parameters);
	OpcodeRet hear(OpcodeParameters *parameters);
};

} // End of namespace Liath

#endif // LIATH_HERO_H.....
