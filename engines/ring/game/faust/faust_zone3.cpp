/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 03110-1301, USA.
 */

#include "ring/game/faust/faust_zone3.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone3Faust::Zone3Faust(ApplicationFaust *application) : _app(application) {
}

Zone3Faust::~Zone3Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone3Faust::onInit() {
	_app->setCurrentZone(kZone3);

	_app->puzzleAdd(kPuzzle12001);
	_app->puzzleAddBackgroundImage(kPuzzle12001, "a01s02n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12002);
	_app->puzzleAddBackgroundImage(kPuzzle12002, "a01s02n02p04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12004);
	_app->puzzleAddBackgroundImage(kPuzzle12004, "a01s02n03p02l01a.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12005);
	_app->puzzleAddBackgroundImage(kPuzzle12005, "a01s02n03p02l02a.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12006);
	_app->puzzleAddBackgroundImage(kPuzzle12006, "a01s02n03p02l01b.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12007);
	_app->puzzleAddBackgroundImage(kPuzzle12007, "a01s02n03p02l02b.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12008);
	_app->puzzleAddBackgroundImage(kPuzzle12008, "a01s02n03p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12009);
	_app->puzzleAddBackgroundImage(kPuzzle12009, "a01s02n03p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12010);
	_app->puzzleAddBackgroundImage(kPuzzle12010, "a01s02n03p04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12011);
	_app->puzzleAddBackgroundImage(kPuzzle12011, "a01s02n02p03.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(12001, "1044", 0, 10);
	_app->rotationSetCompressionBufferLength(12001, 1400000);
	_app->rotationAdd(12002, "1045", 0, 5);
	_app->rotationAdd(12003, "1046", 0, 1);
	_app->rotationAdd(12004, "1047", 0, 8);
	_app->rotationSetCompressionBufferLength(12004, 2000000);
	_app->rotationAddMovabilityToRotation(12001, 12002, "1049", Common::Rect(311, -122, 599, 256), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(12001, 0, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(12001, kPuzzle12011, "", Common::Rect(956, -52, 1221, 143), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(12001, 1, 65.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(12002, 12001, "1050", Common::Rect(2056, -170, 2436, 242), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(12002, 0, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(12002, 12003, "1051", Common::Rect(253, -163, 660, 268), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(12002, 1, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(12002, kPuzzle12011, "", Common::Rect(1538, -132, 1785, 78), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(12002, 2, 122.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(12003, 12002, "1052", Common::Rect(2107, -122, 2371, 237), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(12003, 0, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(12003, 12004, "1053", Common::Rect(286, -177, 638, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(12003, 1, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(12004, 12003, "1054", Common::Rect(2142, -289, 2571, 280), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(12004, 0, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12001, 12001, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12001, 0, 90.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12004, 12002, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle12005, 12002, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle12006, 12003, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle12007, 12003, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle12008, 12002, "", Common::Rect(320, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12008, 0, 67.0f, 12.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12008, 12003, "", Common::Rect(0, 420, 320, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12008, 1, 138.0f, 12.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12009, 12002, "", Common::Rect(320, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12009, 0, 67.0f, 12.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12009, 12003, "", Common::Rect(0, 420, 320, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12009, 1, 138.0f, 12.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12010, 12002, "", Common::Rect(320, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12010, 0, 67.0f, 12.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12010, 12003, "", Common::Rect(0, 420, 320, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12010, 1, 138.0f, 12.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12011, 12001, "", Common::Rect(320, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12011, 0, 65.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle12011, 12002, "", Common::Rect(0, 420, 320, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle12011, 1, 122.0f, 0.0f, 87.0f);
	_app->objectAdd(kObject12013, "", "", 1);
	_app->objectAddRotationAccessibility(kObject12013, 12001, Common::Rect(1914, -214, 2649, 327), true, kCursorMove, 0);
	_app->objectAdd(kObjectAshShovel, "Ash shovel", "A01_Shovel", 9);
	_app->objectSetActiveCursor(kObjectAshShovel, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectAshShovel, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObjectAshShovel);
	_app->objectPresentationAddImageToRotation(kObjectAshShovel, 0, 12001, 0);
	_app->objectAdd(kObject12007, "", "", 1);
	_app->objectAdd(kObjectBurnedKey, "Burned Key", "A01_BurnedKey", 1);
	_app->objectSetActiveCursor(kObjectBurnedKey, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBurnedKey, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectShovelWithAshes, "Shovel with ashes", "A01_ShovelAshes", 1);
	_app->objectSetActiveCursor(kObjectShovelWithAshes, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectShovelWithAshes, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject12007);
	_app->objectPresentationAddImageToPuzzle(kObject12007, 0, kPuzzle12001, "a01s02n02p02l01.tga", Common::Point(309, 304), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject12007);
	_app->objectPresentationAddImageToPuzzle(kObject12007, 1, kPuzzle12001, "a01s02n02p02l02.tga", Common::Point(264, 285), true, kDrawType3, 1001);
	_app->objectAddPresentation(kObject12007);
	_app->objectPresentationAddImageToPuzzle(kObject12007, 2, kPuzzle12001, "a01s02n02p02l03.tga", Common::Point(281, 268), true, kDrawType3, 1002);
	_app->objectAddPresentation(kObject12007);
	_app->objectPresentationAddImageToPuzzle(kObject12007, 3, kPuzzle12001, "a01s02n02p02l04.tga", Common::Point(307, 286), true, kDrawType3, 1003);
	_app->objectAddPresentation(kObject12007);
	_app->objectPresentationAddImageToPuzzle(kObject12007, 4, kPuzzle12001, "a01s02n02p02l05.bmp", Common::Point(467, 56), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObject12007, 0);
	_app->objectPresentationShow(kObject12007, 1);
	_app->objectPresentationShow(kObject12007, 2);
	_app->objectPresentationShow(kObject12007, 3);
	_app->objectAddPuzzleAccessibility(kObjectAshShovel, kPuzzle12001, Common::Rect(466, 48, 553, 241), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObject12007, kPuzzle12001, Common::Rect(314, 309, 356, 324), false, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObject12007, kPuzzle12001, Common::Rect(267, 287, 304, 321), true, kCursorActionObject, 1);
	_app->objectAddPuzzleAccessibility(kObject12007, kPuzzle12001, Common::Rect(297, 277, 333, 309), true, kCursorActionObject, 2);
	_app->objectAddPuzzleAccessibility(kObject12007, kPuzzle12001, Common::Rect(321, 294, 355, 325), true, kCursorActionObject, 3);
	_app->objectAdd(kObject12003, "", "", 1);
	_app->objectAdd(kObjectBronzeKey, "Bronze key", "A01_BronzeKey", 1);
	_app->objectSetActiveCursor(kObjectBronzeKey, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBronzeKey, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject12003);
	_app->objectPresentationAddAnimationToRotation(kObject12003, 0, 12001, 3, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12003, 0, 0, 12002);
	_app->objectAddPresentation(kObject12003);
	_app->objectPresentationAddImageToPuzzle(kObject12003, 1, kPuzzle12002, "a01s02n02p04l02.tga", Common::Point(170, 132), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject12003);
	_app->objectPresentationAddImageToPuzzle(kObject12003, 2, kPuzzle12002, "a01s02n02p04l01.tga", Common::Point(233, 145), true, kDrawType3, 1000);
	_app->objectPresentationHide(kObject12003, 1);
	_app->objectPresentationHide(kObject12003, 2);
	_app->objectAddPresentation(kObject12003);
	_app->objectPresentationAddAnimationToRotation(kObject12003, 3, 12001, 3, 10, 12.5f, 10);
	_app->objectAddPresentation(kObject12003);
	_app->objectPresentationAddAnimationToRotation(kObject12003, 4, 12002, 4, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12003, 4, 0, 12002);
	_app->objectAddPresentation(kObject12003);
	_app->objectPresentationAddAnimationToRotation(kObject12003, 5, 12002, 4, 10, 12.5f, 10);
	_app->objectAddRotationAccessibility(kObject12003, 12001, Common::Rect(945, 169, 1031, 225), true, kCursorZoom, 0);
	_app->objectAddPuzzleAccessibility(kObject12003, kPuzzle12002, Common::Rect(352, 249, 390, 305), true, kCursorZoom, 1);
	_app->objectAddPuzzleAccessibility(kObject12003, kPuzzle12002, Common::Rect(213, 243, 323, 302), true, kCursorZoom, 2);
	_app->objectAddPuzzleAccessibility(kObject12003, kPuzzle12002, Common::Rect(0, 420, 640, 464), false, kCursorBack, 3);
	_app->objectAddRotationAccessibility(kObject12003, 12002, Common::Rect(1590, 92, 1712, 167), true, kCursorZoom, 4);
	_app->objectAddPuzzleAccessibility(kObject12003, kPuzzle12002, Common::Rect(0, 420, 640, 464), true, kCursorBack, 5);
	_app->objectAdd(kObject12004, "", "", 1);
	_app->objectAddPresentation(kObject12004);
	_app->objectPresentationAddAnimationToRotation(kObject12004, 0, 12001, 1, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12004, 0, 0, 12001);
	_app->objectAddPresentation(kObject12004);
	_app->objectPresentationAddAnimationToRotation(kObject12004, 1, 12001, 1, 15, 12.5f, 10);
	_app->objectAddRotationAccessibility(kObject12004, 12001, Common::Rect(1255, 387, 1369, 441), true, kCursorZoom, 0);
	_app->objectAdd(kObject12005, "", "", 1);
	_app->objectAddPresentation(kObject12005);
	_app->objectPresentationAddAnimationToRotation(kObject12005, 0, 12001, 4, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12005, 0, 0, 12006);
	_app->objectAddPresentation(kObject12005);
	_app->objectPresentationAddAnimationToRotation(kObject12005, 1, 12001, 4, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12005, 1, 0, 12007);
	_app->objectAddRotationAccessibility(kObject12005, 12001, Common::Rect(3058, 359, 3373, 446), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject12005, 12001, Common::Rect(3343, 352, 3570, 418), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject12005, 12001, Common::Rect(2921, 350, 3069, 495), false, kCursorAction, 2);
	_app->objectAddRotationAccessibility(kObject12005, 12001, Common::Rect(3076, 368, 3359, 441), false, kCursorZoom, 3);
	_app->objectAdd(kObject12006, "", "", 1);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 0, 12001, 5, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 0, 0, 12008);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 1, 12001, 6, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 1, 0, 12008);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 2, 12001, 7, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 2, 0, 12008);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 3, 12001, 8, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 3, 0, 12008);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 4, 12001, 5, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 4, 0, 12009);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 5, 12001, 6, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 5, 0, 12009);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 6, 12001, 7, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 6, 0, 12009);
	_app->objectAddPresentation(kObject12006);
	_app->objectPresentationAddAnimationToRotation(kObject12006, 7, 12001, 8, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12006, 7, 0, 12009);
	_app->objectAddRotationAccessibility(kObject12006, 12001, Common::Rect(3454, 320, 3527, 345), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject12006, 12001, Common::Rect(3454, 354, 3533, 364), true, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject12006, 12001, Common::Rect(3454, 373, 3527, 381), true, kCursorAction, 2);
	_app->objectAddRotationAccessibility(kObject12006, 12001, Common::Rect(3454, 390, 3529, 404), true, kCursorAction, 3);
	_app->objectAddRotationAccessibility(kObject12006, 12001, Common::Rect(3443, 319, 3564, 406), false, kCursorAction, 4);
	_app->objectAdd(kObject12010, "", "", 1);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddAnimationToRotation(kObject12010, 0, 12002, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12010, 0, 0, 12004);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddAnimationToRotation(kObject12010, 1, 12002, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12010, 1, 0, 12004);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddAnimationToRotation(kObject12010, 2, 12002, 3, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12010, 2, 0, 12004);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddAnimationToRotation(kObject12010, 3, 12002, 3, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12010, 3, 0, 12004);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddAnimationToRotation(kObject12010, 4, 12002, 4, 10, 12.5f, 6);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddAnimationToRotation(kObject12010, 5, 12002, 4, 10, 12.5f, 10);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddImageToRotation(kObject12010, 6, 12002, 0);
	_app->objectAddPresentation(kObject12010);
	_app->objectPresentationAddImageToRotation(kObject12010, 7, 12002, 2);
	_app->objectAddRotationAccessibility(kObject12010, 12002, Common::Rect(666, 279, 864, 352), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject12010, 12002, Common::Rect(683, 312, 736, 355), false, kCursorTake, 1);
	_app->objectAddRotationAccessibility(kObject12010, 12002, Common::Rect(646, 357, 838, 394), false, kCursorAction, 2);
	_app->objectAdd(kObject12034, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject12034, kPuzzle12010, Common::Rect(314, 375, 355, 422), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject12034, kPuzzle12010, Common::Rect(197, 359, 228, 401), true, kCursorActionObject, 1);
	_app->objectAddPresentation(kObject12034);
	_app->objectPresentationAddImageToPuzzle(kObject12034, 0, kPuzzle12010, "a01s02n03p04l01.bmp", Common::Point(298, 366), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject12034);
	_app->objectPresentationAddImageToPuzzle(kObject12034, 1, kPuzzle12010, "a01s02n03p04l02.bmp", Common::Point(177, 355), true, kDrawType1, 1000);
	_app->objectAdd(kObject12009, "", "", 1);
	_app->objectAdd(kObjectLotteryTicket, "Lottery ticket", "A01_LotteryTicket", 1);
	_app->objectSetActiveCursor(kObjectLotteryTicket, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectLotteryTicket, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject12009, 12002, Common::Rect(965, -155, 1228, 392), true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject12009, kPuzzle12008, Common::Rect(265, 63, 372, 168), false, kCursorZoom, 1);
	_app->objectAddPuzzleAccessibility(kObject12009, kPuzzle12009, Common::Rect(277, 292, 357, 331), false, kCursorTake, 2);
	_app->objectAddRotationAccessibility(kObject12009, 12003, Common::Rect(1780, -132, 1908, 238), true, kCursorMove, 3);
	_app->objectAddPresentation(kObject12009);
	_app->objectPresentationAddImageToPuzzle(kObject12009, 0, kPuzzle12009, "a01s02n03p03l01.bmp", Common::Point(176, 42), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject12009);
	_app->objectPresentationAddImageToPuzzle(kObject12009, 1, kPuzzle12009, "a01s02n03p03l02.bmp", Common::Point(252, 289), true, kDrawType1, 1000);
	_app->objectAdd(kObject12028, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject12028, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject12028, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 40; i++) {
		_app->objectAddPresentation(kObject12028);
		_app->objectPresentationAddImageToPuzzle(kObject12028, i, kPuzzle12009, Common::String::format("a01s02n03p03s01.%04d.bmp", i + 1), Common::Point(239, 157), true, kDrawType1, 1000);
	}

	_app->objectAddPuzzleAccessibility(kObject12028, kPuzzle12009, Common::Rect(207, 132, 442, 349), true, kCursorDragDrop, 0);
	_app->objectAdd(kObject12025, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject12025, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject12025, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject12026, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject12026, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject12026, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject12027, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject12027, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject12027, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject12027, kPuzzle12008, Common::Rect(113, 180, 380, 420), true, kCursorDragDrop, 0);
	_app->objectAddPuzzleAccessibility(kObject12027, kPuzzle12008, Common::Rect(386, 180, 639, 420), true, kCursorDragDrop, 1);

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject12025);
		_app->objectPresentationAddImageToPuzzle(kObject12025, i, kPuzzle12008, Common::String::format("a01s02n03p01s02.%04d.bmp", i + 1), Common::Point(100, 16), true, kDrawType1, 1000);
	}

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject12026);
		_app->objectPresentationAddImageToPuzzle(kObject12026, i, kPuzzle12008, Common::String::format("a01s02n03p01s03.%04d.bmp", i + 1), Common::Point(382, 16), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObjectTooth, "Tooth", "A01_Tooth", 1);
	_app->objectSetActiveCursor(kObjectTooth, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectTooth, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject12012, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject12012, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject12012, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject12012);
		_app->objectPresentationAddImageToPuzzle(kObject12012, i, kPuzzle12001, Common::String::format("a01s02n02p01s01.%04d.tga", i + 1), Common::Point(208, 129), true, kDrawType3, 1100);
	}

	_app->objectAddPuzzleAccessibility(kObject12012, kPuzzle12001, Common::Rect(0, 16, 640, 420), false, kCursorDragDrop, 0);
	_app->objectAdd(kObject12018, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject12018, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject12018, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject12018);
		_app->objectPresentationAddImageToPuzzle(kObject12018, i, kPuzzle12001, Common::String::format("a01s02n02p01s02.%04d.tga", i + 1), Common::Point(207, 144), true, kDrawType3, 1100);
	}

	_app->objectAddPuzzleAccessibility(kObject12018, kPuzzle12001, Common::Rect(0, 16, 640, 420), false, kCursorDragDrop, 0);
	_app->objectAdd(kObjectStrainer, "Strainer", "A01_Strainer", 1);
	_app->objectSetActiveCursor(kObjectStrainer, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectStrainer, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObjectStrainer);
	_app->objectPresentationAddImageToRotation(kObjectStrainer, 0, 12001, 2);
	_app->objectAddRotationAccessibility(kObjectStrainer, 12001, Common::Rect(3111, 261, 3218, 301), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObjectStrainer, kPuzzle12001, Common::Rect(250, 164, 391, 259), false, kCursorAction, 1);
	_app->objectAddPresentation(kObjectStrainer);
	_app->objectPresentationAddImageToPuzzle(kObjectStrainer, 1, kPuzzle12001, "a01s02n02p01s02.0015.tga", Common::Point(207, 144), true, kDrawType3, 1100);
	_app->objectAdd(kObject12030, "", "", 1);
	_app->objectAdd(kObject12031, "", "", 1);
	_app->objectAdd(kObject12032, "", "", 1);
	_app->objectAdd(kObjectButcherKnife, "Butcher knife", "A01_Knife", 1);
	_app->objectSetActiveCursor(kObjectButcherKnife, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectButcherKnife, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject12030);
	_app->objectPresentationAddAnimationToRotation(kObject12030, 0, 12004, 1, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12030, 0, 0, 12012);
	_app->objectAddPresentation(kObject12030);
	_app->objectPresentationAddAnimationToRotation(kObject12030, 1, 12004, 1, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12030, 1, 0, 12012);
	_app->objectAddPresentation(kObject12030);
	_app->objectPresentationAddImageToRotation(kObject12030, 2, 12004, 3);
	_app->objectAddPresentation(kObject12031);
	_app->objectPresentationAddAnimationToRotation(kObject12031, 0, 12004, 4, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12031, 0, 0, 12010);
	_app->objectAddPresentation(kObject12031);
	_app->objectPresentationAddAnimationToRotation(kObject12031, 1, 12004, 4, 10, 8.0f, 10);
	_app->objectAddPresentation(kObject12031);
	_app->objectPresentationAddAnimationToRotation(kObject12031, 2, 12004, 4, 3, 12.5f, 6);
	_app->objectAddPresentation(kObject12031);
	_app->objectPresentationAddAnimationToRotation(kObject12031, 3, 12004, 4, 3, 12.5f, 10);
	_app->objectAddPresentation(kObject12032);
	_app->objectPresentationAddAnimationToRotation(kObject12032, 0, 12004, 5, 9, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12032, 0, 0, 12013);
	_app->objectAddPresentation(kObject12032);
	_app->objectPresentationAddAnimationToRotation(kObject12032, 1, 12004, 5, 9, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12032, 1, 0, 12014);
	_app->objectAddPresentation(kObject12032);
	_app->objectPresentationAddAnimationToRotation(kObject12032, 2, 12004, 6, 9, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject12032, 2, 0, 12013);
	_app->objectAddPresentation(kObject12032);
	_app->objectPresentationAddAnimationToRotation(kObject12032, 3, 12004, 6, 9, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject12032, 3, 0, 12014);
	_app->objectAddPresentation(kObject12032);
	_app->objectPresentationAddImageToRotation(kObject12032, 4, 12004, 0);
	_app->objectAddPresentation(kObject12032);
	_app->objectPresentationAddImageToRotation(kObject12032, 5, 12004, 2);
	_app->objectAddRotationAccessibility(kObject12030, 12004, Common::Rect(2875, 294, 3436, 526), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject12030, 12004, Common::Rect(2958, 118, 3382, 322), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject12031, 12004, Common::Rect(3255, 80, 3350, 115), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject12032, 12004, Common::Rect(3004, 71, 3260, 129), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject12032, 12004, Common::Rect(3028, 12, 3329, 78), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObjectButcherKnife, 12004, Common::Rect(3065, 80, 3148, 115), false, kCursorTake, 0);
	_app->objectAdd(kObject12038, "", "", 1);
	_app->objectAddPresentation(kObject12038);
	_app->objectPresentationAddAnimationToRotation(kObject12038, 0, 12004, 7, 2, 15.0f, 5);
	_app->objectAddRotationAccessibility(kObject12038, 12004, Common::Rect(1246, 261, 1284, 297), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject12038, 12004, Common::Rect(1318, 255, 1355, 289), true, kCursorAction, 1);
	_app->objectAdd(kObject12039, "", "", 1);
	_app->objectAddPresentation(kObject12039);
	_app->objectPresentationAddImageToRotation(kObject12039, 0, 12003, 0);
	_app->objectAdd(kObject12040, "", "", 1);
	_app->objectAddPresentation(kObject12040);
	_app->objectPresentationAddAnimationToRotation(kObject12040, 0, 12001, 9, 60, 12.5f, 6);
	_app->objectPresentationShow(kObject12040, 0);
	_app->objectPresentationPauseAnimation(kObject12040, 0);
	_app->objectAdd(kObject12020, "", "", 1);
	_app->objectAddRotationAccessibility(kObject12020, 12003, Common::Rect(2758, 111, 2826, 160), true, kCursorZoom, 0);
	_app->objectAdd(kObject12021, "", "", 1);
	_app->objectAddRotationAccessibility(kObject12021, 12002, Common::Rect(3243, -78, 3533, 144), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject12021, 12003, Common::Rect(2675, -83, 2888, 99), true, kCursorZoom, 1);
	_app->objectAdd(kObject12022, "", "", 1);
	_app->objectAddRotationAccessibility(kObject12022, 12003, Common::Rect(1119, 216, 1743, 383), true, kCursorZoom, 0);
	_app->objectAdd(kObject12019, "", "", 1);
	_app->objectAddRotationAccessibility(kObject12019, 12002, Common::Rect(660,  205,  718, 275), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject12019, 12002, Common::Rect(903,  113,  942, 162), true, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject12019, 12003, Common::Rect(1782, 333, 1842, 427), true, kCursorZoom, 2);
	_app->objectAddRotationAccessibility(kObject12019, 12003, Common::Rect(1488, 150, 1529, 209), true, kCursorZoom, 3);
	_app->objectAdd(kObject12024, "", "", 1);
	_app->objectAddRotationAccessibility(kObject12024, 12003, Common::Rect(2891, 47, 2970, 158), true, kCursorZoom, 0);
	_app->objectAdd(kObject12011, "", "", 1);
	_app->objectAddRotationAccessibility(kObject12011, 12003, Common::Rect(2990, 296, 3086, 373), true, kCursorZoom, 0);
	_app->objectAdd(kObject12036, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject12036, kPuzzle12002, Common::Rect(222, 161, 322, 229), false, kCursorIdle, 0);
	_app->objectAddPuzzleAccessibility(kObject12036, kPuzzle12002, Common::Rect(258, 168, 477, 348), false, kCursorIdle, 1);
	_app->soundAdd(12201, kSoundTypeBackgroundMusic, "1055.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12201, 90);
	_app->soundAdd(12109, kSoundTypeEffect, "1056.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12110, kSoundTypeEffect, "1057.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12101, kSoundTypeEffect, "1058.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12102, kSoundTypeEffect, "1059.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12103, kSoundTypeEffect, "1060.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12104, kSoundTypeEffect, "1061.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12105, kSoundTypeEffect, "1062.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12106, kSoundTypeEffect, "1063.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12107, kSoundTypeEffect, "1064.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12108, kSoundTypeEffect, "1065.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12111, kSoundTypeEffect, "1066.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12112, kSoundTypeEffect, "1067.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12113, kSoundTypeEffect, "1068.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12114, kSoundTypeEffect, "1069.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12115, kSoundTypeEffect, "1070.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12116, kSoundTypeEffect, "1071.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12116, 75);
	_app->soundAdd(12117, kSoundTypeEffect, "1072.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12119, kSoundTypeEffect, "1073.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12119, 95);
	_app->soundAdd(12126, kSoundTypeEffect, "1074.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12126, 95);
	_app->soundAdd(12120, kSoundTypeEffect, "1075.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12120, 90);
	_app->soundAdd(12122, kSoundTypeEffect, "1076.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12124, kSoundTypeEffect, "1077.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12124, 95);
	_app->soundAdd(12123, kSoundTypeEffect, "1078.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12123, 95);
	_app->soundAdd(12125, kSoundTypeEffect, "1079.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12125, 90);
	_app->soundAdd(12121, kSoundTypeEffect, "1080.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12127, kSoundTypeEffect, "1081.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12128, kSoundTypeEffect, "1082.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12129, kSoundTypeEffect, "1083.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12130, kSoundTypeEffect, "1084.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12131, kSoundTypeEffect, "1085.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12132, kSoundTypeEffect, "1086.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12134, kSoundTypeEffect, "1087.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12134, 90);
	_app->soundAdd(12135, kSoundTypeEffect, "1088.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12135, 90);
	_app->soundAdd(12136, kSoundTypeEffect, "1089.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12136, 90);
	_app->soundAdd(12137, kSoundTypeEffect, "1090.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12137, 90);
	_app->soundAdd(12138, kSoundTypeEffect, "1091.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12138, 90);
	_app->soundAdd(12139, kSoundTypeEffect, "1092.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12139, 90);
	_app->soundAdd(12140, kSoundTypeEffect, "1093.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(12140, 90);
	_app->soundAdd(12141, kSoundTypeEffect, "1094.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12142, kSoundTypeEffect, "1095.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12143, kSoundTypeEffect, "1096.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12144, kSoundTypeEffect, "1097.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12145, kSoundTypeEffect, "1098.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12146, kSoundTypeEffect, "1099.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12147, kSoundTypeEffect, "1100.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12133, kSoundTypeAmbientEffect, "1101.wav", _app->getConfiguration().backgroundMusic.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(12004, 12133, 1, 1, 15, 100, 85.0f, 10);
	_app->rotationAdd3DSound(12003, 12133, 1, 1, 15, 95, 41.0f, 10);
	_app->rotationAdd3DSound(12002, 12133, 1, 1, 15, 85, 15.0f, 10);
	_app->rotationAdd3DSound(12001, 12133, 1, 1, 15, 80, 12.0f, 10);
	_app->rotationSet3DSoundOff(12001, 12133);
	_app->rotationSet3DSoundOff(12002, 12133);
	_app->rotationSet3DSoundOff(12003, 12133);
	_app->rotationSet3DSoundOff(12004, 12133);
	_app->puzzleAdd3DSound(12006, 12133, 1, 1, 15, 95, 270.0f, 10);
	_app->puzzleAdd3DSound(12007, 12133, 1, 1, 15, 95, 270.0f, 10);
	_app->puzzleAdd3DSound(12004, 12133, 1, 1, 15, 85, 180.0f, 10);
	_app->puzzleAdd3DSound(12005, 12133, 1, 1, 15, 85, 180.0f, 10);
	_app->puzzleAdd3DSound(12008, 12133, 1, 1, 15, 90, 250.0f, 10);
	_app->puzzleAdd3DSound(12009, 12133, 1, 1, 15, 90, 250.0f, 10);
	_app->puzzleAdd3DSound(12010, 12133, 1, 1, 15, 90, 250.0f, 10);
	_app->puzzleAdd3DSound(12011, 12133, 1, 1, 15, 83, 250.0f, 10);
	_app->puzzleAdd3DSound(12002, 12133, 1, 1, 15, 83, 250.0f, 10);
	_app->puzzleAdd3DSound(12001, 12133, 1, 1, 15, 80, 250.0f, 10);
	_app->puzzleSet3DSoundOff(12006, 12133);
	_app->puzzleSet3DSoundOff(12007, 12133);
	_app->puzzleSet3DSoundOff(12004, 12133);
	_app->puzzleSet3DSoundOff(12005, 12133);
	_app->puzzleSet3DSoundOff(12008, 12133);
	_app->puzzleSet3DSoundOff(12009, 12133);
	_app->puzzleSet3DSoundOff(12010, 12133);
	_app->puzzleSet3DSoundOff(12011, 12133);
	_app->puzzleSet3DSoundOff(12002, 12133);
	_app->puzzleSet3DSoundOff(12001, 12133);
	_app->soundAdd(12001, kSoundTypeDialog, "1102.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12002, kSoundTypeDialog, "1103.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12003, kSoundTypeDialog, "1104.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12004, kSoundTypeDialog, "1105.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12005, kSoundTypeDialog, "1106.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12006, kSoundTypeDialog, "1107.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12008, kSoundTypeDialog, "1109.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12009, kSoundTypeDialog, "1110.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12010, kSoundTypeDialog, "1111.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12011, kSoundTypeDialog, "1112.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12012, kSoundTypeDialog, "1113.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12013, kSoundTypeDialog, "1114.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12014, kSoundTypeDialog, "1115.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12015, kSoundTypeDialog, "1116.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12016, kSoundTypeDialog, "1117.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12017, kSoundTypeDialog, "1118.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12018, kSoundTypeDialog, "1119.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12019, kSoundTypeDialog, "1120.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12020, kSoundTypeDialog, "1121.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12022, kSoundTypeDialog, "1122.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12023, kSoundTypeDialog, "1123.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12024, kSoundTypeDialog, "1124.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12025, kSoundTypeDialog, "1125.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineByte(12001, 0);
	_app->varDefineByte(12002, -1);
	_app->varDefineByte(12003, 0);
	_app->varDefineByte(12004, 0);
	_app->varDefineByte(12005, 0);
	_app->varDefineByte(12006, 0);
	_app->varDefineByte(12007, 0);
	_app->varDefineByte(12008, 0);
	_app->varDefineByte(12009, 0);
	_app->varDefineByte(12010, 0);
	_app->varDefineByte(12011, 0);
	_app->varDefineByte(12012, 0);
	_app->varDefineByte(12013, 0);
	_app->varDefineByte(12014, 0);
	_app->varDefineByte(12015, 0);
	_app->varDefineWord(12001, 208);
	_app->varDefineWord(12002, 169);
	_app->varDefineWord(12003, 1);
	_app->varDefineWord(12004, 0);
	_app->varDefineWord(12005, 0);
	_app->varDefineWord(12006, 0);
	_app->varDefineDword(12001, 0);
	_app->varDefineFloat(12001, 0.0f);
	_app->varDefineFloat(12002, 0.0f);
	_app->varDefineFloat(12003, 0.0f);
	_app->varDefineFloat(12004, 0.0f);
	_app->varDefineFloat(12005, 1.0f);
	_app->varDefineFloat(12007, 0.0f);
	_app->varDefineString(12001, "000");
}

void Zone3Faust::onSetup(SetupType type) {
	error("[Zone3Faust::onSetup] Not implemented");
}

void Zone3Faust::onTimer(TimerId id) {
	error("[Zone3Faust::onTimer] Not implemented");
}

void Zone3Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone3Faust::onButtonUp] Not implemented");
}

void Zone3Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone3] Not implemented");
}

void Zone3Faust::onSound(Id id, SoundType type, uint32 a3, bool process) {
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

void Zone3Faust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[Zone3Faust::onBag] Not implemented");
}

void Zone3Faust::onUpdateBag(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(12001), 1024);
	_app->varSetDword(12001, 0);
}

void Zone3Faust::onBagClickedObject(ObjectId id) {
	if (id == kObjectStrainer) {
		if (_app->hasCurrentPuzzle() && _app->getCurrentPuzzleId() == kPuzzle12001) {
			_app->objectPresentationShow(kObjectStrainer, 1);
			_app->objectSetAccessibilityOn(kObjectStrainer, 1, 1);
			_app->cursorDelete();
		}
	}
}

void Zone3Faust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
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

void Zone3Faust::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	error("[Zone3Faust::onBeforeRide] Not implemented");
}

void Zone3Faust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	error("[Zone3Faust::onAfterRide] Not implemented");
}

} // End of namespace Ring
