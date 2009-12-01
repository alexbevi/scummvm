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

#ifndef DARKSEED2_INTER_H
#define DARKSEED2_INTER_H

#include "common/str.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/script.h"

namespace DarkSeed2 {

class ScriptInterpreter {
public:
	ScriptInterpreter(DarkSeed2Engine &vm);
	~ScriptInterpreter();

	/** Are there any scripts currently in the queue? */
	bool hasScripts() const;

	/** Remove all scripts from the queue. */
	void clear();

	/** Push the chunks with met conditions into the queue. */
	bool interpret(Common::List<ScriptChunk *> &chunks, bool permanent = false);

	/** Update status, interpret next lines, .... */
	bool updateStatus();

private:
	/** The result of a script action. */
	enum Result {
		kResultOK,     ///< Everything was okay, proceed to the next line.
		kResultWait,   ///< Wait on this line.
		kResultInvalid ///< Invalid script line.
	};

	/** Waiting for something to happen. */
	enum Wait {
		kWaitNone,        ///< Waiting for nothing
		kWaitConversation ///< Waiting for the conversation to end
	};

	struct Script {
		ScriptChunk *chunk;
		const ScriptChunk::Action *action;
		bool started;
		bool permanent;
		int soundID;
		Wait waitingFor;
		int lastWaitDebug;

		Script(ScriptChunk *chnk = 0, bool perm = false);
	};

	typedef Result (ScriptInterpreter::*func_t)(Script &script);
	struct OpcodeEntry {
		func_t func;
		const char *name;
	};

	DarkSeed2Engine *_vm;

	/** All opcodes. */
	static OpcodeEntry _scriptFunc[kScriptActionNone];

	int _updatesWithoutChanges;

	/** The ID of the last started sound. */
	int _soundID;
	/** The current variable that gets changed when the talking/sfx ends. */
	Common::String _speechVar;

	/** The currently active scripts. */
	Common::List<Script> _scripts;

	// Interpreting helper
	Result interpret(Script &script);

	// Opcodes
	Result oXYRoom(Script &script);
	Result oCursor(Script &script);
	Result oChange(Script &script);
	Result oText(Script &script);
	Result oMidi(Script &script);
	Result oAnim(Script &script);
	Result oStatus(Script &script);
	Result oSequence(Script &script);
	Result oSpriteIDX(Script &script);
	Result oClipXY(Script &script);
	Result oPosX(Script &script);
	Result oPosY(Script &script);
	Result oScaleVal(Script &script);
	Result oFrom(Script &script);
	Result oPaletteChange(Script &script);
	Result oXYRoomEffect(Script &script);
	Result oChangeAt(Script &script);
	Result oDialog(Script &script);
	Result oPicture(Script &script);
	Result oSpeech(Script &script);
	Result oSpeechVar(Script &script);
	Result oWaitUntil(Script &script);
	Result oEffect(Script &script);
	Result oLoadCond(Script &script);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_INTER_H
