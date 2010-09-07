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

#include "liath/game/progress.h"

#include "liath/game/game.h"

#include "liath/helpers.h"
#include "liath/liath.h"
#include "liath/resource.h"

#include "common/rational.h"

namespace Liath {

ProgressManager::ProgressManager(LiathEngine *engine) : _engine(engine) {
	memset(&_data, 0, 11 * sizeof(int16));
}

ProgressManager::~ProgressManager() {
	// Zero-out passed pointers
	_engine = NULL;

	CLEAR_ARRAY(Progress, _progress);
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet ProgressManager::init(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersWWWW);

	debugC(kLiathDebugInterpreter, "  count: %d - data = [%d, 0, %d, %d, %d, 0, %d, %d, %d, 0, 0]\n",
		   params->param1, params->param3, params->param5, params->param7, params->param9,
		   params->param11, params->param13, params->param15);

	// Clear previous array
	CLEAR_ARRAY(Progress, _progress);

	// Number of progress instances to create
	uint16 count = params->param1;
	for (int i = 0; i < count; i++)
		_progress.push_back(new Progress());

	// Initialize global progress
	_data[0] = params->param3;
	_data[2] = params->param5;
	_data[3] = params->param7;
	_data[4] = params->param9;
	_data[6] = params->param11;
	_data[7] = params->param13;
	_data[8] = params->param15;
	_data[9] = 0;
	_data[10] = 0;

	return kOpcodeRetDefault;
}

OpcodeRet ProgressManager::set(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersWWWW);

	if (_progress.size() == 0)
		return kOpcodeRetDefault;

	int16 index = params->param1;
	if (index < 0 || (uint32)index >= _progress.size() || _progress[index]->field_0)
		return kOpcodeRetDefault;

	_data[9] += _data[0];
	++_data[10];

	_progress[index]->field_0 = 1;

	return kOpcodeRetDefault;
}


OpcodeRet ProgressManager::get(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersBDDB);

	if (_progress.size() == 0)
		return kOpcodeRetDefault;

	getGame()->letValue((ParamOrigin)params->param1, params->param2, params->param3, INT2DSI(_data[9]));
	getGame()->letValue((ParamOrigin)params->param4, params->param5, params->param6, INT2DSI(Common::Rational(51 * _data[10], _progress.size()).toInt()));

	return kOpcodeRetDefault;
}

OpcodeRet ProgressManager::help(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersWWBD);

	if (_progress.size() == 0 || params->param1 < 1 || params->param1 > 3) {
		getGame()->letValue((ParamOrigin)params->param3, params->param4, params->param5, INT2DSI(0));
		return kOpcodeRetDefault;
	}

	// Get index of first empty progress entry
	uint32 index;
	for (index = 0; index < _progress.size() && _progress[index]->field_0; ++index);

	Message message;
	uint32 messageIndex = _data[params->param1] + index;
	if (index == _progress.size() || !getResource()->readMessage(&messageIndex, &message)) {
		getGame()->letValue((ParamOrigin)params->param3, params->param4, params->param5, INT2DSI(0));
		return kOpcodeRetDefault;
	}

	if (!message.field_B1)
		_data[9] += _data[params->param1] * _progress[index]->getData(params->param1);

	_progress[index]->setData(params->param1, 0);

	getGame()->letValue((ParamOrigin)params->param3, params->param4, params->param5, INT2DSI(messageIndex));

	return kOpcodeRetDefault;
}

} // End of namespace Liath
