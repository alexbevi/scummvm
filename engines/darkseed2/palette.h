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

#ifndef DARKSEED2_PALETTE_H
#define DARKSEED2_PALETTE_H

#include "engines/darkseed2/darkseed2.h"

namespace DarkSeed2 {

class Palette {
public:
	Palette();
	Palette(const Palette &palette);
	~Palette();

	Palette &operator=(const Palette &palette);

	void copyFrom(const Palette &palette);
	void copyFrom(const byte *palette, int size);

	int getSize() const;

	byte &operator[](int n);
	const byte &operator[](int n) const;

	void clear();

	void makeSystemCompatible(byte *pal) const;

	int findColor(byte c1, byte c2, byte c3) const;
	int findWhite() const;
	int findBlack() const;

private:
	int _size;
	byte _palette[768];
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_PALETTE_H
