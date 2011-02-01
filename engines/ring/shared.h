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
enum ArchiveType {
	kArchiveInvalid = 0,
	kArchiveFile    = 101,
	kArchiveArt     = 102
};

enum LoadFrom {
	kLoadFromInvalid = 0,
	kLoadFromCd      = 1,
	kLoadFromDisk    = 2,
	kLoadFrom3       = 3,
	kLoadFrom4       = 4,
	kLoadFrom5       = 5
};

enum Zone {
	kZoneInvalid = 0,
	kZoneSY      = 1,
	kZoneNI      = 2,
	kZoneRH      = 3,
	kZoneFO      = 4,
	kZoneRO      = 5,
	kZoneWA      = 6,
	kZoneAS      = 7,
	kZoneN2      = 8
};

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
	kTimerInvalid = 0
};

enum FontId {
	kFontInvalid = 0,
	kFontDefault = 1
};

enum DialogId {
	kDialogInvalid = 0
};

enum ObjectId {
	kObjectInvalid                  = -1,
	kObjectNone                     = 0,
	kObject1                        = 1,
	kObject2                        = 2,
	kObject3                        = 3,
	kObject4                        = 4,
	kObject5                        = 5,
	kObject6                        = 6,
	kObject7                        = 7,
	kObjectBrutality                = 10000,
	kObjectGlug                     = 10001,
	kObjectMinerals                 = 10002,
	kObjectMagicFrog                = 10302,
	kObjectTile                     = 10303,
	kObjectGold                     = 10304,
	kObjectLogeTear                 = 10305,
	kObjectDivingHelmet             = 10503,
	kObjectDivingHelmet2            = 10504,
	kObjectAntiGCells               = 10505,
	kObjectKeyIndifference          = 20004,
	kObjectKeyMistrust              = 20005,
	kObjectKeySelfishness           = 20006,
	kObjectKeyDisgust               = 20007,
	kObjectRedfish                  = 20201,
	kObjectDolphin                  = 20202,
	kObjectMedallion                = 20203,
	kObjectAntiGCells2              = 20403,
	kObject30001                    = 30001,
	kObject30002                    = 30002,
	kObject30003                    = 30003,
	kObject30004                    = 30004,
	kObject30005                    = 30005,
	kObject30006                    = 30006,
	kObject30007                    = 30007,
	kObject30008                    = 30008,
	kObjectPatience                 = 30009,
	kObjectMovementAndIntuition     = 30010,
	kObjectLove                     = 30011,
	kObjectImagination              = 30012,
	kObjectDestruction              = 30013,
	kObjectJudgementAndDirection    = 30014,
	kObjectWill                     = 30015,
	kObject30016                    = 30016,
	kObjectBerries                  = 30017,
	kObjectSleepingBerries          = 30018,
	kObjectWolfInstinct             = 30019,
	kObjectHare                     = 30020,
	kObjectIngot                    = 30021,
	kObjectIngot2                   = 30022,
	kObjectIngot3                   = 30023,
	kObjectSleepingPotion           = 30025,
	kObjectBow                      = 30026,
	kObject30027                    = 30027,
	kObject30028                    = 30028,
	kObjectMould                    = 30029,
	kObjectLogeTear2                = 30030,
	kObjectBerriesJuice             = 30031,
	kObjectSleepingPotion2          = 30032,
	kObjectMeltedGold               = 30033,
	kObjectMeltedSilver             = 30034,
	kObjectMeltedCopper             = 30035,
	kObjectMeltedLead               = 30036,
	kObjectMeltedSteel              = 30037,
	kObjectMeltedTin                = 30038,
	kObjectQuicksilver              = 30039,
	kObject30040                    = 30040,
	kObjectMetals                   = 30041,
	kObject30042                    = 30042,
	kObjectGolem                    = 30043,
	kObject30044                    = 30044,
	kObject30045                    = 30045,
	kObjectFishingRod               = 30046,
	kObjectFishingRodWithWorms      = 30047,
	kObjectFish                     = 30048,
	kObjectWorms                    = 30049,
	kObject30050                    = 30050,
	kObject30051                    = 30051,
	kObject30052                    = 30052,
	kObjectBurningArrow             = 30053,
	kObjectKey                      = 30054,
	kObjectWolfBadge                = 30055,
	kObjectWolfBrooch               = 30056,
	kObjectNotung                   = 30058,
	kObject30059                    = 30059,
	kObject30061                    = 30061,
	kObject30100                    = 30100,
	kObject30101                    = 30101,
	kObject30102                    = 30102,
	kObject30103                    = 30103,
	kObject30104                    = 30104,
	kObject30105                    = 30105,
	kObject30106                    = 30106,
	kObject30107                    = 30107,
	kObject30108                    = 30108,
	kObject30109                    = 30109,
	kObject30110                    = 30110,
	kObject30200                    = 30200,
	kObjectRing                     = 40012,
	kObjectCrown                    = 40013,
	kObjectMagicLance               = 50000,
	kObject50001                    = 50001,
	kObject50100                    = 50100,
	kObjectInk                      = 50101,
	kObjectPaper                    = 50102,
	kObjectStylet                   = 50103,
	kObjectInkedStylet              = 50104,
	kObject50105                    = 50105,
	kObjectCounch                   = 50200,
	kObjectDeadLeaf                 = 50201,
	kObjectRopes                    = 50202,
	kObjectCloud                    = 50203,
	kObjectPhoenix                  = 50300,
	kObjectCounch1                  = 50301,
	kObjectBark                     = 50302,
	kObjectGolem1                   = 50400,
	kObjectFeather                  = 50401,
	kObjectFlower                   = 50402,
	kObjectFronthead                = 50431,
	kObjectBackhead                 = 50432,
	kObjectBelly                    = 50433,
	kObjectRightArm                 = 50434,
	kObjectLeftArm                  = 50435,
	kObjectLegs                     = 50436,
	kObjectHeart                    = 50437,
	kObject50451                    = 50451,
	kObject50452                    = 50452,
	kObject50453                    = 50453,
	kObject50454                    = 50454,
	kObject50455                    = 50455,
	kObject50456                    = 50456,
	kObject50457                    = 50457,
	kObjectMagnet                   = 50499,
	kObjectTree                     = 50500,
	kObjectDragonSword              = 50501,
	kObjectApple                    = 50502,
	kObjectTotems                   = 50503,
	kObjectThread                   = 50504,
	kObject50600                    = 50600,
	kObject50601                    = 50601,
	kObject50700                    = 50700,
	kObject51000                    = 51000,
	kObjectFire                     = 70000,
	kObjectMagicFrog2               = 70302,
	kObjectCage                     = 70303,
	kObjectLogeTear3                = 70304,
	kObjectCentaur                  = 70500,
	kObjectDragon                   = 70501,
	kObjectPhoenix1                 = 70502,
	kObjectChrysoberyl              = 70503,
	kObjectDeath                    = 80020,
	kObjectMenuNewGame              = 90000,
	kObjectMenuPreferences          = 90001,
	kObjectMenuLoad                 = 90002,
	kObjectMenuSave                 = 90003,
	kObjectMenuContinue             = 90004,
	kObjectMenuStatus               = 90005,
	kObjectMenuExit                 = 90006,
	kObjectPreferencesCancel        = 90101,
	kObjectPreferencesOk            = 90102,
	kObjectPreferencesSubtitles     = 90103,
	kObjectPreferences3dSound       = 90104,
	kObjectPreferencesSliderVolume  = 90105,
	kObjectPreferencesSliderDialog  = 90106,
	kObject90107                    = 90107,
	kObjectLoadCancel               = 90207,
	kObjectLoadOk                   = 90208,
	kObjectSaveOk                   = 90309,
	kObjectSaveCancel               = 90310,
	kObjectSave90313                = 90313,
	kObjectStatusOk                 = 90401,
	kObjectStatusProgress           = 90402,
	kObject90912                    = 90912,
	kObjectFirePower                = 40000
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

enum CursorType {
	kCursorTypeInvalid  = 0,
	kCursorTypeNormal   = 1,
	kCursorType2        = 2,
	kCursorTypeImage    = 3,
	kCursorTypeAnimated = 4
};

enum ImageType {
	kImageCursor   = 3,
	kImageListIcon = 4
};

enum PuzzleId {
	kPuzzleInvalid     = 0,
	kPuzzle1           = 1,
	kPuzzle35001       = 35001,
	kPuzzle35002       = 35002,
	kPuzzle35003       = 35003,
	kPuzzle35004       = 35004,
	kPuzzle35005       = 35005,
	kPuzzle35006       = 35006,
	kPuzzle35007       = 35007,
	kPuzzle35008       = 35008,
	kPuzzle35009       = 35009,
	kPuzzle35010       = 35010,
	kPuzzle35011       = 35011,
	kPuzzle35019       = 35019,
	kPuzzle35020       = 35020,
	kPuzzle35100       = 35100,
	kPuzzle35101       = 35101,
	kPuzzle35103       = 35103,
	kPuzzle35104       = 35104,
	kPuzzle35105       = 35105,
	kPuzzle35106       = 35106,
	kPuzzle35107       = 35107,
	kPuzzle35108       = 35108,
	kPuzzle35109       = 35109,
	kPuzzle35110       = 35110,
	kPuzzle35111       = 35111,
	kPuzzle50001       = 50001,
	kPuzzle50002       = 50002,
	kPuzzle50100       = 50100,
	kPuzzle50303       = 50303,
	kPuzzle50304       = 50304,
	kPuzzle50400       = 50400,
	kPuzzle50401       = 50401,
	kPuzzle50501       = 50501,
	kPuzzle50502       = 50502,
	kPuzzle50503       = 50503,
	kPuzzle50504       = 50504,
	kPuzzle50601       = 50601,
	kPuzzle50602       = 50602,
	kPuzzle50701       = 50701,
	kPuzzle50702       = 50702,
	kPuzzle50703       = 50703,
	kPuzzle51001       = 51001,
	kPuzzle51002       = 51002,
	kPuzzle51003       = 51003,
	kPuzzle51004       = 51004,
	kPuzzle51005       = 51005,
	kPuzzle51006       = 51006,
	kPuzzle51007       = 51007,
	kPuzzle51008       = 51008,
	kPuzzle51009       = 51009,
	kPuzzle51010       = 51010,
	kPuzzle51011       = 51011,
	kPuzzle51012       = 51012,
	kPuzzle51013       = 51013,
	kPuzzleMenu        = 90000,
	kPuzzlePreferences = 90001,
	kPuzzleLoad        = 90002,
	kPuzzleSave        = 90003,
	kPuzzleGameStatus  = 90004,
	kPuzzleInsertCd    = 90005,
};

//////////////////////////////////////////////////////////////////////////
// Structures and types
//////////////////////////////////////////////////////////////////////////
typedef int32 Id;

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
		uint32 size = this->size();
		s.syncAsUint32LE(size);

		for (uint32 i = 0; i < size; i++)
			this[i].saveLoadWithSerializer(s);
	}

	T get(Id id) {
		for (Common::Array<T>::iterator i = this->begin(); i != this->end(); i++) {
			if (((BaseObject*) *i)->getId() == id)
				return *i;
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
		for (Common::Array<T>::iterator i = this->begin(); i != this->end(); i++) {
			if (((BaseObject*) *i)->getId() == id)
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
