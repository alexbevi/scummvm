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

#include "ring/graphics/image.h"

#include "common/system.h"

namespace Ring {

ScreenManager::ScreenManager() {
}

ScreenManager::~ScreenManager() {
	_screen.free();
}

void ScreenManager::init() {
	_screen.create(640, 480, 2);
}

void ScreenManager::clear() {
	g_system->fillScreen(0);
}

void ScreenManager::drawAndUpdate(Image *image) {
	drawAndUpdate(image, Common::Point(0, 0));
}

void ScreenManager::drawAndUpdate(Image *image, Common::Point point) {
	draw(image, point, 1);
	updateScreen();
}

void ScreenManager::draw(Image *image, byte type) {
	draw(image, Common::Point(0, 0), type);
}

void ScreenManager::draw(Image *image, Common::Point point, byte type) {
	warning("[ScreenManager::update] Not implemented");

	// HACK (direct surface copy, ignore drawing type)
	image->draw(&_screen, point, image->getWidth(), image->getHeight(), 0, 0);
}

void ScreenManager::drawImage(Image *image, Common::Point dest, int srcWidth, int srcHeight, int srcX, int offset) {
	image->draw(&_screen, dest, srcWidth, srcHeight, srcX, offset);

	updateScreen();
}

void ScreenManager::drawRectangle(Common::Rect rect, uint32 color) {
	_screen.frameRect(rect, color);
}

void ScreenManager::updateScreen() {
	g_system->fillScreen(0);
	g_system->copyRectToScreen((byte *)_screen.getBasePtr(0, 0), 640 * 2, 0, 0, 640, 480);
}

} // End of namespace Ring
