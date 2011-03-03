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

#include "ring/base/application.h"
#include "ring/base/font.h"

#include "ring/ring.h"
#include "ring/helpers.h"

#include "graphics/fonts/winfont.h"

namespace Ring {

Text::Text() {
	_fontId  = kFontInvalid;
	_foregroundColor = 0;
	_width = 0;
	_height = 0;
	_transparentBackground = false;
	_backgroundColor = 0;
	_objectPresentation = NULL;
}

Text::~Text() {
	_objectPresentation = NULL;
}

void Text::init(Common::String text, const Common::Point &point, FontId fontId, Color foreground, Color background) {
	_text = text;
	_fontId = fontId;
	_point = point;
	_foregroundColor = foreground.getColor();

	set(text);

	if (!background.isTransparent()) {
		_transparentBackground = false;
		_objectPresentation = NULL;
		_backgroundColor = background.getColor();
	} else {
		_transparentBackground = true;
		_backgroundColor = 0;
		_objectPresentation = NULL;
	}
}

void Text::set(Common::String text) {
	_text = text;

	// Get the text font and calculate text dimensions
	Graphics::WinFont *font = getApp()->getFontHandler()->getFont(_fontId);
	if (font) {
		_width = (uint32)font->getStringWidth(text);
		_height = (uint32)font->getFontHeight();
	} else {
		_width = 0;
		_height = 0;
	}
}

void Text::draw() {
	error("[Text::draw] Not implemented");
}

void Text::setForegroundColor(Color foreground) {
	_foregroundColor = foreground.getColor();
}

void Text::setBackgroundColor(Color background) {
	if (!background.isTransparent()) {
		_transparentBackground = false;
		_backgroundColor = background.getColor();
	} else {
		_transparentBackground = true;
		_backgroundColor = 0;
	}
}

Common::Rect Text::getBoundingBox() {
	return Common::Rect(_point.x, _point.y, _point.x + _width, _point.y + _height);
}

#pragma region Serializable

void Text::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncString(_text);
	s.syncAsUint32LE(_fontId);
	s.syncAsSint16LE(_point.x);
	s.syncAsSint16LE(_point.y);
	s.syncAsUint32LE(_foregroundColor);
	s.syncAsUint32LE(_width);
	s.syncAsUint32LE(_height);
	s.syncAsByte(_transparentBackground);
	s.syncAsUint32LE(_backgroundColor);
}

#pragma endregion

} // End of namespace Ring
