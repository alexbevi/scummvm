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

#include "common/types.h"
#include "common/stream.h"

#include "sound/audiostream.h"
#include "sound/wave.h"

#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/options.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Sound::Sound(Audio::Mixer &mixer, Variables &variables) {
	_mixer     = &mixer;
	_variables = &variables;

	_id = 0;

	for (int i = 0 ; i < kChannelCount; i++) {
		_channels[i].id = -1;
		_channels[i].speech = false;
	}
}

Sound::~Sound() {
	stopAll();
}

bool Sound::playWAV(Common::SeekableReadStream &wav,
		Audio::Mixer::SoundType type, bool autoFree) {

	// Try to find an unoccupied channel
	SoundChannel *channel = 0;
	for (int i = 0 ; i < kChannelCount; i++)
		if (!_mixer->isSoundHandleActive(_channels[i].handle)) {
			channel = &_channels[i];
			break;
		}

	if (!channel) {
		warning("Sound::playWAV(): Sound::playWAV(): All channels occupied");
		return false;
	}

	wav.seek(0);

	// Load WAV
	Audio::AudioStream *wavStream = Audio::makeWAVStream(&wav, autoFree);
	if (!wavStream)
		return false;

	channel->id = _id++;
	channel->speech = type == Audio::Mixer::kSpeechSoundType;

	// Play it
	_mixer->playInputStream(type, &channel->handle, wavStream, channel->id);

	return true;
}

bool Sound::playWAV(const Resource &resource, Audio::Mixer::SoundType type) {

	return playWAV(resource.getStream(), type);
}

bool Sound::playWAV(Resources &resources, const Common::String &wav,
		Audio::Mixer::SoundType type) {

	debugC(-1, kDebugSound, "Playing WAV \"%s\"", wav.c_str());

	if (!resources.hasResource(wav + ".wav"))
		return false;

	Resource *resWAV = resources.getResource(wav + ".wav");

	uint32 size = resWAV->getSize();

	byte *data = (byte *) malloc(size);
	resWAV->getStream().read(data, size);

	delete resWAV;

	Common::MemoryReadStream *stream =
	 new Common::MemoryReadStream(data, size, DisposeAfterUse::YES);

	if (!playWAV(*stream, type, true)) {
		delete stream;
		return false;
	}

	return true;
}

bool Sound::playWAV(Common::SeekableReadStream &wav, int &id,
		Audio::Mixer::SoundType type) {

	id = _id;
	return playWAV(wav, type);
}

bool Sound::playWAV(const Resource &resource, int &id,
		Audio::Mixer::SoundType type) {

	id = _id;
	return playWAV(resource, type);
}

bool Sound::playWAV(Resources &resources, const Common::String &wav, int &id,
		Audio::Mixer::SoundType type) {

	id = _id;
	return playWAV(resources, wav, type);
}

void Sound::stopID(int id) {
	if (id == -1)
		return;

	debugC(0, kDebugSound, "Stopping sound ID %d", id);

	_mixer->stopID(id);
}

bool Sound::isIDPlaying(int id) {
	return _mixer->isSoundIDActive(id);
}

void Sound::syncSettings(const Options &options) {
	int volumeSFX    = options.getVolumeSFX();
	int volumeSpeech = options.getVolumeSpeech();

	// Setting values
	_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType   , volumeSFX);
	_mixer->setVolumeForSoundType(Audio::Mixer::kSpeechSoundType, volumeSpeech);
}

void Sound::stopAll() {
	debugC(-1, kDebugSound, "Stopping all sounds");

	// Stopping all channels
	for (int i = 0; i < kChannelCount; i++)
		_mixer->stopHandle(_channels[i].handle);
}

void Sound::pauseAll(bool pause) {
	for (int i = 0; i < kChannelCount; i++) {
		SoundChannel &channel = _channels[i];

		if (channel.id >= 0)
			_mixer->pauseID(channel.id, pause);
	}
}

void Sound::signalSpeechEnd(int id) {
	if (id == -1)
		return;

	for (int i = 0; i < kChannelCount; i++) {
		SoundChannel &channel = _channels[i];

		if ((channel.id == id) && channel.speech) {
			channel.id = -1;
			channel.speech = false;
			if (!channel.soundVar.empty()) {
				_variables->set(channel.soundVar, 0);
				channel.soundVar.clear();
			}
		}
	}
}

bool Sound::setSoundVar(int id, const Common::String &soundVar) {
	if (id == -1)
		return false;

	for (int i = 0; i < kChannelCount; i++) {
		SoundChannel &channel = _channels[i];

		if (channel.id == id)
			channel.soundVar = soundVar;
	}

	return false;
}

void Sound::updateStatus() {
	for (int i = 0; i < kChannelCount; i++) {
		SoundChannel &channel = _channels[i];
		if (!_mixer->isSoundHandleActive(channel.handle)) {
			if (!channel.soundVar.empty() && channel.speech)
				continue;

			channel.id = -1;
			channel.speech = false;

			if (!channel.soundVar.empty()) {
				_variables->set(channel.soundVar, 0);
				channel.soundVar.clear();
			}

		}
	}
}

} // End of namespace DarkSeed2
