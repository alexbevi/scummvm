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

#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Resource::Resource(const Common::String &name, const byte *data, uint32 size) {
	_name        = name;
	_ownData     = 0;
	_foreignData = data;
	_size        = size;

	_stream = new Common::MemoryReadStream(_foreignData, size);
}

Resource::Resource(const Common::String &name, Common::ReadStream &stream, uint32 size) {
	_name        = name;
	_foreignData = 0;
	_ownData     = new byte[size];
	_size        = size;

	uint32 nRead;

	nRead = stream.read(_ownData, size);

	assert(nRead == size);

	_stream = new Common::MemoryReadStream(_ownData, size);
}

Resource::~Resource() {
	delete[] _ownData;
	delete _stream;
}

const Common::String &Resource::getName() const {
	return _name;
}

uint32 Resource::getSize() const {
	return _size;
}

const byte *Resource::getData() const {
	if (_ownData)
		return _ownData;

	return _foreignData;
}

Common::SeekableReadStream &Resource::getStream() const {
	return *_stream;
}


Resources::Glue::Glue() : data(0), stream(0), size(0), indexed(false) {
}

Resources::Glue::~Glue() {
	delete stream;
	delete[] data;
}


Resources::Res::Res() : glue(0), offset(0), size(0), exists(false), indexed(false) {
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

	debugC(1, kDebugResources, "Resource index file \"%s\"", fileName);

	if (!indexFile.open(fileName))
		return false;

	// Read the different sections of the index file
	if (!readIndexHeader(indexFile))
		return false;
	if (!readIndexGlues(indexFile))
		return false;
	if (!readIndexResources(indexFile))
		return false;

	indexFile.close();

	return true;
}

void Resources::clear() {
	_resources.clear();
	_glues.clear();

	_glueCount = 0;
	_resCount  = 0;
}

void Resources::clearUncompressedData() {
	for (int i = 0; i < _glueCount; i++) {
		Glue &glue = _glues[i];

		delete glue.stream;
		delete[] glue.data;

		glue.stream = 0;
		glue.data   = 0;
	}
}

bool Resources::readIndexHeader(Common::File &indexFile) {
	_glueCount = indexFile.readUint16LE();
	_resCount  = indexFile.readUint16LE();

	_glues.resize(_glueCount);

	debugC(1, kDebugResources, "Found %d glues and %d resources", _glueCount, _resCount);

	return true;
}

bool Resources::readIndexGlues(Common::File &indexFile) {
	byte buffer[33];

	// Read the names of all available glues
	for (int i = 0; i < _glueCount; i++) {
		indexFile.read(buffer, 32);
		indexFile.skip(32);

		buffer[32] = '\0';

		_glues[i].fileName = (const char *) buffer;

		debugC(2, kDebugResources, "Glue file \"%s\"", _glues[i].fileName.c_str());
	}

	return true;
}

bool Resources::readIndexResources(Common::File &indexFile) {
	byte buffer[13];

	// Read information about all avaiable resources
	for (int i = 0; i < _resCount; i++) {
		// In which glue is it found?
		uint16 glue = indexFile.readUint16LE();

		// File name
		indexFile.read(buffer, 12);
		buffer[12] = '\0';
		Common::String resFile = (const char *) buffer;

		if (glue >= _glueCount) {
			warning("Resources::readIndexResources(): Glue number out "
					"of range for resource \"%s\" (%d vs. %d)",
					resFile.c_str(), glue, _glueCount);
			return false;
		}

		Res resource;

		// Unknown
		indexFile.read(resource.unknown, 8);

		resource.glue = &_glues[glue];

		_resources.setVal(resFile, resource);

		debugC(3, kDebugResources, "Resource \"%s\", in glue \"%s\"",
				resFile.c_str(), resource.glue->fileName.c_str());
	}

	return true;
}

bool Resources::indexGlueContents(Glue &glue) {
	if (glue.data || glue.stream)
		// Already indexed
		return true;

	Common::File glueFile;

	if (!glueFile.open(glue.fileName)) {
		warning("Resources::indexGlueContents(): "
				"Can't open glue file \"%s\"", glue.fileName.c_str());
		return false;
	}

	if (isCompressedGlue(glueFile)) {
		// If the glue is compressed, uncompress it and keep it in memory

		glue.data   = uncompressGlue(glueFile, glue.size);
		glue.stream = new Common::MemoryReadStream(glue.data, glue.size);

		if (!glue.indexed)
			if (!readGlueContents(*glue.stream, glue.fileName))
				return false;

	} else
		if (!glue.indexed)
			if (!readGlueContents(glueFile, glue.fileName))
				return false;

	glue.indexed = true;

	return true;
}

