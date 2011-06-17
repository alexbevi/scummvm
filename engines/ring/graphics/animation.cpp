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
 */

#include "ring/graphics/animation.h"

#include "ring/base/application.h"
#include "ring/base/event.h"
#include "ring/base/saveload.h"

#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/ring.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region Animation

Animation::Animation() : BaseObject(0) {
	_frameCount  = 0;
	_framerate  = 0;
	_startFrame = 0;
	_field_14 = 0;
	_field_18 = 0;
	_priority = 0;
	_field_20 = 1;
	_field_21 = 1;
	_activeFrame = 0;
	_field_26 = 0;
	_paused   = false;
	_lastTicks = 0;
	_field_2C = 0;
	_field_2D = 0;
	_tickInterval = 0;
	_field_32 = 0;
	_field_36 = 0;
	_field_3A = 0;
	_field_3E = 0;
	_field_42 = 0;
	_field_46 = 0;
	_field_4A = 0;
	_field_4E = 0;
	_ticks    = 0;
	_field_53 = 0;
	_field_57 = 0;
	_field_58 = 0;
	_field_5C = 0;
	_field_60 = 0;
	_currentFrame = 0;
	_field_65 = 0;
}

Animation::~Animation() {
}

void Animation::init(uint32 frameCount, float framerate, uint32 startFrame, byte a4, uint32 priority) {
	_id = 0;
	_framerate = framerate;
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
	_paused = false;
	_lastTicks = 0;
	_field_2C = 1;
	_field_2D = 0;
	_tickInterval = 0;
	_field_32 = 0;
	_field_36 = 1;
	_field_3A = 1;
	_field_3E = 0;
	_field_42 = 0;
	_field_46 = 0;
	_field_4A = 0;
	_field_4E = 1;
	_ticks    = 0;
	_field_53 = 1000.0f / _framerate;

	if (_field_14 == 16)
		_field_57 = 1;
	else if (_field_14 == 32)
		_field_57 = 2;

	_field_58 = 0;
	_field_5C = 1;
	_field_60 = 0;
	_currentFrame = -5;
	_field_65 = 0;
}

void Animation::sub_416710() {
	_field_26 = 0;
	_field_4E = 1;
}

int32 Animation::adjustTicks(uint32 ticks) {
	if (_paused)
		return _activeFrame;

	if (_lastTicks) {
		if ((ticks - _lastTicks) <= _tickInterval)
			return _activeFrame;

		onAnimation3();
	}

	switch (_field_4A) {
	default:
		if ((ticks - _field_4A) > _field_46) {
			onAnimation();
			return -1;
		}
		return _activeFrame;

	case 0:
		switch (_field_32) {
		default:
			if ((ticks - _field_32) <= _field_3E)
				return _activeFrame;

			onAnimation2();
			break;

		case 0:
			// Do nothing
			break;

		case 1:
			if (_field_36 == 0)
				error("[Animation::adjustTicks] Animation not initialized properly");

			if (_activeFrame != _field_3A || _field_58 % _field_36 || !_field_60)
				return -1;

			_field_32 = ticks;

			if (ticks == 1)
				_field_32 = 2;

			getApp()->onAnimation2(1, _id, _name, _activeFrame + 1, _field_3E);
			return _activeFrame;
		}
		return -1;

	case 1:
		// Do nothing
		break;
	}

	if (_activeFrame != _field_42 || !_field_60)
		return -1;

	_field_4A = ticks;
	if (ticks == 1)
		_field_4A = 2;

	getApp()->onAnimation(1, _id, _name, _activeFrame + 1, _field_46);

	return _activeFrame;
}

uint32 Animation::getCurrentFrame() {
	return computeCurrentFrame(g_system->getMillis());
}

