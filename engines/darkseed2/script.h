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

#include "darkseed2/darkseed2.h"
#include "darkseed2/saveable.h"

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
	kScriptActionFrom,
	kScriptActionPaletteChange,
	kScriptActionChangeAt,
	kScriptActionDialog,
	kScriptActionPicture,
	kScriptActionSpeech,
	kScriptActionSpeechVar,
	kScriptActionWaitUntil,
	kScriptActionEffect,
	kScriptActionNone
};

class ScriptChunk;

/** A global register of all script, for saving/loading script positions. */
class ScriptRegister : public Saveable {
public:
	ScriptRegister();
	~ScriptRegister();

	void clear();

	/** Add a script to the register. */
	void addScript(ScriptChunk &chunk);
	/** Remove a script to the register. */
	void removeScript(ScriptChunk &chunk);

	/** Return the script's current line. */
	uint32 getLine(const Common::String &signature) const;
	/** Return the script's current line. */
	uint32 getLine(const ScriptChunk &chunk) const;

	ScriptChunk *getScript(const Common::String &signature) const;

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	friend class SaveLoad;

	struct Script {
		ScriptChunk *chunk;
		uint32 line;

		Script();
		Script(ScriptChunk &c);
		Script(uint32 l);

		uint32 getLine() const;
	};

	typedef Common::HashMap<Common::String, Script, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> ScriptMap;

	ScriptMap _scriptMap;
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

	ScriptChunk(const Variables &variables, ScriptRegister &scriptRegister);
	~ScriptChunk();

	/** Was the end of the ScriptChunk reached? */
	bool atEnd() const;

	/** Return the "from" room flag. */
	uint32 getFrom() const;

	/** Advance the script to the next line. */
	void next();
	/** Rewind the ScriptChunk to the start. */
	void rewind();
	/** Seek the ScriptChunk to the end. */
	void seekEnd();
	/** Seek to a specific position. */
	void seekTo(uint32 n);

	/** Return the chunk's signature. */
	const Common::String &getSignature() const;
	/** Get the current position. */
	uint32 getCurLine() const;

	ScriptChunk &operator++();

	/** Reset the chunk to its empty factory state. */
	void clear();
	/** Parse a chunk out of a suitably positioned DAT file. */
	bool parse(DATFile &dat);

	/** Are all conditions for this chunk met? */
	bool conditionsMet() const;

	/** Get the script conditions. */
	const Common::List<Common::String> &getConditions() const;

	/** Get all script actions. */
	const Common::List<Action> &getActions() const;

	/** Return the current action. */
	const Action &getAction() const;

public:
	static const Action invalidAction;

	const Variables *_variables;

	ScriptRegister *_scriptRegister;

	/** The conditions required for this script. */
	Common::List<Common::String> _conditions;

	/** The script file's signature. */
	Common::String _signature;

	/** Was everything loaded so that the ScriptChunk can be interpreted? */
	bool _ready;

	uint32 _from; ///< The "from" room flag.

	/** All actions. */
	Common::List<Action> _actions;

	/** The current position within the actions. */
	Common::List<Action>::iterator _curPos;
	/** The current position's number within the actions. */
	uint32 _curPosNumber;

	/** Parse a script action string. */
	static ScriptAction parseScriptAction(const Common::String &action);

	void parseFrom(const Common::String &args);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_SCRIPT_H
