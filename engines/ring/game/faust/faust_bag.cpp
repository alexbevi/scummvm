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

#include "ring/game/faust/faust_bag.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventBagFaust::EventBagFaust(ApplicationFaust *application) : _app(application) {
}

EventBagFaust::~EventBagFaust() {
	// Zero-out passed pointers
	_app   = NULL;
}

#pragma region Event

void EventBagFaust::onBagZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZoneSY] Not implemented");
}

void EventBagFaust::onBagZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZone3] Not implemented");
}

void EventBagFaust::onBagZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZone4] Not implemented");
}

void EventBagFaust::onBagZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZone5] Not implemented");
}

void EventBagFaust::onBagZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZone8] Not implemented");
}

void EventBagFaust::onBagZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZone12] Not implemented");
}

void EventBagFaust::onBagZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZone13] Not implemented");
}

void EventBagFaust::onBagZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[EventBagFaust::onBagZone14] Not implemented");
}

#pragma endregion

#pragma region Clicked

void EventBagFaust::onBagClickedObjectZone2(ObjectId id) {
	if (id == kObjectHomunculus)
		if (_app->varGetByte(11002) == 1)
			_app->objectSetAccessibilityOn(kObject11004, 2, 2);
}

void EventBagFaust::onBagClickedObjectZone3(ObjectId id) {
	if (id == kObjectStrainer) {
		if (_app->hasCurrentPuzzle() && _app->getCurrentPuzzleId() == kPuzzle12001) {
			_app->objectPresentationShow(kObjectStrainer, 1);
			_app->objectSetAccessibilityOn(kObjectStrainer, 1, 1);
			_app->cursorDelete();
		}
	}
}

void EventBagFaust::onBagClickedObjectZone4(ObjectId id) {
	if (id == kObjectMatches)
		_app->soundPlay(13033);
}

void EventBagFaust::onBagClickedObjectZone8(ObjectId id) {
	switch (id) {
	default:
		break;

	case kObjectMatches2:
		_app->soundPlay(53125);
		break;

	case kObjectTeddyBear:
		if (_app->getCurrentPuzzleId() != kPuzzle53110) {
			_app->puzzleSetMod(kPuzzleMenu, 2, 17);
			_app->objectPresentationShow(kObject17);
			_app->objectSetAccessibilityOn(kObject17);
		}
		break;
	}
}

void EventBagFaust::onBagClickedObjectZone14(ObjectId id) {
	switch (id) {
	default:
		break;

	case kObjectPipe:
		if (!_app->varGetByte(114522)) {
			_app->cursorDelete();
			_app->puzzleSetMod(kPuzzleMenu, 2, 21);
			_app->objectSetAccessibilityOff(20, 13, 13);
			_app->objectSetAccessibilityOff(kObject114503);
			_app->objectPresentationHide(kObject11);
			_app->objectPresentationShow(kObject11, 16);
			_app->objectSetAccessibilityOn(kObjectPipe, 0, 0);
			_app->objectSetAccessibilityOn(kObjectPipe, 2, 2);
			_app->varSetByte(114522, 1);
		}
		break;

	case kObjectCodingWheel:
		if (!_app->varGetByte(114520)) {
			_app->cursorDelete();
			_app->puzzleSetMod(kPuzzleMenu, 2, 23);
			_app->objectPresentationShow(kObjectCodingWheel, 35 - _app->varGetByte(114521));
			_app->objectSetAccessibilityOn(kObject23);
			_app->varSetByte(114520, 1);
		}
		break;

	case kObjectPaperKnife:
		if (_app->varGetByte(114100) > 0) {
			if (_app->soundIsPlaying(112016)) {
				_app->sub_45FF30();
			} else {
				_app->objectPresentationShow(kObject12, 1);
				_app->cursorDelete();
				_app->objectSetAccessibilityOff(kObject22);
				_app->objectSetAccessibilityOn(kObject22, 1, 2);
				_app->soundPlay(112016, kSoundLoop);
				_app->soundSetVolume(112016, 1);
			}
		}
		break;
	}
}