uint32 Animation::computeCurrentFrame(uint32 ticks) {
	if (!_field_26)
		return _activeFrame + 1;

	if (!_field_4E) {
		if (adjustTicks(ticks) != -1)
			return _activeFrame + 1;

		uint32 offset = 0;
		switch (_field_21) {
		default:
			break;

		case 1:
			if ((ticks - _ticks) <= _field_53)
				break;

			offset = 1;

			_ticks = ticks;
			++_field_5C;
			_field_60 = 1;

			if (_field_65 && ((_field_5C % _field_65) == 1)) {
				_activeFrame = rnd(_frameCount);
				offset = 0;
			}
			break;

		case 2:
			if (((ticks - _ticks) / _field_53) == 0)
				break;

			_ticks = ticks;
			++_field_5C;
			_field_60 = 1;

			if (_field_65 && ((_field_5C % _field_65) == 1)) {
				_activeFrame = rnd(_frameCount);
				offset = 0;
			}
			break;
		}

		switch (_field_14) {
		default:
			break;

		case 4:
			if (offset)
				_activeFrame += offset;

			if (_activeFrame >= _frameCount) {
				_activeFrame = _startFrame;

				++_field_58;
				if (_field_2D)
					sub_416710();
			}
			break;

		case 8:
			if (offset)
				_activeFrame -= offset;

			if (_activeFrame < _frameCount) {
				_activeFrame = _frameCount - 1;

				++_field_58;
				if (_field_2D)
					sub_416710();
			}
			break;

		case 16:
		case 32:
			switch (_field_57) {
			default:
				break;

			case 1:
				if (offset)
					_activeFrame += offset;

				if (_activeFrame >= _frameCount) {
					_field_57 = 2;
					_activeFrame = _frameCount - 1;

					++_field_58;
					if (_field_2D)
						sub_416710();
				}
				break;

			case 2:
				if (offset)
					_activeFrame -= offset;


				if (_activeFrame < _frameCount) {
					_field_57 = 1;
					_activeFrame = _startFrame;

					++_field_58;
					if (_field_2D)
						sub_416710();
				}
				break;
			}
			break;
		}
	} else {
		_field_4E = 0;
	}

	// Notify event handler
	if (_currentFrame != (int32)(_activeFrame + 1)) {
		if (!_name.empty())
			getApp()->onAnimationNextFrame(_id, _name, _activeFrame + 1, _frameCount);
	}

	_currentFrame = _activeFrame + 1;

	return (uint32)_currentFrame;
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

	_currentFrame = -5;
	_ticks = ticks;
	_field_58 = 0;
	_field_5C = 1;
}

void Animation::setStartFrame(uint32 frame) {
	if (frame > _frameCount)
		error("[Animation::setActiveFrame] Frame number is too big (was: %d, max: %d)", frame, _frameCount);

	if (frame == 0)
		error("[Animation::setActiveFrame] Frame number is too small (was: %d, min: 1)", frame);

	_activeFrame = frame;
}

void Animation::setActiveFrame(uint32 frame) {
	if (frame > _frameCount)
		error("[Animation::setActiveFrame] Frame number is too big (was: %d, max: %d)", frame, _frameCount);

	if (frame == 0)
		error("[Animation::setActiveFrame] Frame number is too small (was: %d, min: 1)", frame);

	_startFrame = frame;
	_activeFrame = frame;
}

void Animation::pauseOnFrame(uint32 frame, uint32 a2, uint32 a3) {
	if (frame > _frameCount)
		error("[Animation::setActiveFrame] Frame number is too big (was: %d, max: %d)", frame, _frameCount);

	if (frame == 0)
		error("[Animation::setActiveFrame] Frame number is too small (was: %d, min: 1)", frame);

	_field_46 = a2;
	_field_42 = frame - 1;
	_field_4A = 1;

	if (a3 != 2) {
		int32 f1 = _activeFrame - (frame - 1);
		int32 f2 = 0;

		if (f1 <= 0) {
			f1 = _activeFrame - (_startFrame + frame - _frameCount);
			f2 = frame - (_activeFrame - 1);
		} else {
			f2 = _startFrame - (_activeFrame + frame - _frameCount);
		}

		if (_field_14 == 4 || ((_field_14 == 16 || _field_14 == 32) && _field_57 == 1)) {
			if (f1 < f2)
				_field_14 = 8;
		} else {
			if (f2 < f1)
				_field_14 = 4;
		}
	}
}

void Animation::onAnimation3() {
	uint32 tickInterval = _tickInterval;

	_lastTicks = 0;
	_tickInterval = 0;

	getApp()->onAnimation2(2, _id, _name, _activeFrame + 1, tickInterval);
}

void Animation::onAnimation2() {
	_field_32 = 1;
	_field_60 = 0;

	getApp()->onAnimation2(2, _id, _name, _activeFrame + 1, _field_3E);
}

void Animation::onAnimation() {
	_field_14 = _field_18;
	_field_4A = 0;
	_field_60 = 0;

	getApp()->onAnimation(2, _id, _name, _activeFrame + 1, _field_46);
}

#pragma region Serializable

