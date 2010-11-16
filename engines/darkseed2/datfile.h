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

#ifndef DARKSEED2_DATFILE_H
#define DARKSEED2_DATFILE_H

#include "common/str.h"
#include "common/list.h"
#include "common/array.h"

#include "engines/darkseed2/darkseed2.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Resource;

class DATFile {
public:
	DATFile(const Common::String &fileName, Common::SeekableReadStream &stream);
	~DATFile();

	/** Reached the end? */
	bool atEnd() const;

	/** Get the next line. */
	bool nextLine(const Common::String *&command, const Common::String *&arguments);

	/** Skip the current line. */
	void next();
	/** Back off one line. */
	void previous();
	/** Seek to the beginning. */
	void rewind();
	/** Seek to a specific line. */
	void seekTo(uint32 n);

	/** Get the current file name. */
	const Common::String &getName() const;
	/** Get the current line number. */
	uint32 getLineNumber() const;

	/** Get the signature ("file:line"). */
	Common::String getSignature() const;

	/** Get the number of arguments in the string. */
	static int argCount(const Common::String &arguments);
	/** Get the nth argument out of the string. */
	static Common::String argGet(const Common::String &arguments, int n);
	/** Split the string into separate argument strings. */
	static Common::Array<Common::String> argGet(const Common::String &arguments);

	/** Split the argument string into separate argument integers. */
	static Common::Array<int32> argGetInts(const Common::String &arguments, int n = -1, int def = 0);

	/** Merge arguments back together to a string. */
	static Common::String mergeArgs(const Common::Array<Common::String> &args, uint32 n = 0);

private:
	/** A DATFile line. */
	struct Line {
		Common::String command;   ///< The command.
		Common::String arguments; ///< The arguments.

		Line();
		Line(const char *cmd, int cmdLen, const char *args);
	};

	/** The file's name. */
	Common::String _name;

	/** All lines. */
	Common::List<Line> _lines;
	/** The current line. */
	Common::List<Line>::const_iterator _pos;
	/** The current line's number. */
	uint32 _lineNumber;

	/** Load from a stream. */
	void load(Common::SeekableReadStream &dat);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_DATFILE_H
