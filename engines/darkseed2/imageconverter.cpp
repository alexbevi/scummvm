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

#include "engines/darkseed2/imageconverter.h"
#include "engines/darkseed2/palette.h"

DECLARE_SINGLETON(DarkSeed2::ImageConverter)

namespace DarkSeed2 {

ImageConverter::ImageConverter() {
}

ImageConverter::~ImageConverter() {
}

void ImageConverter::registerStandardPalette(Palette &palette) {
	_palettes.push(&palette);
}

void ImageConverter::unregisterStandardPalette() {
	if (_palettes.empty())
		return;

	_palettes.pop();
}

const ::Graphics::PixelFormat &ImageConverter::getPixelFormat() const {
	return _format;
}

void ImageConverter::setPixelFormat(const ::Graphics::PixelFormat &format) {
	_format = format;
}

void ImageConverter::convert8bit(::Graphics::Surface &trueColor,
		const ::Graphics::Surface &paletted, const Palette &palette) const {

	assert(trueColor.pixels && paletted.pixels);
	assert(trueColor.w == paletted.w);
	assert(trueColor.h == paletted.h);

	assert(paletted.bytesPerPixel == 1);
	assert(trueColor.bytesPerPixel == _format.bytesPerPixel);

	if (palette.empty() && (&palette != getStandardPalette()))
		// If the palette empty and not the standard palette, use that one instead
		return convert8bit(trueColor, paletted);

	// For now, we only support 8bit->16bit conversion
	assert(trueColor.bytesPerPixel == 2);

	uint16 *dst = (uint16 *) trueColor.pixels;
	const byte *src = (const byte *) paletted.pixels;

	for (int32 y = 0; y < paletted.h; y++)
		for (int32 x = 0; x < paletted.w; x++, dst++, src++)
			*dst = (uint16) getColor(*src, palette);
}

void ImageConverter::convert8bit(::Graphics::Surface &trueColor,
		const ::Graphics::Surface &paletted) const {

	Palette *stdPalette = getStandardPalette();
	assert(stdPalette);

	return convert8bit(trueColor, paletted, *stdPalette);
}

void ImageConverter::convert8bitSystem(::Graphics::Surface &trueColor,
		const ::Graphics::Surface &paletted) const {

	assert(trueColor.pixels && paletted.pixels);
	assert(trueColor.w == paletted.w);
	assert(trueColor.h == paletted.h);

	assert(paletted.bytesPerPixel == 1);
	assert(trueColor.bytesPerPixel == _format.bytesPerPixel);

	// For now, we only support 8bit->16bit conversion
	assert(trueColor.bytesPerPixel == 2);

	uint16 *dst = (uint16 *) trueColor.pixels;
	const byte *src = (const byte *) paletted.pixels;

	for (int32 y = 0; y < paletted.h; y++)
		for (int32 x = 0; x < paletted.w; x++, dst++, src++)
			*dst = (uint16) getColorSystem(*src);
}

uint32 ImageConverter::readColor(const byte *img) const {
	if (_format.bytesPerPixel == 2)
		return *((uint16 *) img);

	return 0;
}

void ImageConverter::writeColor(byte *img, uint32 color) const {
	if (_format.bytesPerPixel == 2)
		*((uint16 *) img) = (uint16) color;
}

void ImageConverter::swapColor(byte *img1, byte *img2) const {
	if (_format.bytesPerPixel == 2)
		SWAP(*((uint16 *) img1), *((uint16 *) img2));
}

void ImageConverter::mixTrueColor(byte *dst, const byte *src) {
	assert(_format.bytesPerPixel == 2);

	uint32 c1 = *((uint16 *) dst);
	uint32 c2 = *((uint16 *) src);

	uint8 r1, g1, b1, r2, g2, b2;
	_format.colorToRGB(c1, r1, g1, b1);
	_format.colorToRGB(c2, r2, g2, b2);

	const uint8  r3 = (r1 + r2) / 2;
	const uint8  g3 = (g1 + g2) / 2;
	const uint8  b3 = (b1 + b2) / 2;
	const uint32 c3 = _format.RGBToColor(r3, g3, b3);

	*((uint16 *) dst) = c3;
}

uint32 ImageConverter::convertColor(uint8 c, const Palette &palette) const {
	if (palette.empty() && (&palette != getStandardPalette()))
		// If the palette empty and not the standard palette, use that one instead
		return convertColor(c);

	return getColor(c, palette);
}

uint32 ImageConverter::convertColor(uint8 c) const {
	Palette *stdPalette = getStandardPalette();
	assert(stdPalette);

	return convertColor(c, *stdPalette);
}

uint32 ImageConverter::getColor(uint8 r, uint8 g, uint8 b) const {
	return _format.RGBToColor(r, g, b);
}

void ImageConverter::getColorComponents(uint32 color, uint8 &r, uint8 &g, uint8 &b) const {
	_format.colorToRGB(color, r, g, b);
}

inline Palette *ImageConverter::getStandardPalette() const {
	if (_palettes.empty())
		return 0;

	return _palettes.top();
}

inline uint32 ImageConverter::getColor(uint8 c, const Palette &palette) const {
	const uint8 r = palette[c * 3 + 0];
	const uint8 g = palette[c * 3 + 1];
	const uint8 b = palette[c * 3 + 2];

	return _format.RGBToColor(r, g, b);
}

inline uint32 ImageConverter::getColorSystem(uint8 c) const {
	byte pal[4];

	g_system->grabPalette(pal, c, 1);

	const uint8 r = pal[0];
	const uint8 g = pal[1];
	const uint8 b = pal[2];

	return _format.RGBToColor(r, g, b);
}

} // End of namespace DarkSeed2
