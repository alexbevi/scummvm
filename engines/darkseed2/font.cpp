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

#include "common/types.h"
#include "common/stream.h"
#include "common/str-array.h"

#include "graphics/fontman.h"

#include "engines/darkseed2/font.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/imageconverter.h"

namespace DarkSeed2 {

TextLine::TextLine() {
	_length = 0;
	_str = 0;
}

TextLine::TextLine(Common::SeekableReadStream &stream) {
	_length = stream.size() - stream.pos();

	assert(_length > 0);

	_str = new byte[_length + 1];
	uint32 r = stream.read(_str, _length);
	assert(r == _length);
	_str[_length] = '\0';
}

TextLine::TextLine(const byte *str, uint32 length) {
	assert(str);
	assert(length > 0);

	_length = length;

	_str = new byte[_length + 1];
	memcpy(_str, str, _length);
	_str[_length] = '\0';
}

TextLine::TextLine(const Common::String &str) {
	assert(!str.empty());

	_length = str.size();
	_str = new byte[_length + 1];
	memcpy(_str, str.c_str(), _length);
	_str[_length] = '\0';
}

TextLine::TextLine(const TextLine &right) {
	_str = 0;
	*this = right;
}

TextLine::~TextLine() {
	delete[] _str;
}

TextLine &TextLine::operator=(const TextLine &right) {
	delete[] _str;

	_length = right._length;

	_str = new byte[_length + 1];
	memcpy(_str, right._str, _length);
	_str[_length] = '\0';

	return *this;
}

uint32 TextLine::getLength() const {
	assert(_length);

	return _length;
}

const byte *TextLine::getText() const {
	assert(_str);

	return _str;
}


Font::Font() {
}

Font::~Font() {
}


Saturn2Byte::Saturn2Byte() {
	_fileSize = 0;
	_fontData = 0;
}

Saturn2Byte::~Saturn2Byte() {
	clear();
}

void Saturn2Byte::clear() {
	delete[] _fontData;

	_fileSize = 0;
	_fontData = 0;
}

bool Saturn2Byte::load(Resources &resources, const Common::String &file) {
	Common::String fonFile = Resources::addExtension(file, "FON");

	if (!resources.hasResource(fonFile))
		return false;

	Resource *resFON = resources.getResource(fonFile);

	bool result = load(resFON->getStream());

	delete resFON;

	return result;
}

bool Saturn2Byte::load(Common::SeekableReadStream &stream) {
	clear();

	stream.seek(0);

	_fileSize = stream.size();

	if (_fileSize == 0)
		return false;

	_fontData = new byte[_fileSize];

	if (stream.read(_fontData, _fileSize) != _fileSize) {
		clear();
		return false;
	}

	return true;
}

int32 Saturn2Byte::getFontHeight() const {
	return 16;
}

int32 Saturn2Byte::getCharWidth(uint32 c) const {
	return 16;
}

uint32 Saturn2Byte::getChar(const byte *str) const {
	if (*str == '\0')
		return *str;
	if (*str == '\n')
		return *str;
	if (*str == '\r')
		return *str;

	return READ_BE_UINT16(str);
}

const byte *Saturn2Byte::nextChar(const byte *str) const {
	if (*str == '\0')
		return str + 1;
	else if (*str == '\n')
		return str + 1;
	else if (*str == '\r')
		return str + 1;

	return str + 2;
}

uint32 Saturn2Byte::getStringLength(const TextLine &line) const {
	const byte *text = line.getText();
	uint32 length = 0;

	uint32 c = getChar(text);
	while (c) {
		text = nextChar(text);
		c = getChar(text);
	}

	return length;
}

bool Saturn2Byte::validBreakSpace(uint32 prev, uint32 cur, uint32 next) const {
	return true;
}

bool Saturn2Byte::isValidJIS(uint8 j1, uint8 j2) {
	uint8 c1 = (j1 >> 4);  // First byte column
	uint8 l1 =  j1 & 0x0F; // First byte line
	uint8 c2 = (j2 >> 4);  // Second byte column
	uint8 l2 =  j2 & 0x0F; // Second byte line

	// The column/line values are only allowed to run from 2/1 to 7/14

	if ((c1 < 2) || (c1 > 7))
		return false;
	if ((l1 < 1) || (l1 > 14))
		return false;

	if ((c2 < 2) || (c2 > 7))
		return false;
	if ((l2 < 1) || (l2 > 14))
		return false;

	return true;
}

uint16 Saturn2Byte::convertShiftJISToJIS(uint16 c) {
	const uint8 s1 = (c >> 8) & 0xFF;
	const uint8 s2 =  c       & 0xFF;

	uint8 j1 = s1;
	uint8 j2 = s2;

	// First convert the higher-order byte
	if (s1 > 176)
		j1 -= 176;
	else
		j1 -= 112;

	j1 = (j1 * 2) - 1;

	if (s2 < 126) {
		// This case is unambiguous

		j2 -= 31;
		if (j2 >= 97)
			j2--;

		return (j1 << 8) | j2;
	}

	// This case is ambiguous, so we first try one possibility and examine if
	// the result is a valid JIS sequence. If not, try the other possibility.

	j2 -= 126;
	j1++;

	if (isValidJIS(j1, j2))
		// It's valid, return that.
		return (j1 << 8) | j2;

	// Reset the result bytes
	j1--;
	j2 = s2;

	j2 -= 31;
	if (j2 >= 97)
		j2--;

	return (j1 << 8) | j2;
}

void Saturn2Byte::drawChar(uint32 c, ::Graphics::Surface &surface, int32 x, int32 y, uint32 color) const {
	if ((c == '\n') || (c == '\r'))
		return;

	// We get Shift_JIS data, but the font is in JIS X 0208
	c = convertShiftJISToJIS(c);

	const uint8 highByte = (c >> 8) & 0xFF;
	const uint8 lowByte  =  c       & 0xFF;

	uint32 filePos = ((highByte - 0x21) * (0x7E - 0x21 + 1) + (lowByte - 0x21)) * 32;
	if (filePos >= _fileSize)
		return;

	assert(_fontData);

	const byte *charOffset = _fontData + filePos;

	byte *img = (byte *) surface.getBasePtr(x, y);
	for (int dY = 0; dY < 16; dY++) {
		byte *imgRow = img;

		for (int n = 0; n < 2; n++) {
			byte charData = *charOffset++;

			for (int dX = 0; dX < 8; dX++, imgRow += surface.bytesPerPixel) {
				if ((charData & 0x80) != 0) {
					if (surface.bytesPerPixel == 1)
						*imgRow = color;
					else if (surface.bytesPerPixel == 2)
						*((uint16 *) imgRow) = color;
				}
				charData <<= 1;
			}
		}

		img += surface.pitch;
	}
}


ScummVMLatin1::ScummVMLatin1() {
	// We want the big font
	const ::Graphics::FontManager::FontUsage fontUsage = ::Graphics::FontManager::kBigGUIFont;
	_font = ::Graphics::FontManager::instance().getFontByUsage(fontUsage);
}

ScummVMLatin1::~ScummVMLatin1() {
}

int32 ScummVMLatin1::getFontHeight() const {
	return _font->getFontHeight();
}

int32 ScummVMLatin1::getCharWidth(uint32 c) const {
	return _font->getCharWidth(c);
}

uint32 ScummVMLatin1::getChar(const byte *str) const {
	return *str;
}

const byte *ScummVMLatin1::nextChar(const byte *str) const {
	return str + 1;
}

uint32 ScummVMLatin1::getStringLength(const TextLine &line) const {
	return strlen((const char *) line.getText());
}

bool ScummVMLatin1::validBreakSpace(uint32 prev, uint32 cur, uint32 next) const {
	return isspace(cur);
}

void ScummVMLatin1::drawChar(uint32 c, ::Graphics::Surface &surface, int32 x, int32 y, uint32 color) const {
	_font->drawChar(&surface, c, x, y, color);
}


FontManager::FontManager(Resources &resources) {
	_resources = &resources;

	_font = 0;
}

FontManager::~FontManager() {
	delete _font;
}

bool FontManager::init(GameVersion gameVersion, Common::Language language) {
	if (language == Common::JA_JPN) {
		if (gameVersion == kGameVersionSaturn) {
			Saturn2Byte *kanji = new Saturn2Byte();

			if (!kanji->load(*_resources, "KANJI")) {
				delete kanji;
				return false;
			}

			_font = kanji;

			return true;
		}

		warning("Unknown game version");
		return false;
	}

	_font = new ScummVMLatin1();

	return true;
}

void FontManager::drawText(::Graphics::Surface &surface, const TextLine &text,
		int32 x, int32 y, uint32 color) const {

	if (!_font)
		return;

	const byte *txt = text.getText();

	uint32 c = _font->getChar(txt);
	while (c) {
		if ((x + _font->getCharWidth(c) - 1) >= surface.w)
			// Reached the surface's right border
			break;

		_font->drawChar(c, surface, x, y, color);

		x += _font->getCharWidth(c);

		txt = _font->nextChar(txt);
		c = _font->getChar(txt);
	}
}

int FontManager::wordWrapText(const TextLine &text, int maxWidth, TextList &lines) const {
	if (!_font)
		return 0;

	const byte *txt = text.getText();

	const byte *lineStart = txt;

	int length     = 0;
	int lineLength = 0;

	uint32 c = _font->getChar(txt);
	while (c) {
		int32 charWidth = _font->getCharWidth(c);

		if ((lineLength + charWidth) > maxWidth) {
			lines.push_back(TextLine(lineStart, txt - lineStart));

			length = MAX(length, lineLength);

			lineStart = txt;
			lineLength = 0;
		}

		lineLength += charWidth;

		txt = _font->nextChar(txt);
		c = _font->getChar(txt);
	}

	if ((txt - lineStart) > 0) {
		lines.push_back(TextLine(lineStart, txt - lineStart));

		length = MAX(length, lineLength);
	}

	return length;
}

int32 FontManager::getFontHeight() const {
	if (!_font)
		return 0;

	return _font->getFontHeight();
}

}
