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

#ifndef DARKSEED2_FONT_H
#define DARKSEED2_FONT_H

#include "common/util.h"
#include "common/str.h"

#include "graphics/font.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/versionformats.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Resources;

class TextLine {
public:
	TextLine();
	TextLine(const byte *str, uint32 length);
	TextLine(Common::SeekableReadStream &stream);
	TextLine(const Common::String &str);
	TextLine(const TextLine &right);
	~TextLine();

	TextLine &operator=(const TextLine &right);

	uint32 getLength() const;
	const byte *getText() const;

private:
	uint32 _length;
	byte *_str;
};

class Font {
public:
	Font();
	virtual ~Font();

	virtual int32 getFontHeight() const = 0;
	virtual int32 getCharWidth(uint32 c) const = 0;

	virtual bool hasSpaceChars() const = 0;
	virtual bool isSpaceChar(uint32 c) const = 0;

	virtual void drawChar(uint32 c, ::Graphics::Surface &surface, int32 x, int32 y, uint32 color) const = 0;
};

class Saturn2Byte : public Font {
public:
	Saturn2Byte();
	~Saturn2Byte();

	void clear();

	bool load(Resources &resources, const Common::String &file);
	bool load(Common::SeekableReadStream &stream);

	int32 getFontHeight() const;
	int32 getCharWidth(uint32 c) const;

	bool hasSpaceChars() const;
	bool isSpaceChar(uint32 c) const;

	void drawChar(uint32 c, ::Graphics::Surface &surface, int32 x, int32 y, uint32 color) const;

private:
	uint32 _fileSize;
	byte *_fontData;

	static uint16 convertShiftJISToJIS(uint16 c);
	static bool isValidJIS(uint8 j1, uint8 j2);
};

class FontManager {
public:
	typedef Common::Array<TextLine> TextList;

	FontManager(Resources &resources);
	~FontManager();

	bool init(GameVersion gameVersion, Common::Language language);

	void drawText(::Graphics::Surface &surface, const TextLine &text, int32 x, int32 y, uint32 color) const;

	int32 wordWrapText(const TextLine &text, int maxWidth, TextList &lines) const;
	int32 getFontHeight() const;

private:
	Resources *_resources;

	const ::Graphics::Font *_fontLatin1;
	Font *_fontJapanese;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_FONT_H
