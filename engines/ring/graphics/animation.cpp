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

#include "ring/game/application.h"

#include "ring/graphics/image.h"
#include "ring/graphics/presentation.h"

#include "ring/ring.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region Animation

Animation::Animation() {
	_field_0  = 0;
	_frameCount  = 0;
	_field_C  = 0;
	_startFrame = 0;
	_field_14 = 0;
	_field_18 = 0;
	_priority = 0;
	_field_20 = 1;
	_field_21 = 1;
	_activeFrame = 0;
}

Animation::~Animation() {
}

void Animation::init(uint32 frameCount, float a2, uint32 startFrame, byte a4, uint32 priority) {
	_field_0 = 0;
	_field_C = a2;
	_frameCount = frameCount;

	setStartFrame(startFrame);

	if (a4 & 4)
		_field_14 = 4;
	else if (a4 & 8)
		_field_14 = 8;
	else if (a4 & 16)
		_field_14 = 16;
	else if (a4 & 32)
		_field_14 = 32;

	_field_2D = (a4 & 2) ? 1 : 0;

	_field_18 = _field_14;
	_priority = priority;
	_field_20 = 1;
	_field_21 = 1;

	if (!_frameCount)
		error("[Animation::init] Number of frames cannot be 0!");

	switch (_field_14) {
	default:
		break;

	case 4:
	case 16:
		_activeFrame = _startFrame;
		break;

	case 8:
	case 32:
		_activeFrame = _frameCount - 1;
		break;
	}

	_field_26 = 0;
	_field_27 = 0;
	_field_28 = 0;
	_field_2C = 1;
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
	_field_53 = 1000.0 / _field_C;

	if (_field_14 == 16)
		_field_57 = 1;
	else if (_field_14 == 32)
		_field_57 = 2;

	_field_58 = 0;
	_field_5C = 1;
	_field_60 = 0;
	_field_61 = -5;
	_field_65 = 0;
}

void Animation::sub_416710() {
	error("[AnimationImage::setTicks] Not implemented");
}

void Animation::setTicks(uint32 ticks) {
	_field_26 = 1;

	if (_field_20) {
		switch (_field_14) {
		default:
			break;

		case 4:
		case 16:
			_activeFrame = _startFrame;
			break;

		case 8:
		case 32:
			_activeFrame = _frameCount - 1;
			break;
		}

		if (_field_14 == 16)
			_field_57 = 1;
		else if (_field_14 == 32)
			_field_57 = 2;
	}

	_field_61 = -5;
	_ticks = ticks;
	_field_58 = 0;
	_field_5C = 1;
}

void Animation::setStartFrame(uint32 frame) {
	if (frame - 1 >= _frameCount)
		error("[Animation::setActiveFrame] Frame number is too big (was: %d, max: %d)", frame, _frameCount - 2);

	if (frame - 1 < 0)
		error("[Animation::setActiveFrame] Frame number is too small (was: %d, min: 1)", frame);

	_activeFrame = frame;
}

void Animation::setActiveFrame(uint32 frame) {
	if (frame - 1 >= _frameCount)
		error("[Animation::setActiveFrame] Frame number is too big (was: %d, max: %d)", frame, _frameCount - 2);

	if (frame - 1 < 0)
		error("[Animation::setActiveFrame] Frame number is too small (was: %d, min: 1)", frame);

	_startFrame = frame;
	_activeFrame = frame;
}

#pragma endregion

#pragma region AnimationImage

AnimationImage::AnimationImage() : Animation() {
	_field_6D = 0;
	_field_79 = 0;
	_field_7D = 0;
	_field_81 = 0;
	_currentImage = NULL;
	_field_89 = 0;
}

AnimationImage::~AnimationImage() {
	CLEAR_ARRAY(ImageHandle, _imageHandles);

	_currentImage = NULL;
}

void AnimationImage::init(Common::String name, uint32 a2, Common::Point point, uint32 a5, uint32 a6, uint32 frameCount, float a8, uint32 startFrame, char a10, uint32 a11, uint32 priority, LoadFrom loadFrom, ArchiveType archiveType) {
	Animation::init(frameCount, a8, startFrame, a10, priority);

	_name = name;
	_field_6D = a2;
	_field_79 = a5;

	_coordinates = point;
	_field_7D = a6;
	_field_81 = a11;

	// FIXME: This should never happen, as the parent checks for framecount != 0
	if (_frameCount == 0) {
		if (_field_81 == 1)
			alloc();

		_currentImage = _imageHandles[_startFrame];

		return;
	}

	// Create image storage for each frame
	for (uint32 i = 0; i < _frameCount; i++) {
		ImageHandle *image = new ImageHandle(name, point, true, LOBYTE(_field_7D), priority, a11, getApp()->getCurrentZone(), loadFrom, a2, archiveType);

		image->setField6C(2);
		image->setAnimation(this);

		_imageHandles.push_back(image);
	}
}

void AnimationImage::alloc() {
	error("[AnimationImage::alloc] Not implemented");
}

void AnimationImage::dealloc() {
	error("[AnimationImage::dealloc] Not implemented");
}

void AnimationImage::drawActiveFrame() {
	drawActiveFrame(_coordinates);
}

void AnimationImage::drawActiveFrame(Common::Point point) {
	error("[AnimationImage::drawActiveFrame] Not implemented");
}

void AnimationImage::draw() {
	error("[AnimationImage::draw] Not implemented");
}

void AnimationImage::setCoordinates(Common::Point point) {
	_coordinates =  point;

	for	(Common::Array<ImageHandle *>::iterator it = _imageHandles.begin(); it != _imageHandles.end(); it++)
		(*it)->setCoordinates(point);
}

void AnimationImage::updatePresentation(ObjectPresentation *objectPresentation) {
	for	(Common::Array<ImageHandle *>::iterator it = _imageHandles.begin(); it != _imageHandles.end(); it++)
		(*it)->setObjectPresentation(objectPresentation);
}

void AnimationImage::updateCurrentImage() {
	if (_activeFrame >= _imageHandles.size())
		error("[AnimationImage::updateCurrentImage] Current index is not valid (%d)", _activeFrame);

	_currentImage = _imageHandles[_activeFrame];
}

#pragma endregion

} // End of namespace Ring
