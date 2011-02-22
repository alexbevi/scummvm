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

#include "ring/base/stream.h"

#include "ring/base/application.h"
#include "ring/base/art.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/archive.h"

namespace Ring {

CompressedStream::CompressedStream() : _fileStream(NULL), _artStream(NULL), _memoryStream(NULL), _buffer(NULL) {
	_field_8 = 0;
	_field_C = 0;
	memset(&_field_10, 0, sizeof(_field_10));
	_type = 1;
	_field_211 = 0;
}

CompressedStream::~CompressedStream() {
	SAFE_DELETE(_fileStream);
	SAFE_DELETE(_artStream);
	SAFE_DELETE(_memoryStream);
	// Buffer is disposed as part of _memoryStream
}

bool CompressedStream::init(Common::String filename, uint32 type, uint32 size) {
	_fileStream = SearchMan.createReadStreamForMember(filename);
	if (!_fileStream) {
		warning("[CompressedStream::init] Error opening file (%s)", filename.c_str());
		return false;
	}

	initDecompression();

	return true;
}

bool CompressedStream::initArt(Common::String filename, Zone zone, LoadFrom loadFrom) {
	_artStream = getApp()->getArtHandler()->get(filename, zone, loadFrom);
	if (!_artStream) {
		warning("[CompressedStream::initArt] Error opening file (%s)", filename.c_str());
		return false;
	}

	initDecompression();

	return true;
}

#pragma region Decompression

Common::SeekableReadStream *CompressedStream::getCompressedStream() {
	if (_fileStream)
		return _fileStream;

	if (_artStream)
		return _artStream;

	return NULL;
}

void CompressedStream::initDecompression() {
	_field_8 = 0;
	_field_C = 0;
	memset(&_field_10, 0, sizeof(_field_10));
	_field_211 = 0;
}


void CompressedStream::decompressChuncks(uint32 chuncks, uint32 size) {
	Common::SeekableReadStream *stream = getCompressedStream();
	if (!stream)
		error("[CompressedStream::decompressChuncks] Invalid stream!");

	// Rewind stream to data start
	stream->seek(8, SEEK_SET);

	// Initialize buffer
	if (_buffer)
		free(_buffer);

	_buffer = (byte *)malloc(size);
	memset(_buffer, 0, size);
	byte *pBuffer = _buffer;

	uint32 decompSize = 0;
	for (uint32 i = 0; i < chuncks; i++) {
		uint32 chunkSize = stream->readUint32LE();
		uint32 chunkDataSize = stream->readUint32LE();

		decompSize += decompress(stream, 16, 6, 8 * stream->pos(), 8 * (stream->pos() + chunkSize), pBuffer);

		pBuffer += chunkDataSize;
	}

	// Check decompressed size
	if (decompSize > size)
		warning("[CompressedStream::decompressChuncks] Error during decompression (buffer overrun)!");

	_memoryStream = new Common::MemoryReadStream(_buffer, size, DisposeAfterUse::YES);
}

uint32 CompressedStream::decompress(Common::SeekableReadStream *stream, uint32 a2, uint32 a3, uint32 start, uint32 end, byte* buffer) {
	warning("[CompressedStream::decompress] Not implemented!");

	return 0;
}

#pragma endregion

#pragma region ReadStream

bool CompressedStream::eos() const {
	if (!_memoryStream)
		error("[CompressedStream::eos] Not initialized properly!");

	return _memoryStream->eos();
}

uint32 CompressedStream::read(void *dataPtr, uint32 dataSize) {
	if (!_memoryStream)
		error("[CompressedStream::read] Not initialized properly!");

	return _memoryStream->read(dataPtr, dataSize);
}

#pragma endregion

#pragma region SeekableReadStream

int32 CompressedStream::pos() const {
	if (!_memoryStream)
		error("[CompressedStream::pos] Not initialized properly!");

	return _memoryStream->pos();
}

int32 CompressedStream::size() const {
	if (!_memoryStream)
		error("[CompressedStream::size] Not initialized properly!");

	return _memoryStream->size();
}

bool CompressedStream::seek(int32 offset, int whence) {
	if (!_memoryStream)
		error("[CompressedStream::seek] Not initialized properly!");

	return _memoryStream->seek(offset, whence);
}

#pragma endregion

} // End of namespace Ring
