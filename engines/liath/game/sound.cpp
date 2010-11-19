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

#include "liath/game/sound.h"

#include "liath/data/resource.h"

#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

#include "sound/decoders/wave.h"

namespace Liath {

SoundManager::SoundManager(LiathEngine *engine) : _engine(engine) {
	_mixer = g_system->getMixer();
}

SoundManager::~SoundManager() {
	CLEAR_ARRAY(WaveEntry, _waves);

	// Zero-out passed pointers
	_engine = NULL;
	_mixer = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void SoundManager::load() {
	// Load the file stream
	Common::SeekableReadStream *stream = getResource()->createReadStreamForMember("wave.dat");
	if (!stream)
		error("SoundManager::load: File not found (wave.dat)!");

	// Get the number of entries
	stream->seek(256, SEEK_SET);
	uint32 count = stream->readUint32LE();

	// Read each entry
	stream->seek(280, SEEK_SET);
	for (uint i = 0; i < count; i++) {
		WaveEntry *entry = new WaveEntry();

		// Read filename
		char filename[256];
		stream->read(&filename, 256);
		entry->name = filename;

		// Skip the rest of the fields (they are loaded from the actual wave file)
		stream->skip(24);

		_waves.push_back(entry);
	}

	delete stream;
}

void SoundManager::unload() {
	warning("SoundManager::unload: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet SoundManager::cash(OpcodeParameters *parameters) {
	error("SoundManager::cash: Not implemented!");
}

OpcodeRet SoundManager::playWave(OpcodeParameters *parameters) {
	uint32 index = parameters->getDword(0);

	if (index <= _waves.size() && _waves[index]->stream != NULL) {
		_waves[index]->stream->rewind();

		int32 volume = _effectsLevel;
		convertVolume(volume);

		_mixer->playStream(Audio::Mixer::kSFXSoundType, _waves[index]->handle, (Audio::AudioStream *)_waves[index]->stream, -1, volume, 0, DisposeAfterUse::NO);
	}

	getWork()->getCurrent()->field_EC = parameters->getWord(4);
	getWork()->getCurrent()->field_EE = 2;

	return kOpcodeRetDefault;
}

OpcodeRet SoundManager::playMusic(OpcodeParameters *parameters, bool useEffectLevel) {
	Common::String filename = Common::String::format("%s", (char *)&parameters->params);

	debugC(kLiathDebugInterpreter, "  filename: %s  -  attenuation: %d\n", filename.c_str(), parameters->getDword(256));

	// TODO get the first empty entry
	MusicEntry *entry = getMusicEntry(filename);
	entry->name = filename;

	// Open audio stream
	Common::SeekableReadStream *stream = getResource()->createReadStreamForMember(entry->name);
	if (!stream)
		return kOpcodeRetDefault;

	Audio::RewindableAudioStream *waveStream = Audio::makeWAVStream(stream, DisposeAfterUse::YES);
	if (waveStream) {
		entry->attenuation = (parameters->getDword(256) < 1) ? 1 : 2 * parameters->getDword(256);
		entry->level = Common::Rational(useEffectLevel ? _effectsLevel + 4000 : _musicLevel + 4000, entry->attenuation);
		entry->volume = -4000;

		int32 volume = (entry->attenuation ? entry->volume : 0);
		convertVolume(volume);

		// TODO identify other fields

		_mixer->playStream(Audio::Mixer::kMusicSoundType, entry->handle, (Audio::AudioStream *)waveStream, -1, volume);
	}

	return kOpcodeRetDefault;
}

OpcodeRet SoundManager::stopMusic(OpcodeParameters *parameters) {
	error("SoundManager::stopMusic: Not implemented!");
}

OpcodeRet SoundManager::gstopMusic(OpcodeParameters *parameters) {
	error("SoundManager::gstopMusic: Not implemented!");
}

OpcodeRet SoundManager::playMidi(OpcodeParameters *parameters, bool doLoop) {
	error("SoundManager::playMidi: Not implemented!");
}

OpcodeRet SoundManager::stopMidi() {
	error("SoundManager::stopMidi: Not implemented!");
}

OpcodeRet SoundManager::volume(OpcodeParameters *parameters) {
	debugC(kLiathDebugInterpreter, "  sound type: %d / level: %d\n", parameters->getDword(0), parameters->getWord(4));

	setLevel((SoundManager::SoundType)parameters->getWord(0), 140 * parameters->getDword(4) - 15);

	return kOpcodeRetDefault;
}

//////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////

void SoundManager::setLevel(SoundType type, int32 level) {
	switch (type) {
	default:
		error("SoundManager::setVolume: Invalid volume type (was: %d, valid=1-3)", type);

	case kSoundMusic:
		_musicLevel = level;
		break;

	case kSoundEffects:
		_effectsLevel = level;
		break;

	case kSoundDialog:
		_dialogLevel = level;
		break;
	}
}

/**
 * Get the first entry with an unactive handle
 *
 * Also checks for an existing entry for that file and if it exists and does not have an active handle, returns null
 *
 *
 * @param filename Filename of the music file.
 *
 * @return null if it fails, else the music entry.
 */
SoundManager::MusicEntry *SoundManager::getMusicEntry(const Common::String &filename) {
	MusicEntry *entry = NULL;

	for (uint i = 0; i < sizeof(_musicEntries); i++) {

		// Get the first empty entry
		if (!entry && (!_musicEntries[i].handle || !_mixer->isSoundHandleActive(*_musicEntries[i].handle)))
			entry = &_musicEntries[i];

		// Check the filename
		if (entry->name == filename && (!_musicEntries[i].handle || !_mixer->isSoundHandleActive(*_musicEntries[i].handle)))
			return NULL;
	}

	if (entry)
		entry->reset();

	return entry;
}

//////////////////////////////////////////////////////////////////////////
// Conversion functions
//
// Those are from engines/agos/sound.cpp (FIXME: Move to common code?)
//////////////////////////////////////////////////////////////////////////

void SoundManager::convertVolume(int32 &vol) {
	// DirectSound was originally used, which specifies volume
	// and panning differently than ScummVM does, using a logarithmic scale
	// rather than a linear one.
	//
	// Volume is a value between -10,000 and 0.
	//
	// In both cases, the -10,000 represents -100 dB. When panning, only
	// one speaker's volume is affected - just like in ScummVM - with
	// negative values affecting the left speaker, and positive values
	// affecting the right speaker. Thus -10,000 means the left speaker is
	// silent.

	int32 v = CLIP(vol, -10000, 0);
	if (v) {
		vol = (int)((double)Audio::Mixer::kMaxChannelVolume * pow(10.0, (double)v / 2000.0) + 0.5);
	} else {
		vol = Audio::Mixer::kMaxChannelVolume;
	}
}

} // End of namespace Liath
