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

#ifndef INNOCENT_MAPFILE_H
#define INNOCENT_MAPFILE_H

#include "common/stream.h"
#include "common/str.h"

#include "innocent/datafile.h"

namespace Innocent {

class Resources;

class MapFile : public Datafile {
public:
	MapFile(const char *name) : Datafile(0), _filename(name) {}
	const char *filename() const { return _filename.c_str(); }
	void readFile(Common::SeekableReadStream &stream);

	uint32 offsetOfEntry(uint16 index);

private:
	byte _data[1200];
	uint16 _imgCount;
	Common::String _filename;
};

} // End of namespace Innocent

#endif // INNOCENT_MAPFILE_H
