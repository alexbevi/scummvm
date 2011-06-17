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

#include "ring/game/ring/ring_zoneas.h"

#include "ring/base/bag.h"
#include "ring/base/rotation.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneASRing::ZoneASRing(ApplicationRing *application) : _app(application) {
}

ZoneASRing::~ZoneASRing() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneASRing::onInit() {
	debugC(1, kRingDebugLogic, "Initializing Dril zone (AS)...");

	_app->setZoneAndEnableBag(kZoneAS);

	_app->setLoadFrom(kLoadFromDisk);
	_app->setArchiveType(kArchiveFile);

	_app->puzzleAdd(kPuzzle80001);
	_app->puzzleAddBackgroundImage(kPuzzle80001, "Old_Ish.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80011);
	_app->puzzleAddBackgroundImage(kPuzzle80011, "DRIL_02.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80012);
	_app->puzzleAddBackgroundImage(kPuzzle80012, "DRIL_03.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80013);
	_app->puzzleAddBackgroundImage(kPuzzle80013, "DRIL_05.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80014);
	_app->puzzleAddBackgroundImage(kPuzzle80014, "DRIL_06.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80002);
	_app->puzzleAddBackgroundImage(kPuzzle80002, "ASV01.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80003);
	_app->puzzleAddBackgroundImage(kPuzzle80003, "ASV02.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80004);
	_app->puzzleAddBackgroundImage(kPuzzle80004, "ASV03.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80005);
	_app->puzzleAddBackgroundImage(kPuzzle80005, "ASV04.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80008);
	_app->puzzleAddBackgroundImage(kPuzzle80008, "ASP04L01.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80010);
	_app->puzzleAddBackgroundImage(kPuzzle80010, "ASP05L01.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80009);
	_app->puzzleAddBackgroundImage(kPuzzle80009, "ASP03L01.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80006);
	_app->puzzleAddBackgroundImage(kPuzzle80006, "ASP01L01.bma", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle80007);
	_app->puzzleAddBackgroundImage(kPuzzle80007, "ASP02L01.bma", Common::Point(0, 16), true);

	_app->setLoadFrom(kLoadFromCd);
	if (_app->getConfiguration().artAS)
		_app->setArchiveType(kArchiveArt);

	_app->rotationAdd(80001, "ASS00N01", 0, 1);
	_app->rotationAdd(80002, "ASS00N02", 0, 1);
	_app->rotationAdd(80003, "ASS00N03", 0, 1);
	_app->rotationAdd(80004, "ASS00N04", 0, 1);
	_app->rotationAdd(80005, "ASS00N05", 0, 1);
	_app->rotationAdd(80006, "ASS00N06", 0, 1);
	_app->rotationAdd(80007, "ASS00N07", 0, 1);
	_app->rotationAdd(80008, "ASS00N08", 0, 1);
	_app->rotationAdd(80009, "ASS00N09", 0, 1);
	_app->rotationAdd(80010, "ASS00N10", 0, 1);
	_app->rotationAdd(80011, "ASS00N11", 0, 1);
	_app->rotationAdd(80013, "ASS00N13", 0, 1);
	_app->rotationAdd(80014, "ASS00N14", 0, 1);
	_app->rotationAdd(80015, "ASS00N15", 0, 1);

	_app->setLoadFrom(kLoadFromDisk);
	_app->rotationAdd(80101, "ASS01N01", 0, 6);
	_app->setLoadFrom(kLoadFromCd);

	_app->rotationSetCompressionBufferLength(80101, 1800000);
	_app->rotationAddMovabilityToRotation(80001, 80002, "1001", Common::Rect(757, -71, 928, 85), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80001, 0, 52.0f, 0, 85.3f, 0, 0, 52.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80001, 80003, "1002", Common::Rect(1251, -64, 1444, 95), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80001, 1, 90.0f, 0, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80001, 80004, "1003", Common::Rect(1784, -62, 1982, 90), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80001, 2, 128.0f, 0, 85.3f, 0, 0, 128.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80002, 80001, "1004", Common::Rect(2696, -167, 2956, 81), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80002, 0, 232.0f, 0, 85.3f, 0, 0, 232.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80002, 80003, "1005", Common::Rect(2130, -78, 2366, 130), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80002, 1, 182.0f, 0, 85.3f, 0, 0, 182.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80002, 80005, "1006", Common::Rect(1634, -64, 1910, 87), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80002, 2, 131.0f, 0, 85.3f, 0, 0, 131.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80002, "1007", Common::Rect(321, -69, 548, 108), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 0, 2.0f, 0, 85.3f, 0, 0, 2.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80005, "1008", Common::Rect(1168, -69, 1538, 106), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 1, 90.0f, 0, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80004, "1009", Common::Rect(2137, -66, 2394, 99), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 2, 178.0f, 0, 85.3f, 0, 0, 178.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80013, "1010", Common::Rect(2721, -317, 2882, -198), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 3, 231.0f, -26.0f, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80014, "1011", Common::Rect(3406, -319, 3578, -204), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 4, 309.0f, -26.0f, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80001, "1012", Common::Rect(3053, -97, 3246, 57), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 5, 270.0f, 0, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80101, "1013", Common::Rect(952, 326, 1798, 590), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 6, 90.0f, 26.0f, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80003, 80015, "1014", Common::Rect(1306, -270, 1397, -162), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80003, 7, 90.0f, -24.0f, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80004, 80001, "1015", Common::Rect(3387, -95, 3596, 69), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80004, 0, 308.0f, 0, 85.3f, 0, 0, 308.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80004, 80003, "1016", Common::Rect(374, -22, 541, 104), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80004, 1, 358.0f, 0, 85.3f, 0, 0, 358.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80004, 80005, "1017", Common::Rect(810, -27, 1089, 73), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80004, 2, 49.0f, 0, 85.3f, 0, 0, 49.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80003, "1018", Common::Rect(3053, -153, 3244, -6), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 0, 270.0f, 0, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80003, "1018", Common::Rect(2984, -24, 3090, 172), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 1, 270.0f, 0, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80003, "1018", Common::Rect(3230, -24, 3336, 172), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 2, 270.0f, 0, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80002, "1019", Common::Rect(3408, -115, 3600, 102), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 3, 311.0f, 0, 85.3f, 0, 0, 311.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80002, "1019", Common::Rect(0, -115, 135, 102), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 4, 311.0f, 0, 85.3f, 0, 0, 311.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80004, "1020", Common::Rect(2617, -71, 2870, 83), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 5, 229.0f, 0, 85.3f, 0, 0, 229.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80008, "1021", Common::Rect(1209, -109, 1313, 181), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 6, 90.0f, 0, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80005, 80008, "1021", Common::Rect(1397, -130, 1509, 181), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80005, 7, 90.0f, 0, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80006, 80007, "1022", Common::Rect(2026, 413, 2387, 586), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80006, 0, 180.0f, 26.0f, 85.3f, 0, 0, 180.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80006, 80011, "1023", Common::Rect(1740, -33, 1875, 102), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80006, 1, 137.0f, 0, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80007, 80006, "1024", Common::Rect(29, -540, 639, -388), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80007, 0, 0, -26.0f, 85.3f, 0, 0, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80007, 80008, "1025", Common::Rect(2047, -34, 2427, 163), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80007, 1, 180.0f, 0, 85.3f, 0, 0, 180.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80008, 80007, "1026", Common::Rect(305, -48, 622, 122), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80008, 0, 0, 0, 85.3f, 0, 0, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80008, 80011, "1027", Common::Rect(1274, -272, 1429, 50), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80008, 1, 90.0f, -26.0f, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80008, 80009, "1028", Common::Rect(2102, -71, 2431, 136), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80008, 2, 180.0f, 0, 85.3f, 0, 0, 180.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80008, 80005, "1029", Common::Rect(3000, -73, 3100, 218), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80008, 3, 270.0f, 0, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80008, 80005, "1029", Common::Rect(3200, -73, 3300, 218), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80008, 4, 270.0f, 0, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80008, 80005, "1029", Common::Rect(3100, -73, 3200, -34), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80008, 5, 270.0f, 0, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80009, 80008, "1031", Common::Rect(333, -33, 678, 176), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80009, 0, 0, 0, 85.3f, 0, 0, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80009, 80010, "1032", Common::Rect(1745, -495, 2478, -369), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80009, 1, 180.0f, -26.0f, 85.3f, 0, 0, 180.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80010, 80009, "1033", Common::Rect(182, 422, 745, 552), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80010, 0, 0, 26.0f, 85.3f, 0, 0, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80010, 80011, "1034", Common::Rect(849, -83, 1091, 104), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80010, 1, 43.0f, 0, 85.3f, 0, 0, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80011, 80006, "1035", Common::Rect(3531, -47, 3600, 95), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80011, 0, 317.0f, 0, 85.3f, 0, 0, 317.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80011, 80006, "1035", Common::Rect(0, -47, 249, 95), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80011, 1, 317.0f, 0, 85.3f, 0, 0, 317.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80011, 80008, "1036", Common::Rect(3009, 280, 3320, 469), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80011, 2, 270.0f, 26.0f, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80011, 80010, "1037", Common::Rect(2571, -29, 2784, 80), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80011, 3, 223.0f, 0, 85.3f, 0, 0, 223.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80013, 80014, "1038", Common::Rect(342, -55, 444, 47), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80013, 0, 0, 0, 85.3f, 0, 0, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80013, 80015, "1039", Common::Rect(1149, -31, 1225, 27), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80013, 1, 73.0f, 0, 85.3f, 0, 0, 73.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80013, 80003, "1040", Common::Rect(892, 244, 1045, 348), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80013, 2, 51.0f, 26.0f, 85.3f, 0, 0, 51.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80014, 80013, "1041", Common::Rect(2253, -57, 2366, 52), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80014, 0, 180.0f, 0, 85.3f, 0, 0, 180.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80014, 80015, "1042", Common::Rect(1478, -41, 1550, 34), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80014, 1, 107.0f, 0, 85.3f, 0, 0, 107.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80014, 80003, "1043", Common::Rect(1668, 238, 1814, 359), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80014, 2, 129.0f, 26.0f, 85.3f, 0, 0, 129.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80015, 80013, "1044", Common::Rect(2951, -36, 3019, 34), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80015, 0, 253.0f, 0, 85.3f, 0, 0, 253.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80015, 80014, "1045", Common::Rect(3276, -29, 3355, 29), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80015, 1, 287.0f, 0, 85.3f, 0, 0, 287.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(80015, 80003, "1046", Common::Rect(3079, 212, 3236, 345), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(80015, 2, 270.0f, 26.0f, 85.3f, 0, 0, 270.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80002, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80002, 0, 51.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80003, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80003, 0, 75.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80004, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80004, 0, 102.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80005, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80005, 0, 130.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80006, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80006, 0, 90.0f, 17.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80007, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80007, 0, 90.0f, 17.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80008, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80008, 0, 90.0f, 17.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80009, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80009, 0, 90.0f, 17.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle80010, 80101, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle80010, 0, 90.0f, 17.0f, 85.3f);
	_app->objectAdd(kObject80017, "", "", 1);
	_app->objectAdd(kObject80007, "", "", 1);
	_app->objectAddRotationAccessibility(kObject80007, 80101, Common::Rect(1314, 183, 1397, 226), true, kCursorHotspot, 0);
	_app->objectAdd(kObjectDeath, "Death", "AS_Nor_d", 1);
	_app->objectAddBagAnimation(kObjectDeath, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectDeath, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectDeath, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObject80012, "", "", 1);
	_app->objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(3097, -1, 3197, 176),   true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(1318, -66, 1381, 191),  true, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject80012, 80008, Common::Rect(3095, -27, 3215, 73),   true, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(384, -61, 478, 197),    true, kCursorHotspot, 3);
	_app->objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(2225, -59, 2308, 190),  true, kCursorHotspot, 4);
	_app->objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(467, -195, 606, 3),     true, kCursorHotspot, 5);
	_app->objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(2088, -167, 2235, 8),   true, kCursorHotspot, 6);
	_app->objectAddRotationAccessibility(kObject80012, 80011, Common::Rect(1202, -156, 1552, 355), true, kCursorHotspot, 7);
	_app->objectAdd(kObject80016, "", "", 1);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 0, 80001, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 1, 80002, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 2, 80003, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 3, 80004, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 4, 80005, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 5, 80006, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 6, 80007, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 7, 80008, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 8, 80009, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 9, 80010, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 10, 80011, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 11, 80013, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 12, 80014, 0);
	_app->objectAddPresentation(kObject80016);
	_app->objectPresentationAddImageToRotation(kObject80016, 13, 80015, 0);
	_app->objectAdd(kObject80018, "", "", 1);
	_app->objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(843, -66, 1008, 188),   false, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(1147, -104, 1277, 136), false, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(1418, -97, 1562, 130),  false, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(1698, -99, 1849, 186),  false, kCursorHotspot, 3);
	_app->objectAddPresentation(kObject80018);
	_app->objectPresentationAddImageToRotation(kObject80018, 0, 80101, 0);
	_app->objectAddPresentation(kObject80018);
	_app->objectPresentationAddAnimationToRotation(kObject80018, 1, 80101, 1, 49, 12.5f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject80018, 1, 0, 80001);
	_app->objectPresentationShow(kObject80018, 1);
	_app->objectPresentationPauseAnimation(kObject80018, 1);
	_app->objectAddPresentation(kObject80018);
	_app->objectPresentationAddImageToRotation(kObject80018, 2, 80101, 2);
	_app->objectAddPresentation(kObject80018);
	_app->objectPresentationAddImageToRotation(kObject80018, 3, 80101, 3);
	_app->objectAddPresentation(kObject80018);
	_app->objectPresentationAddImageToRotation(kObject80018, 4, 80101, 4);
	_app->objectAddPresentation(kObject80018);
	_app->objectPresentationAddImageToRotation(kObject80018, 5, 80101, 5);
	_app->objectAdd(kObject80019, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject80019, kPuzzle80006, Common::Rect(159, 161, 477, 393), true,  kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject80019, kPuzzle80006, Common::Rect(159, 161, 477, 393), false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject80019, kPuzzle80007, Common::Rect(159, 161, 477, 393), true,  kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject80019, kPuzzle80008, Common::Rect(159, 161, 477, 393), true,  kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject80019, kPuzzle80009, Common::Rect(159, 161, 477, 393), true,  kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject80019, kPuzzle80010, Common::Rect(159, 161, 477, 393), true,  kCursorHotspot, 5);
	_app->objectSetAccessibilityOff(kObject80019, 1, 1);
	_app->objectAdd(kObject80021, "", "", 1);
	_app->objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1248, 235, 1466, 348), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1089, 170, 1232, 268), true, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1181, 125, 1341, 190), true, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1353, 130, 1552, 183), true, kCursorHotspot, 3);
	_app->objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1444, 183, 1620, 249), true, kCursorHotspot, 4);
	_app->objectAddPresentation(kObject80019);

	_app->setLoadFrom(kLoadFromDisk);
	_app->setArchiveType(kArchiveFile);

	_app->objectPresentationAddImageToPuzzle(kObject80019, 0, kPuzzle80006, "ASP01L02.bma", Common::Point(0, 16), true, kDrawType1, 1000);

	_app->setLoadFrom(kLoadFromCd);
	if (_app->getConfiguration().artAS)
		_app->setArchiveType(kArchiveArt);

	_app->objectAdd(kObject80022, "", "", 1);
	_app->objectAddRotationAccessibility(kObject80022, 80101, Common::Rect(0, -580, 3600, -279), true, kCursorMove, 0);
	_app->soundAdd(80200, kSoundTypeAmbientMusic, "1048.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(80201, kSoundTypeAmbientMusic, "1049.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(80203, kSoundTypeAmbientMusic, "1050.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(80204, kSoundTypeAmbientMusic, "1051.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(80205, kSoundTypeAmbientMusic, "1052.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(80207, kSoundTypeAmbientMusic, "1053.wav", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->rotationAddAmbientSound(80001, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80002, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80003, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80004, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80005, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80006, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80007, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80008, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80009, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80010, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80011, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80013, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80014, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80015, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80101, 80200, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(80101, 80207, 100, 0, 1, 1, 20);
	_app->puzzleAddAmbientSound(kPuzzle80002, 80201, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80003, 80203, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80004, 80204, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80005, 80205, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80006, 80207, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80007, 80207, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80008, 80207, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80010, 80207, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80009, 80207, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80006, 80200, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80007, 80200, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80008, 80200, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80010, 80200, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle80009, 80200, 100, 0, 1, 1, 10);
	_app->soundAdd(80206, kSoundTypeAmbientEffect, "1053.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(80018, kSoundTypeAmbientEffect, "1054.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(80019, kSoundTypeAmbientEffect, "1055.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(80020, kSoundTypeAmbientEffect, "1056.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(80208, kSoundTypeAmbientEffect, "1057.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(80209, kSoundTypeAmbientEffect, "1058.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(80210, kSoundTypeAmbientEffect, "1059.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(80210, 90);
	_app->soundAdd(80004, kSoundTypeDialog, "1060.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80005, kSoundTypeDialog, "1061.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80006, kSoundTypeDialog, "1062.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80007, kSoundTypeDialog, "1063.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80008, kSoundTypeDialog, "1064.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80009, kSoundTypeDialog, "1065.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80010, kSoundTypeDialog, "1066.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80011, kSoundTypeDialog, "1067.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80012, kSoundTypeDialog, "1068.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80013, kSoundTypeDialog, "1069.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80014, kSoundTypeDialog, "1070.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80015, kSoundTypeDialog, "1071.was", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80040, kSoundTypeDialog, "1072.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80041, kSoundTypeDialog, "1073.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80042, kSoundTypeDialog, "1074.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80043, kSoundTypeDialog, "1075.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80044, kSoundTypeDialog, "1076.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80045, kSoundTypeDialog, "1077.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80046, kSoundTypeDialog, "1078.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80047, kSoundTypeDialog, "1079.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80048, kSoundTypeDialog, "1080.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80049, kSoundTypeDialog, "1081.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80050, kSoundTypeDialog, "1082.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80051, kSoundTypeDialog, "1083.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80052, kSoundTypeDialog, "1084.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80053, kSoundTypeDialog, "1085.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80054, kSoundTypeDialog, "1086.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80055, kSoundTypeDialog, "1087.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80056, kSoundTypeDialog, "1088.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80057, kSoundTypeDialog, "1089.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80058, kSoundTypeDialog, "1090.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80059, kSoundTypeDialog, "1091.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80060, kSoundTypeDialog, "1092.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80061, kSoundTypeDialog, "1093.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80062, kSoundTypeDialog, "1094.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80063, kSoundTypeDialog, "1095.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80064, kSoundTypeDialog, "1096.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80065, kSoundTypeDialog, "1097.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80066, kSoundTypeDialog, "1098.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80067, kSoundTypeDialog, "1099.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80068, kSoundTypeDialog, "1100.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80069, kSoundTypeDialog, "1101.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80070, kSoundTypeDialog, "1102.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80071, kSoundTypeDialog, "1103.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80072, kSoundTypeDialog, "1104.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80073, kSoundTypeDialog, "1105.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80074, kSoundTypeDialog, "1106.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80075, kSoundTypeDialog, "1107.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80076, kSoundTypeDialog, "1108.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80077, kSoundTypeDialog, "1109.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80078, kSoundTypeDialog, "1110.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80079, kSoundTypeDialog, "1111.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80080, kSoundTypeDialog, "1112.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80021, kSoundTypeDialog, "1125.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80022, kSoundTypeDialog, "1126.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80023, kSoundTypeDialog, "1127.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80024, kSoundTypeDialog, "1128.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80025, kSoundTypeDialog, "1129.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80026, kSoundTypeDialog, "1130.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80027, kSoundTypeDialog, "1131.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80028, kSoundTypeDialog, "1132.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80100, kSoundTypeDialog, "1133.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80101, kSoundTypeDialog, "1134.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80102, kSoundTypeDialog, "1135.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80103, kSoundTypeDialog, "1136.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80104, kSoundTypeDialog, "1137.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80105, kSoundTypeDialog, "1138.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80106, kSoundTypeDialog, "1139.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(80107, kSoundTypeDialog, "1140.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->rotationAdd3DSound(80001, 80206, 1, 1, 10, 90, 270.0f, 20);
	_app->varDefineByte(80000, 0);
	_app->varDefineByte(80001, 0);
	_app->varDefineByte(80002, 1);
	_app->varDefineByte(80003, 0);
	_app->varDefineByte(80004, 1);
	_app->varDefineByte(80005, 0);
	_app->varDefineFloat(80001, 1.0);
	_app->varDefineFloat(80002, 2.0);
	_app->varDefineFloat(80003, 100.0);
}

void ZoneASRing::onSetup(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupType5:
		_app->playMovie("1047");
		_app->rotationSetAlp(80003, 270.0f);
		_app->rotationSetBet(80003, 0.0f);
		_app->rotationSetRan(80003, 85.3f);
		_app->rotationSetActive(80003);
		break;

	case kSetupType6:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1162", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1161", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1160", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle80001);
		_app->soundPlay(80107);
		break;

	case kSetupType998:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1164", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1163", _app->getLanguageChannel());
			break;
		}

		_app->playMovie("1666");
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80100);
		break;

	case kSetupType999:
		_app->rotationSetAlp(80001, 90.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void ZoneASRing::onSwitch(uint32 type) {
	switch (type) {
	default:
		break;

	case 1:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 0, 0);
		_app->varSetByte(90001, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 2);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80040);
		_app->objectSetAccessibilityOff(kObject80019, 0, 0);
		_app->objectSetAccessibilityOn(kObject80019, 1, 1);
		break;

	case 2:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 1, 1);
		_app->varSetByte(90002, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 3);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80049);
		break;

	case 3:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->soundSetMultiplier(kSoundTypeAmbientMusic, 100);
		_app->objectSetAccessibilityOn(kObject80018, 2, 2);
		_app->varSetByte(90003, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);


		_app->objectPresentationShow(kObject80018, 4);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80058);
		break;

	case 4:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 3, 3);
		_app->varSetByte(90004, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 5);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80068);
		break;

	case 5:
		_app->setupZone(kZoneAS, kSetupType5);
		break;
	case 13:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->setZoneAndEnableBag(kZoneAS);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);

		_app->getBag()->reset();

		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void ZoneASRing::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer2:
		_app->soundPlay(80018);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 20);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer3:
		_app->soundPlay(80019);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 30);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer4:
		_app->soundPlay(80020);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 10);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004);
		break;

	case kTimer5:
		_app->varSetByte(80001, _app->varGetByte(80001) + 1);

		_app->getCurrentRotation()->setBet(_app->getCurrentRotation()->getBet() + _app->varGetFloat(80001) * _app->varGetFloat(80002));
		_app->getCurrentRotation()->setAlp(_app->getCurrentRotation()->getAlp() + _app->varGetFloat(80001) * _app->varGetFloat(80002) * 0.5f);

		// Update values
		_app->varSetFloat(80001, _app->varGetFloat(80001) * -1.0f);
		_app->varSetFloat(80002, _app->varGetFloat(80002) * 5.0f / 6.0f);

		if (_app->varGetByte(80001) > 50) {
			_app->varSetByte(80001, 0);
			_app->varSetFloat(80002, 2.0f);

			_app->timerStop(kTimer5);
			_app->timerStop(kTimer6);

			_app->objectPresentationHide(kObject80016);
		}
		break;

	case kTimer6: {
		int32 val = rnd(10) & 0x80000001;

		bool show = (val == 0);
		if (val < 0)
			show = (((val - 1) | -2) == -1);

		if (show)
			_app->objectPresentationShow(kObject80016);
		else
			_app->objectPresentationHide(kObject80016);
		}
		break;
	}
}

