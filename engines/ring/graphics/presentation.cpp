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

#include "ring/graphics/presentation.h"

#include "ring/base/text.h"

#include "ring/game/application.h"
#include "ring/game/puzzle.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/image.h"
#include "ring/graphics/rotation.h"

#include "ring/ring.h"
#include "ring/helpers.h"

namespace Ring {

#pragma region Presentation

PresentationImage::PresentationImage() {
}

PresentationImage::~PresentationImage() {
}

#pragma endregion

#pragma region Presentation

ObjectPresentation::ObjectPresentation(Object *object) : _object(object) {
	_isShown = false;
}

ObjectPresentation::~ObjectPresentation() {
	CLEAR_ARRAY(ImageHandle, _imagePuzzle);
	CLEAR_ARRAY(Puzzle, _imagePuzzlePtr);
	CLEAR_ARRAY(AnimationImage, _animationPuzzle);
	CLEAR_ARRAY(Puzzle, _animationPuzzlePtr);
	CLEAR_ARRAY(uint32, _layerImagePtr);
	CLEAR_ARRAY(Rotation, _layImageRotationPtr);
	CLEAR_ARRAY(uint32, _layerAnimationRotation);
	CLEAR_ARRAY(Rotation, _layerAnimationRotationPtr);
	CLEAR_ARRAY(AnimationImage, _layerAnimationRotationAnimation);
	CLEAR_ARRAY(Text, _textPuzzle);
	CLEAR_ARRAY(Puzzle, _textPuzzlePtr);
	CLEAR_ARRAY(Text, _textRotation);
	CLEAR_ARRAY(Rotation, _textRotationPtr);
}

void ObjectPresentation::addTextToPuzzle(Puzzle *puzzle, Common::String text, uint32 a4, uint32 a5, FontId fontId, byte a7, byte a8, byte a9, int32 a10, int32 a11, int32 a12) {
	Text *textObject = new Text();
	textObject->init(text, a4, a5, fontId, a7, a8, a9, a10, a11, a12);
	textObject->setObjectPresentation(this);

	_textPuzzle.push_back(textObject);
	_textPuzzlePtr.push_back(puzzle);

	puzzle->addPresentationText(textObject);
}

void ObjectPresentation::addImageToPuzzle(Puzzle *puzzle, Common::String filename, Common::Point point, bool isActive, byte a7, uint32 priority, byte a9, LoadFrom loadFrom) {
	ImageHandle *image = new ImageHandle(filename, point, isActive, a7, priority, a9, getApp()->getCurrentZone(), loadFrom, 4, getApp()->getArchiveType());
	image->setObjectPresentation(this);

	_imagePuzzle.push_back(image);
	_imagePuzzlePtr.push_back(puzzle);

	puzzle->addPresentationImage(image);
}

void ObjectPresentation::addAnimationToPuzzle(Puzzle *puzzle, Common::String filename, uint32 a4, Common::Point point, uint32 a7, uint32 a8, uint32 priority, uint32 a10, uint32 frameCount, uint32 a12, uint32 a13, LoadFrom loadFrom) {
	AnimationImage *animation = new AnimationImage();
	animation->init(filename, a4, point, 0, a8, frameCount, a12, 1, a13, a10, priority, loadFrom, getApp()->getArchiveType());
	animation->updatePresentation(this);
	if (!(a13 & 2)) {
		animation->setField20(0);
		animation->setField89();
	}

	_animationPuzzle.push_back(animation);
	_animationPuzzlePtr.push_back(puzzle);

	animation->updateCurrentImage();

	puzzle->addPresentationImage(animation->getCurrentImage());
	puzzle->addPresentationAnimation(this);
}

void ObjectPresentation::setAnimationCoordinatesOnPuzzle(Common::Point point) {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->setCoordinates(point);
}

void ObjectPresentation::show() {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->setTicks(g_system->getMillis());

	for (Common::Array<AnimationImage *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->setTicks(g_system->getMillis());

	for (uint32 i = 0; i < _layerImagePtr.size(); i++)
		_layImageRotationPtr[i]->updateData(*(_layerImagePtr[i]), 1);

	for (uint32 i = 0; i < _layerAnimationRotation.size(); i++)
		_layerAnimationRotationPtr[i]->updateData(*(_layerAnimationRotation[i]), 1);
}

void ObjectPresentation::hide() {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->sub_416710();

	for (Common::Array<AnimationImage *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->sub_416710();

	for (uint32 i = 0; i < _layerImagePtr.size(); i++)
		_layImageRotationPtr[i]->updateData(*(_layerImagePtr[i]), 0);

	for (uint32 i = 0; i < _layerAnimationRotation.size(); i++)
		_layerAnimationRotationPtr[i]->updateData(*(_layerAnimationRotation[i]), 0);
}

void ObjectPresentation::hideAndRemove() {
	error("[ObjectPresentation::hide] Not implemented");
}

#pragma endregion

} // End of namespace Ring
