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

#include "ring/ring.h"

#include "ring/helpers.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// SoundItem
//////////////////////////////////////////////////////////////////////////
SoundItem::SoundItem(Id id) : BaseObject(id) {
	_entry    = NULL;
	_field_8  = 0;
	_field_C  = 0;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = 0;
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
}

void SoundItem::init(SoundEntry *entry, uint32 a2, uint32 a3, byte a4, uint32 fadeFrames, uint32 a6, uint32 a7) {
	init(entry, a2, a3, a4, a6, a7, fadeFrames, 0.0, 20);
}

void SoundItem::init(SoundEntry *entry, uint32 a2, uint32 a3, byte a4, uint32 a5, uint32 a6, uint32 fadeFrames, float angle, int a9) {
	if (fadeFrames <= 1)
		error( "[SoundItem::init] Fade number of frames needs to be greater then 1 (was: %d)", fadeFrames);

	_entry    = entry;
	entry->setField125(0);

	_field_8  = a2;
	_field_C  = a3;
	_field_10 = a5;
	_field_14 = a6;
	_field_18 = a4;
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

	// TODO missing two method calls (setting up volume?)
	warning( "[SoundItem::init] Implementation incomplete");
}

void SoundItem::setField1D(int32 val) {
	if (val > 100 || val < 0)
		return;

	_field_1D = val;
}

void SoundItem::setAngle(float angle) {
	if (angle < -360.0 || angle > 360.0)
		return;

	_angle = getSoundDirection() * angle * .01745328888888889f;
}

//////////////////////////////////////////////////////////////////////////
// SoundHandler
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// SoundEntry
//////////////////////////////////////////////////////////////////////////
SoundEntry::SoundEntry(Id soundId, SoundType type, Common::String name, byte a5) : BaseObject(soundId) {
	_type = type;
	_name = name;
	_field_10C = 0;
	_field_110 = a5;
	_field_111 = 0;
	_field_115 = 0;
	_field_119 = 0;
	_field_11D = 0;
	_loadFrom = kLoadFromInvalid;
	_field_125 = 1;
}

SoundEntry::~SoundEntry() {
}

//////////////////////////////////////////////////////////////////////////
// SoundManager
//////////////////////////////////////////////////////////////////////////
SoundManager::SoundManager(Application *application) : _application(application) {
}

SoundManager::~SoundManager() {
	CLEAR_ARRAY(SoundEntry, _entries);

	// Zero-out passed pointers
	_application = NULL;
}

SoundEntry *SoundManager::getSoundEntry(Id soundId) {
	if (!_entries.has(soundId))
		return NULL;

	return _entries.get(soundId);
}

} // End of namespace Ring
