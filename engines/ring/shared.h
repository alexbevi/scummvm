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

#ifndef RING_SHARED_H
#define RING_SHARED_H

#include "common/array.h"
#include "common/serializer.h"
#include "common/system.h"
#include "common/textconsole.h"

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
	kTimer7       = 7,
	kTimer8       = 8,
	kTimer9       = 9,
	kTimer10      = 10,
	kTimer11      = 11,
	kTimer12      = 12,
	kTimerInit    = 100,
	kTimerStartup = 101,

	kTimerInvalid = 99999
};

enum FontId {
	kFontInvalid = 0,
	kFontDefault = 1
};

enum CursorId {
	kCursorInvalid      = 0,
	kCursorPassive      = 1,
	kCursorActive       = 2,
	kCursorPassiveDraw  = 3,
	kCursorActiveDraw   = 4,

	kCursorIdle         = 50,
	kCursorBusy         = 51,
	kCursorHotspot      = 52,
	kCursorMove         = 53,
	kCursorDefault      = 54,
	kCursorBack         = 55,
	kCursorMenuIdle     = 56,
	kCursorMenuActive   = 57,
	kCursorShoot        = 58,
	kCursorZoom         = 59,
	kCursorTake         = 60,
	kCursorAction       = 61,
	kCursorActionObject = 62,
	kCursorDragDrop     = 63,

	kCursor1001         = 1001,
	kCursor1002         = 1002,
	kCursor1003         = 1003,
	kCursor1004         = 1004,
	kCursor1005         = 1005,

	kCursorHandSelection = 10000
};

#pragma endregion

#pragma region Others

enum State {
	kStateNone           = 0,
	kStateUpdateRotation = 1,
	kStateUpdatePuzzle   = 2,
	kStateDrawBag        = 3,
	kStateShowMenu       = 4
};

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

enum ZoneId {
	kZoneNone    = 0,

	// Default zones
	kZone1       = 1,
	kZone2       = 2,
	kZone3       = 3,
	kZone4       = 4,
	kZone5       = 5,
	kZone6       = 6,
	kZone7       = 7,
	kZone8       = 8,
	kZone9       = 9,
	kZone10      = 10,
	kZone11      = 11,
	kZone12      = 12,
	kZone13      = 13,
	kZone14      = 14,
	kZone15      = 15,
	kZone16      = 16,
	kZone17      = 17,

	// System zones
	kZoneSY      = 1,   // used by pre-pompeii games
	kZone100     = 100, // used by pompeii and later games

	// Game-specific zones for Ring
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
	kLoadSaveInvalid = 0,
	kLoadSaveRead    = 1,
	kLoadSaveWrite   = 2
};

enum SetupType {
	kSetupTypeNone    = 0,
	kSetupType1       = 1,
	kSetupType2       = 2,
	kSetupType3       = 3,
	kSetupType5       = 5,
	kSetupType6       = 6,
	kSetupType10      = 10,
	kSetupType998     = 998,
	kSetupType999     = 999,
	kSetupTypeLoading = 1000
};

enum ImageType {
	kImageTypeBMP = 0,
	kImageTypeTGA = 1,
	kImageTypeCIN = 2,
	kImageTypeCNM = 3,
	kImageTypeBackground = 4,
	kImageTypeBMA = 5,
	kImageTypeTGC = 6
};

enum DrawType {
	kDrawTypeInvalid    = 0,
	kDrawType1          = 1,
	kDrawType2          = 2,
	kDrawType3          = 3
};

enum SoundLoopType {
	kSoundOnce = 1,
	kSoundLoop = 2
};

enum MenuAction {
	kMenuAction0  = 0,
	kMenuAction1  = 1,
	kMenuAction2  = 2,
	kMenuAction3  = 3,
	kMenuAction4  = 4,
	kMenuAction10 = 10
};

enum ProgressState {
	kProgressStateNone = 0,
	kProgressState1    = 1,
	kProgressState2    = 2,
	kProgressState3    = 3,
	kProgressState4    = 4,
	kProgressState5    = 5,
	kProgressState6    = 6,
	kProgressState7    = 7,
	kProgressState8    = 8
};

#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Structures and types
//////////////////////////////////////////////////////////////////////////

enum PuzzleIdEnum {
	kPuzzleInvalid          = 0,
	kPuzzleMenu             = 1,

	// Shared menu puzzles
	kPuzzleGeneralMenu      = 90000,
	kPuzzlePreferences      = 90001,
	kPuzzleLoad             = 90002,
	kPuzzleSave             = 90003,
	kPuzzleGameStatus       = 90004
};

enum ObjectIdEnum {
	kObjectNone                     = -1,
	kObjectInvalid                  = 0,
	kObject1                        = 1,
	kObject2                        = 2,
	kObject3                        = 3,
	kObject4                        = 4,
	kObject5                        = 5,
	kObject6                        = 6,
	kObject7                        = 7
};

typedef int32 Id;

class BaseId {
public:
	BaseId() : _id(0) {}
	BaseId(int32 i) : _id(i) {}

	operator int32()  { return _id; }
	int32 id()        { return _id; }

	bool operator==(const int32 &i) {
		return _id == i;
	}

protected:
	int32 _id;
};

class PuzzleId : public BaseId {
public:
	PuzzleId() : BaseId(0) {}
	PuzzleId(PuzzleIdEnum i) : BaseId(i) {}
	PuzzleId(int32 i) : BaseId(i) {}

	bool operator==(const PuzzleIdEnum &i) {
		return _id == (int32)i;
	}
};

class ObjectId : public BaseId {
public:
	ObjectId() : BaseId(0) {}
	ObjectId(ObjectIdEnum i) : BaseId(i) {}
	ObjectId(int32 i) : BaseId(i) {}

	bool operator==(const ObjectIdEnum &i) {
		return _id == (int32)i;
	}

	bool operator==(const int32 &i) {
		return _id == i;
	}
};

struct Color {
	int32 red;
	int32 green;
	int32 blue;

	Color() {
		red = 0;
		green = 0;
		blue = 0;
	}

	Color(int32 r, int g, int32 b) {
		red = r;
		green = g;
		blue = b;
	}

	bool isTransparent() {
		return (red == -1 && green == -1 && blue == -1);
	}

	uint32 getColor() {
		return g_system->getScreenFormat().RGBToColor(red, green, blue);
	}
};

// Configuration data
struct SoundConfiguration {
	int32 soundChunck;
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

		for (typename Common::Array<T>::iterator it = this->begin(); it != this->end(); it++)
			((Common::Serializable*) *it)->saveLoadWithSerializer(s);
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
