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

#include "ring/base/font.h"

#include "graphics/fonts/winfont.h"

#include "ring/helpers.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Font
//////////////////////////////////////////////////////////////////////////
Font::Font(Id id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId) : BaseObject(id) {

	// Setup font description
	memset(&_description, 0, sizeof(_description));
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
	if (!_font->loadFromFON(filename, Graphics::WinFontDirEntry(facename, height)))
		error("Unable to load font %s, face %s size %d", filename.c_str(), facename.c_str(), height);
}

Font::~Font() {
	delete _font;
}

//////////////////////////////////////////////////////////////////////////
// FontHandler
//////////////////////////////////////////////////////////////////////////
FontHandler::FontHandler() {
}

FontHandler::~FontHandler() {
}

void FontHandler::add(Id id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId) {
	if (_fonts.has(id))
		error("[LanguageHandler::add] ID already exists (%d)", id);

	_fonts.push_back(Font(id, filename, facename, height, smallWeight, underline, italic, strikeout, langId));
}

} // End of namespace Ring
