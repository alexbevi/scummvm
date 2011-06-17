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

#include "ring/game/faust/faust_timer.h"

#include "ring/base/saveload.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/helpers.h"
#include "ring/ring.h"

using namespace FaustGame;

namespace Ring {

EventTimerFaust::EventTimerFaust(ApplicationFaust *application) : _app(application) {
}

EventTimerFaust::~EventTimerFaust() {
	// Zero-out passed pointers
	_app   = NULL;
}

#pragma region Timers

void EventTimerFaust::onTimerZoneSY(TimerId id) {
	if (id == kTimer12) {
		_app->timerStop(kTimer12);
		_app->varSetByte(97006, 1);
		_app->objectPresentationHide(kObject14, 7);
		_app->objectPresentationShow(kObject14, 8);
	}
}

void EventTimerFaust::onTimerZone2(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->timerStart(kTimer0, 8000 + rnd(5000));
		_app->soundPlay(11102 + rnd(2));
		break;

	case kTimer1:
		_app->timerStop(kTimer1);
		_app->soundSetVolume(11106 + rnd(7), 80);
		_app->soundPlay(1);
		break;
	}
}

void EventTimerFaust::onTimerZone3(TimerId id) {
	error("[EventTimerFaust::onTimerZone3] Not implemented");
}

void EventTimerFaust::onTimerZone4(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->objectPresentationHide(kObject13121, 4);
		_app->timerStop(kTimer0);
		break;

	case kTimer1:
		if (_app->varGetByte(13019) == 5)
			_app->timerStop(kTimer1);

		_app->soundPlay(_app->varGetByte(13019) + 13322);
		_app->varSetByte(13019, _app->varGetByte(13019) + 1);
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->soundPlay(13327);
		break;

	case kTimer3:
		_app->varSetByte(13900, _app->varGetByte(13900) + 1);
		_app->varGetByte(13900);
		if (_app->varGetByte(13900) == 5)
			_app->timerStop(kTimer3);
		break;

	case kTimer4:
		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, rnd(5000) + 10000);
		_app->objectPresentationShow(kObject13140);
		_app->timerStart(kTimer5, 100);
		_app->soundPlay(13072);
		break;

	case kTimer5:
		_app->timerStop(kTimer5);
		_app->objectPresentationHide(kObject13140);
		break;
	}
}

void EventTimerFaust::onTimerZone5(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->soundStop(17047, 1024);
		_app->soundPlay(17044);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->puzzleSetActive(kPuzzle17022);
		break;

	case kTimer3:
		if (_app->varGetByte(17030) == 5) {
			_app->varSetByte(17030, 0);
			_app->timerStop(kTimer3);

			if (_app->varGetByte(17041) == 4) {
				_app->varSetByte(17102, 1);

				if ((_app->varGetByte(17101) + _app->varGetByte(17102)) >= 2) {
					_app->playMovie("1356");
					_app->fadeOut(15, Color(0, 0, 0), 0);

					if (_app->getSaveManager()->getData()->progressState == 5 )
						_app->getSaveManager()->getData()->progress++;

					_app->setupRotation(2);
				}
			}

			_app->objectSetAccessibilityOn(kObjectHandle, 2, 2);

		} else {
			_app->varSetByte(17030, _app->varGetByte(17030) + 1);

			uint32 index = _app->varGetByte(_app->varGetByte(17030) + 17030) + 1;
			_app->objectPresentationSetAnimationOnPuzzle(kObjectHandle, index, 0, 17006);
			_app->objectPresentationShow(kObjectHandle, index);
			_app->soundPlay(17056);
		}
		break;

	case kTimer4: {
		_app->timerStop(kTimer4);
		_app->objectSetAccessibilityOff(17301, 1, 5);

		uint32 index = _app->varGetByte(_app->varGetByte(17030) + 17030);
		_app->objectPresentationUnpauseAnimation(17303, index + 1);
		_app->objectPresentationShow(kObjectHandle, index + 6);
		_app->objectPresentationHide(kObjectHandle, index + 1);
		_app->soundPlay(17055);
		}
		break;

	case kTimer5:
		if (rnd(2) == 0)
			_app->soundPlay(17041 + rnd(3));
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->soundStop(17047, 1024);
		_app->soundPlay(17044);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}

		_app->varSetByte(17042, 1);
		break;

	case kTimer7:
		_app->timerStop(kTimer7);
		_app->timerStart(kTimer7, rnd(5000) + 8000);
		_app->soundPlay(17020);
		break;

	case kTimer8:
		_app->timerStop(kTimer8);
		_app->soundStop(17047, 1024);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}
		break;
	}
}

