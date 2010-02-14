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
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

GraphicalObject::GraphicalObject() {
}

GraphicalObject::~GraphicalObject() {
}

Common::Rect GraphicalObject::getArea() const {
	return _area;
}

void GraphicalObject::moveTo(int32 x, int32 y) {
	// Sanity checks
	assert((ABS(x) <= 0x7FFF) && (ABS(y) <= 0x7FFF));

	_area.moveTo(x, y);
}

void GraphicalObject::move(int32 x, int32 y) {
	// Sanity checks
	assert((ABS(x) <= 0x7FFF) && (ABS(y) <= 0x7FFF));

	_area.translate(x, y);
}

void GraphicalObject::clearArea() {
	_area.left   = 0;
	_area.top    = 0;
	_area.right  = 0;
	_area.bottom = 0;
}


TextObject::TextObject(const Common::String &text, int32 x, int32 y,
		byte color, int32 maxWidth) {

	// Sanity checks
	assert((x >= 0) && (y >= 0) && (x <= 0x7FFF) && (y <= 0x7FFF));

	_color = color;

	if (maxWidth <= 0)
		maxWidth = Graphics::kScreenWidth;

	// We want the big font
	::Graphics::FontManager::FontUsage fontUsage = ::Graphics::FontManager::kBigGUIFont;
	const ::Graphics::Font *font =
		::Graphics::FontManager::instance().getFontByUsage(fontUsage);

	Common::StringList lines;

	// Wrap the string
	int width = font->wordWrapText(text, maxWidth, lines);

	// Removing leading and trailing spaces from all resulting lines
	for (Common::StringList::iterator it = lines.begin(); it != lines.end(); ++it)
		it->trim();

	// Set area
	_area.left = x;
	_area.top  = y;
	_area.setWidth(width);
	_area.setHeight(lines.size() * font->getFontHeight());

	// Create sprite
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

	int32 x = area.left;
	int32 y = area.top;

	area.moveTo(area.left - _area.left, area.top - _area.top);

	sprite.blit(*_sprite, area, x, y, true);
}

void TextObject::recolor(byte color) {
	if (_color == color)
		// Color didn't change
		return;

	_sprite->recolor(_color, color);
	_color = color;
}

