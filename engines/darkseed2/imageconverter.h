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

#ifndef DARKSEED2_IMAGECONVERTER_H
#define DARKSEED2_IMAGECONVERTER_H

#include "common/singleton.h"
#include "common/stack.h"

#include "graphics/pixelformat.h"
#include "graphics/surface.h"

namespace DarkSeed2 {

class Palette;

/** A class to convert images of various depths to a specific true color mode. */
class ImageConverter : public Common::Singleton<ImageConverter> {
public:
	/** Register a standard palette for 8bit images. */
	void registerStandardPalette(Palette &palette);
	/** Unregister the standard palette. */
	void unregisterStandardPalette();

	/** Set the target pixel format. */
	void setPixelFormat(const ::Graphics::PixelFormat &format);

	/** Convert an 8bit image (using the specified palette). */
	void convert8bit(::Graphics::Surface &trueColor,
			const ::Graphics::Surface &paletted, const Palette &palette) const;
	/** Convert an 8bit image (using the standard palette). */
	void convert8bit(::Graphics::Surface &trueColor,
			const ::Graphics::Surface &paletted) const;
	/** Convert an 8bit image (using the system palette). */
	void convert8bitSystem(::Graphics::Surface &trueColor,
			const ::Graphics::Surface &paletted) const;

	/** Mix one pixel to another. */
	void mixTrueColor(byte *dst, const byte *src);

	/** Convert a color value (using the specified palette). */
	uint32 convertColor(uint8 c, const Palette &palette) const;
	/** Convert a color value (using the standard palette). */
	uint32 convertColor(uint8 c) const;

	/** Return a color in the target format. */
	uint32 getColor(uint8 r, uint8 g, uint8 b) const;

	/** Return the color components in the target format. */
	void getColorComponents(uint32 color, uint8 &r, uint8 &g, uint8 &b) const;

private:
	friend class Common::Singleton<SingletonBaseType>;

	::Graphics::PixelFormat _format; ///< The target format.

	Common::Stack<Palette *> _palettes; ///< The standard palette stack.

	ImageConverter();
	~ImageConverter();

	inline Palette *getStandardPalette() const;
	inline uint32 getColor(uint8 c, const Palette &palette) const;
	inline uint32 getColorSystem(uint8 c) const;
};

} // End of namespace DarkSeed2

#define ImgConv (::DarkSeed2::ImageConverter::instance())

#endif // DARKSEED2_IMAGECONVERTER_H
