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

#include "liath/game/position.h"

#include "liath/game/expression.h"
#include "liath/game/game.h"
#include "liath/game/hero.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

// Used when calculating direction in feel opcode
const uint32 feelData[20] = { 0, 0, 0, 0, 1, 4, 2, 3, 2, 1, 3, 4, 3, 2, 4, 1, 4, 3, 1, 2};

PositionManager::PositionManager(LiathEngine *engine) : _engine(engine) {}

PositionManager::~PositionManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet PositionManager::checkLimits(OpcodeParameters *parameters) {
	uint32 a2, a3;
	bool limit = checkLimit(parameters->objectIndex, &a2, &a3);

	getGame()->letValue((ParamOrigin)parameters->getByte(0), parameters->getDword(2), parameters->getDword(6), a2);
	getGame()->letValue((ParamOrigin)parameters->getByte(1), parameters->getDword(10), parameters->getDword(14), a2);

	return RET(limit, parameters->test);
}

OpcodeRet PositionManager::autoStart(OpcodeParameters *parameters) {
	error("PositionManager::auto_start: Not implemented!");
}

OpcodeRet PositionManager::autoCheck(OpcodeParameters *parameters) {
	error("PositionManager::auto_check: Not implemented!");
}

OpcodeRet PositionManager::autoStop(OpcodeParameters *parameters) {
	error("PositionManager::auto_stop: Not implemented!");
}

OpcodeRet PositionManager::pers(OpcodeParameters *parameters) {
	HeroIndex index = checkPers(parameters);

	getGame()->letValue((ParamOrigin)parameters->getByte(0), parameters->getDword(1), parameters->getDword(5), index);

	return RET(index, parameters->test);
}

OpcodeRet PositionManager::hpers(OpcodeParameters *parameters) {
	HeroIndex index = (HeroIndex)getGame()->getValue((ParamOrigin)parameters->getByte(0), parameters->getDword(1), parameters->getDword(5));

	uint32 val = EXPR(parameters->getDword(9), parameters->getDword(13));

	return RET(checkHPers(index, val), parameters->test);
}

OpcodeRet PositionManager::dist(OpcodeParameters *parameters) {
	HeroIndex heroIndex = (HeroIndex)getGame()->getValue((ParamOrigin)parameters->getByte(0), parameters->getDword(3), parameters->getDword(7));

	int32 expr = EXPR(parameters->getDword(27), parameters->getDword(31));
	int32 distance = calcDist(getHero()->get(heroIndex)->work, &expr);

	getGame()->letValue((ParamOrigin)parameters->getByte(1), parameters->getDword(3), parameters->getDword(15), distance);
	getGame()->letValue((ParamOrigin)parameters->getByte(2), parameters->getDword(19), parameters->getDword(23), expr);

	if (distance != -1 && distance > EXPR(parameters->getDword(35), parameters->getDword(39)))
		distance = -1;

	return RET(distance == -1, parameters->test);
}

OpcodeRet PositionManager::feel(OpcodeParameters *parameters) {
	FeelResult result;

	feeler(EXPR(parameters->getDword(43), parameters->getDword(47)),
	       EXPR(parameters->getDword(35), parameters->getDword(39)),
	       EXPR(parameters->getDword(27), parameters->getDword(31)),
	       &result);

	getGame()->letValue((ParamOrigin)parameters->getByte(0), parameters->getDword(3), parameters->getDword(7), result.res1);
	getGame()->letValue((ParamOrigin)parameters->getByte(1), parameters->getDword(11), parameters->getDword(15), result.res2);
	getGame()->letValue((ParamOrigin)parameters->getByte(2), parameters->getDword(19), parameters->getDword(23), result.res3);

	return RET(result.res1 == -1, parameters->test);
}

OpcodeRet PositionManager::barrier(OpcodeParameters *parameters) {
	error("PositionManager::barrier: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Helper methods
//////////////////////////////////////////////////////////////////////////
bool PositionManager::checkLimit(ObjectIndex objectIndex, uint32 *a2, uint32 *a3) {
	*a2 = 0;
	*a3 = 0;

	// BUG: The rest seems to be dead code in the original

	return false;
}

void PositionManager::feeler(int32 expr1, int32 expr2, int32 expr3, FeelResult *data) {
	// We already have set the values to -1 in FeelData constructor

	for (uint i = 0; i < 4; i++) {
		int32 res = calcDir(feelData[expr2 + i], expr1, data);

		if (res != -1 && res <= expr3)
			break;
	}
}

int32 PositionManager::calcDir(uint32 val, int32 expr1, FeelResult *result) {
	error("PositionManager::calcDir: Not implemented!");
}

HeroIndex PositionManager::checkPers(OpcodeParameters *parameters) {
	error("PositionManager::checkPers: Not implemented!");
}

int32 PositionManager::calcDist(Work *work, int32 *expr) {
	if (checkDistPers(work)) {
		*expr = 100;
		return 0;
	}

	error("PositionManager::checkPers: Not implemented!");
}

bool PositionManager::checkDistPers(Work *work) {
	Work *current = getWork()->getCurrent();

	if (((current->field_A9 < work->field_A9 || current->field_A9 > work->field_AB) && (current->field_AB < work->field_A9 || current->field_AB > work->field_AB))
	 || ((current->field_AD < work->field_AD || current->field_AD > work->field_AF) && (current->field_AF < work->field_AD || current->field_AF > work->field_AF)))
		return false;

	return true;
}

bool PositionManager::checkHPers(HeroIndex heroIndex, int32 expr) {
	Work* work = getHero()->get(heroIndex)->work;

	// Check variables
	uint32 test1 = 0;
	uint32 test2 = 0;
	uint32 test3 = 0;
	uint32 test4 = 0;

	switch (expr) {
	default:
		return true;

	case 1:
		test1 = getWork()->getCurrent()->field_AB;
		test2 = getWork()->getCurrent()->field_A9;
		test3 = test4 = getWork()->getCurrent()->field_AF;
		break;

	case 2:
		test1 = test2 = getWork()->getCurrent()->field_AB;
		test3 = getWork()->getCurrent()->field_AF;
		test4 = getWork()->getCurrent()->field_AD;
		break;

	case 3:
		test1 = getWork()->getCurrent()->field_AB;
		test2 = getWork()->getCurrent()->field_A9;
		test3 = test4 = getWork()->getCurrent()->field_AD;
		break;

	case 4:
		test1 = test2 = getWork()->getCurrent()->field_A9;
		test3 = getWork()->getCurrent()->field_AF;
		test4 = getWork()->getCurrent()->field_AD;
		break;
	}

	if (work->field_A9 <= test1
	 && work->field_AB >= test2
	 && work->field_AD <= test3)
		return (work->field_AF >= test4);

	return false;
}

} // End of namespace Liath
