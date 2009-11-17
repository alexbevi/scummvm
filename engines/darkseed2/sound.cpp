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

#include "sound/wave.h"

#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Sound::Sound(Audio::Mixer &mixer) : _mixer(&mixer) {
	syncSettings();
}

Sound::~Sound() {
	stopAll();
}

bool Sound::playWAV(Common::SeekableReadStream &wav,
		Audio::Mixer::SoundType type) {

	// Is the type want to play muted?
	if (_muteSFX    && (type == Audio::Mixer::kSFXSoundType))
		return true;
	if (_muteSpeech && (type == Audio::Mixer::kSpeechSoundType))
		return true;

	// Try to find an unoccupied channel
	Audio::SoundHandle *handle = 0;
	for (int i = 0 ; i < _channelCount; i++)
		if (!_mixer->isSoundHandleActive(_handles[i])) {
			handle = &_handles[i];
			break;
		}

	if (!handle) {
		warning("Sound::playWAV(): Sound::playWAV(): All channels occupied");
		return false;
	}

	// Load WAV
	Audio::AudioStream *wavStream = Audio::makeWAVStream(&wav);
	if (!wavStream)
		return false;

	// Play it
	_mixer->playInputStream(type, handle, wavStream);

	return true;
}

bool Sound::playWAV(const Resource &resource,
		Audio::Mixer::SoundType type) {

	return playWAV(resource.getStream(), type);
}

void Sound::syncSettings() {
	// Getting conf values
	int volumeSFX    = ConfMan.getInt("sfx_volume");
	int volumeSpeech = ConfMan.getInt("speech_volume");
	bool muteSFX     = ConfMan.getBool("speech_mute");
	bool muteSpeech  = ConfMan.getBool("sfx_mute");
	bool mute        = ConfMan.getBool("mute");

	// Looking for muted types

	_muteSFX    = false;
	_muteSpeech = false;

	if (muteSFX || mute)
		volumeSFX = 0;
	if (volumeSFX == 0)
		_muteSFX = true;

	if (muteSpeech || mute)
		volumeSpeech = 0;
	if (muteSpeech == 0)
		_muteSpeech = true;

	// Setting values
	_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType   , volumeSFX);
	_mixer->setVolumeForSoundType(Audio::Mixer::kSpeechSoundType, volumeSpeech);
}

void Sound::stopAll() {
	// Stopping all channels
	for (int i = 0; i < _channelCount; i++)
		_mixer->stopHandle(_handles[i]);
}

} // End of namespace DarkSeed2
