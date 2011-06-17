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

#include "ring/game/ring/ring_zonero.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneRORing::ZoneRORing(ApplicationRing *application) : _app(application) {
	_presentationIndexRO = 0;

	_presentationIndexBagRO = 0;
	_presentationIndexBagRO2 = 0;

	_dword_4A1C00 = 0;
}

ZoneRORing::~ZoneRORing() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneRORing::onInit() {
	debugC(1, kRingDebugLogic, "Initializing Loge zone (RO)...");

	_app->setZoneAndEnableBag(kZoneRO);

	_app->puzzleAdd(kPuzzle40010);
	_app->puzzleAddBackgroundImage(kPuzzle40010, "ROS00N01P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40011);
	_app->puzzleAddBackgroundImage(kPuzzle40011, "ROS00N01P01L02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40013);
	_app->puzzleAddBackgroundImage(kPuzzle40013, "ROS00N01P01L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40012);
	_app->puzzleAddBackgroundImage(kPuzzle40012, "ROS00N01P01L03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40060);
	_app->puzzleAddBackgroundImage(kPuzzle40060, "ROS00N06P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40100);
	_app->puzzleAddBackgroundImage(kPuzzle40100, "ROS00N01A01_A.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40101);
	_app->puzzleAddBackgroundImage(kPuzzle40101, "ROS00N01A01_B.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40102);
	_app->puzzleAddBackgroundImage(kPuzzle40102, "ROS00N01A01_C.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40103);
	_app->puzzleAddBackgroundImage(kPuzzle40103, "ROS00N01A01_D.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle40104);
	_app->puzzleAddBackgroundImage(kPuzzle40104, "RO_ERDA.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(40000, "ROS00N01", 0, 1);
	_app->rotationAdd(40001, "ROS00N02", 0, 1);
	_app->rotationAdd(40002, "ROS00N03", 0, 1);
	_app->rotationAdd(40003, "ROS00N04", 0, 1);
	_app->rotationAdd(40004, "ROS00N05", 0, 1);
	_app->rotationAdd(40005, "ROS00N06", 0, 0);
	_app->rotationAddMovabilityToRotation(40000, 40001, "1789", Common::Rect(300, -250, 600, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40000, 0, 0, 0, 85.7f, 0, 0, 0, 0, 85.7f);
	_app->rotationAddMovabilityToPuzzle(40000, kPuzzle40010, "", Common::Rect(3192, -104, 3378, 90), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(40000, 1, 290.0f, 0, 85.0f, 0, 0);
	_app->rotationAddMovabilityToRotation(40001, 40004, "1790", Common::Rect(300, -250, 600, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40001, 0, 0, 0, 85.7f, 0, 0, 0, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40001, 40003, "1791", Common::Rect(1200, -250, 1500, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40001, 1, 90.0f, 0, 85.7f, 0, 0, 90.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40001, 40000, "1792", Common::Rect(2100, -250, 2400, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40001, 2, 180.0f, 0, 85.7f, 0, 0, 180.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40001, 40002, "1793", Common::Rect(3000, -250, 3300, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40001, 3, 270.0f, 0, 85.7f, 0, 0, 270.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40002, 40001, "1794", Common::Rect(1200, -250, 1500, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40002, 0, 90.0f, 0, 85.7f, 0, 0, 90.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40003, 40001, "1795", Common::Rect(3000, -250, 3300, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40003, 0, 270.0f, 0, 85.7f, 0, 0, 270.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40004, 40005, "1796", Common::Rect(300, -250, 600, 300), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40004, 0, 0, 0, 85.7f, 0, 0, 0, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40004, 40001, "1797", Common::Rect(2100, -250, 2400, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40004, 1, 180.0f, 0, 85.7f, 0, 0, 180.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(40005, 40004, "1798", Common::Rect(2100, -250, 2400, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(40005, 0, 180.0f, 0, 85.7f, 0, 0, 180.0f, 0, 85.7f);
	_app->rotationAddMovabilityToPuzzle(40005, kPuzzle40060, "", Common::Rect(246, -90, 715, 376), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(40005, 1, 0, 22.0f, 85.7f, 0, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle40010, 40000, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle40010, 0, 287.0f, 10.0f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle40011, 40000, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle40011, 0, 287.0f, 10.0f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle40012, 40000, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle40012, 0, 287.0f, 10.0f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle40013, 40000, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle40013, 0, 287.0f, 10.0f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle40060, 40005, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle40060, 0, 0, 22.0f, 85.7f);
	_app->objectAdd(kObjectFirePower, "Fire Power", "RO_Fire_", 1);
	_app->objectAddBagAnimation(kObjectFirePower, kImageTypeTGA, kDrawType3, 13, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFirePower, Common::Point(22, 22), 13, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFirePower, Common::Point(22, 22), 1, kCursorTypeAnimated, 1.0f, 4, kLoadFromListIcon);
	_app->objectAdd(kObjectRing, "Ring", "RO_Ring", 1);
	_app->objectAddBagAnimation(kObjectRing, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectRing, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectRing, Common::Point(22, 22), 1, kCursorTypeAnimated, 1.0f, 4, kLoadFromListIcon);
	_app->objectAdd(kObjectCrown, "Crown", "RO_Crown", 1);
	_app->objectAddBagAnimation(kObjectCrown, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectCrown, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectCrown, Common::Point(22, 22), 1, kCursorTypeAnimated, 1.0f, 4, kLoadFromListIcon);
	_app->objectAdd(kObject40010, "The Egg", "RO_Egg", 1);
	_app->objectAddPuzzleAccessibility(kObject40010, kPuzzle40010, Common::Rect(200,  80, 440, 350), true,  kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject40010, kPuzzle40011, Common::Rect(200,  80, 440, 350), false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject40010, kPuzzle40012, Common::Rect(219, 189, 430, 352), true,  kCursorHotspot, 2);
	_app->objectAddPresentation(kObject40010);
	_app->objectPresentationAddAnimationToPuzzle(kObject40010, 0, kPuzzle40012, "ROS00N01P01s03", kImageTypeBMP, Common::Point(217, 192), kDrawType1, 1000, 69, 25.0f, 4);
	_app->objectAddPresentation(kObject40010);
	_app->objectPresentationAddAnimationToPuzzle(kObject40010, 1, kPuzzle40012, "ROS00N01P01s04", kImageTypeBMP, Common::Point(211, 180), kDrawType1, 1000, 69, 25.0f, 4);
	_app->objectAdd(kObject40203, "", "", 1);
	_app->objectAddPresentation(kObject40203);
	_app->objectPresentationAddImageToRotation(kObject40203, 0, 40000, 0);
	_app->objectAddPresentation(kObject40203);
	_app->objectPresentationAddImageToRotation(kObject40203, 1, 40001, 0);
	_app->objectAddPresentation(kObject40203);
	_app->objectPresentationAddImageToRotation(kObject40203, 2, 40002, 0);
	_app->objectAddPresentation(kObject40203);
	_app->objectPresentationAddImageToRotation(kObject40203, 3, 40003, 0);
	_app->objectAddPresentation(kObject40203);
	_app->objectPresentationAddImageToRotation(kObject40203, 4, 40004, 0);
	_app->objectAdd(kObject40101, "", "NI_HandSel", 4);
	_app->objectSetActiveDrawCursor(kObject40101, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject40101, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 99; i++) {
		_app->objectAddPresentation(kObject40101);
		_app->objectPresentationAddImageToPuzzle(kObject40101, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF1.%04d.bmp", i + 1), Common::Point(232, 203), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject40102, "", "NI_HandSel", 4);
	_app->objectSetActiveDrawCursor(kObject40102, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject40102, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 99; i++) {
		_app->objectAddPresentation(kObject40102);
		_app->objectPresentationAddImageToPuzzle(kObject40102, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF2.%04d.bmp", i + 1), Common::Point(265, 204), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject40103, "", "NI_HandSel", 4);
	_app->objectSetActiveDrawCursor(kObject40103, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject40103, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 99; i++) {
		_app->objectAddPresentation(kObject40103);
		_app->objectPresentationAddImageToPuzzle(kObject40103, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF3.%04d.bmp", i + 1), Common::Point(295, 203), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject40104, "", "NI_HandSel", 4);
	_app->objectSetActiveDrawCursor(kObject40104, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject40104, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 99; i++) {
		_app->objectAddPresentation(kObject40104);
		_app->objectPresentationAddImageToPuzzle(kObject40104, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF4.%04d.bmp", i + 1), Common::Point(326, 203), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject40105, "", "NI_HandSel", 4);
	_app->objectSetActiveDrawCursor(kObject40105, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject40105, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 99; i++) {
		_app->objectAddPresentation(kObject40105);
		_app->objectPresentationAddImageToPuzzle(kObject40105, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF5.%04d.bmp", i + 1), Common::Point(356, 204), true, kDrawType1, 1000);
	}

	_app->objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(  0,   0,   0,   0), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(  0,   0,   0,   0), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(298, 202, 328, 245), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(329, 202, 359, 245), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(360, 202, 390, 245), true, kCursorHotspot, 4);
	_app->objectAdd(kObject40011, "", "", 1);
	_app->objectAddPresentation(kObject40011);

	for (uint32 i = 0; i < 7; i++) {
		for (uint32 j = 0; j < 7; j++)
			_app->varDefineByte(40501 + (j + 1) * 10 + i, 99);
	}

	_app->objectAddPuzzleAccessibility(kObject40011, kPuzzle40011, Common::Rect(263, 71, 319, 127), true, kCursorHotspot, 12);
	_app->varSetByte(40513, 0);

	for (uint32 i = 0; i < 5; i++) {
		for (uint32 j = 0; j < 5; j++) {
			int16 x = (int16)(207 + j * 56);
			int16 y = (int16)(127 + 56 * i);

			_app->objectPresentationAddImageToPuzzle(kObject40011,
			                                         i,
			                                         kPuzzle40011,
			                                         Common::String::format("L01T%d%d.bmp", i, j),
			                                         Common::Point(x, y),
			                                         true,
			                                         kDrawType1,
			                                         1000);

			_app->objectAddPuzzleAccessibility(kObject40011,
			                                   kPuzzle40011,
			                                   Common::Rect(x, y, x + 56, y + 56),
			                                   true,
			                                   kCursorHotspot,
			                                   (21 + i * 10 + j));

			_app->varSetByte(40501 + (21 + i * 10 + j), (byte)(10 * (i + 2) + j + 1));
		}

		_app->objectAddPresentation(kObject40011);
	}

	_app->objectPresentationShow(kObject40011, 0);
	_app->objectPresentationShow(kObject40011, 1);
	_app->objectPresentationShow(kObject40011, 2);
	_app->objectPresentationShow(kObject40011, 3);
	_app->objectAdd(kObject40060, "", "NI_HandSel", 4);
	_app->objectSetActiveDrawCursor(kObject40060, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject40060, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 73; i++) {
		_app->objectAddPresentation(kObject40060);
		_app->objectPresentationAddImageToPuzzle(kObject40060, i, kPuzzle40060, Common::String::format("ROS00N06P01S02.%04d.bmp", i + 1), Common::Point(67, 34), true, kDrawType1, 1000);
	}

	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(531, 112, 576, 135), true,  kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(466, 110, 511, 135), false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(404, 110, 449, 135), false, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(343, 110, 387, 135), false, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(282, 110, 325, 135), false, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(218, 110, 263, 135), false, kCursorHotspot, 5);
	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(157, 110, 201, 135), false, kCursorHotspot, 6);
	_app->objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect( 93, 110, 140, 135), false, kCursorHotspot, 7);
	_app->objectAdd(kObject40201, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(461,  63, 488, 103), true,  kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(409,  63, 436, 103), true,  kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(357,  63, 383, 103), true,  kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(307,  63, 331, 103), true,  kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(253,  63, 280, 103), true,  kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(200,  63, 228, 103), true,  kCursorHotspot, 5);
	_app->objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(150,  63, 176, 103), true,  kCursorHotspot, 6);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 0, kPuzzle40060, "ROS00N06P01S03", kImageTypeBMP, Common::Point(457, 17), kDrawType1, 1000, 70, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 0, 0, 40100);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 1, kPuzzle40060, "ROS00N06P01S04", kImageTypeBMP, Common::Point(412, 19), kDrawType1, 1000, 70, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 1, 0, 40101);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 2, kPuzzle40060, "ROS00N06P01S05", kImageTypeBMP, Common::Point(359, 20), kDrawType1, 1000, 70, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 2, 0, 40102);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 3, kPuzzle40060, "ROS00N06P01S06", kImageTypeBMP, Common::Point(298, 23), kDrawType1, 1000, 70, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 3, 0, 40103);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 4, kPuzzle40060, "ROS00N06P01S07", kImageTypeBMP, Common::Point(239, 20), kDrawType1, 1000, 70, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 4, 0, 40104);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 5, kPuzzle40060, "ROS00N06P01S08", kImageTypeBMP, Common::Point(173, 22), kDrawType1, 1000, 70, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 5, 0, 40105);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 6, kPuzzle40060, "ROS00N06P01S09", kImageTypeBMP, Common::Point(109, 22), kDrawType1, 1000, 70, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 6, 0, 40106);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 7, kPuzzle40060, "ROS00N06P02S01", kImageTypeBMP, Common::Point(454, 61), kDrawType1, 1000, 26, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 7, 0, 40201);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 8, kPuzzle40060, "ROS00N06P02S02", kImageTypeBMP, Common::Point(400, 62), kDrawType1, 1000, 26, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 8, 0, 40202);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 9, kPuzzle40060, "ROS00N06P02S03", kImageTypeBMP, Common::Point(349, 61), kDrawType1, 1000, 26, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 9, 0, 40203);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 10, kPuzzle40060, "ROS00N06P02S04", kImageTypeBMP, Common::Point(297, 63), kDrawType1, 1000, 26, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 10, 0, 40204);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 11, kPuzzle40060, "ROS00N06P02S05", kImageTypeBMP, Common::Point(247, 62), kDrawType1, 1000, 26, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 11, 0, 40205);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 12, kPuzzle40060, "ROS00N06P02S06", kImageTypeBMP, Common::Point(195, 63), kDrawType1, 1000, 26, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 12, 0, 40206);
	_app->objectAddPresentation(kObject40201);
	_app->objectPresentationAddAnimationToPuzzle(kObject40201, 13, kPuzzle40060, "ROS00N06P02S07", kImageTypeBMP, Common::Point(142, 62), kDrawType1, 1000, 26, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject40201, 13, 0, 40207);
	_app->objectAdd(kObject40202, "", "", 3);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 0, kPuzzle40060, "ROS00N06P01L01.0001.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 1, kPuzzle40060, "ROS00N06P01L01.0002.bmp", Common::Point(57, 360), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 2, kPuzzle40060, "ROS00N06P01L01.0003.bmp", Common::Point(100, 353), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 3, kPuzzle40060, "ROS00N06P01L01.0004.bmp", Common::Point(127, 353), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 4, kPuzzle40060, "ROS00N06P01L01.0005.bmp", Common::Point(165, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 5, kPuzzle40060, "ROS00N06P01L01.0006.bmp", Common::Point(197, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 6, kPuzzle40060, "ROS00N06P01L01.0007.bmp", Common::Point(234, 352), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 7, kPuzzle40060, "ROS00N06P01L01.0008.bmp", Common::Point(260, 342), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 8, kPuzzle40060, "ROS00N06P01L01.0009.bmp", Common::Point(299, 331), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 9, kPuzzle40060, "ROS00N06P01L01.0010.bmp", Common::Point(330, 349), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 10, kPuzzle40060, "ROS00N06P01L01.0011.bmp", Common::Point(364, 335), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 11, kPuzzle40060, "ROS00N06P01L01.0012.bmp", Common::Point(393, 354), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 12, kPuzzle40060, "ROS00N06P01L01.0013.bmp", Common::Point(421, 353), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 13, kPuzzle40060, "ROS00N06P01L01.0014.bmp", Common::Point(454, 358), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 14, kPuzzle40060, "ROS00N06P01L01.0015.bmp", Common::Point(484, 345), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject40202);
	_app->objectPresentationAddImageToPuzzle(kObject40202, 15, kPuzzle40060, "ROS00N06P01L01.0016.bmp", Common::Point(516, 349), true, kDrawType1, 1000);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect( 67, 384, 100, 438), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(101, 384, 133, 438), false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(136, 384, 164, 438), false, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(169, 384, 200, 438), false, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(203, 384, 234, 438), false, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(236, 384, 267, 438), false, kCursorHotspot, 5);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(269, 384, 299, 438), false, kCursorHotspot, 6);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(303, 384, 335, 438), false, kCursorHotspot, 7);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(336, 384, 368, 438), false, kCursorHotspot, 8);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(368, 384, 400, 438), false, kCursorHotspot, 9);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(402, 384, 436, 438), false, kCursorHotspot, 10);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(435, 384, 466, 438), false, kCursorHotspot, 11);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(466, 384, 499, 438), false, kCursorHotspot, 12);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(503, 384, 534, 438), false, kCursorHotspot, 13);
	_app->objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(535, 384, 571, 438), false, kCursorHotspot, 14);
	_app->objectAdd(kObject40300, "", "", 1);
	_app->objectAddPresentation(kObject40300);
	_app->objectPresentationAddAnimationToPuzzle(kObject40300, 0, kPuzzle40101, "ROS00N01A01_B", kImageTypeBMP, Common::Point(301, 217), kDrawType1, 1000, 19, 12.5f, 4);
	_app->soundAdd(40001, kSoundTypeAmbientMusic, "1799.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(40604, kSoundTypeAmbientMusic, "1800.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->rotationAddAmbientSound(40000, 40001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40001, 40001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40002, 40001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40003, 40001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40004, 40001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40005, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40010, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40011, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40012, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40013, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40100, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40101, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40102, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40103, 40001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40104, 40001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40000, 40604, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40001, 40604, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40002, 40604, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40003, 40604, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40004, 40604, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(40005, 40604, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle40060, 40604, 100, 0, 1, 1, 10);
	_app->rotationSetAmbientSoundOff(40000, 40604);
	_app->rotationSetAmbientSoundOff(40001, 40604);
	_app->rotationSetAmbientSoundOff(40002, 40604);
	_app->rotationSetAmbientSoundOff(40003, 40604);
	_app->rotationSetAmbientSoundOff(40004, 40604);
	_app->rotationSetAmbientSoundOff(40005, 40604);
	_app->puzzleSetAmbientSoundOff(kPuzzle40060, 40604);
	_app->soundAdd(40002, kSoundTypeAmbientEffect, "1802.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(40003, kSoundTypeAmbientEffect, "1803.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(40102, kSoundTypeEffect, "1318.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundSetVolume(40102, 80);
	_app->soundAdd(40103, kSoundTypeEffect, "1804.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundSetVolume(40103, 80);
	_app->soundAdd(40500, kSoundTypeEffect, "1805.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40501, kSoundTypeEffect, "1806.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40502, kSoundTypeEffect, "1807.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40503, kSoundTypeEffect, "1808.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40504, kSoundTypeEffect, "1809.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40505, kSoundTypeEffect, "1810.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40506, kSoundTypeEffect, "1811.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40507, kSoundTypeEffect, "1805.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40508, kSoundTypeEffect, "1806.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40509, kSoundTypeEffect, "1807.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40510, kSoundTypeEffect, "1811.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40511, kSoundTypeEffect, "1812.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40512, kSoundTypeEffect, "1810.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40513, kSoundTypeEffect, "1809.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40514, kSoundTypeEffect, "1808.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40600, kSoundTypeEffect, "1813.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40601, kSoundTypeEffect, "1814.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40602, kSoundTypeEffect, "1815.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40603, kSoundTypeEffect, "1816.was", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(40400, kSoundTypeDialog, "1817.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40200, kSoundTypeDialog, "1818.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40201, kSoundTypeDialog, "1819.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40202, kSoundTypeDialog, "1820.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40203, kSoundTypeDialog, "1821.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40204, kSoundTypeDialog, "1822.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40205, kSoundTypeDialog, "1823.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40206, kSoundTypeDialog, "1824.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40300, kSoundTypeDialog, "1825.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40301, kSoundTypeDialog, "1826.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40302, kSoundTypeDialog, "1827.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40303, kSoundTypeDialog, "1828.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40304, kSoundTypeDialog, "1829.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40305, kSoundTypeDialog, "1830.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40306, kSoundTypeDialog, "1831.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40700, kSoundTypeDialog, "1832.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40701, kSoundTypeDialog, "1833.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40706, kSoundTypeDialog, "1834.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40707, kSoundTypeDialog, "1835.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40708, kSoundTypeDialog, "1836.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40709, kSoundTypeDialog, "1837.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40710, kSoundTypeDialog, "1838.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40711, kSoundTypeDialog, "1839.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40702, kSoundTypeDialog, "1840.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40703, kSoundTypeDialog, "1841.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40704, kSoundTypeDialog, "1842.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(40705, kSoundTypeDialog, "1843.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->rotationAdd3DSound(40000, 40002, 1, 1, 10, 87, 290.0f, 10);
	_app->rotationAdd3DSound(40001, 40002, 1, 1, 10, 85, 194.0f, 5);
	_app->rotationAdd3DSound(40002, 40002, 1, 1, 10, 83, 141.0f, 5);
	_app->rotationAdd3DSound(40003, 40002, 1, 1, 10, 83, 233.0f, 5);
	_app->rotationAdd3DSound(40004, 40002, 1, 1, 10, 80, 189.0f, 2);
	_app->rotationAdd3DSound(40005, 40002, 1, 1, 10, 77, 187.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40010, 40002, 1, 1, 10, 93, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40011, 40002, 1, 1, 10, 95, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40012, 40002, 1, 1, 10, 100, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40013, 40002, 1, 1, 10, 97, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40100, 40002, 1, 1, 10, 90, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40101, 40002, 1, 1, 10, 90, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40102, 40002, 1, 1, 10, 90, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40103, 40002, 1, 1, 10, 90, 290.0f, 2);
	_app->puzzleAdd3DSound(kPuzzle40104, 40002, 1, 1, 10, 90, 290.0f, 2);
	_app->rotationAdd3DSound(40000, 40003, 1, 1, 10, 70, 0.0f, 2);
	_app->rotationAdd3DSound(40001, 40003, 1, 1, 10, 75, 0.0f, 5);
	_app->rotationAdd3DSound(40002, 40003, 1, 1, 10, 72, 25.0f, 5);
	_app->rotationAdd3DSound(40003, 40003, 1, 1, 10, 72, 333.0f, 5);
	_app->rotationAdd3DSound(40004, 40003, 1, 1, 10, 76, 0.0f, 10);
	_app->rotationAdd3DSound(40005, 40003, 1, 1, 10, 82, 0.0f, 10);
	_app->varDefineByte(40000, 0);
	_app->varDefineByte(40601, 0);
	_app->varDefineByte(40602, 0);
	_app->varDefineByte(40603, 0);
	_app->varDefineByte(40604, 0);
	_app->varDefineByte(40605, 0);
	_app->varDefineByte(40701, 0);
	_app->varDefineByte(40702, 0);
	_app->varDefineByte(40703, 0);
	_app->varDefineByte(40801, 0);
	_app->varDefineByte(40200, 1);
	_app->varDefineByte(40201, 0);
	_app->varDefineByte(40202, 1);
	_app->varDefineByte(40203, 0);
	_app->varDefineByte(40204, 0);
	_app->varDefineByte(40205, 1);
	_app->varDefineByte(40206, 0);
	_app->varDefineByte(40802, 0);
	_app->varDefineByte(40803, 0);
	_app->varDefineByte(40804, 0);
	_app->varDefineByte(40805, 56);
	_app->varDefineByte(40806, 0);
	_app->varDefineByte(40807, 0);
	_app->varDefineByte(40901, 26);
	_app->varDefineByte(40902, 26);
	_app->varDefineByte(40903, 26);
	_app->varDefineByte(40904, 26);
	_app->varDefineByte(40905, 26);
	_app->varDefineByte(40906, 26);
	_app->varDefineByte(40907, 26);
	_app->varDefineByte(40911, 70);
	_app->varDefineByte(40912, 70);
	_app->varDefineByte(40913, 70);
	_app->varDefineByte(40914, 70);
	_app->varDefineByte(40915, 70);
	_app->varDefineByte(40916, 70);
	_app->varDefineByte(40917, 70);
	_app->varDefineString(40901, "0000000");
	_app->varDefineString(40902, "00000000");
}

void ZoneRORing::onSetup(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupType10:
		_app->onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case kSetupType999:
		_app->rotationSetAlp(40000, 0.0f);
		_app->rotationSetRan(40000, 85.3f);
		_app->rotationSetActive(40000);
		_app->bagRemove(kObjectFire);
		_app->playMovie("1506");
		_app->bagAdd(kObjectFirePower);
		_app->bagAdd(kObjectRing);
		_app->bagAdd(kObjectCrown);
		_app->puzzleSetActive(kPuzzle40100);
		_app->soundPlay(40700);
		break;
	}
}

void ZoneRORing::onSwitch(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRO, kSetupTypeNone);
}

void ZoneRORing::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetByte(40806, _app->varGetByte(40806) + 1);

		if (_app->varGetByte(40806) == 10) {
			_app->timerStop(kTimer0);
			_app->varSetByte(40601, 10);
		}

		_app->objectPresentationShow(kObject40101, (uint8)_app->varGetByte(40806));
		break;

	case kTimer1:
		_app->varSetByte(40807, _app->varGetByte(40807) + 1);

		if (_app->varGetByte(40807) == 90) {
			_app->timerStop(kTimer1);
			_app->varSetByte(40602, 90);
		}

		_app->objectPresentationShow(kObject40102, (uint8)_app->varGetByte(40807));
		break;
	}
}

void ZoneRORing::onButtonDown(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	if (id != kObject40202)
		return;

	if (_app->bagHasClickedObject()) {
		_app->cursorDelete();
		return;
	}

	for (uint32 i = 1; i < 16; i++)
		_app->objectPresentationHide(kObject40202, i);

	_app->objectPresentationShow(kObject40202, target + 1);

	if (target >= 7) {
		_app->soundPlay(target + 40500);

		Common::String str = _app->varGetString(40902);
		while (str.size() > 7)
			str.deleteLastChar();

		_app->varSetString(40902, Common::String::format("%s%1d", str.c_str(), target - 7));

		if (_app->varGetString(40902) == "01276534"
		 || _app->varGetString(40902) == "01476534"
		 || _app->varGetString(40902) == "01276532"
		 || _app->varGetString(40902) == "01476532") {
			_app->varSetFloat(90006, 100.0f);
			_app->soundPlay(40603);
		}
	} else {
		_app->soundPlay(target + 40500);
	}
}

void ZoneRORing::onButtonUp(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject40010:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				if (_app->varGetByte(40000)) {
					if (!_app->varGetByte(40701)
					 && !_app->varGetByte(40702)
					 && !_app->varGetByte(40703)) {
						_app->puzzleSetActive(kPuzzle40011);
						_app->objectPresentationShow(kObject40011);
						_app->objectSetAccessibilityOn(kObject40011);
					}

					if (_app->varGetByte(40701) == 1
					 && !_app->varGetByte(40702)
					 && !_app->varGetByte(40703))
						_app->puzzleSetActive(kPuzzle40013);

					if (_app->varGetByte(40701) == 1
					 && _app->varGetByte(40702) == 1
					 && !_app->varGetByte(40703))
						_app->puzzleSetActive(kPuzzle40012);

				} else {
					_app->puzzleSetActive(kPuzzle40010);
				}
			}
			break;
		}

		switch (target) {
		default:
			_app->cursorDelete();
			break;

		case 0:
			if (_app->bagGetClickedObject() != kObject40000 || _app->varGetByte(40000)) {
				_app->cursorDelete();
				break;
			}

			_app->playMovie("1780");
			_app->varSetFloat(90006, 51.8f);
			_app->puzzleSetActive(kPuzzle40011);

			handleEvents();

			if (_app->varGetByte(40701)) {
				handleEvents();

				_app->varSetByte(40000, 1);
				_app->cursorDelete();
				break;
			}

			for (uint32 i = 2000; i >= 1; i--) {
				target = (rnd(4) + 1) + 10 * (rnd(4) + 2);

				if (_app->varGetByte(target + 40501)) {
					uint32 presentationIndex = (uint8)_app->varGetByte(target + 40501) / 10 - 2;
					uint32 imageIndex = (uint8)_app->varGetByte(target + 40501) % 10 - 1;
					Common::Point coords = _app->objectPresentationGetImageCoordinatesOnPuzzle(40011, presentationIndex, imageIndex);

					if (!_app->varGetByte(target + 40491)) {
						_app->varSetByte(target + 40491, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y - _app->varGetByte(40805)));
						continue;
					}

					if (!_app->varGetByte(target + 40511)) {
						_app->varSetByte(target + 40511, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y + _app->varGetByte(40805)));
						continue;
					}

					if (!_app->varGetByte(target + 40500)) {
						_app->varSetByte(target + 40500, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x - _app->varGetByte(40805), coords.y));
						continue;
					}

					if (!_app->varGetByte(target + 40502)) {
						_app->varSetByte(target + 40502, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x + _app->varGetByte(40805), coords.y));
						continue;
					}
				}
			}

			handleEvents();

			_app->varSetByte(40000, 1);
			_app->cursorDelete();
			break;

		case 2:
			if (_app->bagGetClickedObject() == kObjectRing) {
				_app->objectPresentationShow(kObject40010, 0);
				_app->bagRemove(kObjectRing);
				_app->varSetByte(40801, 1);
			}

			if (_app->bagGetClickedObject() == kObjectCrown && _app->varGetByte(40801) == 1) {
				_app->objectPresentationHide(kObject40010, 0);
				_app->playMovie("1781");
				_app->varSetFloat(90006, 78.6f);
				_app->bagRemove(kObjectCrown);
				_app->varSetByte(40703, 1);
				_app->puzzleSetActive(kPuzzle40103);
				_app->soundStop(40002, 1024);
				_app->puzzleSetActive(kPuzzle40101);
				_app->soundPlay(40706);
			}

			_app->cursorDelete();
			break;
		}
		break;

	case kObject40011:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(target + 40501)) {
			uint32 presentationIndex = (uint8)_app->varGetByte(target + 40501) / 10 - 2;
			uint32 imageIndex = (uint8)_app->varGetByte(target + 40501) % 10 - 1;
			Common::Point coords = _app->objectPresentationGetImageCoordinatesOnPuzzle(40011, presentationIndex, imageIndex);

			if (_app->varGetByte(target + 40491)) {
				if (!_app->varGetByte(target + 40511)) {
					_app->varSetByte(target + 40511, _app->varGetByte(target + 40501));
					_app->varSetByte(target + 40501, 0);
					_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y + _app->varGetByte(40805)));

					if (target == 12) {
						uint32 counter = 1;
						for (uint32 i = 0; i < (uint8)_app->varGetByte(40522); i++) {
							++counter;

							if (counter >= 5) {
								//counter = 1;

								// Original does some checks on a2, but they seem useless :S

								handleEvents();

								_app->objectPresentationHide(kObject40011);
								_app->objectSetAccessibilityOff(kObject40011);
								_app->objectSetAccessibilityOff(kObject40010, 1, 1);
								_app->objectSetAccessibilityOn(kObject40010, 2, 2);
								_app->varSetByte(40701, 1);
								_app->playMovie("1782");
								_app->varSetFloat(90006, 64.3f);
								_app->puzzleSetActive(kPuzzle40013);
								_app->objectPresentationShow(kObject40101, 0);
								_app->objectPresentationShow(kObject40102, 0);
								_app->objectPresentationShow(kObject40103, 0);
								_app->objectPresentationShow(kObject40104, 0);
								_app->objectPresentationShow(kObject40105, 0);
								_app->timerStart(kTimer0, 50);
								_app->timerStart(kTimer1, 30);
								break;
							}
						}
					}

					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				if (!_app->varGetByte(target + 40500)) {
					_app->varSetByte(target + 40500, _app->varGetByte(target + 40501));
					_app->varSetByte(target + 40501, 0);
					_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x - _app->varGetByte(40805), coords.y));
					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				if (_app->varGetByte(target + 40502)) {
					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				_app->varSetByte(target + 40502, _app->varGetByte(target + 40501));
				_app->varSetByte(target + 40501, 0);
				_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x + _app->varGetByte(40805), coords.y));
				_app->soundSetVolume(40103, rnd(20) + 80);
				_app->soundPlay(40103);
			} else {
				_app->varSetByte(target + 40491, _app->varGetByte(target + 40501));
				_app->varSetByte(target + 40501, 0);
				_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y - _app->varGetByte(40805)));
				_app->soundSetVolume(40103, rnd(20) + 80);
				_app->soundPlay(40103);
			}
		}
		break;

	case kObject40201:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectSetAccessibilityOff(kObject40060);
		_app->puzzleSetMovabilityOff(kPuzzle40060, 0, 0);
		if ((_presentationIndexRO / 10) == (int32)(target + 1)) {
			_app->objectPresentationShow(kObject40201, target);

			Common::String str = _app->varGetString(40901);
			while (str.size() > 6)
				str.deleteLastChar();

			_app->varSetString(40901, Common::String::format("%s%d", str.c_str(), target));
		} else {
			_app->varSetByte(target + 40200, _app->varGetByte(target + 40200) ? 0 : 1);
			_app->objectPresentationShow(kObject40201, target + 7);
			_app->soundSetVolume(40602, rnd(20) + 80);
			_app->soundPlay(40602);
		}
		break;
	}
}

