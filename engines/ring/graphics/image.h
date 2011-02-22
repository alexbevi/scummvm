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

#ifndef RING_IMAGE_H
#define RING_IMAGE_H

#include "ring/shared.h"

#include "graphics/surface.h"

namespace Ring {

class AnimationImage;
class ObjectPresentation;

class Image {
public:
	Image();
	~Image();

	void create(uint32 depth, uint32 direction, uint32 width, uint32 height);
	void destroy();

	bool load(Common::String filename, ArchiveType type, Zone zone, LoadFrom loadFrom);
	Image *zoom(uint32 xZoom, uint32 yZoom);

	// Drawing
	void draw(Graphics::Surface *surface, Common::Point dest);
	void draw(Graphics::Surface *surface, Common::Point dest, uint32 srcWidth, uint32 srcHeight, uint32 srcX, uint32 offset);

	// Accessors
	uint32 getBPP()    { return _surface ? _surface->bytesPerPixel : 0; }
	uint32 getWidth()  { return _surface ? _surface->w : 0; }
	uint32 getHeight() { return _surface ? _surface->h : 0; }

	Common::String getName() { return _filename; }
	void setName(Common::String name) { _filename = name; }

	bool isInitialized() { return _surface != NULL; }

protected:
	//bool _loaded;
	Common::String _filename;
	Graphics::Surface *_surface;
	//uint32 _direction;
	//uint32 _offset;
	//Common::Rect _rect;
};

class ImageHandle : public Image {
public:
	ImageHandle(Common::String nameId, const Common::Point &point, bool isActive, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType);
	ImageHandle(Common::String nameId, const Common::Point &point, bool isActive, byte a6, uint32 priority, byte frameCount, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType);
	~ImageHandle();

	// Accessors
	Common::String getNameId() { return _nameId; }
	Common::String getDirectory() { return _directory; }
	void setDirectory(Common::String directory) { _directory = directory; }
	void setCoordinates(const Common::Point &point) { _coordinates = point; }
	Common::Point getCoordinates() { return _coordinates; }
	Common::Point getOriginalCoordinates() { return _originalCoordinates; }
	bool isActive() { return _isActive; }
	uint32 getPriority() { return _priority; }
	void setField6C(uint32 val) { _field_6C = val; }
	uint32 getField6C() { return _field_6C; }
	void setObjectPresentation(ObjectPresentation *objectPresentation) { _objectPresentation = objectPresentation; }
	void setAnimation(AnimationImage *animation) { _animation = animation; }
	AnimationImage *getAnimation() { return _animation; }
	Zone getZone() { return _zone; }
	LoadFrom getLoadFrom() { return _loadFrom; }
	ArchiveType getArchiveType() { return _archiveType; }

private:
	Common::String _nameId;
	Common::String _directory;
	Common::Point _coordinates;
	Common::Point _originalCoordinates;
	bool _isActive;
	byte _field_66;
	uint32 _priority;
	byte _frameCount;
	uint32 _field_6C;
	ImageType _imageType;
	ObjectPresentation *_objectPresentation;
	AnimationImage *_animation;
	Zone _zone;
	LoadFrom _loadFrom;
	ArchiveType _archiveType;

	void init(Common::String nameId, const Common::Point &point, bool isActive, byte a6, uint32 priority, byte a8, Zone zone, LoadFrom loadFrom, ImageType imageType, ArchiveType archiveType);
};

} // End of namespace Ring

#endif // RING_IMAGE_H
