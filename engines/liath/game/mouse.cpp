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

#include "liath/game/mouse.h"

#include "liath/game/game.h"
#include "liath/game/hero.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

MouseManager::MouseManager(LiathEngine *engine) : _engine(engine), _count(0), _pressMouse(false) {}

MouseManager::~MouseManager() {
	// Zero-out passed pointers
	_engine = NULL;

	unload();
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void MouseManager::unload() {
	for (Common::Array<MouseBox *>::iterator it = _boxes.begin(); it != _boxes.end(); it++)
		SAFE_DELETE(*it);

	_boxes.clear();
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet MouseManager::push() {
	for (Common::Array<MouseBox *>::iterator i = _boxes.begin(); i != _boxes.end(); ++i)
		++(*i)->count;

	++_count;

	return kOpcodeRetDefault;
}

OpcodeRet MouseManager::pop() {
	for (Common::Array<MouseBox *>::iterator i = _boxes.begin(); i != _boxes.end(); ++i)
		--(*i)->count;

	--_count;

	return kOpcodeRetDefault;
}

OpcodeRet MouseManager::commandNumberBox(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersBDDD);

	int32 result = numberBox(params->param4, params->param5);

	switch (params->param1) {
	default:
		break;

	case kOriginGlobal:
		*GLOBAL(params->param3) = result;
		break;

	case kOriginHero:
		*getHero()->getData(params->param2, params->param3) = result;
		break;

	case kOriginHeroWork:
		*getHero()->getData(getWork()->getCurrent()->heroIndex, params->param3) = result;
		break;
	}

	return kOpcodeRetDefault;
}

OpcodeRet MouseManager::type(OpcodeParameters *parameters) {
	error("MouseManager::MouseManager: Not implemented!");
}

OpcodeRet MouseManager::reset(OpcodeParameters *parameters) {
	error("MouseManager::reset: Not implemented!");
}

OpcodeRet MouseManager::key(OpcodeParameters *parameters) {
	error("MouseManager::key: Not implemented!");
}

OpcodeRet MouseManager::press(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	if (_pressMouse) {
		if (_pressMouse == (bool)params->param1) {
			_pressMouse = true;
			return RET(true, params->test);
		} else {
			return RET(false, params->test);
		}
	}

	return RET(false, params->test);
}

OpcodeRet MouseManager::scroll(OpcodeParameters *parameters) {
	error("MouseManager::scroll: Not implemented!");
}

OpcodeRet MouseManager::check(OpcodeParameters *parameters) {
	error("MouseManager::check: Not implemented!");
}


OpcodeRet MouseManager::addBox(OpcodeParameters *parameters) {
	error("MouseManager::addBox: Not implemented!");
}

OpcodeRet MouseManager::removeBox(OpcodeParameters *parameters) {
	error("MouseManager::removeBox: Not implemented!");
}


//////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////

uint32 MouseManager::numberBox(uint32 param1, uint32 param2) {
	error("MouseManager::numberBox: not implemented!");
}

} // End of namespace Liath
