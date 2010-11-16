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
#include "common/file.h"

#include "common/archive.h"

#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Archive::Archive() {
	_isIndexed = false;
}

GlueArchive::GlueArchive() : Archive() {
	_file = 0;
}

GlueArchive::~GlueArchive() {
	delete _file;
}

bool GlueArchive::open(const Common::String &fileName, Archive *parentArchive) {
	_fileName = fileName;
	return Common::File::exists(fileName);
}

void GlueArchive::index(ResourceMap &map) {
	if (_isIndexed)
		return;

	if (!_file) {
		// Open up the file if we have not done so already
		Common::File *file = new Common::File();
		assert(file->open(_fileName));
		_file = file;

		if (isCompressed())
			uncompressGlue();
	}

	debugC(3, kDebugResources, "Reading contents of glue file \"%s\"", _fileName.c_str());

	_file->seek(0);

	uint16 glueResCount = _file->readUint16LE();
	_resources.resize(glueResCount);

	debugC(4, kDebugResources, "Has %d resources", glueResCount);

	for (uint16 i = 0; i < glueResCount; i++) {
		// Resource's file name
		byte buffer[13];
		_file->read(buffer, 12);
		buffer[12] = '\0';
		Common::String resFile = (const char *)buffer;

		// Was the resource also listed in the index file?
		if (!map.contains(resFile)) {
			warning("GlueArchive::index(): "
					"Unindexed resource \"%s\" found", resFile.c_str());
			_file->skip(8);
			continue;
		}

		// Just to make sure that the resource is the really in the glue file it should be
		assert(map[resFile] == this);

		_resources[i].fileName = resFile;
		_resources[i].size = _file->readUint32LE();
		_resources[i].offset = _file->readUint32LE();

		debugC(5, kDebugResources, "Resource \"%s\", offset %d, size %d",
				resFile.c_str(), _resources[i].offset, _resources[i].size);
	}

	_isIndexed = true;
}

Common::SeekableReadStream *GlueArchive::getStream(const Common::String &fileName) {
	if (!_file)
		return 0;

	for (uint32 i = 0; i < _resources.size(); i++) {
		if (_resources[i].fileName.equalsIgnoreCase(fileName)) {
			_file->seek(_resources[i].offset);
			return _file->readStream(_resources[i].size);
		}
	}

	return 0;
}

void GlueArchive::clearUncompressedData() {
	delete _file;
	_file = 0;
	_isIndexed = false;
	_resources.clear();
}

void GlueArchive::uncompressGlue() {
	if (!_file->seek(0))
		error("GlueArchive::uncompressGlue(): Can't seek glue file");

	byte inBuf[2048];

	memset(inBuf, 0, 2048);

	int nRead = _file->read(inBuf, 2048);

	if (nRead != 2048)
		error("GlueArchive::uncompressGlue(): "
				"Can't uncompress glue file: Need at least 2048 bytes");

	uint32 size = READ_LE_UINT32(inBuf + 2044) + 128;

	// Sanity check
	assert(size < (10*1024*1024));

	byte *outBuf = new byte[size];

	memset(outBuf, 0, size);

	byte *oBuf = outBuf;
	while (nRead != 0) {
		uint32 toRead = 2040;
		uint32 written;

		if (nRead != 2048)
			// Round up to the next 17 byte block
			toRead = ((nRead + 16) / 17) * 17;

		// Decompress that chunk
		written = uncompressGlueChunk(oBuf, inBuf, toRead);

		oBuf += written;

		memset(inBuf, 0, 2048);
		nRead = _file->read(inBuf, 2048);
	}

	delete _file;
	_file = new Common::MemoryReadStream(outBuf, size, DisposeAfterUse::YES);
}

uint32 GlueArchive::uncompressGlueChunk(byte *outBuf, const byte *inBuf, int n) const {
	int countRead    = 0;
	int countWritten = 0;

	uint16 mask;
	int32 offset;
	uint32 count;

	mask = 0xFF00 | *inBuf++;

	while (1) {
		if (mask & 1) {
			// Direct copy

			mask >>= 1;

			*outBuf++ = *inBuf++;
			*outBuf++ = *inBuf++;

			countWritten += 2;

		} else {
			// Copy from previous output

			mask >>= 1;

			count = READ_LE_UINT16(inBuf);
			inBuf += 2;

			offset = (count >> 4)  + 1;
			count  = (count & 0xF) + 3;

			for (int i = 0; i < 8; i++)
				outBuf[i] = outBuf[-offset + i];

			if (count > 8)
				for (int i = 0; i < 10; i++)
					outBuf[i + 8] = outBuf[-offset + 8 + i];

			outBuf += count;
			countWritten += count;
		}

		if ((mask & 0xFF00) == 0) {
			countRead += 17;
			if (countRead >= n)
				break;

			mask = 0xFF00 | *inBuf++;
		}
	}

	return countWritten;
}

