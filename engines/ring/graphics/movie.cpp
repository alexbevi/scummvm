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

#include "ring/graphics/movie.h"

#include "ring/base/application.h"
#include "ring/base/dialog.h"
#include "ring/base/preferences.h"
#include "ring/base/sound.h"

#include "ring/graphics/image.h"
#include "ring/graphics/imageLoader.h"
#include "ring/graphics/screen.h"

#include "ring/ring.h"
#include "ring/helpers.h"

#include "common/archive.h"
#include "common/file.h"

namespace Ring {

#define CINEMATIC_BUFFER_SIZE         1024
#define CINEMATIC_BACKBUFFER_SIZE     1200000
#define CINEMATIC_TCONTROLBUFFER_SIZE 16384
#define CINEMATIC_CACHEBUFFER_SIZE    512

#pragma region Cinematic

Cinematic::Cinematic() {
	_stream = NULL;
	_buffer = NULL;
	_buffer2 = NULL;
	_field_10 = false;
	_backBuffer = NULL;
	_tControlBuffer = NULL;
	_cacheBuffer = NULL;
	_tControlData = NULL;
	_field_3A = 0;
	_field_3E = 0;
	_field_42 = 0;
	_field_46 = 0;
	_isStreaming = false;
}

Cinematic::~Cinematic() {
	deinit();
}

bool Cinematic::init(Common::String filename) {
	_stream = SearchMan.createReadStreamForMember(filename);
	if (!_stream) {
		warning("[Cinematic::init] Error opening file (%s)", filename.c_str());
		return false;
	}

	// Create buffers
	_buffer = (byte *)malloc(CINEMATIC_BUFFER_SIZE);
	if (!_buffer)
		error("[Cinematic::init] Error creating main buffer!");

	_buffer2 = _buffer;
	_field_10 = false;

	_backBuffer = (byte *)malloc(CINEMATIC_BACKBUFFER_SIZE);
	if (!_backBuffer)
		error("[Cinematic::init] Error creating back buffer!");

	_tControlBuffer = (byte *)malloc(CINEMATIC_TCONTROLBUFFER_SIZE);
	if (!_tControlBuffer)
		error("[Cinematic::init] Error creating control buffer!");

	_cacheBuffer = (byte *)malloc(CINEMATIC_CACHEBUFFER_SIZE);
	if (!_cacheBuffer)
		error("[Cinematic::init] Error creating cache buffer!");

	_tControlData = NULL;
	_field_3A = 0;
	_field_3E = 0;
	_field_42 = 0;
	_field_46 = 0;

	return true;
}

void Cinematic::deinit() {
	SAFE_DELETE(_stream);

	// Free buffers
	free(_buffer);
	_buffer2 = NULL;

	free(_backBuffer);
	free(_tControlBuffer);
	free(_cacheBuffer);
	free(_tControlData);
	_field_3A = 0;
	_field_3E = 0;
	_field_42 = 0;
	_field_46 = 0;
}

void Cinematic::readFrameHeader() {
	// Reset tControl buffer
	memset(&_tControlBuffer, 0, CINEMATIC_TCONTROLBUFFER_SIZE);

	// Read offset
	uint32 offset = readUint32LE();

	// Skip rest of header
	skip(16);

	// Move to beginning of frame
	seek(offset, SEEK_CUR);
}

bool Cinematic::tControl() {
	// Reset tControl buffer
	memset(&_tControlBuffer, 0, CINEMATIC_TCONTROLBUFFER_SIZE);

	// Read tControl header
	_tControlHeader.load(_stream);

	// Read data
	uint32 size = 2 * _tControlHeader.field_C + 2;
	free(_tControlData);
	_tControlData = (byte *)malloc(size + _tControlHeader.field_0);

	_stream->read(_tControlData, size + _tControlHeader.field_0);

	// Process
	_field_3A = READ_LE_UINT32(_tControlData + size);
	_field_46 = _tControlHeader.field_8;
	_field_42 = READ_LE_UINT32(_tControlData + _tControlHeader.field_0 - _tControlHeader.field_4) + size;
	_field_3E = READ_LE_UINT32(_tControlData + 2 * (_tControlHeader.field_8 * _tControlHeader.field_A) + size);

	// Decompress data
	uint32 decompressedSize = decompress(_field_3E, _backBuffer, _field_42 - _field_3E);
	if (decompressedSize > CINEMATIC_BACKBUFFER_SIZE) {
		warning("[Cinematic::tControl] Back buffer overrun");
		return false;
	}

	return true;
}

bool Cinematic::sControl(void* buffer) {
	error("[Cinematic::sControl] Not implemented!");
}

uint32 Cinematic::decompress(uint32 a1, byte* buffer, uint32 a3) {
	error("[Cinematic::sControl] Not implemented!");
}

#pragma region Sound

void Cinematic::sub_46A0E0(uint32 a1, uint32 a2, uint32 a3, int32 a4) {
	error("[Cinematic::sub_46A0E0] Not implemented!");
}

void Cinematic::setVolume(uint32 volume) {
	error("[Cinematic::setVolume] Not implemented!");
}

void Cinematic::sub_46A4B0() {
	error("[Cinematic::sub_46A4B0] Not implemented!");
}

void Cinematic::setSoundBuffer(Common::SeekableReadStream *stream, uint32 offset) {
	error("[Cinematic::setSoundBuffer] Not implemented!");
}

#pragma endregion

#pragma region ReadStream

bool Cinematic::eos() const {
	if (!_stream)
		error("[Cinematic::eos] Not initialized properly!");

	return _stream->eos();
}

uint32 Cinematic::read(void *dataPtr, uint32 dataSize) {
	if (!_stream)
		error("[Cinematic::read] Not initialized properly!");

	return _stream->read(dataPtr, dataSize);
}

#pragma endregion

#pragma region SeekableReadStream

int32 Cinematic::pos() const {
	if (!_stream)
		error("[Cinematic::pos] Not initialized properly!");

	return _stream->pos();
}

int32 Cinematic::size() const {
	if (!_stream)
		error("[Cinematic::size] Not initialized properly!");

	return _stream->size();
}

bool Cinematic::seek(int32 offset, int whence) {
	if (!_stream)
		error("[Cinematic::seek] Not initialized properly!");

	return _stream->seek(offset, whence);
}

#pragma endregion

#pragma endregion

#pragma region Movie

Movie::Movie(ScreenManager *screen) : _screen(screen) {
	_imageCIN = NULL;
	// bufferGlobal = NULL;

	_cinematic = new Cinematic();
}

Movie::~Movie() {
	deinit();

	SAFE_DELETE(_cinematic);

	// Zero-out passed pointers
	_screen = NULL;
}

bool Movie::init(Common::String path, Common::String filename, uint32 a3, uint32 channel) {
	// Compute and check path
	Common::String filePath = path + '/' + filename;
	if (!Common::File::exists(filePath))
		return false;

	// Initialize movie stream
	_imageCIN = new ImageLoaderCIN();
	if (!_imageCIN->init(filePath))
		error("[Movie::init] Cannot read cinematic frame size");

	// Set channel and sound state
	_channel = channel;
	_isSoundInitialized = true;

	// Setup sound
	_cinematic->sub_46A0E0(_imageCIN->getHeader()->field_8 + 1, _imageCIN->getHeader()->field_9, _imageCIN->getHeader()->field_A, -CINEMATIC_TCONTROLBUFFER_SIZE);
	_cinematic->setVolume(getApp()->getPreferenceHandler()->getVolume());

	// Setup framerate
	_field_5B = true;
	_framerate = 1000.0f / (_imageCIN->getHeader()->field_12 * 0.01f);

	// Setup sound handler
	SoundHandler *soundHandler = getApp()->getSoundHandler();
	if (soundHandler->getField0()) {
		soundHandler->turnOffItems1();

		if (!soundHandler->updateItems(_imageCIN->getHeader()->chunkCount)) {
			soundHandler->turnOffSounds1(true);
			soundHandler->setField0(false);
		}
	}

	// Init dialog
	Common::String dialogPath = Common::String::format("DATA/%s/DIA/%s/%s", getApp()->getCurrentZoneString().c_str(), getApp()->getLanguageFolder().c_str(), filename.c_str());
	if (!Common::File::exists(dialogPath)) {
		_hasDialog = false;
		return true;
	}

	getApp()->getDialogHandler()->addDialog(new Dialog(500001, filename));
	_hasDialog = true;

	return true;
}

void Movie::deinit() {
	SAFE_DELETE(_imageCIN);

	_screen = NULL;
	_field_5B = true;
}

void Movie::play(const Common::Point &point) {
	if (!_cinematic)
		error("[Movie::play] cinematic not initialized properly");

	SoundHandler *soundHandler = getApp()->getSoundHandler();
	ScreenManager *screen = getApp()->getScreenManager();

	// Setup
	Image *image = new Image();
	bool setupSound = true;
	bool readFrame = false;
	uint32 chunkIndex = 0;
	uint32 waitChunk = 0;
	uint32 ticks = g_system->getMillis();

	// Setup header and state
	_cinematic->setField10(false);
	uint32 chunkCount = _imageCIN->getHeader()->chunkCount;

	// Parse cinematic
	if (chunkCount) {
		while (!_cinematic->eos() && !_cinematic->err()) {

			// Interrupt playing on escape
			if (checkEscape()) {
				if (soundHandler->getField0()) {
					soundHandler->updateItems2(chunkCount);
					soundHandler->updateItems3(chunkCount);
				}
				break;
			}

			// Read chunk type
			byte chunkType = _cinematic->readByte();
			if (_cinematic->eos() || _cinematic->err())
				error("[Movie::play] Cannot read chunk type");

			switch (chunkType) {
			default:
				error("[Movie::play] Invalid chunk type (%d)", chunkType);

			case 65:
				switch (_channel) {
				default:
					if (!skipSound())
						error("[Movie::play] Cannot skip sound (index: %d)", chunkIndex);
					break;

				case 2:
					if (!readSound())
						error("[Movie::play] Cannot read sound (index: %d)", chunkIndex);

					if (setupSound) {
						if (_isSoundInitialized)
							_cinematic->sub_46A4B0();

						setupSound = false;
					}
					break;
				}
				break;

			case 66:
				switch (_channel) {
				default:
					if (!skipSound())
						error("[Movie::play] Cannot skip sound (index: %d)", chunkIndex);
					break;

				case 3:
					if (!readSound())
						error("[Movie::play] Cannot read sound (index: %d)", chunkIndex);

					if (setupSound) {
						if (_isSoundInitialized)
							_cinematic->sub_46A4B0();

						setupSound = false;
					}
					break;
				}
				break;

			case 83:
				if (_field_5B) {
					uint32 tickInterval = (g_system->getMillis() - ticks);

					if (((chunkIndex + 1) * _framerate) < tickInterval) {
						if (readFrame) {
							_cinematic->readFrameHeader();

							// Process sound
							if (soundHandler->getField0()) {
								soundHandler->updateItems2(chunkCount);
								soundHandler->updateItems3(chunkCount);
							}

							++chunkIndex;
							waitChunk = chunkIndex;
							break;
						}

						readFrame = true;
						ticks = g_system->getMillis();

					} else {
						if (!readFrame) {
							readFrame = true;
							ticks = g_system->getMillis();
						} else {
							// Wait for tick interval
							while ((waitChunk * _framerate) > (tickInterval + 50))
								checkEvents();
						}
					}
				}

				if (!_imageCIN->readImage(image))
					error("[Movie::play] Error reading image (index: %d)", chunkIndex);

				if (_hasDialog) {
					screen->draw(image, point, kDrawType1);
					getApp()->getDialogHandler()->play();

					screen->updateScreen();
				} else {
					screen->drawAndUpdate(image, point);
				}

				// Process sound
				if (soundHandler->getField0()) {
					soundHandler->updateItems2(chunkCount);
					soundHandler->updateItems3(chunkCount);
				}

				++chunkIndex;
				waitChunk = chunkIndex;
				break;

			case 84:
				if (!_cinematic->tControl())
					error("[Movie::play] Error reading T control (index: %d)", chunkIndex);
				break;

			case 90:
				switch (_channel) {
				default:
					if (!skipSound())
						error("[Movie::play] Cannot skip sound (index: %d)", chunkIndex);
					break;

				case 0:
				case 1:
					if (!readSound())
						error("[Movie::play] Cannot read sound (index: %d)", chunkIndex);

					if (setupSound) {
						if (_isSoundInitialized)
							_cinematic->sub_46A4B0();

						setupSound = false;
					}
					break;
				}
				break;
			}

			// Stop after processing all chunks
			if (chunkIndex >= chunkCount)
				break;
		}
	}

	// Cleanup
	delete image;

	if (_isSoundInitialized)
		_cinematic->deinit();

	if (_hasDialog)
		getApp()->getDialogHandler()->removeDialog(500001);
}

bool Movie::readSound() {
	if (!_cinematic)
		error("[Movie::readSound] Cinematic not initialized properly");

	// Read sound data offset
	uint32 offset = _cinematic->readUint32LE();
	if (_cinematic->err() || _cinematic->eos()) {
		warning("[Movie::readSound] Error reading from file");
		deinit();
		return false;
	}

	// Check if there is any sound data
	if (!offset)
		return true;

	if (offset > 10000000) {
		warning("[Movie::readSound] Invalid sound offset (was:%d, max:10000000)", offset);
		return false;
	}

	// Check remaining file size
	if ((_cinematic->pos() + offset) >= (uint32)_cinematic->size()) {
		warning("[Movie::readSound] Invalid sound offset (would read after end of file: %d)", offset);
		deinit();
		return false;
	}

	// Create a substream and initialize cinematic sound buffer
	if (!_isSoundInitialized)
		return true;

	_cinematic->setSoundBuffer(new Common::SeekableSubReadStream(_cinematic, _cinematic->pos(), _cinematic->pos() + offset), offset);

	return true;
}

bool Movie::skipSound() {
	if (!_cinematic)
		error("[Movie::skipSound] Cinematic not initialized properly");

	// Read sound data offset
	uint32 offset = _cinematic->readUint32LE();
	if (_cinematic->err() || _cinematic->eos()) {
		warning("[Movie::skipSound] Error reading from file");
		deinit();
		return false;
	}

	// Check if there is any sound data
	if (!offset)
		return true;

	// Skip sound data
	_cinematic->seek(offset, SEEK_CUR);
	if (_cinematic->err() || _cinematic->eos()) {
		warning("[Movie::skipSound] Error reading from file");
		deinit();
		return false;
	}

	return true;
}

#pragma endregion

} // End of namespace Ring
