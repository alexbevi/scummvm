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

#include "ring/game/faust/faust_zone2.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone2Faust::Zone2Faust(ApplicationFaust *application) : _app(application) {
}

Zone2Faust::~Zone2Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone2Faust::onInit() {
	_app->setCurrentZone(kZone2);
	_app->rotationAdd(11001, "1006", 0, 1);
	_app->rotationAdd(11002, "1007", 0, 2);
	_app->rotationAdd(11003, "1008", 0, 1);
	_app->rotationAdd(11004, "1009", 0, 0);
	_app->rotationAdd(11005, "1010", 0, 1);
	_app->rotationAdd(11006, "1011", 0, 1);
	_app->rotationAddMovabilityToRotation(11001, 11005, "1012", Common::Rect(2111, 111, 2450, 345), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11001, 0, 180.0f, 13.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(11002, 11005, "1013", Common::Rect(3056, -45, 3533, 369), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11002, 0, 286.0f, 0.0f, 87.0f, 0.0f, 2, 286.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(11002, 11003, "1014", Common::Rect(1226, -15, 1501, 289), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11002, 1, 91.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(11002, 11004, "1015", Common::Rect(1047, -15, 1255, 214), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11002, 2, 79.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(11003, 11002, "1016", Common::Rect(3033, -26, 3232, 355), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11003, 0, 272.0f, 0.0f, 87.0f, 0.0f, 2, 272.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(11003, 11004, "1017", Common::Rect(399, -52, 875, 416), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11003, 1, 12.0f, 13.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(11004, 11003, "1018", Common::Rect(2828, -99, 3190, 183), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11004, 0, 248.0f, 0.0f, 87.0f, 0.0f, 2, 248.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(11005, 11001, "1019", Common::Rect(318, -151, 569, 205), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11005, 0, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(11005, 11002, "1020", Common::Rect(1255, -31, 1476, 304), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11005, 1, 92.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(11005, 11006, "1021", Common::Rect(3012, -95, 3331, 256), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11005, 2, 270.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(11006, 11005, "1022", Common::Rect(1128, -71, 1455, 231), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(11006, 0, 85.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.3f, 87.0f);
	_app->objectAdd(kObject11001, "", "", 1);
	_app->objectAddRotationAccessibility(kObject11001, 11001, Common::Rect(103, -146, 856, 336), true, kCursorMove, 0);
	_app->objectAdd(kObject11002, "", "", 1);
	_app->objectAddRotationAccessibility(kObject11002, 11003, Common::Rect(1058, 155, 1745, 514), false, kCursorMove, 0);
	_app->objectAdd(kObject11003, "", "", 1);
	_app->objectAddRotationAccessibility(kObject11003, 11006, Common::Rect(2895, -254, 3343, 317), true, kCursorMove, 0);
	_app->objectAdd(kObject11007, "", "", 1);
	_app->objectAddRotationAccessibility(kObject11007, 11006, Common::Rect(2921, -260, 3332, 480), false, kCursorMove, 0);
	_app->objectAdd(kObject11004, "", "", 1);
	_app->objectAddRotationAccessibility(kObject11004, 11002, Common::Rect(2162, -27, 2341, 244), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject11004, 11002, Common::Rect(2095, -476, 2374, -291), true, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject11004, 11002, Common::Rect(1286, 6, 1416, 156), false, kCursorAction, 2);
	_app->objectAddPresentation(kObject11004);
	_app->objectPresentationAddImageToRotation(11004, 0, 11002, 0);
	_app->objectAddPresentation(kObject11004);
	_app->objectPresentationAddImageToRotation(11004, 1, 11002, 1);
	_app->objectAddPresentation(kObject11004);
	_app->objectPresentationAddImageToRotation(11004, 2, 11001, 0);
	_app->objectAddPresentation(kObject11004);
	_app->objectPresentationAddImageToRotation(11004, 3, 11003, 0);
	_app->objectAddPresentation(kObject11004);
	_app->objectPresentationAddImageToRotation(11004, 4, 11005, 0);
	_app->objectAddPresentation(kObject11004);
	_app->objectPresentationAddImageToRotation(11004, 5, 11006, 0);
	_app->objectAdd(kObject11005, "", "", 1);
	_app->objectAddRotationAccessibility(kObject11005, 11004, Common::Rect(2139, -289, 2346, -113), true, kCursorZoom, 0);
	_app->soundAdd(11105, kSoundTypeEffect, "1024.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11106, kSoundTypeEffect, "1025.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11107, kSoundTypeEffect, "1026.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11108, kSoundTypeEffect, "1027.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11109, kSoundTypeEffect, "1028.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11110, kSoundTypeEffect, "1029.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11111, kSoundTypeEffect, "1030.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11112, kSoundTypeEffect, "1031.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11113, kSoundTypeEffect, "1032.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11114, kSoundTypeEffect, "1033.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(11102, kSoundTypeAmbientEffect, "1034.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(11001, 11102, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11002, 11102, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11003, 11102, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11004, 11102, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11005, 11102, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11006, 11102, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationSet3DSoundOff(11001, 11102);
	_app->rotationSet3DSoundOff(11002, 11102);
	_app->rotationSet3DSoundOff(11003, 11102);
	_app->rotationSet3DSoundOff(11004, 11102);
	_app->rotationSet3DSoundOff(11005, 11102);
	_app->rotationSet3DSoundOff(11006, 11102);
	_app->soundAdd(11103, kSoundTypeAmbientEffect, "1035.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(11001, 11103, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11002, 11103, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11003, 11103, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11004, 11103, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11005, 11103, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11006, 11103, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationSet3DSoundOff(11001, 11103);
	_app->rotationSet3DSoundOff(11002, 11103);
	_app->rotationSet3DSoundOff(11003, 11103);
	_app->rotationSet3DSoundOff(11004, 11103);
	_app->rotationSet3DSoundOff(11005, 11103);
	_app->rotationSet3DSoundOff(11006, 11103);
	_app->soundAdd(11104, kSoundTypeAmbientEffect, "1036.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(11001, 11104, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11002, 11104, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11003, 11104, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11004, 11104, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11005, 11104, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(11006, 11104, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationSet3DSoundOff(11001, 11104);
	_app->rotationSet3DSoundOff(11002, 11104);
	_app->rotationSet3DSoundOff(11003, 11104);
	_app->rotationSet3DSoundOff(11004, 11104);
	_app->rotationSet3DSoundOff(11005, 11104);
	_app->rotationSet3DSoundOff(11006, 11104);
	_app->soundAdd(11001, kSoundTypeDialog, "1037.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(11101, kSoundTypeAmbientMusic, "1023.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAddAmbientSound(11001, 11101, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(11002, 11101, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(11003, 11101, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(11004, 11101, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(11005, 11101, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(11006, 11101, 90, 0, 1, 1, 10);
	_app->varDefineByte(11001, 0);
	_app->varDefineByte(11002, 0);
	_app->varDefineByte(11003, 0);
}

void Zone2Faust::onSetup(SetupType type) {
	error("[Zone2Faust::onSetup] Not implemented");
}

void Zone2Faust::onTimer(TimerId id) {
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

void Zone2Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone2Faust::onButtonUp] Not implemented");
}

void Zone2Faust::onBagClickedObject(ObjectId id) {
	if (id == kObjectHomunculus)
		if (_app->varGetByte(11002) == 1)
			_app->objectSetAccessibilityOn(kObject11004, 2, 2);
}

void Zone2Faust::onBeforeRide(Id, Id, uint32, Id, MovabilityType) {
	_app->objectPresentationHide(kObject25);
}

void Zone2Faust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	error("[Zone2Faust::onAfterRide] Not implemented");
}

} // End of namespace Ring
