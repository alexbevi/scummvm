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

#include "ring/game/ring/ring_zonefo.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneFORing::ZoneFORing(ApplicationRing *application) : _app(application) {
}

ZoneFORing::~ZoneFORing() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneFORing::onInit() {
	debugC(1, kRingDebugLogic, "Initializing Siegmund zone (FO)...");

	_app->setZoneAndEnableBag(kZoneFO);

	_app->puzzleAdd(kPuzzle35001);
	_app->puzzleAddBackgroundImage(kPuzzle35001, "FOS03N02P01S00.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35002);
	_app->puzzleAddBackgroundImage(kPuzzle35002, "FOS08N01P01.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35003);
	_app->puzzleAddBackgroundImage(kPuzzle35003, "FOS06N01P06.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35004);
	_app->puzzleAddBackgroundImage(kPuzzle35004, "FOS06N01P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35005);
	_app->puzzleAddBackgroundImage(kPuzzle35005, "FOS06N01P05.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35006);
	_app->puzzleAddBackgroundImage(kPuzzle35006, "FOS06N01P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35007);
	_app->puzzleAddBackgroundImage(kPuzzle35007, "FOS06N01P03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35008);
	_app->puzzleAddBackgroundImage(kPuzzle35008, "FOS06N01P04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35009);
	_app->puzzleAddBackgroundImage(kPuzzle35009, "FOS06N02P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35010);
	_app->puzzleAddBackgroundImage(kPuzzle35010, "FOS07N03P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35011);
	_app->puzzleAddBackgroundImage(kPuzzle35011, "FOS03N01P01.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35019);
	_app->puzzleAddBackgroundImage(kPuzzle35019, "FOS00N04P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35020);
	_app->puzzleAddBackgroundImage(kPuzzle35020, "FOS07N04P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35100);
	_app->puzzleAddBackgroundImage(kPuzzle35100, "FOS00N03P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35101);
	_app->puzzleAddBackgroundImage(kPuzzle35101, "FOS00N03P02L02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35103);
	_app->puzzleAddBackgroundImage(kPuzzle35103, "FOS01N01P07.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35104);
	_app->puzzleAddBackgroundImage(kPuzzle35104, "FOS01N01P03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35105);
	_app->puzzleAddBackgroundImage(kPuzzle35105, "FOS01N01P04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35110);
	_app->puzzleAddBackgroundImage(kPuzzle35110, "FOS01N01P09.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35106);
	_app->puzzleAddBackgroundImage(kPuzzle35106, "FOS01N01P05.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35107);
	_app->puzzleAddBackgroundImage(kPuzzle35107, "FOS01N01P06.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35108);
	_app->puzzleAddBackgroundImage(kPuzzle35108, "FOS01N01P10.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35111);
	_app->puzzleAddBackgroundImage(kPuzzle35111, "FOS01N01P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle35109);
	_app->puzzleAddBackgroundImage(kPuzzle35109, "FOS01N01P11.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(30001, "FOS00N01", 0, 1);
	_app->rotationAdd(30002, "FOS00N02", 0, 0);
	_app->rotationAdd(30003, "FOS00N03", 0, 1);
	_app->rotationSetCompressionBufferLength(30003, 1400000);
	_app->rotationAdd(30004, "FOS00N04", 0, 0);
	_app->rotationAdd(30005, "FOS00N05", 0, 0);
	_app->rotationAdd(30006, "FOS00N06", 0, 0);
	_app->rotationAdd(30008, "FOS00N08", 0, 1);
	_app->rotationAdd(30009, "FOS00N09", 0, 1);
	_app->rotationAdd(30010, "FOS00N10", 0, 1);
	_app->rotationAdd(30011, "FOS00N11", 0, 3);
	_app->rotationSetCompressionBufferLength(30011, 1500000);
	_app->rotationAdd(30012, "FOS00N12", 0, 0);
	_app->rotationAdd(30101, "FOS01N01", 0, 4);
	_app->rotationSetCompressionBufferLength(30101, 2000000);
	_app->rotationAdd(30301, "FOS03N01", 0, 1);
	_app->rotationAdd(30302, "FOS03N02", 0, 0);
	_app->rotationAdd(30303, "FOS03N03", 0, 0);
	_app->rotationAdd(30401, "FOS04N01", 0, 0);
	_app->rotationAdd(30402, "FOS04N02", 0, 1);
	_app->rotationAdd(30501, "FOS05N01", 0, 3);
	_app->rotationAdd(30601, "FOS06N01", 0, 1);
	_app->rotationAdd(30602, "FOS06N02", 0, 1);
	_app->rotationSetCompressionBufferLength(30602, 1750000);
	_app->rotationAdd(30701, "FOS07N01", 0, 0);
	_app->rotationAdd(30702, "FOS07N02", 0, 1);
	_app->rotationAdd(30703, "FOS07N03", 0, 1);
	_app->rotationAdd(30704, "FOS07N04", 0, 0);
	_app->rotationAdd(30801, "FOS08N01", 0, 0);
	_app->rotationAddMovabilityToRotation(30001, 30003, "1219", Common::Rect(2787, -143, 3113, 169), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30001, 0, 247.0f, 0, 85.3f, 0, 2, 247.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30001, 30004, "1219", Common::Rect(3406, -125, 3599, 73), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30001, 1, 312.0f, 0, 85.3f, 0, 2, 312.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30001, 30004, "1219", Common::Rect(0, -125, 72, 73), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30001, 2, 312.0f, 0, 85.3f, 0, 2, 312.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30002, 30003, "1220", Common::Rect(272, -81, 437, 118), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30002, 0, 356.0f, 0, 85.3f, 0, 2, 356.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30002, 30004, "1220", Common::Rect(89, -104, 254, 45), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30002, 1, 338.0f, 0, 85.3f, 0, 2, 338.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30003, 30001, "1221", Common::Rect(954, -129, 1320, 190), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30003, 0, 75.0f, 0, 85.3f, 0, 2, 75.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30003, 30002, "1222", Common::Rect(2002, -97, 2364, 156), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30003, 1, 167.0f, 0, 85.3f, 0, 2, 167.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30003, 30004, "1223", Common::Rect(45, -104, 261, 136), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30003, 2, 330.0f, 0, 85.3f, 0, 2, 330.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30004, 30001, "1224", Common::Rect(1620, -75, 1773, 52), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30004, 0, 128.0f, 0, 85.3f, 0, 2, 128.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30004, 30002, "1224", Common::Rect(1928, -48, 2037, 38), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30004, 1, 149.0f, 0, 85.3f, 0, 2, 149.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30004, 30003, "1224", Common::Rect(1791, -54, 1900, 73), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30004, 2, 138.0f, 0, 85.3f, 0, 2, 138.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30004, 30005, "1225", Common::Rect(40, -115, 274, 146), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30004, 3, 331.0f, 0, 85.3f, 0, 2, 331.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30004, 30009, "1226", Common::Rect(402, -116, 601, 75), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30004, 4, 0, 0, 85.3f, 0, 2, 0, 0, 85.3f);
	_app->rotationAddMovabilityToPuzzle(30004, kPuzzle35019, "", Common::Rect(1033, -116, 1233, 120), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30004, 5, 70.0f, 0, 85.3f, 0, 2);
	_app->rotationAddMovabilityToRotation(30005, 30004, "1227", Common::Rect(1587, -134, 1829, 87), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30005, 0, 123.0f, 0, 85.3f, 0, 2, 123.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30005, 30006, "1228", Common::Rect(3311, -127, 3513, 113), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30005, 1, 292.0f, 0, 85.3f, 0, 2, 356.0f, -15.0f, 85.3f);
	_app->rotationAddMovabilityToRotation(30006, 30005, "1229", Common::Rect(1817, -179, 2149, 144), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30006, 0, 152.0f, 0, 85.3f, 0, 2, 152.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30006, 30008, "1230", Common::Rect(249, -385, 580, 26), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30006, 1, 356.0f, -15.0f, 85.3f, 0, 2, 356.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30008, 30006, "1231", Common::Rect(2037, 130, 2455, 362), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30008, 0, 180.0f, 15.0f, 85.3f, 0, 2, 180.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30008, 30301, "1232", Common::Rect(358, -43, 567, 113), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30008, 1, 0, 0, 85.3f, 0, 2, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30009, 30004, "1233", Common::Rect(2536, -68, 2698, 87), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30009, 0, 216.0f, 0, 85.3f, 0, 2, 216.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30009, 30010, "1234", Common::Rect(924, -136, 1121, 123), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30009, 1, 58.0f, 0, 85.3f, 0, 2, 58.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30009, 30005, "1233", Common::Rect(2726, -88, 2940, 90), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30009, 2, 300.0f, 0, 85.3f, 0, 2, 58.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30010, 30009, "1235", Common::Rect(2583, -137, 2845, 160), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30010, 0, 230.0f, 0, 85.3f, 0, 2, 230.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30010, 30011, "1236", Common::Rect(1469, -47, 1668, 134), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30010, 1, 106.0f, 0, 85.3f, 0, 2, 106.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30010, 30012, "1237", Common::Rect(573, -52, 703, 108), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30010, 2, 21.0f, 0, 85.3f, 0, 2, 21.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30011, 30010, "1238", Common::Rect(3107, -45, 3309, 163), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30011, 0, 265.0f, 0, 85.3f, 0, 2, 265.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30011, 30401, "1239", Common::Rect(3517, 195, 3600, 418), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30011, 1, 344.0f, 15.0f, 85.3f, 0, 2, 330.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30011, 30401, "1239", Common::Rect(0, 195, 769, 418), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30011, 2, 344.0f, 15.0f, 85.3f, 0, 2, 330.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30012, 30010, "1240", Common::Rect(2290, -64, 2599, 111), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30012, 0, 207.0f, 0, 85.3f, 0, 2, 207.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30012, 30501, "1241", Common::Rect(543, -83, 768, 329), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30012, 1, 21.0f, 6.0f, 85.3f, 0, 2, 21.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30101, 30003, "1242", Common::Rect(3594, -76, 3599, 423), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30101, 0, 327.0f, 15.0f, 85.3f, 0, 2, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30101, 30003, "1242", Common::Rect(0, -76, 469, 423), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30101, 1, 327.0f, 15.0f, 85.3f, 0, 2, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30301, 30008, "1243", Common::Rect(1979, -71, 2418, 214), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30301, 0, 180.0f, 15.0f, 85.3f, 0, 2, 180.0f, 15.0f, 85.3f);
	_app->rotationAddMovabilityToPuzzle(30301, kPuzzle35011, "", Common::Rect(1907, -127, 1965, 13), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30301, 1, 150.0f, 3.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30303, kPuzzle35001, "", Common::Rect(2023, 62, 2255, 252), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30303, 0, 170.0f, 15.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToRotation(30401, 30402, "1244", Common::Rect(442, -134, 782, 153), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30401, 0, 354.0f, 0, 85.3f, 0, 2, 261.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30401, 30011, "1245", Common::Rect(2074, 141, 2808, 345), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30401, 1, 200.0f, 15.0f, 85.3f, 0, 2, 190.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30401, 30801, "1246", Common::Rect(1014, -104, 1239, 41), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30401, 2, 70.0f, 0, 85.3f, 0, 2, 73.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30402, 30401, "1247", Common::Rect(1745, -59, 2100, 104), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30402, 0, 170.0f, 0, 85.3f, 0, 2, 186.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30402, 30801, "1248", Common::Rect(1318, -92, 1504, 17), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30402, 1, 92.0f, 0, 85.3f, 0, 2, 80.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30402, 30501, "1249", Common::Rect(2967, -102, 3237, 190), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30402, 2, 261.0f, 0, 85.3f, 0, 2, 30.0f, 15.0f, 85.3f);
	_app->rotationSetMovabilityOff(30402, 2, 2);
	_app->rotationAddMovabilityToRotation(30501, 30012, "1250", Common::Rect(1981, -64, 2218, 324), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30501, 0, 165.0f, 8.0f, 85.3f, 0, 2, 207.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30501, 30601, "1251", Common::Rect(390, -247, 766, 179), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30501, 1, 2.0f, -15.0f, 85.3f, 0, 2, 207.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30601, 30602, "1253", Common::Rect(1571, -68, 1916, 132), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30601, 0, 142.0f, -15.0f, 85.3f, 0, 2, 256.0f, -15.0f, 85.3f);
	_app->rotationAddMovabilityToRotation(30601, 30602, "1253", Common::Rect(1274, 73, 1778, 525), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30601, 1, 142.0f, -15.0f, 85.3f, 0, 2, 256.0f, -15.0f, 85.3f);
	_app->rotationAddMovabilityToRotation(30601, 30501, "1254", Common::Rect(2654, 191, 3427, 436), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30601, 2, 260.0f, 15.0f, 85.3f, 0, 2, 81.0f, 15.0f, 85.3f);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35003, "", Common::Rect(3529, -177, 3599, 108), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 3, 315.0f, 1.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35003, "", Common::Rect(0, -177, 96, 108), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 4, 315.0f, 1.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35004, "", Common::Rect(2077, -43, 2466, 251), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 5, 180.0f, 13.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35005, "", Common::Rect(530, -193, 703, 102), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 6, 17.0f, 0, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35006, "", Common::Rect(174, -111, 451, 52), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 7, 347.0f, 0, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35007, "", Common::Rect(1775, 127, 2065, 375), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 8, 145.0f, 27.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35008, "", Common::Rect(251, 123, 305, 181), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 9, 343.0f, 14.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToRotation(30602, 30601, "1255", Common::Rect(2536, 303, 3188, 511), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30602, 0, 245.0f, 15.0f, 85.3f, 0, 2, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30602, 30701, "1256", Common::Rect(1423, -125, 1617, 139), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30602, 1, 110.0f, 0, 85.3f, 0, 2, 89.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30701, 30702, "1257", Common::Rect(1269, -69, 1436, 261), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30701, 0, 87.0f, 0, 85.3f, 0, 2, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30701, 30602, "1258", Common::Rect(2990, -174, 3348, 225), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30701, 1, 268.0f, 0, 85.3f, 0, 2, 254.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30702, 30703, "1259", Common::Rect(1451, 144, 1773, 439), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30702, 0, 116.0f, 15.0f, 85.3f, 0, 2, 0, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30702, 30701, "1260", Common::Rect(3091, -71, 3202, 209), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30702, 1, 271.0f, 0, 85.3f, 0, 2, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30702, 30704, "1261", Common::Rect(1265, -92, 1455, 219), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30702, 2, 90.0f, 0, 85.3f, 0, 2, 90.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30703, 30702, "1262", Common::Rect(219, -530, 639, -404), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30703, 0, 284.0f, -15.0f, 85.3f, 0, 2, 284.0f, 0, 85.3f);
	_app->rotationAddMovabilityToPuzzle(30703, kPuzzle35010, "", Common::Rect(105, 266, 857, 456), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30703, 1, 0, 15.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToRotation(30704, 30702, "1263", Common::Rect(3070, -68, 3253, 225), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30704, 0, 273.0f, 0, 85.3f, 0, 2, 270.0f, 0, 85.3f);
	_app->rotationAddMovabilityToPuzzle(30704, kPuzzle35020, "", Common::Rect(1214, -8, 1462, 176), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30704, 1, 89.0f, 7.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToRotation(30801, 30401, "1264", Common::Rect(2847, -75, 2991, 17), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30801, 0, 251.0f, 0, 85.3f, 0, 2, 186.0f, 0, 85.3f);
	_app->rotationAddMovabilityToRotation(30801, 30402, "1265", Common::Rect(3183, -85, 3280, 5), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(30801, 1, 280.0f, 0, 85.3f, 0, 2, 261.0f, 0, 85.3f);
	_app->rotationAddMovabilityToPuzzle(30801, kPuzzle35002, "", Common::Rect(984, -69, 1840, 256), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(30801, 2, 93.0f, 15.0f, 85.3f, 0, 2);
	_app->puzzleAddMovabilityToRotation(kPuzzle35001, 30302, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35001, 0, 170.0f, 15.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35002, 30801, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35002, 0, 93.0f, 15.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35003, 30601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35003, 0, 315.0f, 1.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35004, 30601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35004, 0, 180.0f, 13.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35005, 30601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35005, 0, 17.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35006, 30601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35006, 0, 347.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35007, 30601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35007, 0, 145.0f, 15.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35008, 30601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35008, 0, 343.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35009, 30602, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35009, 0, 175.0f, -23.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35010, 30703, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35010, 0, 0, 15.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35011, 30301, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35011, 0, 150.0f, 3.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35019, 30004, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35019, 0, 70.0f, 0, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle35020, 30704, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35020, 0, 89.0f, 7.0f, 85.3f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle35109, kPuzzle35111, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle35100, 30003, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->objectAdd(kObjectWolfInstinct, "Wolf Vision", "FO_WolfVision", 1);
	_app->objectAddBagAnimation(kObjectWolfInstinct, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectWolfInstinct, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectWolfInstinct, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectIngot, "Panel", "FO_Panel01_", 1);
	_app->objectAddBagAnimation(kObjectIngot, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectIngot, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectIngot, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectIngot2, "Panel", "FO_Panel01_", 1);
	_app->objectAddBagAnimation(kObjectIngot2, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectIngot2, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectIngot2, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectIngot3, "Panel", "FO_Panel01_", 1);
	_app->objectAddBagAnimation(kObjectIngot3, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectIngot3, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectIngot3, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectLogeTear2, "Loge's Tear", "NI_Tear", 1);
	_app->objectAddBagAnimation(kObjectLogeTear2, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectLogeTear2, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectLogeTear2, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObject30001, "", "", 1);
	_app->objectAdd(kObjectWolfBadge, "Medallion", "FO_WolfLogo", 1);
	_app->objectAddBagAnimation(kObjectWolfBadge, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectWolfBadge, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectWolfBadge, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 0, kPuzzle35011, "FOS03N01P01L01.bmp", Common::Point(170, 133), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 1, kPuzzle35011, "FOS03N01P01L02.bmp", Common::Point(266, 178), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 2, kPuzzle35011, "FOS03N01P01L03.bmp", Common::Point(268, 211), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 3, kPuzzle35011, "FOS03N01P01L04.bmp", Common::Point(266, 249), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 4, kPuzzle35011, "FOS03N01P01L05.bmp", Common::Point(293, 332), true, kDrawType1, 1000);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(260, 171, 376, 374), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(273, 182, 361, 202), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(276, 218, 359, 238), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(275, 257, 361, 312), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(292, 333, 345, 388), true, kCursorHotspot, 4);
	_app->objectSetAccessibilityOff(kObject30001, 1, 4);
	_app->objectAdd(kObjectPatience, "Saturn", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectPatience, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectPatience, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectPatience, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMovementAndIntuition, "Mercury", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectMovementAndIntuition, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMovementAndIntuition, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMovementAndIntuition, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectLove, "Venus", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectLove, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectLove, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectLove, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectImagination, "Moon", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectImagination, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectImagination, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectImagination, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectDestruction, "Mars", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectDestruction, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectDestruction, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectDestruction, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectJudgementAndDirection, "Jupiter", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectJudgementAndDirection, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectJudgementAndDirection, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectJudgementAndDirection, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectWill, "Sun", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectWill, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectWill, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectWill, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObject30002, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30002, 30301, Common::Rect(1858, 280, 2021, 366), true, kCursorHotspot, 0);
	_app->objectAdd(kObject30003, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30003, 30301, Common::Rect(2371, 280, 2540, 366), true, kCursorHotspot, 0);
	_app->objectAdd(kObject30004, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30004, 30301, Common::Rect(2884, 280, 3048, 366), true, kCursorHotspot, 0);
	_app->objectAdd(kObject30005, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30005, 30301, Common::Rect(3401, 280, 3561, 366), true, kCursorHotspot, 0);
	_app->objectAdd(kObject30006, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30006, 30301, Common::Rect(311, 280, 481, 366), true, kCursorHotspot, 0);
	_app->objectAdd(kObject30007, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30007, 30301, Common::Rect(827, 280, 998, 366), true, kCursorHotspot, 0);
	_app->objectAdd(kObject30008, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30008, 30301, Common::Rect(1337, 280, 1509, 366), true, kCursorHotspot, 0);
	_app->objectAdd(kObject30027, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30027, kPuzzle35001, Common::Rect(410, 230, 466, 273), true, kCursorMove, 0);
	_app->objectAdd(kObject30016, "", "", 4);
	_app->objectSetPassiveDrawCursor(kObject30016, Common::Point(20, 20), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject30016, Common::Point(20, 20), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 51; i++) {
		_app->objectAddPresentation(kObject30016);
		_app->objectPresentationAddImageToPuzzle(kObject30016, i, kPuzzle35001, Common::String::format("FOS03N02P01S01.%04d.bmp", i + 1), Common::Point(341, 150), true, kDrawType1, 1000);
	}

	_app->objectPresentationShow(kObject30016, 25);
	_app->objectAddPuzzleAccessibility(kObject30016, kPuzzle35001, Common::Rect(349, 164, 532, 332), true, kCursorHandSelection, 0);
	_app->objectAdd(kObjectBerries, "Berries", "FO_Berries", 1);
	_app->objectAddBagAnimation(kObjectBerries, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBerries, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBerries, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectSleepingBerries, "Poison berries", "FO_BerriesPoison", 1);
	_app->objectAddBagAnimation(kObjectSleepingBerries, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectSleepingBerries, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectSleepingBerries, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(232, 322, 346, 436), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(110, 120, 492, 305), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(116, 296, 205, 426), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(348, 251, 636, 414), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(505, 194, 556, 238), true, kCursorHotspot, 4);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 0, kPuzzle35002, "FOS08N01P01L01.BMP", Common::Point(508, 197), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 1, kPuzzle35002, "FOS08N01P01L02.BMP", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 2, kPuzzle35002, "FOS08N01P01L03.BMP", Common::Point(510, 198), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 3, kPuzzle35002, "FOS08N01P01L04.BMP", Common::Point(209, 319), true, kDrawType1, 1000);
	_app->objectAdd(kObjectBerriesJuice, "Berries juice", "FO_BerriesJuice", 1);
	_app->objectAddBagAnimation(kObjectBerriesJuice, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBerriesJuice, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBerriesJuice, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectSleepingPotion2, "Poison berries juice", "FO_BerriesPoisonJuice", 1);
	_app->objectAddBagAnimation(kObjectSleepingPotion2, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectSleepingPotion2, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectSleepingPotion2, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35005, Common::Rect(155, 283, 407, 447), true, kCursorHotspot, 5);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 4, kPuzzle35005, "FOS06N01P05L01.BMP", Common::Point(209, 281), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 5, kPuzzle35005, "FOS06N01P05L02.BMP", Common::Point(243, 402), true, kDrawType1, 1000);
	_app->objectAdd(kObjectSleepingPotion, "", "", 1);
	_app->objectAddPresentation(kObjectSleepingPotion);
	_app->objectPresentationAddAnimationToRotation(kObjectSleepingPotion, 0, 30003, 0, 26, 20.0f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObjectSleepingPotion, 0, 0, 30006);
	_app->objectPresentationShow(kObjectSleepingPotion, 0);
	_app->objectAddRotationAccessibility(kObjectSleepingPotion, 30003, Common::Rect(1931, 76, 2112, 263), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObjectSleepingPotion, 30003, Common::Rect(1476, -50, 1741, 245), true, kCursorMove, 1);
	_app->objectAdd(kObjectHare, "Rabbit hare", "FO_Hares", 1);
	_app->objectAddBagAnimation(kObjectHare, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectHare, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectHare, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectBow, "Hunting bow", "FO_HuntingBow", 1);
	_app->objectAddBagAnimation(kObjectBow, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBow, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBow, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(240, 200, 400, 410), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(230, 219, 278, 275), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(237, 280, 273, 334), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(353, 228, 387, 283), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(384, 226, 415, 281), true, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(328, 286, 366, 452), true, kCursorHotspot, 5);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(277, 174, 326, 447), true, kCursorHotspot, 6);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 0, kPuzzle35003, "FOS06N01P06L01.bmp", Common::Point(72, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 1, kPuzzle35003, "FOS06N01P06L02.bmp", Common::Point(233, 219), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 2, kPuzzle35003, "FOS06N01P06L03.bmp", Common::Point(241, 273), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 3, kPuzzle35003, "FOS06N01P06L04.bmp", Common::Point(357, 221), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 4, kPuzzle35003, "FOS06N01P06L05.bmp", Common::Point(387, 222), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 5, kPuzzle35003, "FOS06N01P06L06.bmp", Common::Point(326, 282), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 6, kPuzzle35003, "FOS06N01P06L07.bmp", Common::Point(274, 170), true, kDrawType1, 1000);
	_app->objectAdd(kObject30028, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30028, kPuzzle35004, Common::Rect(450, 265, 512, 340), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject30028, kPuzzle35004, Common::Rect( 70, 350, 240, 415), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddImageToPuzzle(kObject30028, 0, kPuzzle35004, "FOS06N01P01L01.bmp", Common::Point(0, 339), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddAnimationToPuzzle(kObject30028, 1, kPuzzle35004, "FOS06N01P01LS01", kImageTypeBMP, Common::Point(1, 250), kDrawType1, 1000, 13, 12.5f, 4);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddAnimationToPuzzle(kObject30028, 2, kPuzzle35004, "FOS06N01P01LS02", kImageTypeBMP, Common::Point(1, 242), kDrawType1, 1000, 13, 12.5f, 4);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddImageToRotation(kObject30028, 3, 30601, 0);
	_app->objectAdd(kObject30040, "", "", 1);
	_app->objectAdd(kObjectMetals, "Ignots", "FO_Ignots", 1);
	_app->objectAddBagAnimation(kObjectMetals, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMetals, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMetals, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMould, "Pan", "FO_Pan", 1);
	_app->objectAddBagAnimation(kObjectMould, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMould, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMould, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMeltedGold, "Gold", "FO_IgnotGld", 1);
	_app->objectAddBagAnimation(kObjectMeltedGold, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMeltedGold, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMeltedGold, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMeltedSilver, "Silver", "FO_IgnotSil", 1);
	_app->objectAddBagAnimation(kObjectMeltedSilver, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMeltedSilver, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMeltedSilver, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMeltedCopper, "Copper", "FO_IgnotCop", 1);
	_app->objectAddBagAnimation(kObjectMeltedCopper, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMeltedCopper, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMeltedCopper, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMeltedLead, "Lead", "FO_IgnotLed", 1);
	_app->objectAddBagAnimation(kObjectMeltedLead, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMeltedLead, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMeltedLead, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMeltedSteel, "Steel", "FO_IgnotSte", 1);
	_app->objectAddBagAnimation(kObjectMeltedSteel, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMeltedSteel, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMeltedSteel, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMeltedTin, "Tin", "FO_IgnotTin", 1);
	_app->objectAddBagAnimation(kObjectMeltedTin, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMeltedTin, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMeltedTin, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectQuicksilver, "Mercury", "FO_IgnotMer", 1);
	_app->objectAddBagAnimation(kObjectQuicksilver, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectQuicksilver, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectQuicksilver, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(293, 132, 351, 173), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(285, 207, 354, 245), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(173, 229, 244, 271), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(201, 289, 284, 329), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(214, 135, 364, 189), true, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(222, 210, 353, 277), true, kCursorHotspot, 5);
	_app->objectSetAccessibilityOff(kObject30040, 2, 2);
	_app->objectSetAccessibilityOff(kObject30040, 3, 3);
	_app->objectSetAccessibilityOff(kObject30040, 4, 4);
	_app->objectSetAccessibilityOff(kObject30040, 5, 5);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 0, kPuzzle35006, "FOS06N01P02L02.bmp", Common::Point(1, 44), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 1, kPuzzle35006, "FOS06N01P02L01.bmp", Common::Point(2, 45), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 2, kPuzzle35006, "FOS06N01P02L03.bmp", Common::Point(58, 151), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 3, kPuzzle35006, "FOS06N01P02L04.bmp", Common::Point(54, 132), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddAnimationToPuzzle(kObject30040, 4, kPuzzle35006, "FOS06N01P02LS01", kImageTypeBMP, Common::Point(157, 168), kDrawType1, 1000, 25, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject30040, 4, 0, 30008);
	_app->objectAdd(kObject30042, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(248, 261, 287, 278), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(405,  79, 453, 115), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(337,  47, 379,  77), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(453, 199, 492, 228), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(245,  48, 285,  74), true, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(285,  46, 330,  73), true, kCursorHotspot, 5);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(419, 120, 457, 151), true, kCursorHotspot, 6);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(441, 266, 476, 294), true, kCursorHotspot, 7);
	_app->objectSetAccessibilityOff(kObject30042, 1, 7);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 0, kPuzzle35007, "FOS06N01P03L02.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 1, kPuzzle35007, "FOS06N01P03L03_03.bmp", Common::Point(334, 200), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 2, kPuzzle35007, "FOS06N01P03L03_02.bmp", Common::Point(328, 182), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 3, kPuzzle35007, "FOS06N01P03L03_05.bmp", Common::Point(335, 229), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 4, kPuzzle35007, "FOS06N01P03L03_06.bmp", Common::Point(326, 252), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 5, kPuzzle35007, "FOS06N01P03L03_01.bmp", Common::Point(294, 221), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 6, kPuzzle35007, "FOS06N01P03L03_04.bmp", Common::Point(368, 229), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 7, kPuzzle35007, "FOS06N01P03L03_07.bmp", Common::Point(312, 307), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 8, kPuzzle35007, "FOS06N01P03L01.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAdd(kObjectGolem, "Golem", "FO_Golem", 1);
	_app->objectAddBagAnimation(kObjectGolem, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectGolem, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectGolem, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObject30044, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30044, kPuzzle35008, Common::Rect(253, 206, 333, 300), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject30044, kPuzzle35008, Common::Rect(203, 262, 300, 379), true, kCursorHotspot, 1);
	_app->objectSetAccessibilityOff(kObject30044, 1, 1);
	_app->objectAddPresentation(kObject30044);
	_app->objectPresentationAddImageToPuzzle(kObject30044, 0, kPuzzle35008, "FOS06N01P04L01.bmp", Common::Point(189, 229), true, kDrawType1, 1000);
	_app->objectAdd(kObject30045, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(283, 238, 347, 299), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(273, 315, 306, 352), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(293, 198, 330, 236), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(342, 217, 387, 265), true, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(225, 241, 261, 281), true, kCursorHotspot, 4);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(250, 268, 288, 306), true, kCursorHotspot, 5);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(351, 267, 403, 325), true, kCursorHotspot, 6);
	_app->objectAddRotationAccessibility(kObject30045, 30602, Common::Rect(1856, -381, 2445, -20), true, kCursorHotspot, 7);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 0, kPuzzle35009, "FOS06N02P01L01.bmp", Common::Point(195, 34), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 1, kPuzzle35009, "FOS06N02P01L02.bmp", Common::Point(193, 32), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 2, kPuzzle35009, "FOS06N02P01L03.bmp", Common::Point(148, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 3, kPuzzle35009, "FOS06N02P01L06.bmp", Common::Point(174, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 4, kPuzzle35009, "FOS06N02P01L05.bmp", Common::Point(203, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 5, kPuzzle35009, "FOS06N02P01L04.bmp", Common::Point(171, 22), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 6, kPuzzle35009, "FOS06N02P01L07.bmp", Common::Point(161, 37), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddAnimationToRotation(kObject30045, 7, 30602, 0, 75, 30.0f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject30045, 7, 0, 30000);
	_app->objectPresentationShow(kObject30045, 7);
	_app->objectAdd(kObjectFishingRod, "Fishing pole", "FO_Fishing", 1);
	_app->objectAddBagAnimation(kObjectFishingRod, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFishingRod, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFishingRod, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectFish, "Fish", "FO_Fish", 1);
	_app->objectAddBagAnimation(kObjectFish, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFish, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFish, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectFishingRodWithWorms, "Fishing pole and worms", "FO_FishingWorms", 1);
	_app->objectAddBagAnimation(kObjectFishingRodWithWorms, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFishingRodWithWorms, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFishingRodWithWorms, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectKey, "Key form the fish", "FO_FishKey", 1);
	_app->objectAddBagAnimation(kObjectKey, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectKey, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectKey, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 208, 639, 384), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 208, 639, 384), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0,  63, 639, 197), true, kCursorHotspot, 2);
	_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 2);
	_app->objectAddRotationAccessibility(kObjectFishingRod, 30703, Common::Rect(1126, 141, 3373, 523), true, kCursorHotspot, 3);
	_app->objectAddPresentation(kObjectFishingRod);
	_app->objectPresentationAddImageToPuzzle(kObjectFishingRod, 0, kPuzzle35010, "FOS07N03P01L01.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectFishingRod);
	_app->objectPresentationAddImageToPuzzle(kObjectFishingRod, 1, kPuzzle35010, "FOS07N03P01L02.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAdd(kObjectWorms, "Worms", "FO_Worms", 1);
	_app->objectAddBagAnimation(kObjectWorms, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectWorms, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectWorms, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObjectWorms);
	_app->objectPresentationAddImageToPuzzle(kObjectWorms, 0, kPuzzle35020, "FOS07N04P01L01.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectWorms);
	_app->objectPresentationAddAnimationToPuzzle(kObjectWorms, 1, kPuzzle35020, "FOS07N04P01S01", kImageTypeBMP, Common::Point(312, 209), kDrawType1, 1000, 12, 12.5f, 4);
	_app->objectAddPuzzleAccessibility(kObjectWorms, kPuzzle35020, Common::Rect(285, 196, 391, 330), false, kCursorHotspot, 0);
	_app->objectAdd(kObject30050, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30050, 30501, Common::Rect(1177, 298, 1263, 530), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddImageToRotation(kObject30050, 0, 30501, 0);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddAnimationToRotation(kObject30050, 1, 30501, 1, 13, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject30050, 1, 0, 30009);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddImageToRotation(kObject30050, 2, 30402, 0);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddImageToRotation(kObject30050, 3, 30501, 2);
	_app->objectAdd(kObject30051, "", "", 1);
	_app->objectAdd(kObject30052, "", "", 1);
	_app->objectAdd(kObjectBurningArrow, "Inflamed arrow and bow", "FO_BowFire", 1);
	_app->objectAddBagAnimation(kObjectBurningArrow, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBurningArrow, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBurningArrow, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(411, 0, 511, 57), false, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(958, 102, 1105, 333), true, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(0, 0, 0, 0), true, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(421, 15, 764, 170), false, kCursorHotspot, 3);
	_app->objectAddPresentation(kObject30051);
	_app->objectPresentationAddImageToRotation(kObject30051, 0, 30011, 0);
	_app->objectAddPresentation(kObject30051);
	_app->objectPresentationAddAnimationToRotation(kObject30051, 1, 30011, 1, 22, 20.0f, 4);
	_app->objectAddPresentation(kObject30051);
	_app->objectPresentationAddImageToRotation(kObject30051, 2, 30011, 2);
	_app->objectPresentationShow(kObject30051, 1);
	_app->objectAdd(kObject30100, "", "", 1);
	_app->objectAddPresentation(kObject30100);
	_app->objectPresentationAddImageToPuzzle(kObject30100, 0, kPuzzle35100, "FOS00N03P02L01.bmp", Common::Point(117, 70), true, kDrawType1, 1000);
	_app->objectAddPuzzleAccessibility(kObject30100, kPuzzle35100, Common::Rect(114, 106, 406, 463), true,  kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject30100, kPuzzle35100, Common::Rect(311, 249, 397, 463), false, kCursorHotspot, 1);
	_app->objectAdd(kObjectWolfBrooch, "Sieglinde's Medallion", "FO_WolfLogo", 1);
	_app->objectAddBagAnimation(kObjectWolfBrooch, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectWolfBrooch, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectWolfBrooch, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObject30101, "", "", 1);
	_app->objectAddPresentation(kObject30101);
	_app->objectPresentationAddAnimationToPuzzle(kObject30101, 0, kPuzzle35101, "FOS00N03P02S02", kImageTypeBMP, Common::Point(297, 209), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject30102, "", "", 1);
	_app->objectAdd(kObject30103, "", "", 1);
	_app->objectAddPresentation(kObject30103);
	_app->objectPresentationAddAnimationToPuzzle(kObject30103, 0, kPuzzle35103, "FOS01N01P07", kImageTypeBMP, Common::Point(380, 143), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject30104, "", "", 1);
	_app->objectAddPresentation(kObject30104);
	_app->objectPresentationAddAnimationToPuzzle(kObject30104, 0, kPuzzle35104, "FOS01N01P03", kImageTypeBMP, Common::Point(165, 129), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject30105, "", "", 1);
	_app->objectAddPresentation(kObject30105);
	_app->objectPresentationAddAnimationToPuzzle(kObject30105, 0, kPuzzle35105, "FOS01N01P04", kImageTypeBMP, Common::Point(383, 171), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject30106, "", "", 1);
	_app->objectAddPresentation(kObject30106);
	_app->objectPresentationAddAnimationToPuzzle(kObject30106, 0, kPuzzle35106, "FOS01N01P05", kImageTypeBMP, Common::Point(443, 94), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject30107, "", "", 1);
	_app->objectAddPresentation(kObject30107);
	_app->objectPresentationAddAnimationToPuzzle(kObject30107, 0, kPuzzle35107, "FOS01N01P06", kImageTypeBMP, Common::Point(303, 161), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject30109, "", "", 1);
	_app->objectAddPresentation(kObject30109);
	_app->objectPresentationAddAnimationToPuzzle(kObject30109, 0, kPuzzle35111, "FOS01N01P02S01", kImageTypeBMP, Common::Point(190, 141), kDrawType1, 1000, 202, 12.5f, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObject30109, 0, 0, 30007);
	_app->objectAddPuzzleAccessibility(kObject30109, kPuzzle35111, Common::Rect(  0, 334, 463, 418), false, kCursorHotspot, 0);
	_app->objectAdd(kObject30108, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30108, kPuzzle35109, Common::Rect(181, 165, 256, 259), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject30102, 30101, Common::Rect(0, 0, 0, 0), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject30102, 30101, Common::Rect(1741, 134, 1826, 221), true, kCursorHotspot, 1);
	_app->objectAdd(kObject30110, "", "", 1);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddImageToRotation(kObject30110, 0, 30101, 0);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddAnimationToRotation(kObject30110, 1, 30101, 1, 200, 12.5f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject30110, 1, 0, 30001);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddImageToRotation(kObject30110, 2, 30101, 2);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddImageToRotation(kObject30110, 3, 30101, 3);
	_app->objectAdd(kObjectNotung, "Sword Notung", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectNotung, kPuzzle35019, Common::Rect(290, 167, 363, 232), false, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 0, kPuzzle35019, "FOS00N04P01L01.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 1, kPuzzle35019, "FOS00N04P01L02.bmp", Common::Point(303, 192), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 2, kPuzzle35019, "FOS00N04P01L03.bmp", Common::Point(252, 41), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 3, kPuzzle35019, "FOS00N04P01L04.bmp", Common::Point(284, 193), true, kDrawType1, 1000);
	_app->objectAdd(kObject30059, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30059, 30302, Common::Rect(2183, -268, 2752, 33), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject30059, 30303, Common::Rect(2183, -268, 2752, 33), true, kCursorHotspot, 1);
	_app->objectAdd(kObject30061, "", "", 1);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 0, 30001, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 1, 30010, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 2, 30301, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 3, 30008, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 4, 30009, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 5, 30702, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 6, 30703, 0);
	_app->objectAdd(kObject30200, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(3452, 64, 3505, 109), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(2939, 57, 2995, 99), true, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(873, 68, 949, 120), true, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(2425, 71, 2478, 118), true, kCursorHotspot, 3);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(1390, 62, 1451, 108), true, kCursorHotspot, 4);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(1912, 54, 1959, 118), true, kCursorHotspot, 5);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(367, 62, 425, 111), true, kCursorHotspot, 6);
	_app->soundAdd(30007, kSoundTypeBackgroundMusic, "1267.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30000, kSoundTypeAmbientMusic,    "1269.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30001, kSoundTypeAmbientMusic,    "1270.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30002, kSoundTypeAmbientMusic,    "1271.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30003, kSoundTypeAmbientMusic,    "1272.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30004, kSoundTypeAmbientMusic,    "1273.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30005, kSoundTypeAmbientMusic,    "1274.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30006, kSoundTypeAmbientMusic,    "1275.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundSetVolume(30005, 90);
	_app->soundSetVolume(30002, 95);
	_app->rotationAddAmbientSound(30001, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30002, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30003, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30004, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30005, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30006, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30008, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30009, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30010, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30011, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30012, 30000, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35019, 30000, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35100, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30101, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35101, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35103, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35104, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35105, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35106, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35107, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35108, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35109, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35110, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35111, 30003, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30301, 30004, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30302, 30004, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30303, 30004, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35001, 30004, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35011, 30004, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30401, 30005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30402, 30005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30801, 30005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30401, 30006, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30402, 30006, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30801, 30006, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30011, 30006, 87, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35002, 30006, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30501, 30002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30601, 30002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30602, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35009, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35005, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35003, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35004, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35006, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35007, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35008, 30002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30701, 30001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30702, 30001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30703, 30001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30704, 30001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35010, 30001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35020, 30001, 100, 0, 1, 1, 10);
	_app->soundAdd(30201, kSoundTypeAmbientEffect, "1276.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30202, kSoundTypeAmbientEffect, "1277.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30300, kSoundTypeAmbientEffect, "1278.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30301, kSoundTypeAmbientEffect, "1279.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30302, kSoundTypeAmbientEffect, "1280.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30303, kSoundTypeAmbientEffect, "1281.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30304, kSoundTypeAmbientEffect, "1282.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30305, kSoundTypeAmbientEffect, "1283.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30306, kSoundTypeAmbientEffect, "1284.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30307, kSoundTypeAmbientEffect, "1285.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30308, kSoundTypeAmbientEffect, "1286.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30309, kSoundTypeAmbientEffect, "1287.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30310, kSoundTypeAmbientEffect, "1288.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30311, kSoundTypeAmbientEffect, "1289.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30312, kSoundTypeAmbientEffect, "1290.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30313, kSoundTypeAmbientEffect, "1291.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30314, kSoundTypeAmbientEffect, "1292.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30501, kSoundTypeAmbientEffect, "1293.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30502, kSoundTypeAmbientEffect, "1294.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30315, kSoundTypeAmbientEffect, "1295.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30316, kSoundTypeAmbientEffect, "1296.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30317, kSoundTypeAmbientEffect, "1297.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30318, kSoundTypeAmbientEffect, "1298.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30319, kSoundTypeAmbientEffect, "1299.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30320, kSoundTypeAmbientEffect, "1300.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30321, kSoundTypeAmbientEffect, "1301.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30322, kSoundTypeAmbientEffect, "1302.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30323, kSoundTypeAmbientEffect, "1303.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30324, kSoundTypeAmbientEffect, "1304.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30325, kSoundTypeAmbientEffect, "1305.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30326, kSoundTypeAmbientEffect, "1306.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30503, kSoundTypeAmbientEffect, "1307.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30504, kSoundTypeAmbientEffect, "1308.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30505, kSoundTypeAmbientEffect, "1309.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30506, kSoundTypeAmbientEffect, "1310.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30508, kSoundTypeAmbientEffect, "1311.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30509, kSoundTypeAmbientEffect, "1312.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30510, kSoundTypeAmbientEffect, "1313.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30511, kSoundTypeAmbientEffect, "1314.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30512, kSoundTypeAmbientEffect, "1315.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30513, kSoundTypeAmbientEffect, "1316.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30514, kSoundTypeAmbientEffect, "1317.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30500, kSoundTypeEffect, "1320.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30100, kSoundTypeDialog, "1322.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30101, kSoundTypeDialog, "1323.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30102, kSoundTypeDialog, "1324.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30103, kSoundTypeDialog, "1325.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30104, kSoundTypeDialog, "1326.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30105, kSoundTypeDialog, "1327.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30106, kSoundTypeDialog, "1328.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30107, kSoundTypeDialog, "1329.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30108, kSoundTypeDialog, "1330.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30161, kSoundTypeDialog, "1331.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30109, kSoundTypeDialog, "1332.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30118, kSoundTypeDialog, "1333.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30111, kSoundTypeDialog, "1334.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30112, kSoundTypeDialog, "1335.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30113, kSoundTypeDialog, "1336.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30114, kSoundTypeDialog, "1337.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30115, kSoundTypeDialog, "1338.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30116, kSoundTypeDialog, "1340.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30110, kSoundTypeDialog, "1339.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30117, kSoundTypeDialog, "1341.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30120, kSoundTypeDialog, "1342.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30121, kSoundTypeDialog, "1343.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30122, kSoundTypeDialog, "1344.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30123, kSoundTypeDialog, "1345.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30124, kSoundTypeDialog, "1346.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30125, kSoundTypeDialog, "1347.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30126, kSoundTypeDialog, "1348.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30127, kSoundTypeDialog, "1349.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30128, kSoundTypeDialog, "1350.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30129, kSoundTypeDialog, "1351.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30130, kSoundTypeDialog, "1352.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30131, kSoundTypeDialog, "1353.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30132, kSoundTypeDialog, "1354.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30133, kSoundTypeDialog, "1355.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30134, kSoundTypeDialog, "1356.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30135, kSoundTypeDialog, "1357.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30136, kSoundTypeDialog, "1358.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30150, kSoundTypeDialog, "1359.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30151, kSoundTypeDialog, "1360.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30152, kSoundTypeDialog, "1361.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30153, kSoundTypeDialog, "1362.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30154, kSoundTypeDialog, "1363.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30155, kSoundTypeDialog, "1364.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30156, kSoundTypeDialog, "1365.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30162, kSoundTypeDialog, "1366.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30163, kSoundTypeDialog, "1367.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30164, kSoundTypeDialog, "1368.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30165, kSoundTypeDialog, "1369.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30166, kSoundTypeDialog, "1370.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30167, kSoundTypeDialog, "1371.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30168, kSoundTypeDialog, "1372.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->rotationAdd3DSound(30602, 30202, 1, 1, 15, 100, 174.0f, 10);
	_app->rotationAdd3DSound(30601, 30202, 1, 1, 15, 80, 140.0f, 10);
	_app->rotationAdd3DSound(30001, 30501, 1, 1, 20, 75, 244.0f, 3);
	_app->rotationAdd3DSound(30002, 30501, 1, 1, 20, 75, 347.0f, 6);
	_app->rotationAdd3DSound(30003, 30501, 1, 1, 20, 85, 158.0f, 3);
	_app->rotationAdd3DSound(30004, 30501, 1, 1, 20, 70, 142.0f, 2);
	_app->rotationAdd3DSound(30011, 30513, 1, 1, 40, 90, 56.0f, 10);
	_app->varDefineByte(30009, 2);
	_app->varDefineByte(30010, 1);
	_app->varDefineByte(30011, 3);
	_app->varDefineByte(30012, 4);
	_app->varDefineByte(30013, 5);
	_app->varDefineByte(30014, 6);
	_app->varDefineByte(30015, 7);
	_app->varDefineByte(30016, 25);
	_app->varDefineByte(30017, 0);
	_app->varDefineByte(30050, 0);
	_app->varDefineByte(30051, 0);
	_app->varDefineByte(30052, 0);
	_app->varDefineByte(30053, 0);
	_app->varDefineByte(30054, 0);
	_app->varDefineByte(30055, 0);
	_app->varDefineByte(30019, 0);
	_app->varDefineByte(30020, 0);
	_app->varDefineByte(30021, 0);
	_app->varDefineByte(30022, 0);
	_app->varDefineByte(30023, 0);
	_app->varDefineByte(30024, 0);
	_app->varDefineByte(30025, 0);
	_app->varDefineByte(30026, 0);
	_app->varDefineByte(30027, 0);
	_app->varDefineByte(30028, 0);
	_app->varDefineByte(30029, 0);
	_app->varDefineByte(30030, 0);
	_app->varDefineByte(30031, 0);
	_app->varDefineByte(30032, 0);
	_app->varDefineByte(30033, 0);
	_app->varDefineByte(30038, 0);
	_app->varDefineByte(30034, 0);
	_app->varDefineByte(30035, 0);
	_app->varDefineByte(30036, 0);
	_app->varDefineByte(30037, 0);
	_app->varDefineByte(30039, 0);
	_app->varDefineByte(30040, 0);
	_app->varDefineByte(30041, 0);
	_app->varDefineByte(30042, 0);
	_app->varDefineByte(30043, 0);
	_app->varDefineByte(30044, 0);
	_app->varDefineByte(30045, 0);
	_app->varDefineByte(30047, 0);
	_app->varDefineByte(30056, 1);
	_app->varDefineByte(30060, 0);
	_app->varDefineByte(30061, 0);
	_app->varDefineByte(30062, 0);
	_app->varDefineByte(30063, 0);
	_app->varDefineByte(30064, 0);
	_app->varDefineByte(30065, 0);
	_app->varDefineByte(30070, 0);
	_app->varDefineByte(30071, 0);
	_app->varDefineByte(30072, 0);
	_app->varDefineByte(30066, 0);
	_app->varDefineByte(30067, 0);
	_app->varDefineByte(30068, 0);
	_app->varDefineByte(30069, 1);
	_app->varDefineByte(30073, 0);
	_app->varDefineByte(30074, 0);
	_app->varDefineByte(30075, 0);
	_app->varDefineByte(30076, 0);
	_app->varDefineByte(30077, 0);
	_app->varDefineByte(30078, 0);
	_app->varDefineByte(30201, 0);
	_app->varDefineByte(30203, 0);
	_app->varDefineByte(30204, 0);
	_app->varDefineByte(30205, 0);
	_app->varDefineByte(30206, 0);
	_app->varDefineByte(30207, 0);
	_app->varDefineByte(30208, 0);
	_app->varDefineByte(30209, 0);
	_app->varDefineByte(30210, 0);
	_app->varDefineByte(30211, 0);
	_app->varDefineFloat(30046, -10.0);
	_app->varDefineFloat(30042, 1.0);
}

void ZoneFORing::onSetup(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectLogeTear2);
		_app->timerStart(kTimer1, 3000);
		_app->timerStart(kTimer2, 5000);
		_app->timerStart(kTimer3, 17000);
		_app->timerStart(kTimer4, 10000);
		_app->timerStart(kTimer5, 10);
		_app->bagAdd(kObjectWolfInstinct);
		_app->playMovie("1217");
		_app->playMovie("1218");
		_app->rotationSetAlp(30003, 180.0f);
		_app->rotationSetActive(30003);
		break;

	case kSetupType10:
		_app->onSetupLoadTimers("sie", 90019, 90023, 90027);
		break;

	case kSetupType999:
		_app->bagAdd(kObjectWolfInstinct);
		_app->bagAdd(kObjectKey);
		_app->bagAdd(kObjectIngot);
		_app->bagAdd(kObjectIngot2);
		_app->bagAdd(kObjectIngot3);
		_app->puzzleSetActive(kPuzzle35011);
		break;
	}
}

void ZoneFORing::onSwitch(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90011))
			_app->setupZone((ZoneId)_app->varGetDword(90015), kSetupType10);
		else
			_app->setupZone(kZoneRO, kSetupTypeNone);
	}
}

void ZoneFORing::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		switch (_app->varGetByte(30073)) {
		default:
			break;

		case 0:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 1);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 1:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 2);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 10);
			break;

		case 2:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 3);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 3:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 0);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 1000 * (rnd(10) + 15));
			break;
		}
		break;

	case kTimer1: {
		Id soundId = rnd(15) + 30301;
		_app->soundSetPan(soundId, 10 - rnd(20));
		_app->soundPlay(soundId);

		_app->timerStop(kTimer1);
		_app->timerStart(kTimer1, 500 * (rnd(10) + 10));
		}
		break;

	case kTimer2:
		_app->objectPresentationUnpauseAnimation(kObjectSleepingPotion, 0);
		_app->timerStop(kTimer2);
		break;

	case kTimer3: {
		Id soundId = rnd(3) + 30316;
		_app->soundSetPan(soundId, 95);
		_app->soundPlay(soundId);

		_app->timerStop(kTimer3);
		_app->timerStart(kTimer3, 500 * (rnd(10) + 30));
		}
		break;

	case kTimer4: {
		Id soundId = rnd(9) + 30319;
		_app->soundSetPan(soundId, 5 - rnd(10));
		_app->soundPlay(soundId);

		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, 4000 * (rnd(10) + 5));
		}
		break;

	case kTimer5:
		_app->objectPresentationUnpauseAnimation(kObject30110, 1);
		_app->objectPresentationUnpauseAnimation(kObject30109, 0);
		_app->objectSetAccessibilityOff(kObject30109, 0, 0);
		_app->timerStop(kTimer5);
		_app->timerStart(kTimer5, 2000 * rnd(10) + 15000);
		break;
	}
}

void ZoneFORing::onButtonUp(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject30001:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKey) {
				if (!target) {
					_app->objectPresentationShow(kObject30001, 0);
					_app->objectSetAccessibilityOff(kObject30001, 0, 0);
					_app->objectSetAccessibilityOn(kObject30001, 1, 3);

					if (_app->varGetByte(30040) != 1
					 || _app->varGetByte(30041) != 1
					 || _app->varGetByte(30042) != 1) {
						if (_app->varGetByte(30040) == 1) {
							_app->objectPresentationShow(kObject30001, 1);
							_app->objectSetAccessibilityOff(kObject30001, 1, 1);
						}

						if (_app->varGetByte(30041) == 1) {
							_app->objectPresentationShow(kObject30001, 2);
							_app->objectSetAccessibilityOff(kObject30001, 2, 2);
						}

						if (_app->varGetByte(30042) == 1) {
							_app->objectPresentationShow(kObject30001, 3);
							_app->objectSetAccessibilityOff(kObject30001, 3, 3);
						}
					} else {
						_app->objectPresentationShow(kObject30001, 4);
						_app->objectSetAccessibilityOn(kObject30001, 4, 4);
					}
				}
			}

			if (_app->bagGetClickedObject() != kObjectIngot
			 && _app->bagGetClickedObject() != kObjectIngot2
			 && _app->bagGetClickedObject() != kObjectIngot3) {
				_app->cursorDelete();
				break;
			}

			switch (target) {
			default:
				_app->cursorDelete();
				break;

			case 1:
				_app->varSetByte(30040, 1);
				_app->objectPresentationShow(kObject30001, 1);
				_app->objectSetAccessibilityOff(kObject30001, 1, 1);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;

			case 2:
				_app->varSetByte(30041, 1);
				_app->objectPresentationShow(kObject30001, 2);
				_app->objectSetAccessibilityOff(kObject30001, 2, 2);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;

			case 3:
				_app->varSetByte(30042, 1);
				_app->objectPresentationShow(kObject30001, 3);
				_app->objectSetAccessibilityOff(kObject30001, 3, 3);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;
			}

			_app->playMovie("1167");
			_app->objectPresentationHide(kObject30001, 1);
			_app->objectPresentationHide(kObject30001, 2);
			_app->objectPresentationHide(kObject30001, 3);
			_app->objectPresentationShow(kObject30001, 4);
			_app->objectSetAccessibilityOn(kObject30001, 4, 4);
			_app->objectSetAccessibilityOn(kObjectSleepingPotion, 1, 1);
			_app->cursorDelete();

		} else {
			if (target == 4) {
				_app->bagAdd(kObjectWolfBadge);
				_app->objectPresentationHide(kObject30001, 4);
				_app->objectSetAccessibilityOff(kObject30001, 4, 4);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.0f);
				_app->varSetByte(30043, 1);
			}
		}
		break;

	case kObject30002: // byte: 30009 / sound: 30110
	case kObject30003: // byte: 30010 / sound: 30111
	case kObject30004: // byte: 30011 / sound: 30112
	case kObject30005: // byte: 30012 / sound: 30113
	case kObject30006: // byte: 30013 / sound: 30114
	case kObject30007: // byte: 30014 / sound: 30115
	case kObject30008: // byte: 30015 / sound: 30116
		if (_app->bagHasClickedObject()) {
			switch (_app->bagGetClickedObject()) {
			default:
				break;

			case kObjectPatience:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 1);
					_app->bagRemove(kObjectPatience);
					_app->playMovie("1168");
				}
				break;

			case kObjectMovementAndIntuition:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 2);
					_app->bagRemove(kObjectMovementAndIntuition);
					_app->playMovie("1168");
				}
				break;

			case kObjectLove:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 3);
					_app->bagRemove(kObjectLove);
					_app->playMovie("1168");
				}
				break;

			case kObjectImagination:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 4);
					_app->bagRemove(kObjectImagination);
					_app->playMovie("1168");
				}
				break;

			case kObjectDestruction:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 5);
					_app->bagRemove(kObjectDestruction);
					_app->playMovie("1168");
				}
				break;

			case kObjectJudgementAndDirection:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 6);
					_app->bagRemove(kObjectJudgementAndDirection);
					_app->playMovie("1168");
				}
				break;

			case kObjectWill:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 7);
					_app->bagRemove(kObjectWill);
					_app->playMovie("1168");
				}
				break;
			}

			if (_app->varGetByte(30009) == 1
			 && _app->varGetByte(30010) == 2
			 && _app->varGetByte(30011) == 3
			 && _app->varGetByte(30012) == 4
			 && _app->varGetByte(30013) == 5
			 && _app->varGetByte(30014) == 6
			 && _app->varGetByte(30015) == 7) {
				_app->objectSetAccessibilityOff(kObject30002);
				_app->objectSetAccessibilityOff(kObject30003);
				_app->objectSetAccessibilityOff(kObject30004);
				_app->objectSetAccessibilityOff(kObject30005);
				_app->objectSetAccessibilityOff(kObject30006);
				_app->objectSetAccessibilityOff(kObject30007);
				_app->objectSetAccessibilityOff(kObject30008);
				_app->playMovie("1170");
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
				_app->rotationSetActive(30302);
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);
			}

			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(id + 7)) {
			_app->bagAdd((ObjectId)(_app->varGetByte(id + 7) + 30008));
			_app->playMovie("1171");
			_app->soundPlay(id + 108);
			_app->varSetByte(id + 7, 0);
		}
		break;

	case kObject30027:
		if (_app->bagHasClickedObject())
			break;

		if (!_app->varGetByte(30016)) {
			_app->playMovie("1172");
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);

			for (uint32 i = 0; i < 27; i++)
				_app->soundSetVolume(30300 + i, 90);

			_app->varSetByte(30017, 0);
			_app->varSetByte(30035, 1);
			_app->rotationSetMovabilityOn(30402, 2, 2);
			_app->objectPresentationShow(kObject30050, 0);
			_app->objectPresentationShow(kObject30050, 2);
			_app->rotationSetActive(30501);
			_app->rotationSetAlp(30501, 88.0);
			_app->rotationSetBet(30501, 13.0);
			_app->objectSetAccessibilityOff(kObject30002);
			_app->objectSetAccessibilityOff(kObject30003);
			_app->objectSetAccessibilityOff(kObject30004);
			_app->objectSetAccessibilityOff(kObject30005);
			_app->objectSetAccessibilityOff(kObject30006);
			_app->objectSetAccessibilityOff(kObject30007);
			_app->objectSetAccessibilityOff(kObject30008);
		}
		break;

	case kObjectBerries:
		if (_app->bagHasClickedObject()) {
			if (target == 5) {
				switch (_app->bagGetClickedObject()) {
				default:
					break;

				case kObjectSleepingBerries:
					if (!_app->varGetByte(30060)) {
						_app->bagRemove(kObjectSleepingBerries);
						_app->varSetByte(30063, 1);
						_app->objectPresentationShow(kObjectBerries, 4);
					}
					break;

				case kObjectBerries:
					if (!_app->varGetByte(30063)) {
						_app->bagRemove(kObjectBerries);
						_app->varSetByte(30060, 1);
						_app->objectPresentationShow(kObjectBerries, 4);
					}
					break;
				}
			}

			_app->cursorDelete();
			break;
		}


		switch (target) {
		default:
			break;

		case 0:
		case 4:
			if (_app->varGetByte(30017) == 1) {
				_app->bagAdd(kObjectSleepingBerries);

				if (!_app->varGetByte(30201)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
					_app->varSetByte(30201, 1);
				}

				_app->objectPresentationHide(kObjectBerries, 3);
				_app->varSetByte(30075, 1);
				_app->objectSetAccessibilityOff(kObjectBerries, 0, 0);
				_app->bagAdd(kObjectIngot2);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
				_app->objectPresentationHide(kObjectBerries, 0);
				_app->objectPresentationHide(kObjectBerries, 2);
				_app->varSetByte(30076, 1);
			}
			break;

		case 1:
		case 2:
		case 3:
			_app->bagAdd(kObjectBerries);
			break;

		case 5:
			if (!_app->varGetByte(30060) && !_app->varGetByte(30063))
				_app->playMovie("1173");

			if (_app->varGetByte(30060) != 1 || _app->varGetByte(30061)) {
				if (_app->varGetByte(30060) == 1 && _app->varGetByte(30061) == 1) {
					_app->bagAdd(kObjectBerriesJuice);
					_app->objectPresentationHide(kObjectBerries, 5);
					_app->varSetByte(30060, 0);
					_app->varSetByte(30061, 0);
				}
			} else {
				_app->playMovie("1174");
				_app->varSetByte(30061, 1);
				_app->objectPresentationHide(kObjectBerries, 4);
				_app->objectPresentationShow(kObjectBerries, 5);
			}

			if (_app->varGetByte(30063) != 1 || _app->varGetByte(30064)) {
				if (_app->varGetByte(30063) == 1 && _app->varGetByte(30064) == 1) {
					_app->bagAdd(kObjectSleepingPotion2);

					if (!_app->varGetByte(30203)) {
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
						_app->varSetByte(30203, 1);
					}

					_app->objectPresentationHide(kObjectBerries, 5);
					_app->varSetByte(30063, 0);
					_app->varSetByte(30064, 0);
				}
			} else {
				_app->playMovie("1175");
				_app->varSetByte(30064, 1);
				_app->objectPresentationHide(kObjectBerries, 4);
				_app->objectPresentationShow(kObjectBerries, 5);
			}
			break;
		}
		break;

	case kObjectSleepingPotion:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectHare && target == 0) {
				_app->bagRemove(kObjectHare);
				_app->objectSetAccessibilityOff(kObjectSleepingPotion, 0, 0);
				_app->playMovie("1176");
				_app->timerStop(kTimer2);
				_app->soundStop(30501, 1024);

				if (!_app->varGetByte(30077)) {
					_app->playMovie("1177");
					_app->bagAdd(kObjectIngot3);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
					_app->varSetByte(30077, 1);
				}
			}

			_app->cursorDelete();

			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->playMovie("1178");
			break;

		case 1:
			if (_app->varGetByte(30074)) {
				_app->playMovie("1179");
				_app->rotationSetActive(30101);
				_app->rotationSetAlp(30101, 130.0);
				_app->rotationSetBet(30101, 20.0);
			} else {
				if (_app->varGetByte(30056))
					_app->puzzleSetActive(kPuzzle35100);
				else
					_app->playMovie("1178");
			}
			break;
		}
		break;

	case kObjectBow:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(30511);
			_app->objectPresentationShow(kObjectBow);
			_app->objectSetAccessibilityOff(kObjectBow, 0, 0);
			if (_app->varGetByte(30050) == 1)
				_app->objectPresentationHide(kObjectBow, 1);
			if (_app->varGetByte(30051) == 1)
				_app->objectPresentationHide(kObjectBow, 2);
			if (_app->varGetByte(30052) == 1)
				_app->objectPresentationHide(kObjectBow, 3);
			if (_app->varGetByte(30053) == 1)
				_app->objectPresentationHide(kObjectBow, 4);
			if (_app->varGetByte(30054) == 1)
				_app->objectPresentationHide(kObjectBow, 5);
			if (_app->varGetByte(30055) == 1)
				_app->objectPresentationHide(kObjectBow, 6);
			break;

		case 1:
		case 2:
		case 3:
		case 4:
			_app->soundPlay(30512);
			_app->objectPresentationHide(kObjectBow, target);
			_app->objectSetAccessibilityOff(kObjectBow, target, target);
			_app->varSetByte(target + 30049, 1);

			_app->bagAdd(kObjectHare);
			if (!_app->varGetByte(30204)) {
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				_app->varSetByte(30204, 1);
			}
			break;

		case 5:
		case 6:
			_app->soundPlay(30512);
			_app->objectPresentationHide(kObjectBow, target);
			_app->objectSetAccessibilityOff(kObjectBow, target, target);
			_app->varSetByte(target + 30049, 1);

			_app->bagAdd(kObjectBow);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
			_app->objectPresentationHide(kObjectBow, 5);
			_app->objectPresentationHide(kObjectBow, 6);
			_app->objectSetAccessibilityOff(kObjectBow, 5, 6);
			_app->varSetByte(30054, 1);
			_app->varSetByte(30055, 1);
			break;
		}
		break;

	case kObject30028:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				switch (_app->bagGetClickedObject()) {
				default:
					break;

				case kObjectLogeTear2:
					if (_app->varGetByte(30019)) {
						_app->playMovie("1181");
						_app->bagRemove(kObjectLogeTear2);
						_app->varSetByte(30021, 1);
						if (!_app->varGetByte(30020)) {
							_app->objectPresentationHide(kObject30028, 0);
							_app->objectPresentationShow(kObject30028, 1);
						}
					} else {
						_app->playMovie("1180");
					}
					break;

				case kObjectMould:
					if (_app->varGetByte(30019) == 1) {
						if (_app->varGetByte(30021) == 1) {
							_app->objectPresentationHide(kObject30028, 0);
							_app->objectPresentationShow(kObject30028, 2);
							_app->varSetByte(30020, 1);
							_app->bagRemove(kObjectMould);
						}
					}
					break;

				case kObjectMetals:
					if (_app->varGetByte(30020) == 1) {
						_app->bagRemove(kObjectMetals);
						_app->bagAdd(kObjectMeltedGold);
						_app->bagAdd(kObjectMeltedSilver);
						_app->bagAdd(kObjectMeltedCopper);
						_app->bagAdd(kObjectMeltedLead);
						_app->bagAdd(kObjectMeltedSteel);
						_app->bagAdd(kObjectMeltedTin);
						_app->bagAdd(kObjectQuicksilver);
						_app->bagAdd(kObjectLogeTear2);

						if (!_app->varGetByte(30206)) {
							_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
							_app->varSetByte(30206, 1);
						}

						_app->bagAdd(kObjectMould);
						_app->playMovie("1182");
						_app->objectPresentationHide(kObject30028);
						_app->objectPresentationHide(kObject30028, 3);
						_app->varSetByte(30019, 0);
						_app->varSetByte(30020, 0);
						_app->varSetByte(30021, 0);
						_app->objectSetAccessibilityOn(kObject30028, 0, 0);
					}
					break;
				}
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (!_app->varGetByte(30020)) {
				_app->playMovie("1183");
				_app->objectPresentationShow(kObject30028, 0);
				_app->objectPresentationShow(kObject30028, 3);
				_app->varSetByte(30019, 1);
				_app->objectSetAccessibilityOff(kObject30028, 0, 0);
			}
			break;

		case 1:
			if (_app->varGetByte(30021) == 1) {
				if (!_app->varGetByte(30020)) {
					_app->playMovie("1184");
					_app->objectPresentationHide(kObject30028);
					_app->objectPresentationShow(kObject30028, 0);
					_app->varSetByte(30021, 0);
					_app->bagAdd(kObjectLogeTear2);
				}
			}

			if (_app->varGetByte(30021) == 1) {
				if (_app->varGetByte(30020) == 1) {
					_app->objectPresentationHide(kObject30028, 2);
					_app->objectPresentationShow(kObject30028, 1);
					_app->varSetByte(30020, 0);
					_app->bagAdd(kObjectMould);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				}
			}
			break;
		}
		break;

	case kObject30040:
		if (_app->bagHasClickedObject())
			break;

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(30022))
				break;

			if (_app->varGetByte(30024)) {
				_app->objectPresentationShow(kObject30040, 0);
			} else {
				_app->objectPresentationShow(kObject30040, 1);
				_app->objectSetAccessibilityOn(kObject30040, 4, 4);
			}

			_app->varSetByte(30022, 1);
			_app->objectSetAccessibilityOff(kObject30040, 0, 0);
			_app->objectSetAccessibilityOff(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 2, 2);
			_app->soundPlay(30508);
			break;

		case 1:
			if (_app->varGetByte(30023))
				break;

			_app->objectPresentationShow(kObject30040, 2);
			_app->varSetByte(30023, 1);
			_app->objectSetAccessibilityOff(kObject30040, 0, 0);
			_app->objectSetAccessibilityOff(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 3, 3);
			_app->objectSetAccessibilityOn(kObject30040, 5, 5);
			_app->soundPlay(30508);
			break;

		case 2:
			if (_app->varGetByte(30022) != 1)
				break;

			_app->objectPresentationHide(kObject30040, 0);
			_app->objectPresentationHide(kObject30040, 1);
			_app->varSetByte(30022, 0);
			_app->objectSetAccessibilityOff(kObject30040, 2, 2);
			_app->objectSetAccessibilityOff(kObject30040, 4, 4);
			_app->objectSetAccessibilityOn(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 0, 0);
			_app->soundPlay(30508);
			break;

		case 3:
			if (_app->varGetByte(30023) != 1)
				break;

			_app->objectPresentationHide(kObject30040, 2);
			_app->varSetByte(30023, 0);
			_app->objectSetAccessibilityOff(kObject30040, 3, 3);
			_app->objectSetAccessibilityOff(kObject30040, 5, 5);
			_app->objectSetAccessibilityOn(kObject30040, 0, 0);
			_app->objectSetAccessibilityOn(kObject30040, 1, 1);
			_app->soundPlay(30508);
			break;

		case 4:
			_app->bagAdd(kObjectMould);
			_app->objectPresentationShow(kObject30040, 0);
			_app->objectPresentationHide(kObject30040, 1);
			_app->objectSetAccessibilityOff(kObject30040, 4, 4);
			_app->varSetByte(30024, 1);
			break;

		case 5:
			_app->bagAdd(kObjectMetals);

			if (!_app->varGetByte(30205)) {
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				_app->varSetByte(30205, 1);
			}

			_app->objectSetAccessibilityOff(kObject30040, 5, 5);
			_app->objectSetAccessibilityOff(kObject30040, 3, 3);
			_app->objectPresentationShow(kObject30040, 4);
			break;
		}
		break;

	case kObject30042:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				if (_app->varGetByte(30019)) {
					_app->objectPresentationShow(kObject30042, 0);
					_app->objectPresentationHide(kObject30042, 8);
					_app->objectPresentationHide(kObject30028, 0);
					_app->objectPresentationHide(kObject30028, 3);
					_app->varSetByte(30019, 0);
					_app->objectSetAccessibilityOn(kObject30042, 1, 7);
					_app->objectSetAccessibilityOff(kObject30042, 0, 0);
					_app->objectSetAccessibilityOn(kObject30028, 0, 0);
					_app->playMovie("1187");
				} else {
					_app->objectPresentationShow(kObject30042, 8);
					_app->objectPresentationHide(kObject30042, 0);
					_app->objectSetAccessibilityOff(kObject30042, 0, 0);
					_app->soundPlay(30508);
				}
			}
			break;
		}

		if (target >= 1 && target <= 7) {
			_app->soundPlay(30509);

			switch (_app->bagGetClickedObject()) {
			default:
				break;

			case kObjectMeltedGold:
				_app->varSetByte(30025, (byte)target);
				_app->bagRemove(kObjectMeltedGold);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case  kObjectMeltedSilver:
				_app->varSetByte(30026, (byte)target);
				_app->bagRemove(kObjectMeltedSilver);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedCopper:
				_app->varSetByte(30027, (byte)target);
				_app->bagRemove(kObjectMeltedCopper);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedLead:
				_app->varSetByte(30028, (byte)target);
				_app->bagRemove(kObjectMeltedLead);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedSteel:
				_app->varSetByte(30029, (byte)target);
				_app->bagRemove(kObjectMeltedSteel);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedTin:
				_app->varSetByte(30030, (byte)target);
				_app->bagRemove(kObjectMeltedTin);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectQuicksilver:
				_app->varSetByte(30031, (byte)target);
				_app->bagRemove(kObjectQuicksilver);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;
			}
		}

		if (_app->varGetByte(30025) != 1
		 || _app->varGetByte(30026) != 2
		 || _app->varGetByte(30027) != 3
		 || _app->varGetByte(30028) != 4
		 || _app->varGetByte(30029) != 5
		 || _app->varGetByte(30030) != 6
		 || _app->varGetByte(30031) != 7) {
			if (_app->varGetByte(30025)
			 && _app->varGetByte(30026)
			 && _app->varGetByte(30027)
			 && _app->varGetByte(30028)
			 && _app->varGetByte(30029)
			 && _app->varGetByte(30030)
			 && _app->varGetByte(30031)) {
				_app->objectPresentationHide(kObject30042);
				_app->playMovie("1186");
				_app->rotationSetActive(30601);
				_app->objectSetAccessibilityOn(kObject30042, 0, 0);
				_app->varSetByte(30025, 0);
				_app->varSetByte(30026, 0);
				_app->varSetByte(30027, 0);
				_app->varSetByte(30028, 0);
				_app->varSetByte(30029, 0);
				_app->varSetByte(30030, 0);
				_app->varSetByte(30031, 0);
			}
		} else {
			_app->objectPresentationHide(kObject30042);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 9.9f);
			_app->varSetByte(30032, 1);
			_app->playMovie("1185");
			_app->bagAdd(kObjectGolem);
			_app->rotationSetActive(30601);
			_app->objectSetAccessibilityOff(kObject30042);
		}

		_app->cursorDelete();
		break;

	case kObject30044:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectGolem) {
			switch (target) {
			default:
				break;

			case 0:
				_app->objectPresentationShow(kObject30044);
				_app->objectSetAccessibilityOff(kObject30044, 0, 0);
				_app->objectSetAccessibilityOn(kObject30044, 1, 1);
				_app->bagRemove(kObjectGolem);
				_app->playMovie("1188");
				break;

			case 1:
				_app->objectPresentationShow(kObject30044);
				break;
			}
		}

		if (_app->bagGetClickedObject() == kObjectSleepingPotion2) {
			if (target == 1) {
				_app->objectPresentationHide(kObject30044);
				_app->objectSetAccessibilityOff(kObject30044, 1, 1);
				_app->bagAdd(kObjectGolem);
				_app->bagAdd(kObjectIngot);
				_app->playMovie("1189");
				_app->rotationSetMovabilityOff(30601, 9, 9);
				_app->objectSetAccessibilityOff(kObject30044);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
			}
		}

		_app->cursorDelete();
		break;

	case kObject30045:
		if (_app->bagHasClickedObject())
			break;

		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 0);
			_app->soundPlay(30150);
			break;

		case 1:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 1);
			_app->soundPlay(30151);
			break;

		case 2:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 2);
			_app->soundPlay(30154);
			break;

		case 3:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 3);
			_app->soundPlay(30152);
			break;

		case 4:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 4);
			_app->soundPlay(30155);
			break;

		case 5:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 5);
			_app->soundPlay(30156);
			break;

		case 6:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 6);
			_app->soundPlay(30153);
			break;

		case 7:
			_app->soundPlay(30201);
			_app->varSetByte(30047, 1);
			_app->rotationSetRolTo(30602, 175.0f, -23.0f, 85.3f);
			break;
		}
		break;

	case kObjectFishingRod:
		if (_app->bagHasClickedObject()) {
			if (target == 3) {
				if (_app->bagGetClickedObject() == kObjectFishingRodWithWorms) {
					if (_app->varGetByte(30034)) {
						_app->bagAdd(kObjectKey);
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
						_app->playMovie("1191");
					} else {
						_app->bagAdd(kObjectFish);

						if (!_app->varGetByte(30207)) {
							_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);
							_app->varSetByte(30207, 1);
						}

						_app->playMovie("1190");
					}

					_app->bagAdd(kObjectFishingRod);
					_app->bagRemove(kObjectFishingRodWithWorms);
					_app->varSetByte(30038, 0);
				}
				if (_app->bagGetClickedObject() == kObjectFishingRod)
					_app->playMovie("1192");
			}

			_app->cursorDelete();

			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(30508);

			if (_app->varGetByte(30033)) {
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 2, 2);
			} else {
				_app->objectPresentationHide(kObjectFishingRod, 0);
				_app->objectPresentationShow(kObjectFishingRod, 1);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 1, 2);
			}

			_app->varSetByte(30070, 1);
			break;

		case 1:
			if (_app->varGetByte(30038)) {
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->bagAdd(kObjectFishingRodWithWorms);

				if (!_app->varGetByte(30208)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
					_app->varSetByte(30208, 1);
				}

				_app->bagRemove(kObjectWorms);
				_app->varSetByte(30033, 1);
			} else {
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->bagAdd(kObjectFishingRod);
				_app->varSetByte(30033, 1);
			}
			break;

		case 2:
			_app->soundPlay(30511);

			if (_app->varGetByte(30070)) {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 2);
				_app->varSetByte(30070, 0);
			} else {
				if (_app->varGetByte(30033)) {
					_app->objectPresentationShow(kObjectFishingRod, 0);
					_app->varSetByte(30070, 0);
				} else {
					_app->objectPresentationShow(kObjectFishingRod, 1);
					_app->varSetByte(30070, 0);
				}
			}
			break;
		}
		break;

	case kObjectWorms:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != kObjectGolem || _app->varGetByte(30038)) {
				_app->cursorDelete();
				break;
			}

			if (_app->varGetByte(30033)) {
				_app->bagAdd(kObjectFishingRodWithWorms);

				if (!_app->varGetByte(30208)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
					_app->varSetByte(30208, 1);
				}

				_app->playMovie("1193");
				_app->bagRemove(kObjectFishingRod);
				_app->varSetByte(30038, 1);

				if (_app->varGetByte(30076)) {
					_app->cursorDelete();
					break;
				}
			} else {
				_app->bagAdd(kObjectWorms);

				if (!_app->varGetByte(30209)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);
					_app->varSetByte(30209, 1);
				}

				_app->playMovie("1193");
				_app->varSetByte(30038, 1);

				if (_app->varGetByte(30076)) {
					_app->cursorDelete();
					break;
				}
			}

			_app->bagAdd(kObjectIngot2);
			_app->bagAdd(kObjectSleepingBerries);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
			_app->varSetByte(30076, 1);
			_app->varSetByte(30039, 1);
			_app->playMovie("1194");
			_app->objectPresentationHide(kObjectWorms, 0);
			_app->objectPresentationHide(kObjectWorms, 1);
			_app->objectPresentationHide(kObjectBerries, 0);
			_app->objectSetAccessibilityOff(kObjectWorms);
			_app->cursorDelete();
		}
		break;

	case kObject30050:
		if (!_app->bagHasClickedObject()) {
			if (!_app->varGetByte(30035)) {
				_app->objectPresentationShow(kObject30050, 1);
				_app->soundPlay(30514);
			}

			if (_app->varGetByte(30035) == 1) {
				if (_app->varGetByte(30037) == 1) {
					_app->playMovie("1195");
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
					_app->objectPresentationShow(kObject30051, 2);
					_app->objectPresentationHide(kObject30050);
					_app->varSetByte(30035, 0);
					_app->varSetByte(30036, 1);
					_app->varSetByte(30037, 0);
					_app->rotationSetMovabilityOff(30402, 2, 2);
					_app->rotationSetMovabilityOff(30011, 1, 1);
				}
			}
		}
		break;

	case kObject30051:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				_app->cursorDelete();
				break;

			case 0:
				if (_app->bagGetClickedObject() == kObjectBurningArrow) {
					_app->bagRemove(kObjectBurningArrow);
					_app->bagAdd(kObjectBow);
					_app->varSetByte(30034, 1);
					_app->playMovie("1196", target);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
					_app->objectPresentationHide(kObject30051, 2);
					_app->objectSetAccessibilityOff(kObject30051, target, target);
					_app->objectSetAccessibilityOff(kObject30051, 3, 3);
				}

				if (_app->bagGetClickedObject() != kObjectBow) {
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1197");
				_app->cursorDelete();
				break;

			case 1:
			case 2:
				if (_app->bagGetClickedObject() == kObjectBow) {
					_app->bagAdd(kObjectBurningArrow);

					if (!_app->varGetByte(30210)) {
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
						_app->varSetByte(30210, 1);
					}

					_app->bagRemove(kObjectBow);
				}
				break;

			case 3:
				if (_app->bagGetClickedObject() == kObjectBurningArrow) {
					_app->playMovie("1198");
					_app->bagRemove(kObjectBurningArrow);
					_app->bagAdd(kObjectBow);
				}

				if (_app->bagGetClickedObject() != kObjectBow) {
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1197");
				_app->cursorDelete();
				break;
			}
		}
		break;

	case kObject30102:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				if (_app->bagGetClickedObject() == kObjectWolfBadge) {
					if (_app->varGetByte(30072) == 1) {
						_app->puzzleSetActive(kPuzzle35104);
						_app->soundPlay(30105);
					}
				}
			}

			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			if (_app->varGetByte(30072)) {
				if (!_app->varGetByte(30078)) {
					_app->puzzleSetActive(kPuzzle35103);
					_app->soundPlay(30118);
				}
			} else {
				_app->rotationSetRolTo(30101, 130.0f, 20.0f, 85.3f);
				_app->puzzleSetActive(kPuzzle35104);
				_app->soundPlay(30102);
			}
		}
		break;

	case kObject30109:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->puzzleSetActive(kPuzzle35109);
		break;

	case kObject30108:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != kObjectSleepingPotion2) {
				_app->cursorDelete();
				break;
			}

			_app->playMovie("1199");
			_app->objectPresentationHide(kObject30110, 1);
			_app->objectPresentationShow(kObject30110, 2);
			_app->rotationSetActive(30101);
			_app->rotationSetAlp(30101, 130.0f);
			_app->rotationSetBet(30101, 20.0f);
			_app->objectSetAccessibilityOff(kObject30102, 0, 0);
			_app->varSetByte(30072, 1);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
			_app->cursorDelete();
		}
		break;

	case kObjectNotung:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectWolfBadge
			 || _app->bagGetClickedObject() == kObjectWolfBrooch) {
				if (_app->bagGetClickedObject() == kObjectWolfBrooch) {
					_app->bagRemove(kObjectWolfBrooch);
					_app->varSetByte(30067, 1);
				} else {
					_app->bagRemove(kObjectWolfBadge);
					_app->varSetByte(30066, 1);
				}

				if (_app->varGetByte(30066) == 1
				 && _app->varGetByte(30067) == 1) {
					_app->objectPresentationShow(kObjectNotung, 3);
					_app->playMovie("1200");
					_app->varSetFloat(90007, 100.0f);
					_app->rotationSetAlp(30001, 180.0);
					_app->rotationSetActive(30001);
					_app->soundSetMultiplier(kSoundTypeAmbientMusic, 0);
					_app->timerStopAll();
					_app->soundPlay(30007);
					_app->objectSetAccessibilityOff(kObject30001);
					_app->objectSetAccessibilityOff(kObject30002);
					_app->objectSetAccessibilityOff(kObject30003);
					_app->objectSetAccessibilityOff(kObject30004);
					_app->objectSetAccessibilityOff(kObject30005);
					_app->objectSetAccessibilityOff(kObject30006);
					_app->objectSetAccessibilityOff(kObject30007);
					_app->objectSetAccessibilityOff(kObject30008);
					_app->objectSetAccessibilityOff(kObjectPatience);
					_app->objectSetAccessibilityOff(kObject30016);
					_app->objectSetAccessibilityOff(kObject30027);
					_app->objectSetAccessibilityOff(kObjectBerries);
					_app->objectSetAccessibilityOff(kObjectSleepingBerries);
					_app->objectSetAccessibilityOff(kObjectWolfInstinct);
					_app->objectSetAccessibilityOff(kObjectHare);
					_app->objectSetAccessibilityOff(kObjectIngot);
					_app->objectSetAccessibilityOff(kObjectIngot2);
					_app->objectSetAccessibilityOff(kObjectIngot3);
					_app->objectSetAccessibilityOff(kObjectSleepingPotion);
					_app->objectSetAccessibilityOff(kObjectBow);
					_app->objectSetAccessibilityOff(kObject30028);
					_app->objectSetAccessibilityOff(kObjectMould);
					_app->objectSetAccessibilityOff(kObjectLogeTear2);
					_app->objectSetAccessibilityOff(kObjectBerriesJuice);
					_app->objectSetAccessibilityOff(kObjectSleepingPotion2);
					_app->objectSetAccessibilityOff(kObjectMeltedGold);
					_app->objectSetAccessibilityOff(kObjectMeltedSilver);
					_app->objectSetAccessibilityOff(kObjectMeltedCopper);
					_app->objectSetAccessibilityOff(kObjectMeltedLead);
					_app->objectSetAccessibilityOff(kObjectMeltedSteel);
					_app->objectSetAccessibilityOff(kObjectMeltedTin);
					_app->objectSetAccessibilityOff(kObjectQuicksilver);
					_app->objectSetAccessibilityOff(kObject30040);
					_app->objectSetAccessibilityOff(kObjectMetals);
					_app->objectSetAccessibilityOff(kObject30042);
					_app->objectSetAccessibilityOff(kObjectGolem);
					_app->objectSetAccessibilityOff(kObject30044);
					_app->objectSetAccessibilityOff(kObject30045);
					_app->objectSetAccessibilityOff(kObjectFishingRod);
					_app->objectSetAccessibilityOff(kObjectFishingRodWithWorms);
					_app->objectSetAccessibilityOff(kObjectFish);
					_app->objectSetAccessibilityOff(kObjectWorms);
					_app->objectSetAccessibilityOff(kObject30050);
					_app->objectSetAccessibilityOff(kObject30051);
					_app->objectSetAccessibilityOff(kObject30052);
					_app->objectSetAccessibilityOff(kObjectBurningArrow);
					_app->objectSetAccessibilityOff(kObjectKey);
					_app->objectSetAccessibilityOff(kObjectWolfBadge);
					_app->objectSetAccessibilityOff(kObjectWolfBrooch);
					_app->objectSetAccessibilityOff(kObject30110);
					_app->objectSetAccessibilityOff(kObjectNotung);
					_app->objectSetAccessibilityOff(kObject30059);
					_app->rotationSetMovabilityOff(30001);
					_app->rotationSetMovabilityOff(30002);
					_app->rotationSetMovabilityOff(30003);
					_app->rotationSetMovabilityOff(30004);
					_app->rotationSetMovabilityOff(30005);
					_app->rotationSetMovabilityOff(30006);
					_app->rotationSetMovabilityOff(30008);
					_app->rotationSetMovabilityOff(30009);
					_app->rotationSetMovabilityOff(30010);
					_app->rotationSetMovabilityOff(30011);
					_app->rotationSetMovabilityOff(30012);
					_app->rotationSetMovabilityOff(30101);
					_app->rotationSetMovabilityOff(30301);
					_app->rotationSetMovabilityOff(30302);
					_app->rotationSetMovabilityOff(30303);
					_app->rotationSetMovabilityOff(30401);
					_app->rotationSetMovabilityOff(30402);
					_app->rotationSetMovabilityOff(30501);
					_app->rotationSetMovabilityOff(30601);
					_app->rotationSetMovabilityOff(30602);
					_app->rotationSetMovabilityOff(30701);
					_app->rotationSetMovabilityOff(30702);
					_app->rotationSetMovabilityOff(30703);
					_app->rotationSetMovabilityOff(30704);
					_app->rotationSetMovabilityOff(30801);
					_app->objectPresentationShow(kObject6, 0);
					_app->objectPresentationShow(kObject6, 5);
					_app->soundPlay(30120);
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1201");
				_app->objectPresentationShow(kObjectNotung, 2);
			}
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(30066) == 1
		 || _app->varGetByte(30067) == 1) {
			_app->playMovie("1202");
			_app->objectPresentationHide(kObjectNotung, 2);

			if (_app->varGetByte(30066) == 1) {
				_app->bagAdd(kObjectWolfBadge);
				_app->varSetByte(30066, 0);
			} else {
				_app->bagAdd(kObjectWolfBrooch);
				_app->varSetByte(30067, 0);
			}
		}
		break;

	case kObject30059:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->playMovie(target ? "1204" : "1203");

		if (!_app->varGetByte(30211)) {
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
			_app->varSetByte(30211, 1);
		}
		break;

	case kObject30100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundSetVolume(30506, rnd(10) + 91);
			_app->puzzleSetMovabilityOff(kPuzzle35100, 0, 0);
			_app->soundPlay(30506);
			_app->objectSetAccessibilityOff(kObject30100, 0, 0);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject30100, 1, 1);
			_app->puzzleSetActive(kPuzzle35101);
			_app->soundPlay(30100);
			break;
		}
		break;

	case kObject30200:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->soundPlay(target + 30162);
		break;
	}
}

