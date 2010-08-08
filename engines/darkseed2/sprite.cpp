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

#include "common/serializer.h"

#include "graphics/surface.h"
#include "graphics/fontman.h"
#include "graphics/font.h"

#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/imageconverter.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/neresources.h"
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

Sprite::Sprite() {
	clearData();
}

Sprite::Sprite(const Sprite &sprite) : Saveable(sprite) {
	_transparencyMap = 0;

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
	discard();

	if (sprite._surfacePaletted.pixels) {
		_surfacePaletted.copyFrom(sprite._surfacePaletted);

		_transparencyMap = new uint8[_surfacePaletted.w * _surfacePaletted.h];
		memcpy(_transparencyMap, sprite._transparencyMap, _surfacePaletted.w * _surfacePaletted.h);
	}

	if (sprite._surfaceTrueColor.pixels)
		_surfaceTrueColor.copyFrom(sprite._surfaceTrueColor);

	_palette = sprite._palette;

	_fileName = sprite._fileName;
	_fromCursor = sprite._fromCursor;

	_defaultX = sprite._defaultX;
	_defaultY = sprite._defaultY;

	_feetX = sprite._feetX;
	_feetY = sprite._feetY;

	_flippedHorizontally = sprite._flippedHorizontally;
	_flippedVertically   = sprite._flippedVertically;

	_scale        = sprite._scale;
	_scaleInverse = sprite._scaleInverse;
}

void Sprite::copyFrom(const byte *sprite, uint8 bpp, bool system) {
	if (bpp == 1) {
		memcpy(_surfacePaletted.pixels, sprite, _surfacePaletted.w * _surfacePaletted.h);
		memset(_transparencyMap, 0, _surfacePaletted.w * _surfacePaletted.h);

		convertToTrueColor(system);
	} else {
		memcpy(_surfaceTrueColor.pixels, sprite, _surfacePaletted.w * _surfacePaletted.h * 2);

		memset(_transparencyMap, 0, _surfacePaletted.w * _surfacePaletted.h);
	}
}

bool Sprite::exists() const {
	return _surfacePaletted.pixels != 0;
}

int32 Sprite::getWidth(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _surfacePaletted.w;

	return fracToInt(_surfacePaletted.w * _scale);
}

int32 Sprite::getHeight(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _surfacePaletted.h;

	return fracToInt(_surfacePaletted.h * _scale);
}

int32 Sprite::getDefaultX(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _defaultX;

	return fracToInt(_defaultX * _scale);
}

int32 Sprite::getDefaultY(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _defaultY;

	return fracToInt(_defaultY * _scale);
}

int32 Sprite::getFeetX(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _feetX;

	return fracToInt(_feetX * _scale);
}

int32 Sprite::getFeetY(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return _feetY;

	return fracToInt(_feetY * _scale);
}

Common::Rect Sprite::getArea(bool unscaled) const {
	if (unscaled || (_scale == FRAC_ONE))
		return Common::Rect(_surfacePaletted.w, _surfacePaletted.h);

	return Common::Rect(getWidth(), getHeight());
}

const ::Graphics::Surface &Sprite::getPaletted() const {
	return _surfacePaletted;
}

const ::Graphics::Surface &Sprite::getTrueColor() const {
	return _surfaceTrueColor;
}

void Sprite::setPalette(const Palette &palette) {
	_palette = palette;
}

const Palette &Sprite::getPalette() const {
	return _palette;
}

void Sprite::create(int32 width, int32 height) {
	// Sanity checks
	assert((width > 0) && (height > 0) && (width <= 0x7FFF) && (height <= 0x7FFF));

	discard();

	_surfacePaletted.create(width, height, 1);
	_surfaceTrueColor.create(width, height, 2);

	_transparencyMap = new uint8[width * height];

	clear();
}

void Sprite::discard() {
	_surfacePaletted.free();
	_surfaceTrueColor.free();

	delete[] _transparencyMap;

	clearData();
}

