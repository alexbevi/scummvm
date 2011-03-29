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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/compression.h $
 * $Id: compression.h 12 2010-01-23 12:01:58Z alexbevi $
 *
 */

#ifndef DGDS_COMPRESSION_H
#define DGDS_COMPRESSION_H

#include "common/memstream.h"

namespace Dgds {

Common::MemoryReadStream* decompLZW(Common::SeekableReadStream *in, int size);
Common::MemoryReadStream* decompRLE(Common::SeekableReadStream *in, int size);

class LZW {
public:
	LZW(Common::SeekableReadStream *res, byte *data);

private:
	int nextbit;
	int current;
	Common::SeekableReadStream *_res;

	unsigned int GetBits(const unsigned int n);
	unsigned int GetNextBit();
	void SkipBits();
};

} // End of namespace Dgds

#endif // DGDS_COMPRESSION_H
