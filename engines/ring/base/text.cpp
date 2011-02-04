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

#include "ring/base/text.h"

#include "ring/base/font.h"

#include "ring/game/application.h"

#include "graphics/fonts/winfont.h"

namespace Ring {

Text::Text(Application *application) : _application(application) {
	_fontId  = kFontInvalid;
	_field_8  = 0;
	_field_C  = 0;
	_field_10 = 0;
	_width = 0;
	_height = 0;
	_field_1C = 0;
	_field_1D = 0;
	_objectPresentation = NULL;
}

Text::~Text() {
	_objectPresentation = NULL;

	// Zero-out passed pointers
	_application = NULL;
}

void Text::init(Common::String text, uint32 a1, uint32 a2, FontId fontId, byte a4, byte a5, byte a6, int32 a7, int32 a8, int32 a9) {
	_text = text;
	_fontId = fontId;
	_field_8 = a1;
	_field_C = a2;
	_field_10 = a4 | ((a5 | a6 << 4) << 8);

	set(text);

	if (a7 != -1 || a8 != -1 || a9 != -1) {
		_field_1C = 0;
		_objectPresentation = NULL;
		_field_1D = (byte)a7 | ((a8 | a9 << 4) << 8);
	} else {
		_field_1C = 1;
		_field_1D = 0;
		_objectPresentation = NULL;
	}
}

void Text::set(Common::String text) {
	_text = text;

	// Get the text font and calculate text dimensions
	Graphics::WinFont *font = _application->getFontHandler()->getFont(_fontId);
	if (font) {
		_width = font->getStringWidth(text);
		_height = font->getFontHeight();
	} else {
		_width = 0;
		_height = 0;
	}
}

} // End of namespace Ring
