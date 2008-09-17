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

#include "innocent/prog_dat.h"

#include "common/file.h"
#include "common/util.h"

#include "innocent/resources.h"
#include "innocent/main_dat.h"
#include "innocent/program.h"

using namespace Common;

namespace Innocent {

ProgDat::ProgDat(Resources *res) : Datafile(res), _data(0) {}

ProgDat::~ProgDat() {
	if (_data)
		delete[] _data;
}

void ProgDat::load() {
	File *file = new File;

	if (!file->open(filename()))
		error("could not open %s", filename());

	readFile(*file);
	_file.reset(file);
}

void ProgDat::readFile(Common::SeekableReadStream &stream) {
	// actually reads just header
	uint16 total_entries = _resources->mainDat()->progEntriesCount0();
	total_entries += _resources->mainDat()->progEntriesCount1();


	_data = new byte[total_entries * 4];
	(void) stream.read(_data, total_entries * 4);
}

Program *ProgDat::getScript(uint16 id) {
	if (!id) return 0;

	uint32 offset = READ_LE_UINT32(_data + (id - 1) * 4);
	_file->seek(offset);

	return new Program(*_file, id);
}

} // End of namespace Innocent
