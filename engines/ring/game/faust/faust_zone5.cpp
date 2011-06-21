/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 5
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 05110-1301, USA.
 */

#include "ring/game/faust/faust_zone5.h"

#include "ring/base/saveload.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

 using namespace FaustGame;

namespace Ring {

Zone5Faust::Zone5Faust(ApplicationFaust *application) : _app(application) {
}

Zone5Faust::~Zone5Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone5Faust::onInit() {
	_app->setCurrentZone(kZone5);

	_app->puzzleAdd(kPuzzle17021);
	_app->puzzleAddBackgroundImage(kPuzzle17021, "a01s07n02p01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17031);
	_app->puzzleAddBackgroundImage(kPuzzle17031, "a01s07n03p01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17042);
	_app->puzzleAddBackgroundImage(kPuzzle17042, "a01s07n04p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17022);
	_app->puzzleAddBackgroundImage(kPuzzle17022, "a01s07n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17051);
	_app->puzzleAddBackgroundImage(kPuzzle17051, "a01s07n05p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17062);
	_app->puzzleAddBackgroundImage(kPuzzle17062, "a01s07n0xp02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17063);
	_app->puzzleAddBackgroundImage(kPuzzle17063, "a01s07n0xp03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17064);
	_app->puzzleAddBackgroundImage(kPuzzle17064, "a01s07n0xp04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17065);
	_app->puzzleAddBackgroundImage(kPuzzle17065, "a01s07n0xp05.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle17066);
	_app->puzzleAddBackgroundImage(kPuzzle17066, "a01s07n0xp06.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(17001, "1279", 0, 13);
	_app->rotationAdd(17002, "1280", 0, 8);
	_app->rotationAdd(17003, "1281", 0, 8);
	_app->rotationAdd(17004, "1282", 0, 6);
	_app->rotationAdd(17005, "1283", 0, 13);
	_app->rotationSetCompressionBufferLength(17005, 1600000);
	_app->rotationAddMovabilityToRotation(17001, 17002, "", Common::Rect(0, -140, 103, 436), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17001, 0, 306.0f, 0.3f, 87.0f, 0.0f, 2, 306.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17001, 17002, "", Common::Rect(3334, 148, 3600, 452), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17001, 1, 306.0f, 0.3f, 87.0f, 0.0f, 2, 306.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17001, 17003, "", Common::Rect(3285, -99, 3550, 145), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17001, 2, 294.0f, 0.3f, 87.0f, 0.0f, 2, 294.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17001, 17004, "", Common::Rect(3056, -77, 3269, 266), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17001, 3, 273.0f, 0.3f, 87.0f, 0.0f, 2, 273.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17001, 17005, "", Common::Rect(2750, -24, 3033, 271), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17001, 4, 248.0f, 0.3f, 87.0f, 0.0f, 2, 248.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17002, 17001, "", Common::Rect(1648, -143, 2141, 343), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17002, 0, 126.0f, 0.3f, 87.0f, 0.0f, 2, 126.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17002, 17003, "", Common::Rect(3178, -59, 3566, 343), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17002, 1, 283.0f, 0.3f, 87.0f, 0.0f, 2, 283.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17002, 17004, "", Common::Rect(2661, -87, 2988, 287), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17002, 2, 252.0f, 0.3f, 87.0f, 0.0f, 2, 252.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17002, 17005, "", Common::Rect(2184, -92, 2634, 327), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17002, 3, 198.0f, 0.3f, 87.0f, 0.0f, 2, 198.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17003, 17001, "", Common::Rect(1486, -75, 1686, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17003, 0, 114.0f, 0.3f, 87.0f, 0.0f, 2, 114.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17003, 17002, "", Common::Rect(1096, -141, 1599, 462), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17003, 1, 103.0f, 0.3f, 87.0f, 0.0f, 2, 103.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17003, 17004, "", Common::Rect(2415, -75, 2615, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17003, 2, 207.0f, 0.3f, 87.0f, 0.0f, 2, 207.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17003, 17005, "", Common::Rect(1863, -75, 2063, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17003, 3, 151.0f, 0.3f, 87.0f, 0.0f, 2, 151.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(17003, kPuzzle17031, "", Common::Rect(2300, 448, 2589, 511), false, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(17003, 4, 0.0f, 15.0f, 85.7f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(17004, 17001, "", Common::Rect(1218, -129, 1532, 375), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17004, 0, 93.0f, 0.3f, 87.0f, 0.0f, 2, 93.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17004, 17002, "", Common::Rect(718, -129, 1225, 327), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17004, 1, 72.0f, 0.3f, 87.0f, 0.0f, 2, 72.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17004, 17003, "", Common::Rect(114, -126, 789, 348), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17004, 2, 27.0f, 0.3f, 87.0f, 0.0f, 2, 27.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17004, 17005, "", Common::Rect(1583, -115, 2149, 345), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17004, 3, 119.0f, 0.3f, 87.0f, 0.0f, 2, 119.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17005, 17001, "", Common::Rect(810, -3, 1165, 460), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17005, 0, 68.0f, 0.3f, 87.0f, 0.0f, 2, 68.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17005, 17002, "", Common::Rect(416, -43, 806, 425), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17005, 1, 18.0f, 0.3f, 87.0f, 0.0f, 2, 18.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17005, 17003, "", Common::Rect(63, -24, 432, 241), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17005, 2, 331.0f, 0.3f, 87.0f, 0.0f, 2, 331.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(17005, 17004, "", Common::Rect(3251, -7, 3600, 448), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(17005, 3, 299.0f, 0.3f, 87.0f, 0.0f, 2, 299.0f, 0.3f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle17031, 17003, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 99);
	_app->puzzleSetMovabilityToRotation(kPuzzle17031, 0, 184.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle17022, 17002, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 99);

	_app->objectAdd(kObject18, "", "", 1);
	_app->objectAddRotationAccessibility(kObject18, 17001, Common::Rect(1007, -188, 1232, 322), true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject18, kPuzzleMenu, Common::Rect(0, 0, 640, 480), false, kCursorIdle, 77);

	_app->objectAdd(kObjectRat, "Rat", "a06_Rat", 1);
	_app->objectSetActiveCursor(kObjectRat, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectRat, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectRat, 17004, Common::Rect(1719, 156, 1850, 251), false, kCursorTake, 0);
	_app->objectAddRotationAccessibility(kObjectRat, 17001, Common::Rect(2814, 108, 2879, 173), false, kCursorTake, 0);
	_app->objectAddRotationAccessibility(kObjectRat, 17002, Common::Rect(2350, 131, 2441, 192), false, kCursorTake, 0);
	_app->objectAddPresentation(kObjectRat);
	_app->objectPresentationAddImageToRotation(kObjectRat, 0, 17001, 9);
	_app->objectPresentationAddImageToRotation(kObjectRat, 0, 17002, 4);
	_app->objectPresentationAddImageToRotation(kObjectRat, 0, 17003, 6);
	_app->objectPresentationAddImageToRotation(kObjectRat, 0, 17004, 5);
	_app->objectPresentationShow(kObjectRat);
	_app->objectAdd(kObject17006, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17006, 17003, Common::Rect(59, 170, 724, 486), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject17006, 17003, Common::Rect(174, 34, 624, 291), false, kCursorAction, 1);
	_app->objectAddPresentation(kObject17006);
	_app->objectPresentationAddImageToRotation(kObject17006, 0, 17003, 0);
	_app->objectAddPresentation(kObject17006);
	_app->objectPresentationAddImageToRotation(kObject17006, 1, 17003, 2);
	_app->objectAdd(kObject17304, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17304, 17003, Common::Rect(3086, -24, 3304, 371), true, kCursorZoom, 0);
	_app->objectAdd(kObjectHandle, "Handle", "a06_Handle", 1);
	_app->objectSetActiveCursor(kObjectHandle, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectHandle, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectHandle, 17003, Common::Rect(202, 326, 597, 453), false, kCursorTake, 0);
	_app->objectAddRotationAccessibility(kObjectHandle, 17002, Common::Rect(367, 333, 497, 451), true, kCursorActionObject, 1);
	_app->objectAddPuzzleAccessibility(kObjectHandle, kPuzzle17022, Common::Rect(77, 126, 154, 223), true, kCursorAction, 2);
	_app->objectAddRotationAccessibility(kObjectHandle, 17002, Common::Rect(465, 174, 940, 394), false, kCursorMove, 3);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 1, kPuzzle17022, "a01s07n02p02s01", kImageTypeBMP, Common::Point(92, 140), kDrawType1, 1000, 10, 20.0f, 6);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 2, kPuzzle17022, "a01s07n02p02s02a", kImageTypeBMP, Common::Point(168, 93), kDrawType1, 1000, 5, 25.0f, 6);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 3, kPuzzle17022, "a01s07n02p02s02b", kImageTypeBMP, Common::Point(245, 84), kDrawType1, 1000, 5, 25.0f, 6);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 4, kPuzzle17022, "a01s07n02p02s02c", kImageTypeBMP, Common::Point(319, 92), kDrawType1, 1000, 5, 25.0f, 6);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 5, kPuzzle17022, "a01s07n02p02s02d", kImageTypeBMP, Common::Point(396, 92), kDrawType1, 1000, 5, 25.0f, 6);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 6, kPuzzle17022, "a01s07n02p02s02e", kImageTypeBMP, Common::Point(468, 95), kDrawType1, 1000, 5, 25.0f, 6);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 7, kPuzzle17022, "a01s07n02p02s02a", kImageTypeBMP, Common::Point(168, 93), kDrawType1, 1000, 5, 25.0f, 10);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 8, kPuzzle17022, "a01s07n02p02s02b", kImageTypeBMP, Common::Point(245, 84), kDrawType1, 1000, 5, 25.0f, 10);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 9, kPuzzle17022, "a01s07n02p02s02c", kImageTypeBMP, Common::Point(319, 92), kDrawType1, 1000, 5, 25.0f, 10);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 10, kPuzzle17022, "a01s07n02p02s02d", kImageTypeBMP, Common::Point(396, 92), kDrawType1, 1000, 5, 25.0f, 10);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddAnimationToPuzzle(kObjectHandle, 11, kPuzzle17022, "a01s07n02p02s02e", kImageTypeBMP, Common::Point(468, 95), kDrawType1, 1000, 5, 25.0f, 10);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddImageToRotation(kObjectHandle, 12, 17005, 8);
	_app->objectAddPresentation(kObjectHandle);
	_app->objectPresentationAddImageToRotation(kObjectHandle, 13, 17002, 6);
	_app->objectPresentationShow(kObjectHandle, 13);
	_app->objectAdd(kObject17001, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17001, 17001, Common::Rect(2490, 429, 2724, 526), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject17001, 17001, Common::Rect(2770, 284, 3056, 458), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject17001, 17001, Common::Rect(2221, 430, 2525, 591), false, kCursorAction, 1);
	_app->objectAddPresentation(kObject17001);
	_app->objectPresentationAddImageToRotation(kObject17001, 0, 17001, 0);
	_app->objectAddPresentation(kObject17001);
	_app->objectAdd(kObject17002, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17002, 17001, Common::Rect(2490,    0, 2724, 200), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject17002, 17001, Common::Rect(2719,  -83, 2975, 235), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject17002, 17001, Common::Rect(2100, -181, 2258, 415), false, kCursorAction, 1);
	_app->objectAddPresentation(kObject17002);
	_app->objectPresentationAddImageToRotation(kObject17002, 0, 17001, 2);
	_app->objectAdd(kObject17003, "", "", 1);
	_app->objectAddPresentation(kObject17003);
	_app->objectPresentationAddImageToRotation(kObject17003, 0, 17001, 3);
	_app->objectAddPresentation(kObject17003);
	_app->objectPresentationAddImageToRotation(kObject17003, 1, 17001, 4);
	_app->objectAdd(kObject17022, "", "", 3);
	_app->objectAddRotationAccessibility(kObject17022, 17001, Common::Rect(2590, -45, 2624, 12), false, kCursorMenuActive, 0);
	_app->objectAddRotationAccessibility(kObject17022, 17001, Common::Rect(2626, -45, 2664, 12), false, kCursorMenuActive, 1);
	_app->objectAddRotationAccessibility(kObject17022, 17001, Common::Rect(2666, -45, 2690, 12), false, kCursorMenuActive, 2);
	_app->objectAddPresentation(kObject17022);
	_app->objectPresentationAddImageToRotation(kObject17022, 0, 17001, 10);
	_app->objectAddPresentation(kObject17022);
	_app->objectPresentationAddImageToRotation(kObject17022, 1, 17001, 11);
	_app->objectAddPresentation(kObject17022);
	_app->objectPresentationAddImageToRotation(kObject17022, 2, 17001, 12);
	_app->objectAdd(kObjectKey, "Key", "a06_BarKey", 1);
	_app->objectSetActiveCursor(kObjectKey, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectKey, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectKey, 17001, Common::Rect(2255, 146, 2334, 233), false, kCursorTake, 0);
	_app->objectAdd(kObject17029, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17029, 17001, Common::Rect(2269, -71, 2700,  81), false, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject17029, 17001, Common::Rect(2325, 102, 2541, 218), false, kCursorZoom, 0);
	_app->objectAdd(kObject17504, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17504, 17004, Common::Rect(2116, 174, 2380, 333), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject17504, 17005, Common::Rect(2988, 312, 3290, 455), true, kCursorZoom, 0);
	_app->objectAddPuzzleAccessibility(kObject17504, kPuzzle17042, Common::Rect(0, 420, 640, 464), true, kCursorBack, 1);
	_app->objectAdd(kObject17506, "", "CUR_DRAG", 4);
	_app->objectSetActiveDrawCursor(kObject17506, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject17506, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject17506, kPuzzle17042, Common::Rect(82, 296, 160, 372), true, kCursorDragDrop, 0);

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject17506);
		_app->objectPresentationAddImageToPuzzle(kObject17506, i, kPuzzle17042, Common::String::format("a01s07n04p01s02.%04d.bmp", i + 1), Common::Point(0, 92), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject17401, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject17401, kPuzzle17042, Common::Rect(434, 221, 503, 352), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject17401, kPuzzle17042, Common::Rect(313, 176, 502, 355), true, kCursorActionObject, 1);
	_app->objectAddPresentation(kObject17401);
	_app->objectPresentationAddAnimationToPuzzle(kObject17401, 0, kPuzzle17042, "a01s07n04p01s01", kImageTypeBMP, Common::Point(271, 174), kDrawType1, 1000, 5, 12.5f, 4);
	_app->objectPresentationAddAnimationToRotation(kObject17401, 0, 17001, 1, 5, 12.5f, 4);
	_app->objectPresentationAddAnimationToRotation(kObject17401, 0, 17002, 1, 5, 12.5f, 4);
	_app->objectPresentationAddAnimationToRotation(kObject17401, 0, 17003, 1, 5, 12.5f, 4);
	_app->objectPresentationAddAnimationToRotation(kObject17401, 0, 17004, 1, 5, 12.5f, 4);
	_app->objectAddPresentation(kObject17401);
	_app->objectPresentationAddAnimationToRotation(kObject17401, 1, 17005, 11, 5, 12.5f, 4);
	_app->objectAddPresentation(kObject17401);
	_app->objectPresentationAddAnimationToRotation(kObject17401, 2, 17005, 12, 5, 12.5f, 4);
	_app->objectAdd(kObject17505, "", "", 1);
	_app->objectAddPresentation(kObject17505);
	_app->objectPresentationAddImageToPuzzle(kObject17505, 0, kPuzzle17042, "a01s07n04p01s02.0015.bmp", Common::Point(0, 92), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToRotation(kObject17505, 0, 17001, 8);
	_app->objectPresentationAddImageToRotation(kObject17505, 0, 17002, 5);
	_app->objectPresentationAddImageToRotation(kObject17505, 0, 17003, 7);
	_app->objectPresentationAddImageToRotation(kObject17505, 0, 17004, 2);
	_app->objectPresentationAddImageToRotation(kObject17505, 0, 17005, 7);
	_app->objectAdd(kObjectCylinder, "Cylinder", "a06_RoulCyl", 1);
	_app->objectSetActiveCursor(kObjectCylinder, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectCylinder, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectCylinder, 17004, Common::Rect(2833, -83, 3093, 172), true, kCursorTake, 0);
	_app->objectAddRotationAccessibility(kObjectCylinder, 17002, Common::Rect(2787, 305, 3171, 497), true, kCursorActionObject, 1);
	_app->objectAddRotationAccessibility(kObjectCylinder, 17004, Common::Rect(808,  263, 1179, 427), true, kCursorActionObject, 1);
	_app->objectAddRotationAccessibility(kObjectCylinder, 17003, Common::Rect(1761, 286, 2112, 456), true, kCursorActionObject, 1);
	_app->objectAddRotationAccessibility(kObjectCylinder, 17002, Common::Rect(2787, 305, 3171, 497), false, kCursorActionObject, 2);
	_app->objectAddRotationAccessibility(kObjectCylinder, 17004, Common::Rect(808,  263, 1179, 427), false, kCursorActionObject, 2);
	_app->objectAddRotationAccessibility(kObjectCylinder, 17003, Common::Rect(1761, 286, 2112, 456), false, kCursorActionObject, 2);
	_app->objectAddPuzzleAccessibility(kObjectCylinder, kPuzzle17021, Common::Rect(274, 174, 333, 233), true, kCursorMenuActive, 3);
	_app->objectAddPuzzleAccessibility(kObjectCylinder, kPuzzle17021, Common::Rect(  0, 420, 640, 464), true, kCursorBack, 9);
	_app->objectAddPresentation(kObjectCylinder);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 0, 17001, 5);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 0, 17002, 0);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 0, 17003, 3);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 0, 17004, 0);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 0, 17005, 4);
	_app->objectAddPresentation(kObjectCylinder);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 1, 17001, 6);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 1, 17002, 2);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 1, 17003, 4);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 1, 17004, 3);
	_app->objectPresentationAddImageToRotation(kObjectCylinder, 1, 17005, 5);
	_app->objectAddPresentation(kObjectCylinder);
	_app->objectPresentationAddImageToPuzzle(kObjectCylinder, 2, kPuzzle17021, "a01s07n02p01l01.0001.bmp", Common::Point(294, 186), true, kDrawType1, 1000);
	_app->objectAdd(kObjectWhip, "Whip", "a06_Whip", 1);
	_app->objectSetActiveCursor(kObjectWhip, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectWhip, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectWhip, kPuzzle17031, Common::Rect( 84, 188, 302, 289), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObjectWhip, kPuzzle17022, Common::Rect(160,  90, 220, 150), false, kCursorActionObject, 10);
	_app->objectAddPuzzleAccessibility(kObjectWhip, kPuzzle17022, Common::Rect(240,  90, 300, 150), false, kCursorActionObject, 11);
	_app->objectAddPuzzleAccessibility(kObjectWhip, kPuzzle17022, Common::Rect(320,  90, 380, 150), false, kCursorActionObject, 12);
	_app->objectAddPuzzleAccessibility(kObjectWhip, kPuzzle17022, Common::Rect(400,  90, 460, 150), false, kCursorActionObject, 13);
	_app->objectAddPuzzleAccessibility(kObjectWhip, kPuzzle17022, Common::Rect(480,  90, 540, 150), false, kCursorActionObject, 14);
	_app->objectAddPresentation(kObjectWhip);
	_app->objectPresentationAddImageToPuzzle(kObjectWhip, 0, kPuzzle17031, "a01s07n03p01l01.0001.bmp", Common::Point(91, 195), true, kDrawType1, 1000);
	_app->objectAdd(kObjectReel, "Reel", "a06_Reel", 1);
	_app->objectSetActiveCursor(kObjectReel, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectReel, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectReel, kPuzzle17031, Common::Rect(349, 179, 522, 239), true, kCursorTake, 0);
	_app->objectAddPresentation(kObjectReel);
	_app->objectPresentationAddImageToPuzzle(kObjectReel, 0, kPuzzle17031, "a01s07n03p01l02.0001.bmp", Common::Point(367, 187), true, kDrawType1, 1000);
	_app->objectAdd(kObject17201, "", "CUR_DRAG", 4);
	_app->objectSetActiveDrawCursor(kObject17201, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject17201, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject17201, kPuzzle17021, Common::Rect(4, 239, 531, 459), true, kCursorDragDrop, 0);

