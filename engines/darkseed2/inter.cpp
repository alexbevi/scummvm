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

namespace DarkSeed2 {

#ifndef REDUCE_MEMORY_USAGE
	#define OPCODE(x) { &ScriptInterpreter::x, #x }
#else
	#define OPCODE(x) { &ScriptInterpreter::x, "" }
#endif

ScriptInterpreter::OpcodeEntry ScriptInterpreter::_scriptFunc[kScriptActionNone] = {
	OPCODE(oXYRoom),
	OPCODE(oCursor),
	OPCODE(oChange),
	OPCODE(oText),
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
	OPCODE(oSpeechVar),
	OPCODE(oWaitUntil),
	OPCODE(oEffect)
};

ScriptInterpreter::ScriptInterpreter(DarkSeed2Engine &vm) : _vm(&vm) {
}

ScriptInterpreter::~ScriptInterpreter() {
}

bool ScriptInterpreter::interpret(ScriptChunk &chunk, bool &ran) {
	ran = false;

	// Are all conditions met?
	if (!chunk.conditionsMet())
		return true;

	ran = true;

	const Common::List<ScriptChunk::Action> &actions = chunk.getActions();

	for (Common::List<ScriptChunk::Action>::const_iterator it = actions.begin(); it != actions.end(); ++it) {
		if (_vm->shouldQuit())
			break;

		if (!interpret(*it))
			return false;
	}

	return true;
}

bool ScriptInterpreter::interpret(Common::List<ScriptChunk *> &chunks) {
	for (Common::List<ScriptChunk *>::iterator it = chunks.begin(); it != chunks.end(); ++it) {
		if (_vm->shouldQuit())
			break;

		bool ran;

		if (!interpret(**it, ran))
			return false;

		// Only run the first chunk with met conditions
		if (ran)
			break;
	}

	return true;
}

bool ScriptInterpreter::interpret(const ScriptChunk::Action &action) {
	debugC(-1, kDebugOpcodes, "Script function %s [%s]", _scriptFunc[action.action].name,
			action.arguments.c_str());

	return (this->*_scriptFunc[action.action].func)(action);
}

bool ScriptInterpreter::oXYRoom(const ScriptChunk::Action &action) {
	warning("TODO: Unimplemented script function oXYRoom");
	return true;
}

bool ScriptInterpreter::oCursor(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oCursor");
	return false;
}

bool ScriptInterpreter::oChange(const ScriptChunk::Action &action) {
	_vm->_variables->evalChange(action.arguments);
	return true;
}

bool ScriptInterpreter::oText(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oText");
	return false;
}

bool ScriptInterpreter::oMidi(const ScriptChunk::Action &action) {
	if (!_vm->_music->playMID(*_vm->_resources, action.arguments))
		warning("Failed playing music \"%s\"", action.arguments.c_str());

	return true;
}

bool ScriptInterpreter::oAnim(const ScriptChunk::Action &action) {

	Common::Array<Common::String> lArgs = DATFile::argGet(action.arguments);
	if (lArgs.size() >= 5) {
		warning("TODO: Playing video \"%s\"", lArgs[4].c_str());

		_vm->_movie->play(lArgs[4], atoi(lArgs[0].c_str()), atoi(lArgs[1].c_str()));
	} else
		warning("TODO: oAnim \"%s\"", action.arguments.c_str());

	return true;
}

bool ScriptInterpreter::oStatus(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oStatus");
	return false;
}

bool ScriptInterpreter::oSequence(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oSequence");
	return false;
}

bool ScriptInterpreter::oSpriteIDX(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oSpriteIDX");
	return false;
}

bool ScriptInterpreter::oClipXY(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oClipXY");
	return false;
}

bool ScriptInterpreter::oPosX(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPosX");
	return false;
}

bool ScriptInterpreter::oPosY(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPosY");
	return false;
}

bool ScriptInterpreter::oScaleVal(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oScaleVal");
	return false;
}

bool ScriptInterpreter::oFrom(const ScriptChunk::Action &action) {
	warning("TODO: Unimplemented script function oFrom");
	return true;
}

bool ScriptInterpreter::oPaletteChange(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPaletteChange");
	return false;
}

bool ScriptInterpreter::oXYRoomEffect(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oXYRoomEffect");
	return false;
}

bool ScriptInterpreter::oChangeAt(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oChangeAt");
	return false;
}

bool ScriptInterpreter::oDialog(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oDialog");
	return false;
}

bool ScriptInterpreter::oPicture(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oPicture");
	return false;
}

bool ScriptInterpreter::oSpeech(const ScriptChunk::Action &action) {
	warning("Unimplemented script function oSpeech");
	return false;
}

bool ScriptInterpreter::oSpeechVar(const ScriptChunk::Action &action) {
	_speechVar = action.arguments;
	return true;
}

bool ScriptInterpreter::oWaitUntil(const ScriptChunk::Action &action) {
	if (!_vm->_variables->evalCondition(action.arguments)) {
		warning("TODO: Real WaitUntil condition");
		return false;
	}

	return true;
}

bool ScriptInterpreter::oEffect(const ScriptChunk::Action &action) {
	_vm->_sound->playWAV(*_vm->_resources, action.arguments,
			Audio::Mixer::kSFXSoundType, _speechVar);

	return true;
}

} // End of namespace DarkSeed2
