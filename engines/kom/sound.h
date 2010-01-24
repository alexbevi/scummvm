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

#ifndef KOM_SOUND_H
#define KOM_SOUND_H

#include "common/scummsys.h"
#include "common/file.h"
#include "common/mutex.h"
#include "common/ptr.h"
#include "common/str.h"

#include "sound/mixer.h"
#include "sound/audiostream.h"

namespace Kom {

#define SOUND_HANDLES 16

struct SoundHandle {
	int index;
	Audio::SoundHandle handle;
};

class SoundSample {
	friend class Sound;
public:
	SoundSample() { _stream = 0; }
	~SoundSample() { delete _stream; }

	bool loadFile(Common::String filename);
	void unload();
	bool isLoaded() { return _stream != 0; }

private:
	Audio::SoundHandle _handle;
	uint32 _size;
	Audio::SeekableAudioStream *_stream;
};

class Sound {
public:
	Sound(KomEngine *vm, Audio::Mixer *mixer);
	~Sound();

	bool _musicEnabled;
	bool _sfxEnabled;

	void playSampleSFX(SoundSample &sample, bool loop);
	void playSampleMusic(SoundSample &sample);
	void playSampleSpeech(SoundSample &sample);
	void playSample(SoundSample &sample, bool loop, Audio::Mixer::SoundType type, byte volume);
	void stopSample(SoundSample &sample);
	void pauseSample(SoundSample &sample, bool paused);
	bool isPlaying(SoundSample &sample) { return _mixer->isSoundHandleActive(sample._handle); }

private:

	KomEngine *_vm;
	Audio::Mixer *_mixer;
};

} // end of namespace Kom

#endif
