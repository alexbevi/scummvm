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

#ifndef RING_MOVIE_H
#define RING_MOVIE_H

#include "ring/shared.h"

#include "common/stream.h"
#include "common/substream.h"

namespace Ring {

class ImageLoaderCIN;
class Movie;
class ScreenManager;

class Cinematic : public Common::SeekableReadStream  {
public:
	Cinematic();
	~Cinematic();

	bool init(Common::String name);
	void deinit();

	void readFrameHeader();
	bool tControl();
	bool sControl(void* buffer);

	// Sound
	void sub_46A0E0(uint32 a1, uint32 a2, uint32 a3, int32 a4);
	void setVolume(uint32 volume);
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
	void setField8(bool state) { _field_8 = state; }
	void setChannel(uint32 channel) { _channel = channel; }
	void setSoundInitialized(bool state) { _isSoundInitialized = state; }
	void setField53(bool state) { _field_53 = state; }
	void setRemoveDialog(bool state) { _hasDialog = state; }
	void setFramerate(uint32 framerate) { _framerate = framerate; }

	bool isSoundInitialized() { return _isSoundInitialized; }
	bool getField53() { return _field_53; }
	float getFramerate() { return _framerate; }
	bool hasDialog() { return _hasDialog; }
	uint32 getChannel() { return _channel; }

private:
	Common::SeekableReadStream *_stream;    ///< The movie file stream
	// buffer
	// buffer 2
	byte   _field_8;
	// backbufferGlobal
	// backBuffer
	byte   _field_11;
	uint32 _field_12;
	uint32 _field_16;
	uint32 _field_1A;
	// CinTControlKosiGlobal
	// CinTControlKosi
	// cacheBufferGlobal
	// cacheBuffer
	uint32 _field_2E;
	uint32 _field_32;
	uint32 _field_36;
	uint32 _field_3A;
	// event
	bool _isStreaming; // uint32
	uint32 _field_46;
	uint32 _field_4A;
	float  _field_4E;
	bool   _isSoundInitialized; // FIXME remove (our sound mixer is always initialized)
	bool   _field_53;
	float  _framerate;
	bool   _hasDialog;
	uint32 _channel;
};

class Movie {
public:
	Movie(ScreenManager *screen);
	~Movie();

	bool init(Common::String path, Common::String filename, uint32 a3, uint32 channel);
	void deinit();

	void play(const Common::Point &point);

	void setFramerate(float rate) { _cinematic->setFramerate(rate); }

private:
	ScreenManager  *_screen;
	ImageLoaderCIN *_imageCIN;
	Cinematic      *_cinematic;

	bool readSound();
	bool skipSound();
};

} // End of namespace Ring

#endif // RING_MOVIE_H
