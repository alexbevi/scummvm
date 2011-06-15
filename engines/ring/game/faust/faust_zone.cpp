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

#include "ring/game/faust/faust_zone.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_event.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventZoneFaust::EventZoneFaust(ApplicationFaust *application, EventHandlerFaust *eventHandler) : _app(application), _event(eventHandler) {
}

EventZoneFaust::~EventZoneFaust() {
	// Zero-out passed pointers
	_app   = NULL;
	_event = NULL;
}

void EventZoneFaust::onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventZoneFaust::onUpdateBeforeZoneSY] Not implemented");
}

void EventZoneFaust::onUpdateBeforeZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityFrom == 12036 && !_app->soundIsPlayingType(kSoundTypeDialog)) {
		switch (movabilityTo) {
		default:
			break;

		case 0:
			_app->soundPlay(12022);
			_app->varSetDword(12001, 12022);
			break;

		case 1:
			_app->soundPlay(12023);
			_app->varSetDword(12001, 12023);
			break;
		}
	}
}

void EventZoneFaust::onUpdateBeforeZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventZoneFaust::onUpdateBeforeZone4] Not implemented");
}

void EventZoneFaust::onUpdateBeforeZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityFrom == 31003 && !_app->soundIsPlayingType(kSoundTypeDialog) && movabilityTo == 0) {
		_app->soundPlay(31101);
		_app->varSetDword(31000, 31101);
	}
}

void EventZoneFaust::onUpdateBeforeZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityFrom == 53123 && movabilityTo == 1) {
		if (_app->varGetByte(53123) == 1 && _app->bagGetClickedObject() == kObjectMatches2) {
			_app->objectPresentationShow(kObject53123, 2);
			_app->varSetByte(53121, 2);
			_app->soundPlay(53126);
			_app->soundPlay(53123, kSoundLoop);
			_app->puzzleSetMovabilityOff(kPuzzle53102, 1, 1);
			_app->soundStop(53124, 1024);
			_app->varSetByte(53123, 50);
			_app->timerStop(kTimer3);
			_app->timerStop(kTimer4);
			_app->timerStop(kTimer5);

			if (_app->varGetByte(53120) == 1) {
				_app->soundPlay(53121, kSoundLoop);
				_app->timerStart(kTimer3, 300);
			}
		}
	}
}

void EventZoneFaust::onUpdateBeforeZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityFrom == 81900 && !_app->soundIsPlayingType(kSoundTypeDialog)) {
		switch (movabilityTo) {
		default:
			return;

		case 0:
			_app->varSetDword(81001, 81015);
			break;

		case 1:
			_app->varSetDword(81001, 81017);
			break;

		case 2:
			case 3:
			_app->varSetDword(81001, 81042);
			break;

		case 4:
			_app->varSetDword(81001, 81001);
		}

		_app->soundPlay(_app->varGetDword(81001));
	}
}

void EventZoneFaust::onUpdateBeforeZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventZoneFaust::onUpdateBeforeZone14] Not implemented");
}

void EventZoneFaust::onUpdateBeforeZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	error("[EventZoneFaust::onUpdateBeforeZone15] Not implemented");
}

void EventZoneFaust::onUpdateBeforeZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (_app->soundIsPlayingType(kSoundTypeDialog))
		return;

	switch (movabilityFrom) {
	default:
		break;

	case 310001: {
		Id soundId = 310001 + _app->varGetWord(311012 + movabilityTo);
		_app->soundPlay(soundId);
		_app->varSetDword(311011, soundId);
		}
		break;

	case 310016:
		_app->soundPlay(310013);
		_app->varSetDword(311011, 310013);
		break;
	}
}

} // End of namespace Ring