void ZoneFORing::onAnimationNextFrame(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId) {
	default:
		break;

	case 30001:
		switch (frame) {
		default:
			break;

		case 10:
			_app->soundPlay(30505);
			_app->soundSetVolume(30505, 90);
			break;

		case 90:
			_app->soundPlay(30503);
			_app->soundSetVolume(30503, 90);
			break;

		case 125:
			_app->soundPlay(30504);
			_app->soundSetVolume(30504, 95);
			break;

		case 202:
			_app->objectPresentationPauseAnimation(kObject30110, 1);
			break;
		}
		break;

	case 30007:
		switch (frame) {
		default:
			break;

		case 10:
			_app->soundPlay(30505);
			_app->soundSetVolume(30505, 95);
			break;

		case 90:
			_app->soundPlay(30503);
			_app->soundSetVolume(30503, 95);
			break;

		case 125:
			_app->soundPlay(30504);
			_app->soundSetVolume(30504, 100);
			break;

		case 202:
			_app->objectPresentationPauseAnimation(kObject30109, 0);
			_app->objectSetAccessibilityOn(kObject30109, 0, 0);
			break;
		}
		break;

	case 30000:
		if (_app->varGetByte(30047) == 1 && frame == 28) {
			_app->playMovie("1207");
			_app->varSetByte(30047, 0);
			_app->puzzleSetActive(kPuzzle35009);
		}
		break;

	case 30002:
		if (frame == 19)
			_app->objectPresentationShow(kObject30110, 6);
		break;

	case 30004:
		if (frame == 19)
			_app->objectPresentationShow(kObject30110, 9);
		break;

	case 30003:
		if (frame == 19)
			_app->objectPresentationHide(kObject30110, 6);
		break;

	case 30005:
		if (frame == 19)
			_app->objectPresentationHide(kObject30110, 9);
		break;

	case 30006:
		switch (frame){
		default:
			break;

		case 10:
			_app->soundPlay(30502);
			break;

		case 26:
			_app->objectPresentationPauseAnimation(kObjectSleepingPotion, 0);
			_app->timerStop(kTimer2);
			_app->timerStart(kTimer2, 1000 * (rnd(10) + 5));
			break;
		}
		break;

	case 30008:
		if (frame == 25) {
			_app->objectSetAccessibilityOn(kObject30040, 5, 5);
			_app->objectSetAccessibilityOn(kObject30040, 3, 3);
		}
		break;
	}
}

