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

#include "ring/game/ring/ring_sound.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_event.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/helpers.h"
#include "ring/ring.h"

using namespace RingGame;

namespace Ring {

EventSoundRing::EventSoundRing(ApplicationRing *application, EventHandlerRing *eventHandler) : _app(application), _event(eventHandler) {
}

EventSoundRing::~EventSoundRing() {
	// Zero-out passed pointers
	_app   = NULL;
	_event = NULL;
}

void EventSoundRing::onSoundZoneSY(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	if (id == 90001) {
		_app->objectPresentationHideAndRemove(kObject7, 0);
		_app->showCredits();
		_app->startMenu(false);
	}
}

void EventSoundRing::onSoundZoneNI(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 10001:
		_app->objectPresentationShow(kObject10300, 2);
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10002);
		return;

	case 10002:
		_app->bagAdd(kObjectGlug);
		_app->bagAdd(kObjectMinerals);
		_app->playMovie("1543");
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10003);
		return;

	case 10003:
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10004);
		return;

	case 10004:
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		break;

	case 10005:
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10006);
		return;

	case 10006:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10007);
		return;

	case 10007:
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10008);
		return;

	case 10008:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10009);
		return;

	case 10009:
		_app->playMovie("1544");
		_app->bagAdd(kObjectLogeTear);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
		_app->objectPresentationHide(kObjectLogeTear);
		_app->objectPresentationShow(kObjectLogeTear, 0);
		_app->objectSetAccessibilityOff(10305);
		_app->rotationSetActive(10301);
		break;

	case 10010:
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10011);
		return;

	case 10011:
		_app->playMovie("1545");
		_app->bagAdd(kObjectMagicFrog);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
		_app->objectSetAccessibilityOff(kObjectMagicFrog);
		_app->varSetByte(10300, 1);

		if (_app->bagHas(kObjectDivingHelmet)) {
			_app->bagRemove(kObjectMagicFrog);
			_app->bagRemove(kObjectDivingHelmet);
			_app->bagAdd(kObjectDivingHelmet2);
			_app->playMovie("1522");
		}

		_app->rotationSetActive(10301);
		break;

	case 10012:
		_app->playMovie("1546");
		_app->varSetByte(10301, 1);
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10013);
		return;

	case 10014:
		_app->playMovie("1547");
		_app->bagAdd(kObjectTile);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
		_app->objectPresentationShow(kObjectTile, 0);
		_app->objectSetAccessibilityOff(10303);
		_app->rotationSetActive(10301);
		break;

	case 10013:
	case 10018:
	case 10019:
	case 10020:
	case 10021:
		_app->rotationSetActive(10301);
		break;

	case 10015:
	case 10016:
	case 10017:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(rnd(3) + 10018);
		break;

	case 10022:
		_app->puzzleSetActive(kPuzzle10202);
		_app->soundPlay(10023);
		break;

	case 10023:
		_app->puzzleSetActive(kPuzzle10200);
		_app->soundPlay(10024);
		break;

	case 10024:
		_app->puzzleSetActive(kPuzzle10202);
		_app->soundPlay(10025);
		break;

	case 10025:
		_app->puzzleSetActive(kPuzzle10200);
		_app->soundPlay(10026);
		break;

	case 10026:
		_app->objectSetAccessibilityOn(kObject10201, 0, 0);
		break;

	case 10027:
		_app->puzzleSetActive(kPuzzle10205);
		_app->soundPlay(10028);
		break;

	case 10028:
		_app->puzzleSetActive(kPuzzle10203);
		_app->soundPlay(10029);
		break;

	case 10029:
		_app->puzzleSetActive(kPuzzle10204);
		_app->soundPlay(10030);
		break;

	case 10030:
		_app->puzzleSetActive(kPuzzle10203);
		_app->soundPlay(10031);
		break;

	case 10031:
		_app->puzzleSetActive(kPuzzle10204);
		_app->soundPlay(10032);
		break;

	case 10032:
		_app->playMovie("1548");
		_app->playMovie("1549");
		_app->objectSetAccessibilityOn(kObject10200);
		_app->rotationSetActive(10201);
		_app->rotationSetMovabilityOff(10201, 0, 0);
		break;

	case 12001:
		_app->puzzleSetActive(kPuzzle12003);
		_app->soundPlay(12003);
		break;

	case 12002:
		_app->playMovie("1542");
		_app->puzzleSetActive(kPuzzle10521);
		break;

	case 12003:
		_app->puzzleSetActive(kPuzzle12002);
		_app->soundPlay(12002);
		break;
	}
}

