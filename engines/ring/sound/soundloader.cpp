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

#include "ring/sound/soundloader.h"

#include "ring/base/stream.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region SoundResource

SoundResource::SoundResource() {

}

SoundResource::~SoundResource() {

}

#pragma endregion

#pragma region CompressedSoundMono

CompressedSoundMono::CompressedSoundMono() {
	_stream = NULL;
	_buffer = NULL;
	_field_C = 0;
	_field_10 = 0;
	_resource = NULL;
	_dataSize = 0;
}

CompressedSoundMono::~CompressedSoundMono() {
	SAFE_DELETE(_stream);
	SAFE_FREE(_buffer);
	_field_C = 0;
	SAFE_DELETE(_resource);
}

bool CompressedSoundMono::init(const Common::String &path) {
	_stream = new CompressedStream();
	_stream->initBuffer(path, 2);

	_resource = new SoundResource();

	Header header;
	if (!decompressHeader(&header))
		return false;

	return true;
}

bool CompressedSoundMono::decompressHeader(Header *header) {
	error("[CompressedSoundMono::decompressHeader] Not implemented");
};

bool CompressedSoundMono::decompress(Data *data) {
	error("[CompressedSoundMono::decompress] Not implemented");
};

bool CompressedSoundMono::getChunk() {
	_stream->getCompressedStream()->seek(52, SEEK_SET);
	_header.field_2C = _stream->getCompressedStream()->readUint16LE();

	_field_C  = 0;
	_field_10 = 44;

	return true;
}

#pragma endregion

#pragma region CompressedSoundStereo

CompressedSoundStereo::CompressedSoundStereo() {
	_stream = NULL;
	_buffer = NULL;
	_field_C = 0;
	_field_10 = 0;
	_resource = NULL;
	_dataSize = 0;
}

CompressedSoundStereo::~CompressedSoundStereo() {
	SAFE_DELETE(_stream);
	SAFE_FREE(_buffer);
	_field_C = 0;
	SAFE_DELETE(_resource);
}

bool CompressedSoundStereo::init(const Common::String &path) {
	_stream = new CompressedStream();
	_stream->initBuffer(path, 2);

	_resource = new SoundResource();

	Header header;
	if (!decompressHeader(&header))
		return false;

	return true;
}

bool CompressedSoundStereo::decompressHeader(Header *header) {
	error("[CompressedSoundStereo::decompressHeader] Not implemented");
};

bool CompressedSoundStereo::decompress(Data *data) {
	error("[CompressedSoundStereo::decompress] Not implemented");
};

bool CompressedSoundStereo::getChunk() {
	_stream->getCompressedStream()->seek(52, SEEK_SET);
	_header.field_2C = _stream->getCompressedStream()->readUint32LE();
	_header.field_30 = _stream->getCompressedStream()->readUint32LE();

	_field_C  = 0;
	_field_10 = 44;

	return true;
}

#pragma endregion

#pragma region SoundLoader

SoundLoader::SoundLoader(SoundFormat format) {
	_field_4 = 0;
	_field_8 = 0;
	_field_C = 0;
	_field_10 = 0;
	_field_14 = 0;
	_compressedStream = NULL;
	_field_1A = 0;
	_field_1E = 0;
	_format = format;
}

SoundLoader::~SoundLoader() {
	SAFE_DELETE(_compressedStream);
}

bool SoundLoader::load(const Common::String &path, SoundEntryStream *soundEntry) {
	_field_1A = 0;

	switch (_format) {
	default:
		error("[SoundLoader::load] Invalid sound format (%d)", _format);

	case kSoundFormatWAC:
		_compressedStream = new CompressedSoundMono();
		break;

	case kSoundFormatWAS:
		_compressedStream = new CompressedSoundStereo();
		break;
	}

	if (!_compressedStream->init(path))
		return true;

	// Initialize loader data
	error("[SoundLoader::load] Not implemented");

	return false;
}

void SoundLoader::close() {
	SAFE_DELETE(_compressedStream);
}

bool SoundLoader::getChunk() {
	_compressedStream->getChunk();
	_field_1A = 0;

	return false;
}

uint32 SoundLoader::getDataSize() {
	return _compressedStream->getDataSize();
}

bool SoundLoader::read(uint32 size, byte *buffer, uint32 *offset) {
	error("[SoundLoader::read] Not implemented");
}

#pragma endregion

} // End of namespace Ring