void ZoneFORing::onSound(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 30100:
		_app->soundPlay(30101);
		break;

	case 30101:
		_app->playMovie("1179");
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);

		if (!_app->varGetByte(30074))
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);

		_app->varSetByte(30074, 1);
		break;

	case 30102:
		_app->puzzleSetActive(kPuzzle35103);
		_app->soundPlay(30103);
		break;

	case 30103:
		_app->puzzleSetActive(kPuzzle35104);
		_app->soundPlay(30104);
		break;

	case 30104:
		_app->playMovie("1208");
		_app->puzzleSetActive(kPuzzle35105);
		_app->soundPlay(30106);
		break;

	case 30106:
		_app->playMovie("1209");
		_app->puzzleSetActive(kPuzzle35110);
		_app->soundPlay(30107);
		break;

	case 30107:
		_app->playMovie("1210");
		_app->puzzleSetActive(kPuzzle35106);
		_app->soundPlay(30108);
		break;

	case 30108:
		_app->playMovie("1211");
		_app->puzzleSetActive(kPuzzle35107);
		_app->soundPlay(30161);
		break;

	case 30109:
		_app->playMovie("1213");
		_app->puzzleSetActive(kPuzzle35111);
		_app->objectPresentationShow(kObject30109, 0);
		break;

	case 30118:
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);
		_app->varSetByte(30078, 1);
		break;

	case 30105:
		_app->playMovie("1214");
		_app->bagAdd(kObjectWolfBrooch);
		_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
		_app->objectPresentationShow(kObject30110, 3);
		_app->objectSetAccessibilityOff(kObject30102, 1, 1);
		_app->rotationSetActive(30101);
		_app->rotationSetAlp(30101, 130.0);
		_app->rotationSetBet(30101, 20.0);
		break;

	case 30120:
		_app->objectPresentationHideAndRemove(kObject6, 0);
		_app->rotationSetActive(30002);
		_app->soundPlay(30121);
		_app->objectPresentationShow(kObject6, 1);
		break;

	case 30121:
		_app->objectPresentationHideAndRemove(kObject6, 1);
		_app->objectPresentationHideAndRemove(kObject6, 5);
		_app->rotationSetActive(30003);
		_app->soundPlay(30122);
		_app->objectPresentationShow(kObject6, 2);
		_app->objectPresentationShow(kObject6, 6);
		break;

	case 30122:
		_app->objectPresentationHideAndRemove(kObject6, 2);
		_app->rotationSetActive(30004);
		_app->soundPlay(30123);
		_app->objectPresentationShow(kObject6, 3);
		break;

	case 30123:
		_app->objectPresentationHideAndRemove(kObject6, 3);
		_app->objectPresentationHideAndRemove(kObject6, 6);
		_app->rotationSetActive(30005);
		_app->soundPlay(30124);
		_app->objectPresentationShow(kObject6, 4);
		_app->objectPresentationShow(kObject6, 7);
		break;

	case 30124:
		_app->objectPresentationHideAndRemove(kObject6, 4);
		_app->objectPresentationHideAndRemove(kObject6, 7);
		_app->rotationSetActive(30006);
		_app->soundPlay(30125);
		_app->objectPresentationShow(kObject6, 8);
		_app->objectPresentationShow(kObject6, 9);
		break;

	case 30125:
		_app->objectPresentationHideAndRemove(kObject6, 9);
		_app->rotationSetActive(30008);
		_app->soundPlay(30126);
		_app->objectPresentationShow(kObject6, 10);
		break;

	case 30126:
		_app->objectPresentationHideAndRemove(kObject6, 8);
		_app->objectPresentationHideAndRemove(kObject6, 10);
		_app->rotationSetActive(30009);
		_app->soundPlay(30127);
		_app->objectPresentationShow(kObject6, 11);
		_app->objectPresentationShow(kObject6, 15);
		break;

	case 30127:
		_app->objectPresentationHideAndRemove(kObject6, 11);
		_app->rotationSetActive(30010);
		_app->soundPlay(30128);
		_app->objectPresentationShow(kObject6, 12);
		break;

	case 30128:
		_app->objectPresentationHideAndRemove(kObject6, 12);
		_app->objectPresentationHideAndRemove(kObject6, 15);
		_app->rotationSetActive(30011);
		_app->soundPlay(30129);
		_app->objectPresentationShow(kObject6, 13);
		_app->objectPresentationShow(kObject6, 16);
		break;

	case 30129:
		_app->objectPresentationHideAndRemove(kObject6, 13);
		_app->rotationSetActive(30012);
		_app->soundPlay(30130);
		_app->objectPresentationShow(kObject6, 14);
		break;

	case 30130:
		_app->objectPresentationHideAndRemove(kObject6, 14);
		_app->objectPresentationHideAndRemove(kObject6, 16);
		_app->rotationSetActive(30701);
		_app->soundPlay(30131);
		_app->objectPresentationShow(kObject6, 17);
		_app->objectPresentationShow(kObject6, 18);
		break;

	case 30131:
		_app->objectPresentationHideAndRemove(kObject6, 18);
		_app->rotationSetActive(30702);
		_app->soundPlay(30132);
		_app->objectPresentationShow(kObject6, 19);
		break;

	case 30132:
		_app->objectPresentationHideAndRemove(kObject6, 17);
		_app->objectPresentationHideAndRemove(kObject6, 19);
		_app->rotationSetActive(30703);
		_app->soundPlay(30133);
		_app->objectPresentationShow(kObject6, 20);
		_app->objectPresentationShow(kObject6, 24);
		break;

	case 30133:
		_app->objectPresentationHideAndRemove(kObject6, 20);
		_app->objectPresentationHideAndRemove(kObject6, 24);
		_app->rotationSetActive(30704);
		_app->soundPlay(30134);
		_app->objectPresentationShow(kObject6, 21);
		_app->objectPresentationShow(kObject6, 25);
		break;

	case 30134:
		_app->objectPresentationHideAndRemove(kObject6, 21);
		_app->rotationSetActive(30401);
		_app->soundPlay(30135);
		_app->objectPresentationShow(kObject6, 22);
		break;

	case 30135:
		_app->objectPresentationHideAndRemove(kObject6, 22);
		_app->objectPresentationHideAndRemove(kObject6, 25);
		_app->rotationSetActive(30402);
		_app->soundPlay(30136);
		_app->objectPresentationShow(kObject6, 23);
		_app->objectPresentationShow(kObject6, 26);
		break;

	case 30136:
		_app->objectPresentationHideAndRemove(kObject6, 23);
		_app->objectPresentationHideAndRemove(kObject6, 26);
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->playMovie("1215");
		_app->onSwitchZone(kZoneAS, 3);
		break;

	case 30161:
		_app->playMovie("1212");
		_app->puzzleSetActive(kPuzzle35108);
		_app->soundPlay(30109);
		break;

	case 30506:
		if (_app->varGetByte(30043) == 1 && _app->bagHas(kObjectSleepingPotion2)) {
			_app->playMovie("1216");
			_app->objectPresentationShow(kObject30100, 0);
			_app->objectSetAccessibilityOff(kObject30100, 0, 0);
			_app->objectSetAccessibilityOn(kObject30100, 1, 1);
		} else {
			_app->objectSetAccessibilityOn(kObject30100, 0, 0);
			_app->puzzleSetMovabilityOn(kPuzzle35100, 0, 0);
		}
		break;
	}
}

