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

#ifndef RING_ROTATION_H
#define RING_ROTATION_H

#include "ring/shared.h"

namespace Ring {

class Rotation : public BaseObject {
public:
	Rotation(Id id, Common::String name, uint32 a3, LoadFrom loadFrom, uint32 nodeCount, uint32 a6);
	~Rotation();

	void updateData(uint32 index, uint32 val);

	uint32 getLayerCount();

private:

};

} // End of namespace Ring

#endif // RING_ROTATION_H
