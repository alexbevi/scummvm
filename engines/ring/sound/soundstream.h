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

#ifndef RING_SOUNDSTREAM_H
#define RING_SOUNDSTREAM_H

#include "ring/shared.h"

#include "common/stream.h"

namespace Ring {

class CompressedSoundStream : public Common::SeekableReadStream {
public:
	CompressedSoundStream();
	~CompressedSoundStream();

	void initBuffer(const char *path);

	// ReadStream
	virtual bool eos() const;
	virtual uint32 read(void *dataPtr, uint32 dataSize);

	// SeekableReadStream
	virtual int32 pos() const;
	virtual int32 size() const;
	virtual bool seek(int32 offset, int whence = SEEK_SET);

private:
	Common::SeekableReadStream *_stream;
};

} // End of namespace Ring

#endif // RING_SOUNDSTREAM_H
