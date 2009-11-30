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

#include "common/stream.h"

#include "graphics/surface.h"
#include "graphics/fontman.h"
#include "graphics/font.h"

#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/cursors.h"

namespace DarkSeed2 {

Sprite::Sprite() {
	_width  = 0;
	_height = 0;
	_data   = 0;

	_defaultX = 0;
	_defaultY = 0;
	_feetX    = 0;
	_feetY    = 0;
}

Sprite::Sprite(const Sprite &sprite) {
	_data = 0;
	copyFrom(sprite);
}

Sprite::~Sprite() {
	discard();
}

Sprite &Sprite::operator=(const Sprite &sprite) {
	copyFrom(sprite);
	return *this;
}

void Sprite::copyFrom(const Sprite &sprite) {
	create(sprite._width, sprite._height);

	memcpy(_data, sprite._data, _width * _height);

	_palette = sprite._palette;
}

bool Sprite::exists() const {
	return _data == 0;
}

uint32 Sprite::getWidth() const {
	return _width;
}

uint32 Sprite::getHeight() const {
	return _height;
}

uint16 Sprite::getDefaultX() const {
	return _defaultX;
}

uint16 Sprite::getDefaultY() const {
	return _defaultY;
}

uint16 Sprite::getFeetX() const {
	return _feetX;
}

uint16 Sprite::getFeetY() const {
	return _feetY;
}

Common::Rect Sprite::getArea() const {
	return Common::Rect(_width, _height);
}

const byte *Sprite::getData() const {
	return _data;
}

byte *Sprite::getData() {
	return _data;
}

const Palette &Sprite::getPalette() const {
	return _palette;
}

void Sprite::create(uint32 width, uint32 height) {
	discard();

	_width  = width;
	_height = height;

	_data = new byte[_width * _height];

	clear();
}

void Sprite::discard() {
	delete[] _data;

	_width  = 0;
	_height = 0;
	_data   = 0;

	_defaultX = 0;
	_defaultY = 0;
	_feetX    = 0;
	_feetY    = 0;

	_palette.clear();
}

bool Sprite::loadFromBMP(Common::SeekableReadStream &bmp) {
	discard();

	if (!bmp.seek(0))
		return false;

	uint32 fSize = bmp.size();

	//                         'BM'
	if (bmp.readUint16BE() != 0x424D)
		return false;

	// Size of image + reserved + reserved
	bmp.skip(8);

	uint32 bmpDataOffset = bmp.readUint32LE();
	if (bmpDataOffset >= fSize)
		return false;

	// Header size
	if (bmp.readUint32LE() != 40)
		return false;

	_width  = bmp.readUint32LE();
	_height = bmp.readUint32LE();

	// Number of color planes
	if (bmp.readUint16LE() != 1)
		return false;

	// Bits per pixel
	if (bmp.readUint16LE() != 8)
		return false;

	uint32 compression = bmp.readUint32LE();

	if ((compression != 0) && (compression != 2))
		return false;

	uint32 bmpDataSize = bmp.readUint32LE();

	_feetX = MIN<uint16>(ABS(((int16) bmp.readUint16LE())), _width );
	_feetY = MIN<uint16>(ABS(((int16) bmp.readUint16LE())), _height);

	// Default coordinates
	_defaultX = bmp.readUint16LE();
	_defaultY = bmp.readUint16LE();

	uint32 numPalColors = bmp.readUint32LE();
	if (numPalColors == 0)
		numPalColors = 256;
	if (numPalColors > 256)
		numPalColors = 256;

	// Important colors
	bmp.skip(4);

	byte *palette = new byte[numPalColors * 3];
	for (uint32 i = 0; i < numPalColors; i++) {
		palette[i * 3 + 2] = bmp.readByte();
		palette[i * 3 + 1] = bmp.readByte();
		palette[i * 3 + 0] = bmp.readByte();

		bmp.readByte();
	}
	_palette.copyFrom(palette, numPalColors);
	delete[] palette;

	_data = new byte[_width * _height];
	memset(_data, 0, _width * _height);

	if (!bmp.seek(bmpDataOffset))
		return false;

	if (compression == 0) {
		if (!readBMPDataComp0(bmp, bmpDataSize))
			return false;
	} else if (compression == 2) {
		if (!readBMPDataComp2(bmp, bmpDataSize))
			return false;
	}

	for (int i = MAX<int>(0, _feetX - 2); i < MIN<int>(_feetX + 2, _width - 1); i++)
		for (int j = MAX<int>(0, _feetY - 2); j < MIN<int>(_feetY + 2, _height - 1); j++)
				_data[j * _width + i] = 1;

	return true;
}

bool Sprite::loadFromBMP(const Resource &resource) {
	return loadFromBMP(resource.getStream());
}

bool Sprite::loadFromBMP(const Resources &resources, const Common::String &bmp) {
	if (!resources.hasResource(bmp + ".BMP"))
		return false;

	Resource *resBMP = resources.getResource(bmp + ".BMP");

	bool result = loadFromBMP(*resBMP);

	delete resBMP;

	return result;
}

bool Sprite::loadFromStaticCursor(const StaticCursor &staticCursor) {
	create(Cursors::_cursorWidth, Cursors::_cursorHeight);

	_palette[2 * 3 + 0] = 255;
	_palette[2 * 3 + 1] = 255;
	_palette[2 * 3 + 2] = 255;

	byte *data = _data;
	for (int i = 0; i < (Cursors::_cursorWidth * Cursors::_cursorHeight / 8); i++) {
		byte p = staticCursor.pixels[i];
		byte m = staticCursor.mask[i];

		for (int j = 0; j < 8; j++, data++, p <<= 1, m <<= 1) {
			if ((m & 0x80) == 0x80) {
				if ((p & 0x80) == 0x80)
					*data = 2;
				else
					*data = 1;
			} else
				*data = 0;
		}
	}

	return true;
}

void Sprite::blit(const Sprite &from, const Common::Rect &area,
		uint32 x, uint32 y, bool transp) {

	if (exists() || from.exists())
		return;

	Common::Rect toArea = getArea();

	toArea.left = x;
	toArea.top  = y;
	if (toArea.isEmpty())
		return;

	Common::Rect fromArea = from.getArea();

	fromArea.clip(area);
	fromArea.setWidth (MIN(fromArea.width() , toArea.width()));
	fromArea.setHeight(MIN(fromArea.height(), toArea.height()));
	if (fromArea.isEmpty())
		return;

	uint32 w = fromArea.width();
	uint32 h = fromArea.height();

	const byte *src = from.getData() + fromArea.top * from.getWidth() + fromArea.left;
	byte *dst = _data + y * _width + x;

	if (transp) {
		while (h-- > 0) {
			const byte *srcRow = src;
			byte *dstRow = dst;

			for (uint32 i = 0; i < w; i++, srcRow++, dstRow++)
				if (*srcRow != 0)
					*dstRow = *srcRow;

			src += from.getWidth();
			dst += _width;
		}
	} else
		for (; h > 0; h--, src += from.getWidth(), dst += _width)
			memcpy(dst, src, w);
}

void Sprite::blit(const Sprite &from, uint32 x, uint32 y, bool transp) {
	blit(from, from.getArea(), x, y, transp);
}

void Sprite::blitDouble(const Sprite &from, const Common::Rect &area,
		uint32 x, uint32 y, bool transp) {

	if (exists() || from.exists())
		return;

	Common::Rect toArea = getArea();

	toArea.left = x;
	toArea.top  = y;
	if (toArea.isEmpty())
		return;

	Common::Rect fromArea = from.getArea();

	fromArea.clip(area);
	fromArea.setWidth (MIN(fromArea.width()  * 2, ((toArea.width())  / 2) * 2) / 2);
	fromArea.setHeight(MIN(fromArea.height() * 2, ((toArea.height()) / 2) * 2) / 2);
	if (fromArea.isEmpty())
		return;

	uint32 w = fromArea.width();
	uint32 h = fromArea.height();

	const byte *src = from.getData() + fromArea.top * from.getWidth() + fromArea.left;
	byte *dst = _data + y * _width + x;

	while (h-- > 0) {
		byte *dstRow = dst;
		for (int i = 0; i < 2; i++) {
			const byte *srcRow = src;

			for (uint32 j = 0; j < w; j++, srcRow++, dstRow += 2)
				if (!transp || *srcRow != 0) {
					dstRow[0] = *srcRow;
					dstRow[1] = *srcRow;
				}

			dst += _width;
		}
		src += from.getWidth();
	}
}

void Sprite::blitDouble(const Sprite &from, uint32 x, uint32 y, bool transp) {
	blitDouble(from, from.getArea(), x, y, transp);
}

void Sprite::fill(byte c) {
	memset(_data, c, _width * _height);
}

void Sprite::clear() {
	fill(0);
}

void Sprite::shade(byte c) {
	byte *data = _data;
	bool solid = true;
	bool rowSolid;

	for (uint32 i = 0; i < _height; i++) {
		rowSolid = solid;

		for (uint32 j = 0; j < _width; j++) {
			*data = rowSolid ? c : 0;
			data++;
			rowSolid = !rowSolid;
		}

		solid = !solid;
	}
}

void Sprite::recolor(byte oldColor, byte newColor) {
	uint32 n = _width * _height;
	byte *data = _data;

	while (n-- > 0) {
		if (*data == oldColor)
			*data = newColor;

		data++;
	}
}

::Graphics::Surface *Sprite::wrapInSurface() const {
	::Graphics::Surface *surface = new ::Graphics::Surface;

	surface->w = _width;
	surface->h = _height;

	surface->pitch = _width;

	surface->bytesPerPixel = 1;

	surface->pixels = (void *) _data;

	return surface;
}

void Sprite::drawStrings(const Common::StringList &strings, const ::Graphics::Font &font,
		int x, int y, byte color) {

	::Graphics::Surface *surface = wrapInSurface();

	for (Common::StringList::const_iterator it = strings.begin(); it != strings.end(); ++it) {
		font.drawString(surface, *it, x, y, _width, color,
				::Graphics::kTextAlignLeft, 0, false);

		y += font.getFontHeight();
	}

	delete surface;

}

void Sprite::applyChangeSet(const Common::Array<byte> &changeSet) {
	for (uint32 i = 0; i < _width * _height; i++)
		_data[i] = changeSet[_data[i]];
}

bool Sprite::readBMPDataComp0(Common::SeekableReadStream &bmp, uint32 dataSize) {
	byte *data = _data + ((_height - 1) * _width);

	int extraDataLength = (_width % 4) ? 4 - (_width % 4) : 0;
	for (uint32 i = 0; i < _height; i++) {
		byte *rowData = data;

		for (uint32 j = 0; j < _width; j++)
			*rowData++ = bmp.readByte();

		bmp.skip(extraDataLength);
		data -= _width;
	}

	return true;
}

bool Sprite::readBMPDataComp2(Common::SeekableReadStream &bmp, uint32 dataSize) {
	byte *data = _data + ((_height - 1) * _width);

	for (uint32 i = 0; i < _height; i++) {
		byte *rowData = data;

		// Skip this many pixels (they'll stay transparent)
		uint32 sizeSkip = bmp.readUint16LE();
		// Read this many pixels of data
		uint32 sizeData = bmp.readUint16LE();

		if ((sizeSkip + sizeData) > _width) {
			warning("Sprite::readBMPDataComp2(): Broken image compression: size %d (%d + %d), width %d",
					sizeSkip + sizeData, sizeSkip, sizeData, _width);
			return false;
		}

		rowData += sizeSkip;

		bmp.read(rowData, sizeData);

		data -= _width;
	}

	return true;
}

} // End of namespace DarkSeed2
