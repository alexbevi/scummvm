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

#ifndef RING_MOVIE_H
#define RING_MOVIE_H

#include "ring/shared.h"

#include "common/rect.h"
#include "common/stream.h"
#include "common/substream.h"

namespace Ring {

class ImageLoaderCIN;
class Movie;
class ScreenManager;

class Cinematic : public Common::SeekableReadStream {
public:
	Cinematic();
	~Cinematic();

	bool init(Common::String name);
	void deinit();

	void skipFrame();
	bool tControl();
	bool sControl(byte* buffer);

	// Sound
	void sub_46A0E0(uint32 a1, uint32 a2, uint32 a3, int32 a4);
	void setVolume(int32 volume);
	void sub_46A4B0();
	void setSoundBuffer(Common::SeekableReadStream *stream, uint32 offset);

	// ReadStream
	virtual bool eos() const;
	virtual uint32 read(void *dataPtr, uint32 dataSize);

	// SeekableReadStream
	virtual int32 pos() const;
	virtual int32 size() const;
	virtual bool seek(int32 offset, int whence = SEEK_SET);

	// Accessors
	void setField10(bool state) { _field_10 = state; }

private:
	struct FrameHeader {
		uint32 size;
		uint32 field_4;
		uint32 field_8;
		uint32 field_C;
		uint32 field_10;

		FrameHeader() {
			size     = 0;
			field_4  = 0;
			field_8  = 0;
			field_C  = 0;
			field_10 = 0;
		}

		void read(Common::SeekableReadStream *stream) {
			size     = stream->readUint32LE();
			field_4  = stream->readUint32LE();
			field_8  = stream->readUint32LE();
			field_C  = stream->readUint32LE();
			field_10 = stream->readUint32LE();
		}
	};

	struct TControlHeader {
		uint32 field_0;
		uint32 field_4;
		uint16 field_8;
		uint16 field_A;
		byte   field_C;

		TControlHeader() {
			field_0 = 0;
			field_4 = 0;
			field_8 = 0;
			field_A = 0;
			field_C = 0;
		}

		void load(Common::SeekableReadStream *stream) {
			field_0 = stream->readUint32LE();
			field_4 = stream->readUint32LE();
			field_8 = stream->readUint16LE();
			field_A = stream->readUint16LE();
			field_C = stream->readByte();
		}
	};

	Common::SeekableReadStream *_stream;    ///< The movie file stream
	byte   *_buffer;
	byte   *_buffer2;
	bool    _field_10;
	byte   *_backBuffer;
	TControlHeader _tControlHeader;
	byte   *_tControlBuffer;
	byte   *_cacheBuffer;
	byte   *_controlData;
	uint32  _field_3A;
	uint32  _field_3E;
	uint32  _field_42;
	uint32  _field_46;
	bool    _isStreaming;

	uint32 decompress(uint32 a1, byte* buffer, uint32 a3);
};

class Cinematic2 : public Common::SeekableReadStream {
public:
	Cinematic2();
	~Cinematic2();

	bool init(Common::String filename, ArchiveType type, ZoneId zone, LoadFrom loadFrom);

	bool allocBuffer(size_t bufferSize);

	void skipFrame();
	bool tControl();
	bool sControl(byte* buffer, uint32 bitdepth);

	void setField5404C() { _field_5404C = true; }

	// ReadStream
	virtual bool eos() const;
	virtual uint32 read(void *dataPtr, uint32 dataSize);

	// SeekableReadStream
	virtual int32 pos() const;
	virtual int32 size() const;
	virtual bool seek(int32 offset, int whence = SEEK_SET);

private:
	struct TControlHeader {
		uint32 size;
		uint16 decompressedSize;
		uint16 bufferSize;

		TControlHeader() {
			size             = 0;
			decompressedSize = 0;
			bufferSize       = 0;
		}

		bool read(Common::SeekableReadStream *stream) {
			size             = stream->readUint32LE();
			decompressedSize = stream->readUint16LE();
			bufferSize       = stream->readUint16LE();

			return (!stream->eos() && !stream->err());
		}
	};

	struct FrameHeader {
		uint32 size;
		uint32 offset;
		uint16 decompressedSize;
		uint16 bufferSize;
		uint32 field_C;
		uint32 field_10;
		uint32 field_14;
		uint32 field_18;
		uint32 field_20;
		uint32 field_24;
		uint32 field_28;
		uint16 field_2C;
		byte   field_2E;

		FrameHeader() {
			size             = 0;
			offset           = 0;
			decompressedSize = 0;
			bufferSize       = 0;
			field_C          = 0;
			field_10         = 0;
			field_14         = 0;
			field_18         = 0;
			field_20         = 0;
			field_24         = 0;
			field_28         = 0;
			field_2C         = 0;
			field_2E         = 0;
		}

		bool read(Common::SeekableReadStream *stream) {
			size             = stream->readUint32LE();
			offset           = stream->readUint32LE();
			decompressedSize = stream->readUint16LE();
			bufferSize       = stream->readUint16LE();
			field_C          = stream->readUint32LE();
			field_10         = stream->readUint32LE();
			field_14         = stream->readUint32LE();
			field_18         = stream->readUint32LE();
			field_20         = stream->readUint32LE();
			field_24         = stream->readUint32LE();
			field_28         = stream->readUint32LE();
			field_2C         = stream->readUint16LE();
			field_2E         = stream->readByte();

			return (!stream->eos() && !stream->err());
		}
	};

	Common::SeekableReadStream *_stream;    ///< The movie file stream

	void           *_seqBuffer;
	bool            _field_5404C;
	byte           *_frameBuffer;
	byte           *_tControlBuffer;
	FrameHeader     _frameHeader;
	TControlHeader  _tControlHeader;
	// Original stores a flag to know if the data is streamed or not (movie: true - images: false)

	void decompressTControl(byte *buffer, uint32 bufferSize, uint16 decompressedSize);
	void decompressSeq(byte *buffer);
};

class Movie {
public:
	Movie(ScreenManager *screen);
	~Movie();

	bool init(Common::String path, Common::String filename, uint32 a3, uint32 channel);
	void deinit();

	void play(const Common::Point &point);

	void setFramerate(float rate) { _framerate = rate; }

private:
	ImageLoaderCIN *_imageCIN;
	ScreenManager  *_screen;
	Cinematic      *_cinematic;
	float           _field_56;
	bool            _isSoundInitialized;
	bool            _field_5B;
	float           _framerate;
	bool            _hasDialog;
	uint32          _channel;

	bool readSound();
	bool skipSound();
};

} // End of namespace Ring

#endif // RING_MOVIE_H
