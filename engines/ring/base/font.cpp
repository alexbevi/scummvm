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

#include "ring/base/font.h"

#include "ring/helpers.h"

#include "graphics/fonts/winfont.h"

namespace Ring {

#pragma region Font

Font::Font(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId) : BaseObject(id) {

	// Setup font description
	_description.facename = facename;
	_description.height = height;
	_description.weight = smallWeight ? 400 : 700;
	_description.underline = underline;
	_description.italic = italic;
	_description.strikeOut = strikeout;

	switch (langId) {
	default:
		break;

	case kLanguageGreek:
		_description.charSet = 161;  // GREEK_CHARSET
		break;

	case kLanguageHebrew:
		_description.charSet = 2;    // SYMBOL_CHARSET
		break;
	}

	// Create font object
	_font = new Graphics::WinFont();
	if (!_font->loadFromFON(filename, Graphics::WinFontDirEntry(_description.facename, (uint16)_description.height)))
		error("Unable to load font %s, face %s size %d", filename.c_str(), _description.facename.c_str(), _description.height);
}

Font::~Font() {
	delete _font;
}

#pragma endregion

#pragma region FontHandler

FontHandler::FontHandler() {
}

FontHandler::~FontHandler() {
	CLEAR_ARRAY(Font, _fonts);
}

void FontHandler::add(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId) {
	if (_fonts.has(id))
		error("[FontHandler::add] ID already exists (%d)", id);

	_fonts.push_back(new Font(id, filename, facename, height, smallWeight, underline, italic, strikeout, langId));
}

Graphics::WinFont *FontHandler::getFont(FontId id) {
	if (!_fonts.has(id))
		error("[FontHandler::getFont] ID doesn't exists (%d)", id);

	return _fonts.get(id)->getFont();
}

#pragma endregion

} // End of namespace Ring
