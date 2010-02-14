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

#include "engines/darkseed2/objects.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/script.h"

namespace DarkSeed2 {

static const char *objectVerb[] = {
	"GoStart", "LookStart", "UseStart"
};


Object::Object() : _variables(0) {
}

Object::Object(const Variables &variables, ScriptRegister &scriptRegister) {
	_variables = &variables;

	_scriptRegister = &scriptRegister;

	_scripts.resize(kObjectVerbNone);
}

Object::~Object() {
	for (uint i = 0; i < _scripts.size(); i++)
		for (Common::List<ScriptChunk *>::iterator it = _scripts[i].begin(); it != _scripts[i].end(); ++it)
			delete *it;
}

bool Object::setName(const Common::String &args) {
	if (DATFile::argCount(args) < 1) {
		warning("Object::setName: Broken arguments");
		return false;
	}

	_name = args;

	if (_name.empty())
		_name = "[UNNAMED]";

	return true;
}

bool Object::setDimensions(const Common::String &args) {
	Common::Array<Common::String> lArgs = DATFile::argGet(args);

	if (lArgs.size() != 4) {
		warning("Object::setDimensions(): Broken arguments");
		return false;
	}

	_area = Common::Rect(atoi(lArgs[0].c_str()), atoi(lArgs[1].c_str()),
	                     atoi(lArgs[2].c_str()), atoi(lArgs[3].c_str()));

	return true;
}

bool Object::setVerb(const Common::String &cmd, ObjectVerb &curVerb) {
	curVerb = parseObjectVerb(cmd);
	if (curVerb == kObjectVerbNone) {
		warning("Object::setVerb(): Unknown script verb \"%s\"", cmd.c_str());
		return false;
	}

	return true;
}

bool Object::addScriptChunk(const Common::String &cmd, DATFile &dat, ObjectVerb curVerb) {
	// Need to be in a verb section
	if (curVerb == kObjectVerbNone) {
		warning("Object::addScriptChunk(): Action without a verb \"%s\"", cmd.c_str());
		return false;
	}

	// Rewind past the line we've just read
	dat.previous();

	// Parse the script chunk
	ScriptChunk *script = new ScriptChunk(*_variables, *_scriptRegister);
	if (!script->parse(dat)) {
		delete script;
		return false;
	}

	// Add it to our list
	_scripts[(int) curVerb].push_back(script);

	return true;
}

bool Object::parse(DATFile &dat) {
	assert(_variables);

	ObjectVerb curVerb = kObjectVerbNone;

	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if (cmd->equalsIgnoreCase("ObjDesc")) {
			// Object description / name

			if (!_name.empty()) {
				// Already got a name, so this is already the next object
				dat.previous();
				break;
			}

			if (!setName(*args))
				return false;

			debugC(-1, kDebugObjects, "Parsing object \"%s\"", _name.c_str());

		} else if (cmd->equalsIgnoreCase("ObjXY")) {
			// Object coordinates

			if (!setDimensions(*args))
				return false;

		} else if (cmd->matchString("*Start")) {
			// Start of a verb section

			if (!setVerb(*cmd, curVerb))
				return false;

		} else if (cmd->matchString("*End", true)) {
		} else {
			// Script chunk

			if (!addScriptChunk(*cmd, dat, curVerb))
				return false;

		}
	}

	return true;
}

const Common::String &Object::getName() const {
	return _name;
}

const Common::List<ScriptChunk *> &Object::getScripts(ObjectVerb verb) const {
	assert(_variables);
	assert(verb < kObjectVerbNone);

	return _scripts[(int) verb];
}

Common::List<ScriptChunk *> &Object::getScripts(ObjectVerb verb) {
	assert(_variables);
	assert(verb < kObjectVerbNone);

	return _scripts[(int) verb];
}

bool Object::isIn(uint32 x, uint32 y) const {
	return _area.contains(x, y);
}

bool Object::hasVerb(ObjectVerb verb) const {
	if (verb >= kObjectVerbNone)
		return false;

	return !_scripts[(int) verb].empty();
}

bool Object::hasActiveVerb(ObjectVerb verb) const {
	if (verb >= kObjectVerbNone)
		return false;

	// Iterator through all scripts, looking for one with met conditions
	const Common::List<ScriptChunk *> &scripts = _scripts[(int) verb];
	for (Common::List<ScriptChunk *>::const_iterator it = scripts.begin(); it != scripts.end(); ++it)
		if ((*it)->conditionsMet())
			return true;

	return false;
}

ObjectVerb Object::parseObjectVerb(const Common::String &verb) {
	for (int i = 0; i < kObjectVerbNone; i++)
		if (verb.equalsIgnoreCase(objectVerb[i]))
			return (ObjectVerb) i;

	return kObjectVerbNone;
}


ObjectContainer::ObjectContainer(const Variables &variables, ScriptRegister &scriptRegister) {
	_variables = &variables;

	_scriptRegister = &scriptRegister;
}

ObjectContainer::~ObjectContainer() {
	clear();
}

Common::Array<Object> &ObjectContainer::getObjects() {
	return _objects;
}

void ObjectContainer::clear() {
	_objects.clear();
}

Object *ObjectContainer::findObject(const Common::String &name) {
	for (Common::Array<Object>::iterator it = _objects.begin(); it != _objects.end(); ++it)
		if (it->getName().equalsIgnoreCase(name))
			return &*it;

	return 0;
}

Object *ObjectContainer::findObject(uint32 x, uint32 y) {
	for (Common::Array<Object>::iterator it = _objects.begin(); it != _objects.end(); ++it)
		if (it->isIn(x, y))
			return &*it;

	return 0;
}

Object *ObjectContainer::findAutoObject() {
	for (Common::Array<Object>::iterator it = _objects.begin(); it != _objects.end(); ++it)
		if (it->getName().matchString("auto*", true))
			return &*it;

	return 0;
}

bool ObjectContainer::parse(DATFile &dat) {
	clear();

	const Common::String *cmd, *args;
	int32 objCount = -1;

	// Looking for the object count
	for (int i = 0; i < 2; i++) {
		if (!dat.nextLine(cmd, args)) {
			warning("ObjectContainer::parse(): unexpected EOF");
			return false;
		}

		if (cmd->equalsIgnoreCase("NoObjects"))
			objCount = atoi(args->c_str());
	}

	if (objCount == -1) {
		warning("ObjectContainer::parse(): No object count specified");
		return false;
	}

	// Reading all objects
	_objects.reserve(objCount);
	for (int i = 0; i < objCount; i++) {
		_objects.push_back(Object(*_variables, *_scriptRegister));

		if (!_objects[i].parse(dat))
			return false;
	}

	return true;
}

} // End of namespace DarkSeed2