void ZoneASRing::onAnimationNextFrame(Id animationId, const Common::String &, uint32 frame, uint32) {
	if (animationId == 80001) {
		if ((int32)frame == _app->varGetByte(80004)) {
			_app->objectPresentationPauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOn(kObject80021, 0, 4);
			_app->soundStop(80210, 1024);
			_app->soundPlay(80209);
		}

		_app->varSetByte(80005, (int8)frame);
	}
}

void ZoneASRing::onButtonUp(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject80019:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (!_app->varGetByte(90001)) {
				_app->timerStopAll();
				_app->onSwitchZone(kZoneNI, 0);
			}
			break;

		case 1:
			if (!_app->varGetByte(90002)) {
				_app->timerStopAll();
				_app->onSwitchZone(kZoneN2, 0);
			}
			break;

		case 2:
			if (!_app->varGetByte(90001)) {
				_app->timerStopAll();
				_app->onSwitchZone(kZoneNI, 0);
			}
			break;

		case 3:
			if (!_app->varGetByte(90003)) {
				_app->timerStopAll();
				_app->onSwitchZone(kZoneFO, 0);
			}
			break;

		case 4:
			if (!_app->varGetByte(90002)) {
				_app->timerStopAll();
				_app->onSwitchZone(kZoneN2, 0);
			}
			break;

		case 5:
			if (!_app->varGetByte(90004)) {
				_app->timerStopAll();
				_app->onSwitchZone(kZoneWA, 0);
			}
			break;
		}
		break;

	case kObject80021:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(80004) == 1) {
				if (_app->varGetByte(90001)) {
					_app->puzzleSetActive(kPuzzle80006);
					_app->playMovie("1142");
					_app->objectPresentationShow(kObject80019, 0);
				} else {
					_app->puzzleSetActive(kPuzzle80006);
					_app->playMovie("1141");
				}
			}

			if (_app->varGetByte(80004) == 11) {
				_app->puzzleSetActive(kPuzzle80007);
				_app->playMovie("1143");
			}

			if (_app->varGetByte(80004) == 21) {
				_app->puzzleSetActive(kPuzzle80009);
				_app->playMovie("1144");
			}

			if (_app->varGetByte(80004) == 31) {
				_app->puzzleSetActive(kPuzzle80008);
				_app->playMovie("1145");
			}

			if (_app->varGetByte(80004) == 41) {
				_app->puzzleSetActive(kPuzzle80010);
				_app->playMovie("1146");
			}
			break;

		case 1:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 10);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 2:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 20);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 3:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 30);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 4:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 40);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;
		}
		break;

	case kObject80007:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDeath) {
				_app->bagRemoveAll();
				_app->soundStopAll(1024);
				_app->timerStopAll();
				_app->setupZone(kZoneAS, kSetupType6);
			}

			_app->cursorDelete();
		}
		break;

	case kObject80012:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(80028);
			break;

		case 1:
			_app->soundPlay(80025);
			break;
		case 2:
			_app->soundPlay(80021);
			break;
		case 3:
			_app->soundPlay(80024);
			break;
		case 4:
			_app->soundPlay(80022);
			break;
		case 5:
			_app->soundPlay(80026);
			break;
		case 6:
			_app->soundPlay(80027);
			break;
		case 7:
			_app->soundPlay(80023);
			break;
		}
		break;

	case kObject80018:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(90001) == 1) {
				_app->puzzleSetActive(kPuzzle80002);
				_app->soundSetVolume(80201, 80);
				_app->soundPlay(80040);
			}
			break;

		case 1:
			if (_app->varGetByte(90002) == 1) {
				_app->puzzleSetActive(kPuzzle80003);
				_app->soundSetVolume(80203, 90);
				_app->soundPlay(80049);
			}
			break;
		case 2:
			if (_app->varGetByte(90003) == 1) {
				_app->puzzleSetActive(kPuzzle80004);
				_app->soundSetVolume(80204, 90);
				_app->soundPlay(80058);
			}
			break;
		case 3:
			if (_app->varGetByte(90004) == 1) {
				_app->puzzleSetActive(kPuzzle80005);
				_app->soundSetVolume(80205, 80);
				_app->soundPlay(80068);
			}
			break;
		}
		break;

	case kObject80022:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->onSwitchZone(kZoneAS, 5);
		break;
	}
}

