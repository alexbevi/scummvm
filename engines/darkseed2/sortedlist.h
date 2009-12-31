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

#ifndef DARKSEED2_SORTEDLIST_H
#define DARKSEED2_SORTEDLIST_H

#include "common/list.h"

/** A list that always keeps its elements sorted. */
template<typename t_T>
class SortedList {
private:
	Common::List<t_T> _list;

public:
	typedef typename Common::List<t_T>::iterator iterator;
	typedef typename Common::List<t_T>::const_iterator const_iterator;

	uint size() const {
		return _list.size();
	}

	void clear() {
		_list.clear();
	}

	bool empty() const {
		return _list.empty();
	}

	iterator begin() {
		return _list.begin();
	}

	iterator end() {
		return _list.end();
	}

	const_iterator begin() const {
		return _list.begin();
	}

	const_iterator end() const {
		return _list.end();
	}

	iterator erase(iterator pos) {
		return _list.erase(pos);
	}

	iterator insert(const t_T &element) {
		if (_list.empty()) {
			_list.push_back(element);
			return _list.begin();
		}

		iterator pos = _list.begin();
		for (pos = _list.begin(); pos != _list.end(); ++pos) {
			if (!(*pos < element))
				break;
		}

		_list.insert(pos, element);

		pos--;

		return pos;
	}

};

#endif // DARKSEED2_SORTEDLIST_H
