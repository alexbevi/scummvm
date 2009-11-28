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
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/talk.h"

namespace DarkSeed2 {

Conversation::Action::Action(const Common::String &op, const Common::String &cond) {
	operand   = op;
	condition = cond;
}


Conversation::Assign::Assign(const Common::String &var, const Common::String &val) {
	variable = var;
	value    = atoi(val.c_str());
}


Conversation::Entry::Entry(Node &pa) {
	visible   = false;
	persist   = false;
	initial   = false;
	destroyed = false;
	parent = &pa;
}


Conversation::Node::Node() {
	fallthroughNum = 0;
}


Conversation::Conversation(Variables &variables) : _variables(&variables) {
	_ready = false;
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

	_ready = false;
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
		} else if (cmd->equalsIgnoreCase("declare")) {
			// Useless information, ignoring
		} else if (cmd->equalsIgnoreCase("import")) {
			// Useless information, ignoring
		} else {
			// Unknown command, error

			warning("Conversation::parse(): Unknown command \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
			return false;
		}

	}

	_ready = true;
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

bool Conversation::reset() {
	if (!_ready)
		return false;

	for (NodeMap::iterator n = _nodes.begin(); n != _nodes.end(); ++n) {
		Node &node = *n->_value;

		for (EntryMap::iterator e = node.entries.begin(); e != node.entries.end(); ++e) {
			Entry &entry = *e->_value;

			entry.visible   = entry.initial;
			entry.destroyed = false;
		}

	}

	_currentNode = _startNode;

	return true;
}

bool Conversation::addSpeaker(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() < 2) {
		warning("Conversation::addSpeaker(): Broken arguments");
		return false;
	}

	int num = atoi(lArgs[0].c_str());

	_speakers.resize(MAX<int>(_speakers.size(), num + 1));

	// Speaker names can include spaces
	_speakers[num].clear();
	_speakers[num] = DATFile::mergeArgs(lArgs, 1);

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

	entry.assigns.push_back(Assign(lArgs[0], lArgs[1]));

	return true;
}

bool Conversation::addAction(Common::Array<Action> &actions, const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if ((lArgs.size() != 1) && (lArgs.size() != 2)) {
		warning("Conversation::addAction(): Broken arguments");
		return false;
	}

	if (lArgs.size() == 1) {
		actions.push_back(Action(lArgs[0]));
		return true;
	}

	if ((lArgs[0][0] != '(') || (lArgs[0].lastChar() != ')')) {
		warning("Conversation::addAction(): Broken arguments");
		return false;
	}

	lArgs[0].deleteLastChar();
	lArgs[0].deleteChar(0);

	actions.push_back(Action(lArgs[1], lArgs[0]));

	return true;
}

bool Conversation::addGoTo(Entry &entry, const Common::String &args) {
	return addAction(entry.goTo, args);
}

bool Conversation::addDestroy(Entry &entry, const Common::String &args) {
	return addAction(entry.destroy, args);
}

bool Conversation::addUnhide(Entry &entry, const Common::String &args) {
	return addAction(entry.unhide, args);
}

