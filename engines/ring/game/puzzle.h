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

#ifndef RING_PUZZLE_H
#define RING_PUZZLE_H

#include "ring/shared.h"

namespace Ring {

class Accessibility;
class ImageHandle;
class Movability;
class PresentationAnimation;
class PresentationImage;
class SoundItem;
class Text;
class Visual;

class Puzzle : public BaseObject {
public:
	Puzzle(PuzzleId id);
	~Puzzle();

	void addPresentationText(Text *text);
	void addPresentationImage(ImageHandle *image);
	void addAccessibility(Accessibility *accessibility);

private:
	uint32 _field_4;
	Common::Array<Movability *>            _movabilities;
	Common::Array<Accessibility *>         _accessibilities;
	Common::Array<ImageHandle *>           _presentationImages;
	Common::Array<PresentationAnimation *> _presentationAnimations;
	Common::Array<Text *>                  _texts;
	Common::Array<SoundItem *>             _soundItems;
	Common::Array<Visual *>                _visuals;
	uint32 _field_24;
	byte   _field_28;
	uint32 _field_29;

	static bool imagePriorityCompare(ImageHandle *image1, ImageHandle *image2);
};

} // End of namespace Ring

#endif // RING_PUZZLE_H
