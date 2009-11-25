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

#include "common/array.h"

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

	/** Create a palette that's compatible with ScummVM's O_System. */
	void makeSystemCompatible(byte *pal) const;

	/** Find the color that's nearest to the specified color. */
	byte findColor(byte c1, byte c2, byte c3, uint32 *diff = 0) const;
	byte findWhite() const;
	byte findBlack() const;

	/** Merge with the current palette, returning a change set for images using the other palette. */
	Common::Array<byte> merge(const Palette &palette, bool average = false);

	byte addColor(byte c1, byte c2, byte c3, bool average = false);

private:
	struct Match {
		byte index1;
		byte index2;
		uint32 diff;

		bool operator<(const Match &match) const;
	};

	int _size;
	byte _palette[768];

	void addPalette(const Palette &palette);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_PALETTE_H
