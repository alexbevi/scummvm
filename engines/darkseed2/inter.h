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

	bool interpret(Common::List<ScriptChunk *> &chunks);

private:
	typedef bool (ScriptInterpreter::*func_t)(const ScriptChunk::Action &action);
	struct OpcodeEntry {
		func_t func;
		const char *name;
	};

	DarkSeed2Engine *_vm;

	Common::String _speechVar;

	/** All opcodes. */
	static OpcodeEntry _scriptFunc[kScriptActionNone];

	// Interpreting helpers
	bool interpret(ScriptChunk &chunk, bool &ran);
	bool interpret(const ScriptChunk::Action &action);

	// Opcodes
	bool oXYRoom(const ScriptChunk::Action &action);
	bool oCursor(const ScriptChunk::Action &action);
	bool oChange(const ScriptChunk::Action &action);
	bool oText(const ScriptChunk::Action &action);
	bool oMidi(const ScriptChunk::Action &action);
	bool oAnim(const ScriptChunk::Action &action);
	bool oStatus(const ScriptChunk::Action &action);
	bool oSequence(const ScriptChunk::Action &action);
	bool oSpriteIDX(const ScriptChunk::Action &action);
	bool oClipXY(const ScriptChunk::Action &action);
	bool oPosX(const ScriptChunk::Action &action);
	bool oPosY(const ScriptChunk::Action &action);
	bool oScaleVal(const ScriptChunk::Action &action);
	bool oFrom(const ScriptChunk::Action &action);
	bool oPaletteChange(const ScriptChunk::Action &action);
	bool oXYRoomEffect(const ScriptChunk::Action &action);
	bool oChangeAt(const ScriptChunk::Action &action);
	bool oDialog(const ScriptChunk::Action &action);
	bool oPicture(const ScriptChunk::Action &action);
	bool oSpeech(const ScriptChunk::Action &action);
	bool oSpeechVar(const ScriptChunk::Action &action);
	bool oWaitUntil(const ScriptChunk::Action &action);
	bool oEffect(const ScriptChunk::Action &action);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_INTER_H
