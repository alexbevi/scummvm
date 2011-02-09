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

#include "audio/audiostream.h"
#include "audio/decoders/wave.h"
#include "audio/decoders/aiff.h"
#include "audio/decoders/mac_snd.h"

#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/options.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Sound::Sound(Audio::Mixer &mixer, Variables &variables) {
	_mixer     = &mixer;
	_variables = &variables;
	_soundType = kSoundTypeWAV;

	_id = 0;

	for (int i = 0 ; i < kChannelCount; i++) {
		_channels[i].id = -1;
		_channels[i].speech = false;
		_channels[i].dummyPlaysUntil = 0;
	}
}

Sound::~Sound() {
	stopAll();
}

void Sound::init(SoundType soundType) {
	_soundType = soundType;
}

bool Sound::playSound(Resources &resources, const Common::String &sound, int *id,
		Audio::Mixer::SoundType type) {

	Common::String fileName = Resources::addExtension(sound,
			resources.getVersionFormats().getSoundExtension(_soundType));

	debugC(-1, kDebugSound, "Playing sound \"%s\"", fileName.c_str());

	if (!resources.hasResource(fileName))
		return false;

	Common::SeekableReadStream *resource = resources.getResource(fileName);
	Common::SeekableReadStream *stream = resource->readStream(resource->size());
	delete resource;

	if (!playSound(*stream, id, type, true)) {
		delete stream;
		return false;
	}

	return true;
}

void Sound::playDummySound(int &id, uint32 length, Audio::Mixer::SoundType type) {
	SoundChannel *channel = findEmptyChannel();
	if (!channel) {
		warning("Sound::playDummySound(): Sound::playWAV(): All channels occupied");
		return;
	}

	channel->id = _id++;
	channel->speech = type == Audio::Mixer::kSpeechSoundType;

	id = channel->id;

	channel->dummyPlaysUntil = g_system->getMillis() + length;
}

bool Sound::playSound(Common::SeekableReadStream &stream, int *id,
		Audio::Mixer::SoundType type, bool autoFree) {

	SoundChannel *channel = findEmptyChannel();
	if (!channel) {
		warning("Sound::playSound(): All channels occupied");
		return false;
	}

	stream.seek(0);

	// Load audio stream
	Audio::AudioStream *audioStream = createAudioStream(stream, autoFree);
	if (!audioStream)
		return false;

	channel->id = _id++;
	channel->speech = type == Audio::Mixer::kSpeechSoundType;

	if (id)
		*id = channel->id;

	// Play it
	_mixer->playStream(type, &channel->handle, audioStream, channel->id);

	return true;
}

Sound::SoundChannel *Sound::findEmptyChannel() {
	for (int i = 0 ; i < kChannelCount; i++)
		if (!_mixer->isSoundHandleActive(_channels[i].handle))
			return &_channels[i];

	return 0;
}

Sound::SoundChannel *Sound::findChannel(int id) {
	for (int i = 0 ; i < kChannelCount; i++)
		if (!_channels[i].id == id)
			return &_channels[i];

	return 0;
}

void Sound::stopID(int id) {
	if (id == -1)
		return;

	debugC(0, kDebugSound, "Stopping sound ID %d", id);

	_mixer->stopID(id);

	SoundChannel *channel = findChannel(id);
	if (channel)
		channel->dummyPlaysUntil = 0;
}

bool Sound::isIDPlaying(int id) {
	SoundChannel *channel = findChannel(id);
	if (channel)
		if ((channel->dummyPlaysUntil != 0) && (channel->dummyPlaysUntil > g_system->getMillis()))
			return true;

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
	for (int i = 0; i < kChannelCount; i++) {
		_mixer->stopHandle(_channels[i].handle);
		_channels[i].dummyPlaysUntil = 0;
	}
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
			if ((channel.dummyPlaysUntil != 0) && (channel.dummyPlaysUntil > g_system->getMillis()))
				continue;

			channel.id = -1;
			channel.speech = false;
			channel.dummyPlaysUntil = 0;

			if (!channel.soundVar.empty()) {
				_variables->set(channel.soundVar, 0);
				channel.soundVar.clear();
			}

		}
	}
}

Audio::AudioStream *Sound::createAudioStream(Common::SeekableReadStream &stream, bool autoFree) {
	DisposeAfterUse::Flag dispose = autoFree ? DisposeAfterUse::YES : DisposeAfterUse::NO;

	switch (_soundType) {
	case kSoundTypeWAV:
		return Audio::makeWAVStream(&stream, dispose);
	case kSoundTypeAIF:
		return Audio::makeAIFFStream(&stream, dispose);
	case kSoundTypeSND:
		return Audio::makeMacSndStream(&stream, dispose);
	}

	return 0;
}

} // End of namespace DarkSeed2