void ZoneRORing::onAnimationNextFrame(Id animationId, const Common::String &, uint32 frame, uint32) {
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

void ZoneRORing::onSound(Id id, SoundType, uint32, bool process) {
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
		_app->onSwitchZone(kZoneAS, 2);
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
		_presentationIndexRO = 0;
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

void ZoneRORing::onBag(ObjectId id, Id target, Id, uint32, DragControl *, byte type) {
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

			_presentationIndexRO = _presentationIndexBagRO;
			float test = 10 * (_presentationIndexBagRO * 0.1000000014901161f);
			float index = test;
			int direction = -1;

			if ((test + 5) < _presentationIndexBagRO) {
				direction = 1;
				index = test + 10;
			}

			if (_presentationIndexBagRO != index) {
				do {
					_presentationIndexRO += direction;

					_app->objectPresentationHide(kObject40060);
					_app->objectPresentationShow(kObject40060, (uint32)_presentationIndexRO);

					handleEvents();
					if (checkEscape())
						break;

				} while (_presentationIndexRO != index);
			}

			_app->objectSetAccessibilityOff(kObject40060);
			_app->objectSetAccessibilityOn(kObject40060, (uint32)_presentationIndexRO / 10, (uint32)_presentationIndexRO / 10);
			_app->varSetByte(40804, (uint8)_presentationIndexRO);
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
			_presentationIndexBagRO = _app->varGetByte(target + 40601);
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(0, 0, 640, 480));
			_app->soundPlay(40102, kSoundLoop);
			// Original sets unused value
			_presentationIndexBagRO2 = 97;
			break;

		case 2: {
			_presentationIndexRO = _presentationIndexBagRO;

			float test = 10 * (_presentationIndexBagRO * 0.1000000014901161f);
			float index = test;
			int direction = -1;

			if ((test + 5) < _presentationIndexBagRO) {
				direction = 1;
				index = test + 10;
			}

			if (_presentationIndexBagRO != index) {
				do {
					_presentationIndexRO += direction;

					_app->objectPresentationHide((ObjectId)(target + 40101));
					_app->objectPresentationShow((ObjectId)(target + 40101), (uint32)_presentationIndexRO);

					handleEvents();
					if (checkEscape())
						break;

				} while (_presentationIndexRO != index);
			}

			_app->soundStop(40102, 1024);
			_app->varSetByte(target + 40601, (int8)_presentationIndexRO);

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

				_app->objectPresentationHide((ObjectId)(target + 40101));
				_app->objectPresentationShow((ObjectId)(target + 40101), (uint32)_presentationIndexBagRO);
				break;
			}

			_app->soundStop(40102, 1024);
			break;
		}
		break;
	}
}