void EventSoundRing::onSoundZoneRH(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 20011:
	case 20012:
	case 20013:
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 2);
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 4);
		_app->objectPresentationSetAnimationOnPuzzle((ObjectId)(_app->varGetByte(21001) + 20001), 2, 0, 20002);
		_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20001), 3);
		_app->objectSetAccessibilityOn((ObjectId)(_app->varGetByte(21001) + 20001), 0, 1);
		break;

	case 20021:
	case 20022:
	case 20023:
		_app->playMovie(Common::String::format("rh_%d", _app->varGetByte(21001) + 1));
		_app->playMovie("1696");
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->rotationSetMovabilityOff(10 * (_app->varGetByte(21001) + 2001), 0, 0);
		_app->rotationSetMovabilityOn(10 * (_app->varGetByte(21001) + 2001), 1, 1);
		_app->rotationSetActive(10 * (_app->varGetByte(21001) + 2001));
		break;

	case 20031:
	case 20032:
	case 20033:
		_app->playMovie(Common::String::format("rh_%d_l0", _app->varGetByte(21001) + 1));
		_app->objectSetAccessibilityOff((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->objectSetAccessibilityOn((ObjectId)(_app->varGetByte(21001) + 20004));
		_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20004), 0);
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->puzzleSetActive((PuzzleId)(10 * _app->varGetByte(21001) + 20011));
		break;

	case 20201:
		_app->playMovie("1698");
		_app->bagAdd(kObjectMedallion);
		_app->soundPlay(23009);
		_app->objectSetAccessibilityOff(kObjectDolphin);
		_app->rotationSetActive(20201);
		_app->rotationSetMovabilityOn(20201, 0, 0);
		break;

	case 20202:
		_app->playMovie("1699");
		_app->objectPresentationShow(kObject21003, 0);
		_app->objectPresentationShow(kObject20204, 0);
		_app->rotationSetMovabilityOff(20202, 2, 2);
		_app->rotationSetMovabilityOff(20202, 0, 0);
		_app->rotationSetMovabilityOff(20203, 0, 0);
		_app->rotationSetMovabilityOn(20202, 1, 1);
		_app->rotationSetMovabilityOn(20203, 1, 3);
		_app->rotationSetActive(20202);
		break;

	case 20301:
		_app->objectPresentationHide(kObject20301, 1);
		break;

	case 20302:
		_app->objectPresentationHide(kObject20302, 1);
		_app->bagAdd(kObjectMedallion);
		_app->playMovie("1700");
		_app->rotationSetBet(20302, 0.3f);
		_app->rotationSetActive(20302);
		break;

	case 20303:
		_app->soundStop(23010, 1024);
		_app->playMovie("1687");
		_app->soundPlay(23009);
		_app->rotationSetAlp(20303, 325.0);
		_app->rotationSetBet(20303, 0.3f);
		_app->rotationSetActive(20303);
		break;

	case 20304:
		_app->playMovie("1701");
		_app->objectPresentationShow(kObject20304, 0);
		_app->objectPresentationShow(kObject21003, 1);
		_app->rotationSetMovabilityOff(20304, 2, 2);
		_app->rotationSetMovabilityOff(20304, 0, 0);
		_app->rotationSetMovabilityOff(20305, 0, 0);
		_app->rotationSetMovabilityOn(20304, 1, 1);
		_app->rotationSetMovabilityOn(20305, 1, 2);
		_app->rotationSetActive(20304);
		break;

	case 20401:
		_app->playMovie("1674");
		_app->bagAdd(kObjectAntiGCells2);
		_app->soundPlay(23009);
		_app->objectPresentationHide(kObject20402);
		_app->playMovie("1702");
		_app->rotationSetAlp(20402, 0.0);
		_app->rotationSetActive(20402);
		break;

	case 20402:
		_app->playMovie("1703");
		_app->objectPresentationShow(kObject20404, 0);
		_app->objectPresentationShow(kObject21003, 2);
		_app->rotationSetMovabilityOff(20402, 0, 0);
		_app->rotationSetMovabilityOff(20403, 0, 0);
		_app->rotationSetMovabilityOn(20402, 1, 1);
		_app->rotationSetMovabilityOn(20403, 1, 2);
		_app->rotationSetActive(20402);
		break;

	case 20504:
		_app->playMovie("1705");
		_app->objectPresentationShow(kObject20502, 0);
		_app->rotationSetMovabilityOff(20503, 0, 0);
		_app->rotationSetMovabilityOff(20504, 0, 0);
		_app->rotationSetMovabilityOn(20504, 1, 2);
		_app->rotationSetActive(20503);
		break;

	case 20501:
		_app->puzzleSetActive(kPuzzle20502);
		_app->soundPlay(20502);
		break;

	case 20502:
		_app->objectPresentationHide(kObject20501, 2);
		_app->objectSetAccessibilityOn(kObject20501, 3, 4);
		break;

	case 20503:
		_app->objectPresentationHide(kObject20501, 3);
		_app->playMovie("1704");
		_app->rotationSetAlp(20501, 0.0);
		_app->rotationSetActive(20501);
		break;

	case 22001:
		_app->puzzleSetActive(kPuzzle22003);
		_app->soundPlay(22003);
		break;

	case 22002:
		_app->playMovie("1697");
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->rotationSetAlp(20010, 1.5);
		_app->rotationSetBet(20010, -4.3f);
		_app->rotationSetRan(20010, 79.3f);
		_app->rotationSetActive(20010);
		break;

	case 22003:
		_app->puzzleSetActive(kPuzzle22002);
		_app->soundPlay(22002);
		break;
	}
}

