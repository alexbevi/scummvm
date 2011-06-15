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

#include "ring/game/faust/faust_sound.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_event.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventSoundFaust::EventSoundFaust(ApplicationFaust *application, EventHandlerFaust *eventHandler) : _app(application), _event(eventHandler) {
}

EventSoundFaust::~EventSoundFaust() {
	// Zero-out passed pointers
	_app   = NULL;
	_event = NULL;
}

void EventSoundFaust::onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 90806:
		_app->exitToMenu(kMenuAction1);
		break;

	case 111042:
		_app->objectPresentationHide(kObject15);
		_app->objectSetAccessibilityOff(kObject1);
		_event->sub_468290(3);
		break;
	}
}

void EventSoundFaust::onSoundZone3(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 12001:
		_app->soundPlay(12002);
		break;

	case 12002:
		_app->soundPlay(12003);
		break;

	case 12004:
		_app->soundPlay(12005);
		break;

	case 12005:
		_app->soundPlay(12006);
		break;

	case 12008:
		_app->soundPlay(12009);
		break;

	case 12009:
		_app->soundPlay(12010);
		break;

	case 12010:
		_app->soundPlay(12011);
		break;

	case 12014:
		_app->soundPlay(12015);
		break;

	case 12015:
		_app->soundPlay(12016);
		break;

	case 12017:
		_app->soundPlay(12018);
		break;

	case 12018:
		_app->soundPlay(12019);
		break;

	case 12024:
	case 12025:
		_app->objectPresentationHide(kObject16);
		break;
	}
}

void EventSoundFaust::onSoundZone4(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventSoundFaust::onSoundZone4] Not implemented");
}

void EventSoundFaust::onSoundZone5(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	if (id == 17002) {
		_app->playMovie("1357");
		_app->objectPresentationHide(kObject17204, 0);
		_app->rotationSetMovabilityOn(17002);
		_app->objectSetAccessibilityOff(kObject18, 1, 1);
	}
}

void EventSoundFaust::onSoundZone8(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 53125:
		if (_app->bagHasClickedObject()
		 && _app->bagGetClickedObject() == kObjectMatches2)
			_app->cursorDelete();
		break;

	case 53198:
		_app->soundPlay(53145);
		break;

	case 522011:
	case 522012:
	case 522013:
	case 531411:
	case 531412:
	case 531413:
	case 542021:
		_app->soundPlay(id + 1);
		break;

	case 531021:
		if (_app->varGetByte(52999) > 3) {
			_app->soundStop(52000, 1024);
			_app->soundStop(53103, 1024);
			_app->timerStop(kTimer6);
			_app->rotationSet3DSoundOff(53011, 53102);
			_app->rotationSet3DSoundOff(53021, 53102);

			_app->displayFade("a05_black.bmp", "A15_E07M09a.bmp", 15, 0, kLoadFromCd, kArchiveArt);
			_app->playMovie("1509");
			_app->displayFade("A15_E07M09b.bmp", "a05_black", 15, 0, kLoadFromCd, kArchiveArt);

			_app->rotationSetAlp(55011, 0.0f);
			_app->rotationSetBet(55011, 0.0f);
			_app->rotationSetRan(55011, 87.0f);
			_app->rotationSetActive(55011);
			_app->soundPlay(51001, kSoundLoop);
		}
		break;
	}
}

void EventSoundFaust::onSoundZone12(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 81030:
		_app->soundPlay(81031);
		break;

	case 81040:
		if (_app->varGetByte(81012) == 1
		 && _app->varGetByte(81022) == 1
		 && _app->bagHas(kObjectDiaryOfASeducer)) {
			_app->soundStopAll(1024);
			_app->exitToMenu(kMenuAction0);
		}
		break;

	case 81041:
		_app->playMovie("1717");
		break;
	}
}

void EventSoundFaust::onSoundZone14(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventSoundFaust::onSoundZone14] Not implemented");
}

void EventSoundFaust::onSoundZone15(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 120101:
	case 120102:
	case 120110:
	case 120120:
	case 120130:
	case 120131:
	case 120132:
	case 120133:
	case 120134:
	case 120136:
	case 120137:
	case 120138:
	case 120145:
	case 120146:
	case 120147:
		_app->soundPlay(id + 1);
		break;

	case 120022:
		if (_app->bagHasClickedObject()
		 && _app->bagGetClickedObject() == kObjectMatches4)
			_app->cursorDelete();
		break;

	case 120139:
		_app->playMovie("2148");
		_app->displayFade("A12S06N02R01_FADE_02.bmp", "A12S06N02R01_FADE_01.bmp", 12, 0, kLoadFromCd, kArchiveArt);
		_app->rotationSetAlp(120602, 98.0f);
		_app->rotationSetBet(120602, 16.0f);
		_app->rotationSetRan(120602, 71.0f);
		_app->rotationSetActive(120602);
		_app->rotationSetRolTo(120602, 98.0f, 16.0f, 87.0f);
		break;

	case 120150:
		_app->objectPresentationHide(kObject15);
		_app->objectSetAccessibilityOff(kObject1);
		_app->rotationSetAlp(_app->varGetDword(120007), _app->varGetWord(120006));
		_app->rotationSetBet(_app->varGetDword(120007), _app->varGetWord(120007));
		_app->rotationSetRan(_app->varGetDword(120007), _app->varGetWord(120008));
		_app->rotationSetActive(_app->varGetDword(120007));

		_app->varSetWord(120001, 130);
		_app->varSetWord(120002, 130);
		_app->varSetByte(98088, 1);
		_app->objectPresentationShow(kObject6, 0);
		_app->objectPresentationShow(kObject6, 1);
		_app->objectPresentationShow(kObject6, 2);
		_app->timerStart(kTimer1, 2090);
		_app->timerStart(kTimer2, 3136);
		_app->objectSetAccessibilityOn(kObject6);
		break;
	}
}

void EventSoundFaust::onSoundZone17(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 143001:
		_app->soundPlay(143002);
		break;

	case 143002:
		_app->playMovie("2183");
		_app->soundPlay(143003);
		break;

	case 143003:
		_app->playMovie("2184");
		_app->fadeOut(15, Color(0, 0, 0), 0);
		_app->exitToMenu(kMenuAction0);
		break;
	}
}

} // End of namespace Ring
