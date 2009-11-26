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


#include "engines/darkseed2/conversation.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/talk.h"

namespace DarkSeed2 {

Conversation::Entry::Entry(Node &pa) {
	visible = false;
	initial = false;
	parent = &pa;
}


Conversation::Node::Node() {
	fallthroughNum = 0;
}


Conversation::Conversation(Variables &variables) : _variables(&variables) {
	_startNode = 0;
	_currentNode = 0;
}

Conversation::~Conversation() {
	clear();
}

void Conversation::clear() {
	for (NodeMap::iterator n = _nodes.begin(); n != _nodes.end(); ++n) {
		Node &node = *n->_value;

		for (EntryMap::iterator e = node.entries.begin(); e != node.entries.end(); ++e)
			delete e->_value;

		delete n->_value;
	}

	_startNode = 0;
	_speakers.clear();
}

bool Conversation::parse(DATFile &conversation) {
	const Common::String *cmd, *args;
	while (conversation.nextLine(cmd, args)) {
		if (cmd->equalsIgnoreCase("speaker")) {
			// A character taking part in the conversation

			if (!addSpeaker(*args))
				return false;

		} else if (cmd->equalsIgnoreCase("node")) {
			// A conversation node

			if (!parseNode(*args, conversation))
				return false;

		} else if (cmd->equalsIgnoreCase("conversation")) {
			// Useless information, ignoring
		} else {
			// Unknown command, error

			warning("Conversation::parse(): Unknown command \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
			return false;
		}

	}

	reset();

	return true;
}

bool Conversation::parse(const Resources &resources, const Common::String &convName) {
	if (!resources.hasResource(convName + ".TXT")) {
		warning("Conversation::parse(): No such conversation \"%s\"", convName.c_str());
		return false;
	}

	Resource *resource = resources.getResource(convName + ".TXT");
	DATFile conversation(*resource);

	bool result = parse(conversation);

	delete resource;

	return result;
}

void Conversation::reset() {
	for (NodeMap::iterator n = _nodes.begin(); n != _nodes.end(); ++n) {
		Node &node = *n->_value;

		for (EntryMap::iterator e = node.entries.begin(); e != node.entries.end(); ++e) {
			Entry &entry = *e->_value;

			entry.visible = entry.initial;
		}

	}

	_currentNode = _startNode;
}

bool Conversation::addSpeaker(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 2) {
		warning("Conversation::addSpeaker(): Broken arguments");
		return false;
	}

	int num = atoi(lArgs[0].c_str());

	_speakers.resize(MAX<int>(_speakers.size(), num + 1));

	_speakers[num] = lArgs[1];

	return true;
}

bool Conversation::handleAssign(Entry &entry, const Common::String &args, uint8 &speaker) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 2) {
		warning("Conversation::handleAssign(): Broken arguments");
		return false;
	}

	if (lArgs[0].equalsIgnoreCase("speaker")) {
		speaker = atoi(lArgs[1].c_str());
		return true;
	}

	warning("Conversation::handleAssign(): Real variable assignment unsupported (%s)",
			lArgs[0].c_str());
	return false;
}

bool Conversation::setGoTo(Entry &entry, const Common::String &args) {
	if (DATFile::argCount(args) != 1) {
		warning("Conversation::setGoTo(): Broken arguments");
		return false;
	}

	entry.goTo = args;
	return true;
}

bool Conversation::addUnhide(Entry &entry, const Common::String &args) {
	if (DATFile::argCount(args) != 1) {
		warning("Conversation::addUnhide(): Broken arguments");
		return false;
	}

	entry.unhide.push_back(args);
	return true;
}

bool Conversation::addHide(Entry &entry, const Common::String &args) {
	if (DATFile::argCount(args) != 1) {
		warning("Conversation::addHide(): Broken arguments");
		return false;
	}

	entry.hide.push_back(args);
	return true;
}

bool Conversation::addMessage(Entry &entry, const Common::String &args, uint8 speaker) {
	if (DATFile::argCount(args) != 1) {
		warning("Conversation::addMessage(): Broken arguments");
		return false;
	}

	entry.speakers.push_back(speaker);
	entry.messages.push_back(args);
	return true;
}

