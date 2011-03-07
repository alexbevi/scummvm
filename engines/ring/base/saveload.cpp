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

namespace Ring {

SaveManager::SaveManager(Application *application) : _app(application) {
	_zone = kZoneNone;

	_loadFrom = kLoadFromInvalid;
	_archiveType = kArchiveInvalid;

	_hasPuzzle = false;
	_puzzleId = kPuzzleInvalid;

	_hasRotation = false;
	_rotationId = 0;
	_rotationFre = false;
	_isRotationCompressed = true;

	_setupType = kSetupTypeNone;

	_currentTicks = 0;
}


SaveManager::~SaveManager() {
	// Zero-out passed pointers
	_app = NULL;
}

bool SaveManager::loadSave(Common::String filename, LoadSaveType type) {
	warning("[SaveManager::saveLoad] Not implemented");

	return true;
}

bool SaveManager::loadSaveTimer(Common::String filename, LoadSaveType type) {
	warning("[SaveManager::saveLoad] Not implemented");

	return true;
}

bool SaveManager::has(Common::String filename) {
	warning("[SaveManager::has] Not implemented");

	return false;
}

bool SaveManager::isSaving() {
	error("[SaveManager::isSaving] Not implemented");
}

void SaveManager::process(Common::Serializable *ser) {
	error("[SaveManager::process] Not implemented");
}

} // End of namespace Ring
