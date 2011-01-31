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

#ifndef RING_TEXT_H
#define RING_TEXT_H

#include "ring/shared.h"

namespace Ring {

class Application;

class Text {
public:
	Text(Application *application);
	~Text();

	void init(Common::String text, uint32 a1, uint32 a2, FontId fontId, byte a4, byte a5, byte a6, uint32 a7, uint32 a8, uint32 a9);
	void set(Common::String text);

private:
	Application *_application;

	// Data
	Common::String _text;
	FontId _fontId;
	uint32 _field_8;
	uint32 _field_C;
	uint32 _field_10;
	uint32 _width;
	uint32 _height;
	byte _field_1C;
	uint32 _field_1D;
	uint32 _field_21;
};

} // End of namespace Ring

#endif // RING_TEXT_H