void EventSoundRing::onSoundZoneFO(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 30100:
		_app->soundPlay(30101);
		break;

	case 30101:
		_app->playMovie("1179");
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);

		if (!_app->varGetByte(30074))
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);

		_app->varSetByte(30074, 1);
		break;

	case 30102:
		_app->puzzleSetActive(kPuzzle35103);
		_app->soundPlay(30103);
		break;

	case 30103:
		_app->puzzleSetActive(kPuzzle35104);
		_app->soundPlay(30104);
		break;

	case 30104:
		_app->playMovie("1208");
		_app->puzzleSetActive(kPuzzle35105);
		_app->soundPlay(30106);
		break;

	case 30106:
		_app->playMovie("1209");
		_app->puzzleSetActive(kPuzzle35110);
		_app->soundPlay(30107);
		break;

	case 30107:
		_app->playMovie("1210");
		_app->puzzleSetActive(kPuzzle35106);
		_app->soundPlay(30108);
		break;

	case 30108:
		_app->playMovie("1211");
		_app->puzzleSetActive(kPuzzle35107);
		_app->soundPlay(30161);
		break;

	case 30109:
		_app->playMovie("1213");
		_app->puzzleSetActive(kPuzzle35111);
		_app->objectPresentationShow(kObject30109, 0);
		break;

	case 30118:
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);
		_app->varSetByte(30078, 1);
		break;

	case 30105:
		_app->playMovie("1214");
		_app->bagAdd(kObjectWolfBrooch);
		_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
		_app->objectPresentationShow(kObject30110, 3);
		_app->objectSetAccessibilityOff(kObject30102, 1, 1);
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);
		break;

	case 30120:
		_app->objectPresentationHideAndRemove(kObject6, 0);
		_app->rotationSetActive(30002);
		_app->soundPlay(30121);
		_app->objectPresentationShow(kObject6, 1);
		break;

	case 30121:
		_app->objectPresentationHideAndRemove(kObject6, 1);
		_app->objectPresentationHideAndRemove(kObject6, 5);
		_app->rotationSetActive(30003);
		_app->soundPlay(30122);
		_app->objectPresentationShow(kObject6, 2);
		_app->objectPresentationShow(kObject6, 6);
		break;

	case 30122:
		_app->objectPresentationHideAndRemove(kObject6, 2);
		_app->rotationSetActive(30004);
		_app->soundPlay(30123);
		_app->objectPresentationShow(kObject6, 3);
		break;

	case 30123:
		_app->objectPresentationHideAndRemove(kObject6, 3);
		_app->objectPresentationHideAndRemove(kObject6, 6);
		_app->rotationSetActive(30005);
		_app->soundPlay(30124);
		_app->objectPresentationShow(kObject6, 4);
		_app->objectPresentationShow(kObject6, 7);
		break;

	case 30124:
		_app->objectPresentationHideAndRemove(kObject6, 4);
		_app->objectPresentationHideAndRemove(kObject6, 7);
		_app->rotationSetActive(30006);
		_app->soundPlay(30125);
		_app->objectPresentationShow(kObject6, 8);
		_app->objectPresentationShow(kObject6, 9);
		break;

	case 30125:
		_app->objectPresentationHideAndRemove(kObject6, 9);
		_app->rotationSetActive(30008);
		_app->soundPlay(30126);
		_app->objectPresentationShow(kObject6, 10);
		break;

	case 30126:
		_app->objectPresentationHideAndRemove(kObject6, 8);
		_app->objectPresentationHideAndRemove(kObject6, 10);
		_app->rotationSetActive(30009);
		_app->soundPlay(30127);
		_app->objectPresentationShow(kObject6, 11);
		_app->objectPresentationShow(kObject6, 15);
		break;

	case 30127:
		_app->objectPresentationHideAndRemove(kObject6, 11);
		_app->rotationSetActive(30010);
		_app->soundPlay(30128);
		_app->objectPresentationShow(kObject6, 12);
		break;

	case 30128:
		_app->objectPresentationHideAndRemove(kObject6, 12);
		_app->objectPresentationHideAndRemove(kObject6, 15);
		_app->rotationSetActive(30011);
		_app->soundPlay(30129);
		_app->objectPresentationShow(kObject6, 13);
		_app->objectPresentationShow(kObject6, 16);
		break;

	case 30129:
		_app->objectPresentationHideAndRemove(kObject6, 13);
		_app->rotationSetActive(30012);
		_app->soundPlay(30130);
		_app->objectPresentationShow(kObject6, 14);
		break;

	case 30130:
		_app->objectPresentationHideAndRemove(kObject6, 14);
		_app->objectPresentationHideAndRemove(kObject6, 16);
		_app->rotationSetActive(30701);
		_app->soundPlay(30131);
		_app->objectPresentationShow(kObject6, 17);
		_app->objectPresentationShow(kObject6, 18);
		break;

	case 30131:
		_app->objectPresentationHideAndRemove(kObject6, 18);
		_app->rotationSetActive(30702);
		_app->soundPlay(30132);
		_app->objectPresentationShow(kObject6, 19);
		break;

	case 30132:
		_app->objectPresentationHideAndRemove(kObject6, 17);
		_app->objectPresentationHideAndRemove(kObject6, 19);
		_app->rotationSetActive(30703);
		_app->soundPlay(30133);
		_app->objectPresentationShow(kObject6, 20);
		_app->objectPresentationShow(kObject6, 24);
		break;

	case 30133:
		_app->objectPresentationHideAndRemove(kObject6, 20);
		_app->objectPresentationHideAndRemove(kObject6, 24);
		_app->rotationSetActive(30704);
		_app->soundPlay(30134);
		_app->objectPresentationShow(kObject6, 21);
		_app->objectPresentationShow(kObject6, 25);
		break;

	case 30134:
		_app->objectPresentationHideAndRemove(kObject6, 21);
		_app->rotationSetActive(30401);
		_app->soundPlay(30135);
		_app->objectPresentationShow(kObject6, 22);
		break;

	case 30135:
		_app->objectPresentationHideAndRemove(kObject6, 22);
		_app->objectPresentationHideAndRemove(kObject6, 25);
		_app->rotationSetActive(30402);
		_app->soundPlay(30136);
		_app->objectPresentationShow(kObject6, 23);
		_app->objectPresentationShow(kObject6, 26);
		break;

	case 30136:
		_app->objectPresentationHideAndRemove(kObject6, 23);
		_app->objectPresentationHideAndRemove(kObject6, 26);
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->playMovie("1215");
		_event->onSwitchZone(kZoneAS, 3);
		break;

	case 30161:
		_app->playMovie("1212");
		_app->puzzleSetActive(kPuzzle35108);
		_app->soundPlay(30109);
		break;

	case 30506:
		if (_app->varGetByte(30043) == 1 && _app->bagHas(kObjectSleepingPotion2)) {
			_app->playMovie("1216");
			_app->objectPresentationShow(kObject30100, 0);
			_app->objectSetAccessibilityOff(kObject30100, 0, 0);
			_app->objectSetAccessibilityOn(kObject30100, 1, 1);
		} else {
			_app->objectSetAccessibilityOn(kObject30100, 0, 0);
			_app->puzzleSetMovabilityOn(kPuzzle35100, 0, 0);
		}
		break;
	}
}

