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

#include "engines/darkseed2/script.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/datfile.h"

namespace DarkSeed2 {

static const char *scriptAction[] = {
	"XYRoom"      , "Cursor"   , "Change"       , "Text"         ,
	"Midi"        , "Anim"     , "From"         , "PaletteChange",
	"ChangeAt"    , "Dialog"   , "Picture"      , "Speech"       ,
	"SpeechVar"   , "WaitUntil", "Effect"
};


const ScriptChunk::Action ScriptChunk::invalidAction(kScriptActionNone, "");

ScriptChunk::Action::Action(ScriptAction act, const Common::String &args) {
	action    = act;
	arguments = args;
}


ScriptChunk::ScriptChunk(const Variables &variables) : _variables(&variables) {
	_ready = false;
	_from  = 0;
}

ScriptChunk::~ScriptChunk() {
}

bool ScriptChunk::atEnd() const {
	if (!_ready)
		return true;

	return _curPos == _actions.end();
}

uint32 ScriptChunk::getFrom() const {
	return _from;
}

void ScriptChunk::next() {
	if (!_ready)
		return;

	++_curPos;
}

void ScriptChunk::rewind() {
	if (!_ready)
		return;

	_curPos = _actions.begin();
}

void ScriptChunk::seekEnd() {
	if (!_ready)
		return;

	_curPos = _actions.end();
}

void ScriptChunk::clear() {
	_ready = false;
	_from  = 0;

	_conditions.clear();
	_actions.clear();
}

bool ScriptChunk::parse(DATFile &dat) {
	clear();

	const Common::String *cmd, *arg;
	while (dat.nextLine(cmd, arg)) {
		debugC(2, kDebugScript, "Parsing script action \"%s\" [%s]", cmd->c_str(), arg->c_str());

		if (cmd->equalsIgnoreCase("Cond")) {
			// Found a condition

			if (!_conditions.empty()) {
				// Already got one, must belong to the next chunk
				dat.previous();
				break;
			}

			// A primary condition
			_conditions.push_back(*arg);
		} else if (cmd->equalsIgnoreCase("Cond2")) {
			// A secondary condition
			_conditions.push_back(*arg);
		} else if (cmd->matchString("*End", true)) {
			// Reached the end of the current verb section
			dat.previous();
			break;
		} else {
			// An action

			if (_conditions.empty()) {
				// We do need a condition first
				warning("ScriptChunk::parse(Script sync error, first line must be a condition");
				return false;
			}

			// Is this a known action?
			ScriptAction action = parseScriptAction(*cmd);
			if (action == kScriptActionNone) {
				// No, die
				warning("ScriptChunk::parse(): Unknown script action \"%s\" (\"%s\")", cmd->c_str(), arg->c_str());
				return false;
			}

			if (action == kScriptActionFrom)
				parseFrom(*arg);

			// Put the action into our list
			_actions.push_back(Action(action, *arg));
		}
	}

	_ready = true;

	rewind();

	return true;
}

bool ScriptChunk::conditionsMet() const {
	for (Common::List<Common::String>::const_iterator it = _conditions.begin(); it != _conditions.end(); ++it)
		if (_variables->evalCondition(*it))
			return true;

	return false;
}

const Common::List<Common::String> &ScriptChunk::getConditions() const {
	return _conditions;
}

const Common::List<ScriptChunk::Action> &ScriptChunk::getActions() const {
	return _actions;
}

const ScriptChunk::Action &ScriptChunk::getAction() const {
	if (atEnd())
		return invalidAction;

	return *_curPos;
}

ScriptAction ScriptChunk::parseScriptAction(const Common::String &action) {
	for (int i = 0; i < kScriptActionNone; i++)
		if (action.equalsIgnoreCase(scriptAction[i]))
			return (ScriptAction) i;

	return kScriptActionNone;
}

void ScriptChunk::parseFrom(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() >= 3)
		_from = atoi(lArgs[2].c_str());
}

} // End of namespace DarkSeed2
