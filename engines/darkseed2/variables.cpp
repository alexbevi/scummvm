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

#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/resources.h"

#include "common/str.h"
#include "common/stream.h"
#include "common/util.h"

namespace DarkSeed2 {

Variables::Variables() {
}

Variables::~Variables() {
}

void Variables::clear() {
	_variables.clear();
}

void Variables::set(const Common::String &var, uint8 value) {
	_variables.setVal(var, value);
}

uint8 Variables::get(const Common::String &var) const {
	return _variables.getVal(var);
}

bool Variables::loadFromIDX(Common::SeekableReadStream &idx) {
	clear();

	while (!idx.err() && !idx.eos()) {
		Common::String line = idx.readLine();
		if (line.empty())
			continue;

		Common::StringTokenizer tokenizer(line, "=");

		Common::String varName = tokenizer.nextToken();
		Common::String value   = tokenizer.nextToken();

		if (value.empty())
			continue;

		_variables.setVal(varName, (uint8) atoi(value.c_str()));
	}

	return true;
}

bool Variables::loadFromIDX(const Resource &idx) {
	return loadFromIDX(idx.getStream());
}

bool Variables::evalCondition(const Common::String &condition) {
	bool result = true;

	Common::StringTokenizer tokenizer(condition, " ");

	while (!tokenizer.empty()) {
		Common::String condPart = tokenizer.nextToken();

		if (condPart[0] == '*')
			warning("Meaning of '*' not yet understood in condition part \"%s\"", condPart.c_str());
		else if (condPart[0] == '+')
			warning("Meaning of '+' not yet understood in condition part \"%s\"", condPart.c_str());
		else if (condPart[0] == '@')
			warning("Meaning of '@' not yet understood in condition part \"%s\"", condPart.c_str());
		else if (condPart[0] == '!')
			result = result && (_variables.getVal(condPart.c_str() + 1, 0) == 0);
		else if (condPart[0] == '=') {
			Common::StringTokenizer tokenizerPart(condPart.c_str() + 1, ",");

			Common::String varName = tokenizerPart.nextToken();
			Common::String value   = tokenizerPart.nextToken();

			result = result && (_variables.getVal(varName, 0) == atoi(value.c_str()));
		} else
			result = result && (_variables.getVal(condPart, 0) != 0);
	}

	return result;
}

void Variables::evalChange(const Common::String &change) {
	Common::StringTokenizer tokenizer(change, " ");

	while (!tokenizer.empty()) {
		Common::String changePart = tokenizer.nextToken();

		if (changePart[0] == '*')
			warning("Meaning of '*' not yet understood in change part \"%s\"", changePart.c_str());
		else if (changePart[0] == '+')
			warning("Meaning of '+' not yet understood in change part \"%s\"", changePart.c_str());
		else if (changePart[0] == '@')
			warning("Meaning of '@' not yet understood in change part \"%s\"", changePart.c_str());
		else if (changePart[0] == '!')
			_variables.setVal(changePart.c_str() + 1, 0);
		else if (changePart[0] == '=') {
			Common::StringTokenizer tokenizerPart(changePart.c_str() + 1, ",");

			Common::String varName = tokenizerPart.nextToken();
			Common::String value   = tokenizerPart.nextToken();

			_variables.setVal(varName, atoi(value.c_str()));
		} else
			_variables.setVal(changePart, 1);
	}
}

} // End of namespace DarkSeed2