void ZoneFORing::onBag(ObjectId id, Id, Id, uint32, DragControl *, byte type) {
	if (id != kObject30016)
		return;

	switch (type) {
	default:
		break;

	case 1:
		_app->dragControlSetField45(2);
		_app->soundPlay(30500);
		_app->dragControlSetCoords1(Common::Point(440, 248));
		break;

	case 2:
		_app->soundStop(30500, 1024);
		break;

	case 3:
		if (_app->dragControlXHigher1() && _app->dragControlYLower1()) {
			if (_app->dragControlXHigher() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXLower() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXHigher1() && _app->dragControlYHigher1()) {
			if (_app->dragControlXLower() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXHigher() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXLower1() && _app->dragControlYHigher1()) {
			if (_app->dragControlXLower() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXHigher() && _app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		if (_app->dragControlXLower1() && _app->dragControlYLower1()) {
			if (_app->dragControlXHigher() && _app->dragControlYLower()) {

				_app->varSetByte(30016, _app->varGetByte(30016) + 1);
				if (_app->varGetByte(30016) > 48)
					_app->varSetByte(30016, 0);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}

			if (_app->dragControlXLower() &&_app->dragControlYHigher()) {

				_app->varSetByte(30016, _app->varGetByte(30016) - 1);
				if (_app->varGetByte(30016) < 0)
					_app->varSetByte(30016, 48);

				_app->objectPresentationHide(kObject30016);
				_app->objectPresentationShow(kObject30016, (uint8)_app->varGetByte(30016));
			}
		}

		_app->soundSetVolume(30500, _app->dragControlGetDistance() + 80);
		break;
	}
}

void ZoneFORing::onBagClickedObject(ObjectId objectId) {
	if (objectId != kObjectWolfInstinct)
		return;

	Id id = objectId;
	if (_app->hasCurrentRotation())
		id = _app->getCurrentRotationId();

	if (_app->hasCurrentPuzzle())
		id = _app->getCurrentPuzzleId();

	// Rotation
	if (id == 30302 || id == 30303) {
		if (_app->varGetByte(30017) == 1) {
			_app->rotationSetAlp(30302, _app->rotationGetAlp(30303));
			_app->rotationSetBet(30302, _app->rotationGetBet(30303));
			_app->rotationSetRan(30302, _app->rotationGetRan(30303));
			_app->rotationSetActive(30302);

			_app->varSetByte(30017, 0);
		} else {
			_app->rotationSetAlp(30303, _app->rotationGetAlp(30302));
			_app->rotationSetBet(30303, _app->rotationGetBet(30302));
			_app->rotationSetRan(30303, _app->rotationGetRan(30302));
			_app->rotationSetActive(30303);

			_app->varSetByte(30017, 1);
		}
	} else if (id == kPuzzle35020) { //  Puzzle 35020
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectWorms, 0);
			_app->objectPresentationHide(kObjectWorms, 1);
			_app->objectSetAccessibilityOff(kObjectWorms, 0, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectWorms, 0);
			_app->objectPresentationShow(kObjectWorms, 1);
			_app->objectSetAccessibilityOn(kObjectWorms, 0, 0);
		}
	} else if (id == kPuzzle35019) { //  Puzzle 35019
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectNotung);
			_app->objectSetAccessibilityOff(kObjectNotung, 0, 0);
		}

		_app->varSetByte(30017, 1);
		_app->objectSetAccessibilityOn(kObjectNotung, 0, 0);

		if (_app->varGetByte(30066) != 1 && _app->varGetByte(30067) != 1) {
			_app->objectPresentationShow(kObjectNotung, 0);
		} else {
			_app->objectPresentationShow(kObjectNotung, 0);
			_app->objectPresentationShow(kObjectNotung, 2);
		}
	} else if (id == kPuzzle35002) { //  Puzzle 35002
		if (_app->varGetByte(30017)) {
			_app->objectPresentationHide(kObjectBerries, 1);
			_app->objectPresentationHide(kObjectBerries, 2);
			_app->objectPresentationHide(kObjectBerries, 3);
			_app->varSetByte(30017, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectBerries, 1);

			if (_app->varGetByte(30075) == 1)
				_app->objectPresentationHide(kObjectBerries, 3);
			else
				_app->objectPresentationShow(kObjectBerries, 3);

			if (_app->varGetByte(30076) == 1)
				_app->objectPresentationHide(kObjectBerries, 2);
			else
				_app->objectPresentationShow(kObjectBerries, 2);
		}
	} else {
		return;
	}

	_app->setField78(false);
}

