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

#include "ring/sound/soundhandler.h"

#include "ring/base/application.h"

#include "ring/sound/soundentry.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

#define SOUND_FRAC_VALUE .01745328888888889f

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
