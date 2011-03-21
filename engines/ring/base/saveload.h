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

#include "common/array.h"
#include "common/savefile.h"
#include "common/serializer.h"

#include "engines/savestate.h"

#include "graphics/surface.h"

namespace Ring {

class Application;
class Image;

class SaveManager {
public:
	struct RingSavegameHeader {
		uint8 version;
		Common::String name;
		Common::String description;
		uint32 date;
		uint16 time;
		uint32 playtime;
		Graphics::Surface *thumbnail;

		void setThumbnail(Graphics::Surface *surface) {
			if (thumbnail) {
				thumbnail->free();
				delete thumbnail;
			}

			thumbnail->copyFrom(*surface);
		}
	};

	struct SavegameData {
		State       state;
		byte        field_6A;
		Zone        zone;
		bool        hasCurrentPuzzle;
		PuzzleId    puzzleId;
		bool        hasCurrentRotation;
		Id          rotationId;
		bool        rotationFre;
		LoadFrom    loadFrom;
		bool        isRotationCompressed;
		ArchiveType archiveType;
	};

	SaveManager(Application *application);
	~SaveManager();

	// Header
	static bool readSavegameHeader(Common::InSaveFile *in, RingSavegameHeader &header);
	static void writeSavegameHeader(Common::OutSaveFile *out, RingSavegameHeader &header);

	// Loading & Saving
	bool loadSave(uint32 slot, LoadSaveType type);
	bool loadSaveTimer(Common::String filename, LoadSaveType type);
	void loadSaveSounds();

	static const char *getSavegameFile(uint32 slot);
	static const char *getSavegameFile(const char *gameid, uint32 slot);
	static Common::String getTimerFile(Common::String zone, uint32 slot);

	uint32 getNextSlot();

	// Management
	bool remove(uint32 slot);

	// Thumbnail image
	void setThumbnail(Image *image);

	// Accessors
	bool isSaving();
	bool hasTimer(Common::String zone);
	SavegameData *getData() { return &_data; }
	SetupType getSetupType() const { return _setupType; }
	Common::String *getName() { return &_header.name; }
	uint32 getTicks() { return _currentTicks; }

	void setDescription(const Common::String &description);
	void setSetupType(SetupType type) { _setupType = type; }

	// Helper
	template<class T>
	static void syncArray(Common::Serializer &s, Common::Array<T *> *arr);

private:
	Application *_app;

	RingSavegameHeader _header;
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

	bool has(Common::String filename);
	bool open(Common::String filename, LoadSaveType type);
	void close();
};

template<class T>
void SaveManager::syncArray(Common::Serializer &s, Common::Array<T *> *arr) {
	for (typename Common::Array<T *>::iterator it = arr->begin(); it != arr->end(); it++)
		(*it)->saveLoadWithSerializer(s);
}

} // End of namespace Ring

#endif // RING_SAVELOAD_H
