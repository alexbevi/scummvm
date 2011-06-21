/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 7
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 07110-1301, USA.
 */

#include "ring/game/faust/faust_zone7.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone7Faust::Zone7Faust(ApplicationFaust *application) : _app(application) {
}

Zone7Faust::~Zone7Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone7Faust::onInit() {
	_app->setCurrentZone(kZone7);
	_app->puzzleAdd(kPuzzle49000);
	_app->puzzleAddBackgroundImage(kPuzzle49000, "a04s01n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle49001);
	_app->puzzleAddBackgroundImage(kPuzzle49001, "a04s02n01p02.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(41000, "1376", 0, 2);
	_app->rotationAdd(41001, "1377", 0, 2);
	_app->rotationAdd(41002, "1378", 0, 1);
	_app->rotationAddMovabilityToRotation(41000, 41001, "", Common::Rect(3533, -150, 3600, 52), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(41000, 0, 330.0f, 0.3f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(41000, 41001, "", Common::Rect(0, -150, 138, 52), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(41000, 1, 330.0f, 0.3f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->objectAdd(kObject49000, "", "", 1);
	_app->objectAddRotationAccessibility(kObject49000, 41001, Common::Rect(316, 5, 611, 318), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 0, kPuzzle49000, "a04s01n02a01", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 100, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 1, kPuzzle49000, "a04s02n01p01s01", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 70, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 2, kPuzzle49000, "a04s02n01p01s02", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 23, 15.0f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 3, kPuzzle49000, "a04s02n01p01s03", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 17, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 4, kPuzzle49000, "a04s02n01p01s04", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 15, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 5, kPuzzle49000, "a04s02n01p01s05", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 63, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 6, kPuzzle49000, "a04s02n01p01s06", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 58, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 7, kPuzzle49000, "a04s02n01p01s07", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 70, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 8, kPuzzle49000, "a04s02n01p01s08", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 60, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 9, kPuzzle49000, "a04s02n01p01s09", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 70, 12.5f, 6);
	_app->objectAddPresentation(kObject49000);
	_app->objectPresentationAddAnimationToPuzzle(kObject49000, 10, kPuzzle49000, "a04s02n01p01s10", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 60, 12.5f, 6);
	_app->objectAdd(kObject49003, "", "", 1);
	_app->objectAddRotationAccessibility(kObject49003, 41002, Common::Rect(704, 187, 847, 317), true, kCursorZoom, 0);
	_app->objectAddPuzzleAccessibility(kObject49003, kPuzzle49001, Common::Rect(278, 280, 396, 330), true, kCursorZoom, 1);
	_app->objectAddPuzzleAccessibility(kObject49003, kPuzzle49001, Common::Rect(0,   420, 640, 464), true, kCursorBack, 9);
	_app->objectAddPresentation(kObject49003);
	_app->objectPresentationAddImageToPuzzle(kObject49003, 0, kPuzzle49001, "a04s02n01p02l01.tga", Common::Point(0, 16), true, kDrawType3, 1000);
	_app->objectPresentationAddImageToRotation(49003, 0, 41002, 0);
	_app->objectAdd(kObject49002, "", "", 1);
	_app->objectAddPresentation(kObject49002);
	_app->objectPresentationAddAnimationToRotation(kObject49002, 0, 41000, 0, 40, 12.5f, 5);
	_app->objectPresentationAddAnimationToRotation(kObject49002, 0, 41000, 1, 40, 12.5f, 5);
	_app->objectPresentationAddAnimationToRotation(kObject49002, 0, 41001, 0, 40, 12.5f, 5);
	_app->objectPresentationAddAnimationToRotation(kObject49002, 0, 41001, 1, 40, 12.5f, 5);
	_app->objectPresentationShow(kObject49002);
	_app->objectAdd(kObject19, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject19, kPuzzle49000, Common::Rect(0, 0, 640, 480), false, kCursorMenuIdle, 1);
	_app->objectAddPuzzleAccessibility(kObject19, kPuzzleMenu,  Common::Rect(0, 0, 640, 480), false, kCursorMenuIdle, 1);
	_app->varDefineByte(49000, 0);
	_app->varDefineByte(49001, 0);
	_app->varDefineByte(49002, 0);
	_app->varDefineByte(49003, 0);
	_app->varDefineByte(49004, 0);
	_app->soundAdd(41001, kSoundTypeBackgroundMusic, "1379.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(41001, 90);
	_app->soundAdd(41002, kSoundTypeAmbientMusic, "1380.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41003, kSoundTypeAmbientMusic, "1381.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41004, kSoundTypeAmbientMusic, "1382.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41005, kSoundTypeAmbientMusic, "1383.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41006, kSoundTypeAmbientMusic, "1384.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41007, kSoundTypeAmbientMusic, "1385.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41008, kSoundTypeAmbientMusic, "1386.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41009, kSoundTypeAmbientMusic, "1387.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41011, kSoundTypeAmbientMusic, "1388.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41012, kSoundTypeAmbientMusic, "1389.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41013, kSoundTypeAmbientMusic, "1390.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(41000, kSoundTypeDialog, "1391.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
}

void Zone7Faust::onSetup(SetupType type) {
	error("[Zone7Faust::onSetup] Not implemented");
}

void Zone7Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone7Faust::onButtonUp] Not implemented");
}

void Zone7Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone7] Not implemented");
}

} // End of namespace Ring
