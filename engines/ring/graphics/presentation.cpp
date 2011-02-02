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

#include "ring/game/puzzle.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/rotation.h"


#include "ring/helpers.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Visual
//////////////////////////////////////////////////////////////////////////
Visual::Visual() {
}

Visual::~Visual() {
}

//////////////////////////////////////////////////////////////////////////
// Presentation
//////////////////////////////////////////////////////////////////////////
PresentationAnimation::PresentationAnimation() {
}

PresentationAnimation::~PresentationAnimation() {
}

//////////////////////////////////////////////////////////////////////////
// Presentation
//////////////////////////////////////////////////////////////////////////
PresentationImage::PresentationImage() {
}

PresentationImage::~PresentationImage() {
}

//////////////////////////////////////////////////////////////////////////
// Presentation
//////////////////////////////////////////////////////////////////////////
ObjectPresentation::ObjectPresentation(Object *object) : _object(object) {
	_field_4 = 0;
}

ObjectPresentation::~ObjectPresentation() {
	CLEAR_ARRAY(PresentationImage, _imagePuzzle);
	CLEAR_ARRAY(Puzzle, _imagePuzzlePtr);
	CLEAR_ARRAY(AnimationImage, _animationPuzzle);
	CLEAR_ARRAY(Puzzle, _animationPuzzlePtr);
	CLEAR_ARRAY(uint32, _layImagePtr);
	CLEAR_ARRAY(Rotation, _layImageRotationPtr);
	CLEAR_ARRAY(uint32, _layAnimationRotation);
	CLEAR_ARRAY(Rotation, _layAnimationRotationPtr);
	CLEAR_ARRAY(AnimationImage, _layAnimationRotationAnimation);
	CLEAR_ARRAY(Text, _textPuzzle);
	CLEAR_ARRAY(Puzzle, _textPuzzlePtr);
	CLEAR_ARRAY(Text, _textRotation);
	CLEAR_ARRAY(Rotation, _textRotationPtr);
}

} // End of namespace Ring