bool Conversation::addHide(Entry &entry, const Common::String &args) {
	return addAction(entry.hide, args);
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
		debugC(2, kDebugConversation, "Parsing conversation entry command \"%s\" [%s]", cmd->c_str(), args->c_str());

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

		} else if (cmd->equalsIgnoreCase("destroy")) {
			// Destroying an entry (?)

			if (!addDestroy(entry, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("goto")) {
			// Will continue with that node

			if (!addGoTo(entry, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("assign")) {
			// Assigning a value to a variable

			Common::String sArgs = *args;
			stripComma(sArgs);

			if (!handleAssign(entry, sArgs, curSpeaker))
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

	if (lArgs.size() < 1) {
		warning("Conversation::addEntry(): Broken arguments");
		return false;
	}

	Entry *entry = new Entry(node);

	debugC(1, kDebugConversation, "Parsing conversation entry \"%s\"", lArgs[0].c_str());

	if (!addEntry(*entry, conversation)) {
		delete entry;
		return false;
	}

	for (uint i = 1; i < lArgs.size(); i++) {
		if (lArgs[i].empty())
			continue;

		if (lArgs[i].equalsIgnoreCase("initial")) {
			// Shown at startup
			entry->initial = true;
		} else if (lArgs[i].equalsIgnoreCase("persist")) {
			entry->persist = true;
		} else {
			warning("Conversation::addEntry(): Unknown modifier \"%s\"", lArgs[i].c_str());
			return false;
		}
	}

	entry->name = lArgs[0];
	node.entries.setVal(entry->name, entry);
	node.sortedEntries.push_back(entry);

	return true;
}

bool Conversation::addGoTo(Node &node, const Common::String &args) {
	return addAction(node.goTo, args);
}

bool Conversation::setFallthrough(Node &node, const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 2) {
		warning("Conversation::setFallthrough(): Broken arguments");
		return false;
	}

	node.fallthroughNum = atoi(lArgs[0].c_str());
	node.fallthrough = lArgs[1];

	return true;
}

bool Conversation::parseNode(DATFile &conversation, Node &node) {
	const Common::String *cmd, *args;
	while (conversation.nextLine(cmd, args)) {
		debugC(2, kDebugConversation, "Parsing conversation node command \"%s\" [%s]", cmd->c_str(), args->c_str());

		if (cmd->equalsIgnoreCase("node")) {
			// This node is finished here

			conversation.previous();
			break;

		} else if (cmd->equalsIgnoreCase("fallthrough")) {
			// Fallthrough. When only n lines are avaible, automatically go to the specified node

			if (!setFallthrough(node, *args))
				return false;

		} else if (cmd->equalsIgnoreCase("entry")) {
			// A conversation entry

			if (!addEntry(node, *args, conversation))
				return false;

		} else if (cmd->equalsIgnoreCase("goto")) {
			// Direct falling through to other nodes

			if (!addGoTo(node, *args))
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

	debugC(1, kDebugConversation, "Parsing conversation node \"%s\"", args.c_str());

	// Parsing the node's data
	if (!parseNode(conversation, *node)) {
		delete node;
		return false;
	}

	node->name = args;

	// Adding the node to our hashmap
	_nodes.setVal(args, node);

	if (!node->entries.empty() && !node->goTo.empty())
		warning("TODO: Node \"%s\" has goto and entry!", node->name.c_str());

	// If this is the first node, it's our start node
	if (!_startNode)
		_startNode = node;

	return true;
}

void Conversation::stripComma(Common::String &str) {
	const char *c;
	while ((c = strchr(str.c_str(), ',')) != 0) {
		if (c == str.c_str())
			str.deleteChar(0);
		else if ((c[1] == ' ') || (c[-1] == ' '))
			str.deleteChar(c - str.c_str());
		else
			str.setChar(' ', c - str.c_str());
	}

	for (Common::String::iterator it = str.begin(); it != str.end(); ++it)
		if (*it == ',')
			*it = ' ';
}

void Conversation::nextActiveNode() {
	if (!_currentNode)
		return;

	// While there's still a node and it doesn't have any entries...
	while (_currentNode && !_currentNode->goTo.empty() && goTo(_currentNode->goTo)) {
	}
}

Common::Array<Conversation::Entry *> Conversation::getVisibleEntries(Node &node) {
	Common::Array<Entry *> entries;

	for (EntryList::iterator it = node.sortedEntries.begin(); it != node.sortedEntries.end(); ++it)
		if ((*it)->visible)
			entries.push_back(*it);

	return entries;
}

Common::Array<const Conversation::Entry *> Conversation::getVisibleEntries(Node &node) const {
	Common::Array<const Entry *> entries;

	for (EntryList::const_iterator it = node.sortedEntries.begin(); it != node.sortedEntries.end(); ++it)
		if ((*it)->visible)
			entries.push_back(*it);

	return entries;
}

Common::Array<TalkLine *> Conversation::getCurrentLines(Resources &resources) {
	Common::Array<TalkLine *> lines;

	// Traversing to the next node with entries
	nextActiveNode();

	if (!_currentNode)
		// None found
		return lines;

	if (!_currentNode->entries.empty() && !_currentNode->goTo.empty())
		warning("TODO: Node \"%s\" has goto and entry!", _currentNode->name.c_str());

	Common::Array<Entry *> entries = getVisibleEntries(*_currentNode);
	for (Common::Array<Entry *>::iterator it = entries.begin(); it != entries.end(); ++it) {
		TalkLine *line = new TalkLine(resources, (*it)->text);

		line->setName((*it)->name);
		if (!_speakers.empty())
			line->setSpeaker(_speakers[0]);

		lines.push_back(line);
	}

	return lines;
}

Common::Array<TalkLine *> Conversation::getReplies(Resources &resources,
		const Common::String &entry) const {

	Common::Array<TalkLine *> replies;

	if (!_currentNode || !_currentNode->entries.contains(entry))
		// No replies
		return replies;

	Entry *e = _currentNode->entries.getVal(entry);

	Common::Array<uint8>::const_iterator speaker = e->speakers.begin();
	Common::Array<Common::String>::const_iterator message = e->messages.begin();

	// Building the reply talklines
	for (; (speaker != e->speakers.end()) && (message != e->messages.end()); ++speaker, ++message) {
		TalkLine *reply = new TalkLine(resources, *message);

		if (*speaker < _speakers.size())
			reply->setSpeaker(_speakers[*speaker]);

		replies.push_back(reply);
	}

	return replies;
}

void Conversation::hide(const Action &entry) {
	if (!_currentNode)
		return;

	// Does this entry exist?
	if (!_currentNode->entries.contains(entry.operand))
		return;

	// Condition met?
	if (!_variables->evalCondition(entry.condition))
		return;

	Entry *e = _currentNode->entries.getVal(entry.operand);

	// Hiding
	e->visible = false;
}

void Conversation::hide(const Common::Array<Action> &entries) {
	if (!_currentNode)
		return;

	for (Common::Array<Action>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		hide(*it);
}

void Conversation::unhide(const Action &entry) {
	if (!_currentNode)
		return;

	// Does this entry exist?
	if (!_currentNode->entries.contains(entry.operand))
		return;

	// Condition met?
	if (!_variables->evalCondition(entry.condition))
		return;

	Entry *e = _currentNode->entries.getVal(entry.operand);

	// Unhiding
	e->visible = true;
}

void Conversation::unhide(const Common::Array<Action> &entries) {
	if (!_currentNode)
		return;

	for (Common::Array<Action>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		unhide(*it);
}

void Conversation::destroy(const Action &entry) {
	if (!_currentNode)
		return;

	// Does this entry exist?
	if (!_currentNode->entries.contains(entry.operand))
		return;

	// Condition met?
	if (!_variables->evalCondition(entry.condition))
		return;

	Entry *e = _currentNode->entries.getVal(entry.operand);

	warning("TODO: Destroying \"%s\" (%s)", entry.operand.c_str(), e->text.c_str());

	// Destroying
	e->destroyed = true;
}

void Conversation::destroy(const Common::Array<Action> &entries) {
	if (!_currentNode)
		return;

	for (Common::Array<Action>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		destroy(*it);
}

void Conversation::assign(const Assign &entry) {
	if (!_currentNode)
		return;

	_variables->set(entry.variable, entry.value);
}

void Conversation::assign(const Common::Array<Assign> &entries) {
	if (!_currentNode)
		return;

	for (Common::Array<Assign>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		assign(*it);
}

bool Conversation::hasEnded() const {
	return !_ready || _currentNode == 0;
}

bool Conversation::goTo(const Common::Array<Action> &node) {
	bool gone = false;

	_currentNode = 0;

	for (Common::Array<Action>::const_iterator it = node.begin(); it != node.end(); ++it) {
		if (!_variables->evalCondition(it->condition))
			// Condition failed, next goto
			continue;

		if (it->operand.equalsIgnoreCase("exit") || !_nodes.contains(it->operand))
			// This is an exit
			break;

		gone = true;
		_currentNode = _nodes.getVal(it->operand);
		break;
	}

	if (!_currentNode)
		return gone;

	if (_currentNode->entries.empty() && !_currentNode->goTo.empty())
		// No entries, but gotos. Evaluate these instead
		return gone;

	if (getVisibleEntries(*_currentNode).size() <= _currentNode->fallthroughNum) {
		// Few enough visible entries, moving along to the fallthrough

		const Common::String fallthrough = _currentNode->fallthrough;
		if (fallthrough.empty() ||
				fallthrough.equalsIgnoreCase("exit") || !_nodes.contains(fallthrough)) {

			// No (valid) fallthrough, exit
			_currentNode = 0;
			return true;
		}

		gone = true;
		_currentNode = _nodes.getVal(fallthrough);
	}

	return gone;
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
	assign(e->assigns);
	hide(e->hide);
	unhide(e->unhide);
	destroy(e->destroy);
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
