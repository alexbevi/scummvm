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

#include "ring/game/ring/ring_bag.h"

#include "ring/base/bag.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_event.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"

#include "ring/helpers.h"
#include "ring/ring.h"

using namespace RingGame;

namespace Ring {

EventBagRing::EventBagRing(ApplicationRing *application, EventHandlerRing *eventHandler) : _app(application), _event(eventHandler) {
	// SY
	_offsetX = 0;
	_offsetX0 = 0;
	_coordX = 0;

	// NI
	_stateBagNI = false;
	_presentationIndexNI = 0;
	_indexOffsetNI = 0;
	_distNI = 0;

	// RO
	_presentationIndexBagRO = 0;
	_presentationIndexBagRO2 = 0;

	// N2
	_presentationIndexN2 = 0;
	_distN2 = 0;
}

EventBagRing::~EventBagRing() {
	// Zero-out passed pointers
	_app = NULL;
	_event = NULL;
}

void EventBagRing::onBagZoneSY(ObjectId id, uint32, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObjectPreferencesSliderVolume:
	case kObjectPreferencesSliderDialog:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);

			if (id == kObjectPreferencesSliderVolume)
				_app->dragControlSetHotspot(Common::Rect(310, 140, 600, 180));
			else
				_app->dragControlSetHotspot(Common::Rect(310, 197, 600, 237));

			_offsetX = (_app->getDragControl()->getCurrentCoords().x - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;
			} else {
				_offsetX = 54;
			}

			_coordX = (int16)(5 * _offsetX + 314);
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;

		case 2:
			_offsetX = (_offsetX0 + _coordX - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;

				_event->_prefsVolume = _offsetX + 46;
			} else {
				_offsetX = 54;
				_event->_prefsVolume = 100;
			}

			_coordX = (int16)(5 * _offsetX + 314);
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;

		case 3:
			_offsetX0 = (int16)_app->dragControlGetOffsetX0();
			if (_app->dragControlXLower0())
				_offsetX0 = -(int16)_app->dragControlGetOffsetX0();

			_offsetX = (_offsetX0 + _coordX - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;
			} else {
				_offsetX = 54;
			}

			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_offsetX0 + _coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;
		}
		break;
	}
}

