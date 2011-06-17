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

#include "ring/game/ring/ring_animation.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/helpers.h"
#include "ring/ring.h"

using namespace RingGame;

namespace Ring {

EventAnimationRing::EventAnimationRing(ApplicationRing *application) : _app(application) {
}

EventAnimationRing::~EventAnimationRing() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventAnimationRing::onAnimationNextFrameZoneNI(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId) {
	default:
		break;

	case 10000:
		if (frame == 1) {
			uint32 val = rnd(10);
			if (val > 4)
				val = 0;

			_app->objectPresentationPauseAnimationFrame(kObjectGlug, 0, 1, 300 * val, 0);
		}
		break;

	case 10101:
		if (frame == 36) {
			_app->objectSetAccessibilityOn(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOn(kObject10101, 1, 1);
		}
		break;

	case 10103:
		if (frame == 1) {
			_app->objectSetAccessibilityOff(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOn(kObject10101, 1, 1);

			if (_app->varGetByte(10113) < 9)
				_app->objectSetAccessibilityOn(kObject10109);
		}
		break;

	case 10100:
		if (frame == 1) {
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10101, 3);
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10102:
		if (frame == 36) {
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10101, 4);
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectPresentationHide(kObject10101, 5);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10105:
		if (frame == 36) {
			_app->objectSetAccessibilityOn(kObject10105, 1, 1);
			_app->objectSetAccessibilityOn(kObject10102, 1, 1);
		}
		break;

	case 10104:
		if (frame == 15) {
			_app->objectSetAccessibilityOn(kObject10102, 1, 1);
			_app->objectSetAccessibilityOn(kObject10105, 0, 0);
		}
		break;

	case 10106:
		if (frame == 1) {
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectPresentationHide(kObject10102, 0);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10200:
		if (frame == 1) {
			uint32 val = rnd(10);
			if (val > 4)
				val = 0;

			_app->objectPresentationPauseAnimationFrame(kObject10200, 1, 1, 1000 * val, 0);
		}
		break;

	case 10300:
		if (frame == 3) {
			_app->rotationSet3DSoundOn(10301, 10301);
			_app->rotationSet3DSoundOff(10301, 10301);
			_app->soundPlay(10301);
		}
		break;

	case 10422:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject10430, 1, 2);
			break;

		case 2:
			_app->soundStop(10407, 1024);
			break;

		case 14:
			_app->soundPlay(10407);
			break;

		case 15:
			_app->soundStop(10405, 1024);
			break;

		case 26:
			_app->soundPlay(10405);
			break;
		}
		break;

	case 10423:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(10408);
			break;

		case 15:
			_app->soundPlay(10406);
			break;

		case 26:
			_app->soundStop(10406, 1024);
			_app->soundStop(10408, 1024);
			break;

		case 27:
			_app->puzzleSetMovabilityOn(kPuzzle10411);
			_app->objectSetAccessibilityOn(kObject10430, 0, 0);
			break;
		}
		break;

	case 10424:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectPresentationHide(kObject10432, 0);
			_app->objectPresentationShow(kObject10430, 6);
			_app->objectSetAccessibilityOn(kObject10430, 1, 2);
			break;

		case 2:
			_app->soundStop(10407, 1024);
			break;

		case 14:
			_app->soundPlay(10407);
			break;
		case 15:
			_app->soundStop(10405, 1024);
			break;

		case 26:
			_app->soundPlay(10405);
			break;
		}
		break;

	case 10425:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(10408);
			_app->objectPresentationHide(kObject10430, 6);
			break;

		case 15:
			_app->soundPlay(10406);
			break;

		case 26:
			_app->puzzleSetMovabilityOn(kPuzzle10411);
			_app->objectSetAccessibilityOn(kObject10430, 0, 0);
			_app->varSetByte(10431, 1);
			_app->objectPresentationShow(kObject10432, 0);
			_app->sub_445A10();
			_app->soundStop(10406, 1024);
			_app->soundStop(10408, 1024);
			break;
		}
		break;
	}
}