void ZoneFORing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 30402 && movabilityTo == 30501) {
			_app->varSetByte(30037, 1);
			_app->objectPresentationShow(kObject30051, 0);
			_app->objectPresentationHide(kObject30050);
			_app->objectPresentationShow(kObject30050, 3);
			_app->rotationSetMovabilityOff(30011, 1, 2);
			_app->objectSetAccessibilityOn(kObject30051, 0, 0);
			_app->objectSetAccessibilityOn(kObject30051, 3, 3);
		}
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom == 30601 && movabilityTo == 35003)
			_app->objectSetAccessibilityOn(kObjectBow, 0, 0);
		break;

	case kMovabilityPuzzleToRotation:
		switch (movabilityFrom) {
		default:
			break;

		case 35002:
			if (movabilityTo == 30801) {
				if (!_app->varGetByte(30076)) {
					_app->objectPresentationHide(kObjectBerries);
					_app->varSetByte(30017, 0);
				}
			}
			break;

		case 35003:
			if (movabilityTo == 30601)
				_app->objectPresentationHide(kObjectBow);
			break;

		case 35004:
			if (movabilityTo == 30601) {
				if (_app->varGetByte(30091) == 1 && _app->varGetByte(30021) == 1) {
					_app->playMovie("1184");
					_app->bagAdd(kObjectLogeTear2);

					if (_app->varGetByte(30020) == 1) {
						_app->bagAdd(kObjectMould);
						_app->varSetByte(30020, 0);
					}

					_app->objectPresentationHide(kObject30028);
					_app->objectPresentationShow(kObject30028, 0);
					_app->objectPresentationShow(kObject30028, 3);

					_app->varSetByte(30021, 0);
				}
			}
			break;

		case 35006:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30040);
				_app->varSetByte(30022, 0);
				_app->varSetByte(30023, 0);
				_app->objectSetAccessibilityOn(kObject30040, 0, 1);
				_app->objectSetAccessibilityOff(kObject30040, 2, 5);
			}
			break;

		case 35007:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30042);
				_app->objectSetAccessibilityOff(kObject30042, 1, 7);
				_app->objectSetAccessibilityOn(kObject30042, 0, 0);
				_app->varSetByte(30025, 0);
				_app->varSetByte(30026, 0);
				_app->varSetByte(30027, 0);
				_app->varSetByte(30028, 0);
				_app->varSetByte(30029, 0);
				_app->varSetByte(30030, 0);
				_app->varSetByte(30031, 0);
			}
			break;

		case 35008:
			if (movabilityTo == 30601) {
				if (_app->varGetByte(30032) == 1) {
					_app->bagAdd(kObjectGolem);
					_app->objectPresentationHide(30044, 0);
				}
			}
			break;

		case 35009:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30045);
			} else if (movabilityTo == 30602) {
				_app->playMovie("1205");
				_app->objectPresentationShow(kObject30045, 7);
			}
			break;

		case 30701:
		case 30702:
		case 30703:
		case 30704:
			if (_app->varGetByte(30033)) {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 1, 1);
			} else {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 1);
			}
			break;

		case 35111:
			if (movabilityTo == 30101) {
				_app->playMovie("1206");
				_app->timerStop(kTimer5);
			}
			break;
		}

		_app->objectSetAccessibilityOff(kObjectNotung, 0, 0);
		_app->objectPresentationHide(kObjectNotung);
		_app->objectPresentationHide(kObjectWorms, 0);
		_app->objectPresentationHide(kObjectWorms, 1);
		_app->objectPresentationHide(kObjectBerries, 3);
		_app->objectPresentationHide(kObjectBerries, 1);
		_app->varSetByte(30017, 0);
		_app->objectSetAccessibilityOff(kObjectWorms, 0, 0);
		break;
	}
}

