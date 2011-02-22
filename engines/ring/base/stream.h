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

#ifndef RING_STREAM_H
#define RING_STREAM_H

#include "ring/shared.h"

#include "common/memstream.h"
#include "common/stream.h"

namespace Ring {

class CompressedStream : public Common::SeekableReadStream {
public:
	CompressedStream();
	~CompressedStream();

	bool init(Common::String filename, uint32 a2, uint32 a3);
	bool initArt(Common::String filename, Zone zone, LoadFrom loadFrom);

	// ReadStream
	virtual bool eos() const;
	virtual uint32 read(void *dataPtr, uint32 dataSize);

	// SeekableReadStream
	virtual int32 pos() const;
	virtual int32 size() const;
	virtual bool seek(int32 offset, int whence = SEEK_SET);

private:
	Common::SeekableReadStream *_fileStream;    ///< The file stream
	Common::SeekableReadStream *_artStream;     ///< The art stream
	Common::MemoryReadStream   *_memoryStream;  ///< Memory data buffer stream

	void *_buffer;  ///< The buffer to hold decompressed data
};

} // End of namespace Ring

#endif // RING_STREAM_H
