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

#ifndef RING_ANIMATION_H
#define RING_ANIMATION_H

#include "ring/shared.h"

namespace Ring {

class ImageHandle;
class ObjectPresentation;

class AnimationImage {
public:
	AnimationImage();
	~AnimationImage();

	void init(Common::String name, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10, uint32 a11, uint32 a12, uint32 a13, ArchiveType archiveType);
	void deinit();
	void alloc();
	void dealloc();

	void draw();
	void sub_416710();

	void updatePresentation(ObjectPresentation *objectPresentation);
	void updateCurrentImage();

	void setCoordinates(Common::Point point);

	// Accessors
	void setTicks(uint32 ticks);
	void setField20(byte val) { _field_20 = 0; }
	void setField89() { _field_89 = 1; }
	ImageHandle *getCurrentImage() { return _currentImage; }

private:
	uint32 _field_0;
	Common::String _name;
	uint32 _field_8;
	uint32 _field_C;
	uint32 _field_10;
	uint32 _field_14;
	uint32 _field_18;
	uint32 _field_1C;
	byte   _field_20;
	byte   _field_21;
	uint32 _currentIndex;
	byte   _field_26;
	byte   _field_27;
	uint32 _field_28;
	byte   _field_2C;
	byte   _field_2D;
	uint32 _field_2E;
	uint32 _field_32;
	uint32 _field_36;
	uint32 _field_3A;
	uint32 _field_3E;
	uint32 _field_42;
	uint32 _field_46;
	uint32 _field_4A;
	byte   _field_4E;
	uint32 _ticks;
	uint32 _field_53;
	byte   _field_57;
	uint32 _field_58;
	uint32 _field_5C;
	byte   _field_60;
	int32  _field_61;
	uint32 _field_65;
	Common::Array<ImageHandle *> _imageHandles;
	uint32 _field_6D;
	Common::Point _coordinates;
	uint32 _field_79;
	uint32 _field_7D;
	uint32 _field_81;
	ImageHandle *_currentImage;
	byte _field_89;
};

} // End of namespace Ring

#endif // RING_ANIMATION_H
