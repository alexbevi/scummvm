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

#include "ring/game/faust/faust_ride.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/helpers.h"
#include "ring/ring.h"

using namespace FaustGame;

namespace Ring {

EventRideFaust::EventRideFaust(ApplicationFaust *application) : _app(application) {
}

EventRideFaust::~EventRideFaust() {
	// Zero-out passed pointers
	_app   = NULL;
}

#pragma region Before

void EventRideFaust::onBeforeRideZone2(Id, Id, uint32, uint32, MovabilityType) {
	_app->objectPresentationHide(kObject25);
}

void EventRideFaust::onBeforeRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityTo == 13611) {
			if (movabilityFrom == 13531)
				_app->timerStop(kTimer4);
		} else if (movabilityTo == 13531) {
			if (movabilityTo == 13611)
				_app->timerStart(kTimer4, rnd(4000));
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (movabilityFrom == 13902)
			_app->sub_4A2CF0();
		break;
	}
}

void EventRideFaust::onBeforeRideZone5(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onBeforeRideZone5] Not implemented");
}

void EventRideFaust::onBeforeRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	_app->objectPresentationHide(kObject25);

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom == 52021 && movabilityTo == 52220)
			if (_app->varGetByte(52230) == 1)
				_app->soundPlay(52212);
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityFrom == 53110 && movabilityTo == 53011)
			if (_app->varGetByte(531101) == 1)
				_app->soundPlay(53218);
		break;
	}
}

void EventRideFaust::onBeforeRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onBeforeRideZone14] Not implemented");
}

void EventRideFaust::onBeforeRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityPuzzleToRotation) {
		if (movabilityFrom == 120006 && movabilityTo == 120402) {
			if (_app->varGetByte(120013) == 1 && _app->varGetByte(120003) == 1) {
				_app->fadeOut(15, Color(0, 0, 0), 0);
				_app->displayFade("a12_black.bmp", "a12s01_e04m10_S.bmp", 12, 0, kLoadFromCd, kArchiveArt);
				_app->playMovie("2130");
				_app->displayFade("a12s01_e04m10_E.bmp", "a12_black.bmp", 12, 0, kLoadFromCd, kArchiveArt);
				_app->varSetByte(98088, 0);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);
				_app->exitToMenu(kMenuAction0);
			}
		}
	}
}

#pragma endregion

#pragma region After

void EventRideFaust::onAfterRideZone2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone2] Not implemented");
}

void EventRideFaust::onAfterRideZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone3] Not implemented");
}

void EventRideFaust::onAfterRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone4] Not implemented");
}

void EventRideFaust::onAfterRideZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone6] Not implemented");
}

void EventRideFaust::onAfterRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone8] Not implemented");
}

void EventRideFaust::onAfterRideZone10(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	_app->soundPlay(71009);

	if (movabilityType == kMovabilityRotationToRotation) {
		switch (movabilityFrom) {
		default:
			break;

		case 71011:
			if (movabilityTo == 71008)
				_app->soundPlay(71010);
			break;

		case 71008:
			if (movabilityTo == 71011)
				_app->soundPlay(71010);
			break;
		}
	}
}

void EventRideFaust::onAfterRideZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone12] Not implemented");
}

void EventRideFaust::onAfterRideZone13(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone13] Not implemented");
}

void EventRideFaust::onAfterRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone14] Not implemented");
}

void EventRideFaust::onAfterRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone15] Not implemented");
}

void EventRideFaust::onAfterRideZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityRotationToPuzzle && movabilityFrom == 310001 && movabilityTo == 311251) {
		_app->soundPlay(310103, kSoundLoop);
		_app->timerStart(kTimer0, 30);
	}
}

void EventRideFaust::onAfterRideZone17(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityRotationToRotation) {
		switch (movabilityFrom) {
		default:
			break;

		case 141102:
			if (movabilityTo == 141201)
				_app->soundPlay(142004);
			break;

		case 141201:
		case 141202:
			_app->soundPlay(142004);
			break;
		}
	}
}

#pragma endregion

} // End of namespace Ring