void ZoneFORing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		switch (movabilityFrom) {
		default:
			break;

		case 30006:
			if (movabilityTo == 30008) {
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 100);
			} else if (movabilityTo == 30005) {
				_app->rotationSetMovabilityRideName(30005, 1, "fom");
			}
			break;

		case 30008:
			if (movabilityTo == 30006)
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 80);
			break;

		case 30011:
			if (movabilityTo == 30401)
				for (uint32 i = 0; i < 27; i++)
					_app->soundSetVolume(i + 30300, 100);
			break;

		case 30012:
			if (movabilityTo == 30501) {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);
				_app->timerStart(kTimer1, 10);
				_app->timerStart(kTimer3, 10);
			}
			break;

		case 30101:
			if (movabilityTo == 30003) {
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 80);

				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 100);
			}
			break;

		case 30401:
			if (movabilityTo == 30011)
				for (uint32 i = 0; i < 27; i++)
					_app->soundSetVolume(i + 30300, 85);
			break;

		case 30501:
			if (movabilityTo == 30012) {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);

				if (!_app->varGetByte(30071)) {
					_app->soundPlay(30117);
					_app->varSetByte(30071, 1);
				}
			}
			break;
		}
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityFrom == 30703 && movabilityTo == 35010) {
			_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
			_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 2);
			_app->objectPresentationHide(kObjectFishingRod);
		} else if (movabilityFrom == 30301 && movabilityTo == 35011) {
			_app->objectPresentationHide(kObject30001);
			_app->objectSetAccessibilityOff(kObject30001, 1, 3);
			if (!_app->varGetByte(30043))
				_app->objectSetAccessibilityOn(kObject30001, 0, 0);
		}
		break;
	}
}

} // End of namespace Ring
