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

#include "ring/graphics/imageLoader.h"

#include "ring/graphics/image.h"

namespace Ring {

bool ImageLoaderBMP::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	warning("[ImageLoaderBMP::load] Not implemented (%s)!", image->getName().c_str());

	return true;
}

bool ImageLoaderBMA::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	warning("[ImageLoaderBMA::load] Not implemented (%s)!", image->getName().c_str());

	return true;
}

bool ImageLoaderTGC::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	warning("[ImageLoaderTGC::load] Not implemented (%s)!", image->getName().c_str());

	return true;
}

bool ImageLoaderTGA::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	warning("[ImageLoaderTGA::load] Not implemented (%s)!", image->getName().c_str());

	return true;
}

bool ImageLoaderCNM::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	warning("[ImageLoaderCNM::load] Not implemented (%s)!", image->getName().c_str());

	return true;
}


} // End of namespace Ring
