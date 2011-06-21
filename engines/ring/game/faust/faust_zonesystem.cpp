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

#include "ring/game/faust/faust_zonesystem.h"

#include "ring/base/object.h"
#include "ring/base/saveload.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

ZoneSystemFaust::ZoneSystemFaust(ApplicationFaust *application) : _app(application) {
}

ZoneSystemFaust::~ZoneSystemFaust() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneSystemFaust::onInit() {
	_app->setCurrentZone(kZoneSY);
	_app->objectAdd(kObjectHomunculus, "Homunculus", "a08_Humunculus", 1);
	_app->objectSetActiveCursor(kObjectHomunculus, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectHomunculus, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->puzzleAdd(kPuzzleMenu);
	_app->objectAdd(kObject1, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject1, kPuzzleMenu, Common::Rect(0, 0, 640, 480), false, kCursorDefault, 0);
	_app->objectAddPresentation(kObject1);
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 16),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 32),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 48),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 64),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 80),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 96),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 112), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 128), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 144), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectPresentationShow(kObject1, 0);
	_app->puzzleAdd(kPuzzleGeneralMenu);
	_app->puzzleAddBackgroundImage(kPuzzleGeneralMenu, "Menu.bmp", Common::Point(0, 0), true);
	_app->puzzleAdd(kPuzzlePreferences);
	_app->puzzleAddBackgroundImage(kPuzzlePreferences, "Login.bmp", Common::Point(0, 0), true);
	_app->objectAdd(kObject6, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject6, kPuzzleMenu, Common::Rect(498, 16, 528, 71), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject6, kPuzzleMenu, Common::Rect(0, 0, 640, 72), false, kCursorTake, 1);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 0, kPuzzleMenu, "a12_trak.tga", Common::Point(0, 16), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 1, kPuzzleMenu, "a12_timer.tga", Common::Point(130, 16), true, kDrawType3, 1010);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 2, kPuzzleMenu, "a12_timer.tga", Common::Point(130, 46), true, kDrawType3, 1010);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 3, kPuzzleMenu, "a12_stairs.0001.tga", Common::Point(477, 16), true, kDrawType3, 1005);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 4, kPuzzleMenu, "a12_stairs.0002.tga", Common::Point(477, 16), true, kDrawType3, 1005);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 5, kPuzzleMenu, "a12_humunculus.tga", Common::Point(502, 20), true, kDrawType3, 1020);
	_app->objectAdd(kObject11, "", "", 1);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 0, kPuzzleMenu, "a11s02n06p07.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 1, kPuzzleMenu, "a11s02n05p03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 2, kPuzzleMenu, "a11s01n03p02l03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 3, kPuzzleMenu, "a11s01n08p01l04.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 4, kPuzzleMenu, "a11s01n07p01l03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 5, kPuzzleMenu, "a11s01n06p01l05.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 6, kPuzzleMenu, "a11s04n02p04l04.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 7, kPuzzleMenu, "a11s04n02p03l04.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 8, kPuzzleMenu, "a11s02n05p01.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 9, kPuzzleMenu, "a11s02n06p10.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 10, kPuzzleMenu, "a11s02n06p10l01.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 11, kPuzzleMenu, "a11s02n06p11.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 12, kPuzzleMenu, "a11s02n06p11l01.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 13, kPuzzleMenu, "a11s02n06p12.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 14, kPuzzleMenu, "a11s02n06p13.tga", Common::Point(163, 32), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 15, kPuzzleMenu, "a11s02n06p13l01.tga", Common::Point(163, 32), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 16, kPuzzleMenu, "a11s02n06p08.tga", Common::Point(0, 74), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 17, kPuzzleMenu, "a11s02n06p08l01.tga", Common::Point(0, 161), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject11);
	_app->objectPresentationAddImageToPuzzle(kObject11, 18, kPuzzleMenu, "a11s04n01p08l05.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAdd(kObject12, "", "", 1);
	_app->objectAddPresentation(kObject12);
	_app->objectPresentationAddImageToPuzzle(kObject12, 0, kPuzzleMenu, "a11s04n01p11.tga", Common::Point(118, 130), true, kDrawType3, 1100);
	_app->objectPresentationAddImageToPuzzle(kObject12, 0, kPuzzleMenu, "a11s04n01p11l01.tga", Common::Point(244, 210), true, kDrawType3, 1200);
	_app->objectAddPresentation(kObject12);
	_app->objectPresentationAddImageToPuzzle(kObject12, 1, kPuzzleMenu, "a11s04n01p11l02.tga", Common::Point(0, 20), true, kDrawType3, 1500);
	_app->objectAddPresentation(kObject12);
	_app->objectPresentationAddImageToPuzzle(kObject12, 2, kPuzzleMenu, "a11s04n01p11l01.tga", Common::Point(244, 210), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject12);
	_app->objectPresentationAddImageToPuzzle(kObject12, 3, kPuzzleMenu, "a11s04n01p11l03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectAdd(kObject23, "", "Cur_Drag", 5);
	_app->objectSetActiveDrawCursor(kObject23, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject23, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject23, kPuzzleMenu, Common::Rect(220, 150, 420, 340), false, kCursorDragDrop, 0);
	_app->objectAddPuzzleAccessibility(kObject23, kPuzzleMenu, Common::Rect(0, 0, 640, 480), false, kCursorBack, 99);
	_app->objectAdd(kObjectCodingWheel, "Coding Wheel", "a11_CodeWheel", 1);
	_app->objectSetActiveCursor(kObjectCodingWheel, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectCodingWheel, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 36; i++) {
		_app->objectAddPresentation(kObjectCodingWheel);
		_app->objectPresentationAddImageToPuzzle(kObjectCodingWheel, i, kPuzzleMenu, Common::String::format("a11s04n01p16s01.%04d.tga", i + 1), Common::Point(220, 150), true, kDrawType3, 1000);
	}

	_app->objectAdd(kObject13, "", "", 1);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 0, kPuzzleMenu, "a11s02n05p02.tga", Common::Point(0, 360), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 1, kPuzzleMenu, "a11s02n05p02l01.bmp", Common::Point(33, 395), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 2, kPuzzleMenu, "a11s02n05p02l02.bmp", Common::Point(56, 395), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 3, kPuzzleMenu, "a11s02n05p02l03.bmp", Common::Point(73, 392), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 4, kPuzzleMenu, "a11s02n05p02l04.bmp", Common::Point(93, 392), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 5, kPuzzleMenu, "a11s02n05p02l05.bmp", Common::Point(112, 390), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 6, kPuzzleMenu, "a11s02n05p02l06.bmp", Common::Point(24, 418), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 7, kPuzzleMenu, "a11s02n05p02l07.bmp", Common::Point(39, 414), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 8, kPuzzleMenu, "a11s02n05p02l08.bmp", Common::Point(55, 415), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 9, kPuzzleMenu, "a11s02n05p02l09.bmp", Common::Point(72, 414), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 10, kPuzzleMenu, "a11s02n05p02l10.bmp", Common::Point(104, 420), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13);
	_app->objectPresentationAddImageToPuzzle(kObject13, 11, kPuzzleMenu, "a11s02n05p02l11.bmp", Common::Point(121, 419), true, kDrawType1, 1000);
	_app->objectAdd(kObject7, "", "", 1);
	_app->objectAddPresentation(kObject7);
	_app->objectPresentationAddImageToPuzzle(kObject7, 0, kPuzzleMenu, "a12s07n01p01.tga", Common::Point(6, 350), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject7);
	_app->objectPresentationAddImageToPuzzle(kObject7, 1, kPuzzleMenu, "a12s07n01p01l01.tga", Common::Point(21, 387), true, kDrawType3, 1010);
	_app->objectAddPresentation(kObject7);
	_app->objectPresentationAddImageToPuzzle(kObject7, 2, kPuzzleMenu, "a12s07n01p01l02.tga", Common::Point(80, 387), true, kDrawType3, 1010);
	_app->objectAddPresentation(kObject7);
	_app->objectPresentationAddImageToPuzzle(kObject7, 3, kPuzzleMenu, "a12s07n01p01l03.tga", Common::Point(30, 360), true, kDrawType3, 1010);
	_app->objectAddPresentation(kObject7);
	_app->objectPresentationAddImageToPuzzle(kObject7, 4, kPuzzleMenu, "a12s07n01p01l04.tga", Common::Point(93, 360), true, kDrawType3, 1010);
	_app->objectAdd(kObject8, "", "", 1);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 0, kPuzzleMenu, "a07s01n14r01p01.tga", Common::Point(0, 16), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 1, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(10, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 2, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(30, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 3, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(50, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 4, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(70, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 5, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(90, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 6, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(110, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 7, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(130, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 8, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(150, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 9, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(170, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 10, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(190, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 11, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(210, 24), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject8);
	_app->objectPresentationAddImageToPuzzle(kObject8, 12, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(230, 24), true, kDrawType3, 1100);
	_app->objectAdd(kObject9, "", "", 1);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 0, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(484, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 1, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(494, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 2, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(504, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 3, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(514, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 4, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(524, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 5, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(534, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 6, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(544, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 7, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(554, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 8, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(564, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 9, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(574, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 10, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(584, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 11, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(594, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 12, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(604, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 13, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(614, 22), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject9);
	_app->objectPresentationAddImageToPuzzle(kObject9, 14, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(624, 22), true, kDrawType3, 1100);
	_app->objectAdd(kObject10, "", "", 1);
	_app->objectAddPresentation(kObject10);
	_app->objectPresentationAddImageToPuzzle(kObject10, 0, kPuzzleMenu, "a07s01n14r01p01l03.tga", Common::Point(270, 18), true, kDrawType3, 1100);
	_app->objectAdd(kObject14, "", "", 1);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 0, kPuzzleMenu, "space.tga", Common::Point(0, 440), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 1, kPuzzleMenu, "spacel_d.tga", Common::Point(0, 440), true, kDrawType3, 2100);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 2, kPuzzleMenu, "spacel_h.tga", Common::Point(0, 440), true, kDrawType3, 2100);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 3, kPuzzleMenu, "spacer_d.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 4, kPuzzleMenu, "spacer_h.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 5, kPuzzleMenu, "spacer_d.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 6, kPuzzleMenu, "spacer_h.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 7, kPuzzleMenu, "space_hP.tga", Common::Point(0, 264), true, kDrawType3, 2050);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 8, kPuzzleMenu, "space_hA.tga", Common::Point(0, 264), true, kDrawType3, 2050);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 9, kPuzzleMenu, "space_m.tga", Common::Point(440, 264), true, kDrawType3, 2050);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 10, kPuzzleMenu, "space_n.tga", Common::Point(440, 264), true, kDrawType3, 2050);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 11, kPuzzleMenu, "SY_map_A01.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 12, kPuzzleMenu, "SY_map_A01.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 13, kPuzzleMenu, "SY_map_A03.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 14, kPuzzleMenu, "SY_map_A05.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 15, kPuzzleMenu, "SY_map_A06.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 16, kPuzzleMenu, "SY_map_A07.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 17, kPuzzleMenu, "SY_map_A08.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 18, kPuzzleMenu, "SY_map_A09.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 19, kPuzzleMenu, "SY_map_A11.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 20, kPuzzleMenu, "SY_map_A12.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 21, kPuzzleMenu, "SY_map_A13.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 22, kPuzzleMenu, "SY_map_A14.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 23, kPuzzleMenu, "rol_l_j1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 24, kPuzzleMenu, "rol_l_j2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 25, kPuzzleMenu, "rol_nat1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 26, kPuzzleMenu, "rol_nat2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 27, kPuzzleMenu, "rol_fra1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 28, kPuzzleMenu, "rol_fra2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 29, kPuzzleMenu, "rol_kal1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 30, kPuzzleMenu, "rol_kal2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 31, kPuzzleMenu, "rol_han1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 32, kPuzzleMenu, "rol_han2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 33, kPuzzleMenu, "rol_tod1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 34, kPuzzleMenu, "rol_tod2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 35, kPuzzleMenu, "rol_gis1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPresentation(kObject14);
	_app->objectPresentationAddImageToPuzzle(kObject14, 36, kPuzzleMenu, "rol_gis2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(7, 443, 169, 459), false, kCursorMenuActive, 0);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(489, 441, 637, 462), false, kCursorMenuActive, 1);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleGeneralMenu, Common::Rect(469, 454, 637, 480), false, kCursorMenuActive, 2);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(67, 329, 103, 411), false, kCursorMenuActive, 3);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(0, 0, 1, 1), true, kCursorIdle, 4);
	_app->objectSetPuzzleAccessibilityKey(kObject14, 4, Common::KEYCODE_ESCAPE);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(346, 55, 585, 423), false, kCursorMenuActive, 5);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect( 81, 56, 288, 418), false, kCursorMenuActive, 6);
	_app->objectAddPuzzleAccessibility(kObject14, kPuzzleGeneralMenu, Common::Rect(0, 420, 640, 480), false, kCursorBack, 99);
	_app->objectAdd(kObject15, "", "", 1);
	_app->objectAddPresentation(kObject15);
	_app->objectPresentationAddAnimationToPuzzle(kObject15, 0, kPuzzleMenu, "limbo", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 2200, 183, 15.0f, 6);
	_app->objectAdd(kObject16, "", "", 1);
	_app->objectAddPresentation(kObject16);
	_app->objectPresentationAddImageToPuzzle(kObject16, 0, kPuzzleMenu, "a01s02_mephisto.tga", Common::Point(80, 57), true, kDrawType3, 2100);
	_app->objectAdd(kObject17, "", "", 1);
	_app->objectAddPresentation(kObject17);
	_app->objectPresentationAddImageToPuzzle(kObject17, 0, kPuzzleMenu, "a05s03n01p08.tga", Common::Point(0, 16), true, kDrawType3, 2100);
	_app->objectAddPuzzleAccessibility(kObject17, kPuzzleMenu, Common::Rect(0, 0, 640, 480), false, kCursorBack, 0);
	_app->objectAdd(kObject2, "", "", 1);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 0, kPuzzleMenu, "Exit.tga", Common::Point(160, 165), true, kDrawType3, 2000);
	_app->objectAddPuzzleAccessibility(kObject2, kPuzzleMenu, Common::Rect(290, 261, 370, 286), false, kCursorMenuActive, 1);
	_app->objectAddPuzzleAccessibility(kObject2, kPuzzleMenu, Common::Rect(370, 261, 450, 286), false, kCursorMenuActive, 0);
	_app->objectSetPuzzleAccessibilityKey(kObject2, 0, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject2, 1, Common::KEYCODE_ESCAPE);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 1, kPuzzleMenu, "box_yes.tga", Common::Point(291, 261), true, kDrawType3, 2000);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 2, kPuzzleMenu, "box_no.tga", Common::Point(371, 261), true, kDrawType3, 2000);
	_app->objectAdd(kObject3, "", "", 1);
	_app->objectAddPresentation(kObject3);
	_app->objectPresentationAddImageToPuzzle(kObject3, 0, kPuzzleMenu, "Warning.tga", Common::Point(160, 165), true, kDrawType3, 1000);
	_app->objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzleMenu, "", Common::Point(200, 200), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzleMenu, "", Common::Point(300, 280), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectAddPresentation(kObject3);
	_app->objectPresentationAddImageToPuzzle(kObject3, 1, kPuzzleMenu, "box_ok.tga", Common::Point(291, 261), true, kDrawType3, 1000);
	_app->objectAddPuzzleAccessibility(kObject3, kPuzzleMenu, Common::Rect(286, 261, 363, 307), false, kCursorMenuActive, 0);
	_app->objectSetPuzzleAccessibilityKey(kObject3, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObject4, "", "", 1);
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 0, kPuzzleMenu, "Question.tga", Common::Point(160, 165), true, kDrawType3, 1000);
	_app->objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzleMenu, "", Common::Point(200, 200), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzleMenu, "", Common::Point(200, 280), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 1, kPuzzleMenu, "box_yes.tga", Common::Point(291, 261), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 2, kPuzzleMenu, "box_no.tga", Common::Point(371, 261), true, kDrawType3, 1000);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 0);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 1);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 0, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 1, Common::KEYCODE_ESCAPE);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 2);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 3);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 2, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 3, Common::KEYCODE_ESCAPE);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 4);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 5);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 4, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 5, Common::KEYCODE_ESCAPE);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 6);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 7);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 6, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 7, Common::KEYCODE_ESCAPE);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 8);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 9);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 8, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 9, Common::KEYCODE_ESCAPE);
	_app->objectAdd(kObject5, "", "", 1);
	_app->puzzleAdd(kPuzzleInsertCd);
	_app->puzzleAddBackgroundImage(kPuzzleInsertCd, "insertcd.bmp", Common::Point(0, 16), true);
	_app->objectAdd(kObject55, "", "", 1);
	_app->objectAddPresentation(kObject55);
	_app->objectPresentationAddImageToPuzzle(kObject55, 0, kPuzzleInsertCd, "box_yes.tga", Common::Point(291, 261), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55);
	_app->objectPresentationAddTextToPuzzle(kObject55, 1, kPuzzleInsertCd, "", Common::Point(320, 240), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObject55, 1, kPuzzleInsertCd, "", Common::Point(320, 260), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectAddPresentation(kObject55);
	_app->objectPresentationAddImageToPuzzle(kObject55, 2, kPuzzleInsertCd, "box_no.tga", Common::Point(371, 261), true, kDrawType3, 1000);
	_app->objectPresentationShow(kObject55, 1);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 0);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 1);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 2);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 3);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 4);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 5);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 6);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 7);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 8);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 9);
	_app->objectAddPuzzleAccessibility(kObject55, kPuzzleInsertCd, Common::Rect(371, 261, 450, 311), true, kCursorMenuActive, 99);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 0, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 1, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 2, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 3, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 4, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 5, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 6, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 7, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 8, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 9, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject55, 10, Common::KEYCODE_ESCAPE);
	_app->objectAdd(kObject99500, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99500, kPuzzlePreferences, Common::Rect(132, 413, 252, 440), false, kCursorMenuActive, 1);
	_app->objectAddPuzzleAccessibility(kObject99500, kPuzzlePreferences, Common::Rect(387, 413, 507, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99500);
	_app->objectPresentationAddImageToPuzzle(kObject99500, 0, kPuzzlePreferences, "log_l1h.bmp", Common::Point(132, 413), true, kDrawType1, 1100);
	_app->objectAddPresentation(kObject99500);
	_app->objectPresentationAddImageToPuzzle(kObject99500, 1, kPuzzlePreferences, "log_r1h.bmp", Common::Point(387, 413), true, kDrawType1, 1100);
	_app->objectSetPuzzleAccessibilityKey(kObject99500, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObject99501, "", "", 1);
	_app->objectAddPresentation(kObject99501);
	_app->objectPresentationAddAnimationToPuzzle(kObject99501, 0, kPuzzlePreferences, "kybcur", kImageTypeBMP, Common::Point(0, 0), kDrawType1, 1000, 6, 12.5f, 16);
	_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99501, 0, Common::Point(233, 334));
	_app->objectPresentationAddTextToPuzzle(kObject99501, 0, kPuzzlePreferences, "", Common::Point(230, 334), kFontDefault, Color(255, 255, 255), Color(-1, -1, -1));
	_app->objectPresentationShow(kObject99501);
	_app->objectAdd(kObject99600, "", "", 1);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 0, kPuzzleGeneralMenu, "Evidence_01.bmp", Common::Point(172, 452), true, kDrawType1, 6000);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 1, kPuzzleGeneralMenu, "Evidence_02.bmp", Common::Point(213, 452), true, kDrawType1, 6000);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 2, kPuzzleGeneralMenu, "Evidence_03.bmp", Common::Point(255, 452), true, kDrawType1, 6000);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 3, kPuzzleGeneralMenu, "Evidence_04.bmp", Common::Point(297, 452), true, kDrawType1, 6000);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 4, kPuzzleGeneralMenu, "Evidence_05.bmp", Common::Point(339, 452), true, kDrawType1, 6000);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 5, kPuzzleGeneralMenu, "Evidence_06.bmp", Common::Point(381, 452), true, kDrawType1, 6000);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 6, kPuzzleGeneralMenu, "Evidence_07.bmp", Common::Point(423, 452), true, kDrawType1, 6000);
	_app->objectAddPresentation(kObject99600);
	_app->objectPresentationAddImageToPuzzle(kObject99600, 7, kPuzzleGeneralMenu, "notebook.tga", Common::Point(456, 453), true, kDrawType3, 6000);
	_app->objectAdd(kObject99010, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99010, kPuzzleGeneralMenu, Common::Rect(7, 19, 38, 49), true, kCursorMenuActive, 0);
	_app->objectAddPuzzleAccessibility(kObject99010, kPuzzleGeneralMenu, Common::Rect(600, 19, 633, 50), true, kCursorMenuActive, 1);
	_app->objectAddPresentation(kObject99010);
	_app->objectPresentationAddAnimationToPuzzle(kObject99010, 0, kPuzzleGeneralMenu, "main", kImageTypeBMP, Common::Point(154, 50), kDrawType1, 2000, 63, 15.0f, 5);
	_app->objectAddPresentation(kObject99010);
	_app->objectPresentationAddImageToPuzzle(kObject99010, 1, kPuzzleGeneralMenu, "tit_ln.bmp", Common::Point(9, 22), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99010);
	_app->objectPresentationAddImageToPuzzle(kObject99010, 2, kPuzzleGeneralMenu, "tit_lh.bmp", Common::Point(9, 22), true, kDrawType1, 2100);
	_app->objectAddPresentation(kObject99010);
	_app->objectPresentationAddImageToPuzzle(kObject99010, 3, kPuzzleGeneralMenu, "tit_rn.bmp", Common::Point(603, 22), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99010);
	_app->objectPresentationAddImageToPuzzle(kObject99010, 4, kPuzzleGeneralMenu, "tit_rh.bmp", Common::Point(603, 22), true, kDrawType1, 2100);
	_app->objectAdd(kObject99011, "", "", 3);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 0, kPuzzleGeneralMenu, "tit_e00.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 1, kPuzzleGeneralMenu, "tit_e01.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 2, kPuzzleGeneralMenu, "tit_e02.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 3, kPuzzleGeneralMenu, "tit_e03.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 4, kPuzzleGeneralMenu, "tit_e04.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 5, kPuzzleGeneralMenu, "tit_e05.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 6, kPuzzleGeneralMenu, "tit_e06.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectAddPresentation(kObject99011);
	_app->objectPresentationAddImageToPuzzle(kObject99011, 7, kPuzzleGeneralMenu, "tit_e07.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	_app->objectPresentationShow(kObject99011, 0);
	_app->objectAdd(kObject99012, "", "", 3);
	_app->objectAddPresentation(kObject99012);
	_app->objectPresentationAddImageToPuzzle(kObject99012, 0, kPuzzleGeneralMenu, "main.bmp", Common::Point(130, 50), true, kDrawType1, 1000);
	_app->objectAdd(kObject99000, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99000, kPuzzleGeneralMenu, Common::Rect(0, 135, 120, 162), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99000);
	_app->objectPresentationAddImageToPuzzle(kObject99000, 0, kPuzzleGeneralMenu, "But_l1h.bmp", Common::Point(0, 135), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99000);
	_app->objectPresentationAddImageToPuzzle(kObject99000, 1, kPuzzleGeneralMenu, "But_l1d.bmp", Common::Point(0, 135), true, kDrawType1, 1000);
	_app->objectAdd(kObject99001, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99001, kPuzzleGeneralMenu, Common::Rect(0, 185, 120, 212), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99001);
	_app->objectPresentationAddImageToPuzzle(kObject99001, 0, kPuzzleGeneralMenu, "But_l2h.bmp", Common::Point(0, 185), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99001);
	_app->objectPresentationAddImageToPuzzle(kObject99001, 1, kPuzzleGeneralMenu, "But_l2d.bmp", Common::Point(0, 185), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99001);
	_app->objectPresentationAddImageToPuzzle(kObject99001, 2, kPuzzleGeneralMenu, "Load.bmp", Common::Point(130, 50), true, kDrawType1, 2000);
	_app->objectAdd(kObject99002, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99002, kPuzzleGeneralMenu, Common::Rect(0, 235, 120, 262), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99002);
	_app->objectPresentationAddImageToPuzzle(kObject99002, 0, kPuzzleGeneralMenu, "But_l3h.bmp", Common::Point(0, 235), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99002);
	_app->objectPresentationAddImageToPuzzle(kObject99002, 1, kPuzzleGeneralMenu, "But_l3d.bmp", Common::Point(0, 235), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99002);
	_app->objectPresentationAddImageToPuzzle(kObject99002, 2, kPuzzleGeneralMenu, "Save.bmp", Common::Point(130, 50), true, kDrawType1, 2000);
	_app->objectAdd(kObject99003, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99003, kPuzzleGeneralMenu, Common::Rect(0, 285, 120, 312), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99003);
	_app->objectPresentationAddImageToPuzzle(kObject99003, 0, kPuzzleGeneralMenu, "But_l4h.bmp", Common::Point(0, 285), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99003);
	_app->objectPresentationAddImageToPuzzle(kObject99003, 1, kPuzzleGeneralMenu, "But_l4d.bmp", Common::Point(0, 285), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99003);
	_app->objectPresentationAddImageToPuzzle(kObject99003, 2, kPuzzleGeneralMenu, "Preferences.bmp", Common::Point(130, 50), true, kDrawType1, 2000);
	_app->objectAdd(kObject99004, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99004, kPuzzleGeneralMenu, Common::Rect(0, 335, 120, 362), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99004);
	_app->objectPresentationAddImageToPuzzle(kObject99004, 0, kPuzzleGeneralMenu, "But_l5h.bmp", Common::Point(0, 335), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99004);
	_app->objectPresentationAddImageToPuzzle(kObject99004, 1, kPuzzleGeneralMenu, "But_l5d.bmp", Common::Point(0, 335), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99004);
	_app->objectPresentationAddTextToPuzzle(kObject99004, 2, kPuzzleGeneralMenu, "", Common::Point(1, 16), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	_app->objectAdd(kObject99005, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99005, kPuzzleGeneralMenu, Common::Rect(520, 135, 640, 162), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 0, kPuzzleGeneralMenu, "But_r1h.bmp", Common::Point(520, 135), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 1, kPuzzleGeneralMenu, "But_r1d.bmp", Common::Point(520, 135), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 2, kPuzzleGeneralMenu, "stars_e01.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 3, kPuzzleGeneralMenu, "stars_e02.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 4, kPuzzleGeneralMenu, "stars_e03.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 5, kPuzzleGeneralMenu, "stars_e04.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 6, kPuzzleGeneralMenu, "stars_e05.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 7, kPuzzleGeneralMenu, "stars_e06.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99005);
	_app->objectPresentationAddImageToPuzzle(kObject99005, 8, kPuzzleGeneralMenu, "stars_e07.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAdd(kObject99006, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99006, kPuzzleGeneralMenu, Common::Rect(520, 185, 640, 212), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99006);
	_app->objectPresentationAddImageToPuzzle(kObject99006, 0, kPuzzleGeneralMenu, "But_r2h.bmp", Common::Point(520, 185), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99006);
	_app->objectPresentationAddImageToPuzzle(kObject99006, 1, kPuzzleGeneralMenu, "But_r2d.bmp", Common::Point(520, 185), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99006);
	_app->objectPresentationAddImageToPuzzle(kObject99006, 2, kPuzzleGeneralMenu, "Movies.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAdd(kObject99007, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99007, kPuzzleGeneralMenu, Common::Rect(520, 235, 640, 262), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99007);
	_app->objectPresentationAddImageToPuzzle(kObject99007, 0, kPuzzleGeneralMenu, "But_r3h.bmp", Common::Point(520, 235), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99007);
	_app->objectPresentationAddImageToPuzzle(kObject99007, 1, kPuzzleGeneralMenu, "But_r3d.bmp", Common::Point(520, 235), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99007);
	_app->objectPresentationAddImageToPuzzle(kObject99007, 2, kPuzzleGeneralMenu, "Music.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAdd(kObject99008, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99008, kPuzzleGeneralMenu, Common::Rect(520, 285, 640, 312), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99008);
	_app->objectPresentationAddImageToPuzzle(kObject99008, 0, kPuzzleGeneralMenu, "But_r4h.bmp", Common::Point(520, 285), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99008);
	_app->objectPresentationAddImageToPuzzle(kObject99008, 1, kPuzzleGeneralMenu, "But_r4d.bmp", Common::Point(520, 285), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99008);
	_app->objectPresentationAddImageToPuzzle(kObject99008, 2, kPuzzleGeneralMenu, "Status.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAdd(kObject99009, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99009, kPuzzleGeneralMenu, Common::Rect(520, 335, 640, 362), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99009);
	_app->objectPresentationAddImageToPuzzle(kObject99009, 0, kPuzzleGeneralMenu, "But_r5h.bmp", Common::Point(520, 335), true, kDrawType1, 1200);
	_app->objectAddPresentation(kObject99009);
	_app->objectPresentationAddImageToPuzzle(kObject99009, 1, kPuzzleGeneralMenu, "But_r5d.bmp", Common::Point(520, 335), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject99009);
	_app->objectPresentationAddImageToPuzzle(kObject99009, 2, kPuzzleGeneralMenu, "Attraction.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	_app->objectAdd(kObject99021, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99021, kPuzzleGeneralMenu, Common::Rect(259, 413, 379, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99021);
	_app->objectPresentationAddImageToPuzzle(kObject99021, 0, kPuzzleGeneralMenu, "but_d2h.bmp", Common::Point(259, 413), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99021);
	_app->objectPresentationAddImageToPuzzle(kObject99021, 1, kPuzzleGeneralMenu, "but_d2d.bmp", Common::Point(259, 413), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99021);
	_app->objectPresentationAddImageToPuzzle(kObject99021, 2, kPuzzleGeneralMenu, "but_d2n.bmp", Common::Point(259, 413), true, kDrawType1, 2300);
	_app->objectAdd(kObject99022, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99022, kPuzzleGeneralMenu, Common::Rect(386, 413, 506, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99022);
	_app->objectPresentationAddImageToPuzzle(kObject99022, 0, kPuzzleGeneralMenu, "but_d3h.bmp", Common::Point(386, 413), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99022);
	_app->objectPresentationAddImageToPuzzle(kObject99022, 1, kPuzzleGeneralMenu, "but_d3d.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99022);
	_app->objectPresentationAddImageToPuzzle(kObject99022, 2, kPuzzleGeneralMenu, "but_d3n.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	_app->objectSetPuzzleAccessibilityKey(kObject99022, 0, Common::KEYCODE_ESCAPE);
	_app->objectAdd(kObject99023, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99023, kPuzzleGeneralMenu, Common::Rect(131, 413, 251, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99023);
	_app->objectPresentationAddImageToPuzzle(kObject99023, 0, kPuzzleGeneralMenu, "but_d1h.bmp", Common::Point(131, 413), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99023);
	_app->objectPresentationAddImageToPuzzle(kObject99023, 1, kPuzzleGeneralMenu, "but_d1d.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99023);
	_app->objectPresentationAddImageToPuzzle(kObject99023, 2, kPuzzleGeneralMenu, "but_d1n.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	_app->objectSetPuzzleAccessibilityKey(kObject99023, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObject99024, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99024, kPuzzleGeneralMenu, Common::Rect(386, 413, 506, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99024);
	_app->objectPresentationAddImageToPuzzle(kObject99024, 0, kPuzzleGeneralMenu, "but_d3h.bmp", Common::Point(386, 413), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99024);
	_app->objectPresentationAddImageToPuzzle(kObject99024, 1, kPuzzleGeneralMenu, "but_d3d.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99024);
	_app->objectPresentationAddImageToPuzzle(kObject99024, 2, kPuzzleGeneralMenu, "but_d3n.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	_app->objectSetPuzzleAccessibilityKey(kObject99024, 0, Common::KEYCODE_ESCAPE);
	_app->objectAdd(kObject99025, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99025, kPuzzleGeneralMenu, Common::Rect(131, 413, 251, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99025);
	_app->objectPresentationAddImageToPuzzle(kObject99025, 0, kPuzzleGeneralMenu, "but_d1h.bmp", Common::Point(131, 413), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99025);
	_app->objectPresentationAddImageToPuzzle(kObject99025, 1, kPuzzleGeneralMenu, "but_d1d.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99025);
	_app->objectPresentationAddImageToPuzzle(kObject99025, 2, kPuzzleGeneralMenu, "but_d1n.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	_app->objectSetPuzzleAccessibilityKey(kObject99025, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObject99041, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99041, kPuzzleGeneralMenu, Common::Rect(386, 413, 506, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99041);
	_app->objectPresentationAddImageToPuzzle(kObject99041, 0, kPuzzleGeneralMenu, "but_d3h.bmp", Common::Point(386, 413), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99041);
	_app->objectPresentationAddImageToPuzzle(kObject99041, 1, kPuzzleGeneralMenu, "but_d3d.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99041);
	_app->objectPresentationAddImageToPuzzle(kObject99041, 2, kPuzzleGeneralMenu, "but_d3n.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	_app->objectSetPuzzleAccessibilityKey(kObject99041, 0, Common::KEYCODE_ESCAPE);
	_app->objectAdd(kObject99042, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99042, kPuzzleGeneralMenu, Common::Rect(131, 413, 251, 440), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99042);
	_app->objectPresentationAddImageToPuzzle(kObject99042, 0, kPuzzleGeneralMenu, "but_d1h.bmp", Common::Point(131, 413), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99042);
	_app->objectPresentationAddImageToPuzzle(kObject99042, 1, kPuzzleGeneralMenu, "but_d1d.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99042);
	_app->objectPresentationAddImageToPuzzle(kObject99042, 2, kPuzzleGeneralMenu, "but_d1n.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	_app->objectSetPuzzleAccessibilityKey(kObject99042, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObject99043, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99043, kPuzzleGeneralMenu, Common::Rect(338, 316, 405, 342), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99043);
	_app->objectPresentationAddImageToPuzzle(kObject99043, 0, kPuzzleGeneralMenu, "Pref_off.tga", Common::Point(300, 322), true, kDrawType3, 2200);
	_app->objectAddPresentation(kObject99043);
	_app->objectPresentationAddImageToPuzzle(kObject99043, 1, kPuzzleGeneralMenu, "Pref_on.tga", Common::Point(300, 322), true, kDrawType3, 2200);
	_app->objectAdd(kObject99044, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99044, kPuzzleGeneralMenu, Common::Rect(338, 251, 405, 277), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99044);
	_app->objectPresentationAddImageToPuzzle(kObject99044, 0, kPuzzleGeneralMenu, "Pref_nor.tga", Common::Point(300, 257), true, kDrawType3, 2200);
	_app->objectAddPresentation(kObject99044);
	_app->objectPresentationAddImageToPuzzle(kObject99044, 1, kPuzzleGeneralMenu, "Pref_rev.tga", Common::Point(300, 257), true, kDrawType3, 2300);
	_app->objectAdd(kObject99045, "", "CUR_DRAG", 4);
	_app->objectSetActiveDrawCursor(kObject99045, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject99045, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject99045, kPuzzleGeneralMenu, Common::Rect(150, 160, 492, 184), false, kCursorDragDrop, 0);
	_app->objectAddPresentation(kObject99045);
	_app->objectPresentationAddImageToPuzzle(kObject99045, 0, kPuzzleGeneralMenu, "Volume.tga", Common::Point(0, 0), true, kDrawType3, 2200);
	_app->objectAdd(kObject99046, "", "CUR_DRAG", 4);
	_app->objectSetActiveDrawCursor(kObject99046, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject99046, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject99046, kPuzzleGeneralMenu, Common::Rect(150, 204, 492, 228), false, kCursorDragDrop, 0);
	_app->objectAddPresentation(kObject99046);
	_app->objectPresentationAddImageToPuzzle(kObject99046, 0, kPuzzleGeneralMenu, "Volume.tga", Common::Point(0, 0), true, kDrawType3, 2200);
	_app->objectAdd(kObject99601, "", "", 1);
	_app->objectAddPresentation(kObject99601);
	_app->objectPresentationAddAnimationToPuzzle(kObject99601, 0, kPuzzleGeneralMenu, "kybcur", kImageTypeBMP, Common::Point(0, 0), kDrawType1, 2300, 6, 12.5f, 16);
	_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(192, 388));
	_app->objectAddPresentation(kObject99601);
	_app->objectPresentationAddTextToPuzzle(kObject99601, 1, kPuzzleGeneralMenu, "", Common::Point(230, 382), kFontDefault, Color(255, 255, 255), Color(-1, -1, -1));
	_app->objectAddPresentation(kObject99601);

	_app->setArchiveType(kArchiveFile);
	_app->setLoadFrom(kLoadFrom5);
	_app->objectPresentationAddImageToPuzzle(kObject99601, 2, kPuzzleGeneralMenu, "osc.bmp", Common::Point(240, 82), true, kDrawType1, 5000);
	_app->setLoadFrom(kLoadFromDisk);
	_app->setArchiveType(_app->getZoneArchiveType(kZoneSY));

	_app->objectAdd(kObject99070, "", "", 1);
	_app->objectAddPresentation(kObject99070);
	_app->objectPresentationAddAnimationToPuzzle(kObject99070, 0, kPuzzleGeneralMenu, "movies", kImageTypeBMP, Common::Point(137, 52), kDrawType1, 2500, 36, 15.0f, 5);
	_app->objectPresentationAddImageToPuzzle(kObject99070, 0, kPuzzleGeneralMenu, "tit_movies.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	_app->objectAdd(kObject99071, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99071, kPuzzleGeneralMenu, Common::Rect(200, 220, 430, 399), false, kCursorMenuActive, 0);

	for (uint32 i = 0; i < 32; i++) {
		_app->objectAddPresentation(kObject99071);
		_app->objectPresentationAddImageToPuzzle(kObject99071, i, kPuzzleGeneralMenu, Common::String::format("mov_tit%02d.bmp", i), Common::Point(130, 214), true, kDrawType1, 2200);
	}

	_app->objectAdd(kObject99080, "", "", 1);
	_app->objectAddPresentation(kObject99080);
	_app->objectPresentationAddAnimationToPuzzle(kObject99080, 0, kPuzzleGeneralMenu, "music", kImageTypeBMP, Common::Point(204, 66), kDrawType1, 2500, 4, 15.0f, 5);
	_app->objectPresentationAddImageToPuzzle(kObject99080, 0, kPuzzleGeneralMenu, "tit_music.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	_app->objectAdd(kObject99081, "", "", 1);

	for (uint32 i = 0; i < 13; i++) {
		_app->objectAddPresentation(kObject99081);
		_app->objectPresentationAddImageToPuzzle(kObject99081, i, kPuzzleGeneralMenu, Common::String::format("mus_tit%02d.bmp", i + 1), Common::Point(135, 300), true, kDrawType1, 2200);
	}

	_app->objectAdd(kObject99090, "", "", 1);
	_app->objectAddPresentation(kObject99090);
	_app->objectPresentationAddAnimationToPuzzle(kObject99090, 0, kPuzzleGeneralMenu, "status", kImageTypeBMP, Common::Point(181, 102), kDrawType1, 2500, 50, 15.0f, 6);
	_app->objectPresentationAddImageToPuzzle(kObject99090, 0, kPuzzleGeneralMenu, "tit_status.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	_app->objectAdd(kObject99091, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99091, kPuzzleGeneralMenu, Common::Rect(200, 120, 450, 150), false, kCursorMenuActive, 1);
	_app->objectAddPuzzleAccessibility(kObject99091, kPuzzleGeneralMenu, Common::Rect(200, 240, 450, 270), false, kCursorMenuActive, 2);
	_app->objectAddPuzzleAccessibility(kObject99091, kPuzzleGeneralMenu, Common::Rect(200, 360, 450, 390), false, kCursorMenuActive, 3);
	_app->objectAddPresentation(kObject99091);
	_app->objectPresentationAddImageToPuzzle(kObject99091, 0, kPuzzleGeneralMenu, "tit_attraction.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99091);
	_app->objectPresentationAddImageToPuzzle(kObject99091, 1, kPuzzleGeneralMenu, "att_01.bmp", Common::Point(200, 118), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99091);
	_app->objectPresentationAddImageToPuzzle(kObject99091, 2, kPuzzleGeneralMenu, "att_02.bmp", Common::Point(200, 238), true, kDrawType1, 2200);
	_app->objectAddPresentation(kObject99091);
	_app->objectPresentationAddImageToPuzzle(kObject99091, 3, kPuzzleGeneralMenu, "att_03.bmp", Common::Point(200, 358), true, kDrawType1, 2200);
	_app->objectAdd(kObject99100, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99100, kPuzzleGeneralMenu, Common::Rect(0, 454, 50, 480), false, kCursorMenuActive, 0);
	_app->objectAdd(kObject99097, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject99097, kPuzzleGeneralMenu, Common::Rect(261, 414, 381, 444), false, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObject99097);
	_app->objectPresentationAddImageToPuzzle(kObject99097, 0, kPuzzleGeneralMenu, "but_d4h.bmp", Common::Point(261, 414), true, kDrawType1, 2400);
	_app->objectAddPresentation(kObject99097);
	_app->objectPresentationAddImageToPuzzle(kObject99097, 1, kPuzzleGeneralMenu, "but_d4d.bmp", Common::Point(261, 414), true, kDrawType1, 2300);
	_app->objectAddPresentation(kObject99097);
	_app->objectPresentationAddImageToPuzzle(kObject99097, 2, kPuzzleGeneralMenu, "but_d4n.bmp", Common::Point(261, 414), true, kDrawType1, 2300);
	_app->objectSetPuzzleAccessibilityKey(kObject99097, 0, Common::KEYCODE_ESCAPE);
	_app->objectAdd(kObject25, "", "", 1);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 0, kPuzzleMenu, "dat01_1.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 1, kPuzzleMenu, "dat02_2.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 2, kPuzzleMenu, "dat03_2.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 3, kPuzzleMenu, "dat04_3.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 4, kPuzzleMenu, "dat05_3.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 5, kPuzzleMenu, "dat06_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 6, kPuzzleMenu, "dat07_5.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 7, kPuzzleMenu, "dat08_6.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 8, kPuzzleMenu, "dat09_7.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 9, kPuzzleMenu, "dat10_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 10, kPuzzleMenu, "dat11_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 11, kPuzzleMenu, "dat12_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	_app->objectAddPresentation(kObject25);
	_app->objectPresentationAddImageToPuzzle(kObject25, 12, kPuzzleMenu, "dat13_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);

	_app->visualListAddToPuzzle(3, kPuzzlePreferences, 129,
	                            "", "", "", "", "ar_uph.tga", "", "ar_upn.tga", "", "ar_dnh.tga", "", "ar_dnn.tga", "", "",
	                            kDrawType3, Common::Point(0, 0), Common::Point(0, 0), 230, 67, 170, 20, 20, 3,
	                            Common::Point(418, 67), 408, 57, 40, 40, Common::Point(418, 276), 408, 266,
	                            40, 40, Common::Point(0, 0), kImageTypeBMP, kDrawType1, 375, 30, 11,
	                            Color(245, 235, 50), Color(255, 255, 255), Color(-1, -1, -1), kFontDefault, kArchiveFile);

	_app->visualListAddToPuzzle(1, kPuzzleGeneralMenu, 129,
	                            "", "", "", "", "ar_uph.tga", "", "ar_upn.tga", "", "ar_dnh.tga", "", "ar_dnn.tga", "", "",
	                            kDrawType3, Common::Point(0, 0), Common::Point(0, 0), 230, 222, 150, 20, 20, 3,
	                            Common::Point(418, 224), 408, 214, 40, 40, Common::Point(418, 351), 408, 341,
	                            40, 40, Common::Point(0, 0), kImageTypeBMP, kDrawType1, 170, 300, 7,
	                            Color(245, 235, 50), Color(255, 255, 255), Color(-1, -1, -1), kFontDefault, kArchiveFile);

	_app->visualListSetOff(1, kPuzzleGeneralMenu);

	_app->visualListAddToPuzzle(4, kPuzzleGeneralMenu, 65,
	                            "", "", "", "", "ar_uph.tga", "", "ar_upn.tga", "", "ar_dnh.tga", "", "ar_dnn.tga", "", "",
	                            kDrawType3, Common::Point(0, 0), Common::Point(0, 0), 230, 222, 150, 20, 20, 3,
	                            Common::Point(418, 224), 408, 214, 40, 40, Common::Point(418, 351), 408, 341,
	                            40, 40, Common::Point(240, 82), kImageTypeBMP, kDrawType1, 170, 300, 7,
	                            Color(245, 235, 50), Color(255, 255, 255), Color(-1, -1, -1), kFontDefault, kArchiveFile);

	_app->visualListSetOff(4, kPuzzleGeneralMenu);

	_app->soundAdd(90002, kSoundTypeAmbientEffect, "2199.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(90003, kSoundTypeAmbientEffect, "2200.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(90004, kSoundTypeAmbientEffect, "2201.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(90005, kSoundTypeAmbientEffect, "2202.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(90001, kSoundTypeAmbientMusic, "2203.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(90001, 90);
	_app->soundAdd(90708, kSoundTypeAmbientMusic, "2204.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->puzzleAddAmbientSound(kPuzzleGeneralMenu, 90001, 100, 0, 1, 1, 10);
	_app->soundAdd(90801, kSoundTypeDialog, "1124.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90802, kSoundTypeDialog, "2205.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90803, kSoundTypeDialog, "2206.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90804, kSoundTypeDialog, "2207.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90805, kSoundTypeDialog, "2208.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90806, kSoundTypeDialog, "2209.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90301, kSoundTypeDialog, "2210.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90302, kSoundTypeDialog, "2211.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90303, kSoundTypeDialog, "2212.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90304, kSoundTypeDialog, "2213.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90305, kSoundTypeDialog, "2214.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90306, kSoundTypeDialog, "2215.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90307, kSoundTypeDialog, "2216.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90308, kSoundTypeDialog, "2217.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90309, kSoundTypeDialog, "2218.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90401, kSoundTypeDialog, "2219.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90402, kSoundTypeDialog, "2220.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90403, kSoundTypeDialog, "2221.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90404, kSoundTypeDialog, "2222.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90405, kSoundTypeDialog, "2223.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90406, kSoundTypeDialog, "2224.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90407, kSoundTypeDialog, "2225.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90408, kSoundTypeDialog, "2226.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90409, kSoundTypeDialog, "2227.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90410, kSoundTypeDialog, "2228.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90411, kSoundTypeDialog, "2229.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90412, kSoundTypeDialog, "2230.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90501, kSoundTypeDialog, "2231.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90502, kSoundTypeDialog, "2232.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90503, kSoundTypeDialog, "2233.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90504, kSoundTypeDialog, "2234.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90505, kSoundTypeDialog, "2235.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90506, kSoundTypeDialog, "2236.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90507, kSoundTypeDialog, "2237.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90601, kSoundTypeDialog, "2238.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90602, kSoundTypeDialog, "2239.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90603, kSoundTypeDialog, "2240.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90604, kSoundTypeDialog, "2241.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90605, kSoundTypeDialog, "2242.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90606, kSoundTypeDialog, "2243.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90701, kSoundTypeDialog, "2244.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90702, kSoundTypeDialog, "2245.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90703, kSoundTypeDialog, "2246.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90704, kSoundTypeDialog, "2247.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90705, kSoundTypeDialog, "2248.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90706, kSoundTypeDialog, "2249.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90707, kSoundTypeDialog, "2250.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(90800, kSoundTypeDialog, "1125.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineByte(97001, 0);
	_app->varDefineByte(98001, _app->getSaveManager()->getData()->progressState);
	_app->varDefineByte(98002, 0);
	_app->varDefineByte(97002, 0);
	_app->varDefineByte(97006, 1);
	_app->varDefineByte(97007, 0);
	_app->varDefineByte(97008, 0);
	_app->varDefineByte(97009, 0);
	_app->varDefineByte(97010, 0);
	_app->varDefineByte(97011, 0);
	_app->varDefineByte(97012, 1);

	for (uint32 i = 0; i < 100; i++)
		_app->varDefineByte(98090 + i, 0);

	for (uint32 i = 0; i < 18; i++)
		_app->varDefineByte(99000 + i, 0);

	_app->varDefineByte(98004, 0);
	_app->varDefineByte(98003, 0);
	_app->varDefineByte(98005, 0);
	_app->varDefineByte(98006, 0);
	_app->varDefineByte(98007, 0);
	_app->varDefineByte(98008, 0);
	_app->varDefineByte(98009, 0);
	_app->varDefineByte(98010, 0);
	_app->varDefineByte(98011, 0);
	_app->varDefineByte(98087, 0);
	_app->varDefineByte(98088, 0);
	_app->varDefineWord(97013, 0);
	_app->varDefineDword(90025, 0);
	_app->varDefineDword(90028, 0);
	_app->varDefineDword(90027, 0);
	_app->varDefineDword(90026, 0);
	_app->varDefineByte(98012, 0);
	_app->varDefineByte(98013, 0);
	_app->varDefineByte(98014, 0);
	_app->varDefineByte(98015, 0);
	_app->varDefineByte(98016, 0);
	_app->varDefineByte(98089, 0);
}

void ZoneSystemFaust::onTimer(TimerId id) {
	if (id == kTimer12) {
		_app->timerStop(kTimer12);
		_app->varSetByte(97006, 1);
		_app->objectPresentationHide(kObject14, 7);
		_app->objectPresentationShow(kObject14, 8);
	}
}

void ZoneSystemFaust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[ZoneSystemFaust::onButtonUp] Not implemented");
}

void ZoneSystemFaust::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	if (id == kObject22 && target == 2) {
		_app->objectPresentationShow(kObject12, 2);
		_app->varSetByte(114101, 1);
	}
}

void ZoneSystemFaust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	if (animationId == 2 && frame == _app->getSaveManager()->getData()->progress)
		_app->objectPresentationPauseAnimation(kObject99090, 0);
}

void ZoneSystemFaust::onSound(Id id, SoundType type, uint32 a3, bool process) {
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
		_app->sub_468290(3);
		break;
	}
}

void ZoneSystemFaust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[ZoneSystemFaust::onBag] Not implemented");
}

void ZoneSystemFaust::onUpdateBag(const Common::Point &point) {
	if (_app->hasCurrentPuzzle() && _app->getCurrentPuzzleId() == kPuzzleMenu) {
		_app->objectPresentationHide(kObject14, 2);
		_app->objectPresentationHide(kObject14, 4);
		_app->objectPresentationHide(kObject14, 6);
	}

	_app->objectPresentationHide(kObject99091, 1);
	_app->objectPresentationHide(kObject99091, 2);
	_app->objectPresentationHide(kObject99091, 3);
	_app->objectPresentationHide(kObject2, 2);
	_app->objectPresentationHide(kObject2, 1);
	_app->objectPresentationHide(kObject4, 1);
	_app->objectPresentationHide(kObject4, 2);
	_app->objectPresentationHide(kObject3, 1);
	_app->objectPresentationHide(kObject99500);
	_app->objectPresentationHide(kObject99010, 2);
	_app->objectPresentationHide(kObject99010, 4);
	_app->objectPresentationHide(kObject99000, 0);

	if (!_app->varGetByte(98003))
		_app->objectPresentationHide(kObject99001, 0);

	if (!_app->varGetByte(98004))
		_app->objectPresentationHide(kObject99002, 0);

	if (!_app->varGetByte(98005))
		_app->objectPresentationHide(kObject99003, 0);

	if (!_app->varGetByte(98006))
		_app->objectPresentationHide(kObject99004, 0);

	if (!_app->varGetByte(98007))
		_app->objectPresentationHide(kObject99005, 0);

	if (!_app->varGetByte(98008))
		_app->objectPresentationHide(kObject99006, 0);

	if (!_app->varGetByte(98009))
		_app->objectPresentationHide(kObject99007, 0);

	if (!_app->varGetByte(98010))
		_app->objectPresentationHide(kObject99008, 0);

	if (!_app->varGetByte(98011))
		_app->objectPresentationHide(kObject99009, 0);

	_app->objectPresentationHide(kObject99023, 0);
	_app->objectPresentationHide(kObject99022, 0);
	_app->objectPresentationHide(kObject99021, 0);
	_app->objectPresentationHide(kObject99025, 0);
	_app->objectPresentationHide(kObject99024, 0);
	_app->objectPresentationHide(kObject99042, 0);
	_app->objectPresentationHide(kObject99041, 0);
	_app->objectPresentationHide(kObject99097, 0);
	_app->objectPresentationHide(kObject55, 0);
	_app->objectPresentationHide(kObject55, 2);
}

void ZoneSystemFaust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
	error("[ZoneSystemFaust::onUpdateBefore] Not implemented");
}

void ZoneSystemFaust::onVisualList(Id id, uint32 type, const Common::Point &) {
	switch (id) {
	default:
		break;

	case 1:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzleGeneralMenu);
			_app->objectPresentationSetTextToPuzzle(kObject99601, 1, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99601, 1, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(width + 233, 384));
		}
		break;

	case 3:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzlePreferences);
			_app->objectPresentationSetTextToPuzzle(kObject99501, 0, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99501, 0, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99501, 0, Common::Point(width + 233, 334));
		}
		break;

	case 4:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzleGeneralMenu);
			_app->objectPresentationSetTextToPuzzle(kObject99601, 1, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99601, 1, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(width + 233, 384));

			_app->objectSetAccessibilityOn(kObject99023);
			_app->objectPresentationHide(kObject99023, 1);
			_app->objectPresentationShow(kObject99023, 2);
		}
		break;
	}
}

} // End of namespace Ring
