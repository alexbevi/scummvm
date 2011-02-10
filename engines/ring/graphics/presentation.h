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

#ifndef RING_PRESENTATION_H
#define RING_PRESENTATION_H

#include "ring/shared.h"

namespace Ring {

class Application;
class Animation;
class AnimationImage;
class ImageHandle;
class Object;
class Puzzle;
class Rotation;
class Text;

class PresentationImage {
public:
	PresentationImage();
	~PresentationImage();

private:

};

class ObjectPresentation {
public:
	ObjectPresentation(Object *object);
	~ObjectPresentation();

	// Text
	void addTextToPuzzle(Puzzle *puzzle, Common::String text, const Common::Point &point, FontId fontId, byte a7, byte a8, byte a9, int32 a10, int32 a11, int32 a12);

	// Image
	void addImageToPuzzle(Puzzle *puzzle, Common::String filename, const Common::Point &point, bool isActive, byte a7, uint32 priority, byte a9, LoadFrom loadFrom);
	void addImageToRotation(Rotation *rotation, uint32 layer);

	// Animation
	void addAnimationToPuzzle(Puzzle *puzzle, Common::String filename, uint32 a4, const Common::Point &point, uint32 a7, uint32 a8, uint32 priority, byte frameCount, uint32 a11, float a12, byte a13, LoadFrom loadFrom);
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

} // End of namespace Ring

#endif // RING_PRESENTATION_H