void Sprite::clearData() {
	_fileName.clear();

	_fromCursor = false;

	_transparencyMap = 0;

	_defaultX = 0;
	_defaultY = 0;
	_feetX    = 0;
	_feetY    = 0;

	_flippedHorizontally = false;
	_flippedVertically   = false;

	_scale        = FRAC_ONE;
	_scaleInverse = FRAC_ONE;

	_palette.clear();
}

void Sprite::convertToTrueColor(bool system) {
	if (!exists())
		return;

	if (system)
		ImgConv.convert8bitSystem(_surfaceTrueColor, _surfacePaletted);
	else
		ImgConv.convert8bit(_surfaceTrueColor, _surfacePaletted, _palette);
}

void Sprite::createTransparencyMap() {
	if (!exists())
		return;

	const byte *img = (const byte *) _surfacePaletted.pixels;
	uint8 *map = _transparencyMap;

	for (int32 y = 0; y < _surfacePaletted.h; y++)
		for (int32 x = 0; x < _surfacePaletted.w; x++)
			*map++ = (*img++ == 0) ? 1 : 0;
}

void Sprite::updateTransparencyMap() {
	if (!exists())
		return;

	const byte *img = (const byte *) _surfaceTrueColor.pixels;
	uint8 *map = _transparencyMap;

	uint32 colorTransp = ImgConv.convertColor(0, _palette);

	for (int32 y = 0; y < _surfaceTrueColor.h; y++) {
		for (int32 x = 0; x < _surfaceTrueColor.w; x++, map++, img += _surfaceTrueColor.bytesPerPixel) {
			const uint32 p = ImgConv.readColor(img);

			if ((*map == 1) && (p != colorTransp))
				*map = 0;
		}
	}
}

bool Sprite::loadFromImage(Resources &resources, const Common::String &image) {
	return loadFromImage(resources, image, resources.getVersionFormats().getImageType());
}

bool Sprite::loadFromRoomImage(Resources &resources, const Common::String &image) {
	return loadFromImage(resources, image, resources.getVersionFormats().getRoomImageType());
}

bool Sprite::loadFromBoxImage(Resources &resources, const Common::String &image,
		int32 width, int32 height) {

	switch (resources.getVersionFormats().getBoxImageType()) {
	case kImageTypeBMP:
		return loadFromBMP(resources, image);

	case kImageTypeRGB:
		return loadFromRGB(resources, image);

	case kImageTypeBDP:
		return loadFromBDP(resources, image);

	case kImageType256:
		return loadFrom256(resources, image, width, height);
		break;
	}

	return false;
}

bool Sprite::loadFromImage(Resources &resources, const Common::String &image, ImageType imageType) {
	switch (imageType) {
	case kImageTypeBMP:
		return loadFromBMP(resources, image);

	case kImageTypeRGB:
		return loadFromRGB(resources, image);

	case kImageTypeBDP:
		return loadFromBDP(resources, image);

	case kImageType256:
		break;
	}

	return false;
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

	int32 width  = (int32) bmp.readUint32LE();
	int32 height = (int32) bmp.readUint32LE();

	// Sanity checks
	assert((width > 0) && (height > 0) && (width <= 0x7FFF) && (height <= 0x7FFF));

	// Create surfaces
	create(width, height);

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

	// Sprite's feet position
	_feetX = (int32) MIN<uint16>(ABS(((int16) bmp.readUint16LE())), width  - 1);
	_feetY = (int32) MIN<uint16>(ABS(((int16) bmp.readUint16LE())), height - 1);

	// Default coordinates
	_defaultX = (int32) bmp.readUint16LE();
	_defaultY = (int32) bmp.readUint16LE();

	uint32 numPalColors = bmp.readUint32LE();
	if (numPalColors == 0)
		numPalColors = 256;
	if (numPalColors > 256)
		numPalColors = 256;

	if (bmpDataOffset == 54) {
		// Image data begins right after the header => no palette
		numPalColors = 0;
	}

	// Important colors
	bmp.skip(4);

	loadPalette(bmp, numPalColors);

	if (!bmp.seek(bmpDataOffset))
		return false;

	if (compression == 0) {
		if (!readBMPDataComp0(bmp, bmpDataSize))
			return false;
	} else if (compression == 2) {
		if (!readBMPDataComp2(bmp, bmpDataSize))
			return false;
	}

	createTransparencyMap();
	convertToTrueColor();

	return true;
}