void EventSoundRing::onSoundZoneRO(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 40400:
		_app->playMovie("1787");
		_app->objectPresentationShow(kObject40202, 0);
		_app->objectSetAccessibilityOn(kObject40202);
		break;

	case 40603:
		_app->soundStop(40003, 1024);
		_app->timerStopAll();
		_app->playMovie("1786");
		_event->onSwitchZone(kZoneAS, 2);
		break;

	case 40700:
		_app->puzzleSetActive(kPuzzle40101);
		_app->soundPlay(40701);
		break;

	case 40701:
		_app->rotationSetActive(40000);
		break;

	case 40702:
		_app->puzzleSetActive(kPuzzle40102);
		_app->soundPlay(40703);
		break;

	case 40703:
		_app->puzzleSetActive(kPuzzle40103);
		_app->soundPlay(40704);
		break;

	case 40704:
		_app->soundStopAll(1024);
		_app->playMovie("1788");
		_event->_presentationIndexRO = 0;
		_app->rotationSet3DSoundOff(40000, 40002);
		_app->rotationSet3DSoundOff(40001, 40002);
		_app->rotationSet3DSoundOff(40002, 40002);
		_app->rotationSet3DSoundOff(40003, 40002);
		_app->rotationSet3DSoundOff(40004, 40002);
		_app->rotationSetAmbientSoundOff(40000, 40001);
		_app->rotationSetAmbientSoundOff(40001, 40001);
		_app->rotationSetAmbientSoundOff(40002, 40001);
		_app->rotationSetAmbientSoundOff(40003, 40001);
		_app->rotationSetAmbientSoundOff(40004, 40001);
		_app->rotationSetAmbientSoundOff(40005, 40001);
		_app->rotationSetAmbientSoundOn(40000, 40604);
		_app->rotationSetAmbientSoundOn(40001, 40604);
		_app->rotationSetAmbientSoundOn(40002, 40604);
		_app->rotationSetAmbientSoundOn(40003, 40604);
		_app->rotationSetAmbientSoundOn(40004, 40604);
		_app->rotationSetAmbientSoundOn(40005, 40604);
		_app->puzzleSetAmbientSoundOn(kPuzzle40060, 40604);
		_app->objectPresentationShow(kObject40203);
		_app->rotationSetMovabilityOff(40000, 1, 1);
		_app->rotationSetMovabilityOn(40004, 0, 0);
		_app->rotationSetActive(40000);
		_app->rotationSetMovabilityRideName(40000, 0, "ro0102");
		_app->rotationSetMovabilityRideName(40001, 2, "ro0201");
		_app->rotationSetMovabilityRideName(40001, 3, "ro0203");
		_app->rotationSetMovabilityRideName(40002, 0, "ro0302");
		_app->rotationSetMovabilityRideName(40003, 0, "ro0402");
		_app->rotationSetMovabilityRideName(40004, 0, "1796");
		_app->rotationSetMovabilityRideName(40004, 1, "ro0502");
		break;

	case 40706:
		_app->puzzleSetActive(kPuzzle40102);
		_app->soundPlay(40707);
		break;

	case 40707:
		_app->puzzleSetActive(kPuzzle40101);
		_app->soundPlay(40708);
		break;

	case 40708:
		_app->puzzleSetActive(kPuzzle40104);
		_app->soundPlay(40709);
		break;

	case 40709:
		_app->puzzleSetActive(kPuzzle40102);
		_app->soundPlay(40710);
		break;

	case 40710:
		_app->puzzleSetActive(kPuzzle40101);
		_app->soundPlay(40711);
		break;

	case 40711:
		_app->puzzleSetActive(kPuzzle40103);
		_app->soundPlay(40702);
		break;
	}
}

