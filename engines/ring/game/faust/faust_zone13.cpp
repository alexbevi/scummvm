/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 13
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 013110-1301, USA.
 */

#include "ring/game/faust/faust_zone13.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace FaustGame;

namespace Ring {

Zone13Faust::Zone13Faust(ApplicationFaust *application) : _app(application) {
}

Zone13Faust::~Zone13Faust() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone13Faust::onInit() {
	_app->setCurrentZone(kZone13);

	_app->puzzleAdd(kPuzzle91001);
	_app->puzzleAddBackgroundImage(kPuzzle91001, "a09s02n01p01.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(91001, "1718", 0, 1);
	_app->rotationAdd(91002, "1719", 0, 0);
	_app->rotationAdd(91003, "1720", 0, 0);
	_app->rotationAdd(91004, "1721", 0, 0);
	_app->rotationAdd(91005, "1722", 0, 0);
	_app->rotationAdd(91006, "1723", 0, 1);
	_app->rotationAdd(91007, "1724", 0, 0);
	_app->rotationAdd(93001, "1725", 0, 0);
	_app->rotationAdd(93002, "1726", 0, 0);
	_app->rotationAdd(93003, "1725", 0, 0);
	_app->rotationAdd(93004, "1727", 0, 0);
	_app->rotationAdd(93005, "1728", 0, 0);
	_app->rotationAdd(93006, "1725", 0, 0);
	_app->rotationAdd(93007, "1729", 0, 0);
	_app->rotationAdd(93008, "1730", 0, 0);
	_app->rotationAdd(93009, "1726", 0, 0);
	_app->rotationAdd(93010, "1728", 0, 0);
	_app->rotationAdd(93011, "1729", 0, 0);
	_app->rotationAdd(93012, "1725", 0, 0);
	_app->rotationAdd(93013, "1731", 0, 0);
	_app->rotationAdd(93014, "1728", 0, 0);
	_app->rotationAdd(93015, "1725", 0, 0);
	_app->rotationAdd(93016, "1729", 0, 0);
	_app->rotationAdd(93017, "1732", 0, 0);
	_app->rotationAdd(93018, "1726", 0, 0);
	_app->rotationAdd(93019, "1728", 0, 0);
	_app->rotationAdd(93020, "1729", 0, 0);
	_app->rotationAdd(93021, "1725", 0, 0);
	_app->rotationAdd(93022, "1726", 0, 0);
	_app->rotationAdd(93023, "1725", 0, 0);
	_app->rotationAdd(93024, "1726", 0, 0);
	_app->rotationAdd(93025, "1725", 0, 0);
	_app->rotationAdd(93026, "1733", 0, 0);
	_app->rotationAdd(93027, "1729", 0, 0);
	_app->rotationAdd(93028, "1725", 0, 0);
	_app->rotationAdd(93029, "1734", 0, 0);
	_app->rotationAdd(93030, "1728", 0, 0);
	_app->rotationAdd(93031, "1726", 0, 0);
	_app->rotationAdd(93032, "1729", 0, 0);
	_app->rotationAdd(93033, "1728", 0, 0);
	_app->rotationAdd(93034, "1725", 0, 0);
	_app->rotationAdd(93035, "1726", 0, 0);
	_app->rotationAdd(93036, "1725", 0, 0);
	_app->rotationAdd(93050, "1735", 0, 0);
	_app->rotationAdd(93051, "1736", 0, 0);
	_app->rotationAddMovabilityToRotation(93050, 93001, "", Common::Rect(1678, -154, 2040, 278), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93050, 0, 270.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93001, 93002, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93001, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 145.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93001, 93050, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93001, 1, 90.0f, 15.0f, 87.0f, 0.0f, 2, 280.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93002, 93003, "", Common::Rect(1850, -75, 2050, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93002, 0, 150.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93002, 93001, "", Common::Rect(3350, -75, 3550, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93002, 1, 259.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93003, 93004, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93003, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93003, 93002, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93003, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 295.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93004, 93011, "", Common::Rect(940, -20, 1200, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93004, 0, 60.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93004, 93005, "", Common::Rect(1500, -20, 1770, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93004, 1, 120.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93004, 93003, "", Common::Rect(2884, -130, 3394, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93004, 2, 270.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93005, 93004, "", Common::Rect(3580, -20, 3600, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93005, 0, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93005, 93004, "", Common::Rect(0, -20, 145, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93005, 1, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93005, 93011, "", Common::Rect(185, -20, 340, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93005, 2, 330.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93005, 93006, "", Common::Rect(1263, -130, 1719, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93005, 3, 100.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93006, 93007, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93006, 0, 270.0f, 15.0f, 87.0f, 0.0f, 2, 210.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93006, 93005, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93006, 1, 90.0f, 15.0f, 87.0f, 0.0f, 2, 330.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93007, 93008, "", Common::Rect(2550, -20, 2700, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93007, 0, 210.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93007, 93010, "", Common::Rect(2350, -20, 2500, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93007, 1, 210.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93007, 93006, "", Common::Rect(991, -130, 1451, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93007, 2, 75.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93008, 93007, "", Common::Rect(940, -20, 1200, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93008, 0, 60.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93008, 93010, "", Common::Rect(1500, -20, 1770, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93008, 1, 120.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93008, 93009, "", Common::Rect(2884, -130, 3394, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93008, 2, 270.0f, 15.0f, 87.0f, 0.0f, 2, 150.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93009, 93010, "", Common::Rect(1850, -75, 2050, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93009, 0, 150.0f, 15.0f, 87.0f, 0.0f, 2, 330.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93009, 93008, "", Common::Rect(3350, -75, 3550, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93009, 1, 259.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93010, 93008, "", Common::Rect(3580, -20, 3600, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93010, 0, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93010, 93008, "", Common::Rect(0, -20, 145, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93010, 1, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93010, 93007, "", Common::Rect(185, -20, 340, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93010, 2, 330.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93010, 93009, "", Common::Rect(1263, -130, 1719, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93010, 3, 100.0f, 15.0f, 87.0f, 0.0f, 2, 295.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93011, 93004, "", Common::Rect(2550, -20, 2700, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93011, 0, 210.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93011, 93005, "", Common::Rect(2350, -20, 2500, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93011, 1, 210.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93011, 93012, "", Common::Rect(991, -130, 1451, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93011, 2, 75.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93012, 93013, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93012, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93012, 93011, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93012, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 210.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93013, 93020, "", Common::Rect(940, -20, 1200, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93013, 0, 60.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93013, 93014, "", Common::Rect(1500, -20, 1770, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93013, 1, 120.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93013, 93012, "", Common::Rect(2884, -130, 3394, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93013, 2, 270.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93014, 93013, "", Common::Rect(3580, -20, 3600, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93014, 0, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93014, 93013, "", Common::Rect(0, -20, 145, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93014, 1, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93014, 93020, "", Common::Rect(185, -20, 340, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93014, 2, 330.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93014, 93015, "", Common::Rect(1263, -130, 1719, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93014, 3, 100.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93015, 93016, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93015, 0, 270.0f, 15.0f, 87.0f, 0.0f, 2, 210.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93015, 93014, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93015, 1, 90.0f, 15.0f, 87.0f, 0.0f, 2, 330.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93016, 93017, "", Common::Rect(2550, -20, 2700, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93016, 0, 210.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93016, 93019, "", Common::Rect(2350, -20, 2500, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93016, 1, 210.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93016, 93015, "", Common::Rect(991, -130, 1451, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93016, 2, 75.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93017, 93016, "", Common::Rect(940, -20, 1200, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93017, 0, 60.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93017, 93019, "", Common::Rect(1500, -20, 1770, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93017, 1, 120.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93017, 93018, "", Common::Rect(2884, -130, 3394, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93017, 2, 270.0f, 15.0f, 87.0f, 0.0f, 2, 150.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93018, 93019, "", Common::Rect(1850, -75, 2050, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93018, 0, 150.0f, 15.0f, 87.0f, 0.0f, 2, 330.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93018, 93017, "", Common::Rect(3350, -75, 3550, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93018, 1, 259.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93019, 93017, "", Common::Rect(3580, -20, 3600, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93019, 0, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93019, 93017, "", Common::Rect(0, -20, 145, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93019, 1, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93019, 93016, "", Common::Rect(185, -20, 340, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93019, 2, 330.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93019, 93018, "", Common::Rect(1263, -130, 1719, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93019, 3, 100.0f, 15.0f, 87.0f, 0.0f, 2, 295.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93020, 93013, "", Common::Rect(2550, -20, 2700, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93020, 0, 210.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93020, 93014, "", Common::Rect(2350, -20, 2500, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93020, 1, 210.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93020, 93021, "", Common::Rect(991, -130, 1451, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93020, 2, 75.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93021, 93022, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93021, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 150.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93021, 93020, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93021, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 210.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93022, 93023, "", Common::Rect(1850, -75, 2050, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93022, 0, 150.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93022, 93021, "", Common::Rect(3350, -75, 3550, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93022, 1, 259.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93023, 93024, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93023, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 150.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93023, 93022, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93023, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 295.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93024, 93025, "", Common::Rect(1850, -75, 2050, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93024, 0, 150.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93024, 93023, "", Common::Rect(3350, -75, 3550, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93024, 1, 259.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93025, 93026, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93025, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93025, 93024, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93025, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 295.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93026, 93027, "", Common::Rect(940, -20, 1200, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93026, 0, 60.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93026, 93033, "", Common::Rect(1500, -20, 1770, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93026, 1, 120.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93026, 93025, "", Common::Rect(2884, -130, 3394, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93026, 2, 270.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93027, 93026, "", Common::Rect(2550, -20, 2700, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93027, 0, 210.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93027, 93033, "", Common::Rect(2350, -20, 2500, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93027, 1, 210.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93027, 93028, "", Common::Rect(991, -130, 1451, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93027, 2, 75.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93028, 93029, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93028, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93028, 93027, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93028, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 210.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93029, 93032, "", Common::Rect(940, -20, 1200, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93029, 0, 60.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93029, 93030, "", Common::Rect(1500, -20, 1770, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93029, 1, 120.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93029, 93028, "", Common::Rect(2884, -130, 3394, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93029, 2, 270.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93030, 93029, "", Common::Rect(3580, -20, 3600, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93030, 0, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93030, 93029, "", Common::Rect(0, -20, 145, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93030, 1, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93030, 93032, "", Common::Rect(185, -20, 340, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93030, 2, 330.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93030, 93031, "", Common::Rect(1263, -130, 1719, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93030, 3, 100.0f, 15.0f, 87.0f, 0.0f, 2, 295.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93031, 93030, "", Common::Rect(1850, -75, 2050, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93031, 0, 150.0f, 15.0f, 87.0f, 0.0f, 2, 330.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93031, 93032, "", Common::Rect(3350, -75, 3550, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93031, 1, 259.0f, 15.0f, 87.0f, 0.0f, 2, 210.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93032, 93029, "", Common::Rect(2550, -20, 2700, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93032, 0, 210.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93032, 93030, "", Common::Rect(2350, -20, 2500, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93032, 1, 210.0f, 15.0f, 87.0f, 0.0f, 2, 105.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93032, 93031, "", Common::Rect(991, -130, 1451, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93032, 2, 75.0f, 15.0f, 87.0f, 0.0f, 2, 145.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93033, 93026, "", Common::Rect(3580, -20, 3600, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93033, 0, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93033, 93026, "", Common::Rect(0, -20, 145, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93033, 1, 330.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93033, 93027, "", Common::Rect(185, -20, 340, 270), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93033, 2, 330.0f, 15.0f, 87.0f, 0.0f, 2, 75.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93033, 93034, "", Common::Rect(1263, -130, 1719, 300), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93033, 3, 100.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93034, 93033, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93034, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 330.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93034, 93035, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93034, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 295.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93035, 93034, "", Common::Rect(1850, -75, 2050, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93035, 0, 150.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93035, 93036, "", Common::Rect(3350, -75, 3550, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93035, 1, 259.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93036, 93035, "", Common::Rect(1250, -75, 1450, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93036, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 150.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93036, 93051, "", Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93036, 1, 270.0f, 15.0f, 87.0f, 0.0f, 2, 270.0f, 15.0f, 87.0f);
	_app->rotationAddMovabilityToRotation(93051, 93036, "", Common::Rect(1218, -40, 1560, 317), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(93051, 0, 90.0f, 15.0f, 87.0f, 0.0f, 2, 90.0f, 15.0f, 87.0f);
	_app->soundAdd(93001, kSoundTypeAmbientEffect, "1737.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(93002, kSoundTypeAmbientEffect, "1738.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAdd3DSound(93001, 93001, 1, 1, 15, 100, 90.0f, 10);
	_app->rotationAdd3DSound(93002, 93001, 1, 1, 15, 100, 145.0f, 10);
	_app->rotationAdd3DSound(93003, 93001, 1, 1, 15, 100, 90.0f, 10);
	_app->rotationAdd3DSound(93004, 93001, 1, 1, 15, 100, 60.0f, 10);
	_app->rotationAdd3DSound(93005, 93001, 1, 1, 15, 100, 330.0f, 10);
	_app->rotationAdd3DSound(93006, 93001, 1, 1, 15, 100, 90.0f, 10);
	_app->rotationAdd3DSound(93007, 93001, 1, 1, 15, 100, 75.0f, 10);
	_app->rotationAdd3DSound(93008, 93001, 1, 1, 15, 100, 60.0f, 10);
	_app->rotationAdd3DSound(93009, 93001, 1, 1, 15, 100, 145.0f, 10);
	_app->rotationAdd3DSound(93010, 93001, 1, 1, 15, 100, 330.0f, 10);
	_app->rotationAdd3DSound(93011, 93001, 1, 1, 15, 100, 75.0f, 10);
	_app->rotationAdd3DSound(93012, 93001, 1, 1, 15, 100, 90.0f, 10);
	_app->rotationAdd3DSound(93013, 93001, 1, 1, 15, 100, 60.0f, 10);
	_app->rotationAdd3DSound(93014, 93001, 1, 1, 15, 100, 330.0f, 10);
	_app->rotationAdd3DSound(93015, 93001, 1, 1, 15, 100, 270.0f, 10);
	_app->rotationAdd3DSound(93016, 93001, 1, 1, 15, 100, 75.0f, 10);
	_app->rotationAdd3DSound(93017, 93001, 1, 1, 15, 100, 60.0f, 10);
	_app->rotationAdd3DSound(93018, 93001, 1, 1, 15, 100, 295.0f, 10);
	_app->rotationAdd3DSound(93019, 93001, 1, 1, 15, 100, 330.0f, 10);
	_app->rotationAdd3DSound(93020, 93001, 1, 1, 15, 100, 75.0f, 10);
	_app->rotationAdd3DSound(93021, 93001, 1, 1, 15, 100, 90.0f, 10);
	_app->rotationAdd3DSound(93022, 93001, 1, 1, 15, 100, 145.0f, 10);
	_app->rotationAdd3DSound(93023, 93001, 1, 1, 15, 100, 90.0f, 10);
	_app->rotationAdd3DSound(93024, 93001, 1, 1, 15, 100, 145.0f, 10);
	_app->rotationAdd3DSound(93025, 93001, 1, 1, 15, 100, 90.0f, 10);
	_app->rotationAdd3DSound(93026, 93001, 1, 1, 15, 100, 120.0f, 10);
	_app->rotationAdd3DSound(93027, 93001, 1, 1, 15, 100, 210.0f, 10);
	_app->rotationAdd3DSound(93028, 93001, 1, 1, 15, 100, 270.0f, 10);
	_app->rotationAdd3DSound(93029, 93001, 1, 1, 15, 100, 270.0f, 10);
	_app->rotationAdd3DSound(93030, 93001, 1, 1, 15, 100, 330.0f, 10);
	_app->rotationAdd3DSound(93031, 93001, 1, 1, 15, 100, 145.0f, 10);
	_app->rotationAdd3DSound(93032, 93001, 1, 1, 15, 100, 210.0f, 10);
	_app->rotationAdd3DSound(93033, 93001, 1, 1, 15, 100, 105.0f, 10);
	_app->rotationAdd3DSound(93034, 93001, 1, 1, 15, 100, 270.0f, 10);
	_app->rotationAdd3DSound(93035, 93001, 1, 1, 15, 100, 295.0f, 10);
	_app->rotationAdd3DSound(93036, 93001, 1, 1, 15, 100, 270.0f, 10);
	_app->rotationAdd3DSound(93050, 93001, 1, 1, 15, 100, 145.0f, 10);
	_app->rotationAdd3DSound(93051, 93001, 1, 1, 15, 100, 270.0f, 10);
	_app->objectAdd(kObject91301, "", "", 1);
	_app->objectAddRotationAccessibility(kObject91301, 93050, Common::Rect(3311, -304, 3600, 155), true, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject91301, 93050, Common::Rect(0, -304, 80, 155), true, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject91301, 93051, Common::Rect(3050, -75, 3250, 250), true, kCursorMove, 1);
	_app->rotationAddMovabilityToRotation(91001, 91002, "", Common::Rect(3443, -43, 3600, 233), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91001, 0, 315.0f, 0.0f, 87.0f, 0.0f, 2, 315.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91001, 91002, "", Common::Rect(0, -43, 251, 233), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91001, 1, 315.0f, 0.0f, 87.0f, 0.0f, 2, 315.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91001, 91004, "", Common::Rect(1620, -90, 2084, 252), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91001, 2, 133.0f, 0.0f, 87.0f, 0.0f, 2, 133.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91002, 91001, "", Common::Rect(924, -99, 1371, 258), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91002, 0, 70.0f, 0.0f, 87.0f, 0.0f, 2, 70.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91002, 91003, "", Common::Rect(3360, -151, 3600, 258), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91002, 1, 318.0f, 0.0f, 87.0f, 0.0f, 2, 318.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91002, 91003, "", Common::Rect(0, -151, 212, 258), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91002, 2, 318.0f, 0.0f, 87.0f, 0.0f, 2, 318.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91002, 91005, "", Common::Rect(305, -282, 722, 57), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91002, 3, 9.0f, -14.0f, 87.0f, 0.0f, 2, 9.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91003, 91002, "", Common::Rect(1998, -132, 2450, 219), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91003, 0, 180.0f, 0.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91003, 91004, "", Common::Rect(669, -172, 1153, 221), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91003, 1, 44.0f, 0.0f, 87.0f, 0.0f, 2, 44.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91003, 91005, "", Common::Rect(1256, -357, 1852, 55), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91003, 2, 114.0f, -14.0f, 87.0f, 0.0f, 2, 114.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91004, 91001, "", Common::Rect(2334, -177, 2803, 200), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91004, 0, 213.0f, 0.0f, 87.0f, 0.0f, 2, 213.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91004, 91003, "", Common::Rect(3250, -170, 3600, 181), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91004, 1, 296.0f, 0.0f, 87.0f, 0.0f, 2, 296.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91004, 91003, "", Common::Rect(0, -170, 75, 181), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91004, 2, 296.0f, 0.0f, 87.0f, 0.0f, 2, 296.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91004, 91005, "", Common::Rect(2805, -259, 3216, 68), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91004, 3, 258.0f, -10.0f, 87.0f, 0.0f, 2, 258.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91005, 91002, "", Common::Rect(2060, 17, 2466, 380), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91005, 0, 182.0f, 22.0f, 87.0f, 0.0f, 2, 258.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91005, 91003, "", Common::Rect(3142, -5, 3600, 397), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91005, 1, 291.0f, 22.0f, 87.0f, 0.0f, 2, 291.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91005, 91003, "", Common::Rect(0, -5, 63, 397), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91005, 2, 291.0f, 22.0f, 87.0f, 0.0f, 2, 291.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91005, 91004, "", Common::Rect(1047, -55, 1566, 343), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91005, 3, 80.0f, 16.0f, 87.0f, 0.0f, 2, 80.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91006, 91007, "", Common::Rect(1764, -108, 2007, 144), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91006, 0, 141.0f, 0.0f, 87.0f, 0.0f, 2, 141.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91007, 91001, "", Common::Rect(2039, -231, 2169, 313), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91007, 0, 144.0f, 0.0f, 87.0f, 0.0f, 2, 227.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91007, 91006, "", Common::Rect(3492, -84, 3600, 306), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91007, 1, 322.0f, 0.0f, 87.0f, 0.0f, 2, 322.0f, 0.3f, 87.0f);
	_app->rotationAddMovabilityToRotation(91007, 91006, "", Common::Rect(0, -84, 298, 306), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(91007, 2, 322.0f, 0.0f, 87.0f, 0.0f, 2, 322.0f, 0.3f, 87.0f);
	_app->objectAdd(kObject91001, "", "", 1);
	_app->objectAddRotationAccessibility(kObject91001, 91001, Common::Rect(747, 26,  865, 250), true, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject91001, 91001, Common::Rect(856, 26, 1139, 250), true, kCursorMove, 1);
	_app->objectAddPresentation(kObject91001);
	_app->objectPresentationAddAnimationToRotation(kObject91001, 0, 91001, 0, 10, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject91001, 0, 0, 91003);
	_app->objectAdd(kObject91002, "", "", 1);
	_app->objectAddRotationAccessibility(kObject91002, 91006, Common::Rect(3023, -19, 3172, 148), true, kCursorAction, 0);
	_app->objectAddPuzzleAccessibility(kObject91002, kPuzzle91001, Common::Rect(0, 420, 640, 464), true, kCursorBack, 1);
	_app->objectAddRotationAccessibility(kObject91002, 91007, Common::Rect(1620, -231, 2038, 313), true, kCursorMove, 2);
	_app->objectAddPresentation(kObject91002);
	_app->objectPresentationAddAnimationToRotation(kObject91002, 0, 91006, 0, 15, 12.5f, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject91002, 0, 0, 91001);
	_app->objectAddPresentation(kObject91002);
	_app->objectPresentationAddAnimationToRotation(kObject91002, 1, 91006, 0, 15, 12.5f, 10);
	_app->objectPresentationSetAnimationOnRotation(kObject91002, 1, 0, 91002);
	_app->objectAdd(kObject91003, "", "", 1);
	_app->objectAddRotationAccessibility(kObject91003, 91006, Common::Rect(3512, 12, 3600, 207), true, kCursorZoom, 0);
	_app->objectAddRotationAccessibility(kObject91003, 91006, Common::Rect(   0, 12,  251, 207), true, kCursorZoom, 1);
	_app->objectAdd(kObject91004, "", "Cur_drag", 4);
	_app->objectSetPassiveDrawCursor(kObject91004, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	_app->objectSetActiveDrawCursor(kObject91004, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 11; i++) {
		_app->objectAddPresentation(kObject91004);
		_app->objectPresentationAddImageToPuzzle(kObject91004, i, kPuzzle91001, Common::String::format("a09s02n01p01s01.%04d.bmp", i + 1), Common::Point(74, 62), true, kDrawType1, 1000);
	}

	_app->objectAddPuzzleAccessibility(kObject91004, kPuzzle91001, Common::Rect(94, 280, 365, 400), true, kCursorDragDrop, 0);
	_app->soundAdd(91920, kSoundTypeBackgroundMusic, "1739.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(91921, kSoundTypeBackgroundMusic, "1740.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(91101, kSoundTypeAmbientEffect, "1741.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91102, kSoundTypeAmbientEffect, "1742.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91103, kSoundTypeAmbientEffect, "1743.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91120, kSoundTypeAmbientEffect, "1744.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91121, kSoundTypeAmbientEffect, "1745.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91122, kSoundTypeAmbientEffect, "1746.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91105, kSoundTypeAmbientEffect, "1747.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91106, kSoundTypeAmbientEffect, "1748.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91107, kSoundTypeAmbientEffect, "1749.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91108, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91109, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91110, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91111, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91112, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91113, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91114, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91115, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91116, kSoundTypeAmbientEffect, "1750.wav", _app->getConfiguration().ambientEffect.loadFrom, 1, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(91117, kSoundTypeAmbientEffect, "1751.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(91117, 90);
	_app->soundAdd(91118, kSoundTypeAmbientEffect, "1752.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(91118, 90);
	_app->soundAdd(91119, kSoundTypeAmbientEffect, "1753.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundSetVolume(91119, 90);
	_app->soundAdd(91123, kSoundTypeAmbientEffect, "1754.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(91001, 91123, 1, 1, 15, 95, 205.0f, 10);
	_app->rotationAdd3DSound(91002, 91123, 1, 1, 15, 85, 85.0f, 10);
	_app->rotationAdd3DSound(91004, 91123, 1, 1, 15, 100, 78.0f, 10);
	_app->rotationAdd3DSound(91005, 91123, 1, 1, 15, 80, 120.0f, 10);
	_app->soundAdd(91124, kSoundTypeAmbientEffect, "1755.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->rotationAdd3DSound(91002, 91124, 1, 1, 15, 100, 167.0f, 10);
	_app->rotationAdd3DSound(91001, 91124, 1, 1, 15, 90, 307.0f, 10);
	_app->rotationAdd3DSound(91003, 91124, 1, 1, 15, 80, 177.0f, 10);
	_app->rotationAdd3DSound(91005, 91124, 1, 1, 15, 85, 178.0f, 10);
	_app->soundAdd(91001, kSoundTypeDialog, "1756.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(91002, kSoundTypeDialog, "1757.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineByte(91000, 0);
	_app->varDefineByte(91001, 0);
	_app->varDefineByte(91002, 0);
	_app->varDefineFloat(91001, 0.0f);
}

void Zone13Faust::onTimer(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetFloat(91001, _app->varGetFloat(91001) - 1.0f);
		_app->objectPresentationHide(kObject91004);
		_app->objectPresentationShow(kObject91004, (uint32)_app->varGetFloat(91001));

		if (floor(_app->varGetFloat(91001)) <= 1.0f) {
			_app->timerStop(kTimer0);
			_app->soundPlay(91107);
			_app->waitForEscape(10);
			_app->soundStop(91105, 1024);
			_app->soundStop(91106, 1024);
		}
		break;

	case kTimer1:
		_app->timerStop(kTimer1);
		_app->timerStart(kTimer1, rnd(15000) + 25000);
		_app->soundPlay(91117 + rnd(3));
		break;
	}
}

void Zone13Faust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone13Faust::onButtonUp] Not implemented");
}

void Zone13Faust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 91001:
		if (frame == 15) {
			_app->objectPresentationHide(kObject91002);
			_app->puzzleSetActive(kPuzzle91001);
		}
		break;

	case 91002:
		if (frame == 1) {
			_app->soundPlay(91103);
			_app->objectSetAccessibilityOn(kObject91002, 0, 0);
		}
		break;

	case 91003:
		if (frame == 10) {
			_app->rotationSetAlp(91007, 320.0f);
			_app->rotationSetBet(91007, 0.0f);
			_app->rotationSetRan(91007, 87.0f);
			_app->rotationSetActive(91007);
			_app->objectPresentationHide(kObject91001, 0);
			_app->objectSetAccessibilityOn(kObject91001, 0, 0);
			_app->soundPlay(91121);
		}
		break;
	}
}

void Zone13Faust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	error("[Zone13Faust::onBag] Not implemented");
}

void Zone13Faust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	error("[EventRideFaust::onAfterRideZone13] Not implemented");
}

} // End of namespace Ring
