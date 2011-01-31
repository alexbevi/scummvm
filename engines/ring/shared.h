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
	kObjectInvalid               = -1,
	kObjectNone                  = 0,
	kObject1                     = 1,
	kObject2                     = 2,
	kObject3                     = 3,
	kObject4                     = 4,
	kObject6                     = 6,
	kObjectBrutality             = 10000,
	kObjectGlug                  = 10001,
	kObjectMinerals              = 10002,
	kObjectMagicFrog             = 10302,
	kObjectTile                  = 10303,
	kObjectGold                  = 10304,
	kObjectLogeTear              = 10305,
	kObjectDivingHelmet          = 10503,
	kObjectDivingHelmet2         = 10504,
	kObjectAntiGCells            = 10505,
	kObjectKeyIndifference       = 20004,
	kObjectKeyMistrust           = 20005,
	kObjectKeySelfishness        = 20006,
	kObjectKeyDisgust            = 20007,
	kObjectRedfish               = 20201,
	kObjectDolphin               = 20202,
	kObjectMedallion             = 20203,
	kObjectAntiGCells2           = 20403,
	kObjectPatience              = 30009,
	kObjectMovementAndIntuition  = 30010,
	kObjectLove                  = 30011,
	kObjectImagination           = 30012,
	kObjectDestruction           = 30013,
	kObjectJudgementAndDirection = 30014,
	kObjectWill                  = 30015,
	kObjectBerries               = 30017,
	kObjectSleepingBerries       = 30018,
	kObjectWolfInstinct          = 30019,
	kObjectHare                  = 30020,
	kObjectIngot                 = 30021,
	kObjectIngot2                = 30022,
	kObjectIngot3                = 30023,
	kObjectSleepingPotion        = 30025,
	kObjectBow                   = 30026,
	kObject30028                 = 30028,
	kObjectMould                 = 30029,
	kObjectLogeTear2             = 30030,
	kObjectBerriesJuice          = 30031,
	kObjectSleepingPotion2       = 30032,
	kObjectMeltedGold            = 30033,
	kObjectMeltedSilver          = 30034,
	kObjectMeltedCopper          = 30035,
	kObjectMeltedLead            = 30036,
	kObjectMeltedSteel           = 30037,
	kObjectMeltedTin             = 30038,
	kObjectQuicksilver           = 30039,
	kObjectMetals                = 30041,
	kObjectGolem                 = 30043,
	kObjectFishingRod            = 30046,
	kObjectFishingRodWithWorms   = 30047,
	kObjectFish                  = 30048,
	kObjectWorms                 = 30049,
	kObjectBurningArrow          = 30053,
	kObjectKey                   = 30054,
	kObjectWolfBadge             = 30055,
	kObjectWolfBrooch            = 30056,
	kObjectNotung                = 30058,
	kObjectRing                  = 40012,
	kObjectCrown                 = 40013,
	kObjectMagicLance            = 50000,
	kObjectInk                   = 50101,
	kObjectPaper                 = 50102,
	kObjectStylet                = 50103,
	kObjectInkedStylet           = 50104,
	kObjectCounch                = 50200,
	kObjectDeadLeaf              = 50201,
	kObjectRopes                 = 50202,
	kObjectCloud                 = 50203,
	kObjectPhoenix               = 50300,
	kObjectCounch1               = 50301,
	kObjectBark                  = 50302,
	kObjectGolem1                = 50400,
	kObjectFeather               = 50401,
	kObjectFlower                = 50402,
	kObjectFronthead             = 50431,
	kObjectBackhead              = 50432,
	kObjectBelly                 = 50433,
	kObjectRightArm              = 50434,
	kObjectLeftArm               = 50435,
	kObjectLegs                  = 50436,
	kObjectHeart                 = 50437,
	kObjectMagnet                = 50499,
	kObjectTree                  = 50500,
	kObjectDragonSword           = 50501,
	kObjectApple                 = 50502,
	kObjectTotems                = 50503,
	kObjectThread                = 50504,
	kObjectFire                  = 70000,
	kObjectMagicFrog2            = 70302,
	kObjectCage                  = 70303,
	kObjectLogeTear3             = 70304,
	kObjectCentaur               = 70500,
	kObjectDragon                = 70501,
	kObjectPhoenix1              = 70502,
	kObjectChrysoberyl           = 70503,
	kObjectDeath                 = 80020,
	kObjectFirePower             = 40000
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
	kPuzzleMenu        = 90000,
	kPuzzlePreferences = 90001,
	kPuzzleLoad        = 90002,
	kPuzzleSave        = 90003,
	kPuzzleGameStats   = 90004,
	kPuzzleInsertCd    = 90005
};

//////////////////////////////////////////////////////////////////////////
// Types
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
