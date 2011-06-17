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

#include "ring/game/ring/ring_zoneni.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneNIRing::ZoneNIRing(ApplicationRing *application) : _app(application) {
	_stateBagNI = false;
	_indexOffsetNI = 0;
	_distNI = 0;
}

ZoneNIRing::~ZoneNIRing() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneNIRing::onInit() {
	debugC(1, kRingDebugLogic, "Initializing Alberich zone (NI)...");

	_app->setZoneAndEnableBag(kZoneNI);

	_app->rotationAdd(10000, "NIS00N01", 0, 2);
	_app->rotationAdd(10001, "NIS00N03", 0, 2);
	_app->rotationAdd(10002, "NIS00N04", 0, 2);
	_app->rotationSetCompressionBufferLength(10002, 1300000);
	_app->rotationAdd(10003, "NIS00N05", 0, 2);
	_app->rotationAdd(10004, "NIS00N06", 0, 2);
	_app->rotationAdd(10005, "NIS00N07", 0, 2);
	_app->rotationAdd(10101, "NIS01N01", 0, 1);
	_app->rotationSetCompressionBufferLength(10101, 1300000);
	_app->rotationAdd(10102, "NIS01N02", 0, 1);
	_app->rotationAdd(10201, "NIS02N01", 0, 1);
	_app->rotationAdd(10301, "NIS03N01", 0, 1);
	_app->rotationAdd(10401, "NIS04N01", 0, 2);
	_app->rotationAdd(10402, "NIS04N02", 0, 2);
	_app->rotationSetCompressionBufferLength(10402, 1300000);
	_app->rotationAdd(10403, "NIS04N03", 0, 0);
	_app->rotationAdd(10404, "NIS04N04", 0, 0);
	_app->rotationAdd(10405, "NIS04N05", 0, 0);
	_app->rotationAdd(10415, "NIS04N15", 0, 0);
	_app->rotationAdd(10406, "NIS04N06", 0, 0);
	_app->rotationSetJugOn(10406, 10.0f, 1.0);
	_app->rotationAdd(10501, "NIS05N01", 0, 0);
	_app->rotationAdd(10601, "NIS06N01", 0, 0);
	_app->puzzleAdd(kPuzzle12001);
	_app->puzzleAddBackgroundImage(kPuzzle12001, "TR_NI_RH_BP01.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12002);
	_app->puzzleAddBackgroundImage(kPuzzle12002, "TR_NI_RH_BP02.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle12003);
	_app->puzzleAddBackgroundImage(kPuzzle12003, "TR_NI_RH_BP03.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10000);
	_app->puzzleAddBackgroundImage(kPuzzle10000, "NIS00N01P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10001);
	_app->puzzleAddBackgroundImage(kPuzzle10001, "NIS00N01P01L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10002);
	_app->puzzleAddBackgroundImage(kPuzzle10002, "NIS00N01P01L02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10100);
	_app->puzzleAddBackgroundImage(kPuzzle10100, "NIS01N01P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10102);
	_app->puzzleAddBackgroundImage(kPuzzle10102, "NIS01N01P03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10200);
	_app->puzzleAddBackgroundImage(kPuzzle10200, "NIS02N01P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10201);
	_app->puzzleAddBackgroundImage(kPuzzle10201, "NIS02N01P02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10202);
	_app->puzzleAddBackgroundImage(kPuzzle10202, "NIS02N01P03S01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10203);
	_app->puzzleAddBackgroundImage(kPuzzle10203, "NIS02N01P02S01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10204);
	_app->puzzleAddBackgroundImage(kPuzzle10204, "NIS02N01P02S02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10205);
	_app->puzzleAddBackgroundImage(kPuzzle10205, "NIS02N01P02S03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10391);
	_app->puzzleAddBackgroundImage(kPuzzle10391, "NIS03N01P01S01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10390);
	_app->puzzleAddBackgroundImage(kPuzzle10390, "NIS03N01P01S02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10392);
	_app->puzzleAddBackgroundImage(kPuzzle10392, "NIS03N01P03S01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10300);
	_app->puzzleAddBackgroundImage(kPuzzle10300, "NIS03N01P02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10400);
	_app->puzzleAddBackgroundImage(kPuzzle10400, "NIS04N02P01L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10401);
	_app->puzzleAddBackgroundImage(kPuzzle10401, "NIS04N02P02L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10410);
	_app->puzzleAddBackgroundImage(kPuzzle10410, "NIS04N03P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10411);
	_app->puzzleAddBackgroundImage(kPuzzle10411, "NIS04N03P03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10500);
	_app->puzzleAddBackgroundImage(kPuzzle10500, "NIS05N01P01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10501);
	_app->puzzleAddBackgroundImage(kPuzzle10501, "NIS05N01P01L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10510);
	_app->puzzleAddBackgroundImage(kPuzzle10510, "NIS05N01P02.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10511);
	_app->puzzleAddBackgroundImage(kPuzzle10511, "NIS05N01P02L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10520);
	_app->puzzleAddBackgroundImage(kPuzzle10520, "NIS05N01P03.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10521);
	_app->puzzleAddBackgroundImage(kPuzzle10521, "NIS05N01P03L01.0001.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle10600);
	_app->puzzleAddBackgroundImage(kPuzzle10600, "NIS06N01P01.0001.bmp", Common::Point(0, 16), true);
	_app->rotationAddMovabilityToRotation(10000, 10001, "1551", Common::Rect(1211, -137, 1494, 151), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(10000, 0, 90.0f, 0.3f, 85.7f, 0, 0, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10000, 10401, "1552", Common::Rect(2847, -364, 3394, 97), false, kCursorMove, 41);
	_app->rotationSetMovabilityToRotation(10000, 1, 270.0f, 0.3f, 85.7f, 0, 0, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10000, 10201, "1553", Common::Rect(300, -244, 1000, 209), false, kCursorMove, 21);
	_app->rotationSetMovabilityToRotation(10000, 2, 90.0f, 0.3f, 85.7f, 0, 0, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10001, 10002, "1554", Common::Rect(1239, -116, 1485, 127), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(10001, 0, 90.0f, 0.3f, 85.7f, 0, 0, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10001, 10000, "1555", Common::Rect(2998, -212, 3315, 163), false, kCursorMove, 2);
	_app->rotationSetMovabilityToRotation(10001, 1, 270.0f, 0.3f, 85.7f, 0, 0, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10002, 10004, "1556", Common::Rect(1212, -132, 1500, 190), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(10002, 0, 90.0f, 0.3f, 85.7f, 0, 0, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10002, 10001, "1557", Common::Rect(2951, -167, 3327, 218), false, kCursorMove, 2);
	_app->rotationSetMovabilityToRotation(10002, 1, 270.0f, 0.3f, 85.7f, 0, 0, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10002, 10003, "1558", Common::Rect(1600, -214, 1923, 169), false, kCursorMove, 5);
	_app->rotationSetMovabilityToRotation(10002, 2, 90.0f, 0.3f, 85.7f, 0, 0, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10003, 10002, "1559", Common::Rect(0, -156, 492, 219), false, kCursorMove, 2);
	_app->rotationSetMovabilityToRotation(10003, 0, 328.3f, 0.3f, 85.7f, 0, 0, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10003, 10301, "1560", Common::Rect(2141, -190, 2666, 265), true, kCursorMove, 110);
	_app->rotationSetMovabilityToRotation(10003, 1, 195.0f, 0.3f, 85.7f, 0, 2, 150.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10004, 10005, "1561", Common::Rect(1263, -47, 1610, 221), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(10004, 0, 90.0f, 0.3f, 85.7f, 0, 0, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10004, 10002, "1562", Common::Rect(3009, -146, 3301, 160), false, kCursorMove, 2);
	_app->rotationSetMovabilityToRotation(10004, 1, 270.0f, 0.3f, 85.7f, 0, 0, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10005, 10101, "1563", Common::Rect(1121, -195, 1606, 331), false, kCursorMove, 4);
	_app->rotationSetMovabilityToRotation(10005, 0, 90.0f, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10005, 10004, "1564", Common::Rect(3009, -319, 3487, 172), false, kCursorMove, 2);
	_app->rotationSetMovabilityToRotation(10005, 1, 270.0f, 0.3f, 85.7f, 0, 0, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10101, 10102, "1565", Common::Rect(1186, -143, 1534, 195), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10101, 0, 90.0f, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10102, 10101, "1566", Common::Rect(2951, -125, 3350, 193), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10102, 0, 270.0f, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10102, 10601, "1567", Common::Rect(1323, -57, 1587, 225), true, kCursorMove, 16);
	_app->rotationSetMovabilityToRotation(10102, 1, 90.0f, 0.3f, 85.7f, 0, 2, 130.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10102, 10601, "1568", Common::Rect(1323, -57, 1587, 225), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10102, 2, 90.0f, 0.3f, 85.7f, 0, 2, 224.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10201, 10000, "1509", Common::Rect(1947, -158, 2418, 301), false, kCursorMove, 2);
	_app->rotationSetMovabilityToRotation(10201, 0, 190.1f, 0.3f, 85.7f, 0, 0, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToPuzzle(10201, kPuzzle10200, "1569", Common::Rect(198, -34, 495, 366), true, kCursorHotspot, 110);
	_app->rotationSetMovabilityToPuzzle(10201, 1, 345.0f, 15.0f, 80.0f, 0, 2);
	_app->rotationAddMovabilityToRotation(10301, 10003, "1570", Common::Rect(318, -202, 789, 226), true, kCursorMove, 100);
	_app->rotationSetMovabilityToRotation(10301, 0, 12.0f, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToPuzzle(10301, kPuzzle10300, "", Common::Rect(2272, -8, 2418, 144), true, kCursorMove, 0);
	_app->rotationSetMovabilityToPuzzle(10301, 1, 190.0f, 0.3f, 85.7f, 0, 0);
	_app->rotationAddMovabilityToRotation(10401, 10000, "1571", Common::Rect(1123, -153, 1599, 279), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(10401, 0, 90.0f, 0.3f, 85.7f, 0, 0, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10401, 10402, "1572", Common::Rect(2953, -223, 3359, 155), true, kCursorMove, 110);
	_app->rotationSetMovabilityToRotation(10401, 1, 270.0f, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10402, 10401, "1573", Common::Rect(1081, 13, 1657, 286), true, kCursorMove, 100);
	_app->rotationSetMovabilityToRotation(10402, 0, 90.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10402, 10403, "1574", Common::Rect(3002, -139, 3306, 190), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10402, 1, 270.0f, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToPuzzle(10402, kPuzzle10400, "", Common::Rect(2213, 87, 2371, 202), true, kCursorHotspot, 41);
	_app->rotationSetMovabilityToPuzzle(10402, 2, 185.0f, 15.0f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(10402, kPuzzle10401, "", Common::Rect(326, 87, 485, 198), true, kCursorHotspot, 42);
	_app->rotationSetMovabilityToPuzzle(10402, 3, 0, 15.0f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToRotation(10403, 10402, "1575", Common::Rect(1146, -94, 1560, 221), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10403, 0, 90.0f, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToPuzzle(10403, kPuzzle10410, "", Common::Rect(3041, -265, 3290, 0), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(10403, 1, 270.0f, 0.3f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToRotation(10404, 10403, "1576", Common::Rect(207, 249, 564, 423), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToRotation(10404, 0, 90.0f, 0, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10405, 10404, "1577", Common::Rect(1165, -221, 1527, 284), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10405, 0, 90.0f, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10415, 10404, "1577", Common::Rect(1165, -221, 1527, 284), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10415, 0, 90.0f, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(10501, 10601, "1579", Common::Rect(546, -345, 980, 120), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10501, 0, 34.0f, 0.3f, 85.7f, 0, 2, 44.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToPuzzle(10501, kPuzzle10500, "", Common::Rect(1139, -204, 1520, 156), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(10501, 1, 90.0f, 0.3f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(10501, kPuzzle10510, "", Common::Rect(1787, -143, 2033, 108), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(10501, 2, 145.0f, 0.3f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(10501, kPuzzle10520, "", Common::Rect(2228, -118, 2411, 80), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(10501, 3, 185.0f, 0.3f, 85.7f, 0, 2);
	_app->rotationAddMovabilityToRotation(10601, 10102, "1580", Common::Rect(863, -211, 1123, 101), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(10601, 0, 44.0f, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToPuzzle(10601, kPuzzle10600, "", Common::Rect(2436, -256, 2951, 284), true, kCursorHotspot, 61);
	_app->rotationSetMovabilityToPuzzle(10601, 1, 224.4f, 0.3f, 85.7f, 0, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10102, kPuzzle10100, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle10200, 10201, "1549", Common::Rect(0, 420, 640, 464), true, kCursorBack, 100);
	_app->puzzleSetMovabilityToRotation(kPuzzle10200, 0, 345.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle10300, 10301, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle10300, 0, 190.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle10400, 10402, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 41);
	_app->puzzleSetMovabilityToRotation(kPuzzle10400, 0, 190.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle10401, 10402, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 42);
	_app->puzzleSetMovabilityToRotation(kPuzzle10401, 0, 10.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10410, kPuzzle10411, "", Common::Rect(250, 280, 320, 370), false, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle10410, 10403, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle10410, 1, 270.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToRotation(kPuzzle10410, 10404, "1581", Common::Rect(455, 185, 534, 230), true, kCursorHotspot, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle10410, 2, 270.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10411, kPuzzle10410, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle10500, 10501, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle10500, 0, 90.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10500, kPuzzle10501, "", Common::Rect(200, 100, 400, 300), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10501, kPuzzle10500, "", Common::Rect(430, 48, 640, 434), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle10510, 10501, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle10510, 0, 144.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10510, kPuzzle10511, "", Common::Rect(200, 100, 400, 300), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10511, kPuzzle10510, "", Common::Rect(430, 48, 640, 434), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle10520, 10501, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle10520, 0, 184.0f, 0.3f, 85.7f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10520, kPuzzle10521, "", Common::Rect(200, 100, 400, 300), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10521, kPuzzle10520, "", Common::Rect(430, 48, 640, 434), true, kCursorHotspot, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle10521, kPuzzle10521, "1582", Common::Rect(216, 124, 406, 327), true, kCursorHotspot, 55);
	_app->objectAdd(kObject12000, "", "", 1);
	_app->objectAddPresentation(kObject12000);
	_app->objectPresentationAddAnimationToPuzzle(kObject12000, 0, kPuzzle12001, "TR_NI_RH_BP01S01", kImageTypeBMP, Common::Point(239, 208), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject12000);
	_app->objectPresentationAddAnimationToPuzzle(kObject12000, 1, kPuzzle12002, "TR_NI_RH_BP02S01", kImageTypeBMP, Common::Point(187, 94), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject12000);
	_app->objectPresentationAddAnimationToPuzzle(kObject12000, 2, kPuzzle12003, "TR_NI_RH_BP03S01", kImageTypeBMP, Common::Point(207, 134), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject10003, "Bike", "", 1);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 0, 10003, 1);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 1, 10201, 0);
	_app->objectAddPresentation(kObject10003);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 3, 10000, 0);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 4, 10001, 0);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 5, 10002, 0);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 6, 10004, 0);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 7, 10005, 0);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 8, 10000, 1);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 9, 10001, 1);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 10, 10002, 1);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 11, 10004, 1);
	_app->objectAddPresentation(kObject10003);
	_app->objectPresentationAddImageToRotation(kObject10003, 12, 10005, 1);
	_app->objectPresentationShow(kObject10003, 0);
	_app->objectPresentationShow(kObject10003, 1);
	_app->objectAdd(kObjectBrutality, "SP/Brutality", "NI_Brutality", 1);
	_app->objectAddBagAnimation(kObjectBrutality, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBrutality, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBrutality, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMinerals, "Minerals", "NI_Minerals", 1);
	_app->objectAddBagAnimation(kObjectMinerals, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMinerals, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMinerals, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectGlug, "Glug", "NI_Glug", 1);
	_app->objectAddBagAnimation(kObjectGlug, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectGlug, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectGlug, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10000, Common::Rect(227, 206, 410, 394), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10001, Common::Rect(48, 40, 567, 416), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10002, Common::Rect(246, 188, 383, 322), true, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObjectGlug, 10003, Common::Rect(1089, 390, 1664, 680), true, kCursorHotspot, 3);
	_app->objectAddRotationAccessibility(kObjectGlug, 10401, Common::Rect(1963, 390, 2615, 680), true, kCursorHotspot, 3);
	_app->objectAddRotationAccessibility(kObjectGlug, 10201, Common::Rect(2830, 390, 3505, 680), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10000, Common::Rect(0, 420, 640, 464), true, kCursorBack, 4);
	_app->objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10001, Common::Rect(0, 420, 640, 464), true, kCursorBack, 4);
	_app->objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10002, Common::Rect(0, 420, 640, 464), true, kCursorBack, 4);
	_app->objectAddPresentation(kObjectGlug);
	_app->objectPresentationAddAnimationToPuzzle(kObjectGlug, 0, kPuzzle10002, "NIS00N01P01S01", kImageTypeBMP, Common::Point(203, 191), kDrawType1, 1000, 5, 12.5f, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObjectGlug, 0, 0, 10000);
	_app->objectAdd(kObject10110, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(236, 79, 289, 118),  false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(171, 124, 229, 159), false, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(272, 126, 342, 165), false, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(195, 169, 259, 213), false, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(107, 204, 188, 285), false, kCursorHotspot, 5);
	_app->objectAdd(kObject10100, "Console", "", 1);
	_app->objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(0, 16, 640, 66),    true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(0, 414, 640, 464),  true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(0, 66, 50, 414),    true, kCursorMove, 0);
	_app->objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(610, 66, 640, 414), true, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject10100, 10101, Common::Rect(3406, 26, 3541, 155), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject10100);
	_app->objectPresentationAddImageToPuzzle(kObject10100, 0, kPuzzle10100, "NIS01N01P01L01.0001.bmp", Common::Point(101, 68), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10100);
	_app->objectPresentationAddImageToPuzzle(kObject10100, 1, kPuzzle10100, "NIS01N01P01L02.0001.bmp", Common::Point(80, 63), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10100);
	_app->objectPresentationAddImageToPuzzle(kObject10100, 2, kPuzzle10100, "NIS01N01P01L03.0001.bmp", Common::Point(116, 159), true, kDrawType1, 1000);
	_app->objectAdd(kObject10101, "CCButton1", "", 2);
	_app->objectAddRotationAccessibility(kObject10101, 10101, Common::Rect(3208, 109, 3322, 184), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject10101, kPuzzle10100, Common::Rect(359, 265, 440, 304), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject10101, kPuzzle10100, Common::Rect(359, 265, 440, 304), false, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject10101);
	_app->objectPresentationAddImageToPuzzle(kObject10101, 0, kPuzzle10100, "NIS01N01P01S03.0001.bmp", Common::Point(361, 259), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10101);
	_app->objectPresentationAddAnimationToPuzzle(kObject10101, 1, kPuzzle10100, "NIS01N01P01S01", kImageTypeBMP, Common::Point(85, 68), kDrawType1, 1000, 36, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10101, 1, 0, 10101);
	_app->objectAddPresentation(kObject10101);
	_app->objectPresentationAddAnimationToPuzzle(kObject10101, 2, kPuzzle10100, "NIS01N01P01L02S01", kImageTypeBMP, Common::Point(80, 63), kDrawType1, 1000, 36, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10101, 2, 0, 10103);
	_app->objectAddPresentation(kObject10101);
	_app->objectPresentationAddAnimationToPuzzle(kObject10101, 3, kPuzzle10100, "NIS01N01P01S01", kImageTypeBMP, Common::Point(85, 68), kDrawType1, 1000, 36, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10101, 3, 0, 10100);
	_app->objectAddPresentation(kObject10101);
	_app->objectPresentationAddAnimationToPuzzle(kObject10101, 4, kPuzzle10100, "NIS01N01P01L02S01", kImageTypeBMP, Common::Point(80, 63), kDrawType1, 1000, 36, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10101, 4, 0, 10102);
	_app->objectAddPresentation(kObject10101);
	_app->objectPresentationAddImageToPuzzle(kObject10101, 5, kPuzzle10100, "NIS01N01P01S03.0003.bmp", Common::Point(361, 259), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10101);
	_app->objectPresentationAddImageToRotation(kObject10101, 6, 10101, 0);
	_app->objectPresentationAddImageToRotation(kObject10101, 6, 10102, 0);
	_app->objectAdd(kObject10102, "CCButton2", "", 2);
	_app->objectAddRotationAccessibility(kObject10102, 10101, Common::Rect(3250, 195, 3357, 245), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject10102, kPuzzle10100, Common::Rect(382, 308, 472, 352), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject10102, kPuzzle10100, Common::Rect(382, 308, 472, 352), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject10102);
	_app->objectPresentationAddImageToPuzzle(kObject10102, 0, kPuzzle10100, "NIS01N01P01S03.0004.bmp", Common::Point(385, 299), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10102);
	_app->objectPresentationAddAnimationToPuzzle(kObject10102, 1, kPuzzle10100, "NIS01N01P01S02", kImageTypeBMP, Common::Point(40, 65), kDrawType1, 1000, 46, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10102, 1, 0, 10105);
	_app->objectAddPresentation(kObject10102);
	_app->objectPresentationAddAnimationToPuzzle(kObject10102, 2, kPuzzle10100, "NIS01N01P01S02", kImageTypeBMP, Common::Point(40, 65), kDrawType1, 1000, 23, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10102, 2, 0, 10104);
	_app->objectAddPresentation(kObject10102);
	_app->objectPresentationAddAnimationToPuzzle(kObject10102, 3, kPuzzle10100, "NIS01N01P01S02", kImageTypeBMP, Common::Point(40, 65), kDrawType1, 1000, 23, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10102, 3, 0, 10106);
	_app->objectAddPresentation(kObject10102);
	_app->objectPresentationAddImageToPuzzle(kObject10102, 4, kPuzzle10100, "NIS01N01P01S03.0002.bmp", Common::Point(385, 299), true, kDrawType1, 1000);
	_app->objectAdd(kObject10103, "CCHandle", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10103, kPuzzle10100, Common::Rect(497, 192, 543, 249), true, kCursorHandSelection, 1);
	_app->objectSetActiveDrawCursor(kObject10103, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10103, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 14; i++) {
		_app->objectAddPresentation(kObject10103);
		_app->objectPresentationAddImageToPuzzle(kObject10103, i, kPuzzle10100, Common::String::format("NIS01N01P02S01.%04d.bmp", i + 1), Common::Point(504, 194), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject10104, "CCTileHold", "", 1);
	_app->objectAddPuzzleAccessibility(kObject10104, kPuzzle10100, Common::Rect(60, 175, 187, 320), false, kCursorHotspot, 0);
	_app->objectAdd(kObject10109, "CCMosaicBtns", "", 3);
	_app->objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(267, 257, 284, 272), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(265, 205, 282, 220), false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(244, 220, 261, 235), false, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(278, 220, 295, 235), false, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(256, 235, 273, 250), false, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(230, 257, 247, 272), false, kCursorHotspot, 5);
	_app->objectAddPresentation(kObject10109);
	_app->objectPresentationAddImageToPuzzle(kObject10109, 0, kPuzzle10100, "NIS01N01P01L02S02.0006.bmp", Common::Point(221, 196), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10109);
	_app->objectPresentationAddImageToPuzzle(kObject10109, 1, kPuzzle10100, "NIS01N01P01L02S02.0001.bmp", Common::Point(221, 196), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10109);
	_app->objectPresentationAddImageToPuzzle(kObject10109, 2, kPuzzle10100, "NIS01N01P01L02S02.0002.bmp", Common::Point(221, 196), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10109);
	_app->objectPresentationAddImageToPuzzle(kObject10109, 3, kPuzzle10100, "NIS01N01P01L02S02.0003.bmp", Common::Point(221, 196), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10109);
	_app->objectPresentationAddImageToPuzzle(kObject10109, 4, kPuzzle10100, "NIS01N01P01L02S02.0004.bmp", Common::Point(221, 196), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10109);
	_app->objectPresentationAddImageToPuzzle(kObject10109, 5, kPuzzle10100, "NIS01N01P01L02S02.0005.bmp", Common::Point(221, 196), true, kDrawType1, 1000);
	_app->objectAdd(kObject10105, "CCHolo", "", 1);
	_app->objectAddPuzzleAccessibility(kObject10105, kPuzzle10100, Common::Rect(77, 132, 340, 298),  false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject10105, kPuzzle10100, Common::Rect(140, 170, 430, 380), false, kCursorHotspot, 0);
	_app->objectAdd(kObject10106, "CCHoloCross", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10106, kPuzzle10102, Common::Rect(203, 236, 283, 316), true, kCursorHandSelection, 0);
	_app->objectSetActiveDrawCursor(kObject10106, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10106, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 20; i++) {
		_app->objectAddPresentation(kObject10106);
		_app->objectPresentationAddImageToPuzzle(kObject10106, i, kPuzzle10102, Common::String::format("NIS01N01P03L01S01.%04d.bmp", i + 1), Common::Point(213, 254), true, kDrawType1, 1000);
	}

	for (uint32 i = 0; i < 20; i++) {
		_app->objectAddPresentation(kObject10106);
		_app->objectPresentationAddImageToPuzzle(kObject10106, i, kPuzzle10102, Common::String::format("NIS01N01P03L01S01_a.%04d.bmp", i + 1), Common::Point(213, 254), true, kDrawType1, 1000);
	}

	_app->objectAddPresentation(kObject10106);
	_app->objectPresentationAddImageToPuzzle(kObject10106, 38, kPuzzle10102, "NIS01N01P03L02.0001.bmp", Common::Point(267, 283), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10106);
	_app->objectPresentationAddImageToPuzzle(kObject10106, 39, kPuzzle10102, "NIS01N01P03L01.0001.bmp", Common::Point(56, 139), true, kDrawType1, 1000);
	_app->objectAdd(kObject10107, "CCHoloDam1", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10107, kPuzzle10102, Common::Rect(295, 255, 345, 315), true,  kCursorHandSelection, 0);
	_app->objectAddPuzzleAccessibility(kObject10107, kPuzzle10102, Common::Rect(295, 315, 345, 375), false, kCursorHandSelection, 1);
	_app->objectSetActiveDrawCursor(kObject10107, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10107, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject10107);
		_app->objectPresentationAddImageToPuzzle(kObject10107, i, kPuzzle10102, Common::String::format("NIS01N01P03L01S02.%04d.bmp", i + 1), Common::Point(302, 264), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject10200, "Speaker", "", 1);
	_app->objectAddPuzzleAccessibility(kObject10200, kPuzzle10200, Common::Rect(150, 110, 470, 200), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject10200);
	_app->objectPresentationAddAnimationToPuzzle(kObject10200, 0, kPuzzle10202, "NIS02N01P03S01", kImageTypeBMP, Common::Point(229, 188), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject10200);
	_app->objectPresentationAddAnimationToPuzzle(kObject10200, 1, kPuzzle10203, "NIS02N01P02S01", kImageTypeBMP, Common::Point(197, 44), kDrawType1, 1000, 10, 12.5f, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10200, 1, 0, 10200);
	_app->objectAddPresentation(kObject10200);
	_app->objectPresentationAddAnimationToPuzzle(kObject10200, 2, kPuzzle10204, "NIS02N01P02S02", kImageTypeBMP, Common::Point(311, 166), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject10200);
	_app->objectPresentationAddAnimationToPuzzle(kObject10200, 3, kPuzzle10205, "NIS02N01P02S03", kImageTypeBMP, Common::Point(97, 185), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject10201, "Handle", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10201, kPuzzle10200, Common::Rect(300, 210, 430, 270), false, kCursorHandSelection, 0);
	_app->objectSetActiveDrawCursor(kObject10201, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10201, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 15; i++) {
		_app->objectAddPresentation(kObject10201);
		_app->objectPresentationAddImageToPuzzle(kObject10201, i, kPuzzle10200, Common::String::format("NIS02N01P01S01.%04d.bmp", i + 1), Common::Point(305, 213), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject10300, "Mime", "", 1);
	_app->objectAddRotationAccessibility(kObject10300, 10301, Common::Rect(1937, -26, 2079, 88), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject10300);
	_app->objectPresentationAddAnimationToRotation(kObject10300, 0, 10301, 0, 25, 10.0f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject10300, 0, 0, 10300);
	_app->objectPresentationShow(kObject10300, 0);
	_app->objectAddPresentation(kObject10300);
	_app->objectPresentationAddAnimationToPuzzle(kObject10300, 1, kPuzzle10390, "NIS03N01P01S02", kImageTypeBMP, Common::Point(289, 79), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject10300);
	_app->objectPresentationAddAnimationToPuzzle(kObject10300, 2, kPuzzle10391, "NIS03N01P01S01", kImageTypeBMP, Common::Point(260, 73), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject10300);
	_app->objectPresentationAddAnimationToPuzzle(kObject10300, 3, kPuzzle10392, "NIS03N01P03S01", kImageTypeBMP, Common::Point(246, 183), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectLogeTear, "Loge's Tear", "NI_Tear", 1);
	_app->objectAddBagAnimation(kObjectLogeTear, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectLogeTear, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectLogeTear, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectLogeTear, kPuzzle10300, Common::Rect(470, 254, 541, 328), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectLogeTear);
	_app->objectPresentationAddImageToPuzzle(kObjectLogeTear, 0, kPuzzle10300, "NIS03N01P02L02.0001.bmp", Common::Point(466, 249), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectLogeTear);
	_app->objectPresentationAddAnimationToPuzzle(kObjectLogeTear, 1, kPuzzle10300, "NIS03N01P02S01", kImageTypeBMP, Common::Point(466, 250), kDrawType1, 1000, 21, 12.5f, 4);
	_app->objectPresentationShow(kObjectLogeTear, 1);
	_app->objectAdd(kObjectGold, "Rhine Gold", "RhineGold", 1);
	_app->objectAdd(kObjectTile, "Tile", "NI_Mosaic", 1);
	_app->objectAddBagAnimation(kObjectTile, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectTile, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectTile, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectTile, kPuzzle10300, Common::Rect(247, 24, 337, 126), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectTile);
	_app->objectPresentationAddImageToPuzzle(kObjectTile, 0, kPuzzle10300, "NIS03N01P02L01.0001.bmp", Common::Point(251, 45), true, kDrawType1, 1000);
	_app->objectAdd(kObjectMagicFrog, "Frog", "NI_Frog", 1);
	_app->objectAddBagAnimation(kObjectMagicFrog, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMagicFrog, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMagicFrog, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectMagicFrog, kPuzzle10300, Common::Rect(300, 230, 430, 430), true, kCursorHotspot, 0);
	_app->objectAdd(kObject10420, "LValve", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10420, kPuzzle10400, Common::Rect(287, 223, 344, 252), true, kCursorHandSelection, 0);
	_app->objectAddPuzzleAccessibility(kObject10420, kPuzzle10400, Common::Rect(312, 274, 368, 300), false, kCursorHandSelection, 1);
	_app->objectSetActiveDrawCursor(kObject10420, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10420, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject10420);
	_app->objectPresentationAddImageToPuzzle(kObject10420, 0, kPuzzle10400, "NIS04N02P01L01S01.0012.bmp", Common::Point(257, 211), true, kDrawType1, 1000);

	for (uint32 i = 1; i < 13; i++) {
		_app->objectAddPresentation(kObject10420);
		_app->objectPresentationAddImageToPuzzle(kObject10420, i, kPuzzle10400, Common::String::format("NIS04N02P01L01S01.%04d.bmp", i), Common::Point(257, 211), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject10421, "RValve", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10421, kPuzzle10401, Common::Rect(291, 221, 357, 257), true,  kCursorHandSelection, 0);
	_app->objectAddPuzzleAccessibility(kObject10421, kPuzzle10401, Common::Rect(271, 268, 336, 308), false, kCursorHandSelection, 1);
	_app->objectSetActiveDrawCursor(kObject10421, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10421, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject10421);
	_app->objectPresentationAddImageToPuzzle(kObject10421, 0, kPuzzle10401, "NIS04N02P02L01S01.0012.bmp", Common::Point(227, 204), true, kDrawType1, 1000);

	for (uint32 i = 1; i < 13; i++) {
		_app->objectAddPresentation(kObject10421);
		_app->objectPresentationAddImageToPuzzle(kObject10421, i, kPuzzle10401, Common::String::format("NIS04N02P02L01S01.%04d.bmp", i), Common::Point(227, 204), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject10430, "TearCasing", "", 3);
	_app->objectAddPuzzleAccessibility(kObject10430, kPuzzle10411, Common::Rect(189, 189, 362, 377), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject10430, kPuzzle10411, Common::Rect(224, 90, 362, 209),  false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject10430, kPuzzle10411, Common::Rect(293, 241, 475, 462), false, kCursorHotspot, 2);
	_app->objectAddPresentation(kObject10430);
	_app->objectPresentationAddImageToPuzzle(kObject10430, 0, kPuzzle10411, "NIS04N03P03L01.0001.bmp", Common::Point(214, 173), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10430);
	_app->objectPresentationAddImageToPuzzle(kObject10430, 1, kPuzzle10411, "NIS04N03P03L02.0001.bmp", Common::Point(270, 116), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject10430);
	_app->objectPresentationAddAnimationToPuzzle(kObject10430, 2, kPuzzle10411, "NIS04N03P03L01S01", kImageTypeBMP, Common::Point(217, 171), kDrawType1, 1000, 27, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10430, 2, 0, 10422);
	_app->objectAddPresentation(kObject10430);
	_app->objectPresentationAddAnimationToPuzzle(kObject10430, 3, kPuzzle10411, "NIS04N03P03L01S01", kImageTypeBMP, Common::Point(217, 171), kDrawType1, 1000, 27, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10430, 3, 0, 10423);
	_app->objectAddPresentation(kObject10430);
	_app->objectPresentationAddAnimationToPuzzle(kObject10430, 4, kPuzzle10411, "NIS04N03P03L02S01", kImageTypeBMP, Common::Point(215, 111), kDrawType1, 1000, 26, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10430, 4, 0, 10424);
	_app->objectAddPresentation(kObject10430);
	_app->objectPresentationAddAnimationToPuzzle(kObject10430, 5, kPuzzle10411, "NIS04N03P03L02S01", kImageTypeBMP, Common::Point(215, 111), kDrawType1, 1000, 26, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject10430, 5, 0, 10425);
	_app->objectAddPresentation(kObject10430);
	_app->objectPresentationAddAnimationToPuzzle(kObject10430, 6, kPuzzle10411, "NIS04N03P03L02S02", kImageTypeBMP, Common::Point(275, 126), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject10440, "Door", "", 1);
	_app->objectAddRotationAccessibility(kObject10440, 10404, Common::Rect(2923, -370, 3549, 460), true, kCursorMove, 0);
	_app->objectAdd(kObject10450, "Water", "", 1);
	_app->objectAddRotationAccessibility(kObject10450, 10405, Common::Rect(2000, 400, 3600, 680), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject10450, 10415, Common::Rect(2000, 400, 3600, 680), true, kCursorHotspot, 0);
	_app->objectAdd(kObject10460, "Water", "", 1);
	_app->objectAddRotationAccessibility(kObject10460, 10406, Common::Rect(0, -600, 3599, 600), true, kCursorMove, 1);
	_app->objectAdd(kObject10432, "", "", 1);
	_app->objectAddPresentation(kObject10432);
	_app->objectPresentationAddAnimationToPuzzle(kObject10432, 0, kPuzzle10410, "NIS04N03P01S02", kImageTypeBMP, Common::Point(171, 285), kDrawType1, 1000, 13, 10.0f, 32);
	_app->objectPresentationAddAnimationToPuzzle(kObject10432, 0, kPuzzle10410, "NIS04N03P01S03", kImageTypeBMP, Common::Point(244, 268), kDrawType1, 1000, 12, 10.0f, 32);
	_app->objectPresentationAddAnimationToPuzzle(kObject10432, 0, kPuzzle10410, "NIS04N03P01S04", kImageTypeBMP, Common::Point(317, 261), kDrawType1, 1000, 7, 10.0f, 32);
	_app->objectAddPresentation(kObject10432);
	_app->objectPresentationAddAnimationToRotation(kObject10432, 1, 10401, 0, 25, 12.5f, 6);
	_app->objectPresentationAddAnimationToRotation(kObject10432, 1, 10402, 0, 26, 12.5f, 6);
	_app->objectAddPresentation(kObject10432);
	_app->objectPresentationAddAnimationToRotation(kObject10432, 2, 10401, 1, 25, 12.5f, 6);
	_app->objectPresentationAddAnimationToRotation(kObject10432, 2, 10402, 1, 26, 12.5f, 6);
	_app->objectAdd(kObject10431, "Temperat", "", 1);
	_app->objectAddPresentation(kObject10431);

	for (uint32 i = 1; i < 13; i++) {
		_app->objectAddPresentation(kObject10431);
		_app->objectPresentationAddImageToPuzzle(kObject10431, i, kPuzzle10410, Common::String::format("NIS04N03P01S01.%04d.bmp", i + 1), Common::Point(107, 155), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObjectDivingHelmet2, "Helmet&Frog", "NI_HelmetFrog", 1);
	_app->objectAddBagAnimation(kObjectDivingHelmet2, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectDivingHelmet2, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectDivingHelmet2, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectDivingHelmet, "Helmet", "NI_Helmet", 1);
	_app->objectAddBagAnimation(kObjectDivingHelmet, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectDivingHelmet, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectDivingHelmet, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectDivingHelmet, kPuzzle10501, Common::Rect(218, 249, 419, 346), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectDivingHelmet);
	_app->objectPresentationAddImageToPuzzle(kObjectDivingHelmet, 0, kPuzzle10501, "NIS05N01P01L02.0001.bmp", Common::Point(229, 252), true, kDrawType1, 1000);
	_app->objectAdd(kObjectAntiGCells, "AGCells", "RH_AntiG", 1);
	_app->objectAddBagAnimation(kObjectAntiGCells, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectAntiGCells, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectAntiGCells, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectAntiGCells, 10501, Common::Rect(2921, 68, 3185, 477), true, kCursorHotspot, 0);
	_app->objectAdd(kObject10600, "UTiles", "", 1);
	_app->objectAddPuzzleAccessibility(kObject10600, kPuzzle10600, Common::Rect(0, 420, 640, 464),  true, kCursorHotspot, 0);
	_app->objectAdd(kObject10601, "UTile1", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10601, kPuzzle10600, Common::Rect(215, 73, 430, 207), true, kCursorHandSelection, 0);
	_app->objectSetActiveDrawCursor(kObject10601, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10601, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 49; i++) {
		_app->objectAddPresentation(kObject10601);
		_app->objectPresentationAddImageToPuzzle(kObject10601, i, kPuzzle10600, Common::String::format("NIS06N01P01S01.%04d.bmp", i + 1), Common::Point(203, 82), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject10602, "UTile2", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10602, kPuzzle10600, Common::Rect(222, 211, 420, 266), true, kCursorHandSelection, 0);
	_app->objectSetActiveDrawCursor(kObject10602, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10602, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 49; i++) {
		_app->objectAddPresentation(kObject10602);
		_app->objectPresentationAddImageToPuzzle(kObject10602, i, kPuzzle10600, Common::String::format("NIS06N01P01S02.%04d.bmp", i + 1), Common::Point(245, 211), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject10603, "UTile3", "NI_HandSel", 4);
	_app->objectAddPuzzleAccessibility(kObject10603, kPuzzle10600, Common::Rect(216, 272, 424, 398), true, kCursorHandSelection, 0);
	_app->objectSetActiveDrawCursor(kObject10603, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObject10603, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 49; i++) {
		_app->objectAddPresentation(kObject10603);
		_app->objectPresentationAddImageToPuzzle(kObject10603, i, kPuzzle10600, Common::String::format("NIS06N01P01S03.%04d.bmp", i + 1), Common::Point(199, 272), true, kDrawType1, 1000);
	}

	_app->soundAdd(14001, kSoundTypeBackgroundMusic, "1583.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(14002, kSoundTypeBackgroundMusic, "1584.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(14003, kSoundTypeBackgroundMusic, "1585.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(14003, 90);
	_app->soundSetVolume(14001, 90);
	_app->soundSetVolume(14002, 90);
	_app->soundAdd(10101, kSoundTypeAmbientEffect, "1586.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10300, kSoundTypeAmbientEffect, "1587.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10301, kSoundTypeAmbientEffect, "1588.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(10800, kSoundTypeAmbientEffect, "1589.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10801, kSoundTypeAmbientEffect, "1590.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10900, kSoundTypeAmbientEffect, "1591.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10901, kSoundTypeAmbientEffect, "1592.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10902, kSoundTypeAmbientEffect, "1593.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10803, kSoundTypeAmbientEffect, "1594.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(10804, kSoundTypeAmbientEffect, "1595.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(10409, kSoundTypeAmbientEffect, "1596.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10410, kSoundTypeAmbientEffect, "1597.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10411, kSoundTypeAmbientEffect, "1598.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10412, kSoundTypeAmbientEffect, "1599.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10413, kSoundTypeAmbientEffect, "1600.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10501, kSoundTypeAmbientEffect, "1601.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10502, kSoundTypeAmbientEffect, "1602.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10102, kSoundTypeAmbientEffect, "1603.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10103, kSoundTypeAmbientEffect, "1604.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10104, kSoundTypeAmbientEffect, "1605.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10105, kSoundTypeAmbientEffect, "1606.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13001, kSoundTypeAmbientEffect, "1607.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13002, kSoundTypeAmbientEffect, "1608.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13003, kSoundTypeAmbientEffect, "1609.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13004, kSoundTypeAmbientEffect, "1610.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13005, kSoundTypeAmbientEffect, "1611.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13006, kSoundTypeAmbientEffect, "1612.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13007, kSoundTypeAmbientEffect, "1613.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13008, kSoundTypeAmbientEffect, "1614.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(13009, kSoundTypeAmbientEffect, "1615.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10415, kSoundTypeAmbientEffect, "1616.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(10106, kSoundTypeEffect, "1617.wav", _app->getConfiguration().effect.loadFrom);
	_app->soundAdd(10414, kSoundTypeEffect, "1618.wav", _app->getConfiguration().effect.loadFrom);
	_app->soundAdd(10401, kSoundTypeEffect, "1619.wav", _app->getConfiguration().effect.loadFrom);
	_app->soundAdd(10402, kSoundTypeEffect, "1620.wav", _app->getConfiguration().effect.loadFrom);
	_app->soundAdd(10403, kSoundTypeEffect, "1621.wav", _app->getConfiguration().effect.loadFrom);
	_app->soundAdd(10404, kSoundTypeEffect, "1616.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(10405, kSoundTypeEffect, "1622.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(10406, kSoundTypeEffect, "1623.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(10407, kSoundTypeEffect, "1624.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(10408, kSoundTypeEffect, "1625.wav", _app->getConfiguration().effect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(12001, kSoundTypeDialog, "1626.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12002, kSoundTypeDialog, "1627.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(12003, kSoundTypeDialog, "1628.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10001, kSoundTypeDialog, "1629.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10002, kSoundTypeDialog, "1630.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10003, kSoundTypeDialog, "1631.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10004, kSoundTypeDialog, "1632.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10005, kSoundTypeDialog, "1633.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10006, kSoundTypeDialog, "1634.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10007, kSoundTypeDialog, "1635.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10008, kSoundTypeDialog, "1636.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10009, kSoundTypeDialog, "1637.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10010, kSoundTypeDialog, "1638.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10011, kSoundTypeDialog, "1639.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10012, kSoundTypeDialog, "1640.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10013, kSoundTypeDialog, "1641.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10014, kSoundTypeDialog, "1642.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10015, kSoundTypeDialog, "1643.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10018, kSoundTypeDialog, "1644.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10016, kSoundTypeDialog, "1645.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10019, kSoundTypeDialog, "1646.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10017, kSoundTypeDialog, "1647.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10020, kSoundTypeDialog, "1648.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10021, kSoundTypeDialog, "1649.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10022, kSoundTypeDialog, "1650.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10023, kSoundTypeDialog, "1651.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10024, kSoundTypeDialog, "1652.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10025, kSoundTypeDialog, "1653.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10026, kSoundTypeDialog, "1654.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10027, kSoundTypeDialog, "1655.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10028, kSoundTypeDialog, "1656.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10029, kSoundTypeDialog, "1657.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10030, kSoundTypeDialog, "1658.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10031, kSoundTypeDialog, "1659.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10032, kSoundTypeDialog, "1660.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10037, kSoundTypeDialog, "1661.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10034, kSoundTypeDialog, "1662.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10035, kSoundTypeDialog, "1663.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10036, kSoundTypeDialog, "1664.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(10033, kSoundTypeDialog, "1665.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->rotationAdd3DSound(10301, 10301, 1, 1, 20, 100, 160.0f, 20);
	_app->rotationSet3DSoundOff(10301, 10301);
	_app->rotationAdd3DSound(10301, 10300, 1, 1, 20, 100, 160.0f, 5);
	_app->rotationAdd3DSound(10003, 10300, 1, 1, 20, 95, 195.0f, 10);
	_app->puzzleAdd3DSound(kPuzzle10300, 10300, 1, 1, 2, 93, 200.0f, 5);
	_app->rotationAdd3DSound(10403, 10412, 1, 1, 20, 90, 270.0f, 10);
	_app->rotationSet3DSoundOff(10403, 10412);
	_app->rotationAdd3DSound(10402, 10412, 1, 1, 20, 85, 270.0f, 10);
	_app->rotationSet3DSoundOff(10402, 10412);
	_app->rotationAdd3DSound(10401, 10412, 1, 1, 20, 80, 270.0f, 10);
	_app->rotationSet3DSoundOff(10401, 10412);
	_app->rotationAdd3DSound(10404, 10412, 1, 1, 20, 90, 270.0f, 10);
	_app->rotationSet3DSoundOff(10404, 10412);
	_app->puzzleAdd3DSound(kPuzzle10410, 10412, 1, 1, 2, 93, 300.0f, 5);
	_app->puzzleSet3DSoundOff(kPuzzle10410, 10412);
	_app->puzzleAdd3DSound(kPuzzle10411, 10412, 1, 1, 2, 95, 300.0f, 5);
	_app->puzzleSet3DSoundOff(kPuzzle10411, 10412);
	_app->puzzleAdd3DSound(kPuzzle10400, 10412, 1, 1, 2, 95, 205.0f, 5);
	_app->puzzleSet3DSoundOff(kPuzzle10400, 10412);
	_app->puzzleAdd3DSound(kPuzzle10401, 10412, 1, 1, 2, 95, 345.0f, 5);
	_app->puzzleSet3DSoundOff(kPuzzle10401, 10412);
	_app->varDefineWord(10100, 0);
	_app->varDefineWord(10101, 0);
	_app->varDefineWord(10600, 12);
	_app->varDefineWord(10601, 0);
	_app->varDefineWord(10602, 24);
	_app->varDefineDword(10000, 0);
	_app->varDefineDword(10001, 0);
	_app->varDefineByte(10432, 0);
	_app->varDefineByte(10200, 0);
	_app->varDefineByte(10000, 0);
	_app->varDefineByte(10300, 0);
	_app->varDefineByte(10301, 0);
	_app->varDefineByte(10302, 0);
	_app->varDefineByte(10303, 0);
	_app->varDefineByte(10420, 0);
	_app->varDefineByte(10421, 0);
	_app->varDefineByte(10430, 0);
	_app->varDefineByte(10431, 0);
	_app->varDefineByte(10100, 0);
	_app->varDefineByte(10101, 0);
	_app->varDefineByte(10102, 0);
	_app->varDefineByte(10103, 0);
	_app->varDefineByte(10104, 0);
	_app->varDefineByte(10105, 0);
	_app->varDefineByte(10500, 0);
	_app->varDefineByte(10501, 1);
	_app->varDefineByte(10106, 0);
	_app->varDefineByte(10107, 0);
	_app->varDefineByte(10113, 0);
	_app->varDefineByte(10108, 0);
	_app->varDefineByte(10109, 0);
	_app->varDefineByte(10110, 0);
	_app->varDefineByte(10111, 0);
	_app->varDefineByte(10112, 0);
}

void ZoneNIRing::onSetup(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->playMovie("1540");
		_app->soundPlay(14001, kSoundLoop);
		_app->playMovie("1541");
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10001);
		break;

	case kSetupType3:
		_app->timerStopAll();
		_app->puzzleSetMovabilityOn(kPuzzle10410, 0, 0);
		_app->playMovie("1550");
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		_app->soundPlay(14001, kSoundLoop);
		_app->soundPlay(10021);
		_app->varSetByte(10303, 1);
		break;

	case kSetupType10:
		_app->onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectGlug);
		_app->bagAdd(kObjectMinerals);
		_app->bagAdd(kObjectLogeTear);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->varSetByte(10106, 1);
		_app->objectPresentationShow(kObject1);
		_app->soundPlay(10409, kSoundLoop);
		_app->rotationSetActive(10415);
		break;
	}
}

void ZoneNIRing::onSwitch(uint32 type) {
	switch (type) {
	default:
		break;

	case 0:
		if (_app->varGetByte(90009))
			_app->setupZone((ZoneId)_app->varGetDword(90013), kSetupType10);
		else
			_app->setupZone(kZoneNI, kSetupTypeNone);
		break;

	case 3:
		_app->setupZone(kZoneNI, kSetupType3);
		break;

	case 9:
		_app->timerStart(kTimer9, 1000);
		break;
	}
}

void ZoneNIRing::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0: {
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10432) < 70) {
			_app->objectPresentationHide(kObject10431);
			_app->objectPresentationShow(kObject10431, ((uint8)_app->varGetByte(10432) - 10) / 5);
		} else if (_app->varGetByte(10432) == 100) {
			_app->objectPresentationShow(kObject10431, 12);
		}

		uint32 mod = (uint8)(120 - _app->varGetByte(10432)) / 10;
		if (mod < 5)
			mod = 5;

		if (!((uint8)(_app->varGetByte(10432)) % mod)) {
			_app->soundPlay(10415);
			_app->objectPresentationShow(kObject10432, rnd(2) + 1);
		}

		uint32 volume = (uint8)_app->varGetByte(10432) / 5 + 80;
		if (volume > 100)
			volume = 100;

		_app->soundSetVolume(10412, volume);

		if (_app->varGetByte(10412) > 120) {
			_app->timerStop(kTimer0);
			_app->soundStopType(kSoundTypeAmbientEffect, 1024);
			_app->soundStopType(kSoundTypeBackgroundMusic, 1024);
			_app->playMovie("1538");
			_app->exitToMenu(kMenuAction4);
		}
		}
		break;

	case kTimer1:
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10303) == 1) {
			_app->timerStopAll();
			_app->soundStopAll(1024);
			_app->varSetFloat(90005, 100.0f);
			_app->playMovie("1539");
			_app->onSwitchZone(kZoneAS, 1);
		}
		break;
	}
}

void ZoneNIRing::onButtonDown(ObjectId id, Id, Id puzzleRotationId, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject10101:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (puzzleRotationId) {
		default:
			break;

		case 0:
			_app->soundPlay(10103);
			_app->objectSetAccessibilityOff(kObject10101, 1, 2);
			_app->objectSetAccessibilityOff(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOff(kObject10110);
			_app->objectPresentationShow(kObject10101, _app->varGetByte(10102) ? 4 : 3);
			_app->objectPresentationHide(kObject10100);
			_app->objectPresentationHide(kObject10109);
			break;

		case 1:
			_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10102);
			_app->soundPlay(10102);
			_app->objectSetAccessibilityOff(kObject10101, 1, 2);
			_app->objectSetAccessibilityOff(kObject10102, 1, 2);
			_app->objectSetAccessibilityOff(kObject10100);
			_app->objectSetAccessibilityOff(kObject10103);

			if (_app->varGetByte(10102)) {
				if (_app->varGetByte(10113) == 9) {
					_app->objectPresentationHide(kObject10101, 0);
					_app->objectPresentationShow(kObject10101, 5);
				} else {
					_app->objectSetAccessibilityOn(kObject10110);
				}

				_app->objectPresentationShow(kObject10100, 1);
				_app->objectPresentationShow(kObject10101, 2);
			} else {
				_app->objectPresentationShow(kObject10101, 0);
				_app->objectPresentationShow(kObject10101, 1);
			}

			_app->objectPresentationShow(kObject10100, 0);
			break;
		}
		break;

	case kObject10102:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		if (_app->varGetByte(10113) != 9)
			break;

		switch (puzzleRotationId) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject10102, 1, 2);

			if (_app->varGetByte(10103)) {
				_app->soundPlay(10105);
				_app->objectSetAccessibilityOff(kObject10105);
				_app->objectPresentationShow(kObject10102, 3);
				_app->objectPresentationHide(kObject10100);
			} else {
				_app->objectSetAccessibilityOff(kObject10105);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationHide(kObject10102, 0);
				_app->objectPresentationHide(kObject10100);
				_app->objectSetAccessibilityOn(kObject10102, 0, 0);
				_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			}
			break;

		case 1:
			_app->objectPresentationHide(kObject10102);
			if (_app->varGetByte(10102) == 1) {
				_app->soundPlay(10104);
				_app->objectSetAccessibilityOff(kObject10101, 1, 2);
				_app->objectSetAccessibilityOff(kObject10102, 1, 2);
				_app->objectSetAccessibilityOff(kObject10100);
				_app->objectSetAccessibilityOff(kObject10103);
				_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationShow(kObject10102, 0);

				if (_app->varGetByte(10103)) {
					_app->objectPresentationShow(kObject10100, 2);
					_app->objectPresentationShow(kObject10102, 2);
				} else {
					_app->objectPresentationShow(kObject10102, 1);
				}
			} else {
				_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationShow(kObject10102, 4);

				// FIXME: Original calls it 4 times. Check why.
				for (uint32 i = 0; i < 4; i++)
					handleEvents();


				_app->objectPresentationHide(kObject10102, 4);
				_app->objectSetAccessibilityOn(kObject10101, 0, 0);
				_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			}
			break;
		}
		break;

	case kObject10109:
		if (!_app->bagHasClickedObject()) {
			_app->soundPlay(10106);
			_app->objectPresentationHide(kObject10109);
			_app->objectPresentationShow(kObject10109, (uint32)puzzleRotationId);

			if (puzzleRotationId) {
				_app->varSetByte(_app->varGetByte(10113) + 10108, (byte)puzzleRotationId);
				_app->varSetByte(10113, _app->varGetByte(10113) + 1);
			}
		}
		break;

	case kObject10430:
		_app->objectSetAccessibilityOff(kObject10430, 1, 2);
		_app->objectPresentationShow(kObject10430, _app->varGetByte(10430) ? 5 : 3);
		_app->objectPresentationHide(kObject10430, 0);
		_app->objectPresentationHide(kObject10430, 1);
		break;
	}
}

void ZoneNIRing::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject10100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			g_system->warpMouse(505, 205);
		}
		break;

	case kObject10104:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectTile) {
				if (!_app->varGetByte(10102)) {
					_app->soundPlay(10106);
					_app->varSetByte(10102, 1);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
					_app->objectPresentationShow(kObject10100, 1);
					_app->bagRemove(kObjectTile);
					_app->objectSetAccessibilityOff(kObject10104);
					_app->objectSetAccessibilityOn(kObject10109);
					_app->objectSetAccessibilityOn(kObject10110);
				}
			}

			_app->cursorDelete();
		}
		break;

	case kObject10105:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectBrutality) {
				if (!_app->varGetByte(10103)) {
					_app->varSetByte(10103, 1);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->playMovie("1510");
					_app->objectPresentationShow(kObject10100, 2);
					_app->soundPlay(10104);
					_app->objectPresentationShow(kObject10102, 2);
					_app->objectSetAccessibilityOff(kObject10100);
					_app->objectSetAccessibilityOff(kObject10103);
					_app->objectSetAccessibilityOff(kObject10101);
					_app->objectSetAccessibilityOff(kObject10102);
					_app->objectSetAccessibilityOff(kObject10105, 1, 1);
				}
			}
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(10103) == 1)
			_app->puzzleSetActive(kPuzzle10102);
		break;

	case kObjectGlug:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 1:
				if (_app->bagGetClickedObject() == kObjectGlug) {
					_app->playMovie("1513");
					_app->bagRemove(kObjectGlug);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->puzzleSetActive(kPuzzle10002);
					_app->varSetByte(10000, 1);
					_app->objectPresentationShow(kObjectGlug);
					_app->objectPresentationPauseAnimationFrame(kObjectGlug, 0, 1, 1000, 0);
					_app->soundPlay(10800, kSoundLoop);
					_app->cursorDelete();
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectMinerals) {
					if (!_app->bagHas(kObjectGlug)) {
						_app->soundStop(10800, 1024);
						_app->playMovie("1514");
						_app->rotationSetMovabilityOff(10003, 0, 0);
						_app->rotationSetMovabilityOff(10201, 0, 0);
						_app->rotationSetMovabilityOff(10401, 0, 0);
						_app->rotationSetMovabilityOn(_app->varGetDword(10000), 0, 0);

						if (!_app->soundIsPlaying(10901)) {
							_app->soundPlay(10900);
							_app->soundPlay(10901, kSoundLoop);
						}

						if (_app->varGetDword((int)10000) == 10401) {
							_app->objectPresentationHide(kObject10003, 3);
							_app->objectPresentationHide(kObject10003, 4);
							_app->objectPresentationHide(kObject10003, 5);
							_app->objectPresentationHide(kObject10003, 6);
							_app->objectPresentationHide(kObject10003, 7);
							_app->objectPresentationShow(kObject10003, 8);
							_app->objectPresentationShow(kObject10003, 9);
							_app->objectPresentationShow(kObject10003, 10);
							_app->objectPresentationShow(kObject10003, 11);
							_app->objectPresentationShow(kObject10003, 12);
						} else {
							_app->objectPresentationHide(kObject10003, 8);
							_app->objectPresentationHide(kObject10003, 9);
							_app->objectPresentationHide(kObject10003, 10);
							_app->objectPresentationHide(kObject10003, 11);
							_app->objectPresentationHide(kObject10003, 12);
							_app->objectPresentationShow(kObject10003, 3);
							_app->objectPresentationShow(kObject10003, 4);
							_app->objectPresentationShow(kObject10003, 5);
							_app->objectPresentationShow(kObject10003, 6);
							_app->objectPresentationShow(kObject10003, 7);
						}
					}
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 3:
			_app->varSetDword(10000, puzzleRotationId);
			_app->varSetDword(10001, (uint32)_app->rotationGetAlp(puzzleRotationId));
			_app->puzzleSetActive(kPuzzle10000);

			waitTicks(200);

			if (_app->varGetByte(10000)) {
				_app->puzzleSetActive(kPuzzle10002);
				_app->soundPlay(10800, kSoundLoop);
				_app->soundPlay(10803);
			} else {
				_app->puzzleSetActive(kPuzzle10001);
				_app->soundPlay(10803);
			}
			break;

		case 4:
			_app->puzzleSetActive(kPuzzle10000);

			waitTicks(200);

			_app->soundPlay(10804);
			_app->rotationSetAlp(_app->varGetDword(10000), _app->varGetDword(10001));
			_app->rotationSetBet(_app->varGetDword(10000), 60.0f);
			_app->rotationSetRan(_app->varGetDword(10000), 85.7f);
			_app->rotationSetActive(_app->varGetDword(10000));
			break;
		}
		break;

	case kObject10108:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		break;

	case kObject10110:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->soundPlay(target + 10032);
		break;

	case kObject10109:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (!target) {
			_app->soundPlay(10106);
			_app->objectPresentationHide(kObject10109, 0);

			handleEvents();

			if (_app->varGetByte(10108) != 5
			 || _app->varGetByte(10109) != 2
			 || _app->varGetByte(10110) != 3
			 || _app->varGetByte(10111) != 4
			 || _app->varGetByte(10112) != 1) {
				_app->varSetByte(10108, 0);
				_app->varSetByte(10109, 0);
				_app->varSetByte(10110, 0);
				_app->varSetByte(10111, 0);
				_app->varSetByte(10112, 0);
				_app->varSetByte(10113, 0);
			} else {
				_app->objectSetAccessibilityOff(kObject10109);
				_app->objectSetAccessibilityOff(kObject10110);
				_app->puzzleAddBackgroundImage(kPuzzle10100, "NIS01N01P02.0001.bmp", Common::Point(0, 16), true);
				_app->varSetByte(10113, 9);
				_app->objectPresentationHide(kObject10101, 0);
				_app->objectPresentationShow(kObject10101, 5);
				_app->objectPresentationShow(kObject10101, 6);
				_app->objectPresentationHide(kObject10109);
			}
		}
		break;

	case kObject10200:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectSetAccessibilityOff(kObject10200);
		_app->soundPlay(10022);
		break;

	case kObject10300:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectBrutality) {
			_app->playMovie("1512");

			if (_app->varGetByte(10301) == 1) {
				_app->varSetByte(10301, 2);
				_app->puzzleSetActive(kPuzzle10390);
				_app->soundPlay(10014);
				_app->cursorDelete();
				break;
			}

			_app->puzzleSetActive(kPuzzle10390);
			_app->soundPlay(rnd(3) + 10015);
		}

		_app->cursorDelete();
		break;

	case kObjectMagicFrog:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(10301, 0);
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10010);
		break;

	case kObjectTile:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10012);
		break;

	case kObjectLogeTear:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(10301, 0);
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10005);
		break;

	case kObject10440:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(10106) != 1
		 || _app->varGetByte(10431) != 1) {
			if (_app->varGetByte(10106)) {
				_app->playMovie("1516");

				if (_app->varGetByte(10106) == 1)
					if (!_app->soundIsPlaying(10409))
						_app->soundPlay(10409, kSoundLoop);

				_app->rotationSetAlp(10415, 270.0f);
				_app->rotationSetBet(10415, 0.3f);
				_app->rotationSetRan(10415, 85.7f);
				_app->rotationSetActive(10415);
			} else {
				_app->playMovie("1515");
				_app->rotationSetAlp(10405, 270.0f);
				_app->rotationSetBet(10405, 0.3f);
				_app->rotationSetRan(10405, 85.7f);
				_app->rotationSetActive(10405);
			}
		} else {
			_app->soundPlay(10414);
		}
		break;

	case kObject10430:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectLogeTear) {
				if (target == 1) {
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->objectPresentationShow(kObject10430, 6);
					_app->objectPresentationShow(kObject10430, 1);
					_app->bagRemove(kObjectLogeTear);
					_app->varSetByte(10430, 1);
				}
			}

			_app->cursorDelete();
			_app->sub_445A10();
			break;
		}

		_app->varSetByte(10431, 0);

		switch (target) {
		default:
			_app->sub_445A10();
			break;

		case 0:
			_app->objectPresentationHide(kObject10430);
			_app->puzzleSetMovabilityOff(10411);
			_app->objectSetAccessibilityOff(kObject10430, 0, 0);

			if (_app->varGetByte(10430)) {
				_app->objectPresentationShow(kObject10430, 4);
				_app->objectPresentationShow(kObject10430, 1);
			} else {
				_app->objectPresentationShow(kObject10430, 2);
			}

			_app->objectPresentationShow(kObject10430, 0);

			_app->sub_445A10();
			break;

		case 1:
			if (_app->varGetByte(10430) == 1) {
				_app->objectPresentationHide(kObject10430, 1);
				_app->objectPresentationHide(kObject10430, 6);
				_app->bagAdd(kObjectLogeTear);
				_app->varSetFloat(90005, _app->varGetFloat(90005) - 3.0f);
				_app->varSetByte(10430, 0);
			}

			_app->sub_445A10();
			break;
		}
		break;

	case kObject10450:
		if (_app->bagHasClickedObject()) {
			if (_app->varGetByte(10106) == 1) {
				if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
					_app->playMovie("1517");
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 8.0f);
					_app->rotationSetAlp(10406, 270.0f);
					_app->rotationSetBet(10406, 0.3f);
					_app->rotationSetRan(10406, 85.7f);
					_app->rotationSetActive(10406);
					_app->objectSetAccessibilityOff(kObject10450);
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1518");
				_app->exitToMenu(kMenuAction2);
				break;
			}
			break;
		}

		if (_app->varGetByte(10106) == 1) {
			_app->playMovie("1518");
			_app->exitToMenu(kMenuAction2);
		} else {
			_app->playMovie("1519");
			_app->exitToMenu(kMenuAction1);
		}
		break;

	case kObject10460:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->playMovie("1520");
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
		_app->onSwitchZone(kZoneRH, 0);
		break;

	case kObjectAntiGCells:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->playMovie("1521");
		_app->varSetByte(10501, 1);
		_app->bagAdd(kObjectAntiGCells);
		break;

	case kObjectDivingHelmet:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (!_app->varGetByte(10500)) {
			_app->objectPresentationShow(kObjectDivingHelmet);
			_app->varSetByte(10500, 1);
			_app->objectSetAccessibilityOff(kObjectDivingHelmet);
			_app->bagAdd(kObjectDivingHelmet);
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);

			if (_app->bagHas(kObjectMagicFrog)) {
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->bagRemove(kObjectMagicFrog);
				_app->bagRemove(kObjectDivingHelmet);
				_app->bagAdd(kObjectDivingHelmet2);
				_app->playMovie("1522");
			}
		}
		break;

	case kObject10600: {
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		int16 val1 = _app->varGetWord(10600) + 36;
		if (val1 > 47)
			val1 -= 48;

		int16 val2 = _app->varGetWord(10602) + 24;
		if (val2 > 47)
			val2 -= 48;

		uint32 index = 0;
		if (_app->varGetWord(10600) == 12)
			index = 1;
		if (!_app->varGetWord(10601))
			++index;
		if (_app->varGetWord(10602) == 24)
			++index;

		while (index < 3) {
			if (!_app->soundIsPlaying(10401))
				_app->soundPlay(10401);

			if (_app->varGetWord(10600) != 12) {
				_app->varSetWord(10600, (val1 <= 36 ? -1 : 1) + _app->varGetWord(10600));

				if (_app->varGetWord(10600) > 47)
					_app->varSetWord(10600, 0);

				if (_app->varGetWord(10600) < 0)
					_app->varSetWord(10600, 47);

				if (_app->varGetWord(10600) == 12)
					++index;

				_app->objectPresentationHideAndRemove(kObject10601);
				_app->objectPresentationShow(kObject10601, (uint16)_app->varGetWord(10600));
			}

			if (_app->varGetWord(10601)) {
				_app->varSetWord(10601, (_app->varGetWord(10601) <= 24 ? -1 : 1) + _app->varGetWord(10601));

				if (_app->varGetWord(10601) > 47)
					_app->varSetWord(10601, 0);

				if (_app->varGetWord(10601) < 0)
					_app->varSetWord(10601, 47);

				if (!_app->varGetWord(10601))
					++index;

				_app->objectPresentationHideAndRemove(kObject10602);
				_app->objectPresentationShow(kObject10602, (uint16)_app->varGetWord(10601));
			}

			if (_app->varGetWord(10602) != 24) {
				_app->varSetWord(10602, (val2 <= 24 ? -1 : 1) + _app->varGetWord(10602));

				if (_app->varGetWord(10602) > 47)
					_app->varSetWord(10602, 0);

				if (_app->varGetWord(10602) < 0)
					_app->varSetWord(10602, 47);

				if (_app->varGetWord(10602) == 24)
					++index;

				_app->objectPresentationHideAndRemove(kObject10603);
				_app->objectPresentationShow(kObject10603, (uint16)_app->varGetWord(10602));
			}

			handleEvents();
		}

		_app->rotationSetActive(10601);
		}
		break;
	}
}

void ZoneNIRing::onAnimationNextFrame(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId) {
	default:
		break;

	case 10000:
		if (frame == 1) {
			uint32 val = rnd(10);
			if (val > 4)
				val = 0;

			_app->objectPresentationPauseAnimationFrame(kObjectGlug, 0, 1, 300 * val, 0);
		}
		break;

	case 10101:
		if (frame == 36) {
			_app->objectSetAccessibilityOn(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOn(kObject10101, 1, 1);
		}
		break;

	case 10103:
		if (frame == 1) {
			_app->objectSetAccessibilityOff(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOn(kObject10101, 1, 1);

			if (_app->varGetByte(10113) < 9)
				_app->objectSetAccessibilityOn(kObject10109);
		}
		break;

	case 10100:
		if (frame == 1) {
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10101, 3);
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10102:
		if (frame == 36) {
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10101, 4);
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectPresentationHide(kObject10101, 5);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10105:
		if (frame == 36) {
			_app->objectSetAccessibilityOn(kObject10105, 1, 1);
			_app->objectSetAccessibilityOn(kObject10102, 1, 1);
		}
		break;

	case 10104:
		if (frame == 15) {
			_app->objectSetAccessibilityOn(kObject10102, 1, 1);
			_app->objectSetAccessibilityOn(kObject10105, 0, 0);
		}
		break;

	case 10106:
		if (frame == 1) {
			_app->objectPresentationHide(kObject10101, 0);
			_app->objectPresentationHide(kObject10102, 0);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			_app->objectSetAccessibilityOn(kObject10103);
		}
		break;

	case 10200:
		if (frame == 1) {
			uint32 val = rnd(10);
			if (val > 4)
				val = 0;

			_app->objectPresentationPauseAnimationFrame(kObject10200, 1, 1, 1000 * val, 0);
		}
		break;

	case 10300:
		if (frame == 3) {
			_app->rotationSet3DSoundOn(10301, 10301);
			_app->rotationSet3DSoundOff(10301, 10301);
			_app->soundPlay(10301);
		}
		break;

	case 10422:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectSetAccessibilityOn(kObject10430, 1, 2);
			break;

		case 2:
			_app->soundStop(10407, 1024);
			break;

		case 14:
			_app->soundPlay(10407);
			break;

		case 15:
			_app->soundStop(10405, 1024);
			break;

		case 26:
			_app->soundPlay(10405);
			break;
		}
		break;

	case 10423:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(10408);
			break;

		case 15:
			_app->soundPlay(10406);
			break;

		case 26:
			_app->soundStop(10406, 1024);
			_app->soundStop(10408, 1024);
			break;

		case 27:
			_app->puzzleSetMovabilityOn(kPuzzle10411);
			_app->objectSetAccessibilityOn(kObject10430, 0, 0);
			break;
		}
		break;

	case 10424:
		switch (frame) {
		default:
			break;

		case 1:
			_app->objectPresentationHide(kObject10432, 0);
			_app->objectPresentationShow(kObject10430, 6);
			_app->objectSetAccessibilityOn(kObject10430, 1, 2);
			break;

		case 2:
			_app->soundStop(10407, 1024);
			break;

		case 14:
			_app->soundPlay(10407);
			break;
		case 15:
			_app->soundStop(10405, 1024);
			break;

		case 26:
			_app->soundPlay(10405);
			break;
		}
		break;

	case 10425:
		switch (frame) {
		default:
			break;

		case 2:
			_app->soundPlay(10408);
			_app->objectPresentationHide(kObject10430, 6);
			break;

		case 15:
			_app->soundPlay(10406);
			break;

		case 26:
			_app->puzzleSetMovabilityOn(kPuzzle10411);
			_app->objectSetAccessibilityOn(kObject10430, 0, 0);
			_app->varSetByte(10431, 1);
			_app->objectPresentationShow(kObject10432, 0);
			_app->sub_445A10();
			_app->soundStop(10406, 1024);
			_app->soundStop(10408, 1024);
			break;
		}
		break;
	}
}

void ZoneNIRing::onSound(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 10001:
		_app->objectPresentationShow(kObject10300, 2);
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10002);
		return;

	case 10002:
		_app->bagAdd(kObjectGlug);
		_app->bagAdd(kObjectMinerals);
		_app->playMovie("1543");
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10003);
		return;

	case 10003:
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10004);
		return;

	case 10004:
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		break;

	case 10005:
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10006);
		return;

	case 10006:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10007);
		return;

	case 10007:
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10008);
		return;

	case 10008:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10009);
		return;

	case 10009:
		_app->playMovie("1544");
		_app->bagAdd(kObjectLogeTear);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
		_app->objectPresentationHide(kObjectLogeTear);
		_app->objectPresentationShow(kObjectLogeTear, 0);
		_app->objectSetAccessibilityOff(10305);
		_app->rotationSetActive(10301);
		break;

	case 10010:
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10011);
		return;

	case 10011:
		_app->playMovie("1545");
		_app->bagAdd(kObjectMagicFrog);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
		_app->objectSetAccessibilityOff(kObjectMagicFrog);
		_app->varSetByte(10300, 1);

		if (_app->bagHas(kObjectDivingHelmet)) {
			_app->bagRemove(kObjectMagicFrog);
			_app->bagRemove(kObjectDivingHelmet);
			_app->bagAdd(kObjectDivingHelmet2);
			_app->playMovie("1522");
		}

		_app->rotationSetActive(10301);
		break;

	case 10012:
		_app->playMovie("1546");
		_app->varSetByte(10301, 1);
		_app->puzzleSetActive(kPuzzle10391);
		_app->soundPlay(10013);
		return;

	case 10014:
		_app->playMovie("1547");
		_app->bagAdd(kObjectTile);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
		_app->objectPresentationShow(kObjectTile, 0);
		_app->objectSetAccessibilityOff(10303);
		_app->rotationSetActive(10301);
		break;

	case 10013:
	case 10018:
	case 10019:
	case 10020:
	case 10021:
		_app->rotationSetActive(10301);
		break;

	case 10015:
	case 10016:
	case 10017:
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(rnd(3) + 10018);
		break;

	case 10022:
		_app->puzzleSetActive(kPuzzle10202);
		_app->soundPlay(10023);
		break;

	case 10023:
		_app->puzzleSetActive(kPuzzle10200);
		_app->soundPlay(10024);
		break;

	case 10024:
		_app->puzzleSetActive(kPuzzle10202);
		_app->soundPlay(10025);
		break;

	case 10025:
		_app->puzzleSetActive(kPuzzle10200);
		_app->soundPlay(10026);
		break;

	case 10026:
		_app->objectSetAccessibilityOn(kObject10201, 0, 0);
		break;

	case 10027:
		_app->puzzleSetActive(kPuzzle10205);
		_app->soundPlay(10028);
		break;

	case 10028:
		_app->puzzleSetActive(kPuzzle10203);
		_app->soundPlay(10029);
		break;

	case 10029:
		_app->puzzleSetActive(kPuzzle10204);
		_app->soundPlay(10030);
		break;

	case 10030:
		_app->puzzleSetActive(kPuzzle10203);
		_app->soundPlay(10031);
		break;

	case 10031:
		_app->puzzleSetActive(kPuzzle10204);
		_app->soundPlay(10032);
		break;

	case 10032:
		_app->playMovie("1548");
		_app->playMovie("1549");
		_app->objectSetAccessibilityOn(kObject10200);
		_app->rotationSetActive(10201);
		_app->rotationSetMovabilityOff(10201, 0, 0);
		break;

	case 12001:
		_app->puzzleSetActive(kPuzzle12003);
		_app->soundPlay(12003);
		break;

	case 12002:
		_app->playMovie("1542");
		_app->puzzleSetActive(kPuzzle10521);
		break;

	case 12003:
		_app->puzzleSetActive(kPuzzle12002);
		_app->soundPlay(12002);
		break;
	}
}

void ZoneNIRing::onBag(ObjectId id, Id target, Id, uint32, DragControl *, byte type) {
	switch (id) {
	default:
		break;

	case kObject10103:
		switch (type) {
		default:
			break;

		case 1:
			_app->_presentationIndexNI = 0;
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(495, 194, 598, 284));
			break;

		case 2:
			if (!_app->soundIsPlaying(10401))
				_app->soundStop(10401, 1024);

			_app->objectSetAccessibilityOff(kObject10103);

			if (_app->_presentationIndexNI <= 6) {
				while (_app->_presentationIndexNI > 0) {
					--_app->_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_app->_presentationIndexNI);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject10103);
			} else {
				while (_app->_presentationIndexNI < 12) {
					++_app->_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_app->_presentationIndexNI);

					handleEvents();
				}

				uint32 startTick = g_system->getMillis();
				while (g_system->getMillis() - startTick < 1000)
					handleEvents();

				while (_app->_presentationIndexNI > 0) {
					--_app->_presentationIndexNI;

					_app->objectPresentationHide(kObject10103);
					_app->objectPresentationShow(kObject10103, (uint32)_app->_presentationIndexNI);

					handleEvents();
				}

				_app->objectSetAccessibilityOn(kObject10103);
				_app->objectPresentationHide(kObject10003, 8);
				_app->objectPresentationHide(kObject10003, 9);
				_app->objectPresentationHide(kObject10003, 10);
				_app->objectPresentationHide(kObject10003, 11);
				_app->objectPresentationHide(kObject10003, 12);
				_app->objectPresentationShow(kObject10003, 3);
				_app->objectPresentationShow(kObject10003, 4);
				_app->objectPresentationShow(kObject10003, 5);
				_app->objectPresentationShow(kObject10003, 6);
				_app->objectPresentationShow(kObject10003, 7);
				_app->playMovie("1511");
				_app->rotationSetAlp(10005, 270.0f);
				_app->rotationSetRan(10005, 85.7f);
				_app->rotationSetActive(10005);
				_app->rotationSetMovabilityOff(10005);
				_app->rotationSetMovabilityOn(10005, 1, 1);
				_app->soundPlay(10901, kSoundLoop);
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				_app->_presentationIndexNI = _app->dragControlGetOffsetX0() / 5;

				if (_app->dragControlXLower0()) {
					_app->_presentationIndexNI = 0;
				} else if (_app->_presentationIndexNI >= 0) {
					if (_app->_presentationIndexNI > 12)
						_app->_presentationIndexNI = 12;
				} else {
					_app->_presentationIndexNI = 0;
				}

				_app->objectPresentationHide(kObject10103);
				_app->objectPresentationShow(10103, (uint32)_app->_presentationIndexNI);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;

	case kObject10106:
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

			_distNI = val1 + val2 - _app->varGetWord(10100);
			if (_distNI < 0)
				_distNI	= 0;

			if (_distNI > 18)
				_distNI %= 19;

			_indexOffsetNI = (_app->varGetByte(10105) == 1) ? 19 : 256;
			}
			break;

		case 2: {
			int32 offset = 0;
			bool update = true;

			switch (_app->_presentationIndexNI) {
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
					_app->_presentationIndexNI += offset;

					if (_app->_presentationIndexNI <= 18) {
						if (_app->_presentationIndexNI < 0)
							_app->_presentationIndexNI = 18;
					} else {
						_app->_presentationIndexNI = 0;
					}

					_app->objectPresentationHideAndRemove(kObject10106);
					_app->objectPresentationShow(kObject10106, (uint32)(_app->_presentationIndexNI + _indexOffsetNI));
					sub_445930();

					handleEvents();

				} while (_app->_presentationIndexNI != 12);
			}

			_app->varSetWord(10100, (int16)_app->_presentationIndexNI);

			if (_app->_presentationIndexNI == 12) {
				_app->varSetByte(10104, 1);

				_app->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

				_app->rotationSetMovabilityOff(10102, 1, 1);
				_app->rotationSetMovabilityOn(10102, 2, 2);

				sub_445930();
			} else {
				_app->varSetByte(10104, 0);

				_app->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

				_app->rotationSetMovabilityOn(10102, 1, 1);
				_app->rotationSetMovabilityOff(10102, 2, 2);
				sub_445930();
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

				_app->_presentationIndexNI = val1 + val2 + _distNI;
				if (_app->_presentationIndexNI < 0)
					_app->_presentationIndexNI	= 0;

				if (_app->_presentationIndexNI > 18)
					_app->_presentationIndexNI %= 19;

				_app->objectPresentationHide(kObject10106);
				_app->objectPresentationShow(kObject10106, (uint32)(_app->_presentationIndexNI + _indexOffsetNI));
				_app->varSetWord(10100, (int16)_app->_presentationIndexNI);
				sub_445930();
			}
			break;
		}
		break;

	case kObject10107:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(295, 255, 345, 375));
			break;

		case 2:
			_app->objectPresentationHide(kObject10107);

			if (target) {
				if (_app->dragControlGetOffsetY0() < 20) {
					_app->objectPresentationShow(kObject10107, 13);
					_app->varSetByte(10105, 0);
					_app->sub_445A10();
					sub_445930();
					break;
				}

				if (_app->dragControlYHigher0()) {
					_app->objectPresentationShow(kObject10107, 13);
					_app->varSetByte(10105, 1);
				} else {
					_app->objectPresentationShow(kObject10107, 0);
					_app->objectSetAccessibilityOn(kObject10107, 0, 0);
					_app->objectSetAccessibilityOff(kObject10107, 1, 1);
					_app->varSetByte(10105, 0);
				}

				_app->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);

			} else {
				if (_app->dragControlGetOffsetY0() < 20) {
					_app->objectPresentationShow(kObject10107, 0);
					_app->varSetByte(10105, 0);
					_app->sub_445A10();
					sub_445930();
					break;
				}

				if (_app->dragControlYLower0()) {
					_app->objectPresentationShow(kObject10107, 0);
					_app->varSetByte(10105, 0);
				} else {
					_app->objectPresentationShow(kObject10107, 13);
					_app->objectSetAccessibilityOff(kObject10107, 0, 0);
					_app->objectSetAccessibilityOn(kObject10107, 1, 1);
					_app->varSetByte(10105, 1);
				}

				_app->sub_445A10();

				if (_app->varGetByte(10106) == 1)
					_app->soundPlay(10101);
			}

			sub_445930();
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 index = 0;

				if (target) {
					index = 13 - _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYHigher0())
						index = 13;
				} else {
					index = _app->dragControlGetOffsetY0() / 3;

					if (_app->dragControlYLower0())
						index = 0;
				}

				_app->varSetByte(10105, 0);

				if (index >= 0) {
					if (index > 12) {
						index = 13;
						_app->varSetByte(10105, 1);
					}
				} else {
					index = 0;
				}

				sub_445930();
				_app->objectPresentationHide(kObject10107);
				_app->objectPresentationShow(kObject10107, (uint32)index);
				sub_445930();
			}
			break;
		}
		break;

	case kObject10201:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(299, 214, 431, 356));
			break;

		case 2:
			if (!_app->soundIsPlaying(10401))
				_app->soundStop(10401, 1024);

			if (_app->_presentationIndexNI <= 3) {
				while (_app->_presentationIndexNI > 0) {
					--_app->_presentationIndexNI;

					_app->objectPresentationHide(kObject10201);
					_app->objectPresentationShow(kObject10201, (uint32)_app->_presentationIndexNI);

					handleEvents();
				}
			} else {
				if (_app->_presentationIndexNI <= 10) {
					int32 offset = (_app->_presentationIndexNI >= 6) ? -1 : 1;
					while (_app->_presentationIndexNI != 6) {
						_app->_presentationIndexNI += offset;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_app->_presentationIndexNI);

						handleEvents();
					}

					uint32 startTick = g_system->getMillis();
					while (g_system->getMillis() - startTick < 1000)
						handleEvents();

					while (_app->_presentationIndexNI > 0) {
						--_app->_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_app->_presentationIndexNI);

						handleEvents();
					}

					_app->playMovie("1527");

					if (_app->varGetByte(10200)) {
						_app->puzzleSetActive(kPuzzle10202);
						_app->soundPlay(10030);
					} else {
						_app->varSetByte(10200, 1);
						_app->puzzleSetActive(kPuzzle10203);
						_app->soundPlay(10027);
						_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					}
				} else {
					_app->objectSetAccessibilityOff(kObject10201);
					while (_app->_presentationIndexNI < 13) {
						++_app->_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_app->_presentationIndexNI);

						handleEvents();
					}

					uint32 startTick = g_system->getMillis();
					while (g_system->getMillis() - startTick < 1000)
						handleEvents();

					while (_app->_presentationIndexNI > 0) {
						--_app->_presentationIndexNI;

						_app->objectPresentationHide(kObject10201);
						_app->objectPresentationShow(kObject10201, (uint32)_app->_presentationIndexNI);

						handleEvents();
					}

					_app->playMovie("1525");
					_app->playMovie("1526");
					_app->objectSetAccessibilityOn(kObject10200);
				}
			}
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				_app->_presentationIndexNI = _app->dragControlGetOffsetY0() / 5;

				if (_app->dragControlYLower0()) {
					_app->_presentationIndexNI = 0;
				} else if (_app->_presentationIndexNI >= 0) {
					if (_app->_presentationIndexNI > 13)
						_app->_presentationIndexNI = 13;
				} else {
					_app->_presentationIndexNI = 0;
				}

				_app->objectPresentationHide(kObject10201);
				_app->objectPresentationShow(10201, (uint32)_app->_presentationIndexNI);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;

	case kObject10420:
	case kObject10421:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);
			_app->dragControlSetHotspot(Common::Rect(263, 206, 390, 341));
			_stateBagNI = (target != 0);
			break;

		case 2:
			_app->objectPresentationHide(id);
			if (target) {
				if (_app->dragControlGetOffsetY0() < 30 || _app->dragControlYHigher0()) {
					_app->objectPresentationShow(id, 12);
					_app->varSetByte(id, 1);
				} else {
					_app->objectPresentationShow(id, 1);
					_app->objectSetAccessibilityOn(id, 0, 0);
					_app->objectSetAccessibilityOff(id, 1, 1);
					_app->varSetByte(id, 0);
				}
			} else {
				if (_app->dragControlGetOffsetY0() < 30 || _app->dragControlYLower0()) {
					_app->objectPresentationShow(id, 1);
					_app->varSetByte(id, 0);
				} else {
					_app->objectPresentationShow(id, 12);
					_app->objectSetAccessibilityOff(id, 0, 0);
					_app->objectSetAccessibilityOn(id, 1, 1);
					_app->varSetByte(id, 1);
				}
			}

			_app->sub_445A10();
			break;

		case 3:
			if (!_app->dragControlYEqual()) {
				int32 offset = 0;
				if (target) {
					offset = 11 - _app->dragControlGetOffsetY0() / 5;

					if (_app->dragControlXHigher0())
						offset = 12;
				} else {
					offset = _app->dragControlGetOffsetY0() / 5 + 1;

					if (_app->dragControlXLower0())
						offset = 1;
				}

				if (offset < 1)
					offset = 1;

				if (offset > 12)
					offset = 12;

				_app->objectPresentationHideAndRemove(id);
				_app->objectPresentationShow(id, (uint32)offset);

				if (offset <= 8 || _stateBagNI) {
					if (offset < 5 && _stateBagNI) {
						_app->soundStop(10404, 1024);
						_app->soundPlay(10403);
						_app->varSetFloat(90005, _app->varGetFloat(90005) - 3.0f);

						_stateBagNI = false;
					}
				} else {
					_app->soundPlay(10402);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);

					_stateBagNI = true;

					if (_app->varGetByte(10107)) {
						_app->varSetByte(10432, _app->varGetByte(10432) / 2);

						if (id == kObject10420)
							_app->objectSetAccessibilityOff(kObject10420);

						_app->soundPlay(10404);
						_app->sub_445A10();
					}
				}
			}
			break;
		}
		break;

	case kObject10601:
	case kObject10602:
	case kObject10603:
		switch (type) {
		default:
			break;

		case 1:
			_app->_presentationIndexNI = _app->varGetWord(id - 1);
			break;

		case 2:
			_app->varSetWord(id - 1, (int16)_app->_presentationIndexNI);

			if (!_app->varGetWord(10600)
			 && !_app->varGetWord(10601)
			 && !_app->varGetWord(10602)) {
				_app->varSetWord(10600, 12);
				_app->varSetWord(10601, 0);
				_app->varSetWord(10602, 24);
				_app->playMovie("1524");
				_app->rotationSetAlp(10501, 232.0f);
				_app->rotationSetActive(10501);
			}
			break;

		case 3:
			if (!_app->dragControlXEqual()) {
				if (_app->dragControlXLower0()) {
					_app->_presentationIndexNI = _app->varGetWord(id - 1) - ((int32)_app->dragControlGetOffsetX0() / 12);
				} else if (_app->dragControlXHigher0()) {
					_app->_presentationIndexNI = _app->varGetWord(id - 1) + ((int32)_app->dragControlGetOffsetX0() / 12);
				}

				if (_app->_presentationIndexNI < 0)
					_app->_presentationIndexNI  = 47;

				if (_app->_presentationIndexNI > 47)
					_app->_presentationIndexNI %= 48;

				_app->objectPresentationHideAndRemove(id);
				_app->objectPresentationShow(id);

				if (!_app->soundIsPlaying(10401))
					_app->soundPlay(10401);
			}
			break;
		}
		break;
	}
}

