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

#ifndef RING_OBJECT_H
#define RING_OBJECT_H

#include "ring/shared.h"

#include "common/events.h"

namespace Ring {

class Application;
class Accessibility;
class AnimationImage;
class ObjectPresentation;
class Puzzle;
class Rotation;

class Object : public BaseObject {
public:
	Object(Application *application, ObjectId id, Common::String language, Common::String name, byte a5);
	~Object();

	// Presentation
	void addPresentation();
	void addTextToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String text, uint32 a5, uint32 a6, FontId fontId, byte a8, byte a9, byte a10, uint32 a11, uint32 a12, uint32 a13);
	void setTextToPuzzle(uint32 presentationIndex, uint32 textIndex, Common::String text);
	void setTextCoordinatesToPuzzle(uint32 presentationIndex, uint32 textIndex, Common::Point point);
	uint32 getTextWidth(uint32 presentationIndex, uint32 textIndex);

	void addImageToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String filename, Common::Point point, bool isActive, byte a8, uint32 priority, byte a10, LoadFrom loadFrom);
	void addImageToRotation(uint32 presentationIndex, Rotation *rotation, uint32 layer);
	void setImageCoordinatesOnPuzzle(uint32 presentationIndex, Common::Point point);
	void setImageCoordinatesOnPuzzle(uint32 presentationIndex, uint32 imageIndex, Common::Point point);
	void setImageOriginalCoordinatesOnPuzzle(uint32 presentationIndex);
	Common::Point getImageCoordinatesOnPuzzle(uint32 presentationIndex, uint32 imageIndex);

	void show(uint32 presentationIndex);
	void show();
	void hide(uint32 presentationIndex);
	void hide();
	void hideAndRemove(uint32 presentationIndex);
	void hideAndRemove();

	// Accessibility
	void addPuzzleAccessibility(Puzzle *puzzle, Common::Rect rect, bool enabled, uint32 a9, uint32 a10);
	void addRotationAccessibility(Rotation *rotation, Common::Rect rect, bool enabled, uint32 a9, uint32 a10);
	void setAccessibilityKey(uint32 accessibilityIndex, Common::KeyCode key);
	void setAccessibilityOnOrOff(bool enableHotspot);
	void setAccessibilityOnOrOff(bool enableHotspot, uint32 fromAcceleration, uint32 toAcceleration);

	// Animation
	void addAnimationToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String name, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10, uint32 a11, uint32 a12, uint32 a13, uint32 a14, LoadFrom loadFrom);
	void addAnimationToRotation(uint32 presentationIndex, Rotation *rotation, uint32 layer, uint32 a5, float a6, uint32 a7);
	void setAnimationOnPuzzle(uint32 presentationIndex, uint32 animationIndex, uint32 a3);
	void setAnimationOnRotation(uint32 presentationIndex, uint32 animationIndex, uint32 a3);
	void setAnimationStartFrame(uint32 presentationIndex, uint32 startFrame);
	void setAnimationActiveFrame(uint32 presentationIndex, uint32 active);
	void setAnimationCoordinatesOnPuzzle(uint32 presentationIndex, Common::Point point);
	void pauseAnimation(uint32 presentationIndex);
	void unpauseAnimation(uint32 presentationIndex);
	void pauseFrameAnimation(uint32 presentationIndex, uint32 frame, uint32 a3, uint32 a4);

	// Cursor
	void setActiveCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType);
	void setPassiveCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType);
	void setActiveDrawCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType);
	void setPassiveDrawCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType);

	// Accessors
	Common::String getName() { return _name; }
	void setAnimationImage(AnimationImage *image) { _animationImage = image; }

private:
	struct ObjectCursor {
		uint32      _field_0;
		uint32      _field_4;
		uint32      _field_8;
		uint32      _field_C;
		float       _field_10;
		uint32      _field_14;
		uint32      _field_18;
		ArchiveType _archiveType;

		ObjectCursor() {
			_field_0     = 0;
			_field_4     = 0;
			_field_8     = 0;
			_field_C     = 3;
			_field_10    = 0;
			_field_14    = 0;
			_field_18    = 3;
			_archiveType = kArchiveFile;
		}
	};

	Application *_application;

	Common::String _language;
	Common::String _name;
	byte    _field_C;
	Common::Array<Accessibility *> _accessibilities;
	Common::Array<ObjectPresentation *> _presentations;
	ObjectCursor _passiveCursor;
	ObjectCursor _activeCursor;
	ObjectCursor _passiveDrawCursor;
	ObjectCursor _activeDrawCursor;
	AnimationImage *_animationImage;

	void setCursor(ObjectCursor *cursor, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType);
};

class ObjectInfo : public BaseObject {
public:
	ObjectInfo(ObjectId id, Common::String language, Common::String name);
	~ObjectInfo() {}

	// Accessors
	Common::String getLanguage() { return _language; }
	Common::String getName() { return _name; }

private:
	Common::String _language;
	Common::String _name;
};

class ObjectHandler {
public:
	ObjectHandler();
	~ObjectHandler();

	void addFromFile(Common::String filename, Common::String language);

	// Accessors
	Common::String getLanguage(ObjectId id);
	Common::String getName(ObjectId id);

private:
	AssociativeArray<ObjectInfo *> _objects;
};

} // End of namespace Ring

#endif // RING_OBJECT_H
