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
 */

#include "ring/sound/soundentry.h"

#include "ring/base/application.h"

#include "ring/sound/soundloader.h"
#include "ring/sound/soundmanager.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "audio/decoders/wave.h"

#include "common/file.h"

namespace Ring {

#pragma region SoundEntry

SoundEntry::SoundEntry(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format) : BaseObject(soundId) {
	_type       = type;
	_name       = name;
	_isPlaying  = false;
	_loadFrom   = loadFrom;
	_volume     = 100;
	_multiplier = 100;
	_pan        = 0;
	_loop       = false;
	_format     = format;
	_field_125  = 1;
}

SoundEntry::~SoundEntry() {
}

bool SoundEntry::isPlaying() const {
	return getSound()->getMixer()->isSoundHandleActive(_handle);
}

void SoundEntry::setVolume(int32 volume) {
	if (volume >= 0)
		_volume = (volume > 100) ? 100 : volume;
	else
		_volume = 0;

	setVolumeAndPan();
}

void SoundEntry::setMultiplier(int32 multiplier) {
	if (multiplier >= 0)
		_multiplier = (multiplier > 100) ? 100 : multiplier;
	else
		_multiplier = 0;

	setVolumeAndPan();
}

void SoundEntry::setPan(int32 pan) {
	if (pan >= -100)
		_pan = (pan > 100) ? 100 : pan;
	else
		_pan = -100;

	setVolumeAndPan();
}

bool SoundEntry::checkPlaying() {
	if (!_isPlaying)
		return false;

	bool playing = isPlaying();
	if (!playing) {
		_isPlaying = false;
		stop();
	}

	return (playing == false);
}

void SoundEntry::setVolumeAndPan() const {
	// Compute volume and pan

	int32 volume = (int32)(-10000.0f - _multiplier * 0.01f * _volume * 0.01f * getSound()->getGlobalVolume() * -10000.0f);
	int32 pan = (int32)(-10000.0f - (_pan + 100.0f) * -100.0f);

	// Convert volume and panning
	convertVolumeFrom(volume);
	convertPan(pan);

	getSound()->getMixer()->setChannelVolume(_handle, (byte)volume);
	getSound()->getMixer()->setChannelBalance(_handle, (byte)pan);
}

SoundFormat SoundEntry::getFormat(Common::String filename) {
	filename.toLowercase();

	if (filename.hasSuffix("wav"))
		return kSoundFormatWAV;

	if (filename.hasSuffix("wac"))
		return kSoundFormatWAC;

	if (filename.hasSuffix("was"))
		return kSoundFormatWAS;

	return kSoundFormatInvalid;
}

//////////////////////////////////////////////////////////////////////////
// Conversion functions
//
// Those are from engines/agos/sound.cpp (FIXME: Move to common code?)
//////////////////////////////////////////////////////////////////////////

void SoundEntry::convertVolumeFrom(int32 &vol) {
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

void SoundEntry::convertVolumeTo(int32 &vol) {
	vol = (int32)(log10(vol / (double)Audio::Mixer::kMaxChannelVolume) - 0.5) * 2000;
}

void SoundEntry::convertPan(int32 &pan) {
	// DirectSound was originally used, which specifies volume
	// and panning differently than ScummVM does, using a logarithmic scale
	// rather than a linear one.
	//
	// Panning is a value between -10,000 and 10,000.
	//
	// In both cases, the -10,000 represents -100 dB. When panning, only
	// one speaker's volume is affected - just like in ScummVM - with
	// negative values affecting the left speaker, and positive values
	// affecting the right speaker. Thus -10,000 means the left speaker is
	// silent.

	int32 p = CLIP(pan, -10000, 10000);
	if (p < 0) {
		pan = (int)(255.0 * pow(10.0, (double)p / 2000.0) + 127.5);
	} else if (p > 0) {
		pan = (int)(255.0 * pow(10.0, (double)p / -2000.0) - 127.5);
	} else {
		pan = 0;
	}
}

#pragma endregion

#pragma region SoundEntryStream

SoundEntryStream::SoundEntryStream(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format, uint32 soundChunk) : SoundEntry(soundId, type, name, loadFrom, format) {
	_field_126 = 0;
	_field_12A = 0;
	_audioStream = NULL;
	_field_132 = NULL;
	_field_136 = 0;
	_field_13A = 0;
	_field_13E = 0;
	_field_142 = 0;
	_field_146 = 0;
	_field_14A = 0;
	_field_14E = 0;
	_field_152 = 0;
	_field_156 = 0;
	//_event1 = 0;
	//_event2 = 0;
	_isBufferPlaying = false;
	//_event = NULL;
	_soundChunk = soundChunk;

	_loader = NULL;
}

SoundEntryStream::~SoundEntryStream() {
	stopAndReleaseSoundBuffer();
}

void SoundEntryStream::play(bool loop) {
	// Compute sound path
	Common::String path;
	if (_type == kSoundTypeDialog)
		path = Common::String::format("DATA/%s/SOUND/%s/%s", getApp()->getCurrentZoneFolder().c_str(), getApp()->getLanguageFolder().c_str(), _name.c_str());
	else
		path = Common::String::format("DATA/%s/SOUND/%s", getApp()->getCurrentZoneFolder().c_str(), _name.c_str());

	if (!Common::File::exists(path))
		error("[SoundEntryS::play] File doesn't exist (%s)", path.c_str());

	initSoundBuffer(path, _soundChunk, loop, _format);

	// Rewind and play sound
	stop();

	if (_audioStream) {
		_audioStream->rewind();
		g_system->getMixer()->playStream(Audio::Mixer::kPlainSoundType, &_handle, makeLoopingAudioStream(_audioStream, loop ? 0 : 1));

		setVolumeAndPan();
		_isPlaying = true;
	}
}

void SoundEntryStream::stop() {
	getSound()->getMixer()->stopHandle(_handle);

	_isPlaying = false;
}

void SoundEntryStream::stopAndClear() {
	stop();
	stopAndReleaseSoundBuffer();
}

void SoundEntryStream::initSoundBuffer(const Common::String &path, uint32 soundChunk, bool loop, SoundFormat format) {
	stopAndReleaseSoundBuffer();

	_loop = loop;

	// Handle compressed sounds
	if (format != kSoundFormatWAV) {
		_loader = new SoundLoader(format);

		if (loadData(format, path, soundChunk))
			SAFE_DELETE(_loader);

		return;
	}

	// Handle uncompressed wav file
	if (_audioStream)
		delete _audioStream;

	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(path);
	if (!stream)
		error("[SoundEntryStream::initSoundBuffer] Cannot open stream to file (%s)", path.c_str());

	_audioStream = Audio::makeWAVStream(stream, DisposeAfterUse::YES);
}

void SoundEntryStream::stopAndReleaseSoundBuffer() {
	if (!_audioStream)
		return;

	stop();

	// Close loader
	if (_loader)
		_loader->close();

	SAFE_DELETE(_loader);

	// Delete ???
	SAFE_DELETE(_field_132);

	// Delete audiostream
	SAFE_DELETE(_audioStream);
}

bool SoundEntryStream::loadData(SoundFormat format, const Common::String &path, uint32 soundChunk) {
	// Single chunk
	if (_loader->load(path, this))
		return true;

	// Multiple chunks, need to keep the loader around
	if (_field_126 == 1) {

		// Cannot happen for compressed sounds, always returns 0
		//if (_loader->getChunk()) {
		//	_loader->close();
		//	return true;
		//}

		error("[SoundEntryStream::loadData] Not implemented");
	} else {
		_loader->close();
		return true;
	}
}

#pragma endregion

#pragma region SoundEntryData

SoundEntryData::SoundEntryData(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format) : SoundEntry(soundId, type, name, loadFrom, format) {
	_audioStream = NULL;
	_isPreloaded = 0;
}

SoundEntryData::~SoundEntryData() {
	stopAndReleaseSoundBuffer();
}

void SoundEntryData::play(bool loop) {
	if (!_audioStream)
		preload();

	// Rewind and play sound
	_audioStream->rewind();
	g_system->getMixer()->playStream(Audio::Mixer::kPlainSoundType, &_handle, makeLoopingAudioStream(_audioStream, loop ? 0 : 1));

	setVolumeAndPan();
	_isPlaying = !loop;
}

void SoundEntryData::stop() {
	getSound()->getMixer()->stopHandle(_handle);

	if (!_isPreloaded)
		stopAndReleaseSoundBuffer();

	_isPlaying = false;
}

void SoundEntryData::stopAndClear() {
	stop();
	stopAndReleaseSoundBuffer();
}

void SoundEntryData::preload() {
	_isPreloaded = true;

	// Compute sound path
	Common::String path;
	if (_type == kSoundTypeDialog)
		path = Common::String::format("DATA/%s/SOUND/%s/%s", getApp()->getCurrentZoneFolder().c_str(), getApp()->getLanguageFolder().c_str(), _name.c_str());
	else
		path = Common::String::format("DATA/%s/SOUND/%s", getApp()->getCurrentZoneFolder().c_str(), _name.c_str());

	if (!Common::File::exists(path))
		error("[SoundEntryD::preload] File doesn't exist (%s)", path.c_str());

	initSoundBuffer(path);
}

void SoundEntryData::unload() {
	if (_isPreloaded)
		stopAndReleaseSoundBuffer();

	_isPreloaded = false;
}

void SoundEntryData::initSoundBuffer(const Common::String &path) {
	if (_audioStream)
		delete _audioStream;

	// Open stream to file
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(path);
	if (!stream)
		error("[SoundEntryData::initSoundBuffer] Cannot open stream to file (%s)", path.c_str());

	_audioStream = Audio::makeWAVStream(stream, DisposeAfterUse::YES);
}

void SoundEntryData::stopAndReleaseSoundBuffer() {
	if (!_audioStream)
		return;

	stop();
	SAFE_DELETE(_audioStream);
}

#pragma endregion

} // End of namespace Ring
