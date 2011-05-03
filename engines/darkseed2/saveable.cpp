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

#include "common/system.h"
#include "common/serializer.h"

#include "darkseed2/saveable.h"

namespace DarkSeed2 {

Saveable::~Saveable() {
}

bool Saveable::doSaveLoad(Common::Serializer &serializer, Resources &resources) {
	if (!saveLoad(serializer, resources))
		return false;

	if (serializer.isLoading())
		if (!loading(resources))
			return false;

	return true;
}

bool Saveable::saveLoad(Common::Serializer &serializer, Resources &resources) {
	return true;
}

bool Saveable::loading(Resources &resources) {
	return true;
}

} // End of namespace DarkSeed2
