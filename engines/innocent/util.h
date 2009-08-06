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

#ifndef INNOCENT_UTIL_H
#define INNOCENT_UTIL_H

namespace Innocent {
//
#define foreach(T, L) for (Common::List<T>::iterator it = L.begin(); it != L.end(); ++it)
#define foreach_const(T, L) for (Common::List<T>::const_iterator it = L.begin(); it != L.end(); ++it)
#define unless(x) if (!(x))
#define forever for(;;)

template<typename F1, typename F2>
class Composite {
public:
	Composite(const F1 &a, const F2 &b) : _a(a), _b(b) {}
	typename F1::result_type operator()(const typename F2::argument_type &v) { return _a(_b(v)); }
private:
	F1 _a;
	F2 _b;
};

template<typename F1, typename F2>
Composite<F1, F2> compose(const F1 &a, const F2 &b) {
	return Composite<F1, F2>(a, b);
}


}

#endif
