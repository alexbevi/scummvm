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

struct CompressedSoundMonoHeader : public CompressedSoundHeader {
	int16 field_2C;
};

struct CompressedSoundStereoHeader : public CompressedSoundHeader {
	int32 field_2C;
	int32 field_30;
};

// Sound decompression classes
class CompressedSound {
public:
	struct Header {
		CompressedSoundHeader* header;
		int32 field_4;
		byte field_8;
	};

	struct Data {
		byte *data;
		int32 field_4;
		byte field_8;
	};

	virtual ~CompressedSound() {};

	virtual bool init(const Common::String &path) = 0;
	virtual bool decompressHeader(Header *header) = 0;
	virtual bool decompress(Data *data) = 0;
	virtual uint32 getDataSize() = 0;
	virtual bool getChunk() = 0;
	virtual int getFunction8() = 0;
	virtual int16 getFunction9() = 0;
	virtual int16 getFunction10() = 0;
	virtual CompressedSoundHeader *getHeader() = 0;

protected:
	CompressedStream            *_stream;
	byte                        *_buffer;
	int32                        _field_C;
	int32                        _field_10;
	SoundResource               *_resource;
};

class CompressedSoundMono : public CompressedSound {
public:
	CompressedSoundMono();
	~CompressedSoundMono();

	virtual bool init(const Common::String &path);
	virtual bool decompressHeader(Header *header);
	virtual bool decompress(Data *data);
	virtual uint32 getDataSize() { return _dataSize; }
	virtual bool getChunk();
	virtual int getFunction8() { return _header.field_18; }
	virtual int16 getFunction9() { return _header.field_22; }
	virtual int16 getFunction10() { return _header.field_14; }
	virtual CompressedSoundHeader *getHeader() { return &_header; }

private:
	CompressedSoundMonoHeader  _header;
	int32                      _field_46;
	uint32                     _dataSize;
};

class CompressedSoundStereo : public CompressedSound {
public:
	CompressedSoundStereo();
	~CompressedSoundStereo();

	virtual bool init(const Common::String &path);
	virtual bool decompressHeader(Header *header);
	virtual bool decompress(Data *data);
	virtual uint32 getDataSize() { return _dataSize; }
	virtual bool getChunk();
	virtual int getFunction8() { return _header.field_18; }
	virtual int16 getFunction9() { return _header.field_22; }
	virtual int16 getFunction10() { return _header.field_14; }
	virtual CompressedSoundHeader *getHeader() { return &_header; }

private:
	CompressedSoundStereoHeader  _header;
	int32                        _field_4C;
	uint32                       _dataSize;
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

private:
	uint32 _field_4;
	uint32 _field_8;
	uint32 _field_C;
	uint32 _field_10;
	uint16 _field_14;
	CompressedSound *_compressedStream;
	uint32 _field_1A;
	uint32 _field_1E;
	SoundFormat _format;
};

} // End of namespace Ring

#endif // RING_SOUNDLOADER_H
