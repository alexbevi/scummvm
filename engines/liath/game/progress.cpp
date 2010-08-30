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

#include "liath/liath.h"

namespace Liath {

ProgressManager::ProgressManager(LiathEngine *engine) : _engine(engine) {}

ProgressManager::~ProgressManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet ProgressManager::init(OpcodeParameters *parameters) {
	error("ProgressManager::init: Not implemented!");
}

OpcodeRet ProgressManager::set(OpcodeParameters *parameters) {
	error("ProgressManager::set: Not implemented!");
}

OpcodeRet ProgressManager::get(OpcodeParameters *parameters) {
	error("ProgressManager::get: Not implemented!");
}

OpcodeRet ProgressManager::help(OpcodeParameters *parameters) {
	error("ProgressManager::help: Not implemented!");
}

} // End of namespace Liath
