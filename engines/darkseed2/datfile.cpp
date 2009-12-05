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

#include "common/stream.h"
#include "common/util.h"

#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

DATFile::Line::Line() {
}

DATFile::Line::Line(const char *cmd, int cmdLen, const char *args) {
	command   = Common::String(cmd, cmdLen);
	arguments = Common::String(args);

	command.trim();
	arguments.trim();
}

DATFile::DATFile(Common::SeekableReadStream &dat) {
	load(dat);
}

DATFile::DATFile(const Resource &dat) {
	load(dat.getStream());
}

DATFile::~DATFile() {
}

bool DATFile::atEnd() const {
	return _pos == _lines.end();
}

void DATFile::load(Common::SeekableReadStream &dat) {
	dat.seek(0);

	// Reading all lines
	while (!dat.err() && !dat.eos()) {
		Common::String line = dat.readLine();
		// Ignore empty lines
		if (line.empty())
			continue;

		// Workaround for CONV0032.TXT
		if (line[0] == '.')
			line.setChar(' ', 0);;

		// Remove comments
		const char *semicolon = strchr(line.c_str(), ';');
		if (semicolon)
			line = Common::String(line.c_str(), line.size() - strlen(semicolon));

		// Remove white space
		line.trim();
		if (line.empty())
			continue;

		// Find the command-argument separator
		const char *equals = strchr(line.c_str(), '=');
		if (!equals) {
			// Workaround for CONV0008.TXT *sigh*
			if (line.matchString("*message*"))
				_lines.push_back(Line(line.c_str(), line.size(), ""));
			continue;
		}

		_lines.push_back(Line(line.c_str(), line.size() - strlen(equals), equals + 1));
	}

	_pos = _lines.begin();
}

bool DATFile::nextLine(const Common::String *&command, const Common::String *&arguments) {
	command   = 0;
	arguments = 0;

	// Reached the end?
	if (_pos == _lines.end())
		return false;

	command   = &_pos->command;
	arguments = &_pos->arguments;

	++_pos;

	return true;
}

void DATFile::next() {
	if (_pos != _lines.end())
		++_pos;
}

void DATFile::previous() {
	if (_pos != _lines.begin())
		--_pos;
}

void DATFile::rewind() {
	_pos = _lines.begin();
}

int DATFile::argCount(const Common::String &arguments) {
	const char *args = arguments.c_str();

	int count = 1;
	while (*args) {
		if (*args == ' ') {
			// Found separating space

			// Ignore all spaces
			while (*args == ' ')
				args++;

			if (*args)
				// Not line end => New argument
				count++;

		} else
			args++;
	}

	return count;
}

Common::String DATFile::argGet(const Common::String &arguments, int n) {
	const char *start = arguments.c_str();
	for (int i = 0; i < n; i++) {
		// Looking for the next separator
		start = strchr(start, ' ');
		if (!start)
			// None found, argument n doesn't exist
			return "";

		// Skip consecutive spaces
		while (*start == ' ')
			start++;
	}

	// Look for the end of the argument
	const char *end = strchr(start, ' ');
	if (!end)
		end = start + strlen(start) - 1;

	Common::String string(start, end - start);
	if (string.lastChar() == ',')
		// Strip trailing ,
		string.deleteLastChar();

	return string;
}

Common::Array<Common::String> DATFile::argGet(const Common::String &arguments) {
	Common::Array<Common::String> list;

	const char *start = arguments.c_str();
	if (!start)
		return list;

	// Skip leading spaces
	while (*start == ' ')
		start++;

	// Look for the first separator
	const char *end  = strchr(start, ' ');

	int count = argCount(arguments);

	list.reserve(count);

	while (end) {
		Common::String string(start, end - start);
		if (string.lastChar() == ',')
			// Strip trailing ,
			string.deleteLastChar();

		list.push_back(string);

		start = end + 1;

		// Skip consecutive spaces
		while (*start == ' ')
			start++;

		// Look for the next separator
		end = strchr(start, ' ');
	}

	Common::String string(start);
	if (string.lastChar() == ',')
		// Strip trailing ,
		string.deleteLastChar();
	list.push_back(string);

	return list;
}

Common::String DATFile::mergeArgs(const Common::Array<Common::String> &args, uint32 n) {
	Common::String str;

	// Merge args with spaces inbetween
	for (uint32 i = n; i < args.size(); i++)
		str += args[i] + " ";

	// Trim the last (unecessary) space
	str.trim();

	return str;
}

} // End of namespace DarkSeed2
