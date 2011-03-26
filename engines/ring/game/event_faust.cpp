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

#include "ring/game/event_faust.h"

#include "ring/base/application.h"

//using namespace FaustGame;

namespace Ring {

EventHandlerFaust::EventHandlerFaust(Application *application) : _app(application) {
}

EventHandlerFaust::~EventHandlerFaust() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerFaust::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerFaust::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void EventHandlerFaust::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[EventHandlerFaust::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void EventHandlerFaust::onMouseRightButtonUp(const Common::Event &evt) {
	error("[EventHandlerFaust::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void EventHandlerFaust::onKeyDown(Common::Event &evt) {
	error("[EventHandlerFaust::onKeyDown] Not implemented (evt: %d)", evt.type);
}

#pragma endregion

#pragma region Timer

void EventHandlerFaust::onTimer(TimerId timerId) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onTimerZoneSY(timerId);
		break;

	case kZone2:
		onTimerZone2(timerId);
		break;

	case kZone3:
		onTimerZone3(timerId);
		break;

	case kZone4:
		onTimerZone4(timerId);
		break;

	case kZone5:
		onTimerZone5(timerId);
		break;

	case kZone6:
		onTimerZone6(timerId);
		break;

	case kZone8:
		onTimerZone8(timerId);
		break;

	case kZone9:
		onTimerZone9(timerId);
		break;

	case kZone10:
		onTimerZone10(timerId);
		break;

	case kZone11:
		onTimerZone11(timerId);
		break;

	case kZone12:
		onTimerZone12(timerId);
		break;

	case kZone13:
		onTimerZone13(timerId);
		break;

	case kZone14:
		onTimerZone14(timerId);
		break;

	case kZone15:
		onTimerZone15(timerId);
		break;

	case kZone16:
		onTimerZone16(timerId);
		break;
	}
}

void EventHandlerFaust::onTimerZoneSY(TimerId id) {
	error("[EventHandlerFaust::onTimerZoneSY] Not implemented");
}

void EventHandlerFaust::onTimerZone2(TimerId id) {
	error("[EventHandlerFaust::onTimerZone2] Not implemented");
}

void EventHandlerFaust::onTimerZone3(TimerId id) {
	error("[EventHandlerFaust::onTimerZone3] Not implemented");
}

void EventHandlerFaust::onTimerZone4(TimerId id) {
	error("[EventHandlerFaust::onTimerZone4] Not implemented");
}

void EventHandlerFaust::onTimerZone5(TimerId id) {
	error("[EventHandlerFaust::onTimerZone5] Not implemented");
}

void EventHandlerFaust::onTimerZone6(TimerId id) {
	error("[EventHandlerFaust::onTimerZone6] Not implemented");
}

void EventHandlerFaust::onTimerZone8(TimerId id) {
	error("[EventHandlerFaust::onTimerZone8] Not implemented");
}

void EventHandlerFaust::onTimerZone9(TimerId id) {
	error("[EventHandlerFaust::onTimerZone9] Not implemented");
}

void EventHandlerFaust::onTimerZone10(TimerId id) {
	error("[EventHandlerFaust::onTimerZone10] Not implemented");
}

void EventHandlerFaust::onTimerZone11(TimerId id) {
	error("[EventHandlerFaust::onTimerZone11] Not implemented");
}

void EventHandlerFaust::onTimerZone12(TimerId id) {
	error("[EventHandlerFaust::onTimerZone12] Not implemented");
}

void EventHandlerFaust::onTimerZone13(TimerId id) {
	error("[EventHandlerFaust::onTimerZone13] Not implemented");
}

void EventHandlerFaust::onTimerZone14(TimerId id) {
	error("[EventHandlerFaust::onTimerZone14] Not implemented");
}

void EventHandlerFaust::onTimerZone15(TimerId id) {
	error("[EventHandlerFaust::onTimerZone15] Not implemented");
}

void EventHandlerFaust::onTimerZone16(TimerId id) {
	error("[EventHandlerFaust::onTimerZone16] Not implemented");
}

#pragma endregion

#pragma region Zone

void EventHandlerFaust::onSetup(ZoneId zone, SetupType type) {
	switch (zone) {
	default:
		break;

	case kZone2:
		onSetupZone2(type);
		break;

	case kZone3:
		onSetupZone3(type);
		break;

	case kZone4:
		onSetupZone4(type);
		break;

	case kZone5:
		onSetupZone5(type);
		break;

	case kZone6:
		onSetupZone6(type);
		break;

	case kZone7:
		onSetupZone7(type);
		break;

	case kZone8:
		onSetupZone8(type);
		break;
	}
}

void EventHandlerFaust::onSetupZone2(SetupType type) {
	error("[EventHandlerFaust::onSetupZone2] Not implemented");
}

void EventHandlerFaust::onSetupZone3(SetupType type) {
	error("[EventHandlerFaust::onSetupZone3] Not implemented");
}

void EventHandlerFaust::onSetupZone4(SetupType type) {
	error("[EventHandlerFaust::onSetupZone4] Not implemented");
}

void EventHandlerFaust::onSetupZone5(SetupType type) {
	error("[EventHandlerFaust::onSetupZone5] Not implemented");
}

void EventHandlerFaust::onSetupZone6(SetupType type) {
	error("[EventHandlerFaust::onSetupZone6] Not implemented");
}

void EventHandlerFaust::onSetupZone7(SetupType type) {
	error("[EventHandlerFaust::onSetupZone7] Not implemented");
}

void EventHandlerFaust::onSetupZone8(SetupType type) {
	error("[EventHandlerFaust::onSetupZone8] Not implemented");
}

#pragma endregion

} // End of namespace Ring
