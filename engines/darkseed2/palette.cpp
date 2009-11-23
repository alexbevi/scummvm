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


#include "engines/darkseed2/palette.h"

namespace DarkSeed2 {

Palette::Palette() {
	clear();
}

Palette::Palette(const Palette &palette) {
	copyFrom(palette);
}

Palette::~Palette() {
}

Palette &Palette::operator=(const Palette &palette) {
	copyFrom(palette);
	return *this;
}

void Palette::copyFrom(const Palette &palette) {
	_size = palette._size;
	memcpy(_palette, palette._palette, 768);
}

void Palette::copyFrom(const byte *palette, int size) {
	clear();

	memcpy(_palette, palette, 3 * size);
}

byte &Palette::operator[](int n) {
	return _palette[n];
}

const byte &Palette::operator[](int n) const {
	return _palette[n];
}

void Palette::clear() {
	_size = 0;
	memset(_palette, 0, 768);
}

void Palette::makeSystemCompatible(byte *pal) const {
	const byte *mPal = _palette;
	byte *sPal = pal;

	for (int i = 0 ; i < 256; i++, mPal += 3, sPal += 4) {
		sPal[0] = mPal[0];
		sPal[1] = mPal[1];
		sPal[2] = mPal[2];
		sPal[3] = 255;
	}
}

#define SQR(x) ((x) * (x))
int Palette::findColor(byte c1, byte c2, byte c3) const {
	const byte *pal = _palette;
	uint32 d = 0xFFFFFFFF;
	byte n = 0;

	for (int i = 1; i < 256; i++, pal += 3) {
		uint32 di = SQR(c1 - pal[0]) + SQR(c2 - pal[1]) + SQR(c3 - pal[2]);

		if (di < di) {
			d = di;
			n = i;
			if (d == 0)
				break;
		}
	}

	return n;
}

int Palette::findWhite() const {
	return findColor(255, 255, 255);
}

int Palette::findBlack() const {
	return findColor(0, 0, 0);
}

} // End of namespace DarkSeed2