bool Sprite::loadFromRGB(Common::SeekableReadStream &rgb) {
	rgb.seek(0);

	int32 size = rgb.size();

	uint16 width  = rgb.readUint16BE();
	uint16 height = rgb.readUint16BE();

	if (size < (12 + width * height * 2))
		return false;

	// Each line might be padded. I don't quite understand to which limits (some files pad to
	// a power of two, some to 80 bytes, some not at all), so we just calculate the pad from
	// the file size.
	uint16 linePad = (size - 4 - 8 - (width * height * 2)) / height;

	create(width, height);

	// TODO: Are those correct for RGB files?

	// Sprite's feet position
	_feetX = (int32) MIN<uint16>(ABS(((int16) rgb.readUint16BE())), width  - 1);
	_feetY = (int32) MIN<uint16>(ABS(((int16) rgb.readUint16BE())), height - 1);

	// Default coordinates
	_defaultX = (int32) rgb.readUint16BE();
	_defaultY = (int32) rgb.readUint16BE();

	byte *img = (byte *) _surfaceTrueColor.pixels;
	uint8 *transp = _transparencyMap;
	for (int32 y = 0; y < height; y++) {
		for (int32 x = 0; x < width; x++) {
			ImgConv.writeColor(img, readColor555(rgb, transp));

			img += _surfaceTrueColor.bytesPerPixel;
			transp++;
		}
		rgb.skip(linePad);
	}

	return true;
}

bool Sprite::loadFromBDP(Common::SeekableReadStream &bdp) {
	bdp.seek(0);

	if (bdp.size() != (320*240*2))
		return false;

	create(320, 240);

	byte *img = (byte *) _surfaceTrueColor.pixels;
	for (int32 y = 0; y < 320; y++) {
		for (int32 x = 0; x < 240; x++) {
			ImgConv.writeColor(img, readColor555(bdp));

			img += _surfaceTrueColor.bytesPerPixel;
		}
	}

	// Completely non-transparent
	memset(_transparencyMap, 0, _surfacePaletted.w * _surfacePaletted.h);

	return true;
}

bool Sprite::loadFrom256(Common::SeekableReadStream &f256, int32 width, int32 height) {
	if (f256.size() < (width * height))
		return false;

	create(width, height);

	byte *img = (byte *) _surfacePaletted.pixels;
	for (int32 y = 0; y < width; y++) {
		for (int32 x = 0; x < height; x++) {
			*img++ = f256.readByte();
		}
	}

	createTransparencyMap();
	convertToTrueColor();

	return true;
}