void EventBagRing::onBagZoneNI(ObjectId id, uint32 a2, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject10103:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexNI = 0;
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(495, 194, 598, 284));
			break;

		case 2:
			if (!_app->soundIsPlaying(10401))
				_app->soundStop(10401, 1024);

			_app->objectSetAccessibilityOff(kObject10103);

			if (_presentationIndexNI <= 6) {
				while (_presentationIndexNI > 0) {
					--_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_presentationIndexNI);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject10103);
			} else {
				while (_presentationIndexNI < 12) {
					++_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_presentationIndexNI);

					handleEvents();
				}

				uint32 startTick = g_system->getMillis();
				while (g_system->getMillis() - startTick < 1000)
					handleEvents();

				while (_presentationIndexNI > 0) {
					--_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_presentationIndexNI);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject10103);
				_app->objectPresentationHide(kObject10003, 8);
				_app->objectPresentationHide(kObject10003, 9);
				_app->objectPresentationHide(kObject10003, 10);
				_app->objectPresentationHide(kObject10003, 11);
				_app->objectPresentationHide(kObject10003, 12);
				_app->objectPresentationShow(kObject10003, 3);
				_app->objectPresentationShow(kObject10003, 4);
				_app->objectPresentationShow(kObject10003, 5);
				_app->objectPresentationShow(kObject10003, 6);
				_app->objectPresentationShow(kObject10003, 7);
				_app->playMovie("1511");
				_app->rotationSetAlp(10005, 270.0f);
				_app->rotationSetRan(10005, 85.7f);
				_app->rotationSetActive(10005);
				_app->rotationSetMovabilityOff(10005);
				_app->rotationSetMovabilityOn(10005, 1, 1);
				_app->soundPlay(10901, kSoundLoop);
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				_presentationIndexNI = _app->dragControlGetOffsetX0() / 5;

				if (_app->dragControlXLower0()) {
					_presentationIndexNI = 0;
				} else if (_presentationIndexNI >= 0) {
					if (_presentationIndexNI > 12)
						_presentationIndexNI = 12;
				} else {
					_presentationIndexNI = 0;
				}

				_app->objectPresentationHide(kObject10103);
				_app->objectPresentationShow(10103, (uint32)_presentationIndexNI);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;

	case kObject10106:
		switch (type) {
		default:
			break;

		case 1: {
			_app->dragControlSetCoords1(Common::Point(243, 276));

			int32 val1 = 0;
			int32 val2 = 0;
			int32 offsetX1 = 0;
			int32 offsetY1 = 0;

			if (_app->dragControlXLower1())
				offsetX1 = -(int32)_app->dragControlGetOffsetX1();
			else
				offsetX1 = _app->dragControlGetOffsetX1();

			if (_app->dragControlYLower1())
				offsetY1 = -(int32)_app->dragControlGetOffsetY1();
			else
				offsetY1 = _app->dragControlGetOffsetY1();

			if (offsetX1 <= 0) {
				if (offsetY1 > 0) {
					val1 = (offsetY1 + offsetX1 + 40) / 6;
					val2 = 30;
				} else {
					val1 = ((offsetY1 - offsetX1) + 40) / 6;
					val2 = 20;
				}
			} else {
				if (offsetY1 > 0) {
					val1 = ((offsetX1 - offsetY1) + 40) / 6;
					val2 = 0;
				} else {
					val1 = (40 - (offsetX1 + offsetY1)) / 6;
					val2 = 10;
				}
			}

			if (val1 < 0)
				val1 = 0;

			_distNI = val1 + val2 - _app->varGetWord(10100);
			if (_distNI < 0)
				_distNI	= 0;

			if (_distNI > 18)
				_distNI %= 19;

			_indexOffsetNI = (_app->varGetByte(10105) == 1) ? 19 : 256;
			}
			break;

		case 2: {
			int32 offset = 0;
			bool update = true;

			switch (_presentationIndexNI) {
			default:
				offset = 2;
				break;

			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 13:
			case 14:
			case 15:
				offset = -1;
				break;

			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 16:
			case 17:
			case 18:
				offset = 1;
				break;

			case 12:
				// Skip update
				update = false;
				break;
			}

			if (update) {
				do {
					_presentationIndexNI += offset;

					if (_presentationIndexNI <= 18) {
						if (_presentationIndexNI < 0)
							_presentationIndexNI = 18;
					} else {
						_presentationIndexNI = 0;
					}

					_app->objectPresentationHideAndRemove(kObject10106);
					_app->objectPresentationShow(kObject10106, (uint32)(_presentationIndexNI + _indexOffsetNI));
					sub_445930();

					handleEvents();

				} while (_presentationIndexNI != 12);
			}

			_app->varSetWord(10100, (int16)_presentationIndexNI);

			if (_presentationIndexNI == 12) {
				_app->varSetByte(10104, 1);

				_event->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

				_app->rotationSetMovabilityOff(10102, 1, 1);
				_app->rotationSetMovabilityOn(10102, 2, 2);

				sub_445930();
			} else {
				_app->varSetByte(10104, 0);

				_event->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

				_app->rotationSetMovabilityOn(10102, 1, 1);
				_app->rotationSetMovabilityOff(10102, 2, 2);
				sub_445930();
			}
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				int32 val1 = 0;
				int32 val2 = 0;
				int32 offsetX1 = 0;
				int32 offsetY1 = 0;

				if (_app->dragControlXLower1())
					offsetX1 = -(int32)_app->dragControlGetOffsetX1();
				else
					offsetX1 = _app->dragControlGetOffsetX1();

				if (_app->dragControlYLower1())
					offsetY1 = -(int32)_app->dragControlGetOffsetY1();
				else
					offsetY1 = _app->dragControlGetOffsetY1();

				// Check interval
				if (offsetX1 > 40 || offsetY1 > 40 || offsetX1 < -40 || offsetY1 < -40)
					break;

				if (offsetX1 <= 0) {
					if (offsetY1 > 0) {
						val1 = (offsetY1 + offsetX1 + 40) / 6;
						val2 = 30;
					} else {
						val1 = ((offsetY1 - offsetX1) + 40) / 6;
						val2 = 20;
					}
				} else {
					if (offsetY1 > 0) {
						val1 = ((offsetX1 - offsetY1) + 40) / 6;
						val2 = 0;
					} else {
						val1 = (40 - (offsetX1 + offsetY1)) / 6;
						val2 = 10;
					}
				}

				if (val1 < 0)
					val1 = 0;

				_presentationIndexNI = val1 + val2 + _distNI;
				if (_presentationIndexNI < 0)
					_presentationIndexNI	= 0;

				if (_presentationIndexNI > 18)
					_presentationIndexNI %= 19;

				_app->objectPresentationHide(kObject10106);
				_app->objectPresentationShow(kObject10106, (uint32)(_presentationIndexNI + _indexOffsetNI));
				_app->varSetWord(10100, (int16)_presentationIndexNI);
				sub_445930();
			}
			break;
		}
		break;

	case kObject10107:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(295, 255, 345, 375));
			break;

		case 2:
			_app->objectPresentationHide(kObject10107);

			if (a2) {
				if (_app->dragControlGetOffsetY0() < 20) {
					_app->objectPresentationShow(kObject10107, 13);
					_app->varSetByte(10105, 0);
					_event->sub_445A10();
					sub_445930();
					break;
				}

				if (_app->dragControlYHigher0()) {
					_app->objectPresentationShow(kObject10107, 13);
					_app->varSetByte(10105, 1);
				} else {
					_app->objectPresentationShow(kObject10107, 0);
					_app->objectSetAccessibilityOn(kObject10107, 0, 0);
					_app->objectSetAccessibilityOff(kObject10107, 1, 1);
					_app->varSetByte(10105, 0);
				}

				_event->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

			} else {
				if (_app->dragControlGetOffsetY0() < 20) {
					_app->objectPresentationShow(kObject10107, 0);
					_app->varSetByte(10105, 0);
					_event->sub_445A10();
					sub_445930();
					break;
				}

				if (_app->dragControlYLower0()) {
					_app->objectPresentationShow(kObject10107, 0);
					_app->varSetByte(10105, 0);
				} else {
					_app->objectPresentationShow(kObject10107, 13);
					_app->objectSetAccessibilityOff(kObject10107, 0, 0);
					_app->objectSetAccessibilityOn(kObject10107, 1, 1);
					_app->varSetByte(10105, 1);
				}

				_event->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);
			}

			sub_445930();
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 index = 0;

				if (a2) {
					index = 13 - _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYHigher0())
						index = 13;
				} else {
					index = _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYLower0())
						index = 0;
				}

				_app->varSetByte(10105, 0);

				if (index >= 0) {
					if (index > 12) {
						index = 13;
						_app->varSetByte(10105, 1);
					}
				} else {
					index = 0;
				}

				sub_445930();
				_app->objectPresentationHide(kObject10107);
				_app->objectPresentationShow(kObject10107, (uint32)index);
				sub_445930();
			}
			break;
		}
		break;

	case kObject10201:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(299, 214, 431, 356));
			break;

		case 2:
			if (!_app->soundIsPlaying(10401))
				_app->soundStop(10401, 1024);

			if (_presentationIndexNI <= 3) {
				while (_presentationIndexNI > 0) {
					--_presentationIndexNI;

					_app->objectPresentationHide(kObject10201);
					_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

					handleEvents();
				}
			} else {
				if (_presentationIndexNI <= 10) {
					int32 offset = (_presentationIndexNI >= 6) ? -1 : 1;
					while (_presentationIndexNI != 6) {
						_presentationIndexNI += offset;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					uint32 startTick = g_system->getMillis();
					while (g_system->getMillis() - startTick < 1000)
						handleEvents();

					while (_presentationIndexNI > 0) {
						--_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					_app->playMovie("1527");

					if (_app->varGetByte(10200)) {
						_app->puzzleSetActive(kPuzzle10202);
						_app->soundPlay(10030);
					} else {
						_app->varSetByte(10200, 1);
						_app->puzzleSetActive(kPuzzle10203);
						_app->soundPlay(10027);
						_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					}
				} else {
					_app->objectSetAccessibilityOff(kObject10201);
					while (_presentationIndexNI < 13) {
						++_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					uint32 startTick = g_system->getMillis();
					while (g_system->getMillis() - startTick < 1000)
						handleEvents();

					while (_presentationIndexNI > 0) {
						--_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_presentationIndexNI);

						handleEvents();
					}

					_app->playMovie("1525");
					_app->playMovie("1526");
					_app->objectSetAccessibilityOn(kObject10200);
				}
			}
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				_presentationIndexNI = _app->dragControlGetOffsetY0() / 5;

				if (_app->dragControlYLower0()) {
					_presentationIndexNI = 0;
				} else if (_presentationIndexNI >= 0) {
					if (_presentationIndexNI > 13)
						_presentationIndexNI = 13;
				} else {
					_presentationIndexNI = 0;
				}

				_app->objectPresentationHide(kObject10201);
				_app->objectPresentationShow(10201, (uint32)_presentationIndexNI);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;

	case kObject10420:
	case kObject10421:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(263, 206, 390, 341));
			_stateBagNI = (a2 != 0);
			break;

		case 2:
			_app->objectPresentationHide(id);
			if (a2) {
				if (_app->dragControlGetOffsetY0() < 30 || _app->dragControlYHigher0()) {
					_app->objectPresentationShow(id, 12);
					_app->varSetByte(id, 1);
				} else {
					_app->objectPresentationShow(id, 1);
					_app->objectSetAccessibilityOn(id, 0, 0);
					_app->objectSetAccessibilityOff(id, 1, 1);
					_app->varSetByte(id, 0);
				}
			} else {
				if (_app->dragControlGetOffsetY0() < 30 || _app->dragControlYLower0()) {
					_app->objectPresentationShow(id, 1);
					_app->varSetByte(id, 0);
				} else {
					_app->objectPresentationShow(id, 12);
					_app->objectSetAccessibilityOff(id, 0, 0);
					_app->objectSetAccessibilityOn(id, 1, 1);
					_app->varSetByte(id, 1);
				}
			}

			_event->sub_445A10();
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 offset = 0;
				if (a2) {
					offset = 11 - _app->dragControlGetOffsetY0() / 5;

					if (_app->dragControlXHigher0())
						offset = 12;
				} else {
					offset = _app->dragControlGetOffsetY0() / 5 + 1;

					if (_app->dragControlXLower0())
						offset = 1;
				}

				if (offset < 1)
					offset = 1;

				if (offset > 12)
					offset = 12;

				_app->objectPresentationHideAndRemove(id);
				_app->objectPresentationShow(id, (uint32)offset);

				if (offset <= 8 || _stateBagNI) {
					if (offset < 5 && _stateBagNI) {
						_app->soundStop(10404, 1024);
						_app->soundPlay(10403);
						_app->varSetFloat(90005, _app->varGetFloat(90005) - 3.0f);

						_stateBagNI = false;
					}
				} else {
					_app->soundPlay(10402);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);

					_stateBagNI = true;

					if (_app->varGetByte(10107)) {
						_app->varSetByte(10432, _app->varGetByte(10432) / 2);

						if (id == kObject10420)
							_app->objectSetAccessibilityOff(kObject10420);

						_app->soundPlay(10404);
						_event->sub_445A10();
					}
				}
			}
			break;
		}
		break;

	case kObject10601:
	case kObject10602:
	case kObject10603:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexNI = _app->varGetWord(id - 1);
			break;

		case 2:
			_app->varSetWord(id - 1, (int16)_presentationIndexNI);

			if (!_app->varGetWord(10600)
			 && !_app->varGetWord(10601)
			 && !_app->varGetWord(10602)) {
				_app->varSetWord(10600, 12);
				_app->varSetWord(10601, 0);
				_app->varSetWord(10602, 24);
				_app->playMovie("1524");
				_app->rotationSetAlp(10501, 232.0f);
				_app->rotationSetActive(10501);
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				if (_app->dragControlXLower0()) {
					_presentationIndexNI = _app->varGetWord(id - 1) - ((int32)_app->dragControlGetOffsetX0() / 12);
				} else if (_app->dragControlXHigher0()) {
					_presentationIndexNI = _app->varGetWord(id - 1) + ((int32)_app->dragControlGetOffsetX0() / 12);
				}

				if (_presentationIndexNI < 0)
					_presentationIndexNI  = 47;

				if (_presentationIndexNI > 47)
					_presentationIndexNI %= 48;

				_app->objectPresentationHideAndRemove(id);
				_app->objectPresentationShow(id);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;
	}
}

