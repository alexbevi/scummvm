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

/** A line of text, no matter the encoding. */
class TextLine {
public:
	/** Create an empty line. */
	TextLine();
	/** Create a line out of a memory area. */
	TextLine(const byte *str, uint32 length);
	/** Create a line out of stream. */
	TextLine(Common::SeekableReadStream &stream);
	/** Create a line out of string. */
	TextLine(const Common::String &str);
	TextLine(const TextLine &right);
	~TextLine();

	TextLine &operator=(const TextLine &right);

	/** Return the length in bytes of the line. */
	uint32 getLength() const;
	/** Return the text of the line. */
	const byte *getText() const;

	void append(const TextLine &line);
	void append(const byte *str, uint32 length);

	/** Trim n bytes from the front of the line. */
	void trimFront(uint32 n);
	/** Trim n bytes from the back of the line. */
	void trimBack(uint32 n);

private:
	uint32 _length;
	byte *_str;
	byte *_mem;
};

/** A font. */
class Font {
public:
	Font();
	virtual ~Font();

	/** Return the height of the font's characters. */
	virtual int32 getFontHeight() const = 0;
	/** Return the width of a certain character. */
	virtual int32 getCharWidth(uint32 c) const = 0;

	/** Read the character at that memory location. */
	virtual uint32 getChar(const byte *str) const = 0;
	/** Proceed the pointer to the next character. */
	virtual const byte *nextChar(const byte *str) const = 0;

	/** Return the length of the line in characters. */
	virtual uint32 getStringLength(const TextLine &line) const = 0;

	/** Is this a valid position to break a line? */
	virtual bool validBreakSpace(const byte *textStart, const byte *curPosition) const = 0;
	/** Can this character be trimmed off a line? */
	virtual bool isTrimmable(uint32 c) const = 0;

	/** Draw a character onto a surface. */
	virtual void drawChar(uint32 c, ::Graphics::Surface &surface, int32 x, int32 y, uint32 color) const = 0;
};

/** A Japanese 2-bytes font used by the Sega Saturn. */
class Saturn2Byte : public Font {
public:
	Saturn2Byte();
	~Saturn2Byte();

	void clear();

	bool load(Resources &resources, const Common::String &file);
	bool load(Common::SeekableReadStream &stream);

	int32 getFontHeight() const;
	int32 getCharWidth(uint32 c) const;

	uint32 getChar(const byte *str) const;
	const byte *nextChar(const byte *str) const;

	uint32 getStringLength(const TextLine &line) const;

	bool validBreakSpace(const byte *textStart, const byte *curPosition) const;
	bool isTrimmable(uint32 c) const;

	void drawChar(uint32 c, ::Graphics::Surface &surface, int32 x, int32 y, uint32 color) const;

private:
	uint32 _fileSize;
	byte *_fontData;

	static uint16 convertShiftJISToJIS(uint16 c);
	static bool isValidJIS(uint8 j1, uint8 j2);
};

/** A ScummVM-provided latin1 font. */
class ScummVMLatin1 : public Font {
public:
	ScummVMLatin1();
	~ScummVMLatin1();

	int32 getFontHeight() const;
	int32 getCharWidth(uint32 c) const;

	uint32 getChar(const byte *str) const;
	const byte *nextChar(const byte *str) const;

	uint32 getStringLength(const TextLine &line) const;

	bool validBreakSpace(const byte *textStart, const byte *curPosition) const;
	bool isTrimmable(uint32 c) const;

	void drawChar(uint32 c, ::Graphics::Surface &surface, int32 x, int32 y, uint32 color) const;

private:
	const ::Graphics::Font *_font;
};

/** The font manager. */
class FontManager {
public:
	typedef Common::Array<TextLine> TextList;

	FontManager(Resources &resources);
	~FontManager();

	/** Initialize the font manager for the specified game version and language. */
	bool init(GameVersion gameVersion, Common::Language language);

	/** Draw a text line onto a surface. */
	void drawText(::Graphics::Surface &surface, const TextLine &text, int32 x, int32 y, uint32 color) const;

	/** Wrap the text line. */
	int32 wordWrapText(const TextLine &text, int maxWidth, TextList &lines) const;
	/** Get the height of the font used. */
	int32 getFontHeight() const;

	/** Trim unecessary characters off a text line. */
	void trim(TextLine &text) const;
	/** Trim unecessary characters off text lines. */
	void trim(TextList &lines) const;

private:
	Resources *_resources;

	Font *_font;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_FONT_H
