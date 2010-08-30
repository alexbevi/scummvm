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

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

PositionManager::PositionManager(LiathEngine *engine) : _engine(engine) {}

PositionManager::~PositionManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet PositionManager::checkLimits(OpcodeParameters *parameters) {
	error("PositionManager::checkLimits: not implemented!");
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
	error("PositionManager::pers: Not implemented!");
}

OpcodeRet PositionManager::hpers(OpcodeParameters *parameters) {
	error("PositionManager::hpers: Not implemented!");
}

OpcodeRet PositionManager::dist(OpcodeParameters *parameters) {
	error("PositionManager::dist: Not implemented!");
}

OpcodeRet PositionManager::feel(OpcodeParameters *parameters) {
	error("PositionManager::feel: Not implemented!");
}

OpcodeRet PositionManager::barrier(OpcodeParameters *parameters) {
	error("PositionManager::barrier: Not implemented!");
}

} // End of namespace Liath