void ZoneNIRing::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
	if (movabilityFrom == 10100 && movabilityTo == 0) {
		_app->rotationSetActive(10101);
		_app->objectSetAccessibilityOn(kObject10101, 0, 0);
		_app->objectSetAccessibilityOn(kObject10102, 0, 0);
		_app->objectSetAccessibilityOn(kObject10101, 2, 2);
		_app->objectSetAccessibilityOn(kObject10102, 2, 2);
		_app->objectSetAccessibilityOn(kObject10100, 4, 4);

		g_system->warpMouse(point.x, point.y);
	}
}

void ZoneNIRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 10005)
			if (movabilityTo == 10101) {
				_app->soundStop(10901, 1024);
				_app->soundPlay(rnd(9) + 13001);
			}
		break;

	case kMovabilityRotationToPuzzle:
		switch (movabilityIndex) {
		default:
			break;

		case 41:
			if (_app->varGetByte(10420))
				_app->playMovie("1529");
			else
				_app->playMovie("1528");
			break;

		case 42:
			if (_app->varGetByte(10421))
				_app->playMovie("1531");
			else
				_app->playMovie("1530");
			break;

		case 61:
			_app->varSetWord(10600, 12);
			_app->varSetWord(10601, 0);
			_app->varSetWord(10602, 24);

			_app->objectPresentationHideAndRemove(kObject10601);
			_app->objectPresentationShow(kObject10601, 12);
			_app->objectPresentationHideAndRemove(kObject10602);
			_app->objectPresentationShow(kObject10602, 0);
			_app->objectPresentationHideAndRemove(kObject10603);
			_app->objectPresentationShow(kObject10603, 24);
			break;
		}
		break;

	case kMovabilityPuzzleToRotation:
		switch (movabilityIndex) {
		default:
			break;

		case 41:
			if (_app->varGetByte(10420))
				_app->playMovie("1534");
			else
				_app->playMovie("1533");
			break;

		case 42:
			if (_app->varGetByte(10421))
				_app->playMovie("1536");
			else
				_app->playMovie("1535");
			break;
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (movabilityTo == 10501 || movabilityTo == 10511 || movabilityTo == 10521)
			if (!movabilityIndex)
				_app->soundPlay(10501);

		if (movabilityFrom == 10501 || movabilityFrom == 10511 || movabilityFrom == 10521)
			if (!movabilityIndex)
				_app->soundPlay(10502);

		if (movabilityFrom == 10001) {
			_app->soundPlay(10804);
		} else if (movabilityFrom == 10002) {
			_app->soundPlay(10804);
			_app->soundStop(10800, 1024);
		}
		break;
	}
}

