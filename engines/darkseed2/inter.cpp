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
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/events.h"

namespace DarkSeed2 {

ScriptInterpreter::Script::Script(ScriptChunk *chnk) {
	chunk      = chnk;
	action     = 0;
	started    = false;
	soundID    = -1;
	waitingFor = kWaitNone;

	lastWaitDebug = 0;
}

#ifndef REDUCE_MEMORY_USAGE
	#define OPCODE(x)          { &ScriptInterpreter::x, #x }
#else
	#define OPCODE(x)          { &ScriptInterpreter::x, "" }
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
	_updatesWithoutChanges = 0;
}

ScriptInterpreter::~ScriptInterpreter() {
}

bool ScriptInterpreter::hasScripts() const {
	for (int i = 0; i < kPriorityLevels; i++)
		if (!_scripts[i].empty())
			return true;

	return false;
}

void ScriptInterpreter::clear() {
	for (int i = 0; i < kPriorityLevels; i++)
		_scripts[i].clear();
	for (int i = 0; i < kPriorityLevels; i++)
		_scriptsQueues[i].clear();

	_updatesWithoutChanges = 0;
}

bool ScriptInterpreter::updateStatus() {
	Common::List<Script>::iterator script;

	_updatesWithoutChanges++;

	for (int i = 0; i < kPriorityLevels; i++) {
		if (_scripts[i].empty()) {
			Common::List< Common::List<ScriptChunk *> >::iterator it1 = _scriptsQueues[i].begin();
			while (it1 != _scriptsQueues[i].end()) {
				Common::List<ScriptChunk *>::iterator it2;
				for (it2 = it1->begin(); it2 != it1->end(); ++it2) {
					if ((*it2)->conditionsMet() && _vm->_events->cameFrom((*it2)->getFrom())) {
						(*it2)->rewind();
						_scripts[i].push_back(Script(*it2));
						break;
					}
				}

				it1 = _scriptsQueues[i].erase(it1);

				if (!_scripts[i].empty())
					break;
			}

			if (_scripts[i].empty())
				continue;
		}

		// Interpret one action from every script in the queue
		for (script = _scripts[i].begin(); script != _scripts[i].end(); ++script) {
			if (_vm->shouldQuit())
				break;

			if (!script->started) {
				// If the script wasn't started yet, check if the conditions are met
				if (!script->chunk->conditionsMet()) {
					// Conditions not met
					continue;
				}

				// Conditions met, start the script
				script->started = true;
				_updatesWithoutChanges = 0;
			}

			// Evaluating waiting orders
			if (script->waitingFor != kWaitNone) {
				bool waitEnd = false;

				if      (script->waitingFor == kWaitConversation)
					waitEnd = !_vm->_graphics->getConversationBox().isActive();
				else if (script->waitingFor == kWaitMovie)
					waitEnd = !_vm->_movie->isPlaying();

				if (waitEnd)
					script->waitingFor = kWaitNone;
				else
					continue;

				_updatesWithoutChanges = 0;
			}

			Result result = interpret(*script);

			if        (result == kResultInvalid) {
				script->chunk->seekEnd();
				_updatesWithoutChanges = 0;
			} else if (result == kResultOK) {
				script->chunk->next();
				script->lastWaitDebug = 0;
				_updatesWithoutChanges = 0;
			} else if (result == kResultStop) {
				script->chunk->seekEnd();
				_updatesWithoutChanges = 0;
			}
		}

		break;
	}

	for (int i = 0; i < kPriorityLevels; i++) {
		// Go through all scripts and erase those that ended
		script = _scripts[i].begin();
		while (script != _scripts[i].end()) {
			if (script->chunk->atEnd()) {
				//script->chunk->setUsed(false);
				script = _scripts[i].erase(script);
				_updatesWithoutChanges = 0;
			} else
				++script;
		}
	}

	return _updatesWithoutChanges < 10;
}

bool ScriptInterpreter::interpret(Common::List<ScriptChunk *> &chunks, int priority) {
	if ((priority < 0) || (priority >= kPriorityLevels))
		priority = kPriorityLevels - 1;

	_scriptsQueues[priority].push_back(chunks);

	_updatesWithoutChanges = 0;

	return true;
}

