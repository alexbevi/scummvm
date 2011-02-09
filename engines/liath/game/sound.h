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

#ifndef LIATH_SOUND_H
#define LIATH_SOUND_H

#include "liath/shared.h"

#include "liath/helpers.h"
#include "common/rational.h"

#include "common/stream.h"

#include "audio/audiostream.h"
#include "audio/mixer.h"

namespace Liath {

class LiathEngine;

class SoundManager {
public:
	enum SoundType {
		kSoundMusic = 1,
		kSoundEffects = 2,
		kSoundDialog = 3
	};

	SoundManager(LiathEngine *engine);
	~SoundManager();

	void load();
	void unload();

	// Opcodes
	OpcodeRet cash(OpcodeParameters *parameters);
	OpcodeRet playWave(OpcodeParameters *parameters);
	OpcodeRet playMusic(OpcodeParameters *parameters, bool useEffectLevel);
	OpcodeRet stopMusic(OpcodeParameters *parameters);
	OpcodeRet gstopMusic(OpcodeParameters *parameters);
	OpcodeRet playMidi(OpcodeParameters *parameters, bool doLoop);
	OpcodeRet stopMidi();
	OpcodeRet volume(OpcodeParameters *parameters);

private:
	struct MusicEntry {
		Common::String name;
		Audio::SoundHandle *handle;
		Common::Rational level;   // in dB
		int32 volume;  // in dB
		int32 attenuation;

		MusicEntry() {
			handle = NULL;
			reset();
		}

		~MusicEntry() {
			SAFE_DELETE(handle);
		}

		void reset() {
			name = "";
			SAFE_DELETE(handle);
			handle = new Audio::SoundHandle();
			level = 0;
			volume = 0;
			attenuation = 0;
		}
	};

	struct WaveEntry {
		Common::String name;   // 256
		uint32 field_100;
		Audio::RewindableAudioStream *stream;
		uint32 field_108;
		uint32 size;
		uint32 field_110;
		uint32 field_114;

		// Sound handle (not part of the original struct)
		Audio::SoundHandle *handle;

		WaveEntry() {
			field_100 = 0;
			stream = NULL;
			field_108 = 0;
			size = 0;
			field_110 = 0;
			field_114 = 0;

			handle = new Audio::SoundHandle();
		}

		~WaveEntry() {
			SAFE_DELETE(stream);
			SAFE_DELETE(handle);
		}
	};

	LiathEngine  *_engine;
	Audio::Mixer *_mixer;

	Common::Array<WaveEntry *> _waves;

	MusicEntry _musicEntries[10];

	int32 _musicLevel;
	int32 _effectsLevel;
	int32 _dialogLevel;

	void setLevel(SoundType type, int32 level);

	MusicEntry *getMusicEntry(const Common::String &filename);

	/**
	 * Convert volume.
	 *
	 * @param [in,out] vol The volume.
	 */
	static void convertVolume(int32 &vol);
};

} // End of namespace Liath

#endif // LIATH_SOUND_H
