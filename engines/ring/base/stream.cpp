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

#include "ring/helpers.h"

namespace Ring {

CompressedStream::CompressedStream() : _fileStream(NULL), _artStream(NULL), _memoryStream(NULL), _buffer(NULL) {}

CompressedStream::~CompressedStream() {
	SAFE_DELETE(_fileStream);
	SAFE_DELETE(_artStream);
	SAFE_DELETE(_memoryStream);
	SAFE_DELETE(_buffer);
}

bool CompressedStream::init(Common::String filename, uint32 a2, uint32 a3) {
	return false;
}

bool CompressedStream::initArt(Common::String filename, Zone zone, LoadFrom loadFrom) {
	return false;
}

// ReadStream
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

// SeekableReadStream
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

} // End of namespace Ring