bool GlueArchive::isCompressed() {
	_file->seek(0);

	uint32 fSize = _file->size();

	uint32 numRes = _file->readUint16LE();

	// The resource list has to fit
	if (fSize <= (numRes * 22))
		return true;

	byte buffer[12];
	while (numRes-- > 0) {
		_file->read(buffer, 12);

		// Only these character are allowed in a resource file name
		for (int i = 0; (i < 12) && (buffer[i] != 0); i++)
			if (!isalnum(buffer[i]) && (buffer[i] != '.') && (buffer[i] != '_'))
				return true;

		uint32 size   = _file->readUint32LE();
		uint32 offset = _file->readUint32LE();

		// The resources have to fit
		if ((size + offset) > fSize)
			return true;
	}

	return false;
}

PGFArchive::~PGFArchive() {
	for (uint32 i = 0; i < _subArchives.size(); i++)
		delete _subArchives[i];
}

bool PGFArchive::open(const Common::String &fileName, Archive *parentArchive) {
	_fileName = fileName;
	return Common::File::exists(fileName);
}

void PGFArchive::index(ResourceMap &map) {
	if (_isIndexed)
		return;

	if (!_file.isOpen())
		assert(_file.open(_fileName));

	debugC(3, kDebugResources, "Reading contents of PGF file \"%s\"", _fileName.c_str());

	_file.seek(0);

	uint32 resCount = _file.readUint32BE();
	_resources.resize(resCount);

	debugC(4, kDebugResources, "Has %d resources", resCount);

	uint32 startOffset = resCount * (12 + 4 + 4) + 4;

	for (uint32 i = 0; i < resCount; i++) {
		byte buffer[13];

		// Resource's file name
		_file.read(buffer, 12);
		buffer[12] = '\0';
		Common::String resFile = (const char *)buffer;

		map[resFile] = this;

		_resources[i].fileName = resFile;
		_resources[i].size = _file.readUint32BE();
		_resources[i].offset = _file.readUint32BE() + startOffset;

		debugC(5, kDebugResources, "Resource \"%s\", offset %d, size %d",
				resFile.c_str(), _resources[i].offset, _resources[i].size);
	}

	// Now to index all TND files
	for (uint32 i = 0; i < _resources.size(); i++) {
		if (_resources[i].fileName.hasSuffix(".TND")) {
			Archive *archive = new TNDArchive();
			_subArchives.push_back(archive);

			if (!archive->open(_resources[i].fileName, this))
				error("Could not index TND");

			archive->index(map);
		}
	}

	_file.close();
	_isIndexed = true;
}

Common::SeekableReadStream *PGFArchive::getStream(const Common::String &fileName) {
	for (uint32 i = 0; i < _resources.size(); i++) {
		if (_resources[i].fileName.equalsIgnoreCase(fileName)) {
			if (_resources[i].size == 0)
				return 0;
			if (!_file.isOpen())
				assert(_file.open(_fileName));
			_file.seek(_resources[i].offset);
			Common::SeekableReadStream *stream = _file.readStream(_resources[i].size);
			_file.close();
			return stream;
		}
	}

	return 0;
}

TNDArchive::TNDArchive() : Archive() {
	_file = 0;
}

TNDArchive::~TNDArchive() {
	delete _file;
}

bool TNDArchive::open(const Common::String &fileName, Archive *parentArchive) {
	if (!parentArchive)
		return false;

	_file = parentArchive->getStream(fileName);
	if (!_file)
		return true;

	_fileName = fileName;

	return _file->readUint32BE() == (uint32)_file->size();
}

void TNDArchive::index(ResourceMap &map) {
	if (!_file || _isIndexed)
		return;

	uint32 txtCount = _file->readUint32BE();
	uint32 startOffset = txtCount * 16 + 8;
	_resources.resize(txtCount);

	for (uint32 i = 0; i < txtCount; i++) {
		byte buffer[9];

		_file->read(buffer, 8);
		buffer[8] = '\0';
		Common::String txtFile = (const char *)buffer;

		txtFile += ".TXT";

		map[txtFile] = this;

		_resources[i].fileName = txtFile;
		_resources[i].size = _file->readUint32BE();
		_resources[i].offset = _file->readUint32BE() + startOffset;
	}

	_isIndexed = true;
}

