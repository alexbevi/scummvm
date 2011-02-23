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

class Animation;
class Application;
class Accessibility;
class AnimationImage;
class ImageHandle;
class Object;
class Puzzle;
class Rotation;
class Text;

class ObjectPresentation : public Common::Serializable {
public:
	ObjectPresentation(Object *object);
	~ObjectPresentation();

	// Text
	void addTextToPuzzle(Puzzle *puzzle, Common::String text, const Common::Point &point, FontId fontId, Color foreground, Color background);
	void setTextToPuzzle(uint32 textIndex, Common::String text);
	void setTextCoordinatesToPuzzle(uint32 textIndex, const Common::Point &point);
	uint32 getTextWidth(uint32 textIndex);

	// Image
	void addImageToPuzzle(Puzzle *puzzle, Common::String filename, const Common::Point &point, bool isActive, byte a7, uint32 priority, byte a9, LoadFrom loadFrom);
	void addImageToRotation(Rotation *rotation, uint32 layer);
	void setImageCoordinatesOnPuzzle(const Common::Point &point);
	void setImageCoordinatesOnPuzzle(uint32 imageIndex, const Common::Point &point);
	void setImageOriginalCoordinatesOnPuzzle();
	Common::Point getImageCoordinatesOnPuzzle(uint32 imageIndex);

	// Animation
	void addAnimationToPuzzle(Puzzle *puzzle, Common::String filename, ImageType imageType, const Common::Point &point, uint32 a7, uint32 a8, uint32 priority, byte frameCount, uint32 a11, float a12, byte a13, LoadFrom loadFrom);
	void addAnimationToRotation(Rotation *rotation, uint32 layer, uint32 a3, float a4, uint32 a5);
	void setAnimationOnPuzzle(uint32 animationIndex, ObjectId objectId);
	void setAnimationOnRotation(uint32 animationIndex, ObjectId objectId);
	void setAnimationStartFrame(uint32 startFrame);
	void setAnimationActiveFrame(uint32 activeFrame);
	void setAnimationCoordinatesOnPuzzle(const Common::Point &point);
	void pauseAnimation();
	void unpauseAnimation();
	void pauseFrameAnimation(uint32 frame, uint32 a3, uint32 a4);

	// Show/Hide
	void show();
	void hide();
	void hideAndRemove();

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

	// Accessors
	bool isShown() { return _isShown; }

private:
	Object *_object;
	bool _isShown;
	Common::Array<ImageHandle *>    _imagePuzzle;
	Common::Array<Puzzle *>         _imagePuzzlePtr;
	Common::Array<AnimationImage *> _animationPuzzle;
	Common::Array<Puzzle *>         _animationPuzzlePtr;
	Common::Array<BaseId *>         _layerImagePtr;
	Common::Array<Rotation *>       _layImageRotationPtr;
	Common::Array<BaseId *>         _layerAnimationRotation;
	Common::Array<Rotation *>       _layerAnimationRotationPtr;
	Common::Array<Animation *>      _layerAnimationRotationAnimation;
	Common::Array<Text *>           _textPuzzle;
	Common::Array<Puzzle *>         _textPuzzlePtr;
	Common::Array<Text *>           _textRotation;
	Common::Array<Rotation *>       _textRotationPtr;
};

class Object : public BaseObject, public Common::Serializable {
public:
	Object(Application *application, ObjectId id, Common::String description, Common::String name, byte a5);
	~Object();

	// Presentation
	void addPresentation();
	void addTextToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String text, const Common::Point &point, FontId fontId, Color foreground, Color background);
	void setTextToPuzzle(uint32 presentationIndex, uint32 textIndex, Common::String text);
	void setTextCoordinatesToPuzzle(uint32 presentationIndex, uint32 textIndex, const Common::Point &point);
	uint32 getTextWidth(uint32 presentationIndex, uint32 textIndex);

	void addImageToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String filename, const Common::Point &point, bool isActive, byte a8, uint32 priority, byte a10, LoadFrom loadFrom);
	void addImageToRotation(uint32 presentationIndex, Rotation *rotation, uint32 layer);
	void setImageCoordinatesOnPuzzle(uint32 presentationIndex, const Common::Point &point);
	void setImageCoordinatesOnPuzzle(uint32 presentationIndex, uint32 imageIndex, const Common::Point &point);
	void setImageOriginalCoordinatesOnPuzzle(uint32 presentationIndex);
	Common::Point getImageCoordinatesOnPuzzle(uint32 presentationIndex, uint32 imageIndex);

	void show(uint32 presentationIndex);
	void show();
	void hide(uint32 presentationIndex);
	void hide();
	void hideAndRemove(uint32 presentationIndex);
	void hideAndRemove();

	// Accessibility
	void addPuzzleAccessibility(Puzzle *puzzle, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);
	void addRotationAccessibility(Rotation *rotation, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);
	void setAccessibilityKey(uint32 accessibilityIndex, Common::KeyCode key);
	void setAccessibilityOnOrOff(bool enableHotspot);
	void setAccessibilityOnOrOff(bool enableHotspot, uint32 fromAcceleration, uint32 toAcceleration);

	// Animation
	void addAnimationToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String name, ImageType imageType, const Common::Point &point, uint32 a8, uint32 a9, uint32 priority, byte frameCount, uint32 a12, float a13, byte a14, LoadFrom loadFrom);
	void addAnimationToRotation(uint32 presentationIndex, Rotation *rotation, uint32 layer, uint32 a5, float a6, uint32 a7);
	void setAnimationOnPuzzle(uint32 presentationIndex, uint32 animationIndex, const ObjectId &objectId);
	void setAnimationOnRotation(uint32 presentationIndex, uint32 animationIndex, const ObjectId &objectId);
	void setAnimationStartFrame(uint32 presentationIndex, uint32 startFrame);
	void setAnimationActiveFrame(uint32 presentationIndex, uint32 activeFrame);
	void setAnimationCoordinatesOnPuzzle(uint32 presentationIndex, const Common::Point &point);
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
	AnimationImage *getAnimationImage() { return _animationImage; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

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

	Common::String _description;
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
	ObjectInfo(ObjectId id, Common::String description, Common::String name);
	~ObjectInfo() {}

	// Accessors
	Common::String getDescription() { return _description; }
	Common::String getName() { return _name; }

private:
	Common::String _description;
	Common::String _name;
};

class ObjectHandler {
public:
	ObjectHandler();
	~ObjectHandler();

	void addFromFile(Common::String filename, Common::String description);

	// Accessors
	Common::String getDescription(ObjectId id);
	Common::String getName(ObjectId id);

private:
	AssociativeArray<ObjectInfo *> _objects;
};

} // End of namespace Ring

#endif // RING_OBJECT_H
