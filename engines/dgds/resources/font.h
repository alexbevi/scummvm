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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/resources/font.h $
 * $Id: font.h 19 2010-01-24 00:40:01Z alexbevi $
 *
 */

#ifndef DGDS_FONT_H
#define DGDS_FONT_H

#include "graphics/surface.h"
#include "dgds/resource.h"

namespace Dgds {

class Font {
public:
	Font(Resource *res);
	~Font();

	bool drawChar(Graphics::Surface *surf, int screenx, int screeny, char c, int color);
	bool drawString(Graphics::Surface *surf, int screenx, int screeny, const char *str, int color);

	void show(Graphics::Surface *surf, int screenx, int screeny, int color);

private:
	void empty();
	void loadFNT(Resource *resFNT);

	bool  hasChar(char c);
	uint16 getCharOffset(char c);
	uint8  getCharWidth(char c);
	uint8  getLineHeight(void);

	byte _sizex;
	byte _sizey;
	byte _firstChar;
	byte _numChars;

	uint8 _lineHeight;

	uint16 *_offsets;
	uint8 *_widths;
	byte *_faces;
};

} // End of namespace Dgds

#endif // DGDS_FONT_H
