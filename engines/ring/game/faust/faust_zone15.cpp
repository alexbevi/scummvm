/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 15
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 015110-1301, USA.
 */

#include "ring/game/faust/faust_zone15.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone15Faust::Zone15Faust(ApplicationFaust *application) : _app(application) {
}

Zone15Faust::~Zone15Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone15Faust::onInit() {
	_app->setCurrentZone(kZone15);

	_app->puzzleAdd(kPuzzle120001);
	_app->puzzleAddBackgroundImage(kPuzzle120001, "a12s01n04p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120002);
	_app->puzzleAddBackgroundImage(kPuzzle120002, "a12s01n04p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120003);
	_app->puzzleAddBackgroundImage(kPuzzle120003, "a12s01n04p04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120004);
	_app->puzzleAddBackgroundImage(kPuzzle120004, "a12s02n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120005);
	_app->puzzleAddBackgroundImage(kPuzzle120005, "a12s04n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120006);
	_app->puzzleAddBackgroundImage(kPuzzle120006, "a12s04n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120007);
	_app->puzzleAddBackgroundImage(kPuzzle120007, "a12s05n01p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120008);
	_app->puzzleAddBackgroundImage(kPuzzle120008, "a12s05n01p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120015);
	_app->puzzleAddBackgroundImage(kPuzzle120015, "a12s05n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120009);
	_app->puzzleAddBackgroundImage(kPuzzle120009, "a12s07n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120010);
	_app->puzzleAddBackgroundImage(kPuzzle120010, "a12s01n04p05.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120011);
	_app->puzzleAddBackgroundImage(kPuzzle120011, "a12s02n03p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120014);
	_app->puzzleAddBackgroundImage(kPuzzle120014, "a12s02n03p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120012);
	_app->puzzleAddBackgroundImage(kPuzzle120012, "a12s04n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle120013);
	_app->puzzleAddBackgroundImage(kPuzzle120013, "A12S06N02R01_FADE_02.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(120101, "1974", 0, 1);
	_app->rotationAdd(120102, "1975", 0, 1);
	_app->rotationAdd(120103, "1976", 0, 5);
	_app->rotationAdd(120104, "1977", 0, 5);
	_app->rotationAdd(120105, "1978", 0, 4);
	_app->rotationAdd(120106, "1979", 0, 7);
	_app->rotationAdd(120107, "1980", 0, 3);
	_app->rotationAdd(120108, "1981", 0, 3);
	_app->rotationAdd(120109, "1982", 0, 4);
	_app->rotationAdd(120110, "1983", 0, 1);
	_app->rotationAdd(120201, "1984", 0, 8);
	_app->rotationAdd(120202, "1985", 0, 7);
	_app->rotationAdd(120203, "1986", 0, 0);
	_app->rotationAdd(120204, "1987", 0, 0);
	_app->rotationAdd(120301, "1988", 0, 8);
	_app->rotationAdd(120401, "1989", 0, 2);
	_app->rotationAdd(120402, "1990", 0, 4);
	_app->rotationAdd(120501, "1991", 0, 12);
	_app->rotationSetCompressionBufferLength(120501, 2600000);
	_app->rotationAdd(120601, "1992", 0, 3);
	_app->rotationAdd(120602, "1993", 0, 4);
	_app->rotationAdd(120603, "1994", 0, 1);
	_app->rotationAdd(120701, "1995", 0, 1);
	_app->rotationAdd(120702, "1996", 0, 0);
	_app->rotationAdd(120703, "1997", 0, 0);
	_app->rotationAdd(120704, "1998", 0, 0);
	_app->rotationAddMovabilityToRotation(120101, 120102, "", Common::Rect(2942, -81, 3355, 390), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120101, 0, 270.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120102, 120101, "", Common::Rect(1167, -50, 1589, 359), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120102, 0, 90.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120102, 120103, "", Common::Rect(3074, -324, 3369, 165), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120102, 1, 274.0f, -14.0f, 87.0f, 0.0f, 2, 274.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120102, 120110, "", Common::Rect(2322, -81, 2701, 413), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120102, 2, 213.0f, 0.0f, 87.0f, 0.0f, 2, 213.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120103, 120102, "", Common::Rect(1279, 240, 1652, 582), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120103, 0, 104.0f, 26.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120103, 120104, "", Common::Rect(421, 20, 733, 231), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120103, 1, 7.0f, 3.0f, 87.0f, 0.0f, 2, 7.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120104, 120106, "", Common::Rect(3273, -211, 3570, 268), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120104, 0, 297.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120104, 120107, "", Common::Rect(156, -226, 393, 218), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120104, 1, 343.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120104, 120108, "", Common::Rect(1026, -219, 1274, 191), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120104, 2, 69.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120104, 120103, "", Common::Rect(2253, -61, 2610, 376), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120104, 3, 196.0f, 0.0f, 87.0f, 0.0f, 2, 196.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(120104, kPuzzle120001, "", Common::Rect(2856, 214, 2912, 273), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(120104, 4, 263.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(120104, kPuzzle120002, "", Common::Rect(2939, 245, 2983, 310), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(120104, 5, 263.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(120104, kPuzzle120003, "", Common::Rect(3067, 256, 3106, 294), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(120104, 6, 263.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(120105, 120106, "", Common::Rect(311, -85, 532, 375), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120105, 0, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120106, 120104, "", Common::Rect(1230, 312, 1657, 538), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120106, 0, 94.0f, 26.0f, 87.0f, 0.0f, 2, 94.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120106, 120105, "", Common::Rect(2199, -80, 2429, 368), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120106, 1, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120106, 120107, "", Common::Rect(675, -71, 900, 198), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120106, 2, 30.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120106, 120201, "", Common::Rect(2859, -118, 3524, 441), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120106, 3, 270.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120107, 120104, "", Common::Rect(2091, 273, 2439, 512), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120107, 0, 180.0f, 26.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120107, 120106, "", Common::Rect(2587, -109, 2793, 225), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120107, 1, 225.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120107, 120108, "", Common::Rect(1515, -62, 1710, 190), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120107, 2, 116.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120108, 120104, "", Common::Rect(3009, 214, 3294, 460), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120108, 0, 272.0f, 21.0f, 87.0f, 0.0f, 2, 272.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120108, 120107, "", Common::Rect(3355, -59, 3600, 184), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120108, 1, 305.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120108, 120107, "", Common::Rect(0, -59, 31, 184), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120108, 2, 305.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120108, 120109, "", Common::Rect(2093, -94, 2308, 416), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120108, 3, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120109, 120108, "", Common::Rect(384, -38, 711, 458), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120109, 0, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120110, 120102, "", Common::Rect(910, -94, 1207, 298), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120110, 0, 64.0f, 0.0f, 87.0f, 0.0f, 2, 64.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120201, 120202, "", Common::Rect(1908, -130, 2153, 331), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120201, 0, 161.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120201, 120203, "", Common::Rect(2438, -83, 2719, 368), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120201, 1, 211.0f, 0.0f, 87.0f, 0.0f, 2, 211.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120201, 120501, "", Common::Rect(3512, -195, 3600, 364), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120201, 2, 323.0f, 0.0f, 87.0f, 0.0f, 2, 323.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120201, 120501, "", Common::Rect(0, -195, 256, 364), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120201, 3, 323.0f, 0.0f, 87.0f, 0.0f, 2, 323.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120201, 120106, "", Common::Rect(1177, -90, 1645, 406), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120201, 4, 93.0f, 10.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(120201, kPuzzle120004, "", Common::Rect(1636, 19, 1720, 160), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(120201, 5, 263.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(120202, 120201, "", Common::Rect(321, -57, 624, 361), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120202, 0, 356.0f, 0.0f, 87.0f, 0.0f, 2, 356.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120203, 120201, "", Common::Rect(638, -12, 873, 310), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120203, 0, 23.0f, 0.0f, 87.0f, 0.0f, 2, 23.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120203, 120204, "", Common::Rect(418, -230, 638, 387), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120203, 1, 2.0f, -3.0f, 87.0f, 0.0f, 2, 0.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120203, 120601, "", Common::Rect(2756, -163, 3266, 368), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120203, 2, 253.0f, 0.0f, 87.0f, 0.0f, 2, 253.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120203, 120301, "", Common::Rect(2047, -85, 2350, 395), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120203, 3, 178.0f, 0.0f, 87.0f, 0.0f, 2, 178.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(120203, kPuzzle120011, "", Common::Rect(1481, 304, 1555, 357), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(120203, 4, 99.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(120203, kPuzzle120014, "", Common::Rect(1337, 301, 1460, 380), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(120203, 5, 99.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(120204, 120203, "", Common::Rect(2177, 313, 2450, 579), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120204, 0, 180.0f, 26.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120204, 120401, "", Common::Rect(22, -254, 875, 251), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120204, 1, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120301, 120203, "", Common::Rect(370, -125, 757, 455), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120301, 0, 5.0f, 0.0f, 87.0f, 0.0f, 2, 5.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120401, 120204, "", Common::Rect(1854, -219, 2698, 289), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120401, 0, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120401, 120402, "", Common::Rect(813, -184, 1091, 446), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120401, 1, 51.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120402, 120401, "", Common::Rect(3014, -153, 3600, 432), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120402, 0, 283.0f, 0.0f, 87.0f, 0.0f, 2, 283.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120402, 120401, "", Common::Rect(0, -153, 10, 432), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120402, 1, 283.0f, 0.0f, 87.0f, 0.0f, 2, 283.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120501, 120201, "", Common::Rect(2334, -219, 2765, 397), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120501, 0, 208.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(120501, kPuzzle120015, "", Common::Rect(395, -292, 646, -70), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(120501, 1, 263.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(120601, 120203, "", Common::Rect(952, -179, 1393, 395), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120601, 0, 72.0f, 0.0f, 87.0f, 0.0f, 2, 72.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120601, 120602, "", Common::Rect(277, -184, 643, 313), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120601, 1, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120602, 120601, "", Common::Rect(2181, -143, 2436, 303), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120602, 0, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120602, 120603, "", Common::Rect(344, -57, 564, 341), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120602, 1, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120603, 120602, "", Common::Rect(1942, -150, 2517, 424), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120603, 0, 180.0f, 12.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(120701, 120501, "", Common::Rect(1835, -193, 2188, 418), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(120701, 0, 156.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120001, 120104, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120001, 0, 263.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120002, 120104, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120002, 0, 263.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120003, 120104, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120003, 0, 263.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120010, 120104, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120010, 0, 263.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120004, 120201, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120004, 0, 123.0f, 10.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120005, 120401, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120005, 0, 333.0f, 24.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120006, 120402, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120006, 0, 137.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120007, 120501, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120007, 0, 85.0f, 11.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120011, 120203, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120011, 0, 99.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120014, 120203, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120014, 0, 99.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120012, 120402, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120012, 0, 41.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle120015, 120501, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle120015, 0, 2.0f, -10.0f, 87.0f);
	_app->objectAdd(kObject120001, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120001, 120102, Common::Rect(3125, -348, 3169, -300), true, kCursorActionObject, 0);
	_app->objectAddRotationAccessibility(kObject120001, 120103, Common::Rect(1627, 395, 1722, 573), true, kCursorAction, 1);
	_app->objectAddPresentation(kObject120001);
	_app->objectPresentationAddImageToRotation(kObject120001, 0, 120101, 0);
	_app->objectAddPresentation(kObject120001);
	_app->objectPresentationAddImageToRotation(kObject120001, 1, 120102, 0);
	_app->objectAddPresentation(kObject120001);
	_app->objectPresentationAddImageToRotation(kObject120001, 2, 120110, 0);
	_app->objectAddPresentation(kObject120001);
	_app->objectPresentationAddImageToRotation(kObject120001, 3, 120103, 0);
	_app->objectAddPresentation(kObject120001);
	_app->objectPresentationAddImageToRotation(kObject120001, 4, 120105, 0);
	_app->objectAddPresentation(kObject120001);
	_app->objectPresentationAddImageToRotation(kObject120001, 5, 120109, 0);
	_app->objectAddPresentation(kObject120001);
	_app->objectPresentationAddImageToRotation(kObject120001, 6, 120104, 4);
	_app->objectPresentationShow(kObject120001);
	_app->objectAdd(kObject120002, "", "", 1);
	_app->objectAddPresentation(kObject120002);
	_app->objectPresentationAddAnimationToRotation(kObject120002, 0, 120103, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120002, 0, 0, 120001);
	_app->objectAddPresentation(kObject120002);
	_app->objectPresentationAddAnimationToRotation(kObject120002, 1, 120103, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120002, 1, 0, 120002);
	_app->objectAdd(kObject120003, "", "", 1);
	_app->objectAdd(kObjectNail, "Nail", "A12_Nail", 1);
	_app->objectSetActiveCursor(kObjectNail, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectNail, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject120003, kPuzzle120001, Common::Rect(273, 131, 351, 193), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObject120003, kPuzzle120001, Common::Rect(104, 233, 291, 432), true, kCursorAction, 1);
	_app->objectAddPresentation(kObject120003);
	_app->objectPresentationAddImageToPuzzle(kObject120003, 0, kPuzzle120001, "a12s01n04p02l01.bmp", Common::Point(297, 146), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120003);
	_app->objectPresentationAddImageToPuzzle(kObject120003, 1, kPuzzle120001, "a12s01n04p02l02.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120003);
	_app->objectPresentationAddImageToRotation(kObject120003, 2, 120104, 0);
	_app->objectAdd(kObject120004, "", "", 1);
	_app->objectAdd(kObjectNecklace2, "Handkerchief", "A12_neckles", 1);
	_app->objectSetActiveCursor(kObjectNecklace2, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectNecklace2, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject120004, kPuzzle120002, Common::Rect(291, 163, 340, 233), false, kCursorTake, 0);
	_app->objectAddPresentation(kObject120004);
	_app->objectPresentationAddImageToPuzzle(kObject120004, 0, kPuzzle120002, "a12s01n04p03l01.bmp", Common::Point(274, 159), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObject120004, 0);
	_app->objectAdd(kObject120005, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject120005, kPuzzle120003, Common::Rect(458, 60, 485, 95), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject120005);
	_app->objectPresentationAddImageToPuzzle(kObject120005, 0, kPuzzle120003, "a12s01n04p04l01.bmp", Common::Point(449, 58), true, kDrawType1, 1000);
	_app->objectAdd(kObject120008, "", "", 1);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToRotation(kObject120008, 0, 120103, 2);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToRotation(kObject120008, 1, 120104, 1);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToRotation(kObject120008, 2, 120105, 1);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToRotation(kObject120008, 3, 120106, 3);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToRotation(kObject120008, 4, 120107, 0);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToRotation(kObject120008, 5, 120108, 0);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToRotation(kObject120008, 6, 120109, 1);
	_app->objectAddPresentation(kObject120008);
	_app->objectPresentationAddImageToPuzzle(kObject120008, 7, kPuzzle120010, "a12s01n04p05l01.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObject120008);
	_app->objectAdd(kObject120009, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120009, 120104, Common::Rect(2977, 130, 3095, 177), true, kCursorZoom, 0);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToRotation(kObject120009, 0, 120103, 3);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToRotation(kObject120009, 1, 120104, 2);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToRotation(kObject120009, 2, 120105, 2);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToRotation(kObject120009, 3, 120106, 4);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToRotation(kObject120009, 4, 120107, 1);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToRotation(kObject120009, 5, 120108, 1);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToRotation(kObject120009, 6, 120109, 2);
	_app->objectAddPresentation(kObject120009);
	_app->objectPresentationAddImageToPuzzle(kObject120009, 7, kPuzzle120010, "a12s01n04p05l02.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAdd(kObject120010, "", "", 1);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToRotation(kObject120010, 0, 120103, 4);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToRotation(kObject120010, 1, 120104, 3);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToRotation(kObject120010, 2, 120105, 3);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToRotation(kObject120010, 3, 120106, 5);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToRotation(kObject120010, 4, 120107, 2);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToRotation(kObject120010, 5, 120108, 2);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToRotation(kObject120010, 6, 120109, 3);
	_app->objectAddPresentation(kObject120010);
	_app->objectPresentationAddImageToPuzzle(kObject120010, 7, kPuzzle120010, "a12s01n04p05l03.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAdd(kObject120011, "", "", 1);
	_app->objectAdd(kObjectPeacockFeathers, "Peacock feathers", "A12_peacockF", 1);
	_app->objectSetActiveCursor(kObjectPeacockFeathers, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPeacockFeathers, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120011, 120106, Common::Rect(2573, 341, 2719, 509), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject120011, 120106, Common::Rect(2606, 392, 2656, 476), false, kCursorTake, 1);
	_app->objectAddRotationAccessibility(kObject120011, 120106, Common::Rect(2620, 308, 2707, 366), false, kCursorAction, 2);
	_app->objectAddPresentation(kObject120011);
	_app->objectPresentationAddAnimationToRotation(kObject120011, 0, 120106, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120011, 0, 0, 120003);
	_app->objectAddPresentation(kObject120011);
	_app->objectPresentationAddAnimationToRotation(kObject120011, 1, 120106, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120011, 1, 0, 120004);
	_app->objectAddPresentation(kObject120011);
	_app->objectPresentationAddAnimationToRotation(kObject120011, 2, 120106, 6, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120011, 2, 0, 120003);
	_app->objectAddPresentation(kObject120011);
	_app->objectPresentationAddAnimationToRotation(kObject120011, 3, 120106, 6, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120011, 3, 0, 120004);
	_app->objectAddPresentation(kObject120011);
	_app->objectPresentationAddImageToRotation(kObject120011, 4, 120106, 0);
	_app->objectAddPresentation(kObject120011);
	_app->objectPresentationAddImageToRotation(kObject120011, 5, 120106, 2);
	_app->objectAdd(kObject120013, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120013, 120104, Common::Rect(1734, 34, 1826, 235), true, kCursorZoom, 0);
	_app->objectAdd(kObject120014, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120014, 120106, Common::Rect(3062, -10, 3237, 177), true, kCursorZoom, 0);
	_app->objectAdd(kObjectFlour, "Flour", "A12_Flour", 1);
	_app->objectSetActiveCursor(kObjectFlour, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectFlour, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectFlour, 120501, Common::Rect(1265, 345, 1313, 383), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObjectFlour, 120501, Common::Rect(1084, 334, 1160, 525), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObjectFlour, 120501, Common::Rect(1233, 383, 1316, 456), false, kCursorTake, 2);
	_app->objectAddPresentation(kObjectFlour);
	_app->objectPresentationAddAnimationToRotation(kObjectFlour, 0, 120501, 10, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObjectFlour, 0, 0, 120019);
	_app->objectAddPresentation(kObjectFlour);
	_app->objectPresentationAddAnimationToRotation(kObjectFlour, 1, 120501, 10, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObjectFlour, 1, 0, 120020);
	_app->objectAddPresentation(kObjectFlour);
	_app->objectPresentationAddAnimationToRotation(kObjectFlour, 2, 120501, 11, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObjectFlour, 2, 0, 120019);
	_app->objectAddPresentation(kObjectFlour);
	_app->objectPresentationAddAnimationToRotation(kObjectFlour, 3, 120501, 11, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObjectFlour, 3, 0, 120020);
	_app->objectAddPresentation(kObjectFlour);
	_app->objectPresentationAddImageToRotation(kObjectFlour, 4, 120501, 4);
	_app->objectAddPresentation(kObjectFlour);
	_app->objectPresentationAddImageToRotation(kObjectFlour, 5, 120501, 5);
	_app->objectAdd(kObject120016, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject120016, kPuzzle120004, Common::Rect(219, 128, 404, 201), true, kCursorZoom, 0);
	_app->objectAddPuzzleAccessibility(kObject120016, kPuzzle120004, Common::Rect(233, 249, 367, 301), true, kCursorZoom, 1);
	_app->objectAddPuzzleAccessibility(kObject120016, kPuzzle120004, Common::Rect(241, 323, 369, 365), true, kCursorZoom, 2);
	_app->objectAdd(kObject120017, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120017, 120602, Common::Rect(667, 49, 752, 147), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject120017, 120602, Common::Rect(768, 78, 880, 173), true, kCursorZoom, 1);
	_app->objectAddPresentation(kObject120017);
	_app->objectPresentationAddImageToRotation(kObject120017, 0, 120602, 1);
	_app->objectAddPresentation(kObject120017);
	_app->objectPresentationAddImageToRotation(kObject120017, 1, 120602, 2);
	_app->objectAddPresentation(kObject120017);
	_app->objectPresentationAddImageToRotation(kObject120017, 2, 120601, 1);
	_app->objectAddPresentation(kObject120017);
	_app->objectPresentationAddImageToRotation(kObject120017, 3, 120601, 2);
	_app->objectAdd(kObject120018, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120018, 120602, Common::Rect(1880, -42, 2025, 396), true, kCursorZoom, 0);
	_app->objectAddPresentation(kObject120018);
	_app->objectPresentationAddImageToRotation(kObject120018, 0, 120602, 3);
	_app->objectAdd(kObject120019, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120019, 120601, Common::Rect(1627, -64, 1773, 243), true, kCursorZoom, 0);
	_app->objectAddPresentation(kObject120019);
	_app->objectPresentationAddImageToRotation(kObject120019, 0, 120601, 0);
	_app->objectAddPresentation(kObject120019);
	_app->objectPresentationAddImageToRotation(kObject120019, 1, 120602, 0);
	_app->objectAdd(kObject120020, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120020, 120601, Common::Rect(2032,  19, 2376, 306), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject120020, 120601, Common::Rect(265,  -45,  355, 318), true, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject120020, 120601, Common::Rect(2543, 238, 2826, 533), true, kCursorZoom, 2);
	_app->objectAddRotationAccessibility(kObject120020, 120602, Common::Rect(2522, -71, 2649, 406), true, kCursorZoom, 3);
	_app->objectAdd(kObject120024, "", "", 1);
	_app->objectAdd(kObjectShoeLaces, "Shoe laces", "A12_laces", 1);
	_app->objectSetActiveCursor(kObjectShoeLaces, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectShoeLaces, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectRespirationGrille, "Respiration grille", "A12_grile", 1);
	_app->objectSetActiveCursor(kObjectRespirationGrille, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectRespirationGrille, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120024, 120201, Common::Rect(391, 420, 569, 479), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject120024, 120201, Common::Rect(453, 514, 509, 538), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject120024, 120201, Common::Rect(407, 443, 536, 497), false, kCursorTake, 2);
	_app->objectAddPresentation(kObject120024);
	_app->objectPresentationAddAnimationToRotation(kObject120024, 0, 120201, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120024, 0, 0, 120005);
	_app->objectAddPresentation(kObject120024);
	_app->objectPresentationAddAnimationToRotation(kObject120024, 1, 120201, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120024, 1, 0, 120006);
	_app->objectAddPresentation(kObject120024);
	_app->objectPresentationAddAnimationToRotation(kObject120024, 2, 120201, 7, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120024, 2, 0, 120005);
	_app->objectAddPresentation(kObject120024);
	_app->objectPresentationAddAnimationToRotation(kObject120024, 3, 120201, 7, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120024, 3, 0, 120006);
	_app->objectAddPresentation(kObject120024);
	_app->objectPresentationAddImageToRotation(kObject120024, 4, 120201, 0);
	_app->objectAddPresentation(kObject120024);
	_app->objectPresentationAddImageToRotation(kObject120024, 5, 120201, 2);
	_app->objectAdd(kObject120029, "", "", 1);
	_app->objectAdd(kObjectGauze, "Gauze", "A12_gauze", 1);
	_app->objectSetActiveCursor(kObjectGauze, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectGauze, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectSoftTissue, "Soft tissue", "A12_S_tissue", 1);
	_app->objectSetActiveCursor(kObjectSoftTissue, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectSoftTissue, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120029, 120301, Common::Rect(2455, 431, 2666, 520), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject120029, 120301, Common::Rect(2489, 508, 2559, 530), false, kCursorAction, 1);
	_app->objectAddRotationAccessibility(kObject120029, 120301, Common::Rect(2566, 474, 2598, 502), false, kCursorTake, 2);
	_app->objectAddRotationAccessibility(kObject120029, 120301, Common::Rect(2489, 465, 2533, 502), false, kCursorTake, 3);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 0, 120301, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 0, 0, 120007);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 1, 120301, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 1, 0, 120008);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 2, 120301, 7, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 2, 0, 120007);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 3, 120301, 7, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 3, 0, 120008);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 4, 120301, 5, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 4, 0, 120007);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 5, 120301, 5, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 5, 0, 120008);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 6, 120301, 6, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 6, 0, 120007);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddAnimationToRotation(kObject120029, 7, 120301, 6, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120029, 7, 0, 120008);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddImageToRotation(kObject120029, 8, 120301, 0);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddImageToRotation(kObject120029, 9, 120301, 4);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddImageToRotation(kObject120029, 10, 120301, 3);
	_app->objectAddPresentation(kObject120029);
	_app->objectPresentationAddImageToRotation(kObject120029, 11, 120301, 2);
	_app->objectAdd(kObjectKalinkaJadeAmoulet, "Kalinka's jade amoulet", "A12_amoulet", 1);
	_app->objectSetActiveCursor(kObjectKalinkaJadeAmoulet, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectKalinkaJadeAmoulet, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectKalinkaJadeAmoulet, 120401, Common::Rect(130, 214, 221, 294), true, kCursorZoom, 0);
	_app->objectAddPuzzleAccessibility(kObjectKalinkaJadeAmoulet, kPuzzle120005, Common::Rect(222, 337, 302, 396), true, kCursorTake, 1);
	_app->objectAddPresentation(kObjectKalinkaJadeAmoulet);
	_app->objectPresentationAddAnimationToRotation(kObjectKalinkaJadeAmoulet, 0, 120401, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObjectKalinkaJadeAmoulet, 0, 0, 120009);
	_app->objectAddPresentation(kObjectKalinkaJadeAmoulet);
	_app->objectPresentationAddAnimationToRotation(kObjectKalinkaJadeAmoulet, 1, 120401, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObjectKalinkaJadeAmoulet, 1, 0, 120010);
	_app->objectAddPresentation(kObjectKalinkaJadeAmoulet);
	_app->objectPresentationAddImageToPuzzle(kObjectKalinkaJadeAmoulet, 2, kPuzzle120005, "a12s04n01p01l01.bmp", Common::Point(228, 341), true, kDrawType1, 1000);
	_app->objectAdd(kObjectLittleSilverMoon, "Little silver moon", "A12_moon", 1);
	_app->objectSetActiveCursor(kObjectLittleSilverMoon, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectLittleSilverMoon, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectLittleSilverMoon, 120402, Common::Rect(815, 523,  885, 570), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObjectLittleSilverMoon, 120402, Common::Rect(673, 352, 1030, 570), true, kCursorZoom, 1);
	_app->objectAddPresentation(kObjectLittleSilverMoon);
	_app->objectPresentationAddImageToRotation(kObjectLittleSilverMoon, 0, 120402, 0);
	_app->objectAdd(kObjectMatches4, "Matches", "A05_Matches", 1);
	_app->objectSetActiveCursor(kObjectMatches4, Common::Point(11, 27), 5, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMatches4, Common::Point(11, 27), 5, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectAdd(kObject120034, "", "", 1);
	_app->objectAdd(kObject120037, "", "", 1);
	_app->objectAdd(kObject120038, "", "", 1);
	_app->objectAdd(kObjectWeddingRing, "Wedding ring", "A12_ring", 1);
	_app->objectSetActiveCursor(kObjectWeddingRing, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectWeddingRing, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120034, 120402, Common::Rect(1701, -41, 1907, 221), false, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject120034, 120402, Common::Rect(1761, 270, 1817, 362), false, kCursorAction, 1);
	_app->objectAddPuzzleAccessibility(kObject120034, kPuzzle120006, Common::Rect(198, 269, 471, 402), false, kCursorTake, 2);
	_app->objectAddPresentation(kObject120034);
	_app->objectPresentationAddAnimationToRotation(kObject120034, 0, 120402, 1, 75, 12.5f, 5);
	_app->objectPresentationSetAnimationOnRotation(kObject120034, 0, 0, 120011);
	_app->objectAddPresentation(kObject120034);
	_app->objectPresentationAddImageToRotation(kObject120034, 1, 120402, 2);
	_app->objectAddPresentation(kObject120034);
	_app->objectPresentationAddAnimationToRotation(kObject120034, 2, 120402, 3, 3, 12.5f, 5);
	_app->objectPresentationShow(kObject120034, 1);

	for (uint32 i = 0; i < 75; i++) {
		_app->objectAddPresentation(kObject120037);
		_app->objectPresentationAddImageToPuzzle(kObject120037, i, kPuzzle120006, Common::String::format("a12s04n02p01s01.%04d.bmp", i + 1), Common::Point(177, 223), true, kDrawType1, 1000);
	}

	for (uint32 i = 0; i < 75; i++) {
		_app->objectAddPresentation(kObject120038);
		_app->objectPresentationAddImageToPuzzle(kObject120038, i, kPuzzle120006, Common::String::format("a12s04n02p01s02.%04d.bmp", i + 1), Common::Point(178, 225), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject120039, "", "", 1);
	_app->objectAdd(kObjectPiecesOfSoftLeather, "Pieces of soft leather", "A12_Peaces_mask", 1);
	_app->objectSetActiveCursor(kObjectPiecesOfSoftLeather, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiecesOfSoftLeather, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120039, 120202, Common::Rect(3044, 272, 3241, 395), true, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject120039);
	_app->objectPresentationAddAnimationToRotation(kObject120039, 0, 120202, 1, 20, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120039, 0, 0, 120012);
	_app->objectAddPresentation(kObject120039);
	_app->objectPresentationAddAnimationToRotation(kObject120039, 1, 120202, 6, 20, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120039, 1, 0, 120013);
	_app->objectAddPresentation(kObject120039);
	_app->objectPresentationAddImageToRotation(kObject120039, 2, 120202, 4);
	_app->objectAddPresentation(kObject120039);
	_app->objectPresentationAddImageToRotation(kObject120039, 3, 120202, 5);
	_app->objectAdd(kObject120040, "", "", 1);
	_app->objectAdd(kObjectHardLeather, "Hard leather", "A12_H_leder", 1);
	_app->objectSetActiveCursor(kObjectHardLeather, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectHardLeather, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectSoftLeather, "Soft leather", "A12_S_leder", 1);
	_app->objectSetActiveCursor(kObjectSoftLeather, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectSoftLeather, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120040, 120202, Common::Rect(168,  55, 297, 108), true, kCursorTake, 0);
	_app->objectAddRotationAccessibility(kObject120040, 120202, Common::Rect(175, 104, 298, 162), true, kCursorTake, 1);
	_app->objectAdd(kObject120041, "", "", 1);
	_app->objectAdd(kObjectStrongLeatherStrap, "Strong leather strap", "A12_H_L_strap", 1);
	_app->objectSetActiveCursor(kObjectStrongLeatherStrap, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectStrongLeatherStrap, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectSoftLeatherStrap, "Soft leather strap", "A12_S_L_strap", 1);
	_app->objectSetActiveCursor(kObjectSoftLeatherStrap, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectSoftLeatherStrap, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectPiecesOfSoftLeatherWithHoles, "Pieces of soft leather with holes", "A12_Peaces_mask", 1);
	_app->objectSetActiveCursor(kObjectPiecesOfSoftLeatherWithHoles, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiecesOfSoftLeatherWithHoles, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120041, 120202, Common::Rect(3420, 320, 3541, 455), true, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject120041);
	_app->objectPresentationAddImageToRotation(kObject120041, 0, 120202, 0);
	_app->objectAddPresentation(kObject120041);
	_app->objectPresentationAddImageToRotation(kObject120041, 1, 120202, 2);
	_app->objectAddPresentation(kObject120041);
	_app->objectPresentationAddImageToRotation(kObject120041, 2, 120202, 3);
	_app->objectAdd(kObject120047, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120047, 120201, Common::Rect(2107, 261, 2264, 381), true, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject120047);
	_app->objectPresentationAddImageToRotation(kObject120047, 0, 120201, 3);
	_app->objectAddPresentation(kObject120047);
	_app->objectPresentationAddImageToRotation(kObject120047, 1, 120201, 4);
	_app->objectAddPresentation(kObject120047);
	_app->objectPresentationAddImageToRotation(kObject120047, 2, 120201, 5);
	_app->objectAddPresentation(kObject120047);
	_app->objectPresentationAddImageToRotation(kObject120047, 3, 120201, 6);
	_app->objectAdd(kObject120048, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject120048, kPuzzle120015, Common::Rect(241, 125, 277, 164), true, kCursorActionObject, 0);
	_app->objectAddRotationAccessibility(kObject120048, 120501, Common::Rect(425, 5, 701, 504), false, kCursorMove, 1);
	_app->objectAddPresentation(kObject120048);
	_app->objectPresentationAddAnimationToRotation(kObject120048, 0, 120501, 1, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120048, 0, 0, 120014);
	_app->objectAddPresentation(kObject120048);
	_app->objectPresentationAddImageToRotation(kObject120048, 1, 120501, 2);
	_app->objectAddPresentation(kObject120048);
	_app->objectPresentationAddImageToPuzzle(kObject120048, 2, kPuzzle120015, "a12s05n01p01l01.bmp", Common::Point(228, 124), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120048);
	_app->objectPresentationAddImageToRotation(kObject120048, 3, 120501, 0);
	_app->objectAdd(kObject120049, "", "", 1);
	_app->objectAdd(kObject120051, "", "", 1);
	_app->objectAdd(kObject120052, "", "", 1);
	_app->objectAdd(kObject120050, "", "", 1);
	_app->objectAdd(kObjectTheFourHerbs, "The Four Herbs", "A12_herbs", 1);
	_app->objectSetActiveCursor(kObjectTheFourHerbs, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectTheFourHerbs, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject120058, "", "", 1);
	_app->objectAddRotationAccessibility(kObject120049, 120701, Common::Rect(1378, 159, 1524, 252), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject120049, 120702, Common::Rect(1378, 159, 1524, 252), true, kCursorZoom, 1);
	_app->objectAddRotationAccessibility(kObject120049, 120703, Common::Rect(1378, 159, 1524, 252), true, kCursorZoom, 2);
	_app->objectAddRotationAccessibility(kObject120049, 120704, Common::Rect(1378, 159, 1524, 252), true, kCursorZoom, 3);
	_app->objectAddPuzzleAccessibility(kObject120049, kPuzzle120009, Common::Rect(173, 139, 335, 277), false, kCursorAction, 4);
	_app->objectAddPuzzleAccessibility(kObject120049, kPuzzle120009, Common::Rect(221,  43, 351, 131), false, kCursorAction, 5);
	_app->objectAddPuzzleAccessibility(kObject120049, kPuzzle120009, Common::Rect(370,  46, 541, 157), false, kCursorAction, 6);
	_app->objectAddPuzzleAccessibility(kObject120049, kPuzzle120009, Common::Rect(352, 149, 526, 281), false, kCursorAction, 7);
	_app->objectAddPuzzleAccessibility(kObject120049, kPuzzle120009, Common::Rect(0, 420, 640, 464), true, kCursorBack, 8);
	_app->objectAddRotationAccessibility(kObject120050, 120701, Common::Rect(94, -36, 239, 219), true, kCursorTake, 0);
	_app->objectAddRotationAccessibility(kObject120050, 120702, Common::Rect(94, -36, 239, 219), true, kCursorTake, 1);
	_app->objectAddRotationAccessibility(kObject120050, 120703, Common::Rect(94, -36, 239, 219), true, kCursorTake, 2);
	_app->objectAddRotationAccessibility(kObject120050, 120704, Common::Rect(94, -36, 239, 219), true, kCursorTake, 3);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(307, 155, 329, 170), true, kCursorAction, 0);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(336, 160, 355, 173), true, kCursorAction, 1);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(363, 161, 380, 175), true, kCursorAction, 2);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(391, 163, 408, 176), true, kCursorAction, 3);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(308, 176, 323, 190), true, kCursorAction, 4);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(332, 178, 346, 192), true, kCursorAction, 5);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(354, 181, 375, 195), true, kCursorAction, 6);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(381, 183, 399, 197), true, kCursorAction, 7);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(298, 198, 319, 212), true, kCursorAction, 8);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(326, 199, 344, 215), true, kCursorAction, 9);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(352, 202, 378, 217), true, kCursorAction, 10);
	_app->objectAddPuzzleAccessibility(kObject120058, kPuzzle120009, Common::Rect(385, 204, 398, 220), true, kCursorAction, 11);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 0, kPuzzle120009, "a12s07n01p01l02.bmp", Common::Point(293, 142), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 1, kPuzzle120009, "a12s07n01p01l03.bmp", Common::Point(319, 147), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 2, kPuzzle120009, "a12s07n01p01l04.bmp", Common::Point(346, 147), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 3, kPuzzle120009, "a12s07n01p01l05.bmp", Common::Point(373, 148), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 4, kPuzzle120009, "a12s07n01p01l06.bmp", Common::Point(289, 160), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 5, kPuzzle120009, "a12s07n01p01l07.bmp", Common::Point(314, 160), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 6, kPuzzle120009, "a12s07n01p01l08.bmp", Common::Point(337, 167), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 7, kPuzzle120009, "a12s07n01p01l09.bmp", Common::Point(363, 169), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 8, kPuzzle120009, "a12s07n01p01l10.bmp", Common::Point(285, 182), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 9, kPuzzle120009, "a12s07n01p01l11.bmp", Common::Point(311, 186), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 10, kPuzzle120009, "a12s07n01p01l12.bmp", Common::Point(340, 191), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120058);
	_app->objectPresentationAddImageToPuzzle(kObject120058, 11, kPuzzle120009, "a12s07n01p01l13.bmp", Common::Point(371, 191), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120051);
	_app->objectPresentationAddImageToRotation(kObject120051, 0, 120701, 0);
	_app->objectAddPresentation(kObject120051);
	_app->objectPresentationAddImageToPuzzle(kObject120051, 1, kPuzzle120009, "a12s07n01p01l01.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120049);
	_app->objectPresentationAddImageToPuzzle(kObject120049, 0, kPuzzle120009, "a12s07n01p02.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120049);
	_app->objectPresentationAddImageToPuzzle(kObject120049, 1, kPuzzle120009, "a12s07n01p03.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120049);
	_app->objectPresentationAddImageToPuzzle(kObject120049, 2, kPuzzle120009, "a12s07n01p04.bmp", Common::Point(0, 16), true, kDrawType1, 1000);
	_app->objectAdd(kObject120054, "", "", 1);
	_app->objectAdd(kObjectMixedHerbs, "Mixed Herbs", "A12_mixHerbs", 1);
	_app->objectSetActiveCursor(kObjectMixedHerbs, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectMixedHerbs, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectCork, "Cork", "A12_cork", 1);
	_app->objectSetActiveCursor(kObjectCork, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectCork, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject120054, kPuzzle120007, Common::Rect(94, 142, 156, 213), true, kCursorAction, 0);
	_app->objectAddRotationAccessibility(kObject120054, 120501, Common::Rect(1065, 75, 1372, 273), true, kCursorZoom, 1);
	_app->objectAddPuzzleAccessibility(kObject120054, kPuzzle120007, Common::Rect(191, 329, 205, 350), true, kCursorActionObject, 2);
	_app->objectAddPuzzleAccessibility(kObject120054, kPuzzle120007, Common::Rect(517, 353, 595, 419), false, kCursorTake, 3);
	_app->objectAddPresentation(kObject120054);
	_app->objectPresentationAddAnimationToPuzzle(kObject120054, 0, kPuzzle120007, "a12s05n01p02s01", kImageTypeBMP, Common::Point(387, 29), kDrawType1, 1000, 15, 15.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject120054, 0, 0, 120021);
	_app->objectAddPresentation(kObject120054);
	_app->objectPresentationAddAnimationToPuzzle(kObject120054, 1, kPuzzle120007, "a12s05n01p02s02", kImageTypeBMP, Common::Point(164, 304), kDrawType1, 1000, 2, 12.5f, 5);
	_app->objectAddPresentation(kObject120054);
	_app->objectPresentationAddImageToPuzzle(kObject120054, 2, kPuzzle120007, "a12s05n01p02l01.bmp", Common::Point(100, 143), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120054);
	_app->objectPresentationAddImageToPuzzle(kObject120054, 3, kPuzzle120007, "a12s05n01p02l02.bmp", Common::Point(135, 262), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120054);
	_app->objectPresentationAddImageToPuzzle(kObject120054, 4, kPuzzle120007, "a12s05n01p02l03.bmp", Common::Point(520, 391), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120054);
	_app->objectPresentationAddImageToRotation(kObject120054, 5, 120501, 3);
	_app->objectAddPresentation(kObject120054);
	_app->objectPresentationAddImageToRotation(kObject120054, 6, 120501, 6);
	_app->objectAdd(kObject120056, "", "", 1);
	_app->objectAdd(kObjectCompletedMask, "Completed mask", "A12_mask", 1);
	_app->objectSetActiveCursor(kObjectCompletedMask, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectCompletedMask, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject120056, 120501, Common::Rect(1390, 156, 1536, 270), true, kCursorZoom, 0);
	_app->objectAddPuzzleAccessibility(kObject120056, kPuzzle120008, Common::Rect(250, 150, 432, 304), true, kCursorActionObject, 1);
	_app->objectAddPuzzleAccessibility(kObject120056, kPuzzle120008, Common::Rect(0, 420, 640, 464), true, kCursorBack, 2);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddAnimationToRotation(kObject120056, 0, 120501, 7, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120056, 0, 0, 120017);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddAnimationToRotation(kObject120056, 1, 120501, 7, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120056, 1, 0, 120018);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddAnimationToRotation(kObject120056, 2, 120501, 8, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120056, 2, 0, 120017);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddAnimationToRotation(kObject120056, 3, 120501, 8, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120056, 3, 0, 120018);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddAnimationToRotation(kObject120056, 4, 120501, 9, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject120056, 4, 0, 120017);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddAnimationToRotation(kObject120056, 5, 120501, 9, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject120056, 5, 0, 120018);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddImageToPuzzle(kObject120056, 6, kPuzzle120008, "a12s05n01p03l01.bmp", Common::Point(173, 88), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddImageToPuzzle(kObject120056, 7, kPuzzle120008, "a12s05n01p03l02.bmp", Common::Point(173, 93), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddImageToPuzzle(kObject120056, 8, kPuzzle120008, "a12s05n01p03l03.bmp", Common::Point(164, 80), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject120056);
	_app->objectPresentationAddImageToPuzzle(kObject120056, 9, kPuzzle120008, "a12s05n01p03l04.bmp", Common::Point(170, 85), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObject120056, 7);
	_app->objectAdd(kObjectMask, "Mask", "A12_mask", 1);
	_app->objectSetActiveCursor(kObjectMask, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectMask, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject120059, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(524, 212, 564, 233), true, kCursorIdle, 0);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(444, 243, 473, 256), true, kCursorIdle, 1);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(517, 338, 546, 351), true, kCursorIdle, 2);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(439, 370, 467, 380), true, kCursorIdle, 3);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(439, 380, 478, 388), true, kCursorIdle, 4);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(437, 390, 473, 400), true, kCursorIdle, 5);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect( 72, 147, 272, 392), true, kCursorIdle, 6);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(283,  67, 432, 267), true, kCursorIdle, 7);
	_app->objectAddPuzzleAccessibility(kObject120059, kPuzzle120011, Common::Rect(288, 360, 407, 416), true, kCursorIdle, 8);
	_app->objectAdd(kObject120060, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(293, 157, 358, 208), true, kCursorAction, 0);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(359, 209, 408, 273), true, kCursorAction, 1);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(298, 270, 357, 324), true, kCursorAction, 2);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(246, 198, 289, 281), true, kCursorAction, 3);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(294, 103, 357, 154), false, kCursorAction, 4);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(409, 199, 457, 280), false, kCursorAction, 5);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(291, 322, 364, 374), false, kCursorAction, 6);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(200, 193, 242, 283), false, kCursorAction, 7);
	_app->objectAddPuzzleAccessibility(kObject120060, kPuzzle120012, Common::Rect(290, 202, 366, 280), false, kCursorAction, 8);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 0, kPuzzle120012, "a12s04n02p02l01.tga", Common::Point(234, 154), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 1, kPuzzle120012, "a12s04n02p02l02.tga", Common::Point(321, 154), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 2, kPuzzle120012, "a12s04n02p02l03.tga", Common::Point(234, 241), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 3, kPuzzle120012, "a12s04n02p02l04.tga", Common::Point(234, 154), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 4, kPuzzle120012, "a12s04n02p02l05.tga", Common::Point(234,  67), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 5, kPuzzle120012, "a12s04n02p02l06.tga", Common::Point(407, 154), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 6, kPuzzle120012, "a12s04n02p02l07.tga", Common::Point(234, 327), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 7, kPuzzle120012, "a12s04n02p02l08.tga", Common::Point(147, 154), true, kDrawType3, 1100);
	_app->objectAddPresentation(kObject120060);
	_app->objectPresentationAddImageToPuzzle(kObject120060, 8, kPuzzle120012, "a12s04n02p02l09.bmp", Common::Point(234, 154), true, kDrawType1, 1200);
	_app->objectPresentationShow(kObject120060, 0);
	_app->objectPresentationShow(kObject120060, 1);
	_app->objectPresentationShow(kObject120060, 2);
	_app->objectPresentationShow(kObject120060, 3);
	_app->objectAdd(kObject120061, "", "", 1);
	_app->objectAddPresentation(kObject120061);
	_app->objectPresentationAddAnimationToRotation(kObject120061, 0, 120603, 0, 9, 8.0f, 6);
	_app->soundAdd(120000, kSoundTypeBackgroundMusic, "1999.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(120000, 90);
	_app->soundAdd(120048, kSoundTypeEffect, "2000.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120049, kSoundTypeEffect, "2001.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120072, kSoundTypeEffect, "2002.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120068, kSoundTypeEffect, "2003.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120041, kSoundTypeEffect, "2004.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120046, kSoundTypeEffect, "2005.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120071, kSoundTypeEffect, "2006.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120038, kSoundTypeEffect, "2007.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120022, kSoundTypeEffect, "2008.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120064, kSoundTypeEffect, "2009.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120063, kSoundTypeEffect, "2010.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120040, kSoundTypeEffect, "2011.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120021, kSoundTypeEffect, "2012.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120023, kSoundTypeEffect, "2013.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120024, kSoundTypeEffect, "2014.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120025, kSoundTypeEffect, "2015.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120026, kSoundTypeEffect, "2016.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(120026, 90);
	_app->soundAdd(120027, kSoundTypeEffect, "2017.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120028, kSoundTypeEffect, "2018.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120029, kSoundTypeEffect, "2019.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120030, kSoundTypeEffect, "2020.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120032, kSoundTypeEffect, "2021.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120033, kSoundTypeEffect, "2022.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120034, kSoundTypeEffect, "2023.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120035, kSoundTypeEffect, "2024.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120036, kSoundTypeEffect, "2025.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120037, kSoundTypeEffect, "2026.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120039, kSoundTypeEffect, "2027.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120042, kSoundTypeEffect, "2028.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120043, kSoundTypeEffect, "2029.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120044, kSoundTypeEffect, "2030.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120045, kSoundTypeEffect, "2031.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120047, kSoundTypeEffect, "2032.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120051, kSoundTypeEffect, "2033.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120052, kSoundTypeEffect, "2034.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120053, kSoundTypeEffect, "2035.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120054, kSoundTypeEffect, "2036.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120055, kSoundTypeEffect, "2037.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120060, kSoundTypeEffect, "2038.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120061, kSoundTypeEffect, "2039.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120065, kSoundTypeEffect, "2040.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120066, kSoundTypeEffect, "2041.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120069, kSoundTypeEffect, "2042.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120070, kSoundTypeEffect, "2043.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120073, kSoundTypeEffect, "2044.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120074, kSoundTypeEffect, "2045.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120075, kSoundTypeEffect, "2046.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120078, kSoundTypeEffect, "2047.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120079, kSoundTypeEffect, "2048.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120050, kSoundTypeAmbientEffect, "2049.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(120201, 120050, 1, 1, 15, 75, 319.0f, 10);
	_app->rotationAdd3DSound(120202, 120050, 1, 1, 15, 70, 336.0f, 10);
	_app->rotationAdd3DSound(120203, 120050, 1, 1, 15, 70, 5.0f, 10);
	_app->rotationAdd3DSound(120204, 120050, 1, 1, 15, 65, 0.0f, 10);
	_app->rotationAdd3DSound(120501, 120050, 1, 1, 15, 90, 4.0f, 10);
	_app->puzzleAdd3DSound(120015, 120050, 1, 1, 15, 90, 4.0f, 10);
	_app->puzzleAdd3DSound(120014, 120050, 1, 1, 15, 70, 270.0f, 10);
	_app->puzzleAdd3DSound(120011, 120050, 1, 1, 15, 70, 270.0f, 10);
	_app->soundAdd(120018, kSoundTypeAmbientEffect, "2050.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120058, kSoundTypeAmbientEffect, "2051.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(120058, 80);
	_app->soundAdd(120056, kSoundTypeAmbientEffect, "2052.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(120057, kSoundTypeAmbientEffect, "2053.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(120057, 90);
	_app->soundAdd(120059, kSoundTypeAmbientEffect, "2054.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(120006, kSoundTypeAmbientEffect, "2055.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120008, kSoundTypeAmbientEffect, "2056.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120009, kSoundTypeAmbientEffect, "2057.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120010, kSoundTypeAmbientEffect, "2058.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120011, kSoundTypeAmbientEffect, "2059.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120012, kSoundTypeAmbientEffect, "2060.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120031, kSoundTypeAmbientEffect, "2061.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(120144, kSoundTypeAmbientEffect, "2062.WAC", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120003, kSoundTypeAmbientEffect, "2063.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120004, kSoundTypeAmbientEffect, "2064.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120005, kSoundTypeAmbientEffect, "2065.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120007, kSoundTypeAmbientEffect, "2066.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120013, kSoundTypeAmbientEffect, "2067.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120014, kSoundTypeAmbientEffect, "2068.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120015, kSoundTypeAmbientEffect, "2069.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120017, kSoundTypeAmbientEffect, "2070.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120019, kSoundTypeAmbientEffect, "2071.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(120020, kSoundTypeAmbientEffect, "2072.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAdd3DSound(120301, 120031, 1, 1, 15, 95, 57.0f, 10);
	_app->rotationAdd3DSound(120203, 120031, 1, 1, 15, 80, 177.0f, 10);
	_app->rotationAdd3DSound(120202, 120031, 1, 1, 15, 80, 234.0f, 10);
	_app->rotationAdd3DSound(120201, 120031, 1, 1, 15, 75, 234.0f, 10);
	_app->rotationAdd3DSound(120204, 120031, 1, 1, 15, 70, 234.0f, 10);
	_app->puzzleAdd3DSound(120014, 120031, 1, 1, 15, 80, 90.0f, 10);
	_app->puzzleAdd3DSound(120011, 120031, 1, 1, 15, 80, 90.0f, 10);
	_app->soundAdd(120067, kSoundTypeAmbientEffect, "2073.wav", _app->getConfiguration().backgroundMusic.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAdd3DSound(120603, 120067, 1, 1, 15, 100, 6.0f, 10);
	_app->rotationSet3DSoundOff(120603, 120067);
	_app->soundAdd(120016, kSoundTypeAmbientMusic, "2074.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAddAmbientSound(120101, 120016, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120102, 120016, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120110, 120016, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120103, 120016, 80, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120104, 120016, 70, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120106, 120016, 75, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120105, 120016, 80, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120107, 120016, 70, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120108, 120016, 75, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(120109, 120016, 80, 0, 1, 1, 10);
	_app->soundAdd(120101, kSoundTypeDialog, "2075.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120102, kSoundTypeDialog, "2076.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120103, kSoundTypeDialog, "2077.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120108, kSoundTypeDialog, "2078.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120110, kSoundTypeDialog, "2079.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120111, kSoundTypeDialog, "2080.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120115, kSoundTypeDialog, "2082.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120117, kSoundTypeDialog, "2083.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120118, kSoundTypeDialog, "2084.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120120, kSoundTypeDialog, "2085.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120121, kSoundTypeDialog, "2086.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120123, kSoundTypeDialog, "2087.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120124, kSoundTypeDialog, "2088.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120125, kSoundTypeDialog, "2089.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120126, kSoundTypeDialog, "2090.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120130, kSoundTypeDialog, "2094.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120131, kSoundTypeDialog, "2095.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120132, kSoundTypeDialog, "2096.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120133, kSoundTypeDialog, "2097.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120134, kSoundTypeDialog, "2098.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120135, kSoundTypeDialog, "2099.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120136, kSoundTypeDialog, "2100.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120137, kSoundTypeDialog, "2101.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120138, kSoundTypeDialog, "2102.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120139, kSoundTypeDialog, "2103.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120140, kSoundTypeDialog, "2104.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120141, kSoundTypeDialog, "2105.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120142, kSoundTypeDialog, "2106.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120145, kSoundTypeDialog, "2107.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120147, kSoundTypeDialog, "2108.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120148, kSoundTypeDialog, "2109.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120149, kSoundTypeDialog, "2110.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120150, kSoundTypeDialog, "2111.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120151, kSoundTypeDialog, "2112.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120146, kSoundTypeDialog, "2114.WAV", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120122, kSoundTypeDialog, "2115.WAV", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120109, kSoundTypeDialog, "2116.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120114, kSoundTypeDialog, "2117.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120116, kSoundTypeDialog, "2118.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120152, kSoundTypeDialog, "2119.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120153, kSoundTypeDialog, "2120.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120154, kSoundTypeDialog, "2121.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120155, kSoundTypeDialog, "2122.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120156, kSoundTypeDialog, "2123.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120157, kSoundTypeDialog, "2124.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120158, kSoundTypeDialog, "2125.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120159, kSoundTypeDialog, "2126.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(120160, kSoundTypeDialog, "2127.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineByte(120001, 0);
	_app->varDefineByte(120002, 0);
	_app->varDefineByte(120003, 0);
	_app->varDefineByte(120004, 0);
	_app->varDefineByte(120005, 0);
	_app->varDefineByte(120006, 1);
	_app->varDefineByte(120007, 0);
	_app->varDefineByte(120008, 0);
	_app->varDefineByte(120041, 0);
	_app->varDefineByte(120042, 0);
	_app->varDefineByte(120043, 0);
	_app->varDefineByte(120010, 0);
	_app->varDefineByte(120011, 0);
	_app->varDefineByte(120012, 0);
	_app->varDefineByte(120013, 0);
	_app->varDefineByte(120014, 0);
	_app->varDefineByte(120015, 0);
	_app->varDefineByte(120016, 0);
	_app->varDefineByte(120017, 0);
	_app->varDefineByte(120018, 0);
	_app->varDefineByte(120019, 0);
	_app->varDefineByte(120020, 0);
	_app->varDefineByte(120021, 0);
	_app->varDefineByte(120022, 0);
	_app->varDefineByte(120023, 0);
	_app->varDefineByte(120024, 0);
	_app->varDefineByte(120025, 0);
	_app->varDefineByte(120026, 0);
	_app->varDefineByte(120027, 0);
	_app->varDefineByte(120028, 0);
	_app->varDefineByte(120029, 0);
	_app->varDefineByte(120030, 0);
	_app->varDefineByte(120031, 0);
	_app->varDefineByte(120032, 0);
	_app->varDefineByte(120033, 0);
	_app->varDefineByte(120034, 0);
	_app->varDefineByte(120035, 0);
	_app->varDefineByte(120036, 1);
	_app->varDefineByte(120037, 0);
	_app->varDefineByte(120038, 0);
	_app->varDefineByte(120039, 0);
	_app->varDefineByte(120040, 0);
	_app->varDefineWord(120001, 130);
	_app->varDefineWord(120002, 130);
	_app->varDefineWord(120003, 0);
	_app->varDefineWord(120004, 0);
	_app->varDefineWord(120005, 0);
	_app->varDefineWord(120006, 0);
	_app->varDefineWord(120007, 0);
	_app->varDefineWord(120008, 0);
	_app->varDefineDword(120006, 0);
	_app->varDefineDword(120007, 0);
	_app->varDefineString(120001, "00000000");
}

void Zone15Faust::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->objectPresentationHide(kObject120003, 1);
		_app->timerStop(kTimer0);
		break;

	case kTimer1:
		if (_app->varGetWord(120001) != 400 || _app->varGetByte(97001) != 1) {
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject6, 1, Common::Point(_app->varGetWord(120001), 16));
			_app->varSetWord(120001, _app->varGetWord(120001) + 1);
		} else {
			if (_app->varGetWord(120002) == 400) {
				_app->varSetWord(120001, 401);
				_app->objectPresentationHide(kObject6, 5);
				_app->objectPresentationShow(kObject6, 3);
				_app->objectPresentationHide(kObject6, 4);
			}
		}

		if (_app->varGetWord(120001) == 400
		 && _app->varGetByte(97001) == 0) {
			_app->objectPresentationShow(kObject6, 3);
			_app->objectPresentationHide(kObject6, 4);
		}

		if (_app->varGetWord(120001) == 440) {
			if (_app->varGetByte(120003)) {
				_app->objectSetAccessibilityOn(kObject120001, 1, 1);
				_app->rotationSetMovabilityOn(120104, 3, 3);
				_app->rotationSetMovabilityOn(120106, 1, 1);
				_app->rotationSetMovabilityOn(120108, 3, 3);
				_app->varSetByte(98088, 0);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer2);
				_app->rotationSetActive(120401);
			} else {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer2);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);

				Id rotationId = _app->getCurrentRotationId();

				_app->varSetDword(120007, rotationId);
				_app->varSetWord(120006, (int16)_app->rotationGetAlp(rotationId));
				_app->varSetWord(120007, (int16)_app->rotationGetBet(rotationId));
				_app->varSetWord(120008, (int16)_app->rotationGetRan(rotationId));

				_app->objectSetAccessibilityOn(kObject1);
				_app->fadeOut(15, Color(0, 0, 0), 0);
				_app->objectPresentationShow(kObject15);
				_app->soundPlay(120150);
			}
		}
		break;

	case kTimer2:
		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject6, 2, Common::Point(_app->varGetWord(120002), 46));
		_app->varSetWord(120002, _app->varGetWord(120002) + 1);
		break;

	case kTimer3:
		_app->timerStop(kTimer3);
		_app->objectPresentationShow(kObject120054, 0);
		_app->soundPlay(120072);
		break;

	case kTimer4: {
		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, rnd(3000) + 2000);

		Id soundId = 120003 + rnd(11);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer5: {
		_app->timerStop(kTimer5);
		_app->timerStart(kTimer5, rnd(3000) + 3000);

		Id soundId = 120013 + rnd(4);
		_app->soundSetPan(soundId, rnd(10) - 10);
		_app->soundPlay(soundId);
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);
		_app->timerStart(kTimer6, rnd(3000) + 3000);
		_app->objectPresentationShow(kObject120061);
		_app->soundPlay(120067);
		break;

	case kTimer7:
		_app->timerStop(kTimer7);
		_app->varSetByte(120035, 0);
		_app->objectPresentationHide(kObject120054, 1);
		_app->objectSetAccessibilityOn(kObject120054, 2, 2);
		_app->soundStop(120071, 1024);
		break;
	}
}

void Zone15Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone15Faust::onButtonUp] Not implemented");
}

void Zone15Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone15] Not implemented");
}

void Zone15Faust::onSound(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 120101:
	case 120102:
	case 120110:
	case 120120:
	case 120130:
	case 120131:
	case 120132:
	case 120133:
	case 120134:
	case 120136:
	case 120137:
	case 120138:
	case 120145:
	case 120146:
	case 120147:
		_app->soundPlay(id + 1);
		break;

	case 120022:
		if (_app->bagHasClickedObject()
		 && _app->bagGetClickedObject() == kObjectMatches4)
			_app->cursorDelete();
		break;

	case 120139:
		_app->playMovie("2148");
		_app->displayFade("A12S06N02R01_FADE_02.bmp", "A12S06N02R01_FADE_01.bmp", 12, 0, kLoadFromCd, kArchiveArt);
		_app->rotationSetAlp(120602, 98.0f);
		_app->rotationSetBet(120602, 16.0f);
		_app->rotationSetRan(120602, 71.0f);
		_app->rotationSetActive(120602);
		_app->rotationSetRolTo(120602, 98.0f, 16.0f, 87.0f);
		break;

	case 120150:
		_app->objectPresentationHide(kObject15);
		_app->objectSetAccessibilityOff(kObject1);
		_app->rotationSetAlp(_app->varGetDword(120007), _app->varGetWord(120006));
		_app->rotationSetBet(_app->varGetDword(120007), _app->varGetWord(120007));
		_app->rotationSetRan(_app->varGetDword(120007), _app->varGetWord(120008));
		_app->rotationSetActive(_app->varGetDword(120007));

		_app->varSetWord(120001, 130);
		_app->varSetWord(120002, 130);
		_app->varSetByte(98088, 1);
		_app->objectPresentationShow(kObject6, 0);
		_app->objectPresentationShow(kObject6, 1);
		_app->objectPresentationShow(kObject6, 2);
		_app->timerStart(kTimer1, 2090);
		_app->timerStart(kTimer2, 3136);
		_app->objectSetAccessibilityOn(kObject6);
		break;
	}
}

void Zone15Faust::onUpdateBag(const Common::Point &point) {
	_app->soundStop(_app->varGetDword(120006), 1024);
	_app->varSetDword(120006, 0);

	_app->objectPresentationHide(kObject120058);
}

void Zone15Faust::onBagClickedObject(ObjectId id) {
	if (id == kObjectMatches4)
		_app->soundPlay(120022);
}

void Zone15Faust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
	error("[Zone15Faust::onUpdateBefore] Not implemented");
}

void Zone15Faust::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityPuzzleToRotation) {
		if (movabilityFrom == 120006 && movabilityTo == 120402) {
			if (_app->varGetByte(120013) == 1 && _app->varGetByte(120003) == 1) {
				_app->fadeOut(15, Color(0, 0, 0), 0);
				_app->displayFade("a12_black.bmp", "a12s01_e04m10_S.bmp", 12, 0, kLoadFromCd, kArchiveArt);
				_app->playMovie("2130");
				_app->displayFade("a12s01_e04m10_E.bmp", "a12_black.bmp", 12, 0, kLoadFromCd, kArchiveArt);
				_app->varSetByte(98088, 0);
				_app->objectSetAccessibilityOff(kObject6);
				_app->objectPresentationHideAndRemove(kObject6);
				_app->exitToMenu(kMenuAction0);
			}
		}
	}
}

void Zone15Faust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone15] Not implemented");
}

} // End of namespace Ring