void EventSoundRing::onSoundZoneWA(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 50021:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1875", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1873", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1874", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51002);
		_app->soundPlay(50022);
		break;

	case 50022:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1878", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1876", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1877", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51003);
		_app->soundPlay(51008, kSoundLoop);
		_app->soundPlay(50026);
		break;

	case 50026:
		_app->puzzleSetActive(kPuzzle51004);
		_app->soundPlay(50027);
		break;

	case 50027:
		_app->puzzleSetActive(kPuzzle51005);
		_app->soundPlay(50028);
		break;

	case 50028:
		_app->puzzleSetActive(kPuzzle51006);
		_app->soundPlay(50029);
		break;

	case 50029:
		_app->puzzleSetActive(kPuzzle51007);
		_app->soundPlay(50030);
		break;

	case 50030:
		_app->puzzleSetActive(kPuzzle51008);
		_app->soundPlay(50031);
		break;

	case 50031:
		_app->puzzleSetActive(kPuzzle51009);
		_app->soundPlay(50032);
		break;

	case 50032:
		_app->puzzleSetActive(kPuzzle51010);
		_app->soundPlay(50033);
		break;

	case 50033:
		_app->puzzleSetActive(kPuzzle51011);
		_app->soundPlay(50034);
		break;

	case 50034:
		_app->puzzleSetActive(kPuzzle51012);
		_app->soundPlay(50035);
		break;

	case 50035:
		_app->puzzleSetActive(kPuzzle51013);
		_app->soundPlay(50036);
		break;

	case 50036:
		_app->playMovie("1879");
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_event->onSwitchZone(kZoneAS, 4);
		break;

	case 50001:
		_app->puzzleSetActive(kPuzzle50001);
		_app->soundPlay(50002);

		break;
	case 50002:
		_app->rotationSetActive(50001);
		_app->soundPlay(50003);
		break;

	case 50003:
		_app->soundPlay(51006, kSoundLoop);
		break;

	case 50009:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50010);
		break;

	case 50010:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50011);
		break;

	case 50011:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50012);
		break;

	case 50012:
		_app->puzzleSetActive(kPuzzle50701);
		_app->soundPlay(50013);
		break;

	case 50013:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50014);
		break;

	case 50014:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50015);
		break;

	case 50015:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50016);
		break;

	case 50016:
		_app->objectSetAccessibilityOff(kObject50700);
		_app->rotationSetActive(50701);
		break;
	}
}