void EventBagRing::onBagZoneFO(ObjectId id, uint32, Id, uint32, DragControl *, byte type) {
	if (id != kObject30016)
		return;

	switch (type) {
	default:
		break;

	case 1:
		_app->dragControlSetField45(2);
		_app->soundPlay(30500);
		_app->dragControlSetCoords1(Common::Point(440, 248));
		break;

	case 2:
		_app->soundStop(30500, 1024);
		break;

	case 3:
		if (_app->dragControlXHigher1() && _app->dragControlYLower1()) {
			if (_app->dragControlXHigher() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXLower() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXHigher1() && _app->dragControlYHigher1()) {
			if (_app->dragControlXLower() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXHigher() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXLower1() && _app->dragControlYHigher1()) {
			if (_app->dragControlXLower() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXHigher() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXLower1() && _app->dragControlYLower1()) {
			if (_app->dragControlXHigher() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXLower() &&_app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		_app->soundSetVolume(30500, _app->dragControlGetDistance() + 80);
		break;
	}
}

void EventBagRing::onBagZoneRO(ObjectId id, uint32 a2, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject40060:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(0, 0, 640, 480));

			_app->soundPlay(40102, kSoundLoop);

			_presentationIndexBagRO2 = 71;
			_presentationIndexBagRO = _app->varGetByte(40804);
			break;

		case 2: {
			_app->soundStop(40102, 1024);

			_event->_presentationIndexRO = _presentationIndexBagRO;
			float test = 10 * (_presentationIndexBagRO * 0.1000000014901161f);
			float index = test;
			int direction = -1;

			if ((test + 5) < _presentationIndexBagRO) {
				direction = 1;
				index = test + 10;
			}

			if (_presentationIndexBagRO != index) {
				do {
					_event->_presentationIndexRO += direction;

					_app->objectPresentationHide(kObject40060);
					_app->objectPresentationShow(kObject40060, (uint32)_event->_presentationIndexRO);

					handleEvents();
					if (checkEscape())
						break;

				} while (_event->_presentationIndexRO != index);
			}

			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOn(kObject40060, (uint32)_event->_presentationIndexRO / 10, (uint32)_event->_presentationIndexRO / 10);
			_app->varSetByte(40804, (uint8)_event->_presentationIndexRO);
			}
			break;

		case 3: {
			if (_app->bagHasClickedObject()) {
				_app->cursorDelete();
				break;
			}

			if (_app->dragControlXEqual()) {
				_app->soundStop(40102, 1024);
				break;
			}

			if (!_app->soundIsPlaying(40102))
				_app->soundPlay(40102, kSoundLoop);

			_app->soundSetVolume(40102, _app->dragControlGetOffsetX() / 2 + 80);

			int32 offset = (int32)(_app->dragControlGetOffsetX() * 0.1666666666666667f);

			if (_app->dragControlXLower())
				_presentationIndexBagRO += offset;

			if (_app->dragControlXHigher())
				_presentationIndexBagRO -= offset;

			if (_presentationIndexBagRO < 0)
				_presentationIndexBagRO = 0;

			if (_presentationIndexBagRO > _presentationIndexBagRO2)
				_presentationIndexBagRO = _presentationIndexBagRO2;

			_app->objectPresentationHide(kObject40060);
			_app->objectPresentationShow(kObject40060, (uint32)_presentationIndexBagRO);
			}
			break;
		}
		break;

	case kObject40101:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexBagRO = _app->varGetByte(a2 + 40601);
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(0, 0, 640, 480));
			_app->soundPlay(40102, kSoundLoop);
			// Original sets unused value
			_presentationIndexBagRO2 = 97;
			break;

		case 2: {
			_event->_presentationIndexRO = _presentationIndexBagRO;

			float test = 10 * (_presentationIndexBagRO * 0.1000000014901161f);
			float index = test;
			int direction = -1;

			if ((test + 5) < _presentationIndexBagRO) {
				direction = 1;
				index = test + 10;
			}

			if (_presentationIndexBagRO != index) {
				do {
					_event->_presentationIndexRO += direction;

					_app->objectPresentationHide((ObjectId)(a2 + 40101));
					_app->objectPresentationShow((ObjectId)(a2 + 40101), (uint32)_event->_presentationIndexRO);

					handleEvents();
					if (checkEscape())
						break;

				} while (_event->_presentationIndexRO != index);
			}

			_app->soundStop(40102, 1024);
			_app->varSetByte(a2 + 40601, (int8)_event->_presentationIndexRO);

			if (_app->varGetByte(40601) == 10
			 && _app->varGetByte(40602) == 90
			 && _app->varGetByte(40603) == 60
			 && _app->varGetByte(40604) == 50
			 && (_app->varGetByte(40605) == 50 || _app->varGetByte(40605) == 40)) {
				_app->varSetByte(40702, 1);
				_app->objectPresentationHide(kObject40101);
				_app->objectPresentationHide(kObject40102);
				_app->objectPresentationHide(kObject40103);
				_app->objectPresentationHide(kObject40104);
				_app->objectPresentationHide(kObject40105);
				_app->objectSetAccessibilityOff(kObject40101);
				_app->objectSetAccessibilityOff(kObject40102);
				_app->objectSetAccessibilityOff(kObject40103);
				_app->objectSetAccessibilityOff(kObject40104);
				_app->objectSetAccessibilityOff(kObject40105);

				if (_app->varGetByte(40605) == 50)
					_app->playMovie("1783");

				if (_app->varGetByte(40605) == 40)
					_app->playMovie("1784");

				_app->varSetFloat(90006, 75.0);

				_app->puzzleSetActive(kPuzzle40012);
			}
			}
			break;

		case 3:
			if (_app->bagHasClickedObject()) {
				_app->cursorDelete();
				break;
			}

			if (!_app->dragControlYEqual()) 	{
				if (!_app->soundIsPlaying(40102))
					_app->soundPlay(40102, kSoundLoop);

				// Original sets unused value

				_app->soundSetVolume(40102, _app->dragControlGetOffsetY() / 2 + 80);

				if (_app->dragControlYHigher())
					_presentationIndexBagRO += (int32)_app->dragControlGetOffsetY();

				if (_app->dragControlYLower())
					_presentationIndexBagRO -= (int32)_app->dragControlGetOffsetY();

				if (_presentationIndexBagRO < 0)
					_presentationIndexBagRO = _presentationIndexBagRO2;

				if (_presentationIndexBagRO2 < _presentationIndexBagRO)
					_presentationIndexBagRO = 0;

				_app->objectPresentationHide((ObjectId)(a2 + 40101));
				_app->objectPresentationShow((ObjectId)(a2 + 40101), (uint32)_presentationIndexBagRO);
				break;
			}

			_app->soundStop(40102, 1024);
			break;
		}
		break;
	}
}

