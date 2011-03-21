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

#include "ring/graphics/imageLoader.h"

#include "ring/debug.h"
#include "ring/helpers.h"

namespace Ring {

#pragma region ImageHandle

ImageHandle::ImageHandle(Common::String nameId, const Common::Point &point, bool active, DrawType drawType, uint32 priority, byte imageCount, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType) {
	init(nameId, point, active, drawType, priority, imageCount, zone, loadFrom, imageType, archiveType);
}

ImageHandle::ImageHandle(Common::String nameId, const Common::Point &point, bool active, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType) {
	init(nameId, point, active, kDrawType1, 0, 0, zone, loadFrom, imageType, archiveType);
}

ImageHandle::~ImageHandle() {
	// Zero-out passed pointers
	_objectPresentation = NULL;
	_animation = NULL;
}

void ImageHandle::init(Common::String nameId, const Common::Point &point, bool active, DrawType drawType, uint32 priority, byte imageCount, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType) {
	_nameId = nameId;
	_coordinates = point;
	_originalCoordinates = point;
	_isActive = active;
	_drawType = drawType;
	_priority = priority;
	_imageCount = imageCount;
	_field_6C = 1;
	_imageType = imageType;
	_objectPresentation = NULL;
	_animation = NULL;
	_zone = zone;
	_loadFrom = loadFrom;
	_archiveType = archiveType;
}

#pragma region Serializable

void ImageHandle::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncString(_directory);
	s.syncString(_nameId);
	s.syncAsSint16LE(_coordinates.x);
	s.syncAsSint16LE(_coordinates.y);
	s.syncAsByte(_isActive);
	s.syncAsUint32LE(_zone);
	s.syncAsUint32LE(_loadFrom);
	s.syncAsUint32LE(_imageType);
	s.syncAsUint32LE(_archiveType);
}

#pragma endregion

#pragma endregion

#pragma region Image

Image::Image() {
	_surface = NULL;
}

Image::~Image() {
	destroy();
}

void Image::create(uint32 depth, uint32 direction, uint32 width, uint32 height) {
	if (_surface)
		_surface->free();

	_surface = new Graphics::Surface();
	_surface->create(width, height, depth);
}

void Image::destroy() {
	if (_surface)
		_surface->free();

	SAFE_DELETE(_surface);
}

bool Image::load(Common::String filename, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	if (filename.empty())
		return false;

	debugC(kRingDebugGraphics, "Loading image: %s", filename.c_str());
	_filename = filename;

	ImageLoader *loader = NULL;

	// Special case for files with no extension
	if (filename.size() < 4) {
		if (type == kArchiveFile)
			loader = new ImageLoaderBMP();
		else
			loader = new ImageLoaderBMA();
	} else if (filename.hasSuffix(".bmp")) {
		// BMP images in art files are always compressed
		if (type == kArchiveFile)
			loader = new ImageLoaderBMP();
		else
			loader = new ImageLoaderBMA();
	} else if (filename.hasSuffix(".cnm")) {
		if (type == kArchiveArt)
			error("[Image::load] Archive files do not contains cinematic files (%s)", filename.c_str());

		loader = new ImageLoaderCIN();
	} else if (filename.hasSuffix(".tga")) {
		// TGA images in art files are always compressed
		if (type == kArchiveFile)
			loader = new ImageLoaderTGA();
		else
			loader = new ImageLoaderTGC();
	} else if (filename.hasSuffix(".bma")) {
		loader = new ImageLoaderBMA();
	} else if (filename.hasSuffix(".tgc")) {
		loader = new ImageLoaderTGC();
	} else {
		// Defaults to BMP or BMA
		if (type == kArchiveFile)
			loader = new ImageLoaderBMP();
		else
			loader = new ImageLoaderBMA();
	}

	if (!loader)
		error("[Image::load] Cannot find an image loader for file %s", filename.c_str());

	if (!loader->load(this, type, zone, loadFrom)) {
		delete loader;

		return false;
	}

	delete loader;

	return true;
}

Image *Image::zoom(uint32 xZoom, uint32 yZoom) {
	if (!isInitialized())
		error("[Image::zoom] Image not initialized!");

	if (getBPP() != 24)
		error("[Image::zoom] Not a 24bpp image!");

	// Create new image to store zoomed image
	uint32 width = getWidth() * xZoom;
	uint32 height = getHeight() * yZoom;

	Image *image = new Image();
	image->create(24, 2, width, height);

	byte *zoomedData = (byte *)image->getSurface()->getBasePtr(0, 0);

	for (uint32 j = 0; j < height; j++) {
		for (uint32 i = 0; i < width; i++) {
			byte *data = (byte *)_surface->getBasePtr(i / xZoom, j / yZoom);

			zoomedData[0] = data[0];
			zoomedData[1] = data[1];
			zoomedData[2] = data[2];

			zoomedData += 3;
		}
	}

	return image;
}

Common::Rect Image::draw(Graphics::Surface *surface, Common::Point dest) {
	return draw(surface, dest, _surface->w, _surface->h, 0, 0);
}

Common::Rect Image::draw(Graphics::Surface *surface, Common::Point dest, uint32 srcWidth, uint32 srcHeight, uint32 srcX, uint32 offset) {
	// Compute destination rectangle
	Common::Rect destRect(dest.x, dest.y, dest.x + srcWidth, dest.y + srcHeight);
	destRect.clip(640, 480);

	//////////////////////////////////////////////////////////////////////////
	// DEBUG: Draw destination rect
	surface->fillRect(destRect, Color(255, 0, 0).getColor());
	//////////////////////////////////////////////////////////////////////////

	byte *src  = (byte*)_surface->getBasePtr(srcX, getHeight() - (srcHeight + offset));
	byte *dst = (byte *)surface->getBasePtr(640, 480);
	uint32 height = destRect.height();

	while (--height) {
		memcpy(dst - (destRect.top * surface->pitch + destRect.left), src, destRect.width() * _surface->bytesPerPixel);
		dst -= surface->pitch;
		src += _surface->pitch;
	}

	return destRect;
}

void Image::setSurface(Graphics::Surface *surface) {
	destroy();

	_surface = surface;
}

#pragma endregion

} // End of namespace Ring
