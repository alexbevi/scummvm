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

	/** Copy from another palette. */
	void copyFrom(const Palette &palette);
	/** Copy from another palette. */
	void copyFrom(const byte *palette, int size);

	/** Number of entries filled. */
	int getSize() const;

	/** Return the complete palette data. */
	byte *get();
	/** Return the complete palette data. */
	const byte *get() const;

	/** Return a specific palette index. */
	byte &operator[](int n);
	/** Return a specific palette index. */
	const byte &operator[](int n) const;

	/** Clear the palette. */
	void clear();

	/** Create a palette that's compatible with ScummVM's O_System. */
	void makeSystemCompatible(byte *pal) const;

	/** Find the color that's nearest to the specified color. */
	byte findColor(byte c1, byte c2, byte c3, uint32 *diff = 0) const;
	/** Find the color index that's nearest to pure white. */
	byte findWhite() const;
	/** Find the color index that's nearest to pure black. */
	byte findBlack() const;

	/** Merge with the current palette, returning a change set for images using the other palette. */
	Common::Array<byte> merge(const Palette &palette, bool average = false);

	/** Add the color to the palette. */
	byte addColor(byte c1, byte c2, byte c3, bool average = false);

private:
	/** A palettes comparison match. */
	struct Match {
		byte   index1; ///< Color index in the first palette.
		byte   index2; ///< Color index in the second palette.
		uint32 diff;   ///< Difference between the two colors.

		bool operator<(const Match &match) const;
	};

	int  _size;         ///< Number of indices filled.
	byte _palette[768]; ///< The palette data.

	/** Add another palette to the back. */
	void addPalette(const Palette &palette);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_PALETTE_H
