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

#ifndef LIATH_RESOURCE_H
#define LIATH_RESOURCE_H

#include "liath/shared.h"

#include "common/hashmap.h"
#include "common/hash-str.h"
#include "common/str.h"

namespace Liath {

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	// Archives
	void readPathFile();
	void readMultiData();

	// Data
	Common::Array<FileData *> *readData(Common::String name, uint32 index);



private:
	enum MediaType {
		kMediaCd,
		kMediaHd
	};

	struct FilePath {
		MediaType      type;
		Common::String archive;
		Common::String directory;
		int16 indicator;

		Common::String toString() {
			return Common::String::printf("FilePath:  type=%d, archive=%s, directory=%s, indicator=%d", type, archive.c_str(), directory.c_str(), indicator);
		}
	};

	typedef Common::HashMap<Common::String, FilePath *> Files;

	Files _files;
};

} // End of namespace Liath

#endif // LIATH_RESOURCE_H
