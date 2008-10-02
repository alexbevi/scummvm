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

using Common::File;
using Common::String;

namespace Kom {

void SoundSample::loadFile(Common::FSNode dirNode, String name) {
	File f;

	unload();

	_handle.index = -1;

	f.open(dirNode.getChild(name + ".raw"));
	_size = f.size();
	_data = new byte[_size];
	f.read(_data, f.size());
	f.close();
}

void SoundSample::unload() {
	delete[] _data;
	_data = 0;
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

void Sound::playSample(SoundSample &sample, bool loop, Audio::Mixer::SoundType type, byte volume) {
	byte flags;

	uint8 i;

	if (_mixer->isSoundHandleActive(sample._handle.handle)) {
		_mixer->stopHandle(sample._handle.handle);
	} else {
		i = getFreeHandle();
		sample._handle.index = i;
		_handles[i] = &(sample._handle);
	}

	flags = Audio::Mixer::FLAG_UNSIGNED;

	if (loop)
		flags |= Audio::Mixer::FLAG_LOOP;

	_mixer->playRaw(type, &(sample._handle.handle), sample._data,
			sample._size, 11025, flags, -1, volume);
}

void Sound::stopSample(SoundSample &sample) {
	if (_mixer->isSoundHandleActive(sample._handle.handle)) {
		_handles[sample._handle.index] = 0;
		sample._handle.index = -1;
		_mixer->stopHandle(sample._handle.handle);
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