Common::SeekableReadStream *TNDArchive::getStream(const Common::String &fileName) {
	if (!_file)
		return 0;

	for (uint32 i = 0; i < _resources.size(); i++) {
		if (_resources[i].fileName.equalsIgnoreCase(fileName)) {
			_file->seek(_resources[i].offset);
			return _file->readStream(_resources[i].size);
		}
	}

	return 0;
}

SaturnGlueArchive::~SaturnGlueArchive() {
	for (uint32 i = 0; i < _subArchives.size(); i++)
		delete _subArchives[i];
}

bool SaturnGlueArchive::open(const Common::String &fileName, Archive *parentArchive) {
	if (!_indexFile.open(fileName + ".IDX"))
		return false;

	if (!_glueFile.open(fileName + ".GLU"))
		return false;

	_fileName = fileName + ".GLU";
	return true;
}

void SaturnGlueArchive::index(ResourceMap &map) {
	if (_isIndexed || !_indexFile.isOpen())
		return;

	debugC(3, kDebugResources, "Reading contents of glue file \"%s\"", _fileName.c_str());

	uint32 glueResCount = _indexFile.readUint32BE();
	_resources.resize(glueResCount);

	debugC(4, kDebugResources, "Has %d resources", glueResCount);

	for (uint32 i = 0; i < glueResCount; i++) {
		byte buffer[13];

		// Resource's file name
		_indexFile.read(buffer, 12);
		buffer[12] = '\0';
		Common::String resFile = (const char *)buffer;

		map[resFile] = this;

		_resources[i].fileName = resFile;
		_resources[i].size     = _indexFile.readUint32BE();
		_resources[i].offset   = _indexFile.readUint32BE();

		debugC(5, kDebugResources, "Resource \"%s\", offset %d, size %d",
				resFile.c_str(), _resources[i].offset, _resources[i].size);
	}

	// Now to index all TND files
	for (uint32 i = 0; i < _resources.size(); i++) {
		if (_resources[i].fileName.hasSuffix(".TND")) {
			Archive *archive = new TNDArchive();
			_subArchives.push_back(archive);

			if (!archive->open(_resources[i].fileName, this))
				error("Could not index TND");

			archive->index(map);
		}
	}

	_isIndexed = true;
	_indexFile.close();
}

Common::SeekableReadStream *SaturnGlueArchive::getStream(const Common::String &fileName) {
	for (uint32 i = 0; i < _resources.size(); i++) {
		if (_resources[i].fileName.equalsIgnoreCase(fileName)) {
			_glueFile.seek(_resources[i].offset);
			return _glueFile.readStream(_resources[i].size);
		}
	}

	return 0;
}

Resources::Resources() {
	clear();
}

Resources::~Resources() {
	clear();
}

void Resources::setGameVersion(GameVersion gameVersion, Common::Language language) {
	_versionFormats.setGameVersion(gameVersion);
	_versionFormats.setLanguage(language);
}

const VersionFormats &Resources::getVersionFormats() {
	return _versionFormats;
}

bool Resources::index(const char *fileName) {
	debugC(1, kDebugResources, "Resource index file \"%s\"", fileName);

	clear();

	Common::File indexFile;

	if (!indexFile.open(fileName))
		return false;

	uint16 resCount = 0;

	// Read the different sections of the index file
	if (!readIndexHeader(indexFile, resCount))
		return false;
	if (!readIndexGlues(indexFile))
		return false;
	if (!readIndexResources(indexFile, resCount))
		return false;

	return true;
}

bool Resources::indexPGF() {
	// Find all PGFs

	Common::ArchiveMemberList pgfs;

	SearchMan.listMatchingMembers(pgfs, "*.PGF");

	uint32 archiveCount = pgfs.size();

	if (archiveCount == 0)
		return false;

	// Create the initial archive
	_archives.resize(archiveCount + 1);
	_archives[0] = new SaturnGlueArchive();

	if (!_archives[0]->open("initial")) {
		warning("Resources::indexPGF(): Can't open initial.[idx,glu] file");
		return false;
	}

	_archives[0]->index(_resources);

	// Indexing all PGFs

	Common::ArchiveMemberList::const_iterator it = pgfs.begin();
	for (uint i = 1; it != pgfs.end(); ++it, ++i) {
		_archives[i] = new PGFArchive();

		if (!_archives[i]->open((*it)->getName()))
			return false;

		_archives[i]->index(_resources);
	}

	return true;
}