void EventAnimationRing::onAnimationNextFrameZoneRH(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId){
	default:
		break;

	case 20001:
		if (frame == 5) {
			_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 2);
			_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20001), 4);
			_app->soundPlay(_app->varGetByte(21001) + 20011);
		}
		break;

	case 20003:
		if (frame == 2)
			_app->soundPlay(23014);
		break;
	}
}

void EventAnimationRing::onAnimationNextFrameZoneFO(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId) {
	default:
		break;

	case 30001:
		switch (frame) {
		default:
			break;

		case 10:
			_app->soundPlay(30505);
			_app->soundSetVolume(30505, 90);
			break;

		case 90:
			_app->soundPlay(30503);
			_app->soundSetVolume(30503, 90);
			break;

		case 125:
			_app->soundPlay(30504);
			_app->soundSetVolume(30504, 95);
			break;

		case 202:
			_app->objectPresentationPauseAnimation(kObject30110, 1);
			break;
		}
		break;

	case 30007:
		switch (frame) {
		default:
			break;

		case 10:
			_app->soundPlay(30505);
			_app->soundSetVolume(30505, 95);
			break;

		case 90:
			_app->soundPlay(30503);
			_app->soundSetVolume(30503, 95);
			break;

		case 125:
			_app->soundPlay(30504);
			_app->soundSetVolume(30504, 100);
			break;

		case 202:
			_app->objectPresentationPauseAnimation(kObject30109, 0);
			_app->objectSetAccessibilityOn(kObject30109, 0, 0);
			break;
		}
		break;

	case 30000:
		if (_app->varGetByte(30047) == 1 && frame == 28) {
			_app->playMovie("1207");
			_app->varSetByte(30047, 0);
			_app->puzzleSetActive(kPuzzle35009);
		}
		break;

	case 30002:
		if (frame == 19)
			_app->objectPresentationShow(kObject30110, 6);
		break;

	case 30004:
		if (frame == 19)
			_app->objectPresentationShow(kObject30110, 9);
		break;

	case 30003:
		if (frame == 19)
			_app->objectPresentationHide(kObject30110, 6);
		break;

	case 30005:
		if (frame == 19)
			_app->objectPresentationHide(kObject30110, 9);
		break;

	case 30006:
		switch (frame){
		default:
			break;

		case 10:
			_app->soundPlay(30502);
			break;

		case 26:
			_app->objectPresentationPauseAnimation(kObjectSleepingPotion, 0);
			_app->timerStop(kTimer2);
			_app->timerStart(kTimer2, 1000 * (rnd(10) + 5));
			break;
		}
		break;

	case 30008:
		if (frame == 25) {
			_app->objectSetAccessibilityOn(kObject30040, 5, 5);
			_app->objectSetAccessibilityOn(kObject30040, 3, 3);
		}
		break;
	}
}

