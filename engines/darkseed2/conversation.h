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
	bool reset();

	/** Get all currently available lines. */
	Common::Array<TalkLine *> getCurrentLines(Resources &resources);
	/** Get the replies to a certain line. */
	Common::Array<TalkLine *> getReplies(Resources &resources, const Common::String &entry) const;

	/** Has the conversation ended? */
	bool hasEnded() const;

	/** The user has picked a certain entry. */
	void pick(const Common::String &entry);

	/** Free the TalkLines. */
	void discardLines(Common::Array<TalkLine *> &lines);
	/** Free the TalkLine. */
	void discardLines(TalkLine *&lines);

private:
	struct Node;

	/** A when-picked action. */
	struct Action {
		Common::String operand;   ///< The action's target.
		Common::String condition; ///< The condition that has to be met.

		Action(const Common::String &op = "", const Common::String &cond = "");
	};

	/** A variable assignment action. */
	struct Assign {
		Common::String variable; ///< The variable name.
		uint8 value;             ///< The new value.

		Assign(const Common::String &var = "", const Common::String &val = "");
	};

	/** A conversation entry. */
	struct Entry {
		bool visible;   ///< Currently visible?
		bool persist;   ///< Unknown.
		bool initial;   ///< Initially visible?
		bool destroyed; ///< Unknown.

		Common::String name; ///< The entry's name.
		Common::String text; ///< The entry's text.

		Common::Array<uint8> speakers;          ///< The reply line speakers.
		Common::Array<Common::String> messages; ///< The reply lines.

		Common::Array<Action> hide;    ///< When picked, these lines will be hidden.
		Common::Array<Action> unhide;  ///< When picked, these lines will be unhidden.
		Common::Array<Action> destroy; ///< When picked, these lines will be destroyed.
		Common::Array<Action> goTo;    ///< Candidates for the following node.

		Common::Array<Assign> assigns; ///< When picked, these variables will be assigned values.

		Node *parent; ///< The node this entry belongs too.

		Entry(Node &pa);
	};

	typedef Common::HashMap<Common::String, Entry *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> EntryMap;
	typedef Common::Array<Entry *> EntryList;

	/** A conversation node. */
	struct Node {
		/** Number of entries left for the fallthrough to kick in. */
		uint32 fallthroughNum;
		/** Name of the node to fall through. */
		Common::String fallthrough;

		EntryMap entries;        ///< Entries mapped by name.
		EntryList sortedEntries; ///< Entries sorted by occurence in the file.

		Common::String name; ///< The name of the node.

		Common::Array<Action> goTo; ///< Node names to jump to.

		Node();
	};

	typedef Common::HashMap<Common::String, Node *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> NodeMap;

	Variables *_variables;

	/** Was everything set up so that the conversation can be held? */
	bool _ready;

	NodeMap _nodes;     ///< All nodes.
	Node *_startNode;   ///< The starting node.
	Node *_currentNode; ///< The current node.

	/** The people active in the conversation. */
	Common::Array<Common::String> _speakers;

	/** Find the next node that has active entries. */
	void nextActiveNode();
	/** Find all entries of the current node that are not hidden. */
	Common::Array<Entry *> getVisibleEntries(Node &node);
	/** Find all entries of the current node that are not hidden. */
	Common::Array<const Entry *> getVisibleEntries(Node &node) const;

	// Conversation execution helpers
	void hide(const Action&entry);
	void hide(const Common::Array<Action> &entries);
	void unhide(const Action &entry);
	void unhide(const Common::Array<Action> &entries);
	void destroy(const Action &entry);
	void destroy(const Common::Array<Action> &entries);
	void assign(const Assign &entry);
	void assign(const Common::Array<Assign> &entries);
	bool goTo(const Common::Array<Action> &node);

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
	bool addDestroy(Entry &entry, const Common::String &args);
	bool addUnhide(Entry &entry, const Common::String &args);
	bool addHide(Entry &entry, const Common::String &args);
	bool addMessage(Entry &entry, const Common::String &args, uint8 speaker);
	bool setText(Entry &entry, const Common::String &args);
	void stripComma(Common::String &str);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATION_H