bool Conversation::setText(Entry &entry, const Common::String &args) {
	if (DATFile::argCount(args) != 1) {
		warning("Conversation::setText(): Broken arguments");
		return false;
	}

	entry.text = args;
	return true;
}

bool Conversation::addEntry(Entry &entry, DATFile &conversation) {
	uint8 curSpeaker = 1;

	const Common::String *cmd, *args;
	while (conversation.nextLine(cmd, args)) {
		if ((cmd->equalsIgnoreCase("entry")) || (cmd->equalsIgnoreCase("node"))) {
			// This entry is finished here

			conversation.previous();
			break;

		} else if (cmd->equalsIgnoreCase("text")) {
			// The entry's main text

			if (!setText(entry, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("message")) {
			// The entry's reply

			if (!addMessage(entry, *args, curSpeaker))
				return false;

		} else if (cmd->equalsIgnoreCase("hide")) {
			// Selecting this entry will hide that entry

			if (!addHide(entry, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("unhide")) {
			// Selecting this entry will unhide that entry

			if (!addUnhide(entry, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("goto")) {
			// Will continue with that node

			if (!setGoTo(entry, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("assign")) {
			// Assigning a value to a variable

			if (!handleAssign(entry, *args, curSpeaker))
				return false;

		} else {
			// Unknown command, error

			warning("Conversation::addEntry(): Unknown command \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
			return false;
		}

	}

	return true;
}

bool Conversation::addEntry(Node &node, const Common::String &args, DATFile &conversation) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if ((lArgs.size() != 1) && (lArgs.size() != 2)) {
		warning("Conversation::addEntry(): Broken arguments");
		return false;
	}

	if ((lArgs.size() == 2) && !lArgs[1].equalsIgnoreCase("initial")) {
		warning("Conversation::addEntry(): Unknown modifier \"%s\"", lArgs[1].c_str());
		return false;
	}

	Entry *entry = new Entry(node);

	if (!addEntry(*entry, conversation)) {
		delete entry;
		return false;
	}

	if ((lArgs.size() == 2)) {
		entry->initial = true;
	}

	entry->name = lArgs[0];
	node.entries.setVal(entry->name, entry);

	return true;
}

bool Conversation::setFallthrough(Node &node, const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 2) {
		warning("Conversation::setFallthrough(): Broken arguments");
		return false;
	}

	node.fallthroughNum = atoi(lArgs[0].c_str());
	node.fallthrough = lArgs[1];

	if (node.fallthroughNum != 0)
		warning("Conversation::setFallthrough(): Fallthrough to \"%s\" with number %d\n",
				node.fallthrough.c_str(), node.fallthroughNum);

	return true;
}

bool Conversation::parseNode(DATFile &conversation, Node &node) {
	const Common::String *cmd, *args;
	while (conversation.nextLine(cmd, args)) {
		if (cmd->equalsIgnoreCase("node")) {
			// This node is finished here

			conversation.previous();
			break;

		} else if (cmd->equalsIgnoreCase("fallthrough")) {
			// Unknown

			if (!setFallthrough(node, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("entry")) {
			// A conversation entry

			if (!addEntry(node, *args, conversation))
				return false;

		} else {
			// Unknown command, error

			warning("Conversation::parseNode(): Unknown command \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
			return false;
		}
	}

	return true;
}

bool Conversation::parseNode(const Common::String &args, DATFile &conversation) {
	if (DATFile::argCount(args) != 1) {
		warning("Conversation::parseNode(): Broken arguments");
		return false;
	}

	Node *node = new Node;

	// Parsing the node's data
	if (!parseNode(conversation, *node)) {
		delete node;
		return false;
	}

	// Adding the node to our hashmap
	_nodes.setVal(args, node);

	// If this is the first node, it's our start node
	if (!_startNode)
		_startNode = node;

	return true;
}

Common::Array<Conversation::Entry *> Conversation::getVisibleEntries(Node &node) {
	Common::Array<Entry *> entries;

	for (EntryMap::iterator it = node.entries.begin(); it != node.entries.end(); ++it)
		if (it->_value->visible)
			entries.push_back(it->_value);

	return entries;
}

Common::Array<const Conversation::Entry *> Conversation::getVisibleEntries(Node &node) const {
	Common::Array<const Entry *> entries;

	for (EntryMap::const_iterator it = node.entries.begin(); it != node.entries.end(); ++it)
		if (it->_value->visible)
			entries.push_back(it->_value);

	return entries;
}

Common::Array<TalkLine *> Conversation::getCurrentLines(Resources &resources) const {
	Common::Array<TalkLine *> lines;

	if (!_currentNode)
		return lines;

	Common::Array<const Entry *> entries = getVisibleEntries(*_currentNode);
	for (Common::Array<const Entry *>::iterator it = entries.begin(); it != entries.end(); ++it) {
		TalkLine *line = new TalkLine(resources, (*it)->text);

		line->setName((*it)->name);
		line->setSpeaker(_speakers[0]);

		lines.push_back(line);
	}

	return lines;
}

Common::Array<TalkLine *> Conversation::getReplies(Resources &resources,
		const Common::String &entry) const {

	Common::Array<TalkLine *> replies;

	if (!_currentNode || !_currentNode->entries.contains(entry))
		return replies;

	Entry *e = _currentNode->entries.getVal(entry);

	Common::Array<uint8>::const_iterator speaker = e->speakers.begin();
	Common::Array<Common::String>::const_iterator message = e->messages.begin();

	for (; (speaker != e->speakers.end()) && (message != e->messages.end()); ++speaker, ++message) {
		TalkLine *reply = new TalkLine(resources, *message);

		reply->setSpeaker(_speakers[*speaker]);

		replies.push_back(reply);
	}

	return replies;
}

void Conversation::hide(const Common::String &entry) {
	if (!_currentNode)
		return;

	if (!_currentNode->entries.contains(entry))
		return;

	Entry *e = _currentNode->entries.getVal(entry);

	e->visible = false;
}

void Conversation::hide(const Common::Array<Common::String> &entries) {
	if (!_currentNode)
		return;

	for (Common::Array<Common::String>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		hide(*it);
}

void Conversation::unhide(const Common::String &entry) {
	if (!_currentNode)
		return;

	if (!_currentNode->entries.contains(entry))
		return;

	Entry *e = _currentNode->entries.getVal(entry);

	e->visible = true;
}

void Conversation::unhide(const Common::Array<Common::String> &entries) {
	if (!_currentNode)
		return;

	for (Common::Array<Common::String>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		unhide(*it);
}

bool Conversation::hasEnded() const {
	return _currentNode == 0;
}

void Conversation::goTo(const Common::String &node) {
	if (node.equalsIgnoreCase("exit") || !_nodes.contains(node)) {
		_currentNode = 0;
		return;
	}

	_currentNode = _nodes.getVal(node);

	if (getVisibleEntries(*_currentNode).empty()) {
		// No visible entries anymore, moving along to the fallthrough

		const Common::String fallthrough = _currentNode->fallthrough;
		if (fallthrough.empty() ||
				fallthrough.equalsIgnoreCase("exit") || !_nodes.contains(fallthrough)) {

			// No (valid) fallthrough, exit
			_currentNode = 0;
			return;
		}

		_currentNode = _nodes.getVal(fallthrough);
	}
}

void Conversation::pick(const Common::String &entry) {
	if (!_currentNode)
		return;

	if (!_currentNode->entries.contains(entry))
		return;

	Entry *e = _currentNode->entries.getVal(entry);

	if (!e->visible) {
		// O_o
		warning("Conversation::pick(): An invisible entry picked?!?");
		return;
	}

	// A entry should automatically hide itself
	hide(entry);

	// Evaluate changes brought in by the entry
	hide(e->hide);
	unhide(e->unhide);
	goTo(e->goTo);
}

void Conversation::discardLines(Common::Array<TalkLine *> &lines) {
	for (Common::Array<TalkLine *>::iterator it = lines.begin(); it != lines.end(); ++it)
		delete *it;

	lines.clear();
}

void Conversation::discardLines(TalkLine *&lines) {
	delete lines;
	lines = 0;
}

} // End of namespace DarkSeed2
