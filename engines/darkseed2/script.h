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

#ifndef DARKSEED2_SCRIPT_H
#define DARKSEED2_SCRIPT_H

#include "common/str.h"
#include "common/list.h"

#include "engines/darkseed2/darkseed2.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Variables;

class DATFile;

/** All possible actions a script can perform. */
enum ScriptAction {
	kScriptActionXYRoom = 0,
	kScriptActionCursor,
	kScriptActionChange,
	kScriptActionText,
	kScriptActionMidi,
	kScriptActionAnim,
	kScriptActionStatus,
	kScriptActionSequence,
	kScriptActionSpriteIDX,
	kScriptActionClipXY,
	kScriptActionPosX,
	kScriptActionPosY,
	kScriptActionScaleVal,
	kScriptActionFrom,
	kScriptActionPaletteChange,
	kScriptActionXYRoomEffect,
	kScriptActionChangeAt,
	kScriptActionDialog,
	kScriptActionPicture,
	kScriptActionSpeech,
	kScriptActionSpeechVar,
	kScriptActionWaitUntil,
	kScriptActionEffect,
	kScriptActionNone
};

/** A self-contained script chunk. */
class ScriptChunk {
public:
	/** A script action. */
	struct Action {
		ScriptAction action;      ///< The action.
		Common::String arguments; ///< The arguments.

		Action(ScriptAction act, const Common::String &args);
	};

	ScriptChunk(const Variables &variables);
	~ScriptChunk();

	/** Was the end of the ScriptChunk reached? */
	bool atEnd() const;

	/** Advance the script to the next line. */
	void next();
	/** Rewind the ScriptChunk to the start. */
	void rewind();
	/** Seek the ScriptChunk to the end. */
	void seekEnd();

	ScriptChunk &operator++();

	/** Reset the chunk to its empty factory state. */
	void clear();
	/** Parse a chunk out of a suitably positioned DAT file. */
	bool parse(DATFile &dat);

	/** Are all conditions for this chunk met? */
	bool conditionsMet() const;

	/** Get all script actions. */
	const Common::List<Action> &getActions() const;

	/** Return the current action. */
	const Action &getAction() const;

private:
	static const Action invalidAction;

	const Variables *_variables;

	Common::String _cond1; ///< The first condition.
	Common::String _cond2; ///< The second condition.

	/** Was everything loaded so that the ScriptChunk can be interpreted? */
	bool _ready;

	/** All actions. */
	Common::List<Action> _actions;

	/** The current position within the actions. */
	Common::List<Action>::iterator _curPos;

	/** Parse a script action string. */
	static ScriptAction parseScriptAction(const Common::String &action);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_SCRIPT_H
