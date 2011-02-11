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

#ifndef RING_ART_H
#define RING_ART_H

#include "ring/base/object.h"

#include "ring/shared.h"

#include "common/archive.h"

namespace Ring {

class Application;

class Art : public Common::Archive {
public:
	Art();
	~Art();

	void init(const Common::String &path, Zone zone, LoadFrom loadFrom);

	// Archive
	bool hasFile(const Common::String &name);
	int listMembers(Common::ArchiveMemberList &list);
	Common::ArchiveMemberPtr getMember(const Common::String &name);
	Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const;

	// Accessors
	LoadFrom getLoadFrom() { return _loadFrom; }
	Zone getZone() { return _zone; }

private:
	// Art header
	struct Header {
		char format[8];
		uint32 field_8;
		uint32 count;
		uint32 field_10;
		uint32 field_14;
		uint32 field_18;
		uint32 field_1C;

		Header() {
			memset(&format, 0, sizeof(format));
			field_8 = 0;
			count = 0;
			field_10 = 0;
			field_14 = 0;
			field_18 = 0;
			field_1C = 0;
		}
	};

	// Record entry
	struct Record {
		uint32 offset;          ///< Offset
		uint32 size;            ///< Size
		uint32 field_FB;        ///< ???
	};

	typedef Common::HashMap<Common::String, Record> RecordMap;

	RecordMap _records;             ///< List of files
	Common::String _path;           ///< Path of the archive
	Header _header;
	Zone _zone;                     ///< The game zone
	LoadFrom _loadFrom;
};

class ArtHandler : public ObjectHandler {
public:
	ArtHandler(Application *application);
	~ArtHandler();

	void open(Zone zone, LoadFrom loadFrom);

	/**
	 * Removes all art entries that are not loaded from disk (ie. were on the CD in the original)
	 */
	void remove();

	Common::SeekableReadStream *get(Common::String filename, Zone zone, LoadFrom loadFrom);

private:
	Application *_app;

	Common::Array<Art *> _arts;

	bool isPresent(Zone zone, LoadFrom loadFrom);
	int32 getIndex(Zone zone, LoadFrom loadFrom);
};

} // End of namespace Ring

#endif // RING_ART_H
