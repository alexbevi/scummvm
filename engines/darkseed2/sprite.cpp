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
#include "engines/darkseed2/neresources.h"

namespace DarkSeed2 {

Sprite::Sprite() {
	_width  = 0;
	_height = 0;
	_data   = 0;

	_defaultX = 0;
	_defaultY = 0;
	_feetX    = 0;
	_feetY    = 0;

	_scale        = FRAC_ONE;
	_scaleInverse = FRAC_ONE;
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
	return _data != 0;
}

uint32 Sprite::getWidth(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _width;

	return fracToInt(_width * _scale);
}

uint32 Sprite::getHeight(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _height;

	return fracToInt(_height * _scale);
}

uint16 Sprite::getDefaultX(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _defaultX;

	return fracToInt(_defaultX * _scale);
}

uint16 Sprite::getDefaultY(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _defaultY;

	return fracToInt(_defaultY * _scale);
}

uint16 Sprite::getFeetX(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _feetX;

	return fracToInt(_feetX * _scale);
}

uint16 Sprite::getFeetY(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _feetY;

	return fracToInt(_feetY * _scale);
}

Common::Rect Sprite::getArea(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return Common::Rect(_width, _height);

	return Common::Rect(getWidth(), getHeight());
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

	_scale        = FRAC_ONE;
	_scaleInverse = FRAC_ONE;

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

	_feetX = MIN<uint16>(ABS(((int16) bmp.readUint16LE())), _width  - 1);
	_feetY = MIN<uint16>(ABS(((int16) bmp.readUint16LE())), _height - 1);

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

	return true;
}

bool Sprite::loadFromBMP(const Resource &resource) {
	return loadFromBMP(resource.getStream());
}

bool Sprite::loadFromBMP(Resources &resources, const Common::String &bmp) {
	if (!resources.hasResource(bmp + ".BMP"))
		return false;

	Resource *resBMP = resources.getResource(bmp + ".BMP");

	bool result = loadFromBMP(*resBMP);

	delete resBMP;

	return result;
}

void Sprite::flipHorizontally() {
	if (!exists())
		return;

	uint32 halfWidth = _width / 2;

	byte *data = _data;
	for (uint32 i = 0; i < _height; i++, data += _width) {
		byte *dataStart = data;
		byte *dataEnd   = data + _width - 1;

		for (uint32 j = 0; j < halfWidth; j++, dataStart++, dataEnd--)
			SWAP(*dataStart, *dataEnd);
	}

	_feetX = _width - _feetX;
}

void Sprite::flipVertically() {
	if (!exists())
		return;

	uint32 halfHeight = _height / 2;

	byte *dataStart = _data;
	byte *dataEnd   = _data + (_width * _height) - _width;

	byte *buffer = new byte[_width];

	for (uint32 i = 0; i < halfHeight; i++, dataStart += _width, dataEnd -= _width) {
		memcpy(buffer   , dataStart, _width);
		memcpy(dataStart, dataEnd  , _width);
		memcpy(dataEnd  , buffer   , _width);
	}

	delete[] buffer;

	_feetY = _height - _feetY;
}

bool Sprite::loadFromCursorResource(const NECursor &cursor) {
	uint16 width  = cursor.getWidth();
	uint16 height = cursor.getHeight() * 2;

	Common::SeekableReadStream &stream = cursor.getStream();

	if (stream.size() <= 40)
		return false;

	// Check header size
	if (stream.readUint32LE() != 40)
		return false;

	// Check dimensions
	if (stream.readUint32LE() != width)
		return false;
	if (stream.readUint32LE() != height)
		return false;

	// Color planes
	if (stream.readUint16LE() != 1)
		return false;
	// Bits per pixel
	if (stream.readUint16LE() != 1)
		return false;
	// Compression
	if (stream.readUint32LE() != 0)
		return false;

	// Image size + X resolution + Y resolution
	stream.skip(4 + 4 + 4);

	uint32 numColors = stream.readUint32LE();

	if (numColors == 0)
		numColors = 2;

	if (numColors > 2)
		return false;

	// Assert that enough data is there for the whole cursor
	if (((uint32) stream.size()) < (40 + numColors * 4 + ((width * height) / 8)))
		return false;

	// Height includes AND-mask and XOR-mask
	height /= 2;

	create(width, height);

	// Standard palette: transparent, black white
	memset(_palette.get()    , 0, 6);
	memset(_palette.get() + 6, 0, 3);

	// Reading the palette
	stream.seek(40);
	for (uint32 i = 0 ; i < numColors; i++) {
		_palette[(i + 1) * 3 + 2] = stream.readByte();
		_palette[(i + 1) * 3 + 1] = stream.readByte();
		_palette[(i + 1) * 3 + 0] = stream.readByte();
		stream.skip(1);
	}

	// Reading the bitmap data
	const byte *srcP = cursor.getData() + 40 + numColors * 4;
	const byte *srcM = srcP + ((width * height) / 8);
	byte *dest = _data + (width * height) - width;
	for (int i = 0; i < height; i++) {
		byte *rowDest = dest;

		for (int j = 0; j < (width / 8); j++) {
			byte p = srcP[j];
			byte m = srcM[j];

			for (int k = 0; k < 8; k++, rowDest++, p <<= 1, m <<= 1) {
				if ((m & 0x80) != 0x80) {
					if ((p & 0x80) == 0x80)
						*rowDest = 2;
					else
						*rowDest = 1;
				} else
					*rowDest = 0;
			}
		}

		dest -= width;
		srcP += width / 8;
		srcM += width / 8;
	}

	return true;
}

void Sprite::blit(const Sprite &from, const Common::Rect &area,
		uint32 x, uint32 y, bool transp) {

	if (!exists() || !from.exists())
		return;

	Common::Rect toArea = getArea(true);

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

	const uint32 fromTop   = fracToInt(fromArea.top  * from._scaleInverse);
	const uint32 fromLeft  = fracToInt(fromArea.left * from._scaleInverse);
	const uint32 fromWidth = from.getWidth(true);

	const byte *src = from.getData() + fromTop * fromWidth + fromLeft;
	byte *dst = _data + y * _width + x;

	frac_t posW = 0, posH = 0;
	while (h-- > 0) {
		posW = 0;

		byte *dstRow = dst;
		const byte *srcRow = src;

		for (uint32 j = 0; j < w; j++, dstRow++) {
			if (!transp || *srcRow != 0)
				dstRow[0] = *srcRow;

			posW += from._scaleInverse;
			while (posW >= ((frac_t) FRAC_ONE)) {
				srcRow++;
				posW -= FRAC_ONE;
			}

		}

		dst += _width;

		posH += from._scaleInverse;
		while (posH >= ((frac_t) FRAC_ONE)) {
			src += from.getWidth(true);
			posH -= FRAC_ONE;
		}

	}
}

void Sprite::blit(const Sprite &from, uint32 x, uint32 y, bool transp) {
	blit(from, from.getArea(), x, y, transp);
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

void Sprite::setScale(frac_t scale) {
	assert(scale != 0);

	_scale        = scale;
	// Is there a better way to do that? :/
	_scaleInverse = doubleToFrac(1.0 / fracToDouble(scale));
}

} // End of namespace DarkSeed2