void ZoneRORing::onBeforeRide(Id movabilityFrom, Id, uint32, Id, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityPuzzleToRotation)
		return;

	if (movabilityFrom == 40060 || movabilityFrom == 40005) {
		_app->soundSetVolume(40102, 100);
		_app->soundPlay(40102, kSoundLoop);

		if (_app->varGetByte(40804) > 4) {
			do {
				_app->objectPresentationHide(kObject40060);
				_app->objectPresentationShow(kObject40060, (uint8)_app->varGetByte(40804));

				handleEvents();

				_app->varSetByte(40804, _app->varGetByte(40804) + 1);
			} while (!checkEvents() && _app->varGetByte(40804) > 0);
		}

		_app->varSetByte(40804, 0);
		_app->soundStop(40102, 1024);
		if (_app->varGetByte(40802) == 1)
			_app->playMovie("1785");

		_app->objectSetAccessibilityOff(kObject40060);
		_app->objectSetAccessibilityOn(kObject40060, 0, 0);
		_app->objectSetAccessibilityOn(kObject40201);
		_app->objectSetAccessibilityOff(kObject40202);

		_app->objectPresentationHide(kObject40060);
		_app->objectPresentationHide(kObject40201);
		_app->objectPresentationHide(kObject40202);

		_app->varSetByte(40802, 0);
		_app->varSetString(40902, "00000000");

		_dword_4A1C00 = 0;
	}
}

void ZoneRORing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityTo == 40000 || movabilityFrom == 40010)
			_app->varSetByte(40804, 0);

		if (movabilityTo == 40005 || movabilityFrom == 40060) {
			_app->soundPlay(40003, kSoundLoop);
			_app->soundSetVolume(40003, 88);
		}
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityTo == 40060 || movabilityFrom == 40005)
			_app->soundSetVolume(40003, 82);
		break;
	}
}

} // End of namespace Ring
