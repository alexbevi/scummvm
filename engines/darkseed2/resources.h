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

#ifndef DARKSEED2_RESOURCES_H
#define DARKSEED2_RESOURCES_H

#include "engines/darkseed2/darkseed2.h"

#include "common/str.h"
#include "common/array.h"
#include "common/hashmap.h"

namespace Common {
	class File;
}

namespace DarkSeed2 {

/** The resource manager. */
class Resources {
public:
	Resources();
	~Resources();

	/** Index all resources, based on a resource index file. */
	bool index(const char *fileName);

	/** Clear all resource information. */
	void clear();

	bool hasResource(const Common::String &resource) const;

	byte *getResource(const Common::String &resource) const;

private:
	struct Glue {
		Common::String fileName;
	};

	/** Information about a resource. */
	struct Resource {
		Glue *glue;      ///< Pointer to its glue file.
		uint32 offset;   ///< Offset within the glue file.
		uint32 size;     ///< Size in bytes.
		byte unknown[8];

		bool exists; ///< Have we found it while indexing its glue file?

		Resource();
	};

	uint16 _glueCount; ///< Number of indexed glue files.
	uint16 _resCount;  ///< Number of indexed resources.

	/** All indexed glues. */
	Common::Array<Glue> _glues;
	/** All indexed resources. */
	Common::HashMap<Common::String, Resource> _resources;

	/** Read the index file's header. */
	bool readIndexHeader(Common::File &indexFile);
	/** Read the glue file section of the index file. */
	bool readIndexGlues(Common::File &indexFile);
	/** Read the resources section of the index file. */
	bool readIndexResources(Common::File &indexFile);

	/** Index all resources in all indexed glue files. */
	bool indexGluesContents();

	/** Index all resources in the specified glue file. */
	bool readGlueContents(Common::File &glueFile, const Common::String &fileName);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_RESOURCES_H
