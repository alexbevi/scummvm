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

#ifndef RING_SHARED_H
#define RING_SHARED_H

#include "common/array.h"
#include "common/serializer.h"
#include "common/system.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Enumerations
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Types
//////////////////////////////////////////////////////////////////////////
typedef uint32 ID;

class BaseObject {
public:
	BaseObject(ID id) : _id(id) {}
	virtual ~BaseObject();

	ID getID() { return _id; }

protected:
	ID _id;
};

template<class T>
class AssociativeArray : public Common::Array<T>, public Common::Serializable {
public:
	void saveLoadWithSerializer(Common::Serializer &s) {
		uint32 size = this->size();
		s.syncAsUint32LE(size);

		for (uint32 i = 0; i < size; i++)
			this[i].saveLoadWithSerializer(s);
	}

	T *get(ID id) {
		for (Common::Array<T>::iterator i = this->begin(); i != this->end(); i++)
			if (((BaseObject)*i).getID() == id)
				return i;

		error("[AssociativeArray::get] No object has this ID (%d)", id);
	}

	bool has(ID id) {
		for (Common::Array<T>::iterator i = this->begin(); i != this->end(); i++)
			if (((BaseObject)*i).getID() == id)
				return true;

		return false;
	}
};

} // End of namespace Ring

#endif // RING_SHARED_H