// TODO: AFTER REFACTORING
bool Resources::indexMacResources() {
	// TODO: Index resource forks with 'snd ' resources in the sound folder (Voices/Sound Effects)
	// TODO: Index the "action" resource fork with 'Sprt' resources (Animations)
	// TODO: Index the "art" resource fork with 'PICT' resources (Inventory Images)
	// TODO: Index the "music" resource fork with 'Tune' resources (QuickTime MIDI)
	// TODO: Index the "talk" resource fork with 'TEXT' resources (Game Scripts)
	// TODO: Index the "text" file (Subtitles)
	// TODO: Index the "walk" file (Mike Walk Animations)
	// TODO: Index files in the "rooms" folder (Room Images)
	return false;
}

void Resources::clear() {
	for (uint32 i = 0; i < _archives.size(); i++)
		delete _archives[i];

	_resources.clear();
	_archives.clear();
}

void Resources::clearUncompressedData() {
	for (uint32 i = 0; i < _archives.size(); i++)
		_archives[i]->clearUncompressedData();
}

bool Resources::readIndexHeader(Common::File &indexFile, uint16 &resCount) {
	uint16 archiveCount = indexFile.readUint16LE();
	resCount  = indexFile.readUint16LE();

	_archives.resize(archiveCount);

	debugC(1, kDebugResources, "Found %d glues and %d resources", archiveCount, resCount);

	return true;
}

bool Resources::readIndexGlues(Common::File &indexFile) {
	byte buffer[33];

	// Read the names of all available glues
	for (uint32 i = 0; i < _archives.size(); i++) {
		indexFile.read(buffer, 32);
		indexFile.skip(32);

		buffer[32] = '\0';

		Common::String fileName = (const char *)buffer;

		_archives[i] = new GlueArchive();

		if (!_archives[i]->open(fileName)) {
			warning("Could not open Glue file '%s'", fileName.c_str());
			return false;
		}

		debugC(2, kDebugResources, "Glue file \"%s\"", fileName.c_str());
	}

	return true;
}

bool Resources::readIndexResources(Common::File &indexFile, uint16 resCount) {
	byte buffer[13];

	// Read information about all avaiable resources
	for (uint32 i = 0; i < resCount; i++) {
		// In which glue is it found?
		uint16 archive = indexFile.readUint16LE();

		// File name
		indexFile.read(buffer, 12);
		buffer[12] = '\0';
		Common::String resFile = (const char *) buffer;

		if (archive >= _archives.size()) {
			warning("Resources::readIndexResources(): Glue number out "
					"of range for resource \"%s\" (%d vs. %d)",
					resFile.c_str(), archive, _archives.size());
			return false;
		}

		_resources[resFile] = _archives[archive];

		// Unknown
		indexFile.skip(8);

		debugC(3, kDebugResources, "Resource \"%s\", in glue \"%s\"",
				resFile.c_str(), _archives[archive]->getFileName().c_str());
	}

	return true;
}

bool Resources::hasResource(const Common::String &resource) {
	return Common::File::exists(resource) || _resources.contains(resource);
}

Common::SeekableReadStream *Resources::getResource(const Common::String &resource) {
	debugC(3, kDebugResources, "Getting resource \"%s\"", resource.c_str());

	// First try loading directly from the file
	Common::File *plainFile = new Common::File();
	if (plainFile->open(resource))
		return plainFile;

	delete plainFile;

	if (!_resources.contains(resource))
		error("Resources::getResource(): Resource \"%s\" does not exist",
				resource.c_str());

	Archive *archive = _resources[resource];

	if (!archive->isIndexed())
		archive->index(_resources);

	Common::SeekableReadStream *stream = archive->getStream(resource);

	if (!stream)
		error("Resources::getResource(): Could not open resource '%s'", resource.c_str());

	return stream;
}

Common::String Resources::addExtension(const Common::String &name, const Common::String &extension) {
	if (name.empty() || extension.empty())
		return name;

	const char *str = name.c_str();
	const char *dot = strrchr(str, '.');

	if (!dot)
		return name + "." + extension;

	return Common::String(str, dot + 1) + extension;
}

} // End of namespace DarkSeed2
