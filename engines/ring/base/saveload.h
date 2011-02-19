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

#ifndef RING_SAVELOAD_H
#define RING_SAVELOAD_H

#include "ring/shared.h"

namespace Ring {

class Application;

class SaveManager {
public:
	SaveManager(Application *application);
	~SaveManager();

	bool loadSave(Common::String filename, LoadSaveType type);
	bool loadSaveTimer(Common::String filename, LoadSaveType type);

	bool has(Common::String filename);

	// Accessors
	Zone getZone() const { return _zone; }
	bool hasRotation() const { return _hasRotation; }
	Id getRotationId() const { return _rotationId; }
	PuzzleId getPuzzleId() const { return _puzzleId; }
	SetupType getSetupType() const { return _setupType; }

	void setSetupType(SetupType type) { _setupType = type; }

private:
	Application *_app;

	Zone _zone;
	bool _hasRotation;
	Id _rotationId;
	PuzzleId _puzzleId;
	SetupType _setupType;
};

} // End of namespace Ring

#endif // RING_SAVELOAD_H
