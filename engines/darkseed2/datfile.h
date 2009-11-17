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

#include "engines/darkseed2/darkseed2.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Resource;

class DATFile {
public:
	DATFile(Common::SeekableReadStream &dat);
	DATFile(const Resource &dat);
	~DATFile();

	/** Reached the end? */
	bool atEnd() const;

	bool nextLine(const Common::String *&command, const Common::String *&arguments);

	/** Skip the current line. */
	void next();
	/** Back off one line. */
	void previous();
	/** Seek to the beginning. */
	void rewind();

private:
	struct Line {
		Common::String command;
		Common::String arguments;

		Line();
		Line(const char *cmd, int cmdLen, const char *args);
	};

	Common::List<Line> _lines;
	Common::List<Line>::const_iterator _pos;

	void load(Common::SeekableReadStream &dat);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_DATFILE_H