	for (uint32 i = 0; i < 76; i++) {
		_app->objectAddPresentation(kObject17201);
		_app->objectPresentationAddImageToPuzzle(kObject17201, i, kPuzzle17021, Common::String::format("a01s07n02p01s01.%04d.bmp", i + 1), Common::Point(0, 228), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject17203, "", "", 1);
	_app->objectAddPresentation(kObject17203);
	_app->objectPresentationAddImageToPuzzle(kObject17203, 0, kPuzzle17021, "a01s07n02p01s02.bmp", Common::Point(0, 16), true, kDrawType1, 800);
	_app->objectPresentationAddAnimationToPuzzle(kObject17203, 0, kPuzzle17021, "a01s07n02p01s02", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 800, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject17203, 0, 0, 17008);
	_app->objectPresentationAddImageToRotation(kObject17203, 0, 17001, 7);
	_app->objectPresentationAddImageToRotation(kObject17203, 0, 17002, 3);
	_app->objectPresentationAddImageToRotation(kObject17203, 0, 17003, 5);
	_app->objectPresentationAddImageToRotation(kObject17203, 0, 17004, 4);
	_app->objectPresentationAddImageToRotation(kObject17203, 0, 17005, 6);
	_app->objectAdd(kObject17204, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17204, 17002, Common::Rect(3596, 92, 3600, 493), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject17204, 17002, Common::Rect(0, 92, 346, 493), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject17204);
	_app->objectPresentationAddImageToRotation(kObject17204, 0, 17002, 7);
	_app->objectAdd(kObject17402, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17402, 17004, Common::Rect(2358, 169, 2657, 470), true, kCursorZoom, 0);
	_app->objectAdd(kObject17403, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17403, 17004, Common::Rect(3366, 162, 3564, 240), true, kCursorZoom, 0);
	_app->objectAdd(kObject17205, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17205, 17002, Common::Rect(3577, -219, 3600, -139), true, kCursorZoom, 4);
	_app->objectAddRotationAccessibility(kObject17205, 17002, Common::Rect(   0, -219,  131, -139), true, kCursorZoom, 4);
	_app->objectAddRotationAccessibility(kObject17205, 17002, Common::Rect( 383, -286,  588, -174), true, kCursorZoom, 5);
	_app->objectAddRotationAccessibility(kObject17205, 17003, Common::Rect( 416, -401,  718, -261), true, kCursorZoom, 4);
	_app->objectAddRotationAccessibility(kObject17205, 17003, Common::Rect( 383, -286,  588, -174), true, kCursorZoom, 5);
	_app->objectAddRotationAccessibility(kObject17205, 17003, Common::Rect(2626, -198, 2743, -136), true, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject17205, 17003, Common::Rect(2768, -240, 2921, -158), true, kCursorZoom, 2);
	_app->objectAddRotationAccessibility(kObject17205, 17003, Common::Rect(2965, -300, 3142, -177), true, kCursorZoom, 3);
	_app->objectAddRotationAccessibility(kObject17205, 17004, Common::Rect(2990, -347, 3264, -223), true, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject17205, 17004, Common::Rect(3341, -336, 3550, -181), true, kCursorZoom, 2);
	_app->objectAddRotationAccessibility(kObject17205, 17004, Common::Rect(3566, -263, 3600, -144), true, kCursorZoom, 3);
	_app->objectAddPuzzleAccessibility(kObject17205, kPuzzle17062, Common::Rect(0, 420, 640, 464), true, kCursorBack, 999);
	_app->objectAddPuzzleAccessibility(kObject17205, kPuzzle17063, Common::Rect(0, 420, 640, 464), true, kCursorBack, 999);
	_app->objectAddPuzzleAccessibility(kObject17205, kPuzzle17064, Common::Rect(0, 420, 640, 464), true, kCursorBack, 999);
	_app->objectAddPuzzleAccessibility(kObject17205, kPuzzle17065, Common::Rect(0, 420, 640, 464), true, kCursorBack, 999);
	_app->objectAddPuzzleAccessibility(kObject17205, kPuzzle17066, Common::Rect(0, 420, 640, 464), true, kCursorBack, 999);
	_app->objectAdd(kObject17004, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17004, 17005, Common::Rect(2097, -71, 2429, 317), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject17004);
	_app->objectPresentationAddAnimationToRotation(kObject17004, 0, 17005, 1, 10, 12.5f, 6);
	_app->objectAddPresentation(kObject17004);
	_app->objectPresentationAddImageToRotation(kObject17004, 1, 17005, 0);
	_app->objectAddPresentation(kObject17004);
	_app->objectPresentationAddAnimationToRotation(kObject17004, 2, 17005, 1, 10, 12.5f, 10);
	_app->objectAdd(kObject17005, "", "", 1);
	_app->objectAddRotationAccessibility(kObject17005, 17005, Common::Rect(2237, 144, 2343, 228), false, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject17005);
	_app->objectPresentationAddAnimationToRotation(kObject17005, 0, 17005, 9, 7, 12.5f, 6);
	_app->objectAddPresentation(kObject17005);
	_app->objectPresentationAddAnimationToRotation(kObject17005, 1, 17005, 10, 7, 12.5f, 6);
	_app->objectAddPresentation(kObject17005);
	_app->objectPresentationAddImageToRotation(kObject17005, 2, 17005, 2);
	_app->objectPresentationAddImageToPuzzle(kObject17005, 2, kPuzzle17051, "a01s07n05p01l01.bmp", Common::Point(276, 17), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject17005);
	_app->objectPresentationAddImageToRotation(kObject17005, 3, 17005, 3);
	_app->objectAddPresentation(kObject17005);
	_app->objectPresentationAddAnimationToRotation(kObject17005, 4, 17005, 9, 7, 12.5f, 10);
	_app->objectAddPresentation(kObject17005);
	_app->objectPresentationAddAnimationToRotation(kObject17005, 5, 17005, 10, 7, 12.5f, 10);
	_app->objectAdd(kObject17503, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject17503, kPuzzle17051, Common::Rect(250,  24, 342, 254), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject17503, kPuzzle17051, Common::Rect(  0, 420, 640, 464), true, kCursorBack, 9);
	_app->objectAddPuzzleAccessibility(kObject17503, kPuzzle17051, Common::Rect(266, 340, 303, 390), true, kCursorAction, 3);
	_app->objectAddPresentation(kObject17503);
	_app->objectPresentationAddImageToPuzzle(kObject17503, 0, kPuzzle17051, "a01s07n05p01l02.bmp", Common::Point(262, 328), true, kDrawType1, 1000);
	_app->soundAdd(17020, kSoundTypeBackgroundMusic, "1284.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(17020, 95);
	_app->soundAdd(17019, kSoundTypeBackgroundMusic, "1285.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(17019, 85);
	_app->soundAdd(17045, kSoundTypeAmbientEffect, "1286.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17044, kSoundTypeAmbientEffect, "1287.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17034, kSoundTypeAmbientEffect, "1288.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17047, kSoundTypeAmbientEffect, "1289.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(17047, 85);
	_app->soundAdd(17038, kSoundTypeAmbientEffect, "1290.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17027, kSoundTypeAmbientEffect, "1291.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17035, kSoundTypeAmbientEffect, "1292.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17028, kSoundTypeAmbientEffect, "1293.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17032, kSoundTypeAmbientEffect, "1294.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17033, kSoundTypeAmbientEffect, "1295.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17025, kSoundTypeAmbientEffect, "1296.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17030, kSoundTypeAmbientEffect, "1297.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17021, kSoundTypeAmbientEffect, "1298.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17022, kSoundTypeAmbientEffect, "1299.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17023, kSoundTypeAmbientEffect, "1300.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17024, kSoundTypeAmbientEffect, "1301.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17026, kSoundTypeAmbientEffect, "1302.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17029, kSoundTypeAmbientEffect, "1303.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17031, kSoundTypeAmbientEffect, "1304.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17036, kSoundTypeAmbientEffect, "1305.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17037, kSoundTypeAmbientEffect, "1306.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17039, kSoundTypeAmbientEffect, "1307.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17041, kSoundTypeAmbientEffect, "1309.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17042, kSoundTypeAmbientEffect, "1310.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17043, kSoundTypeAmbientEffect, "1311.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17046, kSoundTypeAmbientEffect, "1312.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17048, kSoundTypeAmbientEffect, "1313.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17049, kSoundTypeAmbientEffect, "1314.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17050, kSoundTypeAmbientEffect, "1315.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17051, kSoundTypeAmbientEffect, "1316.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17052, kSoundTypeAmbientEffect, "1317.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17053, kSoundTypeAmbientEffect, "1318.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17054, kSoundTypeAmbientEffect, "1319.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17055, kSoundTypeAmbientEffect, "1320.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17056, kSoundTypeAmbientEffect, "1321.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17057, kSoundTypeAmbientEffect, "1322.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17058, kSoundTypeAmbientEffect, "1323.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17059, kSoundTypeAmbientEffect, "1324.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17060, kSoundTypeAmbientEffect, "1325.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(17060, 95);
	_app->soundAdd(17061, kSoundTypeAmbientEffect, "1326.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17062, kSoundTypeAmbientEffect, "1327.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(17000, kSoundTypeDialog, "1328.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17002, kSoundTypeDialog, "1329.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17003, kSoundTypeDialog, "1330.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17004, kSoundTypeDialog, "1331.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17005, kSoundTypeDialog, "1332.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17006, kSoundTypeDialog, "1333.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17007, kSoundTypeDialog, "1334.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17009, kSoundTypeDialog, "1335.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17010, kSoundTypeDialog, "1336.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17013, kSoundTypeDialog, "1339.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17014, kSoundTypeDialog, "1340.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(17015, kSoundTypeDialog, "1341.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);

	for (uint32 i = 0; i < 11; i++) {
		_app->varDefineByte(17000 + i, 99);
		_app->varDefineDword(17000 + i, 0);
	}

	_app->varDefineByte(17011, 9);
	_app->varDefineByte(17020, 0);
	_app->varDefineByte(17021, 0);
	_app->varDefineByte(17022, 0);
	_app->varDefineByte(17023, 1);
	_app->varDefineByte(17024, 0);
	_app->varDefineByte(17025, 0);
	_app->varDefineWord(17001, 0);
	_app->varDefineWord(17002, 0);
	_app->varDefineWord(17003, 0);
	_app->varDefineWord(17004, 0);
	_app->varDefineDword(17021, 0);
	_app->varDefineDword(17022, 0);
	_app->varDefineString(17001, "99999");
	_app->varDefineByte(17800, 18);
	_app->varDefineByte(17850, 2);
	_app->varDefineByte(17801, 6);
	_app->varDefineByte(17851, 1);
	_app->varDefineByte(17802, 21);
	_app->varDefineByte(17852, 2);
	_app->varDefineByte(17803, 33);
	_app->varDefineByte(17853, 1);
	_app->varDefineByte(17804, 16);
	_app->varDefineByte(17854, 2);
	_app->varDefineByte(17805, 4);
	_app->varDefineByte(17855, 1);
	_app->varDefineByte(17806, 23);
	_app->varDefineByte(17856, 2);
	_app->varDefineByte(17807, 35);
	_app->varDefineByte(17857, 1);
	_app->varDefineByte(17808, 14);
	_app->varDefineByte(17858, 2);
	_app->varDefineByte(17809, 2);
	_app->varDefineByte(17859, 1);
	_app->varDefineByte(17810, 100);
	_app->varDefineByte(17860, 3);
	_app->varDefineByte(17811, 28);
	_app->varDefineByte(17861, 1);
	_app->varDefineByte(17812, 9);
	_app->varDefineByte(17862, 2);
	_app->varDefineByte(17813, 26);
	_app->varDefineByte(17863, 1);
	_app->varDefineByte(17814, 30);
	_app->varDefineByte(17864, 2);
	_app->varDefineByte(17815, 11);
	_app->varDefineByte(17865, 1);
	_app->varDefineByte(17816, 7);
	_app->varDefineByte(17866, 2);
	_app->varDefineByte(17817, 20);
	_app->varDefineByte(17867, 1);
	_app->varDefineByte(17818, 32);
	_app->varDefineByte(17868, 2);
	_app->varDefineByte(17819, 17);
	_app->varDefineByte(17869, 1);
	_app->varDefineByte(17820, 5);
	_app->varDefineByte(17870, 2);
	_app->varDefineByte(17821, 22);
	_app->varDefineByte(17871, 1);
	_app->varDefineByte(17822, 34);
	_app->varDefineByte(17872, 2);
	_app->varDefineByte(17823, 15);
	_app->varDefineByte(17873, 1);
	_app->varDefineByte(17824, 3);
	_app->varDefineByte(17874, 2);
	_app->varDefineByte(17825, 24);
	_app->varDefineByte(17875, 1);
	_app->varDefineByte(17826, 36);
	_app->varDefineByte(17876, 2);
	_app->varDefineByte(17827, 13);
	_app->varDefineByte(17877, 1);
	_app->varDefineByte(17828, 1);
	_app->varDefineByte(17878, 2);
	_app->varDefineByte(17829, 50);
	_app->varDefineByte(17879, 3);
	_app->varDefineByte(17830, 27);
	_app->varDefineByte(17880, 2);
	_app->varDefineByte(17831, 10);
	_app->varDefineByte(17881, 1);
	_app->varDefineByte(17832, 25);
	_app->varDefineByte(17882, 2);
	_app->varDefineByte(17833, 29);
	_app->varDefineByte(17883, 1);
	_app->varDefineByte(17834, 12);
	_app->varDefineByte(17884, 2);
	_app->varDefineByte(17835, 8);
	_app->varDefineByte(17885, 1);
	_app->varDefineByte(17836, 19);
	_app->varDefineByte(17886, 2);
	_app->varDefineByte(17837, 31);
	_app->varDefineByte(17887, 1);
	_app->varDefineByte(17900, 19);
	_app->varDefineByte(17910, -1);
	_app->varDefineByte(17901, 100);
	_app->varDefineByte(17911, -1);
	_app->varDefineByte(17902, -1);
	_app->varDefineByte(17912, 1);
	_app->varDefineByte(17903, 7);
	_app->varDefineByte(17913, -1);
	_app->varDefineByte(17904, -1);
	_app->varDefineByte(17914, 2);
	_app->varDefineByte(17920, 0);
	_app->varDefineByte(17930, 0);
	_app->varDefineByte(17921, 0);
	_app->varDefineByte(17931, 0);
	_app->varDefineByte(17922, 0);
	_app->varDefineByte(17932, 0);
	_app->varDefineByte(17923, 0);
	_app->varDefineByte(17933, 0);
	_app->varDefineByte(17924, 0);
	_app->varDefineByte(17934, 0);
	_app->varDefineByte(17940, 0);
	_app->varDefineByte(17950, 0);
	_app->varDefineByte(17027, 2);
	_app->varDefineByte(17026, 0);
	_app->varDefineByte(17042, 0);
	_app->varDefineByte(17028, 0);
	_app->varDefineByte(17029, 0);
	_app->varDefineByte(17030, 0);
	_app->varDefineByte(17031, 2);
	_app->varDefineByte(17032, 5);
	_app->varDefineByte(17033, 4);
	_app->varDefineByte(17034, 3);
	_app->varDefineByte(17035, 1);
	_app->varDefineByte(17041, 0);
	_app->varDefineByte(17036, 1);
	_app->varDefineByte(17037, 1);
	_app->varDefineByte(17038, 1);
	_app->varDefineByte(17039, 10);
	_app->varDefineByte(17040, 1);
	_app->varDefineByte(17043, 0);
	_app->varDefineByte(17012, 0);
	_app->varDefineByte(17013, 0);
	_app->varDefineByte(17014, 0);
	_app->varDefineByte(17015, 0);
	_app->varDefineByte(17016, 0);
	_app->varDefineByte(17017, 0);
	_app->varDefineByte(17101, 0);
	_app->varDefineByte(17102, 0);
}

void Zone5Faust::onSetup(SetupType type) {
	error("[Zone5Faust::onSetup] Not implemented");
}

void Zone5Faust::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->soundStop(17047, 1024);
		_app->soundPlay(17044);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->puzzleSetActive(kPuzzle17022);
		break;

	case kTimer3:
		if (_app->varGetByte(17030) == 5) {
			_app->varSetByte(17030, 0);
			_app->timerStop(kTimer3);

			if (_app->varGetByte(17041) == 4) {
				_app->varSetByte(17102, 1);

				if ((_app->varGetByte(17101) + _app->varGetByte(17102)) >= 2) {
					_app->playMovie("1356");
					_app->fadeOut(15, Color(0, 0, 0), 0);

					if (_app->getSaveManager()->getData()->progressState == 5 )
						_app->getSaveManager()->getData()->progress++;

					_app->setupRotation(2);
				}
			}

			_app->objectSetAccessibilityOn(kObjectHandle, 2, 2);

		} else {
			_app->varSetByte(17030, _app->varGetByte(17030) + 1);

			uint32 index = _app->varGetByte(_app->varGetByte(17030) + 17030) + 1;
			_app->objectPresentationSetAnimationOnPuzzle(kObjectHandle, index, 0, 17006);
			_app->objectPresentationShow(kObjectHandle, index);
			_app->soundPlay(17056);
		}
		break;

	case kTimer4: {
		_app->timerStop(kTimer4);
		_app->objectSetAccessibilityOff(17301, 1, 5);

		uint32 index = _app->varGetByte(_app->varGetByte(17030) + 17030);
		_app->objectPresentationUnpauseAnimation(17303, index + 1);
		_app->objectPresentationShow(kObjectHandle, index + 6);
		_app->objectPresentationHide(kObjectHandle, index + 1);
		_app->soundPlay(17055);
		}
		break;

	case kTimer5:
		if (rnd(2) == 0)
			_app->soundPlay(17041 + rnd(3));
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->soundStop(17047, 1024);
		_app->soundPlay(17044);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}

		_app->varSetByte(17042, 1);
		break;

	case kTimer7:
		_app->timerStop(kTimer7);
		_app->timerStart(kTimer7, rnd(5000) + 8000);
		_app->soundPlay(17020);
		break;

	case kTimer8:
		_app->timerStop(kTimer8);
		_app->soundStop(17047, 1024);
		_app->objectPresentationHide(kObject17401);

		if (_app->varGetByte(17027) == 1) {
			_app->objectPresentationHide(kObject17505);
			_app->objectPresentationShow(kObject17505);
		}
		break;
	}
}

void Zone5Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone5Faust::onButtonUp] Not implemented");
}

void Zone5Faust::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	if (id == kObject17022) {
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->objectPresentationShow(kObject17022, target);
	}
}

void Zone5Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone5] Not implemented");
}

void Zone5Faust::onSound(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	if (id == 17002) {
		_app->playMovie("1357");
		_app->objectPresentationHide(kObject17204, 0);
		_app->rotationSetMovabilityOn(17002);
		_app->objectSetAccessibilityOff(kObject18, 1, 1);
	}
}

void Zone5Faust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[Zone5Faust::onBag] Not implemented");
}

} // End of namespace Ring