void EventAnimationRing::onAnimationNextFrameZoneRO(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId) {
	default:
		break;

	case 40100:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40200))
				_app->soundPlay(40200);
			else
				_app->soundPlay(40300);
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 1, 1);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);

			if (_app->varGetString(40901) == "6543210") {
				if ((_app->varGetByte(40200) + _app->varGetByte(40201) + _app->varGetByte(40202) + _app->varGetByte(40203)
				   + _app->varGetByte(40204) + _app->varGetByte(40205) + _app->varGetByte(40206)) == 7) {
					_app->varSetByte(40802, 1);
					_app->varSetString(40901, "0000000");
					_app->soundPlay(40400);
					_app->varSetFloat(90006, 85.7f);
					_app->objectSetAccessibilityOff(kObject40060);
					_app->objectSetAccessibilityOff(kObject40201);

					for (uint32 i = 10; i > 0; i--) {
						_app->objectPresentationHide(kObject40060);
						_app->objectPresentationShow(kObject40060, i);
						handleEvents();

						if (checkEscape())
							break;
					}
				}
			}
			break;
		}

		_app->varSetByte(40911, (int8)frame);
		return;

	case 40101:
		switch (frame) {
		default:
			_app->varSetByte(40912, (int8)frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40912, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40201)) {
				_app->soundPlay(40201);
				_app->varSetByte(40912, (int8)frame);
			} else {
				_app->soundPlay(40301);
				_app->varSetByte(40912, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 2, 2);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40912, (int8)frame);
			break;
		}
		break;

	case 40102:
		switch (frame) {
		default:
			_app->varSetByte(40913, (int8)frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40913, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40202)) {
				_app->soundPlay(40202);
				_app->varSetByte(40913, (int8)frame);
			} else {
				_app->soundPlay(40302);
				_app->varSetByte(40913, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 3, 3);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40913, (int8)frame);
			break;
		}
		break;

	case 40103:
		switch (frame) {
		default:
			_app->varSetByte(40914, (int8)frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40914, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);


			if (_app->varGetByte(40203)) {
				_app->soundPlay(40203);
				_app->varSetByte(40914, (int8)frame);
			} else {
				_app->soundPlay(40303);
				_app->varSetByte(40914, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 4, 4);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40914, (int8)frame);
			break;
		}
		break;

	case 40104:
		switch (frame) {
		default:
			_app->varSetByte(40915, (int8)frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40915, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40204)) {
				_app->soundPlay(40204);
				_app->varSetByte(40915, (int8)frame);
			} else {
				_app->soundPlay(40304);
				_app->varSetByte(40915, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 5, 5);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40915, (int8)frame);
			break;
		}
		break;

	case 40105:
		switch (frame) {
		default:
			_app->varSetByte(40916, (int8)frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40916, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);

			if (_app->varGetByte(40205)) {
				_app->soundPlay(40205);
				_app->varSetByte(40916, (int8)frame);
			} else {
				_app->soundPlay(40305);
				_app->varSetByte(40916, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 6, 6);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40916, (int8)frame);
			break;
		}
		break;

	case 40106:
		switch (frame) {
		default:
			_app->varSetByte(40917, (int8)frame);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOff(kObject40201);
			_app->soundSetVolume(40600, rnd(20) + 80);
			_app->soundPlay(40600);
			_app->varSetByte(40917, 1);
			break;

		case 30:
			_app->soundSetVolume(40601, rnd(20) + 80);
			_app->soundPlay(40601);


			if (_app->varGetByte(40206)) {
				_app->soundPlay(40206);
				_app->varSetByte(40917, (int8)frame);
			} else {
				_app->soundPlay(40306);
				_app->varSetByte(40917, (int8)frame);
			}
			break;

		case 70:
			_app->objectSetAccessibilityOn(kObject40060, 7, 7);
			_app->objectSetAccessibilityOn(kObject40201);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
			_app->varSetByte(40917, (int8)frame);
			break;
		}
		break;

	case 40201:
	case 40202:
	case 40203:
	case 40204:
	case 40205:
	case 40206:
	case 40207:
		_app->varSetByte(frame + 700, (int8)frame); // id: frame + 700 = 4090x

		if (_app->varGetByte(40901) == 26 && _app->varGetByte(40902) == 26 && _app->varGetByte(40903) == 26 && _app->varGetByte(40904) == 26 \
		 && _app->varGetByte(40905) == 26 && _app->varGetByte(40906) == 26 && _app->varGetByte(40907) == 26 && _app->varGetByte(40911) == 70 \
		 && _app->varGetByte(40912) == 70 && _app->varGetByte(40913) == 70 && _app->varGetByte(40914) == 70 && _app->varGetByte(40915) == 70 \
		 && _app->varGetByte(40916) == 70 && _app->varGetByte(40917) == 70) {
			_app->objectSetAccessibilityOn(kObject40060, (uint8)_app->varGetByte(40804) / 10, (uint8)_app->varGetByte(40804) / 10);
			_app->puzzleSetMovabilityOn(kPuzzle40060, 0, 0);
		}
		break;
	}
}

