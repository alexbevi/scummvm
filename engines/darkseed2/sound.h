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

#include "sound/mixer.h"

#include "engines/darkseed2/darkseed2.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Resource;

class Sound {
public:
	Sound(Audio::Mixer &mixer);
	~Sound();

	bool playWAV(Common::SeekableReadStream &wav,
			Audio::Mixer::SoundType type = Audio::Mixer::kSFXSoundType);
	bool playWAV(const Resource &resource,
			Audio::Mixer::SoundType type = Audio::Mixer::kSFXSoundType);

	void stopAll();

	/** Apply volume settings. */
	void syncSettings();

private:
	static const int _channelCount = 8;

	bool _muteSFX;
	bool _muteSpeech;

	Audio::SoundHandle _handles[_channelCount];

	Audio::Mixer *_mixer;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_SOUND_H
