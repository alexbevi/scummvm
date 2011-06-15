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

#include "ring/game/ring/event_timer.h"

#include "ring/base/accessibility.h"
#include "ring/base/bag.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"

#include "ring/game/ring/event.h"
#include "ring/game/ring/ring.h"
#include "ring/game/ring/shared.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"

#include "ring/sound/soundhandler.h"
#include "ring/sound/soundmanager.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/textconsole.h"

using namespace RingGame;

namespace Ring {

EventTimerRing::EventTimerRing(ApplicationRing *application, EventHandlerRing *eventHandler) : _app(application), _event(eventHandler) {
	_disableTimerRH = false;
	_frameNumberTimerRH = 0;
}

EventTimerRing::~EventTimerRing() {
	// Zero-out passed pointers
	_app   = NULL;
	_event = NULL;
}


void EventTimerRing::onTimerZoneNI(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0: {
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10432) < 70) {
			_app->objectPresentationHide(kObject10431);
			_app->objectPresentationShow(kObject10431, ((uint8)_app->varGetByte(10432) - 10) / 5);
		} else if (_app->varGetByte(10432) == 100) {
			_app->objectPresentationShow(kObject10431, 12);
		}

		uint32 mod = (uint8)(120 - _app->varGetByte(10432)) / 10;
		if (mod < 5)
			mod = 5;

		if (!((uint8)(_app->varGetByte(10432)) % mod)) {
			_app->soundPlay(10415);
			_app->objectPresentationShow(kObject10432, rnd(2) + 1);
		}

		uint32 volume = (uint8)_app->varGetByte(10432) / 5 + 80;
		if (volume > 100)
			volume = 100;

		_app->soundSetVolume(10412, volume);

		if (_app->varGetByte(10412) > 120) {
			_app->timerStop(kTimer0);
			_app->soundStopType(kSoundTypeAmbientEffect, 1024);
			_app->soundStopType(kSoundTypeBackgroundMusic, 1024);
			_app->playMovie("1538");
			_app->exitToMenu(kMenuAction4);
		}
		}
		break;

	case kTimer1:
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10303) == 1) {
			_app->timerStopAll();
			_app->soundStopAll(1024);
			_app->varSetFloat(90005, 100.0f);
			_app->playMovie("1539");
			_event->onSwitchZone(kZoneAS, 1);
		}
		break;
	}
}

void EventTimerRing::onTimerZoneRH(TimerId id) {
	if (id != kTimer0)
		return;

	// FIXME: Since we are called synchronously, is this needed? If so, replace by mutex?
	if (_disableTimerRH)
		return;

	_disableTimerRH = true;

	float alp = _app->rotationGetAlp(20401) - 35.0f;
	if (alp > 0.0f && alp < 146.0f) {
		uint32 frame = (uint32)(alp * 0.2631578947368421f);

		if (frame >= 1 && frame <= 28 && _frameNumberTimerRH != frame) {
			_app->objectPresentationSetAnimationActiveFrame(20401, 0, frame);
			_frameNumberTimerRH = frame;
		}
	}

	_disableTimerRH = false;
}

void EventTimerRing::onTimerZoneFO(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		switch (_app->varGetByte(30073)) {
		default:
			break;

		case 0:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 1);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 1:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 2);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 10);
			break;

		case 2:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 3);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 3:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 0);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 1000 * (rnd(10) + 15));
			break;
		}
		break;

	case kTimer1: {
		Id soundId = rnd(15) + 30301;
		_app->soundSetPan(soundId, 10 - rnd(20));
		_app->soundPlay(soundId);

		_app->timerStop(kTimer1);
		_app->timerStart(kTimer1, 500 * (rnd(10) + 10));
		}
		break;

	case kTimer2:
		_app->objectPresentationUnpauseAnimation(kObjectSleepingPotion, 0);
		_app->timerStop(kTimer2);
		break;

	case kTimer3: {
		Id soundId = rnd(3) + 30316;
		_app->soundSetPan(soundId, 95);
		_app->soundPlay(soundId);

		_app->timerStop(kTimer3);
		_app->timerStart(kTimer3, 500 * (rnd(10) + 30));
		}
		break;

	case kTimer4: {
		Id soundId = rnd(9) + 30319;
		_app->soundSetPan(soundId, 5 - rnd(10));
		_app->soundPlay(soundId);

		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, 4000 * (rnd(10) + 5));
		}
		break;

	case kTimer5:
		_app->objectPresentationUnpauseAnimation(kObject30110, 1);
		_app->objectPresentationUnpauseAnimation(kObject30109, 0);
		_app->objectSetAccessibilityOff(kObject30109, 0, 0);
		_app->timerStop(kTimer5);
		_app->timerStart(kTimer5, 2000 * rnd(10) + 15000);
		break;
	}
}

void EventTimerRing::onTimerZoneRO(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetByte(40806, _app->varGetByte(40806) + 1);

		if (_app->varGetByte(40806) == 10) {
			_app->timerStop(kTimer0);
			_app->varSetByte(40601, 10);
		}

		_app->objectPresentationShow(kObject40101, (uint8)_app->varGetByte(40806));
		break;

	case kTimer1:
		_app->varSetByte(40807, _app->varGetByte(40807) + 1);

		if (_app->varGetByte(40807) == 90) {
			_app->timerStop(kTimer1);
			_app->varSetByte(40602, 90);
		}

		_app->objectPresentationShow(kObject40102, (uint8)_app->varGetByte(40807));
		break;
	}
}

void EventTimerRing::onTimerZoneAS(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer2:
		_app->soundPlay(80018);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 20);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer3:
		_app->soundPlay(80019);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 30);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer4:
		_app->soundPlay(80020);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 10);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer5:
		_app->varSetByte(80001, _app->varGetByte(80001) + 1);

		_app->getCurrentRotation()->setBet(_app->getCurrentRotation()->getBet() + _app->varGetFloat(80001) * _app->varGetFloat(80002));
		_app->getCurrentRotation()->setAlp(_app->getCurrentRotation()->getAlp() + _app->varGetFloat(80001) * _app->varGetFloat(80002) * 0.5f);

		// Update values
		_app->varSetFloat(80001, _app->varGetFloat(80001) * -1.0f);
		_app->varSetFloat(80002, _app->varGetFloat(80002) * 5.0f / 6.0f);

		if (_app->varGetByte(80001) > 50) {
			_app->varSetByte(80001, 0);
			_app->varSetFloat(80002, 2.0f);

			_app->timerStop(kTimer5);
			_app->timerStop(kTimer6);

			_app->objectPresentationHide(kObject80016);
		}
		break;

	case kTimer6: {
		int32 val = rnd(10) & 0x80000001;

		bool show = (val == 0);
		if (val < 0)
			show = (((val - 1) | -2) == -1);

		if (show)
			_app->objectPresentationShow(kObject80016);
		else
			_app->objectPresentationHide(kObject80016);
		}
		break;
	}
}

void EventTimerRing::onTimerZoneN2(TimerId id) {
	if (id == kTimer0)
		_app->soundPlay(rnd(12) + 70004);
}

} // End of namespace Ring
