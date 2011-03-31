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

#include "ring/base/sound.h"

#include "ring/base/application.h"
#include "ring/base/event.h"
#include "ring/base/dialog.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "audio/decoders/wave.h"

#include "common/file.h"

namespace Ring {

#define SOUND_FRAC_VALUE .01745328888888889f

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

#pragma region SoundEntryS

SoundEntryStream::SoundEntryStream(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format, uint32 soundChunk) : SoundEntry(soundId, type, name, loadFrom, format) {
	_field_126 = 0;
	_field_12A = 0;
	_audioStream = NULL;
	_field_132 = 0;
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
}

SoundEntryStream::~SoundEntryStream() {
	stopAndReleaseSoundBuffer();

	// Duplicated?
	SAFE_DELETE(_audioStream);
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
	// Handle uncompressed wav file
	if (format == kSoundFormatWAV) {
		if (_audioStream)
			delete _audioStream;

		// Open stream to file
		Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(path);
		if (!stream)
			error("[SoundEntryData::initSoundBuffer] Cannot open stream to file (%s)", path.c_str());

		_audioStream = Audio::makeWAVStream(stream, DisposeAfterUse::YES);

		return;
	}

	// Handle compressed sounds
	warning("[SoundEntryS::initSoundBuffer] Not implemented (compressed sounds)");
}

void SoundEntryStream::stopAndReleaseSoundBuffer() {
	if (!_audioStream)
		return;

	stop();

	SAFE_DELETE(_audioStream);

	error("[SoundEntryS::stopAndReleaseSoundBuffer] Not implemented");
}

#pragma endregion

#pragma region SoundEntryD

SoundEntryData::SoundEntryData(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format) : SoundEntry(soundId, type, name, loadFrom, format) {
	_audioStream = NULL;
	_isPreloaded = 0;
}

SoundEntryData::~SoundEntryData() {
	stopAndReleaseSoundBuffer();

	SAFE_DELETE(_audioStream); // Duplicated?
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


#pragma region SoundManager

SoundManager::SoundManager(Application *application, Audio::Mixer *mixer) : _app(application), _mixer(mixer) {
	_globalVolume = 1.0f;
}

SoundManager::~SoundManager() {
	CLEAR_ARRAY(SoundEntry, _entries);

	// Zero-out passed pointers
	_app = NULL;
	_mixer = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Playing
void SoundManager::updateQueue() {
	for (Common::Array<SoundEntry *>::iterator it = _entries.begin(); it != _entries.end(); it++) {
		if (!(*it)->checkPlaying())
			continue;

		if ((*it)->getType() == kSoundTypeDialog)
			continue;

		_app->getEventHandler()->onSound((*it)->getId(), (*it)->getType(), 4097);
	}
}

void SoundManager::play(Id soundId, bool loop) {
	SoundEntry *entry = getEntry(soundId);
	if (!entry)
		return;

	// Stop sound type
	if (entry->getType() == kSoundTypeDialog) {
		stopType(kSoundTypeDialog, 32);
	} else if (entry->isPlaying()) {
		entry->stop();

		_app->getEventHandler()->onSound(soundId, entry->getType(), 32);
	}

	// Setup and play entry
	Dialog *dialog = NULL;
	if (entry->getType() == kSoundTypeDialog) {
		dialog = new Dialog(soundId, entry->getName());
		_app->getDialogHandler()->addDialog(dialog);
	}

	entry->play(loop);
	entry->setPlaying(true);

	if (entry->getType() == kSoundTypeDialog && dialog != NULL)
		dialog->setTicks();
}

void SoundManager::stop(Id soundId, uint32 a2) {
	SoundEntry *entry = getEntry(soundId);
	if (!entry)
		return;

	if (entry->getType() == kSoundTypeDialog) {
		if (entry->isPlaying())
			entry->stop();

		if (_app->getDialogHandler()->removeDialog(soundId))
			_app->getEventHandler()->onSound(soundId, entry->getType(), a2);
	} else {
		if (entry->isPlaying())
			entry->stop();

		_app->getEventHandler()->onSound(soundId, entry->getType(), a2);
	}
}

void SoundManager::preload(Id soundId) {
	SoundEntry *entry = getEntry(soundId);
	if (entry)
		entry->preload();
}

void SoundManager::setVolume(Id soundId, uint32 volume) {
	if (!_entries.has(soundId))
		return;

	_entries.get(soundId)->setVolume(volume);
}

void SoundManager::stopType(SoundType soundType, uint32 a2) {
	bool stopDialog = true;

	for (Common::Array<SoundEntry *>::iterator it = _entries.begin(); it != _entries.end(); it++) {
		SoundEntry *entry = (*it);

		if (entry->getType() != soundType)
			continue;

		if (entry->getType() == kSoundTypeDialog && stopDialog) {
			if (entry->isPlaying())
				entry->stop();

			if (_app->getDialogHandler()->removeDialog(entry->getId())) {
				_app->getEventHandler()->onSound(entry->getId(), entry->getType(), a2);

				stopDialog = false;
			}
		} else {
			if (entry->isPlaying()) {
				entry->stop();

				_app->getEventHandler()->onSound(entry->getId(), entry->getType(), a2);
			}
		}
	}
}

void SoundManager::setMultiplier(SoundType soundType, int32 multiplier) {
	for (Common::Array<SoundEntry *>::iterator it = _entries.begin(); it != _entries.end(); it++) {
		SoundEntry *entry = (*it);

		if (entry->getType() == soundType)
			entry->setMultiplier(multiplier);
	}
}

void SoundManager::setMultiplierIfNotType(SoundType soundType, int32 multiplier) {
	for (Common::Array<SoundEntry *>::iterator it = _entries.begin(); it != _entries.end(); it++) {
		SoundEntry *entry = (*it);

		if (entry->getType() != soundType)
			entry->setMultiplier(multiplier);
	}
}

void SoundManager::stopAll(uint32 a1) {
	bool stopDialog = true;

	for (Common::Array<SoundEntry *>::iterator it = _entries.begin(); it != _entries.end(); it++) {
		SoundEntry *entry = (*it);

		if (entry->getType() == kSoundTypeDialog && stopDialog) {
			if (entry->isPlaying())
				entry->stop();

			if (_app->getDialogHandler()->removeDialog(entry->getId())) {
				_app->getEventHandler()->onSound(entry->getId(), entry->getType(), a1);

				stopDialog = false;
			}
		} else {
			if (entry->isPlaying()) {
				entry->stop();

				_app->getEventHandler()->onSound(entry->getId(), entry->getType(), a1);
			}
		}
	}
}

void SoundManager::setPan(Id soundId, int32 pan) {
	if (!_entries.has(soundId))
		return;

	_entries.get(soundId)->setPan(pan);
}

bool SoundManager::isPlaying(Id soundId) {
	SoundEntry *entry = getEntry(soundId);
	if (!entry)
		return false;

	if (entry->getType() == kSoundTypeDialog)
		return _app->getDialogHandler()->isPlaying(soundId);
	else
		return entry->isPlaying();
}

bool SoundManager::isPlayingType(SoundType soundType) {
	for (Common::Array<SoundEntry *>::iterator it = _entries.begin(); it != _entries.end(); it++) {
		SoundEntry *entry = (*it);

		if (entry->getType() == soundType) {
			if (entry->getType() == kSoundTypeDialog)
				return _app->getDialogHandler()->isPlaying(entry->getId());
			else
				return entry->isPlaying();
		}
	}

	return false;
}

void SoundManager::clear() {
	CLEAR_ARRAY(SoundEntry, _entries);
}

void SoundManager::playSounds() {
	for (Common::Array<SavedSound *>::iterator it = _savedSounds.begin(); it != _savedSounds.end(); it++)
		play((*it)->soundId, (*it)->loop);

	CLEAR_ARRAY(SavedSound, _savedSounds);
}

//////////////////////////////////////////////////////////////////////////
// Sound Entries
void SoundManager::addEntry(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, SoundFormat format, bool a4, uint32 soundChunk) {
	// Check if we already have a sound entry for this id
	if (getEntry(soundId))
		return;

	SoundEntry *entry = NULL;
	if (a4)
		entry = new SoundEntryStream(soundId, type, filename, loadFrom, format, soundChunk);
	else
		entry = new SoundEntryData(soundId, type, filename, loadFrom, format);

	_entries.push_back(entry);
}

SoundEntry *SoundManager::getEntry(Id soundId) {
	if (!_entries.has(soundId))
		return NULL;

	return _entries.get(soundId);
}

void SoundManager::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsUint32LE(_globalVolume);

	Common::Array<Id> _preloadedSounds;

	// Load sound data
	for (Common::Array<SoundEntry *>::iterator it = _entries.begin(); it != _entries.end(); it++) {
		SoundEntry *entry = (*it);

		int32 volume     = entry->getVolume();
		int32 multiplier = entry->getVolume();
		int32 pan        = entry->getVolume();

		s.syncAsSint32LE(volume);
		s.syncAsSint32LE(multiplier);
		s.syncAsSint32LE(pan);

		entry->setSoundInfo(volume, multiplier, pan);

		if (s.isSaving()) {

			if (entry->isPlaying()) {
				SavedSound *savedSound = new SavedSound();
				savedSound->soundId = entry->getId();
				savedSound->loop = entry->isLooping();

				_savedSounds.push_back(savedSound);
			}

			if (entry->isPreloaded())
				_preloadedSounds.push_back(entry->getId());
		}
	}

	// Compute playing and preloaded sounds
	uint32 preloadedCount = _preloadedSounds.size();
	uint32 playingCount   = _savedSounds.size();

	// Handle preloaded sounds
	s.syncAsUint32LE(preloadedCount);

	for (uint32 i = 0; i < preloadedCount; i++) {
		if (s.isSaving())
			s.syncAsUint32LE(_preloadedSounds[i]);

		if (s.isLoading()) {
			Id id = 0;
			s.syncAsUint32LE(id);
			preload(id);
		}
	}

	// Handle saved sounds
	if (s.isLoading())
		CLEAR_ARRAY(SavedSound, _savedSounds);

	s.syncAsUint32LE(playingCount);

	for (uint32 i = 0; i < preloadedCount; i++) {
		if (s.isSaving()) {
			s.syncAsUint32LE(_savedSounds[i]->soundId);
			s.syncAsByte(_savedSounds[i]->loop);
		}

		if (s.isLoading()) {
			SavedSound *sound = new SavedSound();
			s.syncAsUint32LE(sound->soundId);
			s.syncAsByte(sound->loop);

			_savedSounds.push_back(sound);
		}
	}

	if (s.isSaving())
		CLEAR_ARRAY(SavedSound, _savedSounds);
}

#pragma endregion

#pragma region SoundItem

SoundItem::SoundItem(Id id) : BaseObject(id) {
	_entry    = NULL;
	_volume  = 0;
	_pan  = 0;
	_field_10 = 0;
	_field_14 = 0;
	_isOn = false;
	_field_19 = 0;
	_field_1D = 20;
	_angle    = 0.0f;
	_volume1 = 0;
	_pan1 = 0;
	_volume2 = 0;
	_pan2 = 0;
	_field_35 = 0;
	_field_39 = 0;
	_field_3D = 0.0f;
}

SoundItem::~SoundItem() {
	_entry = NULL;
}

void SoundItem::init(SoundEntry *entry, uint32 volume, int32 pan, bool isTurnedOn, uint32 fadeFrames, uint32 a6, uint32 a7) {
	init(entry, volume, pan, isTurnedOn, a6, a7, fadeFrames, 0.0, 20);
}

void SoundItem::init(SoundEntry *entry, uint32 volume, int32 pan, bool isTurnedOn, uint32 a5, uint32 a6, uint32 fadeFrames, float angle, int a9) {
	if (fadeFrames < 1) // FIXME??? (was <= 1)
		error( "[SoundItem::init] Fade number of frames needs to be greater then 1 (was: %d)", fadeFrames);

	_entry    = entry;
	entry->setField125(0);

	_volume  = volume;
	_pan  = pan;
	_field_10 = a5;
	_field_14 = a6;
	_isOn = isTurnedOn;
	_field_19 = fadeFrames - 1;
	setField1D(a9);
	setAngle(angle);
	_volume1 = 0;
	_pan1 = 0;
	_volume2 = 0;
	_pan2 = 0;
	_field_35 = 0;
	_field_39 = 0;
	_field_3D = 0.0f;

	setVolume(volume);
	setPanOnEntry(pan);
}

void SoundItem::on() {
	_isOn = true;

	if (checkCurrentPuzzle())
		turnOn();
}

void SoundItem::off() {
	_isOn = false;

	if (checkCurrentPuzzle())
		turnOff();
}

void SoundItem::turnOn() {
	if (!_entry)
		return;

	if (_entry->isPlaying())
		_entry->stop();

	setVolume(_volume);
	setPanOnEntry(_pan);

	_entry->play(true);

	_entry->setPlaying(true);
}

void SoundItem::turnOff() {
	if (_entry && _entry->isPlaying())
		_entry->stop();
}

void SoundItem::setVolume(int32 volume) {
	if (_entry)
		_entry->setVolume(volume);

	_volume = volume;
}

void SoundItem::setPanOnEntry(int32 pan) {
	if (_entry)
		_entry->setPan(pan);

	_pan = pan;
}

void SoundItem::setField1D(int32 val) {
	if (val > 100 || val < 0)
		return;

	_field_1D = (uint32)val;
}

void SoundItem::setAngle(float angle) {
	if (angle < -360.0 || angle > 360.0)
		return;

	_angle = getSoundDirection() * angle * SOUND_FRAC_VALUE;
}

void SoundItem::setVolumes(float volume1, float volume2) {
	_volume1 = volume1;
	_volume2 = volume2;
}

void SoundItem::setPans(float pan1, float pan2) {
	_pan1 = pan1;
	_pan2 = pan2;
}

int32 SoundItem::computePan(float angle) const {
	return (int32)(getSoundDirection() * (sin(angle * SOUND_FRAC_VALUE + _angle) * _field_1D));
}

void SoundItem::computeAndSetPan(float alp, bool apply) {
	_pan = computePan(alp);

	if (apply)
		setPanOnEntry(_pan);
}

bool SoundItem::checkCurrentPuzzle() const {
	Id id = 0;

	if (getApp()->hasCurrentPuzzle())
		id = getApp()->getCurrentPuzzleId();

	if (getApp()->hasCurrentRotation())
		id = getApp()->getCurrentRotationId();

	if (id == 0 || id != _id)
		return false;

	return true;
}

#pragma region Serializable

void SoundItem::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsSint32LE(_volume);
	s.syncAsSint32LE(_pan);
	s.syncAsByte(_isOn);
}

#pragma endregion

#pragma endregion

#pragma region SoundHandler

SoundHandler::SoundHandler() {
	_field_0 = false;
	_sounds1 = NULL;
	_sounds2 = NULL;

	_direction = -1.0;
}

SoundHandler::~SoundHandler() {
	CLEAR_ARRAY(SoundItem, _soundItems1);
	CLEAR_ARRAY(SoundItem, _soundItems2);
	CLEAR_ARRAY(SoundItem, _soundItems3);
	CLEAR_ARRAY(SoundItem, _soundItems4);

	// Zero-out passed pointers
	_sounds1 = NULL;
	_sounds2 = NULL;
}

void SoundHandler::reset() {
	_field_0 = false;
	_sounds1 = NULL;
	_sounds2 = NULL;
}

void SoundHandler::turnOffSounds1(bool process) {
	if (!process || !_sounds1)
		return;

	for (Common::Array<SoundItem *>::iterator it = _sounds1->begin(); it != _sounds1->end(); it++)
		(*it)->turnOff();
}

bool SoundHandler::processSounds() {
	if (!_sounds1 || !_sounds2)
		return false;

	// Examine sounds 1
	for (Common::Array<SoundItem *>::iterator it = _sounds1->begin(); it != _sounds1->end(); it++) {
		byte foundType = 0;
		SoundItem *item (*it);
		SoundItem* item2 = NULL;

		if (_sounds2->has(item->getId())) {
			foundType = 1;

			item2 = _sounds2->get(item->getId());
			if (!item2->isOn()) {
				if (item->isOn())
					foundType = 0;
				else
					foundType = 2;
			}
		} else {
			// FIXME: Check that this is intended
			item2 = _sounds2->front();
		}

		switch (foundType){
		default:
			break;

		case 0:
			switch (item->getField14()) {
			default:
				break;

			case 1:
				item->setVolumes(item->getVolume(), 40.0f);
				item->setPans(item->getPan(), item->getPan());
				_soundItems2.push_back(item);
				break;

			case 2:
				_soundItems1.push_back(item);
				break;
			}
			break;

		case 1:
			switch (item->getField10()) {
			default:
				break;

			case 1:
				item->setVolumes(item->getVolume(), item2->getVolume());
				item->setPans(item->getPan(), item2->getPan());
				_soundItems3.push_back(item);
				break;

			case 2:
				item->setVolumes(item->getVolume(), item->getVolume());
				item->setPans(item->getPan(), item->getPan());
				_soundItems2.push_back(item);
				_soundItems4.push_back(item2);
				break;

			case 3:
				_soundItems1.push_back(item);
				_soundItems4.push_back(item2);
				break;
			}
			break;

		case 2:
			// Do nothing
			break;
		}
	}

	// Examine sounds 2
	for (Common::Array<SoundItem *>::iterator it = _sounds1->begin(); it != _sounds1->end(); it++) {
		SoundItem *item (*it);

		if (!_sounds1->has(item->getId())) {
			_soundItems4.push_back(item);
			continue;
		}

		// Check found item
		SoundItem *item1 = _sounds1->get(item->getId());
		if (item1->isOn() || !item->isOn())
			continue;

		_soundItems4.push_back(item);
	}

	return true;
}

bool SoundHandler::updateItems(uint32 chunkCount) {
	if (_soundItems2.size() == 0)
		return false;

	for (Common::Array<SoundItem *>::iterator it = _soundItems2.begin(); it != _soundItems2.end(); it++) {
		SoundItem *item = (*it);

		int32 field3D = (chunkCount >= item->getField19()) ? item->getField19() : chunkCount - 1;
		if (field3D <= 0)
			return false;

		item->setField3D(field3D);
		item->setField35(-(item->getVolume1() - item->getVolume2()) / field3D);
		item->setField39(-(item->getPan1() - item->getPan2()) / field3D);
	}

	if (chunkCount <= 1)
		return false;

	for (Common::Array<SoundItem *>::iterator it = _soundItems3.begin(); it != _soundItems3.end(); it++) {
		SoundItem *item = (*it);

		item->setField3D(chunkCount - 1);
		item->setField35(-(item->getVolume1() - item->getVolume2()) / (chunkCount - 1));
		item->setField39(-(item->getPan1() - item->getPan2()) / (chunkCount - 1));
	}

	return true;
}


void SoundHandler::turnOffItems1() {
	for (Common::Array<SoundItem *>::iterator it = _soundItems1.begin(); it != _soundItems1.end(); it++)
		(*it)->turnOff();
}

bool SoundHandler::updateItems2(uint32 chunkCount) {
	if (_soundItems2.size() == 0)
		return false;

	for (uint32 i = 0; i < _soundItems2.size();) {
		SoundItem *item = _soundItems2[i];

		if (item->getField3D() <= chunkCount) {
			item->turnOff();

			// Remove item
			SAFE_DELETE(item);
			_soundItems2.remove_at(i);
		} else {
			item->setPan1(item->getPan1() + item->getField39());
			item->getEntry()->setPan((uint32)item->getPan1());

			item->setVolume1(item->getVolume1() + item->getField35());
			item->getEntry()->setVolume((uint32)item->getVolume1());

			i++;
		}
	}

	return true;
}

bool SoundHandler::updateItems3(uint32 chunkCount) {
	if (_soundItems3.size() == 0)
		return false;

	for (Common::Array<SoundItem *>::iterator it = _soundItems3.begin(); it != _soundItems3.end(); it++) {
		SoundItem *item = (*it);

		if (item->getField3D() <= chunkCount) {
			item->getEntry()->setVolume((uint32)item->getVolume2());
			item->getEntry()->setPan((uint32)item->getPan2());
		} else {
			item->setPan1(item->getPan1() + item->getField39());
			item->getEntry()->setPan((uint32)item->getPan1());

			item->setVolume1(item->getVolume1() + item->getField35());
			item->getEntry()->setVolume((uint32)item->getVolume1());
		}
	}

	return true;
}

void SoundHandler::turnOnItems4() {
	for (Common::Array<SoundItem *>::iterator it = _soundItems4.begin(); it != _soundItems4.end(); it++) {
		SoundItem *item = (*it);

		if (!item->isOn())
			continue;

		if (!item->getSoundEntry()) {
			item->turnOn();
			continue;
		}

		if (!item->getSoundEntry()->isPlaying())
			item->turnOn();
	}
}

void SoundHandler::setReverseStereo(int32 reverseStereo) {
	switch (reverseStereo) {
	default:
		error("[SoundHandler::setReverseStereo] Invalid argument value (%d)", reverseStereo);

	case -1:
		_direction = -1.0;
		break;

	case 1:
		_direction = 1.0;
		break;
	}
}

#pragma endregion

} // End of namespace Ring
