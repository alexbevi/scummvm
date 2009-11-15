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

#include "common/stream.h"

#include "common/config-manager.h"

#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Sound::Sound(Audio::Mixer &mixer) : _mixer(&mixer) {
	syncSettings();
}

Sound::~Sound() {
}

bool Sound::playWAV(Common::SeekableReadStream &wav,
		Audio::Mixer::SoundType type) {

	if (_muteSFX    && (type == Audio::Mixer::kSFXSoundType))
		return true;
	if (_muteSpeech && (type == Audio::Mixer::kSpeechSoundType))
		return true;

	return true;
}

bool Sound::playWAV(const Resource &resource,
		Audio::Mixer::SoundType type) {

	return playWAV(resource.getStream(), type);
}

void Sound::syncSettings() {
	int volumeSFX    = ConfMan.getInt("sfx_volume");
	int volumeSpeech = ConfMan.getInt("speech_volume");
	bool muteSFX     = ConfMan.getBool("speech_mute");
	bool muteSpeech  = ConfMan.getBool("sfx_mute");
	bool mute        = ConfMan.getBool("mute");

	if (muteSFX || mute)
		volumeSFX = 0;
	if (volumeSFX == 0)
		_muteSFX = true;

	if (muteSpeech || mute)
		volumeSpeech = 0;
	if (muteSpeech == 0)
		_muteSpeech = true;

	_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType   , volumeSFX);
	_mixer->setVolumeForSoundType(Audio::Mixer::kSpeechSoundType, volumeSpeech);
}

} // End of namespace DarkSeed2