int32 TextObject::wrap(const Common::String &string, Common::StringList &list, int32 maxWidth) {
	if (maxWidth <= 0)
		maxWidth = Graphics::kScreenWidth;

	// We want the big font
	::Graphics::FontManager::FontUsage fontUsage = ::Graphics::FontManager::kBigGUIFont;
	const ::Graphics::Font *font =
		::Graphics::FontManager::instance().getFontByUsage(fontUsage);

	// Wrap the string
	int32 width = font->wordWrapText(string, maxWidth, list);

	// Removing leading and trailing spaces from all resulting lines
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

void SpriteObject::moveTo() {
	moveTo(_sprite->getDefaultX(), _sprite->getDefaultY());
}

void SpriteObject::moveTo(int32 x, int32 y) {
	GraphicalObject::moveTo(x, y);
}

void SpriteObject::moveFeetTo(int32 x, int32 y) {
	GraphicalObject::moveTo(x - _sprite->getFeetX(), y - _sprite->getFeetY());
}

void SpriteObject::move(int32 x, int32 y) {
	GraphicalObject::move(x, y);
}

void SpriteObject::clear() {
	clearArea();

	delete _sprite;
	_sprite = 0;
}

bool SpriteObject::isIn(int32 x, int32 y) const {
	return _area.contains(x, y);
}

bool SpriteObject::empty() const {
	return _sprite == 0;
}

int32 SpriteObject::getX() const {
	return _area.left;
}

int32 SpriteObject::getY() const {
	return _area.top;
}

int32 SpriteObject::getFeetX() const {
	return getX() + _sprite->getFeetX();
}

int32 SpriteObject::getFeetY() const {
	return getY() + _sprite->getFeetY();
}

frac_t SpriteObject::getScale() const {
	return _sprite->getScale();
}

frac_t SpriteObject::calculateScaleVal(int32 height) {
	// Sanity checks
	assert((height >= 0) && (height <= 0x7FFF));

	return intToFrac(height) / _sprite->getHeight(true);
}

void SpriteObject::setScale(frac_t scale) {
	if (scale == _sprite->getScale())
		// Nothing to do
		return;

	// Remember the current feet position
	int32 feetX = _area.left + _sprite->getFeetX();
	int32 feetY = _area.top  + _sprite->getFeetY();

	// Scale
	_sprite->setScale(scale);

	// Update the drawing area
	_area.setWidth(_sprite->getWidth());
	_area.setHeight(_sprite->getHeight());

	// Move the feet back to their original position
	moveFeetTo(feetX, feetY);
}

Sprite &SpriteObject::getSprite() {
	return *_sprite;
}

const Sprite &SpriteObject::getSprite() const {
	return *_sprite;
}

bool SpriteObject::loadFromBMP(Resources &resources, const Common::String &bmp) {
	clear();

	_sprite = new Sprite;

	if (!_sprite->loadFromBMP(resources, bmp))
		return false;

	_area = _sprite->getArea();
	_area.moveTo(_sprite->getDefaultX(), _sprite->getDefaultY());

	return true;
}

void SpriteObject::redraw(Sprite &sprite, Common::Rect area) {
	if (!_area.intersects(area))
		return;

	area.clip(_area);

	int32 x = area.left;
	int32 y = area.top;

	area.moveTo(area.left - _area.left, area.top - _area.top);

	sprite.blit(*_sprite, area, x, y, true);
}


Animation::Animation() {
	_visible  = false;
	_curFrame = 0;
}

Animation::~Animation() {
	clear();
}

void Animation::clear() {
	for (Common::Array<SpriteObject *>::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete *it;

	_sprites.clear();
	_frames.clear();
	_name.clear();

	_visible   = false;
	_curFrame  = 0;
}

bool Animation::empty() const {
	return _frames.empty();
}

int Animation::frameCount() const {
	return _frames.size();
}

int Animation::currentFrame() const {
	return _curFrame;
}

bool Animation::isVisible() const {
	return _visible;
}

void Animation::setVisible(bool visible) {
	_visible = visible;
}

void Animation::moveTo() {
	for (Common::Array<SpriteObject *>::iterator frame = _frames.begin(); frame != _frames.end(); ++frame)
		(*frame)->moveTo();
}

void Animation::moveTo(int32 x, int32 y) {
	for (Common::Array<SpriteObject *>::iterator frame = _frames.begin(); frame != _frames.end(); ++frame)
		(*frame)->moveTo(x, y);
}

void Animation::moveFeetTo(int32 x, int32 y) {
	for (Common::Array<SpriteObject *>::iterator frame = _frames.begin(); frame != _frames.end(); ++frame)
		(*frame)->moveFeetTo(x, y);
}

void Animation::move(int32 x, int32 y) {
	for (Common::Array<SpriteObject *>::iterator frame = _frames.begin(); frame != _frames.end(); ++frame)
		(*frame)->move(x, y);
}

frac_t Animation::calculateScaleVal(int frame, int32 height) {
	if (frame < 0)
		frame = _curFrame;

	if (((uint) frame) >= _frames.size())
		return FRAC_ONE;

	return _frames[frame]->calculateScaleVal(height);
}

void Animation::setScale(frac_t scale) {
	for (Common::Array<SpriteObject *>::iterator frame = _frames.begin(); frame != _frames.end(); ++frame)
		(*frame)->setScale(scale);
}

void Animation::setFrame(int frame) {
	if (frame < 0)
		return;

	_curFrame = frame % _frames.size();
}

void Animation::nextFrame() {
	_curFrame = (_curFrame + 1) % _frames.size();
}

void Animation::previousFrame() {
	_curFrame--;
	if (_curFrame < 0)
		_curFrame = _frames.size() - 1;
}

void Animation::operator++(int) {
	nextFrame();
}

void Animation::operator--(int) {
	previousFrame();
}

const Common::String &Animation::getName() const {
	return _name;
}

bool Animation::load(Resources &resources, const Common::String &base) {
	clear();

	// Find the frame with the biggest number that still exists
	uint8 count = 0;
	for (int i = 99; i > 0; i--) {
		if (resources.hasResource(Resources::addExtension(base + Common::String::printf("%02d", i), "BMP"))) {
			count = i;
			break;
		}
	}

	// None found
	if (count == 0) {
		// Try to open the file without a frame number attached
		if (!resources.hasResource(Resources::addExtension(base, "BMP"))) {
			warning("Animation::load(): No such animation \"%s\"", base.c_str());
			return false;
		}

		// Load it
		SpriteObject *object = new SpriteObject;
		if (!object->loadFromBMP(resources, base)) {
			warning("Animation::load(): Failed loading sprite \"%s\"", base.c_str());
			delete object;
			return false;
		}

		// Put it into the arrays
		_sprites.push_back(object);
		_frames.push_back(object);
		return true;
	}

	_frames.resize(count);
	for (int i = 0; i < count; i++) {
		// Open every frame in sequence
		Common::String bmp = base + Common::String::printf("%02d", i + 1);

		SpriteObject *object = new SpriteObject;
		if (!object->loadFromBMP(resources, bmp)) {
			// Frame doesn't exist

			// If it's the first one, take the newly created empty one.
			if (i > 0) {
				// Otherwise, take the last one.
				delete object;
				object = _frames[i - 1];
			} else
				_sprites.push_back(object);

		} else
			// Actually exist, also remember it here
			_sprites.push_back(object);

		// Put the frame into the array
		_frames[i] = object;
	}

	_name = base;

	return true;
}

void Animation::flipHorizontally() {
	for (Common::Array<SpriteObject *>::iterator sprite = _sprites.begin(); sprite != _sprites.end(); ++sprite)
		(*sprite)->getSprite().flipHorizontally();
}

void Animation::flipVertically() {
	for (Common::Array<SpriteObject *>::iterator sprite = _sprites.begin(); sprite != _sprites.end(); ++sprite)
		(*sprite)->getSprite().flipVertically();
}

SpriteObject &Animation::getFrame(int n) {
	return *_frames[n];
}

const SpriteObject &Animation::getFrame(int n) const {
	return *_frames[n];
}

SpriteObject &Animation::operator[](int n) {
	return getFrame(n);
}

const SpriteObject &Animation::operator[](int n) const {
	return getFrame(n);
}

SpriteObject &Animation::getCurrentFrame() {
	return *_frames[_curFrame];
}

const SpriteObject &Animation::getCurrentFrame() const {
	return *_frames[_curFrame];
}

SpriteObject &Animation::operator*() {
	return getCurrentFrame();
}

const SpriteObject &Animation::operator*() const {
	return getCurrentFrame();
}

SpriteObject *Animation::operator->() {
	return &getCurrentFrame();
}

const SpriteObject *Animation::operator->() const {
	return &getCurrentFrame();
}

} // End of namespace DarkSeed2
