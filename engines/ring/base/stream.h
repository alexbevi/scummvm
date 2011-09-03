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

#ifndef RING_STREAM_H
#define RING_STREAM_H

#include "ring/shared.h"

#include "common/memstream.h"
#include "common/stream.h"

namespace Ring {

class CompressedStream {
public:
	CompressedStream();
	~CompressedStream();

	bool init(Common::String filename, uint32 type, uint32 size = 0);
	bool initArt(Common::String filename, ZoneId zone, LoadFrom loadFrom);

	Common::SeekableReadStream *getCompressedStream();

	// Decompression functions
	Common::MemoryReadStream *decompressChuncks(uint32 chuncks, uint32 size);
	Common::MemoryReadStream *decompressIndexed(uint32 blockSize, uint32 seqSize, uint32 seqDataSize, uint32 coreSize, uint32 coreDataSize, uint32 size, uint32 indexEnd, uint32 field_C, uint16 field_10);
	Common::MemoryReadStream *decompressNode();
	Common::MemoryReadStream *decompressChannel();

private:
	Common::SeekableReadStream *_fileStream;    ///< The file stream
	Common::SeekableReadStream *_artStream;     ///< The art stream

	byte   _decBuffer[512];
	byte   _type;

	void initDecompression();
	uint32 decodeNode(Common::SeekableReadStream *stream, uint32 start, uint32 end, byte *buffer);
	uint32 decodeChannel(Common::SeekableReadStream *stream, uint32 start, uint32 end, byte *buffer);
	uint32 decodeSound(Common::SeekableReadStream *stream, uint32 a2, uint32 a3, uint32 start, uint32 end, byte *buffer);
	uint32 decode(Common::SeekableReadStream *stream, uint32 a2, uint32 a3, uint32 start, uint32 end, byte *buffer, uint32 multiplier = 1);
};

} // End of namespace Ring

#endif // RING_STREAM_H
