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

#ifndef LIATH_EXPRESSION_H
#define LIATH_EXPRESSION_H

#include "liath/shared.h"

#include "common/queue.h"

namespace Liath {

class LiathEngine;

class ExpressionManager {
public:

	ExpressionManager(LiathEngine *engine);
	~ExpressionManager();

	// Opcodes
	OpcodeRet expression(OpcodeParameters *parameters);

	int32 expression(uint32 offset, uint32 count);

private:
	enum Operation {
		kOperationHeroWorkVar = 1,
		kOperationGlobalVar = 2,
		kOperationHeroVar = 3,
		kOperation4 = 4,
		kOperation5 = 5,
		kOperation6 = 6,
		kOperation11 = 11,
		kOperation12 = 12,
		kOperation13 = 13,
		kOperationPlus = 16,
		kOperationMinus = 17,
		kOperationMultiply = 18,
		kOperationDivide = 19,
		kOperationEqual = 20,
		kOperationInferior = 21,
		kOperationSuperior = 22,
		kOperationInferiorOrEqual = 23,
		kOperationSuperiorOrEqual = 24,
		kOperationDifferent = 25,
		kOperationInvert = 26,
	};

	struct Expression {
		Operation operation;
		uint32 field_1;
		uint32 field_5;
		uint32 field_9;
	};

	LiathEngine* _engine;

	Segment _expressionSegment;
	Common::Queue<int32> _queue;

	// Helpers for cleaner looking code
	void push(int32 val) { _queue.push(val); }
	int32 pop() { return _queue.pop(); }
};

} // End of namespace Liath

#endif // LIATH_EXPRESSION_H
