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

#ifndef DARKSEED2_NECURSORS_H
#define DARKSEED2_NECURSORS_H

#include "common/str.h"
#include "common/stream.h"
#include "common/array.h"
#include "common/list.h"
#include "common/file.h"

#include "engines/darkseed2/darkseed2.h"

namespace DarkSeed2 {

/** A New Executable cursor. */
class NECursor {
public:
	NECursor();
	~NECursor();

	/** Return the cursor's width. */
	uint16 getWidth() const;
	/** Return the cursor's height . */
	uint16 getHeight() const;
	/** Return the cursor's hotspot's x coordinate. */
	uint16 getHotspotX() const;
	/** Return the cursor's hotspot's y coordinate.. */
	uint16 getHotspotY() const;

	/** Return the cursor's data. */
	const byte *getData() const;
	/** Return the cursor's data in a stream. */
	Common::SeekableReadStream &getStream() const;

	/** Set the cursor's dimensions. */
	void setDimensions(uint16 width, uint16 height);
	/** Set the cursor's hotspot. */
	void setHotspot(uint16 x, uint16 y);

	/** Read the cursor's data out of a stream. */
	bool readData(Common::ReadStream &stream, uint32 count);
	/** Set the cursor's data. */
	void setData(byte *data, uint32 size);

private:
	bool _needFree; ///< Do we need to free the data an destruction?

	uint16 _width;    ///< The cursor's width.
	uint16 _height;   ///< The cursor's height.
	uint16 _hotspotX; ///< The cursor's hotspot's x coordinate.
	uint16 _hotspotY; ///< The cursor's hotspot's y coordinate.

	uint32 _dataSize; ///< The size of the cursor's data.
	byte  *_data;     ///< The cursor's data.

	/** A stream holding the cursor's data. */
	Common::MemoryReadStream *_stream;

	/** Clear the cursor. */
	void clear();
};

/** A New Executable cursor group. */
struct NECursorGroup {
	Common::String name;             ///< The group's name.
	Common::Array<NECursor> cursors; ///< The cursors.
};

/** A class able to load resources from a New Executable. */
class NEResources {
public:
	NEResources();
	~NEResources();

	/** Clear all information. */
	void clear();

	/** Load from an EXE file. */
	bool loadFromEXE(const Common::String &fileName);

	/** Get all cursor's read from the New Executable. */
	const Common::Array<NECursorGroup> &getCursors() const;

private:
	/** An ID Type. */
	enum IDType {
		kIDTypeNumerical, ///< A numerical ID.
		kIDTypeString     ///< A string ID.
	};

	/** A resource. */
	struct Resource {
		IDType idType; ///< The type of the ID.

		Common::String name; ///< The resource's string ID.
		uint32 id;           ///< The resource's numerical ID.

		uint16 type; ///< Type of the resource.

		uint32 offset; ///< Offset within the EXE.
		uint32 size;   ///< Size of the data.

		uint16 flags;
		uint16 handle;
		uint16 usage;
	};

	Common::String _fileName; ///< Current file name.
	Common::File _exe;        ///< Current file.

	/** All resources. */
	Common::List<Resource> _resources;

	/** All cursor resources. */
	Common::Array<NECursorGroup> _cursors;

	/** Try to open the assigned file. */
	bool tryOpen();
	/** Close the assigned file. */
	void close();

	/** Read the offset to the resource table. */
	uint32 getResourceTableOffset();
	/** Read the resource table. */
	bool readResourceTable(uint32 offset);

	// Cursor reading helpers
	bool readCursors();
	bool readCursorGroup(NECursorGroup &group, const Resource &resource);
	bool readCursor(NECursor &cursor, const Resource &resource, uint32 size);

	/** Find a specific resource. */
	const Resource *findResource(uint16 type, uint16 id) const;

	/** Read a resource string. */
	static Common::String getResourceString(Common::SeekableReadStream &exe, uint32 offset);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_NECURSORS_H
