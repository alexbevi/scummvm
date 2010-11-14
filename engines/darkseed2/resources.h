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

#include "common/util.h"
#include "common/str.h"
#include "common/array.h"
#include "common/hashmap.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/versionformats.h"

namespace Common {
	class File;
	class ReadStream;
	class SeekableReadStream;
	class MemoryReadStream;
}

namespace DarkSeed2 {

/** A resource. */
class Resource {
public:
	/** Use that foreign data as resource memory. */
	Resource(const Common::String &name, const byte *data, uint32 size);
	/** Read resource data from stream. */
	Resource(const Common::String &name, Common::ReadStream &stream, uint32 size);
	~Resource();

	/** Return the resource's name. */
	const Common::String &getName() const;

	/** Size of resource data in bytes. */
	uint32 getSize() const;

	/** Return the resource's data. */
	const byte *getData() const;
	/** Return the resource's data as a stream. */
	Common::SeekableReadStream &getStream() const;

private:
	Common::String _name; ///< The resource's name.

	const byte *_foreignData; ///< Static foreign data.
	      byte *_ownData;     ///< Self-managed data.

	uint32 _size; ///< Size of the data in bytes
	Common::MemoryReadStream *_stream; ///< The stream.
};

/** The resource manager. */
class Resources {
public:
	Resources();
	~Resources();

	/** Index all resources, based on a resource index file. */
	bool index(const char *fileName);

	/** Index all availabe PGF resources. */
	bool indexPGF(const char *initalIndex, const char *initialGlue);

	/** Clear all resource information. */
	void clear();

	/** Remove the file data from unused compressed archives. */
	void clearUncompressedData();

	/** Does a specific resource exist? */
	bool hasResource(const Common::String &resource);

	/** Get a specific resource. */
	Resource *getResource(const Common::String &resource);

	/** Set the specific game version. */
	void setGameVersion(GameVersion gameVersion, Common::Language language);

	/** Get the information class about which formats the game uses. */
	const VersionFormats &getVersionFormats();

	static Common::String addExtension(const Common::String &name, const Common::String &extension);

private:
	/** An archive type. */
	enum ArchiveType {
		kArchiveTypeNone = 0, ///< No valid archive.
		kArchiveTypeGlue,     ///< A glue archive.
		kArchiveTypePGF       ///< A PGF archive.
	};

	/** An archive file. */
	struct Archive {
		ArchiveType type; ///< The archive type.

		Common::String fileName; ///< File name.

		uint32 size; ///< File size.
		byte  *data; ///< File data.

		Common::MemoryReadStream *stream; ///< Stream to the file data.

		bool indexed; ///< Have we indexed that archive yet?

		Archive();
		~Archive();
	};

	/** Information about a resource. */
	struct Res {
		Archive *archive; ///< Pointer to its archive file.

		uint32 offset; ///< Offset within the archive file.
		uint32 size;   ///< Size in bytes.

		byte unknown[8];

		bool indexed; ///< Have we indexed that resource yet?

		bool exists; ///< Have we found it while indexing its resource file?

		Res();
	};

	VersionFormats _versionFormats;

	uint16 _archiveCount; ///< Number of indexed archive files.
	uint32 _resCount;  ///< Number of indexed resources.

	/** All indexed archives. */
	Common::Array<Archive> _archives;
	/** All indexed resources. */
	Common::HashMap<Common::String, Res, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> _resources;

	/** Read the index file's header. */
	bool readIndexHeader(Common::File &indexFile);
	/** Read the glue file section of the index file. */
	bool readIndexGlues(Common::File &indexFile);
	/** Read the resources section of the index file. */
	bool readIndexResources(Common::File &indexFile);

	/** Index the glue where this resource can be found. */
	bool indexParentArchive(Res &res);

	/** Index all resources contained in this archive file. */
	bool indexArchiveContents(Archive &archive);
	/** Index all resources contained in this glue file. */
	bool indexGlueContents(Archive &archive);
	/** Index all resources contained in this PGF file. */
	bool indexPGFContents(Archive &archive);

	/** Index all resources in the specified glue file. */
	bool readGlueContents(Common::SeekableReadStream &glueFile, const Common::String &fileName);
	/** Index all resources in the specified pgf file. */
	bool readPGFContents(Common::SeekableReadStream &pgfFile, Archive &archive);
	/** Index all resources in the specified initial resource pair. */
	bool readInitialIndexContents(Common::SeekableReadStream &IndexFile, Archive &archive);

	/** Read a resource list entry with big endian values. */
	bool readBEResourcList(Common::SeekableReadStream &file, Archive &archive, uint32 startOffset);

	bool indexTND(Common::SeekableReadStream &file, Archive &archive, Res &tnd);

	/** Uncompress a glue file. */
	byte *uncompressGlue(Common::File &file, uint32 &size) const;
	/** Uncompress a compress glue file chunk. */
	uint32 uncompressGlueChunk(byte *outBuf, const byte *inBuf, int n) const;

	/** Simple heuristic to guess whether a glue file is compressed. */
	static bool isCompressedGlue(Common::SeekableReadStream &stream);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_RESOURCES_H