void EventAnimationRing::onAnimationNextFrameZoneWA(Id animationId, const Common::String &, uint32 frame, uint32) {
	if (animationId == 50001) {
		if (_app->varGetByte(50007) == 1 && _app->varGetByte(50009) == 1) {
			if (frame == 35 || frame == 12) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 6);
				_app->objectPresentationHide(kObject50600, 7);
				_app->soundStop(50018, 1024);

				if (_app->varGetByte(50012) == 6) {
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 5.0f);
					_app->varSetByte(50012, 5);
				}
			}
		}

		if (_app->varGetByte(50008) == 1 && _app->varGetByte(50010) == 1) {
			if (frame == 49 || frame == 24) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 6);
				_app->objectPresentationShow(kObject50600, 7);
				_app->soundStop(50018, 1024);

				if (_app->varGetByte(50012) == 6) {
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 5.0f);
					_app->varSetByte(50012, 5);
				}
			}
		}

		if (_app->varGetByte(50008) == 1 && _app->varGetByte(50009) == 1) {
			if (frame == 17 || frame == 41) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 7);
				_app->objectPresentationShow(kObject50600, 6);
				_app->soundStop(50018, 1024);
				_app->varSetByte(50012, 6);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			}
		}

		if (_app->varGetByte(50007) == 1 && _app->varGetByte(50010) == 1) {
			if (frame == 4 || frame == 30) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 7);
				_app->objectPresentationShow(kObject50600, 6);
				_app->soundStop(50018, 1024);
				_app->varSetByte(50012, 6);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			}
		}
	}
}

void EventAnimationRing::onAnimationNextFrameZoneAS(Id animationId, const Common::String &, uint32 frame, uint32) {
	if (animationId == 80001) {
		if ((int32)frame == _app->varGetByte(80004)) {
			_app->objectPresentationPauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOn(kObject80021, 0, 4);
			_app->soundStop(80210, 1024);
			_app->soundPlay(80209);
		}

		_app->varSetByte(80005, (int8)frame);
	}
}

void EventAnimationRing::onAnimationNextFrameZoneN2(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId) {
	default:
		break;

	case 70300:
		if (frame == 3)
			_app->soundPlay(70301);
		break;

	case 70102:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundPlay(70103);
			break;

		case 36:
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70101, 4);
			_app->objectPresentationHide(kObject70101, 0);
			_app->objectPresentationHide(kObject70101, 5);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);
			_app->objectSetAccessibilityOn(kObject70103);
			break;
		}
		break;

	case 70103:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject70101, 1, 1);
			_app->objectPresentationShow(kObject70100, 1);
			break;

		case 36:
			_app->soundPlay(70102);
			break;
		}
		break;

	case 70104:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundPlay(70104);
			break;

		case 15:
			_app->soundStop(70104, 1024);
			_app->objectSetAccessibilityOn(kObject70102, 1, 1);
			_app->objectSetAccessibilityOn(kObject70105, 0, 0);
			break;
		}
		break;

	case 70106:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundStop(70105, 1024);
			_app->objectPresentationHide(kObject70101, 0);
			_app->objectPresentationHide(kObject70102, 0);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);
			_app->objectSetAccessibilityOn(kObject70103);
			break;

		case 15:
			_app->soundPlay(70105);
			break;
		}
		break;

	case 70422:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject70404, 1, 2);
			break;

		case 2:
			_app->soundStop(70407, 1024);
			break;

		case 14:
			_app->soundPlay(70407);
			break;

		case 15:
			_app->soundStop(70405, 1024);
			break;

		case 26:
			_app->soundPlay(70405);
			break;
		}
		break;

	case 70423:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(70408);
			break;

		case 15:
			_app->soundPlay(70406);
			break;

		case 26:
			_app->soundStop(70406, 1024);
			_app->soundStop(70408, 1024);
			break;

		case 27:
			_app->puzzleSetMovabilityOn(70411);
			_app->objectSetAccessibilityOn(kObject70404, 0, 0);
			break;
		}
		break;

	case 70424:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectPresentationShow(kObject70404, 6);
			_app->objectSetAccessibilityOn(kObject70404, 1, 2);
			break;

		case 2:
			_app->soundStop(70407, 1024);
			break;

		case 14:
			_app->soundPlay(70407);
			break;

		case 15:
			_app->soundStop(70405, 1024);
			break;

		case 26:
			_app->soundPlay(70405);
			_app->objectPresentationHide(kObject70406);
			sub_433FA0();
			break;
		}
		break;

	case 70425:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(70408);
			_app->objectPresentationHide(kObject70404, 6);
			break;

		case 15:
			_app->soundPlay(70406);
			break;

		case 26:
			_app->puzzleSetMovabilityOn(70411);
			_app->objectSetAccessibilityOn(kObject70404, 0, 0);
			_app->varSetByte(70005, 1);
			_app->objectPresentationShow(kObject70406);
			sub_4340C0();

			if (!_app->varGetByte(70012))
				_app->objectPresentationShow(kObjectFire);
			break;
		}
		break;
	}
}

