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
}

Graphics::~Graphics() {
}

void Graphics::setPalette(const byte *pal) {
	byte palette[1024];

	for (int i = 0; i < 256; i++) {
		palette[i * 4 + 0] = pal[i * 3 + 0];
		palette[i * 4 + 1] = pal[i * 3 + 1];
		palette[i * 4 + 2] = pal[i * 3 + 2];
		palette[i * 4 + 3] = 255;
	}

	g_system->setPalette(palette, 0, 256);
}

} // End of namespace DarkSeed2
