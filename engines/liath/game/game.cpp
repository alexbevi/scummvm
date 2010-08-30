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

#include "liath/game/expression.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

GameManager::GameManager(LiathEngine *engine) : _engine(engine) {}

GameManager::~GameManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet GameManager::rnd(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	uint32 val = (params->param ? _engine->getRandom().getRandomNumber(params->param): 0);

	return getReturnValue(val + 1 == params->param, params->field_5);
}

OpcodeRet GameManager::global(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	*getGlobal(params->param) = EXPR(params->param3, params->param2);

	return kOpcodeRetDefault;
}

OpcodeRet GameManager::key(OpcodeParameters *parameters) {
	error("Play::key: Not implemented!");
}

OpcodeRet GameManager::bkey(OpcodeParameters *parameters) {
	error("Play::bkey: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Helper Functions
//////////////////////////////////////////////////////////////////////////

int *GameManager::getGlobal(uint32 offset) {
	error("Play::getGlobal: Not implemented!");
}

OpcodeRet GameManager::getReturnValue(int val, bool testValue) {
	if (val)
		return (testValue ? kOpcodeRetDefault : kOpcodeRetNextOffset);
	else
		return (testValue ? kOpcodeRetNextOffset : kOpcodeRetDefault);
}

} // End of namespace Liath
