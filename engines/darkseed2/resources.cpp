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

#include "common/file.h"

#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Resources::Resource::Resource() : glue(0), offset(0), size(0) {
}

Resources::Resources() {
	clear();
}

Resources::~Resources() {
	clear();
}

bool Resources::index(const char *fileName) {
	clear();

	Common::File indexFile;

	if (!indexFile.open(fileName))
		return false;

	_glueCount = indexFile.readUint16LE();
	_resCount  = indexFile.readUint16LE();

	_glues.resize(_glueCount);

	byte buffer[33];

	warning("Number of glues: %d, number of resources: %d", _glueCount, _resCount);

	for (int i = 0; i < _glueCount; i++) {
		indexFile.read(buffer, 32);
		indexFile.skip(32);

		buffer[32] = '\0';

		_glues[i].fileName = (const char *) buffer;
	}

	for (int i = 0; i < _resCount; i++) {
		uint16 glue = indexFile.readUint16LE();

		indexFile.read(buffer, 12);
		buffer[12] = '\0';

		Common::String resFile = (const char *) buffer;

		if (glue >= _glueCount) {
			warning("Glue number out of range for resource \"%s\" (%d vs. %d)",
					resFile.c_str(), glue, _glueCount);
			return false;
		}

		Resource resource;

		indexFile.read(resource.unknown, 8);

		resource.glue = &_glues[glue];

		_resources.setVal(resFile, resource);
	}

	return true;
}

void Resources::clear() {
	_resources.clear();
	_glues.clear();

	_glueCount = 0;
	_resCount  = 0;
}

} // End of namespace DarkSeed2
