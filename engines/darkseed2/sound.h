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

#ifndef DARKSEED2_SOUND_H
#define DARKSEED2_SOUND_H

#include "common/str.h"

#include "audio/mixer.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/versionformats.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Variables;

class Options;
class Resource;

class Sound {
public:
	Sound(Audio::Mixer &mixer, Variables &variables);
	~Sound();

	/** Set the sound type */
	void init(SoundType soundType);

	bool playSound(Common::SeekableReadStream &stream, int *id = 0,
			Audio::Mixer::SoundType type = Audio::Mixer::kSFXSoundType, bool autoFree = false);
	bool playSound(Resources &resources, const Common::String &stream, int *id = 0,
			Audio::Mixer::SoundType type = Audio::Mixer::kSFXSoundType);

	void playDummySound(int &id, uint32 length, Audio::Mixer::SoundType type = Audio::Mixer::kSFXSoundType);

	/** Is the that ID playing? */
	bool isIDPlaying(int id);

	/** Stop that ID. */
	void stopID(int id);
	/** Stop all playing sounds. */
	void stopAll();

	/** Pause/Unpause all playing sounds. */
	void pauseAll(bool pause);

	/** Set the sound variable of playing sound with the given ID. */
	bool setSoundVar(int id, const Common::String &soundVar);

	/** Signal that a speech has ended. */
	void signalSpeechEnd(int id);

	/** Apply volume settings. */
	void syncSettings(const Options &options);

	/** Check for status changes. */
	void updateStatus();

private:
	static const int kChannelCount = 8; ///< Number of usable channels.

	/** A sound channel. */
	struct SoundChannel {
		/** The sound handle. */
		Audio::SoundHandle handle;
		/** The variable that changes when playing stopped. */
		Common::String soundVar;
		/** The ID of the playing sound. */
		int id;
		/** Currently playing speech? */
		bool speech;
		uint32 dummyPlaysUntil;
	};

	Audio::Mixer *_mixer;
	Variables *_variables;
	SoundType _soundType;

	int _id; ///< The next ID.

	/** All sound channels. */
	SoundChannel _channels[kChannelCount];

	SoundChannel *findEmptyChannel();
	SoundChannel *findChannel(int id);

	Audio::AudioStream *createAudioStream(Common::SeekableReadStream &stream, bool autoFree = false);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_SOUND_H
