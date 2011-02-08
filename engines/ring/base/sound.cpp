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

#include "ring/game/application.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

#define SOUND_FRAC_VALUE .01745328888888889f

#pragma region SoundEntry

SoundEntry::SoundEntry(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format) : BaseObject(soundId) {
	_type       = type;
	_name       = name;
	_field_10C  = 0;
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

void SoundEntry::setVolume(uint32 volume) {
	if (volume >= 0)
		_volume = (volume > 100) ? 100 : volume;
	else
		_volume = 0;

	setVolumeAndPan();
}

void SoundEntry::setMultiplier(uint32 multiplier) {
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

void SoundEntry::setVolumeAndPan() {
	// Compute volume and pan

	float volume = -10000.0f - _multiplier * 0.01f * _volume * 0.01f * getSound()->getGlobalVolume() * -10000.0f;
	float pan = -10000.0f - (_pan + 100.0f) * -100.0f;

	// Convert volume and panning
	convertVolumeFrom(volume);
	convertPan(pan);

	getSound()->updateVolumeAndPan(_handle, volume, pan);
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

void SoundEntry::convertVolumeFrom(float &vol) {
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

	int32 v = CLIP(vol, -10000.0f, 0.0f);
	if (v) {
		vol = (int)((double)Audio::Mixer::kMaxChannelVolume * pow(10.0, (double)v / 2000.0) + 0.5);
	} else {
		vol = Audio::Mixer::kMaxChannelVolume;
	}
}

void SoundEntry::convertVolumeTo(float &vol) {
	vol = (log10(vol / (double)Audio::Mixer::kMaxChannelVolume) - 0.5) * 2000;
}

void SoundEntry::convertPan(float &pan) {
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

	int32 p = CLIP(pan, -10000.0f, 10000.0f);
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

SoundManager::SoundManager(Application *application, Audio::Mixer *mixer) : _application(application), _mixer(mixer) {
	_globalVolume = 1.0f;
}

SoundManager::~SoundManager() {
	CLEAR_ARRAY(SoundEntry, _entries);

	// Zero-out passed pointers
	_application = NULL;
	_mixer = NULL;
}

void SoundManager::addEntry(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, SoundFormat format, bool a4, int soundChunk) {
	// Check if we already have a sound entry for this id
	if (getSoundEntry(soundId))
		return;

	SoundEntry *entry = NULL;
	if (a4) {
		entry = new SoundEntryS(soundId, type, filename, loadFrom, format, soundChunk);
	} else {
		entry = new SoundEntryD(soundId, type, filename, loadFrom, format);
	}

	_entries.push_back(entry);
}

SoundEntry *SoundManager::getSoundEntry(Id soundId) {
	if (!_entries.has(soundId))
		return NULL;

	return _entries.get(soundId);
}

void SoundManager::setVolume(Id soundId, uint32 volume) {
	if (!_entries.has(soundId))
		return;

	_entries.get(soundId)->setVolume(volume);
}

void SoundManager::updateVolumeAndPan(Audio::SoundHandle handle, int32 volume, int32 pan) {
	_mixer->setChannelVolume(handle, (byte)volume);
	_mixer->setChannelBalance(handle, (byte)pan);
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

void SoundItem::init(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7) {
	init(entry, volume, pan, isOn, a6, a7, fadeFrames, 0.0, 20);
}

void SoundItem::init(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 a5, uint32 a6, uint32 fadeFrames, float angle, int a9) {
	if (fadeFrames < 1) // FIXME??? (was <= 1)
		error( "[SoundItem::init] Fade number of frames needs to be greater then 1 (was: %d)", fadeFrames);

	_entry    = entry;
	entry->setField125(0);

	_volume  = volume;
	_pan  = pan;
	_field_10 = a5;
	_field_14 = a6;
	_isOn = isOn;
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
	error( "[SoundItem::turnOn] Not implemented");
}

void SoundItem::turnOff() {
	error( "[SoundItem::turnOff] Not implemented");
}

void SoundItem::setVolume(uint32 volume) {
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

float SoundItem::computePan(float angle) {
	return getSoundDirection() * (sin(angle * SOUND_FRAC_VALUE + _angle) * _field_1D);
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
	_field_0 = 0;
	_field_1 = 0;
	_field_5 = 0;

	_direction = -1.0;
}

SoundHandler::~SoundHandler() {
	CLEAR_ARRAY(SoundItem, _soundItems1);
	CLEAR_ARRAY(SoundItem, _soundItems2);
	CLEAR_ARRAY(SoundItem, _soundItems3);
	CLEAR_ARRAY(SoundItem, _soundItems4);
}

void SoundHandler::reset() {
	_field_0 = 0;
	_field_1 = 0;
	_field_5 = 0;
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
