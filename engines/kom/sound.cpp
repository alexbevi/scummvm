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
 */

#include "common/file.h"
#include "common/str.h"

#include "kom/kom.h"
#include "kom/sound.h"

#include "audio/mixer.h"
#include "audio/audiostream.h"
#include "audio/decoders/raw.h"

using Common::File;

namespace Kom {

bool SoundSample::loadFile(Common::String filename) {
	File f;
	byte *data;

	unload();

	if (!f.open(filename))
		return false;

	_size = f.size();
	data = (byte *)malloc(_size);
	f.read(data, f.size());
	f.close();

	_stream = Audio::makeRawStream(data, _size,
			11025, Audio::FLAG_UNSIGNED);

	return true;
}

void SoundSample::unload() {
	delete _stream;
	_stream = 0;
}

Sound::Sound(KomEngine *vm, Audio::Mixer *mixer)
	: _vm(vm), _mixer(mixer), _musicEnabled(true),
	_sfxEnabled(true) {
}

Sound::~Sound() {
}

bool Sound::playFileSFX(Common::String filename, SoundHandle *handle) {
	return playFile(filename, handle, Audio::Mixer::kSFXSoundType, 255);
}

bool Sound::playFileSpeech(Common::String filename, SoundHandle *handle) {
	return playFile(filename, handle, Audio::Mixer::kSpeechSoundType, 255);
}

void Sound::playSampleSFX(SoundSample &sample, bool loop) {
	playSample(sample, loop, Audio::Mixer::kSFXSoundType, 255);
}

void Sound::playSampleMusic(SoundSample &sample) {
	playSample(sample, true, Audio::Mixer::kMusicSoundType, 100);
}

void Sound::playSampleSpeech(SoundSample &sample) {
	playSample(sample, false, Audio::Mixer::kSpeechSoundType, 255);
}

bool Sound::playFile(Common::String filename, SoundHandle *handle, Audio::Mixer::SoundType type, byte volume) {
	File *f = new File();
	Audio::SeekableAudioStream *stream;

	if (!f->open(filename))
		return false;

	stream = Audio::makeRawStream(f, 11025, Audio::FLAG_UNSIGNED);

	_mixer->playStream(type, handle, stream, -1, volume);

	return true;
}

void Sound::playSample(SoundSample &sample, bool loop, Audio::Mixer::SoundType type, byte volume) {

	if (!sample.isLoaded())
		return;

	sample._stream->rewind();

	if (_mixer->isSoundHandleActive(sample._handle)) {
		_mixer->stopHandle(sample._handle);
	}

	if (loop) {
		Audio::AudioStream *stream = new Audio::LoopingAudioStream(sample._stream, 0, DisposeAfterUse::NO);
		_mixer->playStream(type, &(sample._handle), stream, -1, volume, 0, DisposeAfterUse::YES);
	} else {
		_mixer->playStream(type, &(sample._handle), sample._stream, -1, volume, 0, DisposeAfterUse::NO);
	}
}

void Sound::stopHandle(SoundHandle handle) {
	if (_mixer->isSoundHandleActive(handle)) {
		_mixer->stopHandle(handle);
	}
}

void Sound::stopSample(SoundSample &sample) {
	stopHandle(sample._handle);
}

void Sound::pauseSample(SoundSample &sample, bool paused) {
	if (_mixer->isSoundHandleActive(sample._handle)) {
		_mixer->pauseHandle(sample._handle, paused);
	}
}

} // end of namespace Kom
