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
 */

#ifndef RING_FONT_H
#define RING_FONT_H

#include "ring/shared.h"

namespace Graphics {
	class WinFont;
}

namespace Ring {

class Font : public BaseObject {
public:
	Font() : BaseObject(kFontInvalid) {}
	Font(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId);
	~Font();

	Graphics::WinFont *getFont() { return _font; }

private:
	// Font description (see Windows LOGFONT structure)
	struct FontDescription {
		uint32 height;
		uint32 width;
		uint32 escapement;
		uint32 orientation;
		uint32 weight;
		bool   italic;
		bool   underline;
		bool   strikeOut;
		byte   charSet;
		byte   outPrecision;
		byte   clipPrecision;
		byte   quality;
		byte   pitchAndFamily;
		Common::String facename;

		FontDescription() {
			height = 0;
			width = 0;
			escapement = 0;
			orientation = 0;
			weight = 0;
			italic = false;
			underline = false;
			strikeOut = false;
			charSet = 0;
			outPrecision = 0;
			clipPrecision = 0;
			quality = 0;
			pitchAndFamily = 0;
		}
	};

	FontDescription _description;
	Graphics::WinFont *_font;
	Common::String _filename;
};

class FontHandler {
public:
	FontHandler();
	~FontHandler();

	void add(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId);

	Graphics::WinFont *getFont(FontId id);

private:
	AssociativeArray<Font *> _fonts;
};

} // End of namespace Ring

#endif // RING_FONT_H
