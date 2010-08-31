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

#include "liath/game/expression.h"

#include "liath/game/action.h"
#include "liath/game/game.h"
#include "liath/game/hero.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

ExpressionManager::ExpressionManager(LiathEngine *engine) : _engine(engine) {}

ExpressionManager::~ExpressionManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet ExpressionManager::expression(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	int expr = expression(params->param2, params->param);

	bool ret = params->field_5 ? (bool)expr : (expr < 1);

	if (!ret)
		return kOpcodeRetNextOffset;

	return kOpcodeRetDefault;
}

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////
int32 ExpressionManager::expression(uint32 offset, uint32 count) {
	if (!offset)
		error("ExpressionManager::expression: Invalid offset!");

	if (!count)
		error("ExpressionManager::expression: Invalid count!");

	// Get the expression array
	Expression *expressions = (Expression *)SEGMENTDATA(_expressionSegment, offset);
	Work *work = getWork()->getCurrent();

	for (uint index = 0; index < count; index++) {
		Expression expr = expressions[index];

		switch (expr.operation) {
		default:
			error("ExpressionManager::expression: Invalid operation!");

		case kOperationHeroWorkVar:
			push(*getHero()->getData(work->heroIndex, expr.field_5));
			break;

		case kOperationGlobalVar:
			push(*GLOBAL(expr.field_5));
			break;

		case kOperationHeroVar:
			push(*getHero()->getData(expr.field_1, expr.field_5));
			break;

		case kOperation4:
			switch (expr.field_5) {
			default:
				push(*getHero()->getData(work->heroIndex, expr.field_5));
				break;

			case 0:
				push(work->field_35);
				break;

			case 1:
				push(work->field_39);
				break;

			case 2:
				push(work->field_3D);
				break;

			case 3:
				push(getAction()->getCurrentAction());
				break;

			case 4:
				push(work->field_68);
				break;

			case 5:
				push(work->field_76);
				break;

			case 7:
				push(work->status);
				break;

			case 11:
				push(work->field_DC);
				break;

			case 12:
				push(work->field_41);
				break;

			case 13:
				push(work->field_45);
				break;

			case 14:
				push(SCR2CEL(work->object->field_41));
				break;
			}
			break;

		case kOperation5: {
			Hero *hero = getHero()->get(expr.field_1);

			if (hero->work) {
				switch (expr.field_5) {
				default:
					push(*hero->getData(expr.field_5));
					break;

				case 0:
					push(SCR2CEL(LOWORD(hero->work->field_35)));
					break;

				case 1:
					push(SCR2CEL(LOWORD(hero->work->field_39)));
					break;

				case 2:
					push(hero->work->field_3D);
					break;

				case 3:
					push(getAction()->getCurrentAction());
					break;

				case 4:
					push(hero->work->field_68);
					break;

				case 5:
					push(hero->work->field_76);
					break;

				case 7:
					push(hero->work->status);
					break;

				case 11:
					push(hero->work->field_DC);
					break;

				case 12:
					push(hero->work->field_41);
					break;

				case 13:
					push(hero->work->field_45);
					break;

				case 14:
					push(SCR2CEL(hero->work->object->field_41));
					break;
				}
			} else {
				push(*hero->getData(expr.field_5));
			}
			break;
		}

		case kOperation6:
			push(expr.field_5);
			break;

		case kOperation11:
		case kOperation12:
		case kOperation13: {
			Hero *hero;
			if (expr.operation == kOperation11) {
				hero = getHero()->get(*GLOBAL(expr.field_5));
			} else if (expr.operation == kOperation12) {
				hero = getHero()->get(*getHero()->getData(expr.field_1, expr.field_5));
			} else {
				hero = getHero()->get(*getHero()->getData(work->heroIndex, expr.field_5));
			}

			if (expr.field_9 < 14 && hero->work) {
				switch (expr.field_9) {
				default:
					push(*hero->getData(expr.field_9));
					break;

				case 0:
					push(hero->work->field_35);
					break;

				case 1:
					push(hero->work->field_39);
					break;

				case 2:
					push(hero->work->field_3D);
					break;

				case 3:
					push(getAction()->getCurrentAction());
					break;

				case 4:
					push(hero->work->field_68);
					break;

				case 5:
					push(hero->work->field_76);
					break;

				case 7:
					push(hero->work->status);
					break;

				case 11:
					push(hero->work->field_DC);
					break;

				case 12:
					push(hero->work->field_41);
					break;

				case 13:
					push(hero->work->field_45);
					break;

				case 14:
					push(SCR2CEL(hero->work->object->field_41));
					break;
				}
			} else {
				push(*hero->getData(expr.field_9));
			}
			break;
		}

		case kOperationPlus:
			push(pop() + pop());
			break;

		case kOperationMinus: {
			int32 val1 = pop();
			int32 val2 = pop();
			push(val2 - val1);
			break;
		}

		case kOperationMultiply:
			push(pop() * pop());
			break;

		case kOperationDivide: {
			int32 val1 = pop();
			if (val1) {
				push(pop() / val1);
			} else
				error("ExpressionManager::expression: Division by 0!");
			break;
		}

		case kOperationEqual:
			push(pop() == pop());
			break;

		case kOperationInferior: {
			int32 val1 = pop();
			int32 val2 = pop();
			push(val1 < val2);
			break;
		}

		case kOperationSuperior: {
			int32 val1 = pop();
			int32 val2 = pop();
			push(val1 > val2);
			break;
		}

		case kOperationInferiorOrEqual: {
			int32 val1 = pop();
			int32 val2 = pop();
			push(val1 <= val2);
			break;
		}

		case kOperationSuperiorOrEqual: {
			int32 val1 = pop();
			int32 val2 = pop();
			push(val1 >= val2);
			break;
		}

		case kOperationDifferent:
			push(pop() != pop());
			break;

		case kOperationInvert:
			push(-pop());
			break;
		}
	}

	return _queue.pop();
}


} // End of namespace Liath