void EventAnimationRing::onAnimationZoneWA(uint32 type, Id animationId, const Common::String &, uint32, uint32) {
	switch (animationId) {
	default:
		break;

	case 50003:
		if (type == 1) {
			_app->varSetByte(50011, 1);
		} else if (type == 2) {
			_app->varSetByte(50011, 0);
			_app->objectPresentationHide(kObject50100, 3);
			_app->objectPresentationHide(kObject50100, 4);
			_app->objectPresentationHide(kObject50100, 5);
			_app->objectPresentationHide(kObject50100, 6);
			_app->objectPresentationShow(kObject50100, 0);
			_app->objectPresentationShow(kObject50100, 1);
			_app->objectPresentationShow(kObject50100, 2);
			_app->varSetWord(50101, 0);
			_app->varSetWord(50102, 0);
			_app->varSetWord(50103, 0);
			_app->varSetWord(50104, 0);
			_app->varSetWord(50105, 0);
			_app->objectSetAccessibilityOn(kObjectInk);
			_app->objectSetAccessibilityOn(kObjectPaper);
			_app->objectSetAccessibilityOn(kObjectStylet);
			_app->objectSetAccessibilityOn(kObjectInkedStylet);

		}
		break;

	case 50004:
		if (type == 2) {
			_app->objectPresentationHide(kObject50100);
			_app->rotationSetAlp(50601, 250.0);
			_app->rotationSetActive(50601);
			_app->playMovie("1872");
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////

void EventAnimationRing::sub_433FA0() {
	_app->puzzleSet3DSoundOff(kPuzzle70410, 70412);
	_app->rotationSet3DSoundOff(70400, 70412);

	for (int i = 95; i > 70; i--) {
		_app->puzzleSet3DSoundVolume(kPuzzle70411, 70412, i);
		handleEvents();
	}

	_app->puzzleSet3DSoundOff(kPuzzle70411, 70412);
	_app->rotationSet3DSoundOff(70000, 70107);
	_app->rotationSet3DSoundOff(70001, 70107);
	_app->rotationSet3DSoundOff(70400, 70107);
	_app->rotationSet3DSoundOff(70100, 70107);
	_app->rotationSet3DSoundOff(70101, 70107);
	_app->puzzleSet3DSoundOff(kPuzzle70100, 70107);
	_app->puzzleSet3DSoundOff(kPuzzle70102, 70107);
	_app->soundPlay(70000);

}

void EventAnimationRing::sub_4340C0() {
	_app->soundStop(70000, 1024);
	_app->puzzleSet3DSoundOn(kPuzzle70411, 70412);
	_app->puzzleSet3DSoundOn(kPuzzle70410, 70412);
	_app->rotationSet3DSoundOn(70400, 70412);

	for (int i = 70; i < 95; i++) {
		_app->puzzleSet3DSoundVolume(kPuzzle70411, 70412, i);
		handleEvents();
	}

	_app->rotationSet3DSoundOn(70000, 70107);
	_app->rotationSet3DSoundOn(70001, 70107);
	_app->rotationSet3DSoundOn(70400, 70107);
	_app->rotationSet3DSoundOn(70100, 70107);
	_app->rotationSet3DSoundOn(70101, 70107);
	_app->puzzleSet3DSoundOn(kPuzzle70100, 70107);
	_app->puzzleSet3DSoundOn(kPuzzle70102, 70107);
}

} // End of namespace Ring