ScriptInterpreter::Result ScriptInterpreter::interpret(Script &script) {
	script.action = &script.chunk->getAction();

	if (script.action->action >= kScriptActionNone) {
		warning("ScriptInterpreter::interpret(): Invalid script action %d",
				script.action->action);

		return kResultInvalid;
	}

	bool doDebug = true;
	if (script.action->action == kScriptActionWaitUntil) {
		if (script.lastWaitDebug++ != 0)
			doDebug = false;

		if (script.lastWaitDebug >= 100)
			script.lastWaitDebug = 0;
	}

	if (doDebug)
		debugC(-1, kDebugOpcodes, "Script function %s [%s]", _scriptFunc[script.action->action].name,
				script.action->arguments.c_str());

	Result result = (this->*_scriptFunc[script.action->action].func)(script);

	script.action = 0;

	return result;
}

ScriptInterpreter::Result ScriptInterpreter::oXYRoom(Script &script) {
	warning("TODO: Unimplemented script function oXYRoom [%s]", script.action->arguments.c_str());

	Common::Array<Common::String> lArgs = DATFile::argGet(script.action->arguments);
	if (lArgs.size() >= 3) {
		uint32 room = atoi(lArgs[2].c_str());

		if (room != 0)
			_vm->_events->setNextRoom(room);
	}

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oCursor(Script &script) {
	warning("Unimplemented script function oCursor");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oChange(Script &script) {
	_vm->_variables->evalChange(script.action->arguments);
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oText(Script &script) {
	_vm->_talkMan->talk(*_vm->_resources, script.action->arguments);

	script.soundID = _vm->_talkMan->getSoundID();

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oMidi(Script &script) {
	warning("Unimplemented script function oMidi");

	if (!_vm->_music->playMID(*_vm->_resources, script.action->arguments))
		warning("Failed playing music \"%s\"", script.action->arguments.c_str());

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oAnim(Script &script) {

	Common::Array<Common::String> lArgs = DATFile::argGet(script.action->arguments);
	if (lArgs.size() >= 5) {
		if (!_vm->_movie->play(lArgs[4], atoi(lArgs[0].c_str()), atoi(lArgs[1].c_str())))
			warning("oAnim: Failed playing video \"%s\"", lArgs[4].c_str());
		else
			script.waitingFor = kWaitMovie;
	} else
		warning("TODO: oAnim \"%s\"", script.action->arguments.c_str());

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oStatus(Script &script) {
	warning("TODO: Unimplemented script function oStatus");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oSequence(Script &script) {
	warning("TODO: Unimplemented script function oSequence");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oSpriteIDX(Script &script) {
	warning("TODO: Unimplemented script function oSpriteIDX");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oClipXY(Script &script) {
	warning("Unimplemented script function oClipXY");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oPosX(Script &script) {
	warning("TODO: Unimplemented script function oPosX");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oPosY(Script &script) {
	warning("TODO: Unimplemented script function oPosY");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oScaleVal(Script &script) {
	warning("TODO: Unimplemented script function oScaleVal");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oFrom(Script &script) {
	Common::Array<Common::String> lArgs = DATFile::argGet(script.action->arguments);

	uint32 args[3];

	for (uint i = 0; i < 3; i++) {
		args[i] = 0;
		if (lArgs.size() > i)
			args[i] = atoi(lArgs[i].c_str());
	}

	warning("TODO: oFrom: Coming from %d? %dx%d", args[2], args[1], args[2]);

	if (!_vm->_events->cameFrom(args[2]))
		return kResultStop;

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oPaletteChange(Script &script) {
	warning("TODO: Unimplemented script function oPaletteChange");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oXYRoomEffect(Script &script) {
	warning("Unimplemented script function oXYRoomEffect");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oChangeAt(Script &script) {
	warning("TODO: Unimplemented script function oChangeAt");
	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oDialog(Script &script) {
	_vm->_graphics->getConversationBox().start(script.action->arguments);

	script.waitingFor = kWaitConversation;

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oPicture(Script &script) {
	warning("Unimplemented script function oPicture");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oSpeech(Script &script) {
	warning("Unimplemented script function oSpeech");
	return kResultInvalid;
}

ScriptInterpreter::Result ScriptInterpreter::oSpeechVar(Script &script) {
	_vm->_sound->setSoundVar(script.soundID, script.action->arguments);

	return kResultOK;
}

ScriptInterpreter::Result ScriptInterpreter::oWaitUntil(Script &script) {
	if (_vm->_variables->evalCondition(script.action->arguments))
		// Condition is true => Proceed
		return kResultOK;

	// Condition is false => Keep waiting here
	return kResultWait;
}

ScriptInterpreter::Result ScriptInterpreter::oEffect(Script &script) {
	_vm->_sound->playWAV(*_vm->_resources, script.action->arguments, script.soundID);

	return kResultOK;
}

} // End of namespace DarkSeed2