void Animation::saveLoadWithSerializer(Common::Serializer &s) {
	uint32 currentTicks = getApp()->getSaveManager()->getTicks();
	uint32 field_28 = 0;
	uint32 field_32 = 0;
	uint32 field_4A = 0;
	uint32 ticks = 0;

	if (s.isSaving()) {
		if (_lastTicks > 1.0)
			field_28 = currentTicks - _lastTicks;
		else
			field_28 = _lastTicks;

		if (_field_32 > 1.0)
			field_32 = currentTicks - _field_32;
		else
			field_32 = _field_32;

		if (_field_4A > 1.0)
			field_4A = currentTicks - _field_4A;
		else
			field_4A = _field_4A;

		if (_ticks)
			ticks = currentTicks - _ticks;
		else
			ticks = 0;
	}

	s.syncString(_name);
	s.syncAsUint32LE(_framerate);
	s.syncAsUint32LE(_startFrame);
	s.syncAsUint32LE(_field_14);
	s.syncAsUint32LE(_field_18);
	s.syncAsUint32LE(_priority);
	s.syncAsByte(_field_20);
	s.syncAsByte(_field_21);
	s.syncAsUint32LE(_activeFrame);
	s.syncAsByte(_field_26);
	s.syncAsByte(_paused);
	s.syncAsUint32LE(field_28);
	s.syncAsByte(_field_2C);
	s.syncAsByte(_field_2D);
	s.syncAsUint32LE(_tickInterval);
	s.syncAsUint32LE(_field_32);
	s.syncAsUint32LE(_field_36);
	s.syncAsUint32LE(_field_3A);
	s.syncAsUint32LE(_field_3E);
	s.syncAsUint32LE(_field_42);
	s.syncAsUint32LE(_field_46);
	s.syncAsUint32LE(_field_4A);
	s.syncAsByte(_field_4E);
	s.syncAsUint32LE(_ticks);
	s.syncAsUint32LE(_field_53);
	s.syncAsByte(_field_57);
	s.syncAsUint32LE(_field_58);
	s.syncAsUint32LE(_field_5C);
	s.syncAsByte(_field_60);
	s.syncAsSint32LE(_currentFrame);
	s.syncAsUint32LE(_field_65);

	// Adjust ticks
	if (s.isLoading()) {
		if (field_28 > 1.0)
			_lastTicks = currentTicks - field_28;
		else
			_lastTicks = field_28;

		if (field_32 > 1.0)
			_field_32 = currentTicks - field_32;
		else
			_field_32 = field_32;

		if (field_4A > 1.0)
			_field_4A = currentTicks - field_4A;
		else
			_field_4A = field_4A;

		if (ticks)
			_ticks = currentTicks - ticks;
		else
			_ticks = 0;
	}
}

#pragma endregion

#pragma endregion

#pragma region AnimationImage

AnimationImage::AnimationImage() : Animation() {
	_imageType = kImageTypeBMP;
	_field_79 = 0;
	_drawType = kDrawTypeInvalid;
	_imageCount = 0;
	_currentImage = NULL;
	_field_89 = 0;
}

AnimationImage::~AnimationImage() {
	CLEAR_ARRAY(ImageHandle, _imageHandles);

	_currentImage = NULL;
}

void AnimationImage::init(Common::String name, ImageType imageType, const Common::Point &point, uint32 a5, DrawType drawType, uint32 frameCount, float framerate, uint32 startFrame, byte a10, byte imageCount, uint32 priority, LoadFrom loadFrom, ArchiveType archiveType) {
	Animation::init(frameCount, framerate, startFrame, a10, priority);

	_name = name;
	_imageType = imageType;
	_field_79 = a5;

	_coordinates = point;
	_drawType = drawType;
	_imageCount = imageCount;

	if (_frameCount != 0) {
		// Create image storage for each frame
		for (uint32 i = 0; i < _frameCount; i++) {
			ImageHandle *image = new ImageHandle(name, point, true, _drawType, priority, imageCount, getApp()->getCurrentZone(), loadFrom, imageType, archiveType);

			image->setField6C(2);
			image->setAnimation(this);

			_imageHandles.push_back(image);
		}

		if (_imageCount == 1)
			alloc();

		_currentImage = _imageHandles[_startFrame];
	}
}

