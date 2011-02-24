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

#include "ring/graphics/screen.h"

#include "ring/base/application.h"
#include "ring/base/font.h"
#include "ring/base/object.h"
#include "ring/base/text.h"

#include "ring/graphics/image.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/system.h"

#include "graphics/fonts/winfont.h"

namespace Ring {

ScreenManager::ScreenManager() {
}

ScreenManager::~ScreenManager() {
	_screen.free();
}

void ScreenManager::init() {
	_screen.create(640, 480, g_system->getScreenFormat().bytesPerPixel);
}

void ScreenManager::clear() {
	_screen.fillRect(Common::Rect(0, 0, 640, 480), 0);
	g_system->fillScreen(0);
}

void ScreenManager::drawAndUpdate(Image *image) {
	drawAndUpdate(image, Common::Point(0, 0));
}

void ScreenManager::drawAndUpdate(Image *image, Common::Point point) {
	draw(image, point, kDrawType1);
	updateScreen();
}

void ScreenManager::draw(Image *image, DrawType type) {
	draw(image, Common::Point(0, 0), type);
}

void ScreenManager::draw(Image *image, Common::Point point, DrawType type) {
	if (!image)
		error("[ScreenManager::draw] Invalid image pointer!");

	// Check the screen surface bit depth (we only handle 16bpp)
	if (_screen.bytesPerPixel != 2)
		error("[ScreenManager::draw] Engine only handles 16bpp surfaces (was: %d)!", _screen.bytesPerPixel * 8);

	// Check the image bit depth
	switch (image->getBPP()) {
	default:
		error("[ScreenManager::draw] Invalid image bpp (%d)!", image->getBPP());

	case 8:
	case 16:
		image->draw(&_screen, point + _offset);
		break;

	case 24:
	case 32: {
		// Adjust coordinates
		Common::Point coords = point;

		// TODO intersect image rectangle with screen and get update drawing rect

		switch (type) {
		default:
			error("[ScreenManager::draw] Invalid draw type (%d)!", type);

		case 1:
			image->draw(&_screen, coords + _offset);
			break;

		case 2:
			warning("[ScreenManager::draw] Not implemented type 2");
			break;

		case 3:
			warning("[ScreenManager::draw] Not implemented type 2");
			break;
		}
		}
		break;
	}
}

void ScreenManager::drawImage(Image *image, Common::Point dest, int srcWidth, int srcHeight, int srcX, int offset) {
	image->draw(&_screen, dest, srcWidth, srcHeight, srcX, offset);

	updateScreen();
}

void ScreenManager::drawRectangle(Common::Rect rect, uint32 color) {
	_screen.frameRect(rect, color);
}

void ScreenManager::drawText(Common::String text, Common::Point coords, Color color) {
	Graphics::WinFont *font = getApp()->getFontHandler()->getFont(kFontDefault);
	if (!font)
		error("[ScreenManager::drawText] Cannot get the font to draw text (id: %d)", kFontDefault);

	// Draw text
	uint32 width = font->getStringWidth(text);
	font->drawString(&_screen, text, coords.x, coords.y, width, color.getColor());
}

void ScreenManager::drawText(Text *text) {
	if (!text)
		return;

	// Check text presentation
	if (text->getPresentation() && !text->getPresentation()->isShown())
		return;

	// Draw background if needed
	if (!text->hasTransparentBackground())
		_screen.fillRect(text->getBoundingBox(), text->getBackgroundColor());

	Graphics::WinFont *font = getApp()->getFontHandler()->getFont(kFontDefault);
	if (text->getFontId())
		font = getApp()->getFontHandler()->getFont(text->getFontId());

	if (!font)
		error("[ScreenManager::drawText] Cannot get the font to draw text (id: %d)", text->getFontId() ? text->getFontId() : kFontDefault);

	// Draw text
	font->drawString(&_screen, text->getString(), text->getCoordinates().x, text->getCoordinates().y, text->getWidth(), text->getForegroundColor());
}

void ScreenManager::updateScreen() {
	g_system->fillScreen(0);
	g_system->copyRectToScreen((byte *)_screen.getBasePtr(0, 0), 640 * 2, 0, 0, 640, 480);
}

} // End of namespace Ring