void EventSoundRing::onSoundZoneAS(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 80100:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80101);
		break;

	case 80101:
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80102);
		break;

	case 80102:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80103);
		break;

	case 80103:
		_app->playMovie("1157");
		_app->puzzleSetActive(kPuzzle80013);
		_app->soundPlay(80104);
		break;

	case 80104:
	case 80040:
	case 80041:
	case 80042:
	case 80043:
	case 80044:
	case 80045:
	case 80046:
	case 80047:
	case 80049:
	case 80050:
	case 80051:
	case 80052:
	case 80053:
	case 80054:
	case 80055:
	case 80056:
	case 80058:
	case 80059:
	case 80060:
	case 80061:
	case 80062:
	case 80063:
	case 80064:
	case 80065:
	case 80066:
	case 80068:
	case 80069:
	case 80070:
	case 80071:
	case 80072:
	case 80073:
	case 80074:
	case 80075:
	case 80076:
	case 80077:
	case 80078:
	case 80079:
		_app->soundPlay(id + 1);
		break;

	case 80105:
		_app->playMovie("1158");
		_app->puzzleSetActive(kPuzzle80014);
		_app->soundPlay(80106);
		break;

	case 80106:
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		_app->rotationSetAlp(80001, 270.0f);
		_app->rotationSetBet(80001, -26.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		break;

	case 80048:
		_app->soundSetVolume(80201, 100);
		_app->varSetByte(80002, 2);
		break;

	case 80057:
		_app->soundSetVolume(80203, 100);
		_app->varSetByte(80002, 3);
		break;

	case 80067:
		_app->soundSetVolume(80204, 100);
		_app->varSetByte(80002, 4);
		break;

	case 80080:
		_app->soundSetVolume(80205, 100);
		_app->varSetByte(80002, 5);
		break;
	case 80107:
		_app->playMovie("1159");
		_app->showMenu(kZoneAS, kMenuAction0);
		break;
	}
}

