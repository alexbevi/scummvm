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
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

static const char *scriptAction[] = {
	"XYRoom"      , "Cursor"   , "Change"       , "Text"         ,
	"Midi"        , "Anim"     , "From"         , "PaletteChange",
	"ChangeAt"    , "Dialog"   , "Picture"      , "Speech"       ,
	"SpeechVar"   , "WaitUntil", "Effect"
};


template<>
void SaveLoad::sync<ScriptRegister::Script>(Common::Serializer &serializer, ScriptRegister::Script &var) {
	if (serializer.isSaving())
		if (var.chunk)
			var.line = var.chunk->getCurLine();

	serializer.syncAsUint32LE(var.line);

	if (serializer.isLoading())
		var.chunk = 0;
}


ScriptRegister::Script::Script() {
	chunk = 0;
	line  = 0;
}

ScriptRegister::Script::Script(ScriptChunk &c) {
	chunk = &c;
	line  = 0;
}

ScriptRegister::Script::Script(uint32 l) {
	chunk = 0;
	line  = l;
}

uint32 ScriptRegister::Script::getLine() const {
	if (!chunk)
		return line;

	return chunk->getCurLine();
}


ScriptRegister::ScriptRegister() {
}

ScriptRegister::~ScriptRegister() {
}

void ScriptRegister::clear() {
	_scriptMap.clear();
}

void ScriptRegister::addScript(ScriptChunk &chunk) {
	_scriptMap.setVal(chunk.getSignature(), Script(chunk));
}

void ScriptRegister::removeScript(ScriptChunk &chunk) {
	_scriptMap.setVal(chunk.getSignature(), Script(0xFFFFFFFF));
}

uint32 ScriptRegister::getLine(const Common::String &signature) const {
	ScriptMap::const_iterator script = _scriptMap.find(signature);

	if (script == _scriptMap.end())
		return 0;

	return script->_value.getLine();
}

uint32 ScriptRegister::getLine(const ScriptChunk &chunk) const {
	return getLine(chunk.getSignature());
}

ScriptChunk *ScriptRegister::getScript(const Common::String &signature) const {
	ScriptMap::const_iterator script = _scriptMap.find(signature);

	if (script == _scriptMap.end())
		return 0;

	return script->_value.chunk;
}

bool ScriptRegister::saveLoad(Common::Serializer &serializer, Resources &resources) {
	SaveLoad::sync(serializer, _scriptMap);
	return true;
}

bool ScriptRegister::loading(Resources &resources) {
	return true;
}


const ScriptChunk::Action ScriptChunk::invalidAction(kScriptActionNone, "");

ScriptChunk::Action::Action(ScriptAction act, const Common::String &args) {
	action    = act;
	arguments = args;
}


ScriptChunk::ScriptChunk(const Variables &variables, ScriptRegister &scriptRegister) {
	_variables = &variables;

	_scriptRegister = &scriptRegister;

	_ready = false;
	_from  = 0;
}

ScriptChunk::~ScriptChunk() {
	if (_ready)
		_scriptRegister->removeScript(*this);
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
	++_curPosNumber;
}

void ScriptChunk::rewind() {
	if (!_ready)
		return;

	_curPos       = _actions.begin();
	_curPosNumber = 0;
}

void ScriptChunk::seekEnd() {
	if (!_ready)
		return;

	_curPos       = _actions.end();
	_curPosNumber = _actions.size();
}

void ScriptChunk::seekTo(uint32 n) {
	if (n < _curPosNumber)
		rewind();

	while (!atEnd() && (_curPosNumber != n))
		next();
}

const Common::String &ScriptChunk::getSignature() const {
	return _signature;
}

uint32 ScriptChunk::getCurLine() const {
	return _curPosNumber;
}

void ScriptChunk::clear() {
	if (_ready)
		_scriptRegister->removeScript(*this);

	_ready = false;
	_from  = 0;

	_signature.clear();
	_conditions.clear();
	_actions.clear();
}

bool ScriptChunk::parse(DATFile &dat) {
	clear();

	_signature = dat.getSignature();

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
				if (cmd->equalsIgnoreCase("Chnage")) {
					// Workaround for OBJ_0307.DAT
					action = kScriptActionChange;
				} else {
					// No know action, die
					warning("ScriptChunk::parse(): Unknown script action \"%s\" (\"%s\")", cmd->c_str(), arg->c_str());
					return false;
				}
			}

			if (action == kScriptActionFrom)
				parseFrom(*arg);

			// Put the action into our list
			_actions.push_back(Action(action, *arg));
		}
	}

	uint32 lineNumber = _scriptRegister->getLine(*this);
	if (lineNumber == 0xFFFFFFFF) {
		clear();
		return true;
	}

	_ready = true;

	rewind();
	seekTo(lineNumber);

	_scriptRegister->addScript(*this);

	return true;
}

bool ScriptChunk::conditionsMet() const {
	return _variables->evalCondition(_conditions);
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
