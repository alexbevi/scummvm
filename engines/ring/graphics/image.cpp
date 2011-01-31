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

#include "ring/graphics/image.h"

namespace Ring {

Image::Image() {
	_bpp = 32; // HACK
}

Image::~Image() {
}

void Image::destroy() {
	error("[Image::isInitialized] Not implemented");
}

bool Image::load(Common::String path, ArchiveType type, uint32 a3, uint32 a4) {
	warning("[Image::load] Not implemented (%s)", path.c_str());

	return true;
}

bool Image::isInitialized() {
	warning("[Image::isInitialized] Not implemented");

	return true;
}

} // End of namespace Ring
