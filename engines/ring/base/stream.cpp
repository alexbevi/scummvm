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
	memset(&_decBuffer, 0, sizeof(_decBuffer));
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
	memset(&_decBuffer, 0, sizeof(_decBuffer));
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

	_buffer = (byte *)malloc(size + 1024);
	memset(_buffer, 0, size + 1024);
	byte *pBuffer = _buffer;

	uint32 streamPosition = 8;
	uint32 decompSize = 0;
	for (uint32 i = 0; i < chuncks; i++) {
		uint32 chunkSize = stream->readUint32LE();
		uint32 chunkDataSize = stream->readUint32LE();

		decompSize += decompress(stream, 16, 6, stream->pos() * 8, (stream->pos() + chunkSize) * 8, pBuffer);

		// Advance stream and buffer
		streamPosition += chunkSize + 8;
		stream->seek(streamPosition, SEEK_SET);
		pBuffer += chunkDataSize;
	}

	// Check decompressed size
	if (decompSize > size + 64)
		warning("[CompressedStream::decompressChuncks] Error during decompression (buffer overrun)!");

	_memoryStream = new Common::MemoryReadStream(_buffer, size, DisposeAfterUse::YES);
}

uint32 CompressedStream::decompress(Common::SeekableReadStream *stream, uint32 a2, uint32 a3, uint32 start, uint32 end, byte* buffer) {
	// Reset decompression buffer
	memset(&_decBuffer, 0, sizeof(_decBuffer));

#define CHECK_BIT(var,pos) !!((var) & (1<<(pos)))

	// Store buffer position
	byte *bufferStart = buffer;

	uint16 index0 = 0;
	uint16 index1 = 0;

	while (start < end) {
		stream->seek(start >> 3, SEEK_SET);

		uint32 var = stream->readUint32BE();
		uint32 pos = 31 - (start & 7);

		// Check bit of var at pos
		if (CHECK_BIT(var, pos)) {
			// Check previous bit
			pos -= 1;

			if (CHECK_BIT(var, pos)) {
				start += 2;

				*(uint16 *)buffer = *(uint16 *)(buffer - 2);
				buffer += 2;

				*(uint32 *)&_decBuffer[8 * index0 + 2] = start;

				if (index1 == index0) {
					uint32 minVal = *(uint32 *)&_decBuffer[2];

					for (int i = 1; i < 64; i++) {
						uint32 current = *(uint32 *)&_decBuffer[i * 8 + 2];

						if (current < minVal) {
							minVal = current;
							index1 = i;
						}
					}
				}
			} else {
				uint32 decoded = (var << (32 - pos)) >> (32 - a3);

				*(uint16 *)buffer = *(uint16 *)&_decBuffer[8 * decoded];
				buffer += 2;

				start += a3 + 2;

				index0 = decoded;
				*(uint32 *)&_decBuffer[8 * decoded + 2] = start;

				if (index1 == index0) {
					uint32 minVal = *(uint32 *)&_decBuffer[2];

					for (int i = 1; i < 64; i++) {
						uint32 current = *(uint32 *)&_decBuffer[i * 8 + 2];

						if (current < minVal) {
							minVal = current;
							index1 = i;
						}
					}
				}
			}

		} else {
			uint32 decoded = (uint32)(var << (32 - pos)) >> (32 - a2);

			*(uint16 *)buffer = decoded;
			buffer += 2;

			// Store decoded value
			index0 = index1;
			*(uint16 *)&_decBuffer[8 * index1] = decoded;

			// Advance buffer start pointer
			start += a2 + 1;
		}
	}

	return buffer - bufferStart;
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
