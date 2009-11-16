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

#ifndef DARKSEED2_VARIABLES_H
#define DARKSEED2_VARIABLES_H

#include "engines/darkseed2/darkseed2.h"

#include "common/hashmap.h"
#include "common/hash-str.h"

namespace Common {
	class SeekableReadStream;
	class String;
}

namespace DarkSeed2 {

class Resource;

class Variables {
public:
	Variables();
	~Variables();

	void clear();

	void set(const Common::String &var, uint8 value);
	uint8 get(const Common::String &var) const;

	/*** Load initial variable values from an IDX file. */
	bool loadFromIDX(Common::SeekableReadStream &idx);
	/*** Load initial variable values from an IDX file. */
	bool loadFromIDX(const Resource &idx);

	/** Evaluates a condition string, like they are found in the game scripts. */
	bool evalCondition(const Common::String &condition);

private:
	typedef Common::HashMap<Common::String, uint8, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> VarMap;

	VarMap _variables;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_VARIABLES_H
