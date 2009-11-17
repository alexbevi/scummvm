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

#include "common/stream.h"
#include "common/util.h"

#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/resources.h"

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

bool Variables::evalCondition(const Common::String &condition) const {
	bool result = true;

	Common::StringTokenizer tokenizer(condition, " ");

	while (!tokenizer.empty()) {
		Common::String condPart = tokenizer.nextToken();

		result = result && evalConditionPart(condPart);
	}

	return result;
}

bool Variables::evalCondition(const Common::List<Common::String> &condition) const {
	bool result = true;

	for (Common::List<Common::String>::const_iterator it = condition.begin(); it != condition.end(); ++it)
		result = result && evalConditionPart(*it);

	return result;
}

bool Variables::evalConditionPart(const Common::String &conditionPart) const {
	if (conditionPart[0] == '*')
		warning("Variables::evalConditionPart(): "
				"Meaning of '*' not yet understood in condition part \"%s\"",
				conditionPart.c_str());
	else if (conditionPart[0] == '+')
		warning("Variables::evalConditionPart(): "
				"Meaning of '+' not yet understood in condition part \"%s\"",
				conditionPart.c_str());
	else if (conditionPart[0] == '@')
		warning("Variables::evalConditionPart(): "
				"Meaning of '@' not yet understood in condition part \"%s\"",
				conditionPart.c_str());
	else if (conditionPart[0] == '!')
		return _variables.getVal(conditionPart.c_str() + 1, 0) == 0;
	else if (conditionPart[0] == '=') {
		Common::StringTokenizer tokenizerPart(conditionPart.c_str() + 1, ",");

		Common::String varName = tokenizerPart.nextToken();
		Common::String value   = tokenizerPart.nextToken();

		return _variables.getVal(varName, 0) == atoi(value.c_str());
	} else
		return _variables.getVal(conditionPart, 0) != 0;

	return false;
}

void Variables::evalChange(const Common::String &change) {
	Common::StringTokenizer tokenizer(change, " ");

	while (!tokenizer.empty()) {
		Common::String changePart = tokenizer.nextToken();

		evalChangePart(changePart);
	}
}

void Variables::evalChange(const Common::List<Common::String> &change) {
	for (Common::List<Common::String>::const_iterator it = change.begin(); it != change.end(); ++it)
		evalChangePart(*it);
}

void Variables::evalChangePart(const Common::String &changePart) {
	if (changePart[0] == '*')
		warning("Variables::evalChangePart(): "
				"Meaning of '*' not yet understood in change part \"%s\"",
				changePart.c_str());
	else if (changePart[0] == '+')
		warning("Variables::evalChangePart(): "
				"Meaning of '+' not yet understood in change part \"%s\"",
				changePart.c_str());
	else if (changePart[0] == '@')
		warning("Variables::evalChangePart(): "
				"Meaning of '@' not yet understood in change part \"%s\"",
				changePart.c_str());
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

} // End of namespace DarkSeed2