void EventSoundRing::onSoundZoneN2(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id){
	default:
		break;

	case 70001:
		_app->puzzleSetActive(kPuzzle70306);
		_app->soundPlay(70002);
		break;

	case 70002:
		_app->puzzleSetActive(kPuzzle70303);
		_app->soundPlay(70003);
		break;

	case 70003:
		_app->playMovie("1504");
		_app->playMovie("1505");
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->varSetFloat(90006, 50.0);
		_event->onSwitchZone(kZoneRO, 0);
		break;

	case 71002:
	case 71003:
		_app->playMovie(Common::String::format("N2_%dA", _app->varGetByte(70014)));
		_app->objectPresentationShow(kObject70001, (uint8)_app->varGetByte(70014) - 1);
		break;

	case 72001:
		_app->puzzleSetActive(kPuzzle72002);
		_app->soundPlay(72002);
		break;

	case 72002:
		_app->playMovie("1499");
		_app->puzzleSetActive(kPuzzle72003);
		_app->soundPlay(72003);
		break;

	case 72003:
		_app->puzzleSetActive(kPuzzle72004);
		_app->soundPlay(72004);
		break;

	case 72004:
		_app->puzzleSetActive(kPuzzle72005);
		_app->soundPlay(72005);
		break;

	case 72005:
		_app->playMovie("1500");
		_app->puzzleSetActive(kPuzzle72006);
		_app->soundPlay(72006);
		break;

	case 72006:
		_app->puzzleSetActive(kPuzzle72007);
		_app->soundPlay(72007);
		break;

	case 72007:
		_app->puzzleSetActive(kPuzzle72008);
		_app->soundPlay(72008);
		break;

	case 72008:
		_app->puzzleSetActive(kPuzzle72009);
		_app->soundPlay(72009);
		break;

	case 72009:
		_app->playMovie("1501");
		_app->puzzleSetActive(kPuzzle72010);
		_app->soundPlay(72010);
		break;

	case 72010:
		_app->playMovie("1502");
		_app->soundPlay(70000, kSoundLoop);
		_app->varSetByte(70013, 0);
		_app->puzzleSetActive(kPuzzle70301);
		_app->soundPlay(70056);
		break;

	case 70043:
	case 70044:
	case 70045:
	case 70046:
	case 70047:
	case 70048:
	case 70049:
	case 70050:
	case 70051:
	case 70052:
	case 70053:
	case 70054: {
		int8 index = _app->varGetByte(70013);
		char str = _app->varGetString(70099)[index];
		if (str > '0')
			_app->puzzleSetActive((PuzzleId)(str + 70551));

		_app->soundPlay(id + 1);

		_app->varSetByte(70013, index + 1);
		}
		break;

	case 70055:
		_app->objectSetAccessibilityOff(kObject70700, 0, 0);
		_app->objectSetAccessibilityOn(kObject70700, 1, 1);
		_app->rotationSetActive(70200);
		break;

	case 70017:
	case 70018:
	case 70019:
	case 70020:
	case 70021:
	case 70024:
	case 70025:
	case 70026:
	case 70056:
	case 70057:
	case 70058:
	case 70059:
	case 70060:
	case 70061:
	case 70062:
	case 70063:
	case 70064:
	case 70065: {
		int8 index = _app->varGetByte(70013);
		char str = _app->varGetString(70099)[index];
		_app->puzzleSetActive((PuzzleId)(str + 70251));

		_app->soundPlay(id + 1);

		_app->varSetByte(70013, index + 1);
		}
		break;

	case 70066:
		_app->rotationSetAlp(70300, 160.0f);
		_app->rotationSetActive(70300);
		break;

	case 70022:
		_event->sub_433EE0();
		_app->rotationSetActive(70001);
		_app->playMovie("1503");
		_app->puzzleSetActive(kPuzzle70303);
		_app->varSetByte(70013, 51);
		_app->soundPlay(70024);
		break;

	case 70101:
		_app->puzzleSetActive(kPuzzle70305);
		_app->soundPlay(70023);
		break;

	case 70023:
		_app->objectSetAccessibilityOff(kObject70108);
		_app->puzzleSetActive(kPuzzle70102);
		break;

	case 70027:
		_app->puzzleSetActive(kPuzzle70000);
		_app->soundPlay(71001);
		break;

	case 71001:
		_app->playMovie(Common::String::format("N2_%dA", _app->varGetByte(70014)));
		_app->objectSetAccessibilityOn(kObjectCentaur, 1, 1);
		_app->objectPresentationShow(kObject70001, (uint8)_app->varGetByte(70014) - 1);
		break;

	case 71010:
	case 71011:
	case 71012:
		_app->objectPresentationHide(kObject70001);
		_app->playMovie(Common::String::format("N2_%d%c", _app->varGetByte(70014), _app->varGetByte(70015) + 67));
		_app->soundPlay(_app->varGetByte(70015) + 10 * (_app->varGetByte(70014) + 7109));
		break;

	case 71100:
	case 71101:
	case 71102:
	case 71103:
	case 71104:
	case 71105:
	case 71106:
	case 71107:
	case 71108:
	case 71109:
	case 71110:
	case 71111:
	case 71112:
	case 71113:
	case 71114:
	case 71115:
	case 71116:
	case 71117:
	case 71118:
	case 71119:
	case 71120:
	case 71121:
	case 71122:
	case 71123:
	case 71124:
	case 71125:
	case 71126:
	case 71127:
	case 71128:
	case 71129:
		if (_app->varGetByte(70014) >= 3) {
			if (_app->varGetByte(70015) != 2) {
				_app->exitToMenu(kMenuAction2);
				break;
			}

			_app->puzzleSetActive(kPuzzle70303);
			_app->soundPlay(70001);
		} else {
			if (_app->varGetByte(70014) == 2 && !_app->varGetByte(70015)) {
				_app->exitToMenu(kMenuAction2);
				break;
			}

			_app->soundPlay(_app->varGetByte(70014) + 71001);
			_app->varSetByte(70014, _app->varGetByte(70014) + 1);

			_app->objectSetAccessibilityOn(kObjectCentaur);
			_app->objectSetAccessibilityOn(kObjectDragon);
			_app->objectSetAccessibilityOn(kObjectPhoenix1);
		}
		break;
	}
}

} // End of namespace Ring
