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

#include "ring/sound/soundmanager.h"

#include "ring/base/application.h"
#include "ring/base/event.h"
#include "ring/base/dialog.h"

#include "ring/sound/soundentry.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

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

void SoundManager::play(Id soundId, bool loop) {
	SoundEntry *entry = getEntry(soundId);
	if (!entry)
		return;

	// Stop sound type
	if (entry->getType() == kSoundTypeDialog) {
		stopType(kSoundTypeDialog, 32);
	} else if (entry->isPlaying()) {
		entry->stop();

		_app->onSound(soundId, entry->getType(), 32);
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
			_app->onSound(soundId, entry->getType(), a2);
	} else {
		if (entry->isPlaying())
			entry->stop();

		_app->onSound(soundId, entry->getType(), a2);
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
void SoundManager::addEntry(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, SoundFormat format, bool a4, int32 soundChunk) {
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

} // End of namespace Ring