bool Resources::readGlueContents(Common::SeekableReadStream &glueFile, const Common::String &fileName) {
	debugC(3, kDebugResources, "Reading contents of glue file \"%s\"", fileName.c_str());

	glueFile.seek(0);

	uint16 glueResCount = glueFile.readUint16LE();

	debugC(4, kDebugResources, "Has %d resources", glueResCount);

	byte buffer[13];

	for (int i = 0; i < glueResCount; i++) {
		// Resource's file name
		glueFile.read(buffer, 12);
		buffer[12] = '\0';
		Common::String resFile = (const char *) buffer;

		// Was the resource also listed in the index file?
		if (!_resources.contains(resFile)) {
			warning("Resources::readGlueContents(): "
					"Unindexed resource \"%s\" found", resFile.c_str());
			glueFile.skip(8);
			continue;
		}

		Res &resource = _resources.getVal(resFile);

		// Just to make sure that the resource is the really in the glue file it should be
		assert(!strcmp(fileName.c_str(), resource.glue->fileName.c_str()));

		resource.exists = true;
		resource.size   = glueFile.readUint32LE();
		resource.offset = glueFile.readUint32LE();

		debugC(5, kDebugResources, "Resource \"%s\", offset %d, size %d",
				resFile.c_str(), resource.offset, resource.size);
	}

	return true;
}

bool Resources::indexParentGlue(Res &res) {
	if (res.indexed)
		return true;

	if (!res.glue) {
		warning("Resources::indexParentGlue(): Resource has no parent glue");
		return false;
	}

	if (!indexGlueContents(*res.glue))
		return false;

	if (res.indexed) {
		warning("Resources::indexParentGlue(): Resource not in parent glue");
		return false;
	}

	return true;
}

bool Resources::hasResource(const Common::String &resource) {
	if (Common::File::exists(resource))
		return true;

	if (!_resources.contains(resource))
		return false;

	Res &res = _resources.getVal(resource);

	if (!indexParentGlue(res))
		return false;

	return res.exists && (res.size != 0);
}

Resource *Resources::getResource(const Common::String &resource) {
	debugC(3, kDebugResources, "Getting resource \"%s\"", resource.c_str());

	Common::File plainFile;
	if (plainFile.open(resource)) {
		// Loading directly from file

		return new Resource(resource, plainFile, plainFile.size());
	}

	if (!_resources.contains(resource))
		error("Resources::getResource(): Resource \"%s\" does not exist",
				resource.c_str());

	Res &res = _resources.getVal(resource);

	if (!indexParentGlue(res))
		return false;

	if (!res.exists || (res.size == 0))
		error("Resources::getResource(): Resource \"%s\" not available",
				resource.c_str());

	if (res.glue->data) {
		// Compressed glue, constructing new resource with direct data held in memory

		if ((res.offset + res.size) > res.glue->size)
			error("Resources::getResource(): Resource \"%s\" offset %d out of bounds",
					resource.c_str(), res.offset);

		return new Resource(resource, res.glue->data + res.offset, res.size);
	}

	// Uncompressed glue, constructing new resource with data from file

	Common::File glueFile;

	if (!glueFile.open(res.glue->fileName))
		error("Resources::getResource(): Couldn't open glue file \"%s\"",
				res.glue->fileName.c_str());

	if (!glueFile.seek(res.offset))
		error("Resources::getResource(): Couldn't seek glue file \"%s\" to offset %d",
				res.glue->fileName.c_str(), res.offset);

	return new Resource(resource, glueFile, res.size);
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

byte *Resources::uncompressGlue(Common::File &file, uint32 &size) const {
	if (!file.seek(0))
		error("Resources::uncompressGlue(): Can't seek glue file");

	byte inBuf[2048];
	int nRead;

	memset(inBuf, 0, 2048);

	nRead = file.read(inBuf, 2048);

	if (nRead != 2048)
		error("Resources::uncompressGlue(): "
				"Can't uncompress glue file: Need at least 2048 bytes");

	size = READ_LE_UINT32(inBuf + 2044) + 128;

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
		nRead = file.read(inBuf, 2048);
	}

	return outBuf;
}

uint32 Resources::uncompressGlueChunk(byte *outBuf, const byte *inBuf, int n) const {
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

bool Resources::isCompressedGlue(Common::SeekableReadStream &stream) {
	stream.seek(0);

	uint32 fSize = stream.size();

	uint32 numRes = stream.readUint16LE();

	// The resource list has to fit
	if (fSize <= (numRes * 22))
		return true;

	byte buffer[12];
	while (numRes-- > 0) {
		stream.read(buffer, 12);

		// Only these character are allowed in a resource file name
		for (int i = 0; (i < 12) && (buffer[i] != 0); i++)
			if (!isalnum(buffer[i]) && (buffer[i] != '.') && (buffer[i] != '_'))
				return true;

		uint32 size   = stream.readUint32LE();
		uint32 offset = stream.readUint32LE();

		// The resources have to fit
		if ((size + offset) > fSize)
			return true;
	}

	return false;
}

} // End of namespace DarkSeed2
