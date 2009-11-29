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

	/** Push the first chunk where the conditions are met into the queue. */
	bool interpret(Common::List<ScriptChunk *> &chunks);

	/** Update status, interpret next lines, .... */
	void updateStatus();

private:
	/** The result of a script action. */
	enum Result {
		kResultOK,     ///< Everything was okay, proceed to the next line.
		kResultWait,   ///< Wait on this line.
		kResultInvalid ///< Invalid script line.
	};

	typedef Result (ScriptInterpreter::*func_t)(const ScriptChunk::Action &action);
	typedef void (ScriptInterpreter::*updateFunc_t)(ScriptChunk &script);
	struct OpcodeEntry {
		func_t func;
		updateFunc_t updateFunc;
		const char *name;
	};

	DarkSeed2Engine *_vm;

	/** All opcodes. */
	static OpcodeEntry _scriptFunc[kScriptActionNone];

	/** The ID of the last started sound. */
	int _soundID;
	/** The current variable that gets changed when the talking/sfx ends. */
	Common::String _speechVar;

	/** The currently active scripts. */
	Common::List<ScriptChunk *> _scripts;

	// Interpreting helper
	Result interpret(const ScriptChunk::Action &action);
	void updateScriptState(ScriptChunk &script, const ScriptChunk::Action &action);

	// Opcodes
	Result oXYRoom(const ScriptChunk::Action &action);
	Result oCursor(const ScriptChunk::Action &action);
	Result oChange(const ScriptChunk::Action &action);
	Result oText(const ScriptChunk::Action &action);
	Result oMidi(const ScriptChunk::Action &action);
	Result oAnim(const ScriptChunk::Action &action);
	Result oStatus(const ScriptChunk::Action &action);
	Result oSequence(const ScriptChunk::Action &action);
	Result oSpriteIDX(const ScriptChunk::Action &action);
	Result oClipXY(const ScriptChunk::Action &action);
	Result oPosX(const ScriptChunk::Action &action);
	Result oPosY(const ScriptChunk::Action &action);
	Result oScaleVal(const ScriptChunk::Action &action);
	Result oFrom(const ScriptChunk::Action &action);
	Result oPaletteChange(const ScriptChunk::Action &action);
	Result oXYRoomEffect(const ScriptChunk::Action &action);
	Result oChangeAt(const ScriptChunk::Action &action);
	Result oDialog(const ScriptChunk::Action &action);
	Result oPicture(const ScriptChunk::Action &action);
	Result oSpeech(const ScriptChunk::Action &action);
	Result oSpeechVar(const ScriptChunk::Action &action);
	Result oWaitUntil(const ScriptChunk::Action &action);
	Result oEffect(const ScriptChunk::Action &action);

	void uText(ScriptChunk &script);
	void uSpeechVar(ScriptChunk &script);
	void uEffect(ScriptChunk &script);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_INTER_H
