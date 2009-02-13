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

#ifndef INNOCENT_TYPES_H
#define INNOCENT_TYPES_H

namespace Innocent {
//
class Graphics;

class Paintable {
public:
	virtual ~Paintable() {}
	virtual void paint(Graphics *g) const = 0;
	virtual byte zIndex() const = 0;
};

enum StringSpecial {
	kStringCountSpacesTerminate = 2,
	kStringMove = 3,
	kStringMenuOption = 5,
	kStringGlobalWord = 6,
	kStringSetColour = 7,
	kStringDefaultColour = 8,
	kStringAdvance = 9,
	kStringCountSpacesIf0 = 0x0a,
	kStringCountSpacesIf1 = 0x0b,
	kStringCenter = 12
};


enum FramePart {
	kFrameTopLeft,
	kFrameTop,
	kFrameTopRight,
	kFrameLeft,
	kFrameFill,
	kFrameRight,
	kFrameBottomLeft,
	kFrameBottom,
	kFrameBottomRight,
	kFrameNum
};

enum SpeechBubblePart {
	kBubbleTopLeft,
	kBubbleLeft,
	kBubbleBottomLeft,
	kBubbleTop,
	kBubbleFill,
	kBubbleBottom,
	kBubbleTopRight,
	kBubbleRight,
	kBubbleBottomRight,

	kBubbleBottomLeftPoint,
	kBubbleBottomRightPoint,
	kBubbleTopLeftPoint,
	kBubbleTopRightPoint
};

} // end of namespace Innocent

#endif