void ZoneASRing::onSound(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 80100:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80101);
		break;

	case 80101:
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80102);
		break;

	case 80102:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80103);
		break;

	case 80103:
		_app->playMovie("1157");
		_app->puzzleSetActive(kPuzzle80013);
		_app->soundPlay(80104);
		break;

	case 80104:
	case 80040:
	case 80041:
	case 80042:
	case 80043:
	case 80044:
	case 80045:
	case 80046:
	case 80047:
	case 80049:
	case 80050:
	case 80051:
	case 80052:
	case 80053:
	case 80054:
	case 80055:
	case 80056:
	case 80058:
	case 80059:
	case 80060:
	case 80061:
	case 80062:
	case 80063:
	case 80064:
	case 80065:
	case 80066:
	case 80068:
	case 80069:
	case 80070:
	case 80071:
	case 80072:
	case 80073:
	case 80074:
	case 80075:
	case 80076:
	case 80077:
	case 80078:
	case 80079:
		_app->soundPlay(id + 1);
		break;

	case 80105:
		_app->playMovie("1158");
		_app->puzzleSetActive(kPuzzle80014);
		_app->soundPlay(80106);
		break;

	case 80106:
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		_app->rotationSetAlp(80001, 270.0f);
		_app->rotationSetBet(80001, -26.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		break;

	case 80048:
		_app->soundSetVolume(80201, 100);
		_app->varSetByte(80002, 2);
		break;

	case 80057:
		_app->soundSetVolume(80203, 100);
		_app->varSetByte(80002, 3);
		break;

	case 80067:
		_app->soundSetVolume(80204, 100);
		_app->varSetByte(80002, 4);
		break;

	case 80080:
		_app->soundSetVolume(80205, 100);
		_app->varSetByte(80002, 5);
		break;
	case 80107:
		_app->playMovie("1159");
		_app->showMenu(kZoneAS, kMenuAction0);
		break;
	}
}

void ZoneASRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityPuzzleToRotation)
		return;

	switch (movabilityFrom){
	default:
		break;

	case 80006:
		if (movabilityTo == 80101) {
			if (_app->varGetByte(90001))
				_app->playMovie("1152");
			else
				_app->playMovie("1151");
		}
		break;

	case 80007:
		if (movabilityTo == 80101)
			_app->playMovie("1153");
		break;

	case 80008:
		if (movabilityTo == 80101)
			_app->playMovie("1155");
		break;

	case 80009:
		if (movabilityTo == 80101)
			_app->playMovie("1154");
		break;

	case 80010:
		if (movabilityTo == 80101)
			_app->playMovie("1156");
		break;
	}
}

void ZoneASRing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityPuzzleToRotation && movabilityFrom == 80101)
		if (movabilityTo == 80006 || movabilityTo == 80007 || movabilityTo == 80008
		 || movabilityTo == 80009 || movabilityTo == 80010)
			_app->varSetByte(80003, 0);
}

} // End of namespace Ring
