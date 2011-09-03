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

#include "audio/decoders/wave.h"
#include "audio/decoders/raw.h"

namespace Ring {

#pragma region SoundResource

SoundResource::SoundResource() {
	_buffer  = NULL;
	_field_4 = NULL;
	_size    = 0;
}

SoundResource::~SoundResource() {
	cleanup();
}

void SoundResource::add(void *data, uint32 dataSize) {
	if (data == NULL)
		error("[SoundResource::add] Invalid data pointer");

	if (dataSize == 0)
		error("[SoundResource::add] Invalid data size (cannot be 0)");

	// Check if existing data is present
	if (_size) {

		// Create new buffer to hold the data
		uint32 totalSize = _size + dataSize;
		byte *buffer = (byte *)malloc(totalSize);
		if (!buffer)
			error("[SoundResource::add] Cannot reallocate memory for SoundResource buffer");

		memcpy(buffer, _buffer, _size);
		memcpy((byte*)(buffer + _size), data, dataSize);

		cleanup();

		_buffer = buffer;
		_field_4 = buffer;
		_size = totalSize;

	} else {
		cleanup();

		byte *buffer = (byte *)malloc(dataSize);
		if (!buffer)
			error("[SoundResource::add] Cannot allocate memory for SoundResource buffer");

		memcpy(buffer, data, dataSize);

		_buffer = buffer;
		_field_4 = buffer;
		_size = dataSize;
	}
}

void SoundResource::cleanup() {
	SAFE_FREE(_buffer);
	_field_4 = NULL;
	_size = 0;
}

#pragma endregion

#pragma region CompressedSound

bool CompressedSound::init(const Common::String &path) {
	_stream = new CompressedStream();
	_stream->initBuffer(path, 2);

	_resource = new SoundResource();

	if (!decompressHeader())
		return false;

	return true;
}

int16 CompressedSound::getBitsPerSample() {
	if (_flags & Audio::FLAG_UNSIGNED)
		return 8;

	if (_flags & Audio::FLAG_16BITS) {
		if (_flags & Audio::FLAG_LITTLE_ENDIAN)
			return 16;

		return 4;
	}

	error("[CompressedSound::getBitsPerSample] Invalid set of flags");
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

bool CompressedSoundMono::decompressHeader() {
	if (!_stream)
		error("[CompressedSoundMono::decompressHeader] sound stream not initialized");

	_chunkCount = _stream->getCompressedStream()->readUint32LE();
	_dataSize   = _stream->getCompressedStream()->readUint32LE();

	int size;
	if (!Audio::loadWAVFromStream(*_stream->getCompressedStream(), size, _samplesPerSec, _flags, &_type, &_blockAlign)) {
		warning("[CompressedSoundMono::decompressHeader] Cannot parse WAVE header");
		return false;
	}

	// Setup fields and buffer
	--_chunkCount;

	_field_2C = _stream->getCompressedStream()->readUint16LE();
	_field_C  = 0;
	_field_10 = 44;

	_buffer = (byte *)malloc(1536);
	if (!_buffer)
		error("[CompressedSoundMono::decompressHeader] Cannot allocate sound decoding buffer");

	return true;
};

bool CompressedSoundMono::decompress(Data *data) {
	error("[CompressedSoundMono::decompress] Not implemented");
};

bool CompressedSoundMono::getChunk() {
	_stream->getCompressedStream()->seek(52, SEEK_SET);
	_field_2C = _stream->getCompressedStream()->readUint16LE();

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

bool CompressedSoundStereo::decompressHeader() {
	if (!_stream)
		error("[CompressedSoundStereo::decompressHeader] sound stream not initialized");

	_chunkCount = _stream->getCompressedStream()->readUint32LE();
	_dataSize   = _stream->getCompressedStream()->readUint32LE();

	int size;
	if (!Audio::loadWAVFromStream(*_stream->getCompressedStream(), size, _samplesPerSec, _flags, &_type, &_blockAlign)) {
		warning("[CompressedSoundStereo::decompressHeader] Cannot parse WAVE header");
		return false;
	}

	// Setup fields and buffer
	--_chunkCount;

	_field_C  = 0;
	_field_10 = 44;
	_field_2C = _stream->getCompressedStream()->readUint32LE();
	_field_30 = _stream->getCompressedStream()->readUint32LE();

	// Compute buffer size
	uint32 bufferSize = (getType() != 0 ? 2 : 1) * getSamplesPerSec() * getBitsPerSample();

	_buffer = (byte *)malloc(bufferSize + 1024);
	if (!_buffer)
		error("[CompressedSoundStereo::decompressHeader] Cannot allocate sound decoding buffer");

	return true;
};

bool CompressedSoundStereo::decompress(Data *data) {
	error("[CompressedSoundStereo::decompress] Not implemented");
};

bool CompressedSoundStereo::getChunk() {
	_stream->getCompressedStream()->seek(52, SEEK_SET);
	_field_2C = _stream->getCompressedStream()->readUint32LE();
	_field_30 = _stream->getCompressedStream()->readUint32LE();

	_field_C  = 0;
	_field_10 = 44;

	return true;
}

#pragma endregion

#pragma region SoundLoader

SoundLoader::SoundLoader(SoundFormat format) {
	_type = 0;
	_channels = 0;
	_samplesPerSec = 0;
	_avgBytesPerSec = 0;
	_blockAlign = 0;
	_bitsPerSample = 0;
	//_field_10 = 0;
	_compressedStream = NULL;
	_field_16 = 0;
	//_field_1A = 0;
	_format = format;
}

SoundLoader::~SoundLoader() {
	SAFE_DELETE(_compressedStream);
}

bool SoundLoader::load(const Common::String &path, SoundEntryStream *soundEntry) {
	_field_16 = 0;

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

	// Initialize loader data from wave header
	_type = _compressedStream->getType();
	//_channels = _compressedStream->getChannels();
	_samplesPerSec = _compressedStream->getSamplesPerSec();
	//_avgBytesPerSec = _compressedStream->getAvgBytesPerSec();
	_bitsPerSample = _compressedStream->getBitsPerSample();
	_blockAlign    = _compressedStream->getBlockAlign();
	//_field_10 = ???

	return false;
}

void SoundLoader::close() {
	SAFE_DELETE(_compressedStream);
}

bool SoundLoader::getChunk() {
	_compressedStream->getChunk();
	_field_16 = 0;

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
