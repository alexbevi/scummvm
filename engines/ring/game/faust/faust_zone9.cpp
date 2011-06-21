/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 9
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 09110-1301, USA.
 */

#include "ring/game/faust/faust_zone9.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone9Faust::Zone9Faust(ApplicationFaust *application) : _app(application) {
}

Zone9Faust::~Zone9Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone9Faust::onInit() {
	_app->setCurrentZone(kZone9);

	_app->puzzleAdd(kPuzzle61901);
	_app->puzzleAddBackgroundImage(kPuzzle61901, "a06s01n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle61207);
	_app->puzzleAddBackgroundImage(kPuzzle61207, "a06s03n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle61208);
	_app->puzzleAddBackgroundImage(kPuzzle61208, "a06s03n02p02l01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle61209);
	_app->puzzleAddBackgroundImage(kPuzzle61209, "a06s03n02p02l01.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(61001, "1514", 0, 2);
	_app->rotationAdd(61200, "1515", 0, 0);
	_app->rotationAdd(61207, "1516", 0, 2);
	_app->rotationAddMovabilityToRotation(61001, 61200, "", Common::Rect(2257, 15, 2448, 192), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(61001, 0, 20.0f, 0.3f, 87.0f, 0.0f, 2, 20.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(61001, 61207, "", Common::Rect(3339, 145, 3600, 268), false, kCursorMove, 666);
	_app->rotationSetMovabilityToRotation(61001, 1, 300.0f, 20.0f, 60.0f, 0.0f, 2, 360.0f, 25.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(61200, 61207, "", Common::Rect(615, -5, 743, 176), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(61200, 0, 20.0f, 0.3f, 87.0f, 0.0f, 2, 20.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(61200, kPuzzle61901, "1002", Common::Rect(1506, -101, 1777, 170), false, kCursorMove, 0);
	_app->rotationSetMovabilityToPuzzle(61200, 1, 120.0f, 0.3f, 85.7f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(61200, 61001, "", Common::Rect(1506, -101, 1777, 170), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(61200, 2, 120.0f, 0.3f, 87.0f, 0.0f, 2, 120.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(61207, 61200, "", Common::Rect(2381, -13, 2564, 230), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(61207, 0, 205.0f, 0.3f, 87.0f, 0.0f, 2, 205.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(61207, kPuzzle61207, "", Common::Rect(2754, 345, 2902, 432), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(61207, 1, 0.0f, 0.3f, 85.7f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(61207, kPuzzle61208, "", Common::Rect(333, 308, 416, 359), false, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(61207, 2, 0.0f, 0.3f, 85.7f, 0.0f, 2);
	_app->puzzleAddMovabilityToRotation(kPuzzle61207, 61207, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 99);
	_app->puzzleSetMovabilityToRotation(kPuzzle61207, 0, 245.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle61208, 61207, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 99);
	_app->puzzleSetMovabilityToRotation(kPuzzle61208, 0, 0.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle61208, kPuzzle61209, "", Common::Rect(295, 309, 359, 363), true, kCursorZoom, 0);
	_app->objectAdd(kObjectNecklace, "Necklace", "a06_Necklace", 1);
	_app->objectSetActiveCursor(kObjectNecklace, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectNecklace, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectNecklace, kPuzzle61207, Common::Rect(213, 295, 238, 328), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObjectNecklace, kPuzzle61207, Common::Rect(262, 271, 286, 295), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObjectNecklace, kPuzzle61207, Common::Rect(308, 264, 330, 287), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObjectNecklace, kPuzzle61207, Common::Rect(349, 284, 366, 307), true, kCursorTake, 0);
	_app->objectAddPresentation(kObjectNecklace);
	_app->objectPresentationAddImageToPuzzle(kObjectNecklace, 0, kPuzzle61207, "a06s03n02p01l01.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectNecklace);
	_app->objectPresentationAddImageToRotation(kObjectNecklace, 1, 61207, 0);
	_app->objectPresentationShow(kObjectNecklace, 1);
	_app->objectAdd(kObject61304, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject61304, kPuzzle61209, Common::Rect(0, 420, 640, 464), true, kCursorBack, 9);
	_app->objectAddPresentation(kObject61304);
	_app->objectPresentationAddImageToRotation(kObject61304, 0, 61207, 1);
	_app->objectAddPresentation(kObject61304);
	_app->objectPresentationAddImageToPuzzle(kObject61304, 1, kPuzzle61209, "a06s03n02p02l02.tga", Common::Point(0, 16), true, kDrawType3, 1100);
	_app->objectPresentationShow(kObject61304);
	_app->objectAddPresentation(kObject61304);
	_app->objectPresentationAddImageToRotation(kObject61304, 2, 61001, 0);
	_app->objectPresentationAddImageToRotation(kObject61304, 2, 61001, 1);
	_app->objectAdd(kObjectWhip2, "Whip", "a06_Whip", 1);
	_app->objectSetActiveCursor(kObjectWhip2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectWhip2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObjectWhip2);
	_app->objectPresentationAddTextToPuzzle(kObjectWhip2, 0, kPuzzle61901, "", Common::Point(20, 16), kFontDefault, Color(255, 255, 255), Color(0, 0, 0));
	_app->objectPresentationAddTextToPuzzle(kObjectWhip2, 0, kPuzzle61901, "", Common::Point(20, 30), kFontDefault, Color(255, 255, 255), Color(0, 0, 0));
	_app->objectPresentationShow(kObjectWhip2, 0);
	_app->objectAddPuzzleAccessibility(kObjectWhip, kPuzzle61901, Common::Rect(0, 0, 640, 480), true, kCursorIdle, 0);
	_app->objectAdd(kObject61901, "", "", 1);
	_app->objectAdd(kObject61302, "", "", 1);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddImageToPuzzle(kObject61302, 0, kPuzzle61901, "a06s01n01p01s01.0001.tga", Common::Point(28, 182), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddImageToPuzzle(kObject61302, 1, kPuzzle61901, "a06s01n01p01s03.0001.tga", Common::Point(153, 176), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddImageToPuzzle(kObject61302, 2, kPuzzle61901, "a06s01n01p01s05.0001.tga", Common::Point(288, 169), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddImageToPuzzle(kObject61302, 3, kPuzzle61901, "a06s01n01p01s07.0001.tga", Common::Point(396, 132), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddImageToPuzzle(kObject61302, 4, kPuzzle61901, "a06s01n01p01s08.0001.tga", Common::Point(385, 126), true, kDrawType3, 1000);
	_app->objectPresentationShow(kObject61302, 0);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 5, kPuzzle61901, "a06s01n01p01s01", kImageTypeTGA, Common::Point(28, 182), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 6, kPuzzle61901, "a06s01n01p01s02", kImageTypeTGA, Common::Point(12, 173), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 7, kPuzzle61901, "a06s01n01p01s03", kImageTypeTGA, Common::Point(153, 176), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 8, kPuzzle61901, "a06s01n01p01s04", kImageTypeTGA, Common::Point(142, 145), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 9, kPuzzle61901, "a06s01n01p01s05", kImageTypeTGA, Common::Point(288, 169), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 10, kPuzzle61901, "a06s01n01p01s06", kImageTypeTGA, Common::Point(278, 169), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 11, kPuzzle61901, "a06s01n01p01s07", kImageTypeTGA, Common::Point(396, 132), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61302);
	_app->objectPresentationAddAnimationToPuzzle(kObject61302, 12, kPuzzle61901, "a06s01n01p01s08", kImageTypeTGA, Common::Point(385, 126), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAdd(kObject61303, "", "", 1);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddImageToPuzzle(kObject61303, 0, kPuzzle61901, "a06s01n01p01s09.0001.tga", Common::Point(50, 166), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddImageToPuzzle(kObject61303, 1, kPuzzle61901, "a06s01n01p01s11.0001.tga", Common::Point(190, 149), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddImageToPuzzle(kObject61303, 2, kPuzzle61901, "a06s01n01p01s13.0001.tga", Common::Point(303, 173), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddImageToPuzzle(kObject61303, 3, kPuzzle61901, "a06s01n01p01s15.0001.tga", Common::Point(413, 160), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddImageToPuzzle(kObject61303, 4, kPuzzle61901, "a06s01n01p01s16.0001.tga", Common::Point(411, 137), true, kDrawType3, 1000);
	_app->objectPresentationShow(kObject61303, 0);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 5, kPuzzle61901, "a06s01n01p01s09", kImageTypeTGA, Common::Point(50, 166), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 6, kPuzzle61901, "a06s01n01p01s10", kImageTypeTGA, Common::Point(55, 174), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 7, kPuzzle61901, "a06s01n01p01s11", kImageTypeTGA, Common::Point(190, 149), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 8, kPuzzle61901, "a06s01n01p01s12", kImageTypeTGA, Common::Point(184, 145), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 9, kPuzzle61901, "a06s01n01p01s13", kImageTypeTGA, Common::Point(303, 173), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 10, kPuzzle61901, "a06s01n01p01s14", kImageTypeTGA, Common::Point(293, 164), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 11, kPuzzle61901, "a06s01n01p01s15", kImageTypeTGA, Common::Point(413, 160), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAddPresentation(kObject61303);
	_app->objectPresentationAddAnimationToPuzzle(kObject61303, 12, kPuzzle61901, "a06s01n01p01s16", kImageTypeTGA, Common::Point(411, 137), kDrawType3, 1000, 13, 12.5f, 6);
	_app->objectAdd(kObject61209, "", "", 1);
	_app->objectAddRotationAccessibility(kObject61209, 61200, Common::Rect(2334, -38, 2636, 186), true, kCursorMove, 0);
	_app->soundAdd(61303, kSoundTypeAmbientEffect, "1517.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(61304, kSoundTypeAmbientEffect, "1518.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(61305, kSoundTypeAmbientEffect, "1519.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(61306, kSoundTypeAmbientEffect, "1520.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(61307, kSoundTypeAmbientEffect, "1521.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(61300, kSoundTypeDialog, "1338.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(61301, kSoundTypeEffect, "1522.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(61301, 95);
	_app->soundAdd(61302, kSoundTypeEffect, "1522.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(61302, 95);
	_app->soundAdd(61308, kSoundTypeAmbientEffect, "1523.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(61308, 90);
	_app->varDefineByte(61900, 0);
	_app->varDefineByte(61901, 0);
	_app->varDefineByte(61902, 0);
	_app->varDefineByte(61909, 0);
	_app->varDefineByte(61910, 0);
	_app->varDefineByte(61903, 1);
	_app->varDefineByte(61904, 1);
}

void Zone9Faust::onTimer(TimerId id) {
	error("[EventTimerFaust::onTimerZone9] Not implemented");
}

void Zone9Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone9Faust::onButtonUp] Not implemented");
}

void Zone9Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 61300:
		if (frame == frameCount) {
			_app->objectPresentationShow((ObjectId)(kObject61302 + _app->varGetByte(61909)), _app->varGetByte(61901 + _app->varGetByte(61909)));
			_app->objectSetAccessibilityOn(kObjectWhip);

			if (_app->varGetByte(61901) == 2 && _app->varGetByte(61902) == 2)
				_app->timerStart(kTimer2, 1000);
		}
		break;

	case 61301:
		if (frame == frameCount) {
			_app->objectPresentationShow(kObject61303, _app->varGetByte(61902));
			_app->objectSetAccessibilityOn(kObjectWhip);
		}
		break;
	}
}

} // End of namespace Ring
