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
	debugC(kLiathDebugInterpreter, "  count: %d - data = [%d, 0, %d, %d, %d, 0, %d, %d, %d, 0, 0]\n",
		   parameters->getWord(0), parameters->getWord(4), parameters->getWord(8), parameters->getWord(12),
		   parameters->getWord(16), parameters->getWord(20), parameters->getWord(24), parameters->getWord(28));

	// Clear previous array
	CLEAR_ARRAY(Progress, _progress);

	// Number of progress instances to create
	uint16 count = parameters->getWord(0);
	for (int i = 0; i < count; i++)
		_progress.push_back(new Progress());

	// Initialize global progress
	_data[0] = parameters->getWord(4);
	_data[2] = parameters->getWord(8);
	_data[3] = parameters->getWord(12);
	_data[4] = parameters->getWord(16);
	_data[6] = parameters->getWord(20);
	_data[7] = parameters->getWord(24);
	_data[8] = parameters->getWord(28);
	_data[9] = 0;
	_data[10] = 0;

	return kOpcodeRetDefault;
}

OpcodeRet ProgressManager::set(OpcodeParameters *parameters) {
	if (_progress.size() == 0)
		return kOpcodeRetDefault;

	int16 index = parameters->getWord(0);
	if (index < 0 || (uint32)index >= _progress.size() || _progress[index]->field_0)
		return kOpcodeRetDefault;

	_data[9] += _data[0];
	++_data[10];

	_progress[index]->field_0 = 1;

	return kOpcodeRetDefault;
}


OpcodeRet ProgressManager::get(OpcodeParameters *parameters) {
	if (_progress.size() == 0)
		return kOpcodeRetDefault;

	getGame()->letValue((ParamOrigin)parameters->getByte(0),
		                parameters->getDword(1),
	                    parameters->getDword(5),
	                    INT2DSI(_data[9]));

	getGame()->letValue((ParamOrigin)parameters->getByte(9),
	                    parameters->getDword(10),
	                    parameters->getDword(14),
	                    INT2DSI(Common::Rational(51 * _data[10], _progress.size()).toInt()));

	return kOpcodeRetDefault;
}

OpcodeRet ProgressManager::help(OpcodeParameters *parameters) {
	byte param1 = parameters->getWord(0);
	if (_progress.size() == 0 || param1 < 1 || param1 > 3) {
		getGame()->letValue((ParamOrigin)parameters->getByte(4), parameters->getDword(5), parameters->getDword(9), INT2DSI(0));
		return kOpcodeRetDefault;
	}

	// Get index of first empty progress entry
	uint32 index;
	for (index = 0; index < _progress.size() && _progress[index]->field_0; ++index);

	Message message;
	uint32 messageIndex = _data[param1] + index;
	if (index == _progress.size() || !getResource()->readMessage(&messageIndex, &message)) {
		getGame()->letValue((ParamOrigin)parameters->getByte(4), parameters->getDword(5), parameters->getDword(9), INT2DSI(0));
		return kOpcodeRetDefault;
	}

	if (!message.field_B1)
		_data[9] += _data[param1] * _progress[index]->getData(param1);

	_progress[index]->setData(param1, 0);

	getGame()->letValue((ParamOrigin)parameters->getByte(4), parameters->getDword(5), parameters->getDword(9), INT2DSI(messageIndex));

	return kOpcodeRetDefault;
}

} // End of namespace Liath
