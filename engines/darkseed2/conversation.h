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
	Common::Array<TalkLine *> getCurrentLines(Resources &resources) const;
	/** Get the reply to a certain line. */
	TalkLine *getReply(Resources &resources, const Common::String &entry) const;

	/** Has the conversation ended? */
	bool hasEnded() const;

	/** The user has picked a certain entry. */
	void pick(const Common::String &entry);

	void discardLines(Common::Array<TalkLine *> &lines);
	void discardLines(TalkLine *&lines);

private:
	struct Node;

	struct Entry {
		bool visible;
		bool initial;

		Common::String name;
		Common::String text;
		Common::String message;
		Common::String goTo;

		Common::Array<Common::String> hide;
		Common::Array<Common::String> unhide;

		Node *parent;

		Entry(Node &pa);
	};

	typedef Common::HashMap<Common::String, Entry *> EntryMap;

	struct Node {
		Common::String fallthrough;
		EntryMap entries;
	};

	typedef Common::HashMap<Common::String, Node *> NodeMap;

	Variables *_variables;

	NodeMap _nodes;
	Node *_startNode;
	Node *_currentNode;

	Common::Array<Common::String> _speakers;

	Common::Array<Entry *> getVisibleEntries(Node &node);
	Common::Array<const Entry *> getVisibleEntries(Node &node) const;

	// Conversation execution helpers
	void hide(const Common::String &entry);
	void hide(const Common::Array<Common::String> &entries);
	void unhide(const Common::String &entry);
	void unhide(const Common::Array<Common::String> &entries);
	void goTo(const Common::String &node);

	// Parsing helpers
	bool addSpeaker(const Common::String &args);
	bool parseNode(const Common::String &args, DATFile &conversation);
	bool parseNode(DATFile &conversation, Node &node);
	bool addEntry(Entry &entry, DATFile &conversation);
	bool addEntry(Node &node, const Common::String &args, DATFile &conversation);
	bool setFallthrough(Node &node, const Common::String &args);
	bool setGoTo(Entry &entry, const Common::String &args);
	bool addUnhide(Entry &entry, const Common::String &args);
	bool addHide(Entry &entry, const Common::String &args);
	bool setMessage(Entry &entry, const Common::String &args);
	bool setText(Entry &entry, const Common::String &args);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATION_H
