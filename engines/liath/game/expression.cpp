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

int ExpressionManager::expression(int offset, int count) {
	error("ExpressionManager::expression: Not implemented!");
}


} // End of namespace Liath
