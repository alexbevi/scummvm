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

#include "ring/graphics/presentation.h"

namespace Ring {

#pragma region ImageHandle

ImageHandle::ImageHandle(Common::String nameId, uint32 a3, uint32 a4, bool isActive, byte a6, uint32 priority, byte a8, Zone zone, LoadFrom loadFrom, byte a11, ArchiveType archiveType) {
	init(nameId, a3, a4, isActive, a6, priority, a8, zone, loadFrom, a11, archiveType);
}

ImageHandle::ImageHandle(Common::String nameId, uint32 a3, uint32 a4, bool isActive, Zone zone, LoadFrom loadFrom, byte a8, ArchiveType archiveType) {
	init(nameId, a3, a4, isActive, 1, 0, 0, zone, loadFrom, a8, archiveType);
}

ImageHandle::~ImageHandle() {
	// Zero-out passed pointers
	_objectPresentation = NULL;
}

void ImageHandle::init(Common::String nameId, uint32 a3, uint32 a4, bool isActive, byte a6, uint32 priority, byte a8, Zone zone, LoadFrom loadFrom, byte a11, ArchiveType archiveType) {
	_nameId = nameId;
	_field_55 = a3;
	_field_59 = a4;
	_field_5D = a3;
	_field_61 = a4;
	_isActive = isActive;
	_field_66 = a6;
	_priority = priority;
	_field_6B = a8;
	_field_6C = 1;
	_field_70 = a11;
	_objectPresentation = NULL;
	_field_75 = 0;
	_zone = zone;
	_loadFrom = loadFrom;
	_archiveType = archiveType;
}

#pragma endregion

#pragma region Image

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

#pragma endregion

} // End of namespace Ring
