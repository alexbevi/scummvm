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

#include "liath/game/text.h"

#include "liath/liath.h"

namespace Liath {

TextManager::TextManager(LiathEngine *engine) : _engine(engine) {}

TextManager::~TextManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet TextManager::out(OpcodeParameters *parameters) {
	error("TextManager::out: Not implemented!");
}

OpcodeRet TextManager::quit(OpcodeParameters *parameters) {
	error("TextManager::quit: Not implemented!");
}

OpcodeRet TextManager::exit(OpcodeParameters *parameters) {
	error("TextManager::exit: Not implemented!");
}

OpcodeRet TextManager::check(OpcodeParameters *parameters) {
	error("TextManager::check: Not implemented!");
}

OpcodeRet TextManager::change(OpcodeParameters *parameters) {
	// Original just returns 1
	return kOpcodeRetDefault;
}

} // End of namespace Liath
