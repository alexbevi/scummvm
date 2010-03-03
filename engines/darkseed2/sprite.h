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
#include "graphics/surface.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/saveable.h"
#include "engines/darkseed2/palette.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class NECursor;

class Resources;

class Resource;

class Sprite : public Saveable {
public:
	Sprite();
	Sprite(const Sprite &sprite);
	~Sprite();

	Sprite &operator=(const Sprite &sprite);

	/** Does the sprite have any data? */
	bool exists() const;

	/** Return the sprite's width. */
	int32 getWidth(bool unscaled = false) const;
	/** Return the sprite's height. */
	int32 getHeight(bool unscaled = false) const;

	/** Return the sprite's default X coordinate. */
	int32 getDefaultX(bool unscaled = false) const;
	/** Return the sprite's default Y coordinate. */
	int32 getDefaultY(bool unscaled = false) const;

	/** Return the sprite's "feet" X coordinate. */
	int32 getFeetX(bool unscaled = false) const;
	/** Return the sprite's "feet" Y coordinate. */
	int32 getFeetY(bool unscaled = false) const;

	/** Return the sprite's area. */
	Common::Rect getArea(bool unscaled = false) const;

	/** Return the sprite's paletted data. */
	const ::Graphics::Surface &getPaletted() const;
	/** Return the sprite's true color data. */
	const ::Graphics::Surface &getTrueColor() const;

	/** Return the sprite's palette. */
	const Palette &getPalette() const;

	/** Create a new sprite with the specified dimensions. */
	void create(int32 width, int32 height);
	/** Discard the sprite data. */
	void discard();

	/** Copy from another sprite. */
	void copyFrom(const Sprite &sprite);
	/** Copy from a buffer. */
	void copyFrom(const byte *sprite, bool system = false);

	/** Load a sprite from an image file. */
	bool loadFromImage(Resources &resources, const Common::String &image);

	/** Load a sprite from a room image file. */
	bool loadFromRoomImage(Resources &resources, const Common::String &image);

	/** Load from a cursor resource embedded in an EXE file. */
	bool loadFromCursorResource(const NECursor &cursor);

	/** Flip the sprite horizontally. */
	void flipHorizontally();
	/** Flip the sprite vertically. */
	void flipVertically();

	/** Blit that sprite onto this sprite. */
	void blit(const Sprite &from, const Common::Rect &area,
			int32 x, int32 y, bool transp = false);
	/** Blit that sprite onto this sprite. */
	void blit(const Sprite &from, int32 x, int32 y, bool transp = false);

	/** Fill the whole sprite with one palette entry. */
	void fill(byte c);
	/** Fill the whole sprite with one color entry. */
	void fill(uint32 c);
	/** Fill the whole sprite with palette entry 0, making it completely transparent. */
	void clear();
	/** Fill the whole sprite with the color black. */
	void darken();

	/** Shade the sprite. */
	void shade(uint32 c);

	/** Draw a string. */
	void drawStrings(const Common::StringList &strings, const ::Graphics::Font &font,
			int x, int y, uint32 color);

	/** Get the scaling value. */
	frac_t getScale() const;
	/** Set the scaling value. */
	void setScale(frac_t scale);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	Common::String _fileName; ///< The file from which the sprite was loaded.
	bool _fromCursor; ///< Was the sprite loaded from a cursor resource?

	::Graphics::Surface _surfacePaletted;  ///< The sprite's (original) paletted data.
	::Graphics::Surface _surfaceTrueColor; ///< The sprite's true color data.

	uint8 *_transparencyMap; ///< The sprite's transparency map.

	int32 _defaultX; ///< The sprite's default X coordinate.
	int32 _defaultY; ///< The sprite's default Y coordinate.

	int32 _feetX; ///< The sprite's "feet" X coordinate.
	int32 _feetY; ///< The sprite's "feet" Y coordinate.

	bool _flippedHorizontally; ///< Sprite was flipped horizontally.
	bool _flippedVertically;   ///< Sprite was flipped vertically.

	Palette _palette; ///< The sprite's palette.

	frac_t _scale;        ///< The sprite's current scaling value.
	frac_t _scaleInverse; ///< The inverse value to the current scaling value.

	/** Clear/Initialize. */
	void clearData();

	/** Convert the paletted 8bit data to true color data. */
	void convertToTrueColor(bool system = false);

	/** Create the transparency map. */
	void createTransparencyMap();
	/** Update the transparency map after drawing to the true color surface. */
	void updateTransparencyMap();

	void fillImage(byte cP, uint32 cT);

	/** Load a sprite from a BMP. */
	bool loadFromBMP(Resources &resources, const Common::String &bmp);
	/** Load a sprite from a BMP. */
	bool loadFromBMP(Common::SeekableReadStream &bmp);

	/** Load a sprite from a RGB. */
	bool loadFromRGB(Resources &resources, const Common::String &rgb);
	/** Load a sprite from a RGB. */
	bool loadFromRGB(Common::SeekableReadStream &rgb);

	/** Load a sprite from a BDP. */
	bool loadFromBDP(Resources &resources, const Common::String &rgb);
	/** Load a sprite from a BDP. */
	bool loadFromBDP(Common::SeekableReadStream &bdp);

	uint32 readColor555(Common::SeekableReadStream &stream) const;

	void loadPalette(Common::SeekableReadStream &stream, uint32 count);

	/** Read uncompressed BMP data. */
	bool readBMPDataComp0(Common::SeekableReadStream &bmp, uint32 dataSize);
	/** Read BMP data, compressed with method 2. */
	bool readBMPDataComp2(Common::SeekableReadStream &bmp, uint32 dataSize);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_SPRITE_H