void EventBagRing::onBagZoneN2(ObjectId id, uint32 a2, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject70103:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexN2 = 0;
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(495, 194, 598, 284));
			break;

		case 2:
			if (!_app->soundIsPlaying(70401))
				_app->soundStop(70401, 1024);

			_app->objectSetAccessibilityOff(kObject70103);

			if (_presentationIndexN2 <= 6) {
				while (_presentationIndexN2 > 0) {
					--_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject70103);
			} else {
				while (_presentationIndexN2 < 12) {
					++_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				uint32 startTick = g_system->getMillis();
				while (g_system->getMillis() - startTick < 1000)
					handleEvents();

				while (_presentationIndexN2 > 0) {
					--_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject70103);
				_app->playMovie("1498");
				_app->rotationSetAlp(70001, 270.0f);
				_app->rotationSetRan(70001, 85.7f);
				_app->rotationSetActive(70001);
				_event->sub_433EE0();
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				_presentationIndexN2 = _app->dragControlGetOffsetX0() / 5;

				if (_app->dragControlXLower0()) {
					_presentationIndexN2 = 0;
				} else if (_presentationIndexN2 >= 0) {
					if (_presentationIndexN2 > 12)
						_presentationIndexN2 = 12;
				} else {
					_presentationIndexN2 = 0;
				}

				_app->objectPresentationHide(kObject70103);
				_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

				if (!_app->soundIsPlaying(70401))
					_app->soundPlay(70401);
			}
			break;
		}
		break;

	case kObject70106:
		switch (type) {
		default:
			break;

		case 1: {
			_app->dragControlSetCoords1(Common::Point(243, 276));

			int32 val1 = 0;
			int32 val2 = 0;
			int32 offsetX1 = 0;
			int32 offsetY1 = 0;

			if (_app->dragControlXLower1())
				offsetX1 = -(int32)_app->dragControlGetOffsetX1();
			else
				offsetX1 = _app->dragControlGetOffsetX1();

			if (_app->dragControlYLower1())
				offsetY1 = -(int32)_app->dragControlGetOffsetY1();
			else
				offsetY1 = _app->dragControlGetOffsetY1();

			if (offsetX1 <= 0) {
				if (offsetY1 > 0) {
					val1 = (offsetY1 + offsetX1 + 40) / 6;
					val2 = 30;
				} else {
					val1 = ((offsetY1 - offsetX1) + 40) / 6;
					val2 = 20;
				}
			} else {
				if (offsetY1 > 0) {
					val1 = ((offsetX1 - offsetY1) + 40) / 6;
					val2 = 0;
				} else {
					val1 = (40 - (offsetX1 + offsetY1)) / 6;
					val2 = 10;
				}
			}

			if (val1 < 0)
				val1 = 0;

			_distN2 = val1 + val2 - _app->varGetWord(70016);
			if (_distN2 < 0)
				_distN2	= 0;

			if (_distN2 > 18)
				_distN2 %= 19;
			}
			break;

		case 2: {
			int32 offset = 0;
			bool update = true;

			switch (_presentationIndexN2) {
			default:
				offset = 2;
				break;

			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 13:
			case 14:
			case 15:
				offset = -1;
				break;

			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 16:
			case 17:
			case 18:
				offset = 1;
				break;

			case 12:
				// Skip update
				update = false;
				break;
			}

			if (update) {
				do {
					_presentationIndexN2 += offset;

					if (_presentationIndexN2 <= 18) {
						if (_presentationIndexN2 < 0)
							_presentationIndexN2 = 18;
					} else {
						_presentationIndexN2 = 0;
					}

					_app->objectPresentationHideAndRemove(kObject70106);
					_app->objectPresentationShow(kObject70106, (uint32)_presentationIndexN2);

					handleEvents();

				} while (_presentationIndexN2 != 12);
			}

			_app->varSetWord(70016, (int16)offset);

			if (_presentationIndexN2 == 12 ) {
				_app->varSetByte(10104, 1);
				_app->rotationSetMovabilityOff(70101, 1, 1);
				_app->rotationSetMovabilityOn(70101, 2, 2);
			} else {
				_app->varSetByte(10104, 0);
				_app->rotationSetMovabilityOn(70101, 1, 1);
				_app->rotationSetMovabilityOff(70101, 2, 2);
			}
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				int32 val1 = 0;
				int32 val2 = 0;
				int32 offsetX1 = 0;
				int32 offsetY1 = 0;

				if (_app->dragControlXLower1())
					offsetX1 = -(int32)_app->dragControlGetOffsetX1();
				else
					offsetX1 = _app->dragControlGetOffsetX1();

				if (_app->dragControlYLower1())
					offsetY1 = -(int32)_app->dragControlGetOffsetY1();
				else
					offsetY1 = _app->dragControlGetOffsetY1();

				// Check interval
				if (offsetX1 > 40 || offsetY1 > 40 || offsetX1 < -40 || offsetY1 < -40)
					break;

				if (offsetX1 <= 0) {
					if (offsetY1 > 0) {
						val1 = (offsetY1 + offsetX1 + 40) / 6;
						val2 = 30;
					} else {
						val1 = ((offsetY1 - offsetX1) + 40) / 6;
						val2 = 20;
					}
				} else {
					if (offsetY1 > 0) {
						val1 = ((offsetX1 - offsetY1) + 40) / 6;
						val2 = 0;
					} else {
						val1 = (40 - (offsetX1 + offsetY1)) / 6;
						val2 = 10;
					}
				}

				if (val1 < 0)
					val1 = 0;

				_presentationIndexNI = val1 + val2 + _distN2;
				if (_presentationIndexNI < 0)
					_presentationIndexNI	= 0;

				if (_presentationIndexNI > 18)
					_presentationIndexNI %= 19;

				_app->objectPresentationHide(kObject70106);
				_app->objectPresentationShow(kObject70106, (uint32)_presentationIndexN2);
				_app->varSetWord(70016, (int16)_presentationIndexN2);
			}
			break;
		}
		break;

	case kObject70108:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(389, 270, 434, 390));
			break;

		case 2:
			_app->objectPresentationHide(kObject70108);

			if (a2) {
				if (_app->dragControlGetOffsetY0() < 20 || _app->dragControlYHigher0()) {
					_app->objectPresentationShow(kObject70108, 13);
					_app->varSetByte(70012, 1);
					_app->objectPresentationShow(kObject70108, 14);
					break;
				}

				_app->objectPresentationShow(kObject70108, 0);
				_app->objectSetAccessibilityOn(kObject70108, 0, 0);
				_app->objectSetAccessibilityOff(kObject70108, 1, 1);
			} else {
				if (_app->dragControlGetOffsetY0() >= 20 && !_app->dragControlYLower0()) {
					_app->objectPresentationShow(kObject70108, 13);
					_app->objectSetAccessibilityOff(kObject70108, 0, 0);
					_app->objectSetAccessibilityOn(kObject70108, 1, 1);
					_app->varSetByte(70012, 1);
					_app->objectPresentationShow(kObject70108, 14);
					_app->soundPlay(70101);

					_app->playMovie("1496");
					_app->playMovie("1497");

					_app->varSetFloat(90006, _app->varGetFloat(90006) + 10.0f);
					_app->objectPresentationHide(kObjectFire);
					_app->objectPresentationHide(kObject70108);
					_app->objectSetAccessibilityOff(kObject70700);
					_app->objectPresentationHide(kObject70700);
					_app->soundStop(70701, 1024);
					_app->rotationSet3DSoundOff(70200, 70701);
					break;
				}

				_app->objectPresentationShow(kObject70108, 0);
			}

			_app->varSetByte(70012, 0);
			_app->objectPresentationHide(kObject70108, 14);
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 offset = 0;

				if (a2) {
					offset = 13 - _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYHigher0())
						offset = 13;
				} else {
					offset = _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYLower0())
						offset = 0;
				}

				_app->objectPresentationHide(kObject70108);

				if (offset < 0)
					offset = 0;

				if (offset > 12)
					offset = 13;

				_app->objectPresentationShow(kObject70108, 14);
				_app->objectPresentationShow(kObject70108, (uint32)offset);
			}
			break;
		}
		break;
	}
}

