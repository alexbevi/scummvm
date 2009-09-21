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

#include "gargoyle/frotz_funcs.h"

namespace Frotz {
/*
uint32 RGB5ToTrue(unsigned short five) {
  int r = five&0x001F;
  int g = (five&0x03E0)>>5;
  int b = (five&0x7C00)>>10;
  return (r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2);
}
*/

/*
 * mem_diff
 *
 * Set diff to a Quetzal-like difference between a and b,
 * copying a to b as we go.  It is assumed that diff points to a
 * buffer which is large enough to hold the diff.
 * mem_size is the number of bytes to compare.
 * Returns the number of bytes copied to diff.
 *
 */

long mem_diff(zbyte *a, zbyte *b, zword mem_size, zbyte *diff) {
    unsigned size = mem_size;
    zbyte *p = diff;
    unsigned j;
    zbyte c = 0;

    for (;;) {
		for (j = 0; size > 0 && (c = *a++ ^ *b++) == 0; j++)
			size--;
		if (size == 0) break;
		size--;

		if (j > 0x8000) {
			*p++ = 0;
			*p++ = 0xff;
			*p++ = 0xff;
			j -= 0x8000;
		}
		if (j > 0) {
			*p++ = 0;
			j--;
			if (j <= 0x7f) {
				*p++ = j;
			} else {
				*p++ = (j & 0x7f) | 0x80;
				*p++ = (j & 0x7f80) >> 7;
			}
		}
		*p++ = c;
		*(b - 1) ^= c;
	}

    return p - diff;
}/* mem_diff */

/*
 * mem_undiff
 *
 * Applies a quetzal-like diff to dest
 *
 */

void mem_undiff(zbyte *diff, long diff_length, zbyte *dest) {
    zbyte c;

    while (diff_length) {
		c = *diff++;
		diff_length--;
		if (c == 0) {
		    unsigned runlen;

		    if (!diff_length)
				return;  /* Incomplete run */
			runlen = *diff++;
			diff_length--;
			if (runlen & 0x80) {
				if (!diff_length)
					return; /* Incomplete extended run */
				c = *diff++;
				diff_length--;
				runlen = (runlen & 0x7f) | (((unsigned) c) << 7);
			}

			dest += runlen + 1;
		} else {
			*dest++ ^= c;
		}
	}
}/* mem_undiff */


} // end of namespace Frotz