void EventTimerFaust::onTimerZone6(TimerId id) {
	error("[EventTimerFaust::onTimerZone6] Not implemented");
}

void EventTimerFaust::onTimerZone8(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->timerStart(kTimer0, rnd(5000) + 2000);
		_app->soundPlay(51003 + rnd(4));
		break;

	case kTimer1:
		_app->soundPlay(52211, kSoundLoop);
		_app->objectPresentationShow(kObject52231, 0);
		_app->timerStop(kTimer1);
		break;

	case kTimer3:
		_app->timerStop(kTimer3);

		if (!_app->varGetByte(53124)) {
			_app->objectPresentationShow(kObject53121, 1);
			_app->varSetByte(53124, 1);
			_app->puzzleSetMovabilityOff(53102, 0, 0);
		}

		_app->timerStart(kTimer4, 10);

		if (_app->varGetByte(53000) == 1) {
			_app->soundPlay(53122, kSoundLoop);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer4:
		if (_app->varGetByte(53123) >= 100) {
			_app->timerStop(kTimer4);
		} else {
			_app->varSetByte(53123, _app->varGetByte(53123) + 5);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer5:
		if (_app->varGetByte(53123) <= _app->varGetByte(53126)) {
			_app->timerStop(kTimer5);
			_app->soundStop(53122, 1024);
			_app->varSetByte(53123, 50);
		} else {
			_app->varSetByte(53123, _app->varGetByte(53123) - 5);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);

		_app->timerStart(kTimer6, rnd(30000) + 30000);

		if (_app->varGetByte(53103)) {
			_app->rotationSet3DSoundOff(53011, 53102);
			_app->rotationSet3DSoundOff(53021, 53102);
			_app->soundPlay(53104);
			_app->soundSetVolume(53104, 90);
			_app->soundStop(53103, 1024);
			_app->varSetByte(53103, 0);
		} else {
			_app->rotationSet3DSoundOn(53011, 53102);
			_app->rotationSet3DSoundOn(53021, 53102);
			_app->soundPlay(53101);
			_app->soundPlay(53103, kSoundLoop);
			_app->soundSetVolume(53103, 90);
			_app->soundSetVolume(53101, 90);
			_app->varSetByte(53103, 1);
		}
		break;
	}
}

void EventTimerFaust::onTimerZone9(TimerId id) {
	error("[EventTimerFaust::onTimerZone9] Not implemented");
}

void EventTimerFaust::onTimerZone10(TimerId id) {
	if (id == kTimer0) {
		if (rnd(5) == 3) {
			_app->objectPresentationHide(kObject71201, 2);
			_app->objectPresentationShow(kObject71201, 0);

			handleEvents();

			_app->objectPresentationHide(kObject71201, 0);
			_app->objectPresentationShow(kObject71201, 2);
		}
	}
}

void EventTimerFaust::onTimerZone11(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0: {
		_app->timerStop(kTimer0);
		_app->timerStart(kTimer0, rnd(2000) + 500);

		uint32 index = rnd(6);
		_app->objectPresentationShow(kObject72000, index);
		_app->soundPlay(72006 + index);
		}
		break;

	case kTimer1:
		_app->timerStop(kTimer1);
		_app->objectPresentationHideAndRemove(kObject8);
		_app->objectPresentationHideAndRemove(kObject9);
		_app->objectPresentationHideAndRemove(kObject10);
		_app->exitToMenu(kMenuAction0);
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->objectPresentationHideAndRemove(kObject8);
		_app->objectPresentationHideAndRemove(kObject9);
		_app->objectPresentationHideAndRemove(kObject10);
		_app->exitToMenu(kMenuAction1);
		break;

	case kTimer3:
		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject10, 0, Common::Point(_app->varGetWord(72000), 18));
		_app->varSetWord(72000, _app->varGetWord(72000) + 1);

		if (_app->varGetWord(72000) == 434) {
			_app->timerStop(kTimer3);
			_app->objectPresentationHideAndRemove(kObject8);
			_app->objectPresentationHideAndRemove(kObject9);
			_app->objectPresentationHideAndRemove(kObject10);
			_app->exitToMenu(kMenuAction1);
		}
		break;

	case kTimer5:
		_app->soundPlay(72013, kSoundLoop);
		_app->rotationSet3DSoundOn(72014, 72014 + rnd(3));
		_app->timerStop(kTimer5);
		break;
	}
}

