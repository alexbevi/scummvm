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

#include "mutationofjb/commands/ifcommand.h"
#include "mutationofjb/game.h"
#include "mutationofjb/script.h"
#include "common/str.h"
#include "common/translation.h"

namespace MutationOfJB {

bool IfCommandParser::parse(const Common::String &line, ScriptParseContext &parseContext, Command *&command) {
	// IFtss oo val!
	// <t>   1B Tag.
	// <ss>  2B Scene.
	// <oo>  2B Object ID.
	// <val> VL Value.
	// !     1B Negation (optional).

	if (line.size() < 10) {
		return false;
	}
	
	if (!line.hasPrefix("IF")) {
		return false;
	}

	const char *const cstr = line.c_str();
	const char tag = cstr[2];
	const uint8 sceneId = atoi(cstr + 3);
	const uint8 objectId = atoi(cstr + 6);
	const uint8 value = atoi(cstr + 9);
	const bool negative = (line.lastChar() == '!');

	IfCommand *ifCommand = new IfCommand(sceneId, objectId, value, negative);

	command = ifCommand;
	parseContext.addConditionalCommand(ifCommand, tag);
	return true;
}

void IfCommandParser::transition(ScriptParseContext &, Command *oldCommand, Command *newCommand, CommandParser *) {
	if (!oldCommand || !newCommand) {
		warning(_("Unexpected empty command in transition"));
		return;
	}

	static_cast<IfCommand *>(oldCommand)->setTrueCommand(newCommand);
}

IfCommand::IfCommand(uint8 sceneId, uint8 objectId, uint16 value, bool negative) :
	_sceneId(sceneId),
	_objectId(objectId),
	_value(value),
	_negative(negative)
{}

Command::ExecuteResult IfCommand::execute(GameData &gameData) {
	Scene* const scene = gameData.getScene(_sceneId);
	if (!scene) {
		return Finished;
	}

	Object* const object = scene->getObject(_objectId);
	if (!object) {
		return Finished;
	}

	_cachedResult = (object->_WX == _value);
	if (_negative) {
		_cachedResult = !_cachedResult;
	}

	return Finished;
}

Common::String IfCommand::debugString() const {
	return Common::String::format("IF scene%d.object%d.WX %s %d", _sceneId, _objectId, _negative ? "!=" : "==", _value);
}

}

