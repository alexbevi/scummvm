/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 8
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 08110-1301, USA.
 */

#include "ring/game/faust/faust_zone8.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone8Faust::Zone8Faust(ApplicationFaust *application) : _app(application) {
}

Zone8Faust::~Zone8Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone8Faust::onInit() {
	_app->setCurrentZone(kZone8);

	_app->puzzleAdd(kPuzzle52111);
	_app->puzzleAddBackgroundImage(kPuzzle52111, "a05s02n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle52112);
	_app->puzzleAddBackgroundImage(kPuzzle52112, "a05s02n01p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle52113);
	_app->puzzleAddBackgroundImage(kPuzzle52113, "a05s02n01p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle52114);
	_app->puzzleAddBackgroundImage(kPuzzle52114, "a05s02n01p04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle52210);
	_app->puzzleAddBackgroundImage(kPuzzle52210, "a05s02n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle52220);
	_app->puzzleAddBackgroundImage(kPuzzle52220, "a05s02n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle52230);
	_app->puzzleAddBackgroundImage(kPuzzle52230, "a05s02n02p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53101);
	_app->puzzleAddBackgroundImage(kPuzzle53101, "a05s03n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53109);
	_app->puzzleAddBackgroundImage(kPuzzle53109, "a05s03n01p09.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53102);
	_app->puzzleAddBackgroundImage(kPuzzle53102, "a05s03n01p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53103);
	_app->puzzleAddBackgroundImage(kPuzzle53103, "a05s03n01p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53104);
	_app->puzzleAddBackgroundImage(kPuzzle53104, "a05s03n01p04.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53105);
	_app->puzzleAddBackgroundImage(kPuzzle53105, "a05s03n01p05.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53106);
	_app->puzzleAddBackgroundImage(kPuzzle53106, "a05s03n01p06.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53107);
	_app->puzzleAddBackgroundImage(kPuzzle53107, "a05s03n01p07.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53108);
	_app->puzzleAddBackgroundImage(kPuzzle53108, "a05s03n01p08.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53110);
	_app->puzzleAddBackgroundImage(kPuzzle53110, "a05s03n01p10.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53210);
	_app->puzzleAddBackgroundImage(kPuzzle53210, "a05s03n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle53220);
	_app->puzzleAddBackgroundImage(kPuzzle53220, "a05s03n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle54110);
	_app->puzzleAddBackgroundImage(kPuzzle54110, "a05s04n01p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle54120);
	_app->puzzleAddBackgroundImage(kPuzzle54120, "a05s04n01p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle54210);
	_app->puzzleAddBackgroundImage(kPuzzle54210, "a05s04n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle54220);
	_app->puzzleAddBackgroundImage(kPuzzle54220, "a05s04n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle54230);
	_app->puzzleAddBackgroundImage(kPuzzle54230, "a05s04n02p03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle55210);
	_app->puzzleAddBackgroundImage(kPuzzle55210, "a15s01n02p01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle55220);
	_app->puzzleAddBackgroundImage(kPuzzle55220, "a15s01n02p02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle56000);
	_app->puzzleAddBackgroundImage(kPuzzle56000, "conclusion.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(51011, "1399", 0, 0);
	_app->rotationAdd(51021, "1400", 0, 0);
	_app->rotationAdd(51031, "1401", 0, 0);
	_app->rotationAdd(51041, "1402", 0, 0);
	_app->rotationAdd(52011, "1403", 0, 0);
	_app->rotationAdd(52021, "1404", 0, 1);
	_app->rotationAdd(53011, "1405", 0, 8);
	_app->rotationAdd(53021, "1406", 0, 4);
	_app->rotationSetCompressionBufferLength(53021, 1200000);
	_app->rotationAdd(54011, "1407", 0, 1);
	_app->rotationAdd(54021, "1408", 0, 5);
	_app->rotationAdd(55011, "1409", 0, 0);
	_app->rotationAdd(55021, "1410", 0, 0);
	_app->rotationAdd(55031, "1411", 0, 0);
	_app->rotationAddMovabilityToRotation(51011, 51021, "", Common::Rect(284, -280, 710, 47), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(51011, 0, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(51011, 51031, "", Common::Rect(1193, -120, 1495, 134), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(51011, 1, 94.0f, 0.0f, 87.0f, 0.0f, 2, 142.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(51021, 51011, "", Common::Rect(1981, -31, 2569, 531), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(51021, 0, 180.0f, 26.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(51021, 52011, "", Common::Rect(326, -125, 622, 265), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(51021, 1, 0.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(51031, 51011, "", Common::Rect(3227, -80, 3420, 153), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(51031, 0, 289.0f, 0.0f, 87.0f, 0.0f, 2, 326.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(51031, 51041, "", Common::Rect(1497, -195, 1705, 45), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(51031, 1, 117.0f, -2.0f, 87.0f, 0.0f, 2, 88.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(51041, 51031, "", Common::Rect(3369, 17, 3564, 204), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(51041, 0, 299.0f, 6.0f, 87.0f, 0.0f, 2, 298.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(52011, 51021, "", Common::Rect(460, -233, 822, 347), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(52011, 0, 19.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 7.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(52011, 52021, "", Common::Rect(3164, -134, 3434, 293), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(52011, 1, 283.0f, 0.0f, 87.0f, 0.0f, 2, 227.0f, 20.0f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(52011, kPuzzle52111, "", Common::Rect(3408, -82, 3592, 210), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(52011, 2, 0.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(52011, kPuzzle52112, "", Common::Rect(247, -52, 411, 154), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(52011, 3, 0.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(52011, kPuzzle52113, "", Common::Rect(1318, -148, 1589, 189), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(52011, 4, 0.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(52011, kPuzzle52114, "", Common::Rect(1729, -147, 1944, 198), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(52011, 5, 0.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(52021, 52011, "", Common::Rect(1416, 40, 1710, 467), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(52021, 0, 112.0f, 20.0f, 87.0f, 0.0f, 2, 115.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(52021, kPuzzle52230, "", Common::Rect(2316, 184, 2429, 322), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(52021, 1, 0.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(52021, kPuzzle52220, "", Common::Rect(2316, 381, 2439, 437), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(52021, 2, 194.0f, 27.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(53011, 53021, "", Common::Rect(1636, -29, 1942, 348), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(53011, 0, 124.0f, 0.0f, 87.0f, 0.0f, 2, 124.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(53011, kPuzzle53101, "", Common::Rect(2779, 195, 3032, 420), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53011, 1, 255.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(53011, kPuzzle53104, "", Common::Rect(242, 83, 488, 287), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53011, 2, 34.0f, 20.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(53011, kPuzzle53105, "", Common::Rect(662, 68, 808, 240), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53011, 3, 34.0f, 20.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(53011, kPuzzle53102, "", Common::Rect(3294, 196, 3526, 441), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53011, 4, 289.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(53011, kPuzzle53109, "", Common::Rect(2300, 122, 2571, 327), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53011, 5, 200.0f, 28.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(53011, kPuzzle53110, "", Common::Rect(759, -59, 1158, 275), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53011, 6, 43.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(53021, 52021, "", Common::Rect(2316, -378, 2615, -57), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(53021, 0, 0.0f, 0.0f, 87.0f, 0.0f, 2, 293.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(53021, 53011, "", Common::Rect(3271, -115, 3600, 350), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(53021, 1, 305.0f, 0.0f, 87.0f, 0.0f, 2, 305.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(53021, 53011, "", Common::Rect(0, -115, 116, 350), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(53021, 2, 305.0f, 0.0f, 87.0f, 0.0f, 2, 305.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(53021, 54011, "", Common::Rect(-1, -1, 0, 0), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(53021, 3, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(53021, kPuzzle53210, "", Common::Rect(2181, 247, 2325, 491), false, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53021, 4, 158.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToPuzzle(53021, kPuzzle53210, "", Common::Rect(2181, 247, 2325, 491), false, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(53021, 5, 158.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(54011, 54021, "", Common::Rect(40, -41, 390, 368), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(54011, 0, 323.0f, 0.0f, 87.0f, 0.0f, 2, 323.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(54021, 54011, "", Common::Rect(1879, -48, 2075, 375), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(54021, 0, 158.0f, 0.0f, 87.0f, 0.0f, 2, 158.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(54021, kPuzzle54210, "", Common::Rect(3111, 341, 3280, 467), false, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(54021, 1, 280.0f, 26.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(54021, 53021, "", Common::Rect(2453, -48, 2791, 378), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(54021, 2, 213.0f, 9.0f, 87.0f, 0.0f, 2, 305.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(55011, 55021, "", Common::Rect(3394, -106, 3600, 115), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(55011, 0, 303.0f, 0.0f, 87.0f, 0.0f, 2, 269.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(55011, 55021, "", Common::Rect(0, -106, 35, 115), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(55011, 1, 303.0f, 0.0f, 87.0f, 0.0f, 2, 269.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(55021, 55011, "", Common::Rect(1650, -87, 1859, 92), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(55021, 0, 114.0f, 0.0f, 87.0f, 0.0f, 2, 119.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(55021, 55031, "", Common::Rect(2916, -217, 3331, 273), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(55021, 1, 263.0f, 0.0f, 87.0f, 0.0f, 2, 269.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToPuzzle(55021, kPuzzle55210, "", Common::Rect(2510, -113, 2747, 201), true, kCursorZoom, 0);
	_app->rotationSetMovabilityToPuzzle(55021, 2, 0.0f, 0.0f, 87.0f, 0.0f, 2);
	_app->rotationAddMovabilityToRotation(55031, 55021, "", Common::Rect(1279, -152, 1492, 75), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(55031, 0, 91.0f, 0.0f, 87.0f, 0.0f, 2, 119.0f, 0.3f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle52111, 52011, "", Common::Rect(0, 17, 639, 459), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle52111, 0, 306.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle52112, 52011, "", Common::Rect(0, 17, 639, 459), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle52112, 0, 350.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle52113, 52011, "", Common::Rect(0, 17, 639, 459), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle52113, 0, 100.0f, -2.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle52114, 52011, "", Common::Rect(0, 17, 639, 459), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle52114, 0, 142.0f, -2.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle52210, 52021, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle52210, 0, 187.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle52230, 52021, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle52230, 0, 187.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle52220, 52021, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle52220, 0, 194.0f, 27.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle53101, 53011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle53101, 0, 255.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle53109, 53011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle53109, 0, 200.0f, 27.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle53110, 53011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle53110, 0, 43.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle53104, 53011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle53104, 0, 34.0f, 20.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle53105, 53011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle53105, 0, 354.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle53106, 53011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle53106, 0, 27.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle53106, kPuzzle53107, "", Common::Rect(242, 199, 363, 314), true, kCursorZoom, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle53107, kPuzzle53106, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle53102, 53011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle53102, 0, 289.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle53102, kPuzzle53103, "", Common::Rect(191, 79, 338, 188), false, kCursorZoom, 1);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle53210, kPuzzle53220, "", Common::Rect(15, 340, 149, 426), true, kCursorZoom, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle53220, kPuzzle53210, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle53103, kPuzzle53102, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle54110, 54011, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle54110, 0, 90.0f, 17.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle54120, kPuzzle54110, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle54210, 54021, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle54210, 0, 280.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle54210, kPuzzle54230, "", Common::Rect(182, 108, 418, 271), false, kCursorZoom, 1);
	_app->puzzleAddMovabilityToRotation(kPuzzle54220, 54021, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle54220, 0, 280.0f, 26.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle54230, kPuzzle54210, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle55210, 55021, "", Common::Rect(0, 357, 160, 463), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle55210, 0, 231.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToRotation(kPuzzle55210, 55021, "", Common::Rect(558, 21, 639, 463), true, kCursorBack, 1);
	_app->puzzleSetMovabilityToRotation(kPuzzle55210, 1, 231.0f, 0.0f, 87.0f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle55220, kPuzzle55210, "", Common::Rect(0, 420, 580, 464), true, kCursorBack, 0);
	_app->objectAdd(kObject55999, "", "", 1);
	_app->objectAddRotationAccessibility(kObject55999, 53021, Common::Rect(1854, 310, 2327, 534), false, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject55999, 54011, Common::Rect(2970, -54, 3320, 395), true, kCursorMove, 1);
	_app->objectAdd(kObject52211, "", "", 1);
	_app->objectAddPresentation(kObject52211);
	_app->objectPresentationAddImageToRotation(kObject52211, 0, 52021, 0);
	_app->objectPresentationShow(kObject52211, 0);
	_app->objectAdd(kObject52212, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject52212, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject52212, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject52212, kPuzzle52210, Common::Rect(234, 208, 403, 326), true, kCursorDragDrop, 0);

	for (uint32 i = 0; i < 19; i++) {
		_app->objectAddPresentation(kObject52212);
		_app->objectPresentationAddImageToPuzzle(kObject52212, i, kPuzzle52210, Common::String::format("a05s02n02p01s01.%04d.bmp", i + 1), Common::Point(18, 35), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject52231, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject52231, kPuzzle52230, Common::Rect(265, 147, 386, 200), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject52231);
	_app->objectPresentationAddAnimationToPuzzle(kObject52231, 0, kPuzzle52230, "a05s02n02p03s01", kImageTypeBMP, Common::Point(295, 293), kDrawType1, 1000, 10, 12.5f, 4);
	_app->objectAdd(kObject52221, "", "", 1);
	_app->objectAdd(kObjectBathtubKey, "Bathtub Key", "A05_Key", 1);
	_app->objectSetActiveCursor(kObjectBathtubKey, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBathtubKey, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject52221, kPuzzle52220, Common::Rect(258, 232, 362, 455), true, kCursorTake, 0);
	_app->objectAddPresentation(kObject52221);
	_app->objectPresentationAddImageToPuzzle(kObject52221, 0, kPuzzle52220, "a05s02n02p02l01.bmp", Common::Point(260, 233), true, kDrawType1, 1000);
	_app->objectAddRotationAccessibility(kObjectBathtubKey, 52021, Common::Rect(2543, -1, 2965, 319), true, kCursorActionObject, 0);
	_app->objectAdd(kObject53105, "", "", 1);
	_app->objectAdd(kObject53215, "", "", 1);
	_app->objectAddPresentation(kObject53105);
	_app->objectPresentationAddImageToRotation(kObject53105, 0, 53011, 5);
	_app->objectPresentationAddImageToRotation(kObject53105, 0, 53021, 0);
	_app->objectAddPresentation(kObject53215);
	_app->objectAddRotationAccessibility(kObject53215, 53021, Common::Rect(1970, -275, 2100, 106), true, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject53105);
	_app->objectPresentationAddImageToPuzzle(kObject53105, 0, kPuzzle53210, "a05s03n02p01l06.bmp", Common::Point(281, 16), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObject53105, 0);
	_app->objectAdd(kObject53100, "", "", 1);
	_app->objectAdd(kObjectSpoon, "Spoon", "A05_Spoon", 1);
	_app->objectSetActiveCursor(kObjectSpoon, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectSpoon, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject53100, kPuzzle53109, Common::Rect(348, 170, 509, 368), true, kCursorTake, 0);
	_app->objectAddPresentation(kObject53100);
	_app->objectPresentationAddImageToRotation(kObject53100, 0, 53011, 0);
	_app->objectPresentationAddImageToPuzzle(kObject53100, 0, kPuzzle53109, "a05s03n01p09l01.bmp", Common::Point(410, 166), true, kDrawType1, 1000);
	_app->objectAdd(kObject53101, "", "", 1);
	_app->objectAdd(kObjectApple, "Apple!", "A05_Marr", 1);
	_app->objectSetActiveCursor(kObjectApple, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectApple, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectCola, "Cola!", "A05_Mary", 1);
	_app->objectSetActiveCursor(kObjectCola, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectCola, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectMenthol, "Menthol!", "A05_Marg", 1);
	_app->objectSetActiveCursor(kObjectMenthol, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectMenthol, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectGrenadine, "Grenadine!", "A05_Marb", 1);
	_app->objectSetActiveCursor(kObjectGrenadine, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectGrenadine, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject53101);
	_app->objectPresentationAddImageToRotation(kObject53101, 0, 53011, 2);
	_app->objectPresentationAddImageToRotation(kObject53101, 0, 53021, 2);
	_app->objectAddRotationAccessibility(kObject53101, 53011, Common::Rect(2557, 165, 2743, 322), true, kCursorTake, 0);
	_app->objectAdd(kObject53102, "", "", 1);
	_app->objectAdd(kObjectTeddyBear, "Teddy Bear", "A05_Bear", 1);
	_app->objectSetActiveCursor(kObjectTeddyBear, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectTeddyBear, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject53102);
	_app->objectPresentationAddImageToRotation(kObject53102, 0, 53011, 3);
	_app->objectAddRotationAccessibility(kObject53102, 53011, Common::Rect(2524, 444, 2852, 586), true, kCursorTake, 0);
	_app->objectAdd(kObject531022, "", "", 1);
	_app->objectAdd(kObject531023, "", "", 1);
	_app->objectAddPresentation(kObject531022);
	_app->objectPresentationAddImageToRotation(kObject531022, 0, 53011, 4);
	_app->objectPresentationAddImageToRotation(kObject531022, 0, 53021, 1);
	_app->objectAddPuzzleAccessibility(kObject531022, kPuzzle53110, Common::Rect(197, 107, 410, 461), true, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject531022);
	_app->objectPresentationAddImageToPuzzle(kObject531022, 0, kPuzzle53110, "a05s03n01p10l01.bmp", Common::Point(122, 76), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject531023);
	_app->objectAddPuzzleAccessibility(kObject531023, kPuzzle53110, Common::Rect(141, 256, 204, 339), false, kCursorAction, 0);
	_app->objectAdd(kObject53111, "", "", 1);
	_app->objectAdd(kObjectKey2, "Key", "A05_Key", 1);
	_app->objectSetActiveCursor(kObjectKey2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectKey2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject53111, kPuzzle53101, Common::Rect(283, 123, 352, 176), true, kCursorTake, 0);
	_app->objectAddPresentation(kObject53111);
	_app->objectPresentationAddImageToPuzzle(kObject53111, 0, kPuzzle53101, "a05s03n01p01l01.bmp", Common::Point(290, 127), true, kDrawType1, 1000);
	_app->objectAdd(kObject53205, "", "", 1);
	_app->objectAddPresentation(kObject53205);
	_app->objectPresentationAddImageToRotation(kObject53205, 0, 53011, 6);
	_app->objectPresentationAddImageToRotation(kObject53205, 0, 53021, 3);
	_app->objectAddRotationAccessibility(kObject53205, 53021, Common::Rect(2237, 244, 2404, 357), true, kCursorActionObject, 0);
	_app->objectAdd(kObject53210, "", "", 1);
	_app->objectAddPresentation(kObject53210);
	_app->objectAddPuzzleAccessibility(kObject53210, kPuzzle53210, Common::Rect(174, 410, 639, 463), true, kCursorBack, 0);
	_app->objectAdd(kObject53211, "", "", 1);
	_app->objectAdd(kObject532111, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject53211, kPuzzle53210, Common::Rect(188, 272, 485, 383), true, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject532111);
	_app->objectPresentationAddImageToPuzzle(kObject532111, 0, kPuzzle53210, "a05s03n02p01l01.bmp", Common::Point(167, 266), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject53211);
	_app->objectPresentationAddAnimationToPuzzle(kObject53211, 0, kPuzzle53210, "a05s03n02p01s01", kImageTypeBMP, Common::Point(167, 266), kDrawType1, 1000, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53211, 0, 0, 53211);
	_app->objectAddPresentation(kObject53211);
	_app->objectPresentationAddAnimationToPuzzle(kObject53211, 1, kPuzzle53210, "a05s03n02p01s01", kImageTypeBMP, Common::Point(167, 266), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53211, 1, 0, 53212);
	_app->objectAddPresentation(kObject53211);
	_app->objectPresentationAddAnimationToPuzzle(kObject53211, 2, kPuzzle53210, "a05s03n02p01s01", kImageTypeBMP, Common::Point(167, 266), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53211, 2, 0, 53213);
	_app->objectAddPresentation(kObject53211);
	_app->objectPresentationAddAnimationToPuzzle(kObject53211, 3, kPuzzle53210, "a05s03n02p01s01", kImageTypeBMP, Common::Point(167, 266), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53211, 3, 0, 53214);
	_app->objectAddPresentation(kObject53211);
	_app->objectPresentationAddAnimationToPuzzle(kObject53211, 4, kPuzzle53210, "a05s03n02p01s01", kImageTypeBMP, Common::Point(167, 266), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53211, 4, 0, 53215);
	_app->objectAdd(kObject53212, "", "", 1);
	_app->objectAdd(kObject532121, "", "", 1);
	_app->objectAdd(kObjectFrozenPaper, "Frozen Paper", "A05_PactFrozen", 1);
	_app->objectSetActiveCursor(kObjectFrozenPaper, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectFrozenPaper, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject532123, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject53212, kPuzzle53210, Common::Rect(286, 182, 392, 218), true, kCursorAction, 0);
	_app->objectAddPresentation(kObjectFrozenPaper);
	_app->objectAddPuzzleAccessibility(kObjectFrozenPaper, kPuzzle53210, Common::Rect(330, 187, 461, 257), false, kCursorAction, 0);
	_app->objectAddPresentation(kObject532123);
	_app->objectAddPuzzleAccessibility(kObject532123, kPuzzle53210, Common::Rect(286, 323, 384, 346), false, kCursorAction, 0);
	_app->objectAddPresentation(kObject532121);
	_app->objectPresentationAddImageToPuzzle(kObject532121, 0, kPuzzle53210, "a05s03n02p01l02.bmp", Common::Point(147, 176), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532121);
	_app->objectPresentationAddImageToPuzzle(kObject532121, 1, kPuzzle53210, "a05s03n02p01l03.bmp", Common::Point(148, 175), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject53212);
	_app->objectPresentationAddAnimationToPuzzle(kObject53212, 0, kPuzzle53210, "a05s03n02p01s02", kImageTypeBMP, Common::Point(147, 176), kDrawType1, 1000, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53212, 0, 0, 53221);
	_app->objectAddPresentation(kObject53212);
	_app->objectPresentationAddAnimationToPuzzle(kObject53212, 1, kPuzzle53210, "a05s03n02p01s02", kImageTypeBMP, Common::Point(147, 176), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53212, 1, 0, 53222);
	_app->objectAddPresentation(kObject53212);
	_app->objectPresentationAddAnimationToPuzzle(kObject53212, 2, kPuzzle53210, "a05s03n02p01s03", kImageTypeBMP, Common::Point(148, 175), kDrawType1, 1000, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53212, 2, 0, 53223);
	_app->objectAddPresentation(kObject53212);
	_app->objectPresentationAddAnimationToPuzzle(kObject53212, 3, kPuzzle53210, "a05s03n02p01s03", kImageTypeBMP, Common::Point(148, 175), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53212, 3, 0, 53224);
	_app->objectAdd(kObject53122, "", "", 1);
	_app->objectAdd(kObject53121, "", "", 1);
	_app->objectAdd(kObject53123, "", "", 1);
	_app->objectAdd(kObject53124, "", "", 1);
	_app->objectAdd(kObjectMatches2, "Matches", "A05_Matches", 1);
	_app->objectSetActiveCursor(kObjectMatches2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectMatches2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObjectMatches2);
	_app->objectSetActiveCursor(kObjectMatches2, Common::Point(11, 27), 5, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMatches2, Common::Point(11, 27), 5, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectAddPresentation(kObject53122);
	_app->objectPresentationAddImageToRotation(kObject53122, 0, 53011, 7);
	_app->objectPresentationShow(kObject53122, 0);
	_app->objectAddPresentation(kObject53121);
	_app->objectAddPuzzleAccessibility(kObject53121, kPuzzle53102, Common::Rect(191, 79, 338, 188), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject53121);
	_app->objectPresentationAddImageToPuzzle(kObject53121, 0, kPuzzle53102, "a05s03n01p02l01.bmp", Common::Point(169, 65), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject53123);
	_app->objectAddPuzzleAccessibility(kObject53123, kPuzzle53102, Common::Rect(216, 330, 312, 418), true, kCursorAction, 0);
	_app->objectAddPuzzleAccessibility(kObject53123, kPuzzle53102, Common::Rect(237, 255, 277, 277), true, kCursorIdle, 1);
	_app->objectAddPresentation(kObject53123);
	_app->objectPresentationAddImageToPuzzle(kObject53123, 1, kPuzzle53102, "a05s03n01p02l02.bmp", Common::Point(229, 352), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject53123);
	_app->objectPresentationAddAnimationToPuzzle(kObject53123, 2, kPuzzle53102, "a05s03n01p02s02", kImageTypeBMP, Common::Point(189, 216), kDrawType1, 1000, 10, 12.5f, 4);
	_app->objectAddPresentation(kObject53121);
	_app->objectPresentationAddAnimationToPuzzle(kObject53121, 1, kPuzzle53102, "a05s03n01p02s01", kImageTypeBMP, Common::Point(169, 65), kDrawType1, 1000, 22, 3.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject53121, 1, 0, 53121);
	_app->objectAddPresentation(kObject53124);
	_app->objectPresentationAddImageToPuzzle(kObject53124, 0, kPuzzle53102, "a05s03n01p02l03.bmp", Common::Point(166, 61), true, kDrawType1, 1000);
	_app->objectAdd(kObject532124, "", "", 1);
	_app->objectAdd(kObject532125, "", "", 1);
	_app->objectAdd(kObjectChain, "Chain", "A05_Chain", 1);
	_app->objectSetActiveCursor(kObjectChain, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectChain, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject532127, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject532124, kPuzzle53210, Common::Rect(0, 212, 142, 346), true, kCursorAction, 0);
	_app->objectAddPresentation(kObjectChain);
	_app->objectAddPuzzleAccessibility(kObjectChain, kPuzzle53210, Common::Rect(0, 212, 142, 346), false, kCursorAction, 0);
	_app->objectAddPresentation(kObject532127);
	_app->objectAddPuzzleAccessibility(kObject532127, kPuzzle53210, Common::Rect(0, 212, 142, 346), false, kCursorAction, 0);
	_app->objectAddPresentation(kObject532125);
	_app->objectPresentationAddImageToPuzzle(kObject532125, 0, kPuzzle53210, "a05s03n02p01l04.bmp", Common::Point(0, 150), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532125);
	_app->objectPresentationAddImageToPuzzle(kObject532125, 1, kPuzzle53210, "a05s03n02p01l05.bmp", Common::Point(0, 143), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532124);
	_app->objectPresentationAddAnimationToPuzzle(kObject532124, 0, kPuzzle53210, "a05s03n02p01s04", kImageTypeBMP, Common::Point(0, 150), kDrawType1, 1000, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject532124, 0, 0, 53225);
	_app->objectAddPresentation(kObject532124);
	_app->objectPresentationAddAnimationToPuzzle(kObject532124, 1, kPuzzle53210, "a05s03n02p01s04", kImageTypeBMP, Common::Point(0, 150), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject532124, 1, 0, 53226);
	_app->objectAddPresentation(kObject532124);
	_app->objectPresentationAddAnimationToPuzzle(kObject532124, 2, kPuzzle53210, "a05s03n02p01s05", kImageTypeBMP, Common::Point(0, 143), kDrawType1, 1000, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject532124, 2, 0, 53227);
	_app->objectAddPresentation(kObject532124);
	_app->objectPresentationAddAnimationToPuzzle(kObject532124, 3, kPuzzle53210, "a05s03n02p01s05", kImageTypeBMP, Common::Point(0, 143), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject532124, 3, 0, 53228);
	_app->objectAdd(kObject53221, "", "", 1);
	_app->objectAdd(kObject53222, "", "", 1);
	_app->objectAdd(kObject53223, "", "", 1);
	_app->objectAdd(kObject53224, "", "", 1);
	_app->objectAdd(kObject532201, "", "", 1);
	_app->objectAdd(kObject532202, "", "", 1);
	_app->objectAdd(kObject532203, "", "", 1);
	_app->objectAdd(kObject532204, "", "", 1);
	_app->objectAdd(kObject532205, "", "", 1);
	_app->objectAdd(kObject532206, "", "", 1);
	_app->objectAdd(kObject532207, "", "", 1);
	_app->objectAdd(kObject532208, "", "", 1);
	_app->objectAdd(kObject532209, "", "", 1);
	_app->objectAdd(kObject532210, "", "", 1);
	_app->objectAdd(kObject532211, "", "", 1);
	_app->objectAdd(kObject532212, "", "", 1);
	_app->objectAdd(kObject532213, "", "", 1);
	_app->objectAdd(kObject532214, "", "", 1);
	_app->objectAdd(kObject532215, "", "", 1);
	_app->objectAdd(kObject532216, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject53221, kPuzzle53220, Common::Rect(113, 341, 199, 430), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject53222, kPuzzle53220, Common::Rect(214, 346, 289, 428), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject53223, kPuzzle53220, Common::Rect(302, 334, 381, 424), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject53224, kPuzzle53220, Common::Rect(416, 341, 510, 421), true, kCursorActionObject, 0);
	_app->objectAddPresentation(kObject532201);
	_app->objectPresentationAddImageToPuzzle(kObject532201, 0, kPuzzle53220, "a05s03n02p02l01.bmp", Common::Point(117, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532202);
	_app->objectPresentationAddImageToPuzzle(kObject532202, 0, kPuzzle53220, "a05s03n02p02l02.bmp", Common::Point(214, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532203);
	_app->objectPresentationAddImageToPuzzle(kObject532203, 0, kPuzzle53220, "a05s03n02p02l03.bmp", Common::Point(299, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532204);
	_app->objectPresentationAddImageToPuzzle(kObject532204, 0, kPuzzle53220, "a05s03n02p02l04.bmp", Common::Point(396, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532205);
	_app->objectPresentationAddImageToPuzzle(kObject532205, 0, kPuzzle53220, "a05s03n02p02l05.bmp", Common::Point(118, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532206);
	_app->objectPresentationAddImageToPuzzle(kObject532206, 0, kPuzzle53220, "a05s03n02p02l06.bmp", Common::Point(214, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532207);
	_app->objectPresentationAddImageToPuzzle(kObject532207, 0, kPuzzle53220, "a05s03n02p02l07.bmp", Common::Point(303, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532208);
	_app->objectPresentationAddImageToPuzzle(kObject532208, 0, kPuzzle53220, "a05s03n02p02l08.bmp", Common::Point(396, 354), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532209);
	_app->objectPresentationAddImageToPuzzle(kObject532209, 0, kPuzzle53220, "a05s03n02p02l09.bmp", Common::Point(118, 356), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532210);
	_app->objectPresentationAddImageToPuzzle(kObject532210, 0, kPuzzle53220, "a05s03n02p02l10.bmp", Common::Point(212, 353), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532211);
	_app->objectPresentationAddImageToPuzzle(kObject532211, 0, kPuzzle53220, "a05s03n02p02l11.bmp", Common::Point(298, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532212);
	_app->objectPresentationAddImageToPuzzle(kObject532212, 0, kPuzzle53220, "a05s03n02p02l12.bmp", Common::Point(394, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532213);
	_app->objectPresentationAddImageToPuzzle(kObject532213, 0, kPuzzle53220, "a05s03n02p02l13.bmp", Common::Point(119, 355), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532214);
	_app->objectPresentationAddImageToPuzzle(kObject532214, 0, kPuzzle53220, "a05s03n02p02l14.bmp", Common::Point(211, 353), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532215);
	_app->objectPresentationAddImageToPuzzle(kObject532215, 0, kPuzzle53220, "a05s03n02p02l15.bmp", Common::Point(302, 354), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject532216);
	_app->objectPresentationAddImageToPuzzle(kObject532216, 0, kPuzzle53220, "a05s03n02p02l16.bmp", Common::Point(395, 353), true, kDrawType1, 1000);
	_app->objectAdd(kObject53141, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject53141, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject53141, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject53141, kPuzzle53104, Common::Rect(303, 352, 395, 397), true, kCursorDragDrop, 0);

	for (uint32 i = 0; i < 50; i++) {
		_app->objectAddPresentation(kObject53141);
		_app->objectPresentationAddImageToPuzzle(kObject53141, i, kPuzzle53104, Common::String::format("a05s03n01p04s01.%04d.bmp", i + 1), Common::Point(181, 82), true, kDrawType1, 1000);
	}

	_app->objectAdd(kObject53152, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject53152, kPuzzle53105, Common::Rect(211, 209, 256, 353), true, kCursorAction, 0);
	_app->objectAdd(kObject53151, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject53151, kPuzzle53105, Common::Rect(119, 179, 203, 266), true, kCursorAction, 0);
	_app->objectAdd(kObject53153, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject53153, kPuzzle53105, Common::Rect(398, 189, 532, 363), true, kCursorAction, 0);
	_app->objectAdd(kObject53171, "", "", 1);
	_app->objectAddRotationAccessibility(kObject53171, 53011, Common::Rect(574, 265, 752, 369), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject53171);
	_app->objectPresentationAddAnimationToRotation(kObject53171, 0, 53011, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject53171, 0, 0, 53171);
	_app->objectAddPresentation(kObject53171);
	_app->objectPresentationAddAnimationToRotation(kObject53171, 1, 53011, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject53171, 1, 0, 53172);
	_app->objectAdd(kObject531777, "", "", 1);
	_app->objectAddPresentation(kObject531777);
	_app->objectAddRotationAccessibility(kObject531777, 53011, Common::Rect(1965, 124, 2195, 383), true, kCursorZoom, 0);
	_app->objectAdd(kObject532888, "", "", 1);
	_app->objectAddPresentation(kObject532888);
	_app->objectAddRotationAccessibility(kObject532888, 53021, Common::Rect(2450, 367, 3285, 618), true, kCursorZoom, 0);
	_app->objectAdd(kObject54101, "", "", 1);
	_app->objectAddRotationAccessibility(kObject54101, 54011, Common::Rect(1291, 64, 1458, 268), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject54101);
	_app->objectPresentationAddAnimationToRotation(kObject54101, 0, 54011, 0, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject54101, 0, 0, 54101);
	_app->objectAddPresentation(kObject54101);
	_app->objectPresentationAddAnimationToRotation(kObject54101, 1, 54011, 0, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject54101, 1, 0, 54102);
	_app->objectAdd(kObject54111, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject54111, kPuzzle54110, Common::Rect(234, 272, 449, 424), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject54111);
	_app->objectPresentationAddAnimationToPuzzle(kObject54111, 0, kPuzzle54110, "a05s04n01p01s01", kImageTypeBMP, Common::Point(240, 256), kDrawType1, 1000, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject54111, 0, 0, 54111);
	_app->objectAddPresentation(kObject54111);
	_app->objectPresentationAddAnimationToPuzzle(kObject54111, 1, kPuzzle54110, "a05s04n01p01s01", kImageTypeBMP, Common::Point(240, 256), kDrawType1, 1000, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnPuzzle(kObject54111, 1, 0, 54112);
	_app->objectAdd(kObject54201, "", "", 1);
	_app->objectAdd(kObject54202, "", "", 1);
	_app->objectAdd(kObjectPiecesOfPicture, "Pieces of Picture", "A05_Pieces", 1);
	_app->objectSetActiveCursor(kObjectPiecesOfPicture, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiecesOfPicture, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPresentation(kObject54201);
	_app->objectAddRotationAccessibility(kObject54201, 54021, Common::Rect(3336, 331, 3596, 481), true, kCursorAction, 0);
	_app->objectAddPresentation(kObject54202);
	_app->objectAddRotationAccessibility(kObject54202, 54021, Common::Rect(3422, 436, 3482, 490), false, kCursorAction, 0);
	_app->objectAddPresentation(kObjectPiecesOfPicture);
	_app->objectAddRotationAccessibility(kObjectPiecesOfPicture, 54021, Common::Rect(3369, 333, 3568, 448), false, kCursorTake, 0);
	_app->objectAddPresentation(kObject54202);
	_app->objectPresentationAddImageToRotation(kObject54202, 0, 54021, 0);
	_app->objectAddPresentation(kObject54202);
	_app->objectPresentationAddImageToRotation(kObject54202, 1, 54021, 2);
	_app->objectAddPresentation(kObject54201);
	_app->objectPresentationAddAnimationToRotation(kObject54201, 0, 54021, 1, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject54201, 0, 0, 54211);
	_app->objectAddPresentation(kObject54201);
	_app->objectPresentationAddAnimationToRotation(kObject54201, 1, 54021, 1, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject54201, 1, 0, 54212);
	_app->objectAddPresentation(kObject54201);
	_app->objectPresentationAddAnimationToRotation(kObject54201, 2, 54021, 4, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject54201, 2, 0, 54213);
	_app->objectAddPresentation(kObject54201);
	_app->objectPresentationAddAnimationToRotation(kObject54201, 3, 54021, 4, 10, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject54201, 3, 0, 54214);
	_app->objectAdd(kObject542201, "", "", 1);
	_app->objectAddPresentation(kObject542201);
	_app->objectPresentationAddImageToRotation(kObject542201, 0, 54021, 3);
	_app->objectAdd(kObject54204, "", "", 1);
	_app->objectAddPresentation(kObject54204);
	_app->objectAddRotationAccessibility(kObject54204, 54021, Common::Rect(3336, 251, 3600, 336), true, kCursorAction, 0);
	_app->objectAdd(kObject54210, "", "", 1);
	_app->objectAdd(kObject54220, "", "", 1);
	_app->objectAdd(kObjectPiece1, "Piece 1", "A05_Piece", 1);
	_app->objectSetActiveCursor(kObjectPiece1, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiece1, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectPiece2, "Piece 2", "A05_Piece", 1);
	_app->objectSetActiveCursor(kObjectPiece2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiece2, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectPiece3, "Piece 3", "A05_Piece", 1);
	_app->objectSetActiveCursor(kObjectPiece3, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiece3, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectPiece4, "Piece 4", "A05_Piece", 1);
	_app->objectSetActiveCursor(kObjectPiece4, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiece4, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectPiece5, "Piece 5", "A05_Piece", 1);
	_app->objectSetActiveCursor(kObjectPiece5, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiece5, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectPiece6, "Piece 6", "A05_Piece", 1);
	_app->objectSetActiveCursor(kObjectPiece6, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPiece6, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject542107, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject54210, kPuzzle54210, Common::Rect(112,  91, 284, 207), false, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject54210, kPuzzle54210, Common::Rect(112, 207, 284, 326), false, kCursorActionObject, 1);
	_app->objectAddPuzzleAccessibility(kObject54210, kPuzzle54210, Common::Rect(284, 326, 456, 446), false, kCursorActionObject, 2);
	_app->objectAddPuzzleAccessibility(kObject54210, kPuzzle54210, Common::Rect(284,  91, 456, 207), false, kCursorActionObject, 3);
	_app->objectAddPuzzleAccessibility(kObject54210, kPuzzle54210, Common::Rect(284, 207, 456, 326), false, kCursorActionObject, 4);
	_app->objectAddPuzzleAccessibility(kObject54210, kPuzzle54210, Common::Rect(112, 326, 284, 446), false, kCursorActionObject, 5);
	_app->objectAddPuzzleAccessibility(kObject54210, kPuzzle54210, Common::Rect(159, 119, 517, 373), true, kCursorAction, 6);
	_app->objectAddPresentation(kObject54220);
	_app->objectAddPuzzleAccessibility(kObject54220, kPuzzle54210, Common::Rect(246, 316, 415, 408), false, kCursorZoom, 7);
	_app->objectAddPresentation(kObject54210);
	_app->objectPresentationAddImageToPuzzle(kObject54210, 0, kPuzzle54210, "a05s04n02p01l01.0001.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject54210);
	_app->objectPresentationAddImageToPuzzle(kObject54210, 1, kPuzzle54210, "a05s04n02p01l01.0002.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject54210);
	_app->objectPresentationAddImageToPuzzle(kObject54210, 2, kPuzzle54210, "a05s04n02p01l01.0003.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject54210);
	_app->objectPresentationAddImageToPuzzle(kObject54210, 3, kPuzzle54210, "a05s04n02p01l01.0004.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject54210);
	_app->objectPresentationAddImageToPuzzle(kObject54210, 4, kPuzzle54210, "a05s04n02p01l01.0005.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject54210);
	_app->objectPresentationAddImageToPuzzle(kObject54210, 5, kPuzzle54210, "a05s04n02p01l01.0006.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject542107);
	_app->objectPresentationAddImageToPuzzle(kObject542107, 0, kPuzzle54210, "a05s04n02p01l02.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	_app->objectPresentationShow(kObject542107, 0);
	_app->objectAdd(kObject55210, "", "", 1);
	_app->objectAdd(kObjectPostcards, "Postcards", "A05_Kartice", 1);
	_app->objectSetActiveCursor(kObjectPostcards, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPostcards, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject55219, "", "", 1);
	_app->objectAdd(kObject55220, "", "", 1);
	_app->objectAdd(kObjectLoveIsland, "Love Island", "A05_Kartica1", 1);
	_app->objectSetActiveCursor(kObjectLoveIsland, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectLoveIsland, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectGreatInventor, "Great Inventor", "A05_Kartica2", 1);
	_app->objectSetActiveCursor(kObjectGreatInventor, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectGreatInventor, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectVisitUs, "Visit Us", "A05_Kartica3", 1);
	_app->objectSetActiveCursor(kObjectVisitUs, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectVisitUs, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectEnchantedIsland, "Enchanted Island", "A05_Kartica4", 1);
	_app->objectSetActiveCursor(kObjectEnchantedIsland, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectEnchantedIsland, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectGreatAttractions, "Great Attractions", "A05_Kartica5", 1);
	_app->objectSetActiveCursor(kObjectGreatAttractions, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectGreatAttractions, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectBootlegging, "Bootlegging", "A05_Kartica6", 1);
	_app->objectSetActiveCursor(kObjectBootlegging, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBootlegging, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObjectBathOfGiselle, "Bath of Giselle", "A05_Kartica7", 1);
	_app->objectSetActiveCursor(kObjectBathOfGiselle, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectBathOfGiselle, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject55210, kPuzzle55210, Common::Rect(467, 176, 529, 348), true, kCursorTake, 0);
	_app->objectAddPuzzleAccessibility(kObject55210, kPuzzle55210, Common::Rect(166, 388, 482, 457), false, kCursorZoom, 1);
	_app->objectAddPresentation(kObject55210);
	_app->objectPresentationAddImageToPuzzle(kObject55210, 0, kPuzzle55210, "a15s01n02p01l01.bmp", Common::Point(186, 397), true, kDrawType1, 1000);
	_app->objectAddPuzzleAccessibility(kObject55219, kPuzzle55220, Common::Rect(  0, 110,  77, 232), true, kCursorActionObject, 0);
	_app->objectAddPuzzleAccessibility(kObject55219, kPuzzle55220, Common::Rect(122, 110, 187, 232), true, kCursorActionObject, 1);
	_app->objectAddPuzzleAccessibility(kObject55219, kPuzzle55220, Common::Rect(198, 110, 273, 232), true, kCursorActionObject, 2);
	_app->objectAddPuzzleAccessibility(kObject55219, kPuzzle55220, Common::Rect(286, 110, 348, 232), true, kCursorActionObject, 3);
	_app->objectAddPuzzleAccessibility(kObject55219, kPuzzle55220, Common::Rect(369, 110, 435, 232), true, kCursorActionObject, 4);
	_app->objectAddPuzzleAccessibility(kObject55219, kPuzzle55220, Common::Rect(455, 110, 520, 232), true, kCursorActionObject, 5);
	_app->objectAddPuzzleAccessibility(kObject55219, kPuzzle55220, Common::Rect(544, 110, 636, 232), true, kCursorActionObject, 6);
	_app->objectAddPresentation(kObject55219);
	_app->objectPresentationAddImageToPuzzle(kObject55219, 0, kPuzzle55220, "a15s01n02p02l01.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55219);
	_app->objectPresentationAddImageToPuzzle(kObject55219, 1, kPuzzle55220, "a15s01n02p02l02.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55219);
	_app->objectPresentationAddImageToPuzzle(kObject55219, 2, kPuzzle55220, "a15s01n02p02l03.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55219);
	_app->objectPresentationAddImageToPuzzle(kObject55219, 3, kPuzzle55220, "a15s01n02p02l04.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55219);
	_app->objectPresentationAddImageToPuzzle(kObject55219, 4, kPuzzle55220, "a15s01n02p02l05.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55219);
	_app->objectPresentationAddImageToPuzzle(kObject55219, 5, kPuzzle55220, "a15s01n02p02l06.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55219);
	_app->objectPresentationAddImageToPuzzle(kObject55219, 6, kPuzzle55220, "a15s01n02p02l07.tga", Common::Point(0, 0), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject55220);
	_app->objectPresentationAddAnimationToPuzzle(kObject55220, 0, kPuzzle55220, "a15s01n02p02s01", kImageTypeBMP, Common::Point(39, 138), kDrawType1, 2000, 10, 25.0f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject55220, 0, 0, 55211);
	_app->objectAdd(kObjectPencil, "Pencil", "A05_Pencil", 1);
	_app->objectSetActiveCursor(kObjectPencil, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetPassiveCursor(kObjectPencil, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectAdd(kObject56001, "", "", 1);
	_app->objectAddPresentation(kObject56001);
	_app->objectAddPuzzleAccessibility(kObject56001, kPuzzle56000, Common::Rect(86, 228, 204, 445), true, kCursorActionObject, 0);
	_app->objectAdd(kObject56002, "", "", 1);
	_app->objectAddPresentation(kObject56002);
	_app->objectAddPuzzleAccessibility(kObject56002, kPuzzle56000, Common::Rect(212, 255, 382, 446), true, kCursorActionObject, 0);
	_app->soundAdd(52000, kSoundTypeBackgroundMusic, "1414.wac", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundSetVolume(52000, 90);
	_app->soundAdd(51001, kSoundTypeAmbientMusic, "1415.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAddAmbientSound(51011, 51001, 100, 0, 1, 2, 10);
	_app->rotationAddAmbientSound(51021, 51001, 100, 0, 1, 2, 10);
	_app->rotationAddAmbientSound(51031, 51001, 100, 0, 1, 2, 10);
	_app->rotationAddAmbientSound(51041, 51001, 100, 0, 1, 2, 10);
	_app->rotationAddAmbientSound(55011, 51001, 100, 0, 1, 2, 10);
	_app->rotationAddAmbientSound(55021, 51001, 100, 0, 1, 2, 10);
	_app->rotationAddAmbientSound(55031, 51001, 100, 0, 1, 2, 10);
	_app->soundAdd(51002, kSoundTypeAmbientEffect, "1416.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(51007, kSoundTypeAmbientEffect, "1417.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(51011, 51002, 1, 1, 15, 90, 100.0f, 10);
	_app->rotationAdd3DSound(51021, 51002, 1, 1, 15, 90, 106.0f, 10);
	_app->rotationAdd3DSound(51031, 51002, 1, 1, 15, 95, 105.0f, 10);
	_app->rotationAdd3DSound(51041, 51002, 1, 1, 15, 100, 88.0f, 10);
	_app->rotationAdd3DSound(51011, 51007, 1, 1, 15, 90, 100.0f, 10);
	_app->rotationAdd3DSound(51021, 51007, 1, 1, 15, 90, 106.0f, 10);
	_app->rotationAdd3DSound(51031, 51007, 1, 1, 15, 95, 105.0f, 10);
	_app->rotationAdd3DSound(51041, 51007, 1, 1, 15, 100, 88.0f, 10);
	_app->soundAdd(51003, kSoundTypeAmbientEffect, "1418.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(51004, kSoundTypeAmbientEffect, "1419.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(51005, kSoundTypeAmbientEffect, "1420.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(51006, kSoundTypeAmbientEffect, "1421.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(51009, kSoundTypeAmbientEffect, "1422.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53101, kSoundTypeAmbientEffect, "1423.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53102, kSoundTypeAmbientEffect, "1424.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(53011, 53102, 1, 1, 15, 95, 138.0f, 10);
	_app->rotationAdd3DSound(53021, 53102, 1, 1, 15, 100, 159.0f, 10);
	_app->soundAdd(53103, kSoundTypeAmbientEffect, "1424.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53104, kSoundTypeAmbientEffect, "1425.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53199, kSoundTypeAmbientEffect, "1426.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53198, kSoundTypeAmbientEffect, "1427.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53121, kSoundTypeAmbientEffect, "1428.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(52211, kSoundTypeEffect, "1429.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(52212, kSoundTypeAmbientEffect, "1430.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(532111, kSoundTypeAmbientEffect, "1431.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(532110, kSoundTypeAmbientEffect, "1432.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53122, kSoundTypeAmbientEffect, "1433.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(55201, kSoundTypeAmbientEffect, "1434.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53125, kSoundTypeAmbientEffect, "1435.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53123, kSoundTypeAmbientEffect, "1436.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(52222, kSoundTypeAmbientEffect, "1437.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53216, kSoundTypeAmbientEffect, "1438.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(54101, kSoundTypeAmbientEffect, "1439.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(54102, kSoundTypeAmbientEffect, "1440.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(51008, kSoundTypeEffect, "1441.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(522111, kSoundTypeEffect, "1442.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(522110, kSoundTypeEffect, "1443.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(52221, kSoundTypeAmbientEffect, "1444.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(52223, kSoundTypeAmbientEffect, "1445.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53110, kSoundTypeAmbientEffect, "1446.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53162, kSoundTypeAmbientEffect, "1447.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53163, kSoundTypeAmbientEffect, "1448.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53164, kSoundTypeAmbientEffect, "1449.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53201, kSoundTypeAmbientEffect, "1450.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53220, kSoundTypeAmbientEffect, "1451.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53212, kSoundTypeAmbientEffect, "1452.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53213, kSoundTypeAmbientEffect, "1453.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53214, kSoundTypeAmbientEffect, "1454.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53215, kSoundTypeAmbientEffect, "1455.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53217, kSoundTypeAmbientEffect, "1456.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53218, kSoundTypeAmbientEffect, "1457.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53221, kSoundTypeAmbientEffect, "1458.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53222, kSoundTypeAmbientEffect, "1459.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53112, kSoundTypeAmbientEffect, "1460.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53111, kSoundTypeAmbientEffect, "1461.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53113, kSoundTypeAmbientEffect, "1462.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53120, kSoundTypeAmbientEffect, "1463.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53124, kSoundTypeAmbientEffect, "1464.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(53126, kSoundTypeAmbientEffect, "1465.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(54111, kSoundTypeAmbientEffect, "1466.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(54112, kSoundTypeAmbientEffect, "1467.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(54212, kSoundTypeAmbientEffect, "1468.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(54213, kSoundTypeAmbientEffect, "1469.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(522011, kSoundTypeDialog, "1470.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(522013, kSoundTypeDialog, "1471.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(542021, kSoundTypeDialog, "1472.WAV", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(521111, kSoundTypeDialog, "1473.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(521112, kSoundTypeDialog, "1474.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(521113, kSoundTypeDialog, "1475.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(521114, kSoundTypeDialog, "1476.wav", _app->getConfiguration().dialog.loadFrom, 1, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(522012, kSoundTypeDialog, "1478.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(522014, kSoundTypeDialog, "1479.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(531021, kSoundTypeDialog, "1481.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53131, kSoundTypeDialog, "1482.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(531411, kSoundTypeDialog, "1483.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(531412, kSoundTypeDialog, "1484.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(531413, kSoundTypeDialog, "1485.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(531414, kSoundTypeDialog, "1486.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53142, kSoundTypeDialog, "1487.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53143, kSoundTypeDialog, "1488.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53144, kSoundTypeDialog, "1489.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53145, kSoundTypeDialog, "1490.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53151, kSoundTypeDialog, "1491.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53152, kSoundTypeDialog, "1492.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53153, kSoundTypeDialog, "1493.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(53171, kSoundTypeDialog, "1494.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(54121, kSoundTypeDialog, "1495.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(542022, kSoundTypeDialog, "1497.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(542111, kSoundTypeDialog, "1498.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(542112, kSoundTypeDialog, "1499.WAC", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineFloat(52211, 0.0f);
	_app->varDefineFloat(53141, 0.0f);
	_app->varDefineByte(52999, 0);
	_app->varDefineByte(52101, -1);
	_app->varDefineByte(52201, 0);
	_app->varDefineByte(52230, 0);
	_app->varDefineByte(52231, 0);
	_app->varDefineByte(52221, 0);
	_app->varDefineByte(52222, 0);
	_app->varDefineByte(53000, 0);
	_app->varDefineByte(53103, 0);
	_app->varDefineByte(53100, 0);
	_app->varDefineByte(53102, 0);
	_app->varDefineByte(53101, 0);
	_app->varDefineByte(53111, 0);
	_app->varDefineByte(53112, 0);
	_app->varDefineByte(53120, 0);
	_app->varDefineByte(53121, 0);
	_app->varDefineByte(53123, 50);
	_app->varDefineByte(53125, 0);
	_app->varDefineByte(53124, 0);
	_app->varDefineByte(53126, 10);
	_app->varDefineByte(53127, 0);
	_app->varDefineByte(53141, 0);
	_app->varDefineByte(53142, 1);
	_app->varDefineByte(53211, 0);
	_app->varDefineByte(53212, 0);
	_app->varDefineByte(53213, 0);
	_app->varDefineByte(53214, 0);
	_app->varDefineByte(53215, 0);
	_app->varDefineByte(53216, 0);
	_app->varDefineByte(53217, 0);
	_app->varDefineByte(531101, 0);
	_app->varDefineByte(53221, 0);
	_app->varDefineByte(53222, 0);
	_app->varDefineByte(53223, 0);
	_app->varDefineByte(53234, 0);
	_app->varDefineByte(54201, 0);
	_app->varDefineByte(54202, 0);
	_app->varDefineByte(54101, 0);
	_app->varDefineByte(54228, 0);
	_app->varDefineByte(54222, 0);
	_app->varDefineByte(54223, 0);
	_app->varDefineByte(54224, 0);
	_app->varDefineByte(54225, 0);
	_app->varDefineByte(54226, 0);
	_app->varDefineByte(54227, 0);
	_app->varDefineByte(54212, 2);
	_app->varDefineByte(54213, 3);
	_app->varDefineByte(54214, 5);
	_app->varDefineByte(54215, 1);
	_app->varDefineByte(54216, 0);
	_app->varDefineByte(54217, 4);
	_app->varDefineByte(55210, 0);
	_app->varDefineByte(55211, 6);
	_app->varDefineByte(55212, 3);
	_app->varDefineByte(55213, 0);
	_app->varDefineByte(55214, 1);
	_app->varDefineByte(55215, 4);
	_app->varDefineByte(55216, 5);
	_app->varDefineByte(55217, 2);
	_app->varDefineByte(55221, 1);
	_app->varDefineByte(55222, 1);
	_app->varDefineByte(55223, 1);
	_app->varDefineByte(55224, 1);
	_app->varDefineByte(55225, 1);
	_app->varDefineByte(55226, 1);
	_app->varDefineByte(55227, 1);
	_app->varDefineByte(531532, 0);
	_app->varDefineWord(53001, 112);
	_app->varDefineWord(53011, 91);
	_app->objectPresentationSetImageCoordinatesOnPuzzle(54210, (uint8)_app->varGetByte(54212), Common::Point(_app->varGetWord(53001), _app->varGetWord(53011)));
	_app->varDefineWord(53002, 112);
	_app->varDefineWord(53012, 207);
	_app->objectPresentationSetImageCoordinatesOnPuzzle(54210, (uint8)_app->varGetByte(54213), Common::Point(_app->varGetWord(53002), _app->varGetWord(53012)));
	_app->varDefineWord(53003, 284);
	_app->varDefineWord(53013, 325);
	_app->objectPresentationSetImageCoordinatesOnPuzzle(54210, (uint8)_app->varGetByte(54214), Common::Point(_app->varGetWord(53003), _app->varGetWord(53013)));
	_app->varDefineWord(53004, 284);
	_app->varDefineWord(53014, 91);
	_app->objectPresentationSetImageCoordinatesOnPuzzle(54210, (uint8)_app->varGetByte(54215), Common::Point(_app->varGetWord(53004), _app->varGetWord(53014)));
	_app->varDefineWord(53005, 284);
	_app->varDefineWord(53015, 207);
	_app->objectPresentationSetImageCoordinatesOnPuzzle(54210, (uint8)_app->varGetByte(54216), Common::Point(_app->varGetWord(53005), _app->varGetWord(53015)));
	_app->varDefineWord(53006, 112);
	_app->varDefineWord(53016, 325);
	_app->objectPresentationSetImageCoordinatesOnPuzzle(54210, (uint8)_app->varGetByte(54217), Common::Point(_app->varGetWord(53006), _app->varGetWord(53016)));
}

void Zone8Faust::onSetup(SetupType type) {
	error("[EventSetupFaust::onSetupZone8] Not implemented");
}

void Zone8Faust::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->timerStop(kTimer0);
		_app->timerStart(kTimer0, rnd(5000) + 2000);
		_app->soundPlay(51003 + rnd(4));
		break;

	case kTimer1:
		_app->soundPlay(52211, kSoundLoop);
		_app->objectPresentationShow(kObject52231, 0);
		_app->timerStop(kTimer1);
		break;

	case kTimer3:
		_app->timerStop(kTimer3);

		if (!_app->varGetByte(53124)) {
			_app->objectPresentationShow(kObject53121, 1);
			_app->varSetByte(53124, 1);
			_app->puzzleSetMovabilityOff(53102, 0, 0);
		}

		_app->timerStart(kTimer4, 10);

		if (_app->varGetByte(53000) == 1) {
			_app->soundPlay(53122, kSoundLoop);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer4:
		if (_app->varGetByte(53123) >= 100) {
			_app->timerStop(kTimer4);
		} else {
			_app->varSetByte(53123, _app->varGetByte(53123) + 5);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer5:
		if (_app->varGetByte(53123) <= _app->varGetByte(53126)) {
			_app->timerStop(kTimer5);
			_app->soundStop(53122, 1024);
			_app->varSetByte(53123, 50);
		} else {
			_app->varSetByte(53123, _app->varGetByte(53123) - 5);
			_app->soundSetVolume(53122, _app->varGetByte(53123) - _app->varGetByte(53126));
		}
		break;

	case kTimer6:
		_app->timerStop(kTimer6);

		_app->timerStart(kTimer6, rnd(30000) + 30000);

		if (_app->varGetByte(53103)) {
			_app->rotationSet3DSoundOff(53011, 53102);
			_app->rotationSet3DSoundOff(53021, 53102);
			_app->soundPlay(53104);
			_app->soundSetVolume(53104, 90);
			_app->soundStop(53103, 1024);
			_app->varSetByte(53103, 0);
		} else {
			_app->rotationSet3DSoundOn(53011, 53102);
			_app->rotationSet3DSoundOn(53021, 53102);
			_app->soundPlay(53101);
			_app->soundPlay(53103, kSoundLoop);
			_app->soundSetVolume(53103, 90);
			_app->soundSetVolume(53101, 90);
			_app->varSetByte(53103, 1);
		}
		break;
	}
}

void Zone8Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone8Faust::onButtonUp] Not implemented");
}

void Zone8Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone8] Not implemented");
}

void Zone8Faust::onSound(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 53125:
		if (_app->bagHasClickedObject()
		 && _app->bagGetClickedObject() == kObjectMatches2)
			_app->cursorDelete();
		break;

	case 53198:
		_app->soundPlay(53145);
		break;

	case 522011:
	case 522012:
	case 522013:
	case 531411:
	case 531412:
	case 531413:
	case 542021:
		_app->soundPlay(id + 1);
		break;

	case 531021:
		if (_app->varGetByte(52999) > 3) {
			_app->soundStop(52000, 1024);
			_app->soundStop(53103, 1024);
			_app->timerStop(kTimer6);
			_app->rotationSet3DSoundOff(53011, 53102);
			_app->rotationSet3DSoundOff(53021, 53102);

			_app->displayFade("a05_black.bmp", "A15_E07M09a.bmp", 15, 0, kLoadFromCd, kArchiveArt);
			_app->playMovie("1509");
			_app->displayFade("A15_E07M09b.bmp", "a05_black", 15, 0, kLoadFromCd, kArchiveArt);

			_app->rotationSetAlp(55011, 0.0f);
			_app->rotationSetBet(55011, 0.0f);
			_app->rotationSetRan(55011, 87.0f);
			_app->rotationSetActive(55011);
			_app->soundPlay(51001, kSoundLoop);
		}
		break;
	}
}

void Zone8Faust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[Zone8Faust::onBag] Not implemented");
}

void Zone8Faust::onBagClickedObject(ObjectId id) {
	switch (id) {
	default:
		break;

	case kObjectMatches2:
		_app->soundPlay(53125);
		break;

	case kObjectTeddyBear:
		if (_app->getCurrentPuzzleId() != kPuzzle53110) {
			_app->puzzleSetMod(kPuzzleMenu, 2, 17);
			_app->objectPresentationShow(kObject17);
			_app->objectSetAccessibilityOn(kObject17);
		}
		break;
	}
}

void Zone8Faust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
	if (movabilityFrom == 53123 && movabilityTo == 1) {
		if (_app->varGetByte(53123) == 1 && _app->bagGetClickedObject() == kObjectMatches2) {
			_app->objectPresentationShow(kObject53123, 2);
			_app->varSetByte(53121, 2);
			_app->soundPlay(53126);
			_app->soundPlay(53123, kSoundLoop);
			_app->puzzleSetMovabilityOff(kPuzzle53102, 1, 1);
			_app->soundStop(53124, 1024);
			_app->varSetByte(53123, 50);
			_app->timerStop(kTimer3);
			_app->timerStop(kTimer4);
			_app->timerStop(kTimer5);

			if (_app->varGetByte(53120) == 1) {
				_app->soundPlay(53121, kSoundLoop);
				_app->timerStart(kTimer3, 300);
			}
		}
	}
}

void Zone8Faust::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	_app->objectPresentationHide(kObject25);

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom == 52021 && movabilityTo == 52220)
			if (_app->varGetByte(52230) == 1)
				_app->soundPlay(52212);
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityFrom == 53110 && movabilityTo == 53011)
			if (_app->varGetByte(531101) == 1)
				_app->soundPlay(53218);
		break;
	}
}

void Zone8Faust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	error("[Zone8Faust::onAfterRide] Not implemented");
}

} // End of namespace Ring
