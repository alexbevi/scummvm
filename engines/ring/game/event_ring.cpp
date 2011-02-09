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

#pragma region Zone Setup

void EventHandlerRing::onSetup(Zone zone, uint32 a2) {
	switch (zone) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onSetupZoneNI(a2);
		break;

	case kZoneRH:
		onSetupZoneRH(a2);
		break;

	case kZoneFO:
		onSetupZoneFO(a2);
		break;

	case kZoneRO:
		onSetupZoneRO(a2);
		break;

	case kZoneWA:
		onSetupZoneWA(a2);
		break;

	case kZoneAS:
		onSetupZoneAS(a2);
		break;

	case kZoneN2:
		onSetupZoneN2(a2);
		break;
	}
}

void EventHandlerRing::onSetupZoneNI(uint32 a1) {
	error("[EventHandlerRing::onSetupZoneNI] Not implemented");
}

void EventHandlerRing::onSetupZoneRH(uint32 a1) {
	error("[EventHandlerRing::onSetupZoneRH] Not implemented");
}

void EventHandlerRing::onSetupZoneFO(uint32 a1) {
	error("[EventHandlerRing::onSetupZoneFO] Not implemented");
}

void EventHandlerRing::onSetupZoneRO(uint32 a1) {
	error("[EventHandlerRing::onSetupZoneRO] Not implemented");
}

void EventHandlerRing::onSetupZoneWA(uint32 a1) {
	error("[EventHandlerRing::onSetupZoneWA] Not implemented");
}

void EventHandlerRing::onSetupZoneAS(uint32 a1) {
	error("[EventHandlerRing::onSetupZoneAS] Not implemented");
}

void EventHandlerRing::onSetupZoneN2(uint32 a1) {
	error("[EventHandlerRing::onSetupZoneN2] Not implemented");
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
		onTimerZoneNI(timerId);
		break;

	case kZoneRH:
		onTimerZoneRH(timerId);
		break;

	case kZoneFO:
		onTimerZoneFO(timerId);
		break;

	case kZoneRO:
		onTimerZoneRO(timerId);
		break;

	case kZoneAS:
		onTimerZoneAS(timerId);
		break;

	case kZoneN2:
		onTimerZoneN2(timerId);
		break;
	}
}

void EventHandlerRing::onTimerZoneNI(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneRH(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneFO(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneRO(TimerId id) {
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

void EventHandlerRing::onTimerZoneAS(TimerId id) {
	error("[EventHandlerRing::onZoneTimerNI] Not implemented");
}

void EventHandlerRing::onTimerZoneN2(TimerId id) {
	if (id == kTimer0)
		_app->noiceIdPlay(rnd(12) + 70004, 1);
}

#pragma endregion

#pragma region Sound

void EventHandlerRing::onSound(Id id, SoundType type, uint32 a3) {
	int a4 = a3 & 0x1000;
	a3 &= 239;

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onSoundZoneSY(id, type, a3, a4);
		break;

	case kZoneNI:
		onSoundZoneNI(id, type, a3, a4);
		break;

	case kZoneRH:
		onSoundZoneRH(id, type, a3, a4);
		break;

	case kZoneFO:
		onSoundZoneFO(id, type, a3, a4);
		break;

	case kZoneRO:
		onSoundZoneRO(id, type, a3, a4);
		break;

	case kZoneAS:
		onSoundZoneAS(id, type, a3, a4);
		break;

	case kZoneWA:
		onSoundZoneWA(id, type, a3, a4);
		break;

	case kZoneN2:
		onSoundZoneN2(id, type, a3, a4);
		break;
	}
}

void EventHandlerRing::onSoundZoneSY(Id id, SoundType type, uint32 a3, uint32 a4) {
	if (a4 && id == 90001) {
		_app->objectPresentationHideAndRemove(kObject7, 0);
		_app->showCredits();
		_app->startMenu(false);
	}
}

void EventHandlerRing::onSoundZoneNI(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneNI] Not implemented");
}

void EventHandlerRing::onSoundZoneRH(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneRH] Not implemented");
}

void EventHandlerRing::onSoundZoneFO(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneFO] Not implemented");
}

void EventHandlerRing::onSoundZoneRO(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneRO] Not implemented");
}

void EventHandlerRing::onSoundZoneWA(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneWA] Not implemented");
}

void EventHandlerRing::onSoundZoneAS(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneAS] Not implemented");
}

void EventHandlerRing::onSoundZoneN2(Id id, SoundType type, uint32 a3, uint32 a4) {
	error("[EventHandlerRing::onSoundZoneN2] Not implemented");
}

#pragma endregion

} // End of namespace Ring
