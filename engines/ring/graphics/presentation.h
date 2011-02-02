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
class AnimationImage;
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

class Visual {
public:
	Visual();
	~Visual();

private:

};

class PresentationAnimation {
public:
	PresentationAnimation();
	~PresentationAnimation();

private:

};

class ObjectPresentation {
public:
	ObjectPresentation(Application *application, Object *object);
	~ObjectPresentation();

	void addTextToPuzzle(Puzzle *puzzle, Common::String text, int a4, int a5, FontId fontId, byte a7, char a8, byte a9, int a10, int a11, int a12);

private:
	Application *_application;

	Object *_object;
	byte _field_4;
	Common::Array<PresentationImage *> _imagePuzzle;
	Common::Array<Puzzle *> _imagePuzzlePtr;
	Common::Array<AnimationImage *> _animationPuzzle;
	Common::Array<Puzzle *> _animationPuzzlePtr;
	Common::Array<uint32 *> _layImagePtr;
	Common::Array<Rotation *> _layImageRotationPtr;
	Common::Array<uint32 *> _layAnimationRotation;
	Common::Array<Rotation *> _layAnimationRotationPtr;
	Common::Array<AnimationImage *> _layAnimationRotationAnimation;
	Common::Array<Text *> _textPuzzle;
	Common::Array<Puzzle *> _textPuzzlePtr;
	Common::Array<Text *> _textRotation;
	Common::Array<Rotation *> _textRotationPtr;
};

} // End of namespace Ring

#endif // RING_PRESENTATION_H
