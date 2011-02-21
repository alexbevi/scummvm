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

#include "ring/base/object.h"

namespace Ring {

#pragma region ImageHandle

ImageHandle::ImageHandle(Common::String nameId, const Common::Point &point, bool isActive, byte a6, uint32 priority, byte frameCount, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType) {
	init(nameId, point, isActive, a6, priority, frameCount, zone, loadFrom, imageType, archiveType);
}

ImageHandle::ImageHandle(Common::String nameId, const Common::Point &point, bool isActive, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType) {
	init(nameId, point, isActive, 1, 0, 0, zone, loadFrom, imageType, archiveType);
}

ImageHandle::~ImageHandle() {
	// Zero-out passed pointers
	_objectPresentation = NULL;
	_animation = NULL;
}

void ImageHandle::init(Common::String nameId, const Common::Point &point, bool isActive, byte a6, uint32 priority, byte frameCount, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType) {
	_path = "";
	_nameId = nameId;
	_coordinates = point;
	_originalCoordinates = point;
	_isActive = isActive;
	_field_66 = a6;
	_priority = priority;
	_frameCount = frameCount;
	_field_6C = 1;
	_imageType = imageType;
	_objectPresentation = NULL;
	_animation = NULL;
	_zone = zone;
	_loadFrom = loadFrom;
	_archiveType = archiveType;

	// Image
	_bpp = 0;
}

#pragma endregion

#pragma region Image

Image::Image() {
	_width = 0;
	_height = 0;
	_bpp = 32; // HACK
}

Image::~Image() {
}

void Image::destroy() {
	error("[Image::destroy] Not implemented");
}

bool Image::load(Common::String path, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	warning("[Image::load] Not implemented (%s)", path.c_str());

	return true;
}

bool Image::isInitialized() {
	warning("[Image::isInitialized] Not implemented");

	return false;
}

#pragma endregion

} // End of namespace Ring
