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
 */

#include "ring/base/stream.h"

#include "ring/base/application.h"
#include "ring/base/art.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

CompressedStream::CompressedStream() : _fileStream(NULL), _artStream(NULL) {
	memset(&_decBuffer, 0, sizeof(_decBuffer));
	_type = 1;
}

CompressedStream::~CompressedStream() {
	SAFE_DELETE(_fileStream);
	SAFE_DELETE(_artStream);
}

bool CompressedStream::init(Common::String filename, uint32, uint32) {
	_fileStream = SearchMan.createReadStreamForMember(filename);
	if (!_fileStream) {
		warning("[CompressedStream::init] Error opening file (%s)", filename.c_str());
		return false;
	}

	initDecompression();

	return true;
}

bool CompressedStream::initArt(Common::String filename, ZoneId zone, LoadFrom loadFrom) {
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
	memset(&_decBuffer, 0, sizeof(_decBuffer));
}

Common::MemoryReadStream *CompressedStream::decompressChuncks(uint32 chuncks, uint32 bufferSize) {
	Common::SeekableReadStream *stream = getCompressedStream();
	if (!stream)
		error("[CompressedStream::decompressChuncks] Invalid stream!");

	// Rewind stream to data start
	stream->seek(8, SEEK_SET);

	// Initialize buffer
	byte *buffer = (byte *)malloc(bufferSize + 1024);
	if (!buffer)
		error("[CompressedStream::decompressChuncks] Cannot allocate buffer");

	memset(buffer, 0, bufferSize + 1024);
	byte *pBuffer = buffer;

	uint32 streamPosition = 8;
	uint32 decompSize = 0;
	for (uint32 i = 0; i < chuncks; i++) {
		uint32 chunkSize = stream->readUint32LE();
		uint32 chunkDataSize = stream->readUint32LE();

		decompSize += decode(stream, 16, 6, (uint32)stream->pos() * 8, ((uint32)stream->pos() + chunkSize) * 8, pBuffer);

		// Advance stream and buffer
		streamPosition += chunkSize + 8;
		stream->seek(streamPosition, SEEK_SET);
		pBuffer += chunkDataSize;
	}

	// Check decompressed size
	if (decompSize > (bufferSize + 64))
		warning("[CompressedStream::decompressChuncks] Error during decompression (buffer overrun)!");

	return new Common::MemoryReadStream(buffer, bufferSize, DisposeAfterUse::YES);
}

Common::MemoryReadStream *CompressedStream::decompressIndexed(uint32 blockSize, uint32 seqSize, uint32 seqDataSize, uint32 coreSize, uint32 coreDataSize, uint32 bufferSize, uint32 indexEnd, uint32 field_C, uint16 field_10) {
	Common::SeekableReadStream *stream = getCompressedStream();
	if (!stream)
		error("[CompressedStream::decompressIndexed] Invalid stream!");

	// Initialize buffer
	byte *buffer = (byte *)malloc(bufferSize + 1024);
	if (!buffer)
		error("[CompressedStream::decompressIndexed] Cannot allocate buffer");

	memset(buffer, 0, bufferSize + 1024);
	byte *pBuffer = buffer;

	// Decompress seq
	byte *seqBuffer = (byte *)malloc(seqDataSize + 1024);
	if (!seqBuffer)
		error("[CompressedStream::decompressIndexed] Cannot allocate seq buffer");

	memset(seqBuffer, 0, seqDataSize + 1024);

	uint32 decompressSize = decode(stream, blockSize, 6, 608, 8 * seqSize + 608, seqBuffer);
	if (decompressSize > (seqDataSize + 64))
		warning("[CompressedStream::decompressIndexed] Error during SEQ decompression (buffer overrun)!");

	// Decompress core
	byte *coreBuffer = (byte *)malloc(coreDataSize + 1024);
	if (!coreBuffer)
		error("[CompressedStream::decompressIndexed] Cannot allocate core buffer");

	memset(coreBuffer, 0, coreDataSize + 1024);

	uint32 start = 8 * seqSize + 640;
	decompressSize = decode(stream, 16, 6, start, start + 8 * coreSize, coreBuffer);
	if (decompressSize > (coreDataSize + 64))
		warning("[CompressedStream::decompressIndexed] Error during COR decompression (buffer overrun)!");

	// Store data into buffer
	byte *pSeqBuffer = seqBuffer;
	for (uint32 i = 0; i < seqDataSize; i += 2) {
		uint16 index = READ_UINT16(pSeqBuffer);
		pSeqBuffer += 2;

		WRITE_UINT16(pBuffer,     READ_UINT16(coreBuffer + index * 6));
		WRITE_UINT16(pBuffer + 2, READ_UINT16(coreBuffer + index * 6 + 2));
		WRITE_UINT16(pBuffer + 4, READ_UINT16(coreBuffer + index * 6 + 4));

		pBuffer += 6;
	}

	WRITE_UINT32(buffer + indexEnd, field_C);
	WRITE_UINT16(buffer + indexEnd + 2, field_10);

	// Cleanup buffers
	free(seqBuffer);
	free(coreBuffer);

	// Create decompressed stream
	return new Common::MemoryReadStream(buffer, bufferSize, DisposeAfterUse::YES);
}

