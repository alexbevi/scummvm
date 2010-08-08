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

#include "common/scummsys.h"
#include "common/endian.h"

#include "sound/decoders/raw.h"

#include "engines/darkseed2/cpk_decoder.h"

namespace DarkSeed2 {

SegaFILMDecoder::SegaFILMDecoder() : _stream(0) {
	_sampleTable = 0;
	_audioStream = 0;
	_codec = 0;
}

SegaFILMDecoder::~SegaFILMDecoder() {
	close();
}

bool SegaFILMDecoder::load(Common::SeekableReadStream *stream) {
	close();

	_stream = stream;

	// FILM Header
	if (_stream->readUint32BE() != MKID_BE('FILM'))
		return false;

	uint32 filmHeaderLength = _stream->readUint32BE();
	uint32 filmVersion = _stream->readUint32BE();
	_stream->readUint32BE(); // Reserved

	// We don't support the 3DO/SegaCD/Batman and Robin variants
	if (filmVersion == 0 || filmVersion == 0x20000)
		return false;

	// FDSC Chunk
	if (_stream->readUint32BE() != MKID_BE('FDSC'))
		return false;

	/* uint32 fdscChunkSize = */ _stream->readUint32BE();
	/* uint32 videoCodec = */ _stream->readUint32BE(); // Always Cinepak, so it doesn't matter
	_height = _stream->readUint32BE();
	_width = _stream->readUint32BE();
	_stream->readByte(); // Unknown
	byte audioChannels = _stream->readByte();
	byte audioSampleSize = _stream->readByte();
	_stream->readByte(); // Unknown
	uint16 audioFrequency = _stream->readUint16BE();

	_stream->skip(6);

	// STAB Chunk
	if (_stream->readUint32BE() != MKID_BE('STAB'))
		return false;

	// The STAB chunk size changes definitions depending on the version anyway...
	/* uint32 stabChunkSize = */ _stream->readUint32BE();
	_baseFreq = _stream->readUint32BE();
	_sampleCount = _stream->readUint32BE();
	_nextFrameStartTime = 0;

	_sampleTable = new SampleTableEntry[_sampleCount];
	_frameCount = 0;
	_sampleTablePosition = 0;
	for (uint32 i = 0; i < _sampleCount; i++) {
		_sampleTable[i].offset = _stream->readUint32BE() + filmHeaderLength; // Offset is relative to the end of the header
		_sampleTable[i].length = _stream->readUint32BE();
		_sampleTable[i].sampleInfo1 = _stream->readUint32BE();
		_sampleTable[i].sampleInfo2 = _stream->readUint32BE();

		// Calculate the frame count based on the number of video samples.
		// 0xFFFFFFFF represents an audio frame.
		if (_sampleTable[i].sampleInfo1 != 0xFFFFFFFF)
			_frameCount++;
	}

	// Create the Cinepak decoder
	_codec = new ::Graphics::CinepakDecoder();

	// Create the audio stream if audio is present
	if (audioSampleSize != 0) {
		_audioFlags = 0;
		if (audioChannels == 2)
			_audioFlags |= Audio::FLAG_STEREO;
		if (audioSampleSize == 16)
			_audioFlags |= Audio::FLAG_16BITS;

		_audioStream = Audio::makeQueuingAudioStream(audioFrequency, audioChannels == 2);
		g_system->getMixer()->playStream(Audio::Mixer::kPlainSoundType, &_audioStreamHandle, _audioStream);
	}

	return true;
}

::Graphics::Surface *SegaFILMDecoder::decodeNextFrame() {
	if (endOfVideo())
		return 0;

	// Decode until we get a video frame
	for (;;) {
		_stream->seek(_sampleTable[_sampleTablePosition].offset);

		if (_sampleTable[_sampleTablePosition].sampleInfo1 == 0xFFFFFFFF) {
			// Planar audio data. All left channel first and then left in stereo.
			// TODO: Maybe move this to a new class?
			byte *audioBuffer = (byte *)malloc(_sampleTable[_sampleTablePosition].length);

			if (_audioFlags & Audio::FLAG_16BITS) {
				if (_audioFlags & Audio::FLAG_STEREO) {
					for (byte i = 0; i < 2; i++)
						for (uint16 j = 0; j < _sampleTable[_sampleTablePosition].length / 4; j++)
							WRITE_BE_UINT16(audioBuffer + j * 4 + i * 2, _stream->readUint16BE());
				} else {
					for (uint16 i = 0; i < _sampleTable[_sampleTablePosition].length / 2; i++)
						WRITE_BE_UINT16(audioBuffer + i * 2, _stream->readUint16BE());
				}
			} else {
				if (_audioFlags & Audio::FLAG_STEREO) {
					for (byte i = 0; i < 2; i++)
						for (uint16 j = 0; j < _sampleTable[_sampleTablePosition].length / 2; j++)
							audioBuffer[j * 2 + i] = _stream->readByte();
				} else {
					for (uint16 i = 0; i < _sampleTable[_sampleTablePosition].length; i++)
						audioBuffer[i] = _stream->readByte();
				}
			}

			// Now the audio is loaded, so let's queue it
			_audioStream->queueBuffer(audioBuffer, _sampleTable[_sampleTablePosition].length, DisposeAfterUse::YES, _audioFlags);
			_sampleTablePosition++;
		} else {
			// We have a video frame!
			::Graphics::Surface *surface = _codec->decodeImage(_stream->readStream(_sampleTable[_sampleTablePosition].length));

			// Add the frame's duration to the next frame start
			_nextFrameStartTime += _sampleTable[_sampleTablePosition].sampleInfo2;
			_curFrame++;
			_sampleTablePosition++;

			if (_curFrame == 0)
				_startTime = g_system->getMillis();

			return surface;
		}
	}

	// This should be impossible to get to
	error("Could not find Sega FILM frame %d", getCurFrame());
	return 0;
}

::Graphics::PixelFormat SegaFILMDecoder::getPixelFormat() const {
	assert(_codec);
	return _codec->getPixelFormat();
}

uint32 SegaFILMDecoder::getTimeToNextFrame() const {
	if (endOfVideo() || _curFrame < 0)
		return 0;

	// Convert from the Sega FILM base to 1000
	uint32 nextFrameStartTime = _nextFrameStartTime * 1000 / _baseFreq;
	uint32 elapsedTime = getElapsedTime();

	if (nextFrameStartTime <= elapsedTime)
		return 0;

	return nextFrameStartTime - elapsedTime;
}

uint32 SegaFILMDecoder::getElapsedTime() const {
	if (_audioStream)
		return g_system->getMixer()->getSoundElapsedTime(_audioStreamHandle);

	return ::Graphics::VideoDecoder::getElapsedTime();
}

void SegaFILMDecoder::close() {
	if (!_stream)
		return;

	reset();

	if (_audioStream) {
		if (g_system->getMixer()->isSoundHandleActive(_audioStreamHandle))
			g_system->getMixer()->stopHandle(_audioStreamHandle);

		_audioStream = 0;
	}

	delete _codec; _codec = 0;
	delete[] _sampleTable; _sampleTable = 0;
	delete _stream; _stream = 0;
}

} // End of namespace DarkSeed2
