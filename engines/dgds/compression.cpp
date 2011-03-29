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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/compression.cpp $
 * $Id: compression.cpp 12 2010-01-23 12:01:58Z alexbevi $
 *
 */

#include "dgds/compression.h"

#include "common/textconsole.h" // for warning() :P


namespace Dgds {

Common::MemoryReadStream* decompLZW(Common::SeekableReadStream *in, int size) {
	byte *data = new byte[size];
	LZW l(in, data);
	Common::MemoryReadStream *stream = new Common::MemoryReadStream(data, size, DisposeAfterUse::YES);
	return stream;
}

Common::MemoryReadStream* decompRLE(Common::SeekableReadStream *in, int size) {
	byte *data = new byte[size];
	int32 pos = -1;
	while (!in->eos()) {
		byte control = in->readByte();
		if (control & 0x80) {
			byte len = in->readByte();
			for (int32 i = 0; i < len; i++)
				data[pos++] = control & 0x7F;
		} else {
			for (int32 i = 0; i < control; i++)
				data[pos++] = in->readByte();
		}
	}

	if (pos != size)
		warning ("decompRLE expected %d, wrote %d", size, pos);

	Common::MemoryReadStream *stream = new Common::MemoryReadStream(data, size, DisposeAfterUse::YES);
	return stream;
}

// LZW
void LZW::SkipBits() {
	if (nextbit) {
		_res->skip(1); //XXX
		nextbit = 0;
		current = _res->readByte();
	}
}

unsigned int LZW::GetBits(const unsigned int n) {
	unsigned int x = 0;
	for (unsigned int i = 0; i < n; i++) {
		if (current & (1 << nextbit)) {
			x += (1 << i);
		}
		nextbit++;
		if (nextbit > 7) {
			current = _res->readByte();
			nextbit = 0;
		}
	}
	return x;
}

typedef struct _CodeTableEntry {
	uint16 prefix;
	uint8  append;
} CodeTableEntry;

LZW::LZW(Common::SeekableReadStream *res, byte *data) {
	_res = res;
	current = _res->readByte();
	int posout = 0;

	nextbit = 0;

	CodeTableEntry *codetable = new CodeTableEntry[4096];
	uint8 *decodestack = new uint8[4096];
	uint8 *stackptr = decodestack;
	unsigned int n_bits = 9;
	unsigned int free_entry = 257;
	unsigned int oldcode = GetBits(n_bits);
	unsigned int lastbyte = oldcode;
	unsigned int bitpos = 0;

	data[posout] = oldcode;
	posout++;

	while (!_res->eos()) {
		unsigned int newcode = GetBits(n_bits);
		bitpos += n_bits;
		if (newcode == 256) {
			SkipBits();
			_res->skip((((bitpos-1)+((n_bits<<3)-(bitpos-1+(n_bits<<3))%(n_bits<<3)))-bitpos)>>3);
			n_bits = 9;
			free_entry = 256;
			bitpos = 0;
		} else {
			unsigned int code = newcode;
			if (code >= free_entry) {
				*stackptr++ = lastbyte;
				code = oldcode;
			}
			while (code >= 256) {
				*stackptr++ = codetable[code].append;
				code = codetable[code].prefix;
			}
			*stackptr++ = code;
			lastbyte = code;
			while (stackptr > decodestack) {
				data[posout] = (*--stackptr);
				posout++;
			}
			if (free_entry < 4096) {
				codetable[free_entry].prefix = oldcode;
				codetable[free_entry].append = lastbyte;
				free_entry++;
				if ((free_entry >= (unsigned int)(1 << n_bits)) && (n_bits < 12)) {
					n_bits++;
					bitpos = 0;
				}
			}
			oldcode = newcode;
		}
	}
	delete[] decodestack;
	delete[] codetable;
}

} // End of namespace Dgds
