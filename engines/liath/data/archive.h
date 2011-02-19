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

#ifndef LIATH_ARCHIVE
#define LIATH_ARCHIVE

#include "common/archive.h"

namespace Liath {

/**
 * 	Multi archives (.mul files)
 */
class MultiArchive : public Common::Archive {
public:
	MultiArchive(const Common::String &path);

	// Archive
	bool hasFile(const Common::String &name);
	int listMembers(Common::ArchiveMemberList &list);
	Common::ArchiveMemberPtr getMember(const Common::String &name);
	Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const;

private:
	// File entry
	struct FileEntry {
		uint32 offset;          ///< Offset
		uint32 size;            ///< Size
	};

	typedef Common::HashMap<Common::String, FileEntry> FileMap;

	FileMap _files;             ///< List of files
	Common::String _filename;   ///< Filename of the archive
};

} // End of namespace Liath

#endif // LIATH_ARCHIVE
