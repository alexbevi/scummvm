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
#include "ring/base/dialog.h"

#include "ring/helpers.h"
#include "ring/ring.h"

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
	_field_11D  = 0;
	_format     = format;
	_field_125  = 1;
}

SoundEntry::~SoundEntry() {
}

void SoundEntry::play(bool loop) {
	error("[SoundEntry::play] Not implemented");
}

void SoundEntry::stop() {
	getSound()->getMixer()->stopHandle(_handle);

	_isPlaying = false;
}

bool SoundEntry::isPlaying() {
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

void SoundEntry::setVolumeAndPan() {
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

SoundEntryS::SoundEntryS(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format, uint32 soundChunk) : SoundEntry(soundId, type, name, loadFrom, format) {
	_field_126 = 0;
	_field_12A = 0;
	_field_12E = 0;
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
	_field_15A = 0;
	_field_15E = 0;
	_field_162 = 0;
	//_event = NULL;
	_soundChunk = soundChunk;
}

SoundEntryS::~SoundEntryS() {
}

#pragma endregion

#pragma region SoundEntryD

SoundEntryD::SoundEntryD(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format) : SoundEntry(soundId, type, name, loadFrom, format) {
	_field_126 = 0;
	_field_12A = 0;
	_field_12E = 0;
	_field_132 = 0;
	_field_136 = 0;
	_field_13A = 0;
	_field_13E = 0;
}

SoundEntryD::~SoundEntryD() {
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

		_app->onSound((*it)->getId(), (*it)->getType(), 4097);
	}
}

void SoundManager::play(Id soundId, int a2) {
	error("[SoundManager::play] Not implemented");
}

void SoundManager::stop(Id soundId, uint32 a2) {
	SoundEntry *entry = getEntry(soundId);
	if (!entry)
		return;

	if (entry->getType() == kSoundTypeDialog) {
		if (entry->isPlaying())
			entry->stop();

		if (_app->getDialogHandler()->removeDialog(soundId))
			_app->onSound(soundId, entry->getType(), a2);
	} else {
		if (entry->isPlaying())
			entry->stop();

		_app->onSound(soundId, entry->getType(), a2);
	}
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
				_app->onSound(entry->getId(), entry->getType(), a2);

				stopDialog = false;
			}
		} else {
			if (entry->isPlaying()) {
				entry->stop();

				_app->onSound(entry->getId(), entry->getType(), a2);
			}
		}
	}
}

void SoundManager::setMultiplier(SoundType soundType, uint32 multiplier) {
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
				_app->onSound(entry->getId(), entry->getType(), a1);

				stopDialog = false;
			}
		} else {
			if (entry->isPlaying()) {
				entry->stop();

				_app->onSound(entry->getId(), entry->getType(), a1);
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
	error("[SoundManager::isPlayingType] Not implemented");
}

void SoundManager::sub_4696F0() {
	error("[SoundManager::sub_4696F0] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Sound Entries
void SoundManager::addEntry(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, SoundFormat format, bool a4, int soundChunk) {
	// Check if we already have a sound entry for this id
	if (getEntry(soundId))
		return;

	SoundEntry *entry = NULL;
	if (a4) {
		entry = new SoundEntryS(soundId, type, filename, loadFrom, format, soundChunk);
	} else {
		entry = new SoundEntryD(soundId, type, filename, loadFrom, format);
	}

	_entries.push_back(entry);
}

SoundEntry *SoundManager::getEntry(Id soundId) {
	if (!_entries.has(soundId))
		return NULL;

	return _entries.get(soundId);
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
	_field_25 = 0;
	_field_29 = 0;
	_field_2D = 0;
	_field_31 = 0;
	_field_35 = 0;
	_field_39 = 0;
	_field_3D = 0;
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
	_field_25 = 0;
	_field_29 = 0;
	_field_2D = 0;
	_field_31 = 0;
	_field_35 = 0;
	_field_39 = 0;
	_field_3D = 0;

	setVolume(volume);
	setPan(pan);
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
	setPan(_pan);

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

void SoundItem::setPan(int32 pan) {
	if (_entry)
		_entry->setPan(pan);

	_pan = pan;
}

void SoundItem::setField1D(int32 val) {
	if (val > 100 || val < 0)
		return;

	_field_1D = val;
}

void SoundItem::setAngle(float angle) {
	if (angle < -360.0 || angle > 360.0)
		return;

	_angle = getSoundDirection() * angle * SOUND_FRAC_VALUE;
}

int32 SoundItem::computePan(float angle) {
	return (int32)(getSoundDirection() * (sin(angle * SOUND_FRAC_VALUE + _angle) * _field_1D));
}

void SoundItem::computeAndSetPan(float alp, bool apply) {
	_pan = computePan(alp);

	if (apply)
		setPan(_pan);
}

bool SoundItem::checkCurrentPuzzle() {
	Id id = 0;

	if (getApp()->hasCurrentPuzzle())
		id = getApp()->getCurrentPuzzleId();

	if (getApp()->hasCurrentRotation())
		id = getApp()->getCurrentRotationId();

	if (id == 0 || id != _id)
		return false;

	return true;
}

#pragma endregion

#pragma region SoundHandler

SoundHandler::SoundHandler() {
	_field_0 = false;
	_count1 = 0;
	_count2 = 0;

	_direction = -1.0;
}

SoundHandler::~SoundHandler() {
	CLEAR_ARRAY(SoundItem, _soundItems1);
	CLEAR_ARRAY(SoundItem, _soundItems2);
	CLEAR_ARRAY(SoundItem, _soundItems3);
	CLEAR_ARRAY(SoundItem, _soundItems4);
}

void SoundHandler::reset() {
	_field_0 = false;
	_count1 = 0;
	_count2 = 0;
}

void SoundHandler::turnOffItems2(bool a1) {
	if (!a1 || !_count1)
		return;

	for (Common::Array<SoundItem *>::iterator it = _soundItems2.begin(); it != _soundItems2.end(); it++)
		(*it)->turnOff();
}

bool SoundHandler::sub_41AA00() {
	warning("[SoundHandler::sub_41AA00] Not implemented");

	return false;
}

bool SoundHandler::sub_41AEE0(uint32 a1) {
	warning("[SoundHandler::sub_41AEE0] Not implemented");

	return false;
}


void SoundHandler::turnOffItems1() {
	for (Common::Array<SoundItem *>::iterator it = _soundItems1.begin(); it != _soundItems1.end(); it++)
		(*it)->turnOff();
}

void SoundHandler::sub_41B180(uint32 a1) {
	warning("[SoundHandler::sub_41B180] Not implemented");
}

void SoundHandler::sub_41B350(uint32 a1) {
	warning("[SoundHandler::sub_41B350] Not implemented");
}

void SoundHandler::sub_41B520() {
	warning("[SoundHandler::sub_41AA00] Not implemented");
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
