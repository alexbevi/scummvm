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

#include "ring/game/faust/faust_button.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventButtonFaust::EventButtonFaust(ApplicationFaust *application) : _app(application) {
}

EventButtonFaust::~EventButtonFaust() {
	// Zero-out passed pointers
	_app   = NULL;
}

#pragma region Button Down

void EventButtonFaust::onButtonDownZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	if (id == kObject22 && target == 2) {
		_app->objectPresentationShow(kObject12, 2);
		_app->varSetByte(114101, 1);
	}
}

void EventButtonFaust::onButtonDownZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	if (id == kObject17022) {
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->objectPresentationShow(kObject17022, target);
	}
}

#pragma endregion

#pragma region Button Up

void EventButtonFaust::onButtonUpZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZoneSY] Not implemented");
}

void EventButtonFaust::onButtonUpZone2(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone2] Not implemented");
}

void EventButtonFaust::onButtonUpZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone3] Not implemented");
}

void EventButtonFaust::onButtonUpZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone4] Not implemented");
}

void EventButtonFaust::onButtonUpZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone5] Not implemented");
}

void EventButtonFaust::onButtonUpZone6(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone6] Not implemented");
}

void EventButtonFaust::onButtonUpZone7(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone7] Not implemented");
}

void EventButtonFaust::onButtonUpZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone8] Not implemented");
}

void EventButtonFaust::onButtonUpZone9(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone9] Not implemented");
}

void EventButtonFaust::onButtonUpZone10(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone10] Not implemented");
}

void EventButtonFaust::onButtonUpZone11(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone11] Not implemented");
}

void EventButtonFaust::onButtonUpZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone12] Not implemented");
}

void EventButtonFaust::onButtonUpZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone13] Not implemented");
}

void EventButtonFaust::onButtonUpZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone14] Not implemented");
}

void EventButtonFaust::onButtonUpZone15(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone15] Not implemented");
}

void EventButtonFaust::onButtonUpZone16(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone16] Not implemented");
}

void EventButtonFaust::onButtonUpZone17(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[EventButtonFaust::onButtonUpZone17] Not implemented");
}

#pragma endregion

} // End of namespace Ring
