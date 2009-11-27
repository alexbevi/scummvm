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

#include "common/str.h"

#include "graphics/fontman.h"
#include "graphics/font.h"

#include "engines/darkseed2/graphicalobject.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

GraphicalObject::GraphicalObject() {
}

GraphicalObject::~GraphicalObject() {
}

Common::Rect GraphicalObject::getArea() const {
	return _area;
}

void GraphicalObject::move(uint32 x, uint32 y) {
	_area.moveTo(x, y);
}

void GraphicalObject::clearArea() {
	_area.left   = 0;
	_area.top    = 0;
	_area.right  = 0;
	_area.bottom = 0;
}


TextObject::TextObject(const Common::String &text, uint32 x, uint32 y,
		byte color, uint32 maxWidth) {

	_color = color;

	if (maxWidth == 0)
		maxWidth = Graphics::_screenWidth;

	::Graphics::FontManager::FontUsage fontUsage = ::Graphics::FontManager::kBigGUIFont;

	const ::Graphics::Font *font =
		::Graphics::FontManager::instance().getFontByUsage(fontUsage);

	Common::StringList lines;

	int width = font->wordWrapText(text, maxWidth, lines);

	for (Common::StringList::iterator it = lines.begin(); it != lines.end(); ++it)
		it->trim();

	_area.left = x;
	_area.top  = y;
	_area.setWidth(width);
	_area.setHeight(lines.size() * font->getFontHeight());

	_sprite = new Sprite;

	_sprite->create(_area.width(), _area.height());
	_sprite->drawStrings(lines, *font, 0, 0, color);
}

TextObject::~TextObject() {
	delete _sprite;
}

void TextObject::redraw(Sprite &sprite, Common::Rect area) {
	if (!_area.intersects(area))
		return;

	area.clip(_area);

	uint32 x = area.left;
	uint32 y = area.top;

	area.moveTo(0, 0);

	sprite.blit(*_sprite, area, x, y, true);
}

void TextObject::recolor(byte color) {
	if (_color == color)
		return;

	_sprite->recolor(_color, color);
	_color = color;
}

uint32 TextObject::wrap(const Common::String &string, Common::StringList &list, uint32 maxWidth) {
	::Graphics::FontManager::FontUsage fontUsage = ::Graphics::FontManager::kBigGUIFont;

	const ::Graphics::Font *font =
		::Graphics::FontManager::instance().getFontByUsage(fontUsage);

	uint32 width = font->wordWrapText(string, maxWidth, list);

	for (Common::StringList::iterator it = list.begin(); it != list.end(); ++it)
		it->trim();

	return width;
}

SpriteObject::SpriteObject() {
	_sprite = 0;
}

SpriteObject::~SpriteObject() {
	delete _sprite;
}

void SpriteObject::clear() {
	clearArea();

	delete _sprite;
	_sprite = 0;
}

bool SpriteObject::isIn(uint32 x, uint32 y) const {
	return _area.contains(x, y);
}

bool SpriteObject::empty() const {
	return _sprite == 0;
}

uint32 SpriteObject::getX() const {
	return _area.left;
}

uint32 SpriteObject::getY() const {
	return _area.top;
}

Sprite &SpriteObject::getSprite() {
	return *_sprite;
}

const Sprite &SpriteObject::getSprite() const {
	return *_sprite;
}

bool SpriteObject::loadFromBMP(Resources &resources, const Common::String &bmp, uint32 x, uint32 y) {
	clear();

	_sprite = new Sprite;

	if (!_sprite->loadFromBMP(resources, bmp))
		return false;

	_area = _sprite->getArea();
	_area.moveTo(x, y);

	return true;
}

void SpriteObject::redraw(Sprite &sprite, Common::Rect area) {
	if (!_area.intersects(area))
		return;

	area.clip(_area);

	uint32 x = area.left;
	uint32 y = area.top;

	area.moveTo(0, 0);

	sprite.blit(*_sprite, area, x, y, true);
}

} // End of namespace DarkSeed2
