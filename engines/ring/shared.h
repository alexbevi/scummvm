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

#ifndef RING_SHARED_H
#define RING_SHARED_H

#include "common/array.h"
#include "common/serializer.h"
#include "common/system.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Enumerations
//////////////////////////////////////////////////////////////////////////

#pragma region Elements Ids

enum LanguageId {
	kLanguageInvalid = 0,
	kLanguageEnglish = 1,
	kLanguageFrench  = 2,
	kLanguageGerman  = 3,
	kLanguageItalian = 4,
	kLanguageSpanish = 5,
	kLanguageSwedish = 6,
	kLanguageDutch   = 7,
	kLanguageHebrew  = 8,
	kLanguageGreek   = 9,
	kLanguageSlovak  = 10
};

enum TimerId {
	kTimer0       = 0,
	kTimer1       = 1,
	kTimer2       = 2,
	kTimer3       = 3,
	kTimer4       = 4,
	kTimer5       = 5,
	kTimer6       = 6,
	kTimer9       = 9,
	kTimerInit    = 100,
	kTimerStartup = 101,

	kTimerInvalid = 99999
};

enum FontId {
	kFontInvalid = 0,
	kFontDefault = 1
};

enum CursorId {
	kCursorInvalid    = 0,
	kCursorIdle       = 50,
	kCursorBusy       = 51,
	kCursorHotspot    = 52,
	kCursorMove       = 53,
	kCursorDefault    = 54,
	kCursorBack       = 55,
	kCursorMenuIdle   = 56,
	kCursorMenuActive = 57,

	kCursorHandSelection = 10000
};

#pragma endregion

#pragma region Others

enum CursorType {
	kCursorTypeInvalid  = 0,
	kCursorTypeNormal   = 1,
	kCursorType2        = 2,
	kCursorTypeImage    = 3,
	kCursorTypeAnimated = 4
};

enum MovabilityType {
	kMovabilityRotationToRotation = 0,
	kMovabilityRotationToPuzzle   = 1,
	kMovabilityPuzzleToRotation   = 2,
	kMovabilityPuzzleToPuzzle     = 3
};

enum ArchiveType {
	kArchiveInvalid = 0,
	kArchiveFile    = 101,
	kArchiveArt     = 102
};

enum LoadFrom {
	kLoadFromInvalid  = 0,
	kLoadFromCd       = 1,
	kLoadFromDisk     = 2,
	kLoadFromCursor   = 3,
	kLoadFromListIcon = 4,
	kLoadFrom5        = 5
};

enum Zone {
	kZoneInvalid = 0,
	kZoneSY      = 1,

	// Game-specific zones
	kZoneNI      = 2,
	kZoneRH      = 3,
	kZoneFO      = 4,
	kZoneRO      = 5,
	kZoneWA      = 6,
	kZoneAS      = 7,
	kZoneN2      = 8
};

enum SoundType {
	kSoundTypeBackgroundMusic = 1,
	kSoundTypeAmbientMusic    = 2,
	kSoundTypeAmbientEffect   = 3,
	kSoundTypeEffect          = 4,
	kSoundTypeDialog          = 5
};

enum SoundFormat {
	kSoundFormatWAV = 0,
	kSoundFormatWAC = 1,
	kSoundFormatWAS = 2,

	kSoundFormatInvalid = 999
};

enum LoadSaveType {
	kLoadSaveRead  = 1,
	kLoadSaveWrite = 2
};

#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Structures and types
//////////////////////////////////////////////////////////////////////////

enum PuzzleIdEnum {
	kPuzzleInvalid = 0
};

enum ObjectIdEnum {
	kObjectInvalid = 0
};

typedef uint32 Id;

class BaseId {
public:
	BaseId() : _id(0) {}
	BaseId(uint32 i) : _id(i) {}

	operator uint32() { return _id;	}
	uint32 id()       { return _id; }

	bool operator==(const uint32 &i) {
		return _id == i;
	}

protected:
	uint32 _id;
};

class PuzzleId : public BaseId {
public:
	PuzzleId(PuzzleIdEnum i) : BaseId(i) {}
	PuzzleId(uint32 i) : BaseId(i) {}
};

class ObjectId : public BaseId {
public:
	ObjectId(ObjectIdEnum i) : BaseId(i) {}
	ObjectId(uint32 i) : BaseId(i) {}

	bool operator==(const ObjectIdEnum &i) {
		return _id == (uint32)i;
	}
};

struct Color {
	uint32 red;
	uint32 green;
	uint32 blue;

	Color() {
		red = 0;
		green = 0;
		blue = 0;
	}

	Color(uint32 r, uint g, uint32 b) {
		red = r;
		green = g;
		blue = b;
	}
};

// Configuration data
struct SoundConfiguration {
	int soundChunck;
	LoadFrom loadFrom;

	SoundConfiguration() {
		soundChunck = 9;
		loadFrom = kLoadFromCd;
	}
};

struct Configuration {
	bool runningFromDisk;
	int checkCD;
	SoundConfiguration backgroundMusic;
	SoundConfiguration ambientMusic;
	SoundConfiguration ambientEffect;
	SoundConfiguration effect;
	SoundConfiguration dialog;
	bool artBAG;
	bool artCURSOR;
	bool artSY;
	bool artAS;
	bool artNI;
	bool artN2;
	bool artRO;
	bool artRH;
	bool artWA;
	bool artFO;
	bool checkLoadSave;

	Configuration() {
		runningFromDisk = false;
		checkCD = 0;
		artBAG = false;
		artCURSOR = false;
		artSY = false;
		artAS = false;
		artNI = false;
		artN2 = false;
		artRO = false;
		artRH = false;
		artWA = false;
		artFO = false;
		checkLoadSave = false;
	}
};

//////////////////////////////////////////////////////////////////////////
// Classes
//////////////////////////////////////////////////////////////////////////
class BaseObject {
public:
	BaseObject(Id id) : _id(id) {}

	void setId(Id id) { _id = id; }
	Id getId() { return _id; }

protected:
	Id _id;
};

template<class T>
class AssociativeArray : public Common::Array<T>, public Common::Serializable {
public:
	void saveLoadWithSerializer(Common::Serializer &s) {
		uint32 count = this->size();
		s.syncAsUint32LE(count);

		for (uint32 i = 0; i < count; i++)
			this[i].saveLoadWithSerializer(s);
	}

	T get(Id id) {
		for (typename Common::Array<T>::iterator it = this->begin(); it != this->end(); it++) {
			if (((BaseObject*) *it)->getId() == id)
				return *it;
		}

		error("[AssociativeArray::get] No object has this ID (%d)", id);
	}

	uint32 getIndex(Id id) {
		for (uint32 i = 0; i < this->size(); i++) {
			if (((BaseObject *)(*this)[i])->getId() == id)
				return i;
		}

		error("[AssociativeArray::getIndex] No object has this ID (%d)", id);
	}

	bool has(Id id) {
		for (typename Common::Array<T>::iterator it = this->begin(); it != this->end(); it++) {
			if (((BaseObject*) *it)->getId() == id)
				return true;
		}

		return false;
	}

	void remove(Id id) {
		for (uint32 i = 0; i < this->size();) {
			if (((BaseObject *)(*this)[i])->getId() == id) {
				delete (*this)[i];
				(*this)[i] = NULL;
				this->remove_at(i);
			} else {
				++i;
			}
		}
	}
};

} // End of namespace Ring

#endif // RING_SHARED_H
