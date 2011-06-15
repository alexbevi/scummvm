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

#include "ring/base/saveload.h"

#include "ring/base/application.h"
#include "ring/base/bag.h"
#include "ring/base/timer.h"

#include "ring/graphics/image.h"

#include "ring/sound/soundmanager.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "graphics/thumbnail.h"

namespace Ring {

#define RING_SAVEGAME_VERSION 1
static const char *ringSavegameIdentification = "ArxSav";

SaveManager::SaveManager(Application *application) : _app(application) {
	_setupType = kSetupTypeNone;
	_type = kLoadSaveInvalid;

	_currentTicks = 0;
	_slot = 0;

	_ser = NULL;
	_load = NULL;
	_save = NULL;
}


SaveManager::~SaveManager() {
	SAFE_DELETE(_ser);
	SAFE_DELETE(_load);
	SAFE_DELETE(_save);

	// Zero-out passed pointers
	_app = NULL;
}

// Header
bool SaveManager::readSavegameHeader(Common::InSaveFile *in, SavegameHeader &header) {
	char saveIdent[6];
	header.thumbnail = NULL;

	// Validate the header Id
	in->read(saveIdent, 6);
	if (strcmp(saveIdent, ringSavegameIdentification))
		return false;

	header.version = in->readByte();
	if (header.version != RING_SAVEGAME_VERSION)
		return false;

	// Read in name and description
	header.name.clear();
	char ch;
	while ((ch = (char)in->readByte()) != '\0') header.name += ch;

	header.description.clear();
	while ((ch = (char)in->readByte()) != '\0') header.description += ch;

	header.date = in->readUint32LE();
	header.time = in->readUint16LE();
	header.playtime = in->readUint32LE();

	// Get the thumbnail
	header.setThumbnail(new Graphics::Surface());
	if (!Graphics::loadThumbnail(*in, *header.thumbnail)) {
		header.thumbnail->free();
		delete header.thumbnail;
		header.thumbnail = NULL;
		return false;
	}

	return true;
}

void SaveManager::writeSavegameHeader(Common::OutSaveFile *out, const SavegameHeader &header) {
	// Write out a savegame header
	out->write(ringSavegameIdentification, 6);
	out->writeByte(RING_SAVEGAME_VERSION);

	// Write savegame name and description
	out->write(header.name.c_str(), header.name.size() + 1);
	out->write(header.description.c_str(), header.description.size() + 1);

	out->writeUint32LE(header.date);
	out->writeUint16LE(header.time);
	out->writeUint32LE(header.playtime);

	// Create a thumbnail and save it
	Graphics::saveThumbnail(*out, *header.thumbnail);
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

bool SaveManager::loadSave(const Common::String &name, LoadSaveType type) {
	error("[SaveManager::loadSave] Not implemented");
}

bool SaveManager::loadSave(uint32 slot, LoadSaveType type) {
	if (!open(SaveManager::getSavegameFile(slot), type))
		return false;

	// Set slot (will be used to load timers)
	_slot = slot;

	// Handle header
	switch (type) {
	default:
		error("[SaveManager::loadSave] Invalid type (%d)", type);
		break;

	case kLoadSaveRead:
		// Read savegame header
		readSavegameHeader(_load, _header);
		break;

	case kLoadSaveWrite:
		writeSavegameHeader(_save, _header);
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

bool SaveManager::loadSaveTimer(Common::String zone, LoadSaveType type) {
	if (!open(getTimerFile(zone, _slot), type))
		return false;

	// Load/Save data
	_app->getTimerHandler()->saveLoadWithSerializer(*_ser);
	_app->getBag()->saveLoadWithSerializer(*_ser);
	_app->getSoundManager()->saveLoadWithSerializer(*_ser);

	close();

	return true;
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

bool SaveManager::hasTimer(Common::String zone) {
	return has(getTimerFile(zone, _slot));
}

bool SaveManager::hasZoneSavegame() {
	error("[SaveManager::hasZoneSavegame] not implemented");
}

bool SaveManager::has(Common::String filename) const {
	if (g_system->getSavefileManager()->listSavefiles(filename).size() == 0)
		return false;

	return true;
}

bool SaveManager::isSaving() const {
	return (_type == kLoadSaveWrite);
}

void SaveManager::loadSaveSounds() {
	if (_type == kLoadSaveInvalid || _ser == NULL)
		error("[SaveManager::loadSaveSounds] Savegame not initialized");

	_app->getSoundManager()->saveLoadWithSerializer(*_ser);

	close();
}

void SaveManager::loadProgress() {
	error("[SaveManager::loadProgress] Not implemented");
}

void SaveManager::saveProgress(ProgressState progress) {
	error("[SaveManager::saveProgress] Not implemented");
}

Common::Array<SaveManager::SaveEntry *> *SaveManager::loadUsers() {
	error("[SaveManager::loadUsers] Not implemented");
}

void SaveManager::setThumbnail(Image *image) {
	_header.setThumbnail(image->getSurface());
}

void SaveManager::setDescription(const Common::String &description) {
	_header.description = description;

	// Update current date/time and total playtime
	TimeDate curTime;
	g_system->getTimeAndDate(curTime);
	_header.date = ((curTime.tm_mday & 0xFF) << 24) | (((curTime.tm_mon + 1) & 0xFF) << 16) | ((curTime.tm_year + 1900) & 0xFFFF);
	_header.time = ((curTime.tm_hour & 0xFF) << 8) | ((curTime.tm_min) & 0xFF);
	_header.playtime = g_engine->getTotalPlayTime() / 1000;
}

bool SaveManager::remove(uint32 slot) const {
	return g_system->getSavefileManager()->removeSavefile(SaveManager::getSavegameFile(slot));
}

const char *SaveManager::getSavegameFile(int slot) {
	return getSavegameFile(((RingEngine *)g_engine)->getGameDescription()->desc.gameid, slot);
}

const char *SaveManager::getSavegameFile(const char *gameid, int slot) {
	static char buffer[54];
	assert(strlen(gameid) < 50);

	sprintf(buffer, "%s.s%02d", gameid, slot);
	return buffer;
}

Common::String SaveManager::getTimerFile(Common::String zone, uint32 slot) {
	return Common::String::format("%s_%s.s%02d", ((RingEngine *)g_engine)->getGameDescription()->desc.gameid, zone.c_str(), slot);
}

uint32 SaveManager::getNextSlot() const {
	RingEngine *engine = (RingEngine *)g_engine;

	return engine->listSaves(engine->getGameDescription()->desc.gameid).size() - 1;
}

} // End of namespace Ring
