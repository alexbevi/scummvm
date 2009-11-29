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

#include "engines/darkseed2/inter.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/music.h"
#include "engines/darkseed2/movie.h"
#include "engines/darkseed2/talk.h"

namespace DarkSeed2 {

ScriptInterpreter::Script::Script(ScriptChunk *chnk, bool perm) {
	chunk     = chnk;
	permanent = perm;
	started   = false;
	soundID   = -1;
}

#ifndef REDUCE_MEMORY_USAGE
	#define OPCODE(x)          { &ScriptInterpreter::x, 0                    , #x }
	#define OPCODEUPDATE(x, y) { &ScriptInterpreter::x, &ScriptInterpreter::y, #x }
#else
	#define OPCODE(x)          { &ScriptInterpreter::x, 0                    , "" }
	#define OPCODEUPDATE(x, y) { &ScriptInterpreter::x, &ScriptInterpreter::y, "" }
#endif

ScriptInterpreter::OpcodeEntry ScriptInterpreter::_scriptFunc[kScriptActionNone] = {
	OPCODE(oXYRoom),
	OPCODE(oCursor),
	OPCODE(oChange),
	OPCODEUPDATE(oText, uText),
	OPCODE(oMidi),
	OPCODE(oAnim),
	OPCODE(oStatus),
	OPCODE(oSequence),
	OPCODE(oSpriteIDX),
	OPCODE(oClipXY),
	OPCODE(oPosX),
	OPCODE(oPosY),
	OPCODE(oScaleVal),
	OPCODE(oFrom),
	OPCODE(oPaletteChange),
	OPCODE(oXYRoomEffect),
	OPCODE(oChangeAt),
	OPCODE(oDialog),
	OPCODE(oPicture),
	OPCODE(oSpeech),
	OPCODEUPDATE(oSpeechVar, uSpeechVar),
	OPCODE(oWaitUntil),
	OPCODEUPDATE(oEffect, uEffect)
};

ScriptInterpreter::ScriptInterpreter(DarkSeed2Engine &vm) : _vm(&vm) {
	_updatesWithoutChanges = 0;
	_soundID = -1;
}

ScriptInterpreter::~ScriptInterpreter() {
}

bool ScriptInterpreter::hasScripts() const {
	return !_scripts.empty();
}

void ScriptInterpreter::clear() {
	_scripts.clear();

	_soundID = -1;
	_speechVar.clear();

	_updatesWithoutChanges = 0;
}

bool ScriptInterpreter::updateStatus() {
	Common::List<Script>::iterator script;

	_updatesWithoutChanges++;

	// Interpret one action from every script in the queue
	for (script = _scripts.begin(); script != _scripts.end(); ++script) {
		if (_vm->shouldQuit())
			break;

		if (!script->started) {
			// If the script wasn't started yet, check if the conditions are met
			if (!script->chunk->conditionsMet()) {
				// Conditions not met

				if (!script->permanent)
					// Not a permanent script, end it
					script->chunk->seekEnd();

				continue;
			}

			// Conditions met, start the script
			script->started = true;
			_updatesWithoutChanges = 0;
		}

		const ScriptChunk::Action &action = script->chunk->getAction();
		Result result = interpret(action);

		if (result == kResultInvalid) {
			script->chunk->seekEnd();
			_updatesWithoutChanges = 0;
			continue;
		}

		if (result == kResultOK) {
			script->chunk->next();
			_updatesWithoutChanges = 0;
		}

		updateScriptState(*script, action);
	}

	// Go through all scripts and erase those that ended
	script = _scripts.begin();
	while (script != _scripts.end()) {
		if (script->chunk->atEnd()) {
			script->chunk->setUsed(false);
			script = _scripts.erase(script);
			_updatesWithoutChanges = 0;
		} else
			++script;
	}

	return _updatesWithoutChanges < 10;
}

bool ScriptInterpreter::interpret(Common::List<ScriptChunk *> &chunks, bool permanent) {
	bool has = false;

	for (Common::List<ScriptChunk *>::iterator it = chunks.begin(); it != chunks.end(); ++it) {
		if (!(*it)->isUsed()) {
			(*it)->setUsed(true);
			(*it)->rewind();
			_scripts.push_back(Script(*it, permanent));
			has = true;
		}
	}

	if (has)
		_updatesWithoutChanges = 0;

	return has;
}

ScriptInterpreter::Result ScriptInterpreter::interpret(const ScriptChunk::Action &action) {
	if (action.action >= kScriptActionNone) {
		warning("ScriptInterpreter::interpret(): Invalid script action %d",
				action.action);

		return kResultInvalid;
	}

	debugC(-1, kDebugOpcodes, "Script function %s [%s]", _scriptFunc[action.action].name,
			action.arguments.c_str());

	return (this->*_scriptFunc[action.action].func)(action);
}

void ScriptInterpreter::updateScriptState(Script &script, const ScriptChunk::Action &action) {
	updateFunc_t updateFunc = _scriptFunc[action.action].updateFunc;

	if (updateFunc)
		(this->*updateFunc)(script);
}

ScriptInterpreter::Result ScriptInterpreter::oXYRoom(const ScriptChunk::Action &action) {
	warning("TODO: Unimplemented script function oXYRoom");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oCursor(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oCursor");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oChange(const ScriptChunk::Action &action) {
	_vm->_variables->evalChange(action.arguments);
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oText(const ScriptChunk::Action &action) {
	_vm->_talkMan->talk(*_vm->_resources, action.arguments);

	_soundID = _vm->_talkMan->getSoundID();

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oMidi(const ScriptChunk::Action &action) {
	if (!_vm->_music->playMID(*_vm->_resources, action.arguments))
		warning("Failed playing music \"%s\"", action.arguments.c_str());

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oAnim(const ScriptChunk::Action &action) {

	Common::Array<Common::String> lArgs = DATFile::argGet(action.arguments);
	if (lArgs.size() >= 5) {
		warning("TODO: Playing video \"%s\"", lArgs[4].c_str());

		_vm->_movie->play(lArgs[4], atoi(lArgs[0].c_str()), atoi(lArgs[1].c_str()));
	} else
		warning("TODO: oAnim \"%s\"", action.arguments.c_str());

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oStatus(const ScriptChunk::Action &action) {
	warning("TODO: Unimplemented script function oStatus");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oSequence(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oSequence");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oSpriteIDX(const ScriptChunk::Action &action) {
	warning("TODO: Unimplemented script function oSpriteIDX");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oClipXY(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oClipXY");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oPosX(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPosX");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oPosY(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPosY");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oScaleVal(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oScaleVal");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oFrom(const ScriptChunk::Action &action) {
	warning("TODO: Unimplemented script function oFrom");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oPaletteChange(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPaletteChange");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oXYRoomEffect(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oXYRoomEffect");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oChangeAt(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oChangeAt");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oDialog(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oDialog");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oPicture(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPicture");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oSpeech(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oSpeech");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oSpeechVar(const ScriptChunk::Action &action) {
	_speechVar = action.arguments;
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oWaitUntil(const ScriptChunk::Action &action) {
	if (_vm->_variables->evalCondition(action.arguments))
		// Condition is true => Proceed
		return kResultOK;

	// Condition is false => Keep waiting here
	return kResultWait;
}

ScriptInterpreter::Result ScriptInterpreter::oEffect(const ScriptChunk::Action &action) {
	_vm->_sound->playWAV(*_vm->_resources, action.arguments, _soundID);

	return kResultOK;
}

void ScriptInterpreter::uText(Script &script) {
	script.soundID = _soundID;
}

void ScriptInterpreter::uSpeechVar(Script &script) {
	_vm->_sound->setSoundVar(script.soundID, _speechVar);
}

void ScriptInterpreter::uEffect(Script &script) {
	script.soundID = _soundID;
}

} // End of namespace DarkSeed2
