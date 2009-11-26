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

#ifndef DARKSEED2_CONVERSATION_H
#define DARKSEED2_CONVERSATION_H

#include "common/str.h"
#include "common/array.h"
#include "common/hashmap.h"

#include "engines/darkseed2/darkseed2.h"

namespace DarkSeed2 {

class Resources;
class Variables;

class DATFile;
class TalkLine;

class Conversation {
public:
	Conversation(Variables &variables);
	~Conversation();

	/** Discard the conversation. */
	void clear();

	/** Parse a conversation out of a DAT file. */
	bool parse(DATFile &conversation);
	/** Parse a conversation out of a resource. */
	bool parse(const Resources &resources, const Common::String &convName);

	/** Reset the conversation to the loading defaults. */
	void reset();

	/** Get all currently available lines. */
	Common::Array<TalkLine *> getCurrentLines(Resources &resources);
	/** Get the reply to a certain line. */
	Common::Array<TalkLine *> getReplies(Resources &resources, const Common::String &entry) const;

	/** Has the conversation ended? */
	bool hasEnded() const;

	/** The user has picked a certain entry. */
	void pick(const Common::String &entry);

	void discardLines(Common::Array<TalkLine *> &lines);
	void discardLines(TalkLine *&lines);

private:
	struct Node;

	struct Action {
		Common::String operand;
		Common::String condition;

		Action(const Common::String &op = "", const Common::String &cond = "");
	};

	struct Assign {
		Common::String variable;
		uint8 value;

		Assign(const Common::String &var = "", const Common::String &val = "");
	};

	struct Entry {
		bool visible;
		bool initial;

		Common::String name;
		Common::String text;

		Common::Array<uint8> speakers;
		Common::Array<Common::String> messages;

		Common::Array<Action> hide;
		Common::Array<Action> unhide;
		Common::Array<Action> goTo;

		Common::Array<Assign> assigns;

		Node *parent;

		Entry(Node &pa);
	};

	typedef Common::HashMap<Common::String, Entry *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> EntryMap;

	struct Node {
		uint32 fallthroughNum;
		Common::String fallthrough;
		EntryMap entries;

		Common::String name;
		Common::Array<Action> goTo;

		Node();
	};

	typedef Common::HashMap<Common::String, Node *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> NodeMap;

	Variables *_variables;

	NodeMap _nodes;
	Node *_startNode;
	Node *_currentNode;

	Common::Array<Common::String> _speakers;

	void nextActiveNode();
	Common::Array<Entry *> getVisibleEntries(Node &node);
	Common::Array<const Entry *> getVisibleEntries(Node &node) const;

	// Conversation execution helpers
	void hide(const Action&entry);
	void hide(const Common::Array<Action> &entries);
	void unhide(const Action &entry);
	void unhide(const Common::Array<Action> &entries);
	void assign(const Assign &entry);
	void assign(const Common::Array<Assign> &entries);
	void goTo(const Common::Array<Action> &node);

	// Parsing helpers
	bool addSpeaker(const Common::String &args);
	bool parseNode(const Common::String &args, DATFile &conversation);
	bool parseNode(DATFile &conversation, Node &node);
	bool addEntry(Entry &entry, DATFile &conversation);
	bool addEntry(Node &node, const Common::String &args, DATFile &conversation);
	bool addGoTo(Node &node, const Common::String &args);
	bool setFallthrough(Node &node, const Common::String &args);
	bool handleAssign(Entry &entry, const Common::String &args, uint8 &speaker);
	bool addAction(Common::Array<Action> &actions, const Common::String &args);
	bool addGoTo(Entry &entry, const Common::String &args);
	bool addUnhide(Entry &entry, const Common::String &args);
	bool addHide(Entry &entry, const Common::String &args);
	bool addMessage(Entry &entry, const Common::String &args, uint8 speaker);
	bool setText(Entry &entry, const Common::String &args);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATION_H