#pragma endregion

#pragma region Input

void EventBagRing::onBagClickedObjectZoneFO(ObjectId objectId) {
	if (objectId != kObjectWolfInstinct)
		return;

	Id id = objectId;
	if (_app->hasCurrentRotation())
		id = _app->getCurrentRotationId();

	if (_app->hasCurrentPuzzle())
		id = _app->getCurrentPuzzleId();

	// Rotation
	if (id == 30302 || id == 30303) {
		if (_app->varGetByte(30017) == 1) {
			_app->rotationSetAlp(30302, _app->rotationGetAlp(30303));
			_app->rotationSetBet(30302, _app->rotationGetBet(30303));
			_app->rotationSetRan(30302, _app->rotationGetRan(30303));
			_app->rotationSetActive(30302);

			_app->varSetByte(30017, 0);
		} else {
			_app->rotationSetAlp(30303, _app->rotationGetAlp(30302));
			_app->rotationSetBet(30303, _app->rotationGetBet(30302));
			_app->rotationSetRan(30303, _app->rotationGetRan(30302));
			_app->rotationSetActive(30303);

			_app->varSetByte(30017, 1);
		}
	} else if (id == kPuzzle35020) { //  Puzzle 35020
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectWorms, 0);
			_app->objectPresentationHide(kObjectWorms, 1);
			_app->objectSetAccessibilityOff(kObjectWorms, 0, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectWorms, 0);
			_app->objectPresentationShow(kObjectWorms, 1);
			_app->objectSetAccessibilityOn(kObjectWorms, 0, 0);
		}
	} else if (id == kPuzzle35019) { //  Puzzle 35019
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectNotung);
			_app->objectSetAccessibilityOff(kObjectNotung, 0, 0);
		}

		_app->varSetByte(30017, 1);
		_app->objectSetAccessibilityOn(kObjectNotung, 0, 0);

		if (_app->varGetByte(30066) != 1 && _app->varGetByte(30067) != 1) {
			_app->objectPresentationShow(kObjectNotung, 0);
		} else {
			_app->objectPresentationShow(kObjectNotung, 0);
			_app->objectPresentationShow(kObjectNotung, 2);
		}
	} else if (id == kPuzzle35002) { //  Puzzle 35002
		if (_app->varGetByte(30017)) {
			_app->objectPresentationHide(kObjectBerries, 1);
			_app->objectPresentationHide(kObjectBerries, 2);
			_app->objectPresentationHide(kObjectBerries, 3);
			_app->varSetByte(30017, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectBerries, 1);

			if (_app->varGetByte(30075) == 1)
				_app->objectPresentationHide(kObjectBerries, 3);
			else
				_app->objectPresentationShow(kObjectBerries, 3);

			if (_app->varGetByte(30076) == 1)
				_app->objectPresentationHide(kObjectBerries, 2);
			else
				_app->objectPresentationShow(kObjectBerries, 2);
		}
	} else {
		return;
	}

	_app->setField78(false);
}

