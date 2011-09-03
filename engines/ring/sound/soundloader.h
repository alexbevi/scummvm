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

#ifndef RING_SOUNDLOADER_H
#define RING_SOUNDLOADER_H

#include "ring/shared.h"

namespace Ring {

class CompressedSoundStream;
class CompressedStream;
class SoundEntryStream;

class SoundResource {
public:
	SoundResource();
	~SoundResource();
};

struct CompressedSoundHeader {
	int32 field_0;
	int32 field_4;
	int32 field_8;
	int32 field_C0;
	int32 field_10;
	int32 field_14;
	int32 field_16;
	int32 field_18;
	int32 field_1C;
	int32 field_20;
	int32 field_22;
	int32 field_24;
	int32 field_28;
};

// Sound decompression classes
class CompressedSound {
public:
	struct Data {
		byte *data;
		int32 field_4;
		byte field_8;
	};

	virtual ~CompressedSound() {};

	bool init(const Common::String &path);
	virtual bool decompressHeader() = 0;
	virtual bool decompress(Data *data) = 0;
	uint32 getDataSize() { return _dataSize; }
	virtual bool getChunk() = 0;
	int getSamplesPerSec() { return _samplesPerSec; }
	int16 getBitsPerSample();
	int16 getType() { return _type; }
	int getBlockAlign() { return _blockAlign; }

protected:
	CompressedStream            *_stream;
	byte                        *_buffer;
	int32                        _field_C;
	int32                        _field_10;
	SoundResource               *_resource;
	// Original stores the whole wave header, we only keep the interesting bits
	uint16                       _type;
	int                          _samplesPerSec;
	int                          _blockAlign;
	byte                         _flags;
	uint32                       _chunkCount;
	uint32                       _dataSize;
};

class CompressedSoundMono : public CompressedSound {
public:
	CompressedSoundMono();
	~CompressedSoundMono();

	virtual bool decompressHeader();
	virtual bool decompress(Data *data);
	virtual bool getChunk();

private:
	int16 _field_2C;
};

class CompressedSoundStereo : public CompressedSound {
public:
	CompressedSoundStereo();
	~CompressedSoundStereo();

	virtual bool decompressHeader();
	virtual bool decompress(Data *data);
	virtual bool getChunk();

private:
	int32 _field_2C;
	int32 _field_30;
};

class SoundLoader {
public:
	SoundLoader(SoundFormat format);
	~SoundLoader();

	bool load(const Common::String &path, SoundEntryStream *soundEntry);
	void close();
	bool getChunk();
	uint32 getDataSize();
	bool read(uint32 size, byte *buffer, uint32 *offset);

	// Accessors
	uint16 getType() { return _type; }
	uint16 getSamplesPerSec() { return _samplesPerSec; }
	uint16 getBlockAlign() { return _blockAlign; }

private:
	uint16 _type;
	uint16 _channels;
	uint32 _samplesPerSec;
	uint32 _avgBytesPerSec;
	uint16 _blockAlign;
	uint16 _bitsPerSample;
	//uint16 _field_10;
	CompressedSound *_compressedStream;
	uint32 _field_16;
	//uint32 _field_1A;
	SoundFormat _format;
};

} // End of namespace Ring

#endif // RING_SOUNDLOADER_H
