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

#include "ring/game/ring/ring_zone.h"

#include "ring/base/bag.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/ring.h"

using namespace RingGame;

namespace Ring {

EventZoneRing::EventZoneRing(ApplicationRing *application) : _app(application) {
}

EventZoneRing::~EventZoneRing() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Update

void EventZoneRing::onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32, uint32, const Common::Point &) {
	switch (movabilityFrom) {
	default:
		break;

	case 2:
		switch (movabilityTo) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject2, 1);
			_app->objectPresentationShow(kObject2, 2);
			break;

		case 1:
			_app->objectPresentationHide(kObject2, 2);
			_app->objectPresentationShow(kObject2, 1);
			break;
		}
		break;

	case 3:
		_app->objectPresentationShow(kObject3, 1);
		break;

	case 4: {
		int32 val = (uint32)(movabilityTo) & 0x80000001;
		bool state = (val < 0 ? ((int32)((val - 1) | 0xFFFFFFFE)) == -1 : val == 0);

		if (state) {
			_app->objectPresentationHide(kObject4, 2);
			_app->objectPresentationShow(kObject4, 1);
		} else {
			_app->objectPresentationHide(kObject4, 1);
			_app->objectPresentationShow(kObject4, 2);
		}
		}
		break;

	case 90000:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuNewGame, 0);
		break;

	case 90001:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuPreferences, 0);
		break;

	case 90002:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuLoad, 0);
		break;

	case 90003:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuSave, 0);
		break;

	case 90005:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuStatus, 0);
		break;

	case 90004:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuContinue, 0);
		break;

	case 90006:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuExit, 0);
		break;

	case 90101:
		_app->objectPresentationShow(kObjectPreferencesCancel, 0);
		_app->objectPresentationHide(kObjectPreferencesOk, 0);
		break;

	case 90102:
		_app->objectPresentationHide(kObjectPreferencesCancel, 0);
		_app->objectPresentationShow(kObjectPreferencesOk, 0);
		break;

	case 90104:
		_app->objectPresentationShow(kObjectPreferences3dSound, 2);
		break;

	case 90207:
		_app->objectPresentationHide(kObjectLoadOk, 0);
		_app->objectPresentationShow(kObjectLoadCancel, 0);
		break;

	case 90208:
		_app->objectPresentationShow(kObjectLoadOk, 0);
		_app->objectPresentationHide(kObjectLoadCancel, 0);
		break;

	case 90401:
		_app->objectPresentationShow(kObjectStatusOk, 0);
		break;

	case 90309:
		_app->objectPresentationShow(kObjectSaveOk, 0);
		_app->objectPresentationHide(kObjectSaveCancel, 0);
		break;

	case 90310:
		_app->objectPresentationHide(kObjectSaveOk, 0);
		_app->objectPresentationShow(kObjectSaveCancel, 0);
		break;

	case 90912:
		_app->objectPresentationShow(kObject90912, 0);
		break;
	}
}

void EventZoneRing::onUpdateBeforeZoneNI(Id movabilityFrom, Id movabilityTo, uint32, uint32, const Common::Point &point) {
	if (movabilityFrom == 10100 && movabilityTo == 0) {
		_app->rotationSetActive(10101);
		_app->objectSetAccessibilityOn(kObject10101, 0, 0);
		_app->objectSetAccessibilityOn(kObject10102, 0, 0);
		_app->objectSetAccessibilityOn(kObject10101, 2, 2);
		_app->objectSetAccessibilityOn(kObject10102, 2, 2);
		_app->objectSetAccessibilityOn(kObject10100, 4, 4);

		g_system->warpMouse(point.x, point.y);
	}
}

void EventZoneRing::onUpdateBeforeZoneN2(Id movabilityFrom, Id movabilityTo, uint32, uint32, const Common::Point &point) {
	if (movabilityFrom == 70100 && movabilityTo == 0) {
		_app->rotationSetActive(70100);
		_app->objectSetAccessibilityOn(kObject70101, 0, 0);
		_app->objectSetAccessibilityOn(kObject70102, 0, 0);
		_app->objectSetAccessibilityOn(kObject70101, 2, 2);
		_app->objectSetAccessibilityOn(kObject70102, 2, 2);
		_app->objectSetAccessibilityOn(kObject70100, 4, 4);

		g_system->warpMouse(point.x, point.y);
	}
}

#pragma endregion

#pragma region Switch

void EventZoneRing::onSwitchZoneNI(uint32 type) {
	switch (type) {
	default:
		break;

	case 0:
		if (_app->varGetByte(90009))
			_app->setupZone((ZoneId)_app->varGetDword(90013), kSetupType10);
		else
			_app->setupZone(kZoneNI, kSetupTypeNone);
		break;

	case 3:
		_app->setupZone(kZoneNI, kSetupType3);
		break;

	case 9:
		_app->timerStart(kTimer9, 1000);
		break;
	}
}

void EventZoneRing::onSwitchZoneRH(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRH, kSetupTypeNone);
}

void EventZoneRing::onSwitchZoneFO(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90011))
			_app->setupZone((ZoneId)_app->varGetDword(90015), kSetupType10);
		else
			_app->setupZone(kZoneRO, kSetupTypeNone);
	}
}

void EventZoneRing::onSwitchZoneRO(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRO, kSetupTypeNone);
}

void EventZoneRing::onSwitchZoneAS(uint32 type) {
	switch (type) {
	default:
		break;

	case 1:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 0, 0);
		_app->varSetByte(90001, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 2);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80040);
		_app->objectSetAccessibilityOff(kObject80019, 0, 0);
		_app->objectSetAccessibilityOn(kObject80019, 1, 1);
		break;

	case 2:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 1, 1);
		_app->varSetByte(90002, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 3);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80049);
		break;

	case 3:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->soundSetMultiplier(kSoundTypeAmbientMusic, 100);
		_app->objectSetAccessibilityOn(kObject80018, 2, 2);
		_app->varSetByte(90003, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);


		_app->objectPresentationShow(kObject80018, 4);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80058);
		break;

	case 4:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 3, 3);
		_app->varSetByte(90004, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 5);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80068);
		break;

	case 5:
		_app->setupZone(kZoneAS, kSetupType5);
		break;
	case 13:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->setZoneAndEnableBag(kZoneAS);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);

		_app->getBag()->reset();

		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void EventZoneRing::onSwitchZoneWA(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90012))
			_app->setupZone((ZoneId)_app->varGetDword(90016), kSetupType10);
		else
			_app->setupZone(kZoneWA, kSetupTypeNone);
	}
}

void EventZoneRing::onSwitchZoneN2(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90010))
			_app->setupZone((ZoneId)_app->varGetDword(90014), kSetupType10);
		else
			_app->setupZone(kZoneN2, kSetupTypeNone);
	}
}

#pragma endregion

} // End of namespace Ring
