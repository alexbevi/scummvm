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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef AGS_STD_MAP_H
#define AGS_STD_MAP_H

#include "common/hashmap.h"
#include "ags/std/utility.h"

namespace AGS3 {
namespace std {

template<class Key, class Val, class HashFunc = Common::Hash<Key>,
         class EqualFunc = Common::EqualTo<Key> >
class map : public Common::HashMap<Key, Val, HashFunc, EqualFunc> {
public:
	using iterator = typename Common::HashMap<Key, Val, HashFunc, EqualFunc>::iterator;

	void insert(pair<Key, Val> elem) {
		this->operator[](elem.first) = elem.second;
	}

	// FUNCTION TEMPLATE lower_bound
	iterator lower_bound(Key &val) {
		iterator it;
		for (it = this->begin(); it != this->end(); ++it) {
			if (it->_key >= val)
				break;
		}

		return it;
	}
};

template<class Key, class Val, class HashFunc = Common::Hash<Key>,
         class EqualFunc = Common::EqualTo<Key> >
class unordered_map : public Common::HashMap<Key, Val, HashFunc, EqualFunc> {
public:
	void insert(pair<Key, Val> elem) {
		this->operator[](elem.first) = elem.second;
	}
};

} // namespace std
} // namespace AGS3

#endif