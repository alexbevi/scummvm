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

#include "ring/base/saveload.h"

#include "ring/base/application.h"
#include "ring/base/bag.h"
#include "ring/base/sound.h"
#include "ring/base/timer.h"

#include "ring/helpers.h"

namespace Ring {

SaveManager::SaveManager(Application *application) : _app(application) {
	_setupType = kSetupTypeNone;
	_type = kLoadSaveInvalid;

	_currentTicks = 0;

	_ser = NULL;
	_load = NULL;
	_save = NULL;
}


SaveManager::~SaveManager() {
	// Zero-out passed pointers
	_app = NULL;

	close();
}

bool SaveManager::open(Common::String filename, LoadSaveType type) {
	// Compute full path
	Common::String path = Common::String::format("%s.ars", filename.c_str());

	// Check save file
	if (type == kLoadSaveRead && !has(path))
		return false;

	// Open save file
	switch (type) {
	default:
		error("[SaveManager::loadSaveTimer] Invalid type (%d)", type);
		break;

	case kLoadSaveRead:
		_load = g_system->getSavefileManager()->openForLoading(path);
		_ser = new Common::Serializer(_load, NULL);
		break;

	case kLoadSaveWrite:
		_save = g_system->getSavefileManager()->openForSaving(path);
		_ser = new Common::Serializer(NULL, _save);
		break;
	}

	// Setup global variables
	_currentTicks = g_system->getMillis();
	_type = type;

	return true;
}


void SaveManager::close() {
	SAFE_DELETE(_ser);
	SAFE_DELETE(_load);
	SAFE_DELETE(_save);
}

bool SaveManager::loadSave(Common::String filename, LoadSaveType type) {
	if (!open(filename, type))
		return false;

	// Handle header
	Common::String code = "ArxSav 1.00";
	switch (type) {
	default:
		error("[SaveManager::loadSave] Invalid type (%d)", type);
		break;

	case kLoadSaveRead:
		checkHeader();
		break;

	case kLoadSaveWrite:
		_ser->syncString(code);
		// TODO sync other header data
		_ser->skip(8);
		break;
	}

	// Save/load array data
	_app->saveLoadWithSerializer(*_ser);

	// Initialize save data
	initialize();

	// Application data
	_ser->syncAsUint32LE(_data.state);
	_ser->syncAsByte(_data.field_6A);
	_ser->syncAsUint32LE(_data.zone);
	_ser->syncAsByte(_data.hasCurrentPuzzle);
	_ser->syncAsUint32LE(_data.puzzleId);
	_ser->syncAsByte(_data.hasCurrentRotation);
	_ser->syncAsUint32LE(_data.rotationId);
	_ser->syncAsByte(_data.rotationFre);
	_ser->syncAsUint32LE(_data.loadFrom);
	_ser->syncAsByte(_data.isRotationCompressed);
	_ser->syncAsUint32LE(_data.archiveType);

	if (_ser->isLoading()) {
		_app->setState(_data.state);
		_app->setField6A(_data.field_6A);
		_app->setupZone(_data.zone, kSetupTypeLoading);

		// We do not close the savegame yet, as it will be closed in loadSaveSounds()
	}

	if (_ser->isSaving()) {
		_app->getSoundManager()->saveLoadWithSerializer(*_ser);

		close();
	}

	return true;
}

bool SaveManager::loadSaveTimer(Common::String filename, LoadSaveType type) {
	if (!open(filename, type))
		return false;

	// Load/Save data
	_app->getTimerHandler()->saveLoadWithSerializer(*_ser);
	_app->getBag()->saveLoadWithSerializer(*_ser);
	_app->getSoundManager()->saveLoadWithSerializer(*_ser);

	close();

	return true;
}

void SaveManager::checkHeader() {
	warning("[SaveManager::checkHeader] Not implemented");
	_ser->skip(20);
}

void SaveManager::initialize() {
	_data.state = _app->getState();
	_data.field_6A = _app->getField6A();
	_data.zone = _app->getCurrentZone();

	_data.hasCurrentPuzzle = _app->hasCurrentPuzzle();
	_data.puzzleId = _data.hasCurrentPuzzle ? _app->getCurrentPuzzleId() : kPuzzleInvalid;

	_data.hasCurrentRotation = _app->hasCurrentRotation();
	_data.rotationId = _data.hasCurrentRotation ? _app->getCurrentRotationId() : 0;

	_data.rotationFre = _app->getBag()->getRotationFre();

	_data.loadFrom = _app->getLoadFrom();
	_data.isRotationCompressed = _app->isRotationCompressed();
	_data.archiveType = _app->getArchiveType();
}

bool SaveManager::has(Common::String filename) {
	if (g_system->getSavefileManager()->listSavefiles(filename).size() == 0)
		return false;

	return true;
}

bool SaveManager::isSaving() {
	return (_type == kLoadSaveWrite);
}

void SaveManager::loadSaveSounds() {
	if (_type == kLoadSaveInvalid || _ser == NULL)
		error("[SaveManager::loadSaveSounds] Savegame not initialized!");

	_app->getSoundManager()->saveLoadWithSerializer(*_ser);

	close();
}

bool SaveManager::remove(uint32 slot) {
	error("[SaveManager::remove] Not implemented!");
}

} // End of namespace Ring