void EventTimerFaust::onTimerZone12(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		if (_app->varGetFloat(81001) == 0.0f) {
			_app->timerStop(kTimer0);

			if (!_app->hasTimer(kTimer1)) {
				_app->objectSetAccessibilityOn(kObject81201);
				_app->objectSetAccessibilityOn(kObject81202);
				_app->rotationSetAlp(82021, 152.0f);
				_app->rotationSetBet(82021, 0);
				_app->rotationSetRan(82021, 87.0f);
				_app->rotationSetActive(82021);
				_app->soundStop(81204, 1024);
				_app->objectPresentationShow(kObject81221, 1);
			}
		} else {
			_app->varSetFloat(81001, _app->varGetFloat(81001) - 1.0f);
			if (_app->varGetFloat(81001) < 0.0f)
				_app->varSetFloat(81001, 0);

			_app->objectPresentationHide(kObject81203);
			_app->objectPresentationShow(kObject81203, (uint32)_app->varGetFloat(81001));

			if (_app->varGetFloat(81001) != 25.0f
				&& _app->varGetFloat(81001) != 50.0f)
				break;

			_app->soundPlay(81203);
		}
		break;

	case kTimer1:
		if (_app->varGetFloat(81002) == 10.0f) {
			_app->timerStop(kTimer1);

			if (!_app->hasTimer(kTimer0)) {
				_app->objectSetAccessibilityOn(kObject81201);
				_app->objectSetAccessibilityOn(kObject81202);
				_app->rotationSetAlp(82021, 152.0f);
				_app->rotationSetBet(82021, 0);
				_app->rotationSetRan(82021, 87.0f);
				_app->rotationSetActive(82021);
				_app->soundStop(81204, 1024);
				_app->objectPresentationShow(kObject81221, 1);
			}
		} else {
			_app->varSetFloat(81002, _app->varGetFloat(81002) + 1.0f);
			if (_app->varGetFloat(81002) > 59.0f)
				_app->varSetFloat(81002, 0);

			_app->objectPresentationHide(kObject81202);
			_app->objectPresentationShow(kObject81202, (uint32)_app->varGetFloat(81002));


			if (_app->varGetFloat(81002) != 25.0f
				&& _app->varGetFloat(81002) != 50.0f)
				break;

			_app->soundPlay(81203);
		}
		break;

	case kTimer2:
		_app->objectPresentationHide(kObject81404, 1);
		_app->timerStop(kTimer2);

		if (_app->varGetByte(81007) == 1) {
			_app->objectPresentationHide(kObject81404);
			_app->objectPresentationShow(kObject81404, 0);
			_app->timerStart(kTimer3, 100);
			_app->varSetByte(81007, 0);
		} else {
			_app->objectPresentationHide(kObject81404);
			_app->rotationSetAlp(84021, 53.0f);
			_app->rotationSetBet(84021, 0);
			_app->rotationSetRan(84021, 87.0f);
			_app->rotationSetActive(84021);

			if (_app->varGetByte(81006) == 1)
				_app->soundPlay(81409);

			_app->soundStop(81410, 1024);
			_app->varSetByte(81006, 0);
		}

		if (_app->varGetByte(81012) == 1
		 && _app->varGetByte(81022) == 1
		 && _app->bagHas(kObjectDiaryOfASeducer)) {
			_app->soundStopAll(1024);
			_app->exitToMenu(kMenuAction0);
		}
		break;

	case kTimer3:
		_app->objectPresentationHide(kObject81404, 0);
		_app->bagAdd(81207);
		_app->timerStop(kTimer3);
		_app->rotationSetAlp(84021, 53.0f);
		_app->rotationSetBet(84021, 0);
		_app->rotationSetRan(84021, 87.0f);
		_app->rotationSetActive(84021);

		if (_app->varGetByte(81006) == 1)
			_app->soundPlay(81409);
		_app->soundStop(81410, 1024);
		_app->varSetByte(81006, 0);
		break;

	case kTimer4:
		_app->objectPresentationHide(kObject81306, _app->varGetByte(81021));
		_app->timerStop(kTimer4);
		break;

	case kTimer5:
		if (floor(_app->varGetFloat(81005)) == 0.0f
		 && floor(_app->varGetFloat(81004)) == 0.0f) {
			 _app->soundStop(81301, 1024);
			 _app->timerStop(kTimer5);
			 _app->rotationSetAlp(83031, 180.0f);
			 _app->rotationSetBet(83031, -26.0f);
			 _app->rotationSetRan(83031, 87.0f);
			 _app->rotationSetActive(83031);
			 break;
		}

		if (floor(_app->varGetFloat(81005)) != 0.0f) {
			_app->varSetFloat(81005, _app->varGetFloat(81005) - 1.0f);
			_app->objectPresentationHide(kObject81308);
			_app->objectPresentationShow(kObject81308, (uint32)_app->varGetFloat(81005));
		}

		if (floor(_app->varGetFloat(81004)) != 0.0f) {
			_app->varSetFloat(81004, _app->varGetFloat(81004) - 1.0f);
			_app->objectPresentationHide(kObject81309);
			_app->objectPresentationShow(kObject81309, (uint32)_app->varGetFloat(81004));
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->objectPresentationHide(kObject81303, 6);
		_app->objectPresentationHide(kObject81303, 8);
		_app->rotationSetAlp(83031, 334.0f);
		_app->rotationSetBet(83031, 16.0f);
		_app->rotationSetRan(83031, 87.0f);
		_app->rotationSetActive(83031);
		break;

	case kTimer7: {
		_app->timerStop(kTimer7);
		_app->timerStart(kTimer7, rnd(15000) + 15000);

		Id soundId = 81102 + rnd(2);
		_app->soundSetVolume(soundId, rnd(10) + 90);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer8: {
		_app->timerStop(kTimer8);
		_app->timerStart(kTimer8, rnd(3000) + 6000);
		Id soundId = 81104 + rnd(3);
		_app->soundSetVolume(soundId, rnd(10) + 90);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer9: {
		_app->timerStop(kTimer9);
		_app->timerStart(kTimer9, rnd(5000) + 5000);

		Id soundId = 81415 + rnd(4);
		_app->soundSetVolume(soundId, rnd(10) + 90);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer10: {
		_app->timerStop(kTimer10);
		_app->timerStart(kTimer10, rnd(10000) + 5000);

		_app->objectPresentationShow(kObject81901);
		if (!_app->varGetByte(81023))
			_app->objectPresentationShow(kObject81902);

		Id soundId = 81217 + rnd(6);
		_app->soundSetVolume(soundId, 80 + rnd(10));
		_app->soundPlay(soundId);
		}
		break;

	case kTimer11:
		_app->objectPresentationHide(kObject81222);
		_app->timerStop(kTimer11);
		break;
	}
}

void EventTimerFaust::onTimerZone13(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetFloat(91001, _app->varGetFloat(91001) - 1.0f);
		_app->objectPresentationHide(kObject91004);
		_app->objectPresentationShow(kObject91004, (uint32)_app->varGetFloat(91001));

		if (floor(_app->varGetFloat(91001)) <= 1.0f) {
			_app->timerStop(kTimer0);
			_app->soundPlay(91107);
			_app->waitForEscape(10);
			_app->soundStop(91105, 1024);
			_app->soundStop(91106, 1024);
		}
		break;

	case kTimer1:
		_app->timerStop(kTimer1);
		_app->timerStart(kTimer1, rnd(15000) + 25000);
		_app->soundPlay(91117 + rnd(3));
		break;
	}
}

void EventTimerFaust::onTimerZone14(TimerId id) {
	error("[EventTimerFaust::onTimerZone14] Not implemented");
}

void EventTimerFaust::onTimerZone15(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->objectPresentationHide(kObject120003, 1);
		_app->timerStop(kTimer0);
		break;

	case kTimer1:
		if (_app->varGetWord(120001) != 400 || _app->varGetByte(97001) != 1) {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject6, 1, Common::Point(_app->varGetWord(120001), 16));
			_app->varSetWord(120001, _app->varGetWord(120001) + 1);
		} else {
			if (_app->varGetWord(120002) == 400) {
				_app->varSetWord(120001, 401);
				_app->objectPresentationHide(kObject6, 5);
				_app->objectPresentationShow(kObject6, 3);
				_app->objectPresentationHide(kObject6, 4);
			}
		}

		if (_app->varGetWord(120001) == 400
		 && _app->varGetByte(97001) == 0) {
			_app->objectPresentationShow(kObject6, 3);
			_app->objectPresentationHide(kObject6, 4);
		}

		if (_app->varGetWord(120001) == 440) {
			if (_app->varGetByte(120003)) {
				_app->objectSetAccessibilityOn(kObject120001, 1, 1);
				_app->rotationSetMovabilityOn(120104, 3, 3);
				_app->rotationSetMovabilityOn(120106, 1, 1);
				_app->rotationSetMovabilityOn(120108, 3, 3);
				_app->varSetByte(98088, 0);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer2);
				_app->rotationSetActive(120401);
			} else {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer2);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);

				Id rotationId = _app->getCurrentRotationId();

				_app->varSetDword(120007, rotationId);
				_app->varSetWord(120006, (int16)_app->rotationGetAlp(rotationId));
				_app->varSetWord(120007, (int16)_app->rotationGetBet(rotationId));
				_app->varSetWord(120008, (int16)_app->rotationGetRan(rotationId));

				_app->objectSetAccessibilityOn(kObject1);
				_app->fadeOut(15, Color(0, 0, 0), 0);
				_app->objectPresentationShow(kObject15);
				_app->soundPlay(120150);
			}
		}
		break;

	case kTimer2:
		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject6, 2, Common::Point(_app->varGetWord(120002), 46));
		_app->varSetWord(120002, _app->varGetWord(120002) + 1);
		break;

	case kTimer3:
		_app->timerStop(kTimer3);
		_app->objectPresentationShow(kObject120054, 0);
		_app->soundPlay(120072);
		break;

	case kTimer4: {
		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, rnd(3000) + 2000);

		Id soundId = 120003 + rnd(11);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer5: {
		_app->timerStop(kTimer5);
		_app->timerStart(kTimer5, rnd(3000) + 3000);

		Id soundId = 120013 + rnd(4);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->timerStart(kTimer6, rnd(3000) + 3000);
		_app->objectPresentationShow(kObject120061);
		_app->soundPlay(120067);
		break;

	case kTimer7:
		_app->timerStop(kTimer7);
		_app->varSetByte(120035, 0);
		_app->objectPresentationHide(kObject120054, 1);
		_app->objectSetAccessibilityOn(kObject120054, 2, 2);
		_app->soundStop(120071, 1024);
		break;
	}
}