#pragma endregion

#pragma region Update

void EventBagRing::onUpdateBagZoneSY(const Common::Point &) {
	_app->objectPresentationHide(kObjectMenuNewGame, 0);
	_app->objectPresentationHide(kObjectMenuPreferences, 0);
	_app->objectPresentationHide(kObjectMenuLoad, 0);
	_app->objectPresentationHide(kObjectMenuSave, 0);
	_app->objectPresentationHide(kObjectMenuContinue, 0);
	_app->objectPresentationHide(kObjectMenuExit, 0);
	_app->objectPresentationHide(kObjectPreferencesCancel, 0);
	_app->objectPresentationHide(kObjectPreferencesOk, 0);
	_app->objectPresentationHide(kObjectPreferences3dSound, 2);
	_app->objectPresentationHide(kObjectLoadOk, 0);
	_app->objectPresentationHide(kObjectLoadCancel, 0);
	_app->objectPresentationHide(kObjectSaveOk, 0);
	_app->objectPresentationHide(kObjectSaveCancel, 0);
	_app->objectPresentationHide(kObjectMenuStatus, 0);
	_app->objectPresentationHide(kObjectStatusOk, 0);
	_app->objectPresentationHide(kObject2, 1);
	_app->objectPresentationHide(kObject2, 2);
	_app->objectPresentationHide(kObject4, 1);
	_app->objectPresentationHide(kObject4, 2);
	_app->objectPresentationHide(kObject3, 1);
	_app->objectPresentationHide(kObject90912, 0);

	_app->cursorSelect(kCursorMenuIdle);
}

