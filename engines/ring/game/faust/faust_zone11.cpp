/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 11
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 011110-1301, USA.
 */

#include "ring/game/faust/faust_zone11.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone11Faust::Zone11Faust(ApplicationFaust *application) : _app(application) {
}

Zone11Faust::~Zone11Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone11Faust::onInit() {
	_app->setCurrentZone(kZone11);

	_app->rotationAdd(72014, "1555", 0, 6);
	_app->objectAdd(kObject72000, "", "", 1);
	_app->objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(174,  92,   230, 158), true, kCursorShoot, 0);
	_app->objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(567,  134,  638, 204), true, kCursorShoot, 1);
	_app->objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(1355, 129, 1423, 204), true, kCursorShoot, 2);
	_app->objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(2341, 150, 2408, 230), true, kCursorShoot, 3);
	_app->objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(2408, 68,  2459, 115), true, kCursorShoot, 4);
	_app->objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(3014, 132, 3090, 212), true, kCursorShoot, 5);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToRotation(kObject72000, 0, 72014, 0, 31, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject72000, 0, 0, 72000);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToRotation(kObject72000, 1, 72014, 1, 31, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject72000, 1, 0, 72001);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToRotation(kObject72000, 2, 72014, 2, 31, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject72000, 2, 0, 72002);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToRotation(kObject72000, 3, 72014, 3, 31, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject72000, 3, 0, 72003);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToRotation(kObject72000, 4, 72014, 4, 31, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject72000, 4, 0, 72004);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToRotation(kObject72000, 5, 72014, 5, 31, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject72000, 5, 0, 72005);
	_app->objectAdd(kObject72001, "", "", 1);
	_app->objectAddRotationAccessibility(kObject72001, 72014, Common::Rect(0, -600, 3600, 600), true, kCursorShoot, 0);
	_app->soundAdd(72000, kSoundTypeEffect, "1556.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(72001, kSoundTypeEffect, "1557.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(72002, kSoundTypeEffect, "1558.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(72003, kSoundTypeEffect, "1559.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(72004, kSoundTypeEffect, "1560.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(72005, kSoundTypeEffect, "1561.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(72012, kSoundTypeEffect, "1562.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(72006, kSoundTypeAmbientEffect, "1563.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72006, 1, 1, 25, 88, 335.0f, 10);
	_app->rotationSet3DSoundOff(72014, 72006);
	_app->soundAdd(72007, kSoundTypeAmbientEffect, "1563.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72007, 1, 1, 25, 90, 15.0f, 10);
	_app->rotationSet3DSoundOff(72014, 72007);
	_app->soundAdd(72008, kSoundTypeAmbientEffect, "1563.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72008, 1, 1, 25, 92, 96.0f, 10);
	_app->rotationSet3DSoundOff(72014, 72008);
	_app->soundAdd(72009, kSoundTypeAmbientEffect, "1563.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72009, 1, 1, 25, 95, 199.0f, 10);
	_app->rotationSet3DSoundOff(72014, 72009);
	_app->soundAdd(72010, kSoundTypeAmbientEffect, "1563.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72010, 1, 1, 25, 83, 195.0f, 10);
	_app->rotationSet3DSoundOff(72014, 72010);
	_app->soundAdd(72011, kSoundTypeAmbientEffect, "1563.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72011, 1, 1, 25, 93, 260.0f, 10);
	_app->rotationSet3DSoundOff(72014, 72011);
	_app->soundAdd(72014, kSoundTypeAmbientEffect, "1564.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72014, 1, 1, 15, 100, 0.0f, 10);
	_app->soundAdd(72015, kSoundTypeAmbientEffect, "1565.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72015, 1, 1, 15, 100, 90.0f, 10);
	_app->soundAdd(72016, kSoundTypeAmbientEffect, "1566.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(72014, 72016, 1, 1, 15, 100, 270.0f, 10);
	_app->soundAdd(72013, kSoundTypeBackgroundMusic, "1567.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(72013, 90);
	_app->varDefineByte(72000, 0);
	_app->varDefineByte(72001, 0);
	_app->varDefineByte(72002, 0);
	_app->varDefineByte(72003, 0);
	_app->varDefineByte(72004, 0);
	_app->varDefineByte(72005, 0);
	_app->varDefineByte(72006, 0);
	_app->varDefineByte(72007, 15);
	_app->varDefineWord(72000, 270);
}

void Zone11Faust::onTimer(TimerId id) {
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

void Zone11Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone11Faust::onButtonUp] Not implemented");
}

void Zone11Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone11] Not implemented");
}

} // End of namespace Ring