void EventTimerFaust::onTimerZone16(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetWord(311024, _app->varGetWord(311024) + 5);

		if (_app->varGetWord(311024) < 155) {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject310015, 0, Common::Point(120, 174 - _app->varGetWord(311024)));
		} else {
			_app->objectSetAccessibilityOn(kObject310015, 0, 0);
			_app->objectSetAccessibilityOn(kObject310001);
			_app->soundStop(310103, 1024);
			_app->soundPlay(310104);
			_app->timerStop(kTimer0);
		}
		break;

	case kTimer1:
		_app->varSetWord(311024, _app->varGetWord(311024) - 5);

		if (_app->varGetWord(311024) > -10) {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject310015, 0, Common::Point(120, 174 - _app->varGetWord(311024)));
		} else {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject310015, 0, Common::Point(120, 174));
			_app->soundStop(310103, 1024);
			_app->timerStop(kTimer1);
			_app->rotationSetAlp(311251, 206.0f);
			_app->rotationSetBet(311251, -26.0f);
			_app->rotationSetRan(311251, 87.0f);
			_app->rotationSetActive(311251);
			_app->soundPlay(310104);
		}
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->timerStart(kTimer2, rnd(200));

		if (rnd(2))
			_app->objectPresentationHide(kObject310018);
		else
			_app->objectPresentationShow(kObject310018);
		break;
	}
}

#pragma endregion

} // End of namespace Ring
