/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 4
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 04110-1301, USA.
 */

#include "ring/game/faust/faust_zone4.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone4Faust::Zone4Faust(ApplicationFaust *application) : _app(application) {
}

Zone4Faust::~Zone4Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone4Faust::onInit() {
	_app->setCurrentZone(kZone4);

	_app->puzzleAdd(kPuzzle13101);
	_app->puzzleAddBackgroundImage(kPuzzle13101, "a01s03n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13102);
	_app->puzzleAddBackgroundImage(kPuzzle13102, "a01s03n01p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13110);
	_app->puzzleAddBackgroundImage(kPuzzle13110, "a01s03n01p01l01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13111);
	_app->puzzleAddBackgroundImage(kPuzzle13111, "a01s03n01p01l02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13112);
	_app->puzzleAddBackgroundImage(kPuzzle13112, "a01s03n01p02l01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13113);
	_app->puzzleAddBackgroundImage(kPuzzle13113, "a01s03n01p02l02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13105);
	_app->puzzleAddBackgroundImage(kPuzzle13105, "a01s03n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13106);
	_app->puzzleAddBackgroundImage(kPuzzle13106, "a01s03n02p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13107);
	_app->puzzleAddBackgroundImage(kPuzzle13107, "a01s04n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13108);
	_app->puzzleAddBackgroundImage(kPuzzle13108, "a01s04n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13109);
	_app->puzzleAddBackgroundImage(kPuzzle13109, "a01s06n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13201);
	_app->puzzleAddBackgroundImage(kPuzzle13201, "a01s03n02p04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13202);
	_app->puzzleAddBackgroundImage(kPuzzle13202, "a01s03n02p05.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13203);
	_app->puzzleAddBackgroundImage(kPuzzle13203, "a01s03n02p06.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13901);
	_app->puzzleAddBackgroundImage(kPuzzle13901, "a01s03n01p06.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13902);
	_app->puzzleAddBackgroundImage(kPuzzle13902, "a01s03n01p06l03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle13204);
	_app->puzzleAddBackgroundImage(kPuzzle13204, "a01s03n01p06l04.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(13311, "1137", 0, 14);
	_app->rotationSetCompressionBufferLength(13311, 1800000);
	_app->rotationAdd(13321, "1138", 0, 6);
	_app->rotationSetCompressionBufferLength(13321, 1000000);
	_app->rotationAdd(13312, "1139", 0, 12);
	_app->rotationSetCompressionBufferLength(13312, 1200000);
	_app->rotationAdd(13322, "1140", 0, 1);
	_app->rotationAdd(13411, "1141", 0, 2);
	_app->rotationAdd(13421, "1142", 0, 11);
	_app->rotationSetCompressionBufferLength(13421, 2000000);
	_app->rotationAdd(13412, "1143", 0, 2);
	_app->rotationAdd(13422, "1144", 0, 5);
	_app->rotationAdd(13511, "1145", 0, 2);
	_app->rotationAdd(13521, "1146", 0, 1);
	_app->rotationAdd(13531, "1147", 0, 1);
	_app->rotationAdd(13611, "1148", 0, 5);
	_app->rotationSetCompressionBufferLength(13611, 1500000);
	_app->rotationAddMovabilityToRotation(13311, 13321, "", Common::Rect(1177, 61, 1543, 450), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13311, 0, 90.0f, 16.0f, 87.0f, 0.0f, 2, 90.0f, 16.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13321, 13311, "", Common::Rect(3025, -8, 3294, 348), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13321, 0, 270.0f, 16.0f, 87.0f, 0.0f, 2, 270.0f, 16.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(13321, kPuzzle13101, "", Common::Rect(868, 188, 1075, 443), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13321, 1, 53.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(13321, kPuzzle13105, "", Common::Rect(319, 415, 529, 518), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13321, 2, 0.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(13321, kPuzzle13106, "", Common::Rect(1311, 259, 1511, 359), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13321, 3, 90.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(13411, 13421, "", Common::Rect(3042, 1, 3299, 247), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13411, 0, 270.0f, 26.0f, 87.0f, 0.0f, 2, 270.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(13411, kPuzzle13107, "", Common::Rect(616, 456, 796, 593), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13411, 1, 25.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(13421, 13411, "", Common::Rect(1211, 50, 1624, 320), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13421, 0, 94.0f, 26.0f, 87.0f, 0.0f, 2, 94.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13421, 13321, "1002", Common::Rect(1618, -155, 1747, 355), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13421, 1, 150.0f, -12.0f, 87.0f, 0.0f, 2, 228.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13511, 13531, "", Common::Rect(3051, 78, 3350, 458), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13511, 0, 270.0f, 26.0f, 87.0f, 0.0f, 2, 270.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13511, 13421, "1002", Common::Rect(1183, 441, 1538, 573), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13511, 1, 90.0f, 26.0f, 87.0f, 0.0f, 2, 90.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13511, 13422, "1002", Common::Rect(1183, 441, 1538, 573), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13511, 2, 90.0f, 26.0f, 87.0f, 0.0f, 2, 90.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13521, 13531, "", Common::Rect(210, 48, 827, 505), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13521, 0, 0.0f, 26.0f, 87.0f, 0.0f, 2, 0.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13531, 13511, "", Common::Rect(1221, 1, 1562, 375), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13531, 0, 90.0f, 26.0f, 87.0f, 0.0f, 2, 90.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13531, 13521, "", Common::Rect(2081, 61, 2378, 413), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13531, 1, 180.0f, 26.0f, 87.0f, 0.0f, 2, 180.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13531, 13611, "", Common::Rect(2977, 59, 3299, 448), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13531, 2, 270.0f, 26.0f, 87.0f, 0.0f, 2, 270.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13611, 13531, "", Common::Rect(1037, 160, 1785, 521), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13611, 0, 90.0f, 26.0f, 87.0f, 0.0f, 2, 90.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13412, 13422, "", Common::Rect(2717, -38, 3188, 327), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13412, 0, 243.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(13412, kPuzzle13107, "", Common::Rect(634, 118, 740, 254), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13412, 1, 25.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(13422, 13412, "", Common::Rect(1260, -43, 1529, 298), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13422, 0, 85.0f, 0.0f, 87.0f, 0.0f, 2, 82.0f, 16.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13422, 13322, "1002", Common::Rect(1631, -373, 1743, 266), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13422, 1, 120.0f, -17.0f, 87.0f, 0.0f, 2, 228.0f, 26.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13312, 13322, "", Common::Rect(1212, -73, 1529, 338), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13312, 0, 90.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13312, 13422, "", Common::Rect(1624, 169, 1829, 315), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13312, 1, 130.0f, 20.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(13322, 13312, "", Common::Rect(3014, -115, 3274, 312), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13322, 0, 270.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(13322, kPuzzle13201, "", Common::Rect(898, 40, 1088, 280), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13322, 1, 53.0f, 13.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(13322, kPuzzle13202, "", Common::Rect(339, 233, 508, 359), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13322, 2, 0.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(13322, kPuzzle13203, "", Common::Rect(1328, -97, 1492, 5), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(13322, 3, 90.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(13322, 13422, "", Common::Rect(2634, 170, 2872, 333), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(13322, 4, 230.0f, 23.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle13101, 13321, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle13101, 0, 53.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle13105, 13321, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle13105, 0, 0.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle13106, 13321, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle13106, 0, 90.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle13201, 13322, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle13201, 0, 53.0f, 13.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle13202, 13322, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle13202, 0, 0.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle13203, 13322, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle13203, 0, 90.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle13902, kPuzzle13901, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->objectAdd(kObjectLotteryTicket2, "Lottery ticket", "A03_LotteryTicket", 1);
	_app->objectSetActiveCursor(kObjectLotteryTicket2, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectLotteryTicket2, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject13901, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13901, kPuzzle13901, Common::Rect(380, 244, 436, 306), false, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObject13901, kPuzzle13901, Common::Rect(343, 244, 369, 294), false, kCursorTake, 1);
	_app->objectAddPuzzleAccessibility(kObject13901, kPuzzle13901, Common::Rect(315, 259, 341, 286), false, kCursorTake, 2);
	_app->objectAddPuzzleAccessibility(kObject13901, kPuzzle13901, Common::Rect(290, 277, 317, 305), false, kCursorTake, 3);
	_app->objectAddPuzzleAccessibility(kObject13901, kPuzzle13901, Common::Rect(270, 309, 301, 347), false, kCursorTake, 4);
	_app->objectAdd(kObject13902, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13902, kPuzzle13204, Common::Rect(174, 37, 375, 283), false, kCursorAction, 0);
	_app->objectAddPresentation(kObject13902);

	uint32 index = 1;

	for (uint32 i = 1; i < 32; i++) {
		_app->objectAddPresentation(kObject13902);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13901, Common::String::format("a01s03n01p06s02.%04d.tga", index), Common::Point(246, 133), true, kDrawType3, 0);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(90, 40), true, kDrawType3, 900);
	}

	index++;

	for (uint32 i = 1; i < 32; i++) {
		_app->objectAddPresentation(kObject13902);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13901, Common::String::format("a01s03n01p06s02.%04d.tga", index), Common::Point(246, 133), true, kDrawType3, 100);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(137, 51), true, kDrawType3, 800);
	}

	index++;

	for (uint32 i = 1; i < 32; i++) {
		_app->objectAddPresentation(kObject13902);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13901, Common::String::format("a01s03n01p06s02.%04d.tga", index), Common::Point(246, 133), true, kDrawType3, 200);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(155, 39), true, kDrawType3, 700);
	}

	index++;

	for (uint32 i = 1; i < 32; i++) {
		_app->objectAddPresentation(kObject13902);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13901, Common::String::format("a01s03n01p06s02.%04d.tga", index), Common::Point(246, 133), true, kDrawType3, 300);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(172, 30), true, kDrawType3, 600);
	}

	index++;

	for (uint32 i = 1; i < 32; i++) {
		_app->objectAddPresentation(kObject13902);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13901, Common::String::format("a01s03n01p06s02.%04d.tga", index), Common::Point(246, 133), true, kDrawType3, 400);
		_app->objectPresentationAddImageToPuzzle(kObject13902, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(190, 19), true, kDrawType3, 500);
	}

	_app->objectAdd(kObject13900, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13900, kPuzzle13901, Common::Rect(0, 0, 320, 240), false, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject13900, kPuzzle13901, Common::Rect(50, 295, 141, 341), false, kCursorTake, 1);
	_app->objectAddPresentation(kObject13900);
	_app->objectPresentationAddImageToPuzzle(kObject13900, 0, kPuzzle13901, "a01s03n01p06l01.tga", Common::Point(354, 270), true, kDrawType3, 3000);
	_app->objectPresentationAddImageToPuzzle(kObject13900, 0, kPuzzle13901, "a01s03n01p06l02.tga", Common::Point(204, 152), true, kDrawType3, 3000);
	_app->objectPresentationAddTextToPuzzle(kObject13900, 0, kPuzzle13902, "", Common::Point(320, 220), kFontDefault, Color(255, 255, 255), Color(-1, -1, -1));

	for (int16 i = 0; i < 160; i += 16)
		_app->objectPresentationAddTextToPuzzle(kObject13900, 0, kPuzzle13901, "", Common::Point(420, i + 16), kFontDefault, Color(0, 255, 0), Color(-1, -1, -1));

	for (int16 i = 0; i < 160; i += 16)
		_app->objectPresentationAddTextToPuzzle(kObject13900, 0, kPuzzle13901, "", Common::Point(260, i + 16), kFontDefault, Color(0, 255, 0), Color(-1, -1, -1));

	index = 1;

	for (uint32 i = 1; i < 33; i++) {
		_app->objectAddPresentation(kObject13900);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13901, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(347, 229), true, kDrawType3, 900);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(362, 249), true, kDrawType3, 900);
	}

	index++;

	for (uint32 i = 1; i < 33; i++) {
		_app->objectAddPresentation(kObject13900);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13901, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(338, 235), true, kDrawType3, 800);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(343, 255), true, kDrawType3, 800);
	}

	index++;

	for (uint32 i = 1; i < 33; i++) {
		_app->objectAddPresentation(kObject13900);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13901, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(310, 234), true, kDrawType3, 700);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(305, 254), true, kDrawType3, 700);
	}

	index++;

	for (uint32 i = 1; i < 33; i++) {
		_app->objectAddPresentation(kObject13900);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13901, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(282, 246), true, kDrawType3, 600);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(272, 261), true, kDrawType3, 600);
	}

	index++;

	for (uint32 i = 1; i < 33; i++) {
		_app->objectAddPresentation(kObject13900);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13901, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(263, 268), true, kDrawType3, 500);
		_app->objectPresentationAddImageToPuzzle(kObject13900, index, kPuzzle13902, Common::String::format("a01s03n01p06s01.%d%03d.tga", index, i), Common::Point(238, 288), true, kDrawType3, 500);
	}

	_app->objectPresentationShow(kObject13900, 0);
	_app->varDefineByte(13900, 0);

	int8 offset = 7;
	int8 offset2 = 4;
	for (int8 i = 0; i < 33; i++) {
		_app->varDefineByte(13902 + i, (byte)(13825 + i)); // TODO check validity
		_app->varDefineByte(13802 + i, offset);
		_app->varDefineByte(13842 + i, offset2);

		++offset;
		if (offset > 14) {
			--offset2;
			offset = 7;
		}
	}

	for (uint32 i = 0; i < 10; i++) {
		_app->varDefineByte(13940 + i, 0);
		_app->varDefineByte(13950 + i, 0);
		_app->varDefineByte(13960 + i, 0);
	}

	_app->varDefineByte(13971, 0);
	_app->varDefineByte(13972, 0);
	_app->varDefineByte(13973, 0);
	_app->varDefineByte(13974, 0);
	_app->varDefineByte(13975, 0);
	_app->varDefineByte(13976, 0);
	_app->varDefineByte(13977, 0);
	_app->varDefineByte(13978, 0);
	_app->varDefineByte(13979, 0);
	_app->objectAdd(kObject13903, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13903, kPuzzle13901, Common::Rect(0, 0, 640, 480), true, kCursorIdle, 0);
	_app->objectAddPresentation(kObject13903);
	_app->objectAdd(kObject13101, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13101, 13311, Common::Rect(2874, -38, 3582, 460), true, kCursorMove, 0);
	_app->objectAdd(kObject13102, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject13102, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject13102, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 40; i++) {
		_app->objectAddPresentation(kObject13102);
		_app->objectPresentationAddImageToPuzzle(kObject13102, i, kPuzzle13102, Common::String::format("a01s03n01p03s01a.%04d.bmp", i + 1), Common::Point(171, 168), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject13103, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject13103, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject13103, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 40; i++) {
		_app->objectAddPresentation(kObject13103);
		_app->objectPresentationAddImageToPuzzle(kObject13103, i, kPuzzle13102, Common::String::format("a01s03n01p03s01b.%04d.bmp", i + 1), Common::Point(233, 166), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject13104, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject13104, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject13104, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 40; i++) {
		_app->objectAddPresentation(kObject13104);
		_app->objectPresentationAddImageToPuzzle(kObject13104, i, kPuzzle13102, Common::String::format("a01s03n01p03s01c.%04d.bmp", i + 1), Common::Point(286, 168), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject13105, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject13105, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject13105, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 40; i++) {
		_app->objectAddPresentation(kObject13105);
		_app->objectPresentationAddImageToPuzzle(kObject13105, i, kPuzzle13102, Common::String::format("a01s03n01p03s01d.%04d.bmp", i + 1), Common::Point(339, 167), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject13106, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject13106, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject13106, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 40; i++) {
		_app->objectAddPresentation(kObject13106);
		_app->objectPresentationAddImageToPuzzle(kObject13106, i, kPuzzle13102, Common::String::format("a01s03n01p03s01e.%04d.bmp", i + 1), Common::Point(386, 170), true, kDrawType1, 1000);
	}

	_app->objectAddPuzzleAccessibility(kObject13102, kPuzzle13102, Common::Rect(170, 169, 224, 271), true, kCursorDragDrop, 0);
	_app->objectAddPuzzleAccessibility(kObject13102, kPuzzle13102, Common::Rect(231, 164, 273, 270), true, kCursorDragDrop, 1);
	_app->objectAddPuzzleAccessibility(kObject13102, kPuzzle13102, Common::Rect(283, 170, 328, 269), true, kCursorDragDrop, 2);
	_app->objectAddPuzzleAccessibility(kObject13102, kPuzzle13102, Common::Rect(338, 169, 384, 271), true, kCursorDragDrop, 3);
	_app->objectAddPuzzleAccessibility(kObject13102, kPuzzle13102, Common::Rect(387, 172, 440, 266), true, kCursorDragDrop, 4);
	_app->objectAdd(kObject13107, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13107, 13321, Common::Rect(2371, 505, 2548, 563), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject13107);
	_app->objectPresentationAddImageToRotation(kObject13107, 0, 13321, 4);
	_app->objectAddPresentation(kObject13107);
	_app->objectPresentationAddAnimationToRotation(kObject13107, 1, 13321, 5, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13107, 1, 0, 13001);
	_app->objectAdd(kObject13108, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13108, 13321, Common::Rect(2061, 387, 2475, 486), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject13108);
	_app->objectPresentationAddImageToRotation(kObject13108, 0, 13321, 2);
	_app->objectAddPresentation(kObject13108);
	_app->objectPresentationAddImageToRotation(kObject13108, 1, 13321, 3);
	_app->objectAddPresentation(kObject13108);
	_app->objectPresentationAddAnimationToRotation(kObject13108, 2, 13321, 5, 10, 12.5f, 6);
	_app->objectAddPresentation(kObject13108);
	_app->objectPresentationAddImageToRotation(kObject13108, 3, 13311, 0);
	_app->objectAddPresentation(kObject13108);
	_app->objectPresentationAddImageToRotation(kObject13108, 4, 13311, 2);
	_app->objectAdd(kObject13113, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13113, 13311, Common::Rect(1655, 348, 1720, 394), false, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject13113, 13311, Common::Rect(1749, 371, 1814, 408), false, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject13113, 13321, Common::Rect(2675, 381, 2747, 423), false, kCursorZoom, 2);
	_app->objectAddRotationAccessibility(kObject13113, 13321, Common::Rect(2692, 343, 2749, 380), false, kCursorZoom, 3);
	_app->objectAddRotationAccessibility(kObject13113, 13311, Common::Rect(1810, 430, 1886, 493), false, kCursorZoom, 4);
	_app->objectAddRotationAccessibility(kObject13113, 13311, Common::Rect(1944, 446, 2010, 495), false, kCursorZoom, 5);
	_app->objectAddRotationAccessibility(kObject13113, 13321, Common::Rect(2808, 319, 2866, 354), false, kCursorZoom, 6);
	_app->objectAddRotationAccessibility(kObject13113, 13321, Common::Rect(2800, 287, 2859, 317), false, kCursorZoom, 7);
	_app->objectAddPuzzleAccessibility(kObject13113, kPuzzle13112, Common::Rect(0, 420, 640, 464), true, kCursorBack, 8);
	_app->objectAddPuzzleAccessibility(kObject13113, kPuzzle13113, Common::Rect(0, 420, 640, 464), true, kCursorBack, 9);
	_app->objectAdd(kObject13109, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13109, 13311, Common::Rect(1634, 305, 1858, 453), false, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13109, 13321, Common::Rect(2620, 329, 2865, 451), false, kCursorAction, 1);
	_app->objectAddPresentation(kObject13109);
	_app->objectPresentationAddAnimationToRotation(kObject13109, 0, 13311, 13, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13109, 0, 0, 13003);
	_app->objectAddPresentation(kObject13109);
	_app->objectPresentationAddAnimationToRotation(kObject13109, 1, 13321, 1, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13109, 1, 0, 13003);
	_app->objectAdd(kObject13110, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13110, 13311, Common::Rect(1631, 341, 1800, 472), false, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject13110, 13321, Common::Rect(2649, 359, 2847, 470), false, kCursorZoom, 1);
	_app->objectAddPuzzleAccessibility(kObject13110, kPuzzle13102, Common::Rect(0, 420, 640, 464), true, kCursorBack, 2);
	_app->objectAdd(kObjectRing, "Ring", "A03_Ring", 1);
	_app->objectSetActiveCursor(kObjectRing, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectRing, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectStChristopherMedallion, "St. Christopher Medallion", "A03_Medallion", 1);
	_app->objectSetActiveCursor(kObjectStChristopherMedallion, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectStChristopherMedallion, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject13139, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13139, kPuzzle13110, Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->objectAddPuzzleAccessibility(kObject13139, kPuzzle13111, Common::Rect(0, 420, 640, 464), true, kCursorBack, 1);
	_app->objectAdd(kObject13111, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13111, 13311, Common::Rect(2214, 511, 2469, 530), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13111, 13311, Common::Rect(2272, 538, 2364, 577), false, kCursorTake, 1);
	_app->objectAddRotationAccessibility(kObject13111, 13311, Common::Rect(2383, 561, 2431, 575), false, kCursorTake, 2);
	_app->objectAddRotationAccessibility(kObject13111, 13311, Common::Rect(2343, 602, 2413, 618), false, kCursorAction, 3);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 0, 13311, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 0, 0, 13004);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddImageToRotation(kObject13111, 1, 13311, 3);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 2, 13311, 9, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 2, 0, 13004);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddImageToRotation(kObject13111, 3, 13311, 5);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 4, 13311, 10, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 4, 0, 13004);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddImageToRotation(kObject13111, 5, 13311, 4);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 6, 13311, 11, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 6, 0, 13004);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddImageToRotation(kObject13111, 7, 13311, 6);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 8, 13311, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 8, 0, 13005);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 9, 13311, 9, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 9, 0, 13005);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 10, 13311, 10, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 10, 0, 13005);
	_app->objectAddPresentation(kObject13111);
	_app->objectPresentationAddAnimationToRotation(kObject13111, 11, 13311, 11, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13111, 11, 0, 13005);
	_app->objectAdd(kObject13112, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13112, 13311, Common::Rect(2228, 558, 2464, 572), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13112, 13311, Common::Rect(2322, 556, 2431, 582), false, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject13112, 13311, Common::Rect(2355, 641, 2411, 651), false, kCursorAction, 2);
	_app->objectAddPresentation(kObject13112);
	_app->objectPresentationAddAnimationToRotation(kObject13112, 0, 13311, 12, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13112, 0, 0, 13006);
	_app->objectAddPresentation(kObject13112);
	_app->objectPresentationAddImageToRotation(kObject13112, 1, 13311, 7);
	_app->objectAddPresentation(kObject13112);
	_app->objectPresentationAddAnimationToRotation(kObject13112, 2, 13311, 12, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13112, 2, 0, 13007);
	_app->objectAdd(kObject13114, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13114, kPuzzle13101, Common::Rect(246, 250, 400, 397), true, kCursorZoom, 0);
	_app->objectAddPuzzleAccessibility(kObject13114, kPuzzle13201, Common::Rect(246, 250, 400, 397), true, kCursorZoom, 1);
	_app->objectAdd(kObject13115, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13115, 13311, Common::Rect(2260, -50, 2603, 287), true, kCursorZoom, 0);
	_app->objectAdd(kObject13116, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13116, kPuzzle13105, Common::Rect(189, 154, 266, 214), true, kCursorAction, 0);
	_app->objectAddPuzzleAccessibility(kObject13116, kPuzzle13105, Common::Rect(396, 160, 465, 222), true, kCursorAction, 1);
	_app->objectAddPuzzleAccessibility(kObject13116, kPuzzle13105, Common::Rect(298, 263, 346, 419), false, kCursorZoom, 2);
	_app->objectAddPresentation(kObject13116);
	_app->objectPresentationAddAnimationToPuzzle(kObject13116, 0, kPuzzle13105, "a01s03n02p01s01", kImageTypeBMP, Common::Point(290, 251), kDrawType1, 1000, 2, 15.0f, 4);
	_app->objectAddPresentation(kObject13116);
	_app->objectPresentationAddAnimationToPuzzle(kObject13116, 1, kPuzzle13105, "a01s03n02p01s02", kImageTypeBMP, Common::Point(300, 247), kDrawType1, 1000, 3, 15.0f, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObject13116, 1, 0, 13024);
	_app->objectAddPresentation(kObject13116);
	_app->objectPresentationAddAnimationToPuzzle(kObject13116, 2, kPuzzle13105, "a01s03n02p01s03", kImageTypeBMP, Common::Point(296, 254), kDrawType1, 1000, 2, 15.0f, 4);
	_app->objectAdd(kObject13121, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13121, kPuzzle13106, Common::Rect(270, 212, 368, 325), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject13121, kPuzzle13107, Common::Rect(271, 233, 388, 380), true, kCursorAction, 1);
	_app->objectAddPuzzleAccessibility(kObject13121, kPuzzle13107, Common::Rect(  0, 420, 640, 464), true, kCursorBack, 2);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 0, 13321, 0);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 1, 13311, 8);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 2, 13411, 0);
	_app->objectPresentationShow(kObject13121, 2);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 3, 13421, 0);
	_app->objectPresentationShow(kObject13121, 3);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToPuzzle(kObject13121, 4, kPuzzle13107, "a01s04n01p01l01.bmp", Common::Point(281, 241), true, kDrawType1, 1000);
	_app->objectAdd(kObjectWoodLog, "Wood log", "A03_Log", 1);
	_app->objectSetActiveCursor(kObjectWoodLog, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectWoodLog, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectWoodLog, 13321, Common::Rect(1230, 371, 1548, 514), true, kCursorTake, 0);
	_app->objectAddRotationAccessibility(kObjectWoodLog, 13411, Common::Rect(1167, 437, 1582, 573), false, kCursorTake, 1);
	_app->objectAdd(kObjectOldNew, "Old newspaper", "A03_OldPaper", 1);
	_app->objectSetActiveCursor(kObjectOldNew, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectOldNew, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectOldNew, 13421, Common::Rect(1664, 518, 1863, 589), true, kCursorTake, 0);
	_app->objectAdd(kObject13123, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13123, 13411, Common::Rect(3062, 275, 3592, 538), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject13123, 13421, Common::Rect(1089, 181, 1376, 427), true, kCursorZoom, 1);
	_app->objectAdd(kObject13124, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13124, 13421, Common::Rect(363, 174, 529, 320), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13124, 13421, Common::Rect(330, 486, 506, 551), true, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject13124, 13421, Common::Rect(488, 144, 632, 294), false, kCursorAction, 2);
	_app->objectAddRotationAccessibility(kObject13124, 13421, Common::Rect(492, 500, 590, 577), false, kCursorAction, 3);
	_app->objectAddRotationAccessibility(kObject13124, 13421, Common::Rect(363, 174, 529, 320), false, kCursorActionObject, 4);
	_app->objectAddPresentation(kObject13124);
	_app->objectPresentationAddAnimationToRotation(kObject13124, 0, 13421, 1, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13124, 0, 0, 13008);
	_app->objectAddPresentation(kObject13124);
	_app->objectPresentationAddAnimationToRotation(kObject13124, 1, 13421, 1, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13124, 1, 0, 13009);
	_app->objectAddPresentation(kObject13124);
	_app->objectPresentationAddImageToRotation(kObject13124, 2, 13421, 2);
	_app->objectAddPresentation(kObject13124);
	_app->objectPresentationAddAnimationToRotation(kObject13124, 3, 13421, 8, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13124, 3, 0, 13010);
	_app->objectAddPresentation(kObject13124);
	_app->objectPresentationAddAnimationToRotation(kObject13124, 4, 13421, 8, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13124, 4, 0, 13011);
	_app->objectAddPresentation(kObject13124);
	_app->objectPresentationAddImageToRotation(kObject13124, 5, 13421, 3);
	_app->objectAdd(kObjectBucketWithMolasse, "Bucket with molasse", "A03_FullBucket", 1);
	_app->objectSetActiveCursor(kObjectBucketWithMolasse, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBucketWithMolasse, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectBucket, "Bucket", "A03_Bucket", 1);
	_app->objectSetActiveCursor(kObjectBucket, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBucket, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObjectBucket);
	_app->objectPresentationAddImageToRotation(kObjectBucket, 0, 13411, 1);
	_app->objectAddRotationAccessibility(kObjectBucket, 13411, Common::Rect(43, 523, 165, 593), true, kCursorTake, 0);
	_app->objectAdd(kObject13127, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13127, kPuzzle13108, Common::Rect(157, 178, 487, 352), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject13127, kPuzzle13108, Common::Rect(0, 420, 640, 464), true, kCursorBack, 1);
	_app->objectAddPresentation(kObject13127);
	_app->objectPresentationAddImageToPuzzle(kObject13127, 0, kPuzzle13108, "a01s04n02p01l02.bmp", Common::Point(115, 145), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13127);
	_app->objectPresentationAddImageToPuzzle(kObject13127, 1, kPuzzle13108, "a01s04n02p01l01.bmp", Common::Point(115, 146), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13127);
	_app->objectPresentationAddImageToPuzzle(kObject13127, 2, kPuzzle13108, "a01s04n02p01l03.bmp", Common::Point(116, 148), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject13127);
	_app->objectPresentationAddAnimationToPuzzle(kObject13127, 3, kPuzzle13108, "a01s04n02p01s02", kImageTypeBMP, Common::Point(118, 145), kDrawType1, 1000, 40, 25.0f, 4);
	_app->objectAddPresentation(kObject13127);
	_app->objectPresentationAddAnimationToPuzzle(kObject13127, 4, kPuzzle13108, "a01s04n02p01s01", kImageTypeBMP, Common::Point(108, 137), kDrawType1, 1000, 40, 25.0f, 4);
	_app->objectAdd(kObjectMatches, "Matches", "A05_Matches", 1);
	_app->objectSetActiveCursor(kObjectMatches, Common::Point(11, 27), 5, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMatches, Common::Point(11, 27), 5, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectAdd(kObject13130, "", "", 1);
	_app->objectAdd(kObjectTap, "Tap", "A03_Tap", 1);
	_app->objectSetActiveCursor(kObjectTap, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectTap, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3127,  306, 3218, 367), true, kCursorActionObject, 0);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3287,  455, 3410, 502), true, kCursorActionObject, 1);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3211, -127, 3459, 116), true, kCursorAction, 2);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3046,  151, 3301, 368), true, kCursorAction, 3);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3401,  132, 3587, 293), true, kCursorAction, 4);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(2891,  348, 3109, 526), true, kCursorAction, 5);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3215,  373, 3485, 477), true, kCursorAction, 6);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3522,  310, 3600, 397), true, kCursorAction, 7);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(   0,  310,   86, 397), true, kCursorAction, 8);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(2912,  188, 3032, 463), false, kCursorAction, 9);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3032,  441, 3216, 589), false, kCursorAction, 10);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(   0,    0,    0,   0), false, kCursorAction, 11);
	_app->objectAddRotationAccessibility(kObject13130, 13421, Common::Rect(3215,  373, 3485, 477), false, kCursorMove, 12);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddImageToRotation(kObject13130, 0, 13421, 4);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddAnimationToRotation(kObject13130, 1, 13421, 9, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13130, 1, 0, 13012);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddAnimationToRotation(kObject13130, 2, 13421, 9, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13130, 2, 0, 13013);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddImageToRotation(kObject13130, 3, 13421, 5);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddImageToRotation(kObject13130, 4, 13421, 6);
	_app->objectPresentationShow(kObject13130, 4);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddAnimationToRotation(kObject13130, 5, 13421, 10, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13130, 5, 0, 13014);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddAnimationToRotation(kObject13130, 6, 13421, 10, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13130, 6, 0, 13015);
	_app->objectAddPresentation(kObject13130);
	_app->objectPresentationAddImageToRotation(kObject13130, 7, 13421, 7);
	_app->objectAdd(kObjectBottle, "Bottle", "A03_EmptyBottle", 1);
	_app->objectSetActiveCursor(kObjectBottle, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBottle, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectBottleOfWhiskey, "Bottle of whiskey", "A03_BottleOfWhisky", 1);
	_app->objectSetActiveCursor(kObjectBottleOfWhiskey, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBottleOfWhiskey, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectBottleOfMolasse, "Bottle of molasse", "A03_FullBottle", 1);
	_app->objectSetActiveCursor(kObjectBottleOfMolasse, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBottleOfMolasse, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectBottle, 13421, Common::Rect(722, 310, 1051, 495), true, kCursorTake, 0);
	_app->objectAdd(kObjectBaseballBat, "Baseball Bat", "A03_BaseballBat", 1);
	_app->objectSetActiveCursor(kObjectBaseballBat, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBaseballBat, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectBaseballBat, 13511, Common::Rect(2025, 467, 2177, 615), true, kCursorTake, 0);
	_app->objectAddPresentation(kObjectBaseballBat);
	_app->objectPresentationAddImageToRotation(kObjectBaseballBat, 0, 13511, 0);
	_app->objectAdd(kObject13136, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13136, 13511, Common::Rect(3575, 455, 3600, 561), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject13136, 13511, Common::Rect(0, 455, 237, 561), true, kCursorZoom, 1);
	_app->objectAdd(kObject13137, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13137, 13611, Common::Rect( 290, 155,  416, 352), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13137, 13611, Common::Rect( 441, 134,  599, 327), true, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject13137, 13611, Common::Rect(2847, -31, 3425, 460), true, kCursorZoom, 2);
	_app->objectAddRotationAccessibility(kObject13137, 13611, Common::Rect( 583, 179,  785, 420), true, kCursorZoom, 3);
	_app->objectAddPuzzleAccessibility(kObject13137, kPuzzle13109, Common::Rect(0, 420, 640, 464), true, kCursorBack, 4);
	_app->objectAddPuzzleAccessibility(kObject13137, kPuzzle13109, Common::Rect(242, 172, 343, 358), true, kCursorZoom, 5);
	_app->objectAddPresentation(kObject13137);
	_app->objectPresentationAddAnimationToRotation(kObject13137, 0, 13611, 3, 30, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13137, 0, 0, 13016);
	_app->objectAddPresentation(kObject13137);
	_app->objectPresentationAddAnimationToRotation(kObject13137, 1, 13611, 4, 30, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13137, 1, 0, 13017);
	_app->objectAddPresentation(kObject13137);
	_app->objectPresentationAddAnimationToRotation(kObject13137, 2, 13611, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13137, 2, 0, 13018);
	_app->objectAddPresentation(kObject13137);
	_app->objectPresentationAddAnimationToRotation(kObject13137, 3, 13611, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13137, 3, 0, 13019);
	_app->objectAddPresentation(kObject13137);
	_app->objectPresentationAddImageToRotation(kObject13137, 4, 13611, 0);
	_app->objectAdd(kObject13138, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13138, 13521, Common::Rect(2441, 345, 2659, 573), false, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject13138, 13521, Common::Rect(2441, 345, 2659, 573), true, kCursorAction, 1);
	_app->objectAdd(kObject13140, "", "", 1);
	_app->objectAddPresentation(kObject13140);
	_app->objectPresentationAddImageToRotation(kObject13140, 0, 13521, 0);
	_app->objectAddPresentation(kObject13140);
	_app->objectPresentationAddImageToRotation(kObject13140, 1, 13511, 1);
	_app->objectAddPresentation(kObject13140);
	_app->objectPresentationAddImageToRotation(kObject13140, 2, 13531, 0);
	_app->objectAdd(kObject13201, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13201, 13422, Common::Rect(351, -282, 515, -209), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13201, 13422, Common::Rect(330,  134, 497,  238), true, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject13201, 13422, Common::Rect(492, -320, 601, -212), false, kCursorAction, 2);
	_app->objectAddRotationAccessibility(kObject13201, 13422, Common::Rect(  0,    0,   0,    0), false, kCursorAction, 3);
	_app->objectAddRotationAccessibility(kObject13201, 13422, Common::Rect(351, -282, 515, -209), false, kCursorActionObject, 4);
	_app->objectAddPresentation(kObject13201);
	_app->objectPresentationAddAnimationToRotation(kObject13201, 0, 13422, 1, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13201, 0, 0, 13008);
	_app->objectAddPresentation(kObject13201);
	_app->objectPresentationAddAnimationToRotation(kObject13201, 1, 13422, 1, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13201, 1, 0, 13009);
	_app->objectAddPresentation(kObject13201);
	_app->objectPresentationAddImageToRotation(kObject13201, 2, 13422, 2);
	_app->objectAddPresentation(kObject13201);
	_app->objectPresentationAddAnimationToRotation(kObject13201, 3, 13422, 4, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13201, 3, 0, 13010);
	_app->objectAddPresentation(kObject13201);
	_app->objectPresentationAddAnimationToRotation(kObject13201, 4, 13422, 4, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13201, 4, 0, 13011);
	_app->objectAddPresentation(kObject13201);
	_app->objectPresentationAddImageToRotation(kObject13201, 5, 13422, 3);
	_app->objectAddRotationAccessibility(kObjectBottle, 13422,  Common::Rect( 782, 130, 1102, 347), true, kCursorTake, 1);
	_app->objectAddRotationAccessibility(kObjectWoodLog, 13412, Common::Rect(1188, 158, 1529, 413), false, kCursorTake, 2);
	_app->objectAddPuzzleAccessibility(kObject13121, kPuzzle13203, Common::Rect(270, 212, 368, 325), true, kCursorZoom, 3);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 5, 13412, 0);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 6, 13422, 0);
	_app->objectPresentationShow(kObject13121, 5);
	_app->objectPresentationShow(kObject13121, 6);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 7, 13312, 6);
	_app->objectAddPresentation(kObject13121);
	_app->objectPresentationAddImageToRotation(kObject13121, 8, 13322, 0);
	_app->objectAddRotationAccessibility(kObjectOldNew, 13422, Common::Rect(1659, 289, 1870, 390), true, kCursorTake, 1);
	_app->objectAdd(kObject13202, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13202, 13422, Common::Rect(3046, -188, 3308,  64), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13202, 13422, Common::Rect(3404, -190, 3582,  48), true, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject13202, 13422, Common::Rect(2877,   69, 3113, 345), true, kCursorAction, 2);
	_app->objectAddRotationAccessibility(kObject13202, 13422, Common::Rect(3227,   68, 3461, 296), true, kCursorAction, 3);
	_app->objectAddRotationAccessibility(kObject13202, 13422, Common::Rect(3512,   61, 3600, 228), true, kCursorAction, 4);
	_app->objectAddRotationAccessibility(kObject13202, 13422, Common::Rect(   0,   61,   73, 228), true, kCursorAction, 5);
	_app->objectAddPresentation(kObjectBucket);
	_app->objectPresentationAddImageToRotation(kObjectBucket, 1, 13412, 1);
	_app->objectAddRotationAccessibility(kObjectBucket, 13412, Common::Rect(36, 308, 237, 429), true, kCursorTake, 1);
	_app->objectAdd(kObject13203, "", "", 1);
	_app->objectAddPresentation(kObject13203);
	_app->objectPresentationAddAnimationToRotation(kObject13203, 0, 13312, 11, 12, 7.0f, 5);
	_app->objectPresentationShow(kObject13203, 0);
	_app->objectAdd(kObject13204, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13204, kPuzzle13202, Common::Rect(189, 154, 266, 214), true, kCursorAction, 0);
	_app->objectAddPuzzleAccessibility(kObject13204, kPuzzle13202, Common::Rect(396, 160, 465, 222), true, kCursorAction, 1);
	_app->objectAddPuzzleAccessibility(kObject13204, kPuzzle13202, Common::Rect(298, 263, 346, 419), false, kCursorZoom, 2);
	_app->objectAddPresentation(kObject13204);
	_app->objectPresentationAddAnimationToPuzzle(kObject13204, 0, kPuzzle13202, "a01s03n02p05s01", kImageTypeBMP, Common::Point(288, 248), kDrawType1, 1000, 2, 15.0f, 4);
	_app->objectAddPresentation(kObject13204);
	_app->objectPresentationAddAnimationToPuzzle(kObject13204, 1, kPuzzle13202, "a01s03n02p05s02", kImageTypeBMP, Common::Point(281, 246), kDrawType1, 1000, 3, 15.0f, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObject13204, 1, 0, 13025);
	_app->objectAddPresentation(kObject13204);
	_app->objectPresentationAddAnimationToPuzzle(kObject13204, 2, kPuzzle13202, "a01s03n02p05s03", kImageTypeBMP, Common::Point(292, 248), kDrawType1, 1000, 2, 15.0f, 4);
	_app->objectAdd(kObject13205, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13205, 13312, Common::Rect(731, 34, 803, 130), true, kCursorActionObject, 0);
	_app->objectAdd(kObject13206, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13206, 13312, Common::Rect(2244, 273, 2443, 326), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13206, 13312, Common::Rect(2262, 313, 2369, 362), false, kCursorTake, 1);
	_app->objectAddRotationAccessibility(kObject13206, 13312, Common::Rect(2362, 322, 2420, 350), false, kCursorTake, 2);
	_app->objectAddRotationAccessibility(kObject13206, 13312, Common::Rect(2248, 359, 2501, 402), false, kCursorAction, 3);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 0, 13312, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 0, 0, 13004);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddImageToRotation(kObject13206, 1, 13312, 0);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 2, 13312, 7, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 2, 0, 13004);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddImageToRotation(kObject13206, 3, 13312, 3);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 4, 13312, 8, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 4, 0, 13004);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddImageToRotation(kObject13206, 5, 13312, 2);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 6, 13312, 9, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 6, 0, 13004);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddImageToRotation(kObject13206, 7, 13312, 4);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 8, 13312, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 8, 0, 13005);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 9, 13312, 7, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 9, 0, 13005);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 10, 13312, 8, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 10, 0, 13005);
	_app->objectAddPresentation(kObject13206);
	_app->objectPresentationAddAnimationToRotation(kObject13206, 11, 13312, 9, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13206, 11, 0, 13005);
	_app->objectAdd(kObject13207, "", "", 1);
	_app->objectAddRotationAccessibility(kObject13207, 13312, Common::Rect(2242, 359, 2446, 397), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject13207, 13312, Common::Rect(2315, 366, 2441, 451), false, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject13207, 13312, Common::Rect(2251, 441, 2499, 497), false, kCursorAction, 2);
	_app->objectAddPresentation(kObject13207);
	_app->objectPresentationAddAnimationToRotation(kObject13207, 0, 13312, 10, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject13207, 0, 0, 13006);
	_app->objectAddPresentation(kObject13207);
	_app->objectPresentationAddImageToRotation(kObject13207, 1, 13312, 5);
	_app->objectAddPresentation(kObject13207);
	_app->objectPresentationAddAnimationToRotation(kObject13207, 2, 13312, 10, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject13207, 2, 0, 13007);
	_app->objectAdd(kObject13801, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject13801, kPuzzle13112, Common::Rect(248, 154, 392, 249), true, kCursorIdle, 0);
	_app->objectAddPuzzleAccessibility(kObject13801, kPuzzle13113, Common::Rect(248, 154, 392, 249), true, kCursorIdle, 1);
	_app->objectAddPuzzleAccessibility(kObject13801, kPuzzle13110, Common::Rect(196, 107, 469, 248), true, kCursorIdle, 2);
	_app->objectAddPuzzleAccessibility(kObject13801, kPuzzle13111, Common::Rect(196, 107, 469, 248), true, kCursorIdle, 3);
	_app->soundAdd(13054, kSoundTypeBackgroundMusic, "1149.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(13054, 85);
	_app->soundAdd(13055, kSoundTypeBackgroundMusic, "1150.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(13055, 80);
	_app->soundAdd(13074, kSoundTypeBackgroundMusic, "1151.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(13042, kSoundTypeAmbientEffect, "1153.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13010, kSoundTypeAmbientEffect, "1154.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13009, kSoundTypeAmbientEffect, "1155.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13017, kSoundTypeAmbientEffect, "1156.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13023, kSoundTypeAmbientEffect, "1157.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13001, kSoundTypeAmbientEffect, "1158.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13071, kSoundTypeAmbientEffect, "1159.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13002, kSoundTypeAmbientEffect, "1160.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13076, kSoundTypeAmbientEffect, "1161.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13065, kSoundTypeAmbientEffect, "1162.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13037, kSoundTypeAmbientEffect, "1163.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13033, kSoundTypeAmbientEffect, "1164.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13040, kSoundTypeAmbientEffect, "1165.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13048, kSoundTypeAmbientEffect, "1166.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13038, kSoundTypeAmbientEffect, "1167.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13039, kSoundTypeAmbientEffect, "1168.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13047, kSoundTypeAmbientEffect, "1169.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13016, kSoundTypeAmbientEffect, "1170.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13041, kSoundTypeAmbientEffect, "1171.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13005, kSoundTypeAmbientEffect, "1172.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13025, kSoundTypeAmbientEffect, "1173.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13004, kSoundTypeAmbientEffect, "1174.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13070, kSoundTypeAmbientEffect, "1175.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13018, kSoundTypeAmbientEffect, "1176.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13024, kSoundTypeAmbientEffect, "1177.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13003, kSoundTypeAmbientEffect, "1178.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13006, kSoundTypeAmbientEffect, "1179.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13007, kSoundTypeAmbientEffect, "1180.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13008, kSoundTypeAmbientEffect, "1181.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13011, kSoundTypeAmbientEffect, "1182.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13012, kSoundTypeAmbientEffect, "1183.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13013, kSoundTypeAmbientEffect, "1184.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13014, kSoundTypeAmbientEffect, "1185.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13015, kSoundTypeAmbientEffect, "1186.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13019, kSoundTypeAmbientEffect, "1187.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13020, kSoundTypeAmbientEffect, "1188.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13021, kSoundTypeAmbientEffect, "1189.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13026, kSoundTypeAmbientEffect, "1190.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13027, kSoundTypeAmbientEffect, "1191.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13028, kSoundTypeAmbientEffect, "1192.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13029, kSoundTypeAmbientEffect, "1193.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13032, kSoundTypeAmbientEffect, "1194.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(13032, 85);
	_app->soundAdd(13045, kSoundTypeAmbientEffect, "1195.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13046, kSoundTypeAmbientEffect, "1196.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13049, kSoundTypeAmbientEffect, "1197.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13050, kSoundTypeAmbientEffect, "1198.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13036, kSoundTypeAmbientEffect, "1199.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13043, kSoundTypeAmbientEffect, "1200.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13044, kSoundTypeAmbientEffect, "1201.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13051, kSoundTypeAmbientEffect, "1202.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13052, kSoundTypeAmbientEffect, "1203.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13053, kSoundTypeAmbientEffect, "1204.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13056, kSoundTypeAmbientEffect, "1205.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13057, kSoundTypeAmbientEffect, "1206.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13058, kSoundTypeAmbientEffect, "1207.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13059, kSoundTypeAmbientEffect, "1208.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13060, kSoundTypeAmbientEffect, "1209.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13061, kSoundTypeAmbientEffect, "1210.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13062, kSoundTypeAmbientEffect, "1211.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13063, kSoundTypeAmbientEffect, "1212.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13064, kSoundTypeAmbientEffect, "1213.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13066, kSoundTypeAmbientEffect, "1214.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13067, kSoundTypeAmbientEffect, "1215.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13068, kSoundTypeAmbientEffect, "1216.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13069, kSoundTypeAmbientEffect, "1217.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13072, kSoundTypeAmbientEffect, "1218.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(13072, 90);
	_app->soundAdd(13073, kSoundTypeAmbientEffect, "1219.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13075, kSoundTypeAmbientEffect, "1220.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13077, kSoundTypeAmbientEffect, "1221.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13078, kSoundTypeAmbientEffect, "1221.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(13411, 13010, 1, 1, 15, 90, 290.0f, 10);
	_app->rotationAdd3DSound(13421, 13010, 1, 1, 15, 100, 360.0f, 10);
	_app->rotationAdd3DSound(13412, 13010, 1, 1, 15, 90, 290.0f, 10);
	_app->rotationAdd3DSound(13422, 13010, 1, 1, 15, 100, 360.0f, 10);
	_app->puzzleAdd3DSound(13107, 13010, 1, 1, 15, 90, 250.0f, 10);
	_app->puzzleAdd3DSound(13108, 13010, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationSet3DSoundOff(13411, 13010);
	_app->rotationSet3DSoundOff(13421, 13010);
	_app->rotationSet3DSoundOff(13412, 13010);
	_app->rotationSet3DSoundOff(13422, 13010);
	_app->puzzleSet3DSoundOff(13107, 13010);
	_app->puzzleSet3DSoundOff(13108, 13010);
	_app->rotationAdd3DSound(13611, 13017, 1, 1, 15, 85, 130.0f, 10);
	_app->rotationAdd3DSound(13531, 13017, 1, 1, 15, 95, 180.0f, 10);
	_app->rotationAdd3DSound(13521, 13017, 1, 1, 15, 100, 180.0f, 10);
	_app->rotationAdd3DSound(13511, 13017, 1, 1, 15, 90, 242.0f, 10);
	_app->puzzleAdd3DSound(13109, 13017, 1, 1, 15, 85, 250.0f, 10);
	_app->rotationSet3DSoundOff(13611, 13017);
	_app->rotationSet3DSoundOff(13531, 13017);
	_app->rotationSet3DSoundOff(13521, 13017);
	_app->rotationSet3DSoundOff(13511, 13017);
	_app->puzzleSet3DSoundOff(13109, 13017);
	_app->rotationAdd3DSound(13611, 13073, 1, 1, 15, 90, 183.0f, 10);
	_app->rotationAdd3DSound(13531, 13073, 1, 1, 15, 80, 261.0f, 10);
	_app->rotationAdd3DSound(13521, 13073, 1, 1, 15, 75, 310.0f, 10);
	_app->rotationAdd3DSound(13511, 13073, 1, 1, 15, 75, 270.0f, 10);
	_app->puzzleAdd3DSound(13109, 13073, 1, 1, 15, 90, 250.0f, 10);
	_app->rotationAdd3DSound(13611, 13021, 1, 1, 15, 95, 183.0f, 10);
	_app->rotationAdd3DSound(13531, 13021, 1, 1, 15, 85, 261.0f, 10);
	_app->rotationAdd3DSound(13521, 13021, 1, 1, 15, 80, 310.0f, 10);
	_app->rotationAdd3DSound(13511, 13021, 1, 1, 15, 80, 270.0f, 10);
	_app->puzzleAdd3DSound(13109, 13021, 1, 1, 15, 95, 250.0f, 10);
	_app->soundAdd(13301, kSoundTypeDialog, "1223.WAV", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13302, kSoundTypeDialog, "1224.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13303, kSoundTypeDialog, "1225.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13304, kSoundTypeDialog, "1226.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13305, kSoundTypeDialog, "1227.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13306, kSoundTypeDialog, "1228.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13307, kSoundTypeDialog, "1229.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13308, kSoundTypeDialog, "1230.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13309, kSoundTypeDialog, "1231.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13310, kSoundTypeDialog, "1232.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13311, kSoundTypeDialog, "1233.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13312, kSoundTypeDialog, "1234.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13313, kSoundTypeDialog, "1235.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13314, kSoundTypeDialog, "1236.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13315, kSoundTypeDialog, "1237.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13316, kSoundTypeDialog, "1238.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13317, kSoundTypeDialog, "1239.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13318, kSoundTypeDialog, "1240.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13319, kSoundTypeDialog, "1241.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13320, kSoundTypeDialog, "1242.WAV", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13321, kSoundTypeDialog, "1243.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13322, kSoundTypeDialog, "1244.WAV", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13323, kSoundTypeDialog, "1245.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13324, kSoundTypeDialog, "1246.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13325, kSoundTypeDialog, "1247.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13326, kSoundTypeDialog, "1248.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13327, kSoundTypeDialog, "1249.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13328, kSoundTypeDialog, "1250.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13329, kSoundTypeDialog, "1251.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13330, kSoundTypeDialog, "1252.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13331, kSoundTypeDialog, "1253.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13332, kSoundTypeDialog, "1254.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13333, kSoundTypeDialog, "1255.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13334, kSoundTypeDialog, "1256.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13336, kSoundTypeDialog, "1257.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13337, kSoundTypeDialog, "1258.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(13338, kSoundTypeDialog, "1259.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineByte(13018, 1);
	_app->varDefineByte(13001, 0);
	_app->varDefineByte(13002, 0);
	_app->varDefineByte(13003, 0);
	_app->varDefineByte(13004, 0);
	_app->varDefineByte(13005, 0);
	_app->varDefineByte(13006, 0);
	_app->varDefineByte(13007, 0);
	_app->varDefineByte(13008, 0);
	_app->varDefineByte(13009, 0);
	_app->varDefineByte(13010, 0);
	_app->varDefineByte(13012, 0);
	_app->varDefineByte(13011, 0);
	_app->varDefineByte(13013, 1);
	_app->varDefineByte(13014, 0);
	_app->varDefineByte(13015, 0);
	_app->varDefineByte(13016, 0);
	_app->varDefineByte(13017, 0);
	_app->varDefineByte(13019, 0);
	_app->varDefineByte(13020, 0);
	_app->varDefineByte(13021, 0);
	_app->varDefineByte(13022, 0);
	_app->varDefineByte(13023, 0);
	_app->varDefineByte(13024, 0);
	_app->varDefineByte(13025, 0);
	_app->varDefineByte(13026, 0);
	_app->varDefineByte(13027, 0);
	_app->varDefineByte(13028, 0);
	_app->varDefineByte(13029, 0);
	_app->varDefineWord(13001, 0);
	_app->varDefineWord(13002, 0);
	_app->varDefineWord(13003, 0);
	_app->varDefineDword(13001, 0);
	_app->varDefineFloat(13001, 12.0f);
	_app->varDefineFloat(13002, 29.0f);
	_app->varDefineFloat(13003, 16.0f);
	_app->varDefineFloat(13004, 7.0f);
	_app->varDefineFloat(13005, 33.0f);
	_app->objectPresentationShow(kObject13102, (uint32)_app->varGetFloat(13001));
	_app->objectPresentationShow(kObject13103, (uint32)_app->varGetFloat(13002));
	_app->objectPresentationShow(kObject13104, (uint32)_app->varGetFloat(13003));
	_app->objectPresentationShow(kObject13105, (uint32)_app->varGetFloat(13004));
	_app->objectPresentationShow(kObject13106, (uint32)_app->varGetFloat(13005));
}

void Zone4Faust::onSetup(SetupType type) {
	error("[EventSetupFaust::onSetupZone4] Not implemented");
}

void Zone4Faust::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->objectPresentationHide(kObject13121, 4);
		_app->timerStop(kTimer0);
		break;

	case kTimer1:
		if (_app->varGetByte(13019) == 5)
			_app->timerStop(kTimer1);

		_app->soundPlay(_app->varGetByte(13019) + 13322);
		_app->varSetByte(13019, _app->varGetByte(13019) + 1);
		break;

	case kTimer2:
		_app->timerStop(kTimer2);
		_app->soundPlay(13327);
		break;

	case kTimer3:
		_app->varSetByte(13900, _app->varGetByte(13900) + 1);
		_app->varGetByte(13900);
		if (_app->varGetByte(13900) == 5)
			_app->timerStop(kTimer3);
		break;

	case kTimer4:
		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, rnd(5000) + 10000);
		_app->objectPresentationShow(kObject13140);
		_app->timerStart(kTimer5, 100);
		_app->soundPlay(13072);
		break;

	case kTimer5:
		_app->timerStop(kTimer5);
		_app->objectPresentationHide(kObject13140);
		break;
	}
}

void Zone4Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone4Faust::onButtonUp] Not implemented");
}

void Zone4Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone4] Not implemented");
}

void Zone4Faust::onSound(Id id, SoundType type, uint32 a3, bool process) {
	error("[Zone4Faust::onSound] Not implemented");
}

void Zone4Faust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[Zone4Faust::onBag] Not implemented");
}

void Zone4Faust::onUpdateBag(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(13001), 1024);
	_app->varSetDword(13001, 0);
}

void Zone4Faust::onBagClickedObject(ObjectId id) {
	if (id == kObjectMatches)
		_app->soundPlay(13033);
}

void Zone4Faust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
	error("[Zone4Faust::onUpdateBefore] Not implemented");
}

void Zone4Faust::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityTo == 13611) {
			if (movabilityFrom == 13531)
				_app->timerStop(kTimer4);
		} else if (movabilityTo == 13531) {
			if (movabilityTo == 13611)
				_app->timerStart(kTimer4, rnd(4000));
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (movabilityFrom == 13902)
			_app->sub_4A2CF0();
		break;
	}
}

void Zone4Faust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	error("[Zone4Faust::onAfterRide] Not implemented");
}

} // End of namespace Ring
