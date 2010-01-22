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


#include "common/system.h"
#include "common/config-manager.h"
#include "common/file.h"
#include "common/fs.h"
#include "common/str.h"

#include "kom/kom.h"
#include "kom/sound.h"

#include "sound/mixer.h"
#include "sound/voc.h"
#include "sound/audiostream.h"
#include "sound/raw.h"

using Common::File;

namespace Kom {

bool SoundSample::loadFile(Common::String filename) {
	File f;
	byte *data;

	unload();

	_handle.index = -1;

	if (!File::exists(filename))
		return false;

	f.open(filename);
	_size = f.size();
	data = (byte *)malloc(_size);
	f.read(data, f.size());
	f.close();

	_stream = Audio::makeRawMemoryStream(data, _size,
			DisposeAfterUse::YES, 11025, Audio::FLAG_UNSIGNED);

	return true;
}

void SoundSample::unload() {
	delete _stream;
	_stream = 0;
}

Sound::Sound(KomEngine *vm, Audio::Mixer *mixer)
	: _vm(vm), _mixer(mixer), _musicEnabled(true),
	_sfxEnabled(true) {

	for (int i = 0; i < SOUND_HANDLES; i++)
		_handles[i] = 0;
}

Sound::~Sound() {
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

void Sound::playSample(SoundSample &sample, bool loop, Audio::Mixer::SoundType type, byte volume) {

	uint8 i;

	if (!sample.isLoaded())
		return;

	sample._stream->rewind();

	if (_mixer->isSoundHandleActive(sample._handle.handle)) {
		_mixer->stopHandle(sample._handle.handle);
	} else {
		i = getFreeHandle();
		sample._handle.index = i;
		_handles[i] = &(sample._handle);
	}

	if (loop) {
		Audio::AudioStream *stream = new Audio::LoopingAudioStream(sample._stream, 0, DisposeAfterUse::NO);
		_mixer->playInputStream(type, &(sample._handle.handle), stream, -1, volume, 0, DisposeAfterUse::YES);
	} else {
		_mixer->playInputStream(type, &(sample._handle.handle), sample._stream, -1, volume, 0, DisposeAfterUse::NO);
	}
}

void Sound::stopSample(SoundSample &sample) {
	if (_mixer->isSoundHandleActive(sample._handle.handle)) {
		_handles[sample._handle.index] = 0;
		sample._handle.index = -1;
		_mixer->stopHandle(sample._handle.handle);
	}
}

void Sound::pauseSample(SoundSample &sample, bool paused) {
	if (_mixer->isSoundHandleActive(sample._handle.handle)) {
		_mixer->pauseHandle(sample._handle.handle, paused);
	}
}

uint8 Sound::getFreeHandle() {
	for (int i = 0; i < SOUND_HANDLES; i++) {
		if (_handles[i] == 0)
			return i;

		if (!_mixer->isSoundHandleActive(_handles[i]->handle)) {
			_handles[i] = 0;
			return i;
		}
	}

	error("Sound::getHandle(): Too many sound handles");

	return 0;
}

} // end of namespace Kom