#define NODE_HEADER_SIZE 52
Common::MemoryReadStream *CompressedStream::decompressNode() {
	Common::SeekableReadStream *stream = getCompressedStream();
	if (!stream)
		error("[CompressedStream::decompressIndexed] Invalid stream!");

	// Process header
	byte header[NODE_HEADER_SIZE];
	uint32 offset = stream->readUint32LE();

	stream->read(header, NODE_HEADER_SIZE);

	// Get chunk size
	stream->seek(offset, SEEK_CUR);
	uint32 chunkSize = stream->readUint32LE();
	stream->seek(NODE_HEADER_SIZE + 4, SEEK_SET);
	uint32 streamPosition = (uint32)stream->pos();

	// Compute buffer size
	uint32 size = READ_LE_UINT32(header) * READ_LE_UINT32(header + 4) * READ_LE_UINT32(header + 8) / 4;
	uint32 bufferSize = size + 129652;
	if (bufferSize > 10000000) {
		warning("[CompressedStream::decompressNode] Invalid node buffer size (%d)", bufferSize);
		return NULL;
	}

	// Initialize buffer
	byte *buffer = (byte *)malloc(bufferSize + 1024);
	if (!buffer)
		error("[CompressedStream::decompressNode] Cannot allocate buffer");

	memset(buffer, 0, bufferSize + 1024);

	// Copy header
	memcpy(buffer, &header, NODE_HEADER_SIZE);

	// Decode channel and nodes
	uint32 decodedChannelSize = decodeChannel(stream, (uint32)stream->pos() * 8, ((uint32)stream->pos() + offset) * 8, buffer + NODE_HEADER_SIZE);

	// Advance stream and buffer
	streamPosition += offset + 4;
	stream->seek(streamPosition, SEEK_SET);

	uint32 decodedNodeSize = decodeNode(stream, (uint32)stream->pos() * 8, ((uint32)stream->pos() + chunkSize) * 8, buffer + NODE_HEADER_SIZE + size);

	streamPosition += chunkSize;
	stream->seek(streamPosition, SEEK_SET);

	if ((decodedChannelSize + decodedNodeSize) > (bufferSize + 64))
		warning("[CompressedStream::decompressNode] Error during decompression (buffer overrun)!");

	// Create decompressed stream
	return new Common::MemoryReadStream(buffer, bufferSize, DisposeAfterUse::YES);
}

Common::MemoryReadStream *CompressedStream::decompressChannel() {
	error("[CompressedStream::decompressChannel] Not implemented!");


	//// Create decompressed stream
	//return new Common::MemoryReadStream(buffer, bufferSize, DisposeAfterUse::YES);
}

uint32 CompressedStream::decodeSound(Common::SeekableReadStream *stream, uint32 a2, uint32 a3, uint32 start, uint32 end, byte* buffer) {
	return decode(stream, a2, a3, start, end, buffer, 16);
}

uint32 CompressedStream::decodeNode(Common::SeekableReadStream *stream, uint32 start, uint32 end, byte *buffer) {
	return decode(stream, 16, 8, start, end, buffer);
}

uint32 CompressedStream::decodeChannel(Common::SeekableReadStream *stream, uint32 start, uint32 end, byte *buffer) {
	return decode(stream, 13, 8, start, end, buffer);
}

uint32 CompressedStream::decode(Common::SeekableReadStream *stream, uint32 a2, uint32 a3, uint32 start, uint32 end, byte* buffer, uint32 multiplier) {
	// Reset decompression buffer
	memset(&_decBuffer, 0, sizeof(_decBuffer));

#define CHECK_BIT(var, pos) !!((var) & ((uint32)1 << (pos)))

	// Store buffer position
	byte *bufferStart = buffer;

	uint32 index0 = 0;
	uint32 index1 = 0;

	while (start < end) {
		stream->seek(start >> 3, SEEK_SET);

		uint32 var = stream->readUint32BE();
		uint32 position = 31 - (start & 7);

		// Check bit of var at pos
		if (CHECK_BIT(var, position)) {
			// Check previous bit
			position -= 1;

			if (CHECK_BIT(var, position)) {
				start += 2;

				WRITE_UINT16(buffer, READ_UINT16(buffer - 2));
				buffer += 2;

				WRITE_UINT32(&_decBuffer[8 * index0 + 2], start);
			} else {
				uint32 decoded = (var << (32 - position)) >> (32 - a3);

				WRITE_UINT16(buffer, READ_UINT16(&_decBuffer[8 * decoded]));
				buffer += 2;

				start += a3 + 2;

				index0 = decoded;
				WRITE_UINT32(&_decBuffer[8 * decoded + 2], start);
			}

			if (index1 == index0) {
				uint32 minVal = READ_UINT32(&_decBuffer[2]);

				for (uint32 i = 1; i < 64; i++) {
					uint32 current = READ_UINT32(&_decBuffer[i * 8 + 2]);

					if (current < minVal) {
						minVal = current;
						index1 = i;
					}
				}
			}

		} else {
			uint32 decoded = (multiplier * (uint32)(var << (32 - position))) >> (32 - a2);

			WRITE_UINT16(buffer, (uint16)decoded);
			buffer += 2;

			// Store decoded value
			index0 = index1;
			WRITE_UINT16(&_decBuffer[8 * index1], (uint16)decoded);

			// Advance buffer start pointer
			start += a2 + 1;
		}
	}

	return (uint32)(buffer - bufferStart);
}

#pragma endregion

} // End of namespace Ring
