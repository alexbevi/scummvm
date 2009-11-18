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
	_left   = 0;
	_top    = 0;
	_right  = 0;
	_bottom = 0;
}

Object::Object(const Variables &variables) : _variables(&variables) {
	_left   = 0;
	_top    = 0;
	_right  = 0;
	_bottom = 0;

	_scripts.resize(kObjectVerbNone);
}

Object::~Object() {
	for (uint i = 0; i < _scripts.size(); i++)
		for (Common::List<ScriptChunk *>::iterator it = _scripts[i].begin(); it != _scripts[i].end(); ++it)
			delete *it;
}

bool Object::parse(DATFile &dat) {
	assert(_variables);

	ObjectVerb curVerb = kObjectVerbNone;

	const Common::String *cmd, *args;
	while (dat.nextLine(cmd, args)) {
		if (cmd->equalsIgnoreCase("ObjDesc")) {
			// Object description / name
			_name = *args;
		} else if (cmd->equalsIgnoreCase("ObjXY")) {
			// Object coordinates

			Common::Array<Common::String> lArgs = DATFile::argGet(*args);

			// Need to be 4
			if (lArgs.size() != 4) {
				warning("Object::parse(): ObjXY arguments broken");
				return false;
			}

			_left   = atoi(lArgs[0].c_str());
			_top    = atoi(lArgs[1].c_str());
			_right  = atoi(lArgs[2].c_str()) - 1;
			_bottom = atoi(lArgs[3].c_str()) - 1;

		} else if (cmd->matchString("*Start")) {
			// Start of a verb section

			curVerb = parseObjectVerb(*cmd);
			if (curVerb == kObjectVerbNone) {
				warning("Object::parse(): Unknown script verb \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
				return false;
			}

		} else {
			// Need to be in a verb section
			if (curVerb == kObjectVerbNone) {
				warning("Object::parse(): Action without a verb \"%s\" (\"%s\")", cmd->c_str(), args->c_str());
				return false;
			}

			// Rewind past the line we've just read
			dat.previous();

			// Parse the script chunk
			ScriptChunk *script = new ScriptChunk(*_variables);
			if (!script->parse(dat)) {
				delete script;
				return false;
			}

			// Add it to our list
			_scripts[(int) curVerb].push_back(script);
		}
	}

	return true;
}

const Common::List<ScriptChunk *> *Object::getScripts(ObjectVerb verb) const {
	assert(_variables);

	if ((verb < 0) || (verb >= kObjectVerbNone))
		return 0;

	return &_scripts[(int) verb];
}

ObjectVerb Object::parseObjectVerb(const Common::String &verb) {
	for (int i = 0; i < kObjectVerbNone; i++)
		if (verb.equalsIgnoreCase(objectVerb[i]))
			return (ObjectVerb) i;

	return kObjectVerbNone;
}


ObjectContainer::ObjectContainer(const Variables &variables) : _variables(&variables) {
}

ObjectContainer::~ObjectContainer() {
	clear();
}

void ObjectContainer::clear() {
	_objects.clear();
}

bool ObjectContainer::parse(DATFile &dat) {
	clear();

	const Common::String *cmd, *args;
	int32 objCount = -1;

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

	_objects.reserve(objCount);
	for (int i = 0; i < objCount; i++) {
		_objects.push_back(Object(*_variables));

		if (!_objects[i].parse(dat))
			return false;
	}

	return true;
}

} // End of namespace DarkSeed2