bool Sprite::loadFromCursorResource(const NECursor &cursor) {
	int32 width  = cursor.getWidth();
	int32 height = cursor.getHeight() * 2;

	// Sanity checks
	assert((width > 0) && (height > 0));

	Common::SeekableReadStream &stream = cursor.getStream();

	if (stream.size() <= 40)
		return false;

	// Check header size
	if (stream.readUint32LE() != 40)
		return false;

	// Check dimensions
	if (stream.readUint32LE() != ((uint32) width))
		return false;
	if (stream.readUint32LE() != ((uint32) height))
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

	_palette.resize(3);

	// Standard palette: transparent, black, white
	_palette.get()[0 * 3 + 0] = 0;
	_palette.get()[0 * 3 + 1] = 0;
	_palette.get()[0 * 3 + 2] = 255;
	_palette.get()[1 * 3 + 0] = 0;
	_palette.get()[1 * 3 + 1] = 0;
	_palette.get()[1 * 3 + 2] = 0;
	_palette.get()[2 * 3 + 0] = 255;
	_palette.get()[2 * 3 + 1] = 255;
	_palette.get()[2 * 3 + 2] = 255;

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
	byte *dest = (byte *) _surfacePaletted.getBasePtr(0, height - 1);
	for (int32 i = 0; i < height; i++) {
		byte *rowDest = dest;

		for (int32 j = 0; j < (width / 8); j++) {
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

	_fromCursor = true;

	createTransparencyMap();
	convertToTrueColor();

	return true;
}

bool Sprite::loadFromSaturnCursor(Common::SeekableReadStream &cursor) {
	if (cursor.size() != 260)
		return false;

	_fromCursor = true;

	create(16, 16);

	cursor.seek(0);

	_feetX = cursor.readUint16BE();
	_feetY = cursor.readUint16BE();

	byte *img = (byte *) _surfaceTrueColor.pixels;
	for (int32 y = 0; y < 16; y++) {
		for (int32 x = 0; x < 16; x++) {
			const uint8  p = cursor.readByte();
			const uint32 c = (p == 0) ? ImgConv.getColor(0, 0, 255) : ImgConv.getColor(255 - p, 255 - p, 255 - p);

			ImgConv.writeColor(img, c);

			img += _surfaceTrueColor.bytesPerPixel;
		}
	}

	return true;
}

uint32 Sprite::readColor555(Common::SeekableReadStream &stream, uint8 *transp) const {
	const uint16 p = stream.readUint16BE();
	const uint8  r = ((p & 0x001F)      ) << 3;
	const uint8  g = ((p & 0x03E0) >>  5) << 3;
	const uint8  b = ((p & 0x7C00) >> 10) << 3;

	if (transp)
		*transp = (p == 0) ? 1 : 0;

	return ImgConv.getColor(r, g, b);
}

bool Sprite::loadFromBMP(Resources &resources, const Common::String &bmp) {
	Common::String bmpFile = Resources::addExtension(bmp,
			resources.getVersionFormats().getImageExtension(kImageTypeBMP));

	if (!resources.hasResource(bmpFile))
		return false;

	Resource *resBMP = resources.getResource(bmpFile);

	bool result = loadFromBMP(resBMP->getStream());

	delete resBMP;

	_fileName = bmp;

	return result;
}

bool Sprite::loadFromRGB(Resources &resources, const Common::String &rgb) {
	Common::String rgbFile = Resources::addExtension(rgb,
			resources.getVersionFormats().getImageExtension(kImageTypeRGB));
	if (!resources.hasResource(rgbFile))
		return false;

	Resource *resRGB = resources.getResource(rgbFile);

	bool result = loadFromRGB(resRGB->getStream());

	delete resRGB;

	_fileName = rgb;

	return result;
}

bool Sprite::loadFromBDP(Resources &resources, const Common::String &bdp) {
	Common::String bdpFile = Resources::addExtension(bdp,
			resources.getVersionFormats().getImageExtension(kImageTypeBDP));
	if (!resources.hasResource(bdpFile))
		return false;

	Resource *resBDP = resources.getResource(bdpFile);

	bool result = loadFromBDP(resBDP->getStream());

	delete resBDP;

	_fileName = bdp;

	return result;
}

bool Sprite::loadFrom256(Resources &resources, const Common::String &f256, int32 width, int32 height) {
	Common::String f256File = Resources::addExtension(f256,
			resources.getVersionFormats().getImageExtension(kImageType256));
	if (!resources.hasResource(f256File))
		return false;

	Resource *res256 = resources.getResource(f256File);

	bool result = loadFrom256(res256->getStream(), width, height);

	delete res256;

	_fileName = f256;

	return result;
}

bool Sprite::loadFromSaturnCursor(Resources &resources, const Common::String &cursor) {
	Common::String cursorFile = Resources::addExtension(cursor, "CUR");
	if (!resources.hasResource(cursorFile))
		return false;

	Resource *resCursor = resources.getResource(cursorFile);

	bool result = loadFromSaturnCursor(resCursor->getStream());

	delete resCursor;

	_fileName = cursor;

	return result;
}

void Sprite::loadPalette(Common::SeekableReadStream &stream, uint32 count) {
	if (count == 0)
		return;

	byte *palette = new byte[count * 3];
	for (uint32 i = 0; i < count ; i++) {
		palette[i * 3 + 2] = stream.readByte();
		palette[i * 3 + 1] = stream.readByte();
		palette[i * 3 + 0] = stream.readByte();

		stream.skip(1);
	}
	_palette.copyFrom(palette, count);
	delete[] palette;
}

void Sprite::flipHorizontally() {
	if (!exists())
		return;

	int32 width     = _surfacePaletted.w;
	int32 height    = _surfacePaletted.h;
	int32 halfWidth = width / 2;

	byte  *dataPal    = (byte *) _surfacePaletted.pixels;
	byte  *dataTrue   = (byte *) _surfaceTrueColor.pixels;
	uint8 *dataTransp =          _transparencyMap;

	for (int32 i = 0; i < height; i++) {
		byte  *dataPalStart    = dataPal;
		byte  *dataPalEnd      = dataPal    + width - 1;
		byte  *dataTrueStart   = dataTrue;
		byte  *dataTrueEnd     = dataTrue   + _surfaceTrueColor.pitch - _surfaceTrueColor.bytesPerPixel;
		uint8 *dataTranspStart = dataTransp;
		uint8 *dataTranspEnd   = dataTransp + width - 1;

		for (int32 j = 0; j < halfWidth; j++) {
			SWAP(*dataPalStart, *dataPalEnd);
			dataPalStart++;
			dataPalEnd--;

			ImgConv.swapColor(dataTrueStart, dataTrueEnd);
			dataTrueStart += _surfaceTrueColor.bytesPerPixel;
			dataTrueEnd   -= _surfaceTrueColor.bytesPerPixel;

			SWAP(*dataTranspStart, *dataTranspEnd);
			dataTranspStart++;
			dataTranspEnd--;
		}

		dataPal    += width;
		dataTrue   += _surfaceTrueColor.pitch;
		dataTransp += width;
	}

	_feetX = width - _feetX;
	_flippedHorizontally = !_flippedHorizontally;
}

void Sprite::flipVertically() {
	if (!exists())
		return;

	int32 width      = _surfacePaletted.w;
	int32 height     = _surfacePaletted.h;
	int32 halfHeight = height / 2;

	byte  *dataPal    = (byte *) _surfacePaletted.pixels;
	byte  *dataTrue   = (byte *) _surfaceTrueColor.pixels;
	uint8 *dataTransp =          _transparencyMap;

	byte  *dataPalStart    = dataPal;
	byte  *dataPalEnd      = dataPal    + (width * height) - width;
	byte  *dataTrueStart   = dataTrue;
	byte  *dataTrueEnd     = dataTrue   + (_surfaceTrueColor.pitch * height) - _surfaceTrueColor.pitch;
	uint8 *dataTranspStart = dataTransp;
	uint8 *dataTranspEnd   = dataTransp + (width * height) - width;

	byte   *bufferPal    = new byte  [width];
	byte   *bufferTrue   = new byte  [_surfaceTrueColor.pitch];
	uint8  *bufferTransp = new uint8 [width];

	for (int32 i = 0; i < halfHeight; i++) {
		memcpy(bufferPal   , dataPalStart, width);
		memcpy(dataPalStart, dataPalEnd  , width);
		memcpy(dataPalEnd  , bufferPal   , width);
		dataPalStart += width;
		dataPalEnd   -= width;

		memcpy(bufferTrue   , dataTrueStart, _surfaceTrueColor.pitch);
		memcpy(dataTrueStart, dataTrueEnd  , _surfaceTrueColor.pitch);
		memcpy(dataTrueEnd  , bufferTrue   , _surfaceTrueColor.pitch);
		dataTrueStart += _surfaceTrueColor.pitch;
		dataTrueEnd   -= _surfaceTrueColor.pitch;

		memcpy(bufferTransp   , dataTranspStart, width);
		memcpy(dataTranspStart, dataTranspEnd  , width);
		memcpy(dataTranspEnd  , bufferTransp   , width);
		dataTranspStart += width;
		dataTranspEnd   -= width;
	}

	delete[] bufferPal;
	delete[] bufferTrue;
	delete[] bufferTransp;

	_feetY = height - _feetY;
	_flippedVertically = !_flippedVertically;
}

void Sprite::blit(const Sprite &from, const Common::Rect &area, int32 x, int32 y, bool transp) {
	// Sanity checks
	assert((x >= 0) && (y >= 0) && (x <= 0x7FFF) && (y <= 0x7FFF));

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
	if (fromArea.isEmpty() || !fromArea.isValidRect())
		return;

	int32 w = fromArea.width();
	int32 h = fromArea.height();

	const int32 fromTop   = fracToInt(fromArea.top  * from._scaleInverse);
	const int32 fromLeft  = fracToInt(fromArea.left * from._scaleInverse);

	const byte *src = (const byte *) from._surfaceTrueColor.getBasePtr(fromLeft, fromTop);
	      byte *dst = (      byte *)      _surfaceTrueColor.getBasePtr(x, y);

	const uint8 *srcT = from._transparencyMap + fromTop * from._surfaceTrueColor.w + fromLeft;
	      uint8 *dstT =      _transparencyMap +       y *      _surfaceTrueColor.w + x;

	frac_t posW = 0, posH = 0;
	while (h-- > 0) {
		posW = 0;

		const byte *srcRow = src;
		      byte *dstRow = dst;

		const uint8 *srcRowT = srcT;
		      uint8 *dstRowT = dstT;

		for (int32 j = 0; j < w; j++, dstRow += _surfaceTrueColor.bytesPerPixel, dstRowT++) {
			if (!transp || (*srcRowT == 0)) {
				// Ignore transparency or source is solid => copy
				memcpy(dstRow, srcRow, _surfaceTrueColor.bytesPerPixel);
				*dstRowT = *srcRowT;
			} else if (*srcRowT == 2) {
				// Half-transparent
				if (*dstRowT == 1)
					// But destination is transparent => propagate
					memcpy(dstRow, srcRow, _surfaceTrueColor.bytesPerPixel);
				else
					// Destination is solid => mix
					ImgConv.mixTrueColor(dstRow, srcRow);

				*dstRowT = *srcRowT;
			}

			// Advance source data
			posW += from._scaleInverse;
			while (posW >= ((frac_t) FRAC_ONE)) {
				srcRow += from._surfaceTrueColor.bytesPerPixel;
				srcRowT++;
				posW -= FRAC_ONE;
			}

		}

		dst  += _surfaceTrueColor.pitch;
		dstT += _surfaceTrueColor.w;

		// Advance source data
		posH += from._scaleInverse;
		while (posH >= ((frac_t) FRAC_ONE)) {
			src  += from._surfaceTrueColor.pitch;
			srcT += from._surfaceTrueColor.w;
			posH -= FRAC_ONE;
		}

	}
}

void Sprite::blit(const Sprite &from, int32 x, int32 y, bool transp) {
	blit(from, from.getArea(), x, y, transp);
}

void Sprite::fillImage(byte cP, uint32 cT) {
	memset(_surfacePaletted.pixels, cP,
			_surfacePaletted.w * _surfacePaletted.h * _surfacePaletted.bytesPerPixel);

	byte *trueColor = (byte *) _surfaceTrueColor.pixels;
	for (int32 y = 0; y < _surfaceTrueColor.h; y++) {
		for (int32 x = 0; x < _surfaceTrueColor.w; x++) {
			if (_surfaceTrueColor.bytesPerPixel == 2)
				ImgConv.writeColor(trueColor, cT);

			trueColor += _surfaceTrueColor.bytesPerPixel;
		}
	}
}

void Sprite::fill(byte c) {
	if (!exists())
		return;

	fillImage(c, ImgConv.convertColor(c, _palette));

	memset(_transparencyMap, 0, _surfacePaletted.w * _surfacePaletted.h);
}

void Sprite::fill(uint32 c) {
	if (!exists())
		return;

	fillImage(0, c);

	memset(_transparencyMap, 0, _surfacePaletted.w * _surfacePaletted.h);
}

void Sprite::clear() {
	if (!exists())
		return;

	fillImage(0, ImgConv.convertColor(0, _palette));

	memset(_transparencyMap, 1, _surfacePaletted.w * _surfacePaletted.h);
}

void Sprite::darken() {
	if (!exists())
		return;

	fillImage(0, ImgConv.getColor(0, 0, 0));

	memset(_transparencyMap, 0, _surfacePaletted.w * _surfacePaletted.h);
}

void Sprite::shade(uint32 c) {
	if (!exists())
		return;

	fillImage(0, c);

	memset(_transparencyMap, 2, _surfacePaletted.w * _surfacePaletted.h);
}

void Sprite::drawStrings(const FontManager::TextList &strings, const FontManager &fontManager,
		int x, int y, uint32 color) {

	for (FontManager::TextList::const_iterator it = strings.begin(); it != strings.end(); ++it) {
		fontManager.drawText(_surfaceTrueColor, *it, x, y, color);

		y += fontManager.getFontHeight();
	}

	updateTransparencyMap();
}

bool Sprite::readBMPDataComp0(Common::SeekableReadStream &bmp, uint32 dataSize) {
	int32 width  = _surfacePaletted.w;
	int32 height = _surfacePaletted.h;

	byte *data = (byte *) _surfacePaletted.getBasePtr(0, height - 1);

	int extraDataLength = (width % 4) ? 4 - (width % 4) : 0;
	for (int32 i = 0; i < height; i++) {
		byte *rowData = data;

		for (int32 j = 0; j < width; j++)
			*rowData++ = bmp.readByte();

		bmp.skip(extraDataLength);
		data -= width;
	}

	return true;
}

bool Sprite::readBMPDataComp2(Common::SeekableReadStream &bmp, uint32 dataSize) {
	int32 width  = _surfacePaletted.w;
	int32 height = _surfacePaletted.h;

	byte *data = (byte *) _surfacePaletted.getBasePtr(0, height - 1);

	for (int32 i = 0; i < height; i++) {
		byte *rowData = data;

		// Skip this many pixels (they'll stay transparent)
		int32 sizeSkip = bmp.readUint16LE();
		// Read this many pixels of data
		int32 sizeData = bmp.readUint16LE();

		if ((sizeSkip + sizeData) > width) {
			warning("Sprite::readBMPDataComp2(): Broken image compression: size %d (%d + %d), width %d",
					sizeSkip + sizeData, sizeSkip, sizeData, width);
			return false;
		}

		rowData += sizeSkip;

		bmp.read(rowData, sizeData);

		data -= width;
	}

	return true;
}

frac_t Sprite::getScale() const {
	return _scale;
}

void Sprite::setScale(frac_t scale) {
	assert(scale != 0);

	_scale        = scale;
	// Is there a better way to do that? :/
	_scaleInverse = doubleToFrac(1.0 / fracToDouble(scale));
}

bool Sprite::saveLoad(Common::Serializer &serializer, Resources &resources) {
	assert(!_fromCursor);

	uint32 scale = (uint32) _scale;

	SaveLoad::sync(serializer, _fileName);
	SaveLoad::sync(serializer, _flippedHorizontally);
	SaveLoad::sync(serializer, _flippedVertically);
	SaveLoad::sync(serializer, scale);

	_scale = (frac_t) scale;

	return true;
}

bool Sprite::loading(Resources &resources) {
	if (_fileName.empty())
		return true;

	byte   flippedHorizontally = _flippedHorizontally;
	byte   flippedVertically   = _flippedVertically;
	uint32 scale               = _scale;

	loadFromImage(resources, _fileName);

	if (flippedHorizontally)
		flipHorizontally();
	if (flippedVertically)
		flipVertically();

	setScale(scale);

	return true;
}

} // End of namespace DarkSeed2
