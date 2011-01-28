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

#include "ring/base/font.h"

namespace Ring {

FontHandler::FontHandler() {
}

FontHandler::~FontHandler() {
}

void FontHandler::add(int32 a1, Common::String fontName, int32 a3, int32 a4, int32 a5, int32 a6, int32 a7) {
	error("[FontHandler::add] Not implemented");
}

void FontHandler::addResource(Common::String filename) {
	error("[FontHandler::addResource] Not implemented");
}

} // End of namespace Ring
