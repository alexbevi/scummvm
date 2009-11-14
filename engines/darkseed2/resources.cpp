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

Resources::Glue::Glue() : data(0), stream(0) {
}

Resources::Glue::~Glue() {
	delete stream;
	delete[] data;
}

Resources::Resource::Resource() : glue(0), offset(0), size(0), exists(false) {
}

Resources::Resources() {
	clear();

	Common::File a;
	Common::DumpFile b;
	uint32 s;

	a.open("gl00_txt.000");

	byte *d = uncompressGlue(a, s);

	b.open("gl00_txt.000");

	b.write(d, s);

	b.flush();
	b.close();
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

	// Index all glues
	if (!indexGluesContents())
		return false;

	return true;
}

void Resources::clear() {
	_resources.clear();
	_glues.clear();

	_glueCount = 0;
	_resCount  = 0;
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
			warning("Glue number out of range for resource \"%s\" (%d vs. %d)",
					resFile.c_str(), glue, _glueCount);
			return false;
		}

		Resource resource;

		// Unknown
		indexFile.read(resource.unknown, 8);

		resource.glue = &_glues[glue];

		_resources.setVal(resFile, resource);

		debugC(3, kDebugResources, "Resource \"%s\", in glue \"%s\"",
				resFile.c_str(), resource.glue->fileName.c_str());
	}

	return true;
}

bool Resources::indexGluesContents() {
	for (int i = 0; i < _glueCount; i++) {
		Common::File glueFile;

		if (!glueFile.open(_glues[i].fileName)) {
			warning("Can't open glue file \"%s\"", _glues[i].fileName.c_str());
			return false;
		}

		if (isCompressedGlue(glueFile)) {
			// If the glue is compress, uncompress it and keep it in memory

			uint32 dataSize;

			_glues[i].data   = uncompressGlue(glueFile, dataSize);
			_glues[i].stream = new Common::MemoryReadStream(_glues[i].data, dataSize);

			if (!readGlueContents(*_glues[i].stream, _glues[i].fileName))
				return false;

		} else
			if (!readGlueContents(glueFile, _glues[i].fileName))
				return false;
	}

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
			warning("Unindexed resource \"%s\" found", resFile.c_str());
			glueFile.skip(8);
			continue;
		}

		Resource &resource = _resources.getVal(resFile);

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

bool Resources::hasResource(const Common::String &resource) const {
	if (!_resources.contains(resource))
		return false;

	const Resource &res = _resources.getVal(resource);

	return res.exists && (res.size != 0);
}

byte *Resources::getResource(const Common::String &resource) const {
	debugC(3, kDebugResources, "Getting resource \"%s\"", resource.c_str());

	if (!_resources.contains(resource))
		error("Resource \"%s\" does not exist", resource.c_str());

	const Resource &res = _resources.getVal(resource);

	if (!res.exists || (res.size == 0))
		error("Resource \"%s\" not available", resource.c_str());

	Common::File glueFile;
	Common::SeekableReadStream *stream;

	if (res.glue->stream) {
		// Was a compressed glue, now held in memory

		stream = res.glue->stream;
	} else {
		// Uncompressed glue, reading from file

		if (!glueFile.open(res.glue->fileName))
			error("Couldn't open glue file \"%s\"", res.glue->fileName.c_str());

		stream = &glueFile;
	}

	if (!stream->seek(res.offset))
		error("Couldn't seek glue file \"%s\" to offset %d",
				res.glue->fileName.c_str(), res.offset);

	byte *resData = new byte[res.size];

	if (stream->read(resData, res.size) != res.size) {
		delete[] resData;
		error("Couldn't read resource \"%s\" out of glue file \"%s\"",
				resource.c_str(), res.glue->fileName.c_str());
	}

	glueFile.close();

	return resData;
}

byte *Resources::uncompressGlue(Common::File &file, uint32 &size) const {
	if (!file.seek(0))
		error("Can't seek glue file");

	byte inBuf[2048];
	int nRead;

	memset(inBuf, 0, 2048);

	nRead = file.read(inBuf, 2048);

	if (nRead != 2048)
		error("Can't uncompress glue file: Need at least 2048 bytes");

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

			memmove(outBuf, outBuf - offset, 8);

			if (count > 8)
				memmove(outBuf + 8, outBuf - offset + 8, 10);

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

	uint16 numRes = stream.readUint16LE();

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
