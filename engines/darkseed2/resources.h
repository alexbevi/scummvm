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

#ifndef DARKSEED2_RESOURCES_H
#define DARKSEED2_RESOURCES_H

#include "engines/darkseed2/darkseed2.h"

#include "common/str.h"
#include "common/array.h"
#include "common/hashmap.h"

namespace DarkSeed2 {

class Resources {
public:
	Resources();
	~Resources();

	bool index(const char *fileName);

	void clear();

private:
	struct Glue {
		Common::String fileName;
	};

	struct Resource {
		Glue *glue;
		uint32 offset;
		uint32 size;
		byte unknown[10];

		Resource();
	};

	uint16 _glueCount;
	uint16 _resCount;

	Common::Array<Glue> _glues;
	Common::HashMap<Common::String, Resource> _resources;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_RESOURCES_H
