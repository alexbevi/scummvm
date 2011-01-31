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

#include "ring/game/puzzle.h"

#include "ring/base/text.h"
#include "ring/base/sound.h"

#include "ring/graphics/accessibility.h"
#include "ring/graphics/movability.h"
#include "ring/graphics/presentation.h"

#include "ring/helpers.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Puzzle
Puzzle::Puzzle(PuzzleId id) : BaseObject(id) {
	_field_4 = 0;
	_field_24 = 1;
	_field_28 = 0;
	_field_29 = 0;
}

Puzzle::~Puzzle() {
	CLEAR_ARRAY(Movability, _movabilities);
	CLEAR_ARRAY(Accessibility, _accessibilities);
	CLEAR_ARRAY(PresentationImage, _presentationImages);
	CLEAR_ARRAY(PresentationAnimation, _presentationAnimations);
	CLEAR_ARRAY(Text, _texts);
	CLEAR_ARRAY(SoundItem, _soundItems);
	CLEAR_ARRAY(Visual, _visuals);
}

} // End of namespace Ring