void AnimationImage::alloc() {
	for	(uint32 i = 0; i < _imageHandles.size(); i++) {
		ImageHandle *image = _imageHandles[i];

		if (image->isInitialized())
			continue;

		// Load image
		Common::String path = computePath(image, i);
		if (!image->load(path, image->getArchiveType(), image->getZone(), image->getLoadFrom(), image->getDrawType()))
			error("[AnimationImage::alloc] Cannot load image (%s)", path.c_str());
	}
}

void AnimationImage::allocActive() {
	// Check current image
	if (_activeFrame >= _imageHandles.size())
		error("[AnimationImage::allocActive] Invalid active frame (%d)", _activeFrame);

	_currentImage = _imageHandles[_activeFrame];
	if (!_currentImage->isInitialized()) {
		Common::String path = computePath(_currentImage, _activeFrame);

		if (!_currentImage->load(path, _currentImage->getArchiveType(), _currentImage->getZone(), _currentImage->getLoadFrom(), _currentImage->getDrawType()))
			error("[AnimationImage::alloc] Cannot load image (%s)", path.c_str());
	}
}

void AnimationImage::dealloc() {
	for	(Common::Array<ImageHandle *>::iterator it = _imageHandles.begin(); it != _imageHandles.end(); it++)
		if ((*it)->isInitialized())
			(*it)->destroy();
}

void AnimationImage::playFrame(bool visible) {
	playFrame(_coordinates, visible);
}

void AnimationImage::playFrame(const Common::Point &point, bool visible) {
	if (_field_2D || _field_89)
		if (_currentImage)
			_currentImage->destroy();

	// Check current image
	if (_activeFrame >= _imageHandles.size())
		error("[AnimationImage::playFrame] Invalid active frame (%d)", _activeFrame);

	ImageHandle *image = _imageHandles[_activeFrame];
	if (_field_26 && ((!_paused && !_lastTicks && _field_32 <= 1) || _field_2C)) {

		// Set current image and initialize if needed
		_currentImage = image;
		if (!image->isInitialized()) {
			Common::String path = computePath(image, _activeFrame);
			if (!image->load(path, image->getArchiveType(), image->getZone(), image->getLoadFrom(), image->getDrawType()))
				error("[AnimationImage::playFrame] Cannot load frame (%s)", path.c_str());
		}

		// Draw frame
		if (visible)
			getApp()->getScreenManager()->draw(image, point, _drawType);
	}
}

void AnimationImage::draw(const Common::Point &point) {
	getCurrentFrame();
	playFrame(point);
}

void AnimationImage::setCoordinates(const Common::Point &point) {
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

Common::String AnimationImage::computePath(ImageHandle *image, uint32 index) const {
	// Compute filename
	Common::String filename = Common::String::format("%s/%s.%04d.%s", image->getNameId().c_str(), image->getNameId().c_str(), index + 1, Application::getFileExtension(image->getImageType()).c_str());
	Common::String path;
	if (image->getArchiveType() == kArchiveArt) {
		switch (image->getLoadFrom()) {
		default:
			break;

		case kLoadFromCd:
		case kLoadFromDisk:
			path = Common::String::format("/ANI/%s", filename.c_str());
			break;

		case kLoadFromCursor:
			path = Common::String::format("/CURSOR/%s", filename.c_str());
			break;

		case kLoadFromListIcon:
			path = Common::String::format("/LSTICON/%s", filename.c_str());
			break;
		}
	} else if (image->getArchiveType() == kArchiveFile) {
		switch (image->getLoadFrom()) {
		default:
			break;

		case kLoadFromCd:
		case kLoadFromDisk:
			path = Common::String::format("DATA/%s/ANI/%s", getApp()->getZoneFolder(image->getZone()).c_str(), filename.c_str());
			break;

		case kLoadFromCursor:
			path = Common::String::format("DATA/%s/CURSOR/%s", getApp()->getZoneFolder(image->getZone()).c_str(), filename.c_str());
			break;

		case kLoadFromListIcon:
			path = Common::String::format("DATA/%s/LSTICON/%s", getApp()->getZoneFolder(image->getZone()).c_str(), filename.c_str());
			break;
		}
	}

	return path;
}

#pragma region Serializable

void AnimationImage::saveLoadWithSerializer(Common::Serializer &s) {
	Animation::saveLoadWithSerializer(s);

	s.syncAsSint16LE(_coordinates.x);
	s.syncAsSint16LE(_coordinates.y);
	s.syncAsByte(_field_89);
}

#pragma endregion

#pragma endregion

} // End of namespace Ring