void EventBagFaust::onBagClickedObjectZone15(ObjectId id) {
	if (id == kObjectMatches4)
		_app->soundPlay(120022);
}

#pragma endregion

#pragma region Update

void EventBagFaust::onUpdateBagZoneSY(const Common::Point &point) {
	if (_app->hasCurrentPuzzle() && _app->getCurrentPuzzleId() == kPuzzleMenu) {
		_app->objectPresentationHide(kObject14, 2);
		_app->objectPresentationHide(kObject14, 4);
		_app->objectPresentationHide(kObject14, 6);
	}

	_app->objectPresentationHide(kObject99091, 1);
	_app->objectPresentationHide(kObject99091, 2);
	_app->objectPresentationHide(kObject99091, 3);
	_app->objectPresentationHide(kObject2, 2);
	_app->objectPresentationHide(kObject2, 1);
	_app->objectPresentationHide(kObject4, 1);
	_app->objectPresentationHide(kObject4, 2);
	_app->objectPresentationHide(kObject3, 1);
	_app->objectPresentationHide(kObject99500);
	_app->objectPresentationHide(kObject99010, 2);
	_app->objectPresentationHide(kObject99010, 4);
	_app->objectPresentationHide(kObject99000, 0);

	if (!_app->varGetByte(98003))
		_app->objectPresentationHide(kObject99001, 0);

	if (!_app->varGetByte(98004))
		_app->objectPresentationHide(kObject99002, 0);

	if (!_app->varGetByte(98005))
		_app->objectPresentationHide(kObject99003, 0);

	if (!_app->varGetByte(98006))
		_app->objectPresentationHide(kObject99004, 0);

	if (!_app->varGetByte(98007))
		_app->objectPresentationHide(kObject99005, 0);

	if (!_app->varGetByte(98008))
		_app->objectPresentationHide(kObject99006, 0);

	if (!_app->varGetByte(98009))
		_app->objectPresentationHide(kObject99007, 0);

	if (!_app->varGetByte(98010))
		_app->objectPresentationHide(kObject99008, 0);

	if (!_app->varGetByte(98011))
		_app->objectPresentationHide(kObject99009, 0);

	_app->objectPresentationHide(kObject99023, 0);
	_app->objectPresentationHide(kObject99022, 0);
	_app->objectPresentationHide(kObject99021, 0);
	_app->objectPresentationHide(kObject99025, 0);
	_app->objectPresentationHide(kObject99024, 0);
	_app->objectPresentationHide(kObject99042, 0);
	_app->objectPresentationHide(kObject99041, 0);
	_app->objectPresentationHide(kObject99097, 0);
	_app->objectPresentationHide(kObject55, 0);
	_app->objectPresentationHide(kObject55, 2);
}

void EventBagFaust::onUpdateBagZone3(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(12001), 1024);
	_app->varSetDword(12001, 0);
}

void EventBagFaust::onUpdateBagZone4(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(13001), 1024);
	_app->varSetDword(13001, 0);
}

void EventBagFaust::onUpdateBagZone6(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(31000), 1024);
	_app->varSetDword(31000, 0);
}

void EventBagFaust::onUpdateBagZone12(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(81001), 1024);
	_app->varSetDword(81001, 0);
}

void EventBagFaust::onUpdateBagZone14(const Common::Point &point) {
	if (_app->varGetByte(112023)) {
		_app->soundStop(111000 + _app->varGetByte(112023), 1024);
		_app->varSetByte(112023, 0);
	}
}

void EventBagFaust::onUpdateBagZone15(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(120006), 1024);
	_app->varSetDword(120006, 0);

	_app->objectPresentationHide(kObject120058);
}

void EventBagFaust::onUpdateBagZone16(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(311001), 1024);
	_app->varSetDword(311001, 0);
}

#pragma endregion

} // End of namespace Ring
