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

#include "ring/graphics/animation.h"

#include "ring/graphics/image.h"
#include "ring/graphics/presentation.h"

namespace Ring {

AnimationImage::AnimationImage() {
	_field_0  = 0;
	_field_8  = 0;
	_field_C  = 0;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = 0;
	_field_1C = 0;
	_field_20 = 1;
	_field_21 = 1;
	_currentIndex = 0;
	_field_26 = 0;
	_field_27 = 0;
	_field_28 = 0;
	_field_2C = 0;
	_field_2D = 0;
	_field_2E = 0;
	_field_32 = 0;
	_field_36 = 1;
	_field_3A = 1;
	_field_3E = 0;
	_field_42 = 0;
	_field_46 = 0;
	_field_4A = 0;
	_field_4E = 1;
	_ticks    = 0;
	_field_53 = 0;
	_field_57 = 0;
	_field_58 = 0;
	_field_5C = 0;
	_field_60 = 0;
	_field_61 = -5;
	_field_65 = 0;
	_field_6D = 0;
	_field_79 = 0;
	_field_7D = 0;
	_field_81 = 0;
	_currentImage = NULL;
	_field_89 = 0;
}

AnimationImage::~AnimationImage() {
}

void AnimationImage::init(Common::String name, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10, uint32 a11, uint32 a12, uint32 a13, ArchiveType archiveType) {
	warning("[AnimationImage::init] Not implemented");
}

void AnimationImage::deinit() {
	warning("[AnimationImage::deinit] Not implemented");
}

void AnimationImage::alloc() {
	warning("[AnimationImage::alloc] Not implemented");
}

void AnimationImage::dealloc() {
	warning("[AnimationImage::dealloc] Not implemented");
}

void AnimationImage::draw() {
	warning("[AnimationImage::draw] Not implemented");
}

void AnimationImage::setTicks(uint32 ticks) {
	warning("[AnimationImage::setTicks] Not implemented");
}

void AnimationImage::sub_416710() {
	warning("[AnimationImage::setTicks] Not implemented");
}

void AnimationImage::updatePresentation(ObjectPresentation *objectPresentation) {
	for	(Common::Array<ImageHandle *>::iterator it = _imageHandles.begin(); it != _imageHandles.end(); it++)
		(*it)->setObjectPresentation(objectPresentation);
}

void AnimationImage::updateCurrentImage() {
	if (_currentIndex >= _imageHandles.size())
		error("[AnimationImage::updateCurrentImage] Current index is not valid (%d)", _currentIndex);

	_currentImage = _imageHandles[_currentIndex];
}

void AnimationImage::setCoordinates(Common::Point point) {
	_coordinates =  point;

	for	(Common::Array<ImageHandle *>::iterator it = _imageHandles.begin(); it != _imageHandles.end(); it++)
		(*it)->setCoordinates(point);
}

} // End of namespace Ring
