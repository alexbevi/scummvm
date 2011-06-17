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

#include "ring/game/ring/ring_zonen2.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneN2Ring::ZoneN2Ring(ApplicationRing *application) : _app(application) {
	_presentationIndexN2 = 0;
	_distN2 = 0;
}

ZoneN2Ring::~ZoneN2Ring() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneN2Ring::onInit() {
	debugC(1, kRingDebugLogic, "Initializing Loge zone (N2)...");

	_app->setZoneAndEnableBag(kZoneN2);

	_app->rotationAdd(70000, "N2S00N01", 0, 1);
	_app->rotationAdd(70001, "N2S00N02", 0, 3);
	_app->rotationAdd(70100, "N2S01N01", 0, 1);
	_app->rotationAdd(70101, "N2S01N02", 0, 1);
	_app->rotationAdd(70300, "N2S03N01", 0, 2);
	_app->rotationAdd(70400, "N2S04N01", 0, 0);
	_app->rotationAdd(70500, "N2S05N01", 0, 2);
	_app->rotationAdd(70600, "N2S06N01", 0, 0);
	_app->rotationAdd(70200, "N2S07N01", 0, 1);
	_app->puzzleAdd(kPuzzle72001);
	_app->puzzleAddBackgroundImage(kPuzzle72001, "TR_AS_RO_BS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72002);
	_app->puzzleAddBackgroundImage(kPuzzle72002, "TR_AS_RO_CS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72003);
	_app->puzzleAddBackgroundImage(kPuzzle72003, "TR_AS_RO_DS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72004);
	_app->puzzleAddBackgroundImage(kPuzzle72004, "TR_AS_RO_ES.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72005);
	_app->puzzleAddBackgroundImage(kPuzzle72005, "TR_AS_RO_FS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72006);
	_app->puzzleAddBackgroundImage(kPuzzle72006, "TR_AS_RO_HS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72007);
	_app->puzzleAddBackgroundImage(kPuzzle72007, "TR_AS_RO_IS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72008);
	_app->puzzleAddBackgroundImage(kPuzzle72008, "TR_AS_RO_JS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72009);
	_app->puzzleAddBackgroundImage(kPuzzle72009, "TR_AS_RO_KS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle72010);
	_app->puzzleAddBackgroundImage(kPuzzle72010, "TR_AS_RO_MS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70000);
	_app->puzzleAddBackgroundImage(kPuzzle70000, "N2EXTRA_1A.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70100);
	_app->puzzleAddBackgroundImage(kPuzzle70100, "N2S01N01P02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70102);
	_app->puzzleAddBackgroundImage(kPuzzle70102, "N2S01N01P03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70300);
	_app->puzzleAddBackgroundImage(kPuzzle70300, "N2S03N01P01S01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70301);
	_app->puzzleAddBackgroundImage(kPuzzle70301, "N2S03N01P01S02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70302);
	_app->puzzleAddBackgroundImage(kPuzzle70302, "N2S03N01P01S04.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70303);
	_app->puzzleAddBackgroundImage(kPuzzle70303, "N2S02N01P02S02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70304);
	_app->puzzleAddBackgroundImage(kPuzzle70304, "N2S02N01P02S03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70305);
	_app->puzzleAddBackgroundImage(kPuzzle70305, "N2S00N01P01S01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70306);
	_app->puzzleAddBackgroundImage(kPuzzle70306, "N2S03N01P01S03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70410);
	_app->puzzleAddBackgroundImage(kPuzzle70410, "N2S04N03P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70411);
	_app->puzzleAddBackgroundImage(kPuzzle70411, "N2S04N03P03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70500);
	_app->puzzleAddBackgroundImage(kPuzzle70500, "N2S05N01P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70501);
	_app->puzzleAddBackgroundImage(kPuzzle70501, "N2S05N01P01L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70510);
	_app->puzzleAddBackgroundImage(kPuzzle70510, "N2S05N01P02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70511);
	_app->puzzleAddBackgroundImage(kPuzzle70511, "N2S05N01P02L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70520);
	_app->puzzleAddBackgroundImage(kPuzzle70520, "N2S05N01P03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70521);
	_app->puzzleAddBackgroundImage(kPuzzle70521, "N2S05N01P03L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70600);
	_app->puzzleAddBackgroundImage(kPuzzle70600, "N2S07N01P01S01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle70601);
	_app->puzzleAddBackgroundImage(kPuzzle70601, "N2S07N01P01S04.0001.bmp", Common::Point(0, 16), true);
	_app->rotationAddMovabilityToRotation(70000, 70001, "1373", Common::Rect(1585, -94, 1815, 111), true, kCursorMove, 7);
	_app->rotationSetMovabilityToRotation(70000, 0, 0, 0, 85.7f, 0, 2, 90.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(70000, 70400, "1374", Common::Rect(2515, -165, 2796, 106), true, kCursorMove, 3);
	_app->rotationSetMovabilityToRotation(70000, 1, 0, 0, 85.7f, 0, 2, 270.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(70000, 70200, "1375", Common::Rect(314, -261, 817, 167), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70000, 2, 0, 0, 85.7f, 0, 2, 185.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(70001, 70000, "1376", Common::Rect(3397, -81, 3600, 129), true, kCursorMove, 7);
	_app->rotationSetMovabilityToRotation(70001, 0, 0, 0, 85.7f, 0, 2, 270.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(70001, 70300, "1377", Common::Rect(2132, -198, 2640, 211), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70001, 1, 0, 0, 85.7f, 0, 2, 150.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(70001, 70100, "1378", Common::Rect(1008, 31, 1218, 230), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70001, 2, 0, 0, 85.7f, 0, 2, 270.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(70001, 70400, "1379", Common::Rect(3227, -76, 3364, 61), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70001, 3, 0, 0, 85.7f, 0, 2, 270.0f, 0, 85.7f);
	_app->rotationAddMovabilityToRotation(70100, 70101, "1380", Common::Rect(1186, -143, 1534, 195), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70100, 0, 0, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(70101, 70100, "1381", Common::Rect(2951, -125, 3350, 193), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70101, 0, 0, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(70101, 70600, "1382", Common::Rect(1323, -57, 1587, 225), true, kCursorMove, 16);
	_app->rotationSetMovabilityToRotation(70101, 1, 0, 0.3f, 85.7f, 0, 2, 130.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(70101, 70600, "1383", Common::Rect(1323, -57, 1587, 225), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70101, 2, 0, 0.3f, 85.7f, 0, 2, 224.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(70200, 70000, "1384", Common::Rect(108, -160, 578, 156), true, kCursorMove, 7);
	_app->rotationSetMovabilityToRotation(70200, 0, 0, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(70300, 70001, "1385", Common::Rect(318, -202, 789, 226), true, kCursorMove, 7);
	_app->rotationSetMovabilityToRotation(70300, 0, 0, 0.3f, 85.7f, 0, 2, 320.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(70400, 70000, "1386", Common::Rect(1200, -127, 1508, 143), true, kCursorMove, 7);
	_app->rotationSetMovabilityToRotation(70400, 0, 0, 0, 85.7f, 0, 2, 90.0f, 0, 85.7f);
	_app->rotationAddMovabilityToPuzzle(70400, kPuzzle70410, "", Common::Rect(3011, -211, 3280, 29), true, kCursorMove, 0);
	_app->rotationSetMovabilityToPuzzle(70400, 1, 0, 0, 85.7f, 0, 2);
	_app->rotationAddMovabilityToRotation(70500, 70600, "1387", Common::Rect(546, -345, 980, 120), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70500, 0, 0, 0.3f, 85.7f, 0, 2, 44.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToPuzzle(70500, kPuzzle70500, "", Common::Rect(1139, -204, 1520, 156), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(70500, 1, 0, 0.3f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(70500, kPuzzle70510, "", Common::Rect(1787, -143, 2033, 108), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(70500, 2, 0, 0.3f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(70500, kPuzzle70520, "", Common::Rect(2228, -118, 2411, 80), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(70500, 3, 0, 0.3f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToRotation(70600, 70101, "1388", Common::Rect(863, -211, 1123, 101), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(70600, 0, 0, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70102, kPuzzle70100, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70410, kPuzzle70411, "", Common::Rect(250, 280, 320, 370), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle70410, 70400, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle70410, 1, 270.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70411, kPuzzle70410, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle70500, 70500, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle70500, 0, 90.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70500, kPuzzle70501, "", Common::Rect(180, 80, 470, 370), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70501, kPuzzle70500, "", Common::Rect(430, 48, 640, 434), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle70510, 70500, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle70510, 0, 144.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70510, kPuzzle70511, "", Common::Rect(180, 80, 470, 370), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70511, kPuzzle70510, "", Common::Rect(430, 48, 640, 434), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle70520, 70500, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle70520, 0, 184.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70520, kPuzzle70521, "", Common::Rect(180, 80, 470, 370), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle70521, kPuzzle70520, "", Common::Rect(430, 48, 640, 434), true, kCursorHotspot, 0);
	_app->objectAdd(kObject72000, "", "", 1);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 0, kPuzzle72001, "TR_AS_RO_BS", kImageTypeBMP, Common::Point(375, 203), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 1, kPuzzle72002, "TR_AS_RO_CS", kImageTypeBMP, Common::Point(281, 154), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 2, kPuzzle72003, "TR_AS_RO_DS", kImageTypeBMP, Common::Point(284, 163), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 3, kPuzzle72004, "TR_AS_RO_ES", kImageTypeBMP, Common::Point(147, 140), kDrawType1, 1000, 23, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 4, kPuzzle72005, "TR_AS_RO_FS", kImageTypeBMP, Common::Point(388, 244), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 5, kPuzzle72006, "TR_AS_RO_HS", kImageTypeBMP, Common::Point(146, 156), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 6, kPuzzle72007, "TR_AS_RO_IS", kImageTypeBMP, Common::Point(308, 221), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 7, kPuzzle72008, "TR_AS_RO_JS", kImageTypeBMP, Common::Point(250, 102), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 8, kPuzzle72009, "TR_AS_RO_KS", kImageTypeBMP, Common::Point(224, 262), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject72000);
	_app->objectPresentationAddAnimationToPuzzle(kObject72000, 9, kPuzzle72010, "TR_AS_RO_MS", kImageTypeBMP, Common::Point(173, 127), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject70001, "Alberich", "", 1);
	_app->objectAddPresentation(kObject70001);
	_app->objectPresentationAddAnimationToPuzzle(kObject70001, 0, kPuzzle70000, "N2EXTRA_1B", kImageTypeBMP, Common::Point(1, 153), kDrawType1, 1000, 53, 10.0f, 4);
	_app->objectAddPresentation(kObject70001);
	_app->objectPresentationAddAnimationToPuzzle(kObject70001, 1, kPuzzle70000, "N2EXTRA_2B", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 37, 10.0f, 4);
	_app->objectAddPresentation(kObject70001);
	_app->objectPresentationAddAnimationToPuzzle(kObject70001, 2, kPuzzle70000, "N2EXTRA_3B", kImageTypeBMP, Common::Point(49, 38), kDrawType1, 1000, 53, 10.0f, 4);
	_app->objectAddPresentation(kObject70001);
	_app->objectPresentationAddAnimationToRotation(kObject70001, 3, 70001, 2, 16, 12.5f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject70001, 3, 0, 70000);
	_app->objectPresentationShow(kObject70001, 3);
	_app->objectAdd(kObjectFire, "Fire", "N2_Fire", 1);
	_app->objectAddBagAnimation(kObjectFire, kImageTypeTGA, kDrawType3, 13, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFire, Common::Point(22, 22), 13, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFire, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectFire, 70600, Common::Rect(2413, -256, 2970, 256), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectFire);
	_app->objectPresentationAddAnimationToRotation(kObjectFire, 0, 70000, 0, 25, 10.0f, 4);
	_app->objectPresentationAddAnimationToRotation(kObjectFire, 0, 70001, 0, 25, 10.0f, 4);
	_app->objectPresentationAddAnimationToRotation(kObjectFire, 0, 70001, 1, 25, 10.0f, 4);
	_app->objectPresentationShow(kObjectFire, 0);
	_app->objectAdd(kObject70100, "Console", "", 1);
	_app->objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(  0,  16, 640,  66), true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(  0, 414, 640, 464), true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(  0,  66,  50, 414), true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(610,  66, 640, 414), true, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject70100, 70100, Common::Rect(3406, 26, 3541, 155), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject70100);
	_app->objectPresentationAddImageToPuzzle(kObject70100, 0, kPuzzle70100, "N2S01N01P01L01.0001.bmp", Common::Point(101, 68), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject70100);
	_app->objectPresentationAddImageToPuzzle(kObject70100, 1, kPuzzle70100, "N2S01N01P01L02.0001.bmp", Common::Point(80, 63), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject70100);
	_app->objectPresentationAddImageToPuzzle(kObject70100, 2, kPuzzle70100, "N2S01N01P01L03.0001.bmp", Common::Point(116, 159), true, kDrawType1, 1000);
	_app->objectAdd(kObject70101, "CCButton1", "", 2);
	_app->objectAddRotationAccessibility(kObject70101, 70100, Common::Rect(3208, 109, 3322, 184), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject70101, kPuzzle70100, Common::Rect(359, 265, 440, 304), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject70101, kPuzzle70100, Common::Rect(359, 265, 440, 304), false, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject70101);
	_app->objectPresentationAddImageToPuzzle(kObject70101, 0, kPuzzle70100, "N2S01N01P01S03.0001.bmp", Common::Point(361, 259), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject70101);
	_app->objectPresentationAddAnimationToPuzzle(kObject70101, 1, kPuzzle70100, "N2S01N01P01S01", kImageTypeBMP, Common::Point(85, 68), kDrawType1, 1000, 36, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70101, 1, 0, 70101);
	_app->objectAddPresentation(kObject70101);
	_app->objectPresentationAddAnimationToPuzzle(kObject70101, 2, kPuzzle70100, "N2S01N01P01L02S01", kImageTypeBMP, Common::Point(80, 63), kDrawType1, 1000, 36, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70101, 2, 0, 70103);
	_app->objectAddPresentation(kObject70101);
	_app->objectPresentationAddAnimationToPuzzle(kObject70101, 3, kPuzzle70100, "N2S01N01P01S01", kImageTypeBMP, Common::Point(85, 68), kDrawType1, 1000, 36, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70101, 3, 0, 70100);
	_app->objectAddPresentation(kObject70101);
	_app->objectPresentationAddAnimationToPuzzle(kObject70101, 4, kPuzzle70100, "N2S01N01P01L02S01", kImageTypeBMP, Common::Point(80, 63), kDrawType1, 1000, 36, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70101, 4, 0, 70102);
	_app->objectAddPresentation(kObject70101);
	_app->objectPresentationAddImageToPuzzle(kObject70101, 5, kPuzzle70100, "N2S01N01P01S03.0003.bmp", Common::Point(361, 259), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject70101);
	_app->objectPresentationAddImageToRotation(kObject70101, 6, 70100, 0);
	_app->objectPresentationAddImageToRotation(kObject70101, 6, 70101, 0);
	_app->objectPresentationShow(kObject70101, 6);
	_app->objectAdd(kObject70102, "CCButton2", "", 2);
	_app->objectAddRotationAccessibility(kObject70102, 70100, Common::Rect(3250, 195, 3357, 245), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject70102, kPuzzle70100, Common::Rect(382, 308, 472, 352), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject70102, kPuzzle70100, Common::Rect(382, 308, 472, 352), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject70102);
	_app->objectPresentationAddImageToPuzzle(kObject70102, 0, kPuzzle70100, "N2S01N01P01S03.0004.bmp", Common::Point(385, 299), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject70102);
	_app->objectPresentationAddAnimationToPuzzle(kObject70102, 1, kPuzzle70100, "N2S01N01P01S02", kImageTypeBMP, Common::Point(40, 65), kDrawType1, 1000, 46, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70102, 1, 0, 70105);
	_app->objectAddPresentation(kObject70102);
	_app->objectPresentationAddAnimationToPuzzle(kObject70102, 2, kPuzzle70100, "N2S01N01P01S02", kImageTypeBMP, Common::Point(40, 65), kDrawType1, 1000, 23, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70102, 2, 0, 70104);
	_app->objectAddPresentation(kObject70102);
	_app->objectPresentationAddAnimationToPuzzle(kObject70102, 3, kPuzzle70100, "N2S01N01P01S02", kImageTypeBMP, Common::Point(40, 65), kDrawType1, 1000, 23, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70102, 3, 0, 70106);
	_app->objectAddPresentation(kObject70102);
	_app->objectPresentationAddImageToPuzzle(kObject70102, 4, kPuzzle70100, "N2S01N01P01S03.0002.bmp", Common::Point(385, 299), true, kDrawType1, 1000);
	_app->objectAdd(kObject70103, "CCHandle", "NI_HandSel", 4);
	_app->objectAddRotationAccessibility(kObject70103, 70100, Common::Rect(3406, 26, 3541, 155), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject70103, kPuzzle70100, Common::Rect(497, 192, 543, 249), true, kCursorHandSelection, 1);
	_app->objectSetActiveDrawCursor(kObject70103, Common::Point(5, 5), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject70103, Common::Point(5, 5), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 14; i++) {
		_app->objectAddPresentation(kObject70103);
		_app->objectPresentationAddImageToPuzzle(kObject70103, i, kPuzzle70100, Common::String::format("N2S01N01P02S01.%04d.bmp", i + 1), Common::Point(504, 194), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject70105, "CCHolo", "", 1);
	_app->objectAddPuzzleAccessibility(kObject70105, kPuzzle70100, Common::Rect( 77, 132, 340, 298), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject70105, kPuzzle70100, Common::Rect(140, 170, 430, 380), false, kCursorHotspot, 0);
	_app->objectAdd(kObject70106, "CCHoloCross", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject70106, kPuzzle70102, Common::Rect(203, 236, 283, 316), true, kCursorHandSelection, 0);
	_app->objectSetActiveDrawCursor(kObject70106, Common::Point(5, 5), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject70106, Common::Point(5, 5), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 20; i++) {
		_app->objectAddPresentation(kObject70106);
		_app->objectPresentationAddImageToPuzzle(kObject70106, i, kPuzzle70102, Common::String::format("N2S01N01P03L01S01.%04d.bmp", i + 1), Common::Point(213, 254), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject70107, "CCHoloDam1", "NI_HandSel", 4);
	_app->objectAdd(kObject70108, "CCHoloDam2", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject70108, kPuzzle70102, Common::Rect(389, 270, 434, 330), true,  kCursorHandSelection, 0);
	_app->objectAddPuzzleAccessibility(kObject70108, kPuzzle70102, Common::Rect(389, 330, 434, 390), false, kCursorHandSelection, 1);
	_app->objectSetActiveDrawCursor(kObject70108, Common::Point(5, 5), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject70108, Common::Point(5, 5), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject70108);
		_app->objectPresentationAddImageToPuzzle(kObject70108, i, kPuzzle70102, Common::String::format("N2S01N01P03L01S03.%04d.bmp", i + 1), Common::Point(390, 271), true, kDrawType1, 999);
	}

	_app->objectAddPresentation(kObject70108);
	_app->objectPresentationAddImageToPuzzle(kObject70108, 14, kPuzzle70102, "N2S01N01P03L01S03A.bmp", Common::Point(340, 237), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToPuzzle(kObject70108, 14, kPuzzle70102, "N2S01N01P03L01S03B.bmp", Common::Point(311, 179), true, kDrawType1, 1000);
	_app->objectAdd(kObjectLogeTear3, "Loge's Tear", "NI_Tear", 1);
	_app->objectAddBagAnimation(kObjectLogeTear3, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectLogeTear3, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectLogeTear3, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObject70300, "Mime", "", 1);
	_app->objectAddRotationAccessibility(kObject70300, 70300, Common::Rect(1937, -26, 2079, 88), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject70300);
	_app->objectPresentationAddAnimationToRotation(kObject70300, 0, 70300, 0, 25, 10.0f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject70300, 0, 0, 70300);
	_app->objectPresentationShow(kObject70300, 0);
	_app->objectAddPresentation(kObject70300);
	_app->objectPresentationAddAnimationToPuzzle(kObject70300, 1, kPuzzle70300, "N2S03N01P01S01", kImageTypeBMP, Common::Point(260, 73), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject70300);
	_app->objectPresentationAddAnimationToPuzzle(kObject70300, 2, kPuzzle70301, "N2S03N01P01S02", kImageTypeBMP, Common::Point(289, 79), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject70300);
	_app->objectPresentationAddAnimationToPuzzle(kObject70300, 3, kPuzzle70302, "N2S03N01P01S04", kImageTypeBMP, Common::Point(305, 117), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject70300);
	_app->objectPresentationAddAnimationToPuzzle(kObject70300, 4, kPuzzle70303, "N2S02N01P02S02", kImageTypeBMP, Common::Point(311, 166), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject70300);
	_app->objectPresentationAddAnimationToPuzzle(kObject70300, 5, kPuzzle70304, "N2S02N01P02S03", kImageTypeBMP, Common::Point(97, 185), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject70300);
	_app->objectAddPresentation(kObject70300);
	_app->objectPresentationAddAnimationToPuzzle(kObject70300, 7, kPuzzle70306, "N2S03N01P01S03", kImageTypeBMP, Common::Point(270, 135), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectCage, "Cage", "N2_Cage", 1);
	_app->objectAddBagAnimation(kObjectCage, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectCage, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectCage, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObjectCage);
	_app->objectPresentationAddImageToRotation(kObjectCage, 0, 70300, 1);
	_app->objectAdd(kObject70404, "TearCasing", "", 3);
	_app->objectAddPuzzleAccessibility(kObject70404, kPuzzle70411, Common::Rect(189, 189, 362, 377), true,  kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject70404, kPuzzle70411, Common::Rect(224,  90, 362, 209), false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject70404, kPuzzle70411, Common::Rect(293, 241, 475, 462), false, kCursorHotspot, 2);
	_app->objectAddPresentation(kObject70404);
	_app->objectPresentationAddImageToPuzzle(kObject70404, 0, kPuzzle70411, "N2S04N03P03L01.0001.bmp", Common::Point(214, 173), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject70404);
	_app->objectPresentationAddImageToPuzzle(kObject70404, 1, kPuzzle70411, "N2S04N03P03L02.0001.bmp", Common::Point(270, 116), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject70404);
	_app->objectPresentationAddAnimationToPuzzle(kObject70404, 2, kPuzzle70411, "N2S04N03P03L01S01", kImageTypeBMP, Common::Point(217, 171), kDrawType1, 1000, 27, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70404, 2, 0, 70422);
	_app->objectAddPresentation(kObject70404);
	_app->objectPresentationAddAnimationToPuzzle(kObject70404, 3, kPuzzle70411, "N2S04N03P03L01S01", kImageTypeBMP, Common::Point(217, 171), kDrawType1, 1000, 27, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70404, 3, 0, 70423);
	_app->objectAddPresentation(kObject70404);
	_app->objectPresentationAddAnimationToPuzzle(kObject70404, 4, kPuzzle70411, "N2S04N03P03L02S01", kImageTypeBMP, Common::Point(215, 111), kDrawType1, 1000, 26, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70404, 4, 0, 70424);
	_app->objectAddPresentation(kObject70404);
	_app->objectPresentationAddAnimationToPuzzle(kObject70404, 5, kPuzzle70411, "N2S04N03P03L02S01", kImageTypeBMP, Common::Point(215, 111), kDrawType1, 1000, 26, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject70404, 5, 0, 70425);
	_app->objectAddPresentation(kObject70404);
	_app->objectPresentationAddAnimationToPuzzle(kObject70404, 6, kPuzzle70411, "N2S04N03P03L02S02", kImageTypeBMP, Common::Point(275, 126), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject70406, "", "", 1);
	_app->objectAddPresentation(kObject70406);
	_app->objectPresentationAddAnimationToPuzzle(kObject70406, 0, kPuzzle70410, "N2S04N03P01S02", kImageTypeBMP, Common::Point(171, 285), kDrawType1, 1000, 13, 10.0f, 32);
	_app->objectPresentationAddAnimationToPuzzle(kObject70406, 0, kPuzzle70410, "N2S04N03P01S03", kImageTypeBMP, Common::Point(244, 268), kDrawType1, 1000, 12, 10.0f, 32);
	_app->objectPresentationAddAnimationToPuzzle(kObject70406, 0, kPuzzle70410, "N2S04N03P01S04", kImageTypeBMP, Common::Point(317, 261), kDrawType1, 1000, 7, 10.0f, 32);
	_app->objectPresentationShow(kObject70406, 0);
	_app->objectAdd(kObject70405, "Temperat", "", 1);
	_app->objectAddPresentation(kObject70405);

	for (uint32 i = 0; i < 13; i++) {
		_app->objectAddPresentation(kObject70405);
		_app->objectPresentationAddImageToPuzzle(kObject70405, i, kPuzzle70410, Common::String::format("N2S04N03P01S01.%04d.bmp", i + 1), Common::Point(107, 155), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObjectCentaur, "Centaur", "N2_Centaur", 1);
	_app->objectAddBagAnimation(kObjectCentaur, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectCentaur, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectCentaur, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectCentaur, kPuzzle70501, Common::Rect(270, 250, 364, 363), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectCentaur, kPuzzle70000, Common::Rect(320,  16, 640, 464), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObjectCentaur);
	_app->objectPresentationAddImageToPuzzle(kObjectCentaur, 0, kPuzzle70501, "N2S05N01P01L02.0001.bmp", Common::Point(278, 258), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObjectCentaur);
	_app->objectAdd(kObjectDragon, "Dragon", "N2_Dragon", 1);
	_app->objectAddBagAnimation(kObjectDragon, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectDragon, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectDragon, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectDragon, kPuzzle70511, Common::Rect(290, 260, 380, 371), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectDragon, kPuzzle70000, Common::Rect(320,  16, 640, 464), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObjectDragon);
	_app->objectPresentationAddImageToPuzzle(kObjectDragon, 0, kPuzzle70511, "N2S05N01P02L02.0001.bmp", Common::Point(291, 263), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObjectDragon);
	_app->objectAdd(kObjectPhoenix1, "Phoenix", "N2_Phoenix", 1);
	_app->objectAddBagAnimation(kObjectPhoenix1, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectPhoenix1, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectPhoenix1, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectPhoenix1, kPuzzle70521, Common::Rect(265, 255, 356, 358), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectPhoenix1, kPuzzle70000, Common::Rect(320,  16, 640, 464), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObjectPhoenix1);
	_app->objectPresentationAddImageToPuzzle(kObjectPhoenix1, 0, kPuzzle70521, "N2S05N01P03L02.0001.bmp", Common::Point(268, 255), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObjectPhoenix1);
	_app->objectAdd(kObjectChrysoberyl, "Chrysoberyl", "N2_Beryllium", 1);
	_app->objectAddBagAnimation(kObjectChrysoberyl, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectChrysoberyl, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectChrysoberyl, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectChrysoberyl, 70500, Common::Rect(3483, -22, 3600, 155), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObjectChrysoberyl, 70500, Common::Rect(0, -22, 103, 155), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectChrysoberyl);
	_app->objectPresentationAddImageToRotation(kObjectChrysoberyl, 0, 70500, 0);
	_app->objectPresentationAddImageToRotation(kObjectChrysoberyl, 0, 70500, 1);
	_app->objectAdd(kObject70700, "Nib", "", 1);
	_app->objectAddRotationAccessibility(kObject70700, 70200, Common::Rect(3188, -62, 3331, 123), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject70700, 70200, Common::Rect(3188, -62, 3331, 123), false, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject70700);
	_app->objectPresentationAddAnimationToRotation(kObject70700, 0, 70200, 0, 30, 12.5f, 4);
	_app->objectAddPresentation(kObject70700);
	_app->objectPresentationAddAnimationToPuzzle(kObject70700, 1, kPuzzle70600, "N2S07N01P01S01", kImageTypeBMP, Common::Point(93, 16), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject70700);
	_app->objectPresentationAddAnimationToPuzzle(kObject70700, 2, kPuzzle70601, "N2S07N01P01S04", kImageTypeBMP, Common::Point(286, 160), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectPresentationShow(kObject70700, 0);
	_app->soundAdd(70000, kSoundTypeBackgroundMusic, "1392.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(70101, kSoundTypeAmbientEffect, "1394.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70300, kSoundTypeAmbientEffect, "1395.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70301, kSoundTypeAmbientEffect, "1396.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(70412, kSoundTypeAmbientEffect, "1397.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70501, kSoundTypeAmbientEffect, "1399.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70502, kSoundTypeAmbientEffect, "1400.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70102, kSoundTypeAmbientEffect, "1401.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70103, kSoundTypeAmbientEffect, "1402.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70104, kSoundTypeAmbientEffect, "1403.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70105, kSoundTypeAmbientEffect, "1404.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70107, kSoundTypeAmbientEffect, "1405.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70701, kSoundTypeAmbientEffect, "1406.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(70405, kSoundTypeEffect, "1412.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(70406, kSoundTypeEffect, "1413.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(70407, kSoundTypeEffect, "1414.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(70408, kSoundTypeEffect, "1415.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().effect.soundChunck);
	_app->soundAdd(72001, kSoundTypeDialog, "1417.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72002, kSoundTypeDialog, "1418.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72003, kSoundTypeDialog, "1419.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72004, kSoundTypeDialog, "1420.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72005, kSoundTypeDialog, "1421.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72006, kSoundTypeDialog, "1422.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72007, kSoundTypeDialog, "1423.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72008, kSoundTypeDialog, "1424.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72009, kSoundTypeDialog, "1425.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(72010, kSoundTypeDialog, "1426.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70001, kSoundTypeDialog, "1427.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70002, kSoundTypeDialog, "1428.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70003, kSoundTypeDialog, "1429.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70004, kSoundTypeDialog, "1430.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70005, kSoundTypeDialog, "1431.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70006, kSoundTypeDialog, "1432.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70007, kSoundTypeDialog, "1433.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70008, kSoundTypeDialog, "1434.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70009, kSoundTypeDialog, "1435.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70010, kSoundTypeDialog, "1436.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70011, kSoundTypeDialog, "1437.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70012, kSoundTypeDialog, "1438.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70013, kSoundTypeDialog, "1439.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70014, kSoundTypeDialog, "1440.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70015, kSoundTypeDialog, "1441.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70016, kSoundTypeDialog, "1442.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70017, kSoundTypeDialog, "1443.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70018, kSoundTypeDialog, "1444.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70019, kSoundTypeDialog, "1445.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70020, kSoundTypeDialog, "1446.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70021, kSoundTypeDialog, "1447.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70022, kSoundTypeDialog, "1448.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70023, kSoundTypeDialog, "1449.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70024, kSoundTypeDialog, "1450.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70025, kSoundTypeDialog, "1451.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70026, kSoundTypeDialog, "1452.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70027, kSoundTypeDialog, "1453.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71001, kSoundTypeDialog, "1454.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71010, kSoundTypeDialog, "1455.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71002, kSoundTypeDialog, "1456.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71011, kSoundTypeDialog, "1457.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71003, kSoundTypeDialog, "1458.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71012, kSoundTypeDialog, "1459.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71100, kSoundTypeDialog, "1460.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71101, kSoundTypeDialog, "1461.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71102, kSoundTypeDialog, "1462.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71111, kSoundTypeDialog, "1463.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71110, kSoundTypeDialog, "1464.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71112, kSoundTypeDialog, "1465.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71122, kSoundTypeDialog, "1466.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71120, kSoundTypeDialog, "1467.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(71121, kSoundTypeDialog, "1468.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70043, kSoundTypeDialog, "1469.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70044, kSoundTypeDialog, "1470.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70045, kSoundTypeDialog, "1471.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70046, kSoundTypeDialog, "1472.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70047, kSoundTypeDialog, "1473.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70048, kSoundTypeDialog, "1474.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70049, kSoundTypeDialog, "1475.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70050, kSoundTypeDialog, "1476.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70051, kSoundTypeDialog, "1477.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70052, kSoundTypeDialog, "1478.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70053, kSoundTypeDialog, "1479.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70054, kSoundTypeDialog, "1480.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70055, kSoundTypeDialog, "1481.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70056, kSoundTypeDialog, "1482.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70057, kSoundTypeDialog, "1483.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70058, kSoundTypeDialog, "1484.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70059, kSoundTypeDialog, "1485.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70060, kSoundTypeDialog, "1486.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70061, kSoundTypeDialog, "1487.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70062, kSoundTypeDialog, "1488.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70063, kSoundTypeDialog, "1489.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70064, kSoundTypeDialog, "1490.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70065, kSoundTypeDialog, "1491.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(70066, kSoundTypeDialog, "1492.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->rotationAdd3DSound(70300, 70301, 1, 1, 20, 100, 160.0f, 20);
	_app->rotationSet3DSoundOff(70300, 70301);
	_app->rotationAdd3DSound(70300, 70300, 1, 1, 20, 100, 160.0f, 5);
	_app->rotationAdd3DSound(70400, 70412, 1, 1, 20, 90, 270.0f, 10);
	_app->puzzleAdd3DSound(kPuzzle70410, 70412, 1, 1, 2, 93, 300.0f, 5);
	_app->puzzleAdd3DSound(kPuzzle70411, 70412, 1, 1, 2, 95, 300.0f, 5);
	_app->rotationAdd3DSound(70000, 70107, 1, 1, 50, 100, 135.0f, 8);
	_app->rotationAdd3DSound(70001, 70107, 1, 1, 50, 100, 315.0f, 8);
	_app->rotationAdd3DSound(70400, 70107, 1, 1, 10, 93, 90.0f, 20);
	_app->rotationAdd3DSound(70100, 70107, 1, 1, 10, 93, 270.0f, 5);
	_app->rotationAdd3DSound(70101, 70107, 1, 1, 10, 88, 270.0f, 5);
	_app->puzzleAdd3DSound(kPuzzle70100, 70107, 1, 1, 2, 93, 27.0f, 5);
	_app->puzzleAdd3DSound(kPuzzle70102, 70107, 1, 1, 2, 93, 27.0f, 5);
	_app->rotationAdd3DSound(70200, 70701, 1, 1, 20, 100, 285.0f, 5);
	_app->varDefineByte(70001, 1);
	_app->varDefineByte(70005, 1);
	_app->varDefineByte(70010, 0);
	_app->varDefineByte(70011, 0);
	_app->varDefineByte(70012, 0);
	_app->varDefineByte(70013, 0);
	_app->varDefineByte(70014, 1);
	_app->varDefineByte(70015, 97);
	_app->varDefineWord(70016, 0);
	_app->varDefineString(70099, "3231323131 76766     34        020212121212        776000    ");
}

void ZoneN2Ring::onSetup(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->bagAdd(kObjectFire);
		_app->playMovie("1507");
		_app->playMovie("1508");
		_app->puzzleSetActive(kPuzzle72001);
		_app->objectPresentationHide(kObject72000);
		_app->cursorSelect(kCursorDefault);
		_app->soundPlay(72001);
		break;

	case kSetupType10:
		_app->onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->bagAdd(kObjectFire);
		_app->soundPlay(70000, kSoundLoop);
		_app->rotationSetActive(70400);
		break;
	}
}

void ZoneN2Ring::onSwitch(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90010))
			_app->setupZone((ZoneId)_app->varGetDword(90014), kSetupType10);
		else
			_app->setupZone(kZoneN2, kSetupTypeNone);
	}
}

void ZoneN2Ring::onTimer(TimerId id) {
	if (id == kTimer0)
		_app->soundPlay(rnd(12) + 70004);
}

void ZoneN2Ring::onButtonDown(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject70101:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectPresentationShow(kObject70101, 4);
			_app->objectPresentationHide(kObject70100);
			break;

		case 1:
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70102);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70100);
			_app->objectSetAccessibilityOff(kObject70103);
			_app->objectPresentationShow(kObject70101, 5);
			_app->objectPresentationShow(kObject70100, 1);
			_app->objectPresentationShow(kObject70101, 2);
			break;
		}
		break;

	case kObject70102:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70105);
			_app->objectPresentationShow(kObject70102, 3);
			_app->objectPresentationHide(kObject70100);
			break;

		case 1:
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70102);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70100);
			_app->objectSetAccessibilityOff(kObject70103);
			_app->objectPresentationShow(kObject70102, 0);
			_app->objectPresentationShow(kObject70100, 2);
			_app->objectPresentationShow(kObject70102, 2);
			break;
		}
		break;

	case kObject70404:
		if (target == 2) {
			_app->objectSetAccessibilityOff(kObject70404, 1, 2);
			_app->objectPresentationShow(kObject70404, _app->varGetByte(70001) ? 5 : 3);
			_app->objectPresentationHide(kObject70404, 0);
			_app->objectPresentationHide(kObject70404, 1);

		}
		break;
	}
}

void ZoneN2Ring::onButtonUp(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectFire:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectFire) {
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 2.0f);
				_app->playMovie("1389");
				_app->rotationSetAlp(70500, 243.0f);
				_app->rotationSetRan(70500, 85.7f);
				_app->rotationSetActive(70500);
			}

			_app->cursorDelete();
		}
		break;

	case kObject70100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);

			g_system->warpMouse(505, 205);
		}
		break;

	case kObject70105:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectPresentationShow(kObject70106, (uint16)_app->varGetWord(70016));
		_app->puzzleSetActive(kPuzzle70102);

		break;

	case kObject70300:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectChrysoberyl) {
				_app->bagRemove(kObjectChrysoberyl);
				_app->playMovie("1495");
				_app->objectPresentationShow(kObjectCage);
				_app->bagAdd(kObjectCage);
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
			}

			_app->cursorDelete();
		}
		break;

	case kObject70404:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(70005, 0);

		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject70404);
			_app->puzzleSetMovabilityOff(kPuzzle70411);
			_app->objectSetAccessibilityOff(kObject70404, 0, 0);

			if (_app->varGetByte(70001)) {
				_app->objectPresentationShow(kObject70404, 4);
				_app->objectPresentationShow(kObject70404, 1);
			} else {
				_app->objectPresentationShow(kObject70404, 2);
			}

			_app->objectPresentationShow(kObject70404, 0);
			break;

		case 1:
			if (_app->varGetByte(70001) == 1) {
				_app->objectPresentationHide(kObject70404, 1);
				_app->bagAdd(kObjectLogeTear3);
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
				_app->objectPresentationHide(kObject70404, 6);
				_app->varSetByte(70001, 0);
			}
			break;
		}
		break;

	case kObjectCentaur:
	case kObjectDragon:
	case kObjectPhoenix1:
	case kObjectChrysoberyl:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				if (_app->bagGetClickedObject() == kObjectCentaur
				 || _app->bagGetClickedObject() == kObjectDragon
				 || _app->bagGetClickedObject() == kObjectPhoenix1) {
					_app->bagRemove(_app->bagGetClickedObject());
					_app->varSetByte(70015, (byte)(_app->bagGetClickedObject() - 100));
					_app->objectSetAccessibilityOff(kObjectCentaur);
					_app->objectSetAccessibilityOff(kObjectDragon);
					_app->objectSetAccessibilityOff(kObjectPhoenix1);
					_app->soundPlay(_app->varGetByte(70015) + 71010);
				}
			}

			_app->cursorDelete();

		} else {
			if (!target) {
				if (id == kObjectChrysoberyl) {
					_app->objectPresentationShow(kObjectChrysoberyl);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
				} else {
					_app->objectPresentationHide(id);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 3.0f);
				}

				_app->objectSetAccessibilityOff(id);
				_app->bagAdd(id);
			}
		}
		break;

	case kObject70700:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectFire) {
				if (target == 1) {
					_app->objectSetAccessibilityOff(id);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
					_app->objectPresentationHide(id);
					_app->soundStop(70701, 1024);
					_app->rotationSet3DSoundOff(70200, 70701);
					_app->playMovie("1494");
				}
			}

			_app->cursorDelete();

		} else {
			if (!target) {
				_app->varSetByte(70013, 31);
				_app->puzzleSetActive(kPuzzle70600);
				_app->soundPlay(70043);
			}
		}
		break;
	}
}

void ZoneN2Ring::onAnimationNextFrame(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId) {
	default:
		break;

	case 70300:
		if (frame == 3)
			_app->soundPlay(70301);
		break;

	case 70102:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundPlay(70103);
			break;

		case 36:
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70101, 4);
			_app->objectPresentationHide(kObject70101, 0);
			_app->objectPresentationHide(kObject70101, 5);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);
			_app->objectSetAccessibilityOn(kObject70103);
			break;
		}
		break;

	case 70103:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject70101, 1, 1);
			_app->objectPresentationShow(kObject70100, 1);
			break;

		case 36:
			_app->soundPlay(70102);
			break;
		}
		break;

	case 70104:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundPlay(70104);
			break;

		case 15:
			_app->soundStop(70104, 1024);
			_app->objectSetAccessibilityOn(kObject70102, 1, 1);
			_app->objectSetAccessibilityOn(kObject70105, 0, 0);
			break;
		}
		break;

	case 70106:
		switch (frame) {
		default:
			break;

		case 1:
			_app->soundStop(70105, 1024);
			_app->objectPresentationHide(kObject70101, 0);
			_app->objectPresentationHide(kObject70102, 0);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);
			_app->objectSetAccessibilityOn(kObject70103);
			break;

		case 15:
			_app->soundPlay(70105);
			break;
		}
		break;

	case 70422:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject70404, 1, 2);
			break;

		case 2:
			_app->soundStop(70407, 1024);
			break;

		case 14:
			_app->soundPlay(70407);
			break;

		case 15:
			_app->soundStop(70405, 1024);
			break;

		case 26:
			_app->soundPlay(70405);
			break;
		}
		break;

	case 70423:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(70408);
			break;

		case 15:
			_app->soundPlay(70406);
			break;

		case 26:
			_app->soundStop(70406, 1024);
			_app->soundStop(70408, 1024);
			break;

		case 27:
			_app->puzzleSetMovabilityOn(70411);
			_app->objectSetAccessibilityOn(kObject70404, 0, 0);
			break;
		}
		break;

	case 70424:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectPresentationShow(kObject70404, 6);
			_app->objectSetAccessibilityOn(kObject70404, 1, 2);
			break;

		case 2:
			_app->soundStop(70407, 1024);
			break;

		case 14:
			_app->soundPlay(70407);
			break;

		case 15:
			_app->soundStop(70405, 1024);
			break;

		case 26:
			_app->soundPlay(70405);
			_app->objectPresentationHide(kObject70406);
			sub_433FA0();
			break;
		}
		break;

	case 70425:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(70408);
			_app->objectPresentationHide(kObject70404, 6);
			break;

		case 15:
			_app->soundPlay(70406);
			break;

		case 26:
			_app->puzzleSetMovabilityOn(70411);
			_app->objectSetAccessibilityOn(kObject70404, 0, 0);
			_app->varSetByte(70005, 1);
			_app->objectPresentationShow(kObject70406);
			sub_4340C0();

			if (!_app->varGetByte(70012))
				_app->objectPresentationShow(kObjectFire);
			break;
		}
		break;
	}
}

void ZoneN2Ring::onSound(Id id, SoundType, uint32, bool process) {
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
		_app->onSwitchZone(kZoneRO, 0);
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
		_app->sub_433EE0();
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

void ZoneN2Ring::onBag(ObjectId id, Id target, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject70103:
		switch (type) {
		default:
			break;

		case 1:
			_presentationIndexN2 = 0;
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(495, 194, 598, 284));
			break;

		case 2:
			if (!_app->soundIsPlaying(70401))
				_app->soundStop(70401, 1024);

			_app->objectSetAccessibilityOff(kObject70103);

			if (_presentationIndexN2 <= 6) {
				while (_presentationIndexN2 > 0) {
					--_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject70103);
			} else {
				while (_presentationIndexN2 < 12) {
					++_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				uint32 startTick = g_system->getMillis();
				while (g_system->getMillis() - startTick < 1000)
					handleEvents();

				while (_presentationIndexN2 > 0) {
					--_presentationIndexN2;

					_app->objectPresentationHide(kObject70103);
					_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject70103);
				_app->playMovie("1498");
				_app->rotationSetAlp(70001, 270.0f);
				_app->rotationSetRan(70001, 85.7f);
				_app->rotationSetActive(70001);
				_app->sub_433EE0();
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				_presentationIndexN2 = _app->dragControlGetOffsetX0() / 5;

				if (_app->dragControlXLower0()) {
					_presentationIndexN2 = 0;
				} else if (_presentationIndexN2 >= 0) {
					if (_presentationIndexN2 > 12)
						_presentationIndexN2 = 12;
				} else {
					_presentationIndexN2 = 0;
				}

				_app->objectPresentationHide(kObject70103);
				_app->objectPresentationShow(kObject70103, (uint32)_presentationIndexN2);

				if (!_app->soundIsPlaying(70401))
					_app->soundPlay(70401);
			}
			break;
		}
		break;

	case kObject70106:
		switch (type) {
		default:
			break;

		case 1: {
			_app->dragControlSetCoords1(Common::Point(243, 276));

			int32 val1 = 0;
			int32 val2 = 0;
			int32 offsetX1 = 0;
			int32 offsetY1 = 0;

			if (_app->dragControlXLower1())
				offsetX1 = -(int32)_app->dragControlGetOffsetX1();
			else
				offsetX1 = _app->dragControlGetOffsetX1();

			if (_app->dragControlYLower1())
				offsetY1 = -(int32)_app->dragControlGetOffsetY1();
			else
				offsetY1 = _app->dragControlGetOffsetY1();

			if (offsetX1 <= 0) {
				if (offsetY1 > 0) {
					val1 = (offsetY1 + offsetX1 + 40) / 6;
					val2 = 30;
				} else {
					val1 = ((offsetY1 - offsetX1) + 40) / 6;
					val2 = 20;
				}
			} else {
				if (offsetY1 > 0) {
					val1 = ((offsetX1 - offsetY1) + 40) / 6;
					val2 = 0;
				} else {
					val1 = (40 - (offsetX1 + offsetY1)) / 6;
					val2 = 10;
				}
			}

			if (val1 < 0)
				val1 = 0;

			_distN2 = val1 + val2 - _app->varGetWord(70016);
			if (_distN2 < 0)
				_distN2	= 0;

			if (_distN2 > 18)
				_distN2 %= 19;
			}
			break;

		case 2: {
			int32 offset = 0;
			bool update = true;

			switch (_presentationIndexN2) {
			default:
				offset = 2;
				break;

			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 13:
			case 14:
			case 15:
				offset = -1;
				break;

			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 16:
			case 17:
			case 18:
				offset = 1;
				break;

			case 12:
				// Skip update
				update = false;
				break;
			}

			if (update) {
				do {
					_presentationIndexN2 += offset;

					if (_presentationIndexN2 <= 18) {
						if (_presentationIndexN2 < 0)
							_presentationIndexN2 = 18;
					} else {
						_presentationIndexN2 = 0;
					}

					_app->objectPresentationHideAndRemove(kObject70106);
					_app->objectPresentationShow(kObject70106, (uint32)_presentationIndexN2);

					handleEvents();

				} while (_presentationIndexN2 != 12);
			}

			_app->varSetWord(70016, (int16)offset);

			if (_presentationIndexN2 == 12 ) {
				_app->varSetByte(10104, 1);
				_app->rotationSetMovabilityOff(70101, 1, 1);
				_app->rotationSetMovabilityOn(70101, 2, 2);
			} else {
				_app->varSetByte(10104, 0);
				_app->rotationSetMovabilityOn(70101, 1, 1);
				_app->rotationSetMovabilityOff(70101, 2, 2);
			}
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				int32 val1 = 0;
				int32 val2 = 0;
				int32 offsetX1 = 0;
				int32 offsetY1 = 0;

				if (_app->dragControlXLower1())
					offsetX1 = -(int32)_app->dragControlGetOffsetX1();
				else
					offsetX1 = _app->dragControlGetOffsetX1();

				if (_app->dragControlYLower1())
					offsetY1 = -(int32)_app->dragControlGetOffsetY1();
				else
					offsetY1 = _app->dragControlGetOffsetY1();

				// Check interval
				if (offsetX1 > 40 || offsetY1 > 40 || offsetX1 < -40 || offsetY1 < -40)
					break;

				if (offsetX1 <= 0) {
					if (offsetY1 > 0) {
						val1 = (offsetY1 + offsetX1 + 40) / 6;
						val2 = 30;
					} else {
						val1 = ((offsetY1 - offsetX1) + 40) / 6;
						val2 = 20;
					}
				} else {
					if (offsetY1 > 0) {
						val1 = ((offsetX1 - offsetY1) + 40) / 6;
						val2 = 0;
					} else {
						val1 = (40 - (offsetX1 + offsetY1)) / 6;
						val2 = 10;
					}
				}

				if (val1 < 0)
					val1 = 0;

				_app->_presentationIndexNI = val1 + val2 + _distN2;
				if (_app->_presentationIndexNI < 0)
					_app->_presentationIndexNI	= 0;

				if (_app->_presentationIndexNI > 18)
					_app->_presentationIndexNI %= 19;

				_app->objectPresentationHide(kObject70106);
				_app->objectPresentationShow(kObject70106, (uint32)_presentationIndexN2);
				_app->varSetWord(70016, (int16)_presentationIndexN2);
			}
			break;
		}
		break;

	case kObject70108:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(389, 270, 434, 390));
			break;

		case 2:
			_app->objectPresentationHide(kObject70108);

			if (target) {
				if (_app->dragControlGetOffsetY0() < 20 || _app->dragControlYHigher0()) {
					_app->objectPresentationShow(kObject70108, 13);
					_app->varSetByte(70012, 1);
					_app->objectPresentationShow(kObject70108, 14);
					break;
				}

				_app->objectPresentationShow(kObject70108, 0);
				_app->objectSetAccessibilityOn(kObject70108, 0, 0);
				_app->objectSetAccessibilityOff(kObject70108, 1, 1);
			} else {
				if (_app->dragControlGetOffsetY0() >= 20 && !_app->dragControlYLower0()) {
					_app->objectPresentationShow(kObject70108, 13);
					_app->objectSetAccessibilityOff(kObject70108, 0, 0);
					_app->objectSetAccessibilityOn(kObject70108, 1, 1);
					_app->varSetByte(70012, 1);
					_app->objectPresentationShow(kObject70108, 14);
					_app->soundPlay(70101);

					_app->playMovie("1496");
					_app->playMovie("1497");

					_app->varSetFloat(90006, _app->varGetFloat(90006) + 10.0f);
					_app->objectPresentationHide(kObjectFire);
					_app->objectPresentationHide(kObject70108);
					_app->objectSetAccessibilityOff(kObject70700);
					_app->objectPresentationHide(kObject70700);
					_app->soundStop(70701, 1024);
					_app->rotationSet3DSoundOff(70200, 70701);
					break;
				}

				_app->objectPresentationShow(kObject70108, 0);
			}

			_app->varSetByte(70012, 0);
			_app->objectPresentationHide(kObject70108, 14);
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 offset = 0;

				if (target) {
					offset = 13 - _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYHigher0())
						offset = 13;
				} else {
					offset = _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYLower0())
						offset = 0;
				}

				_app->objectPresentationHide(kObject70108);

				if (offset < 0)
					offset = 0;

				if (offset > 12)
					offset = 13;

				_app->objectPresentationShow(kObject70108, 14);
				_app->objectPresentationShow(kObject70108, (uint32)offset);
			}
			break;
		}
		break;
	}
}

void ZoneN2Ring::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
	if (movabilityFrom == 70100 && movabilityTo == 0) {
		_app->rotationSetActive(70100);
		_app->objectSetAccessibilityOn(kObject70101, 0, 0);
		_app->objectSetAccessibilityOn(kObject70102, 0, 0);
		_app->objectSetAccessibilityOn(kObject70101, 2, 2);
		_app->objectSetAccessibilityOn(kObject70102, 2, 2);
		_app->objectSetAccessibilityOn(kObject70100, 4, 4);

		g_system->warpMouse(point.x, point.y);
	}
}

void ZoneN2Ring::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		_app->timerStop(kTimer0);

	if (movabilityType == kMovabilityPuzzleToPuzzle) {
		if (movabilityTo == 70501 || movabilityTo == 70511 || movabilityTo == 70521)
			_app->soundPlay(70501);

		if (movabilityFrom == 70501 || movabilityFrom == 70511 || movabilityFrom == 70521)
			_app->soundPlay(70502);
	}
}

void ZoneN2Ring::onAfterRide(Id movabilityFrom, Id, uint32, Id target, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		if (!_app->varGetByte(70012))
			_app->timerStart(kTimer0, 10000);

	if (movabilityType == kMovabilityRotationToRotation) {
		if (target == 7)
			_app->sub_433EE0();
		else if (target == 16)
			_app->exitToMenu(kMenuAction1);
	}
}

#pragma region Helper functions

void ZoneN2Ring::sub_433FA0() {
	_app->puzzleSet3DSoundOff(kPuzzle70410, 70412);
	_app->rotationSet3DSoundOff(70400, 70412);

	for (int i = 95; i > 70; i--) {
		_app->puzzleSet3DSoundVolume(kPuzzle70411, 70412, i);
		handleEvents();
	}

	_app->puzzleSet3DSoundOff(kPuzzle70411, 70412);
	_app->rotationSet3DSoundOff(70000, 70107);
	_app->rotationSet3DSoundOff(70001, 70107);
	_app->rotationSet3DSoundOff(70400, 70107);
	_app->rotationSet3DSoundOff(70100, 70107);
	_app->rotationSet3DSoundOff(70101, 70107);
	_app->puzzleSet3DSoundOff(kPuzzle70100, 70107);
	_app->puzzleSet3DSoundOff(kPuzzle70102, 70107);
	_app->soundPlay(70000);
}

void ZoneN2Ring::sub_4340C0() {
	_app->soundStop(70000, 1024);
	_app->puzzleSet3DSoundOn(kPuzzle70411, 70412);
	_app->puzzleSet3DSoundOn(kPuzzle70410, 70412);
	_app->rotationSet3DSoundOn(70400, 70412);

	for (int i = 70; i < 95; i++) {
		_app->puzzleSet3DSoundVolume(kPuzzle70411, 70412, i);
		handleEvents();
	}

	_app->rotationSet3DSoundOn(70000, 70107);
	_app->rotationSet3DSoundOn(70001, 70107);
	_app->rotationSet3DSoundOn(70400, 70107);
	_app->rotationSet3DSoundOn(70100, 70107);
	_app->rotationSet3DSoundOn(70101, 70107);
	_app->puzzleSet3DSoundOn(kPuzzle70100, 70107);
	_app->puzzleSet3DSoundOn(kPuzzle70102, 70107);
}

#pragma endregion

} // End of namespace Ring
