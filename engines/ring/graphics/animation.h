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

#include "common/rect.h"

namespace Ring {

class ImageHandle;
class ObjectPresentation;

class Animation : public BaseObject, public Common::Serializable {
public:
	Animation();
	~Animation();

	void init(uint32 frameCount, float frameRate, uint32 startFrame, byte a4, uint32 priority);

	void pause() { _paused = true; }
	void unpause() { _paused = false; }

	void setTicks(uint32 ticks);
	void sub_416710();
	uint32 getCurrentFrame();
	uint32 computeCurrentFrame(uint32 ticks);
	void setActiveFrame(uint32 frame);
	void pauseOnFrame(uint32 frame, uint32 a2, uint32 a3);
	void setStartFrame(uint32 frame);

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

	// Accessors
	byte getField26() { return _field_26; }
	byte getField2D() { return _field_2D; }
	uint32 getActiveFrame() { return _activeFrame; }

	void setField20(byte val) { _field_20 = val; }
	void setField21(byte val) { _field_21 = val; }

protected:
	Common::String _name;
	uint32         _frameCount;
	float          _framerate;
	uint32         _startFrame;
	uint32         _field_14;
	uint32         _field_18;
	uint32         _priority;
	byte           _field_20;
	byte           _field_21;
	uint32         _activeFrame;
	byte           _field_26;
	bool           _paused;
	uint32         _lastTicks;
	byte           _field_2C;
	byte           _field_2D;
	uint32         _tickInterval;
	uint32         _field_32;
	uint32         _field_36;
	uint32         _field_3A;
	uint32         _field_3E;
	uint32         _field_42;
	uint32         _field_46;
	uint32         _field_4A;
	byte           _field_4E;
	uint32         _ticks;
	float          _field_53;
	byte           _field_57;
	uint32         _field_58;
	uint32         _field_5C;
	byte           _field_60;
	int32          _currentFrame;
	uint32         _field_65;

	int32 adjustTicks(uint32 ticks);

	// Event handling
	void onAnimation3();
	void onAnimation2();
	void onAnimation();
};

class AnimationImage : public Animation {
public:
	AnimationImage();
	virtual ~AnimationImage();

	void init(Common::String name, ImageType imageType, const Common::Point &point, uint32 a5, DrawType drawType, uint32 frameCount, float framerate, uint32 startFrame, byte a10, byte imageCount, uint32 priority, LoadFrom loadFrom, ArchiveType archiveType);
	void alloc();
	void allocActive();
	void dealloc();

	void playFrame(bool visible = true);
	void playFrame(const Common::Point &point, bool visible = true);
	void draw(const Common::Point &point);

	// Accessors
	void setField89() { _field_89 = 1; }
	void setCoordinates(const Common::Point &point);
	ImageHandle *getCurrentImage() { return _currentImage; }

	// Helpers
	void updatePresentation(ObjectPresentation *objectPresentation);
	void updateCurrentImage();

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	Common::Array<ImageHandle *> _imageHandles;
	ImageType                    _imageType;
	Common::Point                _coordinates;
	uint32                       _field_79;
	DrawType                     _drawType;
	uint32                       _imageCount;
	ImageHandle                 *_currentImage;
	byte                         _field_89;

	Common::String computePath(ImageHandle *image, uint32 index) const;
};

} // End of namespace Ring

#endif // RING_ANIMATION_H
