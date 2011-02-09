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

#include "ring/game/event_ring.h"

#include "ring/game/application.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

EventHandlerRing::EventHandlerRing(Application *application) : _app(application),
	_controlNotPressed(false) {
}


EventHandlerRing::~EventHandlerRing() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerRing::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	_controlNotPressed = !isControlPressed;

	onMouseLeftButtonUp(evt);
}

void EventHandlerRing::onMouseLeftButtonUp(Common::Event &evt) {
	error("[EventHandlerRing::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerRing::onMouseLeftButtonDown(Common::Event &evt) {
	error("[EventHandlerRing::onMouseLeftButtonDown] Not implemented");
}

void EventHandlerRing::onMouseRightButtonUp(Common::Event &evt) {
	error("[EventHandlerRing::onMouseRightButtonUp] Not implemented");
}

void EventHandlerRing::onKeyDown(Common::Event &evt) {
	error("[EventHandlerRing::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Timer

void EventHandlerRing::onTimer(TimerId timerId) {
	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneWA:
		break;

	case kZoneNI:
		onZoneTimerNI(timerId);
		break;

	case kZoneRH:
		onZoneTimerRH(timerId);
		break;

	case kZoneFO:
		onZoneTimerFO(timerId);
		break;

	case kZoneRO:
		onZoneTimerRO(timerId);
		break;

	case kZoneAS:
		onZoneTimerAS(timerId);
		break;

	case kZoneN2:
		onZoneTimerN2(timerId);
		break;
	}
}

void EventHandlerRing::onZoneTimerNI(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onZoneTimerRH(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onZoneTimerFO(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onZoneTimerRO(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetByte(40806, _app->varGetByte(40806) + 1);

		if (_app->varGetByte(40806) == 10) {
			_app->timerStop(kTimer0);
			_app->varSetByte(40601, 10);
		}

		_app->objectPresentationShow(kObject40101, _app->varGetByte(40806));
		break;

	case kTimer1:
		_app->varSetByte(40807, _app->varGetByte(40807) + 1);

		if (_app->varGetByte(40807) == 90) {
			_app->timerStop(kTimer1);
			_app->varSetByte(40602, 90);
		}

		_app->objectPresentationShow(kObject40102, _app->varGetByte(40807));
		break;
	}

	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onZoneTimerAS(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onZoneTimerN2(TimerId id) {
	if (id == kTimer0)
		_app->noiceIdPlay(rnd(12) + 70004, 1);
}

#pragma endregion


} // End of namespace Ring
