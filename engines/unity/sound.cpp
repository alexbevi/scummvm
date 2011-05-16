/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "sound.h"
#include "common/system.h"
#include "common/memstream.h"
#include "common/queue.h"
#include "audio/audiostream.h"
#include "audio/decoders/adpcm_intern.h"

namespace Unity {

class Unity_ADPCMStream : public ::Audio::Ima_ADPCMStream {
protected:
	uint32 _loopPoint;
	uint32 _currPos;
	::Audio::ADPCMStream::ADPCMStatus _loopStatus;
	Common::Queue<int16> _samplesBuffer;

public:
	Unity_ADPCMStream(Common::SeekableReadStream *stream, DisposeAfterUse::Flag disposeAfterUse, uint32 size, int rate, int channels, uint32 loopPoint = 0)
		: Ima_ADPCMStream(stream, disposeAfterUse, size, rate, channels, 0), _loopPoint(loopPoint), _currPos(0) {
		memset(&_loopStatus, 0, sizeof(_loopStatus));
	}

	virtual bool endOfData() const;
	virtual int readBuffer(int16 *buffer, const int numSamples);
	virtual bool rewind();
};

bool Unity_ADPCMStream::endOfData() const {
	return _samplesBuffer.empty() && ADPCMStream::endOfData();
}

int Unity_ADPCMStream::readBuffer(int16 *buffer, const int numSamples) {
	bool stereo = _channels == 2;

	int samples = 0;

	// First, use anything left over from last time.
	int16 *target = buffer;
	while (!_samplesBuffer.empty() && samples < numSamples) {
		*(target++) = _samplesBuffer.pop();
		samples++;
	}

	for (; samples < numSamples && !_stream->eos() && _stream->pos() < _endpos; samples += 4) {
		byte data;
		int16 block[4];

		data = _stream->readByte();
		if (_currPos == _loopPoint)
			memcpy(&_loopStatus.ima_ch[0], &_status.ima_ch[0], sizeof(_status.ima_ch[0]));
		block[0] = decodeIMA(data & 0x0f);
		block[stereo ? 2 : 1] = decodeIMA((data >> 4) & 0x0f);

		data = _stream->readByte();
		if (_currPos + (stereo ? 0 : 2) == _loopPoint)
			memcpy(&_loopStatus.ima_ch[stereo ? 1 : 0], &_status.ima_ch[stereo ? 1 : 0], sizeof(_status.ima_ch[0]));
		block[stereo ? 1 : 2] = decodeIMA(data & 0x0f, stereo ? 1 : 0);
		block[3] = decodeIMA((data >> 4) & 0x0f, stereo ? 1 : 0);

		// Put the result in the output buffer, storing anything left.
		for (int i = 0; i < 4; i++) {
			if (samples + i < numSamples) {
				*(target++) = block[i];
			} else {
				_samplesBuffer.push(block[i]);
			}
		}

		_currPos += 2;
	}

	if (samples > numSamples)
		return numSamples;
	return samples;
}

bool Unity_ADPCMStream::rewind() {
	if (_stream->pos() < _startpos + (int)_loopPoint)
		return true;
	memcpy(&_status, &_loopStatus, sizeof(_status));
	_stream->seek(_startpos + _loopPoint);
	_currPos = _loopPoint;
	return true;
}

Sound::Sound(UnityEngine *_engine) : _vm(_engine) {
	_speechSoundHandle = NULL;
	_sfxSoundHandle = NULL;
	_musicSoundHandle = NULL;
}

Sound::~Sound() {
	delete _speechSoundHandle;
	delete _sfxSoundHandle;
	delete _musicSoundHandle;
}

void Sound::init() {
	_sfxSoundHandle = new Audio::SoundHandle();
	_speechSoundHandle = new Audio::SoundHandle();
	_musicSoundHandle = new Audio::SoundHandle();
}

void Sound::playSpeech(Common::String name) {
	debug(1, "playing speech: %s", name.c_str());
	stopSpeech();
	Common::SeekableReadStream *audioFileStream = _vm->data.openFile(name);
	Audio::AudioStream *sampleStream = new Unity_ADPCMStream(
		audioFileStream, DisposeAfterUse::YES, audioFileStream->size(), 22050, 1);
	if (!sampleStream) error("couldn't make sample stream");
	_vm->_mixer->playStream(Audio::Mixer::kSpeechSoundType, _speechSoundHandle, sampleStream);
}

void Sound::playMusic(Common::String name, byte volume, int loopPos) {
	debug(1, "playing music: %s, loop %d, vol %d", name.c_str(), loopPos, volume);
	Common::SeekableReadStream *audioFileStream = _vm->data.openFile(name);
	Audio::RewindableAudioStream *sampleStream = new Unity_ADPCMStream(
		audioFileStream, DisposeAfterUse::YES, audioFileStream->size(), 22050, 2, loopPos);
	if (!sampleStream) error("couldn't make sample stream");
	Audio::AudioStream *audioStream = sampleStream;
	if (loopPos != -1)
		audioStream = makeLoopingAudioStream(sampleStream, 0);
	_vm->_mixer->playStream(Audio::Mixer::kMusicSoundType, _musicSoundHandle, audioStream, -1, volume);
}

bool Sound::musicPlaying() {
	return _vm->_mixer->isSoundHandleActive(*_musicSoundHandle);
}

bool Sound::speechPlaying() {
	return _vm->_mixer->isSoundHandleActive(*_speechSoundHandle);
}

void Sound::stopMusic() {
	_vm->_mixer->stopHandle(*_musicSoundHandle);
}

void Sound::stopSpeech() {
	_vm->_mixer->stopHandle(*_speechSoundHandle);
}

void Sound::playAudioBuffer(unsigned int length, byte *data) {
	Common::MemoryReadStream *audioFileStream = new Common::MemoryReadStream(data, length);
	Audio::AudioStream *sampleStream = new Unity_ADPCMStream(
		audioFileStream, DisposeAfterUse::YES, audioFileStream->size(), 22050, 1);
	if (!sampleStream) error("couldn't make sample stream");
	_vm->_mixer->playStream(Audio::Mixer::kSFXSoundType, _sfxSoundHandle, sampleStream);
}

void Sound::playIntroMusic() {
	stopMusic();

	// FIXME
}

void Sound::updateMusic() {
	if (musicPlaying())
		return;

	// This is hard-coded, with loop offsets and volumes.
	switch (_vm->data.current_screen.world) {
	case 2:
		// Allanor
		switch (_vm->data.current_screen.screen) {
		case 1:
		case 2:
			playMusic("adamb01.rac", 0x1f, 0xab40);
			break;
		case 3:
			playMusic("adamb03.rac", 0x19, 0xae80);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			playMusic("adamb04.rac", 0x1f, 0xcbc0);
			break;
		case 8:
		case 9:
		case 10:
		case 12:
			playMusic("adamb08.rac", 0x19, 0xa1c0);
			break;
		default:
			playMusic("adamb11.rac", 0x19, 0xab40);
			break;
		}
		break;

	case 3:
		// Zoo World
		switch (_vm->data.current_screen.screen) {
		case 3:
		case 10:
			playMusic("zootree.rac", 0x30, 0x548e);
			break;
		case 4:
			playMusic("zoolab.rac", 0x30, 0x5b9c);
			break;
		case 6:
			playMusic("zoodesrt.rac", 0x30, 0x8b82);
			break;
		case 7:
			playMusic("zooswamp.rac", 0x30, 0x5c08);
			break;
		case 8:
			playMusic("zoosulfr.rac", 0x30, 0x419c);
			break;
		case 16:
			playMusic("powersta.rac", 0x30, 0xad80);
			break;
		default:
			playMusic("zoobirds.rac", 0x30, 0x1b1ee);
			break;
		}
		break;

	case 4:
		// Lab (Orbital Station)
		switch (_vm->data.current_screen.screen) {
		case 2:
		case 3:
		case 4:
			playMusic("lbbigfar.rac", 0x3f, 0x3160);
			break;
		case 6:
			// Probe room - music depends on whether probe is still present.
			if (_vm->data.getObject(objectID(4, 6, 3))->flags & OBJFLAG_ACTIVE)
				playMusic("probroom.rac", 0x3f, 0x5a00);
			else
				playMusic("lbbignr.rac", 0x3f, 0x4ebc);
			break;
		case 10:
		case 11:
			playMusic("lbctnear.rac", 0x3f, 0x4896);
			break;
		default:
			playMusic("lbctrfar.rac", 0x3f, 0x4e9c);
			break;
		}
		break;

	case 5:
		// Frigis
		switch (_vm->data.current_screen.screen) {
		case 1:
		case 2:
			playMusic("s5amb01.rac", 0x1f, 0xa7c0);
			break;
		case 3:
			playMusic("s5amb03.rac", 0x1f, 0xeb80);
			break;
		case 4:
			playMusic("s5amb04.rac", 0x1f, 0xb778);
			break;
		case 5:
			playMusic("s5amb05.rac", 0x1f, 0x9a00);
			break;
		case 6:
			playMusic("s5amb06.rac", 0x1f, 0xb0c0);
			break;
		case 7:
			playMusic("s5amb07.rac", 0x1f, 0xb980);
			break;
		default:
			playMusic("s5amb08.rac", 0x1f, 0x13388);
			break;
		}
		break;

	case 6:
		// Unity Device
		switch (_vm->data.current_screen.screen) {
		case 1:
			playMusic("udamb01.rac", 0x1f, 0x4c00);
			break;
		case 2:
			playMusic("udamb02.rac", 0x1f, 0x6c32);
			break;
		case 6:
			playMusic("udamb06.rac", 0x1f, 0x7a40);
			break;
		case 9:
			playMusic("udamb09.rac", 0x1f, 0x62c0);
			break;
		case 12:
			playMusic("udamb12.rac", 0x1f, 0x5c80);
			break;
		default:
			// default to Horst III music
			playMusic("h3amb01.rac", 0x2f, 0x162b0);
			break;
		}
		break;

	case 7:
		// Horst III
		playMusic("h3amb01.rac", 0x2f, 0x162b0);
		break;
	}
}

}

