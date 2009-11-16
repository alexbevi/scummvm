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

	while (!dat.err() && !dat.eos()) {
		Common::String line = dat.readLine();
		if (line.empty())
			continue;

		// Pure comment line, ignore
		if (line[0] == ';')
			continue;

		// Rip out comments
		Common::StringTokenizer uncommentor(line, ";");
		Common::String lineData = uncommentor.nextToken();
		if (lineData.empty())
			continue;

		// Separate by ' ' and '='
		Common::StringTokenizer parter(lineData, " =");
		Common::String firstItem = parter.nextToken();
		// Lines without any real information
		if (firstItem.empty() || (firstItem[0] == '-'))
			continue;

		parter.reset();

		Common::List<Common::String> items;
		while(!parter.empty()) {
			Common::String item = parter.nextToken();
			items.push_back(item);
		}
		_lines.push_back(items);

	}

	_pos = _lines.begin();
}

const Common::List<Common::String> *DATFile::getNextLine() {
	if (_pos == _lines.end())
		return 0;

	return &*_pos++;
}

void DATFile::rewind() {
	_pos = _lines.begin();
}

} // End of namespace DarkSeed2
