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

#include "graphics/fontman.h"

#include "engines/darkseed2/font.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

TextLine::TextLine() {
	_length = 0;
	_str = 0;
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


FontManager::FontManager(Resources &resources) {
	_fontLatin1   = 0;
	_fontJapanese = 0;
}

FontManager::~FontManager() {
}

bool FontManager::init(GameVersion gameVersion, Common::Language language) {
	if (language == Common::JA_JPN) {
		if (gameVersion == kGameVersionSaturn) {
			warning("TODO: Load KANJI.FON");
			_fontJapanese = (Font *) 1;
			return true;
		}

		warning("Unknown game version");
		return false;
	}

	// We want the big font
	const ::Graphics::FontManager::FontUsage fontUsage = ::Graphics::FontManager::kBigGUIFont;
	_fontLatin1 = ::Graphics::FontManager::instance().getFontByUsage(fontUsage);

	return true;
}

void FontManager::drawText(::Graphics::Surface &surface, const TextLine &text,
		int32 x, int32 y, uint32 color) const {

	if (_fontLatin1) {
		_fontLatin1->drawString(&surface, (const char *) text.getText(), x, y, surface.w,
				color, ::Graphics::kTextAlignLeft, 0, false);
		return;
	}

	if (_fontJapanese) {
		return;
	}

}

int32 FontManager::wordWrapText(const TextLine &text, int maxWidth, TextList &lines) const {
	if (_fontLatin1) {
		Common::StringList stringList;

		int32 width = _fontLatin1->wordWrapText((const char *) text.getText(), maxWidth, stringList);

		lines.reserve(stringList.size());
		for (uint i = 0; i < stringList.size(); i++)
			lines.push_back(TextLine(stringList[i]));

		return width;
	}

	if (_fontJapanese) {
		lines.push_back(TextLine((const byte *) "TEXT", 4));
		return 1;
	}

	return 0;
}

int32 FontManager::getFontHeight() const {
	if (_fontLatin1)
		return _fontLatin1->getFontHeight();

	if (_fontJapanese)
		return 1;

	return 0;
}

}
