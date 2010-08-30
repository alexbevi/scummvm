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

#include "liath/game/array.h"

#include "liath/liath.h"

namespace Liath {

ArrayManager::ArrayManager(LiathEngine *engine) : _engine(engine) {}

ArrayManager::~ArrayManager() {
	// Zero-out passed pointers
	_engine = NULL;
}


OpcodeRet ArrayManager::init(OpcodeParameters *parameters) {
	error("ArrayManager::init: Not implemented!");
}

OpcodeRet ArrayManager::img(OpcodeParameters *parameters) {
	error("ArrayManager::img: Not implemented!");
}

OpcodeRet ArrayManager::add(OpcodeParameters *parameters) {
	error("ArrayManager::add: Not implemented!");
}

OpcodeRet ArrayManager::del(OpcodeParameters *parameters) {
	error("ArrayManager::del: Not implemented!");
}

OpcodeRet ArrayManager::kill(OpcodeParameters *parameters) {
	error("ArrayManager::kill: Not implemented!");
}

OpcodeRet ArrayManager::clear(OpcodeParameters *parameters) {
	error("ArrayManager::clear: Not implemented!");
}

OpcodeRet ArrayManager::get(OpcodeParameters *parameters) {
	error("ArrayManager::get: Not implemented!");
}

OpcodeRet ArrayManager::getNumber(OpcodeParameters *parameters) {
	error("ArrayManager::getNumber: Not implemented!");
}

OpcodeRet ArrayManager::size(OpcodeParameters *parameters) {
	error("ArrayManager::size: Not implemented!");
}

OpcodeRet ArrayManager::tget(OpcodeParameters *parameters) {
	error("ArrayManager::tget: Not implemented!");
}

OpcodeRet ArrayManager::cur(OpcodeParameters *parameters) {
	error("ArrayManager::cur: Not implemented!");
}

OpcodeRet ArrayManager::setcur(OpcodeParameters *parameters) {
	error("ArrayManager::setcur: Not implemented!");
}

OpcodeRet ArrayManager::getScroll(OpcodeParameters *parameters) {
	error("ArrayManager::getScroll: Not implemented!");
}

} // End of namespace Liath
