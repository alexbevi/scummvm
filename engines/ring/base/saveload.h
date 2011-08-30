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

#ifndef RING_SAVELOAD_H
#define RING_SAVELOAD_H

#include "ring/shared.h"

#include "common/array.h"
#include "common/savefile.h"
#include "common/serializer.h"

#include "graphics/surface.h"

namespace Ring {

class Application;
class Image;

class SaveManager {
public:
	struct SaveEntry {
		Common::String filename;
		Common::String description;
	};

	struct SavegameHeader {
		uint8 version;
		Common::String name;
		Common::String description;
		uint32 date;
		uint16 time;
		uint32 playtime;
		Graphics::Surface *thumbnail;

		SavegameHeader() {
			version = 0;
			date = 0;
			time = 0;
			playtime = 0;
			thumbnail = NULL;
		}

		~SavegameHeader() {
			delete thumbnail;
		}

		bool setThumbnail(Graphics::Surface *surface) {
			if (!surface)
				return false;

			thumbnail->copyFrom(*surface);

			return true;
		}
	};

	struct SavegameData {
		State       state;
		bool        field_6A;
		ZoneId      zone;
		bool        hasCurrentPuzzle;
		PuzzleId    puzzleId;
		bool        hasCurrentRotation;
		Id          rotationId;
		bool        rotationFre;
		LoadFrom    loadFrom;
		bool        isRotationCompressed;
		ArchiveType archiveType;

		// Progress
		ProgressState progressState;
		uint32        progress;

		SavegameData() {
			state                = kStateNone;
			field_6A             = 0;
			zone                 = kZoneNone;
			hasCurrentPuzzle     = false;
			puzzleId             = kPuzzleInvalid;
			hasCurrentRotation   = false;
			rotationId           = 0;
			rotationFre          = false;
			loadFrom             = kLoadFromInvalid;
			isRotationCompressed = false;
			archiveType          = kArchiveInvalid;
			progressState        = kProgressStateNone;
			progress             = 0;
		}
	};

	SaveManager(Application *application);
	~SaveManager();

	// Header
	static bool readSavegameHeader(Common::InSaveFile *in, SavegameHeader &header);
	static void writeSavegameHeader(Common::OutSaveFile *out, const SavegameHeader &header);

	// Loading & Saving
	bool loadSave(const Common::String &name, LoadSaveType type);
	bool loadSave(uint32 slot, LoadSaveType type);
	bool loadSaveTimer(Common::String filename, LoadSaveType type);
	void loadSaveSounds();

	static const char *getSavegameFile(int slot);
	static const char *getSavegameFile(const char *gameid, int slot);
	static Common::String getTimerFile(Common::String zone, uint32 slot);
	uint32 getNextSlot() const;

	// Progress
	void loadProgress();
	void saveProgress(ProgressState progress);

	// Users
	Common::Array<SaveEntry *> *loadUsers();

	// Management
	bool remove(uint32 slot) const;

	// Thumbnail image
	void setThumbnail(Image *image);

	// Accessors
	bool isSaving() const;
	bool hasTimer(Common::String zone);
	bool hasZoneSavegame();
	SavegameData *getData() { return &_data; }
	SetupType getSetupType() const { return _setupType; }
	Common::String *getName() { return &_header.name; }
	uint32 getTicks() { return _currentTicks; }

	void setDescription(const Common::String &description);
	void setSetupType(SetupType type) { _setupType = type; }

	// Helper
	template<class T>
	static void syncArray(Common::Serializer &s, Common::Array<T *> *arr);
	template<class T>
	static void syncWithFlag(Common::Serializer &s, T *instance);

private:
	Application *_app;

	SavegameHeader _header;
	SavegameData _data;
	uint32 _slot;

	SetupType _setupType;
	LoadSaveType _type;

	uint32 _currentTicks; // Ticks at the time of loading/saving

	// Current savegame
	Common::Serializer  *_ser;
	Common::InSaveFile  *_load;
	Common::OutSaveFile *_save;

	void initialize();

	bool has(Common::String filename) const;
	bool open(Common::String filename, LoadSaveType type);
	void close();
};

template<class T>
void SaveManager::syncArray(Common::Serializer &s, Common::Array<T *> *arr) {
	for (typename Common::Array<T *>::iterator it = arr->begin(); it != arr->end(); it++)
		(*it)->saveLoadWithSerializer(s);
}

template<class T>
void SaveManager::syncWithFlag(Common::Serializer &s, T *instance) {
	// Check hotspot
	bool isPresent = (instance != NULL);
	s.syncAsByte(isPresent);

	if (s.isSaving())
		if (instance)
			instance->saveLoadWithSerializer(s);

	if (s.isLoading()) {
		if (instance) {
			if (!instance)
				instance = new T();

			instance->saveLoadWithSerializer(s);
		}
	}
}

} // End of namespace Ring

#endif // RING_SAVELOAD_H
