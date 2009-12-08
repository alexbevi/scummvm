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

#include "common/str.h"
#include "common/list.h"
#include "common/hashmap.h"
#include "common/hash-str.h"

#include "engines/darkseed2/darkseed2.h"

namespace Common {
	class SeekableReadStream;
	class String;
}

namespace DarkSeed2 {

class Resources;

class Resource;

class Variables {
public:
	Variables();
	~Variables();

	/** Remove all variables. */
	void clear();
	/** Remove all local variables. */
	void clearLocal();

	/** Declare a variable as local. */
	void addLocal(const Common::String &var);

	/** Set a variable's value. */
	void set(const Common::String &var, uint8 value);
	/** Get a variable's value. */
	uint8 get(const Common::String &var) const;

	/** When was a variable change last? */
	uint32 getLastChanged() const;

	/** Load initial variable values from an IDX file. */
	bool loadFromIDX(Common::SeekableReadStream &idx);
	/** Load initial variable values from an IDX file. */
	bool loadFromIDX(const Resource &idx);
	/** Load initial variable values from an IDX file. */
	bool loadFromIDX(Resources &resources, const Common::String &idx);

	/** Evaluates a condition string, like they are found in the game scripts. */
	bool evalCondition(const Common::String &condition) const;
	/** Evaluates several condition strings, like they are found in the game scripts. */
	bool evalCondition(const Common::List<Common::String> &condition) const;

	/** Evaluate a change string, like they are found in the game scripts. */
	void evalChange(const Common::String &change);
	/** Evaluate a change string, like they are found in the game scripts. */
	void evalChange(const Common::List<Common::String> &change);

private:
	typedef Common::HashMap<Common::String, uint8, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> VarMap;

	VarMap _variables;      ///< The variables.
	VarMap _localVariables; ///< The local variables.

	uint32 _lastChanged; ///< Timestamp of when a variable was changed last.

	// Evaluation helpers
	bool evalConditionPart(const Common::String &conditionPart) const;
	void evalChangePart(const Common::String &changePart);
	uint8 get(const Common::String &var, uint8 def) const;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_VARIABLES_H