void ZoneNIRing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32, Id target, MovabilityType movabilityType) {
	if (target == 100) {
		_app->rotationSetMovabilityOff(movabilityFrom, 0, 0);
	} else if (target == 110) {
		if (_app->soundIsPlaying(10800))
			_app->soundStop(10800, 1024);

		if (_app->soundIsPlaying(10901))
			_app->soundPlay(10902);

		_app->soundStop(10901, 1024);

		if (movabilityFrom == 10301) {
			_app->soundPlay(10300, kSoundLoop);
			_app->varSetByte(10301, 0);
		}
	}

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 10201) {
			if (!_app->soundIsPlaying(14002)) {
				_app->soundStop(14001, 1024);
				_app->soundStop(14003, 1024);
				_app->soundPlay(14002, kSoundLoop);
			}
		}

		if (movabilityFrom > 10201 || movabilityFrom == 10101) {
			if (!_app->soundIsPlaying(14003)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14001, 1024);
				_app->soundPlay(14003, kSoundLoop);
			}
		}

		if (movabilityFrom >= 10000 && movabilityFrom <= 10005) {
			if(!_app->soundIsPlaying(14001)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14003, 1024);
				_app->soundPlay(14001, kSoundLoop);
			}
		}

		switch (target) {
		default:
			break;

		case 1:
			_app->rotationSetMovabilityOff(movabilityFrom);
			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);

			if (movabilityFrom == 10000 || movabilityFrom == 10002)
				_app->rotationSetMovabilityOn(movabilityFrom, 2, 2);
			break;

		case 2:
			_app->rotationSetMovabilityOff(movabilityFrom);
			_app->rotationSetMovabilityOn(movabilityFrom, 1, 1);
			break;

		case 3:
			_app->rotationSetMovabilityOff(movabilityFrom, 0, 0);
			break;

		case 5:
		case 21:
			_app->objectPresentationHide(kObject10003, 8);
			_app->objectPresentationHide(kObject10003, 9);
			_app->objectPresentationHide(kObject10003, 10);
			_app->objectPresentationHide(kObject10003, 11);
			_app->objectPresentationHide(kObject10003, 12);
			_app->objectPresentationShow(kObject10003, 3);
			_app->objectPresentationShow(kObject10003, 4);
			_app->objectPresentationShow(kObject10003, 5);
			_app->objectPresentationShow(kObject10003, 6);
			_app->objectPresentationShow(kObject10003, 7);

			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);
			break;

		case 16:
			_app->exitToMenu(kMenuAction3);
			return;

		case 41:
			_app->objectPresentationHide(kObject10003, 3);
			_app->objectPresentationHide(kObject10003, 4);
			_app->objectPresentationHide(kObject10003, 5);
			_app->objectPresentationHide(kObject10003, 6);
			_app->objectPresentationHide(kObject10003, 7);
			_app->objectPresentationShow(kObject10003, 8);
			_app->objectPresentationShow(kObject10003, 9);
			_app->objectPresentationShow(kObject10003, 10);
			_app->objectPresentationShow(kObject10003, 11);
			_app->objectPresentationShow(kObject10003, 12);

			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);
			break;
		}

		if (movabilityTo == 10005) {
			if (movabilityFrom == 10101) {
				_app->soundStop(13001, 1024);
				_app->soundStop(13002, 1024);
				_app->soundStop(13003, 1024);
				_app->soundStop(13004, 1024);
				_app->soundStop(13005, 1024);
				_app->soundStop(13006, 1024);
				_app->soundStop(13007, 1024);
				_app->soundStop(13008, 1024);
				_app->soundStop(13009, 1024);

				return;
			}
		} else if (movabilityTo == 10415) {
			_app->soundStop(10409, 1024);
		}

		if (movabilityFrom == 10406) {
			if (!_app->soundIsPlaying(10410)) {
				_app->soundPlay(10410, kSoundLoop);
				_app->soundPlay(10411, kSoundLoop);
			}
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (target == 55) {
			_app->playMovie("1537");
			_app->puzzleSetActive(kPuzzle12001);
			_app->soundPlay(12001);
		}
		break;
	}
}

#pragma region Helper functions

void ZoneNIRing::sub_445930() {
	if (_app->varGetByte(10105) == 1) {

		_app->objectPresentationHide(kObject10106);
		_app->objectPresentationShow(kObject10106, 38);
		_app->objectPresentationShow(kObject10106, (uint16)_app->varGetWord(10100) + 19);

		if (!_app->varGetWord(10100)) {
			_app->objectPresentationShow(kObject10106, 39);
			_app->varSetByte(10106, 1);
		}

	} else {
		_app->objectPresentationHide(kObject10106);
		_app->objectPresentationShow(kObject10106, (uint16)_app->varGetWord(10100));
		_app->varSetByte(10106, 0);
	}
}

void ZoneNIRing::waitTicks(uint32 ticks) const {
	uint32 startTicks = g_system->getMillis();
	while (g_system->getMillis() - startTicks < ticks)
		handleEvents();
}

#pragma endregion

} // End of namespace Ring
