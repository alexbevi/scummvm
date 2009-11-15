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

#include "engines/darkseed2/graphics.h"

namespace DarkSeed2 {

Graphics::Graphics() {
	clearPalette();

	_screen.create(_screenWidth, _screenHeight);
}

Graphics::~Graphics() {
}

void Graphics::clearPalette() {
	memset(_gamePalette, 0, 768);
	applyGamePalette();
}

void Graphics::setPalette(const byte *pal) {
	memcpy(_gamePalette, pal, 768);

	// Palette entry 0 is transparent
	_gamePalette[0] = 0;
	_gamePalette[1] = 0;
	_gamePalette[2] = 0;

	applyGamePalette();
}

void Graphics::applyGamePalette() {
	byte pal[1024];

	const byte *gPal = _gamePalette;
	byte *sPal = pal;

	for (int i = 0 ; i < 256; i++, gPal += 3, sPal += 4) {
		sPal[0] = gPal[0];
		sPal[1] = gPal[1];
		sPal[2] = gPal[2];
		sPal[3] = 255;
	}

	g_system->setPalette(pal, 0, 256);
}

void Graphics::blitToScreen(const Sprite &from,
		uint32 left, uint32 top, uint32 right, uint32 bottom,
		uint32 x, uint32 y, bool transp) {

	_screen.blit(from, left, top, right, bottom, x, y, transp);
}

void Graphics::blitToScreen(const Sprite &from, uint32 x, uint32 y, bool transp) {
	_screen.blit(from, x, y, transp);
}

void Graphics::retrace() {
	g_system->copyRectToScreen(_screen.getData(), _screen.getWidth(),
			0, 0, _screen.getWidth(), _screen.getHeight());
	g_system->updateScreen();
}

} // End of namespace DarkSeed2