#pragma endregion

#pragma region Switch

void EventBagRing::onBagZoneSwitch() {
	switch (getApp()->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
	case kZoneAS:
		error("[EventBagRing::onBagZoneSwitch] Erda should not be active on this zone (%d)", getApp()->getCurrentZone());

	case kZoneNI:
	case kZoneRH:
		if (!getApp()->getSaveManager()->loadSaveTimer("alb", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (alb)");

		_app->varSetDword(90013, _app->getCurrentZone());
		_app->varSetByte(90009, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90017, 1);
			_app->varSetDword(90021, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90017, 0);
			_app->varSetDword(90021, _app->getCurrentRotationId());
			_app->varSetByte(90025, getApp()->getBag()->getRotationFre());
		}

		_event->onSwitchZone(kZoneAS, 13);
		break;

	case kZoneFO:
		if (!getApp()->getSaveManager()->loadSaveTimer("sie", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (sie)");

		_app->varSetDword(90015, _app->getCurrentZone());
		_app->varSetByte(90011, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90019, 1);
			_app->varSetDword(90023, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90019, 0);
			_app->varSetDword(90023, _app->getCurrentRotationId());
			_app->varSetByte(90027, getApp()->getBag()->getRotationFre());
		}

		_event->onSwitchZone(kZoneAS, 13);
		break;

	case kZoneRO:
	case kZoneN2:
		if (!getApp()->getSaveManager()->loadSaveTimer("log", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (log)");

		_app->varSetDword(90014, _app->getCurrentZone());
		_app->varSetByte(90010, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90018, 1);
			_app->varSetDword(90022, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90018, 0);
			_app->varSetDword(90022, _app->getCurrentRotationId());
			_app->varSetByte(90026, getApp()->getBag()->getRotationFre());
		}

		_event->onSwitchZone(kZoneAS, 13);
		break;

	case kZoneWA:
		if (!getApp()->getSaveManager()->loadSaveTimer("bru", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (bru)");

		_app->varSetDword(90016, _app->getCurrentZone());
		_app->varSetByte(90012, 1);

		if (_app->hasCurrentPuzzle()) {
			_app->varSetByte(90020, 1);
			_app->varSetDword(90024, _app->getCurrentPuzzleId());
		} else {
			_app->varSetByte(90020, 0);
			_app->varSetDword(90024, _app->getCurrentRotationId());
			_app->varSetByte(90028, getApp()->getBag()->getRotationFre());
		}

		_event->onSwitchZone(kZoneAS, 13);
		break;
	}
}

#pragma endregion

#pragma region Helper functions

void EventBagRing::sub_445930() {
	if (_app->varGetByte(10105) == 1) {

		_app->objectPresentationHide(kObject10106);
		_app->objectPresentationShow(kObject10106, 38);
		_app->objectPresentationShow(kObject10106, (uint16)_app->varGetWord(10100) + 19);

		if (!_app->varGetWord(10100)) {
			_app->objectPresentationShow(kObject10106, 39);
			_app->varSetByte(10106, 1);
		}

	} else {
		_app->objectPresentationHide(kObject10106);
		_app->objectPresentationShow(kObject10106, (uint16)_app->varGetWord(10100));
		_app->varSetByte(10106, 0);
	}
}

#pragma endregion

} // End of namespace Ring
