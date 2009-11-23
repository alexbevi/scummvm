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

#ifndef DARKSEED2_SPRITE_H
#define DARKSEED2_SPRITE_H

#include "common/rect.h"
#include "common/str.h"

#include "graphics/font.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/palette.h"

namespace Common {
	class SeekableReadStream;
}

namespace Graphics {
	struct Surface;
}

namespace DarkSeed2 {

class Resources;
class Resource;
class StaticCursor;

class Sprite {
public:
	Sprite();
	~Sprite();

	/** Does the sprite have any data? */
	bool exists() const;

	uint32 getWidth() const;
	uint32 getHeight() const;

	const byte *getData() const;
	const Palette &getPalette() const;

	/** Create a new sprite with the specified dimensions. */
	void create(uint32 width, uint32 height);
	/** Discard the sprite data. */
	void discard();

	bool loadFromBMP(Common::SeekableReadStream &bmp);
	bool loadFromBMP(const Resource &resource);
	bool loadFromBMP(const Resources &resources, const Common::String &bmp);

	/** Load from a cursor embedded in an EXE file. */
	bool loadFromStaticCursor(const StaticCursor &staticCursor);

	void blit(const Sprite &from,
			uint32 left, uint32 top, uint32 right, uint32 bottom,
			uint32 x, uint32 y, bool transp = false);
	void blit(const Sprite &from, uint32 x, uint32 y, bool transp = false);

	/** Fill the whole data with palette entry 0. */
	void clear();

	void drawStrings(const Common::StringList &strings, const ::Graphics::Font &font,
			int x, int y, byte color);

private:
	uint32 _width;
	uint32 _height;
	byte *_data;

	Palette _palette;

	::Graphics::Surface *wrapInSurface() const;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_SPRITE_H
