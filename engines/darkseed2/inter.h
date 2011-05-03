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

#include "darkseed2/darkseed2.h"
#include "darkseed2/saveable.h"
#include "darkseed2/script.h"

namespace DarkSeed2 {

class ScriptInterpreter : public Saveable {
public:
	ScriptInterpreter(DarkSeed2Engine &vm);
	~ScriptInterpreter();

	/** Are there any scripts currently in the queue? */
	bool hasScripts() const;

	/** Remove all scripts from the queue. */
	void clear();

	/** Push the chunks with met conditions into the queue. */
	bool interpret(Common::List<ScriptChunk *> &chunks);

	/** Update status, interpret next lines, .... */
	bool updateStatus();

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	friend class SaveLoad;

	/** The result of a script action. */
	enum Result {
		kResultOK,     ///< Everything was okay, proceed to the next line.
		kResultWait,   ///< Wait on this line.
		kResultStop,   ///< Stop this script.
		kResultInvalid ///< Invalid script line.
	};

	/** Waiting for something to happen. */
	enum Wait {
		kWaitNone         = 0, ///< Waiting for nothing
		kWaitConversation = 1, ///< Waiting for the conversation to end
		kWaitMovie        = 2  ///< Waiting for a movie to end
	};

	/** A script state. */
	struct Script {
		/** The actual script chunk. */
		ScriptChunk *chunk;
		/** The script chunk's signature. */
		Common::String signature;

		/** The current script action within that chunk. */
		const ScriptChunk::Action *action;

		/** The name of the sound last started by that script. */
		Common::String soundName;
		/** The name of the signal variable for the last started sound by that script. */
		Common::String soundVar;
		/** The ID of the sound last started by that script. */
		int soundID;
		/** Was that sound a talk line? */
		bool soundTalk;

		/** The event the script is currently waiting for. */
		Wait waitingFor;
		/** Number of updates since the last wait debug message. */
		int lastWaitDebug;

		Script(ScriptChunk *chnk = 0);
	};

	/** A script function. */
	typedef Result (ScriptInterpreter::*func_t)(Script &script);
	/** An opcode. */
	struct OpcodeEntry {
		func_t func;      ///< The opcode's function.
		const char *name; ///< The opcode's name.
	};

	DarkSeed2Engine *_vm;

	/** All opcodes. */
	static OpcodeEntry _scriptFunc[kScriptActionNone];

	/** Number of updateStatus() class without any changes in the interpreter state. */
	int _updatesWithoutChanges;

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
	Result oFrom(Script &script);
	Result oPaletteChange(Script &script);
	Result oChangeAt(Script &script);
	Result oDialog(Script &script);
	Result oPicture(Script &script);
	Result oSpeech(Script &script);
	Result oSpeechVar(Script &script);
	Result oWaitUntil(Script &script);
	Result oEffect(Script &script);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_INTER_H
