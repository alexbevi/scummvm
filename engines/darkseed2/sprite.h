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
#include "common/array.h"
#include "common/frac.h"

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

class NECursor;

class Resources;

class Resource;

class Sprite {
public:
	Sprite();
	Sprite(const Sprite &sprite);
	~Sprite();

	Sprite &operator=(const Sprite &sprite);

	/** Does the sprite have any data? */
	bool exists() const;

	/** Return the sprite's width. */
	uint32 getWidth(bool unscaled = false) const;
	/** Return the sprite's height. */
	uint32 getHeight(bool unscaled = false) const;

	/** Return the sprite's default X coordinate. */
	uint16 getDefaultX(bool unscaled = false) const;
	/** Return the sprite's default Y coordinate. */
	uint16 getDefaultY(bool unscaled = false) const;

	/** Return the sprite's "feet" X coordinate. */
	uint16 getFeetX(bool unscaled = false) const;
	/** Return the sprite's "feet" Y coordinate. */
	uint16 getFeetY(bool unscaled = false) const;

	/** Return the sprite's area. */
	Common::Rect getArea(bool unscaled = false) const;

	/** Return the sprite's data. */
	const byte *getData() const;
	/** Return the sprite's data. */
	byte *getData();
	/** Return the sprite's palette. */
	const Palette &getPalette() const;

	/** Create a new sprite with the specified dimensions. */
	void create(uint32 width, uint32 height);
	/** Discard the sprite data. */
	void discard();

	/** Copy from another sprite. */
	void copyFrom(const Sprite &sprite);

	/** Load a sprite from a BMP. */
	bool loadFromBMP(Common::SeekableReadStream &bmp);
	/** Load a sprite from a BMP. */
	bool loadFromBMP(const Resource &resource);
	/** Load a sprite from a BMP. */
	bool loadFromBMP(Resources &resources, const Common::String &bmp);

	/** Load from a cursor resource embedded in an EXE file. */
	bool loadFromCursorResource(const NECursor &cursor);

	/** Flip the sprite horizontally. */
	void flipHorizontally();
	/** Flip the sprite vertically. */
	void flipVertically();

	/** Blit that sprite onto this sprite. */
	void blit(const Sprite &from, const Common::Rect &area,
			uint32 x, uint32 y, bool transp = false);
	/** Blit that sprite onto this sprite. */
	void blit(const Sprite &from, uint32 x, uint32 y, bool transp = false);

	/** Fill the whole sprite with one palette entry. */
	void fill(byte c);
	/** Fill the whole sprite with palette entry 0. */
	void clear();

	/** Fill the sprite with a "shading" grid. */
	void shade(byte c);

	/** Change pixels of color oldColor to newColor. */
	void recolor(byte oldColor, byte newColor);

	/** Apply a change set from a palette merge. */
	void applyChangeSet(const Common::Array<byte> &changeSet);

	/** Draw a string. */
	void drawStrings(const Common::StringList &strings, const ::Graphics::Font &font,
			int x, int y, byte color);

	/** Get the scaling value. */
	frac_t getScale() const;
	/** Set the scaling value. */
	void setScale(frac_t scale);

private:
	uint32 _width;  ///< The sprite's width.
	uint32 _height; ///< The sprite's height.
	byte  *_data;   ///< The sprite's data.

	uint16 _defaultX; ///< The sprite's default X coordinate.
	uint16 _defaultY; ///< The sprite's default Y coordinate.

	uint16 _feetX; ///< The sprite's "feet" X coordinate.
	uint16 _feetY; ///< The sprite's "feet" Y coordinate.

	Palette _palette; ///< The sprite's palette.

	frac_t _scale;        ///< The sprite's current scaling value.
	frac_t _scaleInverse; ///< The inverse value to the current scaling value.

	/** Wrap the sprite into a standard ScummVM surface. */
	::Graphics::Surface *wrapInSurface() const;

	/** Read uncompressed BMP data. */
	bool readBMPDataComp0(Common::SeekableReadStream &bmp, uint32 dataSize);
	/** Read BMP data, compressed with method 2. */
	bool readBMPDataComp2(Common::SeekableReadStream &bmp, uint32 dataSize);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_SPRITE_H
